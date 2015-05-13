
#ifndef __CWIFIP2PSERVICERESPONSESTATUS_H__
#define __CWIFIP2PSERVICERESPONSESTATUS_H__

#include "_CWifiP2pServiceResponseStatus.h"

namespace Elastos {
namespace Droid {
namespace Net {
namespace Wifi {
namespace P2p {
namespace Nsd {

CarClass(CWifiP2pServiceResponseStatus)
{
public:
    CARAPI ToString(
        /* [in] */ Int32 status,
        /* [out] */ String * pString);

private:
    // TODO: Add your private member variables here.
};

}
}
}
}
}
}
#endif // __CWIFIP2PSERVICERESPONSESTATUS_H__
