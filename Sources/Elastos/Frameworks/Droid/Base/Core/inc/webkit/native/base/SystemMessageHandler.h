
#ifndef __SYSTEMMESSAGEHANDLER_H__
#define __SYSTEMMESSAGEHANDLER_H__

// import android.os.Handler;
// import android.os.Message;

namespace Elastos {
namespace Droid {
namespace Webkit {
namespace Base {

class SystemMessageHandler
    : public Object
    , public IHandler
{
public:
    //@Override
    ECode HandleMessage(
        /* [in] */ IMessage* msg);

private:
    SystemMessageHandler(
        /* [in] */ Int64 messagePumpDelegateNative);

    //@SuppressWarnings("unused")
    //@CalledByNative
    CARAPI_(void) ScheduleWork();

    //@SuppressWarnings("unused")
    //@CalledByNative
    CARAPI_(void) ScheduleDelayedWork(
        /* [in] */ Int64 delayedTimeTicks,
        /* [in] */ Int64 millis);

    //@SuppressWarnings("unused")
    //@CalledByNative
    CARAPI_(void) RemoveAllPendingMessages();

    //@CalledByNative
    static CARAPI_(AutoPtr<SystemMessageHandler>) Create(
        /* [in] */ Int64 messagePumpDelegateNative);

    CARAPI_(void) NativeDoRunLoopOnce(
        /* [in] */ Int64 messagePumpDelegateNative,
        /* [in] */ Int64 delayedScheduledTimeTicks);

private:
    static const Int32 SCHEDULED_WORK = 1;
    static const Int32 DELAYED_SCHEDULED_WORK = 2;

    // Native class pointer set by the constructor of the SharedClient native class.
    Int64 mMessagePumpDelegateNative;
    Int64 mDelayedScheduledTimeTicks;
};

} // namespace Base
} // namespace Webkit
} // namespace Droid
} // namespace Elastos

#endif//__SYSTEMMESSAGEHANDLER_H__
