
#include "CWifiP2pServiceResponseStatus.h"
#include <ext/frameworkext.h>

namespace Elastos {
namespace Droid {
namespace Net {
namespace Wifi {
namespace P2p {
namespace Nsd {

ECode CWifiP2pServiceResponseStatus::ToString(
    /* [in] */ Int32 status,
    /* [out] */ String* string)
{
    VALIDATE_NOT_NULL(string);

    switch(status) {
        case IWifiP2pServiceResponseStatus::SUCCESS:
            *string = String("SUCCESS");
            break;
        case IWifiP2pServiceResponseStatus::SERVICE_PROTOCOL_NOT_AVAILABLE:
            *string = String("SERVICE_PROTOCOL_NOT_AVAILABLE");
            break;
        case IWifiP2pServiceResponseStatus::REQUESTED_INFORMATION_NOT_AVAILABLE:
            *string = String("REQUESTED_INFORMATION_NOT_AVAILABLE");
            break;
        case IWifiP2pServiceResponseStatus::BAD_REQUEST:
            *string = String("BAD_REQUEST");
            break;
        default:
            *string = String("UNKNOWN");
            break;
    }
    return NOERROR;
}

}
}
}
}
}
}
