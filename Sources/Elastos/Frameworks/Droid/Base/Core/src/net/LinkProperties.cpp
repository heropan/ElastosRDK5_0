
#include "LinkProperties.h"

namespace Elastos {
namespace Droid {
namespace Net {

//====================================================================================
//              LinkProperties
//====================================================================================
CAR_INTERFACE_IMPL_2(LinkProperties, Object, ILinkProperties, IParcelable)

const Int32 LinkProperties::sMIN_MTU = 68;
const Int32 LinkProperties::sMIN_MTU_V6 = 1280;
const Int32 LinkProperties::sMAX_MTU = 10000;

LinkProperties::LinkProperties()
{
#if 0 // TODO: Translate codes below
    private ArrayList<LinkAddress> mLinkAddresses = new ArrayList<LinkAddress>();
    private ArrayList<InetAddress> mDnses = new ArrayList<InetAddress>();
    private ArrayList<RouteInfo> mRoutes = new ArrayList<RouteInfo>();
    private Hashtable<String, LinkProperties> mStackedLinks =
        new Hashtable<String, LinkProperties>();
#endif
}

ECode LinkProperties::constructor()
{
    return NOERROR;
}

ECode LinkProperties::constructor(
    /* [in] */ ILinkProperties* source)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
        if (source != null) {
            mIfaceName = source.getInterfaceName();
            for (LinkAddress l : source.getLinkAddresses()) mLinkAddresses.add(l);
            for (InetAddress i : source.getDnsServers()) mDnses.add(i);
            mDomains = source.getDomains();
            for (RouteInfo r : source.getRoutes()) mRoutes.add(r);
            mHttpProxy = (source.getHttpProxy() == null)  ?
                    null : new ProxyInfo(source.getHttpProxy());
            for (LinkProperties l: source.mStackedLinks.values()) {
                addStackedLink(l);
            }
            setMtu(source.getMtu());
            mTcpBufferSizes = source.mTcpBufferSizes;
        }

#endif
}

ECode LinkProperties::SetInterfaceName(
    /* [in] */ const String& iface)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
        mIfaceName = iface;
        ArrayList<RouteInfo> newRoutes = new ArrayList<RouteInfo>(mRoutes.size());
        for (RouteInfo route : mRoutes) {
            newRoutes.add(routeWithInterface(route));
        }
        mRoutes = newRoutes;

#endif
}

ECode LinkProperties::GetInterfaceName(
    /* [out] */ String* result)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
        return mIfaceName;

#endif
}

ECode LinkProperties::GetAllInterfaceNames(
    /* [out] */ IList** result)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
        List<String> interfaceNames = new ArrayList<String>(mStackedLinks.size() + 1);
        if (mIfaceName != null) interfaceNames.add(new String(mIfaceName));
        for (LinkProperties stacked: mStackedLinks.values()) {
            interfaceNames.addAll(stacked.getAllInterfaceNames());
        }
        return interfaceNames;

#endif
}

ECode LinkProperties::GetAddresses(
    /* [out] */ IList** result)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
        List<InetAddress> addresses = new ArrayList<InetAddress>();
        for (LinkAddress linkAddress : mLinkAddresses) {
            addresses.add(linkAddress.getAddress());
        }
        return Collections.unmodifiableList(addresses);

#endif
}

ECode LinkProperties::GetAllAddresses(
    /* [out] */ IList** result)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
        List<InetAddress> addresses = new ArrayList<InetAddress>();
        for (LinkAddress linkAddress : mLinkAddresses) {
            addresses.add(linkAddress.getAddress());
        }
        for (LinkProperties stacked: mStackedLinks.values()) {
            addresses.addAll(stacked.getAllAddresses());
        }
        return addresses;

#endif
}

ECode LinkProperties::FindLinkAddressIndex(
    /* [in] */ ILinkAddress* address,
    /* [out] */ Int32* result)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
        for (int i = 0; i < mLinkAddresses.size(); i++) {
            if (mLinkAddresses.get(i).isSameAddressAs(address)) {
                return i;
            }
        }
        return -1;

