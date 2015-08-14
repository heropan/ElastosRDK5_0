
#ifndef __ELASTOS_DROID_NET_CROUTEINFO_H__
#define __ELASTOS_DROID_NET_CROUTEINFO_H__

#include "ext/frameworkdef.h"
#include "_CRouteInfo.h"

using Elastos::Net::IInetAddress;
using Elastos::Net::IInet4Address;
using Elastos::Net::IInet6Address;
using Elastos::Droid::Net::ILinkAddress;

namespace Elastos {
namespace Droid {
namespace Net {

CarClass(CRouteInfo)
{
public:
    CRouteInfo();
    ~CRouteInfo();

    CARAPI constructor();

    CARAPI constructor(
        /* [in] */ ILinkAddress* destination,
        /* [in] */ IInetAddress* gateway);

    CARAPI constructor(
        /* [in] */ IInetAddress* gateway);

    CARAPI GetDestination(
        /* [out] */ ILinkAddress** address);

    CARAPI GetGateway(
        /* [out] */ IInetAddress** address);

    CARAPI IsDefaultRoute(
        /* [out] */ Boolean* value);

    CARAPI IsHostRoute(
        /* [out] */ Boolean* value);

    CARAPI ToString(
        /* [out] */ String* str);

    CARAPI WriteToParcel(
        /* [in] */ IParcel* dest);

    CARAPI ReadFromParcel(
        /* [in] */ IParcel* source);

    //@Override
    CARAPI Equals(
        /* [in] */ IInterface* obj,
        /* [out] */ Boolean* result);

    CARAPI Equals(
        /* [in] */ IRouteInfo* obj,
        /* [out] */ Boolean* result);

    //@Override
    CARAPI GetHashCode(
        /* [out] */ Int32* code);

    CARAPI Matches(
        /* [in] */ IInetAddress* destination,
        /* [out] */ Boolean* isMatch);

    static CARAPI MakeHostRoute(
        /* [in] */ IInetAddress* host,
        /* [out] */ IRouteInfo** info);

    static CARAPI MakeHostRoute(
        /* [in] */ IInetAddress* host,
        /* [in] */ IInetAddress* gateway,
        /* [out] */ IRouteInfo** info);

    static CARAPI SelectBestRoute(
        /* [in] */ IObjectContainer* routes,
        /* [in] */ IInetAddress* dest,
        /* [out] */ IRouteInfo** info);

private:
    CARAPI_(Boolean) IsHost();

    CARAPI_(Boolean) IsDefault();

private:
    /**
     * The IP destination address for this route.
     */
    AutoPtr<ILinkAddress> mDestination;

    /**
     * The gateway address for this route.
     */
    AutoPtr<IInetAddress> mGateway;

    Boolean mIsDefault;
    Boolean mIsHost;
};

} //namespace Net
} //namespace Droid
} //namespace Elastos

#endif // __ELASTOS_DROID_NET_CROUTEINFO_H__
