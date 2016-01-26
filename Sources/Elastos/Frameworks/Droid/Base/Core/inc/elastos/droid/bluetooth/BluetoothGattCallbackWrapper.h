#ifndef __ELASTOS_DROID_BLUETOOTH_BLUETOOTHGATTCALLBACKWRAPPER_H__
#define __ELASTOS_DROID_BLUETOOTH_BLUETOOTHGATTCALLBACKWRAPPER_H__

#include "Elastos.Droid.Bluetooth.h"
#include "Elastos.Droid.Os.h"
#include "elastos/droid/ext/frameworkext.h"
#include <elastos/core/Object.h>

using Elastos::Droid::Bluetooth::LE::IAdvertiseSettings;
using Elastos::Droid::Bluetooth::LE::IScanResult;
using Elastos::Droid::Os::IBinder;
using Elastos::Droid::Os::IParcelUuid;
using Elastos::Utility::IList;

namespace Elastos {
namespace Droid {
namespace Bluetooth {

/**
  * Wrapper class for default implementation of IBluetoothGattCallback.
  *
  * @hide
  */
class BluetoothGattCallbackWrapper
    : public Object
    , public IBluetoothGattCallback
    , public IBinder
{
public:
    // @Override
    CARAPI OnClientRegistered(
        /* [in] */ Int32 status,
        /* [in] */ Int32 clientIf);

    // @Override
    CARAPI OnClientConnectionState(
        /* [in] */ Int32 status,
        /* [in] */ Int32 clientIf,
        /* [in] */ Boolean connected,
        /* [in] */ String address);

    // @Override
    CARAPI OnScanResult(
        /* [in] */ IScanResult* scanResult);

    // @Override
    CARAPI OnBatchScanResults(
        /* [in] */ IList* batchResults);// IScanResult

    // @Override
    CARAPI OnGetService(
        /* [in] */ String address,
        /* [in] */ Int32 srvcType,
        /* [in] */ Int32 srvcInstId,
        /* [in] */ IParcelUuid* srvcUuid);

    // @Override
    CARAPI OnGetIncludedService(
        /* [in] */ String address,
        /* [in] */ Int32 srvcType,
        /* [in] */ Int32 srvcInstId,
        /* [in] */ IParcelUuid* srvcUuid,
        /* [in] */ Int32 inclSrvcType,
        /* [in] */ Int32 inclSrvcInstId,
        /* [in] */ IParcelUuid* inclSrvcUuid);

    // @Override
    CARAPI OnGetCharacteristic(
        /* [in] */ String address,
        /* [in] */ Int32 srvcType,
        /* [in] */ Int32 srvcInstId,
        /* [in] */ IParcelUuid* srvcUuid,
        /* [in] */ Int32 charInstId,
        /* [in] */ IParcelUuid* charUuid,
        /* [in] */ Int32 charProps);

    // @Override
    CARAPI OnGetDescriptor(
        /* [in] */ String address,
        /* [in] */ Int32 srvcType,
        /* [in] */ Int32 srvcInstId,
        /* [in] */ IParcelUuid* srvcUuid,
        /* [in] */ Int32 charInstId,
        /* [in] */ IParcelUuid* charUuid,
        /* [in] */ Int32 descrInstId,
        /* [in] */ IParcelUuid* descrUuid);

    // @Override
    CARAPI OnSearchComplete(
        /* [in] */ String address,
        /* [in] */ Int32 status);

    // @Override
    CARAPI OnCharacteristicRead(
        /* [in] */ String address,
        /* [in] */ Int32 status,
        /* [in] */ Int32 srvcType,
        /* [in] */ Int32 srvcInstId,
        /* [in] */ IParcelUuid* srvcUuid,
        /* [in] */ Int32 charInstId,
        /* [in] */ IParcelUuid* charUuid,
        /* [in] */ ArrayOf<Byte>* value);

    // @Override
    CARAPI OnCharacteristicWrite(
        /* [in] */ String address,
        /* [in] */ Int32 status,
        /* [in] */ Int32 srvcType,
        /* [in] */ Int32 srvcInstId,
        /* [in] */ IParcelUuid* srvcUuid,
        /* [in] */ Int32 charInstId,
        /* [in] */ IParcelUuid* charUuid);

    // @Override
    CARAPI OnExecuteWrite(
        /* [in] */ String address,
        /* [in] */ Int32 status);

    // @Override
    CARAPI OnDescriptorRead(
        /* [in] */ String address,
        /* [in] */ Int32 status,
        /* [in] */ Int32 srvcType,
        /* [in] */ Int32 srvcInstId,
        /* [in] */ IParcelUuid* srvcUuid,
        /* [in] */ Int32 charInstId,
        /* [in] */ IParcelUuid* charUuid,
        /* [in] */ Int32 descrInstId,
        /* [in] */ IParcelUuid* descrUuid,
        /* [in] */ ArrayOf<Byte>* value);

    // @Override
    CARAPI OnDescriptorWrite(
        /* [in] */ String address,
        /* [in] */ Int32 status,
        /* [in] */ Int32 srvcType,
        /* [in] */ Int32 srvcInstId,
        /* [in] */ IParcelUuid* srvcUuid,
        /* [in] */ Int32 charInstId,
        /* [in] */ IParcelUuid* charUuid,
        /* [in] */ Int32 descrInstId,
        /* [in] */ IParcelUuid* descrUuid);

    // @Override
    CARAPI OnNotify(
        /* [in] */ String address,
        /* [in] */ Int32 srvcType,
        /* [in] */ Int32 srvcInstId,
        /* [in] */ IParcelUuid* srvcUuid,
        /* [in] */ Int32 charInstId,
        /* [in] */ IParcelUuid* charUuid,
        /* [in] */ ArrayOf<Byte>* value);

    // @Override
    CARAPI OnReadRemoteRssi(
        /* [in] */ String address,
        /* [in] */ Int32 rssi,
        /* [in] */ Int32 status);

    // @Override
    CARAPI OnMultiAdvertiseCallback(
        /* [in] */ Int32 status,
        /* [in] */ Boolean isStart,
        /* [in] */ IAdvertiseSettings* advertiseSettings);

    // @Override
    CARAPI OnConfigureMTU(
        /* [in] */ String address,
        /* [in] */ Int32 mtu,
        /* [in] */ Int32 status);

    // @Override
    CARAPI OnFoundOrLost(
        /* [in] */ Boolean onFound,
        /* [in] */ IScanResult* scanResult);
};

} // namespace Bluetooth
} // namespace Droid
} // namespace Elastos

#endif // __ELASTOS_DROID_BLUETOOTH_BLUETOOTHGATTCALLBACKWRAPPER_H__

