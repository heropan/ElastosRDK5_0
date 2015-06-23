#include "systemui/CExpandHelper.h"
#include <elastos/core/Math.h>
#include <elastos/utility/logging/Slogger.h>
#include "systemui/SystemUIR.h"
#include "view/CScaleGestureDetector.h"
#include "view/CViewConfigurationHelper.h"
#include "view/CMotionEventHelper.h"
#include "animation/CAnimatorSet.h"
#include "animation/CObjectAnimator.h"

using Elastos::Utility::Logging::Slogger;
using Elastos::Core::Math;
using Elastos::Droid::Content::Res::IResources;
using Elastos::Droid::View::IView;
using Elastos::Droid::View::IViewGroup;
using Elastos::Droid::View::IViewParent;
using Elastos::Droid::View::IGravity;
using Elastos::Droid::View::IViewConfiguration;
using Elastos::Droid::View::IViewConfigurationHelper;
using Elastos::Droid::View::CViewConfigurationHelper;
using Elastos::Droid::View::IMotionEventHelper;
using Elastos::Droid::View::CMotionEventHelper;
using Elastos::Droid::Animation::CAnimatorSet;
using Elastos::Droid::Animation::IObjectAnimator;
using Elastos::Droid::Animation::CObjectAnimator;
using Elastos::Droid::Animation::IAnimatorSetBuilder;
using Elastos::Droid::SystemUI::SystemUIR;
using Elastos::Droid::View::CScaleGestureDetector;

