
#ifndef __CBLUETOOTHADAPTERMANAGERCALLBACK_H__
#define __CBLUETOOTHADAPTERMANAGERCALLBACK_H__

#include "_CBluetoothAdapterManagerCallback.h"
#include "CBluetoothAdapter.h"

namespace Elastos {
namespace Droid {
namespace Bluetooth {

CarClass(CBluetoothAdapterManagerCallback)
{
public:
    CARAPI OnBluetoothServiceUp(
        /* [in] */ IIBluetooth* bluetoothService);

    CARAPI OnBluetoothServiceDown();

    CARAPI constructor(
    	/* [in] */ IBluetoothAdapter* host);

private:
	CBluetoothAdapter* mHost;
};

} // namespace Bluetooth
} // namespace Droid
} // namespace Elastos

#endif //__CBLUETOOTHADAPTERMANAGERCALLBACK_H__
