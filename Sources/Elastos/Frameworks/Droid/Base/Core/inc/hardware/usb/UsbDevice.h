
#ifndef __USBDEVICE_H__
#define __USBDEVICE_H__

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

#endif // __USBDEVICE_H__
