
#ifndef __CWIFIP2PSERVICERESPONSEHELPER_H__
#define __CWIFIP2PSERVICERESPONSEHELPER_H__

#include "_CWifiP2pServiceResponseHelper.h"

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
#endif // __CWIFIP2PSERVICERESPONSEHELPER_H__
