
#ifndef __ELASTOS_DROID_NET_WIFI_P2P_NSD_CWIFIP2PUPNPSERVICEREQUESTHELPER_H__
#define __ELASTOS_DROID_NET_WIFI_P2P_NSD_CWIFIP2PUPNPSERVICEREQUESTHELPER_H__

#include "_Elastos_Droid_Net_Wifi_P2p_Nsd_CWifiP2pUpnpServiceRequestHelper.h"

namespace Elastos {
namespace Droid {
namespace Net {
namespace Wifi {
namespace P2p {
namespace Nsd {

CarClass(CWifiP2pUpnpServiceRequestHelper)
{
public:
    CARAPI NewInstance(
        /* [out] */ IWifiP2pUpnpServiceRequest** instance);

    CARAPI NewInstance(
        /* [in] */ const String& st,
        /* [out] */ IWifiP2pUpnpServiceRequest** instance);
};

}
}
}
}
}
}
#endif // __ELASTOS_DROID_NET_WIFI_P2P_NSD_CWIFIP2PUPNPSERVICEREQUESTHELPER_H__
