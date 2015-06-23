
#ifndef __CWIFIP2PDNSSDSERVICEREQUESTHELPER_H__
#define __CWIFIP2PDNSSDSERVICEREQUESTHELPER_H__

#include "_CWifiP2pDnsSdServiceRequestHelper.h"

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
    // TODO: Add your private member variables here.
};

}
}
}
}
}
}
#endif // __CWIFIP2PDNSSDSERVICEREQUESTHELPER_H__
