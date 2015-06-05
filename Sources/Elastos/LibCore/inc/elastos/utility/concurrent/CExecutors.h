
#ifndef __ELASTOS_UTILITY_CEXECUTORS_H__
#define __ELASTOS_UTILITY_CEXECUTORS_H__

#include "_Elastos_Utility_Concurrent_CExecutors.h"
#include "Executors.h"

namespace Elastos {
namespace Utility {
namespace Concurrent {

CarClass(CExecutors)
    , public Executors
{
public:
    CAR_OBJECT_DECL()
};

} // namespace Concurrent
} // namespace Utility
} // namespace Elastos

#endif //__ELASTOS_UTILITY_CEXECUTORS_H__