#endif
}

ECode LinkProperties::AddLinkAddress(
    /* [in] */ ILinkAddress* address,
    /* [out] */ Boolean* result)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
        if (address == null) {
            return false;
        }
        int i = findLinkAddressIndex(address);
        if (i < 0) {
            // Address was not present. Add it.
            mLinkAddresses.add(address);
            return true;
        } else if (mLinkAddresses.get(i).equals(address)) {
            // Address was present and has same properties. Do nothing.
            return false;
        } else {
            // Address was present and has different properties. Update it.
            mLinkAddresses.set(i, address);
            return true;
        }

#endif
}

ECode LinkProperties::RemoveLinkAddress(
    /* [in] */ ILinkAddress* toRemove,
    /* [out] */ Boolean* result)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
        int i = findLinkAddressIndex(toRemove);
        if (i >= 0) {
            mLinkAddresses.remove(i);
            return true;
        }
        return false;

#endif
}

ECode LinkProperties::GetLinkAddresses(
    /* [out] */ IList** result)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
        return Collections.unmodifiableList(mLinkAddresses);

#endif
}

ECode LinkProperties::GetAllLinkAddresses(
    /* [out] */ IList** result)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
        List<LinkAddress> addresses = new ArrayList<LinkAddress>();
        addresses.addAll(mLinkAddresses);
        for (LinkProperties stacked: mStackedLinks.values()) {
            addresses.addAll(stacked.getAllLinkAddresses());
        }
        return addresses;

#endif
}

ECode LinkProperties::SetLinkAddresses(
    /* [in] */ ICollection* addresses)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
        mLinkAddresses.clear();
        for (LinkAddress address: addresses) {
            addLinkAddress(address);
        }

#endif
}

ECode LinkProperties::AddDnsServer(
    /* [in] */ IInetAddress* dnsServer,
    /* [out] */ Boolean* result)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
        if (dnsServer != null && !mDnses.contains(dnsServer)) {
            mDnses.add(dnsServer);
            return true;
        }
        return false;

#endif
}

ECode LinkProperties::SetDnsServers(
    /* [in] */ ICollection* dnsServers)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
        mDnses.clear();
        for (InetAddress dnsServer: dnsServers) {
            addDnsServer(dnsServer);
        }

#endif
}

ECode LinkProperties::GetDnsServers(
    /* [out] */ IList** result)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
        return Collections.unmodifiableList(mDnses);

#endif
}

ECode LinkProperties::SetDomains(
    /* [in] */ const String& domains)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
        mDomains = domains;

#endif
}

ECode LinkProperties::GetDomains(
    /* [out] */ String* result)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
        return mDomains;

#endif
}

ECode LinkProperties::SetMtu(
    /* [in] */ Int32 mtu)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
        mMtu = mtu;

#endif
}

ECode LinkProperties::GetMtu(
    /* [out] */ Int32* result)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
        return mMtu;

#endif
}

ECode LinkProperties::SetTcpBufferSizes(
    /* [in] */ const String& tcpBufferSizes)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
        mTcpBufferSizes = tcpBufferSizes;

#endif
}

ECode LinkProperties::GetTcpBufferSizes(
    /* [out] */ String* result)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
        return mTcpBufferSizes;

#endif
}

ECode LinkProperties::RouteWithInterface(
    /* [in] */ IRouteInfo* route,
    /* [out] */ IRouteInfo** result)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
        return new RouteInfo(
            route.getDestination(),
            route.getGateway(),
            mIfaceName,
            route.getType());

#endif
}

ECode LinkProperties::AddRoute(
    /* [in] */ IRouteInfo* route,
    /* [out] */ Boolean* result)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
        if (route != null) {
            String routeIface = route.getInterface();
            if (routeIface != null && !routeIface.equals(mIfaceName)) {
                throw new IllegalArgumentException(
                   "Route added with non-matching interface: " + routeIface +
                   " vs. " + mIfaceName);
            }
            route = routeWithInterface(route);
            if (!mRoutes.contains(route)) {
                mRoutes.add(route);
                return true;
            }
        }
        return false;