namespace Elastos {
namespace Droid {
namespace SystemUI {

const Boolean CExpandHelper::DEBUG = FALSE;
const Boolean CExpandHelper::DEBUG_SCALE = FALSE;
const Boolean CExpandHelper::DEBUG_GLOW = FALSE;

const String CExpandHelper::TAG("CExpandHelper");
const Int64 CExpandHelper::EXPAND_DURATION = 250;
const Int64 CExpandHelper::GLOW_DURATION = 150;

const Boolean CExpandHelper::USE_DRAG = TRUE;
const Boolean CExpandHelper::USE_SPAN = TRUE;

const Float CExpandHelper::STRETCH_INTERVAL = 2.0f;
const Float CExpandHelper::GLOW_BASE = 0.5f;

const Int32 CExpandHelper::NONE    = 0;
const Int32 CExpandHelper::BLINDS  = 1<<0;
const Int32 CExpandHelper::PULL    = 1<<1;
const Int32 CExpandHelper::STRETCH = 1<<2;

//============================================================================
//              CExpandHelper::ExpandScaleGestureListener
//============================================================================
CExpandHelper::ExpandScaleGestureListener::ExpandScaleGestureListener(
    /* [in] */ CExpandHelper* host)
    : mHost(host)
{
}

ECode CExpandHelper::ExpandScaleGestureListener::OnScale(
    /* [in ]*/ IScaleGestureDetector* detector,
    /* [out] */ Boolean* res)
{
    VALIDATE_NOT_NULL(res);
    if (CExpandHelper::DEBUG_SCALE) Slogger::W(CExpandHelper::TAG, "onscalebegin()");
    Float focusX;
    detector->GetFocusX(&focusX);
    Float focusY;
    detector->GetFocusY(&focusY);

    AutoPtr<IView> underFocus = mHost->FindView(focusX, focusY);
    if (underFocus != NULL) {
        mHost->StartExpanding(underFocus, STRETCH);
    }
    *res = mHost->mExpanding;
    return NOERROR;
}

ECode CExpandHelper::ExpandScaleGestureListener::OnScaleBegin(
    /* [in ]*/ IScaleGestureDetector* detector,
    /* [out] */ Boolean* res)
{
    VALIDATE_NOT_NULL(res);
    *res = TRUE;
    return NOERROR;
}

ECode CExpandHelper::ExpandScaleGestureListener::OnScaleEnd(
    /* [in ]*/ IScaleGestureDetector* detector)
{
    return NOERROR;
}

//============================================================================
//              AnimatorListener
//============================================================================

ECode CExpandHelper::AnimatorListener::OnAnimationStart(
    /* [in] */ IAnimator* animation)
{
    AutoPtr<IObjectAnimator> objAnim = IObjectAnimator::Probe(animation);
    AutoPtr<IInterface> target;
    objAnim->GetTarget((IInterface**)&target);
    AutoPtr<IView> view = IView::Probe(target);
    Float alpha;
    view->GetAlpha(&alpha);
    if(alpha <= 0.f) {
        view->SetVisibility(IView::VISIBLE);
    }
    return NOERROR;
}

ECode CExpandHelper::AnimatorListener::OnAnimationEnd(
    /* [in] */ IAnimator* animation)
{
    AutoPtr<IObjectAnimator> objAnim = IObjectAnimator::Probe(animation);
    AutoPtr<IInterface> target;
    objAnim->GetTarget((IInterface**)&target);
    AutoPtr<IView> view = IView::Probe(target);
    Float alpha;
    view->GetAlpha(&alpha);
    if (alpha <= 0.f) {
        view->SetVisibility(IView::INVISIBLE);
    }
    return NOERROR;
}

//============================================================================
//              CExpandHelper
//============================================================================

CExpandHelper::CExpandHelper()
    : mExpanding(FALSE)
    , mExpansionStyle(NONE)
    , mWatchingForPull(FALSE)
    , mHasPopped(FALSE)
    , mOldHeight(0)
    , mNaturalHeight(0)
    , mInitialTouchFocusY(0)
    , mInitialTouchY(0)
    , mInitialTouchSpan(0)
    , mLastFocusY(0)
    , mLastSpanY(0)
    , mTouchSlop(0)
    , mLastMotionY(0)
    , mPopLimit(0)
    , mPopDuration(0)
    , mPullGestureMinXSpan(0)
    , mSmallSize(0)
    , mLargeSize(0)
    , mMaximumStretch(0)
    , mGravity(0)
{
    mScaleGestureListener = new ExpandScaleGestureListener(this);
}

ECode CExpandHelper::constructor(
    /* [in] */ IContext* context,
    /* [in] */ IExpandHelperCallback* callback,
    /* [in] */ Int32 small,
    /* [in] */ Int32 large)
{
    mSmallSize = small;
    mMaximumStretch = mSmallSize * STRETCH_INTERVAL;
    mLargeSize = large;
    mContext = context;
    mCallback = callback;
    CViewScaller::New((IViewScaller**)&mScaler);
    mGravity = IGravity::TOP;
    AutoPtr<ArrayOf<Float> > param = ArrayOf<Float>::Alloc(1);
    (*param)[0] = 0.0f;
    mScaleAnimation = CObjectAnimator::OfFloat(mScaler, String("height"), param);
    mScaleAnimation->SetDuration(EXPAND_DURATION);

    AutoPtr<IResources> res;
    mContext->GetResources((IResources**)&res);
    res->GetDimension(SystemUIR::dimen::blinds_pop_threshold, &mPopLimit);
    res->GetInteger(SystemUIR::integer::blinds_pop_duration_ms, &mPopDuration);
    res->GetDimension(SystemUIR::dimen::pull_span_min, &mPullGestureMinXSpan);
    AutoPtr<AnimatorListenerAdapter> glowVisibilityController = new AnimatorListener();

    mGlowTopAnimation = CObjectAnimator::OfFloat(NULL, String("alpha"), param);
    mGlowTopAnimation->AddListener(glowVisibilityController);
    mGlowBottomAnimation = CObjectAnimator::OfFloat(NULL, String("alpha"), param);
    mGlowBottomAnimation->AddListener(glowVisibilityController);

    CAnimatorSet::New((IAnimatorSet**)&mGlowAnimationSet);
    AutoPtr<IAnimatorSetBuilder> builder;
    mGlowAnimationSet->Play(mGlowTopAnimation, (IAnimatorSetBuilder**)&builder);
    builder->With(mGlowBottomAnimation);
    mGlowAnimationSet->SetDuration(GLOW_DURATION);

    AutoPtr<IViewConfiguration> configuration;
    AutoPtr<IViewConfigurationHelper> helper;
    CViewConfigurationHelper::AcquireSingleton((IViewConfigurationHelper**)&helper);
    helper->Get(mContext, (IViewConfiguration**)&configuration);
    configuration->GetScaledTouchSlop(&mTouchSlop);

    CScaleGestureDetector::New(mContext, mScaleGestureListener, (IScaleGestureDetector**)&mSGD);
    return NOERROR;
}

void CExpandHelper::UpdateExpansion()
{
    using Elastos::Core::Math;

    if (DEBUG_SCALE) Slogger::W(TAG, "updateExpansion()");
    // are we scaling or dragging?
    Float fy;
    mSGD->GetFocusY(&fy);
    Float curspan;
    mSGD->GetCurrentSpan(&curspan);
    Float span = curspan - mInitialTouchSpan;
    span *= USE_SPAN ? 1.0f : 0.0f;
    Float drag = fy - mInitialTouchFocusY;
    drag *= USE_DRAG ? 1.0f : 0.0f;
    drag *= mGravity == IGravity::BOTTOM ? -1.0f : 1.0f;
    Float pull = Math::Abs(drag) + Math::Abs(span) + 1.0f;
    Float hand = drag * Math::Abs(drag) / pull + span * Math::Abs(span) / pull;
    Float target = hand + mOldHeight;
    Float newHeight = Clamp(target);
    mScaler->SetHeight(newHeight);

    SetGlow(CalculateGlow(target, newHeight));
    mSGD->GetFocusY(&mLastFocusY);
    mSGD->GetCurrentSpan(&mLastSpanY);
}

Float CExpandHelper::Clamp(
    /* [in] */ Float target)
{
    Float out = target;
    out = out < mSmallSize ? mSmallSize : (out > mLargeSize ? mLargeSize : out);
    out = out > mNaturalHeight ? mNaturalHeight : out;
    return out;
}

AutoPtr<IView> CExpandHelper::FindView(
    /* [in] */ Float x,
    /* [in] */ Float y)
{
    if (mEventSource != NULL) {
        Int32 sx, sy;
        mEventSource->GetLocationOnScreen(&sx, &sy);
        x += sx;
        y += sy;
    }

    AutoPtr<IView> v;
    mCallback->GetChildAtPosition(x, y, (IView**)&v);
    return v;
}

Boolean CExpandHelper::IsInside(
    /* [in] */ IView* v,
    /* [in] */ Float x,
    /* [in] */ Float y)
{
    if (DEBUG) Slogger::D(TAG, "isinside (%.2f, %0.2f)", x, y);

    if (v == NULL) {
        if (DEBUG) Slogger::D(TAG, "isinside NULL subject");
        return FALSE;
    }

    Int32 sx, sy;
    if (mEventSource != NULL) {
        mEventSource->GetLocationOnScreen(&sx, &sy);
        x += sx;
        y += sy;
        if (DEBUG) Slogger::D(TAG, "  to global (%.2f, %.2f)", x, y);
    }

    v->GetLocationOnScreen(&sx, &sy);
    x -= sx;
    y -= sy;
    Int32 w, h;
    v->GetWidth(&w);
    v->GetHeight(&h);
    if (DEBUG) Slogger::D(TAG, "  to local (%.2f, %.2f)", x, y);
    if (DEBUG) Slogger::D(TAG, "  inside (%d, %d)", w, h);
    Boolean inside = (x > 0.0f && y > 0.0f && x < w && y < h);
    return inside;
}

ECode CExpandHelper::SetEventSource(
    /* [in] */ IView* eventSource)
{
    mEventSource = eventSource;
    return NOERROR;
}

ECode CExpandHelper::SetGravity(
    /* [in] */ Int32 gravity)
{
    mGravity = gravity;
    return NOERROR;
}

ECode CExpandHelper::SetScrollView(
    /* [in] */ IView* scrollView)
{
    mScrollView = scrollView;
    return NOERROR;
}

Float CExpandHelper::CalculateGlow(
    /* [in] */ Float target,
    /* [in] */ Float actual)
{
    using Elastos::Core::Math;
    // glow if overscale
    if (DEBUG_GLOW) Slogger::D(TAG, "target: %.2f actual: %.2f", target, actual);
    Float stretch = Math::Abs((target - actual) / mMaximumStretch);
    Float strength = 1.0f / (1.0f + (Float) Math::Pow(Math::DOUBLE_E, -1 * ((8.0f * stretch) - 5.0f)));
    if (DEBUG_GLOW) Slogger::D(TAG, "stretch: %.2f strength: %.2f", stretch, strength);
    return (GLOW_BASE + strength * (1.0f - GLOW_BASE));
}

ECode CExpandHelper::SetGlow(
    /* [in] */ Float glow)
{
    Boolean isRunning;
    mGlowAnimationSet->IsRunning(&isRunning);
    if (!isRunning || glow == 0.0f) {
        mGlowAnimationSet->IsRunning(&isRunning);
        if (isRunning) {
            mGlowAnimationSet->End();
        }
        if (mCurrViewTopGlow != NULL && mCurrViewBottomGlow != NULL) {
            Float alpha;
            mCurrViewTopGlow->GetAlpha(&alpha);
            if (glow == 0.0f || alpha == 0.0f) {
                // animate glow in and out
                mGlowTopAnimation->SetTarget(mCurrViewTopGlow);
                mGlowBottomAnimation->SetTarget(mCurrViewBottomGlow);
                AutoPtr<ArrayOf<Float> > values = ArrayOf<Float>::Alloc(1);
                values->Set(0, glow);
                mGlowTopAnimation->SetFloatValues(values);
                mGlowBottomAnimation->SetFloatValues(values);
                mGlowAnimationSet->SetupStartValues();
                mGlowAnimationSet->Start();
            }
            else {
                // set it explicitly in reponse to touches.
                mCurrViewTopGlow->SetAlpha(glow);
                mCurrViewBottomGlow->SetAlpha(glow);
                HandleGlowVisibility();
            }
        }
    }
    return NOERROR;
}

ECode CExpandHelper::HandleGlowVisibility()
{
    Float alpha;
    mCurrViewTopGlow->GetAlpha(&alpha);
    mCurrViewTopGlow->SetVisibility(alpha <= 0.0f ? IView::INVISIBLE : IView::VISIBLE);
    mCurrViewBottomGlow->GetAlpha(&alpha);
    mCurrViewBottomGlow->SetVisibility(alpha <= 0.0f ? IView::INVISIBLE : IView::VISIBLE);
    return NOERROR;
}

ECode CExpandHelper::OnInterceptTouchEvent(
    /* [in] */ IMotionEvent* ev,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
    *result = TRUE;

    Int32 action;
    ev->GetAction(&action);

    if (DEBUG_SCALE) {
        AutoPtr<IMotionEventHelper> helper;
        CMotionEventHelper::AcquireSingleton((IMotionEventHelper**)&helper);
        String actionStr;
        helper->ActionToString(action, &actionStr);
        Slogger::D(TAG, "OnInterceptTouchEvent: act=%s, expanding=%d.", actionStr.string(), mExpanding);
        //                  (0 != (mExpansionStyle & BLINDS) ? " (blinds)" : "") +
        //                  (0 != (mExpansionStyle & PULL) ? " (pull)" : "") +
        //                  (0 != (mExpansionStyle & STRETCH) ? " (stretch)" : ""));
    }

    // check for a spread-finger vertical pull gesture
    Boolean temp;
    mSGD->OnTouchEvent(ev, &temp);
    Float focusX, focusY;
    mSGD->GetFocusX(&focusX);
    mSGD->GetFocusY(&focusY);

    Int32 x = (Int32)focusX;
    Int32 y = (Int32)focusY;

    mInitialTouchFocusY = y;
    mSGD->GetCurrentSpan(&mInitialTouchSpan);
    mLastFocusY = mInitialTouchFocusY;
    mLastSpanY = mInitialTouchSpan;
    if (DEBUG_SCALE) Slogger::D(TAG, "set initial span: %.2f", mInitialTouchSpan);

    if (mExpanding) {
        *result = TRUE;
        return NOERROR;
    } else {
        if ((action == IMotionEvent::ACTION_MOVE) && 0 != (mExpansionStyle & BLINDS)) {
            // we've begun Venetian blinds style expansion
            *result = TRUE;
            return NOERROR;
        }
        Float xspan, yspan;
        mSGD->GetCurrentSpanX(&xspan);
        mSGD->GetCurrentSpanY(&yspan);

        if (action == IMotionEvent::ACTION_MOVE &&
                xspan > mPullGestureMinXSpan &&
                xspan > yspan) {
            // detect a vertical pulling gesture with fingers somewhat separated
            if (DEBUG_SCALE) Slogger::V(TAG, "got pull gesture (xspan=%.2f px.", xspan);

            AutoPtr<IView> underFocus = FindView(x, y);
            if (underFocus != NULL) {
                StartExpanding(underFocus, PULL);
            }
            *result = TRUE;
            return NOERROR;
        }
        if (mScrollView != NULL) {
            Int32 scrollY;
            mScrollView->GetScrollY(&scrollY);
            if (scrollY > 0) {
                *result = FALSE;
                return NOERROR;
            }
        }
        // Now look for other gestures
        switch (action & IMotionEvent::ACTION_MASK) {
        case IMotionEvent::ACTION_MOVE: {
            if (mWatchingForPull) {
                Int32 yDiff = y - mLastMotionY;
                if (yDiff > mTouchSlop) {
                    if (DEBUG) Slogger::V(TAG, "got venetian gesture (dy=%.2f", yDiff);
                    mLastMotionY = y;
                    AutoPtr<IView> underFocus = FindView(x, y);
                    if (underFocus != NULL) {
                        StartExpanding(underFocus, BLINDS);
                        mInitialTouchY = mLastMotionY;
                        mHasPopped = FALSE;
                    }
                }
            }
            break;
        }

        case IMotionEvent::ACTION_DOWN:
            mWatchingForPull = IsInside(mScrollView, x, y);
            mLastMotionY = y;
            break;

        case IMotionEvent::ACTION_CANCEL:
        case IMotionEvent::ACTION_UP:
             if (DEBUG) Slogger::D(TAG, "up/cancel");
            FinishExpanding(FALSE);
            ClearView();
            break;
        }
        *result = mExpanding;
        return NOERROR;
    }
    return NOERROR;
}

ECode CExpandHelper::OnTouchEvent(
    /* [in] */ IMotionEvent* ev,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
    *result = TRUE;

    using Elastos::Core::Math;

    Int32 action;
    ev->GetActionMasked(&action);

    if (DEBUG_SCALE) {
        AutoPtr<IMotionEventHelper> helper;
        CMotionEventHelper::AcquireSingleton((IMotionEventHelper**)&helper);
        String actionStr;
        helper->ActionToString(action, &actionStr);
        Slogger::D(TAG, "OnTouchEvent: act=%s, expanding=%d.", actionStr.string(), mExpanding);
    //         (0 != (mExpansionStyle & BLINDS) ? " (blinds)" : "") +
    //         (0 != (mExpansionStyle & PULL) ? " (pull)" : "") +
    //         (0 != (mExpansionStyle & STRETCH) ? " (stretch)" : ""));
    }

    Boolean value;
    mSGD->OnTouchEvent(ev, &value);

    switch (action) {
        case IMotionEvent::ACTION_MOVE: {
            if (0 != (mExpansionStyle & BLINDS)) {
                Float ey;
                ev->GetY(&ey);
                Float rawHeight = ey - mInitialTouchY + mOldHeight;
                Float newHeight = Clamp(rawHeight);
                Boolean wasClosed = (mOldHeight == mSmallSize);
                Boolean isFinished = FALSE;
                if (rawHeight > mNaturalHeight) {
                    isFinished = TRUE;
                }
                if (rawHeight < mSmallSize) {
                    isFinished = TRUE;
                }

                Float pull = Math::Abs(ey - mInitialTouchY);
                if (mHasPopped || pull > mPopLimit) {
                    if (!mHasPopped) {
                        Vibrate(mPopDuration);
                        mHasPopped = TRUE;
                    }
                }

                if (mHasPopped) {
                    mScaler->SetHeight(newHeight);
                    SetGlow(GLOW_BASE);
                }
                else {
                    SetGlow(CalculateGlow(4.0f * pull, 0.0f));
                }

                Float fx, fy;
                mSGD->GetFocusX(&fx);
                mSGD->GetFocusY(&fy);
                Int32 x = (Int32) fx;
                Int32 y = (Int32) fy;
                AutoPtr<IView> underFocus = FindView(x, y);
                if (isFinished && underFocus != NULL && underFocus != mCurrView) {
                    FinishExpanding(FALSE); // @@@ needed?
                    StartExpanding(underFocus, BLINDS);
                    mInitialTouchY = y;
                    mHasPopped = FALSE;
                }

                return NOERROR;
            }

            if (mExpanding) {
                UpdateExpansion();
                return NOERROR;
            }

            break;
        }

        case IMotionEvent::ACTION_POINTER_UP:
        case IMotionEvent::ACTION_POINTER_DOWN: {
                 if (DEBUG) Slogger::D(TAG, "pointer change");
                Float span, fy;
                mSGD->GetCurrentSpan(&span);
                mSGD->GetFocusY(&fy);
                mInitialTouchY += fy - mLastFocusY;
                mInitialTouchSpan += span - mLastSpanY;
            }

            break;

        case IMotionEvent::ACTION_UP:
        case IMotionEvent::ACTION_CANCEL:
             if (DEBUG) Slogger::D(TAG, "up/cancel");
            FinishExpanding(FALSE);
            ClearView();
            break;
    }

    return NOERROR;
}

void CExpandHelper::StartExpanding(
    /* [in] */ IView* v,
    /* [in] */ Int32 expandType)
{
    mExpansionStyle = expandType;
    if (mExpanding &&  v == mCurrView) {
        return;
    }

    Boolean result;
    mExpanding = TRUE;
     if (DEBUG) Slogger::D(TAG, "scale type %d  beginning on view: %p", expandType, v);
    mCallback->SetUserLockedChild(v, TRUE, &result);
    SetView(v);
    SetGlow(GLOW_BASE);
    mScaler->SetView(v);
    mScaler->GetHeight(&mOldHeight);
    Boolean bval;
    mCallback->CanChildBeExpanded(v, &bval);
    if (bval) {
        if (DEBUG) Slogger::D(TAG, "working on an expandable child");
        Int32 tmp;
        mScaler->GetNaturalHeight(mLargeSize, &tmp);
        mNaturalHeight = tmp;
    }
    else {
        if (DEBUG) Slogger::D(TAG, "working on a non-expandable child");
        mNaturalHeight = mOldHeight;
    }
     if (DEBUG) Slogger::D(TAG, "got mOldHeight: %.2f, mNaturalHeight: %.2f ", mOldHeight, mNaturalHeight);
    AutoPtr<IViewParent> parent;
    v->GetParent((IViewParent**)&parent);
    assert(parent != NULL);
    parent->RequestDisallowInterceptTouchEvent(TRUE);
}

void CExpandHelper::FinishExpanding(
    /* [in] */ Boolean force)
{
    if (!mExpanding) return;

     if (DEBUG) Slogger::D(TAG, "scale in finishing on view: %p", mCurrView.Get());

    Float currentHeight;
    mScaler->GetHeight(&currentHeight);
    Float targetHeight = mSmallSize;
    Float h;
    mScaler->GetHeight(&h);
    Boolean wasClosed = (mOldHeight == mSmallSize);
    if (wasClosed) {
        targetHeight = (force || currentHeight > mSmallSize) ? mNaturalHeight : mSmallSize;
    }
    else {
        targetHeight = (force || currentHeight < mNaturalHeight) ? mSmallSize : mNaturalHeight;
    }

    Boolean bval;
    mScaleAnimation->IsRunning(&bval);
    if (bval) {
        mScaleAnimation->Cancel();
    }
    SetGlow(0.0f);
    mCallback->SetUserExpandedChild(mCurrView, h == mNaturalHeight, &bval);
    if (targetHeight != currentHeight) {
        AutoPtr<ArrayOf<Float> > values = ArrayOf<Float>::Alloc(1);
        values->Set(0, targetHeight);
        mScaleAnimation->SetFloatValues(values);
        mScaleAnimation->SetupStartValues();
        mScaleAnimation->Start();
    }
    mCallback->SetUserLockedChild(mCurrView, FALSE, &bval);

    mExpanding = FALSE;
    mExpansionStyle = NONE;

     if (DEBUG) Slogger::D(TAG, "wasClosed is: %d", wasClosed);
     if (DEBUG) Slogger::D(TAG, "currentHeight is: %.2f", currentHeight);
     if (DEBUG) Slogger::D(TAG, "mSmallSize is: %.2f", mSmallSize);
     if (DEBUG) Slogger::D(TAG, "targetHeight is: %.2f", targetHeight);
     if (DEBUG) Slogger::D(TAG, "scale was finished on view: %p", mCurrView.Get());
}

void CExpandHelper::ClearView()
{
    mCurrView = NULL;
    mCurrViewTopGlow = NULL;
    mCurrViewBottomGlow = NULL;
}

void CExpandHelper::SetView(
    /* [in] */ IView* v)
{
    mCurrView = v;
    AutoPtr<IViewGroup> g = IViewGroup::Probe(v);
    if (g) {
        mCurrViewTopGlow = NULL;
        mCurrViewBottomGlow = NULL;
        g->FindViewById(SystemUIR::id::top_glow, (IView**)&mCurrViewTopGlow);
        g->FindViewById(SystemUIR::id::bottom_glow, (IView**)&mCurrViewBottomGlow);
        if (DEBUG) {
        //     String debugLog = "Looking for glows: " +
        //             (mCurrViewTopGlow != NULL ? "found top " : "didn't find top") +
        //             (mCurrViewBottomGlow != NULL ? "found bottom " : "didn't find bottom");
        //     Slogger::V(TAG,  debugLog);
        }
    }
}

ECode CExpandHelper::OnClick(
    /* [in] */ IView* v)
{
    StartExpanding(v, STRETCH);
    FinishExpanding(TRUE);
    ClearView();
    return NOERROR;
}


ECode CExpandHelper::Cancel()
{
    FinishExpanding(TRUE);
    ClearView();

    // reset the gesture detector
    mSGD = NULL;
    CScaleGestureDetector::New(mContext, mScaleGestureListener,
        (IScaleGestureDetector**)&mSGD);
    return NOERROR;
}

/**
 * Triggers haptic feedback.
 */
void CExpandHelper::Vibrate(
    /* [in] */ Int64 duration)
{
    Mutex::Autolock lock(mVibrateLock);
    if (mVibrator == NULL) {
        AutoPtr<IInterface> tmpObj;
        mContext->GetSystemService(IContext::VIBRATOR_SERVICE, (IInterface**)&tmpObj);
        mVibrator = IVibrator::Probe(tmpObj.Get());
    }
    if (mVibrator)
        mVibrator->Vibrate(duration);
}

}// namespace SystemUI
}// namespace Droid
}// namespace Elastos
