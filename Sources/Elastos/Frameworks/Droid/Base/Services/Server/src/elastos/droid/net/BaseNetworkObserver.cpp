
#include "elastos/droid/net/BaseNetworkObserver.h"

namespace Elastos {
namespace Droid {
namespace Server {
namespace Net {

ECode BaseNetworkObserver::InterfaceStatusChanged(
    /* [in] */ const String& iface,
    /* [in] */ Boolean up)
{
    return NOERROR;
}

ECode BaseNetworkObserver::InterfaceRemoved(
    /* [in] */ const String& iface)
{
    return NOERROR;
}

ECode BaseNetworkObserver::InterfaceLinkStateChanged(
    /* [in] */ const String& iface,
    /* [in] */ Boolean up)
{
    return NOERROR;
}

ECode BaseNetworkObserver::InterfaceAdded(
    /* [in] */ const String& iface)
{
    return NOERROR;
}

ECode BaseNetworkObserver::InterfaceClassDataActivityChanged(
    /* [in] */ const String& label,
    /* [in] */ Boolean active)
{
    return NOERROR;
}

ECode BaseNetworkObserver::LimitReached(
    /* [in] */ const String& limitName,
    /* [in] */ const String& iface)
{
    return NOERROR;
}

} // namespace Net
} // namespace Server
} // namepsace Droid
} // namespace Elastos
