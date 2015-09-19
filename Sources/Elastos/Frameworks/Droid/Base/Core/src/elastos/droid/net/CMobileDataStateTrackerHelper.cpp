
#include "ext/frameworkdef.h"
#include "CMobileDataStateTrackerHelper.h"
#include "CMobileDataStateTracker.h"

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

