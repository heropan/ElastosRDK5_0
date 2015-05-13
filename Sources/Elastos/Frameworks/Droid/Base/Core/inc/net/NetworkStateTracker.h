
#ifndef __NETWORKSTATETRACKER_H__
#define __NETWORKSTATETRACKER_H__

#ifdef DROID_CORE
#include "Elastos.Droid.Core_server.h"
#else
#include "Elastos.Droid.Core.h"
#endif

using Elastos::Droid::Utility::IAttributeSet;
using Elastos::Droid::Content::IContext;
using Elastos::Droid::Os::IHandler;

namespace Elastos {
namespace Droid {
namespace Net {

/**
 * Interface provides the {@link com.android.server.ConnectivityService}
 * with three services. Events to the ConnectivityService when
 * changes occur, an API for controlling the network and storage
 * for network specific information.
 *
 * The Connectivity will call startMonitoring before any other
 * method is called.
 *
 * {@hide}
 */
interface INetworkStateTracker : public IInterface
{
public:
    /**
    * Begin monitoring data connectivity.
    *
    * This is the first method called when this interface is used.
    *
    * @param context is the current Android context
    * @param target is the Hander to which to return the events.
    */
    virtual CARAPI StartMonitoring(
        /* [in] */ IContext* ctx,
        /* [in] */ IHandler* target) = 0;

    /**
    * Fetch NetworkInfo for the network
    */
    virtual CARAPI GetNetworkInfo(
        /* [out] */ INetworkInfo** info) = 0;

    /**
    * Return the LinkProperties for the connection.
    *
    * @return a copy of the LinkProperties, is never null.
    */
    virtual CARAPI GetLinkProperties(
        /* [out] */ ILinkProperties** linkProperties) = 0;

    /**
    * A capability is an Integer/String pair, the capabilities
    * are defined in the class LinkSocket#Key.
    *
    * @return a copy of this connections capabilities, may be empty but never null.
    */
    virtual CARAPI GetLinkCapabilities(
        /* [out] */ ILinkCapabilities** linkCapabilities) = 0;

    /**
    * Return the system properties name associated with the tcp buffer sizes
    * for this network.
    */
    virtual CARAPI GetTcpBufferSizesPropName(
        /* [out] */ String* propName) = 0;

    /**
    * Disable connectivity to a network
    * @return {@code true} if a teardown occurred, {@code false} if the
    * teardown did not occur.
    */
    virtual CARAPI Teardown(
        /* [out] */ Boolean* result) = 0;

    /**
    * Reenable connectivity to a network after a {@link #teardown()}.
    * @return {@code true} if we're connected or expect to be connected
    */
    virtual CARAPI Reconnect(
        /* [out] */ Boolean* result) = 0;

    /**
    * Ready to switch on to the network after captive portal check
    */
    virtual CARAPI CaptivePortalCheckComplete() = 0;

    /**
    * Turn the wireless radio off for a network.
    * @param turnOn {@code true} to turn the radio on, {@code false}
    */
    virtual CARAPI SetRadio(
        /* [in] */ Boolean turnOn,
        /* [out] */ Boolean* result) = 0;

    /**
    * Returns an indication of whether this network is available for
    * connections. A value of {@code false} means that some quasi-permanent
    * condition prevents connectivity to this network.
    *
    * NOTE that this is broken on multi-connection devices.  Should be fixed in J release
    * TODO - fix on multi-pdp devices
    */
    virtual CARAPI IsAvailable(
        /* [out] */ Boolean* isAvailable) = 0;

    /**
    * User control of data connection through this network, typically persisted
    * internally.
    */
    virtual CARAPI SetUserDataEnable(
        /* [in] */ Boolean enabled) = 0;

    /**
    * Policy control of data connection through this network, typically not
    * persisted internally. Usually used when {@link NetworkPolicy#limitBytes}
    * is passed.
    */
    virtual CARAPI SetPolicyDataEnable(
        /* [in] */ Boolean enabled) = 0;

    /**
    * -------------------------------------------------------------
    * Storage API used by ConnectivityService for saving
    * Network specific information.
    * -------------------------------------------------------------
    */

    /**
    * Check if private DNS route is set for the network
    */
    virtual CARAPI IsPrivateDnsRouteSet(
        /* [out] */ Boolean* isSet) = 0;

    /**
    * Set a flag indicating private DNS route is set
    */
    virtual CARAPI SetPrivateDnsRoute(
        /* [in] */ Boolean enabled) = 0;

    /**
    * Check if default route is set
    */
    virtual CARAPI IsDefaultRouteSet(
        /* [out] */ Boolean* isSet) = 0;

    /**
    * Set a flag indicating default route is set for the network
    */
    virtual CARAPI SetDefaultRoute(
        /* [in] */ Boolean enabled) = 0;

    /**
    * Check if tear down was requested
    */
    virtual CARAPI IsTeardownRequested(
        /* [out] */ Boolean* isRequested) = 0;

    /**
    * Indicate tear down requested from connectivity
    */
    virtual CARAPI SetTeardownRequested(
        /* [in] */ Boolean isRequested) = 0;

    /**
    * An external dependency has been met/unmet
    */
    virtual CARAPI SetDependencyMet(
        /* [in] */ Boolean met) = 0;

public:
    /**
     * -------------------------------------------------------------
     * Event Interface back to ConnectivityService.
     *
     * The events that are to be sent back to the Handler passed
     * to startMonitoring when the particular event occurs.
     * -------------------------------------------------------------
     */

    /**
     * The network state has changed and the NetworkInfo object
     * contains the new state.
     *
     * msg.what = EVENT_STATE_CHANGED
     * msg.obj = NetworkInfo object
     */
    static const Int32 EVENT_STATE_CHANGED = 1;

    /**
     * msg.what = EVENT_CONFIGURATION_CHANGED
     * msg.obj = NetworkInfo object
     */
    static const Int32 EVENT_CONFIGURATION_CHANGED = 3;

    /**
     * msg.what = EVENT_RESTORE_DEFAULT_NETWORK
     * msg.obj = FeatureUser object
     */
    static const Int32 EVENT_RESTORE_DEFAULT_NETWORK = 6;

    /**
     * msg.what = EVENT_NETWORK_SUBTYPE_CHANGED
     * msg.obj = NetworkInfo object
     */
    static const Int32 EVENT_NETWORK_SUBTYPE_CHANGED = 7;
};

} // namespace Net
} // namespace Droid
} // namespace Elastos

#endif //__NETWORKSTATETRACKER_H__
