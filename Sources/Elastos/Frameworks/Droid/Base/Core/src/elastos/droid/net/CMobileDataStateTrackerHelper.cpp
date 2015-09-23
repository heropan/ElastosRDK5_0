
#include "elastos/droid/ext/frameworkdef.h"
#include "elastos/droid/net/CMobileDataStateTrackerHelper.h"
#include "elastos/droid/net/CMobileDataStateTracker.h"

namespace Elastos {
namespace Droid {
namespace Net {

CAR_INTERFACE_IMPL(CMobileDataStateTrackerHelper, Singleton, IMobileDataStateTrackerHelper)

CAR_SINGLETON_IMPL(CMobileDataStateTrackerHelper)

ECode CMobileDataStateTrackerHelper::NetworkTypeToApnType(
    /* [in] */ Int32 netType,
    /* [out] */ String* result)
{
    return CMobileDataStateTracker::NetworkTypeToApnType(netType, result);
}

} // namespace Net
} // namespace Droid
} // namespace Elastos

