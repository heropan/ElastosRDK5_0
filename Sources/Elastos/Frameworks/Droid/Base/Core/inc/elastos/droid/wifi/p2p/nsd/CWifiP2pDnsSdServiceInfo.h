
#ifndef __ELASTOS_DROID_NET_WIFI_P2P_NSD_CWIFIP2PDNSSDSERVICEINFO_H__
#define __ELASTOS_DROID_NET_WIFI_P2P_NSD_CWIFIP2PDNSSDSERVICEINFO_H__

#include "_Elastos_Droid_Net_Wifi_P2p_Nsd_CWifiP2pDnsSdServiceInfo.h"
#include "WifiP2pServiceInfo.h"
#include "elastos/droid/ext/frameworkext.h"
#include <elastos/utility/etl/HashMap.h>

using Elastos::Utility::Etl::HashMap;
using Elastos::Droid::Net::Nsd::IDnsSdTxtRecord;

namespace Elastos {
namespace Droid {
namespace Wifi {
namespace P2p {
namespace Nsd {

/**
 * A class for storing Bonjour service information that is advertised
 * over a Wi-Fi peer-to-peer setup.
 *
 * {@see android.net.wifi.p2p.WifiP2pManager#addLocalService}
 * {@see android.net.wifi.p2p.WifiP2pManager#removeLocalService}
 * {@see WifiP2pServiceInfo}
 * {@see WifiP2pUpnpServiceInfo}
 */
CarClass(CWifiP2pDnsSdServiceInfo), public WifiP2pServiceInfo
{
public:
    PInterface Probe(
        /* [in] */ REIID riid);

    CARAPI constructor();

    CARAPI constructor(
        /* [in] */ ArrayOf<String>* queryList);

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
        /* [in] */ const String& instanceName,
        /* [in] */ const String& serviceType,
        /* [in] */ IObjectStringMap* txtMap, // Map<String, String>
        /* [out] */ IWifiP2pDnsSdServiceInfo** instance);

public:
    /**
     * Create wpa_supplicant service query for PTR record.
     *
     * @param instanceName instance name.<br>
     *  e.g) "MyPrinter"
     * @param serviceType service type.<br>
     *  e.g) "_ipp._tcp"
     * @return wpa_supplicant service query.
     */
    static CARAPI CreatePtrServiceQuery(
        /* [in] */ const String& instanceName,
        /* [in] */ const String& serviceType,
        /* [out] */ String* query);

    /**
     * Create wpa_supplicant service query for TXT record.
     *
     * @param instanceName instance name.<br>
     *  e.g) "MyPrinter"
     * @param serviceType service type.<br>
     *  e.g) "_ipp._tcp"
     * @param txtRecord TXT record.<br>
     * @return wpa_supplicant service query.
     */
    static CARAPI CreateTxtServiceQuery(
        /* [in] */ const String& instanceName,
        /* [in] */ const String& serviceType,
        /* [in] */ IDnsSdTxtRecord* txtRecord,
        /* [out] */ String* query);

    /**
     * Create bonjour service discovery request.
     *
     * @param dnsName dns name
     * @param dnsType dns type
     * @param version version number
     * @hide
     */
    static CARAPI CreateRequest(
        /* [in] */ const String& dnsName,
        /* [in] */ const Int32 dnsType,
        /* [in] */ const Int32 versionNumber,
        /* [out] */ String* request);

    /**
     * Compress DNS data.
     *
     * see E.3 of the Wi-Fi Direct technical specification for the detail.
     *
     * @param dnsName dns name
     * @return compressed dns name
     */
    static CARAPI CompressDnsName(
        /* [in] */ const String& dnsName,
        /* [out] */ String* name);

private:
    /**
     * virtual memory packet.
     * see E.3 of the Wi-Fi Direct technical specification for the detail.<br>
     * Key: domain name Value: pointer address.<br>
     */
    class CStatic
    {
    public:
        CStatic()
        {
            mVmPacket[String("_tcp.local.")] = String("c00c");
            mVmPacket[String("local.")] = String("c011");
            mVmPacket[String("_udp.local.")] = String("c01c");
        }
    public:
        HashMap<String, String> mVmPacket;
    };

    static CStatic sStatic;
};

}
}
}
}
}
#endif // __ELASTOS_DROID_NET_WIFI_P2P_NSD_CWIFIP2PDNSSDSERVICEINFO_H__
