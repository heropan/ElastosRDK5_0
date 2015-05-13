
#include "ext/frameworkdef.h"
#include "os/CHandlerThread.h"
#include "os/Process.h"
#include "os/Looper.h"

namespace Elastos {
namespace Droid {
namespace Os {

ECode CHandlerThread::constructor(
    /* [in] */ const String& name)
{
    Thread::Init(name);
    mPriority = IProcess::THREAD_PRIORITY_DEFAULT;
    return NOERROR;
}

/**
 * Constructs a HandlerThread.
 * @param name
 * @param priority The priority to run the thread at. The value supplied must be from
 * {@link android.os.Process} and not from java.lang.Thread.
 */
ECode CHandlerThread::constructor(
    /* [in] */ const String& name,
    /* [in] */ Int32 priority)
{
    Thread::Init(name);
    mPriority = priority;
    return NOERROR;
}

PInterface CHandlerThread::Probe(
    /* [in]  */ REIID riid)
{
    return _CHandlerThread::Probe(riid);
}

ECode CHandlerThread::CheckAccess()
{
    return Thread::CheckAccess();
}

ECode CHandlerThread::CountStackFrames(
    /* [out] */ Int32* number)
{
    return Thread::CountStackFrames(number);
}

ECode CHandlerThread::Destroy()
{
    return Thread::Destroy();
}

ECode CHandlerThread::GetId(
    /* [out] */ Int64* id)
{
    return Thread::GetId(id);
}

ECode CHandlerThread::GetName(
    /* [out] */ String* name)
{
    return Thread::GetName(name);
}

ECode CHandlerThread::GetPriority(
    /* [out] */ Int32* priority)
{
    return Thread::GetPriority(priority);
}

ECode CHandlerThread::GetState(
    /* [out] */ ThreadState* state)
{
    return Thread::GetState(state);
}

ECode CHandlerThread::GetThreadGroup(
    /* [out] */ IThreadGroup** group)
{
    return Thread::GetThreadGroup(group);
}

ECode CHandlerThread::Interrupt()
{
    return Thread::Interrupt();
}

ECode CHandlerThread::IsAlive(
    /* [out] */ Boolean* isAlive)
{
    return Thread::IsAlive(isAlive);
}

ECode CHandlerThread::IsDaemon(
    /* [out] */ Boolean* isDaemon)
{
    return Thread::IsDaemon(isDaemon);
}

ECode CHandlerThread::IsInterrupted(
    /* [out] */ Boolean* isInterrupted)
{
    return Thread::IsInterrupted(isInterrupted);
}

ECode CHandlerThread::Join()
{
    return Thread::Join();
}

ECode CHandlerThread::JoinEx(
    /* [in] */ Int64 millis)
{
    return Thread::JoinEx(millis);
}

ECode CHandlerThread::JoinEx2(
    /* [in] */ Int64 millis,
    /* [in] */ Int32 nanos)
{
    return Thread::JoinEx2(millis, nanos);
}

ECode CHandlerThread::Resume()
{
    return Thread::Resume();
}

ECode CHandlerThread::SetDaemon(
    /* [in] */ Boolean isDaemon)
{
    return Thread::SetDaemon(isDaemon);
}

ECode CHandlerThread::SetName(
    /* [in] */ const String& threadName)
{
    return Thread::SetName(threadName);
}

ECode CHandlerThread::SetPriority(
    /* [in] */ Int32 priority)
{
    return Thread::SetPriority(priority);
}

ECode CHandlerThread::Start()
{
    return Thread::Start();
}

ECode CHandlerThread::Stop()
{
    return Thread::Stop();
}

ECode CHandlerThread::Suspend()
{
    return Thread::Suspend();
}

ECode CHandlerThread::Detach()
{
    return Thread::Detach();
}

ECode CHandlerThread::Unpark()
{
    return Thread::Unpark();
}

ECode CHandlerThread::ParkFor(
    /* [in] */ Int64 nanos)
{
    return Thread::ParkFor(nanos);
}

ECode CHandlerThread::ParkUntil(
    /* [in] */ Int64 time)
{
    return Thread::ParkUntil(time);
}

ECode CHandlerThread::Lock()
{
    return Thread::Lock();
}

ECode CHandlerThread::Unlock()
{
    return Thread::Unlock();
}

ECode CHandlerThread::Notify()
{
    return Thread::Notify();
}

ECode CHandlerThread::NotifyAll()
{
    return Thread::NotifyAll();
}

ECode CHandlerThread::Wait()
{
    return Thread::Wait();
}

ECode CHandlerThread::WaitEx(
    /* [in] */ Int64 millis)
{
    return Thread::Wait(millis);
}

ECode CHandlerThread::WaitEx2(
    /* [in] */ Int64 millis,
    /* [in] */ Int32 nanos)
{
    return Thread::Wait(millis, nanos);
}

void CHandlerThread::OnLooperPrepared()
{}

ECode CHandlerThread::Run()
{
    mTid = Process::MyTid();
    Looper::Prepare();
    {
        Lock();

        mLooper = Looper::MyLooper();
        NotifyAll();

        Unlock();
    }
    Process::SetThreadPriority(mPriority);
    OnLooperPrepared();
    Looper::Loop();
    mTid = -1;
    return NOERROR;
}

ECode CHandlerThread::GetLooper(
    /* [out] */ ILooper** looper)
{
    VALIDATE_NOT_NULL(looper);

    Boolean isAlive;
    if (IsAlive(&isAlive), !isAlive) {
        *looper = NULL;
        return NOERROR;
    }

    // If the thread has been started, wait until the looper has been created.
    {
        Lock();

        while ((IsAlive(&isAlive), isAlive) && mLooper == NULL) {
            // try {
            Wait();
            // } catch (InterruptedException e) {
            // }
        }

        Unlock();
    }
    *looper = mLooper;
    INTERFACE_ADDREF(*looper);
    return NOERROR;
}

ECode CHandlerThread::Quit(
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);

    AutoPtr<ILooper> looper;
    FAIL_RETURN(GetLooper((ILooper**)&looper));
    if (looper != NULL) {
        looper->Quit();
        *result = TRUE;
        return NOERROR;
    }
    *result = FALSE;
    return NOERROR;
}

ECode CHandlerThread::GetThreadId(
    /* [out] */ Int32* tid)
{
    VALIDATE_NOT_NULL(tid);

    *tid = mTid;
    return NOERROR;
}

Mutex* CHandlerThread::GetSelfLock()
{
    return &_m_syncLock;
}

ECode CHandlerThread::GetUncaughtExceptionHandler(
    /* [out] */ IThreadUncaughtExceptionHandler** handler)
{
    return Thread::GetUncaughtExceptionHandler(handler);
}

ECode CHandlerThread::GetContextClassLoader(
    /* [out] */ IClassLoader** outload)
{
    return Thread::GetContextClassLoader(outload);
}

ECode CHandlerThread::SetContextClassLoader(
    /* [in] */ IClassLoader* cl)
{
    return Thread::SetContextClassLoader(cl);
}

ECode CHandlerThread::PushInterruptAction(
    /* [in] */ IRunnable* interruptAction)
{
    return Thread::PushInterruptAction(interruptAction);
}

ECode CHandlerThread::PopInterruptAction(
    /* [in] */ IRunnable* interruptAction)
{
    return Thread::PopInterruptAction(interruptAction);
}

ECode CHandlerThread::SetUncaughtExceptionHandler(
    /* [in] */ IThreadUncaughtExceptionHandler* handler)
{
    return Thread::SetUncaughtExceptionHandler(handler);
}

} // namespace Os
} // namespace Droid
} // namespace Elastos
