
#include "ext/frameworkdef.h"
#include "elastos/droid/text/format/CTime.h"
#include "elastos/droid/text/format/DateUtils.h"
#include "content/res/CResources.h"
#include "R.h"
#include <elastos/core/Math.h>
#include <elastos/core/System.h>
#include <cutils/tztime.h>

using Elastos::Droid::Content::Res::IResources;
using Elastos::Droid::Content::Res::CResources;
using Elastos::Core::Character;
using Elastos::Core::System;
using Elastos::Utility::CTimeZoneHelper;
using Elastos::Utility::ITimeZoneHelper;
using Elastos::Utility::ITimeZone;
using Libcore::ICU::ILocale;
using Libcore::ICU::ILocaleHelper;
using Libcore::ICU::ILocaleData;
using Libcore::ICU::ILocaleDataHelper;
using Libcore::ICU::CLocale;
using Libcore::ICU::CLocaleHelper;
using Libcore::ICU::CLocaleDataHelper;


namespace Elastos {
namespace Droid {
namespace Text {
namespace Format {

const String CTime::Y_M_D_T_H_M_S_000("%Y-%m-%dT%H:%M:%S.000");
const String CTime::Y_M_D_T_H_M_S_000_Z("%Y-%m-%dT%H:%M:%S.000Z");
const String CTime::Y_M_D("%Y-%m-%d");

const Int32 CTime::DAYS_PER_MONTH[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

/**
 * This array is indexed by the weekDay field (SUNDAY=0, MONDAY=1, etc.)
 * and gives a number that can be added to the yearDay to give the
 * closest Thursday yearDay.
 */
const Int32 CTime::sThursdayOffset[] = {-3, 3, 2, 1, 0, -1, -2};

const Int32 CTime::SECOND = 1;
const Int32 CTime::MINUTE = 2;
const Int32 CTime::HOUR = 3;
const Int32 CTime::MONTH_DAY = 4;
const Int32 CTime::MONTH = 5;
const Int32 CTime::YEAR = 6;
const Int32 CTime::WEEK_DAY = 7;
const Int32 CTime::YEAR_DAY = 8;
const Int32 CTime::WEEK_NUM = 9;

const Int32 CTime::SUNDAY = 0;
const Int32 CTime::MONDAY = 1;
const Int32 CTime::TUESDAY = 2;
const Int32 CTime::WEDNESDAY = 3;
const Int32 CTime::THURSDAY = 4;
const Int32 CTime::FRIDAY = 5;
const Int32 CTime::SATURDAY = 6;

CAR_INTERFACE_IMPL(CTime, Object, ITime)
CAR_OBJECT_IMPL(CTime)

CTime::CTime()
    : mAllDay(FALSE)
    , mSecond(0)
    , mMinute(0)
    , mHour(0)
    , mMonthDay(0)
    , mMonth(0)
    , mYear(0)
    , mWeekDay(0)
    , mYearDay(0)
    , mIsDst(0)
    , mGmtoff(0)
{
}

ECode CTime::constructor()
{
    AutoPtr<ITimeZoneHelper> tzh;
    CTimeZoneHelper::AcquireSingleton((ITimeZoneHelper**)&tzh);
    AutoPtr<ITimeZone> tz;
    tzh->GetDefault((ITimeZone**)&tz);
    String timezoneId;
    tz->GetID(&timezoneId);
    return Initialize(timezoneId);
}

ECode CTime::constructor(
    /* [in] */ const String& timezoneId)
{
    if (timezoneId.IsNull()) {
       // throw new NullPointerException("timezoneId is null!");
        return E_NULL_POINTER_EXCEPTION;
    }
    return Initialize(timezoneId);
}

ECode CTime::constructor(
    /* [in] */ ITime* other)
{
    CTime* other_ = (CTime*)other;
    Initialize(other_->mTimeZone);
    return Set(other);
}

/** Initialize the Time to 00:00:00 1/1/1970 in the specified timezone. */
ECode CTime::Initialize(
    /* [in] */ const String& timezoneId)
{
    mTimeZone = timezoneId;
    mYear = 1970;
    mMonthDay = 1;
    // Set the daylight-saving indicator to the unknown value -1 so that
    // it will be recomputed.
    mIsDst = -1;

    // A reusable object that performs the date/time calculations.
    mCalculator = new TimeCalculator(timezoneId);
}

ECode CTime::Normalize(
    /* [in] */ Boolean ignoreDst,
    /* [out] */ Int64* ret)
{
    VALIDATE_NOT_NULL(ret)
    mCalculator->CopyFieldsFromTime(THIS_PROBE(ITime));
    Int64 timeInMillis = mCalculator->ToMillis(ignoreDst);
    mCalculator->CopyFieldsToTime(THIS_PROBE(ITime));
    *ret = timeInMillis;
    return NOERROR;
}

ECode CTime::SwitchTimezone(
    /* [in] */ const String& timezone)
{
    mCalculator->CopyFieldsFromTime(THIS_PROBE(ITime));
    mCalculator->SwitchTimezone(timezone);
    mCalculator->CopyFieldsToTime(THIS_PROBE(ITime));
    mTimeZone = timezone;
    return NOERROR;
}

ECode CTime::GetActualMaximum(
    /* [in] */ Int32 field,
    /* [out] */ Int32* ret)
{
    VALIDATE_NOT_NULL(ret);
    switch (field) {
        case ITime::SECOND:
            *ret = 59;
            return NOERROR; // leap seconds, bah humbug
        case ITime::MINUTE:
            *ret = 59;
            return NOERROR;
        case ITime::HOUR:
            *ret = 23;
            return NOERROR;
        case ITime::MONTH_DAY: {
            Int32 n = DAYS_PER_MONTH[mMonth];
            if (n != 28) {
                *ret = n;
            }
            else {
                Int32 y = mYear;
                *ret = ((y % 4) == 0 && ((y % 100) != 0 || (y % 400) == 0)) ? 29 : 28;
            }
            return NOERROR;
        }
        case ITime::MONTH:
            *ret = 11;
            return NOERROR;
        case ITime::YEAR:
            *ret = 2037;
            return NOERROR;
        case ITime::WEEK_DAY:
            *ret = 6;
            return NOERROR;
        case ITime::YEAR_DAY: {
            Int32 y = mYear;
            // Year days are numbered from 0, so the last one is usually 364.
            *ret = ((y % 4) == 0 && ((y % 100) != 0 || (y % 400) == 0)) ? 365 : 364;
            return NOERROR;
        }
        case ITime::WEEK_NUM:
            //throw new RuntimeException("WEEK_NUM not implemented");
            *ret = 0;
            return E_RUNTIME_EXCEPTION;
        default:
            //throw new RuntimeException("bad field=" + field);
            *ret = 0;
            return  E_RUNTIME_EXCEPTION;
    }
}

ECode CTime::Clear(
    /* [in] */ const String& timezone)
{
    if (timezone.IsNull()) {
        //throw new NullPointerException("timezone is null!");
        return E_NULL_POINTER_EXCEPTION;
    }
    mTimeZone = timezone;
    mAllDay = FALSE;
    mSecond = 0;
    mMinute = 0;
    mHour = 0;
    mMonthDay = 0;
    mMonth = 0;
    mYear = 0;
    mWeekDay = 0;
    mYearDay = 0;
    mGmtoff = 0;
    mIsDst = -1;
    return NOERROR;
}

ECode CTime::Compare(
    /* [in] */ ITime* a,
    /* [in] */ ITime* b,
    /* [out] */ Int32* result)
{
    VALIDATE_NOT_NULL(result);
    if (a == NULL) {
        //throw new NullPointerException("a == null");
        return E_NULL_POINTER_EXCEPTION;
    }
    else if (b == NULL) {
        //throw new NullPointerException("b == null");
        return E_NULL_POINTER_EXCEPTION;
    }
    AutoPtr<TimeCalculator> aCalculator = (CTime*)a->mCalculator;
    AutoPtr<TimeCalculator> bCalculator = (CTime*)b->mCalculator;
    aCalculator->CopyFieldsFromTime(a);
    bCalculator->CopyFieldsFromTime(b);

    *result = TimeCalculator::Compare(aCalculator, bCalculator);
    return NOERROR;
}

ECode CTime::Format(
    /* [in] */ const String& format,
    /* [out] */ String* ret)
{
    VALIDATE_NOT_NULL(ret)
    mCalculator->CopyFieldsFromTime(THIS_PROBE(ITime));
    *ret = mCalculator->Format(format);
    return NOERROR;
}

ECode CTime::ToString(
    /* [out] */ String* ret)
{
    VALIDATE_NOT_NULL(ret)
    // toString() uses its own TimeCalculator rather than the shared one. Otherwise crazy stuff
    // happens during debugging when the debugger calls toString().
    AutoPtr<TimeCalculator> timeCalculator = new TimeCalculator(mTimeZone);
    mCalculator->CopyFieldsFromTime(THIS_PROBE(ITime));
    *ret = mCalculator->ToStringInternal();
    return NOERROR;
}

ECode CTime::Parse(
    /* [in] */ const String& s,
    /* [out] */ Boolean* ret)
{
    VALIDATE_NOT_NULL(ret)
    if (s.IsNull()) {
        //throw new NullPointerException("time string is null");
        *ret = FALSE;
        return  E_NULL_POINTER_EXCEPTION;
    }
    if (ParseInternal(s)) {
            mTimeZone = ITime::TIMEZONE_UTC;
            *ret = TRUE;
            return NOERROR;
        }
        *ret = FALSE;
        return NOERROR;
}

Boolean CTime::ParseInternal(
    /* [in] */ const String& s)
{
    Int32 len = s.GetLength();
    if (len < 8) {
        // throw new TimeFormatException("String is too short: \"" + s +
        //             "\" Expected at least 8 characters.");
        return E_TIME_FORMAT_EXCEPTION;
    }

    Boolean inUtc = FALSE;

    //year
    Int32 n = GetChar(s, 0, 1000);
    n += GetChar(s, 1, 100);
    n += GetChar(s, 2, 10);
    n += GetChar(s, 3, 1);
    mYear = n;

    //month
    n = GetChar(s, 4, 10);
    n += GetChar(s, 5, 1);
    n--;
    mMonth = n;

    // day of month
    n = GetChar(s, 6, 10);
    n += GetChar(s, 7, 1);
    mMonthDay = n;

    if (len > 8) {
        if (len < 15) {
            // throw new TimeFormatException(
            //         "String is too short: \"" + s
            //                 + "\" If there are more than 8 characters there must be at least"
            //                 + " 15.");
            return E_TIME_FORMAT_EXCEPTION;
        }
        CheckChar(s, 8, 'T');
        mAllDay = FALSE;

        // hour
        n = GetChar(s, 9, 10);
        n += GetChar(s, 10, 1);
        mHour = n;

        // min
        n = GetChar(s, 11, 10);
        n += GetChar(s, 12, 1);
        mMinute = n;

        // sec
        n = GetChar(s, 13, 10);
        n += GetChar(s, 14, 1);
        mSecond = n;

        if (len > 15) {
            // Z
            CheckChar(s, 15, 'Z');
            inUtc = TRUE;
        }
    } else {
        mAllDay = TRUE;
        mHour = 0;
        mMinute = 0;
        mSecond = 0;
    }

    mWeekDay = 0;
    mYearDay = 0;
    mIsDst = -1;
    mGmtoff = 0;
    return inUtc;
}

ECode CTime::CheckChar(
    /* [in] */ const String& s,
    /* [in] */ Int32 spos,
    /* [in] */ Char32 expected)
{
    Char32 c = s.GetChar(spos);
    if (c != expected) {
        // throw new TimeFormatException(String.format(
        //             "Unexpected character 0x%02d at pos=%d.  Expected 0x%02d (\'%c\').",
        //             (int) c, spos, (int) expected, expected));
        return E_TIME_FORMAT_EXCEPTION;
    }
}

Int32 CTime::GetChar(
    /* [in] */ const String& s,
    /* [in] */ Int32 spos,
    /* [in] */ Int32 mul)
{
    Char32 c = s.GetChar(spos);
    if (Character::IsDigit(c)) {
        return Character::GetNumericValue(c) * mul
    } else {
        // throw new TimeFormatException("Parse error at pos=" + spos);
        return E_TIME_FORMAT_EXCEPTION;
    }
}

ECode CTime::Parse3339(
    /* [in] */ const String& s,
    /* [out] */ Boolean* ret)
{
    VALIDATE_NOT_NULL(ret);
    if (s.IsNull()) {
        //throw new NullPointerException("time string is null");
        *ret = FALSE;
        return E_NULL_POINTER_EXCEPTION;
    }
    if (Parse3339Internal(s)) {
        mTimeZone = ITime::TIMEZONE_UTC;
        *ret = TRUE;
        return NOERROR;
     }
    *ret = FALSE;
    return NOERROR;
}

Boolean CTime::Parse3339Internal(
    /* [in] */ const String& s)
{
    Int32 len = s.GetLength();
    if (len < 10) {
        // throw new TimeFormatException("String too short --- expected at least 10 characters.");
        return E_TIME_FORMAT_EXCEPTION;
    }
    Boolean inUtc = FALSE;

    // year
    Int32 n = GetChar(s, 0, 1000);
    n += GetChar(s, 1, 100);
    n += GetChar(s, 2, 10);
    n += GetChar(s, 3, 1);
    mYear = n;

    CheckChar(s, 4, '-');

    // month
    n = GetChar(s, 5, 10);
    n += GetChar(s, 6, 1);
    --n;
    mMonth = n;

    CheckChar(s, 7, '-');

    // day
    n = GetChar(s, 8, 10);
    n += GetChar(s, 9, 1);
    mMonthDay = n;

    if (len >= 19) {
        // T
        CheckChar(s, 10, 'T');
        mAllDay = FALSE;

        // hour
        n = GetChar(s, 11, 10);
        n += GetChar(s, 12, 1);

        // Note that this.hour is not set here. It is set later.
        Int32 hour = n;

        CheckChar(s, 13, ':');

        // minute
        n = GetChar(s, 14, 10);
        n += GetChar(s, 15, 1);
        // Note that this.minute is not set here. It is set later.
        Int32 minute = n;

        CheckChar(s, 16, ':');

        // second
        n = GetChar(s, 17, 10);
        n += GetChar(s, 18, 1);
        mSecond = n;

        // skip the '.XYZ' -- we don't care about subsecond precision.

        Int32 tzIndex = 19;
        if (tzIndex < len && s.GetChar(tzIndex) == '.') {
         do {
             tzIndex++;
         } while (tzIndex < len && Character::IsDigit(s.GetChar(tzIndex)));
        }

        Int32 offset = 0;
        if (len > tzIndex) {
            Char32 c = s.GetChar(tzIndex);
            // NOTE: the offset is meant to be subtracted to get from local time
            // to UTC.  we therefore use 1 for '-' and -1 for '+'.
            switch (c) {
                case 'Z':
                    // Zulu time -- UTC
                    offset = 0;
                    break;
                case '-':
                    offset = 1;
                    break;
                case '+':
                    offset = -1;
                    break;
                default:
                    // throw new TimeFormatException(String.format(
                    //      "Unexpected character 0x%02d at position %d.  Expected + or -",
                    //      (int) c, tzIndex));
                    return E_TIME_FORMAT_EXCEPTION;
         }
         inUtc = TRUE;

         if (offset != 0) {
             if (len < tzIndex + 6) {
                 // throw new TimeFormatException(
                 //         String.format("Unexpected length; should be %d characters",
                 //                 tzIndex + 6));
                return E_TIME_FORMAT_EXCEPTION;
             }

             // hour
             n = GetChar(s, tzIndex + 1, 10);
             n += GetChar(s, tzIndex + 2, 1);
             n *= offset;
             hour += n;

             // minute
             n = GetChar(s, tzIndex + 4, 10);
             n += GetChar(s, tzIndex + 5, 1);
             n *= offset;
             minute += n;
            }
        }
        mHour = hour;
        mMinute = minute;

        if (offset != 0) {
            Int32 ret;
            Normalize(FALSE, &ret);
        }
    } else {
     mAllDay = TRUE;
     mHour = 0;
     mMinute = 0;
     mSecond = 0;
    }

    mWeekDay = 0;
    mYearDay = 0;
    mIsDst = -1;
    mGmtoff = 0;
    return inUtc;
}

String CTime::GetCurrentTimezone()
{
    AutoPtr<ITimeZoneHelper> tzh;
    CTimeZoneHelper::AcquireSingleton((ITimeZoneHelper**)&tzh);
    AutoPtr<ITimeZone> tz;
    tzh->GetDefault((ITimeZone**)&tz);
    String ret;
    tz->GetID(&ret);
    return ret;
}

ECode CTime::SetToNow()
{
    Int64 millis;
    return Set((CSystem::GetCurrentTimeMillis(&millis), millis));
}

ECode CTime::ToMillis(
    /* [in] */ Boolean ignoreDst,
    /* [out] */ Int64* ret)
{
    mCalculator->CopyFieldsFromTime(THIS_PROBE(ITime));
    *ret = mCalculator->ToMillis(ignoreDst);
    return NOERROR;
}

ECode CTime::Set(
    /* [in] */ Int64 millis)
{
    mAllDay = FALSE;
    mCalculator->mTimeZone = mTimeZone;
    mCalculator->SetTimeInMillis(millis);
    mCalculator->CopyFieldsToTime(THIS_PROBE(ITime));
    return NOERROR;
}

ECode CTime::Format2445(
    /* [out] */ String* ret)
{
    VALIDATE_NOT_NULL(ret)
    mCalculator->CopyFieldsFromTime(THIS_PROBE(ITime));
    *ret = mCalculator->Format2445(!mAllDay);
    return NOERROR;
}

ECode CTime::Set(
    /* [in] */ ITime* that)
{
    CTime* thatTime = (CTime*)that;
    mTimeZone = thatTime->mTimeZone;
    mAllDay = thatTime->mAllDay;
    mSecond = thatTime->mSecond;
    mMinute = thatTime->mMinute;
    mHour = thatTime->mHour;
    mMonthDay = thatTime->mMonthDay;
    mMonth = thatTime->mMonth;
    mYear = thatTime->mYear;
    mWeekDay = thatTime->mWeekDay;
    mYearDay = thatTime->mYearDay;
    mIsDst = thatTime->mIsDst;
    mGmtoff = thatTime->mGmtoff;
    return NOERROR;
}

ECod CTime::Set(
    /* [in] */ Int32 second,
    /* [in] */ Int32 minute,
    /* [in] */ Int32 hour,
    /* [in] */ Int32 monthDay,
    /* [in] */ Int32 month,
    /* [in] */ Int32 year)
{
    mAllDay = FALSE;
    mSecond = second;
    mMinute = minute;
    mHour = hour;
    mMonthDay = monthDay;
    mMonth = month;
    mYear = year;
    mWeekDay = 0;
    mYearDay = 0;
    mIsDst = -1;
    mGmtoff = 0;
    return NOERROR;
}

ECode CTime::Set(
    /* [in] */ Int32 monthDay,
    /* [in] */ Int32 month,
    /* [in] */ Int32 year)
{
    mAllDay = TRUE;
    mSecond = 0;
    mMinute = 0;
    mHour = 0;
    mMonthDay = monthDay;
    mMonth = month;
    mYear = year;
    mWeekDay = 0;
    mYearDay = 0;
    mIsDst = -1;
    mGmtoff = 0;
    return NOERROR;
}

ECode CTime::Before(
    /* [in] */ ITime* that,
    /* [out] */ Boolean* ret)
{
    VALIDATE_NOT_NULL(ret)
    Int32 result;
    FAIL_RETURN(Compare(this, that, &result));
    *ret = result < 0;
    return NOERROR;
}

ECode CTime::After(
    /* [in] */ ITime* that,
    /* [out] */ Boolean* ret)
{
    VALIDATE_NOT_NULL(ret)
    Int32 result;
    FAIL_RETURN(Compare(this, that, &result));
    *ret = result > 0;
    return NOERROR;
}

ECode CTime::GetWeekNumber(
    /* [out] */ Int32* ret)
{
    VALIDATE_NOT_NULL(ret)
    // Get the year day for the closest Thursday
    Int32 closestThursday = mYearDay + sThursdayOffset[mWeekDay];

    // Year days start at 0
    if (closestThursday >= 0 && closestThursday <= 364) {
        *ret = closestThursday / 7 + 1;
        return NOERROR;
    }

    // The week crosses a year boundary.
    AutoPtr<CTime> temp;
    CTime::NewByFriend(this, (CTime**)&temp);
    temp->mMonthDay += sThursdayOffset[mWeekDay];
    Int64 iTmp;
    temp->Normalize(TRUE, &iTmp/* ignore isDst */);
    *ret = temp->mYearDay / 7 + 1;
    return NOERROR;
}

ECode CTime::Format3339(
    /* [in] */ Boolean allDay,
    /* [out] */ String* ret)
{
    VALIDATE_NOT_NULL(ret);
    if (mAllDay) {
        return Format(Y_M_D, ret);
    }
    else if ((ITime::TIMEZONE_UTC).Equals(mTimeZone)) {
        return Format(Y_M_D_T_H_M_S_000_Z, ret);
    }
    else {
        String base;
        FAIL_RETURN(Format(Y_M_D_T_H_M_S_000, &base));
        String sign = (mGmtoff < 0) ? String("-") : String("+");
        Int32 offset = (Int32)Elastos::Core::Math::Abs(mGmtoff);
        Int32 minutes = (offset % 3600) / 60;
        Int32 hours = offset / 3600;

        assert(0 && "TODO");
        //Java:    return String.format(Locale.US, "%s%s%02d:%02d", base, sign, hours, minutes);
        // String ret;
        // ret = String.format(Locale.US, "%s%s%02d:%02d", base, sign, hours, minutes);
        return NOERROR;
    }
}

Boolean CTime::IsEpoch(
    /* [in] */ ITime* time)
{
    Int64 millis;
    time->ToMillis(TRUE, &millis);
    return GetJulianDay(millis, 0) == ITime::EPOCH_JULIAN_DAY;
}

Int32 CTime::GetJulianDay(
    /* [in] */ Int64 millis,
    /* [in] */ Int64 gmtoff)
{
    Int64 offsetMillis = gmtoff * 1000;
    Int64 julianDay = (millis + offsetMillis) / IDateUtils::DAY_IN_MILLIS;
    return (Int32) julianDay + ITime::EPOCH_JULIAN_DAY;
}

ECode CTime::SetJulianDay(
    /* [in] */ Int32 julianDay,
    /* [out] */ Int64* ret)
{
    VALIDATE_NOT_NULL(ret);
    // Don't bother with the GMT offset since we don't know the correct
    // value for the given Julian day.  Just get close and then adjust
    // the day.
    Int64 millis = (julianDay - ITime::EPOCH_JULIAN_DAY) * IDateUtils::DAY_IN_MILLIS;
    Set(millis);

    // Figure out how close we are to the requested Julian day.
    // We can't be off by more than a day.
    Int32 approximateDay = GetJulianDay(millis, mGmtoff);
    Int32 diff = julianDay - approximateDay;
    mMonthDay += diff;

    // Set the time to 12am and re-normalize.
    mHour = 0;
    mMinute = 0;
    mSecond = 0;
    return Normalize(TRUE, ret);
}

ECode CTime::GetWeeksSinceEpochFromJulianDay(
    /* [in] */ Int32 julianDay,
    /* [in] */ Int32 firstDayOfWeek)
{
    Int32 diff = ITime::THURSDAY - firstDayOfWeek;
    if (diff < 0) {
        diff += 7;
    }
    Int32 refDay = ITime::EPOCH_JULIAN_DAY - diff;
    return (julianDay - refDay) / 7;
}

ECode CTime::GetJulianMondayFromWeeksSinceEpoch(
    /* [in] */ Int32 week)
{
    return ITime::MONDAY_BEFORE_JULIAN_EPOCH + week * 7;
}

ECode CTime::SetAllDay(
    /* [in] */ Boolean bAllDay)
{
    mAllDay = bAllDay;
    return NOERROR;
}

ECode CTime::GetAllDay(
    /* [out] */ Boolean* bAllDay)
{
    VALIDATE_NOT_NULL(bAllDay);
    *bAllDay = mAllDay;
    return NOERROR;
}

ECode CTime::SetSecond(
    /* [in] */ Int32 second)
{
    mSecond = second;
    return NOERROR;
}

ECode CTime::GetSecond(
    /* [out] */ Int32* second)
{
    VALIDATE_NOT_NULL(second);
    *second = mSecond;
    return NOERROR;
}

ECode CTime::SetMinute(
    /* [in] */ Int32 minute)
{
    mMinute = minute;
    return NOERROR;
}

ECode CTime::GetMinute(
    /* [out] */ Int32* minute)
{
    VALIDATE_NOT_NULL(minute);
    *minute = mMinute;
    return NOERROR;
}

ECode CTime::SetHour(
    /* [in] */ Int32 hour)
{
    mHour = hour;
    return NOERROR;
}

ECode CTime::GetHour(
    /* [out] */ Int32* hour)
{
    VALIDATE_NOT_NULL(hour);
    *hour = mHour;
    return NOERROR;
}

ECode CTime::SetMonthDay(
    /* [in] */ Int32 monthDay)
{
    mMonthDay = monthDay;
    return NOERROR;
}

ECode CTime::GetMonthDay(
    /* [out] */ Int32* monthDay)
{
    VALIDATE_NOT_NULL(monthDay);
    *monthDay = mMonthDay;
    return NOERROR;
}

ECode CTime::SetMonth(
    /* [in] */ Int32 month)
{
    mMonth = month;
    return NOERROR;
}

ECode CTime::GetMonth(
    /* [out] */ Int32* month)
{
    VALIDATE_NOT_NULL(month);
    *month = mMonth;
    return NOERROR;
}

ECode CTime::SetYear(
    /* [in] */ Int32 year)
{
    mYear = year;
    return NOERROR;
}

ECode CTime::GetYear(
    /* [out] */ Int32* year)
{
    VALIDATE_NOT_NULL(year);
    *year = mYear;
    return NOERROR;
}

ECode CTime::SetWeekDay(
    /* [in] */ Int32 weekDay)
{
    mWeekDay = weekDay;
    return NOERROR;
}

ECode CTime::GetWeekDay(
    /* [out] */ Int32* weekDay)
{
    VALIDATE_NOT_NULL(weekDay);
    *weekDay = mWeekDay;
    return NOERROR;
}

ECode CTime::SetYearDay(
    /* [in] */ Int32 yearDay)
{
    mYearDay = yearDay;
    return NOERROR;
}

ECode CTime::GetYearDay(
    /* [out] */ Int32* yearDay)
{
    VALIDATE_NOT_NULL(yearDay);
    *yearDay = mYearDay;
    return NOERROR;
}

ECode CTime::SetIsDst(
    /* [in] */ Int32 isDst)
{
    mIsDst = isDst;
    return NOERROR;
}

ECode CTime::GetIsDst(
    /* [out] */ Int32* isDst)
{
    VALIDATE_NOT_NULL(isDst);
    *isDst = mIsDst;
    return NOERROR;
}

ECode CTime::SetGmtoff(
    /* [in] */ Int64 gmtoff)
{
    mGmtoff = gmtoff;
    return NOERROR;
}

ECode CTime::GetGmtoff(
    /* [out] */ Int64* gmtoff)
{
    VALIDATE_NOT_NULL(gmtoff);
    *gmtoff = mGmtoff;
    return NOERROR;
}

ECode CTime::SetTimeZone(
    /* [in] */ const String& timezone)
{
    mTimeZone = timezone;
    return NOERROR;
}

ECode CTime::GetTimeZone(
    /* [out] */ String* timezone)
{
    VALIDATE_NOT_NULL(timezone);
    *timezone = mTimeZone;
    return NOERROR;
}

CTime::TimeCalculator::TimeCalculator(
    /* [in] */ const String& timezonId)
{
    mZoneInfo = LookupZoneInfo(timezoneId);
    CZoneInfoWallTime::New((IZoneInfoWallTime**)&mWallTime);
}

Int64 CTime::TimeCalculator::ToMillis(
    /* [in] */ Boolean ignoreDst)
{
    if (ignoreDst) {
        mWallTime->SetIsDst(-1);
    }

    Int32 r = mWallTime->MkTime(mZoneInfo);
    if (r == -1) {
        return -1;
    }
    return r * 1000L;
}

ECode CTime::TimeCalculator::SetTimeInMillis(
    /* [in] */ Int64 millis)
{
   // Preserve old 32-bit Android behavior.
    Int32 intSeconds = (Int32) (millis / 1000);

    UpdateZoneInfoFromTimeZone();
    return mWallTime->LocalTime(intSeconds, mZoneInfo);
}

String CTime::TimeCalculator::Format(
    /* [in] */ const String& format)
{
    if (format.IsNull())
    {
        format = "%c";
    }
    AutoPtr<ITimeFormatter> formatter;
    CTimeFormatter::New((ITimeFormatter**)&formatter);
    assert(0 && "TODO");
    return formatter->Format(format, mWallTime, mZoneInfo);
}

ECode CTime::TimeCalculator::UpdateZoneInfoFromTimeZone()
{
    String zoneInfoID = mZoneInfo->GetID();
    Boolean isEquals = FALSE;
    if (!(zoneInfoID->Equals(mTimeZone,&isEquals), isEquals))
    {
        mZoneInfo = LookupZoneInfo(mTimeZone);
    }
    return NOERROR;
}

ECode CTime::TimeCalculator::LookupZoneInfo(
    /* [in] */ const String& timezoneId ,
    /* [out] */ ZoneInfo** ret)
{
    VALIDATE_NOT_NULL(ret)
    // try {
    AutoPtr<IZoneInfo> zoneInfo;
    AutoPtr<TzData> tzdata = CZoneInfoDB::GetInstance();
    tzdata->MakeTimeZone(timezoneId, &zoneInfo);
    if (zoneInfo == NULL)
    {
        tzdata->MakeTimeZone("GMT", &zoneInfo);
    }
    if (zoneInfo == NULL)
    {
        // throw new AssertionError("GMT not found: \"" + timezoneId + "\"");
        return E_ASSERTION_ERROR;
    }
    *ret = zoneInfo;
    REFCOUNT_ADD()
    return NOERROR;
    // } catch (IOException e) {
    // This should not ever be thrown.
        // throw new AssertionError("Error loading timezone: \"" + timezoneId + "\"", e);
    // }
}

ECode CTime::TimeCalculator::SwitchTimezone(
    /* [in] */ const String& timezone)
{
    Int32 seconds;
    mWallTime->Mktime(zoneInfo, &seconds);
    mTimeZone = timezone;
    UpdateZoneInfoFromTimeZone();
    return mWallTime->Localtime(seconds, zoneInfo)
}

String CTime::TimeCalculator::Format2445(
    /* [in] */ Boolean hasTime)
{
    AutoPtr<ArrayOf<Char32> > buf = new Char32[hasTime ? 16 : 8];
    Int32 n;
    mWallTime->GetYear(&n);

    buf[0] = ToChar(n / 1000);
    n %= 1000;
    buf[1] = ToChar(n / 100);
    n %= 100;
    buf[2] = ToChar(n / 10);
    n %= 10;
    buf[3] = ToChar(n);

    Int32 n1;
    mWallTime->GetMonth(&n1);
    n = n1 + 1;
    buf[4] = ToChar(n / 10);
    buf[5] = ToChar(n % 10);

    mWallTime->GetMonthDay(&n);
    buf[6] = ToChar(n / 10);
    buf[7] = ToChar(n % 10);

    if (!hasTime) {
        assert(0 && "TODO");
        // return new String(buf, 0, 8);
    }

    buf[8] = 'T';

    mWallTime->GetHour(&n);
    buf[9] = ToChar(n / 10);
    buf[10] = ToChar(n % 10);

    mWallTime->GetMinute(&n);
    buf[11] = ToChar(n / 10);
    buf[12] = ToChar(n % 10);

    mWallTime->GetSecond(&n);
    buf[13] = ToChar(n / 10);
    buf[14] = ToChar(n % 10);

    if (ITime::TIMEZONE_UTC.Equals(mTimeZone))
    {
        // The letter 'Z' is appended to the end.
        buf[15] = 'Z';
        assert(0 && "TODO");
        // return new String(buf, 0, 16);
    } else {
        assert(0 && "TODO");
        // return new String(buf, 0, 15);
    }
}

Char32 CTime::TimeCalculator::ToChar(
    /* [in] */ Int32 n)
{
    return (n >= 0 && n <= 9) ? (Char32)(n + '0') : ' ';
}

String CTime::TimeCalculator::ToStringInternal()
{
    // This implementation possibly displays the un-normalized fields because that is
    // what it has always done.
    assert(0 && "TODO");
    // return String.format("%04d%02d%02dT%02d%02d%02d%s(%d,%d,%d,%d,%d)",
    //                 wallTime.getYear(),
    //                 wallTime.getMonth() + 1,
    //                 wallTime.getMonthDay(),
    //                 wallTime.getHour(),
    //                 wallTime.getMinute(),
    //                 wallTime.getSecond(),
    //                 timezone,
    //                 wallTime.getWeekDay(),
    //                 wallTime.getYearDay(),
    //                 wallTime.getGmtOffset(),
    //                 wallTime.getIsDst(),
    //                 toMillis(false /* use isDst */) / 1000
    //         );

}

Int32 CTime::TimeCalculator::Compare(
    /* [in] */ AutoPtr<TimeCalculator> aObject,
    /* [in] */ AutoPtr<TimeCalculator> bObject)
{
    if((aObject->mTimeZone).Equals(bObject->mTimeZone)) {
        // If the timezones are the same, we can easily compare the two times.
        AutoPtr<IZoneInfoWallTime> awallTime = aObject->mWallTime;
        AutoPtr<IZoneInfoWallTime> bwallTime = bObject->mWallTime;
        Int32 aYear;
        Int32 bYear;
        awallTime->GetYear(&aYear);
        bwallTime->GetYear(&bYear);
        Int32 diff = aYear - bYear;
        if (diff != 0) {
            return diff;
        }

        Int32 aMonth;
        Int32 bMonth;
        awallTime->GetMonth(&aMonth);
        bwallTime->GetMonth(&bMonth);
        diff = aMonth - bMonth;
        if (diff != 0 ) {
            return diff;
        }

        Int32 aMonthDay;
        Int32 bMonthDay;
        awallTime->GetMonthDay(&aMonthDay);
        bwallTime->GetMonthDay(&bMonthDay);
        diff = aMonthDay - bMonthDay;
        if (diff != 0) {
            return diff;
        }

        Int32 aHour;
        Int32 bHour;
        awallTime->GetHour(&aHour);
        bwallTime->GetHour(&bHour);
        diff = aHour - bHour;
        if (diff != 0) {
            return diff;
        }

        Int32 aMinute;
        Int32 bMinute;
        awallTime->GetMinute(&aMinute);
        bwallTime->GetMinute(&bMinute);
        diff = aMinute - bMinute;
        if (diff != 0) {
            return diff;
        }

        Int32 aSecond;
        Int32 bSecond;
        awallTime->GetMinute(&aSecond);
        bwallTime->GetMinute(&bSecond);
        diff = aSecond - bSecond;
        if (diff != 0) {
            return diff;
        }

        return 0;
    } else {
        // Otherwise, convert to milliseconds and compare that. This requires that object be
        // normalized. Note: For dates that do not exist: toMillis() can return -1, which
        // can be confused with a valid time.
        Int64 am = aObject->ToMillis(FALSE);
        Int64 bm = bObject->ToMillis(FALSE);
        Int64 diff = am - bm;
        return (diff < 0) ? -1 : ((diff > 0) ? 1 : 0);
    }
}

ECode CTime::TimeCalculator::CopyFieldsToTime(
    /* [in] */ ITime* time)
{
    CTime* tm = (CTime*)time;
    mWallTime->GetSecond(&tm->mSecond);
    mWallTime->GetMinute(&tm->mMinute);
    mWallTime->GetHour(&tm->mHour);
    mWallTime->GetMonthDay(&tm->mMonthDay);
    mWallTime->GetMonth(&tm->mMonth);
    mWallTime->GetYear(&tm->mYear);

    // Read-only fields that are derived from other information above.
    mWallTime->GetWeekDay(&tm->mWeekDay);
    mWallTime->GetYearDay(&tm->mYearDay);

    // < 0: DST status unknown, 0: is not in DST, 1: is in DST
    mWallTime->GetIsDst(&tm->mIsDst);
    // This is in seconds and includes any DST offset too.
    return mWallTime->GetGmtOffset(&tm->mGmtoff);
}

ECode CTime::TimeCalculator::CopyFieldsFromTime(
    /* [in] */ ITime* time)
{
    CTime* tm = (CTime*)time;
    mWallTime->SetSecond(tm->mSecond);
    mWallTime->SetMinute(tm->mMinute);
    mWallTime->SetHour(tm->mHour);
    mWallTime->SetMonthDay(tm->mMonthDay);
    mWallTime->SetMonth(tm->mMonth);
    mWallTime->SetYear(tm->mYear);
    mWallTime->SetWeekDay(tm->mWeekDay);
    mWallTime->SetYearDay(tm->mYearDay);
    mWallTime->SetIsDst(tm->mIsDst);
    mWallTime->SetGmtOffset(tm->mGmtoff);

    if (tm->mAllDay && (tm->mSecond != 0 || tm->mMinute != 0 || tm->mHour != 0)) {
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }

    mTimeZone = tm->mTimeZone;
    return UpdateZoneInfoFromTimeZone();
}

} // namespace Format
} // namespace Text
} // namepsace Droid
} // namespace Elastos

