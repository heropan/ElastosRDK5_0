
#ifndef __CWIFIP2PDNSSDSERVICEINFOHELPER_H__
#define __CWIFIP2PDNSSDSERVICEINFOHELPER_H__

#include "_CWifiP2pDnsSdServiceInfoHelper.h"

using Elastos::Utility::IObjectStringMap;

namespace Elastos {
namespace Droid {
namespace Net {
namespace Wifi {
namespace P2p {
namespace Nsd {

CarClass(CWifiP2pDnsSdServiceInfoHelper)
{
public:
    CARAPI NewInstance(
        /* [in] */ const String& instanceName,
        /* [in] */ const String& serviceType,
        /* [in] */ IObjectStringMap* txtMap,
        /* [out] */ IWifiP2pDnsSdServiceInfo** instance);

    CARAPI CreateRequest(
        /* [in] */ const String& dnsName,
        /* [in] */ const Int32 dnsType,
        /* [in] */ const Int32 versionNumber,
        /* [out] */ String* request);
};

}
}
}
}
}
}
#endif // __CWIFIP2PDNSSDSERVICEINFOHELPER_H__
