
#ifndef __ELASTOS_DROID_NET_WIFI_P2P_NSD_CWIFIP2PSERVICERESPONSESTATUS_H__
#define __ELASTOS_DROID_NET_WIFI_P2P_NSD_CWIFIP2PSERVICERESPONSESTATUS_H__

#include "_CWifiP2pServiceResponseStatus.h"

namespace Elastos {
namespace Droid {
namespace Net {
namespace Wifi {
namespace P2p {
namespace Nsd {

CarClass(CWifiP2pServiceResponseStatus)
{
public:
    CARAPI ToString(
        /* [in] */ Int32 status,
        /* [out] */ String * pString);

private:
    // TODO: Add your private member variables here.
};

}
}
}
}
}
}
#endif // __ELASTOS_DROID_NET_WIFI_P2P_NSD_CWIFIP2PSERVICERESPONSESTATUS_H__
