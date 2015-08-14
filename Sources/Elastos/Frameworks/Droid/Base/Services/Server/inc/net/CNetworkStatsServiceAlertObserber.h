
#ifndef __ELASTOS_DROID_SERVER_NET_CNETWORKSTATSSERVICEALERTOBSERBER_H__
#define __ELASTOS_DROID_SERVER_NET_CNETWORKSTATSSERVICEALERTOBSERBER_H__

#include "_CNetworkStatsServiceAlertObserber.h"
#include "net/CNetworkStatsService.h"
#include "net/BaseNetworkObserver.h"

namespace Elastos {
namespace Droid {
namespace Server {
namespace Net {

CarClass(CNetworkStatsServiceAlertObserber), public BaseNetworkObserver
{
public:
    /**
     * Interface configuration status has changed.
     *
     * @param iface The interface.
     * @param up True if the interface has been enabled.
     */
    CARAPI InterfaceStatusChanged(
        /* [in] */ const String& iface,
        /* [in] */ Boolean up);

    /**
     * Interface physical-layer link state has changed.  For Ethernet,
     * this method is invoked when the cable is plugged in or unplugged.
     *
     * @param iface The interface.
     * @param up  True if the physical link-layer connection signal is valid.
     */
    CARAPI InterfaceLinkStateChanged(
        /* [in] */ const String& iface,
        /* [in] */ Boolean up);

    /**
     * An interface has been added to the system
     *
     * @param iface The interface.
     */
    CARAPI InterfaceAdded(
        /* [in] */ const String& iface);

    /**
     * An interface has been removed from the system
     *
     * @param iface The interface.
     */
    CARAPI InterfaceRemoved(
        /* [in] */ const String& iface);

    /**
     * A networking quota limit has been reached. The quota might not
     * be specific to an interface.
     *
     * @param limitName The name of the limit that triggered.
     * @param iface The interface on which the limit was detected.
     */
    CARAPI LimitReached(
        /* [in] */ const String& limitName,
        /* [in] */ const String& iface);

    /**
     * Interface data activity status is changed.
     *
     * @param iface The interface.
     * @param active  True if the interface is actively transmitting data, false if it is idle.
     */
    CARAPI InterfaceClassDataActivityChanged(
        /* [in] */ const String& label,
        /* [in] */ Boolean active);

    CARAPI constructor(
        /* [in] */ Handle32 host);

private:
    CNetworkStatsService* mHost;
};

} // namespace Net
} // namespace Server
} // namepsace Droid
} // namespace Elastos

#endif //__ELASTOS_DROID_SERVER_NET_CNETWORKSTATSSERVICEALERTOBSERBER_H__
