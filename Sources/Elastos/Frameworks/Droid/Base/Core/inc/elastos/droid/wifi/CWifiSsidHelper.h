
#ifndef __ELASTOS_DROID_NET_WIFI_CWIFISSIDHELPER_H__
#define __ELASTOS_DROID_NET_WIFI_CWIFISSIDHELPER_H__

#include "_Elastos_Droid_Net_Wifi_CWifiSsidHelper.h"

namespace Elastos {
namespace Droid {
namespace Wifi {

CarClass(CWifiSsidHelper)
{
public:
    CARAPI CreateFromAsciiEncoded(
        /* [in] */ const String& asciiEncoded,
        /* [out] */ IWifiSsid** wifiSsid);

    CARAPI CreateFromHex(
        /* [in] */ const String& hexStr,
        /* [out] */ IWifiSsid** wifiSsid);

private:
};

}
}
}

#endif // __ELASTOS_DROID_NET_WIFI_CWIFISSIDHELPER_H__
