#ifndef __ELASTOS_DROID_VIEW_CSCALEGESTUREDETECTOR_H__
#define __ELASTOS_DROID_VIEW_CSCALEGESTUREDETECTOR_H__

#include "_Elastos_Droid_View_CScaleGestureDetector.h"
#include "Elastos.Droid.Core_server.h"
#include "view/InputEventConsistencyVerifier.h"

using Elastos::Droid::Content::IContext;
using Elastos::Droid::Content::Res::IResources;

namespace Elastos {
namespace Droid {
namespace View {

/**
 * Detects scaling transformation gestures using the supplied {@link MotionEvent}s.
 * The {@link OnScaleGestureListener} callback will notify users when a particular
 * gesture event has occurred.
 *
 * This class should only be used with {@link MotionEvent}s reported via touch.
 *
 * To use this class:
 * <ul>
 *  <li>Create an instance of the {@code ScaleGestureDetector} for your
 *      {@link View}
 *  <li>In the {@link View#onTouchEvent(MotionEvent)} method ensure you call
 *          {@link #onTouchEvent(MotionEvent)}. The methods defined in your
 *          callback will be executed when the events occur.
 * </ul>
 */
CarClass(CScaleGestureDetector)
{
public:
    CScaleGestureDetector();

    CARAPI constructor(
        /* [in] */ IContext* context,
        /* [in] */ IOnScaleGestureListener* listener);

    /**
     * Accepts MotionEvents and dispatches events to a {@link OnScaleGestureListener}
     * when appropriate.
     *
     * <p>Applications should pass a complete and consistent event stream to this method.
     * A complete and consistent event stream involves all MotionEvents from the initial
     * ACTION_DOWN to the final ACTION_UP or ACTION_CANCEL.</p>
     *
     * @param event The event to process
     * @return true if the event was processed and the detector wants to receive the
     *         rest of the MotionEvents in this event stream.
     */
    CARAPI OnTouchEvent(
        /* [in] */ IMotionEvent* event,
        /* [out] */ Boolean* result);

    /**
     * Returns {@code true} if a scale gesture is in progress.
     */
    CARAPI IsInProgress(
        /* [out] */ Boolean* result);

    /**
     * Get the X coordinate of the current gesture's focal point.
     * If a gesture is in progress, the focal point is between
     * each of the pointers forming the gesture.
     *
     * If {@link #isInProgress()} would return false, the result of this
     * function is undefined.
     *
     * @return X coordinate of the focal point in pixels.
     */
    CARAPI GetFocusX(
        /* [out] */ Float* x);

    /**
     * Get the Y coordinate of the current gesture's focal point.
     * If a gesture is in progress, the focal point is between
     * each of the pointers forming the gesture.
     *
     * If {@link #isInProgress()} would return false, the result of this
     * function is undefined.
     *
     * @return Y coordinate of the focal point in pixels.
     */
    CARAPI GetFocusY(
        /* [out] */ Float* y);

    /**
     * Return the average distance between each of the pointers forming the
     * gesture in progress through the focal point.
     *
     * @return Distance between pointers in pixels.
     */
    CARAPI GetCurrentSpan(
        /* [out] */ Float* span);

    /**
     * Return the average X distance between each of the pointers forming the
     * gesture in progress through the focal point.
     *
     * @return Distance between pointers in pixels.
     */
    CARAPI GetCurrentSpanX(
        /* [out] */ Float* spanX);

    /**
     * Return the average Y distance between each of the pointers forming the
     * gesture in progress through the focal point.
     *
     * @return Distance between pointers in pixels.
     */
    CARAPI GetCurrentSpanY(
        /* [out] */ Float* spanY);

    /**
     * Return the previous average distance between each of the pointers forming the
     * gesture in progress through the focal point.
     *
     * @return Previous distance between pointers in pixels.
     */
    CARAPI GetPreviousSpan(
        /* [out] */ Float* span);

    /**
     * Return the previous average X distance between each of the pointers forming the
     * gesture in progress through the focal point.
     *
     * @return Previous distance between pointers in pixels.
     */
    CARAPI GetPreviousSpanX(
        /* [out] */ Float* spanX);

    /**
     * Return the previous average Y distance between each of the pointers forming the
     * gesture in progress through the focal point.
     *
     * @return Previous distance between pointers in pixels.
     */
    CARAPI GetPreviousSpanY(
        /* [out] */ Float* spanY);

    /**
     * Return the scaling factor from the previous scale event to the current
     * event. This value is defined as
     * ({@link #getCurrentSpan()} / {@link #getPreviousSpan()}).
     *
     * @return The current scaling factor.
     */
    CARAPI GetScaleFactor(
        /* [out] */ Float* factor);

    /**
     * Return the time difference in milliseconds between the previous
     * accepted scaling event and the current scaling event.
     *
     * @return Time difference since the last scaling event in milliseconds.
     */
    CARAPI GetTimeDelta(
        /* [in] */ Int64 delta);

    /**
     * Return the event time of the current event being processed.
     *
     * @return Current event time in milliseconds.
     */
    CARAPI GetEventTime(
        /* [in] */ Int64 delta);

private:
    /**
     * The touchMajor/touchMinor elements of a MotionEvent can flutter/jitter on
     * some hardware/driver combos. Smooth it out to get kinder, gentler behavior.
     * @param ev MotionEvent to add to the ongoing history
     */
    void AddTouchHistory(
        /* [in] */ IMotionEvent* ev);

    /**
     * Clear all touch history tracking. Useful in ACTION_CANCEL or ACTION_UP.
     * @see #addTouchHistory(MotionEvent)
     */
    void ClearTouchHistory();

private:
    static const String TAG;

    static const Int64 TOUCH_STABILIZE_TIME;// = 128; // ms
    static const Int32 TOUCH_MIN_MAJOR;// = 48; // dp

    AutoPtr<IContext> mContext;
    AutoPtr<IOnScaleGestureListener> mListener;

    Float mFocusX;
    Float mFocusY;

    Float mCurrSpan;
    Float mPrevSpan;
    Float mInitialSpan;
    Float mCurrSpanX;
    Float mCurrSpanY;
    Float mPrevSpanX;
    Float mPrevSpanY;
    Int64 mCurrTime;
    Int64 mPrevTime;
    Boolean mInProgress;
    Int32 mSpanSlop;
    Int32 mMinSpan;

    // Bounds for recently seen values
    Float mTouchUpper;
    Float mTouchLower;
    Float mTouchHistoryLastAccepted;
    Int32 mTouchHistoryDirection;
    Int64 mTouchHistoryLastAcceptedTime;
    Int32 mTouchMinMajor;

    /**
     * Consistency verifier for debugging purposes.
     */
    AutoPtr<InputEventConsistencyVerifier> mInputEventConsistencyVerifier;
};

} // namespace View
} // namespace Droid
} // namespace Elastos

#endif //__ELASTOS_DROID_VIEW_CSCALEGESTUREDETECTOR_H__
