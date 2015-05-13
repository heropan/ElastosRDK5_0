
#include "cmdef.h"
#include "CExecutorCompletionService.h"
#include "CLinkedBlockingQueue.h"
#include "CFutureTask.h"
#include "AbstractExecutorService.h"

using Elastos::Core::EIID_IRunnable;

namespace Elastos {
namespace Utility {
namespace Concurrent {


CAR_INTERFACE_IMPL_LIGHT_2(CExecutorCompletionService::QueueingFuture, IRunnableFuture, IRunnable);

void CExecutorCompletionService::QueueingFuture::Done()
{
    Boolean result;
    mOwner->mCompletionQueue->Add(mTask, &result);
}

ECode CExecutorCompletionService::QueueingFuture::Cancel(
    /* [in] */ Boolean mayInterruptIfRunning,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
    return FutureTask::Cancel(mayInterruptIfRunning, result);
}

ECode CExecutorCompletionService::QueueingFuture::IsCancelled(
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
    return FutureTask::IsCancelled(result);
}

ECode CExecutorCompletionService::QueueingFuture::IsDone(
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
    return FutureTask::IsDone(result);
}

ECode CExecutorCompletionService::QueueingFuture::Get(
    /* [out] */ IInterface** result)
{
    VALIDATE_NOT_NULL(result);
    return FutureTask::Get(result);
}

ECode CExecutorCompletionService::QueueingFuture::GetEx(
    /* [in] */ Int64 timeout,
    /* [in] */ ITimeUnit* unit,
    /* [out] */ IInterface** result)
{
    VALIDATE_NOT_NULL(result);
    return FutureTask::GetEx(timeout, unit, result);
}

ECode CExecutorCompletionService::QueueingFuture::Run()
{
    return FutureTask::Run();
}


ECode CExecutorCompletionService::constructor(
    /* [in] */ IExecutor* executor)
{
    if (executor == NULL) return E_NULL_POINTER_EXCEPTION;
    mExecutor = executor;
    mAes = (IAbstractExecutorService::Probe(executor)) ?
            IAbstractExecutorService::Probe(executor) : NULL;
    return CLinkedBlockingQueue::New((IBlockingQueue**)&mCompletionQueue);
}

ECode CExecutorCompletionService::constructor(
    /* [in] */ IExecutor* executor,
    /* [in] */ IBlockingQueue* completionQueue)
{
    if (executor == NULL || completionQueue == NULL) return E_NULL_POINTER_EXCEPTION;
    mExecutor = executor;
    mAes = (IAbstractExecutorService::Probe(executor)) ?
            IAbstractExecutorService::Probe(executor) : NULL;
    mCompletionQueue = completionQueue;
    return NOERROR;
}

AutoPtr<IRunnableFuture> CExecutorCompletionService::NewTaskFor(
    /* [in] */ ICallable* task)
{
    if (mAes == NULL) {
        AutoPtr<IRunnableFuture> f;
        CFutureTask::New(task, (IRunnableFuture**)&f);
        return f;
    }
    else {
        AbstractExecutorService* aes = (AbstractExecutorService*)mAes->Probe(EIID_AbstractExecutorService);
        assert(aes != NULL);
        return aes->NewTaskFor(task);
    }
}

AutoPtr<IRunnableFuture> CExecutorCompletionService::NewTaskFor(
    /* [in] */ IRunnable* task,
    /* [in] */ IInterface* result)
{
    if (mAes == NULL) {
        AutoPtr<IRunnableFuture> f;
        CFutureTask::New(task, result, (IRunnableFuture**)&f);
        return f;
    }
    else {
        AbstractExecutorService* aes = (AbstractExecutorService*)mAes->Probe(EIID_AbstractExecutorService);
        assert(aes != NULL);
        return aes->NewTaskFor(task, result);
    }
}

ECode CExecutorCompletionService::Submit(
    /* [in] */ ICallable* task,
    /* [out] */ IFuture** future)
{
    VALIDATE_NOT_NULL(future);
    if (task == NULL) return E_NULL_POINTER_EXCEPTION;
    AutoPtr<IRunnableFuture> f = NewTaskFor(task);
    AutoPtr<IRunnable> r = new QueueingFuture(f, this);
    mExecutor->Execute(r);
    *future = (IFuture*)f.Get();
    INTERFACE_ADDREF(*future);
    return NOERROR;
}

ECode CExecutorCompletionService::SubmitEx(
    /* [in] */ IRunnable* task,
    /* [in] */ IInterface* result,
    /* [out] */ IFuture** future)
{
    VALIDATE_NOT_NULL(future);
    if (task == NULL) return E_NULL_POINTER_EXCEPTION;
    AutoPtr<IRunnableFuture> f = NewTaskFor(task, result);
    AutoPtr<IRunnable> r = new QueueingFuture(f, this);
    mExecutor->Execute(r);
    *future = (IFuture*)f.Get();
    INTERFACE_ADDREF(*future);
    return NOERROR;
}

ECode CExecutorCompletionService::Take(
    /* [out] */ IFuture** future)
{
    VALIDATE_NOT_NULL(future)
    AutoPtr<IInterface> e;
    FAIL_RETURN(mCompletionQueue->Take((IInterface**)&e));
    *future = IFuture::Probe(e);
    INTERFACE_ADDREF(*future);
    return NOERROR;
}

ECode CExecutorCompletionService::Poll(
    /* [out] */ IFuture** future)
{
    VALIDATE_NOT_NULL(future)
    AutoPtr<IInterface> e;
    FAIL_RETURN(mCompletionQueue->Poll((IInterface**)&e));
    *future = IFuture::Probe(e);
    INTERFACE_ADDREF(*future);
    return NOERROR;
}

ECode CExecutorCompletionService::PollEx(
    /* [in] */ Int64 timeout,
    /* [in] */ ITimeUnit* unit,
    /* [out] */ IFuture** future)
{
    VALIDATE_NOT_NULL(future)
    AutoPtr<IInterface> e;
    FAIL_RETURN(mCompletionQueue->PollEx(timeout, unit, (IInterface**)&e));
    *future = IFuture::Probe(e);
    INTERFACE_ADDREF(*future);
    return NOERROR;
}

} // namespace Concurrent
} // namespace Utility
} // namespace Elastos
