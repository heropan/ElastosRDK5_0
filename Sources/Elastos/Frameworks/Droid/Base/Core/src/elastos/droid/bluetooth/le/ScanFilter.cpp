
#include "Elastos.Droid.Os.h"
#include "elastos/droid/bluetooth/le/ScanFilter.h"

namespace Elastos {
namespace Droid {
namespace Bluetooth {
namespace LE {

//=====================================================================
//                         ScanFilter::Builder
//=====================================================================
CAR_INTERFACE_IMPL(ScanFilter::Builder, Object, IScanFilterBuilder);

ScanFilter::Builder::Builder()
{
}

ECode ScanFilter::Builder::SetDeviceName(
    /* [in] */ const String& deviceName)
{
    // ==================before translated======================
    // mDeviceName = deviceName;
    // return this;
    assert(0);
    return NOERROR;
}

ECode ScanFilter::Builder::SetDeviceAddress(
    /* [in] */ const String& deviceAddress)
{
    // ==================before translated======================
    // if (deviceAddress != null && !BluetoothAdapter.checkBluetoothAddress(deviceAddress)) {
    //     throw new IllegalArgumentException("invalid device address " + deviceAddress);
    // }
    // mDeviceAddress = deviceAddress;
    // return this;
    assert(0);
    return NOERROR;
}

ECode ScanFilter::Builder::SetServiceUuid(
    /* [in] */ IParcelUuid* serviceUuid)
{
    VALIDATE_NOT_NULL(serviceUuid);
    // ==================before translated======================
    // mServiceUuid = serviceUuid;
    // mUuidMask = null; // clear uuid mask
    // return this;
    assert(0);
    return NOERROR;
}

ECode ScanFilter::Builder::SetServiceUuid(
    /* [in] */ IParcelUuid* serviceUuid,
    /* [in] */ IParcelUuid* uuidMask)
{
    VALIDATE_NOT_NULL(serviceUuid);
    VALIDATE_NOT_NULL(uuidMask);
    // ==================before translated======================
    // if (mUuidMask != null && mServiceUuid == null) {
    //     throw new IllegalArgumentException("uuid is null while uuidMask is not null!");
    // }
    // mServiceUuid = serviceUuid;
    // mUuidMask = uuidMask;
    // return this;
    assert(0);
    return NOERROR;
}

ECode ScanFilter::Builder::SetServiceData(
    /* [in] */ IParcelUuid* serviceDataUuid,
    /* [in] */ ArrayOf<Byte>* serviceData)
{
    VALIDATE_NOT_NULL(serviceDataUuid);
    VALIDATE_NOT_NULL(serviceData);
    // ==================before translated======================
    // if (serviceDataUuid == null) {
    //     throw new IllegalArgumentException("serviceDataUuid is null");
    // }
    // mServiceDataUuid = serviceDataUuid;
    // mServiceData = serviceData;
    // mServiceDataMask = null; // clear service data mask
    // return this;
    assert(0);
    return NOERROR;
}

ECode ScanFilter::Builder::SetServiceData(
    /* [in] */ IParcelUuid* serviceDataUuid,
    /* [in] */ ArrayOf<Byte>* serviceData,
    /* [in] */ ArrayOf<Byte>* serviceDataMask)
{
    VALIDATE_NOT_NULL(serviceDataUuid);
    VALIDATE_NOT_NULL(serviceData);
    VALIDATE_NOT_NULL(serviceDataMask);
    // ==================before translated======================
    // if (serviceDataUuid == null) {
    //     throw new IllegalArgumentException("serviceDataUuid is null");
    // }
    // if (mServiceDataMask != null) {
    //     if (mServiceData == null) {
    //         throw new IllegalArgumentException(
    //                 "serviceData is null while serviceDataMask is not null");
    //     }
    //     // Since the mServiceDataMask is a bit mask for mServiceData, the lengths of the two
    //     // byte array need to be the same.
    //     if (mServiceData.length != mServiceDataMask.length) {
    //         throw new IllegalArgumentException(
    //                 "size mismatch for service data and service data mask");
    //     }
    // }
    // mServiceDataUuid = serviceDataUuid;
    // mServiceData = serviceData;
    // mServiceDataMask = serviceDataMask;
    // return this;
    assert(0);
    return NOERROR;
}

ECode ScanFilter::Builder::SetManufacturerData(
    /* [in] */ Int32 manufacturerId,
    /* [in] */ ArrayOf<Byte>* manufacturerData)
{
    VALIDATE_NOT_NULL(manufacturerData);
    // ==================before translated======================
    // if (manufacturerData != null && manufacturerId < 0) {
    //     throw new IllegalArgumentException("invalid manufacture id");
    // }
    // mManufacturerId = manufacturerId;
    // mManufacturerData = manufacturerData;
    // mManufacturerDataMask = null; // clear manufacturer data mask
    // return this;
    assert(0);
    return NOERROR;
}

ECode ScanFilter::Builder::SetManufacturerData(
    /* [in] */ Int32 manufacturerId,
    /* [in] */ ArrayOf<Byte>* manufacturerData,
    /* [in] */ ArrayOf<Byte>* manufacturerDataMask)
{
    VALIDATE_NOT_NULL(manufacturerData);
    VALIDATE_NOT_NULL(manufacturerDataMask);
    // ==================before translated======================
    // if (manufacturerData != null && manufacturerId < 0) {
    //     throw new IllegalArgumentException("invalid manufacture id");
    // }
    // if (mManufacturerDataMask != null) {
    //     if (mManufacturerData == null) {
    //         throw new IllegalArgumentException(
    //                 "manufacturerData is null while manufacturerDataMask is not null");
    //     }
    //     // Since the mManufacturerDataMask is a bit mask for mManufacturerData, the lengths
    //     // of the two byte array need to be the same.
    //     if (mManufacturerData.length != mManufacturerDataMask.length) {
    //         throw new IllegalArgumentException(
    //                 "size mismatch for manufacturerData and manufacturerDataMask");
    //     }
    // }
    // mManufacturerId = manufacturerId;
    // mManufacturerData = manufacturerData;
    // mManufacturerDataMask = manufacturerDataMask;
    // return this;
    assert(0);
    return NOERROR;
}

ECode ScanFilter::Builder::Build(
    /* [out] */ IScanFilter** result)
{
    VALIDATE_NOT_NULL(result);
    // ==================before translated======================
    // return new ScanFilter(mDeviceName, mDeviceAddress,
    //         mServiceUuid, mUuidMask,
    //         mServiceDataUuid, mServiceData, mServiceDataMask,
    //         mManufacturerId, mManufacturerData, mManufacturerDataMask);
    assert(0);
    return NOERROR;
}

CAR_INTERFACE_IMPL_2(ScanFilter, Object, IScanFilter, IParcelable);

ScanFilter::ScanFilter()
{
}

ECode ScanFilter::WriteToParcel(
    /* [in] */ IParcel* dest,
    /* [in] */ Int32 flags)
{
    VALIDATE_NOT_NULL(dest);
    // ==================before translated======================
    // dest.writeInt(mDeviceName == null ? 0 : 1);
    // if (mDeviceName != null) {
    //     dest.writeString(mDeviceName);
    // }
    // dest.writeInt(mDeviceAddress == null ? 0 : 1);
    // if (mDeviceAddress != null) {
    //     dest.writeString(mDeviceAddress);
    // }
    // dest.writeInt(mServiceUuid == null ? 0 : 1);
    // if (mServiceUuid != null) {
    //     dest.writeParcelable(mServiceUuid, flags);
    //     dest.writeInt(mServiceUuidMask == null ? 0 : 1);
    //     if (mServiceUuidMask != null) {
    //         dest.writeParcelable(mServiceUuidMask, flags);
    //     }
    // }
    // dest.writeInt(mServiceDataUuid == null ? 0 : 1);
    // if (mServiceDataUuid != null) {
    //     dest.writeParcelable(mServiceDataUuid, flags);
    //     dest.writeInt(mServiceData == null ? 0 : 1);
    //     if (mServiceData != null) {
    //         dest.writeInt(mServiceData.length);
    //         dest.writeByteArray(mServiceData);
    //
    //         dest.writeInt(mServiceDataMask == null ? 0 : 1);
    //         if (mServiceDataMask != null) {
    //             dest.writeInt(mServiceDataMask.length);
    //             dest.writeByteArray(mServiceDataMask);
    //         }
    //     }
    // }
    // dest.writeInt(mManufacturerId);
    // dest.writeInt(mManufacturerData == null ? 0 : 1);
    // if (mManufacturerData != null) {
    //     dest.writeInt(mManufacturerData.length);
    //     dest.writeByteArray(mManufacturerData);
    //
    //     dest.writeInt(mManufacturerDataMask == null ? 0 : 1);
    //     if (mManufacturerDataMask != null) {
    //         dest.writeInt(mManufacturerDataMask.length);
    //         dest.writeByteArray(mManufacturerDataMask);
    //     }
    // }
    assert(0);
    return NOERROR;
}

ECode ScanFilter::ReadFromParcel(
    /* [in] */ IParcel* source)
{
    assert(0);
    return NOERROR;
}

ECode ScanFilter::GetDeviceName(
    /* [out] */ String* str)
{
    VALIDATE_NOT_NULL(str);
    // ==================before translated======================
    // return mDeviceName;
    assert(0);
    *str = String("");
    return NOERROR;
}

ECode ScanFilter::GetServiceUuid(
    /* [out] */ IParcelUuid** uuid)
{
    VALIDATE_NOT_NULL(uuid);
    // ==================before translated======================
    // return mServiceUuid;
    assert(0);
    return NOERROR;
}

ECode ScanFilter::GetServiceUuidMask(
    /* [out] */ IParcelUuid** uuid)
{
    VALIDATE_NOT_NULL(uuid);
    // ==================before translated======================
    // return mServiceUuidMask;
    assert(0);
    return NOERROR;
}

ECode ScanFilter::GetDeviceAddress(
    /* [out] */ String* str)
{
    VALIDATE_NOT_NULL(str);
    // ==================before translated======================
    // return mDeviceAddress;
    assert(0);
    return NOERROR;
}

ECode ScanFilter::GetServiceData(
    /* [out, callee] */ ArrayOf<Byte>** result)
{
    VALIDATE_NOT_NULL(result);
    // ==================before translated======================
    // return mServiceData;
    assert(0);
    return NOERROR;;
}

ECode ScanFilter::GetServiceDataMask(
    /* [out, callee] */ ArrayOf<Byte>** result)
{
    VALIDATE_NOT_NULL(result);
    // ==================before translated======================
    // return mServiceDataMask;
    assert(0);
    return NOERROR;
}

ECode ScanFilter::GetServiceDataUuid(
    /* [out] */ IParcelUuid** uuid)
{
    VALIDATE_NOT_NULL(uuid);
    // ==================before translated======================
    // return mServiceDataUuid;
    assert(0);
    return NOERROR;
}

ECode ScanFilter::GetManufacturerId(
    /* [out] */ Int32* result)
{
    VALIDATE_NOT_NULL(result);
    // ==================before translated======================
    // return mManufacturerId;
    assert(0);
    return NOERROR;
}

ECode ScanFilter::GetManufacturerData(
    /* [out, callee] */ ArrayOf<Byte>** result)
{
    VALIDATE_NOT_NULL(result);
    // ==================before translated======================
    // return mManufacturerData;
    assert(0);
    return NOERROR;
}

ECode ScanFilter::GetManufacturerDataMask(
    /* [out, callee] */ ArrayOf<Byte>** result)
{
    VALIDATE_NOT_NULL(result);
    // ==================before translated======================
    // return mManufacturerDataMask;
    assert(0);
    return NOERROR;
}

ECode ScanFilter::Matches(
    /* [in] */ IScanResult* scanResult,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(scanResult);
    VALIDATE_NOT_NULL(result);
    // ==================before translated======================
    // if (scanResult == null) {
    //     return false;
    // }
    // BluetoothDevice device = scanResult.getDevice();
    // // Device match.
    // if (mDeviceAddress != null
    //         && (device == null || !mDeviceAddress.equals(device.getAddress()))) {
    //     return false;
    // }
    //
    // ScanRecord scanRecord = scanResult.getScanRecord();
    //
    // // Scan record is null but there exist filters on it.
    // if (scanRecord == null
    //         && (mDeviceName != null || mServiceUuid != null || mManufacturerData != null
    //                 || mServiceData != null)) {
    //     return false;
    // }
    //
    // // Local name match.
    // if (mDeviceName != null && !mDeviceName.equals(scanRecord.getDeviceName())) {
    //     return false;
    // }
    //
    // // UUID match.
    // if (mServiceUuid != null && !matchesServiceUuids(mServiceUuid, mServiceUuidMask,
    //         scanRecord.getServiceUuids())) {
    //     return false;
    // }
    //
    // // Service data match
    // if (mServiceDataUuid != null) {
    //     if (!matchesPartialData(mServiceData, mServiceDataMask,
    //             scanRecord.getServiceData(mServiceDataUuid))) {
    //         return false;
    //     }
    // }
    //
    // // Manufacturer data match.
    // if (mManufacturerId >= 0) {
    //     if (!matchesPartialData(mManufacturerData, mManufacturerDataMask,
    //             scanRecord.getManufacturerSpecificData(mManufacturerId))) {
    //         return false;
    //     }
    // }
    // // All filters match.
    // return true;
    assert(0);
    return NOERROR;
}

ECode ScanFilter::ToString(
    /* [out] */ String* info)
{
    VALIDATE_NOT_NULL(info);
    // ==================before translated======================
    // return "BluetoothLeScanFilter [mDeviceName=" + mDeviceName + ", mDeviceAddress="
    //         + mDeviceAddress
    //         + ", mUuid=" + mServiceUuid + ", mUuidMask=" + mServiceUuidMask
    //         + ", mServiceDataUuid=" + Objects.toString(mServiceDataUuid) + ", mServiceData="
    //         + Arrays.toString(mServiceData) + ", mServiceDataMask="
    //         + Arrays.toString(mServiceDataMask) + ", mManufacturerId=" + mManufacturerId
    //         + ", mManufacturerData=" + Arrays.toString(mManufacturerData)
    //         + ", mManufacturerDataMask=" + Arrays.toString(mManufacturerDataMask) + "]";
    assert(0);
    return NOERROR;
}

ECode ScanFilter::GetHashCode(
    /* [out] */ Int32* hashCode)
{
    VALIDATE_NOT_NULL(hashCode);
    // ==================before translated======================
    // return Objects.hash(mDeviceName, mDeviceAddress, mManufacturerId, mManufacturerData,
    //         mManufacturerDataMask, mServiceDataUuid, mServiceData, mServiceDataMask,
    //         mServiceUuid, mServiceUuidMask);
    assert(0);
    return 0;
}

ECode ScanFilter::Equals(
    /* [in] */ IInterface* obj,
    /* [out] */ Boolean* result)
{
    // ==================before translated======================
    // if (this == obj) {
    //     return true;
    // }
    // if (obj == null || getClass() != obj.getClass()) {
    //     return false;
    // }
    // ScanFilter other = (ScanFilter) obj;
    // return Objects.equals(mDeviceName, other.mDeviceName) &&
    //         Objects.equals(mDeviceAddress, other.mDeviceAddress) &&
    //                 mManufacturerId == other.mManufacturerId &&
    //         Objects.deepEquals(mManufacturerData, other.mManufacturerData) &&
    //         Objects.deepEquals(mManufacturerDataMask, other.mManufacturerDataMask) &&
    //         Objects.deepEquals(mServiceDataUuid, other.mServiceDataUuid) &&
    //         Objects.deepEquals(mServiceData, other.mServiceData) &&
    //         Objects.deepEquals(mServiceDataMask, other.mServiceDataMask) &&
    //         Objects.equals(mServiceUuid, other.mServiceUuid) &&
    //         Objects.equals(mServiceUuidMask, other.mServiceUuidMask);
    assert(0);
    return NOERROR;
}

ScanFilter::ScanFilter(
    /* [in] */ const String& name,
    /* [in] */ const String& deviceAddress,
    /* [in] */ IParcelUuid* uuid,
    /* [in] */ IParcelUuid* uuidMask,
    /* [in] */ IParcelUuid* serviceDataUuid,
    /* [in] */ ArrayOf<Byte>* serviceData,
    /* [in] */ ArrayOf<Byte>* serviceDataMask,
    /* [in] */ Int32 manufacturerId,
    /* [in] */ ArrayOf<Byte>* manufacturerData,
    /* [in] */ ArrayOf<Byte>* manufacturerDataMask)
{
    // ==================before translated======================
    // mDeviceName = name;
    // mServiceUuid = uuid;
    // mServiceUuidMask = uuidMask;
    // mDeviceAddress = deviceAddress;
    // mServiceDataUuid = serviceDataUuid;
    // mServiceData = serviceData;
    // mServiceDataMask = serviceDataMask;
    // mManufacturerId = manufacturerId;
    // mManufacturerData = manufacturerData;
    // mManufacturerDataMask = manufacturerDataMask;
}

Boolean ScanFilter::MatchesServiceUuids(
    /* [in] */ IParcelUuid* uuid,
    /* [in] */ IParcelUuid* parcelUuidMask,
    /* [in] */ IList* uuids)
{
    // ==================before translated======================
    // if (uuid == null) {
    //     return true;
    // }
    // if (uuids == null) {
    //     return false;
    // }
    //
    // for (ParcelUuid parcelUuid : uuids) {
    //     UUID uuidMask = parcelUuidMask == null ? null : parcelUuidMask.getUuid();
    //     if (matchesServiceUuid(uuid.getUuid(), uuidMask, parcelUuid.getUuid())) {
    //         return true;
    //     }
    // }
    // return false;
    assert(0);
    return FALSE;
}

Boolean ScanFilter::MatchesServiceUuid(
    /* [in] */ IUUID* uuid,
    /* [in] */ IUUID* mask,
    /* [in] */ IUUID* data)
{
    // ==================before translated======================
    // if (mask == null) {
    //     return uuid.equals(data);
    // }
    // if ((uuid.getLeastSignificantBits() & mask.getLeastSignificantBits()) !=
    //         (data.getLeastSignificantBits() & mask.getLeastSignificantBits())) {
    //     return false;
    // }
    // return ((uuid.getMostSignificantBits() & mask.getMostSignificantBits()) ==
    //         (data.getMostSignificantBits() & mask.getMostSignificantBits()));
    assert(0);
    return FALSE;
}

Boolean ScanFilter::MatchesPartialData(
    /* [in] */ ArrayOf<Byte>* data,
    /* [in] */ ArrayOf<Byte>* dataMask,
    /* [in] */ ArrayOf<Byte>* parsedData)
{
    // ==================before translated======================
    // if (parsedData == null || parsedData.length < data.length) {
    //     return false;
    // }
    // if (dataMask == null) {
    //     for (int i = 0; i < data.length; ++i) {
    //         if (parsedData[i] != data[i]) {
    //             return false;
    //         }
    //     }
    //     return true;
    // }
    // for (int i = 0; i < data.length; ++i) {
    //     if ((dataMask[i] & parsedData[i]) != (dataMask[i] & data[i])) {
    //         return false;
    //     }
    // }
    // return true;
    assert(0);
    return FALSE;
}

} // namespace LE
} // namespace Bluetooth
} // namespace Droid
} // namespace Elastos
