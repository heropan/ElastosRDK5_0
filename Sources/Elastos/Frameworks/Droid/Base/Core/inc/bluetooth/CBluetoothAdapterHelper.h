
#ifndef __ELASTOS_DROID_BLUETOOTH_CBLUETOOTHADAPTERHELPER_H__
#define __ELASTOS_DROID_BLUETOOTH_CBLUETOOTHADAPTERHELPER_H__

#include "_CBluetoothAdapterHelper.h"

namespace Elastos {
namespace Droid {
namespace Bluetooth {

CarClass(CBluetoothAdapterHelper)
{
public:
    CARAPI GetDefaultAdapter(
        /* [out] */ Elastos::Droid::Bluetooth::IBluetoothAdapter** adapter);

    CARAPI CheckBluetoothAddress(
        /* [in] */ const String& address,
        /* [out] */ Boolean* result);

private:
    // TODO: Add your private member variables here.
};

}
}
}

#endif // __ELASTOS_DROID_BLUETOOTH_CBLUETOOTHADAPTERHELPER_H__
