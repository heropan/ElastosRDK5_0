
#ifndef __ELASTOS_DROID_WEBKIT_WEBVIEWINPUTDISPATCHER_H__
#define __ELASTOS_DROID_WEBKIT_WEBVIEWINPUTDISPATCHER_H__

#include "elastos/droid/ext/frameworkext.h"
#include "os/HandlerBase.h"

using Elastos::Droid::Content::IContext;
using Elastos::Droid::Os::HandlerBase;
using Elastos::Droid::View::IMotionEvent;

namespace Elastos {
namespace Droid {
namespace Webkit {

/**
 * Perform asynchronous dispatch of input events in a {@link WebView}.
 *
 * This dispatcher is shared by the UI thread ({@link WebViewClassic}) and web kit
 * thread ({@link WebViewCore}).  The UI thread enqueues events for
 * processing, waits for the web kit thread to handle them, and then performs
 * additional processing depending on the outcome.
 *
 * How it works:
 *
 * 1. The web view thread receives an input event from the input system on the UI
 * thread in its {@link WebViewClassic#onTouchEvent} handler.  It sends the input event
 * to the dispatcher, then immediately returns true to the input system to indicate that
 * it will handle the event.
 *
 * 2. The web kit thread is notified that an event has been enqueued.  Meanwhile additional
 * events may be enqueued from the UI thread.  In some cases, the dispatcher may decide to
 * coalesce motion events into larger batches or to cancel events that have been
 * sitting in the queue for too long.
 *
 * 3. The web kit thread wakes up and handles all input events that are waiting for it.
 * After processing each input event, it informs the dispatcher whether the web application
 * has decided to handle the event itself and to prevent default event handling.
 *
 * 4. If web kit indicates that it wants to prevent default event handling, then web kit
 * consumes the remainder of the gesture and web view receives a cancel event if
 * needed.  Otherwise, the web view handles the gesture on the UI thread normally.
 *
 * 5. If the web kit thread takes too Int64 to handle an input event, then it loses the
 * right to handle it.  The dispatcher synthesizes a cancellation event for web kit and
 * then tells the web view on the UI thread to handle the event that timed out along
 * with the rest of the gesture.
 *
 * One thing to keep in mind about the dispatcher is that what goes into the dispatcher
 * is not necessarily what the web kit or UI thread will see.  As mentioned above, the
 * dispatcher may tweak the input event stream to improve responsiveness.  Both web view and
 * web kit are guaranteed to perceive a consistent stream of input events but
 * they might not always see the same events (especially if one decides
 * to prevent the other from handling a particular gesture).
 *
 * This implementation very deliberately does not refer to the {@link WebViewClassic}
 * or {@link WebViewCore} classes, preferring to communicate with them only via
 * interfaces to avoid unintentional coupling to their implementation details.
 *
 * Currently, the input dispatcher only handles pointer events (includes touch,
 * hover and scroll events).  In principle, it could be extended to handle trackball
 * and key events if needed.
 *
 * @hide
 */
class WebViewInputDispatcher : public ElRefBase
{
public:
    /* Implemented by {@link WebViewClassic} to perform operations on the UI thread. */
    interface IUiCallbacks : public IInterface
    {
    public:
        /**
         * Gets the UI thread's looper.
         * @return The looper.
         */
        virtual CARAPI GetUiLooper(
            /* [out] */ ILooper** looper) = 0;

        /**
         * Gets the UI's context
         * @return The context
         */
        virtual CARAPI GetContext(
            /* [out] */ IContext** context) = 0;

        /**
         * Dispatches an event to the UI.
         * @param event The event.
         * @param eventType The event type.
         * @param flags The event's dispatch flags.
         */
        virtual CARAPI DispatchUiEvent(
            /* [in] */ IMotionEvent* event,
            /* [in] */ Int32 eventType,
            /* [in] */ Int32 flags) = 0;

        /**
         * Asks the UI thread whether this touch event stream should be
         * intercepted based on the touch down event.
         * @param event The touch down event.
         * @return true if the UI stream wants the touch stream without going
         * through webkit or false otherwise.
         */
        virtual CARAPI ShouldInterceptTouchEvent(
            /* [in] */ IMotionEvent* event,
            /* [out] */ Boolean* result) = 0;

        /**
         * Inform's the UI that it should show the tap highlight
         * @param show True if it should show the highlight, false if it should hide it
         */
        virtual CARAPI ShowTapHighlight(
            /* [in] */ Boolean show) = 0;

