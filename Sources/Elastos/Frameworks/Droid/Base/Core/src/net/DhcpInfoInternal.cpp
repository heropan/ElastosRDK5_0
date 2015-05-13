
#include "net/DhcpInfoInternal.h"
#ifdef DROID_CORE
#include "net/CDhcpInfo.h"
#include "net/CLinkAddress.h"
#include "net/CLinkProperties.h"
#endif
#include "net/NetworkUtils.h"
#include <elastos/Logger.h>
#include <elastos/StringBuilder.h>

using Elastos::Net::IInetAddress;
using Elastos::Net::IInet4Address;
using Elastos::Core::StringBuilder;
using Elastos::Utility::Logging::Logger;
using Elastos::Droid::Net::CDhcpInfo;
using Elastos::Droid::Net::CLinkAddress;
using Elastos::Droid::Net::CLinkProperties;
using Elastos::Droid::Net::NetworkUtils;

namespace Elastos {
namespace Droid {
namespace Net {


const String DhcpInfoInternal::TAG("DhcpInfoInternal");

DhcpInfoInternal::DhcpInfoInternal()
    : mIpAddress(String(NULL))
    , mPrefixLength(0)
    , mDns1(String(NULL))
    , mDns2(String(NULL))
    , mServerAddress(String(NULL))
    , mLeaseDuration(0)
    , mVendorInfo(String(NULL))
{}

CAR_INTERFACE_IMPL(DhcpInfoInternal, IInterface)

void DhcpInfoInternal::AddRoute(
    /* [in] */ IRouteInfo* routeInfo)
{
    mRoutes.PushBack(routeInfo);
}

List< AutoPtr<IRouteInfo> >& DhcpInfoInternal::GetRoutes()
{
    // return Collections.unmodifiableCollection(mRoutes);
    return mRoutes;
}

Int32 DhcpInfoInternal::ConvertToInt(
    /* [in] */ const String& addr)
{
    if (!addr.IsNull()) {
        AutoPtr<IInetAddress> inetAddr;
        NetworkUtils::NumericToInetAddress(addr, (IInetAddress**)&inetAddr);
        if (IInet4Address::Probe(inetAddr) != NULL) {
            Int32 result;
            NetworkUtils::InetAddressToInt32(inetAddr,&result);
            return result;
        }
    }
    return 0;
}

AutoPtr<IDhcpInfo> DhcpInfoInternal::MakeDhcpInfo()
{
    AutoPtr<IDhcpInfo> info;
    CDhcpInfo::New((IDhcpInfo**)&info);
    info->SetIpAddress(ConvertToInt(mIpAddress));
    List< AutoPtr<IRouteInfo> >::Iterator it;
    for (it = mRoutes.Begin(); it != mRoutes.End(); ++it) {
        AutoPtr<IRouteInfo> route = *it;
        Boolean b;
        if (route->IsDefaultRoute(&b), b) {
            AutoPtr<IInetAddress> inetAddr;
            route->GetGateway((IInetAddress**)&inetAddr);
            String addr;
            inetAddr->GetHostAddress(&addr);
            info->SetGateway(ConvertToInt(addr));
            break;
        }
    }
    AutoPtr<IInetAddress> inetAddress;
    NetworkUtils::NumericToInetAddress(mIpAddress, (IInetAddress**)&inetAddress);
    Int32 netmask;
    NetworkUtils::PrefixLengthToNetmaskInt(mPrefixLength,&netmask);
    info->SetNetmask(netmask);
    info->SetDns1(ConvertToInt(mDns1));
    info->SetDns2(ConvertToInt(mDns2));
    info->SetServerAddress(ConvertToInt(mServerAddress));
    info->SetLeaseDuration(mLeaseDuration);
    return info;
}

AutoPtr<ILinkAddress> DhcpInfoInternal::MakeLinkAddress()
{
    if (mIpAddress.IsNullOrEmpty()) {
        Logger::E(TAG, "makeLinkAddress with empty ipAddress");
        return NULL;
    }
    AutoPtr<IInetAddress> inetAddr;
    NetworkUtils::NumericToInetAddress(mIpAddress, (IInetAddress**)&inetAddr);
    AutoPtr<ILinkAddress> linkAddr;
    CLinkAddress::New(inetAddr, mPrefixLength, (ILinkAddress**)&linkAddr);
    return linkAddr;
}

AutoPtr<ILinkProperties> DhcpInfoInternal::MakeLinkProperties()
{
    AutoPtr<ILinkProperties> linkp;
    CLinkProperties::New((ILinkProperties**)&linkp);
    AutoPtr<ILinkAddress> address = MakeLinkAddress();
    linkp->AddLinkAddress(address);
    List< AutoPtr<IRouteInfo> >::Iterator it;
    for (it = mRoutes.Begin(); it != mRoutes.End(); ++it) {
        linkp->AddRoute(*it);
    }
    AutoPtr<IInetAddress> inetAddr;
    //if empty, connectivity configures default DNS
    if (!mDns1.IsNullOrEmpty()) {
        NetworkUtils::NumericToInetAddress(mDns1, (IInetAddress**)&inetAddr);
        linkp->AddDns(inetAddr);
    }
    else {
        Logger::D(TAG, "makeLinkProperties with empty dns1!");
    }

    if (!mDns2.IsNullOrEmpty()) {
        inetAddr = NULL;
        NetworkUtils::NumericToInetAddress(mDns2, (IInetAddress**)&inetAddr);
        linkp->AddDns(inetAddr);
    }
    else {
        Logger::D(TAG, "makeLinkProperties with empty dns2!");
    }
    return linkp;
}

/* Updates the DHCP fields that need to be retained from
 * original DHCP request if the DHCP renewal shows them as
 * being empty
 */
void DhcpInfoInternal::UpdateFromDhcpRequest(
    /* [in] */ DhcpInfoInternal* orig)
{
    if (orig == NULL) return;

    if (mDns1.IsNullOrEmpty()) {
        mDns1 = orig->mDns1;
    }

    if (mDns2.IsNullOrEmpty()) {
        mDns2 = orig->mDns2;
    }

    if (mRoutes.Begin() == mRoutes.End()) {
        List< AutoPtr<IRouteInfo> >& routes = orig->GetRoutes();
        List< AutoPtr<IRouteInfo> >::Iterator it;
        for (it = routes.Begin(); it != routes.End(); ++it) {
            AddRoute(*it);
        }
    }
}

/**
 * Test if this DHCP lease includes vendor hint that network link is
 * metered, and sensitive to heavy data transfers.
 */
Boolean DhcpInfoInternal::HasMeteredHint()
{
    if (!mVendorInfo.IsNull()) {
        return mVendorInfo.Contains("ANDROID_METERED");
    }
    else {
        return FALSE;
    }
}

String DhcpInfoInternal::ToString()
{
    String routeString("");
    List< AutoPtr<IRouteInfo> >::Iterator it;
    for (it = mRoutes.Begin(); it != mRoutes.End(); ++it) {
        String rString;
        (*it)->ToString(&rString);
        routeString += rString + " | ";
    }
    StringBuilder sb("addr: ");
    sb += mIpAddress;
    sb += "/";
    sb += mPrefixLength;
    sb += " mRoutes: ";
    sb += routeString;
    sb += " dns: ";
    sb += mDns1;
    sb += ",";
    sb += mDns2;
    sb += " dhcpServer: ";
    sb += mServerAddress;
    sb += " leaseDuration: ";
    sb += mLeaseDuration;

    return sb.ToString();
}

} // namespace Net
} // namepsace Droid
} // namespace Elastos