#endif
}

ECode LinkProperties::RemoveRoute(
    /* [in] */ IRouteInfo* route,
    /* [out] */ Boolean* result)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
        return route != null &&
                Objects.equals(mIfaceName, route.getInterface()) &&
                mRoutes.remove(route);

#endif
}

ECode LinkProperties::GetRoutes(
    /* [out] */ IList** result)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
        return Collections.unmodifiableList(mRoutes);

#endif
}

ECode LinkProperties::GetAllRoutes(
    /* [out] */ IList** result)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
        List<RouteInfo> routes = new ArrayList();
        routes.addAll(mRoutes);
        for (LinkProperties stacked: mStackedLinks.values()) {
            routes.addAll(stacked.getAllRoutes());
        }
        return routes;

#endif
}

ECode LinkProperties::SetHttpProxy(
    /* [in] */ IProxyInfo* proxy)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
        mHttpProxy = proxy;

#endif
}

ECode LinkProperties::GetHttpProxy(
    /* [out] */ IProxyInfo** result)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
        return mHttpProxy;

#endif
}

ECode LinkProperties::AddStackedLink(
    /* [in] */ ILinkProperties* link,
    /* [out] */ Boolean* result)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
        if (link != null && link.getInterfaceName() != null) {
            mStackedLinks.put(link.getInterfaceName(), link);
            return true;
        }
        return false;

#endif
}

ECode LinkProperties::RemoveStackedLink(
    /* [in] */ ILinkProperties* link,
    /* [out] */ Boolean* result)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
        if (link != null && link.getInterfaceName() != null) {
            LinkProperties removed = mStackedLinks.remove(link.getInterfaceName());
            return removed != null;
        }
        return false;

#endif
}

ECode LinkProperties::GetStackedLinks(
    /* [out] */ IList** result)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
        if (mStackedLinks.isEmpty()) {
            return Collections.EMPTY_LIST;
        }
        List<LinkProperties> stacked = new ArrayList<LinkProperties>();
        for (LinkProperties link : mStackedLinks.values()) {
            stacked.add(new LinkProperties(link));
        }
        return Collections.unmodifiableList(stacked);

#endif
}

ECode LinkProperties::Clear()
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
        mIfaceName = null;
        mLinkAddresses.clear();
        mDnses.clear();
        mDomains = null;
        mRoutes.clear();
        mHttpProxy = null;
        mStackedLinks.clear();
        mMtu = 0;
        mTcpBufferSizes = null;

#endif
}

ECode LinkProperties::DescribeContents(
    /* [out] */ Int32* result)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
        return 0;

#endif
}

ECode LinkProperties::ToString(
    /* [out] */ String* result)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
        String ifaceName = (mIfaceName == null ? "" : "InterfaceName: " + mIfaceName + " ");
        String linkAddresses = "LinkAddresses: [";
        for (LinkAddress addr : mLinkAddresses) linkAddresses += addr.toString() + ",";
        linkAddresses += "] ";
        String dns = "DnsAddresses: [";
        for (InetAddress addr : mDnses) dns += addr.getHostAddress() + ",";
        dns += "] ";
        String domainName = "Domains: " + mDomains;
        String mtu = " MTU: " + mMtu;
        String tcpBuffSizes = "";
        if (mTcpBufferSizes != null) {
            tcpBuffSizes = " TcpBufferSizes: " + mTcpBufferSizes;
        }
        String routes = " Routes: [";
        for (RouteInfo route : mRoutes) routes += route.toString() + ",";
        routes += "] ";
        String proxy = (mHttpProxy == null ? "" : " HttpProxy: " + mHttpProxy.toString() + " ");
        String stacked = "";
        if (mStackedLinks.values().size() > 0) {
            stacked += " Stacked: [";
            for (LinkProperties link: mStackedLinks.values()) {
                stacked += " [" + link.toString() + " ],";
            }
            stacked += "] ";
        }
        return "{" + ifaceName + linkAddresses + routes + dns + domainName + mtu
            + tcpBuffSizes + proxy + stacked + "}";