        /**
         * Called when we are sending a new EVENT_TYPE_HIT_TEST to WebKit, so
         * previous hit tests should be cleared as they are obsolete.
         */
        virtual CARAPI ClearPreviousHitTest() = 0;
    };

    /* Implemented by {@link WebViewCore} to perform operations on the web kit thread. */
    interface IWebKitCallbacks : public IInterface
    {
    public:
        /**
         * Gets the web kit thread's looper.
         * @return The looper.
         */
        virtual CARAPI GetWebKitLooper(
            /* [out] */ ILooper** looper) = 0;

        /**
         * Dispatches an event to web kit.
         * @param dispatcher The WebViewInputDispatcher sending the event
         * @param event The event.
         * @param eventType The event type.
         * @param flags The event's dispatch flags.
         * @return True if web kit wants to prevent default event handling.
         */
        virtual CARAPI DispatchWebKitEvent(
            /* [in] */ WebViewInputDispatcher* dispatcher,
            /* [in] */ IMotionEvent* event,
            /* [in] */ Int32 eventType,
            /* [in] */ Int32 flags,
            /* [out] */ Boolean* result) = 0;
    };

private:
    // Runs on UI thread.
    class UiHandler : public HandlerBase
    {
    public:
        static const Int32 MSG_DISPATCH_UI_EVENTS = 1;
        static const Int32 MSG_WEBKIT_TIMEOUT = 2;
        static const Int32 MSG_LONG_PRESS = 3;
        static const Int32 MSG_CLICK = 4;
        static const Int32 MSG_SHOW_TAP_HIGHLIGHT = 5;
        static const Int32 MSG_HIDE_TAP_HIGHLIGHT = 6;

    public:
        UiHandler(
            /* [in] */ WebViewInputDispatcher* owner,
            /* [in] */ ILooper* looper);

        CARAPI HandleMessage(
            /* [in] */ IMessage* msg);

    private:
        AutoPtr<WebViewInputDispatcher> mOwner;
    };

    // Runs on web kit thread.
    class WebKitHandler : public HandlerBase
    {
    public:
        static const Int32 MSG_DISPATCH_WEBKIT_EVENTS = 1;

    public:
        WebKitHandler(
            /* [in] */ WebViewInputDispatcher* owner,
            /* [in] */ ILooper* looper);

        CARAPI HandleMessage(
            /* [in] */ IMessage* msg);

    private:
        AutoPtr<WebViewInputDispatcher> mOwner;
    };

    class DispatchEvent : public ElLightRefBase
    {
    public:
        AutoPtr<DispatchEvent> mNext;

        AutoPtr<IMotionEvent> mEvent;
        Int32 mEventType;
        Int32 mFlags;
        Int64 mTimeoutTime;
        Int32 mWebKitXOffset;
        Int32 mWebKitYOffset;
        Float mWebKitScale;
    };

    class DispatchEventQueue : public ElRefBase
    {
    public:
        virtual CARAPI_(Boolean) IsEmpty();

        virtual CARAPI_(void) Enqueue(
            /* [in] */ DispatchEvent* d);

        virtual CARAPI_(AutoPtr<DispatchEvent>) Dequeue();

        virtual CARAPI_(AutoPtr<DispatchEvent>) DequeueList();

    public:
        AutoPtr<DispatchEvent> mHead;
        AutoPtr<DispatchEvent> mTail;
    };

    /**
     * Keeps track of a stream of touch events so that we can discard touch
     * events that would make the stream inconsistent.
     */
    class TouchStream : public ElRefBase
    {
    public:
        /**
         * Gets the last touch event that was delivered.
         * @return The last touch event, or null if none.
         */
        virtual CARAPI_(AutoPtr<IMotionEvent>) GetLastEvent();

        /**
         * Updates the touch event stream.
         * @param event The event that we intend to send, or null to cancel the
         * touch event stream.
         * @return The event that we should actually send, or null if no event should
         * be sent because the proposed event would make the stream inconsistent.
         */
        virtual CARAPI_(AutoPtr<IMotionEvent>) Update(
            /* [in] */ IMotionEvent* event);

        /**
         * Returns true if there is a gesture in progress that may need to be canceled.
         * @return True if cancel is needed.
         */
        virtual CARAPI_(Boolean) IsCancelNeeded();

    private:
        CARAPI_(void) UpdateLastEvent(
            /* [in] */ IMotionEvent* event);

