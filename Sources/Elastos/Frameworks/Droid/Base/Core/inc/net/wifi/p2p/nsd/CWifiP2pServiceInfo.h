
#ifndef __CWIFIP2PSERVICEINFO_H__
#define __CWIFIP2PSERVICEINFO_H__

#include "_CWifiP2pServiceInfo.h"
#include "WifiP2pServiceInfo.h"

namespace Elastos {
namespace Droid {
namespace Net {
namespace Wifi {
namespace P2p {
namespace Nsd {

CarClass(CWifiP2pServiceInfo), public WifiP2pServiceInfo
{
public:
    PInterface Probe(
        /* [in] */ REIID riid);

    CARAPI constructor();

    CARAPI constructor(
        /* [in] */ ArrayOf<String>* queryList);

    CARAPI GetSupplicantQueryList(
        /* [out, callee] */ ArrayOf<String>** list);

    CARAPI Equals(
        /* [in] */ IInterface* obj,
        /* [out] */ Boolean* isEqual);

    CARAPI GetHashCode(
        /* [out] */ Int32* hashCode);

    CARAPI ReadFromParcel(
        /* [in] */ IParcel* source);

    CARAPI WriteToParcel(
        /* [in] */ IParcel* dest);
};

}
}
}
}
}
}
#endif // __CWIFIP2PSERVICEINFO_H__
