#include "CWifiP2pServiceResponseHelper.h"
#include "CWifiP2pServiceResponse.h"


namespace Elastos {
namespace Droid {
namespace Wifi {
namespace P2p {
namespace Nsd {

ECode CWifiP2pServiceResponseHelper::NewInstance(
    /* [in] */ const String& supplicantEvent,
    /* [out, callee] */ ArrayOf<IWifiP2pServiceResponse*>** list)
{
    return CWifiP2pServiceResponse::NewInstance(supplicantEvent, list);
}

}
}
}
}
}
