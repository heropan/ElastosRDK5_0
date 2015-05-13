
#ifndef __CBLUETOOTHPBAPSTATECHANGECALLBACK_H__
#define __CBLUETOOTHPBAPSTATECHANGECALLBACK_H__

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

#endif //__CBLUETOOTHPBAPSTATECHANGECALLBACK_H__