        AutoPtr<IMotionEvent> mLastEvent;
    };

public:
    /**
     * Event type: Indicates a touch event type.
     *
     * This event is delivered together with a {@link MotionEvent} with one of the
     * following actions: {@link MotionEvent#ACTION_DOWN}, {@link MotionEvent#ACTION_MOVE},
     * {@link MotionEvent#ACTION_UP}, {@link MotionEvent#ACTION_POINTER_DOWN},
     * {@link MotionEvent#ACTION_POINTER_UP}, {@link MotionEvent#ACTION_CANCEL}.
     */
    static const Int32 EVENT_TYPE_TOUCH = 0;

    /**
     * Event type: Indicates a hover event type.
     *
     * This event is delivered together with a {@link MotionEvent} with one of the
     * following actions: {@link MotionEvent#ACTION_HOVER_ENTER},
     * {@link MotionEvent#ACTION_HOVER_MOVE}, {@link MotionEvent#ACTION_HOVER_MOVE}.
     */
    static const Int32 EVENT_TYPE_HOVER = 1;

    /**
     * Event type: Indicates a scroll event type.
     *
     * This event is delivered together with a {@link MotionEvent} with action
     * {@link MotionEvent#ACTION_SCROLL}.
     */
    static const Int32 EVENT_TYPE_SCROLL = 2;

    /**
     * Event type: Indicates a long-press event type.
     *
     * This event is delivered in the middle of a sequence of {@link #EVENT_TYPE_TOUCH} events.
     * It includes a {@link MotionEvent} with action {@link MotionEvent#ACTION_MOVE}
     * that indicates the current touch coordinates of the long-press.
     *
     * This event is sent when the current touch gesture has been held longer than
     * the long-press interval.
     */
    static const Int32 EVENT_TYPE_LONG_PRESS = 3;

    /**
     * Event type: Indicates a click event type.
     *
     * This event is delivered after a sequence of {@link #EVENT_TYPE_TOUCH} events that
     * comprise a complete gesture ending with {@link MotionEvent#ACTION_UP}.
     * It includes a {@link MotionEvent} with action {@link MotionEvent#ACTION_UP}
     * that indicates the location of the click.
     *
     * This event is sent shortly after the end of a touch after the double-tap
     * interval has expired to indicate a click.
     */
    static const Int32 EVENT_TYPE_CLICK = 4;

    /**
     * Event type: Indicates a double-tap event type.
     *
     * This event is delivered after a sequence of {@link #EVENT_TYPE_TOUCH} events that
     * comprise a complete gesture ending with {@link MotionEvent#ACTION_UP}.
     * It includes a {@link MotionEvent} with action {@link MotionEvent#ACTION_UP}
     * that indicates the location of the double-tap.
     *
     * This event is sent immediately after a sequence of two touches separated
     * in time by no more than the double-tap interval and separated in space
     * by no more than the double-tap slop.
     */
    static const Int32 EVENT_TYPE_DOUBLE_TAP = 5;

    /**
     * Event type: Indicates that a hit test should be performed
     */
    static const Int32 EVENT_TYPE_HIT_TEST = 6;

    /**
     * Flag: This event is private to this queue.  Do not forward it.
     */
    static const Int32 FLAG_PRIVATE = 1 << 0;

    /**
     * Flag: This event is currently being processed by web kit.
     * If a timeout occurs, make a copy of it before forwarding the event to another queue.
     */
    static const Int32 FLAG_WEBKIT_IN_PROGRESS = 1 << 1;

    /**
     * Flag: A timeout occurred while waiting for web kit to process this input event.
     */
    static const Int32 FLAG_WEBKIT_TIMEOUT = 1 << 2;

    /**
     * Flag: Indicates that the event was transformed for delivery to web kit.
     * The event must be transformed back before being delivered to the UI.
     */
    static const Int32 FLAG_WEBKIT_TRANSFORMED_EVENT = 1 << 3;

public:
    WebViewInputDispatcher(
        /* [in] */ IUiCallbacks* uiCallbacks,
        /* [in] */ IWebKitCallbacks* webKitCallbacks);

    /**
     * Sets whether web kit wants to receive touch events.
     *
     * @param enable True to enable dispatching of touch events to web kit, otherwise
     * web kit will be skipped.
     */
    virtual CARAPI_(void) SetWebKitWantsTouchEvents(
        /* [in] */ Boolean enable);

