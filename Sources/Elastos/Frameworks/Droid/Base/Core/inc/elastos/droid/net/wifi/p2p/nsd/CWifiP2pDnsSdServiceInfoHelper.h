
#ifndef __ELASTOS_DROID_NET_WIFI_P2P_NSD_CWIFIP2PDNSSDSERVICEINFOHELPER_H__
#define __ELASTOS_DROID_NET_WIFI_P2P_NSD_CWIFIP2PDNSSDSERVICEINFOHELPER_H__

#include "_Elastos_Droid_Net_Wifi_P2p_Nsd_CWifiP2pDnsSdServiceInfoHelper.h"

using Elastos::Utility::IObjectStringMap;

namespace Elastos {
namespace Droid {
namespace Net {
namespace Wifi {
namespace P2p {
namespace Nsd {

CarClass(CWifiP2pDnsSdServiceInfoHelper)
{
public:
    CARAPI NewInstance(
        /* [in] */ const String& instanceName,
        /* [in] */ const String& serviceType,
        /* [in] */ IObjectStringMap* txtMap,
        /* [out] */ IWifiP2pDnsSdServiceInfo** instance);

    CARAPI CreateRequest(
        /* [in] */ const String& dnsName,
        /* [in] */ const Int32 dnsType,
        /* [in] */ const Int32 versionNumber,
        /* [out] */ String* request);
};

}
}
}
}
}
}
#endif // __ELASTOS_DROID_NET_WIFI_P2P_NSD_CWIFIP2PDNSSDSERVICEINFOHELPER_H__
