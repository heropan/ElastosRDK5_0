
#ifndef __ELASTOS_UTILITY_CFUTURETASK_H__
#define __ELASTOS_UTILITY_CFUTURETASK_H__

#include "_Elastos_Utility_Concurrent_CFutureTask.h"
#include "FutureTask.h"

namespace Elastos {
namespace Utility {
namespace Concurrent {

CarClass(CFutureTask), public FutureTask
{
public:
    CAR_OBJECT_DECL()

    CARAPI constructor(
        /* [in] */ ICallable* callable);

    CARAPI constructor(
        /* [in] */ IRunnable* runnable,
        /* [in] */ IInterface* result);
};

} // namespace Concurrent
} // namespace Utility
} // namespace Elastos

#endif //__ELASTOS_UTILITY_CFUTURETASK_H__
