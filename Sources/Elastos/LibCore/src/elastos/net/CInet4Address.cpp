
#include "CInet4Address.h"

namespace Elastos {
namespace Net {

static AutoPtr<IInetAddress> InitANY()
{
    AutoPtr<ArrayOf<Byte> > ipAddress = ArrayOf<Byte>::Alloc(4);
    (*ipAddress)[0] = 0;
    (*ipAddress)[1] = 0;
    (*ipAddress)[2] = 0;
    (*ipAddress)[3] = 0;
    AutoPtr<CInet4Address> ipv4Addr;
    CInet4Address::NewByFriend(ipAddress, String(NULL), (CInet4Address**)&ipv4Addr);
    return (IInetAddress*)(IInet4Address*)ipv4Addr.Get();
}

static AutoPtr<IInetAddress> InitALL()
{
    AutoPtr<ArrayOf<Byte> > ipAddress = ArrayOf<Byte>::Alloc(4);
    (*ipAddress)[0] = (Byte)255;
    (*ipAddress)[1] = (Byte)255;
    (*ipAddress)[2] = (Byte)255;
    (*ipAddress)[3] = (Byte)255;
    AutoPtr<CInet4Address> ipv4Addr;
    CInet4Address::NewByFriend(ipAddress, String(NULL), (CInet4Address**)&ipv4Addr);
    return (IInetAddress*)(IInet4Address*)ipv4Addr.Get();
}

static AutoPtr<IInetAddress> InitLOOPBACK()
{
    AutoPtr<ArrayOf<Byte> > ipAddress = ArrayOf<Byte>::Alloc(4);
    (*ipAddress)[0] = 127;
    (*ipAddress)[1] = 0;
    (*ipAddress)[2] = 0;
    (*ipAddress)[3] = 1;
    AutoPtr<CInet4Address> ipv4Addr;
    CInet4Address::NewByFriend(ipAddress, String("localhost"),
            (CInet4Address**)&ipv4Addr);
    return (IInetAddress*)(IInet4Address*)ipv4Addr.Get();
}

const Int32 CInet4Address::AF_INET;
AutoPtr<IInetAddress> CInet4Address::ANY = InitANY();
AutoPtr<IInetAddress> CInet4Address::LOOPBACK = InitLOOPBACK();
AutoPtr<IInetAddress> CInet4Address::ALL = InitALL();

PInterface CInet4Address::Probe(
    /* [in] */ REIID riid)
{
    if (riid == EIID_InetAddress) {
        return reinterpret_cast<PInterface>((InetAddress*)this);
    }
    else return _CInet4Address::Probe(riid);
}

ECode CInet4Address::GetAddress(
    /* [out, callee] */ ArrayOf<Byte>** address)
{
    VALIDATE_NOT_NULL(address);
    return InetAddress::GetAddress(address);
}

ECode CInet4Address::GetHostAddress(
    /* [out] */ String* hostAddress)
{
    VALIDATE_NOT_NULL(hostAddress);
    return InetAddress::GetHostAddress(hostAddress);
}

ECode CInet4Address::GetHostName(
    /* [out] */ String* hostName)
{
    VALIDATE_NOT_NULL(hostName);
    return InetAddress::GetHostName(hostName);
}

ECode CInet4Address::GetCanonicalHostName(
    /* [out] */ String* hostName)
{
    VALIDATE_NOT_NULL(hostName);
    return InetAddress::GetCanonicalHostName(hostName);
}
ECode CInet4Address::GetHashCode(
    /* [out] */ Int32* hashCode)
{
    VALIDATE_NOT_NULL(hashCode);
    return InetAddress::GetHashCode(hashCode);
}

ECode CInet4Address::IsMulticastAddress(
    /* [out] */ Boolean* isMulticastAddress)
{
    VALIDATE_NOT_NULL(isMulticastAddress);
    *isMulticastAddress = (((*mIpAddress)[0] & 0xF0) == 0xE0);
    return NOERROR;
}

ECode CInet4Address::IsLoopbackAddress(
    /* [out] */ Boolean* isLoopbackAddress)
{
    VALIDATE_NOT_NULL(isLoopbackAddress);
    *isLoopbackAddress = (((*mIpAddress)[0] & 255) == 127);
    return NOERROR;
}

ECode CInet4Address::IsLinkLocalAddress(
    /* [out] */ Boolean* isLinkLocalAddress)
{
    VALIDATE_NOT_NULL(isLinkLocalAddress);
    // The reference implementation does not return true for loopback
    // addresses even though RFC 3484 says to do so
    *isLinkLocalAddress = ((((*mIpAddress)[0] & 255) == 169) &&
                           (((*mIpAddress)[1] & 255) == 254));
    return NOERROR;
}

ECode CInet4Address::IsSiteLocalAddress(
    /* [out] */ Boolean* isSiteLocalAddress)
{
    ArrayOf<Byte>& ipAddress = *mIpAddress;
    VALIDATE_NOT_NULL(isSiteLocalAddress);
    *isSiteLocalAddress = ((ipAddress[0] & 0xFF) == 10) ||
        (((ipAddress[0] & 0xFF) == 172) && ((ipAddress[1] & 0xF0) == 16)) ||
        (((ipAddress[0] & 0xFF) == 192) && ((ipAddress[1] & 0xFF) == 168));
    return NOERROR;
}

ECode CInet4Address::IsMCGlobal(
    /* [out] */ Boolean* isMCGlobal)
{
    VALIDATE_NOT_NULL(isMCGlobal);
    // Check if we have a prefix of 1110
    Boolean isMulticastAddress;
    IsMulticastAddress(&isMulticastAddress);
    if (!isMulticastAddress) {
        *isMCGlobal = FALSE;
        return NOERROR;
    }

    Int32 address = InetAddress::BytesToInt32(*mIpAddress, 0);
    /*
     * Now check the boundaries of the global space if we have an address
     * that is prefixed by something less than 111000000000000000000001
     * (fortunately we don't have to worry about sign after shifting 8 bits
     * right) it is not multicast. ( < 224.0.1.0)
     */
    if (((UInt32)address) >> 8 < 0xE00001) {
        *isMCGlobal = FALSE;
        return NOERROR;
    }

    /*
     * Now check the high boundary which is prefixed by 11101110 = 0xEE. If
     * the value is higher than this than it is not MCGlobal ( >
     * 238.255.255.255 )
     */
    if (((UInt32)address) >> 24 > 0xEE) {
        *isMCGlobal = FALSE;
        return NOERROR;
    }

    *isMCGlobal = TRUE;
    return NOERROR;
}

ECode CInet4Address::IsMCNodeLocal(
    /* [out] */ Boolean* isMCNodeLocal)
{
    VALIDATE_NOT_NULL(isMCNodeLocal);
    *isMCNodeLocal = FALSE;
    return NOERROR;
}

ECode CInet4Address::IsMCLinkLocal(
    /* [out] */ Boolean* isMCLinkLocal)
{
    VALIDATE_NOT_NULL(isMCLinkLocal);
    *isMCLinkLocal = (((*mIpAddress)[0] & 0xff) == 224) &&
                     ((*mIpAddress)[1] == 0) &&
                     ((*mIpAddress)[2] == 0);
    return NOERROR;
}

ECode CInet4Address::IsMCSiteLocal(
    /* [out] */ Boolean* isMCSiteLocal)
{
    VALIDATE_NOT_NULL(isMCSiteLocal);
    *isMCSiteLocal = (((*mIpAddress)[0] & 0xff) == 239) &&
                     (((*mIpAddress)[1] & 0xff) == 255);
    return NOERROR;
}

ECode CInet4Address::IsMCOrgLocal(
    /* [out] */ Boolean* isMCOrgLocal)
{
    VALIDATE_NOT_NULL(isMCOrgLocal);
    *isMCOrgLocal = (((*mIpAddress)[0] & 0xff) == 239) &&
                    (((*mIpAddress)[1] & 0xfc) == 192);
    return NOERROR;
}

ECode CInet4Address::IsAnyLocalAddress(
    /* [out] */ Boolean* isAnyLocalAddress)
{
    VALIDATE_NOT_NULL(isAnyLocalAddress);
    if(((*mIpAddress)[0] == 0) && ((*mIpAddress)[1] == 0) &&
       ((*mIpAddress)[2] == 0) && ((*mIpAddress)[3] == 0))
    {
        *isAnyLocalAddress = TRUE;
    }else{
        *isAnyLocalAddress = FALSE;
    }
    return NOERROR;
}

// private Object writeReplace() throws ObjectStreamException {
//     return new Inet4Address(ipaddress, hostName);
// }

ECode CInet4Address::IsReachable(
    /* [in] */ Int32 timeout,
    /* [out] */ Boolean* isReachable)
{
    VALIDATE_NOT_NULL(isReachable);
    return InetAddress::IsReachable(timeout, isReachable);
}

ECode CInet4Address::IsReachable(
    /* [in] */ INetworkInterface* networkInterface,
    /* [in] */ Int32 ttl,
    /* [in] */ Int32 timeout,
    /* [out] */ Boolean* isReachable)
{
    VALIDATE_NOT_NULL(isReachable);
    return InetAddress::IsReachable(networkInterface, ttl, timeout, isReachable);
}

ECode CInet4Address::constructor(
    /* [in] */ ArrayOf<Byte>* address,
    /* [in] */ const String& name)
{
    mFamily = AF_INET;
    mIpAddress = address;
    mHostName = name;
    return NOERROR;
}

ECode CInet4Address::Equals(
    /* [in] */ IInterface* obj,
    /* [out] */ Boolean* result)
{
    return InetAddress::Equals(obj, result);
}

ECode CInet4Address::ToString(
    /* [out] */ String* result)
{
    return InetAddress::ToString(result);
}

ECode CInet4Address::GetANY(
    /* [out] */ IInetAddress** result)
{
    VALIDATE_NOT_NULL(result);
    *result = ANY;
    REFCOUNT_ADD(*result);
    return NOERROR;
}

ECode CInet4Address::GetLOOPBACK(
    /* [out] */ IInetAddress** result)
{
    VALIDATE_NOT_NULL(result);
    *result = LOOPBACK;
    REFCOUNT_ADD(*result);
    return NOERROR;
}

ECode CInet4Address::GetALL(
    /* [out] */ IInetAddress** result)
{
    VALIDATE_NOT_NULL(result);
    *result = ALL;
    REFCOUNT_ADD(*result);
    return NOERROR;
}

} // namespace Net
} // namespace Elastos
