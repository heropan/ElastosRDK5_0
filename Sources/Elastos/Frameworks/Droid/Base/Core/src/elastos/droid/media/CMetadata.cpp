
#include "media/CMetadata.h"
#include <elastos/utility/logging/Logger.h>
#include <elastos/core/StringUtils.h>

using Elastos::Core::StringUtils;
using Elastos::Utility::Logging::Logger;

using Elastos::Core::CInteger32;
using Elastos::Utility::CDate;
using Elastos::Utility::ITimeZone;
using Elastos::Utility::ITimeZoneHelper;
using Elastos::Utility::CTimeZoneHelper;
using Elastos::Utility::ICalendar;
using Elastos::Utility::ICalendarHelper;
using Elastos::Utility::CCalendarHelper;

namespace Elastos {
namespace Droid {
namespace Media {

const String CMetadata::TAG("media.Metadata");

const Int32 CMetadata::LAST_SYSTEM = 31;
const Int32 CMetadata::FIRST_CUSTOM = 8192;

// FIXME: misses a type for shared heap is missing (MemoryFile).
// FIXME: misses a type for bitmaps.
const Int32 CMetadata::LAST_TYPE = 7;

const Int32 CMetadata::kInt32Size = 4;
const Int32 CMetadata::kMetaHeaderSize = 2 * kInt32Size; //  size + marker
const Int32 CMetadata::kRecordHeaderSize = 3 * kInt32Size; // size + id + type
const Int32 CMetadata::kMetaMarker = 0x4d455441;  // 'M' 'E' 'T' 'A'

static AutoPtr<IObjectContainer> InitMATCH_NONE()
{
    AutoPtr<IObjectContainer> none;
    CObjectContainer::New((IObjectContainer**)&none);
    return none;
}

static AutoPtr<IObjectContainer> InitMATCH_ALL()
{
    AutoPtr<IObjectContainer> all;
    CObjectContainer::New((IObjectContainer**)&all);
    AutoPtr<IInteger32> zero;
    CInteger32::New(IMetadata::ANY, (IInteger32**)&zero);
    return all;
}

AutoPtr<IObjectContainer> CMetadata::MATCH_NONE = InitMATCH_NONE();
AutoPtr<IObjectContainer> CMetadata::MATCH_ALL = InitMATCH_ALL();

CMetadata::CMetadata()
{
}

ECode CMetadata::constructor()
{
    return NOERROR;
}

ECode CMetadata::Parse(
    /* [in] */ IParcel* parcel,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
    *result = FALSE;

    Int32 avail;
    parcel->GetElementSize(&avail);
    if (avail < kMetaHeaderSize) {
        Logger::E(TAG, String("Not enough data ") + StringUtils::Int32ToString(avail));
        return NOERROR;
    }

    Int32 pin;
    parcel->GetDataPosition(&pin);  // to roll back in case of errors.
    Int32 size;
    parcel->ReadInt32(&size);

    // The extra kInt32Size below is to account for the int32 'size' just read.
    if (avail + kInt32Size < size || size < kMetaHeaderSize) {
        Logger::E(TAG, "Bad size %d, avail %d, position %d, ", size, avail, pin);
        parcel->SetDataPosition(pin);
        *result = FALSE;
        return NOERROR;
    }

    // Checks if the 'M' 'E' 'T' 'A' marker is present.
    Int32 kShouldBeMetaMarker;
    parcel->ReadInt32(&kShouldBeMetaMarker);
    if (kShouldBeMetaMarker != kMetaMarker ) {
        Logger::E(TAG, "Marker missing %08x", kShouldBeMetaMarker);
        parcel->SetDataPosition(pin);
        *result = FALSE;
        return NOERROR;
    }

    // Scan the records to collect metadata ids and offsets.
    if (!ScanAllRecords(parcel, size - kMetaHeaderSize)) {
        parcel->SetDataPosition(pin);
        *result = FALSE;
        return NOERROR;
    }
    mParcel = parcel;
    *result = TRUE;
    return NOERROR;
}

ECode CMetadata::KeySet(
    /* [out] */ IObjectContainer** result)
{
    VALIDATE_NOT_NULL(result);

    HashMap< AutoPtr<IInteger32>, AutoPtr<IInteger32> >::Iterator it = mKeyToPosMap.Begin();
    AutoPtr<IObjectContainer> oc;
    CObjectContainer::New((IObjectContainer**)&oc);
    for(; it != mKeyToPosMap.End(); ++it) {
        oc->Add(it->mFirst);
    }
    *result = oc;
    REFCOUNT_ADD(*result);
    return NOERROR;
}

ECode CMetadata::Has(
    /* [in] */ Int32 metadataId,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
    *result = FALSE;

    if (!CheckMetadataId(metadataId)) {
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }

    AutoPtr<IInteger32> integer32;
    CInteger32::New(metadataId, (IInteger32**)&integer32);
    HashMap< AutoPtr<IInteger32>, AutoPtr<IInteger32> >::Iterator it = mKeyToPosMap.Find(integer32);
    if (it != mKeyToPosMap.End()) {
        *result = TRUE;
    }
    return NOERROR;
}

ECode CMetadata::GetString(
    /* [in] */ Int32 key,
    /* [out] */ String* result)
{
    VALIDATE_NOT_NULL(result);

    FAIL_RETURN(CheckType(key, STRING_VAL));
    mParcel->ReadString(result);
    return NOERROR;
}

ECode CMetadata::GetInt32(
    /* [in] */ Int32 key,
    /* [out] */ Int32* result)
{
    VALIDATE_NOT_NULL(result);

    FAIL_RETURN(CheckType(key, INTEGER_VAL));
    mParcel->ReadInt32(result);
    return NOERROR;
}

ECode CMetadata::GetBoolean(
    /* [in] */ Int32 key,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
    *result = FALSE;

    FAIL_RETURN(CheckType(key, BOOLEAN_VAL));
    Int32 tempValue;
    mParcel->ReadInt32(&tempValue);
    if (tempValue == 1) {
        *result = TRUE;
    }
    return NOERROR;
}

ECode CMetadata::GetInt64(
    /* [in] */ Int32 key,
    /* [out] */ Int64* result)
{
    VALIDATE_NOT_NULL(result);

    FAIL_RETURN(CheckType(key, LONG_VAL));
    mParcel->ReadInt64(result);
    return NOERROR;
}

ECode CMetadata::GetDouble(
    /* [in] */ Int32 key,
    /* [out] */ Double* result)
{
    VALIDATE_NOT_NULL(result);

    FAIL_RETURN(CheckType(key, DOUBLE_VAL));
    mParcel->ReadDouble(result);
    return NOERROR;
}

ECode CMetadata::GetByteArray(
    /* [in] */ Int32 key,
    /* [out, callee] */ ArrayOf<Byte>** result)
{
    VALIDATE_NOT_NULL(result);

    FAIL_RETURN(CheckType(key, BYTE_ARRAY_VAL));

    AutoPtr<ArrayOf<Byte> > array;
    mParcel->ReadArrayOf((Handle32*)(&array));
    *result = array;
    REFCOUNT_ADD(*result);
    return NOERROR;
}

ECode CMetadata::GetDate(
    /* [in] */ Int32 key,
    /* [out] */ IDate** result)
{
    VALIDATE_NOT_NULL(result);

    FAIL_RETURN(CheckType(key, DATE_VAL));
    Int64 timeSinceEpoch;
    mParcel->ReadInt64(&timeSinceEpoch);
    String timeZone;
    mParcel->ReadString(&timeZone);

    if (timeZone.IsNullOrEmpty()) {
        AutoPtr<IDate> date;
        CDate::New(timeSinceEpoch, (IDate**)&date);
        *result = date;
    }
    else {
        AutoPtr<ITimeZoneHelper> timeZoneHelper;
        CTimeZoneHelper::AcquireSingleton((ITimeZoneHelper**)&timeZoneHelper);
        AutoPtr<ITimeZone> tz;
        timeZoneHelper->GetTimeZone(timeZone, (ITimeZone**)&tz);

        AutoPtr<ICalendarHelper> calendarHelper;
        CCalendarHelper::AcquireSingleton((ICalendarHelper**)&calendarHelper);
        AutoPtr<ICalendar> cal;
        calendarHelper->GetInstance((ICalendar**)&cal);

        cal->SetTimeInMillis(timeSinceEpoch);
        cal->GetTime(result);
    }
    REFCOUNT_ADD(*result);
    return NOERROR;
}

/*static*/
ECode CMetadata::LastSytemId(
    /* [out] */ Int32* result)
{
    VALIDATE_NOT_NULL(result);

    *result = LAST_SYSTEM;
    return NOERROR;
}

/*static*/
ECode CMetadata::FirstCustomId(
    /* [out] */ Int32* result)
{
    VALIDATE_NOT_NULL(result);

    *result = FIRST_CUSTOM;
    return NOERROR;
}

/*static*/
ECode CMetadata::LastType(
    /* [out] */ Int32* result)
{
    VALIDATE_NOT_NULL(result);

    *result = LAST_TYPE;
    return NOERROR;
}

Boolean CMetadata::ScanAllRecords(
    /* [in] */ IParcel* parcel,
    /* [in] */ Int32 bytesLeft)
{
    Int32 recCount = 0;
    Boolean error = FALSE;

    mKeyToPosMap.Clear();
    while (bytesLeft > kRecordHeaderSize) {
        Int32 start;
        parcel->GetDataPosition(&start);
        // Check the size.
        Int32 size;
        parcel->ReadInt32(&size);

        if (size <= kRecordHeaderSize) {  // at least 1 byte should be present.
            Logger::E(TAG, "Record is too short");
            error = TRUE;
            break;
        }

        // Check the metadata key.
        Int32 metadataId;
        parcel->ReadInt32(&metadataId);
        if (!CheckMetadataId(metadataId)) {
            error = TRUE;
            break;
        }

        // Store the record offset which points to the type
        // field so we can later on read/unmarshall the record
        // payload.
        AutoPtr<IInteger32> integer1;
        CInteger32::New(metadataId, (IInteger32**)&integer1);
        HashMap< AutoPtr<IInteger32>, AutoPtr<IInteger32> >::Iterator it = mKeyToPosMap.Find(integer1);
        if (it != mKeyToPosMap.End()) {
            Logger::E(TAG, "Duplicate metadata ID found");
            error = TRUE;
            break;
        }

        Int32 tempValue;
        parcel->GetDataPosition(&tempValue);
        AutoPtr<IInteger32> integer2;
        CInteger32::New(tempValue, (IInteger32**)&integer2);
        mKeyToPosMap[integer1] = integer2;

        // Check the metadata type.
        Int32 metadataType;
        parcel->ReadInt32(&metadataType);
        if (metadataType <= 0 || metadataType > LAST_TYPE) {
            Logger::E(TAG, "Invalid metadata type %d", metadataType);
            error = TRUE;
            break;
        }

        // Skip to the next one.
        parcel->SetDataPosition(start + size);
        bytesLeft -= size;
        ++recCount;
    }

    if (0 != bytesLeft || error) {
        Logger::E(TAG, "Ran out of data or error on record %d", recCount);
        mKeyToPosMap.Clear();
        return FALSE;
    } else {
        return TRUE;
    }
}

Boolean CMetadata::CheckMetadataId(
    /* [in] */ Int32 val)
{
    if (val <= ANY || (LAST_SYSTEM < val && val < FIRST_CUSTOM)) {
        Logger::E(TAG, "Invalid metadata ID %d", val);
        return FALSE;
    }
    return TRUE;
}

ECode CMetadata::CheckType(
    /* [in] */ Int32 key,
    /* [in] */ Int32 expectedType)
{
    AutoPtr<IInteger32> integer32;
    CInteger32::New(key, (IInteger32**)&integer32);
    HashMap< AutoPtr<IInteger32>, AutoPtr<IInteger32> >::Iterator it = mKeyToPosMap.Find(integer32);
    Int32 pos;
    it->mSecond->GetValue(&pos);

    mParcel->SetDataPosition(pos);

    Int32 type;
    mParcel->ReadInt32(&type);
    if (type != expectedType) {
        Logger::E(TAG, "Wrong type %d but got %d", expectedType, type);
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    return NOERROR;
}

} // namespace Media
} // namepsace Droid
} // namespace Elastos
