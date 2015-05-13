
#include "ext/frameworkdef.h"
#include "net/wifi/CWifiManagerHelper.h"
#include "net/wifi/CWifiManager.h"

namespace Elastos {
namespace Droid {
namespace Net {
namespace Wifi {

ECode CWifiManagerHelper::CalculateSignalLevel(
    /* [in] */ Int32 rssi,
    /* [in] */ Int32 numLevels,
    /* [out] */ Int32* level)
{
    VALIDATE_NOT_NULL(level);
    *level = CWifiManager::CalculateSignalLevel(rssi, numLevels);
    return NOERROR;
}

ECode CWifiManagerHelper::CompareSignalLevel(
    /* [in] */ Int32 rssiA,
    /* [in] */ Int32 rssiB,
    /* [out] */ Int32* result)
{
    VALIDATE_NOT_NULL(result);
    *result = CWifiManager::CompareSignalLevel(rssiA, rssiB);
    return NOERROR;
}

} // namespace Wifi
} // namespace Net
} // namespace Droid
} // namespace Elastos