#endif
}

ECode LinkProperties::HasIPv4Address(
    /* [out] */ Boolean* result)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
        for (LinkAddress address : mLinkAddresses) {
          if (address.getAddress() instanceof Inet4Address) {
            return true;
          }
        }
        return false;

#endif
}

ECode LinkProperties::HasGlobalIPv6Address(
    /* [out] */ Boolean* result)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
        for (LinkAddress address : mLinkAddresses) {
          if (address.getAddress() instanceof Inet6Address && address.isGlobalPreferred()) {
            return true;
          }
        }
        return false;

#endif
}

ECode LinkProperties::HasIPv4DefaultRoute(
    /* [out] */ Boolean* result)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
        for (RouteInfo r : mRoutes) {
          if (r.isIPv4Default()) {
            return true;
          }
        }
        return false;

#endif
}

ECode LinkProperties::HasIPv6DefaultRoute(
    /* [out] */ Boolean* result)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
        for (RouteInfo r : mRoutes) {
          if (r.isIPv6Default()) {
            return true;
          }
        }
        return false;

#endif
}

ECode LinkProperties::HasIPv4DnsServer(
    /* [out] */ Boolean* result)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
        for (InetAddress ia : mDnses) {
          if (ia instanceof Inet4Address) {
            return true;
          }
        }
        return false;

#endif
}

ECode LinkProperties::HasIPv6DnsServer(
    /* [out] */ Boolean* result)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
        for (InetAddress ia : mDnses) {
          if (ia instanceof Inet6Address) {
            return true;
          }
        }
        return false;

#endif
}

ECode LinkProperties::IsProvisioned(
    /* [out] */ Boolean* result)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
        return (hasIPv4Address() ||
                (hasGlobalIPv6Address() && hasIPv6DefaultRoute() && hasIPv6DnsServer()));

#endif
}

ECode LinkProperties::IsIdenticalInterfaceName(
    /* [in] */ ILinkProperties* target,
    /* [out] */ Boolean* result)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
        return TextUtils.equals(getInterfaceName(), target.getInterfaceName());

#endif
}

ECode LinkProperties::IsIdenticalAddresses(
    /* [in] */ ILinkProperties* target,
    /* [out] */ Boolean* result)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
        Collection<InetAddress> targetAddresses = target.getAddresses();
        Collection<InetAddress> sourceAddresses = getAddresses();
        return (sourceAddresses.size() == targetAddresses.size()) ?
                    sourceAddresses.containsAll(targetAddresses) : false;

#endif
}

ECode LinkProperties::IsIdenticalDnses(
    /* [in] */ ILinkProperties* target,
    /* [out] */ Boolean* result)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
        Collection<InetAddress> targetDnses = target.getDnsServers();
        String targetDomains = target.getDomains();
        if (mDomains == null) {
            if (targetDomains != null) return false;
        } else {
            if (mDomains.equals(targetDomains) == false) return false;
        }
        return (mDnses.size() == targetDnses.size()) ?
                    mDnses.containsAll(targetDnses) : false;

#endif
}

ECode LinkProperties::IsIdenticalRoutes(
    /* [in] */ ILinkProperties* target,
    /* [out] */ Boolean* result)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
        Collection<RouteInfo> targetRoutes = target.getRoutes();
        return (mRoutes.size() == targetRoutes.size()) ?
                    mRoutes.containsAll(targetRoutes) : false;

#endif
}

ECode LinkProperties::IsIdenticalHttpProxy(
    /* [in] */ ILinkProperties* target,
    /* [out] */ Boolean* result)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
        return getHttpProxy() == null ? target.getHttpProxy() == null :
                    getHttpProxy().equals(target.getHttpProxy());

