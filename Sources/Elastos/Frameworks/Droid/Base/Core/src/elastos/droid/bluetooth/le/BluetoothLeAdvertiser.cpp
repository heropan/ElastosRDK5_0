
#include "elastos/droid/bluetooth/le/BluetoothLeAdvertiser.h"

namespace Elastos {
namespace Droid {
namespace Bluetooth {
namespace LE {

//=====================================================================
//           BluetoothLeAdvertiser::AdvertiseCallbackWrapper
//=====================================================================
const Int32 BluetoothLeAdvertiser::AdvertiseCallbackWrapper::LE_CALLBACK_TIMEOUT_MILLIS;

BluetoothLeAdvertiser::AdvertiseCallbackWrapper::AdvertiseCallbackWrapper(
    /* [in] */ IAdvertiseCallback* advertiseCallback,
    /* [in] */ IAdvertiseData* advertiseData,
    /* [in] */ IAdvertiseData* scanResponse,
    /* [in] */ IAdvertiseSettings* settings,
    /* [in] */ IIBluetoothGatt* bluetoothGatt)
{
    // ==================before translated======================
    // mAdvertiseCallback = advertiseCallback;
    // mAdvertisement = advertiseData;
    // mScanResponse = scanResponse;
    // mSettings = settings;
    // mBluetoothGatt = bluetoothGatt;
    // mClientIf = 0;
}

ECode BluetoothLeAdvertiser::AdvertiseCallbackWrapper::StartRegisteration()
{
    // ==================before translated======================
    // synchronized (this) {
    //     if (mClientIf == -1) return;
    //
    //     try {
    //         UUID uuid = UUID.randomUUID();
    //         mBluetoothGatt.registerClient(new ParcelUuid(uuid), this);
    //         wait(LE_CALLBACK_TIMEOUT_MILLIS);
    //     } catch (InterruptedException | RemoteException e) {
    //         Log.e(TAG, "Failed to start registeration", e);
    //     }
    //     if (mClientIf > 0 && mIsAdvertising) {
    //         mLeAdvertisers.put(mAdvertiseCallback, this);
    //     } else if (mClientIf <= 0) {
    //         // Post internal error if registration failed.
    //         postStartFailure(mAdvertiseCallback,
    //                 AdvertiseCallback.ADVERTISE_FAILED_INTERNAL_ERROR);
    //     } else {
    //         // Unregister application if it's already registered but advertise failed.
    //         try {
    //             mBluetoothGatt.unregisterClient(mClientIf);
    //             mClientIf = -1;
    //         } catch (RemoteException e) {
    //             Log.e(TAG, "remote exception when unregistering", e);
    //         }
    //     }
    // }
    assert(0);
    return NOERROR;
}

ECode BluetoothLeAdvertiser::AdvertiseCallbackWrapper::StopAdvertising()
{
    // ==================before translated======================
    // synchronized (this) {
    //     try {
    //         mBluetoothGatt.stopMultiAdvertising(mClientIf);
    //         wait(LE_CALLBACK_TIMEOUT_MILLIS);
    //     } catch (InterruptedException | RemoteException e) {
    //         Log.e(TAG, "Failed to stop advertising", e);
    //     }
    //     // Advertise callback should have been removed from LeAdvertisers when
    //     // onMultiAdvertiseCallback was called. In case onMultiAdvertiseCallback is never
    //     // invoked and wait timeout expires, remove callback here.
    //     if (mLeAdvertisers.containsKey(mAdvertiseCallback)) {
    //         mLeAdvertisers.remove(mAdvertiseCallback);
    //     }
    // }
    assert(0);
    return NOERROR;
}

ECode BluetoothLeAdvertiser::AdvertiseCallbackWrapper::OnClientRegistered(
    /* [in] */ Int32 status,
    /* [in] */ Int32 clientIf)
{
    // ==================before translated======================
    // Log.d(TAG, "onClientRegistered() - status=" + status + " clientIf=" + clientIf);
    // synchronized (this) {
    //     if (status == BluetoothGatt.GATT_SUCCESS) {
    //         mClientIf = clientIf;
    //         try {
    //             mBluetoothGatt.startMultiAdvertising(mClientIf, mAdvertisement,
    //                     mScanResponse, mSettings);
    //             return;
    //         } catch (RemoteException e) {
    //             Log.e(TAG, "failed to start advertising", e);
    //         }
    //     }
    //     // Registration failed.
    //     mClientIf = -1;
    //     notifyAll();
    // }
    assert(0);
    return NOERROR;
}

ECode BluetoothLeAdvertiser::AdvertiseCallbackWrapper::OnMultiAdvertiseCallback(
    /* [in] */ Int32 status,
    /* [in] */ Boolean isStart,
    /* [in] */ IAdvertiseSettings* settings)
{
    VALIDATE_NOT_NULL(settings);
    // ==================before translated======================
    // synchronized (this) {
    //     if (isStart) {
    //         if (status == AdvertiseCallback.ADVERTISE_SUCCESS) {
    //             // Start success
    //             mIsAdvertising = true;
    //             postStartSuccess(mAdvertiseCallback, settings);
    //         } else {
    //             // Start failure.
    //             postStartFailure(mAdvertiseCallback, status);
    //         }
    //     } else {
    //         // unregister client for stop.
    //         try {
    //             mBluetoothGatt.unregisterClient(mClientIf);
    //             mClientIf = -1;
    //             mIsAdvertising = false;
    //             mLeAdvertisers.remove(mAdvertiseCallback);
    //         } catch (RemoteException e) {
    //             Log.e(TAG, "remote exception when unregistering", e);
    //         }
    //     }
    //     notifyAll();
    // }
    assert(0);
    return NOERROR;
}

//=====================================================================
//                BluetoothLeAdvertiser::InnerRunnable1
//=====================================================================
BluetoothLeAdvertiser::StartFailureRunnable::StartFailureRunnable(
    /* [in] */ BluetoothLeAdvertiser* owner)
    : mOwner(owner)
{
    // ==================before translated======================
    // mOwner = owner;
}

ECode BluetoothLeAdvertiser::StartFailureRunnable::Run()
{
    // ==================before translated======================
    // callback.onStartFailure(error);
    assert(0);
    return NOERROR;
}

//=====================================================================
//                BluetoothLeAdvertiser::InnerRunnable3
//=====================================================================
BluetoothLeAdvertiser::StartSuccessRunnable::StartSuccessRunnable(
    /* [in] */ BluetoothLeAdvertiser* owner)
    : mOwner(owner)
{
    // ==================before translated======================
    // mOwner = owner;
}

ECode BluetoothLeAdvertiser::StartSuccessRunnable::Run()
{
    // ==================before translated======================
    // callback.onStartSuccess(settings);
    assert(0);
    return NOERROR;
}

//=====================================================================
//                        BluetoothLeAdvertiser
//=====================================================================
const String BluetoothLeAdvertiser::TAG("BluetoothLeAdvertiser");
const Int32 BluetoothLeAdvertiser::MAX_ADVERTISING_DATA_BYTES;
const Int32 BluetoothLeAdvertiser::OVERHEAD_BYTES_PER_FIELD;
const Int32 BluetoothLeAdvertiser::FLAGS_FIELD_BYTES;
const Int32 BluetoothLeAdvertiser::MANUFACTURER_SPECIFIC_DATA_LENGTH;
const Int32 BluetoothLeAdvertiser::SERVICE_DATA_UUID_LENGTH;

CAR_INTERFACE_IMPL(BluetoothLeAdvertiser, Object, IBluetoothLeAdvertiser);

BluetoothLeAdvertiser::BluetoothLeAdvertiser()
{
}

BluetoothLeAdvertiser::BluetoothLeAdvertiser(
    /* [in] */ IIBluetoothManager* bluetoothManager)
{
    // ==================before translated======================
    // mBluetoothManager = bluetoothManager;
    // mBluetoothAdapter = BluetoothAdapter.getDefaultAdapter();
    // mHandler = new Handler(Looper.getMainLooper());
}

ECode BluetoothLeAdvertiser::StartAdvertising(
    /* [in] */ IAdvertiseSettings* settings,
    /* [in] */ IAdvertiseData* advertiseData,
    /* [in] */ IAdvertiseCallback* callback)
{
    VALIDATE_NOT_NULL(settings);
    VALIDATE_NOT_NULL(advertiseData);
    VALIDATE_NOT_NULL(callback);
    // ==================before translated======================
    // startAdvertising(settings, advertiseData, null, callback);
    assert(0);
    return NOERROR;
}

ECode BluetoothLeAdvertiser::StartAdvertising(
    /* [in] */ IAdvertiseSettings* settings,
    /* [in] */ IAdvertiseData* advertiseData,
    /* [in] */ IAdvertiseData* scanResponse,
    /* [in] */ IAdvertiseCallback* callback)
{
    VALIDATE_NOT_NULL(settings);
    VALIDATE_NOT_NULL(advertiseData);
    VALIDATE_NOT_NULL(scanResponse);
    VALIDATE_NOT_NULL(callback);
    // ==================before translated======================
    // synchronized (mLeAdvertisers) {
    //     BluetoothLeUtils.checkAdapterStateOn(mBluetoothAdapter);
    //     if (callback == null) {
    //         throw new IllegalArgumentException("callback cannot be null");
    //     }
    //     if (!mBluetoothAdapter.isMultipleAdvertisementSupported()) {
    //         postStartFailure(callback,
    //                 AdvertiseCallback.ADVERTISE_FAILED_FEATURE_UNSUPPORTED);
    //         return;
    //     }
    //     if (totalBytes(advertiseData) > MAX_ADVERTISING_DATA_BYTES ||
    //             totalBytes(scanResponse) > MAX_ADVERTISING_DATA_BYTES) {
    //         postStartFailure(callback, AdvertiseCallback.ADVERTISE_FAILED_DATA_TOO_LARGE);
    //         return;
    //     }
    //     if (mLeAdvertisers.containsKey(callback)) {
    //         postStartFailure(callback, AdvertiseCallback.ADVERTISE_FAILED_ALREADY_STARTED);
    //         return;
    //     }
    //
    //     IBluetoothGatt gatt;
    //     try {
    //         gatt = mBluetoothManager.getBluetoothGatt();
    //     } catch (RemoteException e) {
    //         Log.e(TAG, "Failed to get Bluetooth gatt - ", e);
    //         postStartFailure(callback, AdvertiseCallback.ADVERTISE_FAILED_INTERNAL_ERROR);
    //         return;
    //     }
    //     AdvertiseCallbackWrapper wrapper = new AdvertiseCallbackWrapper(callback, advertiseData,
    //             scanResponse, settings, gatt);
    //     wrapper.startRegisteration();
    // }
    assert(0);
    return NOERROR;
}

ECode BluetoothLeAdvertiser::StopAdvertising(
    /* [in] */ IAdvertiseCallback* callback)
{
    VALIDATE_NOT_NULL(callback);
    // ==================before translated======================
    // synchronized (mLeAdvertisers) {
    //     BluetoothLeUtils.checkAdapterStateOn(mBluetoothAdapter);
    //     if (callback == null) {
    //         throw new IllegalArgumentException("callback cannot be null");
    //     }
    //     AdvertiseCallbackWrapper wrapper = mLeAdvertisers.get(callback);
    //     if (wrapper == null) return;
    //     wrapper.stopAdvertising();
    // }
    assert(0);
    return NOERROR;
}

ECode BluetoothLeAdvertiser::Cleanup()
{
    // ==================before translated======================
    // mLeAdvertisers.clear();
    assert(0);
    return NOERROR;
}

Int32 BluetoothLeAdvertiser::TotalBytes(
    /* [in] */ IAdvertiseData* data)
{
    // ==================before translated======================
    // if (data == null) return 0;
    // int size = FLAGS_FIELD_BYTES; // flags field is always set.
    // if (data.getServiceUuids() != null) {
    //     int num16BitUuids = 0;
    //     int num32BitUuids = 0;
    //     int num128BitUuids = 0;
    //     for (ParcelUuid uuid : data.getServiceUuids()) {
    //         if (BluetoothUuid.is16BitUuid(uuid)) {
    //             ++num16BitUuids;
    //         } else if (BluetoothUuid.is32BitUuid(uuid)) {
    //             ++num32BitUuids;
    //         } else {
    //             ++num128BitUuids;
    //         }
    //     }
    //     // 16 bit service uuids are grouped into one field when doing advertising.
    //     if (num16BitUuids != 0) {
    //         size += OVERHEAD_BYTES_PER_FIELD +
    //                 num16BitUuids * BluetoothUuid.UUID_BYTES_16_BIT;
    //     }
    //     // 32 bit service uuids are grouped into one field when doing advertising.
    //     if (num32BitUuids != 0) {
    //         size += OVERHEAD_BYTES_PER_FIELD +
    //                 num32BitUuids * BluetoothUuid.UUID_BYTES_32_BIT;
    //     }
    //     // 128 bit service uuids are grouped into one field when doing advertising.
    //     if (num128BitUuids != 0) {
    //         size += OVERHEAD_BYTES_PER_FIELD +
    //                 num128BitUuids * BluetoothUuid.UUID_BYTES_128_BIT;
    //     }
    // }
    // for (ParcelUuid uuid : data.getServiceData().keySet()) {
    //     size += OVERHEAD_BYTES_PER_FIELD + SERVICE_DATA_UUID_LENGTH
    //             + byteLength(data.getServiceData().get(uuid));
    // }
    // for (int i = 0; i < data.getManufacturerSpecificData().size(); ++i) {
    //     size += OVERHEAD_BYTES_PER_FIELD + MANUFACTURER_SPECIFIC_DATA_LENGTH +
    //             byteLength(data.getManufacturerSpecificData().valueAt(i));
    // }
    // if (data.getIncludeTxPowerLevel()) {
    //     size += OVERHEAD_BYTES_PER_FIELD + 1; // tx power level value is one byte.
    // }
    // if (data.getIncludeDeviceName() && mBluetoothAdapter.getName() != null) {
    //     size += OVERHEAD_BYTES_PER_FIELD + mBluetoothAdapter.getName().length();
    // }
    // return size;
    assert(0);
    return 0;
}

Int32 BluetoothLeAdvertiser::ByteLength(
    /* [in] */ ArrayOf<Byte>* array)
{
    // ==================before translated======================
    // return array == null ? 0 : array.length;
    assert(0);
    return 0;
}

void BluetoothLeAdvertiser::PostStartFailure(
    /* [in] */ IAdvertiseCallback* callback,
    /* [in] */ Int32 error)
{
    // ==================before translated======================
    // mHandler.post(new Runnable() {
    //     @Override
    //     public void run() {
    //         callback.onStartFailure(error);
    //     }
    // });
    assert(0);
}

void BluetoothLeAdvertiser::PostStartSuccess(
    /* [in] */ IAdvertiseCallback* callback,
    /* [in] */ IAdvertiseSettings* settings)
{
    // ==================before translated======================
    // mHandler.post(new Runnable() {
    //
    //     @Override
    //     public void run() {
    //         callback.onStartSuccess(settings);
    //     }
    // });
    assert(0);
}

} // namespace LE
} // namespace Bluetooth
} // namespace Droid
} // namespace Elastos
