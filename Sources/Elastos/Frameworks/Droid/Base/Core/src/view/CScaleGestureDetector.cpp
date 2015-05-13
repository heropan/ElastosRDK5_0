#include "view/CScaleGestureDetector.h"
#include "ext/frameworkext.h"
#include <elastos/Math.h>
#include "R.h"
#include "os/SystemClock.h"
#include "view/CViewConfigurationHelper.h"

using Elastos::Core::Math;
using Elastos::Droid::R;
using Elastos::Droid::Os::SystemClock;

namespace Elastos {
namespace Droid {
namespace View {

const String CScaleGestureDetector::TAG = String("CScaleGestureDetector");
const Int64 CScaleGestureDetector::TOUCH_STABILIZE_TIME = 128; // ms
const Int32 CScaleGestureDetector::TOUCH_MIN_MAJOR = 48; // dp

CScaleGestureDetector::CScaleGestureDetector()
    : mFocusX(0)
    , mFocusY(0)
    , mCurrSpan(0)
    , mPrevSpan(0)
    , mInitialSpan(0)
    , mCurrSpanX(0)
    , mCurrSpanY(0)
    , mPrevSpanX(0)
    , mPrevSpanY(0)
    , mCurrTime(0)
    , mPrevTime(0)
    , mInProgress(FALSE)
    , mSpanSlop(0)
    , mMinSpan(0)
    , mTouchUpper(Elastos::Core::Math::FLOAT_NAN)
    , mTouchLower(Elastos::Core::Math::FLOAT_NAN)
    , mTouchHistoryLastAccepted(Elastos::Core::Math::FLOAT_NAN)
    , mTouchHistoryDirection(0)
    , mTouchHistoryLastAcceptedTime(0)
    , mTouchMinMajor(0)
{
}

ECode CScaleGestureDetector::constructor(
    /* [in] */ IContext* context,
    /* [in] */ IOnScaleGestureListener* listener)
{
    assert(context != NULL);

    mInputEventConsistencyVerifier =
            InputEventConsistencyVerifier::IsInstrumentationEnabled() ?
                    new InputEventConsistencyVerifier(THIS_PROBE(IInterface), 0) : NULL;

    mContext = context;
    mListener = listener;

    AutoPtr<IViewConfigurationHelper> helper;
    CViewConfigurationHelper::AcquireSingleton((IViewConfigurationHelper**)&helper);
    AutoPtr<IViewConfiguration> vc;
    helper->Get(context, (IViewConfiguration**)&vc);
    vc->GetScaledTouchSlop(&mSpanSlop);
    mSpanSlop *= 2;

    AutoPtr<IResources> res;
    context->GetResources((IResources**)&res);
    res->GetDimensionPixelSize(R::dimen::config_minScalingTouchMajor, &mTouchMinMajor);
    res->GetDimensionPixelSize(R::dimen::config_minScalingSpan, &mMinSpan);
    return NOERROR;
}

void CScaleGestureDetector::AddTouchHistory(
    /* [in] */ IMotionEvent* ev)
{
    using Elastos::Core::Math;

    Int64 currentTime = SystemClock::GetUptimeMillis();
    Int32 count;
    ev->GetPointerCount(&count);
    Boolean accept = currentTime - mTouchHistoryLastAcceptedTime >= TOUCH_STABILIZE_TIME;
    Float total = 0;
    Int32 sampleCount = 0;
    Boolean hasLastAccepted;
    Int32 historySize, pointerSampleCount;
    Float major;
    Int64 time;

    for (Int32 i = 0; i < count; i++) {
        hasLastAccepted = !Math::IsNaN(mTouchHistoryLastAccepted);
        ev->GetHistorySize(&historySize);
        pointerSampleCount = historySize + 1;
        for (Int32 h = 0; h < pointerSampleCount; h++) {
            if (h < historySize) {
                ev->GetHistoricalSizeEx(i, h, &major);
            } else {
                ev->GetTouchMajorEx(i, &major);
            }
            if (major < mTouchMinMajor) major = mTouchMinMajor;
            total += major;

            if (Math::IsNaN(mTouchUpper) || major > mTouchUpper) {
                mTouchUpper = major;
            }
            if (Math::IsNaN(mTouchLower) || major < mTouchLower) {
                mTouchLower = major;
            }

            if (hasLastAccepted) {
                Int32 directionSig = (Int32) Math::Signum(major - mTouchHistoryLastAccepted);
                if (directionSig != mTouchHistoryDirection ||
                        (directionSig == 0 && mTouchHistoryDirection == 0)) {
                    mTouchHistoryDirection = directionSig;
                    if (h < historySize) {
                        ev->GetHistoricalEventTime(h, &time);
                    }
                    else {
                        ev->GetEventTime(&time);
                    }
                    mTouchHistoryLastAcceptedTime = time;
                    accept = FALSE;
                }
            }
        }
        sampleCount += pointerSampleCount;
    }

    Float avg = total / sampleCount;

    if (accept) {
        Float newAccepted = (mTouchUpper + mTouchLower + avg) / 3;
        mTouchUpper = (mTouchUpper + newAccepted) / 2;
        mTouchLower = (mTouchLower + newAccepted) / 2;
        mTouchHistoryLastAccepted = newAccepted;
        mTouchHistoryDirection = 0;
        ev->GetEventTime(&mTouchHistoryLastAcceptedTime);
    }
}

void CScaleGestureDetector::ClearTouchHistory()
{
    mTouchUpper = Elastos::Core::Math::FLOAT_NAN;
    mTouchLower = Elastos::Core::Math::FLOAT_NAN;
    mTouchHistoryLastAccepted = Elastos::Core::Math::FLOAT_NAN;
    mTouchHistoryDirection = 0;
    mTouchHistoryLastAcceptedTime = 0;
}

ECode CScaleGestureDetector::OnTouchEvent(
    /* [in] */ IMotionEvent* event,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);

