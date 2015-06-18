#include "CZoneInfoHelper.h"
#include "ZoneInfo.h"

namespace Libcore {
namespace Utility {

CAR_SINGLETON_IMPL(CZoneInfoHelper)

CAR_INTERFACE_IMPL(CZoneInfoHelper, Singleton, IZoneInfoHelper)

ECode CZoneInfoHelper::MakeTimeZone(
    /* [in] */ const String& id,
    /* [in] */ IBufferIterator* it,
    /* [out] */ IZoneInfo** zoneInfo)
{
    *zoneInfo =ZoneInfo::MakeTimeZone(id, it);
    REFCOUNT_ADD(*zoneInfo)
    return NOERROR;
}


} // namespace Utility
} // namespace Libcore