
#ifndef __ELASTOS_DROID_SERVER_CONNECTIVITY_CVPNOBSERVER_H__
#define __ELASTOS_DROID_SERVER_CONNECTIVITY_CVPNOBSERVER_H__

#include "_CVpnObserver.h"

namespace Elastos {
namespace Droid {
namespace Server {
namespace Connectivity {

class Vpn;

CarClass(CVpnObserver)
{
public:
    CARAPI constructor(
        /* [in] */ Handle32 owner);

    CARAPI InterfaceStatusChanged(
        /* [in] */ const String& iface,
        /* [in] */ Boolean up);

    CARAPI InterfaceLinkStateChanged(
        /* [in] */ const String& iface,
        /* [in] */ Boolean up);

    CARAPI InterfaceAdded(
        /* [in] */ const String& iface);

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

    CARAPI ToString(
        /* [out] */ String* str);

private:
    Vpn* mOwner;
};

} // Connectivity
} // Server
} // Droid
} // Elastos

#endif //__ELASTOS_DROID_SERVER_CONNECTIVITY_CVPNOBSERVER_H__
