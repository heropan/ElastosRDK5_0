
#ifndef __ELASTOS_DROID_BLUETOOTH_CBLUETOOTHPBAPSTATECHANGECALLBACK_H__
#define __ELASTOS_DROID_BLUETOOTH_CBLUETOOTHPBAPSTATECHANGECALLBACK_H__

#include "_CBluetoothPbapStateChangeCallback.h"
#include "BluetoothPbap.h"

namespace Elastos {
namespace Droid {
namespace Bluetooth {

CarClass(CBluetoothPbapStateChangeCallback)
{
public:
    CARAPI OnBluetoothStateChange(
        /* [in] */ Boolean on);

    CARAPI constructor(
        /* [in] */ Handle32 host);

private:
	BluetoothPbap* mHost;
};

} // namespace Bluetooth
} // namespace Droid
} // namespace Elastos

#endif //__ELASTOS_DROID_BLUETOOTH_CBLUETOOTHPBAPSTATECHANGECALLBACK_H__
