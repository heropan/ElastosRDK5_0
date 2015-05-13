
#ifndef __CINETADDRESS_H__
#define __CINETADDRESS_H__

#include "_CInetAddress.h"
#include "InetAddress.h"

namespace Elastos {
namespace Net {

CarClass(CInetAddress) , public InetAddress
{
public:
    CARAPI_(PInterface) Probe(
        /* [in] */ REIID riid);

    CARAPI constructor();

    /**
     * Constructs an {@code InetAddress}.
     *
     * Note: this constructor is for subclasses only.
     */
    CARAPI constructor(
        /* [in] */ Int32 family,
        /* [in] */ ArrayOf<Byte>* ipaddress,
        /* [in] */ const String& hostName);

    /**
     * Returns the IP address represented by this {@code InetAddress} instance
     * as a byte array. The elements are in network order (the highest order
     * address byte is in the zeroth element).
     *
     * @return the address in form of a byte array.
     */
    CARAPI GetAddress(
        /* [out, callee] */ ArrayOf<Byte>** address);

    /**
     * Returns the numeric representation of this IP address (such as "127.0.0.1").
     */
    CARAPI GetHostAddress(
        /* [out] */ String* hostAddress);

    /**
     * Returns the host name corresponding to this IP address. This may or may not be a
     * fully-qualified name. If the IP address could not be resolved, the numeric representation
     * is returned instead (see {@link #getHostAddress}).
     */
    CARAPI GetHostName(
        /* [out] */ String* hostName);

    /**
     * Returns the fully qualified hostname corresponding to this IP address.
     */
    CARAPI GetCanonicalHostName(
        /* [out] */ String* hostName);

    /**
     * Returns whether this is the IPv6 unspecified wildcard address {@code ::}
     * or the IPv4 "any" address, {@code 0.0.0.0}.
     */
    CARAPI GetHashCode(
        /* [out] */ Int32* hashCode);

    CARAPI IsAnyLocalAddress(
        /* [out] */ Boolean* isAnyLocalAddress);

    /**
     * Returns whether this address is a link-local address or not.
     *
     * <p>Valid IPv6 link-local addresses have the prefix {@code fe80::/10}.
     *
     * <p><a href="http://www.ietf.org/rfc/rfc3484.txt">RFC 3484</a>
     * "Default Address Selection for Internet Protocol Version 6 (IPv6)" states
     * that both IPv4 auto-configuration addresses (prefix {@code 169.254/16}) and
     * IPv4 loopback addresses (prefix {@code 127/8}) have link-local scope, but
     * {@link Inet4Address} only considers the auto-configuration addresses
     * to have link-local scope. That is: the IPv4 loopback address returns false.
     */
    CARAPI IsLinkLocalAddress(
        /* [out] */ Boolean* isLinkLocalAddress);

    /**
     * Returns whether this address is a loopback address or not.
     *
     * <p>Valid IPv4 loopback addresses have the prefix {@code 127/8}.
     *
     * <p>The only valid IPv6 loopback address is {@code ::1}.
     */
    CARAPI IsLoopbackAddress(
        /* [out] */ Boolean* isLoopbackAddress);

    /**
     * Returns whether this address is a global multicast address or not.
     *
     * <p>Valid IPv6 global multicast addresses have the prefix {@code ffxe::/16},
     * where {@code x} is a set of flags and the additional 112 bits make
     * up the global multicast address space.
     *
     * <p>Valid IPv4 global multicast addresses are the range of addresses
     * from {@code 224.0.1.0} to {@code 238.255.255.255}.
     */
    CARAPI IsMCGlobal(
        /* [out] */ Boolean* isMCGlobal);

    /**
     * Returns whether this address is a link-local multicast address or not.
     *
     * <p>Valid IPv6 link-local multicast addresses have the prefix {@code ffx2::/16},
     * where x is a set of flags and the additional 112 bits make up the link-local multicast
     * address space.
     *
     * <p>Valid IPv4 link-local multicast addresses have the prefix {@code 224.0.0/24}.
     */
    CARAPI IsMCLinkLocal(
        /* [out] */ Boolean* isMCLinkLocal);

    /**
     * Returns whether this address is a node-local multicast address or not.
     *
     * <p>Valid IPv6 node-local multicast addresses have the prefix {@code ffx1::/16},
     * where x is a set of flags and the additional 112 bits make up the link-local multicast
     * address space.
     *
     * <p>There are no valid IPv4 node-local multicast addresses.
     */
    CARAPI IsMCNodeLocal(
        /* [out] */ Boolean* isMCNodeLocal);

    /**
     * Returns whether this address is a organization-local multicast address or not.
     *
     * <p>Valid IPv6 organization-local multicast addresses have the prefix {@code ffx8::/16},
     * where x is a set of flags and the additional 112 bits make up the link-local multicast
     * address space.
     *
     * <p>Valid IPv4 organization-local multicast addresses have the prefix {@code 239.192/14}.
     */
    CARAPI IsMCOrgLocal(
        /* [out] */ Boolean* isMCOrgLocal);

    /**
     * Returns whether this address is a site-local multicast address or not.
     *
     * <p>Valid IPv6 site-local multicast addresses have the prefix {@code ffx5::/16},
     * where x is a set of flags and the additional 112 bits make up the link-local multicast
     * address space.
     *
     * <p>Valid IPv4 site-local multicast addresses have the prefix {@code 239.255/16}.
     */
    CARAPI IsMCSiteLocal(
        /* [out] */ Boolean* isMCSiteLocal);

    /**
     * Returns whether this address is a multicast address or not.
     *
     * <p>Valid IPv6 multicast addresses have the prefix {@code ff::/8}.
     *
     * <p>Valid IPv4 multicast addresses have the prefix {@code 224/4}.
     */
    CARAPI IsMulticastAddress(
        /* [out] */ Boolean* isMulticastAddress);

    /**
     * Returns whether this address is a site-local address or not.
     *
     * <p>For the purposes of this method, valid IPv6 site-local addresses have
     * the deprecated prefix {@code fec0::/10} from
     * <a href="http://www.ietf.org/rfc/rfc1884.txt">RFC 1884</a>,
     * <i>not</i> the modern prefix {@code fc00::/7} from
     * <a href="http://www.ietf.org/rfc/rfc4193.txt">RFC 4193</a>.
     *
     * <p><a href="http://www.ietf.org/rfc/rfc3484.txt">RFC 3484</a>
     * "Default Address Selection for Internet Protocol Version 6 (IPv6)" states
     * that IPv4 private addresses have the prefix {@code 10/8}, {@code 172.16/12},
     * or {@code 192.168/16}.
     *
     * @return {@code true} if this instance represents a site-local address,
     *         {@code false} otherwise.
     */
    CARAPI IsSiteLocalAddress(
        /* [out] */ Boolean* isSiteLocalAddress);

    /**
     * Tries to reach this {@code InetAddress}. This method first tries to use
     * ICMP <i>(ICMP ECHO REQUEST)</i>, falling back to a TCP connection
     * on port 7 (Echo) of the remote host.
     *
     * @param timeout
     *            timeout in milliseconds before the test fails if no connection
     *            could be established.
     * @return {@code true} if this address is reachable, {@code false}
     *         otherwise.
     * @throws IOException
     *             if an error occurs during an I/O operation.
     * @throws IllegalArgumentException
     *             if timeout is less than zero.
     */
    CARAPI IsReachable(
        /* [in] */ Int32 timeout,
        /* [out] */ Boolean* isReachable);

    /**
     * Tries to reach this {@code InetAddress}. This method first tries to use
     * ICMP <i>(ICMP ECHO REQUEST)</i>, falling back to a TCP connection
     * on port 7 (Echo) of the remote host.
     *
     * @param networkInterface
     *            the network interface on which to connection should be
     *            established.
     * @param ttl
     *            the maximum count of hops (time-to-live).
     * @param timeout
     *            timeout in milliseconds before the test fails if no connection
     *            could be established.
     * @return {@code true} if this address is reachable, {@code false}
     *         otherwise.
     * @throws IOException
     *             if an error occurs during an I/O operation.
     * @throws IllegalArgumentException
     *             if ttl or timeout is less than zero.
     */
    CARAPI IsReachableEx(
        /* [in] */ INetworkInterface* networkInterface,
        /* [in] */ Int32 ttl,
        /* [in] */ Int32 timeout,
        /* [out] */ Boolean* isReachable);

    /**
     * Compares this {@code InetAddress} instance against the specified address
     * in {@code obj}. Two addresses are equal if their address byte arrays have
     * the same length and if the bytes in the arrays are equal.
     *
     * @param obj
     *            the object to be tested for equality.
     * @return {@code true} if both objects are equal, {@code false} otherwise.
     */
    CARAPI Equals(
        /* [in] */ IInterface* obj,
        /* [out] */ Boolean* result);

    /**
     * Returns a string containing the host name (if available) and host address.
     * For example: {@code "www.google.com/74.125.224.115"} or {@code "/127.0.0.1"}.
     *
     * <p>IPv6 addresses may additionally include an interface name or scope id.
     * For example: {@code "www.google.com/2001:4860:4001:803::1013%eth0"} or
     * {@code "/2001:4860:4001:803::1013%2"}.
     */
    CARAPI ToString(
        /* [out] */ String* result);
};

} // namespace Net
} // namespace Elastos

#endif //__CINETADDRESS_H__
