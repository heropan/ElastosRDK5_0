
#ifndef __CWIFIP2PUPNPSERVICEINFOHELPER_H__
#define __CWIFIP2PUPNPSERVICEINFOHELPER_H__

#include "_CWifiP2pUpnpServiceInfoHelper.h"

namespace Elastos {
namespace Droid {
namespace Net {
namespace Wifi {
namespace P2p {
namespace Nsd {

CarClass(CWifiP2pUpnpServiceInfoHelper)
{
public:
    CARAPI NewInstance(
        /* [in] */ const String& uuid,
        /* [in] */ const String& device,
        /* [in] */ ArrayOf<String>* services,
        /* [out] */ IWifiP2pUpnpServiceInfo** instance);
};

}
}
}
}
}
}
#endif // __CWIFIP2PUPNPSERVICEINFOHELPER_H__
