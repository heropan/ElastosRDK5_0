
#include "CInetAddress.h"

namespace Elastos {
namespace Net {

PInterface CInetAddress::Probe(
    /* [in] */ REIID riid)
{
    if (riid == EIID_InetAddress) {
        return reinterpret_cast<PInterface>((InetAddress*)this);
    }
    return _CInetAddress::Probe(riid);
}

ECode CInetAddress::constructor()
{
    return NOERROR;
}

ECode CInetAddress::constructor(
    /* [in] */ Int32 family,
    /* [in] */ ArrayOf<Byte>* ipaddress,
    /* [in] */ const String& hostName)
{
    return InetAddress::Init(family, ipaddress, hostName);
}

ECode CInetAddress::GetAddress(
    /* [out, callee] */ ArrayOf<Byte>** address)
{
    return InetAddress::GetAddress(address);
}

ECode CInetAddress::GetHostAddress(
    /* [out] */ String* hostAddress)
{
    return InetAddress::GetHostAddress(hostAddress);
}

ECode CInetAddress::GetHostName(
    /* [out] */ String* hostName)
{
    return InetAddress::GetHostName(hostName);
}

ECode CInetAddress::GetCanonicalHostName(
    /* [out] */ String* hostName)
{
    return InetAddress::GetCanonicalHostName(hostName);
}

ECode CInetAddress::GetHashCode(
    /* [out] */ Int32* hashCode)
{
    return InetAddress::GetHashCode(hashCode);
}

ECode CInetAddress::IsAnyLocalAddress(
    /* [out] */ Boolean* isAnyLocalAddress)
{
    return InetAddress::IsAnyLocalAddress(isAnyLocalAddress);
}

ECode CInetAddress::IsLinkLocalAddress(
    /* [out] */ Boolean* isLinkLocalAddress)
{
    return InetAddress::IsLinkLocalAddress(isLinkLocalAddress);
}

ECode CInetAddress::IsLoopbackAddress(
    /* [out] */ Boolean* isLoopbackAddress)
{
    return InetAddress::IsLoopbackAddress(isLoopbackAddress);
}

ECode CInetAddress::IsMCGlobal(
    /* [out] */ Boolean* isMCGlobal)
{
    return InetAddress::IsMCGlobal(isMCGlobal);
}

ECode CInetAddress::IsMCLinkLocal(
    /* [out] */ Boolean* isMCLinkLocal)
{
    return InetAddress::IsMCLinkLocal(isMCLinkLocal);
}

ECode CInetAddress::IsMCNodeLocal(
    /* [out] */ Boolean* isMCNodeLocal)
{
    return InetAddress::IsMCNodeLocal(isMCNodeLocal);
}

ECode CInetAddress::IsMCOrgLocal(
    /* [out] */ Boolean* isMCOrgLocal)
{
    return InetAddress::IsMCOrgLocal(isMCOrgLocal);
}

ECode CInetAddress::IsMCSiteLocal(
    /* [out] */ Boolean* isMCSiteLocal)
{
    return InetAddress::IsMCSiteLocal(isMCSiteLocal);
}

ECode CInetAddress::IsMulticastAddress(
    /* [out] */ Boolean* isMulticastAddress)
{
    return InetAddress::IsMulticastAddress(isMulticastAddress);
}

ECode CInetAddress::IsSiteLocalAddress(
    /* [out] */ Boolean* isSiteLocalAddress)
{
    return InetAddress::IsSiteLocalAddress(isSiteLocalAddress);
}

ECode CInetAddress::IsReachable(
    /* [in] */ Int32 timeout,
    /* [out] */ Boolean* isReachable)
{
    return InetAddress::IsReachable(timeout, isReachable);
}

ECode CInetAddress::IsReachable(
    /* [in] */ INetworkInterface* networkInterface,
    /* [in] */ Int32 ttl,
    /* [in] */ Int32 timeout,
    /* [out] */ Boolean* isReachable)
{
    return InetAddress::IsReachable(networkInterface, ttl, timeout, isReachable);
}

ECode CInetAddress::Equals(
    /* [in] */ IInterface* obj,
    /* [out] */ Boolean* result)
{
    return InetAddress::Equals(obj, result);
}

ECode CInetAddress::ToString(
    /* [out] */ String* result)
{
    return InetAddress::ToString(result);
}

} // namespace Net
} // namespace Elastos
