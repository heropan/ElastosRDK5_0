
#include "CWifiP2pDnsSdServiceRequestHelper.h"
#include "CWifiP2pDnsSdServiceRequest.h"

namespace Elastos {
namespace Droid {
namespace Net {
namespace Wifi {
namespace P2p {
namespace Nsd {

ECode CWifiP2pDnsSdServiceRequestHelper::NewInstance(
    /* [out] */ IWifiP2pDnsSdServiceRequest** instance)
{
    return CWifiP2pDnsSdServiceRequest::NewInstance(instance);
}

ECode CWifiP2pDnsSdServiceRequestHelper::NewInstance(
    /* [in] */ const String& serviceType,
    /* [out] */ IWifiP2pDnsSdServiceRequest** instance)
{
    return CWifiP2pDnsSdServiceRequest::NewInstance(serviceType, instance);
}

ECode CWifiP2pDnsSdServiceRequestHelper::NewInstance(
    /* [in] */ const String& instanceName,
    /* [in] */ const String& serviceType,
    /* [out] */ IWifiP2pDnsSdServiceRequest** instance)
{
    return CWifiP2pDnsSdServiceRequest::NewInstance(
        instanceName, serviceType, instance);
}

}
}
}
}
}
}
