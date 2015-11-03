#include "elastos/droid/app/QueuedWork.h"
#include <elastos/core/AutoLock.h>

using Elastos::Utility::Concurrent::CConcurrentLinkedQueue;
using Elastos::Utility::Concurrent::IExecutors;
using Elastos::Utility::Concurrent::CExecutors;

namespace Elastos{
namespace Droid{
namespace App{

static AutoPtr<IConcurrentLinkedQueue> InitPendingWorkFinishers()
{
    AutoPtr<IConcurrentLinkedQueue> queue;
    CConcurrentLinkedQueue::New((IConcurrentLinkedQueue**)&queue);
    return queue;
}

AutoPtr<IConcurrentLinkedQueue> QueuedWork::sPendingWorkFinishers = InitPendingWorkFinishers();
AutoPtr<IExecutorService> QueuedWork::sSingleThreadExecutor;
Object QueuedWork::sClassLock;


ECode QueuedWork::SingleThreadExecutor(
    /* [out] */ IExecutorService *singleThreadExecutor)
{
    VALIDATE_NOT_NULL(singleThreadExecutor)
    *singleThreadExecutor = NULL;
    synchronized (sClassLock) {
        if (sSingleThreadExecutor == null) {
            AutoPtr<IExecutors> executors;
            CExecutors::AcquireSingleton((IExecutors**)&executors);
            // TODO: can we give this single thread a thread name?
            executors->NewSingleThreadExecutor((IExecutorService**)&sSingleThreadExecutor);
        }

        *singleThreadExecutor = sSingleThreadExecutor;
        REFCOUNT_ADD(*singleThreadExecutor);
    }
    return NOERROR;
}

ECode QueuedWork::Add(
    /* [in] */ IRunnable *finisher)
{
    return sPendingWorkFinishers->Add(finisher);
}

ECode QueuedWork::Remove(
    /* [in] */ IRunnable *finisher)
{
    return sPendingWorkFinishers->Remove(finisher);
}

ECode QueuedWork::WaitToFinish()
{
    AutoPtr<IInterface> obj;
    sPendingWorkFinishers->Poll((IInterface**)&obj);
    IRunnable* toFinish = NULL;
    while (obj != NULL) {
        IRunnable::Probe(obj)->Run();
        obj = NULL;
        sPendingWorkFinishers->Poll((IInterface**)&obj);
    }
    return NOERROR;
}

ECode QueuedWork::HasPendingWork(
    /* [out] */ Boolean result)
{
    VALIDATE_NOT_NULL(result)
    Boolean isEmpty;
    sPendingWorkFinishers->IsEmpty(&isEmpty);
    *result = !isEmpty;
    return NOERROR;
}

}
}
}
