
#include "CWifiP2pServiceRequestHelper.h"
#include "CWifiP2pServiceRequest.h"

namespace Elastos {
namespace Droid {
namespace Wifi {
namespace P2p {
namespace Nsd {

ECode CWifiP2pServiceRequestHelper::NewInstance(
    /* [in] */ Int32 protocolType,
    /* [in] */ const String& queryData,
    /* [out] */ IWifiP2pServiceRequest** instance)
{
    return CWifiP2pServiceRequest::NewInstance(protocolType, queryData, instance);
}

ECode CWifiP2pServiceRequestHelper::NewInstance(
    /* [in] */ Int32 protocolType,
    /* [out] */ IWifiP2pServiceRequest** instance)
{
    return CWifiP2pServiceRequest::NewInstance(protocolType, String(NULL), instance);
}

}
}
}
}
}
