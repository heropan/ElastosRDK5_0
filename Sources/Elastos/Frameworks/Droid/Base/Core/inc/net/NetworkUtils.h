
#ifndef __ELASTOS_DROID_NET_CNETWORKUTILS_H__
#define __ELASTOS_DROID_NET_CNETWORKUTILS_H__

#include "ext/frameworkext.h"
#include "net/DhcpInfoInternal.h"
#include <elastos/Mutex.h>

using Elastos::Net::IInetAddress;
using Elastos::Net::IInetAddressHelper;
using Elastos::Net::IInet4Address;
using Elastos::Net::IInet6Address;
using Elastos::Droid::Net::DhcpInfoInternal;

namespace Elastos {
namespace Droid {
namespace Net {

class NetworkUtils
{
public:
    static CARAPI Int32ToInetAddress(
        /* [in] */ Int32 hostAddress,
        /* [out] */ IInetAddress** result);

    static CARAPI InetAddressToInt32(
        /* [in] */ IInetAddress* inetAddr,
        /* [out] */ Int32* result);

    static CARAPI PrefixLengthToNetmaskInt(
        /* [in] */ Int32 prefixLength,
        /* [out] */ Int32* result);

    static CARAPI_(Int32) NetmaskIntToPrefixLength(
        /* [in] */ Int32 netmask);

    static CARAPI NumericToInetAddress(
        /* [in] */ const String& addrString,
        /* [out] */ IInetAddress** result);

    static CARAPI GetNetworkPart(
        /* [in] */ IInetAddress* address,
        /* [in] */ Int32 prefixLength,
        /* [out] */ IInetAddress** result);

    static CARAPI_(Boolean) AddressTypeMatches(
        /* [in] */ IInetAddress* left,
        /* [in] */ IInetAddress* right);

    static CARAPI HexToInet6Address(
        /* [in] */ const String& addrHexString,
        /* [out] */ IInetAddress** result);

    static CARAPI_(AutoPtr<ArrayOf<String> >) MakeStrings(
        /* [in] */ IObjectContainer* addrs);

    static CARAPI_(String) TrimV4AddrZeros(
        /* [in] */ const String& addr);

    static CARAPI_(Int32) EnableInterface(
        /* [in] */ const String& interfaceName);

    static CARAPI_(Int32) DisableInterface(
        /* [in] */ const String& interfaceName);

    static CARAPI_(Int32) ResetConnections(
        /* [in] */ const String& interfaceName,
        /* [in] */ Int32 mask);

    static CARAPI_(Boolean) RunDhcp(
        /* [in] */ const String& interfaceName,
        /* [in] */ DhcpInfoInternal* ipInfo);

    static CARAPI_(Boolean) RunDhcpRenew(
        /* [in] */ const String& interfaceName,
        /* [in] */ DhcpInfoInternal* ipInfo);

    static CARAPI_(Boolean) StopDhcp(
        /* [in] */ const String& interfaceName);

    static CARAPI_(Boolean) ReleaseDhcpLease(
        /* [in] */ const String& interfaceName);

    static CARAPI_(String) GetDhcpError();

    static CARAPI_(Boolean) NativeRunDhcpCommon(
        /* [in] */ const String& ifname,
        /* [in] */ DhcpInfoInternal* info,
        /* [in] */ Boolean renew);

public:
    /** Setting bit 0 indicates reseting of IPv4 addresses required */
    static const Int32 RESET_IPV4_ADDRESSES = 0x01;

    /** Setting bit 1 indicates reseting of IPv4 addresses required */
    static const Int32 RESET_IPV6_ADDRESSES = 0x02;

    /** Reset all addresses */
    static const Int32 RESET_ALL_ADDRESSES = RESET_IPV4_ADDRESSES | RESET_IPV6_ADDRESSES;

private:
    static const String TAG;
};

} // namespace Net
} // namepsace Droid
} // namespace Elastos

#endif // __ELASTOS_DROID_NET_CNETWORKUTILS_H__

