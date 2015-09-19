
namespace Elastos {
namespace Droid {
namespace Webkit {
namespace AndroidWebview {

//===============================================================
//          ScrollAccessibilityHelper::HandlerCallback
//===============================================================

const Int32 ScrollAccessibilityHelper::HandlerCallback::MSG_VIEW_SCROLLED = 1;

ScrollAccessibilityHelper::HandlerCallback::HandlerCallback(
    /* [in] */ ScrollAccessibilityHelper* owner,
    /* [in] */ IView* eventSender)
    : mOwner(owner)
    , mEventSender(eventSender)
{
}

//@Override
ECode ScrollAccessibilityHelper::HandlerCallback::HandleMessage(
    /* [in] */ IMessage* msg,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(msg);
    VALIDATE_NOT_NULL(result);

    Int32 what;
    msg->GetWhat(&what);
    switch(what) {
        case MSG_VIEW_SCROLLED:
            mMsgViewScrolledQueued = FALSE;
            mEventSender->SendAccessibilityEvent(IAccessibilityEvent::TYPE_VIEW_SCROLLED);
            break;
        default:
            assert(0);
            // throw new IllegalStateException(
            //         "AccessibilityInjector: unhandled message: " + msg.what);
    }
    return TRUE;
}

//===============================================================
//                 ScrollAccessibilityHelper
//===============================================================

const Int64 ScrollAccessibilityHelper::SEND_RECURRING_ACCESSIBILITY_EVENTS_INTERVAL_MILLIS;

ScrollAccessibilityHelper::ScrollAccessibilityHelper(
    /* [in] */ IView* eventSender)
    : mMsgViewScrolledQueued(FALSE)
{
    AutoPtr<IHandlerCallback> callback = new HandlerCallback(this, eventSender);
    CHandler::New(callback, (IHandler**)&mHandler);
}

/**
 * Post a callback to send a {@link AccessibilityEvent#TYPE_VIEW_SCROLLED} event.
 * This event is sent at most once every
 * {@link android.view.ViewConfiguration#getSendRecurringAccessibilityEventsInterval()}
 */
void ScrollAccessibilityHelper::PostViewScrolledAccessibilityEventCallback()
{
    if (mMsgViewScrolledQueued)
        return;

    mMsgViewScrolledQueued = TRUE;

    AutoPtr<IMessage> msg;
    mHandler->ObtainMessage(HandlerCallback::MSG_VIEW_SCROLLED, (IMessage**)&msg);
    mHandler->SendMessageDelayed(msg, SEND_RECURRING_ACCESSIBILITY_EVENTS_INTERVAL_MILLIS);
}

void ScrollAccessibilityHelper::RemovePostedViewScrolledAccessibilityEventCallback()
{
    if (!mMsgViewScrolledQueued)
        return;
    mMsgViewScrolledQueued = FALSE;

    mHandler->RemoveMessages(HandlerCallback::MSG_VIEW_SCROLLED);
}

void ScrollAccessibilityHelper::RemovePostedCallbacks()
{
    RemovePostedViewScrolledAccessibilityEventCallback();
}

} // namespace AndroidWebview
} // namespace Webkit
} // namespace Droid
} // namespace Elastos
