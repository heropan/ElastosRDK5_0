
#include "net/NetworkUtils.h"
#ifdef DROID_CORE
#include "net/CRouteInfo.h"
#else
#include "Elastos.Droid.Core.h"
#endif
#include <elastos/Math.h>
#include <elastos/StringBuffer.h>
#include <elastos/StringUtils.h>
#include <elastos/Logger.h>
#include <netutils/ifc.h>
#include <cutils/properties.h>
#include <elastos/StringBuilder.h>

using Elastos::Net::IInetAddress;
using Elastos::Net::CInetAddressHelper;
using Elastos::Core::StringBuilder;
using Elastos::Core::StringBuffer;
using Elastos::Core::StringUtils;
using Elastos::Utility::Logging::Logger;
using Elastos::Droid::Net::CRouteInfo;

extern "C" {
int ifc_enable(const char *ifname);
int ifc_disable(const char *ifname);
int ifc_reset_connections(const char *ifname, int reset_mask);

int dhcp_do_request(const char *ifname,
                    const char *ipaddr,
                    const char *gateway,
                    uint32_t *prefixLength,
                    const char *dns1,
                    const char *dns2,
                    const char *server,
                    uint32_t *lease,
                    const char *vendorInfo);

int dhcp_do_request_renew(const char *ifname,
                    const char *ipaddr,
                    const char *gateway,
                    uint32_t *prefixLength,
                    const char *dns1,
                    const char *dns2,
                    const char *server,
                    uint32_t *lease,
                    const char *vendorInfo);

int dhcp_stop(const char *ifname);
int dhcp_release_lease(const char *ifname);
char *dhcp_get_errmsg();
}

