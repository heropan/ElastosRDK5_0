
#ifndef __ELASTOS_DROID_NET_WIFI_P2P_NSD_CWIFIP2PSERVICEINFOBASE_H__
#define __ELASTOS_DROID_NET_WIFI_P2P_NSD_CWIFIP2PSERVICEINFOBASE_H__

#include "elastos/droid/ext/frameworkext.h"

namespace Elastos {
namespace Droid {
namespace Net {
namespace Wifi {
namespace P2p {
namespace Nsd {

extern "C" const InterfaceID EIID_WifiP2pServiceInfo;

class WifiP2pServiceInfo
{
public:
    virtual CARAPI_(PInterface) Probe(
        /* [in] */ REIID riid) = 0;

    virtual CARAPI GetSupplicantQueryList(
        /* [out, callee] */ ArrayOf<String>** list);

    virtual CARAPI Equals(
        /* [in] */ IInterface* obj,
        /* [out] */ Boolean* isEqual);

    virtual CARAPI GetHashCode(
        /* [out] */ Int32* hashCode);

    virtual CARAPI ReadFromParcel(
        /* [in] */ IParcel* source);

    virtual CARAPI WriteToParcel(
        /* [in] */ IParcel* dest);

    virtual CARAPI Init(
        /* [in] */ ArrayOf<String>* queryList);

public:
    /**
     * Converts byte array to hex string.
     *
     * @param data
     * @return hex string.
     * @hide
     */
    static CARAPI Bin2HexStr(
        /* [in] */ const ArrayOf<Byte>& data,
        /* [out] */ String* hexStr);

    static CARAPI Bin2HexStr(
        /* [in] */ const char * data,
        /* [in] */ Int32 length,
        /* [out] */ String* hexStr);

public:
    /**
     * the list of query string for wpa_supplicant
     *
     * e.g)
     * # IP Printing over TCP (PTR) (RDATA=MyPrinter._ipp._tcp.local.)
     * {"bonjour", "045f697070c00c000c01", "094d795072696e746572c027"
     *
     * # IP Printing over TCP (TXT) (RDATA=txtvers=1,pdl=application/postscript)
     * {"bonjour", "096d797072696e746572045f697070c00c001001",
     *  "09747874766572733d311a70646c3d6170706c69636174696f6e2f706f7374736372797074"}
     *
     * [UPnP]
     * # UPnP uuid
     * {"upnp", "10", "uuid:6859dede-8574-59ab-9332-123456789012"}
     *
     * # UPnP rootdevice
     * {"upnp", "10", "uuid:6859dede-8574-59ab-9332-123456789012::upnp:rootdevice"}
     *
     * # UPnP device
     * {"upnp", "10", "uuid:6859dede-8574-59ab-9332-123456789012::urn:schemas-upnp
     * -org:device:InternetGatewayDevice:1"}
     *
     *  # UPnP service
     * {"upnp", "10", "uuid:6859dede-8574-59ab-9322-123456789012::urn:schemas-upnp
     * -org:service:ContentDirectory:2"}
     */
    AutoPtr<ArrayOf<String> > mQueryList;
};

}
}
}
}
}
}
#endif // __ELASTOS_DROID_NET_WIFI_P2P_NSD_CWIFIP2PSERVICEINFOBASE_H__
