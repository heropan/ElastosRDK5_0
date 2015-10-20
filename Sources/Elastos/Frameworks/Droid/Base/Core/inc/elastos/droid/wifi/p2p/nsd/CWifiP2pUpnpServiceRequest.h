
#ifndef __ELASTOS_DROID_NET_WIFI_P2P_NSD_CWIFIP2PUPNPSERVICEREQUEST_H__
#define __ELASTOS_DROID_NET_WIFI_P2P_NSD_CWIFIP2PUPNPSERVICEREQUEST_H__

#include "_Elastos_Droid_Net_Wifi_P2p_Nsd_CWifiP2pUpnpServiceRequest.h"
#include "WifiP2pServiceRequest.h"

namespace Elastos {
namespace Droid {
namespace Wifi {
namespace P2p {
namespace Nsd {

/**
 * A class for creating a Upnp service discovery request for use with
 * {@link WifiP2pManager#addServiceRequest} and {@link WifiP2pManager#removeServiceRequest}
 *
 * {@see WifiP2pManager}
 * {@see WifiP2pServiceRequest}
 * {@see WifiP2pDnsSdServiceRequest}
 */
CarClass(CWifiP2pUpnpServiceRequest), public WifiP2pServiceRequest
{
public:
    PInterface Probe(
        /* [in] */ REIID riid);

    CARAPI GetTransactionId(
        /* [out] */ Int32* transactionId);

    CARAPI SetTransactionId(
        /* [in] */ Int32 id);

    CARAPI GetSupplicantQuery(
        /* [out] */ String* supplicantQuery);

    CARAPI Equals(
        /* [in] */ IInterface* obj,
        /* [out] */ Boolean* isEqual);

    CARAPI GetHashCode(
        /* [out] */ Int32* hashCode);

    CARAPI ReadFromParcel(
        /* [in] */ IParcel* source);

    CARAPI WriteToParcel(
        /* [in] */ IParcel* dest);

    CARAPI constructor(
        /* [in] */ const String& query);

    CARAPI constructor();

public:
    static CARAPI NewInstance(
        /* [out] */ IWifiP2pUpnpServiceRequest** instance);

    static CARAPI NewInstance(
        /* [in] */ const String& st,
        /* [out] */ IWifiP2pUpnpServiceRequest ** instance);

private:
};

}
}
}
}
}

#endif // __ELASTOS_DROID_NET_WIFI_P2P_NSD_CWIFIP2PUPNPSERVICEREQUEST_H__
