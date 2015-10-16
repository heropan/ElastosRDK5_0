#include "elastos/droid/webkit/native/android_webview/ScrollAccessibilityHelper.h"
#include "elastos/droid/os/CHandler.h"
#include "elastos/utility/logging/Logger.h"

using Elastos::Droid::Os::CHandler;
using Elastos::Droid::Os::EIID_IHandlerCallback;
using Elastos::Droid::View::Accessibility::IAccessibilityEvent;

using Elastos::Utility::Logging::Logger;

namespace Elastos {
namespace Droid {
namespace Webkit {
namespace AndroidWebview {

//===============================================================
//          ScrollAccessibilityHelper::HandlerCallback
//===============================================================
CAR_INTERFACE_IMPL(ScrollAccessibilityHelper::HandlerCallback, Object, IHandlerCallback);

const Int32 ScrollAccessibilityHelper::HandlerCallback::MSG_VIEW_SCROLLED;

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
            mOwner->mMsgViewScrolledQueued = FALSE;
            mEventSender->SendAccessibilityEvent(IAccessibilityEvent::TYPE_VIEW_SCROLLED);
            break;
        default:
            Logger::W("ScrollAccessibilityHelper", "HandlerCallback::HandleMessage");
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
    CHandler::New(callback, FALSE, (IHandler**)&mHandler);
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
    Boolean result;
    mHandler->SendMessageDelayed(msg, SEND_RECURRING_ACCESSIBILITY_EVENTS_INTERVAL_MILLIS, &result);
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
