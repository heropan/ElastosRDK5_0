
#include "CWifiP2pUpnpServiceInfoHelper.h"
#include "CWifiP2pUpnpServiceInfo.h"

namespace Elastos {
namespace Droid {
namespace Net {
namespace Wifi {
namespace P2p {
namespace Nsd {

ECode CWifiP2pUpnpServiceInfoHelper::NewInstance(
    /* [in] */ const String& uuid,
    /* [in] */ const String& device,
    /* [in] */ ArrayOf<String>* services,
    /* [out] */ IWifiP2pUpnpServiceInfo** instance)
{
    return CWifiP2pUpnpServiceInfo::NewInstance(uuid, device, services, instance);
}

}
}
}
}
}
}
