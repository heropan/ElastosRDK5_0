#ifndef __CEDTINTERFACEOBSERVER_H__
#define __CEDTINTERFACEOBSERVER_H__

#include "_CEDTInterfaceObserver.h"

namespace Elastos {
namespace Droid {
namespace Net {
namespace Ethernet {

class EthernetDataTracker;

CarClass(CEDTInterfaceObserver)
{
public:
    CARAPI constructor(
        /* [in] */ Handle32 tracker);

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

    CARAPI LimitReached(
        /* [in] */ const String& limitName,
        /* [in] */ const String& iface);

    CARAPI InterfaceClassDataActivityChanged(
        /* [in] */ const String& label,
        /* [in] */ Boolean active);

    CARAPI ToString(
        /* [out] */ String* str);

private:
    EthernetDataTracker* mTracker;
};

} // Ethernet
} // Net
} // Droid
} // Elastos

#endif // __CEDTINTERFACEOBSERVER_H__
