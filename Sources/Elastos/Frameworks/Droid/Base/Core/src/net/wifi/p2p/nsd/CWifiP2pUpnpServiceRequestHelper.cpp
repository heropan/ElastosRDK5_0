
#include "CWifiP2pUpnpServiceRequestHelper.h"
#include "CWifiP2pUpnpServiceRequest.h"

namespace Elastos {
namespace Droid {
namespace Net {
namespace Wifi {
namespace P2p {
namespace Nsd {

ECode CWifiP2pUpnpServiceRequestHelper::NewInstance(
    /* [out] */ IWifiP2pUpnpServiceRequest** instance)
{
    return CWifiP2pUpnpServiceRequest::NewInstance(instance);
}

ECode CWifiP2pUpnpServiceRequestHelper::NewInstance(
    /* [in] */ const String& st,
    /* [out] */ IWifiP2pUpnpServiceRequest** instance)
{
    return CWifiP2pUpnpServiceRequest::NewInstance(st, instance);
}

}
}
}
}
}
}
