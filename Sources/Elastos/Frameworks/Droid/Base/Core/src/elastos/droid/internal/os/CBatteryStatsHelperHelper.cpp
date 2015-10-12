
#include "internal/os/CBatteryStatsHelperHelper.h"
#include "internal/os/CBatteryStatsHelper.h"

namespace Elastos {
namespace Droid {
namespace Internal {
namespace Os {

CAR_INTERFACE_IMPL(CBatteryStatsHelperHelper, Singleton, IBatteryStatsHelperHelper)

CAR_SINGLETON_IMPL(CBatteryStatsHelperHelper)

ECode CBatteryStatsHelperHelper::CheckWifiOnly(
    /* [in] */ IContext* context,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result)
    *result = CBatteryStatsHelper::CheckWifiOnly(context);
    return NOERROR;
}

ECode CBatteryStatsHelperHelper::StatsFromFile(
    /* [in] */ IContext* context,
    /* [in] */ const String& fname,
    /* [out] */ IBatteryStats** stats)
{
    VALIDATE_NOT_NULL(stats)
    AutoPtr<IBatteryStats> temp = CBatteryStatsHelper::StatsFromFile(context, fname);
    *stats = temp;
    REFCOUNT_ADD(*stats)
    return NOERROR;
}

ECode CBatteryStatsHelperHelper::DropFile(
    /* [in] */ IContext* context,
    /* [in] */ const String& fname)
{
    return CBatteryStatsHelper::DropFile(context, fname);
}

ECode CBatteryStatsHelperHelper::MakemAh(
    /* [in] */ Double power,
    /* [out] */ String* result)
{
    VALIDATE_NOT_NULL(result)
    *result = CBatteryStatsHelper::MakemAh(power);
    return NOERROR;
}

ECode CBatteryStatsHelperHelper::ReadFully(
    /* [in] */ IFileInputStream* stream,
    /* [out, callee] */ ArrayOf<Byte>** bytes)
{
    return CBatteryStatsHelper::ReadFully(stream, bytes);
}

ECode CBatteryStatsHelperHelper::ReadFully(
    /* [in] */ IFileInputStream* stream,
    /* [in] */ Int32 avail,
    /* [out, callee] */ ArrayOf<Byte>** bytes)
{
    return CBatteryStatsHelper::ReadFully(stream, avail, bytes);
}

} // namespace Os
} // namespace Droid
} // namespace Internal
} // namespace Elastos
