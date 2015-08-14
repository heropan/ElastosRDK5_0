
#ifndef __ELASTOS_DROID_NET_WIFI_P2P_NSD_CWIFIP2PUPNPSERVICEINFO_H__
#define __ELASTOS_DROID_NET_WIFI_P2P_NSD_CWIFIP2PUPNPSERVICEINFO_H__

#include "_CWifiP2pUpnpServiceInfo.h"
#include "WifiP2pServiceInfo.h"

namespace Elastos {
namespace Droid {
namespace Net {
namespace Wifi {
namespace P2p {
namespace Nsd {

/**
 * A class for storing Upnp service information that is advertised
 * over a Wi-Fi peer-to-peer setup.
 *
 * {@see android.net.wifi.p2p.WifiP2pManager#addLocalService}
 * {@see android.net.wifi.p2p.WifiP2pManager#removeLocalService}
 * {@see WifiP2pServiceInfo}
 * {@see WifiP2pDnsSdServiceInfo}
 */
CarClass(CWifiP2pUpnpServiceInfo), public WifiP2pServiceInfo
{
public:
    CARAPI constructor();

    CARAPI constructor(
        /* [in] */ ArrayOf<String>* queryList);

    PInterface Probe(
        /* [in] */ REIID riid);

    CARAPI GetSupplicantQueryList(
        /* [out, callee] */ ArrayOf<String>** list);

    CARAPI Equals(
        /* [in] */ IInterface* obj,
        /* [out] */ Boolean* isEqual);

    CARAPI GetHashCode(
        /* [out] */ Int32* hashCode);

    CARAPI ReadFromParcel(
        /* [in] */ IParcel* source);

    CARAPI WriteToParcel(
        /* [in] */ IParcel* dest);

public:
    static CARAPI NewInstance(
        /* [in] */ const String& uuid,
        /* [in] */ const String& device,
        /* [in] */ ArrayOf<String>* services,
        /* [out] */ IWifiP2pUpnpServiceInfo** instance);

private:
    static CARAPI CreateSupplicantQuery(
        /* [in] */ const String& uuid,
        /* [in] */ const String& data,
        /* [out] */ String* queryString);
};

}
}
}
}
}
}
#endif // __ELASTOS_DROID_NET_WIFI_P2P_NSD_CWIFIP2PUPNPSERVICEINFO_H__
