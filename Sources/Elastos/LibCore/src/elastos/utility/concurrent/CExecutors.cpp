
#include "CExecutors.h"
#include "Executors.h"

namespace Elastos {
namespace Utility {
namespace Concurrent {

ECode CExecutors::Callable(
    /* [in] */ IRunnable* task,
    /* [in] */ IInterface* result,
    /* [out] */ ICallable** object)
{
    return Executors::Callable(task, result, object);
}

} // namespace Concurrent
} // namespace Utility
} // namespace Elastos
