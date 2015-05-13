
#ifndef __CBLUETOOTHSTATECHANGECALLBACKWRAPPER_H__
#define __CBLUETOOTHSTATECHANGECALLBACKWRAPPER_H__

#include "_CBluetoothStateChangeCallbackWrapper.h"
#include "CBluetoothAdapter.h"

namespace Elastos {
namespace Droid {
namespace Bluetooth {

CarClass(CBluetoothStateChangeCallbackWrapper)
{
public:
    CARAPI OnBluetoothStateChange(
        /* [in] */ Boolean on);

    CARAPI constructor(
        /* [in] */ IInterface* callback);

private:
	AutoPtr<CBluetoothAdapter::IBluetoothStateChangeCallback> mCallback;
};

} // namespace Bluetooth
} // namespace Droid
} // namespace Elastos

#endif //__CBLUETOOTHSTATECHANGECALLBACKWRAPPER_H__
