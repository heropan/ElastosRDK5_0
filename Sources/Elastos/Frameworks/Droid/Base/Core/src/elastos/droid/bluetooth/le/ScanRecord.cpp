
#include "elastos/droid/bluetooth/le/ScanRecord.h"

namespace Elastos {
namespace Droid {
namespace Bluetooth {
namespace LE {

//=====================================================================
//                              ScanRecord
//=====================================================================
CAR_INTERFACE_IMPL(ScanRecord, Object, IScanRecord);

const String ScanRecord::TAG("ScanRecord");
const Int32 ScanRecord::DATA_TYPE_FLAGS;
const Int32 ScanRecord::DATA_TYPE_SERVICE_UUIDS_16_BIT_PARTIAL;
const Int32 ScanRecord::DATA_TYPE_SERVICE_UUIDS_16_BIT_COMPLETE;
const Int32 ScanRecord::DATA_TYPE_SERVICE_UUIDS_32_BIT_PARTIAL;
const Int32 ScanRecord::DATA_TYPE_SERVICE_UUIDS_32_BIT_COMPLETE;
const Int32 ScanRecord::DATA_TYPE_SERVICE_UUIDS_128_BIT_PARTIAL;
const Int32 ScanRecord::DATA_TYPE_SERVICE_UUIDS_128_BIT_COMPLETE;
const Int32 ScanRecord::DATA_TYPE_LOCAL_NAME_SHORT;
const Int32 ScanRecord::DATA_TYPE_LOCAL_NAME_COMPLETE;
const Int32 ScanRecord::DATA_TYPE_TX_POWER_LEVEL;
const Int32 ScanRecord::DATA_TYPE_SERVICE_DATA;
const Int32 ScanRecord::DATA_TYPE_MANUFACTURER_SPECIFIC_DATA;

ECode ScanRecord::GetAdvertiseFlags(
    /* [out] */ Int32* result)
{
    VALIDATE_NOT_NULL(result);
    // ==================before translated======================
    // return mAdvertiseFlags;
    assert(0);
    return NOERROR;
}

ECode ScanRecord::GetServiceUuids(
    /* [out] */ IList** result)
{
    VALIDATE_NOT_NULL(result);
    // ==================before translated======================
    // return mServiceUuids;
    assert(0);
    return NOERROR;
}

ECode ScanRecord::GetManufacturerSpecificData(
    /* [out] */ ISparseArray** result)
{
    VALIDATE_NOT_NULL(result);
    // ==================before translated======================
    // return mManufacturerSpecificData;
    assert(0);
    return NOERROR;
}

ECode ScanRecord::GetManufacturerSpecificData(
    /* [in] */ Int32 manufacturerId,
    /* [out, callee] */ ArrayOf<Byte>** data)
{
    VALIDATE_NOT_NULL(data);
    // ==================before translated======================
    // return mManufacturerSpecificData.get(manufacturerId);
    assert(0);
    return NOERROR;
}

ECode ScanRecord::GetServiceData(
    /* [out] */ IMap** result)
{
    VALIDATE_NOT_NULL(result);
    // ==================before translated======================
    // return mServiceData;
    assert(0);
    return NOERROR;
}

ECode ScanRecord::GetServiceData(
    /* [in] */ IParcelUuid* serviceDataUuid,
    /* [out, callee] */ ArrayOf<Byte>** data)
{
    VALIDATE_NOT_NULL(data);
    // ==================before translated======================
    // if (serviceDataUuid == null) {
    //     return null;
    // }
    // return mServiceData.get(serviceDataUuid);
    assert(0);
    return NOERROR;
}

ECode ScanRecord::GetTxPowerLevel(
    /* [out] */ Int32* result)
{
    VALIDATE_NOT_NULL(result);
    // ==================before translated======================
    // return mTxPowerLevel;
    assert(0);
    return NOERROR;
}

ECode ScanRecord::GetDeviceName(
    /* [out] */ String* result)
{
    VALIDATE_NOT_NULL(result);
    // ==================before translated======================
    // return mDeviceName;
    assert(0);
    return NOERROR;
}

ECode ScanRecord::GetBytes(
    /* [out] */ ArrayOf<Byte>** result)
{
    VALIDATE_NOT_NULL(result);
    // ==================before translated======================
    // return mBytes;
    assert(0);
    return NOERROR;
}

AutoPtr<IScanRecord> ScanRecord::ParseFromBytes(
    /* [in] */ ArrayOf<Byte>* scanRecord)
{
    // ==================before translated======================
    // if (scanRecord == null) {
    //     return null;
    // }
    //
    // int currentPos = 0;
    // int advertiseFlag = -1;
    // List<ParcelUuid> serviceUuids = new ArrayList<ParcelUuid>();
    // String localName = null;
    // int txPowerLevel = Integer.MIN_VALUE;
    //
    // SparseArray<byte[]> manufacturerData = new SparseArray<byte[]>();
    // Map<ParcelUuid, byte[]> serviceData = new ArrayMap<ParcelUuid, byte[]>();
    //
    // try {
    //     while (currentPos < scanRecord.length) {
    //         // length is unsigned int.
    //         int length = scanRecord[currentPos++] & 0xFF;
    //         if (length == 0) {
    //             break;
    //         }
    //         // Note the length includes the length of the field type itself.
    //         int dataLength = length - 1;
    //         // fieldType is unsigned int.
    //         int fieldType = scanRecord[currentPos++] & 0xFF;
    //         switch (fieldType) {
    //             case DATA_TYPE_FLAGS:
    //                 advertiseFlag = scanRecord[currentPos] & 0xFF;
    //                 break;
    //             case DATA_TYPE_SERVICE_UUIDS_16_BIT_PARTIAL:
    //             case DATA_TYPE_SERVICE_UUIDS_16_BIT_COMPLETE:
    //                 parseServiceUuid(scanRecord, currentPos,
    //                         dataLength, BluetoothUuid.UUID_BYTES_16_BIT, serviceUuids);
    //                 break;
    //             case DATA_TYPE_SERVICE_UUIDS_32_BIT_PARTIAL:
    //             case DATA_TYPE_SERVICE_UUIDS_32_BIT_COMPLETE:
    //                 parseServiceUuid(scanRecord, currentPos, dataLength,
    //                         BluetoothUuid.UUID_BYTES_32_BIT, serviceUuids);
    //                 break;
    //             case DATA_TYPE_SERVICE_UUIDS_128_BIT_PARTIAL:
    //             case DATA_TYPE_SERVICE_UUIDS_128_BIT_COMPLETE:
    //                 parseServiceUuid(scanRecord, currentPos, dataLength,
    //                         BluetoothUuid.UUID_BYTES_128_BIT, serviceUuids);
    //                 break;
    //             case DATA_TYPE_LOCAL_NAME_SHORT:
    //             case DATA_TYPE_LOCAL_NAME_COMPLETE:
    //                 localName = new String(
    //                         extractBytes(scanRecord, currentPos, dataLength));
    //                 break;
    //             case DATA_TYPE_TX_POWER_LEVEL:
    //                 txPowerLevel = scanRecord[currentPos];
    //                 break;
    //             case DATA_TYPE_SERVICE_DATA:
    //                 // The first two bytes of the service data are service data UUID in little
    //                 // endian. The rest bytes are service data.
    //                 int serviceUuidLength = BluetoothUuid.UUID_BYTES_16_BIT;
    //                 byte[] serviceDataUuidBytes = extractBytes(scanRecord, currentPos,
    //                         serviceUuidLength);
    //                 ParcelUuid serviceDataUuid = BluetoothUuid.parseUuidFrom(
    //                         serviceDataUuidBytes);
    //                 byte[] serviceDataArray = extractBytes(scanRecord,
    //                         currentPos + serviceUuidLength, dataLength - serviceUuidLength);
    //                 serviceData.put(serviceDataUuid, serviceDataArray);
    //                 break;
    //             case DATA_TYPE_MANUFACTURER_SPECIFIC_DATA:
    //                 // The first two bytes of the manufacturer specific data are
    //                 // manufacturer ids in little endian.
    //                 int manufacturerId = ((scanRecord[currentPos + 1] & 0xFF) << 8) +
    //                         (scanRecord[currentPos] & 0xFF);
    //                 byte[] manufacturerDataBytes = extractBytes(scanRecord, currentPos + 2,
    //                         dataLength - 2);
    //                 manufacturerData.put(manufacturerId, manufacturerDataBytes);
    //                 break;
    //             default:
    //                 // Just ignore, we don't handle such data type.
    //                 break;
    //         }
    //         currentPos += dataLength;
    //     }
    //
    //     if (serviceUuids.isEmpty()) {
    //         serviceUuids = null;
    //     }
    //     return new ScanRecord(serviceUuids, manufacturerData, serviceData,
    //             advertiseFlag, txPowerLevel, localName, scanRecord);
    // } catch (Exception e) {
    //     Log.e(TAG, "unable to parse scan record: " + Arrays.toString(scanRecord));
    //     // As the record is invalid, ignore all the parsed results for this packet
    //     // and return an empty record with raw scanRecord bytes in results
    //     return new ScanRecord(null, null, null, -1, Integer.MIN_VALUE, null, scanRecord);
    // }
    assert(0);
    AutoPtr<IScanRecord> empty;
    return empty;
}

ECode ScanRecord::ToString(
    /* [out] */ String* info)
{
    // ==================before translated======================
    // return "ScanRecord [mAdvertiseFlags=" + mAdvertiseFlags + ", mServiceUuids=" + mServiceUuids
    //         + ", mManufacturerSpecificData=" + BluetoothLeUtils.toString(mManufacturerSpecificData)
    //         + ", mServiceData=" + BluetoothLeUtils.toString(mServiceData)
    //         + ", mTxPowerLevel=" + mTxPowerLevel + ", mDeviceName=" + mDeviceName + "]";
    assert(0);
    return NOERROR;
}

ScanRecord::ScanRecord(
    /* [in] */ IList* serviceUuids,
    /* [in] */ ISparseArray* manufacturerData,
    /* [in] */ IMap* serviceData,
    /* [in] */ Int32 advertiseFlags,
    /* [in] */ Int32 txPowerLevel,
    /* [in] */ const String& localName,
    /* [in] */ ArrayOf<Byte>* bytes)
{
    // ==================before translated======================
    // mServiceUuids = serviceUuids;
    // mManufacturerSpecificData = manufacturerData;
    // mServiceData = serviceData;
    // mDeviceName = localName;
    // mAdvertiseFlags = advertiseFlags;
    // mTxPowerLevel = txPowerLevel;
    // mBytes = bytes;
}

Int32 ScanRecord::ParseServiceUuid(
    /* [in] */ ArrayOf<Byte>* scanRecord,
    /* [in] */ Int32 currentPos,
    /* [in] */ Int32 dataLength,
    /* [in] */ Int32 uuidLength,
    /* [in] */ IList* serviceUuids)
{
    // ==================before translated======================
    // while (dataLength > 0) {
    //     byte[] uuidBytes = extractBytes(scanRecord, currentPos,
    //             uuidLength);
    //     serviceUuids.add(BluetoothUuid.parseUuidFrom(uuidBytes));
    //     dataLength -= uuidLength;
    //     currentPos += uuidLength;
    // }
    // return currentPos;
    assert(0);
    return 0;
}

AutoPtr<ArrayOf<Byte> > ScanRecord::ExtractBytes(
    /* [in] */ ArrayOf<Byte>* scanRecord,
    /* [in] */ Int32 start,
    /* [in] */ Int32 length)
{
    // ==================before translated======================
    // byte[] bytes = new byte[length];
    // System.arraycopy(scanRecord, start, bytes, 0, length);
    // return bytes;
    assert(0);
    AutoPtr< ArrayOf<Byte> > empty;
    return empty;
}

} // namespace LE
} // namespace Bluetooth
} // namespace Droid
} // namespace Elastos


