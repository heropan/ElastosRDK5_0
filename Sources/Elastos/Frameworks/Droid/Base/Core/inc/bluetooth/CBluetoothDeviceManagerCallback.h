
#ifndef __CBLUETOOTHDEVICEMANAGERCALLBACK_H__
#define __CBLUETOOTHDEVICEMANAGERCALLBACK_H__

#include "_CBluetoothDeviceManagerCallback.h"

namespace Elastos {
namespace Droid {
namespace Bluetooth {

CarClass(CBluetoothDeviceManagerCallback)
{
public:
    CARAPI OnBluetoothServiceUp(
        /* [in] */ IIBluetooth* bluetoothService);

    CARAPI OnBluetoothServiceDown();
};

} // namespace Bluetooth
} // namespace Droid
} // namespace Elastos

#endif //__CBLUETOOTHDEVICEMANAGERCALLBACK_H__
