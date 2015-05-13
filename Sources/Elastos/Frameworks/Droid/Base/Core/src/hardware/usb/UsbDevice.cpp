
#include "hardware/usb/UsbDevice.h"
#include <usbhost/usbhost.h>

namespace Elastos {
namespace Droid {
namespace Hardware {
namespace Usb {

Int32 UsbDevice::NativeGetDeviceId(
    /* [in] */ const String& name)
{
    const char *nameStr = name.string();
    return usb_device_get_unique_id_from_name(nameStr);
}

String UsbDevice::NativeGetDeviceName(
    /* [in] */ Int32 id)
{
    char* name = usb_device_get_name_from_unique_id(id);
    String deviceName = String(name);
    free(name);
    return deviceName;
}

} // namespace Usb
} // namespace Hardware
} // namespace Droid
} // namespace Elastos
