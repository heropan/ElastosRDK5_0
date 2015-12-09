
#include "Elastos.CoreLibrary.Utility.h"
#include "CForkJoinTaskHelper.h"
#include "ForkJoinTask.h"

using Elastos::Utility::Concurrent::ForkJoinTask;

namespace Elastos {
namespace Utility {
namespace Concurrent {

//====================================================================
// CForkJoinTaskHelper::
//====================================================================
CAR_INTERFACE_IMPL(CForkJoinTaskHelper, Singleton, IForkJoinTaskHelper)

CAR_SINGLETON_IMPL(CForkJoinTaskHelper)

ECode CForkJoinTaskHelper::InvokeAll(
    /* [in] */ IForkJoinTask* t1,
    /* [in] */ IForkJoinTask* t2)
{
    ForkJoinTask::InvokeAll(t1, t2);
    return NOERROR;
}

ECode CForkJoinTaskHelper::InvokeAll(
    /* [in] */ ArrayOf<IForkJoinTask*>* tasks)
{
    ForkJoinTask::InvokeAll(tasks);
    return NOERROR;
}

ECode CForkJoinTaskHelper::InvokeAll(
    /* [in] */ ICollection* tasks,
    /* [out] */ ICollection** outcol)
{
    VALIDATE_NOT_NULL(outcol);
    AutoPtr<ICollection> p = ForkJoinTask::InvokeAll(tasks);
    *outcol = p.Get();
    REFCOUNT_ADD(*outcol);
    return NOERROR;
}

ECode CForkJoinTaskHelper::HelpQuiesce()
{
    ForkJoinTask::HelpQuiesce();
    return NOERROR;
}

ECode CForkJoinTaskHelper::GetPool(
    /* [out] */ IForkJoinPool** outpool)
{
    VALIDATE_NOT_NULL(outpool);
    AutoPtr<IForkJoinPool> p = ForkJoinTask::GetPool();
    *outpool = p.Get();
    REFCOUNT_ADD(*outpool);
    return NOERROR;
}

ECode CForkJoinTaskHelper::InForkJoinPool(
    /* [out] */ Boolean* value)
{
    VALIDATE_NOT_NULL(value);
    *value = ForkJoinTask::InForkJoinPool();
    return NOERROR;
}

ECode CForkJoinTaskHelper::GetQueuedTaskCount(
    /* [out] */ Int32* value)
{
    VALIDATE_NOT_NULL(value);
    *value = ForkJoinTask::GetQueuedTaskCount();
    return NOERROR;
}

ECode CForkJoinTaskHelper::GetSurplusQueuedTaskCount(
    /* [out] */ Int32* value)
{
    VALIDATE_NOT_NULL(value);
    *value = ForkJoinTask::GetSurplusQueuedTaskCount();
    return NOERROR;
}

ECode CForkJoinTaskHelper::Adapt(
    /* [in] */ IRunnable* runnable,
    /* [out] */ IForkJoinTask** outjoin)
{
    VALIDATE_NOT_NULL(outjoin);
    AutoPtr<IForkJoinTask> res = ForkJoinTask::Adapt(runnable);
    *outjoin = res.Get();
    REFCOUNT_ADD(*outjoin);
    return NOERROR;
}

ECode CForkJoinTaskHelper::Adapt(
    /* [in] */ IRunnable* runnable,
    /* [in] */ IInterface* result,
    /* [out] */ IForkJoinTask** outjoin)
{
    VALIDATE_NOT_NULL(outjoin);
    AutoPtr<IForkJoinTask> res = ForkJoinTask::Adapt(runnable, result);
    *outjoin = res.Get();
    REFCOUNT_ADD(*outjoin);
    return NOERROR;
}

ECode CForkJoinTaskHelper::Adapt(
    /* [in] */ ICallable* callable,
    /* [out] */ IForkJoinTask** outjoin)
{
    VALIDATE_NOT_NULL(outjoin);
    AutoPtr<IForkJoinTask> res = ForkJoinTask::Adapt(callable);
    *outjoin = res.Get();
    REFCOUNT_ADD(*outjoin);
    return NOERROR;
}

} // namespace Concurrent
} // namespace Utility
} // namespace Elastos
