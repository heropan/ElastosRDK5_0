
#include "CFutureTask.h"

namespace Elastos {
namespace Utility {
namespace Concurrent {

CAR_OBJECT_IMPL(CFutureTask)

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

} // namespace Concurrent
} // namespace Utility
} // namespace Elastos
