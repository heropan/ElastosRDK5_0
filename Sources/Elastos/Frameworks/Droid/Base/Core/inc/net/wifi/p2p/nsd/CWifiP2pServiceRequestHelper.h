
#ifndef __CWIFIP2PSERVICEREQUESTHELPER_H__
#define __CWIFIP2PSERVICEREQUESTHELPER_H__

#include "_CWifiP2pServiceRequestHelper.h"

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

    CARAPI NewInstanceEx(
        /* [in] */ Int32 protocolType,
        /* [out] */ IWifiP2pServiceRequest** instance);

private:
    // TODO: Add your private member variables here.
};

}
}
}
}
}
}
#endif // __CWIFIP2PSERVICEREQUESTHELPER_H__
