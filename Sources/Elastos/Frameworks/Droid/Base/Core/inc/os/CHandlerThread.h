
#ifndef __CHANDLERTHREAD_H__
#define __CHANDLERTHREAD_H__

#include "_CHandlerThread.h"
#include <elastos/Thread.h>

using Elastos::Core::IClassLoader;
using Elastos::Core::IRunnable;
using Elastos::Core::Threading::Thread;

namespace Elastos {
namespace Droid {
namespace Os {

/**
 * Handy class for starting a new thread that has a looper. The looper can then be
 * used to create handler classes. Note that start() must still be called.
 */
CarClass(CHandlerThread), public Thread
{
public:
    CARAPI constructor(
        /* [in] */ const String& name);

    /**
     * Constructs a HandlerThread.
     * @param name
     * @param priority The priority to run the thread at. The value supplied must be from
     * {@link android.os.Process} and not from java.lang.Thread.
     */
    CARAPI constructor(
        /* [in] */ const String& name,
        /* [in] */ Int32 priority);

    CARAPI_(PInterface) Probe(
        /* [in]  */ REIID riid);

    CARAPI CheckAccess();

    CARAPI CountStackFrames(
        /* [out] */ Int32* number);

    CARAPI Destroy();

    CARAPI GetId(
        /* [out] */ Int64* id);

    CARAPI GetName(
        /* [out] */ String* name);

    CARAPI GetPriority(
        /* [out] */ Int32* priority);

    CARAPI GetState(
        /* [out] */ ThreadState* state);

    CARAPI GetThreadGroup(
        /* [out] */ IThreadGroup** group);

    CARAPI Interrupt();

    CARAPI IsAlive(
        /* [out] */ Boolean* isAlive);

    CARAPI IsDaemon(
        /* [out] */ Boolean* isDaemon);

    CARAPI IsInterrupted(
        /* [out] */ Boolean* isInterrupted);

    CARAPI Join();

    CARAPI JoinEx(
        /* [in] */ Int64 millis);

    CARAPI JoinEx2(
        /* [in] */ Int64 millis,
        /* [in] */ Int32 nanos);

    CARAPI Resume();

    CARAPI SetDaemon(
        /* [in] */ Boolean isDaemon);

    CARAPI SetName(
        /* [in] */ const String& threadName);

    CARAPI SetPriority(
        /* [in] */ Int32 priority);

    CARAPI Start();

    CARAPI Stop();

    CARAPI Suspend();

    CARAPI Detach();

    CARAPI Unpark();

    CARAPI ParkFor(
        /* [in] */ Int64 nanos);

    CARAPI ParkUntil(
        /* [in] */ Int64 time);

    CARAPI Run();

    CARAPI Lock();

    CARAPI Unlock();

    CARAPI Notify();

    CARAPI NotifyAll();

    CARAPI Wait();

    CARAPI WaitEx(
        /* [in] */ Int64 millis);

    CARAPI WaitEx2(
        /* [in] */ Int64 millis,
        /* [in] */ Int32 nanos);

    CARAPI GetLooper(
        /* [out] */ ILooper** looper);

    CARAPI Quit(
        /* [out] */ Boolean* result);

    CARAPI GetThreadId(
        /* [out] */ Int32* tid);

    CARAPI GetUncaughtExceptionHandler(
        /* [out] */ IThreadUncaughtExceptionHandler** handler);

    CARAPI GetContextClassLoader(
        /* [out] */ IClassLoader** outload);

    CARAPI SetContextClassLoader(
        /* [in] */ IClassLoader* cl);

    CARAPI PushInterruptAction(
        /* [in] */ IRunnable* interruptAction);

    CARAPI PopInterruptAction(
        /* [in] */ IRunnable* interruptAction);

    CARAPI SetUncaughtExceptionHandler(
        /* [in] */ IThreadUncaughtExceptionHandler* handler);

protected:
    /**
     * Call back method that can be explicitly overridden if needed to execute some
     * setup before Looper loops.
     */
    CARAPI_(void) OnLooperPrepared();

private:
    CARAPI_(Mutex*) GetSelfLock();

public:
    Int32 mPriority;
    Int32 mTid;
    AutoPtr<ILooper> mLooper;
};

} // namespace Os
} // namespace Droid
} // namespace Elastos

#endif //__CHANDLERTHREAD_H__
