
#ifndef __CUSBDEVICEHELPER_H__
#define __CUSBDEVICEHELPER_H__

#include "_CUsbDeviceHelper.h"

namespace Elastos {
namespace Droid {
namespace Hardware {
namespace Usb {

CarClass(CUsbDeviceHelper)
{
public:
    CARAPI GetDeviceId(
        /* [in] */ const String& name,
        /* [out] */ Int32* id);

    CARAPI GetDeviceName(
        /* [in] */ Int32 id,
        /* [out] */ String* name);
};

} // namespace Usb
} // namespace Hardware
} // namespace Droid
} // namespace Elastos

#endif // __CUSBDEVICEHELPER_H__
