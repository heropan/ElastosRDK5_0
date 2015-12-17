
#include "elastos/droid/internal/policy/impl/SystemGesturesPointerEventListener.h"

using Elastos::Droid::View::EIID_IPointerEventListener;

namespace Elastos {
namespace Droid {
namespace Internal {
namespace Policy {
namespace Impl {

//=====================================================================
//                  SystemGesturesPointerEventListener
//=====================================================================
CAR_INTERFACE_IMPL(SystemGesturesPointerEventListener, Object, IPointerEventListener)

const String SystemGesturesPointerEventListener::TAG("SystemGestures");
const Boolean SystemGesturesPointerEventListener::DEBUG = false;
const Int64 SystemGesturesPointerEventListener::SWIPE_TIMEOUT_MS;
const Int32 SystemGesturesPointerEventListener::MAX_TRACKED_POINTERS;
const Int32 SystemGesturesPointerEventListener::UNTRACKED_POINTER;
const Int32 SystemGesturesPointerEventListener::SWIPE_NONE;
const Int32 SystemGesturesPointerEventListener::SWIPE_FROM_TOP;
const Int32 SystemGesturesPointerEventListener::SWIPE_FROM_BOTTOM;
const Int32 SystemGesturesPointerEventListener::SWIPE_FROM_RIGHT;

SystemGesturesPointerEventListener::SystemGesturesPointerEventListener()
{
}

CARAPI SystemGesturesPointerEventListener::constructor(
    /* [in] */ IContext* context,
    /* [in] */ ISystemGesturesPointerEventListenerCallbacks* callbacks)
{
    // ==================before translated======================
    // mCallbacks = checkNull("callbacks", callbacks);
    // mSwipeStartThreshold = checkNull("context", context).getResources()
    //         .getDimensionPixelSize(com.android.internal.R.dimen.status_bar_height);
    // mSwipeDistanceThreshold = mSwipeStartThreshold;
    // if (DEBUG) Slog.d(TAG,  "mSwipeStartThreshold=" + mSwipeStartThreshold
    //         + " mSwipeDistanceThreshold=" + mSwipeDistanceThreshold);
    return NOERROR;
}

ECode SystemGesturesPointerEventListener::OnPointerEvent(
    /* [in] */ IMotionEvent* event)
{
    VALIDATE_NOT_NULL(event);
    // ==================before translated======================
    // switch (event.getActionMasked()) {
    //     case MotionEvent.ACTION_DOWN:
    //         mSwipeFireable = true;
    //         mDebugFireable = true;
    //         mDownPointers = 0;
    //         captureDown(event, 0);
    //         break;
    //     case MotionEvent.ACTION_POINTER_DOWN:
    //         captureDown(event, event.getActionIndex());
    //         if (mDebugFireable) {
    //             mDebugFireable = event.getPointerCount() < 5;
    //             if (!mDebugFireable) {
    //                 if (DEBUG) Slog.d(TAG, "Firing debug");
    //                 mCallbacks.onDebug();
    //             }
    //         }
    //         break;
    //     case MotionEvent.ACTION_MOVE:
    //         if (mSwipeFireable) {
    //             final int swipe = detectSwipe(event);
    //             mSwipeFireable = swipe == SWIPE_NONE;
    //             if (swipe == SWIPE_FROM_TOP) {
    //                 if (DEBUG) Slog.d(TAG, "Firing onSwipeFromTop");
    //                 mCallbacks.onSwipeFromTop();
    //             } else if (swipe == SWIPE_FROM_BOTTOM) {
    //                 if (DEBUG) Slog.d(TAG, "Firing onSwipeFromBottom");
    //                 mCallbacks.onSwipeFromBottom();
    //             } else if (swipe == SWIPE_FROM_RIGHT) {
    //                 if (DEBUG) Slog.d(TAG, "Firing onSwipeFromRight");
    //                 mCallbacks.onSwipeFromRight();
    //             }
    //         }
    //         break;
    //     case MotionEvent.ACTION_UP:
    //     case MotionEvent.ACTION_CANCEL:
    //         mSwipeFireable = false;
    //         mDebugFireable = false;
    //         break;
    //     default:
    //         if (DEBUG) Slog.d(TAG, "Ignoring " + event);
    // }
    assert(0);
    return NOERROR;
}

AutoPtr<ArrayOf<Int32> > SystemGesturesPointerEventListener::MiddleInitMdownpointerid()
{
    // ==================before translated======================
    // int[] result = new int[MAX_TRACKED_POINTERS];
    assert(0);
    AutoPtr<ArrayOf<Int32> > empty;
    return empty;
}

AutoPtr<ArrayOf<Float> > SystemGesturesPointerEventListener::MiddleInitMdownx()
{
    // ==================before translated======================
    // float[] result = new float[MAX_TRACKED_POINTERS];
    assert(0);
    AutoPtr< ArrayOf<Float> > empty;
    return empty;
}

AutoPtr<ArrayOf<Float> > SystemGesturesPointerEventListener::MiddleInitMdowny()
{
    // ==================before translated======================
    // float[] result = new float[MAX_TRACKED_POINTERS];
    assert(0);
    AutoPtr< ArrayOf<Float> > empty;
    return empty;
}

AutoPtr<ArrayOf<Int64> > SystemGesturesPointerEventListener::MiddleInitMdowntime()
{
    // ==================before translated======================
    // long[] result = new long[MAX_TRACKED_POINTERS];
    assert(0);
    AutoPtr< ArrayOf<Int64> > empty;
    return empty;
}

void SystemGesturesPointerEventListener::CaptureDown(
    /* [in] */ IMotionEvent* event,
    /* [in] */ Int32 pointerIndex)
{
    // ==================before translated======================
    // final int pointerId = event.getPointerId(pointerIndex);
    // final int i = findIndex(pointerId);
    // if (DEBUG) Slog.d(TAG, "pointer " + pointerId +
    //         " down pointerIndex=" + pointerIndex + " trackingIndex=" + i);
    // if (i != UNTRACKED_POINTER) {
    //     mDownX[i] = event.getX(pointerIndex);
    //     mDownY[i] = event.getY(pointerIndex);
    //     mDownTime[i] = event.getEventTime();
    //     if (DEBUG) Slog.d(TAG, "pointer " + pointerId +
    //             " down x=" + mDownX[i] + " y=" + mDownY[i]);
    // }
    assert(0);
}

Int32 SystemGesturesPointerEventListener::FindIndex(
    /* [in] */ Int32 pointerId)
{
    // ==================before translated======================
    // for (int i = 0; i < mDownPointers; i++) {
    //     if (mDownPointerId[i] == pointerId) {
    //         return i;
    //     }
    // }
    // if (mDownPointers == MAX_TRACKED_POINTERS || pointerId == MotionEvent.INVALID_POINTER_ID) {
    //     return UNTRACKED_POINTER;
    // }
    // mDownPointerId[mDownPointers++] = pointerId;
    // return mDownPointers - 1;
    assert(0);
    return 0;
}

Int32 SystemGesturesPointerEventListener::DetectSwipe(
    /* [in] */ IMotionEvent* move)
{
    // ==================before translated======================
    // final int historySize = move.getHistorySize();
    // final int pointerCount = move.getPointerCount();
    // for (int p = 0; p < pointerCount; p++) {
    //     final int pointerId = move.getPointerId(p);
    //     final int i = findIndex(pointerId);
    //     if (i != UNTRACKED_POINTER) {
    //         for (int h = 0; h < historySize; h++) {
    //             final long time = move.getHistoricalEventTime(h);
    //             final float x = move.getHistoricalX(p, h);
    //             final float y = move.getHistoricalY(p,  h);
    //             final int swipe = detectSwipe(i, time, x, y);
    //             if (swipe != SWIPE_NONE) {
    //                 return swipe;
    //             }
    //         }
    //         final int swipe = detectSwipe(i, move.getEventTime(), move.getX(p), move.getY(p));
    //         if (swipe != SWIPE_NONE) {
    //             return swipe;
    //         }
    //     }
    // }
    // return SWIPE_NONE;
    assert(0);
    return 0;
}

Int32 SystemGesturesPointerEventListener::DetectSwipe(
    /* [in] */ Int32 i,
    /* [in] */ Int64 time,
    /* [in] */ Float x,
    /* [in] */ Float y)
{
    // ==================before translated======================
    // final float fromX = mDownX[i];
    // final float fromY = mDownY[i];
    // final long elapsed = time - mDownTime[i];
    // if (DEBUG) Slog.d(TAG, "pointer " + mDownPointerId[i]
    //         + " moved (" + fromX + "->" + x + "," + fromY + "->" + y + ") in " + elapsed);
    // if (fromY <= mSwipeStartThreshold
    //         && y > fromY + mSwipeDistanceThreshold
    //         && elapsed < SWIPE_TIMEOUT_MS) {
    //     return SWIPE_FROM_TOP;
    // }
    // if (fromY >= screenHeight - mSwipeStartThreshold
    //         && y < fromY - mSwipeDistanceThreshold
    //         && elapsed < SWIPE_TIMEOUT_MS) {
    //     return SWIPE_FROM_BOTTOM;
    // }
    // if (fromX >= screenWidth - mSwipeStartThreshold
    //         && x < fromX - mSwipeDistanceThreshold
    //         && elapsed < SWIPE_TIMEOUT_MS) {
    //     return SWIPE_FROM_RIGHT;
    // }
    // return SWIPE_NONE;
    assert(0);
    return 0;
}

} // namespace Impl
} // namespace Policy
} // namespace Internal
} // namespace Droid
} // namespace Elastos