#endif
}

ECode LinkProperties::IsIdenticalStackedLinks(
    /* [in] */ ILinkProperties* target,
    /* [out] */ Boolean* result)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
        if (!mStackedLinks.keySet().equals(target.mStackedLinks.keySet())) {
            return false;
        }
        for (LinkProperties stacked : mStackedLinks.values()) {
            // Hashtable values can never be null.
            String iface = stacked.getInterfaceName();
            if (!stacked.equals(target.mStackedLinks.get(iface))) {
                return false;
            }
        }
        return true;

#endif
}

ECode LinkProperties::IsIdenticalMtu(
    /* [in] */ ILinkProperties* target,
    /* [out] */ Boolean* result)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
        return getMtu() == target.getMtu();

#endif
}

ECode LinkProperties::IsIdenticalTcpBufferSizes(
    /* [in] */ ILinkProperties* target,
    /* [out] */ Boolean* result)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
        return Objects.equals(mTcpBufferSizes, target.mTcpBufferSizes);

#endif
}

ECode LinkProperties::Equals(
    /* [in] */ IObject* obj,
    /* [out] */ Boolean* result)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
        if (this == obj) return true;
        if (!(obj instanceof LinkProperties)) return false;
        LinkProperties target = (LinkProperties) obj;
        /**
         * This method does not check that stacked interfaces are equal, because
         * stacked interfaces are not so much a property of the link as a
         * description of connections between links.
         */
        return isIdenticalInterfaceName(target) &&
                isIdenticalAddresses(target) &&
                isIdenticalDnses(target) &&
                isIdenticalRoutes(target) &&
                isIdenticalHttpProxy(target) &&
                isIdenticalStackedLinks(target) &&
                isIdenticalMtu(target) &&
                isIdenticalTcpBufferSizes(target);

#endif
}

ECode LinkProperties::CompareAddresses(
    /* [in] */ ILinkProperties* target,
    /* [out] */ ILinkPropertiesCompareResult** result)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
        /*
         * Duplicate the LinkAddresses into removed, we will be removing
         * address which are common between mLinkAddresses and target
         * leaving the addresses that are different. And address which
         * are in target but not in mLinkAddresses are placed in the
         * addedAddresses.
         */
        CompareResult<LinkAddress> result = new CompareResult<LinkAddress>();
        result.removed = new ArrayList<LinkAddress>(mLinkAddresses);
        result.added.clear();
        if (target != null) {
            for (LinkAddress newAddress : target.getLinkAddresses()) {
                if (! result.removed.remove(newAddress)) {
                    result.added.add(newAddress);
                }
            }
        }
        return result;

#endif
}

ECode LinkProperties::CompareDnses(
    /* [in] */ ILinkProperties* target,
    /* [out] */ ILinkPropertiesCompareResult** result)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
        /*
         * Duplicate the InetAddresses into removed, we will be removing
         * dns address which are common between mDnses and target
         * leaving the addresses that are different. And dns address which
         * are in target but not in mDnses are placed in the
         * addedAddresses.
         */
        CompareResult<InetAddress> result = new CompareResult<InetAddress>();
        result.removed = new ArrayList<InetAddress>(mDnses);
        result.added.clear();
        if (target != null) {
            for (InetAddress newAddress : target.getDnsServers()) {
                if (! result.removed.remove(newAddress)) {
                    result.added.add(newAddress);
                }
            }
        }
        return result;

#endif
}

ECode LinkProperties::CompareAllRoutes(
    /* [in] */ ILinkProperties* target,
    /* [out] */ ILinkPropertiesCompareResult** result)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
        /*
         * Duplicate the RouteInfos into removed, we will be removing
         * routes which are common between mRoutes and target
         * leaving the routes that are different. And route address which
         * are in target but not in mRoutes are placed in added.
         */
        CompareResult<RouteInfo> result = new CompareResult<RouteInfo>();
        result.removed = getAllRoutes();
        result.added.clear();
        if (target != null) {
            for (RouteInfo r : target.getAllRoutes()) {
                if (! result.removed.remove(r)) {
                    result.added.add(r);
                }
            }
        }
        return result;

