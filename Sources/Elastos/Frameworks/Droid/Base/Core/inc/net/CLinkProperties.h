
#ifndef __ELASTOS_DROID_NET_CLINKPROPERTIES_H__
#define __ELASTOS_DROID_NET_CLINKPROPERTIES_H__

#include "_Elastos_Droid_Net_CLinkProperties.h"
#include "ext/frameworkext.h"
#include <elastos/utility/etl/List.h>

using Elastos::Utility::Etl::List;
using Elastos::Net::IInetAddress;

namespace Elastos {
namespace Droid {
namespace Net {

CarClass(CLinkProperties)
{
public:
    CARAPI constructor();

    CARAPI constructor(
        /* [in] */ ILinkProperties* source);

    CARAPI SetInterfaceName(
        /* [in] */ const String& iface);

    CARAPI GetInterfaceName(
        /* [out] */ String* result);

    CARAPI GetAddresses(
        /* [out] */ IObjectContainer** result);

    CARAPI AddLinkAddress(
        /* [in] */ ILinkAddress* address);

    CARAPI GetLinkAddresses(
        /* [out] */ IObjectContainer** result);

    CARAPI AddDns(
        /* [in] */ IInetAddress* dns);

    CARAPI GetDnses(
        /* [out] */ IObjectContainer** result);

    CARAPI AddRoute(
        /* [in] */ IRouteInfo* route);

    CARAPI GetRoutes(
        /* [out] */ IObjectContainer** result);

    CARAPI SetHttpProxy(
        /* [in] */ IProxyProperties* proxy);

    CARAPI GetHttpProxy(
        /* [out] */ IProxyProperties** result);

    CARAPI Clear();

    CARAPI ToString(
        /* [out] */ String* result);

    CARAPI IsIdenticalInterfaceName(
        /* [in] */ ILinkProperties* target,
        /* [out] */ Boolean* result);

    CARAPI IsIdenticalAddresses(
        /* [in] */ ILinkProperties* target,
        /* [out] */ Boolean* result);

    CARAPI IsIdenticalDnses(
        /* [in] */ ILinkProperties* target,
        /* [out] */ Boolean* result);

    CARAPI IsIdenticalRoutes(
        /* [in] */ ILinkProperties* target,
        /* [out] */ Boolean* result);

    CARAPI IsIdenticalHttpProxy(
        /* [in] */ ILinkProperties* target,
        /* [out] */ Boolean* result);

    CARAPI Equals(
        /* [in] */ ILinkProperties* target,
        /* [out] */ Boolean* result);

    CARAPI Equals(
        /* [in] */ IInterface* target,
        /* [out] */ Boolean* result);

    CARAPI GetHashCode(
        /* [out] */ Int32* result);

    CARAPI CompareAddresses(
        /* [in] */ ILinkProperties* target,
        /* [out] */ ICompareResult** result);

    CARAPI CompareDnses(
        /* [in] */ ILinkProperties* target,
        /* [out] */ ICompareResult** result);

    CARAPI CompareRoutes(
        /* [in] */ ILinkProperties* target,
        /* [out] */ ICompareResult** result);

    CARAPI WriteToParcel(
        /* [in] */ IParcel* dest);

    CARAPI ReadFromParcel(
        /* [in] */ IParcel* source);

private:
    String mIfaceName;
    List< AutoPtr<ILinkAddress> > mLinkAddresses;
    List< AutoPtr<IInetAddress> > mDnses;
    List< AutoPtr<IRouteInfo> > mRoutes;
    AutoPtr<IProxyProperties> mHttpProxy;
};

} // namespace Net
} // namepsace Droid
} // namespace Elastos

#endif // __ELASTOS_DROID_NET_CLINKPROPERTIES_H__

