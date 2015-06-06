
namespace Elastos {
namespace Droid {
namespace Webkit {
namespace Base {

const Int32 SystemMessageHandler::SCHEDULED_WORK;
const Int32 SystemMessageHandler::DELAYED_SCHEDULED_WORK;

SystemMessageHandler::SystemMessageHandler(
    /* [in] */ Int64 messagePumpDelegateNative)
    : mMessagePumpDelegateNative(0)
    , mDelayedScheduledTimeTicks(0)
    , mMessagePumpDelegateNative(messagePumpDelegateNative)
{
}

//@Override
ECode SystemMessageHandler::HandleMessage(
    /* [in] */ IMessage* msg)
{
    Int64 what;
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
    SendEmptyMessage(SCHEDULED_WORK);
}

//@SuppressWarnings("unused")
//@CalledByNative
void SystemMessageHandler::ScheduleDelayedWork(
    /* [in] */ Int64 delayedTimeTicks,
    /* [in] */ Int64 millis)
{
    if (mDelayedScheduledTimeTicks != 0) {
        RemoveMessages(DELAYED_SCHEDULED_WORK);
    }
    mDelayedScheduledTimeTicks = delayedTimeTicks;
    SendEmptyMessageDelayed(DELAYED_SCHEDULED_WORK, millis);
}

//@SuppressWarnings("unused")
//@CalledByNative
void SystemMessageHandler::RemoveAllPendingMessages()
{
    RemoveMessages(SCHEDULED_WORK);
    RemoveMessages(DELAYED_SCHEDULED_WORK);
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
