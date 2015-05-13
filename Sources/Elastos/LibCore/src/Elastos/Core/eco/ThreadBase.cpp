#include "cmdef.h"
#include "elastos/ThreadBase.h"

using Elastos::Core::EIID_IRunnable;
using Elastos::Core::Threading::EIID_IThread;

namespace Elastos {
namespace Core {
namespace Threading {

ThreadBase::ThreadBase()
{}

ThreadBase::~ThreadBase()
{}

UInt32 ThreadBase::AddRef()
{
    return ElRefBase::AddRef();
}

UInt32 ThreadBase::Release()
{
    return ElRefBase::Release();
}

PInterface ThreadBase::Probe(
    /* [in] */ REIID riid)
{
    if (riid == EIID_IThread) {
        return (IThread*)this;
    }
    else if (riid == EIID_IRunnable) {
        return (IRunnable*)this;
    }
    else if (riid == EIID_ISynchronize) {
        return (ISynchronize*)this;
    }
    else if (riid == EIID_IInterface) {
        return (PInterface)(IThread*)this;
    }
    else if (riid == EIID_Thread) {
        return reinterpret_cast<PInterface>((Thread*)this);
    }

    return NULL;
}

ECode ThreadBase::GetInterfaceID(
    /* [in] */ IInterface *pObject,
    /* [out] */ InterfaceID *pIID)
{
    if (pIID == NULL) {
        return E_INVALID_ARGUMENT;
    }

    if (pObject == (IInterface*)(IThread*)this) {
        *pIID = EIID_IThread;
    }
    else if (pObject == (IInterface*)(IRunnable*)this) {
        *pIID = EIID_IRunnable;
    }
    else if (pObject == (IInterface*)(ISynchronize*)this) {
        *pIID = EIID_ISynchronize;
    }
    else {
        return E_INVALID_ARGUMENT;
    }

    return NOERROR;
}

ECode ThreadBase::CheckAccess()
{
    return Thread::CheckAccess();
}

ECode ThreadBase::CountStackFrames(
    /* [out] */ Int32* number)
{
    return Thread::CountStackFrames(number);
}

ECode ThreadBase::Destroy()
{
    return Thread::Destroy();
}

ECode ThreadBase::GetId(
    /* [out] */ Int64* id)
{
    return Thread::GetId(id);
}

ECode ThreadBase::GetName(
    /* [out] */ String* name)
{
    return Thread::GetName(name);
}

ECode ThreadBase::GetPriority(
    /* [out] */ Int32* priority)
{
    return Thread::GetPriority(priority);
}

ECode ThreadBase::GetState(
    /* [out] */ ThreadState* state)
{
    return Thread::GetState(state);
}

ECode ThreadBase::GetThreadGroup(
    /* [out] */ IThreadGroup** group)
{
    return Thread::GetThreadGroup(group);
}

ECode ThreadBase::Interrupt()
{
    return Thread::Interrupt();
}

ECode ThreadBase::IsAlive(
    /* [out] */ Boolean* isAlive)
{
    return Thread::IsAlive(isAlive);
}

ECode ThreadBase::IsDaemon(
    /* [out] */ Boolean* isDaemon)
{
    return Thread::IsDaemon(isDaemon);
}

ECode ThreadBase::IsInterrupted(
    /* [out] */ Boolean* isInterrupted)
{
    return Thread::IsInterrupted(isInterrupted);
}

ECode ThreadBase::Join()
{
    return Thread::Join();
}

ECode ThreadBase::JoinEx(
    /* [in] */ Int64 millis)
{
    return Thread::JoinEx(millis);
}

ECode ThreadBase::JoinEx2(
    /* [in] */ Int64 millis,
    /* [in] */ Int32 nanos)
{
    return Thread::JoinEx2(millis, nanos);
}

ECode ThreadBase::Resume()
{
    return Thread::Resume();
}

ECode ThreadBase::SetDaemon(
    /* [in] */ Boolean isDaemon)
{
    return Thread::SetDaemon(isDaemon);
}

ECode ThreadBase::SetName(
    /* [in] */ const String& threadName)
{
    return Thread::SetName(threadName);
}

ECode ThreadBase::SetPriority(
    /* [in] */ Int32 priority)
{
    return Thread::SetPriority(priority);
}

ECode ThreadBase::Start()
{
    return Thread::Start();
}

ECode ThreadBase::Stop()
{
    return Thread::Stop();
}

ECode ThreadBase::Suspend()
{
    return Thread::Suspend();
}

ECode ThreadBase::Detach()
{
    return Thread::Detach();
}

ECode ThreadBase::Unpark()
{
    return Thread::Unpark();
}

ECode ThreadBase::ParkFor(
    /* [in] */ Int64 nanos)
{
    return Thread::ParkFor(nanos);
}

ECode ThreadBase::ParkUntil(
    /* [in] */ Int64 time)
{
    return Thread::ParkUntil(time);
}

ECode ThreadBase::Lock()
{
    return Thread::Lock();
}

ECode ThreadBase::Unlock()
{
    return Thread::Unlock();
}

ECode ThreadBase::Notify()
{
    return Thread::Notify();
}

ECode ThreadBase::NotifyAll()
{
    return Thread::NotifyAll();
}

ECode ThreadBase::Wait()
{
    return Thread::Wait();
}

ECode ThreadBase::WaitEx(
    /* [in] */ Int64 millis)
{
    return Thread::Wait(millis);
}

ECode ThreadBase::WaitEx2(
    /* [in] */ Int64 millis,
    /* [in] */ Int32 nanos)
{
    return Thread::Wait(millis, nanos);
}

ECode ThreadBase::Run()
{
    return Thread::Run();
}

ECode ThreadBase::GetUncaughtExceptionHandler(
    /* [out] */ IThreadUncaughtExceptionHandler** handler)
{
    return Thread::GetUncaughtExceptionHandler(handler);
}

ECode ThreadBase::GetContextClassLoader(
    /* [out] */ IClassLoader** outload)
{
    return Thread::GetContextClassLoader(outload);
}

ECode ThreadBase::SetContextClassLoader(
    /* [in] */ IClassLoader* cl)
{
    return Thread::SetContextClassLoader(cl);
}

ECode ThreadBase::PushInterruptAction(
    /* [in] */ IRunnable* interruptAction)
{
    return Thread::PushInterruptAction(interruptAction);
}

ECode ThreadBase::PopInterruptAction(
    /* [in] */ IRunnable* interruptAction)
{
    return Thread::PopInterruptAction(interruptAction);
}

ECode ThreadBase::SetUncaughtExceptionHandler(
    /* [in] */ IThreadUncaughtExceptionHandler* handler)
{
    return Thread::SetUncaughtExceptionHandler(handler);
}

} // namespace Threading
} // namespace Core
} // namespace Elastos
