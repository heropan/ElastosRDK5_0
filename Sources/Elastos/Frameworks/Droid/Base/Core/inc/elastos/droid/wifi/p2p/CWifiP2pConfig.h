
#ifndef __ELASTOS_DROID_NET_WIFI_P2P_CWIFIP2PCONFIG_H__
#define __ELASTOS_DROID_NET_WIFI_P2P_CWIFIP2PCONFIG_H__

#include "_Elastos_Droid_Net_Wifi_P2p_CWifiP2pConfig.h"

using Elastos::Droid::Wifi::IWpsInfo;
using Elastos::Droid::Wifi::P2p::IWifiP2pConfig;

namespace Elastos {
namespace Droid {
namespace Wifi {
namespace P2p {

CarClass(CWifiP2pConfig)
{
public:
    CWifiP2pConfig();

public:
    CARAPI GetDeviceAddress(
        /* [out] */ String* deviceAddress);

    CARAPI SetDeviceAddress(
        /* [in] */ const String& deviceAddress);

    CARAPI GetWps(
        /* [out] */ IWpsInfo** wpsInfo);

    CARAPI SetWps(
        /* [in] */ IWpsInfo* wpsInfo);

    CARAPI GetGroupOwnerIntent(
        /* [out] */ Int32* intent);

    CARAPI SetGroupOwnerIntent(
        /* [in] */ Int32 intent);

    CARAPI GetNetId(
        /* [out] */ Int32* netId);

    CARAPI SetNetId(
        /* [in] */ Int32 netId);

    CARAPI ToString(
        /* [out] */ String* string);

    CARAPI ReadFromParcel(
        /* [in] */ IParcel* source);

    CARAPI WriteToParcel(
        /* [in] */ IParcel* dest);

    CARAPI constructor();

    CARAPI constructor(
        /* [in] */ const String& supplicantEvent);

    CARAPI constructor(
        /* [in] */ IWifiP2pConfig* source);

private:
    /**
     * The device MAC address uniquely identifies a Wi-Fi p2p device
     */
    String mDeviceAddress;

    /**
     * Wi-Fi Protected Setup information
     */
    AutoPtr<IWpsInfo> mWps;

    /**
     * This is an integer value between 0 and 15 where 0 indicates the least
     * inclination to be a group owner and 15 indicates the highest inclination
     * to be a group owner.
     *
     * A value of -1 indicates the system can choose an appropriate value.
     */
    Int32 mGroupOwnerIntent;

    /** @hide */
    Int32 mNetId;
};

}
}
}
}

#endif // __ELASTOS_DROID_NET_WIFI_P2P_CWIFIP2PCONFIG_H__
