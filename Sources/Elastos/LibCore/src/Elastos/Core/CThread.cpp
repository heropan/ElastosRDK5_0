
#include "CThread.h"

namespace Elastos {
namespace Core {
namespace Threading {

ECode CThread::constructor()
{
    return Thread::Init();
}

ECode CThread::constructor(
    /* [in] */ IRunnable* runnable)
{
    return Thread::Init(runnable);
}

ECode CThread::constructor(
    /* [in] */ IRunnable* runnable,
    /* [in] */ const String& threadName)
{
    return Thread::Init(runnable, threadName);
}

ECode CThread::constructor(
    /* [in] */ const String& threadName)
{
    return Thread::Init(threadName);
}

ECode CThread::constructor(
    /* [in] */ IThreadGroup* group,
    /* [in] */ IRunnable* runnable)
{
    return Thread::Init(group, runnable);
}

ECode CThread::constructor(
    /* [in] */ IThreadGroup* group,
    /* [in] */ IRunnable* runnable,
    /* [in] */ const String& threadName)
{
    return Thread::Init(group, runnable, threadName);
}

ECode CThread::constructor(
    /* [in] */ IThreadGroup* group,
    /* [in] */ const String& threadName)
{
    return Thread::Init(group, threadName);
}

ECode CThread::constructor(
    /* [in] */ IThreadGroup* group,
    /* [in] */ IRunnable* runnable,
    /* [in] */ const String& threadName,
    /* [in] */ Int64 stackSize)
{
    return Thread::Init(group, runnable, threadName, stackSize);
}

ECode CThread::constructor(
    /* [in] */ IThreadGroup* group,
    /* [in] */ const String& name,
    /* [in] */ Int32 priority,
    /* [in] */ Boolean daemon)
{
    return Thread::Init(group, name, priority, daemon);
}

PInterface CThread::Probe(
    /* [in] */ REIID riid)
{
    if (riid == EIID_Thread) {
        return reinterpret_cast<PInterface>((Thread*)this);
    }
    return _CThread::Probe(riid);
}

ECode CThread::CheckAccess()
{
    return Thread::CheckAccess();
}

ECode CThread::CountStackFrames(
    /* [out] */ Int32* number)
{
    return Thread::CountStackFrames(number);
}

ECode CThread::Destroy()
{
    return Thread::Destroy();
}

ECode CThread::GetId(
    /* [out] */ Int64* id)
{
    return Thread::GetId(id);
}

ECode CThread::GetName(
    /* [out] */ String* name)
{
    return Thread::GetName(name);
}

ECode CThread::GetPriority(
    /* [out] */ Int32* priority)
{
    return Thread::GetPriority(priority);
}

ECode CThread::GetState(
    /* [out] */ ThreadState* state)
{
    return Thread::GetState(state);
}

ECode CThread::GetThreadGroup(
    /* [out] */ IThreadGroup** group)
{
    return Thread::GetThreadGroup(group);
}

ECode CThread::GetUncaughtExceptionHandler(
    /* [out] */ IThreadUncaughtExceptionHandler** handler)
{
    return Thread::GetUncaughtExceptionHandler(handler);
}

ECode CThread::Interrupt()
{
    return Thread::Interrupt();
}

ECode CThread::IsAlive(
    /* [out] */ Boolean* isAlive)
{
    return Thread::IsAlive(isAlive);
}

ECode CThread::IsDaemon(
    /* [out] */ Boolean* isDaemon)
{
    return Thread::IsDaemon(isDaemon);
}

ECode CThread::IsInterrupted(
    /* [out] */ Boolean* isInterrupted)
{
    return Thread::IsInterrupted(isInterrupted);
}

ECode CThread::Join()
{
    return Thread::Join();
}

ECode CThread::JoinEx(
    /* [in] */ Int64 millis)
{
    return Thread::JoinEx(millis);
}

ECode CThread::JoinEx2(
    /* [in] */ Int64 millis,
    /* [in] */ Int32 nanos)
{
    return Thread::JoinEx2(millis, nanos);
}

ECode CThread::Resume()
{
    return Thread::Resume();
}

ECode CThread::SetDaemon(
    /* [in] */ Boolean isDaemon)
{
    return Thread::SetDaemon(isDaemon);
}

ECode CThread::PushInterruptAction(
    /* [in] */ IRunnable* interruptAction)
{
    return Thread::PushInterruptAction(interruptAction);
}

ECode CThread::PopInterruptAction(
    /* [in] */ IRunnable* interruptAction)
{
    return Thread::PopInterruptAction(interruptAction);
}

ECode CThread::SetName(
    /* [in] */ const String& threadName)
{
    return Thread::SetName(threadName);
}

ECode CThread::SetPriority(
    /* [in] */ Int32 priority)
{
    return Thread::SetPriority(priority);
}

ECode CThread::SetUncaughtExceptionHandler(
    /* [in] */ IThreadUncaughtExceptionHandler* handler)
{
    return Thread::SetUncaughtExceptionHandler(handler);
}

ECode CThread::Start()
{
    return Thread::Start();
}

ECode CThread::Stop()
{
    return Thread::Stop();
}

ECode CThread::Suspend()
{
    return Thread::Suspend();
}

ECode CThread::Unpark()
{
    return Thread::Unpark();
}

ECode CThread::ParkFor(
    /* [in] */ Int64 nanos)
{
    return Thread::ParkFor(nanos);
}

ECode CThread::ParkUntil(
    /* [in] */ Int64 time)
{
    return Thread::ParkUntil(time);
}

ECode CThread::Run()
{
    return Thread::Run();
}

ECode CThread::Detach()
{
    return Thread::Detach();
}

ECode CThread::Lock()
{
    return Thread::Lock();
}

ECode CThread::Unlock()
{
    return Thread::Unlock();
}

ECode CThread::Notify()
{
    return Thread::Notify();
}

ECode CThread::NotifyAll()
{
    return Thread::NotifyAll();
}

ECode CThread::Wait()
{
    return Thread::Wait();
}

ECode CThread::WaitEx(
    /* [in] */ Int64 millis)
{
    return Thread::Wait(millis);
}

ECode CThread::WaitEx2(
    /* [in] */ Int64 millis,
    /* [in] */ Int32 nanos)
{
    return Thread::Wait(millis, nanos);
}

Mutex* CThread::GetSelfLock()
{
    return &_m_syncLock;
}

ECode CThread::GetContextClassLoader(
    /* [out] */ IClassLoader** outload)
{
    return Thread::GetContextClassLoader(outload);
}

ECode CThread::SetContextClassLoader(
    /* [in] */ IClassLoader* cl)
{
    return Thread::SetContextClassLoader(cl);
}

} // namespace Threading
} // namespace Core
} // namespace Elastos
