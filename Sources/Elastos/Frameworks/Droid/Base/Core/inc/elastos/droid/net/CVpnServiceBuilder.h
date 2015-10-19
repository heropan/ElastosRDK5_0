
#ifndef __ELASTOS_DROID_NET_CVPNSERVICEBUILDER_H__
#define __ELASTOS_DROID_NET_CVPNSERVICEBUILDER_H__

#include "_Elastos_Droid_Net_CVpnServiceBuilder.h"
#include "elastos/droid/net/VpnService.h"

namespace Elastos {
namespace Droid {
namespace Net {

/**
 * Helper class to create a VPN interface. This class should be always
 * used within the scope of the outer {@link VpnService}.
 *
 * @see VpnService
 */
CarClass(CVpnServiceBuilder)
    , public VpnServiceBuilder
{
public:
    CAR_OBJECT_DECL()

};

} // namespace Net
} // namespace Droid
} // namespace Elastos

#endif // __ELASTOS_DROID_NET_CVPNSERVICEBUILDER_H__

#if 0 // old CVpnServiceBuilder.h
#ifndef __ELASTOS_DROID_NET_CVPNSERVICEBUILDER_H__
#define __ELASTOS_DROID_NET_CVPNSERVICEBUILDER_H__

#include "_Elastos_Droid_Net_CVpnServiceBuilder.h"
#include <Logger.h>

using namespace Elastos::Utility::Logging;

namespace Elastos {
namespace Droid {
namespace Net {

/**
 * Helper class to create a VPN interface. This class should be always
 * used within the scope of the outer {@link VpnService}.
 *
 * @see VpnService
 */
CarClass(CVpnServiceBuilder)
{
public:
    CVpnServiceBuilder();

    /**
     * Set the name of this session. It will be displayed in
     * system-managed dialogs and notifications. This is recommended
     * not required.
     */
    CARAPI SetSession(
        /* [in] */ const String& session);

    /**
     * Set the {@link PendingIntent} to an activity for users to
     * configure the VPN connection. If it is not set, the button
     * to configure will not be shown in system-managed dialogs.
     */
    CARAPI SetConfigureIntent(
        /* [in] */ IPendingIntent* intent);

    /**
     * Set the maximum transmission unit (MTU) of the VPN interface. If
     * it is not set, the default value in the operating system will be
     * used.
     *
     * @throws IllegalArgumentException if the value is not positive.
     */
    CARAPI SetMtu(
        /* [in] */ Int32 mtu);

            /**
     * Add a network address to the VPN interface. Both IPv4 and IPv6
     * addresses are supported. At least one address must be set before
     * calling {@link #establish}.
     *
     * @throws IllegalArgumentException if the address is invalid.
     */
    CARAPI AddAddress(
        /* [in] */ IInetAddress* address,
        /* [in] */ Int32 prefixLength);

    /**
     * Convenience method to add a network address to the VPN interface
     * using a numeric address string. See {@link InetAddress} for the
     * definitions of numeric address formats.
     *
     * @throws IllegalArgumentException if the address is invalid.
     * @see #addAddress(InetAddress, int)
     */
    CARAPI AddAddress(
        /* [in] */ const String& address,
        /* [in] */ Int32 prefixLength);

    /**
     * Add a network route to the VPN interface. Both IPv4 and IPv6
     * routes are supported.
     *
     * @throws IllegalArgumentException if the route is invalid.
     */
    CARAPI AddRoute(
        /* [in] */ IInetAddress* address,
        /* [in] */ Int32 prefixLength);

    /**
     * Convenience method to add a network route to the VPN interface
     * using a numeric address string. See {@link InetAddress} for the
     * definitions of numeric address formats.
     *
     * @throws IllegalArgumentException if the route is invalid.
     * @see #addRoute(InetAddress, int)
     */
    CARAPI AddRoute(
        /* [in] */ const String& address,
        /* [in] */ Int32 prefixLength);

    /**
     * Add a DNS server to the VPN connection. Both IPv4 and IPv6
     * addresses are supported. If none is set, the DNS servers of
     * the default network will be used.
     *
     * @throws IllegalArgumentException if the address is invalid.
     */
    CARAPI AddDnsServer(
        /* [in] */ IInetAddress* address);

    /**
     * Convenience method to add a DNS server to the VPN connection
     * using a numeric address string. See {@link InetAddress} for the
     * definitions of numeric address formats.
     *
     * @throws IllegalArgumentException if the address is invalid.
     * @see #addDnsServer(InetAddress)
     */
    CARAPI AddDnsServer(
        /* [in] */ const String& address);

    /**
     * Add a search domain to the DNS resolver.
     */
    CARAPI AddSearchDomain(
        /* [in] */ const String& dnsDomain);

    /**
     * Create a VPN interface using the parameters supplied to this
     * builder. The interface works on IP packets, and a file descriptor
     * is returned for the application to access them. Each read
     * retrieves an outgoing packet which was routed to the interface.
     * Each write injects an incoming packet just like it was received
     * from the interface. The file descriptor is put into non-blocking
     * mode by default to avoid blocking Java threads. To use the file
     * descriptor completely in native space, see
     * {@link ParcelFileDescriptor#detachFd()}. The application MUST
     * close the file descriptor when the VPN connection is terminated.
     * The VPN interface will be removed and the network will be
     * restored by the system automatically.
     *
     * <p>To avoid conflicts, there can be only one active VPN interface
     * at the same time. Usually network parameters are never changed
     * during the lifetime of a VPN connection. It is also common for an
     * application to create a new file descriptor after closing the
     * previous one. However, it is rare but not impossible to have two
     * interfaces while performing a seamless handover. In this case, the
     * old interface will be deactivated when the new one is created
     * successfully. Both file descriptors are valid but now outgoing
     * packets will be routed to the new interface. Therefore, after
     * draining the old file descriptor, the application MUST close it
     * and start using the new file descriptor. If the new interface
     * cannot be created, the existing interface and its file descriptor
     * remain untouched.
     *
     * <p>An exception will be thrown if the interface cannot be created
     * for any reason. However, this method returns {@code null} if the
     * application is not prepared or is revoked. This helps solve
     * possible race conditions between other VPN applications.
     *
     * @return {@link ParcelFileDescriptor} of the VPN interface, or
     *         {@code null} if the application is not prepared.
     * @throws IllegalArgumentException if a parameter is not accepted
     *         by the operating system.
     * @throws IllegalStateException if a parameter cannot be applied
     *         by the operating system.
     * @throws SecurityException if the service is not properly declared
     *         in {@code AndroidManifest.xml}.
     * @see VpnService
     */
    CARAPI Establish(
        /* [out] */ IParcelFileDescriptor** pfd);

private:
    /**
     * Private method to validate address and prefixLength.
     */
    CARAPI Check(
        /* [in] */ IInetAddress* address,
        /* [in] */ Int32 prefixLength);

private:
    AutoPtr<IVpnConfig> mConfig;
    StringBuffer mAddresses;
    StringBuffer mRoutes;

};

} // namespace Net
} // namepsace Droid
} // namespace Elastos

#endif // __ELASTOS_DROID_NET_CVPNSERVICEBUILDER_H__
#endif