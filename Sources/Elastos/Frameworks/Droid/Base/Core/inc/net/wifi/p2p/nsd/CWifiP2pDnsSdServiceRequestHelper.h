
#ifndef __ELASTOS_DROID_NET_WIFI_P2P_NSD_CWIFIP2PDNSSDSERVICEREQUESTHELPER_H__
#define __ELASTOS_DROID_NET_WIFI_P2P_NSD_CWIFIP2PDNSSDSERVICEREQUESTHELPER_H__

#include "_Elastos_Droid_Net_Wifi_P2p_Nsd_CWifiP2pDnsSdServiceRequestHelper.h"

namespace Elastos {
namespace Droid {
namespace Net {
namespace Wifi {
namespace P2p {
namespace Nsd {

CarClass(CWifiP2pDnsSdServiceRequestHelper)
{
public:
    CARAPI NewInstance(
        /* [out] */ IWifiP2pDnsSdServiceRequest** instance);

    CARAPI NewInstance(
        /* [in] */ const String& serviceType,
        /* [out] */ IWifiP2pDnsSdServiceRequest** instance);

    CARAPI NewInstance(
        /* [in] */ const String& instanceName,
        /* [in] */ const String& serviceType,
        /* [out] */ IWifiP2pDnsSdServiceRequest** instance);

private:
};

}
}
}
}
}
}
#endif // __ELASTOS_DROID_NET_WIFI_P2P_NSD_CWIFIP2PDNSSDSERVICEREQUESTHELPER_H__
