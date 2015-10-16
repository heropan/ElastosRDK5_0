
#ifndef __ELASTOS_DROID_VIEW_CGESTUREDETECTOR_H__
#define __ELASTOS_DROID_VIEW_CGESTUREDETECTOR_H__

#include "_Elastos_Droid_View_CGestureDetector.h"
#include "elastos/droid/os/HandlerBase.h"
#include "elastos/droid/view/VelocityTracker.h"
#include "elastos/droid/view/InputEventConsistencyVerifier.h"

using Elastos::Droid::Content::IContext;
using Elastos::Droid::Os::HandlerBase;

namespace Elastos {
namespace Droid {
namespace View {

CarClass(CGestureDetector)
{
private:
    class GestureHandler : public HandlerBase
    {
    public:
        GestureHandler(
            /* [in] */ CGestureDetector* host)
            : mHost(host)
        {}

        GestureHandler(
            /* [in] */ ILooper* looper,
            /* [in] */ CGestureDetector* host)
            : HandlerBase(looper)
            , mHost(host)
        {}

        CARAPI HandleMessage(
            /* [in] */ IMessage* msg);
    private:
        CGestureDetector* mHost;
    };

public:
    CGestureDetector();

    /**
     * Creates a GestureDetector with the supplied listener.
     * This variant of the constructor should be used from a non-UI thread
     * (as it allows specifying the Handler).
     *
     * @param listener the listener invoked for all the callbacks, this must
     * not be null.
     * @param handler the handler to use
     *
     * @throws NullPointerException if either {@code listener} or
     * {@code handler} is null.
     *
     * @deprecated Use {@link #GestureDetector(android.content.Context,
     *      android.view.GestureDetector.OnGestureListener, android.os.Handler)} instead.
     */
    //@Deprecated
    CARAPI constructor(
        /* [in] */ IOnGestureListener* listener,
        /* [in] */ IHandler* handler);

    /**
     * Creates a GestureDetector with the supplied listener.
     * You may only use this constructor from a UI thread (this is the usual situation).
     * @see android.os.Handler#Handler()
     *
     * @param listener the listener invoked for all the callbacks, this must
     * not be null.
     *
     * @throws NullPointerException if {@code listener} is null.
     *
     * @deprecated Use {@link #GestureDetector(android.content.Context,
     *      android.view.GestureDetector.OnGestureListener)} instead.
     */
    //@Deprecated
    CARAPI constructor(
        /* [in] */ IOnGestureListener* listener);

    /**
     * Creates a GestureDetector with the supplied listener.
     * You may only use this constructor from a UI thread (this is the usual situation).
     * @see android.os.Handler#Handler()
     *
     * @param context the application's context
     * @param listener the listener invoked for all the callbacks, this must
     * not be null.
     *
     * @throws NullPointerException if {@code listener} is null.
     */
    CARAPI constructor(
        /* [in] */ IContext* context,
        /* [in] */ IOnGestureListener* listener);

    /**
     * Creates a GestureDetector with the supplied listener.
     * You may only use this constructor from a UI thread (this is the usual situation).
     * @see android.os.Handler#Handler()
     *
     * @param context the application's context
     * @param listener the listener invoked for all the callbacks, this must
     * not be null.
     * @param handler the handler to use
     *
     * @throws NullPointerException if {@code listener} is null.
     */
    CARAPI constructor(
        /* [in] */ IContext* context,
        /* [in] */ IOnGestureListener* listener,
        /* [in] */ IHandler* handler);

    /**
     * Creates a GestureDetector with the supplied listener.
     * You may only use this constructor from a UI thread (this is the usual situation).
     * @see android.os.Handler#Handler()
     *
     * @param context the application's context
     * @param listener the listener invoked for all the callbacks, this must
     * not be null.
     * @param handler the handler to use
     *
     * @throws NullPointerException if {@code listener} is null.
     */
    CARAPI constructor(
        /* [in] */ IContext* context,
        /* [in] */ IOnGestureListener* listener,
        /* [in] */ IHandler* handler,
        /* [in] */ Boolean unused);

    /**
     * Sets the listener which will be called for double-tap and related
     * gestures.
     *
     * @param onDoubleTapListener the listener invoked for all the callbacks, or
     *        null to stop listening for double-tap gestures.
     */
    CARAPI SetOnDoubleTapListener(
        /* [in] */ IOnDoubleTapListener* onDoubleTapListener);

    /**
     * Set whether longpress is enabled, if this is enabled when a user
     * presses and holds down you get a longpress event and nothing further.
     * If it's disabled the user can press and hold down and then later
     * moved their finger and you will get scroll events. By default
     * longpress is enabled.
     *
     * @param isLongpressEnabled whether longpress should be enabled.
     */
    CARAPI SetIsLongpressEnabled(
        /* [in] */ Boolean isLongpressEnabled);

    /**
     * @return true if longpress is enabled, else false.
     */
    CARAPI IsLongpressEnabled(
        /* [out] */ Boolean* result);

    /**
     * Analyzes the given motion event and if applicable triggers the
     * appropriate callbacks on the {@link OnGestureListener} supplied.
     *
     * @param ev The current motion event.
     * @return true if the {@link OnGestureListener} consumed the event,
     *              else false.
     */
    CARAPI OnTouchEvent(
        /* [in] */ IMotionEvent* ev,
        /* [out] */ Boolean* result);

private:
    CARAPI Init(
        /* [in] */ IContext* context = NULL,
        /* [in] */ IOnGestureListener* listener = NULL,
        /* [in] */ IHandler* handler = NULL);

    CARAPI_(void) Cancel();

    CARAPI_(void) CancelTaps();

    CARAPI_(Boolean) IsConsideredDoubleTap(
        /* [in] */ IMotionEvent* firstDown,
        /* [in] */ IMotionEvent* firstUp,
        /* [in] */ IMotionEvent* secondDown);

    CARAPI_(void) DispatchLongPress();

private:
    Int32 mTouchSlopSquare;
    Int32 mDoubleTapTouchSlopSquare;
    Int32 mDoubleTapSlopSquare;
    Int32 mMinimumFlingVelocity;
    Int32 mMaximumFlingVelocity;

    static Int32 LONGPRESS_TIMEOUT;// = ViewConfiguration.getLongPressTimeout();
    static Int32 TAP_TIMEOUT;// = ViewConfiguration.getTapTimeout();
    static Int32 DOUBLE_TAP_TIMEOUT;// = ViewConfiguration.getDoubleTapTimeout();

    // constants for Message.what used by GestureHandler below
    static const Int32 SHOW_PRESS = 1;
    static const Int32 LONG_PRESS = 2;
    static const Int32 TAP = 3;

    AutoPtr<IHandler> mHandler;
    IOnGestureListener* mListener;
    AutoPtr<IOnDoubleTapListener> mDoubleTapListener;

    Boolean mStillDown;
    Boolean mInLongPress;
    Boolean mAlwaysInTapRegion;
    Boolean mAlwaysInBiggerTapRegion;

    AutoPtr<IMotionEvent> mCurrentDownEvent;
    AutoPtr<IMotionEvent> mPreviousUpEvent;

    /**
     * True when the user is still touching for the second tap (down, move, and
     * up events). Can only be true if there is a double tap listener attached.
     */
    Boolean mIsDoubleTapping;

    Float mLastFocusX;
    Float mLastFocusY;
    Float mDownFocusX;
    Float mDownFocusY;

    Boolean mIsLongpressEnabled;

    /**
     * Determines speed during touch scrolling
     */
    AutoPtr<VelocityTracker> mVelocityTracker;

    /**
     * Consistency verifier for debugging purposes.
     */
    AutoPtr<InputEventConsistencyVerifier> mInputEventConsistencyVerifier;
};

} // namespace View
} // namespace Droid
} // namespace Elastos

#endif //__ELASTOS_DROID_VIEW_CGESTUREDETECTOR_H__
