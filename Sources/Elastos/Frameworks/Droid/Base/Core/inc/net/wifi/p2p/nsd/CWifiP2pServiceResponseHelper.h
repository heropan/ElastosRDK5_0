
#ifndef __ELASTOS_DROID_NET_WIFI_P2P_NSD_CWIFIP2PSERVICERESPONSEHELPER_H__
#define __ELASTOS_DROID_NET_WIFI_P2P_NSD_CWIFIP2PSERVICERESPONSEHELPER_H__

#include "_Elastos_Droid_Net_Wifi_P2p_Nsd_CWifiP2pServiceResponseHelper.h"

namespace Elastos {
namespace Droid {
namespace Net {
namespace Wifi {
namespace P2p {
namespace Nsd {

CarClass(CWifiP2pServiceResponseHelper)
{
public:
    CARAPI NewInstance(
        /* [in] */ const String& supplicantEvent,
        /* [out, callee] */ ArrayOf<IWifiP2pServiceResponse*>** list);
};

}
}
}
}
}
}
#endif // __ELASTOS_DROID_NET_WIFI_P2P_NSD_CWIFIP2PSERVICERESPONSEHELPER_H__
