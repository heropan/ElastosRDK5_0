
#ifndef __CBLUETOOTHHEADSETSTATECHANGECALLBACK_H__
#define __CBLUETOOTHHEADSETSTATECHANGECALLBACK_H__

#include "_CBluetoothHeadsetStateChangeCallback.h"
#include "BluetoothHeadset.h"

namespace Elastos {
namespace Droid {
namespace Bluetooth {

CarClass(CBluetoothHeadsetStateChangeCallback)
{
public:
    CARAPI OnBluetoothStateChange(
        /* [in] */ Boolean on);

    CARAPI constructor(
        /* [in] */ IInterface* host);

private:
	BluetoothHeadset* mHost;
};

} // namespace Bluetooth
} // namespace Droid
} // namespace Elastos

#endif //__CBLUETOOTHHEADSETSTATECHANGECALLBACK_H__
