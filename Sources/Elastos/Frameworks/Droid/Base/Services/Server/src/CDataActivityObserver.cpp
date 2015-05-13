
#include "CDataActivityObserver.h"
#include <elastos/StringUtils.h>

using Elastos::Core::StringUtils;

namespace Elastos {
namespace Droid {
namespace Server {

ECode CDataActivityObserver::InterfaceStatusChanged(
    /* [in] */ const String& iface,
    /* [in] */ Boolean up)
{
    return BaseNetworkObserver::InterfaceStatusChanged(iface, up);
}

ECode CDataActivityObserver::InterfaceLinkStateChanged(
    /* [in] */ const String& iface,
    /* [in] */ Boolean up)
{
    return BaseNetworkObserver::InterfaceLinkStateChanged(iface, up);
}

ECode CDataActivityObserver::InterfaceAdded(
    /* [in] */ const String& iface)
{
    return BaseNetworkObserver::InterfaceAdded(iface);
}

ECode CDataActivityObserver::InterfaceRemoved(
    /* [in] */ const String& iface)
{
    return BaseNetworkObserver::InterfaceRemoved(iface);
}

ECode CDataActivityObserver::LimitReached(
    /* [in] */ const String& limitName,
    /* [in] */ const String& iface)
{
    return BaseNetworkObserver::LimitReached(limitName, iface);
}

ECode CDataActivityObserver::InterfaceClassDataActivityChanged(
    /* [in] */ const String& label,
    /* [in] */ Boolean active)
{
    Int32 deviceType = StringUtils::ParseInt32(label);
    mHost->SendDataActivityBroadcast(deviceType, active);
    return NOERROR;
}

ECode CDataActivityObserver::constructor(
    /* [in] */ Handle32 host)
{
    mHost = (CConnectivityService*)host;
    return NOERROR;
}

} // namespace Server
} // namepsace Droid
} // namespace Elastos
