
#ifndef __ELASTOS_DROID_SERVER_NET_BASENETWORKOBSERVER_H__
#define __ELASTOS_DROID_SERVER_NET_BASENETWORKOBSERVER_H__

#include "elastos/droid/ext/frameworkdef.h"
#include <elastos.h>

namespace Elastos {
namespace Droid {
namespace Server {
namespace Net {

/**
 * Base {@link INetworkManagementEventObserver} that provides no-op
 * implementations which can be overridden.
 *
 * @hide
 */
class BaseNetworkObserver
{
public:
    BaseNetworkObserver() {};

    virtual ~BaseNetworkObserver() {};

    virtual CARAPI InterfaceStatusChanged(
        /* [in] */ const String& iface,
        /* [in] */ Boolean up);

    virtual CARAPI InterfaceRemoved(
        /* [in] */ const String& iface);

    virtual CARAPI InterfaceLinkStateChanged(
        /* [in] */ const String& iface,
        /* [in] */ Boolean up);

    virtual CARAPI InterfaceAdded(
        /* [in] */ const String& iface);

    virtual CARAPI InterfaceClassDataActivityChanged(
        /* [in] */ const String& label,
        /* [in] */ Boolean active);

    virtual CARAPI LimitReached(
        /* [in] */ const String& limitName,
        /* [in] */ const String& iface);
};

} // namespace Net
} // namespace Server
} // namepsace Droid
} // namespace Elastos

#endif // __ELASTOS_DROID_SERVER_NET_BASENETWORKOBSERVER_H__
