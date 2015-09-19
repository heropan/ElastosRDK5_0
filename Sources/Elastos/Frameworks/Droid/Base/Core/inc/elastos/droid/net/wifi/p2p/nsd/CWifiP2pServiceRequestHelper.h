
#ifndef __ELASTOS_DROID_NET_WIFI_P2P_NSD_CWIFIP2PSERVICEREQUESTHELPER_H__
#define __ELASTOS_DROID_NET_WIFI_P2P_NSD_CWIFIP2PSERVICEREQUESTHELPER_H__

#include "_Elastos_Droid_Net_Wifi_P2p_Nsd_CWifiP2pServiceRequestHelper.h"

namespace Elastos {
namespace Droid {
namespace Net {
namespace Wifi {
namespace P2p {
namespace Nsd {

CarClass(CWifiP2pServiceRequestHelper)
{
public:
    CARAPI NewInstance(
        /* [in] */ Int32 protocolType,
        /* [in] */ const String& queryData,
        /* [out] */ IWifiP2pServiceRequest** instance);

    CARAPI NewInstance(
        /* [in] */ Int32 protocolType,
        /* [out] */ IWifiP2pServiceRequest** instance);

private:
};

}
}
}
}
}
}
#endif // __ELASTOS_DROID_NET_WIFI_P2P_NSD_CWIFIP2PSERVICEREQUESTHELPER_H__
