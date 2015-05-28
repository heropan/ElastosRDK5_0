
#include "CDate.h"
#include "CSystem.h"
#include <elastos/Character.h>
#include <elastos/StringBuilder.h>
#include <elastos/StringUtils.h>
#include "CGregorianCalendar.h"
#include "CTimeZoneHelper.h"
#include "CDateFormatHelper.h"
#include "CSimpleDateFormat.h"
#include "CDateFormatSymbols.h"
#include "CLocaleHelper.h"
#include "CLocaleDataHelper.h"

using Elastos::Core::ISystem;
using Elastos::Core::Character;
using Elastos::Core::StringBuilder;
using Elastos::Core::StringUtils;
using Elastos::Text::IFormat;
using Elastos::Text::IDateFormatHelper;
using Elastos::Text::CDateFormatHelper;
using Elastos::Text::ISimpleDateFormat;
using Elastos::Text::CSimpleDateFormat;
using Elastos::Text::IDateFormatSymbols;
using Elastos::Text::CDateFormatSymbols;
using Elastos::Text::IDateFormat;
using Libcore::ICU::ILocale;
using Libcore::ICU::ILocaleHelper;
using Libcore::ICU::CLocaleHelper;
using Libcore::ICU::ILocaleData;
using Libcore::ICU::ILocaleDataHelper;
using Libcore::ICU::CLocaleDataHelper;