    using Elastos::Core::Math;
    if (mInputEventConsistencyVerifier != NULL) {
        mInputEventConsistencyVerifier->OnTouchEvent(event, 0);
    }

    Int32 action;
    event->GetActionMasked(&action);

    Boolean streamComplete = action == IMotionEvent::ACTION_UP ||
            action == IMotionEvent::ACTION_CANCEL;
    if (action == IMotionEvent::ACTION_DOWN || streamComplete) {
        // Reset any scale in progress with the listener.
        // If it's an ACTION_DOWN we're beginning a new event stream.
        // This means the app probably didn't give us all the events. Shame on it.
        if (mInProgress) {
            mListener->OnScaleEnd(THIS_PROBE(IScaleGestureDetector));
            mInProgress = FALSE;
            mInitialSpan = 0;
        }

        if (streamComplete) {
            ClearTouchHistory();
            *result = TRUE;
            return NOERROR;
        }
    }

    Boolean configChanged = action == IMotionEvent::ACTION_DOWN ||
            action == IMotionEvent::ACTION_POINTER_UP ||
            action == IMotionEvent::ACTION_POINTER_DOWN;
    Boolean pointerUp = action == IMotionEvent::ACTION_POINTER_UP;
    Int32 skipIndex = -1;
    if (pointerUp)
        event->GetActionIndex(&skipIndex);

    // Determine focal point
    Float sumX = 0, sumY = 0;
    Float x, y;
    Int32 count;
    event->GetPointerCount(&count);
    for (Int32 i = 0; i < count; i++) {
        if (skipIndex == i) continue;
        event->GetXEx(i, &x);
        event->GetYEx(i, &y);
        sumX += x;
        sumY += y;
    }
    Int32 div = pointerUp ? count - 1 : count;
    Float focusX = sumX / div;
    Float focusY = sumY / div;

    AddTouchHistory(event);

    // Determine average deviation from focal point
    Float devSumX = 0, devSumY = 0;
    for (Int32 i = 0; i < count; i++) {
        if (skipIndex == i) continue;

        // Convert the resulting diameter into a radius.
        Float touchSize = mTouchHistoryLastAccepted / 2;
        event->GetXEx(i, &x);
        event->GetYEx(i, &y);
        devSumX += Math::Abs(x - focusX) + touchSize;
        devSumY += Math::Abs(y - focusY) + touchSize;
    }
    Float devX = devSumX / div;
    Float devY = devSumY / div;

