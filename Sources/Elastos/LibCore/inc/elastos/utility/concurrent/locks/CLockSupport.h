
#ifndef __ELASTOS_UTILITY_CLOCKSUPPORT_H__
#define __ELASTOS_UTILITY_CLOCKSUPPORT_H__

#include "_Elastos_Utility_Concurrent_CLockSupport.h"
#include "LockSupport.h"

namespace Elastos {
namespace Utility {
namespace Concurrent {
namespace Locks {

CarClass(CLockSupport)
    , public LockSupport
{
    CAR_OBJECT_DECL(CLockSupport)
};

} // namespace Locks
} // namespace Concurrent
} // namespace Utility
} // namespace Elastos

#endif //__ELASTOS_UTILITY_CLOCKSUPPORT_H__