#endif
}

ECode LinkProperties::CompareAllInterfaceNames(
    /* [in] */ ILinkProperties* target,
    /* [out] */ ILinkPropertiesCompareResult** result)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
        /*
         * Duplicate the interface names into removed, we will be removing
         * interface names which are common between this and target
         * leaving the interface names that are different. And interface names which
         * are in target but not in this are placed in added.
         */
        CompareResult<String> result = new CompareResult<String>();
        result.removed = getAllInterfaceNames();
        result.added.clear();
        if (target != null) {
            for (String r : target.getAllInterfaceNames()) {
                if (! result.removed.remove(r)) {
                    result.added.add(r);
                }
            }
        }
        return result;

#endif
}

ECode LinkProperties::HashCode(
    /* [out] */ Int32* result)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
        return ((null == mIfaceName) ? 0 : mIfaceName.hashCode()
                + mLinkAddresses.size() * 31
                + mDnses.size() * 37
                + ((null == mDomains) ? 0 : mDomains.hashCode())
                + mRoutes.size() * 41
                + ((null == mHttpProxy) ? 0 : mHttpProxy.hashCode())
                + mStackedLinks.hashCode() * 47)
                + mMtu * 51
                + ((null == mTcpBufferSizes) ? 0 : mTcpBufferSizes.hashCode());

#endif
}

ECode LinkProperties::WriteToParcel(
    /* [in] */ IParcel* dest,
    /* [in] */ Int32 flags)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
        dest.writeString(getInterfaceName());
        dest.writeInt(mLinkAddresses.size());
        for(LinkAddress linkAddress : mLinkAddresses) {
            dest.writeParcelable(linkAddress, flags);
        }
        dest.writeInt(mDnses.size());
        for(InetAddress d : mDnses) {
            dest.writeByteArray(d.getAddress());
        }
        dest.writeString(mDomains);
        dest.writeInt(mMtu);
        dest.writeString(mTcpBufferSizes);
        dest.writeInt(mRoutes.size());
        for(RouteInfo route : mRoutes) {
            dest.writeParcelable(route, flags);
        }
        if (mHttpProxy != null) {
            dest.writeByte((byte)1);
            dest.writeParcelable(mHttpProxy, flags);
        } else {
            dest.writeByte((byte)0);
        }
        ArrayList<LinkProperties> stackedLinks = new ArrayList(mStackedLinks.values());
        dest.writeList(stackedLinks);

#endif
}

ECode LinkProperties::ReadFromParcel(
    /* [in] */ IParcel* parcel)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
            public LinkProperties createFromParcel(Parcel in) {
                LinkProperties netProp = new LinkProperties();
                String iface = in.readString();
                if (iface != null) {
                    netProp.setInterfaceName(iface);
                }
                int addressCount = in.readInt();
                for (int i=0; i<addressCount; i++) {
                    netProp.addLinkAddress((LinkAddress)in.readParcelable(null));
                }
                addressCount = in.readInt();
                for (int i=0; i<addressCount; i++) {
                    try {
                        netProp.addDnsServer(InetAddress.getByAddress(in.createByteArray()));
                    } catch (UnknownHostException e) { }
                }
                netProp.setDomains(in.readString());
                netProp.setMtu(in.readInt());
                netProp.setTcpBufferSizes(in.readString());
                addressCount = in.readInt();
                for (int i=0; i<addressCount; i++) {
                    netProp.addRoute((RouteInfo)in.readParcelable(null));
                }
                if (in.readByte() == 1) {
                    netProp.setHttpProxy((ProxyInfo)in.readParcelable(null));
                }
                ArrayList<LinkProperties> stackedLinks = new ArrayList<LinkProperties>();
                in.readList(stackedLinks, LinkProperties.class.getClassLoader());
                for (LinkProperties stackedLink: stackedLinks) {
                    netProp.addStackedLink(stackedLink);
                }
                return netProp;
            }
            public LinkProperties[] newArray(int size) {
                return new LinkProperties[size];
            }

