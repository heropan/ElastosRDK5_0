
#include "elastos/droid/net/StaticIpConfiguration.h"

namespace Elastos {
namespace Droid {
namespace Net {

CAR_INTERFACE_IMPL_2(StaticIpConfiguration, Object, IParcelable, IStaticIpConfiguration)

ECode StaticIpConfiguration::constructor()
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
        dnsServers = new ArrayList<InetAddress>();

#endif
}

ECode StaticIpConfiguration::constructor(
    /* [in] */ IStaticIpConfiguration* source)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
        this();
        if (source != null) {
            // All of these except dnsServers are immutable, so no need to make copies.
            ipAddress = source.ipAddress;
            gateway = source.gateway;
            dnsServers.addAll(source.dnsServers);
            domains = source.domains;
        }

#endif
}

ECode StaticIpConfiguration::Clear()
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
        ipAddress = null;
        gateway = null;
        dnsServers.clear();
        domains = null;

#endif
}

ECode StaticIpConfiguration::GetRoutes(
    /* [in] */ const String& iface,
    /* [out] */ IList** result)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
        List<RouteInfo> routes = new ArrayList<RouteInfo>(2);
        if (ipAddress != null) {
            routes.add(new RouteInfo(ipAddress, null, iface));
        }
        if (gateway != null) {
            routes.add(new RouteInfo((LinkAddress) null, gateway, iface));
        }
        return routes;

#endif
}

ECode StaticIpConfiguration::ToLinkProperties(
    /* [in] */ const String& iface,
    /* [out] */ ILinkProperties** result)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
        LinkProperties lp = new LinkProperties();
        lp.setInterfaceName(iface);
        if (ipAddress != null) {
            lp.addLinkAddress(ipAddress);
        }
        for (RouteInfo route : getRoutes(iface)) {
            lp.addRoute(route);
        }
        for (InetAddress dns : dnsServers) {
            lp.addDnsServer(dns);
        }
        return lp;

#endif
}

ECode StaticIpConfiguration::ToString(
    /* [out] */ String* result)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
        StringBuffer str = new StringBuffer();
        str.append("IP address ");
        if (ipAddress != null ) str.append(ipAddress).append(" ");
        str.append("Gateway ");
        if (gateway != null) str.append(gateway.getHostAddress()).append(" ");
        str.append(" DNS servers: [");
        for (InetAddress dnsServer : dnsServers) {
            str.append(" ").append(dnsServer.getHostAddress());
        }
        str.append(" ] Domains");
        if (domains != null) str.append(domains);
        return str.toString();

#endif
}

ECode StaticIpConfiguration::HashCode(
    /* [out] */ Int32* result)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
        int result = 13;
        result = 47 * result + (ipAddress == null ? 0 : ipAddress.hashCode());
        result = 47 * result + (gateway == null ? 0 : gateway.hashCode());
        result = 47 * result + (domains == null ? 0 : domains.hashCode());
        result = 47 * result + dnsServers.hashCode();
        return result;

#endif
}

ECode StaticIpConfiguration::Equals(
    /* [in] */ IInterface* obj,
    /* [out] */ Boolean* result)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
        if (this == obj) return true;
        if (!(obj instanceof StaticIpConfiguration)) return false;
        StaticIpConfiguration other = (StaticIpConfiguration) obj;
        return other != null &&
                Objects.equals(ipAddress, other.ipAddress) &&
                Objects.equals(gateway, other.gateway) &&
                dnsServers.equals(other.dnsServers) &&
                Objects.equals(domains, other.domains);

#endif
}

ECode StaticIpConfiguration::ReadFromParcel(
        /* [in] */ IParcel* parcel)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
            public StaticIpConfiguration createFromParcel(Parcel in) {
                StaticIpConfiguration s = new StaticIpConfiguration();
                readFromParcel(s, in);
                return s;
            }
            public StaticIpConfiguration[] newArray(int size) {
                return new StaticIpConfiguration[size];
            }

#endif
}

ECode StaticIpConfiguration::WriteToParcel(
        /* [in] */ IParcel* dest)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
            public StaticIpConfiguration createFromParcel(Parcel in) {
                StaticIpConfiguration s = new StaticIpConfiguration();
                readFromParcel(s, in);
                return s;
            }
            public StaticIpConfiguration[] newArray(int size) {
                return new StaticIpConfiguration[size];
            }

#endif
}

ECode StaticIpConfiguration::ReadFromParcel(
    /* [in] */ IStaticIpConfiguration* s,
    /* [in] */ IParcel* in)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
        s.ipAddress = in.readParcelable(null);
        s.gateway = NetworkUtils.unparcelInetAddress(in);
        s.dnsServers.clear();
        int size = in.readInt();
        for (int i = 0; i < size; i++) {
            s.dnsServers.add(NetworkUtils.unparcelInetAddress(in));
        }

#endif
}

ECode StaticIpConfiguration::GetIpAddress(
    /* [out] */ ILinkAddress** result)
{
    VALIDATE_NOT_NULL(*result)

    *result = mIpAddress;
    REFCOUNT_ADD(*result)
    return NOERROR;
}

ECode StaticIpConfiguration::SetIpAddress(
    /* [in] */ ILinkAddress* ipAddress)
{
    VALIDATE_NOT_NULL(ipAddress)

    mIpAddress = ipAddress;
    return NOERROR;
}

ECode StaticIpConfiguration::GetGateway(
    /* [out] */ IInetAddress** result)
{
    VALIDATE_NOT_NULL(*result)

    *result = mGateway;
    REFCOUNT_ADD(*result)
    return NOERROR;
}

ECode StaticIpConfiguration::SetGateway(
    /* [in] */ IInetAddress* gateway)
{
    VALIDATE_NOT_NULL(gateway)

    mGateway = gateway;
    return NOERROR;
}

ECode StaticIpConfiguration::GetDnsServers(
    /* [out] */ IArrayList** result)
{
    VALIDATE_NOT_NULL(*result)

    *result = mDnsServers;
    REFCOUNT_ADD(*result)
    return NOERROR;
}

ECode StaticIpConfiguration::GetDomains(
    /* [out] */ String* result)
{
    VALIDATE_NOT_NULL(result)

    *result = mDomains;
    return NOERROR;
}

ECode StaticIpConfiguration::SetDomains(
    /* [in] */ const String& domains)
{
    mDomains = domains;
    return NOERROR;
}


} // namespace Net
} // namespace Droid
} // namespace Elastos
