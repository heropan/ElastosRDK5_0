
#ifndef __DHCPINFOINTERNAL_H__
#define __DHCPINFOINTERNAL_H__

#include "ext/frameworkext.h"
#include <elastos/utility/etl/List.h>

using Elastos::Utility::Etl::List;

namespace Elastos {
namespace Droid {
namespace Net {

/**
 * A simple object for retrieving the results of a DHCP request.
 * Replaces (internally) the IPv4-only DhcpInfo class.
 * @hide
 */
class DhcpInfoInternal
    : public ElRefBase
    , public IInterface
{
public:
    DhcpInfoInternal();

    CAR_INTERFACE_DECL()

    CARAPI_(void) AddRoute(
        /* [in] */ IRouteInfo* routeInfo);

    CARAPI_(List< AutoPtr<IRouteInfo> > &) GetRoutes();

    CARAPI_(AutoPtr<IDhcpInfo>) MakeDhcpInfo();

    CARAPI_(AutoPtr<ILinkAddress>) MakeLinkAddress();

    CARAPI_(AutoPtr<ILinkProperties>) MakeLinkProperties();

    /* Updates the DHCP fields that need to be retained from
     * original DHCP request if the DHCP renewal shows them as
     * being empty
     */
    CARAPI_(void) UpdateFromDhcpRequest(
        /* [in] */ DhcpInfoInternal* orig);

    /**
     * Test if this DHCP lease includes vendor hint that network link is
     * metered, and sensitive to heavy data transfers.
     */
    CARAPI_(Boolean) HasMeteredHint();

    CARAPI_(String) ToString();

private:
    CARAPI_(Int32) ConvertToInt(
        /* [in] */ const String& addr);

public:
    String mIpAddress;
    Int32 mPrefixLength;

    String mDns1;
    String mDns2;

    String mServerAddress;
    Int32 mLeaseDuration;

    /**
     * Vendor specific information (from RFC 2132).
     */
    String mVendorInfo;

private:
    static const String TAG;

    List< AutoPtr<IRouteInfo> > mRoutes;
};

} // namespace Net
} // namepsace Droid
} // namespace Elastos

#endif //__DHCPINFOINTERNAL_H__
