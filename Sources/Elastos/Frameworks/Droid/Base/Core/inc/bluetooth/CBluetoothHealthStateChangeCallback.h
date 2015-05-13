
#ifndef __CBLUETOOTHHEALTHSTATECHANGECALLBACK_H__
#define __CBLUETOOTHHEALTHSTATECHANGECALLBACK_H__

#include "_CBluetoothHealthStateChangeCallback.h"
#include "BluetoothHealth.h"

namespace Elastos {
namespace Droid {
namespace Bluetooth {

CarClass(CBluetoothHealthStateChangeCallback)
{
public:
    CARAPI OnBluetoothStateChange(
        /* [in] */ Boolean on);

    CARAPI constructor(
        /* [in] */ IInterface* host);

private:
	BluetoothHealth* mHost;
};

} // namespace Bluetooth
} // namespace Droid
} // namespace Elastos

#endif //__CBLUETOOTHHEALTHSTATECHANGECALLBACK_H__
