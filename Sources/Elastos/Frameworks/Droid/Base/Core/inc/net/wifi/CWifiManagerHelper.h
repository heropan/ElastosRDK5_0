
#ifndef __ELASTOS_DROID_NET_WIFI_CWIFIMANAGERHELPER_H__
#define __ELASTOS_DROID_NET_WIFI_CWIFIMANAGERHELPER_H__

#include "_Elastos_Droid_Net_Wifi_CWifiManagerHelper.h"

namespace Elastos {
namespace Droid {
namespace Net {
namespace Wifi {

CarClass(CWifiManagerHelper)
{
public:
    CARAPI CalculateSignalLevel(
        /* [in] */ Int32 rssi,
        /* [in] */ Int32 numLevels,
        /* [out] */ Int32* level);

    CARAPI CompareSignalLevel(
        /* [in] */ Int32 rssiA,
        /* [in] */ Int32 rssiB,
        /* [out] */ Int32* result);
};

} // namespace Wifi
} // namespace Net
} // namespace Droid
} // namespace Elastos

#endif //__ELASTOS_DROID_NET_WIFI_CWIFIMANAGERHELPER_H__
