
#include "webkit/native/base/SystemMessageHandler.h"

namespace Elastos {
namespace Droid {
namespace Webkit {
namespace Base {

const Int32 SystemMessageHandler::SCHEDULED_WORK;
const Int32 SystemMessageHandler::DELAYED_SCHEDULED_WORK;

SystemMessageHandler::SystemMessageHandler(
    /* [in] */ Int64 messagePumpDelegateNative)
    : mDelayedScheduledTimeTicks(0)
    , mMessagePumpDelegateNative(messagePumpDelegateNative)
{
}

//@Override
ECode SystemMessageHandler::HandleMessage(
    /* [in] */ IMessage* msg)
{
    VALIDATE_NOT_NULL(msg);
    Int32 what;
    msg->GetWhat(&what);
    if (what == DELAYED_SCHEDULED_WORK) {
        mDelayedScheduledTimeTicks = 0;
    }
    NativeDoRunLoopOnce(mMessagePumpDelegateNative, mDelayedScheduledTimeTicks);

    return NOERROR;
}

//@SuppressWarnings("unused")
//@CalledByNative
void SystemMessageHandler::ScheduleWork()
{
    assert(0);
//    SendEmptyMessage(SCHEDULED_WORK);
}

//@SuppressWarnings("unused")
//@CalledByNative
void SystemMessageHandler::ScheduleDelayedWork(
    /* [in] */ Int64 delayedTimeTicks,
    /* [in] */ Int64 millis)
{
    assert(0);
#if 0
    if (mDelayedScheduledTimeTicks != 0) {
        RemoveMessages(DELAYED_SCHEDULED_WORK);
    }
    mDelayedScheduledTimeTicks = delayedTimeTicks;
    SendEmptyMessageDelayed(DELAYED_SCHEDULED_WORK, millis);
#endif
}

//@SuppressWarnings("unused")
//@CalledByNative
void SystemMessageHandler::RemoveAllPendingMessages()
{
    assert(0);
#if 0
    RemoveMessages(SCHEDULED_WORK);
    RemoveMessages(DELAYED_SCHEDULED_WORK);
#endif
}

//@CalledByNative
AutoPtr<SystemMessageHandler> SystemMessageHandler::Create(
    /* [in] */ Int64 messagePumpDelegateNative)
{
    AutoPtr<SystemMessageHandler> handler = new SystemMessageHandler(messagePumpDelegateNative);
    return handler;
}

void SystemMessageHandler::NativeDoRunLoopOnce(
    /* [in] */ Int64 messagePumpDelegateNative,
    /* [in] */ Int64 delayedScheduledTimeTicks)
{
}

} // namespace Base
} // namespace Webkit
} // namespace Droid
} // namespace Elastos