#endif
}

ECode LinkProperties::WriteToParcel(
    /* [in] */ IParcel* dest)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
            public LinkProperties createFromParcel(Parcel in) {
                LinkProperties netProp = new LinkProperties();
                String iface = in.readString();
                if (iface != null) {
                    netProp.setInterfaceName(iface);
                }
                int addressCount = in.readInt();
                for (int i=0; i<addressCount; i++) {
                    netProp.addLinkAddress((LinkAddress)in.readParcelable(null));
                }
                addressCount = in.readInt();
                for (int i=0; i<addressCount; i++) {
                    try {
                        netProp.addDnsServer(InetAddress.getByAddress(in.createByteArray()));
                    } catch (UnknownHostException e) { }
                }
                netProp.setDomains(in.readString());
                netProp.setMtu(in.readInt());
                netProp.setTcpBufferSizes(in.readString());
                addressCount = in.readInt();
                for (int i=0; i<addressCount; i++) {
                    netProp.addRoute((RouteInfo)in.readParcelable(null));
                }
                if (in.readByte() == 1) {
                    netProp.setHttpProxy((ProxyInfo)in.readParcelable(null));
                }
                ArrayList<LinkProperties> stackedLinks = new ArrayList<LinkProperties>();
                in.readList(stackedLinks, LinkProperties.class.getClassLoader());
                for (LinkProperties stackedLink: stackedLinks) {
                    netProp.addStackedLink(stackedLink);
                }
                return netProp;
            }
            public LinkProperties[] newArray(int size) {
                return new LinkProperties[size];
            }

#endif
}


ECode LinkProperties::IsValidMtu(
    /* [in] */ Int32 mtu,
    /* [in] */ Boolean ipv6,
    /* [out] */ Boolean* result)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
            if (ipv6) {
                if ((mtu >= MIN_MTU_V6 && mtu <= MAX_MTU)) return true;
            } else {
                if ((mtu >= MIN_MTU && mtu <= MAX_MTU)) return true;
            }
            return false;

#endif
}

//====================================================================================
//              LinkPropertiesCompareResult
//====================================================================================
CAR_INTERFACE_IMPL(LinkPropertiesCompareResult, Object, ILinkPropertiesCompareResult)

ECode LinkPropertiesCompareResult::ToString(
    /* [out] */ String* result)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
            String retVal = "removed=[";
            for (T addr : removed) retVal += addr.toString() + ",";
            retVal += "] added=[";
            for (T addr : added) retVal += addr.toString() + ",";
            retVal += "]";
            return retVal;

#endif
}

ECode LinkPropertiesCompareResult::GetRemoved(
    /* [out] */ IList** result)
{
    VALIDATE_NOT_NULL(*result)

    *result = mRemoved;
    REFCOUNT_ADD(*result)
    return NOERROR;
}

ECode LinkPropertiesCompareResult::SetRemoved(
    /* [in] */ IList* removed)
{
    VALIDATE_NOT_NULL(removed)

    mRemoved = removed;
    return NOERROR;
}

ECode LinkPropertiesCompareResult::GetAdded(
    /* [out] */ IList** result)
{
    VALIDATE_NOT_NULL(*result)

    *result = mAdded;
    REFCOUNT_ADD(*result)
    return NOERROR;
}

ECode LinkPropertiesCompareResult::SetAdded(
    /* [in] */ IList* added)
{
    VALIDATE_NOT_NULL(added)

    mAdded = added;
    return NOERROR;
}

ECode LinkPropertiesCompareResult::constructor()
{
    return NOERROR;
}

} // namespace Net
} // namespace Droid
} // namespace Elastos
