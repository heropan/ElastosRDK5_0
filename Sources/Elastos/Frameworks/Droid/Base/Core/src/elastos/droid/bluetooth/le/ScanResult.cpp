
#include "elastos/droid/bluetooth/le/ScanResult.h"

namespace Elastos {
namespace Droid {
namespace Bluetooth {
namespace LE {

//=====================================================================
//                              ScanResult
//=====================================================================
CAR_INTERFACE_IMPL_2(ScanResult, Object, IScanResult, IParcelable);

ScanResult::ScanResult()
{
}

ScanResult::ScanResult(
    /* [in] */ IBluetoothDevice* device,
    /* [in] */ IScanRecord* scanRecord,
    /* [in] */ Int32 rssi,
    /* [in] */ Int64 timestampNanos)
{
    // ==================before translated======================
    // mDevice = device;
    // mScanRecord = scanRecord;
    // mRssi = rssi;
    // mTimestampNanos = timestampNanos;
}

ECode ScanResult::WriteToParcel(
    /* [in] */ IParcel* dest,
    /* [in] */ Int32 flags)
{
    VALIDATE_NOT_NULL(dest);
    // ==================before translated======================
    // if (mDevice != null) {
    //     dest.writeInt(1);
    //     mDevice.writeToParcel(dest, flags);
    // } else {
    //     dest.writeInt(0);
    // }
    // if (mScanRecord != null) {
    //     dest.writeInt(1);
    //     dest.writeByteArray(mScanRecord.getBytes());
    // } else {
    //     dest.writeInt(0);
    // }
    // dest.writeInt(mRssi);
    // dest.writeLong(mTimestampNanos);
    assert(0);
    return NOERROR;
}

ECode ScanResult::GetDevice(
    /* [out] */ IBluetoothDevice** result)
{
    VALIDATE_NOT_NULL(result);
    // ==================before translated======================
    // return mDevice;
    assert(0);
    return NOERROR;
}

ECode ScanResult::GetScanRecord(
    /* [out] */ IScanRecord** record)
{
    VALIDATE_NOT_NULL(record);
    // ==================before translated======================
    // return mScanRecord;
    assert(0);
    return NOERROR;
}

ECode ScanResult::GetRssi(
    /* [out] */ Int32* result)
{
    VALIDATE_NOT_NULL(result);
    // ==================before translated======================
    // return mRssi;
    assert(0);
    return NOERROR;
}

ECode ScanResult::GetTimestampNanos(
    /* [out] */ Int64* result)
{
    VALIDATE_NOT_NULL(result);
    // ==================before translated======================
    // return mTimestampNanos;
    assert(0);
    return NOERROR;
}

ECode ScanResult::GetHashCode(
    /* [out] */ Int32* hashCode)
{
    VALIDATE_NOT_NULL(hashCode);
    // ==================before translated======================
    // return Objects.hash(mDevice, mRssi, mScanRecord, mTimestampNanos);
    assert(0);
    return NOERROR;
}

ECode ScanResult::Equals(
    /* [in] */ IInterface* other,
    /* [out] */ Boolean* result)
{
    // ==================before translated======================
    // if (this == obj) {
    //     return true;
    // }
    // if (obj == null || getClass() != obj.getClass()) {
    //     return false;
    // }
    // ScanResult other = (ScanResult) obj;
    // return Objects.equals(mDevice, other.mDevice) && (mRssi == other.mRssi) &&
    //         Objects.equals(mScanRecord, other.mScanRecord)
    //         && (mTimestampNanos == other.mTimestampNanos);
    assert(0);
    return NOERROR;
}

ECode ScanResult::ToString(
    /* [out] */ String* info)
{
    // ==================before translated======================
    // return "ScanResult{" + "mDevice=" + mDevice + ", mScanRecord="
    //         + Objects.toString(mScanRecord) + ", mRssi=" + mRssi + ", mTimestampNanos="
    //         + mTimestampNanos + '}';
    assert(0);
    return NOERROR;
}

ScanResult::ScanResult(
    /* [in] */ IParcel* in)
{
    // ==================before translated======================
    // readFromParcel(in);
}

ECode ScanResult::ReadFromParcel(
    /* [in] */ IParcel* in)
{
    // ==================before translated======================
    // if (in.readInt() == 1) {
    //     mDevice = BluetoothDevice.CREATOR.createFromParcel(in);
    // }
    // if (in.readInt() == 1) {
    //     mScanRecord = ScanRecord.parseFromBytes(in.createByteArray());
    // }
    // mRssi = in.readInt();
    // mTimestampNanos = in.readLong();
    assert(0);
    return NOERROR;
}

} // namespace LE
} // namespace Bluetooth
} // namespace Droid
} // namespace Elastos