namespace Elastos{
namespace Utility{

Int32 CDate::sCreationYear = -1;

Int32 CDate::GetCreationYear()
{
    if (sCreationYear == -1) {
        AutoPtr<CDate> d;
        ASSERT_SUCCEEDED(CDate::NewByFriend((CDate**)&d));
        d->GetYear(&sCreationYear);
    }

    return sCreationYear;
}

CDate::CDate()
    : mMilliseconds(0)
{ }

ECode CDate::constructor()
{
    AutoPtr<ISystem> system;
    Elastos::Core::CSystem::AcquireSingleton((ISystem**)&system);
    system->GetCurrentTimeMillis(&mMilliseconds);
    return NOERROR;
}

ECode CDate::constructor(
    /* [in] */ Int32 year,
    /* [in] */ Int32 month,
    /* [in] */ Int32 day)
{
    AutoPtr<CGregorianCalendar> cal;
    FAIL_RETURN(CGregorianCalendar::NewByFriend(FALSE, (CGregorianCalendar**)&cal));
    FAIL_RETURN(cal->Set(1900 + year, month, day));
    FAIL_RETURN(cal->GetTimeInMillis(&mMilliseconds));
    return NOERROR;
}

ECode CDate::constructor(
    /* [in] */ Int32 year,
    /* [in] */ Int32 month,
    /* [in] */ Int32 day,
    /* [in] */ Int32 hour,
    /* [in] */ Int32 minute)
{
    AutoPtr<CGregorianCalendar> cal;
    FAIL_RETURN(CGregorianCalendar::NewByFriend(FALSE, (CGregorianCalendar**)&cal));
    FAIL_RETURN(cal->Set(1900 + year, month, day, hour, minute));
    FAIL_RETURN(cal->GetTimeInMillis(&mMilliseconds));
    return NOERROR;
}

ECode CDate::constructor(
    /* [in] */ Int32 year,
    /* [in] */ Int32 month,
    /* [in] */ Int32 day,
    /* [in] */ Int32 hour,
    /* [in] */ Int32 minute,
    /* [in] */ Int32 second)
{
    AutoPtr<CGregorianCalendar> cal;
    FAIL_RETURN(CGregorianCalendar::NewByFriend(FALSE, (CGregorianCalendar**)&cal));
    FAIL_RETURN(cal->Set(1900 + year, month, day, hour, minute, second));
    FAIL_RETURN(cal->GetTimeInMillis(&mMilliseconds));
    return NOERROR;
}

ECode CDate::constructor(
    /* [in] */ Int64 milliseconds)
{
    mMilliseconds = milliseconds;
    return NOERROR;
}

ECode CDate::constructor(
    /* [in] */ const String& string)
{
    return Parse(string, &mMilliseconds);
}

ECode CDate::IsAfter(
    /* [in] */ IDate* date,
    /* [out] */ Boolean* isAfter)
{
    VALIDATE_NOT_NULL(date);
    VALIDATE_NOT_NULL(isAfter);

    Int64 otherTime;
    date->GetTime(&otherTime);
    *isAfter = mMilliseconds > otherTime;
    return NOERROR;
}

ECode CDate::IsBefore(
    /* [in] */ IDate* date,
    /* [out] */ Boolean* isBefore)
{
    VALIDATE_NOT_NULL(date);
    VALIDATE_NOT_NULL(isBefore);

    Int64 otherTime;
    date->GetTime(&otherTime);
    *isBefore = mMilliseconds < otherTime;
    return NOERROR;
}

ECode CDate::Clone(
    /* [out] */ IInterface** date)
{
    VALIDATE_NOT_NULL(date);
    AutoPtr<IDate> temp;
    FAIL_RETURN(CDate::New(mMilliseconds,(IDate**)&temp));
    *date = temp;
    INTERFACE_ADDREF(*date);
    return NOERROR;
}

ECode CDate::CompareTo(
    /* [in] */ IInterface* _date,
    /* [out] */ Int32* result)
{
    VALIDATE_NOT_NULL(result);

    AutoPtr<IDate> date = IDate::Probe(_date);
    if (date == NULL) {
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    Int64 otherTime;
    date->GetTime(&otherTime);
    if (mMilliseconds < otherTime) {
        *result = -1;
    }
    else if (mMilliseconds == otherTime) {
        *result = 0;
    }
    else {
        *result = 1;
    }
    return NOERROR;
}

ECode CDate::Equals(
    /* [in] */ IInterface* date,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
    *result = FALSE;

    if (IDate::Probe(date) == NULL)
        return NOERROR;

    Int64 otherTime;
    IDate::Probe(date)->GetTime(&otherTime);
    *result = (otherTime == mMilliseconds);
    return NOERROR;
}

ECode CDate::GetDate(
    /* [out] */ Int32* date)
{
    VALIDATE_NOT_NULL(date);

    AutoPtr<CGregorianCalendar> calendar;
    FAIL_RETURN(CGregorianCalendar::NewByFriend(mMilliseconds, (CGregorianCalendar**)&calendar));
    return calendar->Get(ICalendar::DATE, date);
}

ECode CDate::GetDay(
    /* [out] */ Int32* day)
{
    VALIDATE_NOT_NULL(day);

    AutoPtr<CGregorianCalendar> calendar;
    FAIL_RETURN(CGregorianCalendar::NewByFriend(mMilliseconds, (CGregorianCalendar**)&calendar));
    FAIL_RETURN(calendar->Get(ICalendar::DAY_OF_WEEK, day));
    *day = *day - 1;
    return NOERROR;
}

ECode CDate::GetHours(
    /* [out] */ Int32* hours)
{
    VALIDATE_NOT_NULL(hours);

    AutoPtr<CGregorianCalendar> calendar;
    FAIL_RETURN(CGregorianCalendar::NewByFriend(mMilliseconds, (CGregorianCalendar**)&calendar));
    return calendar->Get(ICalendar::HOUR_OF_DAY, hours);
}

ECode CDate::GetMinutes(
    /* [out] */ Int32* minutes)
{
    VALIDATE_NOT_NULL(minutes);

    AutoPtr<CGregorianCalendar> calendar;
    FAIL_RETURN(CGregorianCalendar::NewByFriend(mMilliseconds, (CGregorianCalendar**)&calendar));
    return calendar->Get(ICalendar::MINUTE, minutes);
}

ECode CDate::GetMonth(
    /* [out] */ Int32* month)
{
    VALIDATE_NOT_NULL(month);

    AutoPtr<CGregorianCalendar> calendar;
    FAIL_RETURN(CGregorianCalendar::NewByFriend(mMilliseconds, (CGregorianCalendar**)&calendar));
    return calendar->Get(ICalendar::MONTH, month);
}

ECode CDate::GetSeconds(
    /* [out] */ Int32* seconds)
{
    VALIDATE_NOT_NULL(seconds);

    AutoPtr<CGregorianCalendar> calendar;
    FAIL_RETURN(CGregorianCalendar::NewByFriend(mMilliseconds, (CGregorianCalendar**)&calendar));
    return calendar->Get(ICalendar::SECOND, seconds);
}

ECode CDate::GetTime(
    /* [out] */ Int64* time)
{
    VALIDATE_NOT_NULL(time);

    *time = mMilliseconds;
    return NOERROR;
}

ECode CDate::GetTimezoneOffset(
    /* [out] */ Int32* timezoneOffset)
{
    VALIDATE_NOT_NULL(timezoneOffset);

    AutoPtr<CGregorianCalendar> calendar;
    FAIL_RETURN(CGregorianCalendar::NewByFriend(mMilliseconds, (CGregorianCalendar**)&calendar));
    Int32 zoneOff, dstOff;
    calendar->Get(ICalendar::ZONE_OFFSET, &zoneOff);
    calendar->Get(ICalendar::DST_OFFSET, &dstOff);
    *timezoneOffset = -(zoneOff + dstOff) / 60000;
    return NOERROR;
}

ECode CDate::GetYear(
    /* [out] */ Int32* year)
{
    VALIDATE_NOT_NULL(year);
    *year = 0;

    AutoPtr<CGregorianCalendar> calendar;
    FAIL_RETURN(CGregorianCalendar::NewByFriend(mMilliseconds, (CGregorianCalendar**)&calendar));
    FAIL_RETURN(calendar->Get(ICalendar::YEAR, year));
    *year = *year - 1900;
    return NOERROR;
}

ECode CDate::GetHashCode(
    /* [out] */ Int32* hashCode)
{
    VALIDATE_NOT_NULL(hashCode)
    *hashCode = (Int32)((UInt64)mMilliseconds >> 32) ^ (Int32)mMilliseconds;
    return NOERROR;
}

Int32 CDate::Parse(
    /* [in] */ const String& string,
    /* [in] */ const ArrayOf<String>& array)
{
    Int32 i = 0;
    Int32 alength = array.GetLength();
    Int32 slength = string.GetLength();
    for (; i < alength; i++) {
        if (string.RegionMatchesIgnoreCase(0, array[i], 0, slength)) {
            return i;
        }
    }
    return -1;
}

ECode CDate::Parse(
    /* [in] */ const String& string,
    /* [out] */ Int64* value)
{
    VALIDATE_NOT_NULL(value);

    if (string.IsNull()) {
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }

    AutoPtr<ArrayOf<Char32> > charArray = string.GetChars();
    Char32 sign = 0, next;
    Int32 commentLevel = 0;
    Int32 offset = 0, length = charArray->GetLength(), state = 0;
    Int32 year = -1, month = -1, date = -1;
    Int32 hour = -1, minute = -1, second = -1, zoneOffset = 0, minutesOffset = 0;
    Boolean zone = FALSE;
    const Int32 PAD = 0, LETTERS = 1, NUMBERS = 2;
    StringBuilder buffer;
    const String markers(",+-:/");

    while (offset <= length) {
        next = offset < length ? (*charArray)[offset] : '\r';
        offset++;

        if (next == '(') {
            commentLevel++;
        }
        if (commentLevel > 0) {
            if (next == ')') {
                commentLevel--;
            }
            if (commentLevel == 0) {
                next = ' ';
            }
            else {
                continue;
            }
        }

        Int32 nextState = PAD;
        if (('a' <= next && next <= 'z') || ('A' <= next && next <= 'Z')) {
            nextState = LETTERS;
        }
        else if ('0' <= next && next <= '9') {
            nextState = NUMBERS;
        }
        else if (!Character::IsSpace(next) && markers.IndexOf(next) == -1) {
            return E_ILLEGAL_ARGUMENT_EXCEPTION;
        }

        if (state == NUMBERS && nextState != NUMBERS) {
            String intString = buffer.ToString();
            Int32 digit = StringUtils::ParseInt32(intString);
            buffer.Reset();
            if (sign == '+' || sign == '-') {
                if (zoneOffset == 0) {
                    zone = TRUE;
                    if (next == ':') {
                        Int32 value1 = StringUtils::ParseInt32(
                            string.Substring(offset, offset + 2));
                        minutesOffset = (sign == '-' ? -value1 : value1);
                        offset += 2;
                    }
                    zoneOffset = sign == '-' ? -digit : digit;
                    sign = 0;
                }
                else {
                    return E_ILLEGAL_ARGUMENT_EXCEPTION;
                }
            }
            else if (digit >= 70) {
                if (year == -1
                        && (Character::IsSpace(next) || next == ','
                                || next == '/' || next == '\r')) {
                    year = digit;
                }
                else {
                    return E_ILLEGAL_ARGUMENT_EXCEPTION;
                }
            }
            else if (next == ':') {
                if (hour == -1) {
                    hour = digit;
                }
                else if (minute == -1) {
                    minute = digit;
                }
                else {
                    return E_ILLEGAL_ARGUMENT_EXCEPTION;
                }
            }
            else if (next == '/') {
                if (month == -1) {
                    month = digit - 1;
                }
                else if (date == -1) {
                    date = digit;
                }
                else {
                    return E_ILLEGAL_ARGUMENT_EXCEPTION;
                }
            }
            else if (Character::IsSpace(next) || next == ','
                    || next == '-' || next == '\r') {
                if (hour != -1 && minute == -1) {
                    minute = digit;
                }
                else if (minute != -1 && second == -1) {
                    second = digit;
                }
                else if (date == -1) {
                    date = digit;
                }
                else if (year == -1) {
                    year = digit;
                }
                else {
                    return E_ILLEGAL_ARGUMENT_EXCEPTION;
                }
            }
            else if (year == -1 && month != -1 && date != -1) {
                year = digit;
            }
            else {
                return E_ILLEGAL_ARGUMENT_EXCEPTION;
            }
        }
        else if (state == LETTERS && nextState != LETTERS) {
            //String text = buffer.ToString().toUpperCase(Locale.US);
            buffer.ToASCIIUpperCase();
            String text = buffer.ToString();

            buffer.Reset();
            if (text.GetLength() == 1) {
                return E_ILLEGAL_ARGUMENT_EXCEPTION;
            }
            if (text == "AM") {
                if (hour == 12) {
                    hour = 0;
                } else if (hour < 1 || hour > 12) {
                    return E_ILLEGAL_ARGUMENT_EXCEPTION;
                }
            }
            else if (text == "PM") {
                if (hour == 12) {
                    hour = 0;
                } else if (hour < 1 || hour > 12) {
                    return E_ILLEGAL_ARGUMENT_EXCEPTION;
                }
                hour += 12;
            }
            else {
                AutoPtr<ILocale> locus;
                AutoPtr<ILocaleHelper> lochep;
                CLocaleHelper::AcquireSingleton((ILocaleHelper**)&lochep);
                lochep->GetUS((ILocale**)&locus);
                AutoPtr<IDateFormatSymbols> symbols;
                CDateFormatSymbols::New(locus, (IDateFormatSymbols**)&symbols);
                AutoPtr<ArrayOf<String> > weekdays, months;
                symbols->GetWeekdays((ArrayOf<String>**)&weekdays);
                symbols->GetMonths((ArrayOf<String>**)&months);
                Int32 value2 = 0;
                if (Parse(text, *weekdays) != -1) {
                    /* empty */
                }
                else if (month == -1 && (month = Parse(text, *months)) != -1) {
                    /* empty */
                }
                else if (text == "GMT" || text == "UT" || text == "UTC") {
                    zone = TRUE;
                    zoneOffset = 0;
                }
                else if ((value2 = Zone(text)) != 0) {
                    zone = TRUE;
                    zoneOffset = value2;
                }
                else {
                    return E_ILLEGAL_ARGUMENT_EXCEPTION;
                }
            }
        }

        if (next == '+' || (year != -1 && next == '-')) {
            sign = next;
        }
        else if (!Character::IsSpace(next) && next != ','
                && nextState != NUMBERS) {
            sign = 0;
        }

        if (nextState == LETTERS || nextState == NUMBERS) {
            buffer.AppendChar(next);
        }
        state = nextState;
    }

    if (year != -1 && month != -1 && date != -1) {
        if (hour == -1) {
            hour = 0;
        }
        if (minute == -1) {
            minute = 0;
        }
        if (second == -1) {
            second = 0;
        }
        if (year < (GetCreationYear() - 80)) {
            year += 2000;
        }
        else if (year < 100) {
            year += 1900;
        }
        minute -= minutesOffset;
        if (zone) {
            if (zoneOffset >= 24 || zoneOffset <= -24) {
                hour -= zoneOffset / 100;
                minute -= zoneOffset % 100;
            }
            else {
                hour -= zoneOffset;
            }
            *value = UTC(year - 1900, month, date, hour, minute, second);
            return NOERROR;
        }

        AutoPtr<IDate> dateObj;
        FAIL_RETURN(CDate::New(year - 1900, month, date, hour, minute, second, (IDate**)&dateObj));
        return dateObj->GetTime(value);
    }

    return E_ILLEGAL_ARGUMENT_EXCEPTION;
}

ECode CDate::SetDate(
    /* [in] */ Int32 day)
{
    AutoPtr<CGregorianCalendar> calendar;
    FAIL_RETURN(CGregorianCalendar::NewByFriend(mMilliseconds, (CGregorianCalendar**)&calendar));
    FAIL_RETURN(calendar->Set(ICalendar::DATE, day));
    return calendar->GetTimeInMillis(&mMilliseconds);
}

ECode CDate::SetHours(
    /* [in] */ Int32 hour)
{
    AutoPtr<CGregorianCalendar> calendar;
    FAIL_RETURN(CGregorianCalendar::NewByFriend(mMilliseconds, (CGregorianCalendar**)&calendar));
    FAIL_RETURN(calendar->Set(ICalendar::HOUR_OF_DAY, hour));
    return calendar->GetTimeInMillis(&mMilliseconds);
}

ECode CDate::SetMinutes(
    /* [in] */ Int32 minute)
{
    AutoPtr<CGregorianCalendar> calendar;
    FAIL_RETURN(CGregorianCalendar::NewByFriend(mMilliseconds, (CGregorianCalendar**)&calendar));
    FAIL_RETURN(calendar->Set(ICalendar::MINUTE, minute));
    return calendar->GetTimeInMillis(&mMilliseconds);
}

ECode CDate::SetMonth(
    /* [in] */ Int32 month)
{
    AutoPtr<CGregorianCalendar> calendar;
    FAIL_RETURN(CGregorianCalendar::NewByFriend(mMilliseconds, (CGregorianCalendar**)&calendar));
    FAIL_RETURN(calendar->Set(ICalendar::MONTH, month));
    return calendar->GetTimeInMillis(&mMilliseconds);
}

ECode CDate::SetSeconds(
    /* [in] */ Int32 second)
{
    AutoPtr<CGregorianCalendar> calendar;
    FAIL_RETURN(CGregorianCalendar::NewByFriend(mMilliseconds, (CGregorianCalendar**)&calendar));
    FAIL_RETURN(calendar->Set(ICalendar::SECOND, second));
    return calendar->GetTimeInMillis(&mMilliseconds);
}

ECode CDate::SetTime(
    /* [in] */ Int64 milliseconds)
{
    mMilliseconds = milliseconds;
    return NOERROR;
}

ECode CDate::SetYear(
    /* [in] */ Int32 year)
{
    AutoPtr<CGregorianCalendar> calendar;
    FAIL_RETURN(CGregorianCalendar::NewByFriend(mMilliseconds, (CGregorianCalendar**)&calendar));
    FAIL_RETURN(calendar->Set(ICalendar::YEAR, year + 1900));
    return calendar->GetTimeInMillis(&mMilliseconds);
}

ECode CDate::ToGMTString(
    /* [out] */ String* gmtStr)
{
    VALIDATE_NOT_NULL(gmtStr)

    AutoPtr<ILocaleHelper> lochep;
    CLocaleHelper::AcquireSingleton((ILocaleHelper**)&lochep);
    AutoPtr<ILocale> loc;
    lochep->GetUS((ILocale**)&loc);
    AutoPtr<ISimpleDateFormat> sdf;
    FAIL_RETURN(CSimpleDateFormat::New(String("d MMM y HH:mm:ss 'GMT'"), loc, (ISimpleDateFormat**)&sdf));
    AutoPtr<ITimeZoneHelper> tzh;
    CTimeZoneHelper::AcquireSingleton((ITimeZoneHelper**)&tzh);
    AutoPtr<ITimeZone> gmtZone;
    tzh->GetTimeZone(String("GMT"), (ITimeZone**)&gmtZone);
    sdf->SetTimeZone(gmtZone);
    AutoPtr<IGregorianCalendar> gc;
    FAIL_RETURN(CGregorianCalendar::New(gmtZone, (IGregorianCalendar**)&gc));
    gc->SetTimeInMillis(mMilliseconds);
    sdf->FormatDate((IDate*)this, gmtStr);

    return NOERROR;
}

ECode CDate::ToLocaleString(
    /* [out] */ String* localeStr)
{
    VALIDATE_NOT_NULL(localeStr)

    AutoPtr<IDateFormatHelper> helper;
    CDateFormatHelper::AcquireSingleton((IDateFormatHelper**)&helper);
    AutoPtr<IDateFormat> format;
    helper->GetDateTimeInstance((IDateFormat**)&format);
    format->FormatDate((IDate*)this, localeStr);

    return NOERROR;
}

ECode CDate::ToString(
    /* [out] */ String* str)
{
    // TODO: equivalent to the following one-liner, though that's slower on stingray
    // at 476us versus 69us...
    //   return new SimpleDateFormat("EEE MMM dd HH:mm:ss zzz yyyy").format(d);
    AutoPtr<ILocaleData> localeData;
    AutoPtr<ILocale> usloc;
    AutoPtr<ILocaleHelper> lochep;
    CLocaleHelper::AcquireSingleton((ILocaleHelper **)&lochep);
    lochep->GetUS((ILocale **)&usloc);
    AutoPtr<ILocaleDataHelper> ldh;
    CLocaleDataHelper::AcquireSingleton((ILocaleDataHelper **)&ldh);
    ldh->Get(usloc , (ILocaleData **)&localeData);
    AutoPtr<IGregorianCalendar> cal;
    CGregorianCalendar::New(mMilliseconds, (IGregorianCalendar**)&cal);
    AutoPtr<ITimeZone> tz;
    cal->GetTimeZone((ITimeZone**)&tz);
    Int32 zonevalue = 0;
    tz->GetRawOffset(&zonevalue);

    Int32 temp;
    StringBuilder result;
    cal->Get(ICalendar::DAY_OF_WEEK, &temp);
    AutoPtr<ArrayOf<String> > swn;
    localeData->GetShortWeekdayNames((ArrayOf<String> **)&swn);
    result.AppendString((*swn)[temp]);
    result.AppendChar(' ');
    localeData->GetShortMonthNames((ArrayOf<String> **)&swn);
    cal->Get(ICalendar::MONTH , &temp);
    result.AppendString((*swn)[temp]);
    result.AppendChar(' ');
    cal->Get(ICalendar::DAY_OF_MONTH, &temp);
    AppendTwoDigits(result, temp);
    result.AppendChar(' ');
    cal->Get(ICalendar::HOUR_OF_DAY, &temp);
    AppendTwoDigits(result, temp);
    result.AppendChar(':');
    cal->Get(ICalendar::MINUTE, &temp);
    AppendTwoDigits(result, temp);
    result.AppendChar(':');
    cal->Get(ICalendar::SECOND, &temp);
    AppendTwoDigits(result, temp);
    result.AppendChar(' ');
    String displayName;
    Boolean isIn;
    tz->InDaylightTime((IDate*)this, &isIn);
    tz->GetDisplayName(isIn, ITimeZone::SHORT, &displayName);
    result.AppendString(displayName);
    result.AppendChar(' ');
    cal->Get(ICalendar::YEAR, &temp);
    result.AppendInt32(temp);
    return result.ToString(str);
}

void CDate::AppendTwoDigits(StringBuilder& sb, Int32 n)
{
    if (n < 10) {
        sb.AppendChar('0');
    }
    sb.AppendInt32(n);
}

Int64 CDate::UTC(
    /* [in] */ Int32 year,
    /* [in] */ Int32 month,
    /* [in] */ Int32 day,
    /* [in] */ Int32 hour,
    /* [in] */ Int32 minute,
    /* [in] */ Int32 second)
{
    AutoPtr<IGregorianCalendar> cal;
    CGregorianCalendar::New(FALSE, (IGregorianCalendar**)&cal);
    AutoPtr<ITimeZone> timezone;
    AutoPtr<ITimeZoneHelper> timezoneHelper;
    CTimeZoneHelper::AcquireSingleton((ITimeZoneHelper**)&timezoneHelper);
    timezoneHelper->GetTimeZone(String("GMT"), (ITimeZone**)&timezone);
    cal->SetTimeZone(timezone);
    cal->Set(1900 + year, month, day, hour, minute, second);
    Int64 time;
    cal->GetTimeInMillis(&time);
    return time;
}

Int32 CDate::Zone(const String& text)
{
    if (text == ("EST")) {
        return -5;
    }
    if (text == ("EDT")) {
        return -4;
    }
    if (text == ("CST")) {
        return -6;
    }
    if (text == ("CDT")) {
        return -5;
    }
    if (text == ("MST")) {
        return -7;
    }
    if (text == ("MDT")) {
        return -6;
    }
    if (text == ("PST")) {
        return -8;
    }
    if (text == ("PDT")) {
        return -7;
    }
    return 0;
}

} // namespace Utility
} // namespace Elastos