namespace Elastos {
namespace Droid {
namespace Net {

const String NetworkUtils::TAG = String("NetworkUtils");

/** Bring the named network interface up. */
Int32 NetworkUtils::EnableInterface(
    /* [in] */ const String& interfaceName)
{
    const char *nameStr = interfaceName.string();
    int n = ::ifc_enable(nameStr);
    return (Int32)n;
}

/** Bring the named network interface down. */
Int32 NetworkUtils::DisableInterface(
    /* [in] */ const String& interfaceName)
{
    const char *nameStr = interfaceName.string();
    int n = ::ifc_disable(nameStr);
    return (Int32)n;
}

/**
 * Reset IPv6 or IPv4 sockets that are connected via the named interface.
 *
 * @param interfaceName is the interface to reset
 * @param mask {@see #RESET_IPV4_ADDRESSES} and {@see #RESET_IPV6_ADDRESSES}
 */
Int32 NetworkUtils::ResetConnections(
    /* [in] */ const String& interfaceName,
    /* [in] */ Int32 mask)
{
    const char *nameStr = interfaceName;

//    ALOGD("android_net_utils_resetConnections in env=%p clazz=%p iface=%s mask=0x%x\n",
//          env, clazz, nameStr, mask);
    int n = ::ifc_reset_connections(nameStr, mask);
    return (Int32)n;
}

/**
 * Start the DHCP client daemon, in order to have it request addresses
 * for the named interface, and then configure the interface with those
 * addresses. This call blocks until it obtains a result (either success
 * or failure) from the daemon.
 * @param interfaceName the name of the interface to configure
 * @param ipInfo if the request succeeds, this object is filled in with
 * the IP address information.
 * @return {@code TRUE} for success, {@code FALSE} for failure
 */
Boolean NetworkUtils::RunDhcp(
    /* [in] */ const String& interfaceName,
    /* [in] */ DhcpInfoInternal* ipInfo)
{
    return NativeRunDhcpCommon(interfaceName, ipInfo, FALSE);
}

/**
 * Initiate renewal on the Dhcp client daemon. This call blocks until it obtains
 * a result (either success or failure) from the daemon.
 * @param interfaceName the name of the interface to configure
 * @param ipInfo if the request succeeds, this object is filled in with
 * the IP address information.
 * @return {@code TRUE} for success, {@code FALSE} for failure
 */
Boolean NetworkUtils::RunDhcpRenew(
    /* [in] */ const String& interfaceName,
    /* [in] */ DhcpInfoInternal* ipInfo)
{
    return NativeRunDhcpCommon(interfaceName, ipInfo, TRUE);
}

/**
 * Shut down the DHCP client daemon.
 * @param interfaceName the name of the interface for which the daemon
 * should be stopped
 * @return {@code TRUE} for success, {@code FALSE} for failure
 */
Boolean NetworkUtils::StopDhcp(
    /* [in] */ const String& interfaceName)
{
    const char *nameStr = interfaceName.string();
    int res = ::dhcp_stop(nameStr);
    return (Boolean)(res == 0);
}

/**
 * Release the current DHCP lease.
 * @param interfaceName the name of the interface for which the lease should
 * be released
 * @return {@code TRUE} for success, {@code FALSE} for failure
 */
Boolean NetworkUtils::ReleaseDhcpLease(
    /* [in] */ const String& interfaceName)
{
    const char *nameStr = interfaceName.string();
    int res = ::dhcp_release_lease(nameStr);
    return (Boolean)(res == 0);
}

/**
 * Return the last DHCP-related error message that was recorded.
 * <p/>NOTE: This string is not localized, but currently it is only
 * used in logging.
 * @return the most recent error message, if any
 */
String NetworkUtils::GetDhcpError()
{
    const char* ch = ::dhcp_get_errmsg();
    return String(ch);
}

/**
 * Convert a IPv4 address from an integer to an InetAddress.
 * @param hostAddress an Int32 corresponding to the IPv4 address in network Byte order
 */
ECode NetworkUtils::Int32ToInetAddress(
    /* [in] */ Int32 hostAddress,
    /* [out] */ IInetAddress** result)
{
    VALIDATE_NOT_NULL(result);

    AutoPtr< ArrayOf<Byte> > addressBytes = ArrayOf<Byte>::Alloc(4);
    addressBytes->Set(0, (Byte)(0xff & hostAddress));
    addressBytes->Set(1, (Byte)(0xff & (hostAddress >> 8)));
    addressBytes->Set(2, (Byte)(0xff & (hostAddress >> 16)));
    addressBytes->Set(3, (Byte)(0xff & (hostAddress >> 24)));
    AutoPtr<IInetAddress> inetaddress;
    AutoPtr<IInetAddressHelper> inetaddresshelper;
    CInetAddressHelper::AcquireSingleton((IInetAddressHelper**)&inetaddresshelper);
    inetaddresshelper->GetByAddress(addressBytes, (IInetAddress**)&inetaddress);
    *result = inetaddress;
    INTERFACE_ADDREF(*result);
    return NOERROR;
}

/**
 * Convert a IPv4 address from an InetAddress to an integer
 * @param inetAddr is an InetAddress corresponding to the IPv4 address
 * @return the IP address as an integer in network Byte order
 */
ECode NetworkUtils::InetAddressToInt32(
    /* [in] */ IInetAddress* inetAddr,
    /* [out] */ Int32* address)
{
    VALIDATE_NOT_NULL(address);
    *address = 0;

    AutoPtr<ArrayOf<Byte> > addr;
    inetAddr->GetAddress((ArrayOf<Byte>**)&addr);
    assert(addr != NULL);

    Int32 length = addr->GetLength();
    if (length != 4) {
        Logger::E("NetworkUtils::InetAddressToInt32", "Not an IPv4 address");
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
        // throw new IllegalArgumentException("Not an IPv4 address");
    }

    *address = (((*addr)[3] & 0xff) << 24) | (((*addr)[2] & 0xff) << 16) |
            (((*addr)[1] & 0xff) << 8) | ((*addr)[0] & 0xff);
    return NOERROR;
}

/**
 * Convert a network prefix length to an IPv4 netmask integer
 * @param prefixLength
 * @return the IPv4 netmask as an integer in network Byte order
 */
ECode NetworkUtils::PrefixLengthToNetmaskInt(
    /* [in] */ Int32 prefixLength,
    /* [out] */ Int32* result)
{
    VALIDATE_NOT_NULL(result);

    if (prefixLength < 0 || prefixLength > 32) {
//        throw new IllegalArgumentException("Invalid prefix length (0 <= prefix <= 32)");
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    Int32 value = (Int32)0xffffffff << (32 - prefixLength);
    *result = value;
    return NOERROR;
}

/**
 * Convert a IPv4 netmask integer to a prefix length
 * @param netmask as an integer in network Byte order
 * @return the network prefix length
 */
Int32 NetworkUtils::NetmaskIntToPrefixLength(
    /* [in] */ Int32 netmask)
{
    return Elastos::Core::Math::BitCount(netmask);
}

/**
 * Create an InetAddress from a string where the string must be a standard
 * representation of a V4 or V6 address.  Avoids doing a DNS lookup on failure
 * but it will throw an IllegalArgumentException in that case.
 * @param addrString
 * @return the InetAddress
 * @hide
 */
ECode NetworkUtils::NumericToInetAddress(
    /* [in] */ const String& addrString,
    /* [out] */ IInetAddress** result)
{
    VALIDATE_NOT_NULL(result);
    AutoPtr<IInetAddress> inetAddress;
    AutoPtr<IInetAddressHelper> inetaddresshelper;
    CInetAddressHelper::AcquireSingleton((IInetAddressHelper**)&inetaddresshelper);
    FAIL_RETURN(inetaddresshelper->ParseNumericAddress(addrString, (IInetAddress**)&inetAddress));
    *result = inetAddress;
    INTERFACE_ADDREF(*result);
    return NOERROR;
}

/**
 * Get InetAddress masked with prefixLength.  Will never return NULL.
 * @param IP address which will be masked with specified prefixLength
 * @param prefixLength the prefixLength used to mask the IP
 */
ECode NetworkUtils::GetNetworkPart(
    /* [in] */ IInetAddress* address,
    /* [in] */ Int32 prefixLength,
    /* [out] */ IInetAddress** result)
{
    VALIDATE_NOT_NULL(result);
    *result = NULL;

    if (address == NULL) {
//        throw new RuntimeException("getNetworkPart doesn't accept NULL address");
        return E_RUNTIME_EXCEPTION;
    }

    AutoPtr<ArrayOf<Byte> > array;
    address->GetAddress((ArrayOf<Byte>**)&array);

    if (prefixLength < 0 || prefixLength > array->GetLength() * 8) {
//        throw new RuntimeException("getNetworkPart - bad prefixLength");
        return E_RUNTIME_EXCEPTION;
    }

    Int32 offset = prefixLength / 8;
    Int32 reminder = prefixLength % 8;
    Byte mask = (Byte)(0xFF << (8 - reminder));

    if (offset < array->GetLength())
        array->Set(offset, (Byte)((*array)[offset] & mask));

    offset++;

    for (; offset < array->GetLength(); offset++) {
        array->Set(offset, 0);
    }

    AutoPtr<IInetAddress> netPart;
    AutoPtr<IInetAddressHelper> inetaddresshelper;
    CInetAddressHelper::AcquireSingleton((IInetAddressHelper**)&inetaddresshelper);
    inetaddresshelper->GetByAddress(array, (IInetAddress**)&netPart);
    *result = netPart;
    INTERFACE_ADDREF(*result);
    return NOERROR;
}

/**
 * Check if IP address type is consistent between two InetAddress.
 * @return TRUE if both are the same type.  FALSE otherwise.
 */
Boolean NetworkUtils::AddressTypeMatches(
    /* [in] */ IInetAddress* left,
    /* [in] */ IInetAddress* right)
{
    return (((IInet4Address::Probe(left) != NULL) && (IInet4Address::Probe(right) != NULL)) ||
            ((IInet6Address::Probe(left) != NULL) && (IInet6Address::Probe(right) != NULL)));
}

/**
 * Convert a 32 char hex string into a Inet6Address.
 * throws a runtime exception if the string isn't 32 chars, isn't hex or can't be
 * made into an Inet6Address
 * @param addrHexString a 32 character hex string representing an IPv6 addr
 * @return addr an InetAddress representation for the string
 */
ECode NetworkUtils::HexToInet6Address(
    /* [in] */ const String& addrHexString,
    /* [out] */ IInetAddress** result)
{
    VALIDATE_NOT_NULL(result);
//    try {
    StringBuilder builder(addrHexString.Substring(0,4));
    builder += String(":");
    builder += addrHexString.Substring(4,8);
    builder += String(":");
    builder += addrHexString.Substring(8,12);
    builder += String(":");
    builder += addrHexString.Substring(12,16);
    builder += String(":");
    builder += addrHexString.Substring(16,20);
    builder += String(":");
    builder += addrHexString.Substring(20,24);
    builder += String(":");
    builder += addrHexString.Substring(24,28);
    builder += String(":");
    builder += addrHexString.Substring(28,32);
    AutoPtr<IInetAddress> inetaddress;
    NumericToInetAddress(builder.ToString(), (IInetAddress**)&inetaddress);

 //   }
//    catch (Exception e) {
//        Log.e("NetworkUtils", "error in hexToInet6Address(" + addrHexString + "): " + e);
//        throw new IllegalArgumentException(e);
//    }
    *result = inetaddress;
    INTERFACE_ADDREF(*result);
    return NOERROR;
}

/**
 * Create a string array of host addresses from a collection of InetAddresses
 * @param addrs a Collection of InetAddresses
 * @return an array of Strings containing their host addresses
 */
AutoPtr<ArrayOf<String> > NetworkUtils::MakeStrings(
    /* [in] */ IObjectContainer* addrs)
{
    Int32 n;
    addrs->GetObjectCount(&n);
    AutoPtr<ArrayOf<String> > strs = ArrayOf<String>::Alloc(n);

    Int32 i = 0;
    AutoPtr<IObjectEnumerator> addrsEmu;
    addrs->GetObjectEnumerator((IObjectEnumerator**)&addrsEmu);
    Boolean hasNext = FALSE;
    while (addrsEmu->MoveNext(&hasNext), hasNext) {
        AutoPtr<IInetAddress> addr;
        addrsEmu->Current((IInterface**)&addr);
        addr->GetHostAddress(&(*strs)[i++]);
    }
    return strs;
}

/**
 * Trim leading zeros from IPv4 address strings
 * Our base libraries will interpret that as octel..
 * Must leave non v4 addresses and host names alone.
 * For example, 192.168.000.010 -> 192.168.0.10
 * TODO - fix base libraries and remove this function
 * @param addr a string representing an ip addr
 * @return a string propertly trimmed
 */
String NetworkUtils::TrimV4AddrZeros(
    /* [in] */ const String& addr)
{
    if (addr.IsNull()) {
        return String(NULL);
    }

    AutoPtr<ArrayOf<String> > octets;
    StringUtils::Split(addr, String("\\."), (ArrayOf<String>**)&octets);
    if (octets->GetLength() != 4) {
        return addr;
    }
    StringBuffer buff;
    for (Int32 i = 0; i < 4; i++) {
//        try {
            if ((*octets)[i].GetLength() > 3) {
                return addr;
            }
            buff += (*octets)[i];
//        } catch (NumberFormatException e) {
//            return addr;
//        }
        if (i < 3)
            buff += String(".");
    }
    return buff.ToString();
}

Boolean NetworkUtils::NativeRunDhcpCommon(
    /* [in] */ const String& ifname,
    /* [in] */ DhcpInfoInternal* info,
    /* [in] */ Boolean renew)
{
    Int32 num;
    char  ipaddr[PROPERTY_VALUE_MAX];
    uint32_t prefixLength;
    char gateway[PROPERTY_VALUE_MAX];
    char    dns1[PROPERTY_VALUE_MAX];
    char    dns2[PROPERTY_VALUE_MAX];
    char  server[PROPERTY_VALUE_MAX];
    uint32_t lease;
    char vendorInfo[PROPERTY_VALUE_MAX];

    const char *nameStr = ifname.string();
    if (nameStr == NULL) {
        return FALSE;
    }
    if (renew) {
        num = ::dhcp_do_request_renew(nameStr, ipaddr, gateway, &prefixLength,
                dns1, dns2, server, &lease, vendorInfo);
    }
    else {
        num = ::dhcp_do_request(nameStr, ipaddr, gateway, &prefixLength,
                dns1, dns2, server, &lease, vendorInfo);
    }

    if (num == 0) {
        info->mIpAddress = ipaddr;
        // set the gateway
        PFL_EX("TODO")
        AutoPtr<IInetAddressHelper> inetaddresshelper;
        CInetAddressHelper::AcquireSingleton((IInetAddressHelper**)&inetaddresshelper);
        AutoPtr<IInetAddress> inetAddress;
        inetaddresshelper->GetByName(String(gateway), (IInetAddress**)&inetAddress);
//          if (!env->ExceptionOccurred()) {
        AutoPtr<IRouteInfo> routeInfo;
        CRouteInfo::New(inetAddress, (IRouteInfo**)&routeInfo);
        info->AddRoute(routeInfo);
//        } else {
            // if we have an exception (host not found perhaps), just don't add the route
//            env->ExceptionClear();
//        }

        info->mPrefixLength = prefixLength;
        info->mDns1 = dns1;
        info->mDns2 = dns2;
        info->mServerAddress = server;
        info->mLeaseDuration = (Int32)lease;
        info->mVendorInfo = vendorInfo;

        //TODO: should be delete
        property_set("net.dns1", dns1);
    }
    return (Boolean)(num == 0);
}

} // namespace Net
} // namepsace Droid
} // namespace Elastos
