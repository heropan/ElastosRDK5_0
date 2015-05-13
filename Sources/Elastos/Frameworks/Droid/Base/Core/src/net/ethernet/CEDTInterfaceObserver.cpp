
#include "net/ethernet/CEDTInterfaceObserver.h"
#include "net/ethernet/EthernetDataTracker.h"

namespace Elastos {
namespace Droid {
namespace Net {
namespace Ethernet {

ECode CEDTInterfaceObserver::constructor(
    /* [in] */ Handle32 tracker)
{
    mTracker = (EthernetDataTracker*)tracker;
    return NOERROR;
}

ECode CEDTInterfaceObserver::InterfaceStatusChanged(
    /* [in] */ const String& iface,
    /* [in] */ Boolean up)
{
    // Slogger::D(TAG, "Interface status changed: " + iface + (up ? "up" : "down"));
    return NOERROR;
}

ECode CEDTInterfaceObserver::InterfaceLinkStateChanged(
    /* [in] */ const String& iface,
    /* [in] */ Boolean up)
{
    mTracker->InterfaceLinkStateChanged(iface, up);
    return NOERROR;
}

ECode CEDTInterfaceObserver::InterfaceAdded(
    /* [in] */ const String& iface)
{
    mTracker->InterfaceAdded(iface);
    return NOERROR;
}

ECode CEDTInterfaceObserver::InterfaceRemoved(
    /* [in] */ const String& iface)
{
    mTracker->InterfaceRemoved(iface);
    return NOERROR;
}

ECode CEDTInterfaceObserver::LimitReached(
    /* [in] */ const String& limitName,
    /* [in] */ const String& iface)
{
    // Ignored.
    return NOERROR;
}

ECode CEDTInterfaceObserver::InterfaceClassDataActivityChanged(
    /* [in] */ const String& label,
    /* [in] */ Boolean active)
{
    // Ignored.
    return NOERROR;
}

ECode CEDTInterfaceObserver::ToString(
    /* [out] */ String* str)
{
    return E_NOT_IMPLEMENTED;
}


} // Ethernet
} // Net
} // Droid
} // Elastos
