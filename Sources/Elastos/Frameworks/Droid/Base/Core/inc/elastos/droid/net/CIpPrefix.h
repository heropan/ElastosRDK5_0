
#ifndef __ELASTOS_DROID_NET_CIPPREFIX_H__
#define __ELASTOS_DROID_NET_CIPPREFIX_H__

#include "_Elastos_Droid_Net_CIpPrefix.h"
#include "elastos/droid/net/IpPrefix.h"

namespace Elastos {
namespace Droid {
namespace Net {

/**
 * This class represents an IP prefix, i.e., a contiguous block of IP addresses aligned on a
 * power of two boundary (also known as an "IP subnet"). A prefix is specified by two pieces of
 * information:
 *
 * <ul>
 * <li>A starting IP address (IPv4 or IPv6). This is the first IP address of the prefix.
 * <li>A prefix length. This specifies the length of the prefix by specifing the number of bits
 *     in the IP address, starting from the most significant bit in network byte order, that
 *     are constant for all addresses in the prefix.
 * </ul>
 *
 * For example, the prefix <code>192.0.2.0/24</code> covers the 256 IPv4 addresses from
 * <code>192.0.2.0</code> to <code>192.0.2.255</code>, inclusive, and the prefix
 * <code>2001:db8:1:2</code>  covers the 2^64 IPv6 addresses from <code>2001:db8:1:2::</code> to
 * <code>2001:db8:1:2:ffff:ffff:ffff:ffff</code>, inclusive.
 *
 * Objects of this class are immutable.
 */
CarClass(CIpPrefix)
    , public IpPrefix
{
public:
    CAR_OBJECT_DECL()
};

} // namespace Net
} // namespace Droid
} // namespace Elastos
#endif // __ELASTOS_DROID_NET_CIPPREFIX_H__
