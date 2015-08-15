
#include "DateUtils.h"
#include <elastos/Logger.h>

using Elastos::Core::CString;
using Elastos::Text::CSimpleDateFormat;
using Elastos::Utility::CTimeZoneHelper;
using Elastos::Utility::CCalendarHelper;
using Elastos::Utility::ICalendar;
using Elastos::Utility::ILocale
using Elastos::Utility::Logging::Logger;

namespace Org {
namespace Apache {
namespace Http {
namespace Impl {
namespace Cookie {

//==============================================================================
// DateUtils::DateFormatHolder
//==============================================================================

AutoPtr<ISimpleDateFormat> DateUtils::DateFormatHolder::FormatFor(
    /* [in] */ const String& pattern)
{
    // SoftReference<Map<String, SimpleDateFormat>> ref = THREADLOCAL_FORMATS.get();
    // Map<String, SimpleDateFormat> formats = ref.get();
    AutoPtr<IHashMap> formats;
    if (formats == NULL) {
        CHashMap::New((IHashMap**)&formats);
        // THREADLOCAL_FORMATS.set(
        //         new SoftReference<Map<String, SimpleDateFormat>>(formats));
    }

    AutoPtr<CString> cs;
    CString::NewByFriend(pattern, (CString**)&cs);
    AutoPtr<IInterface> value;
    formats->Get(cs, (IInterface**)&value);
    AutoPtr<ISimpleDateFormat> format = ISimpleDateFormat::Probe(value);
    if (format == NULL) {
        AutoPtr<CSimpleDateFormat> sdf;
        CSimpleDateFormat::NewByFriend(pattern, ILocale::US, (CSimpleDateFormat**)&sdf);
        format = (ISimpleDateFormat*)sdf.Get();
        AutoPtr<CTimeZoneHelper> helper;
        CTimeZoneHelper::AcquireSingletonByFriend((CTimeZoneHelper**)&helper);
        AutoPtr<ITimeZone> timeZone;
        helper->GetTimeZone(String("GMT"), (ITimeZone**)&timeZone);
        format->SetTimeZone(timeZone);
        formats->Put(cs, format);
    }

    return format;
}


//==============================================================================
// DateUtils
//==============================================================================

static const String DateUtils::PATTERN_RFC1123("EEE, dd MMM yyyy HH:mm:ss zzz");
static const String DateUtils::PATTERN_RFC1036("EEEE, dd-MMM-yy HH:mm:ss zzz");
static const String DateUtils::PATTERN_ASCTIME("EEE MMM d HH:mm:ss yyyy");

static AutoPtr<ITimeZone> InitGMT()
{
    AutoPtr<CTimeZoneHelper> helper;
    CTimeZoneHelper::AcquireSingletonByFriend((CTimeZoneHelper**)&helper);
    AutoPtr<ITimeZone> timeZone;
    helper->GetTimeZone(String("GMT"), (ITimeZone**)&timeZone);
    return timeZone;
}
static const AutoPtr<ITimeZone> DateUtils::GMT = InitGMT();

static AutoPtr< ArrayOf<String> > InitDefaultPatterns()
{
    AutoPtr< ArrayOf<String> > patterns = ArrayOf<String>::Alloc(3);
    (*patterns)[0] = DateUtils::PATTERN_RFC1036;
    (*patterns)[1] = DateUtils::PATTERN_RFC1123;
    (*patterns)[2] = DateUtils::PATTERN_ASCTIME;
}
static const AutoPtr< ArrayOf<String> > DateUtils::DEFAULT_PATTERNS = InitDefaultPatterns();

static AutoPtr<IDate> InitDefaultTwoDigitYearStart()
{
    AutoPtr<CCalendarHelper> helper;
    CCalendarHelper::AcquireSingletonByFriend((CCalendarHelper**)&helper);
    AutoPtr<ICalendar> calender;
    helper->GetInstance((ICalendar**)&calender);
    calendar->SetTimeZone(DateUtils::GMT);
    calendar->Set(2000, ICalendar::JANUARY, 1, 0, 0, 0);
    calendar->Set(ICalendar::MILLISECOND, 0);
    AutoPtr<IDate> date;
    calendar->GetTime((IDate**)&date);
    return date;
}
static const AutoPtr<IDate> DateUtils::DEFAULT_TWO_DIGIT_YEAR_START = InitDefaultTwoDigitYearStart();

ECode DateUtils::ParseDate(
    /* [in] */ const String& dateValue,
    /* [out] */ IDate** date)
{
    VALIDATE_NOT_NULL(date)
    return ParseDate(dateValue, NULL, NULL, date);
}

ECode DateUtils::ParseDate(
    /* [in] */ const String& dateValue,
    /* [in] */ ArrayOf<String>* dateFormats,
    /* [out] */ IDate** date)
{
    VALIDATE_NOT_NULL(date)
    return ParseDate(dateValue, dateFormats, NULL, date);
}

ECode DateUtils::parseDate(
    /* [in] */ const String& dateValue,
    /* [in] */ ArrayOf<String>* dateFormats,
    /* [in] */ IDate* startDate,
    /* [out] */ IDate** date)
{
    VALIDATE_NOT_NULL(date)
    *date = NULL;

    if (dateValue.IsNull()) {
        Logger::E("DateUtils", "dateValue is null");
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    if (dateFormats == NULL) {
        dateFormats = DEFAULT_PATTERNS;
    }
    if (startDate == NULL) {
        startDate = DEFAULT_TWO_DIGIT_YEAR_START;
    }
    // trim single quotes around date if present
    // see issue #5279
    if ((dateValue.GetLength() > 1) && dateValue.StartWith("'")
            && dateValue.EndWith("'")) {
        dateValue = dateValue.Substring (1, dateValue.GetLength() - 1);
    }

    for (Int32 i = 0; i < dateFormats->GetLength(); ++i) {
        String dateFormat = (*dateFormats)[i];
        AutoPtr<ISimpleDateFormat> dateParser = DateFormatHolder::FormatFor(dateFormat);
        dateParser->Set2DigitYearStart(startDate);

        // try {
        return dateParser->Parse(dateValue, date);
        // } catch (ParseException pe) {
        //     // ignore this exception, we will try the next format
        // }
    }

    // we were unable to parse the date
    Logger::E("DateUtils", "Unable to parse the date %s", dateValue.string());
    return E_DATE_PARSE_EXCEPTION;
    // throw new DateParseException("Unable to parse the date " + dateValue);
}

ECode DateUtils::FormatDate(
    /* [in] */ IDate* date,
    /* [out] */ String* formatDate)
{
    VALIDATE_NOT_NULL(formatDate)
    return FormatDate(date, PATTERN_RFC1123, formatDate);
}

ECode DateUtils::FormatDate(
    /* [in] */ IDate* date,
    /* [in] */ const String& pattern,
    /* [out] */ String* formatDate)
{
    VALIDATE_NOT_NULL(formatDate)
    *formatDate = String(NULL)

    if (date == NULL) {
        Logger::E("DateUtils", "date is null");
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    if (pattern.IsNull()) {
        Logger::E("DateUtils", "pattern is null");
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }

    AutoPtr<ISimpleDateFormat> formatter = DateFormatHolder::FormatFor(pattern);
    return formatter->Format(date, formatDate);
}

} // namespace Cookie
} // namespace Impl
} // namespace Http
} // namespace Apache
} // namespace Org