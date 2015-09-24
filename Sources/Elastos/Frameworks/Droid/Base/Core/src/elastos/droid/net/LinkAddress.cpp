
#include "elastos/droid/net/LinkAddress.h"

namespace Elastos {
namespace Droid {
namespace Net {

CAR_INTERFACE_IMPL_2(LinkAddress, Object, ILinkAddress, IParcelable)

ECode LinkAddress::ScopeForUnicastAddress(
    /* [in] */ IInetAddress* addr,
    /* [out] */ Int32* result)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
        if (addr.isAnyLocalAddress()) {
            return RT_SCOPE_HOST;
        }
        if (addr.isLoopbackAddress() || addr.isLinkLocalAddress()) {
            return RT_SCOPE_LINK;
        }
        // isSiteLocalAddress() returns true for private IPv4 addresses, but RFC 6724 section 3.2
        // says that they are assigned global scope.
        if (!(addr instanceof Inet4Address) && addr.isSiteLocalAddress()) {
            return RT_SCOPE_SITE;
        }
        return RT_SCOPE_UNIVERSE;

#endif
}

ECode LinkAddress::Init(
    /* [in] */ IInetAddress* address,
    /* [in] */ Int32 prefixLength,
    /* [in] */ Int32 flags,
    /* [in] */ Int32 scope)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
        if (address == null ||
                address.isMulticastAddress() ||
                prefixLength < 0 ||
                ((address instanceof Inet4Address) && prefixLength > 32) ||
                (prefixLength > 128)) {
            throw new IllegalArgumentException("Bad LinkAddress params " + address +
                    "/" + prefixLength);
        }
        this.address = address;
        this.prefixLength = prefixLength;
        this.flags = flags;
        this.scope = scope;

#endif
}

ECode LinkAddress::constructor(
    /* [in] */ IInetAddress* address,
    /* [in] */ Int32 prefixLength,
    /* [in] */ Int32 flags,
    /* [in] */ Int32 scope)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
        init(address, prefixLength, flags, scope);

#endif
}

ECode LinkAddress::constructor(
    /* [in] */ IInetAddress* address,
    /* [in] */ Int32 prefixLength)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
        this(address, prefixLength, 0, 0);
        this.scope = scopeForUnicastAddress(address);

#endif
}

ECode LinkAddress::constructor(
    /* [in] */ IInterfaceAddress* interfaceAddress)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
        this(interfaceAddress.getAddress(),
             interfaceAddress.getNetworkPrefixLength());

#endif
}

ECode LinkAddress::constructor(
    /* [in] */ const String& address)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
        this(address, 0, 0);
        this.scope = scopeForUnicastAddress(this.address);

#endif
}

ECode LinkAddress::constructor(
    /* [in] */ const String& address,
    /* [in] */ Int32 flags,
    /* [in] */ Int32 scope)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
        // This may throw an IllegalArgumentException; catching it is the caller's responsibility.
        Pair<InetAddress, Integer> ipAndMask = NetworkUtils.parseIpAndMask(address);
        init(ipAndMask.first, ipAndMask.second, flags, scope);

#endif
}

ECode LinkAddress::ToString(
    /* [out] */ String* result)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
        return address.getHostAddress() + "/" + prefixLength;

#endif
}

ECode LinkAddress::Equals(
    /* [in] */ IObject* obj,
    /* [out] */ Boolean* result)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
        if (!(obj instanceof LinkAddress)) {
            return false;
        }
        LinkAddress linkAddress = (LinkAddress) obj;
        return this.address.equals(linkAddress.address) &&
            this.prefixLength == linkAddress.prefixLength &&
            this.flags == linkAddress.flags &&
            this.scope == linkAddress.scope;

#endif
}

ECode LinkAddress::HashCode(
    /* [out] */ Int32* result)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
        return address.hashCode() + 11 * prefixLength + 19 * flags + 43 * scope;

#endif
}

ECode LinkAddress::IsSameAddressAs(
    /* [in] */ ILinkAddress* other,
    /* [out] */ Boolean* result)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
        return address.equals(other.address) && prefixLength == other.prefixLength;

#endif
}

ECode LinkAddress::GetAddress(
    /* [out] */ IInetAddress** result)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
        return address;

#endif
}

ECode LinkAddress::GetPrefixLength(
    /* [out] */ Int32* result)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
        return prefixLength;

#endif
}

ECode LinkAddress::GetNetworkPrefixLength(
    /* [out] */ Int32* result)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
        return getPrefixLength();

#endif
}

ECode LinkAddress::GetFlags(
    /* [out] */ Int32* result)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
        return flags;

#endif
}

ECode LinkAddress::GetScope(
    /* [out] */ Int32* result)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
        return scope;

#endif
}

ECode LinkAddress::IsGlobalPreferred(
    /* [out] */ Boolean* result)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
        return (scope == RT_SCOPE_UNIVERSE &&
                (flags & (IFA_F_DADFAILED | IFA_F_DEPRECATED | IFA_F_TENTATIVE)) == 0L);

#endif
}

ECode LinkAddress::DescribeContents(
    /* [out] */ Int32* result)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
        return 0;

#endif
}

ECode LinkAddress::WriteToParcel(
    /* [in] */ IParcel* dest,
    /* [in] */ Int32 flags)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
        dest.writeByteArray(address.getAddress());
        dest.writeInt(prefixLength);
        dest.writeInt(this.flags);
        dest.writeInt(scope);

#endif
}

ECode LinkAddress::ReadFromParcel(
    /* [in] */ IParcel* source)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
            public LinkAddress createFromParcel(Parcel in) {
                InetAddress address = null;
                try {
                    address = InetAddress.getByAddress(in.createByteArray());
                } catch (UnknownHostException e) {
                    // Nothing we can do here. When we call the constructor, we'll throw an
                    // IllegalArgumentException, because a LinkAddress can't have a null
                    // InetAddress.
                }
                int prefixLength = in.readInt();
                int flags = in.readInt();
                int scope = in.readInt();
                return new LinkAddress(address, prefixLength, flags, scope);
            }
            public LinkAddress[] newArray(int size) {
                return new LinkAddress[size];
            }

#endif
}

ECode LinkAddress::WriteToParcel(
    /* [in] */ IParcel* dest)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
            public LinkAddress createFromParcel(Parcel in) {
                InetAddress address = null;
                try {
                    address = InetAddress.getByAddress(in.createByteArray());
                } catch (UnknownHostException e) {
                    // Nothing we can do here. When we call the constructor, we'll throw an
                    // IllegalArgumentException, because a LinkAddress can't have a null
                    // InetAddress.
                }
                int prefixLength = in.readInt();
                int flags = in.readInt();
                int scope = in.readInt();
                return new LinkAddress(address, prefixLength, flags, scope);
            }
            public LinkAddress[] newArray(int size) {
                return new LinkAddress[size];
            }

#endif
}

} // namespace Net
} // namespace Droid
} // namespace Elastos
