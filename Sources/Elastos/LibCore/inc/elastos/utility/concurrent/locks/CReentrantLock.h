
#ifndef __ELASTOS_UTILITY_CREENTRANTLOCK_H__
#define __ELASTOS_UTILITY_CREENTRANTLOCK_H__

#include "_Elastos_Utility_Concurrent_CReentrantLock.h"
#include "ReentrantLock.h"

namespace Elastos {
namespace Utility {
namespace Concurrent {
namespace Locks {

CarClass(CReentrantLock), public ReentrantLock
{
    CAR_OBJECT_DECL(CReentrantLock)
};

} // namespace Locks
} // namespace Concurrent
} // namespace Utility
} // namespace Elastos

#endif //__ELASTOS_UTILITY_CREENTRANTLOCK_H__
