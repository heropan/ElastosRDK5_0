
#ifndef __CFUTURETASK_H__
#define __CFUTURETASK_H__

#include "_CFutureTask.h"
#include "FutureTask.h"

namespace Elastos {
namespace Utility {
namespace Concurrent {

CarClass(CFutureTask), public FutureTask
{
public:
    CARAPI constructor(
        /* [in] */ ICallable* callable);

    CARAPI constructor(
        /* [in] */ IRunnable* runnable,
        /* [in] */ IInterface* result);

    CARAPI_(PInterface) Probe(
        /* [in] */ REIID riid);

    CARAPI IsCancelled(
        /* [out] */ Boolean* result);

    CARAPI IsDone(
        /* [out] */ Boolean* result);

    CARAPI Cancel(
        /* [in] */ Boolean mayInterruptIfRunning,
        /* [out] */ Boolean* result);

    CARAPI Get(
        /* [out] */ IInterface** result);

    CARAPI Get(
        /* [in] */ Int64 timeout,
        /* [in] */ ITimeUnit* unit,
        /* [out] */ IInterface** result);

    CARAPI Run();
};

} // namespace Concurrent
} // namespace Utility
} // namespace Elastos

#endif //__CFUTURETASK_H__
