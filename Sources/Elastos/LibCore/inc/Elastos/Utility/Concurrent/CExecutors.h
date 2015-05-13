
#ifndef __CEXECUTORS_H__
#define __CEXECUTORS_H__

#include "_CExecutors.h"

using Elastos::Core::IRunnable;

namespace Elastos {
namespace Utility {
namespace Concurrent {

CarClass(CExecutors)
{
public:
    /**
     * Returns a {@link Callable} object that, when
     * called, runs the given task and returns the given result.  This
     * can be useful when applying methods requiring a
     * <tt>Callable</tt> to an otherwise resultless action.
     * @param task the task to run
     * @param result the result to return
     * @return a callable object
     * @throws NullPointerException if task null
     */
    CARAPI Callable(
        /* [in] */ IRunnable* task,
        /* [in] */ IInterface* result,
        /* [out] */ ICallable** object);
};

} // namespace Concurrent
} // namespace Utility
} // namespace Elastos

#endif //__CEXECUTORS_H__