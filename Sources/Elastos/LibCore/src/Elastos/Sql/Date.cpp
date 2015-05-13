
#include "Date.h"
#include <elastos/CSystem.h>
#include <elastos/Character.h>
#include <elastos/StringUtils.h>
#include "CDate.h"
#include "CGregorianCalendar.h"
#include "CTimeZoneHelper.h"

using Elastos::Core::StringUtils;
using Elastos::Core::ISystem;
using Elastos::Core::CSystem;
using Elastos::Core::Character;
using Elastos::Core::StringBuilder;
using Elastos::Utility::CDate;
using Elastos::Utility::CGregorianCalendar;
using Elastos::Utility::IGregorianCalendar;
using Elastos::Utility::ICalendar;
using Elastos::Utility::ITimeZone;
using Elastos::Utility::ITimeZoneHelper;
using Elastos::Utility::CTimeZoneHelper;

namespace Elastos{
namespace Sql {

Int32 Date::sCreationYear = -1;

Int32 Date::GetCreationYear()
{
    if (sCreationYear < 0) {
        Date d;
        sCreationYear = d.GetYear();
    }
    return sCreationYear;
}

Date::Date()
    : mMilliseconds(0)
{
    AutoPtr<ISystem> system;
    Elastos::Core::CSystem::AcquireSingleton((ISystem**)&system);
    system->GetCurrentTimeMillis(&mMilliseconds);
}

Date::Date(
    /* [in] */ Int32 year,
    /* [in] */ Int32 month,
    /* [in] */ Int32 day)
{
    Init(year, month ,day);
}
void Date::Init(
    /* [in] */ Int32 year,
    /* [in] */ Int32 month,
    /* [in] */ Int32 day)
{
    AutoPtr<IGregorianCalendar> cal;
    CGregorianCalendar::New(FALSE, (IGregorianCalendar**)&cal);
    cal->SetEx(1900 + year, month, day);
    cal->GetTimeInMillis(&mMilliseconds);
}

Date::Date(
    /* [in] */ Int32 year,
    /* [in] */ Int32 month,
    /* [in] */ Int32 day,
    /* [in] */ Int32 hour,
    /* [in] */ Int32 minute)
{
    AutoPtr<IGregorianCalendar> cal;
    CGregorianCalendar::New(FALSE, (IGregorianCalendar**)&cal);

    cal->SetEx2(1900 + year, month, day, hour, minute);
    cal->GetTimeInMillis(&mMilliseconds);
}

Date::Date(
    /* [in] */ Int32 year,
    /* [in] */ Int32 month,
    /* [in] */ Int32 day,
    /* [in] */ Int32 hour,
    /* [in] */ Int32 minute,
    /* [in] */ Int32 second)
{
    InitEx2(year,month,day,hour,minute,second);
}

void Date::InitEx2(
    /* [in] */ Int32 year,
    /* [in] */ Int32 month,
    /* [in] */ Int32 day,
    /* [in] */ Int32 hour,
    /* [in] */ Int32 minute,
    /* [in] */ Int32 second)
{
    AutoPtr<IGregorianCalendar> cal;
    CGregorianCalendar::New(FALSE, (IGregorianCalendar**)&cal);
    cal->SetEx3(1900 + year, month, day, hour, minute, second);
    cal->GetTimeInMillis(&mMilliseconds);
}

Date::Date(
    /* [in] */ Int64 milliseconds)
{
    InitEx(milliseconds);
}

void Date::InitEx(
    /* [in] */ Int64 milliseconds)
{
    mMilliseconds = milliseconds;
}

Date::Date(
    /* [in] */ const String& string)
{
    Parse(string, &mMilliseconds);
}

Boolean Date::IsAfter(
    /* [in] */ Elastos::Utility::IDate* date)
{
    Int64 otherTime = 0;
    date->GetTime(&otherTime);
    return mMilliseconds > otherTime;
}

Boolean Date::IsBefore(
    /* [in] */ Elastos::Utility::IDate* date)
{
    Int64 otherTime = 0;
    date->GetTime(&otherTime);
    return mMilliseconds < otherTime;
}

AutoPtr<Elastos::Utility::IDate> Date::Clone()
{
    AutoPtr<Elastos::Utility::IDate> date;
    CDate::New((Elastos::Utility::IDate **)&date);
    date->SetTime(mMilliseconds);
    return date;
}

Int32 Date::CompareTo(
    /* [in] */ Elastos::Utility::IDate* date)
{
    Int64 otherTime = 0;
    date->GetTime(&otherTime);
    if (mMilliseconds < otherTime) {
        return -1;
    }
    if (mMilliseconds == otherTime) {
        return 0;
    }
    return 1;
}

Boolean Date::EqualsEx(
    /* [in] */ Elastos::Utility::IDate* date)
{
    if (date == NULL) {
        return FALSE;
    }

    Int64 otherTime = 0;
    date->GetTime(&otherTime);
    return (otherTime == mMilliseconds);
}

Boolean Date::Equals(
    /* [in] */ IInterface* date)
{
    return EqualsEx(IDate::Probe(date));
}

Int32 Date::GetDate()
{
    Int32 date = 0;
    AutoPtr<IGregorianCalendar> calendar;
    CGregorianCalendar::New(mMilliseconds, (IGregorianCalendar**)&calendar);
    calendar->Get(ICalendar::DATE, &date);
    return date;
}

Int32 Date::GetDay()
{
    Int32 day = 0;
    AutoPtr<IGregorianCalendar> calendar;
    CGregorianCalendar::New(mMilliseconds, (IGregorianCalendar**)&calendar);
    calendar->Get(ICalendar::DAY_OF_WEEK, &day);
    day = day - 1;
    return day;
}

Int32 Date::GetHours()
{
    Int32 hours = 0;
    AutoPtr<IGregorianCalendar> calendar;
    CGregorianCalendar::New(mMilliseconds, (IGregorianCalendar**)&calendar);
    calendar->Get(ICalendar::HOUR_OF_DAY, &hours);
    return hours;
}

Int32 Date::GetMinutes()
{
    Int32 minutes = 0;
    AutoPtr<IGregorianCalendar> calendar;
    CGregorianCalendar::New(mMilliseconds, (IGregorianCalendar**)&calendar);
    calendar->Get(ICalendar::MINUTE, &minutes);
    return minutes;
}

Int32 Date::GetMonth()
{
    Int32 month = 0;

    AutoPtr<IGregorianCalendar> calendar;
    CGregorianCalendar::New(mMilliseconds, (IGregorianCalendar**)&calendar);
    calendar->Get(ICalendar::MONTH, &month);
    return month;
}

Int32 Date::GetSeconds()
{
    Int32 seconds = 0;
    AutoPtr<IGregorianCalendar> calendar;
    CGregorianCalendar::New(mMilliseconds, (IGregorianCalendar**)&calendar);
    calendar->Get(ICalendar::SECOND, &seconds);
    return seconds;
}

Int64 Date::GetTime()
{
    Int64 time = mMilliseconds;
    return time;
}

Int32 Date::GetTimezoneOffset()
{
    Int32 timezoneOffset = 0;
    AutoPtr<IGregorianCalendar> calendar;
    CGregorianCalendar::New(mMilliseconds, (IGregorianCalendar**)&calendar);
    Int32 zoneOff = 0, dstOff = 0;
    calendar->Get(ICalendar::ZONE_OFFSET, &zoneOff);
    calendar->Get(ICalendar::DST_OFFSET, &dstOff);
    timezoneOffset -= (zoneOff + dstOff) / 60000;
    return timezoneOffset;
}

Int32 Date::GetYear()
{
    Int32 year = 0;
    AutoPtr<CGregorianCalendar> calendar;
    CGregorianCalendar::NewByFriend(mMilliseconds, (CGregorianCalendar**)&calendar);
    calendar->Get(ICalendar::YEAR, &year);
    year = year - 1900;
    return year;
}

ECode Date::SetDate(
    /* [in] */ Int32 day)
{
    AutoPtr<IGregorianCalendar> calendar;
    FAIL_RETURN(CGregorianCalendar::New(mMilliseconds, (IGregorianCalendar**)&calendar));
    FAIL_RETURN(calendar->Set(ICalendar::DATE, day));
    return calendar->GetTimeInMillis(&mMilliseconds);
}

ECode Date::SetHours(
    /* [in] */ Int32 hour)
{
    AutoPtr<IGregorianCalendar> calendar;
    FAIL_RETURN(CGregorianCalendar::New(mMilliseconds, (IGregorianCalendar**)&calendar));
    FAIL_RETURN(calendar->Set(ICalendar::HOUR_OF_DAY, hour));
    return calendar->GetTimeInMillis(&mMilliseconds);
}

ECode Date::SetMinutes(
    /* [in] */ Int32 minute)
{
    AutoPtr<IGregorianCalendar> calendar;
    FAIL_RETURN(CGregorianCalendar::New(mMilliseconds, (IGregorianCalendar**)&calendar));
    FAIL_RETURN(calendar->Set(ICalendar::MINUTE, minute));
    return calendar->GetTimeInMillis(&mMilliseconds);
}

ECode Date::SetMonth(
    /* [in] */ Int32 month)
{
    AutoPtr<IGregorianCalendar> calendar;
    FAIL_RETURN(CGregorianCalendar::New(mMilliseconds, (IGregorianCalendar**)&calendar));
    FAIL_RETURN(calendar->Set(ICalendar::MONTH, month));
    return calendar->GetTimeInMillis(&mMilliseconds);
}

ECode Date::SetSeconds(
    /* [in] */ Int32 second)
{
    AutoPtr<IGregorianCalendar> calendar;
    FAIL_RETURN(CGregorianCalendar::New(mMilliseconds, (IGregorianCalendar**)&calendar));
    FAIL_RETURN(calendar->Set(ICalendar::SECOND, second));
    return calendar->GetTimeInMillis(&mMilliseconds);
}

ECode Date::SetTime(
    /* [in] */ Int64 milliseconds)
{
    mMilliseconds = milliseconds;
    return NOERROR;
}

ECode Date::SetYear(
    /* [in] */ Int32 year)
{
    AutoPtr<IGregorianCalendar> calendar;
    FAIL_RETURN(CGregorianCalendar::New(mMilliseconds, (IGregorianCalendar**)&calendar));
    FAIL_RETURN(calendar->Set(ICalendar::YEAR, year + 1900));
    return calendar->GetTimeInMillis(&mMilliseconds);
}

String Date::ToGMTString()
{
    //TODO
//    SimpleDateFormat sdf = new SimpleDateFormat("d MMM y HH:mm:ss 'GMT'", Locale.US);
//    TimeZone gmtZone = TimeZone.getTimeZone("GMT");
//    sdf.setTimeZone(gmtZone);
//    GregorianCalendar gc = new GregorianCalendar(gmtZone);
//    gc.setTimeInMillis(milliseconds);
//    return sdf.format(this);
    return String(NULL);
}

String Date::ToLocaleString()
{
    // TODO
    //return DateFormat.getDateTimeInstance().format(this);
    return String(NULL);
}

void Date::AppendTwoDigits(StringBuilder& sb, Int32 n)
{
    if (n < 10) {
        sb.AppendChar('0');
    }
    sb.AppendInt32(n);
}

String Date::ToString()
{
    // TODO: equivalent to the following one-liner, though that's slower on stingray
    // at 476us versus 69us...
    //   return new SimpleDateFormat("EEE MMM dd HH:mm:ss zzz yyyy").format(d);
//    LocaleData localeData = LocaleData.Get(Locale.US);
    AutoPtr<IGregorianCalendar> cal;
    CGregorianCalendar::New(mMilliseconds, (IGregorianCalendar**)&cal);
    AutoPtr<ITimeZone> tz;
    cal->GetTimeZone((ITimeZone**)&tz);

    Int32 temp;
    StringBuilder result;
    cal->Get(ICalendar::DAY_OF_WEEK, &temp);
//    result.Append(localeData.shortWeekdayNames[temp]);
//    result.AppendChar(' ');
//    cal->Get(ICalendar::MONTH, &temp)
//    result.Append(localeData.shortMonthNames[temp]);
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
    tz->InDaylightTime((Elastos::Utility::IDate*)this, &isIn);
    tz->GetDisplayNameEx2(isIn, ITimeZone::SHORT, &displayName);
    result.AppendString(displayName);
    result.AppendChar(' ');
    cal->Get(ICalendar::YEAR, &temp);
    result.AppendInt32(temp);
    String str = String(NULL);
    result.ToString(&str);
    return str;
}

Int64 Date::UTC(
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
    cal->SetEx3(1900 + year, month, day, hour, minute, second);
    Int64 time;
    cal->GetTimeInMillis(&time);
    return time;
}

Int32 Date::Zone(const String& text)
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

Int32 Date::Parse(
    /* [in] */ const String& string,
    /* [in] */ ArrayOf<String> const& array)
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

ECode Date::Parse(
    /* [in] */ const String& string,
    /* [out] */ Int64* value)
{
    if (string.IsNull()) {
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }

    char sign = 0;
    Int32 commentLevel = 0;
    AutoPtr<ArrayOf<Char32> > charArray = string.GetChars();
    Int32 offset = 0, length = charArray->GetLength(), state = 0;
    Int32 year = -1, month = -1, date = -1;
    Int32 hour = -1, minute = -1, second = -1, zoneOffset = 0, minutesOffset = 0;
    Boolean zone = FALSE;
    const Int32 PAD = 0, LETTERS = 1, NUMBERS = 2;
    StringBuilder buffer;
    const String markers(",+-:/");
    Char32 next;
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
            Int32 digit = StringUtils::ParseInt32(intString);//Integer.parseInt(buffer.toString());
            buffer.Reset();
            if (sign == '+' || sign == '-') {
                if (zoneOffset == 0) {
                    zone = TRUE;
                    if (next == ':') {
                        Int32 value = StringUtils::ParseInt32(string.Substring(offset, offset + 2));
                        minutesOffset = (sign == '-' ? -value : value);
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
                // TODO
//                DateFormatSymbols symbols = new DateFormatSymbols(Locale.US);
//                String[] weekdays = symbols.getWeekdays(), months = symbols
//                        .getMonths();
                AutoPtr<ArrayOf<String> > weekdays, months;
                Int32 value;
                if (Parse(text, *weekdays) != -1) {
                    /* empty */
                }
                else if (month == -1 && (month = Parse(text, *months)) != -1) {/* empty */
                }
                else if (text == "GMT" || text == "UT" || text == "UTC") {
                    zone = TRUE;
                    zoneOffset = 0;
                }
                else if ((value = Zone(text)) != 0) {
                    zone = TRUE;
                    zoneOffset = value;
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
        }

        AutoPtr<Elastos::Utility::IDate> dateObj;
        CDate::New(year - 1900, month, date, hour, minute, second, (Elastos::Utility::IDate**)&dateObj);
        return dateObj->GetTime(value);
    }

    return E_NOT_IMPLEMENTED;
}

} // namespace Sql
} // namespace Elastos
