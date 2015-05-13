
#ifndef __CBLUETOOTHINPUTDEVICESTATECHANGECALLBACK_H__
#define __CBLUETOOTHINPUTDEVICESTATECHANGECALLBACK_H__

#include "_CBluetoothInputDeviceStateChangeCallback.h"
#include "BluetoothInputDevice.h"

namespace Elastos {
namespace Droid {
namespace Bluetooth {

CarClass(CBluetoothInputDeviceStateChangeCallback)
{
public:
    CARAPI OnBluetoothStateChange(
        /* [in] */ Boolean on);

    CARAPI constructor(
        /* [in] */ IInterface* host);

private:
	BluetoothInputDevice* mHost;
};

} // namespace Bluetooth
} // namespace Droid
} // namespace Elastos

#endif //__CBLUETOOTHINPUTDEVICSTATECHANGECALLBACK_H__