    // Span is the average distance between touch points through the focal point;
    // i.e. the diameter of the circle with a radius of the average deviation from
    // the focal point.
    Float spanX = devX * 2;
    Float spanY = devY * 2;
    Float span = Math::Sqrt(spanX * spanX + spanY * spanY);

    // Dispatch begin/end events as needed.
    // If the configuration changes, notify the app to reset its current state by beginning
    // a fresh scale event stream.
    Boolean wasInProgress = mInProgress;
    mFocusX = focusX;
    mFocusY = focusY;
    if (mInProgress && (span < mMinSpan || configChanged)) {
        mListener->OnScaleEnd(THIS_PROBE(IScaleGestureDetector));
        mInProgress = FALSE;
        mInitialSpan = span;
    }
    if (configChanged) {
        mPrevSpanX = mCurrSpanX = spanX;
        mPrevSpanY = mCurrSpanY = spanY;
        mInitialSpan = mPrevSpan = mCurrSpan = span;
    }
    if (!mInProgress && span >= mMinSpan &&
            (wasInProgress || Math::Abs(span - mInitialSpan) > mSpanSlop)) {
        mPrevSpanX = mCurrSpanX = spanX;
        mPrevSpanY = mCurrSpanY = spanY;
        mPrevSpan = mCurrSpan = span;
        Boolean result;
        mInProgress = mListener->OnScaleBegin(THIS_PROBE(IScaleGestureDetector), &result);
    }

    // Handle motion; focal point and span/scale factor are changing.
    if (action == IMotionEvent::ACTION_MOVE) {
        mCurrSpanX = spanX;
        mCurrSpanY = spanY;
        mCurrSpan = span;

        Boolean updatePrev = TRUE;
        if (mInProgress) {
            Boolean result;
            updatePrev = mListener->OnScale(THIS_PROBE(IScaleGestureDetector), &result);
        }

        if (updatePrev) {
            mPrevSpanX = mCurrSpanX;
            mPrevSpanY = mCurrSpanY;
            mPrevSpan = mCurrSpan;
        }
    }

    *result = TRUE;
    return NOERROR;
}

ECode CScaleGestureDetector::IsInProgress(
    /* [out] */ Boolean* result)
{
    return mInProgress;
}

ECode CScaleGestureDetector::GetFocusX(
    /* [out] */ Float* x)
{
    return mFocusX;
}

ECode CScaleGestureDetector::GetFocusY(
    /* [out] */ Float* y)
{
    return mFocusY;
}

ECode CScaleGestureDetector::GetCurrentSpan(
    /* [out] */ Float* span)
{
    return mCurrSpan;
}

ECode CScaleGestureDetector::GetCurrentSpanX(
    /* [out] */ Float* spanX)
{
    return mCurrSpanX;
}

ECode CScaleGestureDetector::GetCurrentSpanY(
    /* [out] */ Float* spanY)
{
    return mCurrSpanY;
}

ECode CScaleGestureDetector::GetPreviousSpan(
    /* [out] */ Float* span)
{
    return mPrevSpan;
}

ECode CScaleGestureDetector::GetPreviousSpanX(
    /* [out] */ Float* spanX)
{
    return mPrevSpanX;
}

ECode CScaleGestureDetector::GetPreviousSpanY(
    /* [out] */ Float* spanY)
{
    return mPrevSpanY;
}

ECode CScaleGestureDetector::GetScaleFactor(
    /* [out] */ Float* factor)
{
    return mPrevSpan > 0 ? mCurrSpan / mPrevSpan : 1;
}

ECode CScaleGestureDetector::GetTimeDelta(
    /* [in] */ Int64 delta)
{
    return mCurrTime - mPrevTime;
}

ECode CScaleGestureDetector::GetEventTime(
    /* [in] */ Int64 delta)
{
    return mCurrTime;
}

} // namespace View
} // namespace Droid
} // namespace Elastos
