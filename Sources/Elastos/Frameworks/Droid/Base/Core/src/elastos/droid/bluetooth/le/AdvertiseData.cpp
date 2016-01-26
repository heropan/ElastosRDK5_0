#include "elastos/droid/bluetooth/le/AdvertiseData.h"

namespace Elastos {
namespace Droid {
namespace Bluetooth {
namespace LE {

//=====================================================================
//                        AdvertiseData::Builder
//=====================================================================
CAR_INTERFACE_IMPL(AdvertiseData::Builder, Object, IAdvertiseDataBuilder);

ECode AdvertiseData::Builder::AddServiceUuid(
    /* [in] */ IParcelUuid* serviceUuid)
{
    // ==================before translated======================
    // if (serviceUuid == null) {
    //     throw new IllegalArgumentException("serivceUuids are null");
    // }
    // mServiceUuids.add(serviceUuid);
    // return this;
    assert(0);
    return NOERROR;
}

ECode AdvertiseData::Builder::AddServiceData(
    /* [in] */ IParcelUuid* serviceDataUuid,
    /* [in] */ ArrayOf<Byte>* serviceData)
{
    // ==================before translated======================
    // if (serviceDataUuid == null || serviceData == null) {
    //     throw new IllegalArgumentException(
    //             "serviceDataUuid or serviceDataUuid is null");
    // }
    // mServiceData.put(serviceDataUuid, serviceData);
    // return this;
    assert(0);
    return NOERROR;
}

ECode AdvertiseData::Builder::AddManufacturerData(
    /* [in] */ Int32 manufacturerId,
    /* [in] */ ArrayOf<Byte>* manufacturerSpecificData)
{
    // ==================before translated======================
    // if (manufacturerId < 0) {
    //     throw new IllegalArgumentException(
    //             "invalid manufacturerId - " + manufacturerId);
    // }
    // if (manufacturerSpecificData == null) {
    //     throw new IllegalArgumentException("manufacturerSpecificData is null");
    // }
    // mManufacturerSpecificData.put(manufacturerId, manufacturerSpecificData);
    // return this;
    assert(0);
    return NOERROR;
}

ECode AdvertiseData::Builder::SetIncludeTxPowerLevel(
    /* [in] */ Boolean includeTxPowerLevel)
{
    // ==================before translated======================
    // mIncludeTxPowerLevel = includeTxPowerLevel;
    // return this;
    assert(0);
    return NOERROR;
}

ECode AdvertiseData::Builder::SetIncludeDeviceName(
    /* [in] */ Boolean includeDeviceName)
{
    // ==================before translated======================
    // mIncludeDeviceName = includeDeviceName;
    // return this;
    assert(0);
    return NOERROR;
}

ECode AdvertiseData::Builder::Build(
    /* [out] */ IAdvertiseData** result)
{
    VALIDATE_NOT_NULL(result);
    // ==================before translated======================
    // return new AdvertiseData(mServiceUuids, mManufacturerSpecificData, mServiceData,
    //         mIncludeTxPowerLevel, mIncludeDeviceName);
    assert(0);
    return NOERROR;
}

AutoPtr<ISparseArray> AdvertiseData::Builder::MiddleInitMmanufacturerspecificdata()
{
    // ==================before translated======================
    // SparseArray<byte[]> result = new SparseArray<byte[]>();
    assert(0);
    AutoPtr<ISparseArray> empty;
    return empty;
}
//=====================================================================
//                            AdvertiseData
//=====================================================================
CAR_INTERFACE_IMPL_2(AdvertiseData, Object, IAdvertiseData, IParcelable);

ECode AdvertiseData::GetServiceUuids(
    /* [out] */ IList** result)
{
    VALIDATE_NOT_NULL(result);
    // ==================before translated======================
    // return mServiceUuids;
    assert(0);
    return NOERROR;
}

ECode AdvertiseData::GetManufacturerSpecificData(
    /* [out] */ ISparseArray** result)
{
    VALIDATE_NOT_NULL(result);
    // ==================before translated======================
    // return mManufacturerSpecificData;
    assert(0);
    return NOERROR;
}

ECode AdvertiseData::GetServiceData(
    /* [out] */ IMap** result)
{
    VALIDATE_NOT_NULL(result);
    // ==================before translated======================
    // return mServiceData;
    assert(0);
    return NOERROR;
}

ECode AdvertiseData::GetIncludeTxPowerLevel(
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
    // ==================before translated======================
    // return mIncludeTxPowerLevel;
    assert(0);
    return NOERROR;
}

ECode AdvertiseData::GetIncludeDeviceName(
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
    // ==================before translated======================
    // return mIncludeDeviceName;
    assert(0);
    return NOERROR;
}

ECode AdvertiseData::GetHashCode(
    /* [out] */ Int32* hashCode)
{
    VALIDATE_NOT_NULL(hashCode);
    // ==================before translated======================
    // return Objects.hash(mServiceUuids, mManufacturerSpecificData, mServiceData,
    //         mIncludeDeviceName, mIncludeTxPowerLevel);
    assert(0);
    return NOERROR;
}

ECode AdvertiseData::Equals(
    /* [in] */ IInterface* other,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
    // ==================before translated======================
    // if (this == obj) {
    //     return true;
    // }
    // if (obj == null || getClass() != obj.getClass()) {
    //     return false;
    // }
    // AdvertiseData other = (AdvertiseData) obj;
    // return Objects.equals(mServiceUuids, other.mServiceUuids) &&
    //         BluetoothLeUtils.equals(mManufacturerSpecificData, other.mManufacturerSpecificData) &&
    //         BluetoothLeUtils.equals(mServiceData, other.mServiceData) &&
    //                 mIncludeDeviceName == other.mIncludeDeviceName &&
    //                 mIncludeTxPowerLevel == other.mIncludeTxPowerLevel;
    assert(0);
    *result = FALSE;
    return NOERROR;
}

ECode AdvertiseData::ToString(
    /* [out] */ String* info)
{
    VALIDATE_NOT_NULL(info);
    // ==================before translated======================
    // return "AdvertiseData [mServiceUuids=" + mServiceUuids + ", mManufacturerSpecificData="
    //         + BluetoothLeUtils.toString(mManufacturerSpecificData) + ", mServiceData="
    //         + BluetoothLeUtils.toString(mServiceData)
    //         + ", mIncludeTxPowerLevel=" + mIncludeTxPowerLevel + ", mIncludeDeviceName="
    //         + mIncludeDeviceName + "]";
    assert(0);
    *info = String("");
    return NOERROR;
}

ECode AdvertiseData::WriteToParcel(
    /* [in] */ IParcel* dest)
{
    VALIDATE_NOT_NULL(dest);
    // ==================before translated======================
    // dest.writeList(mServiceUuids);
    //
    // // mManufacturerSpecificData could not be null.
    // dest.writeInt(mManufacturerSpecificData.size());
    // for (int i = 0; i < mManufacturerSpecificData.size(); ++i) {
    //     dest.writeInt(mManufacturerSpecificData.keyAt(i));
    //     byte[] data = mManufacturerSpecificData.valueAt(i);
    //     if (data == null) {
    //         dest.writeInt(0);
    //     } else {
    //         dest.writeInt(1);
    //         dest.writeInt(data.length);
    //         dest.writeByteArray(data);
    //     }
    // }
    // dest.writeInt(mServiceData.size());
    // for (ParcelUuid uuid : mServiceData.keySet()) {
    //     dest.writeParcelable(uuid, flags);
    //     byte[] data = mServiceData.get(uuid);
    //     if (data == null) {
    //         dest.writeInt(0);
    //     } else {
    //         dest.writeInt(1);
    //         dest.writeInt(data.length);
    //         dest.writeByteArray(data);
    //     }
    // }
    // dest.writeByte((byte) (getIncludeTxPowerLevel() ? 1 : 0));
    // dest.writeByte((byte) (getIncludeDeviceName() ? 1 : 0));
    assert(0);
    return NOERROR;
}

ECode AdvertiseData::ReadFromParcel(
    /* [in] */ IParcel* source)
{
    return NOERROR;
}

AdvertiseData::AdvertiseData(
    /* [in] */ IList* serviceUuids,
    /* [in] */ ISparseArray* manufacturerData,
    /* [in] */ IMap* serviceData,
    /* [in] */ Boolean includeTxPowerLevel,
    /* [in] */ Boolean includeDeviceName)
{
    // ==================before translated======================
    // mServiceUuids = serviceUuids;
    // mManufacturerSpecificData = manufacturerData;
    // mServiceData = serviceData;
    // mIncludeTxPowerLevel = includeTxPowerLevel;
    // mIncludeDeviceName = includeDeviceName;
}

} // namespace LE
} // namespace Bluetooth
} // namespace Droid
} // namespace Elastos


