
#include "os/SystemClock.h"
#include "view/CMotionEventHelper.h"
#include "view/CViewConfiguration.h"
#include "view/CViewConfigurationHelper.h"
#include "webkit/WebViewInputDispatcher.h"
#include "view/InputEvent.h"
#include <elastos/utility/logging/Logger.h>
#include "webkit/DebugFlags.h"

using Elastos::Droid::Os::EIID_IHandler;
using Elastos::Droid::Os::SystemClock;
using Elastos::Droid::View::IMotionEventHelper;
using Elastos::Droid::View::CMotionEventHelper;
using Elastos::Droid::View::CViewConfiguration;
using Elastos::Droid::View::IViewConfiguration;
using Elastos::Droid::View::CViewConfigurationHelper;
using Elastos::Droid::View::IViewConfigurationHelper;
using Elastos::Droid::View::IInputEvent;
using Elastos::Utility::Logging::Logger;

namespace Elastos {
namespace Droid {
namespace Webkit {

//===============================================================
//             WebViewInputDispatcher::UiHandler
//===============================================================

const Int32 WebViewInputDispatcher::UiHandler::MSG_DISPATCH_UI_EVENTS;
const Int32 WebViewInputDispatcher::UiHandler::MSG_WEBKIT_TIMEOUT;
const Int32 WebViewInputDispatcher::UiHandler::MSG_LONG_PRESS;
const Int32 WebViewInputDispatcher::UiHandler::MSG_CLICK;
const Int32 WebViewInputDispatcher::UiHandler::MSG_SHOW_TAP_HIGHLIGHT;
const Int32 WebViewInputDispatcher::UiHandler::MSG_HIDE_TAP_HIGHLIGHT;

WebViewInputDispatcher::UiHandler::UiHandler(
    /* [in] */ WebViewInputDispatcher* owner,
    /* [in] */ ILooper* looper)
    : mOwner(owner)
    , HandlerBase(looper)
{
}

ECode WebViewInputDispatcher::UiHandler::HandleMessage(
    /* [in] */ IMessage* msg)
{
    VALIDATE_NOT_NULL(msg);
    Int32 what;
    msg->GetWhat(&what);
    switch (what) {
        case MSG_DISPATCH_UI_EVENTS:
            mOwner->DispatchUiEvents(TRUE);
            break;
        case MSG_WEBKIT_TIMEOUT:
            mOwner->HandleWebKitTimeout();
            break;
        case MSG_LONG_PRESS:
            mOwner->PostLongPress();
            break;
        case MSG_CLICK:
            mOwner->PostClick();
            break;
        case MSG_SHOW_TAP_HIGHLIGHT:
            mOwner->PostShowTapHighlight(TRUE);
            break;
        case MSG_HIDE_TAP_HIGHLIGHT:
            mOwner->PostShowTapHighlight(FALSE);
            break;
        default:
            //throw new IllegalStateException("Unknown message type: " + msg.what);
            assert(0);
    }
    return NOERROR;
}

//===============================================================
//          WebViewInputDispatcher::WebKitHandler
//===============================================================

const Int32 WebViewInputDispatcher::WebKitHandler::MSG_DISPATCH_WEBKIT_EVENTS;

WebViewInputDispatcher::WebKitHandler::WebKitHandler(
    /* [in] */ WebViewInputDispatcher* owner,
    /* [in] */ ILooper* looper)
    : mOwner(owner)
    , HandlerBase(looper)
{
}

ECode WebViewInputDispatcher::WebKitHandler::HandleMessage(
    /* [in] */ IMessage* msg)
{
    VALIDATE_NOT_NULL(msg);
    Int32 what;
    msg->GetWhat(&what);
    switch (what) {
        case MSG_DISPATCH_WEBKIT_EVENTS:
            mOwner->DispatchWebKitEvents(TRUE);
            break;
        default:
            //throw new IllegalStateException("Unknown message type: " + msg.what);
            assert(0);
    }

    return NOERROR;
}

//===============================================================
//         WebViewInputDispatcher::DispatchEventQueue
//===============================================================

Boolean WebViewInputDispatcher::DispatchEventQueue::IsEmpty()
{
    return mHead != NULL;
}

void WebViewInputDispatcher::DispatchEventQueue::Enqueue(
    /* [in] */ WebViewInputDispatcher::DispatchEvent* d)
{
    if (mHead == NULL) {
        mHead = d;
        mTail = d;
    }
    else {
        mTail->mNext = d;
        mTail = d;
    }
}

AutoPtr<WebViewInputDispatcher::DispatchEvent> WebViewInputDispatcher::DispatchEventQueue::Dequeue()
{
    AutoPtr<DispatchEvent> d = mHead;
    if (d != NULL) {
        AutoPtr<DispatchEvent> next = d->mNext;
        if (next == NULL) {
            mHead = NULL;
            mTail = NULL;
        }
        else {
            mHead = next;
            d->mNext = NULL;
        }
    }

    return d;
}

AutoPtr<WebViewInputDispatcher::DispatchEvent> WebViewInputDispatcher::DispatchEventQueue::DequeueList()
{
    AutoPtr<DispatchEvent> d = mHead;
    if (d != NULL) {
        mHead = NULL;
        mTail = NULL;
    }

    return d;
}

//===============================================================
//             WebViewInputDispatcher::TouchStream
//===============================================================
/**
 * Gets the last touch event that was delivered.
 * @return The last touch event, or null if none.
 */
AutoPtr<IMotionEvent> WebViewInputDispatcher::TouchStream::GetLastEvent()
{
    return mLastEvent;
}

/**
 * Updates the touch event stream.
 * @param event The event that we intend to send, or null to cancel the
 * touch event stream.
 * @return The event that we should actually send, or null if no event should
 * be sent because the proposed event would make the stream inconsistent.
 */
AutoPtr<IMotionEvent> WebViewInputDispatcher::TouchStream::Update(
    /* [in] */ IMotionEvent* event)
{
    if (event == NULL) {
        AutoPtr<IMotionEvent> cancelEvent;
        if (IsCancelNeeded()) {
            cancelEvent = mLastEvent;
            if (cancelEvent != NULL) {
                cancelEvent->SetAction(IMotionEvent::ACTION_CANCEL);
                mLastEvent = NULL;
            }
        }
        return cancelEvent;
    }

    Int32 mask;
    Int32 action;
    event->GetActionMasked(&mask);
    switch (mask) {
        case IMotionEvent::ACTION_MOVE:
        case IMotionEvent::ACTION_UP:
        case IMotionEvent::ACTION_POINTER_DOWN:
        case IMotionEvent::ACTION_POINTER_UP:
            if (mLastEvent == NULL
                    || (mLastEvent->GetAction(&action), action == IMotionEvent::ACTION_UP)) {
                return NULL;
            }

            UpdateLastEvent(event);
            return event;

        case IMotionEvent::ACTION_DOWN:
            UpdateLastEvent(event);
            return event;

        case IMotionEvent::ACTION_CANCEL:
            if (mLastEvent == NULL) {
                return NULL;
            }

            UpdateLastEvent(NULL);

            return event;

        default:
            return NULL;
    }
}

/**
 * Returns true if there is a gesture in progress that may need to be canceled.
 * @return True if cancel is needed.
 */
Boolean WebViewInputDispatcher::TouchStream::IsCancelNeeded()
{
    Int32 action;
    return mLastEvent != NULL && (mLastEvent->GetAction(&action), action != IMotionEvent::ACTION_UP);
}

void WebViewInputDispatcher::TouchStream::UpdateLastEvent(
    /* [in] */ IMotionEvent* event)
{
    if (mLastEvent != NULL) {
        mLastEvent->Recycle();
    }

    if (event != NULL) {
        AutoPtr<IMotionEventHelper> meh;
        CMotionEventHelper::AcquireSingleton((IMotionEventHelper**)&meh);
        meh->ObtainNoHistory(event, (IMotionEvent**)&mLastEvent);
    }
    else {
        mLastEvent = NULL;
    }
}

//===============================================================
//                   WebViewInputDispatcher
//===============================================================

Boolean WebViewInputDispatcher::StaticValueInit()
{
    AutoPtr<IViewConfigurationHelper> viewConfigurationHelper;
    CViewConfigurationHelper::AcquireSingleton((IViewConfigurationHelper**)&viewConfigurationHelper);

    viewConfigurationHelper->GetTapTimeout(&TAP_TIMEOUT);
    viewConfigurationHelper->GetLongPressTimeout(&LONG_PRESS_TIMEOUT);
    LONG_PRESS_TIMEOUT += TAP_TIMEOUT;
    viewConfigurationHelper->GetDoubleTapTimeout(&DOUBLE_TAP_TIMEOUT);
    viewConfigurationHelper->GetPressedStateDuration(&PRESSED_STATE_DURATION);

    return TRUE;
}

const Int32 WebViewInputDispatcher::EVENT_TYPE_TOUCH;
const Int32 WebViewInputDispatcher::EVENT_TYPE_HOVER;
const Int32 WebViewInputDispatcher::EVENT_TYPE_SCROLL;
const Int32 WebViewInputDispatcher::EVENT_TYPE_LONG_PRESS;
const Int32 WebViewInputDispatcher::EVENT_TYPE_CLICK;
const Int32 WebViewInputDispatcher::EVENT_TYPE_DOUBLE_TAP;
const Int32 WebViewInputDispatcher::EVENT_TYPE_HIT_TEST;
const Int32 WebViewInputDispatcher::FLAG_PRIVATE;
const Int32 WebViewInputDispatcher::FLAG_WEBKIT_IN_PROGRESS;
const Int32 WebViewInputDispatcher::FLAG_WEBKIT_TIMEOUT;
const Int32 WebViewInputDispatcher::FLAG_WEBKIT_TRANSFORMED_EVENT;

const String WebViewInputDispatcher::TAG("WebViewInputDispatcher");
const Boolean WebViewInputDispatcher::DEBUG = FALSE;
const Boolean WebViewInputDispatcher::ENABLE_EVENT_BATCHING = TRUE;

const Int32 WebViewInputDispatcher::MAX_DISPATCH_EVENT_POOL_SIZE;
const Int64 WebViewInputDispatcher::WEBKIT_TIMEOUT_MILLIS;

Int32 WebViewInputDispatcher::TAP_TIMEOUT = 0;
Int32 WebViewInputDispatcher::LONG_PRESS_TIMEOUT = 0;
Int32 WebViewInputDispatcher::DOUBLE_TAP_TIMEOUT = 0;
Int32 WebViewInputDispatcher::PRESSED_STATE_DURATION = 0;
static Boolean result = WebViewInputDispatcher::StaticValueInit();

WebViewInputDispatcher::WebViewInputDispatcher(
    /* [in] */ IUiCallbacks* uiCallbacks,
    /* [in] */ IWebKitCallbacks* webKitCallbacks)
    : mDispatchEventPoolSize(0)
    , mPostTouchStream(new TouchStream())
    , mPostSendTouchEventsToWebKit(FALSE)
    , mPostDoNotSendTouchEventsToWebKitUntilNextGesture(FALSE)
    , mPostLongPressScheduled(FALSE)
    , mPostClickScheduled(FALSE)
    , mPostShowTapHighlightScheduled(FALSE)
    , mPostHideTapHighlightScheduled(FALSE)
    , mPostLastWebKitXOffset(0)
    , mPostLastWebKitYOffset(0)
    , mPostLastWebKitScale(0.0f)
    , mIsDoubleTapCandidate(FALSE)
    , mIsTapCandidate(FALSE)
    , mInitialDownX(0.0f)
    , mInitialDownY(0.0f)
    , mTouchSlopSquared(0.0f)
    , mDoubleTapSlopSquared(0.0f)
    , mWebKitDispatchEventQueue(new DispatchEventQueue())
    , mWebKitTouchStream(new TouchStream())
    , mWebKitDispatchScheduled(FALSE)
    , mWebKitTimeoutScheduled(FALSE)
    , mWebKitTimeoutTime(0)
    , mUiDispatchEventQueue(new DispatchEventQueue())
    , mUiTouchStream(new TouchStream())
    , mUiDispatchScheduled(FALSE)
{
    mUiCallbacks = uiCallbacks;
    AutoPtr<ILooper> uiLooper;
    uiCallbacks->GetUiLooper((ILooper**)&uiLooper);
    mUiHandler = new UiHandler(this, uiLooper);

    mWebKitCallbacks = webKitCallbacks;
    AutoPtr<ILooper> looper;
    webKitCallbacks->GetWebKitLooper((ILooper**)&looper);
    mWebKitHandler = new WebKitHandler(this, looper);

    AutoPtr<IViewConfigurationHelper> helper;
    CViewConfigurationHelper::AcquireSingleton((IViewConfigurationHelper**)&helper);
    AutoPtr<IContext> context;
    mUiCallbacks->GetContext((IContext**)&context);
    AutoPtr<IViewConfiguration> config;
    helper->Get(context, (IViewConfiguration**)&config);
    Int32 tmpDoubleTapSlopSquared;
    config->GetScaledDoubleTapSlop(&tmpDoubleTapSlopSquared);
    mDoubleTapSlopSquared = tmpDoubleTapSlopSquared;
    mDoubleTapSlopSquared = (mDoubleTapSlopSquared * mDoubleTapSlopSquared);
    Int32 tmpTouchSlopSquared;
    config->GetScaledTouchSlop(&tmpTouchSlopSquared);
    mTouchSlopSquared = tmpTouchSlopSquared;
    mTouchSlopSquared = (mTouchSlopSquared * mTouchSlopSquared);
}

/**
 * Sets whether web kit wants to receive touch events.
 *
 * @param enable True to enable dispatching of touch events to web kit, otherwise
 * web kit will be skipped.
 */
void WebViewInputDispatcher::SetWebKitWantsTouchEvents(
    /* [in] */ Boolean enable)
{
//    if (DEBUG) {
//        Log.d(TAG, "webkitWantsTouchEvents: " + enable);
//    }

    {
        Mutex::Autolock lock(mLock);
        if (mPostSendTouchEventsToWebKit != enable) {
            if (!enable) {
                EnqueueWebKitCancelTouchEventIfNeededLocked();
            }

            mPostSendTouchEventsToWebKit = enable;
        }
    }
}

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
Boolean WebViewInputDispatcher::PostPointerEvent(
    /* [in] */ IMotionEvent* event,
    /* [in] */ Int32 webKitXOffset,
    /* [in] */ Int32 webKitYOffset,
    /* [in] */ Float webKitScale)
{
    if (event == NULL) {
        //throw new IllegalArgumentException("event cannot be null");
        assert(0);
    }

//    if (DEBUG) {
//        Log.d(TAG, "postPointerEvent: " + event);
//    }

    Int32 action;
    event->GetActionMasked(&action);
    Int32 eventType;
    switch (action) {
        case IMotionEvent::ACTION_DOWN:
        case IMotionEvent::ACTION_MOVE:
        case IMotionEvent::ACTION_UP:
        case IMotionEvent::ACTION_POINTER_DOWN:
        case IMotionEvent::ACTION_POINTER_UP:
        case IMotionEvent::ACTION_CANCEL:
            eventType = EVENT_TYPE_TOUCH;
            break;

        case IMotionEvent::ACTION_SCROLL:
            eventType = EVENT_TYPE_SCROLL;
            break;

        case IMotionEvent::ACTION_HOVER_ENTER:
        case IMotionEvent::ACTION_HOVER_MOVE:
        case IMotionEvent::ACTION_HOVER_EXIT:
            eventType = EVENT_TYPE_HOVER;
            break;

        default:
            return FALSE; // currently unsupported event type
    }

    {
        Mutex::Autolock lock(mLock);

        // Ensure that the event is consistent and should be delivered.
        AutoPtr<IMotionEvent> eventToEnqueue = event;
        if (eventType == EVENT_TYPE_TOUCH) {
            eventToEnqueue = mPostTouchStream->Update(event);
            if (eventToEnqueue == NULL) {
                if(DebugFlags::WEB_VIEW)
                    Logger::D(TAG, "PostPointerEvent, action:%d, enentType:%d", action, eventType);
//                if (DEBUG) {
//                    Log.d(TAG, "postPointerEvent: dropped event " + event);
//                }

                UnscheduleLongPressLocked();
                UnscheduleClickLocked();
                HideTapCandidateLocked();
                return FALSE;
            }

            if (action == IMotionEvent::ACTION_DOWN && mPostSendTouchEventsToWebKit) {
                Boolean result;
                if (mUiCallbacks->ShouldInterceptTouchEvent(eventToEnqueue, &result), result) {
                    mPostDoNotSendTouchEventsToWebKitUntilNextGesture = TRUE;
                }
                else if (mPostDoNotSendTouchEventsToWebKitUntilNextGesture) {
                    // Recover from a previous web kit timeout.
                    mPostDoNotSendTouchEventsToWebKitUntilNextGesture = FALSE;
                }
            }
        }

        // Copy the event because we need to retain ownership.
        if (eventToEnqueue == event) {
            event->Copy((IInputEvent**)&eventToEnqueue);
        }

        AutoPtr<DispatchEvent> d = ObtainDispatchEventLocked(eventToEnqueue, eventType, 0,
                webKitXOffset, webKitYOffset, webKitScale);

        UpdateStateTrackersLocked(d, event);
        EnqueueEventLocked(d);
    }

    return TRUE;
}

/**
 * Dispatches pending web kit events.
 * Must only be called from the web kit thread.
 *
 * This method may be used to flush the queue of pending input events
 * immediately.  This method may help to reduce input dispatch latency
 * if called before certain expensive operations such as drawing.
 */
void WebViewInputDispatcher::DispatchWebKitEvents()
{
    DispatchWebKitEvents(FALSE);
}

// Called by WebKit when it doesn't care about the rest of the touch stream
void WebViewInputDispatcher::SkipWebkitForRemainingTouchStream()
{
    // Just treat this like a timeout
    HandleWebKitTimeout();
}

/**
 * Dispatches pending UI events.
 * Must only be called from the UI thread.
 *
 * This method may be used to flush the queue of pending input events
 * immediately.  This method may help to reduce input dispatch latency
 * if called before certain expensive operations such as drawing.
 */
void WebViewInputDispatcher::DispatchUiEvents()
{
    DispatchUiEvents(FALSE);
}

void WebViewInputDispatcher::ScheduleLongPressLocked()
{
    UnscheduleLongPressLocked();
    mPostLongPressScheduled = TRUE;
    Boolean bResult = FALSE;
    mUiHandler->SendEmptyMessageDelayed(UiHandler::MSG_LONG_PRESS,
         LONG_PRESS_TIMEOUT, &bResult);
}

void WebViewInputDispatcher::UnscheduleLongPressLocked()
{
    if (mPostLongPressScheduled) {
        mPostLongPressScheduled = FALSE;
        mUiHandler->RemoveMessages(UiHandler::MSG_LONG_PRESS);
    }
}

void WebViewInputDispatcher::PostLongPress()
{
    Mutex::Autolock lock(mLock);

    if (!mPostLongPressScheduled) {
        return;
    }

    mPostLongPressScheduled = FALSE;

    AutoPtr<IMotionEvent> event = mPostTouchStream->GetLastEvent();
    if (event == NULL) {
        return;
    }

    Int32 mask;
    event->GetActionMasked(&mask);
    switch (mask) {
        case IMotionEvent::ACTION_DOWN:
        case IMotionEvent::ACTION_MOVE:
        case IMotionEvent::ACTION_POINTER_DOWN:
        case IMotionEvent::ACTION_POINTER_UP:
            break;

        default:
            return;
    }

    AutoPtr<IMotionEventHelper> meh;
    CMotionEventHelper::AcquireSingleton((IMotionEventHelper**)&meh);
    AutoPtr<IMotionEvent> eventToEnqueue;
    meh->ObtainNoHistory(event, (IMotionEvent**)&eventToEnqueue);
    eventToEnqueue->SetAction(IMotionEvent::ACTION_MOVE);
    AutoPtr<DispatchEvent> d = ObtainDispatchEventLocked(eventToEnqueue, EVENT_TYPE_LONG_PRESS, 0,
            mPostLastWebKitXOffset, mPostLastWebKitYOffset, mPostLastWebKitScale);
    EnqueueEventLocked(d);
}

void WebViewInputDispatcher::HideTapCandidateLocked()
{
    UnscheduleHideTapHighlightLocked();
    UnscheduleShowTapHighlightLocked();
    mUiCallbacks->ShowTapHighlight(FALSE);
}

void WebViewInputDispatcher::ShowTapCandidateLocked()
{
    UnscheduleHideTapHighlightLocked();
    UnscheduleShowTapHighlightLocked();
    mUiCallbacks->ShowTapHighlight(TRUE);
}

void WebViewInputDispatcher::ScheduleShowTapHighlightLocked()
{
    UnscheduleShowTapHighlightLocked();
    mPostShowTapHighlightScheduled = TRUE;
    Boolean bResult = FALSE;
    mUiHandler->SendEmptyMessageDelayed(UiHandler::MSG_SHOW_TAP_HIGHLIGHT,
           TAP_TIMEOUT, &bResult);
}

void WebViewInputDispatcher::UnscheduleShowTapHighlightLocked()
{
    if (mPostShowTapHighlightScheduled) {
        mPostShowTapHighlightScheduled = FALSE;
        mUiHandler->RemoveMessages(UiHandler::MSG_SHOW_TAP_HIGHLIGHT);
    }
}

void WebViewInputDispatcher::ScheduleHideTapHighlightLocked()
{
    UnscheduleHideTapHighlightLocked();
    mPostHideTapHighlightScheduled = TRUE;
    Boolean bResult = FALSE;
    mUiHandler->SendEmptyMessageDelayed(UiHandler::MSG_HIDE_TAP_HIGHLIGHT,
            PRESSED_STATE_DURATION, &bResult);
}

void WebViewInputDispatcher::UnscheduleHideTapHighlightLocked()
{
    if (mPostHideTapHighlightScheduled) {
        mPostHideTapHighlightScheduled = FALSE;
        mUiHandler->RemoveMessages(UiHandler::MSG_HIDE_TAP_HIGHLIGHT);
    }
}

void WebViewInputDispatcher::PostShowTapHighlight(
    /* [in] */ Boolean show)
{
    Mutex::Autolock lock(mLock);
    if (show) {
        if (!mPostShowTapHighlightScheduled) {
            return;
        }
        mPostShowTapHighlightScheduled = FALSE;
    }
    else {
        if (!mPostHideTapHighlightScheduled) {
            return;
        }
        mPostHideTapHighlightScheduled = FALSE;
    }

    mUiCallbacks->ShowTapHighlight(show);
}

void WebViewInputDispatcher::ScheduleClickLocked()
{
    UnscheduleClickLocked();
    mPostClickScheduled = TRUE;
    Boolean bResult = FALSE;
    mUiHandler->SendEmptyMessageDelayed(UiHandler::MSG_CLICK, DOUBLE_TAP_TIMEOUT, &bResult);
}

void WebViewInputDispatcher::UnscheduleClickLocked()
{
    if (mPostClickScheduled) {
        mPostClickScheduled = FALSE;
        mUiHandler->RemoveMessages(UiHandler::MSG_CLICK);
    }
}

void WebViewInputDispatcher::PostClick()
{
    Mutex::Autolock lock(mLock);
    if (!mPostClickScheduled) {
        return;
    }

    mPostClickScheduled = FALSE;

    AutoPtr<IMotionEvent> event = mPostTouchStream->GetLastEvent();
    Int32 action;
    if (event == NULL || (event->GetAction(&action), action != IMotionEvent::ACTION_UP)) {
        return;
    }

    ShowTapCandidateLocked();
    AutoPtr<IMotionEventHelper> mh;
    AutoPtr<IMotionEvent> eventToEnqueue;
    CMotionEventHelper::AcquireSingleton((IMotionEventHelper**)&mh);
    mh->ObtainNoHistory(IMotionEvent::Probe(event), (IMotionEvent**)&eventToEnqueue);

    AutoPtr<DispatchEvent> d = ObtainDispatchEventLocked(eventToEnqueue, EVENT_TYPE_CLICK, 0,
            mPostLastWebKitXOffset, mPostLastWebKitYOffset, mPostLastWebKitScale);
    EnqueueEventLocked(d);
}

void WebViewInputDispatcher::CheckForDoubleTapOnDownLocked(
    /* [in] */ IMotionEvent* event)
{
    mIsDoubleTapCandidate = FALSE;
    if (!mPostClickScheduled) {
        return;
    }

    Float x, y;
    event->GetX(&x);
    event->GetY(&y);
    Int32 deltaX = (Int32) mInitialDownX - (Int32)x;
    Int32 deltaY = (Int32) mInitialDownY - (Int32)y;
    if ((deltaX * deltaX + deltaY * deltaY) < mDoubleTapSlopSquared) {
        UnscheduleClickLocked();
        mIsDoubleTapCandidate = TRUE;
    }
}


Boolean WebViewInputDispatcher::IsClickCandidateLocked(
    /* [in] */ IMotionEvent* event)
{
    Int32 mask;
    if (event == NULL
            || (event->GetActionMasked(&mask), mask != IMotionEvent::ACTION_UP)
            || !mIsTapCandidate) {
        return FALSE;
    }

    Int64 eventTime, downTime;
    event->GetEventTime(&eventTime);
    event->GetDownTime(&downTime);
    Int64 downDuration = eventTime - downTime;
    return downDuration < LONG_PRESS_TIMEOUT;
}

void WebViewInputDispatcher::EnqueueDoubleTapLocked(
    /* [in] */ IMotionEvent* event)
{
    AutoPtr<IMotionEventHelper> mh;
    AutoPtr<IMotionEvent> eventToEnqueue;
    CMotionEventHelper::AcquireSingleton((IMotionEventHelper**)&mh);
    mh->ObtainNoHistory(IMotionEvent::Probe(event), (IMotionEvent**)&eventToEnqueue);

    AutoPtr<DispatchEvent> d = ObtainDispatchEventLocked(eventToEnqueue, EVENT_TYPE_DOUBLE_TAP, 0,
            mPostLastWebKitXOffset, mPostLastWebKitYOffset, mPostLastWebKitScale);
    EnqueueEventLocked(d);
}

void WebViewInputDispatcher::EnqueueHitTestLocked(
    /* [in] */ IMotionEvent* event)
{
    mUiCallbacks->ClearPreviousHitTest();
    AutoPtr<IMotionEventHelper> mh;
    AutoPtr<IMotionEvent> eventToEnqueue;
    CMotionEventHelper::AcquireSingleton((IMotionEventHelper**)&mh);
    mh->ObtainNoHistory(IMotionEvent::Probe(event), (IMotionEvent**)&eventToEnqueue);

    AutoPtr<DispatchEvent> d = ObtainDispatchEventLocked(eventToEnqueue, EVENT_TYPE_HIT_TEST, 0,
            mPostLastWebKitXOffset, mPostLastWebKitYOffset, mPostLastWebKitScale);
    EnqueueEventLocked(d);
}

void WebViewInputDispatcher::CheckForSlopLocked(
    /* [in] */ IMotionEvent* event)
{
    if (!mIsTapCandidate) {
        return;
    }

    Float x, y;
    event->GetX(&x);
    event->GetY(&y);
    Int32 deltaX = (Int32) mInitialDownX - (Int32) x;
    Int32 deltaY = (Int32) mInitialDownY - (Int32) y;
    if ((deltaX * deltaX + deltaY * deltaY) > mTouchSlopSquared) {
        UnscheduleLongPressLocked();
        mIsTapCandidate = FALSE;
        HideTapCandidateLocked();
    }
}

void WebViewInputDispatcher::UpdateStateTrackersLocked(
    /* [in] */ DispatchEvent* d,
    /* [in] */ IMotionEvent* event)
{
    mPostLastWebKitXOffset = d->mWebKitXOffset;
    mPostLastWebKitYOffset = d->mWebKitYOffset;
    mPostLastWebKitScale = d->mWebKitScale;
    Int32 a;
    Int32 action = event != NULL ? (event->GetAction(&a), a) : IMotionEvent::ACTION_CANCEL;
    if (d->mEventType != EVENT_TYPE_TOUCH) {
        return;
    }

    Int32 count;
    if (action == IMotionEvent::ACTION_CANCEL
            || (event->GetPointerCount(&count), count > 1)) {
        UnscheduleLongPressLocked();
        UnscheduleClickLocked();
        HideTapCandidateLocked();
        mIsDoubleTapCandidate = FALSE;
        mIsTapCandidate = FALSE;
        HideTapCandidateLocked();
    }
    else if (action == IMotionEvent::ACTION_DOWN) {
        CheckForDoubleTapOnDownLocked(event);
        ScheduleLongPressLocked();
        mIsTapCandidate = TRUE;
        event->GetX(&mInitialDownX);
        event->GetY(&mInitialDownY);
        EnqueueHitTestLocked(event);
        if (mIsDoubleTapCandidate) {
            HideTapCandidateLocked();
        }
        else {
            ScheduleShowTapHighlightLocked();
        }
    }
    else if (action == IMotionEvent::ACTION_UP) {
        UnscheduleLongPressLocked();
        if (IsClickCandidateLocked(event)) {
            if (mIsDoubleTapCandidate) {
                HideTapCandidateLocked();
                EnqueueDoubleTapLocked(event);
            }
            else {
                ScheduleClickLocked();
            }
        }
        else {
            HideTapCandidateLocked();
        }
    }
    else if (action == IMotionEvent::ACTION_MOVE) {
        CheckForSlopLocked(event);
    }
}

void WebViewInputDispatcher::DispatchWebKitEvents(
    /* [in] */ Boolean calledFromHandler)
{
    for (;;) {
        // Get the next event, but leave it in the queue so we can move it to the UI
        // queue if a timeout occurs.
        AutoPtr<DispatchEvent> d;
        AutoPtr<IMotionEvent> event;
        Int32 eventType;
        Int32 flags;

        {
            Mutex::Autolock lock(mLock);
            if (!ENABLE_EVENT_BATCHING) {
                DrainStaleWebKitEventsLocked();
            }

            d = mWebKitDispatchEventQueue->mHead;
            if (d == NULL) {
                if (mWebKitDispatchScheduled) {
                    mWebKitDispatchScheduled = FALSE;
                    if (!calledFromHandler) {
                        mWebKitHandler->RemoveMessages(
                                WebKitHandler::MSG_DISPATCH_WEBKIT_EVENTS);
                    }
                }
                return;
            }

            event = d->mEvent;
            if (event != NULL) {
                event->OffsetLocation(d->mWebKitXOffset, d->mWebKitYOffset);
                event->Scale(d->mWebKitScale);
                d->mFlags |= FLAG_WEBKIT_TRANSFORMED_EVENT;
            }

            eventType = d->mEventType;
            if (eventType == EVENT_TYPE_TOUCH) {
                event = mWebKitTouchStream->Update(event);
//                if (DEBUG && event == null && d.mEvent != null) {
//                    Log.d(TAG, "dispatchWebKitEvents: dropped event " + d.mEvent);
//                }
            }

            d->mFlags |= FLAG_WEBKIT_IN_PROGRESS;
            flags = d->mFlags;
        }

        // Handle the event.
        Boolean preventDefault;
        if (event == NULL) {
            preventDefault = FALSE;
        }
        else {
            preventDefault = DispatchWebKitEvent(event, eventType, flags);
        }

        {
            Mutex::Autolock lock(mLock);

            flags = d->mFlags;
            d->mFlags = flags & ~FLAG_WEBKIT_IN_PROGRESS;
            Boolean recycleEvent = event != d->mEvent;

            if ((flags & FLAG_WEBKIT_TIMEOUT) != 0) {
                // A timeout occurred!
                RecycleDispatchEventLocked(d);
            }
            else {
                // Web kit finished in a timely manner.  Dequeue the event.
                assert(mWebKitDispatchEventQueue->mHead == d);
                mWebKitDispatchEventQueue->Dequeue();

                UpdateWebKitTimeoutLocked();

                if ((flags & FLAG_PRIVATE) != 0) {
                    // Event was intended for web kit only.  All done.
                    RecycleDispatchEventLocked(d);
                }
                else if (preventDefault) {
                    // Web kit has decided to consume the event!
                    if (d->mEventType == EVENT_TYPE_TOUCH) {
                        EnqueueUiCancelTouchEventIfNeededLocked();
                        UnscheduleLongPressLocked();
                    }
                }
                else {
                    // Web kit is being friendly.  Pass the event to the UI.
                    EnqueueUiEventUnbatchedLocked(d);
                }
            }

            if (event != NULL && recycleEvent) {
                event->Recycle();
            }

            if (eventType == EVENT_TYPE_CLICK) {
                ScheduleHideTapHighlightLocked();
            }
        }
    }
}

// Runs on web kit thread.
Boolean WebViewInputDispatcher::DispatchWebKitEvent(
    /* [in] */ IMotionEvent* event,
    /* [in] */ Int32 eventType,
    /* [in] */ Int32 flags)
{
//    if (DEBUG) {
//        Log.d(TAG, "dispatchWebKitEvent: event=" + event
//                + ", eventType=" + eventType + ", flags=" + flags);
//    }

    Boolean preventDefault;
    mWebKitCallbacks->DispatchWebKitEvent(
            this, event, eventType, flags, &preventDefault);

//    if (DEBUG) {
//        Log.d(TAG, "dispatchWebKitEvent: preventDefault=" + preventDefault);
//    }

    return preventDefault;
}

Boolean WebViewInputDispatcher::IsMoveEventLocked(
    /* [in] */ WebViewInputDispatcher::DispatchEvent* d)
{
    Int32 mask;
    return d->mEvent != NULL
                && (d->mEvent->GetActionMasked(&mask), mask == IMotionEvent::ACTION_MOVE);
}

void WebViewInputDispatcher::DrainStaleWebKitEventsLocked()
{
    AutoPtr<DispatchEvent> d = mWebKitDispatchEventQueue->mHead;
    while (d != NULL && d->mNext != NULL
            && IsMoveEventLocked(d)
            && IsMoveEventLocked(d->mNext)) {
        AutoPtr<DispatchEvent> next = d->mNext;
        SkipWebKitEventLocked(d);
        d = next;
    }

    mWebKitDispatchEventQueue->mHead = d;
}

// Runs on UI thread in response to the web kit thread appearing to be unresponsive.
void WebViewInputDispatcher::HandleWebKitTimeout()
{
    Mutex::Autolock lock(mLock);
    if (!mWebKitTimeoutScheduled) {
        return;
    }

    mWebKitTimeoutScheduled = FALSE;

//    if (DEBUG) {
//        Log.d(TAG, "handleWebKitTimeout: timeout occurred!");
//    }

    // Drain the web kit event queue.
    AutoPtr<DispatchEvent> d = mWebKitDispatchEventQueue->DequeueList();

    // If web kit was processing an event (must be at the head of the list because
    // it can only do one at a time), then clone it or ignore it.
    if ((d->mFlags & FLAG_WEBKIT_IN_PROGRESS) != 0) {
        d->mFlags |= FLAG_WEBKIT_TIMEOUT;
        if ((d->mFlags & FLAG_PRIVATE) != 0) {
            d = d->mNext; // the event is private to web kit, ignore it
        }
        else {
            d = CopyDispatchEventLocked(d);
            d->mFlags &= ~FLAG_WEBKIT_IN_PROGRESS;
        }
    }

    // Enqueue all non-private events for handling by the UI thread.
    while (d != NULL) {
        AutoPtr<DispatchEvent> next = d->mNext;
        SkipWebKitEventLocked(d);
        d = next;
    }

    // Tell web kit to cancel all pending touches.
    // This also prevents us from sending web kit any more touches until the
    // next gesture begins.  (As required to ensure touch event stream consistency.)
    EnqueueWebKitCancelTouchEventIfNeededLocked();
}

void WebViewInputDispatcher::SkipWebKitEventLocked(
    /* [in] */ WebViewInputDispatcher::DispatchEvent* d)
{
    d->mNext = NULL;
    if ((d->mFlags & FLAG_PRIVATE) != 0) {
        RecycleDispatchEventLocked(d);
    }
    else {
        d->mFlags |= FLAG_WEBKIT_TIMEOUT;
        EnqueueUiEventUnbatchedLocked(d);
    }
}

void WebViewInputDispatcher::DispatchUiEvents(
    /* [in] */ Boolean calledFromHandler)
{
    for (;;) {
        AutoPtr<IMotionEvent> event;
        Int32 eventType;
        Int32 flags;
        {
            Mutex::Autolock lock(mLock);
            AutoPtr<DispatchEvent> d = mUiDispatchEventQueue->Dequeue();
            if (d == NULL) {
                if (mUiDispatchScheduled) {
                    mUiDispatchScheduled = FALSE;
                    if (!calledFromHandler) {
                        mUiHandler->RemoveMessages(UiHandler::MSG_DISPATCH_UI_EVENTS);
                    }
                }
                return;
            }

            event = d->mEvent;
            if (event != NULL && (d->mFlags & FLAG_WEBKIT_TRANSFORMED_EVENT) != 0) {
                event->Scale(1.0f / d->mWebKitScale);
                event->OffsetLocation(-d->mWebKitXOffset, -d->mWebKitYOffset);
                d->mFlags &= ~FLAG_WEBKIT_TRANSFORMED_EVENT;
            }

            eventType = d->mEventType;
            if (eventType == EVENT_TYPE_TOUCH) {
                event = mUiTouchStream->Update(event);
//                if (DEBUG && event == null && d.mEvent != null) {
//                    Log.d(TAG, "dispatchUiEvents: dropped event " + d.mEvent);
//                }
            }

            flags = d->mFlags;

            if (event == d->mEvent) {
                d->mEvent = NULL; // retain ownership of event, don't recycle it yet
            }

            RecycleDispatchEventLocked(d);

            if (eventType == EVENT_TYPE_CLICK) {
                ScheduleHideTapHighlightLocked();
            }
        }

        // Handle the event.
        if (event != NULL) {
            DispatchUiEvent(event, eventType, flags);
            event->Recycle();
        }
    }
}

// Runs on UI thread.
void WebViewInputDispatcher::DispatchUiEvent(
    /* [in] */ IMotionEvent* event,
    /* [in] */ Int32 eventType,
    /* [in] */ Int32 flags)
{
//    if (DEBUG) {
//        Log.d(TAG, "dispatchUiEvent: event=" + event
//                + ", eventType=" + eventType + ", flags=" + flags);
//    }

    mUiCallbacks->DispatchUiEvent(event, eventType, flags);
}

void WebViewInputDispatcher::EnqueueEventLocked(
    /* [in] */ WebViewInputDispatcher::DispatchEvent* d)
{
    if (!ShouldSkipWebKit(d)) {
        EnqueueWebKitEventLocked(d);
    }
    else {
        EnqueueUiEventLocked(d);
    }
}

Boolean WebViewInputDispatcher::ShouldSkipWebKit(
    /* [in] */ WebViewInputDispatcher::DispatchEvent* d)
{
    Int32 mask;
    switch (d->mEventType) {
        case EVENT_TYPE_CLICK:
        case EVENT_TYPE_HOVER:
        case EVENT_TYPE_SCROLL:
        case EVENT_TYPE_HIT_TEST:
            return FALSE;

        case EVENT_TYPE_TOUCH:
            // TODO: This should be cleaned up. We now have WebViewInputDispatcher
            // and WebViewClassic both checking for slop and doing their own
            // thing - they should be consolidated. And by consolidated, I mean
            // WebViewClassic's version should just be deleted.
            // The reason this is done is because webpages seem to expect
            // that they only get an ontouchmove if the slop has been exceeded.
            if (mIsTapCandidate && d->mEvent != NULL
                    && (d->mEvent->GetActionMasked(&mask), mask == IMotionEvent::ACTION_MOVE)) {
                return TRUE;
            }

            return !mPostSendTouchEventsToWebKit
                    || mPostDoNotSendTouchEventsToWebKitUntilNextGesture;
    }

    return TRUE;
}

void WebViewInputDispatcher::EnqueueWebKitCancelTouchEventIfNeededLocked()
{
    // We want to cancel touch events that were delivered to web kit.
    // Enqueue a null event at the end of the queue if needed.
    if (mWebKitTouchStream->IsCancelNeeded() || !mWebKitDispatchEventQueue->IsEmpty()) {
        AutoPtr<DispatchEvent> d = ObtainDispatchEventLocked(NULL, EVENT_TYPE_TOUCH, FLAG_PRIVATE,
                0, 0, 1.0f);
        EnqueueWebKitEventUnbatchedLocked(d);
        mPostDoNotSendTouchEventsToWebKitUntilNextGesture = TRUE;
    }
}

void WebViewInputDispatcher::EnqueueWebKitEventLocked(
    /* [in] */ WebViewInputDispatcher::DispatchEvent* d)
{
    if (BatchEventLocked(d, mWebKitDispatchEventQueue->mTail)) {
//        if (DEBUG) {
//            Log.d(TAG, "enqueueWebKitEventLocked: batched event " + d.mEvent);
//        }
        RecycleDispatchEventLocked(d);
    }
    else {
        EnqueueWebKitEventUnbatchedLocked(d);
    }
}

void WebViewInputDispatcher::EnqueueWebKitEventUnbatchedLocked(
    /* [in] */ WebViewInputDispatcher::DispatchEvent* d)
{
//    if (DEBUG) {
//        Log.d(TAG, "enqueueWebKitEventUnbatchedLocked: enqueued event " + d.mEvent);
//    }

    mWebKitDispatchEventQueue->Enqueue(d);
    ScheduleWebKitDispatchLocked();
    UpdateWebKitTimeoutLocked();
}

void WebViewInputDispatcher::ScheduleWebKitDispatchLocked()
{
    if (!mWebKitDispatchScheduled) {
        Boolean bResult = FALSE;
        mWebKitHandler->SendEmptyMessage(WebKitHandler::MSG_DISPATCH_WEBKIT_EVENTS, &bResult);
        mWebKitDispatchScheduled = TRUE;
    }
}

void WebViewInputDispatcher::UpdateWebKitTimeoutLocked()
{
    AutoPtr<DispatchEvent> d = mWebKitDispatchEventQueue->mHead;
    if (d != NULL && mWebKitTimeoutScheduled && mWebKitTimeoutTime == d->mTimeoutTime) {
        return;
    }

    if (mWebKitTimeoutScheduled) {
        mUiHandler->RemoveMessages(UiHandler::MSG_WEBKIT_TIMEOUT);
        mWebKitTimeoutScheduled = FALSE;
    }

    if (d != NULL) {
        Boolean bResult = FALSE;
        mUiHandler->SendEmptyMessageAtTime(UiHandler::MSG_WEBKIT_TIMEOUT, d->mTimeoutTime, &bResult);
        mWebKitTimeoutScheduled = TRUE;
        mWebKitTimeoutTime = d->mTimeoutTime;
    }
}

void WebViewInputDispatcher::EnqueueUiCancelTouchEventIfNeededLocked()
{
    // We want to cancel touch events that were delivered to the UI.
    // Enqueue a null event at the end of the queue if needed.
    if (mUiTouchStream->IsCancelNeeded() || !mUiDispatchEventQueue->IsEmpty()) {
        AutoPtr<DispatchEvent> d = ObtainDispatchEventLocked(NULL, EVENT_TYPE_TOUCH, FLAG_PRIVATE,
                0, 0, 1.0f);
        EnqueueUiEventUnbatchedLocked(d);
    }
}

void WebViewInputDispatcher::EnqueueUiEventLocked(
    /* [in] */ WebViewInputDispatcher::DispatchEvent* d)
{
    if (BatchEventLocked(d, mUiDispatchEventQueue->mTail)) {
//        if (DEBUG) {
//            Log.d(TAG, "enqueueUiEventLocked: batched event " + d.mEvent);
//        }
        RecycleDispatchEventLocked(d);
    }
    else {
        EnqueueUiEventUnbatchedLocked(d);
    }
}

void WebViewInputDispatcher::EnqueueUiEventUnbatchedLocked(
    /* [in] */ WebViewInputDispatcher::DispatchEvent* d)
{
//    if (DEBUG) {
//        Log.d(TAG, "enqueueUiEventUnbatchedLocked: enqueued event " + d.mEvent);
//    }

    mUiDispatchEventQueue->Enqueue(d);
    ScheduleUiDispatchLocked();
}

void WebViewInputDispatcher::ScheduleUiDispatchLocked()
{
    if (!mUiDispatchScheduled) {
        Boolean bResult = FALSE;
        mUiHandler->SendEmptyMessage(UiHandler::MSG_DISPATCH_UI_EVENTS, &bResult);
        mUiDispatchScheduled = TRUE;
    }
}

Boolean WebViewInputDispatcher::BatchEventLocked(
    /* [in] */ WebViewInputDispatcher::DispatchEvent* in,
    /* [in] */ WebViewInputDispatcher::DispatchEvent* tail)
{
    if (!ENABLE_EVENT_BATCHING) {
        return FALSE;
    }

    if (tail != NULL && tail->mEvent != NULL && in->mEvent != NULL
            && in->mEventType == tail->mEventType
            && in->mFlags == tail->mFlags
            && in->mWebKitXOffset == tail->mWebKitXOffset
            && in->mWebKitYOffset == tail->mWebKitYOffset
            && in->mWebKitScale == tail->mWebKitScale) {

        Boolean b = FALSE;
        tail->mEvent->AddBatch(in->mEvent, &b);
        return b;
    }

    return FALSE;
}

AutoPtr<WebViewInputDispatcher::DispatchEvent> WebViewInputDispatcher::ObtainDispatchEventLocked(
    /* [in] */ IMotionEvent* event,
    /* [in] */ Int32 eventType,
    /* [in] */ Int32 flags,
    /* [in] */ Int32 webKitXOffset,
    /* [in] */ Int32 webKitYOffset,
    /* [in] */ Float webKitScale)
{
    AutoPtr<DispatchEvent> d = ObtainUninitializedDispatchEventLocked();
    d->mEvent = event;
    d->mEventType = eventType;
    d->mFlags = flags;
    d->mTimeoutTime = SystemClock::GetUptimeMillis() + WEBKIT_TIMEOUT_MILLIS;
    d->mWebKitXOffset = webKitXOffset;
    d->mWebKitYOffset = webKitYOffset;
    d->mWebKitScale = webKitScale;

//    if (DEBUG) {
//        Log.d(TAG, "Timeout time: " + (d.mTimeoutTime - SystemClock.uptimeMillis()));
//    }

    return d;
}

AutoPtr<WebViewInputDispatcher::DispatchEvent> WebViewInputDispatcher::CopyDispatchEventLocked(
    /* [in] */ WebViewInputDispatcher::DispatchEvent* d)
{
    AutoPtr<DispatchEvent> copy = ObtainUninitializedDispatchEventLocked();

    if (d->mEvent != NULL) {
        d->mEvent->Copy((IInputEvent**)&(copy->mEvent));
    }

    copy->mEventType = d->mEventType;
    copy->mFlags = d->mFlags;
    copy->mTimeoutTime = d->mTimeoutTime;
    copy->mWebKitXOffset = d->mWebKitXOffset;
    copy->mWebKitYOffset = d->mWebKitYOffset;
    copy->mWebKitScale = d->mWebKitScale;
    copy->mNext = d->mNext;

    return copy;
}

AutoPtr<WebViewInputDispatcher::DispatchEvent> WebViewInputDispatcher::ObtainUninitializedDispatchEventLocked()
{
    AutoPtr<DispatchEvent> d = mDispatchEventPool;
    if (d != NULL) {
        mDispatchEventPoolSize -= 1;
        mDispatchEventPool = d->mNext;
        d->mNext = NULL;
    }
    else {
        d = new DispatchEvent();
    }

    return d;
}

void WebViewInputDispatcher::RecycleDispatchEventLocked(
    /* [in] */ WebViewInputDispatcher::DispatchEvent* d)
{
    if (d->mEvent != NULL) {
        d->mEvent->Recycle();
        d->mEvent = NULL;
    }

    if (mDispatchEventPoolSize < MAX_DISPATCH_EVENT_POOL_SIZE) {
        mDispatchEventPoolSize += 1;
        d->mNext = mDispatchEventPool;
        mDispatchEventPool = d;
    }
}

} // namespace Webkit
} // namepsace Droid
} // namespace Elastos