    /**
     * Posts a pointer event to the dispatch queue.
     *
     * @param event The event to post.
     * @param webKitXOffset X offset to apply to events before dispatching them to web kit.
     * @param webKitYOffset Y offset to apply to events before dispatching them to web kit.
     * @param webKitScale The scale factor to apply to translated events before dispatching
     * them to web kit.
     * @return True if the dispatcher will handle the event, false if the event is unsupported.
     */
    virtual CARAPI_(Boolean) PostPointerEvent(
        /* [in] */ IMotionEvent* event,
        /* [in] */ Int32 webKitXOffset,
        /* [in] */ Int32 webKitYOffset,
        /* [in] */ Float webKitScale);

    /**
     * Dispatches pending web kit events.
     * Must only be called from the web kit thread.
     *
     * This method may be used to flush the queue of pending input events
     * immediately.  This method may help to reduce input dispatch latency
     * if called before certain expensive operations such as drawing.
     */
    virtual CARAPI_(void) DispatchWebKitEvents();

    // Called by WebKit when it doesn't care about the rest of the touch stream
    virtual CARAPI_(void) SkipWebkitForRemainingTouchStream();

    /**
     * Dispatches pending UI events.
     * Must only be called from the UI thread.
     *
     * This method may be used to flush the queue of pending input events
     * immediately.  This method may help to reduce input dispatch latency
     * if called before certain expensive operations such as drawing.
     */
    virtual CARAPI_(void) DispatchUiEvents();

    static Boolean StaticValueInit();

private:
    CARAPI_(void) ScheduleLongPressLocked();

    CARAPI_(void) UnscheduleLongPressLocked();

    CARAPI_(void) PostLongPress();

    CARAPI_(void) HideTapCandidateLocked();

    CARAPI_(void) ShowTapCandidateLocked();

    CARAPI_(void) ScheduleShowTapHighlightLocked();

    CARAPI_(void) UnscheduleShowTapHighlightLocked();

    CARAPI_(void) ScheduleHideTapHighlightLocked();

    CARAPI_(void) UnscheduleHideTapHighlightLocked();

    CARAPI_(void) PostShowTapHighlight(
        /* [in] */ Boolean show);

    CARAPI_(void) ScheduleClickLocked();

    CARAPI_(void) UnscheduleClickLocked();

    CARAPI_(void) PostClick();

    CARAPI_(void) CheckForDoubleTapOnDownLocked(
        /* [in] */ IMotionEvent* event);

    CARAPI_(Boolean) IsClickCandidateLocked(
        /* [in] */ IMotionEvent* event);

    CARAPI_(void) EnqueueDoubleTapLocked(
        /* [in] */ IMotionEvent* event);

    CARAPI_(void) EnqueueHitTestLocked(
        /* [in] */ IMotionEvent* event);

    CARAPI_(void) CheckForSlopLocked(
        /* [in] */ IMotionEvent* event);

    CARAPI_(void) UpdateStateTrackersLocked(
        /* [in] */ DispatchEvent* d,
        /* [in] */ IMotionEvent* event);

    CARAPI_(void) DispatchWebKitEvents(
        /* [in] */ Boolean calledFromHandler);

    // Runs on web kit thread.
    CARAPI_(Boolean) DispatchWebKitEvent(
        /* [in] */ IMotionEvent* event,
        /* [in] */ Int32 eventType,
        /* [in] */ Int32 flags);

    CARAPI_(Boolean) IsMoveEventLocked(
        /* [in] */ DispatchEvent* d);

    CARAPI_(void) DrainStaleWebKitEventsLocked();

    // Runs on UI thread in response to the web kit thread appearing to be unresponsive.
    CARAPI_(void) HandleWebKitTimeout();

    CARAPI_(void) SkipWebKitEventLocked(
        /* [in] */ DispatchEvent* d);

    CARAPI_(void) DispatchUiEvents(
        /* [in] */ Boolean calledFromHandler);

    // Runs on UI thread.
    CARAPI_(void) DispatchUiEvent(
        /* [in] */ IMotionEvent* event,
        /* [in] */ Int32 eventType,
        /* [in] */ Int32 flags);

    CARAPI_(void) EnqueueEventLocked(
        /* [in] */ DispatchEvent* d);

    CARAPI_(Boolean) ShouldSkipWebKit(
        /* [in] */ DispatchEvent* d);

    CARAPI_(void) EnqueueWebKitCancelTouchEventIfNeededLocked();

    CARAPI_(void) EnqueueWebKitEventLocked(
        /* [in] */ DispatchEvent* d);

    CARAPI_(void) EnqueueWebKitEventUnbatchedLocked(
        /* [in] */ DispatchEvent* d);

    CARAPI_(void) ScheduleWebKitDispatchLocked();

