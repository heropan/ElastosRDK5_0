
#ifndef __CWIFIP2PUPNPSERVICERESPONSEHELPER_H__
#define __CWIFIP2PUPNPSERVICERESPONSEHELPER_H__

#include "_CWifiP2pUpnpServiceResponseHelper.h"

using Elastos::Droid::Net::Wifi::P2p::IWifiP2pDevice;

namespace Elastos {
namespace Droid {
namespace Net {
namespace Wifi {
namespace P2p {
namespace Nsd {

CarClass(CWifiP2pUpnpServiceResponseHelper)
{
public:
    CARAPI NewInstance(
        /* [in] */ Int32 status,
        /* [in] */ Int32 transId,
        /* [in] */ IWifiP2pDevice* dev,
        /* [in] */ ArrayOf<Byte>* data,
        /* [out] */ IWifiP2pUpnpServiceResponse** instance);
};

}
}
}
}
}
}

#endif // __CWIFIP2PUPNPSERVICERESPONSEHELPER_H__
