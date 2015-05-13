
#include "Executors.h"
#include <elastos/Thread.h>
#include <elastos/StringBuilder.h>
#include <elastos/StringUtils.h>

#ifdef ELASTOS_CORE
#include "Elastos.Core_server.h"
#include "CThread.h"
#include "CAtomicInteger32.h"
#else
#include "Elastos.Core.h"
#endif

using Elastos::Core::StringBuilder;
using Elastos::Core::StringUtils;
using Elastos::Core::Threading::CThread;
using Elastos::Core::Threading::Thread;
using Elastos::Utility::Concurrent::Atomic::CAtomicInteger32;

namespace Elastos {
namespace Utility {
namespace Concurrent {

//==============================================
//    Executors::RunnableAdapter
//===============================================
CAR_INTERFACE_IMPL_LIGHT(Executors::RunnableAdapter, ICallable)

ECode Executors::RunnableAdapter::Call(
    /* [out] */ IInterface** result)
{
    VALIDATE_NOT_NULL(result);
    ECode ec = mTask->Run();
    *result = mResult;
    INTERFACE_ADDREF(*result);
    return ec;
}


//==============================================
//    Executors::DefaultThreadFactory
//===============================================
static AutoPtr<IAtomicInteger32> InitsPoolNumber()
{
    AutoPtr<CAtomicInteger32> ai;
    CAtomicInteger32::NewByFriend(1, (CAtomicInteger32**)&ai);
    return (IAtomicInteger32*)ai.Get();
}

const AutoPtr<IAtomicInteger32> Executors::DefaultThreadFactory::sPoolNumber = InitsPoolNumber();

Executors::DefaultThreadFactory::DefaultThreadFactory()
{
    CAtomicInteger32::New(1, (IAtomicInteger32**)&mThreadNumber);
    // SecurityManager s = System.getSecurityManager();
    // group = (s != null) ? s.getThreadGroup() :
    //                       Thread.currentThread().getThreadGroup();
    Thread::GetCurrentThread()->GetThreadGroup((IThreadGroup**)&mGroup);
    Int32 c;
    sPoolNumber->GetAndIncrement(&c);
    StringBuilder sb;
    sb += "pool-";
    sb += c;
    sb += "-thread-";
    mNamePrefix = sb.ToString();
}

CAR_INTERFACE_IMPL_LIGHT(Executors::DefaultThreadFactory, IThreadFactory)

ECode Executors::DefaultThreadFactory::NewThread(
    /* [in] */ IRunnable* r,
    /* [out] */ IThread** thread)
{
    VALIDATE_NOT_NULL(thread);
    Int32 c;
    mThreadNumber->GetAndIncrement(&c);
    FAIL_RETURN(CThread::New(mGroup, r,
            mNamePrefix + StringUtils::Int32ToString(c), 0, thread));
    Boolean isDaemon;
    if ((*thread)->IsDaemon(&isDaemon), isDaemon) {
        (*thread)->SetDaemon(FALSE);
    }
    Int32 priority;
    if ((*thread)->GetPriority(&priority), priority != IThread::NORM_PRIORITY) {
        (*thread)->SetPriority(IThread::NORM_PRIORITY);
    }
    return NOERROR;
}


//==============================================
//    Executors
//===============================================
AutoPtr<IThreadFactory> Executors::GetDefaultThreadFactory()
{
    return new DefaultThreadFactory();
}

ECode Executors::Callable(
    /* [in] */ IRunnable* task,
    /* [in] */ IInterface* result,
    /* [out] */ ICallable** object)
{
    if (task == NULL) return E_NULL_POINTER_EXCEPTION;
    *object = new RunnableAdapter(task, result);
    INTERFACE_ADDREF(*object);
    return NOERROR;
}

} // namespace Concurrent
} // namespace Utility
} // namespace Elastos
