
#include "DateIntervalFormat.h"
#include "BasicLruCache.h"
#include "TimeZone.h"
#include "CLocale.h"
#include "CCalendarHelper.h"
#include "AutoLock.h"
#include <unicode/utypes.h>
#include <unicode/uobject.h>
#include <unicode/locid.h>
#include <unicode/uniset.h>
#include <unicode/dtitvfmt.h>
#include <ElStringByteSink.h>

using Libcore::Utility::BasicLruCache;
using Elastos::Utility::CLocale;
using Elastos::Utility::ICalendarHelper;
using Elastos::Utility::CCalendarHelper;

namespace Libcore {
namespace ICU {

const Int32 DateIntervalFormat::DAY_IN_MS = 24 * 60 * 60 * 1000;
const Int32 DateIntervalFormat::EPOCH_JULIAN_DAY = 2440588;
const AutoPtr<DateIntervalFormat::FormatterCache> DateIntervalFormat::CACHED_FORMATTERS = new DateIntervalFormat::FormatterCache;

void DateIntervalFormat::FormatterCache::EntryEvicted(
    const String& key,
    Int64 value)
{
    // TODO: Wait for class BasicLruCache complete
#if 0 // for compiling
    DestoryDateIntervalFormat(value);
#else
    assert(0);
#endif
}

String DateIntervalFormat::FormatDateRange(
    Int64 startMs,
    Int64 endMs,
    Int32 flags,
    String& olsonId)
{
    if ((flags & IDateIntervalFormat::FORMAT_UTC) != 0) {
        olsonId = "UTC";
    }
    AutoPtr<ITimeZone> tz;
    if (!olsonId.IsNull()) {
        Elastos::Utility::TimeZone::GetTimeZone(olsonId, (ITimeZone**)&tz);
    } else {
        tz = Elastos::Utility::TimeZone::GetDefault();
    }
    return FormatDateRange(CLocale::GetDefault(), tz, startMs, endMs, flags);
}

String DateIntervalFormat::FormatDateRange(ILocale* locale, ITimeZone* tz, Int64 startMs, Int64 endMs, Int32 flags)
{
    AutoPtr<ICalendarHelper> calendarHelper = NULL;
    CCalendarHelper::AcquireSingleton((ICalendarHelper**)&calendarHelper);
    AutoPtr<ICalendar> startCallendar = NULL;
    calendarHelper->GetInstance(tz, (ICalendar**)&startCallendar);
    startCallendar->SetTimeInMillis(startMs);

    AutoPtr<ICalendar> endCallendar = NULL;
    if (startMs == endMs) {
        endCallendar = startCallendar;
    } else {
        calendarHelper->GetInstance(tz, (ICalendar**)&endCallendar);
        endCallendar->SetTimeInMillis(endMs);
    }

    Boolean endsAtMidnight = IsMidnight(endCallendar);

    // If we're not showing the time or the start and end times are on the same day, and the
    // end time is midnight, fudge the end date so we don't count the day that's about to start.
    // This is not the behavior of icu4c's DateIntervalFormat, but it's the historical behavior
    // of Android's DateUtils.formatDateRange.
    if (startMs != endMs && endsAtMidnight &&
        (((flags | IDateIntervalFormat::FORMAT_SHOW_TIME) == 0) || (DayDistance(startCallendar, endCallendar) <= 1))) {
        endCallendar->Roll(ICalendar::DAY_OF_MONTH, false);
        endMs -= DAY_IN_MS;
    }

    String skeleton = ToSkeleton(startCallendar, endCallendar, flags);
    String rev;
    synchronized(CACHED_FORMATTERS) {
        String s;
        locale->ToString(&s);
        String id;
        tz->GetID(&id);
        rev = FormatDateInterval(GetFormatter(skeleton, s, id), startMs, endMs);
    }
    return rev;
}

Int64 DateIntervalFormat::GetFormatter(
    const String& skeleton,
    const String& localeName,
    const String& tzName)
{
    String key = skeleton + "\t" + localeName + "\t" + tzName;
    Int64 formatter = 0;
    // TODO: Waiting for class BasicLruCache
#if 0 // for compiling
    CACHED_FORMATTERS->Get(&key, &formatter);
#else
    assert(0);
#endif
    if (formatter != 0) {
        return formatter;
    }
    Int64 address = CreateDateIntervalFormat(skeleton, localeName, tzName);
    return address;
}

String DateIntervalFormat::ToSkeleton(
    ICalendar* startCalendar,
    ICalendar* endCalendar,
    Int32 flags)
{
    if ((flags & IDateIntervalFormat::FORMAT_ABBREV_ALL) != 0) {
        flags |= IDateIntervalFormat::FORMAT_ABBREV_MONTH | IDateIntervalFormat::FORMAT_ABBREV_TIME | IDateIntervalFormat::FORMAT_ABBREV_WEEKDAY;
    }

    String monthPart("MMMM");
    if ((flags & IDateIntervalFormat::FORMAT_NUMERIC_DATE) != 0) {
        monthPart = "M";
    } else if ((flags & IDateIntervalFormat::FORMAT_ABBREV_MONTH) != 0) {
        monthPart = "MMM";
    }

    String weekPart("EEEE");
    if ((flags & IDateIntervalFormat::FORMAT_ABBREV_WEEKDAY) != 0) {
        weekPart = "EEE";
    }

    String timePart("j"); // "j" means choose 12 or 24 hour based on current locale.
    if ((flags & IDateIntervalFormat::FORMAT_24HOUR) != 0) {
        timePart = "H";
    } else if ((flags & IDateIntervalFormat::FORMAT_12HOUR) != 0) {
        timePart = "h";
    }

    // If we've not been asked to abbreviate times, or we're using the 24-hour clock (where it
    // never makes sense to leave out the minutes), include minutes. This gets us times like
    // "4 PM" while avoiding times like "16" (for "16:00").
    if ((flags & IDateIntervalFormat::FORMAT_ABBREV_TIME) == 0 || (flags & IDateIntervalFormat::FORMAT_24HOUR) != 0) {
        timePart += "m";
    } else {
        // Otherwise, we're abbreviating a 12-hour time, and should only show the minutes
        // if they're not both "00".
        if (!(OnTheHour(startCalendar) && OnTheHour(endCalendar))) {
            timePart = timePart + "m";
        }
    }

    if (FallOnDifferentDates(startCalendar, endCalendar)) {
        flags |= IDateIntervalFormat::FORMAT_SHOW_DATE;
    }

    if (FallInSameMonth(startCalendar, endCalendar) && (flags & IDateIntervalFormat::FORMAT_NO_MONTH_DAY) != 0) {
        flags &= (~IDateIntervalFormat::FORMAT_SHOW_WEEKDAY);
        flags &= (~IDateIntervalFormat::FORMAT_SHOW_TIME);
    }

    if ((flags & (IDateIntervalFormat::FORMAT_SHOW_DATE | IDateIntervalFormat::FORMAT_SHOW_TIME | IDateIntervalFormat::FORMAT_SHOW_WEEKDAY)) == 0) {
        flags |= IDateIntervalFormat::FORMAT_SHOW_DATE;
    }

    // If we've been asked to show the date, work out whether we think we should show the year.
    if ((flags & IDateIntervalFormat::FORMAT_SHOW_DATE) != 0) {
        if ((flags & IDateIntervalFormat::FORMAT_SHOW_YEAR) != 0) {
            // The caller explicitly wants us to show the year.
        } else if ((flags & IDateIntervalFormat::FORMAT_NO_YEAR) != 0) {
            // The caller explicitly doesn't want us to show the year, even if we otherwise would.
        } else if (!FallInSameYear(startCalendar, endCalendar) || !IsThisYear(startCalendar)) {
            flags |= IDateIntervalFormat::FORMAT_SHOW_YEAR;
        }
    }

    AutoPtr<StringBuilder> builder = new StringBuilder();
    if ((flags & (IDateIntervalFormat::FORMAT_SHOW_DATE | IDateIntervalFormat::FORMAT_NO_MONTH_DAY)) != 0) {
        if ((flags & IDateIntervalFormat::FORMAT_SHOW_YEAR) != 0) {
            builder->Append("y");
        }
        builder->Append(monthPart);
        if ((flags & IDateIntervalFormat::FORMAT_NO_MONTH_DAY) == 0) {
            builder->Append("d");
        }
    }
    if ((flags & IDateIntervalFormat::FORMAT_SHOW_WEEKDAY) != 0) {
        builder->Append(weekPart);
    }
    if ((flags & IDateIntervalFormat::FORMAT_SHOW_TIME) != 0) {
        builder->Append(timePart);
    }
    String s;
    builder->ToString(&s);
    return s;
}

static inline Int32 getField(ICalendar* c, Int32 field)
{
    Int32 value;
    c->Get(field, &value);
    return value;
}

Boolean DateIntervalFormat::IsMidnight(
    ICalendar* c)
{
    return getField(c, ICalendar::HOUR_OF_DAY) ==0 &&
        getField(c, ICalendar::MINUTE) == 0 &&
        getField(c, ICalendar::SECOND) == 0 &&
        getField(c, ICalendar::MILLISECOND);
}

Boolean DateIntervalFormat::OnTheHour(
    ICalendar* c)
{
    return getField(c, ICalendar::MINUTE) == 0 && getField(c, ICalendar::SECOND) == 0;
}

Boolean DateIntervalFormat::FallOnDifferentDates(
    ICalendar* c1,
    ICalendar* c2)
{
    return getField(c1, ICalendar::YEAR) != getField(c2, ICalendar::YEAR) ||
        getField(c1, ICalendar::MONTH) != getField(c2, ICalendar::MONTH) ||
        getField(c1, ICalendar::DAY_OF_MONTH) != getField(c2, ICalendar::DAY_OF_MONTH);
}

Boolean DateIntervalFormat::FallInSameMonth(
    ICalendar* c1,
    ICalendar* c2)
{
    return getField(c1, ICalendar::MONTH) == getField(c2, ICalendar::MONTH);
}

Boolean DateIntervalFormat::FallInSameYear(
    ICalendar* c1,
    ICalendar* c2)
{
    return getField(c1, ICalendar::YEAR) == getField(c2, ICalendar::YEAR);
}

Boolean DateIntervalFormat::IsThisYear(
    ICalendar* c)
{
    AutoPtr<ITimeZone> timeZone;
    c->GetTimeZone((ITimeZone**)&timeZone);
    AutoPtr<ICalendar> now;
    AutoPtr<ICalendarHelper> calendarHelper;
    CCalendarHelper::AcquireSingleton((ICalendarHelper**)&calendarHelper);
    calendarHelper->GetInstance(timeZone, (ICalendar**)&now);
    return getField(c, ICalendar::YEAR) == getField(now, ICalendar::YEAR);
}

Int32 DateIntervalFormat::DayDistance(
    ICalendar* c1,
    ICalendar* c2)
{
    return JulianDay(c2) - JulianDay(c1);
}

Int32 DateIntervalFormat::JulianDay(
    ICalendar* c)
{
    Int64 utcMs;
    c->GetTimeInMillis(&utcMs);
    utcMs += getField(c, ICalendar::ZONE_OFFSET);
    utcMs += getField(c, ICalendar::DST_OFFSET);
    return (Int32) (utcMs / DateIntervalFormat::DAY_IN_MS) + DateIntervalFormat::EPOCH_JULIAN_DAY;
}

Int64 DateIntervalFormat::CreateDateIntervalFormat(
    const String& skeleton,
    const String& localeName,
    const String& tzName)
{
    NATIVE(Locale) icuLocale;
    icuLocale = NATIVE(Locale)::createFromName(localeName);
    if (icuLocale.isBogus()) {
        return 0;
    }

    if (skeleton.IsNull()) {
        return 0;
    }

    UErrorCode status = U_ZERO_ERROR;
      NATIVE(DateIntervalFormat)* formatter(NATIVE(DateIntervalFormat)::createInstance(UnicodeString::fromUTF8(skeleton.string()), icuLocale, status));
      if (!U_SUCCESS(status)) {
        return 0;
      }

      if (tzName.IsNull()) {
          return 0;
      }
    formatter->adoptTimeZone(NATIVE(TimeZone)::createTimeZone(UnicodeString::fromUTF8(tzName.string())));

      return reinterpret_cast<uintptr_t>(formatter);
}

void DateIntervalFormat::DestroyDateIntervalFormat(
    Int64 address)
{
    delete reinterpret_cast<NATIVE(DateIntervalFormat)*>(address);
}

String DateIntervalFormat::FormatDateInterval(
    Int64 address,
    Int64 fromDate,
    Int64 toDate)
{
    NATIVE(DateIntervalFormat)* formatter(reinterpret_cast<NATIVE(DateIntervalFormat)*>(address));
    DateInterval date_interval(fromDate, toDate);

    UnicodeString s;
    FieldPosition pos = 0;
    UErrorCode status = U_ZERO_ERROR;
    formatter->format(&date_interval, s, pos, status);
    if (!U_SUCCESS(status)) {
        return String(NULL);
    }

    String rev;
    ElStringByteSink sink(&rev);
    s.toUTF8(sink);
    return rev;
}

} // ICU
} // Libcore