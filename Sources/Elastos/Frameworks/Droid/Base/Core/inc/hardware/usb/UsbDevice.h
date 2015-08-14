
#ifndef __ELASTOS_DROID_HARDWARE_USB_USBDEVICE_H__
#define __ELASTOS_DROID_HARDWARE_USB_USBDEVICE_H__

#include "ext/frameworkext.h"

namespace Elastos {
namespace Droid {
namespace Hardware {
namespace Usb {

class UsbDevice
{
public:
    static CARAPI_(Int32) NativeGetDeviceId(
        /* [in] */ const String& name);

    static CARAPI_(String) NativeGetDeviceName(
        /* [in] */ Int32 id);
};

} // namespace Usb
} // namespace Hardware
} // namespace Droid
} // namespace Elastos

#endif // __ELASTOS_DROID_HARDWARE_USB_USBDEVICE_H__
