#ifndef __ELASTOS_DROID_NET_ETHERNET_CETHERNETDEVINFO_H__
#define __ELASTOS_DROID_NET_ETHERNET_CETHERNETDEVINFO_H__

#include "_Elastos_Droid_Net_Ethernet_CEthernetDevInfo.h"

namespace Elastos {
namespace Droid {
namespace Net {
namespace Ethernet {

CarClass(CEthernetDevInfo)
{
public:
    CEthernetDevInfo();

    CARAPI SetIfName(
        /* [in] */ const String& ifname);

    CARAPI GetIfName(
        /* [out] */ String* str);

    CARAPI SetIpAddress(
        /* [in] */ const String& ip);

    CARAPI GetIpAddress(
        /* [out] */ String* str);

    CARAPI SetNetMask(
        /* [in] */ const String& ip);

    CARAPI GetNetMask(
        /* [out] */ String* str);

    CARAPI SetGateWay(
        /* [in] */ const String& gw);

    CARAPI GetGateWay(
        /* [out] */ String* gw);

    CARAPI SetDnsAddr(
        /* [in] */ const String& dns);

    CARAPI GetDnsAddr(
        /* [out] */ String* str);

    CARAPI SetHwaddr(
        /* [in] */ const String& hwaddr);

    CARAPI GetHwaddr(
        /* [out] */ String* hwaddr);

    CARAPI SetConnectMode(
        /* [in] */ Int32 mode);

    CARAPI GetConnectMode(
        /* [out] */ Int32* mode);

    CARAPI WriteToParcel(
        /* [in] */ IParcel* dest);

    CARAPI ReadFromParcel(
        /* [in] */ IParcel* source);

    CARAPI constructor();

private:
    String mDevName;
    String mIpaddr;
    String mNetmask;
    String mGw;
    String mDns;
    Int32 mMode;
    String mHwaddr;
};

}
}
}
}

#endif // __ELASTOS_DROID_NET_ETHERNET_CETHERNETDEVINFO_H__
