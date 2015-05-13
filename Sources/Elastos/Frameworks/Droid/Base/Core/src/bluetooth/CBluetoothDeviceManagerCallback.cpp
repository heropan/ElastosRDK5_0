
#include "CBluetoothDeviceManagerCallback.h"
#include "CBluetoothDevice.h"

namespace Elastos {
namespace Droid {
namespace Bluetooth {

ECode CBluetoothDeviceManagerCallback::OnBluetoothServiceUp(
    /* [in] */ IIBluetooth* bluetoothService)
{
	Mutex::Autolock lock(CBluetoothDevice::sLock);
    CBluetoothDevice::sService = bluetoothService;
    return NOERROR;
}

ECode CBluetoothDeviceManagerCallback::OnBluetoothServiceDown()
{
	Mutex::Autolock lock(CBluetoothDevice::sLock);
    CBluetoothDevice::sService = NULL;
    return NOERROR;
}

} // namespace Bluetooth
} // namespace Droid
} // namespace Elastos
