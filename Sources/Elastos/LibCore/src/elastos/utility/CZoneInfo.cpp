#include <cmdef.h>
#include "CZoneInfo.h"
#include "CArrays.h"
#include "CFormatter.h"
#include "CInteger32.h"
#include "CBoolean.h"
#include "CStringWrapper.h"
#include "CSimpleDateFormat.h"
#include "CSystem.h"
#include <elastos/Math.h>
#include <assert.h>

namespace Elastos{
namespace Utility{

using Elastos::Core::Math;
using Elastos::Core::ISystem;
using Elastos::Core::IInteger32;
using Elastos::Core::IBoolean;
using Elastos::Core::CInteger32;
using Elastos::Core::CBoolean;
using Elastos::Core::ICharSequence;
using Elastos::Core::CStringWrapper;
using Elastos::Text::ISimpleDateFormat;
using Elastos::Text::CSimpleDateFormat;

const Int64 CZoneInfo::MILLISECONDS_PER_DAY;
const Int64 CZoneInfo::MILLISECONDS_PER_400_YEARS;
const Int64 CZoneInfo::UNIX_OFFSET;

const Int32 CZoneInfo::NORMAL[] = {
        0, 31, 59, 90, 120, 151, 181, 212, 243, 273, 304, 334,
};

const Int32 CZoneInfo::LEAP[] = {
        0, 31, 60, 91, 121, 152, 182, 213, 244, 274, 305, 335,
};

PInterface CZoneInfo::Probe(
        /* [in] */ REIID riid)
{
    return _CZoneInfo::Probe(riid);
}

ECode CZoneInfo::constructor(
    /* [in] */ const String& name,
    /* [in] */ ArrayOf<Int32>* transitions,
    /* [in] */ ArrayOf<Byte>* type,
    /* [in] */ ArrayOf<Int32>* gmtOffsets,
    /* [in] */ ArrayOf<Byte>* isDsts)
{
    mTransitions = transitions;
    mTypes = type;
    mOffsets = gmtOffsets;
    mIsDsts = isDsts;
    SetID(name);

    // Find the latest GMT and non-GMT offsets for their abbreviations
    Int32 lastStd = 0;
    Boolean haveStd = FALSE;
    Int32 lastDst = 0;
    Boolean haveDst = FALSE;
    for (Int32 i = mTransitions->GetLength() - 1;
            (!haveStd || !haveDst) && i >= 0; --i) {
        Int32 type = (*mTypes)[i] & 0xff;
        if (!haveStd && (*mIsDsts)[type] == 0) {
            haveStd = TRUE;
            lastStd = i;
        }
        if (!haveDst && (*mIsDsts)[type] != 0) {
            haveDst = TRUE;
            lastDst = i;
        }
    }

    // Use the latest non-daylight offset (if any) as the raw offset.
    if (lastStd >= mTypes->GetLength()) {
        mRawOffset = (*mOffsets)[0];
    }
    else {
        mRawOffset = (*mOffsets)[(*mTypes)[lastStd] & 0xff];
    }

    // Use the latest transition's pair of offsets to compute the DST savings.
    // This isn't generally useful, but it's exposed by TimeZone.getDSTSavings.
    if (lastDst >= mTypes->GetLength()) {
        mDstSavings = 0;
    }
    else {
        mDstSavings = Math::Abs((*mOffsets)[(*mTypes)[lastStd] & 0xff] - (*mOffsets)[(*mTypes)[lastDst] & 0xff]) * 1000;
    }

    // Cache the oldest known raw offset, in case we're asked about times that predate our
    // transition data.
    Int32 firstStd = -1;
    for (Int32 i = 0; i < mTransitions->GetLength(); ++i) {
        if ((*mIsDsts)[(*mTypes)[i] & 0xff] == 0) {
            firstStd = i;
            break;
        }
    }
    Int32 earliestRawOffset = (firstStd != -1) ?
            (*mOffsets)[(*mTypes)[firstStd] & 0xff] : mRawOffset;

    // Rather than keep offsets from UTC, we use offsets from local time, so the raw offset
    // can be changed and automatically affect all the offsets.
    for (Int32 i = 0; i < mOffsets->GetLength(); i++) {
        (*mOffsets)[i] -= mRawOffset;
    }

    // Is this zone still observing DST?
    // We don't care if they've historically used it: most places have at least once.
    // We want to know whether the last "schedule info" (the unix times in the mTransitions
    // array) is in the future. If it is, DST is still relevant.
    // See http://code.google.com/p/android/issues/detail?id=877.
    // This test means that for somewhere like Morocco, which tried DST in 2009 but has
    // no future plans (and thus no future schedule info) will report "true" from
    // useDaylightTime at the start of 2009 but "false" at the end. This seems appropriate.

    AutoPtr<ISystem> system;
    Elastos::Core::CSystem::AcquireSingleton((ISystem**)&system);

    Boolean usesDst = FALSE;
    Int64 currentUnixTime;
    system->GetCurrentTimeMillis(&currentUnixTime);
    currentUnixTime = currentUnixTime / 1000;
    if (mTransitions->GetLength() > 0) {
        // (We're really dealing with uint32_t values, so long is most convenient in Java.)
        Int64 latestScheduleTime = ((Int64) (*mTransitions)[mTransitions->GetLength() - 1]) & 0xffffffff;
        if (currentUnixTime < latestScheduleTime) {
            usesDst = TRUE;
        }
    }
    mUseDst = usesDst;

    // tzdata uses seconds, but Java uses milliseconds.
    mRawOffset *= 1000;
    mEarliestRawOffset = earliestRawOffset * 1000;
    return NOERROR;
}

ECode CZoneInfo::GetOffset(
    /* [in] */ Int32 era,
    /* [in] */ Int32 year,
    /* [in] */ Int32 month,
    /* [in] */ Int32 day,
    /* [in] */ Int32 dayOfWeek,
    /* [in] */ Int32 millis,
    /* [out] */ Int32* offset)
{
    VALIDATE_NOT_NULL(offset);

    // XXX This assumes Gregorian always; Calendar switches from
    // Julian to Gregorian in 1582.  What calendar system are the
    // arguments supposed to come from?

    Int64 calc = (year / 400) * MILLISECONDS_PER_400_YEARS;
    year %= 400;

    calc += year * (365 * MILLISECONDS_PER_DAY);
    calc += ((year + 3) / 4) * MILLISECONDS_PER_DAY;

    if (year > 0)
        calc -= ((year - 1) / 100) * MILLISECONDS_PER_DAY;

    Boolean isLeap = (year == 0 || (year % 4 == 0 && year % 100 != 0));
    const Int32* mlen = isLeap ? LEAP : NORMAL;

    calc += mlen[month] * MILLISECONDS_PER_DAY;
    calc += (day - 1) * MILLISECONDS_PER_DAY;
    calc += millis;

    calc -= mRawOffset;
    calc -= UNIX_OFFSET;

    return GetOffset(calc, offset);
}

ECode CZoneInfo::GetOffset(
    /* [in] */ Int64 when,
    /* [out] */ Int32* offset)
{
    VALIDATE_NOT_NULL(offset);

    Int32 ix = (Int32)(when / 1000);
    Int32 transition = mTransitions->BinarySearch(ix);
    if (transition < 0) {
        transition = ~transition - 1;
        if (transition < 0) {
            // Assume that all times before our first transition correspond to the
            // oldest-known non-daylight offset. The obvious alternative would be to
            // use the current raw offset, but that seems like a greater leap of faith.
            *offset = mEarliestRawOffset;
            return NOERROR;
        }
    }

    *offset = mRawOffset + (*mOffsets)[(*mTypes)[transition] & 0xff] * 1000;
    return NOERROR;
}

ECode CZoneInfo::InDaylightTime(
    /* [in] */ IDate* time,
    /* [out] */ Boolean* value)
{
    VALIDATE_NOT_NULL(time);
    VALIDATE_NOT_NULL(value);

    Int64 when;
    time->GetTime(&when);

    Int32 ix = (Int32)(when / 1000);
    Int32 transition = mTransitions->BinarySearch(ix);
    if (transition < 0) {
        transition = ~transition - 1;
        if (transition < 0) {
            // Assume that all times before our first transition are non-daylight.
            // Transition data tends to start with a transition to daylight, so just
            // copying the first transition would assume the opposite.
            // http://code.google.com/p/android/issues/detail?id=14395
            *value = FALSE;
            return NOERROR;
        }
    }

    *value = (*mIsDsts)[(*mTypes)[transition] & 0xff] == 1;
    return NOERROR;
}

ECode CZoneInfo::GetRawOffset(
    /* [out] */ Int32* offset)
{
    VALIDATE_NOT_NULL(offset);
     *offset = mRawOffset;
     return NOERROR;
}

ECode CZoneInfo::SetRawOffset(
    /* [in] */ Int32 off)
{
    mRawOffset = off;
    return NOERROR;
}

ECode CZoneInfo::GetDSTSavings(
    /* [out] */ Int32* savings)
{
    VALIDATE_NOT_NULL(savings);
    *savings = mUseDst ? mDstSavings: 0;
    return NOERROR;
}

ECode CZoneInfo::UseDaylightTime(
    /* [out] */ Boolean* value)
{
    VALIDATE_NOT_NULL(value);
    *value = mUseDst;
    return NOERROR;
}

ECode CZoneInfo::HasSameRules(
    /* [in] */ ITimeZone* timeZone,
    /* [out] */ Boolean* value)
{
    VALIDATE_NOT_NULL(timeZone);
    VALIDATE_NOT_NULL(value);

    *value = FALSE;

    if (!IZoneInfo::Probe(timeZone)) {
        return NOERROR;
    }

    CZoneInfo* other = (CZoneInfo*)timeZone;
    if (mUseDst != other->mUseDst) {
        return NOERROR;
    }

    if (!mUseDst) {
        *value = (mRawOffset == other->mRawOffset);
        return NOERROR;
    }

    if (mRawOffset != other->mRawOffset) {
        return NOERROR;
    }

    // Arrays.equals returns true if both arrays are null
    *value = mOffsets->Equals(other->mOffsets)
            && mIsDsts->Equals(other->mIsDsts)
            && mTypes->Equals(other->mTypes)
            && mTransitions->Equals(other->mTransitions);
    return NOERROR;
}

ECode CZoneInfo::Clone(
    /* [out] */ ITimeZone** newObj)
{
    VALIDATE_NOT_NULL(newObj);
    AutoPtr<ArrayOf<Int32> > offsets = mOffsets->Clone();
    for (Int32 i = 0; i < offsets->GetLength(); i++) {
        (*offsets)[i] += mRawOffset / 1000;
    }
    AutoPtr<CZoneInfo> info;
    FAIL_RETURN(CZoneInfo::NewByFriend(mID, mTransitions->Clone(), mTypes->Clone(), offsets, mIsDsts->Clone(),
            (CZoneInfo**)&info));
    *newObj = (ITimeZone*)info.Get();
    INTERFACE_ADDREF(*newObj);
    return NOERROR;
}

ECode CZoneInfo::Equals(
    /* [in] */ IInterface* other,
    /* [out] */ Boolean* isEqual)
{
    VALIDATE_NOT_NULL(isEqual);
    *isEqual = FALSE;
    VALIDATE_NOT_NULL(other);

    IZoneInfo* zoneInfo = (IZoneInfo*)other->Probe(EIID_IZoneInfo);
    if (!zoneInfo)
        return NOERROR;

    String thisID, otherID;
    GetID(&thisID);
    zoneInfo->GetID(&otherID);
    if (thisID != otherID) {
        return NOERROR;
    }

    return HasSameRules(zoneInfo, isEqual);
}

ECode CZoneInfo::GetHashCode(
    /* [out] */ Int32* hashCode)
{
    VALIDATE_NOT_NULL(hashCode);
    const Int32 prime = 31;
    Int32 result = 1;
    String id;
    GetID(&id);
    Int32 code;
    result = prime * result + id.GetHashCode();
    result = prime * result + (CArrays::_HashCodeInt32(mOffsets, &code), code);
    result = prime * result + (CArrays::_HashCodeBoolean(mIsDsts, &code), code);
    result = prime * result + mRawOffset;
    result = prime * result + (CArrays::_HashCodeInt32(mTransitions, &code), code);
    result = prime * result + (CArrays::_HashCodeBoolean(mTypes, &code), code);
    result = prime * result + (mUseDst ? 1231 : 1237);
    *hashCode = result;
    return NOERROR;
}

ECode CZoneInfo::ToString(
    /* [out] */ String* str)
{
    StringBuilder sb;
    // First the basics...
    String id;
    GetID(&id);
    sb += "CZoneInfo[";
    sb += id;
    sb += ",mRawOffset=";
    sb += mRawOffset;
    sb += ",mUseDst=";
    sb += mUseDst;
    sb += ",mDstSavings=";
    sb += mDstSavings;
    sb += "]";

    // ...followed by a zdump(1)-like description of all our transition data.
    sb.AppendChar('\n');
    AutoPtr<IFormatter> f;
    CFormatter::New(&sb, (IFormatter**)&f);
    for (Int32 i = 0; i < mTransitions->GetLength(); ++i) {
        Int32 type = (*mTypes)[i] & 0xff;
        String utcTime = FormatTime((*mTransitions)[i], TimeZone::UTC());
        String localTime = FormatTime((*mTransitions)[i], this);
        Int32 offset = (*mOffsets)[type];
        Int32 gmtOffset = mRawOffset / 1000 + offset;
        AutoPtr<ArrayOf<IInterface*> > arguments = ArrayOf<IInterface*>::Alloc(7);
        AutoPtr<IInteger32> integer;
        CInteger32::New(i, (IInteger32**)&integer);
        arguments->Set(0, integer);
        integer = NULL;
        CInteger32::New((*mTransitions)[i], (IInteger32**)&integer);
        arguments->Set(1, integer);
        AutoPtr<ICharSequence> cs;
        CStringWrapper::New(utcTime, (ICharSequence**)&cs);
        arguments->Set(2, cs);
        cs = NULL;
        CStringWrapper::New(localTime, (ICharSequence**)&cs);
        arguments->Set(3, cs);
        AutoPtr<IBoolean> b;
        CBoolean::New((*mIsDsts)[i],(IBoolean**)&b);
        arguments->Set(4, b);
        integer = NULL;
        CInteger32::New(offset, (IInteger32**)&integer);
        arguments->Set(5, integer);
        integer = NULL;
        CInteger32::New(gmtOffset, (IInteger32**)&integer);
        arguments->Set(6, integer);
        f->Format(String("%4d : time=%11d %s = %s isDst=%d offset=%5d gmtOffset=%d\n"), arguments);
    }
    *str = sb.ToString();
    return NOERROR;
}

String CZoneInfo::FormatTime(
    /* [in] */ Int32 s,
    /* [in] */ ITimeZone* tz)
{
    AutoPtr<ISimpleDateFormat> sdf;
    CSimpleDateFormat::New(String("EEE MMM dd HH:mm:ss yyyy zzz"), (ISimpleDateFormat**)&sdf);
    sdf->SetTimeZone(tz);
    Int64 ms = ((Int64)s) * 1000LL;
    AutoPtr<IDate> date;
    CDate::New(ms, (IDate**)&date);
    String str;
    sdf->FormatDate(date, &str);
    return str;
}

ECode CZoneInfo::Clone(
    /* [out] */ IInterface** newObj)
{
    VALIDATE_NOT_NULL(newObj);
    AutoPtr<ITimeZone> timeZone;
    FAIL_RETURN(Clone((ITimeZone**)&timeZone));
    *newObj = timeZone->Probe(EIID_IInterface);
    INTERFACE_ADDREF(*newObj)
    return NOERROR;
}

ECode CZoneInfo::SetID(
    /* [in] */ const String& id)
{
    return TimeZone::SetID(id);
}

ECode CZoneInfo::GetID(
    /* [out] */ String* id)
{
    return TimeZone::GetID(id);
}

ECode CZoneInfo::GetDisplayName(
    /* [out] */ String* name)
{
    return TimeZone::GetDisplayName(name);
}

ECode CZoneInfo::GetDisplayName(
    /* [in] */ ILocale* locale,
    /* [out] */ String* name)
{
    return TimeZone::GetDisplayName(locale, name);
}

ECode CZoneInfo::GetDisplayName(
    /* [in] */ Boolean daylightTime,
    /* [in] */ Int32 style,
    /* [out] */ String* name)
{
    return TimeZone::GetDisplayName(daylightTime, style, name);
}

ECode CZoneInfo::GetDisplayName(
    /* [in] */ Boolean daylightTime,
    /* [in] */ Int32 style,
    /* [in] */ ILocale* locale,
    /* [out] */ String* name)
{
    return TimeZone::GetDisplayName(daylightTime, style, locale, name);
}


} // Utility
} // Elastos