    CARAPI_(void) UpdateWebKitTimeoutLocked();

    CARAPI_(void) EnqueueUiCancelTouchEventIfNeededLocked();

    CARAPI_(void) EnqueueUiEventLocked(
        /* [in] */ DispatchEvent* d);

    CARAPI_(void) EnqueueUiEventUnbatchedLocked(
        /* [in] */ DispatchEvent* d);

    CARAPI_(void) ScheduleUiDispatchLocked();

    CARAPI_(Boolean) BatchEventLocked(
        /* [in] */ DispatchEvent* in,
        /* [in] */ DispatchEvent* tail);

    CARAPI_(AutoPtr<DispatchEvent>) ObtainDispatchEventLocked(
        /* [in] */ IMotionEvent* event,
        /* [in] */ Int32 eventType,
        /* [in] */ Int32 flags,
        /* [in] */ Int32 webKitXOffset,
        /* [in] */ Int32 webKitYOffset,
        /* [in] */ Float webKitScale);

    CARAPI_(AutoPtr<DispatchEvent>) CopyDispatchEventLocked(
        /* [in] */ DispatchEvent* d);

    CARAPI_(AutoPtr<DispatchEvent>) ObtainUninitializedDispatchEventLocked();

    CARAPI_(void) RecycleDispatchEventLocked(
        /* [in] */ DispatchEvent* d);

private:
    static const String TAG;
    static const Boolean DEBUG;
    // This enables batching of MotionEvents. It will combine multiple MotionEvents
    // together into a single MotionEvent if more events come in while we are
    // still waiting on the processing of a previous event.
    // If this is set to false, we will instead opt to drop ACTION_MOVE
    // events we cannot keep up with.
    // TODO: If batching proves to be working well, remove this
    static const Boolean ENABLE_EVENT_BATCHING;

    Object mLock;

    // Pool of queued input events.  (guarded by mLock)
    static const Int32 MAX_DISPATCH_EVENT_POOL_SIZE = 10;
    AutoPtr<DispatchEvent> mDispatchEventPool;
    Int32 mDispatchEventPoolSize;

    // Posted state, tracks events posted to the dispatcher.  (guarded by mLock)
    AutoPtr<TouchStream> mPostTouchStream;
    Boolean mPostSendTouchEventsToWebKit;
    Boolean mPostDoNotSendTouchEventsToWebKitUntilNextGesture;
    Boolean mPostLongPressScheduled;
    Boolean mPostClickScheduled;
    Boolean mPostShowTapHighlightScheduled;
    Boolean mPostHideTapHighlightScheduled;
    Int32 mPostLastWebKitXOffset;
    Int32 mPostLastWebKitYOffset;
    Float mPostLastWebKitScale;

    // State for event tracking (click, longpress, double tap, etc..)
    Boolean mIsDoubleTapCandidate;
    Boolean mIsTapCandidate;
    Float mInitialDownX;
    Float mInitialDownY;
    Float mTouchSlopSquared;
    Float mDoubleTapSlopSquared;

    // Web kit state, tracks events observed by web kit.  (guarded by mLock)
    AutoPtr<DispatchEventQueue> mWebKitDispatchEventQueue;
    AutoPtr<TouchStream> mWebKitTouchStream;
    AutoPtr<IWebKitCallbacks> mWebKitCallbacks;
    AutoPtr<WebKitHandler> mWebKitHandler;
    Boolean mWebKitDispatchScheduled;
    Boolean mWebKitTimeoutScheduled;
    Int64 mWebKitTimeoutTime;

    // UI state, tracks events observed by the UI.  (guarded by mLock)
    AutoPtr<DispatchEventQueue> mUiDispatchEventQueue;
    AutoPtr<TouchStream> mUiTouchStream;
    AutoPtr<IUiCallbacks> mUiCallbacks;
    AutoPtr<UiHandler> mUiHandler;
    Boolean mUiDispatchScheduled;

    // Give up on web kit handling of input events when this timeout expires.
    static const Int64 WEBKIT_TIMEOUT_MILLIS = 200;
    static /*const*/ Int32 TAP_TIMEOUT;
    static /*const*/ Int32 LONG_PRESS_TIMEOUT;
    static /*const*/ Int32 DOUBLE_TAP_TIMEOUT;
    static /*const*/ Int32 PRESSED_STATE_DURATION;
};

} // namespace Webkit
} // namespace Droid
} // namespace Elastos

#endif //__ELASTOS_DROID_WEBKIT_WEBVIEWINPUTDISPATCHER_H__
