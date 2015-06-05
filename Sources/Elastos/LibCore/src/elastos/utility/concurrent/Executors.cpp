
#include "Executors.h"
#include <Thread.h>
#include <StringBuilder.h>
#include <StringUtils.h>
#include "CThread.h"
#include "CAtomicInteger32.h"

using Elastos::Core::StringBuilder;
using Elastos::Core::StringUtils;
using Elastos::Core::CThread;
using Elastos::Core::Thread;
using Elastos::Utility::Concurrent::Atomic::CAtomicInteger32;

namespace Elastos {
namespace Utility {
namespace Concurrent {

//==============================================
//    Executors::RunnableAdapter
//===============================================
CAR_INTERFACE_IMPL(Executors::RunnableAdapter, Object, ICallable)

ECode Executors::RunnableAdapter::Call(
    /* [out] */ IInterface** result)
{
    VALIDATE_NOT_NULL(result);
    ECode ec = mTask->Run();
    *result = mResult;
    REFCOUNT_ADD(*result);
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

CAR_INTERFACE_IMPL(Executors::DefaultThreadFactory, Object, IThreadFactory)

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
CAR_INTERFACE_IMPL(Executors, Object, IExecutors)

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
    REFCOUNT_ADD(*object);
    return NOERROR;
}

} // namespace Concurrent
} // namespace Utility
} // namespace Elastos
