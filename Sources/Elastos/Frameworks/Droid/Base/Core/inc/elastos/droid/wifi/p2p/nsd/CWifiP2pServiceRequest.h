
#ifndef __ELASTOS_DROID_NET_WIFI_P2P_NSD_CWIFIP2PSERVICEREQUEST_H__
#define __ELASTOS_DROID_NET_WIFI_P2P_NSD_CWIFIP2PSERVICEREQUEST_H__

#include "_Elastos_Droid_Net_Wifi_P2p_Nsd_CWifiP2pServiceRequest.h"
#include "WifiP2pServiceRequest.h"

namespace Elastos {
namespace Droid {
namespace Wifi {
namespace P2p {
namespace Nsd {

CarClass(CWifiP2pServiceRequest), public WifiP2pServiceRequest
{
public:
    virtual CARAPI_(PInterface) Probe(
        /* [in] */ REIID riid);

    CARAPI constructor();

    CARAPI constructor(
        /* [in] */ Int32 protocolType,
        /* [in] */ const String& query);

    CARAPI constructor(
        /* [in] */ Int32 serviceType,
        /* [in] */ Int32 length,
        /* [in] */ Int32 transId,
        /* [in] */ const String& query);

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

    static CARAPI NewInstance(
        /* [in] */ Int32 protocolType,
        /* [in] */ const String& queryData,
        /* [out] */ IWifiP2pServiceRequest** instance);

    static CARAPI NewInstance(
        /* [in] */ Int32 protocolType,
        /* [out] */ IWifiP2pServiceRequest** instance);
private:
};

}
}
}
}
}
#endif // __ELASTOS_DROID_NET_WIFI_P2P_NSD_CWIFIP2PSERVICEREQUEST_H__
