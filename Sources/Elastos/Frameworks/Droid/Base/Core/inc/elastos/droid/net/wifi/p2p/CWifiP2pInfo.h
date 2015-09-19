
#ifndef __ELASTOS_DROID_NET_WIFI_P2P_CWIFIP2PINFO_H__
#define __ELASTOS_DROID_NET_WIFI_P2P_CWIFIP2PINFO_H__

#include "_Elastos_Droid_Net_Wifi_P2p_CWifiP2pInfo.h"

using Elastos::Net::IInetAddress;
using Elastos::Droid::Net::Wifi::P2p::IWifiP2pInfo;

namespace Elastos {
namespace Droid {
namespace Net {
namespace Wifi {
namespace P2p {

CarClass(CWifiP2pInfo)
{
public:
    CARAPI GetGroupFormed(
        /* [out] */ Boolean* formed);

    CARAPI SetGroupFormed(
        /* [in] */ Boolean formed);

    CARAPI IsGroupOwner(
        /* [out] */ Boolean* isGroupOwner);

    CARAPI SetIsGroupOwner(
        /* [in] */ Boolean isGroupOwner);

    CARAPI GetGroupOwnerAddress(
        /* [out] */ IInetAddress** address);

    CARAPI SetGroupOwnerAddress(
        /* [in] */ IInetAddress* address);

    CARAPI ToString(
        /* [out] */ String* string);

    CARAPI ReadFromParcel(
        /* [in] */ IParcel* source);

    CARAPI WriteToParcel(
        /* [in] */ IParcel* dest);

    CARAPI constructor();

    CARAPI constructor(
        /* [in] */ IWifiP2pInfo* source);

private:
    /** Indicates if a p2p group has been successfully formed */
    Boolean mGroupFormed;

    /** Indicates if the current device is the group owner */
    Boolean mIsGroupOwner;

    /** Group owner address */
    AutoPtr<IInetAddress> mGroupOwnerAddress;
};

}
}
}
}
}
#endif // __ELASTOS_DROID_NET_WIFI_P2P_CWIFIP2PINFO_H__
