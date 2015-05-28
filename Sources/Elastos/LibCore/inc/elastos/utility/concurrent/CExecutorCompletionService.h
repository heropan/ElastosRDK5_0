
#ifndef __CEXECUTORCOMPLETIONSERVICE_H__
#define __CEXECUTORCOMPLETIONSERVICE_H__

#include "_CExecutorCompletionService.h"
#include "FutureTask.h"

using Elastos::Core::IRunnable;

namespace Elastos {
namespace Utility {
namespace Concurrent {

CarClass(CExecutorCompletionService)
{
private:
    /**
     * FutureTask extension to enqueue upon completion
     */
    class QueueingFuture
        : public ElLightRefBase
        , public FutureTask
        , public IRunnableFuture
        , public IRunnable
    {
    public:
        QueueingFuture(
            /* [in] */ IRunnableFuture* task,
            /* [in] */ CExecutorCompletionService* owner)
            : FutureTask(IRunnable::Probe(task), NULL)
            , mTask(task)
            , mOwner(owner)
        {}

        CAR_INTERFACE_DECL();

        CARAPI Cancel(
            /* [in] */ Boolean mayInterruptIfRunning,
            /* [out] */ Boolean* result);

        CARAPI IsCancelled(
            /* [out] */ Boolean* result);

        CARAPI IsDone(
            /* [out] */ Boolean* result);

        CARAPI Get(
            /* [out] */ IInterface** result);

        CARAPI Get(
            /* [in] */ Int64 timeout,
            /* [in] */ ITimeUnit* unit,
            /* [out] */ IInterface** result);

        CARAPI Run();

    protected:
        CARAPI_(void) Done();

    private:
        AutoPtr<IFuture> mTask;
        CExecutorCompletionService* mOwner;
    };

public:
    CARAPI constructor(
        /* [in] */ IExecutor* executor);

    CARAPI constructor(
        /* [in] */ IExecutor* executor,
        /* [in] */ IBlockingQueue* completionQueue);

    CARAPI Submit(
        /* [in] */ ICallable* task,
        /* [out] */ IFuture** future);

    CARAPI Submit(
        /* [in] */ IRunnable* task,
        /* [in] */ IInterface* result,
        /* [out] */ IFuture** future);

    CARAPI Take(
        /* [out] */ IFuture** future);

    CARAPI Poll(
        /* [out] */ IFuture** future);

    CARAPI Poll(
        /* [in] */ Int64 timeout,
        /* [in] */ ITimeUnit* unit,
        /* [out] */ IFuture** future);

private:
    CARAPI_(AutoPtr<IRunnableFuture>) NewTaskFor(
        /* [in] */ ICallable* task);

    CARAPI_(AutoPtr<IRunnableFuture>) NewTaskFor(
        /* [in] */ IRunnable* task,
        /* [in] */ IInterface* result);

private:
    AutoPtr<IExecutor> mExecutor;
    AutoPtr<IAbstractExecutorService> mAes;
    AutoPtr<IBlockingQueue> mCompletionQueue;
};

} // namespace Concurrent
} // namespace Utility
} // namespace Elastos

#endif //__CEXECUTORCOMPLETIONSERVICE_H__
