
#ifndef __ELASTOS_DROID_NET_WIFI_P2P_NSD_CWIFIP2PSERVICEREQUESTBASE_H__
#define __ELASTOS_DROID_NET_WIFI_P2P_NSD_CWIFIP2PSERVICEREQUESTBASE_H__

#include "elastos/droid/ext/frameworkext.h"

namespace Elastos {
namespace Droid {
namespace Net {
namespace Wifi {
namespace P2p {
namespace Nsd {

extern "C" const InterfaceID EIID_WifiP2pServiceRequest;

class WifiP2pServiceRequest
{
public:
    WifiP2pServiceRequest();

    virtual CARAPI_(PInterface) Probe(
        /* [in] */ REIID riid) = 0;

    virtual CARAPI Init(
        /* [in] */ Int32 protocolType,
        /* [in] */ const String& query);

    virtual CARAPI Init(
        /* [in] */ Int32 serviceType,
        /* [in] */ Int32 length,
        /* [in] */ Int32 transId,
        /* [in] */ const String& query);

    virtual CARAPI GetTransactionId(
        /* [out] */ Int32* transactionId);

    virtual CARAPI SetTransactionId(
        /* [in] */ Int32 id);

    virtual CARAPI GetSupplicantQuery(
        /* [out] */ String* supplicantQuery);

    virtual CARAPI GetHashCode(
        /* [out] */ Int32* hashCode);

    virtual CARAPI ReadFromParcel(
        /* [in] */ IParcel* source);

    virtual CARAPI WriteToParcel(
        /* [in] */ IParcel* dest);

    virtual CARAPI Equals(
        /* [in] */ IInterface* other,
        /* [out] */ Boolean* result);

private:
    CARAPI ValidateQuery(
        /* [in] */ const String& query);

public:
    /**
     * Service discovery protocol. It's defined in table63 in Wi-Fi Direct specification.
     */
    Int32 mProtocolType;

    /**
     * The length of the service request TLV.
     * The value is equal to 2 plus the number of octets in the
     * query data field.
     */
    Int32 mLength;

    /**
     * Service transaction ID.
     * This is a nonzero value used to match the service request/response TLVs.
     */
    Int32 mTransId;

    /**
     * The hex dump string of query data for the requested service information.
     *
     * e.g) DnsSd apple file sharing over tcp (dns name=_afpovertcp._tcp.local.)
     * 0b5f6166706f766572746370c00c000c01
     */
    String mQuery;
};

}
}
}
}
}
}

#endif // __ELASTOS_DROID_NET_WIFI_P2P_NSD_CWIFIP2PSERVICEREQUESTBASE_H__
