
#ifndef __CWIFIP2PUPNPSERVICEREQUESTHELPER_H__
#define __CWIFIP2PUPNPSERVICEREQUESTHELPER_H__

#include "_CWifiP2pUpnpServiceRequestHelper.h"

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
#endif // __CWIFIP2PUPNPSERVICEREQUESTHELPER_H__
