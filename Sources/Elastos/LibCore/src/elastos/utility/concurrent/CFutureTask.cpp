
#include "CFutureTask.h"

namespace Elastos {
namespace Utility {
namespace Concurrent {

ECode CFutureTask::constructor(
    /* [in] */ ICallable* callable)
{
    return FutureTask::Init(callable);
}

ECode CFutureTask::constructor(
    /* [in] */ IRunnable* runnable,
    /* [in] */ IInterface* result)
{
    return FutureTask::Init(runnable, result);
}

PInterface CFutureTask::Probe(
    /* [in] */ REIID riid)
{
    return _CFutureTask::Probe(riid);
}

ECode CFutureTask::IsCancelled(
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
    return FutureTask::IsCancelled(result);
}

ECode CFutureTask::IsDone(
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
    return FutureTask::IsDone(result);
}

ECode CFutureTask::Cancel(
    /* [in] */ Boolean mayInterruptIfRunning,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
    return FutureTask::Cancel(mayInterruptIfRunning, result);
}

ECode CFutureTask::Get(
    /* [out] */ IInterface** result)
{
    VALIDATE_NOT_NULL(result);
    return FutureTask::Get(result);
}

ECode CFutureTask::Get(
    /* [in] */ Int64 timeout,
    /* [in] */ ITimeUnit* unit,
    /* [out] */ IInterface** result)
{
    VALIDATE_NOT_NULL(result);
    return FutureTask::Get(timeout, unit, result);
}

ECode CFutureTask::Run()
{
    return FutureTask::Run();
}

} // namespace Concurrent
} // namespace Utility
} // namespace Elastos
