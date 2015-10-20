
#include "CWifiP2pDnsSdServiceInfoHelper.h"
#include "CWifiP2pDnsSdServiceInfo.h"

namespace Elastos {
namespace Droid {
namespace Wifi {
namespace P2p {
namespace Nsd {

ECode CWifiP2pDnsSdServiceInfoHelper::NewInstance(
    /* [in] */ const String& instanceName,
    /* [in] */ const String& serviceType,
    /* [in] */ IObjectStringMap* txtMap,
    /* [out] */ IWifiP2pDnsSdServiceInfo** instance)
{
    return CWifiP2pDnsSdServiceInfo::NewInstance(
        instanceName, serviceType, txtMap, instance);
}

ECode CWifiP2pDnsSdServiceInfoHelper::CreateRequest(
    /* [in] */ const String& dnsName,
    /* [in] */ const Int32 dnsType,
    /* [in] */ const Int32 versionNumber,
    /* [out] */ String* request)
{
    return CWifiP2pDnsSdServiceInfo::CreateRequest(
        dnsName, dnsType, versionNumber, request);
}

}
}
}
}
}
