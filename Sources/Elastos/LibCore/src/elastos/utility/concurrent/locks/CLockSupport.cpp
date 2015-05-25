
#include "CLockSupport.h"
#include "LockSupport.h"

namespace Elastos {
namespace Utility {
namespace Concurrent {
namespace Locks {

ECode CLockSupport::Unpark(
    /* [in] */ IThread* thread)
{
    return LockSupport::Unpark(thread);
}

ECode CLockSupport::Park(
    /* [in] */ IInterface* blocker)
{
    return LockSupport::Park(blocker);
}

ECode CLockSupport::ParkNanos(
    /* [in] */ IInterface* blocker,
    /* [in] */ Int64 nanos)
{
    return LockSupport::ParkNanos(blocker, nanos);
}

} // namespace Locks
} // namespace Concurrent
} // namespace Utility
} // namespace Elastos
