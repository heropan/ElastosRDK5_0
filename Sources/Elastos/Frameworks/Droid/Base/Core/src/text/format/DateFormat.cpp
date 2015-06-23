
#include "text/format/DateFormat.h"
#include "text/format/CDateUtils.h"
#include "text/CSpannedString.h"
#include "text/CSpannableStringBuilder.h"
#include "provider/Settings.h"
#include "R.h"
#include <elastos/core/StringBuilder.h>
#include <Elastos.CoreLibrary.h>
#include <elastos/core/StringUtils.h>

using Elastos::Core::StringUtils;
using Elastos::Core::StringBuilder;
using Elastos::Core::CStringWrapper;
using Elastos::Text::IDateFormatHelper;
using Elastos::Text::CDateFormatHelper;
using Elastos::Text::ISimpleDateFormat;
using Elastos::Text::CSimpleDateFormat;
using Elastos::Utility::IDate;
using Elastos::Utility::CDate;
using Elastos::Utility::ICalendar;
using Elastos::Utility::IGregorianCalendar;
using Elastos::Utility::CGregorianCalendar;
using Elastos::Utility::ITimeZone;

using Elastos::Droid::Content::IContentResolver;
using Elastos::Droid::Content::Res::IResources;
using Elastos::Droid::Content::Res::IConfiguration;
using Elastos::Droid::Provider::Settings;
using Elastos::Droid::Provider::ISettingsSystem;
using Elastos::Droid::Text::ISpannableStringBuilder;
using Elastos::Droid::Text::CSpannableStringBuilder;
using Elastos::Droid::Text::ISpannedString;
using Elastos::Droid::Text::CSpannedString;

namespace Elastos {
namespace Droid {
namespace Text {
namespace Format {

Mutex DateFormat::sLocaleLock;
AutoPtr<ILocale> DateFormat::sIs24HourLocale;
Boolean DateFormat::sIs24Hour = FALSE;

Boolean DateFormat::Is24HourFormat(
    /* [in] */ IContext* context)
{
    AutoPtr<IContentResolver> resolver;
    context->GetContentResolver((IContentResolver**)&resolver);
    String value;
    Settings::System::GetString(resolver, ISettingsSystem::TIME_12_24, &value);

    if (value.IsNull()) {
        AutoPtr<IResources> res;
        context->GetResources((IResources**)&res);
        AutoPtr<IConfiguration> config;
        res->GetConfiguration((IConfiguration**)&config);
        AutoPtr<ILocale> locale;
        config->GetLocale((ILocale**)&locale);

        {
            Mutex::Autolock lock(sLocaleLock);

            Boolean bIs24HourLocale;
            if (sIs24HourLocale != NULL &&
                    (sIs24HourLocale->Equals(locale, &bIs24HourLocale), bIs24HourLocale)) {
                return sIs24Hour;
            }
        }

        AutoPtr<IDateFormatHelper> helper;
        CDateFormatHelper::AcquireSingleton((IDateFormatHelper**)&helper);
        AutoPtr<Elastos::Text::IDateFormat> natural;
        helper->GetTimeInstance(Elastos::Text::IDateFormat::LONG, locale, (Elastos::Text::IDateFormat**)&natural);

        if (ISimpleDateFormat::Probe(natural) != NULL) {
            AutoPtr<ISimpleDateFormat> sdf = ISimpleDateFormat::Probe(natural);
            String pattern;
            sdf->ToPattern(&pattern);
            if (pattern.IndexOf('H') >= 0) {
                value = "24";
            }
            else {
                value = "12";
            }
        }
        else {
            value = "12";
        }

        {
            Mutex::Autolock lock(sLocaleLock);

            sIs24HourLocale = locale;
            sIs24Hour = value.Equals("24");
        }

        return sIs24Hour;
    }

    return value.Equals("24");
}

AutoPtr<Elastos::Text::IDateFormat> DateFormat::GetTimeFormat(
    /* [in] */ IContext* context)
{
    Boolean b24 = Is24HourFormat(context);
    Int32 res;

    if (b24) {
        res = R::string::twenty_four_hour_time_format;
    }
    else {
        res = R::string::twelve_hour_time_format;
    }

    String strRes;
    context->GetString(res, &strRes);
    AutoPtr<ISimpleDateFormat> sdf;
    CSimpleDateFormat::New((ISimpleDateFormat**)&sdf);
    return Elastos::Text::IDateFormat::Probe(sdf);
}

AutoPtr<Elastos::Text::IDateFormat> DateFormat::GetDateFormat(
    /* [in] */ IContext* context)
{
    AutoPtr<IContentResolver> resolver;
    context->GetContentResolver((IContentResolver**)&resolver);
    String value;
    Settings::System::GetString(resolver, ISettingsSystem::DATE_FORMAT, &value);
    return GetDateFormatForSetting(context, value);
}

AutoPtr<Elastos::Text::IDateFormat> DateFormat::GetDateFormatForSetting(
    /* [in] */ IContext* context,
    /* [in] */ const String& value)
{
    String format = GetDateFormatStringForSetting(context, value);

    AutoPtr<ISimpleDateFormat> sdf;
    CSimpleDateFormat::New(format, (ISimpleDateFormat**)&sdf);
    return Elastos::Text::IDateFormat::Probe(sdf);
}

String DateFormat::GetDateFormatStringForSetting(
    /* [in] */ IContext* context,
    /* [in] */ const String& value)
{
    PEL("DateFormat::GetDateFormatStringForSetting")
    if (!value.IsNull()) {
        Int32 month = value.IndexOf('M');
        Int32 day = value.IndexOf('d');
        Int32 year = value.IndexOf('y');

        if (month >= 0 && day >= 0 && year >= 0) {
            String temp;
            context->GetString(R::string::numeric_date_template, &temp);
            String value_ = value;
            if (year < month && year < day) {
                if (month < day) {
                    value_.AppendFormat(temp.string(), "yyyy", "MM", "dd");
                }
                else {
                    value_.AppendFormat(temp.string(), "yyyy", "dd", "MM");
                }
            }
            else if (month < day) {
                if (day < year) {
                    value_.AppendFormat(temp.string(), "MM", "dd", "yyyy");
                }
                else { // unlikely
                    value_.AppendFormat(temp.string(), "MM", "yyyy", "dd");
                }
            }
            else { // day < month
                if (month < year) {
                    value_.AppendFormat(temp.string(), "dd", "MM", "yyyy");
                }
                else { // unlikely
                    value_.AppendFormat(temp.string(), "dd", "yyyy", "MM");
                }
            }

            return value_;
        }
    }

    /*
     * The setting is not set; use the default.
     * We use a resource string here instead of just DateFormat.SHORT
     * so that we get a four-digit year instead a two-digit year.
     */
    String retValue = value;
    context->GetString(R::string::numeric_date_format, &retValue);
    return retValue;
}

AutoPtr<Elastos::Text::IDateFormat> DateFormat::GetLongDateFormat(
    /* [in] */ IContext* context)
{
    AutoPtr<IDateFormatHelper> helper;
    CDateFormatHelper::AcquireSingleton((IDateFormatHelper**)&helper);
    AutoPtr<Elastos::Text::IDateFormat> df;
    helper->GetDateInstance(Elastos::Text::IDateFormat::LONG, (Elastos::Text::IDateFormat**)&df);
    return df;
}

AutoPtr<Elastos::Text::IDateFormat> DateFormat::GetMediumDateFormat(
    /* [in] */ IContext* context)
{
    AutoPtr<IDateFormatHelper> helper;
    CDateFormatHelper::AcquireSingleton((IDateFormatHelper**)&helper);
    AutoPtr<Elastos::Text::IDateFormat> df;
    helper->GetDateInstance(Elastos::Text::IDateFormat::MEDIUM, (Elastos::Text::IDateFormat**)&df);
    return df;
}

AutoPtr< ArrayOf<Char32> > DateFormat::GetDateFormatOrder(
    /* [in] */ IContext* context)
{
    AutoPtr< ArrayOf<Char32> > order = ArrayOf<Char32>::Alloc(3);
    (*order)[0] = Elastos::Droid::Text::Format::IDateFormat::DATE;
    (*order)[1] = Elastos::Droid::Text::Format::IDateFormat::MONTH;
    (*order)[2] = Elastos::Droid::Text::Format::IDateFormat::YEAR;
    String value = GetDateFormatString(context);
    Int32 index = 0;
    Boolean foundDate = FALSE;
    Boolean foundMonth = FALSE;
    Boolean foundYear = FALSE;

    Char32 c;
    AutoPtr<ArrayOf<Char32> > chars = value.GetChars();
    Int32 valueLen = chars->GetLength();
    for(Int32 i = 0; i < valueLen; i++) {
        c = (*chars)[i];

        if (!foundDate && (c == Elastos::Droid::Text::Format::IDateFormat::DATE)) {
            foundDate = TRUE;
            (*order)[index] = Elastos::Droid::Text::Format::IDateFormat::DATE;
            index++;
        }

        if (!foundMonth && (c == Elastos::Droid::Text::Format::IDateFormat::MONTH
                         || c == Elastos::Droid::Text::Format::IDateFormat::STANDALONE_MONTH)) {
            foundMonth = TRUE;
            (*order)[index] = Elastos::Droid::Text::Format::IDateFormat::MONTH;
            index++;
        }

        if (!foundYear && (c == Elastos::Droid::Text::Format::IDateFormat::YEAR)) {
            foundYear = TRUE;
            (*order)[index] = Elastos::Droid::Text::Format::IDateFormat::YEAR;
            index++;
        }
    }
    return order;
}

String DateFormat::GetDateFormatString(
    /* [in] */ IContext* context)
{
    AutoPtr<IContentResolver> resolver;
    context->GetContentResolver((IContentResolver**)&resolver);
    String value;
    Settings::System::GetString(resolver, ISettingsSystem::DATE_FORMAT, &value);
    return GetDateFormatStringForSetting(context, value);
}

AutoPtr<ICharSequence> DateFormat::Format(
    /* [in] */ ICharSequence* inFormat,
    /* [in] */ Int64 inTimeInMillis)
{
    AutoPtr<IDate> date;
    CDate::New(inTimeInMillis, (IDate**)&date);
    return Format(inFormat, date);
}

AutoPtr<ICharSequence> DateFormat::Format(
    /* [in] */ ICharSequence* inFormat,
    /* [in] */ IDate* inDate)
{
    AutoPtr<ICalendar> c;
    CGregorianCalendar::New((IGregorianCalendar**)&c);
    c->SetTime(inDate);

    return Format(inFormat, c);
}

AutoPtr<ICharSequence> DateFormat::Format(
    /* [in] */ const String& format,
    /* [in] */ IDate* inDate)
{
    AutoPtr<ICharSequence> inFormat;
    CStringWrapper::New(format, (ICharSequence**)&inFormat);

    AutoPtr<ICalendar> c;
    CGregorianCalendar::New((IGregorianCalendar**)&c);
    c->SetTime(inDate);

    return Format(inFormat, c);
}

Boolean DateFormat::HasSeconds(
    /* [in] */ ICharSequence* inFormat)
{
    if (inFormat == NULL) {
        return FALSE;
    }

    Int32 length;
    inFormat->GetLength(&length);

    Char32 c;
    Int32 count;

    for (Int32 i = 0; i < length; i += count) {
        count = 1;
        inFormat->GetCharAt(i, &c);

        if (c == Elastos::Droid::Text::Format::IDateFormat::QUOTE) {
            count = SkipQuotedText(inFormat, i, length);
        }
        else if (c == Elastos::Droid::Text::Format::IDateFormat::SECONDS) {
            return TRUE;
        }
    }

    return FALSE;
}

Int32 DateFormat::SkipQuotedText(
    /* [in] */ ICharSequence* s,
    /* [in] */ Int32 i,
    /* [in] */ Int32 len)
{
    Char32 cc;
    if(i + 1 < len && (s->GetCharAt(i + 1, &cc),
            cc == Elastos::Droid::Text::Format::IDateFormat::QUOTE)) {
        return 2;
    }

    Int32 count = 1;
    // skip leading quote
    i++;

    while (i < len) {
        Char32 c;
        s->GetCharAt(i, &c);

        if (c == Elastos::Droid::Text::Format::IDateFormat::QUOTE) {
            count++;
            //  QUOTEQUOTE -> QUOTE
            if (i + 1 < len && (s->GetCharAt(i + 1, &cc),
                    cc == Elastos::Droid::Text::Format::IDateFormat::QUOTE)) {
                i++;
            } else {
                break;
            }
        } else {
            i++;
            count++;
        }
    }

    return count;
}

AutoPtr<ICharSequence> DateFormat::Format(
    /* [in] */ ICharSequence* inFormat,
    /* [in] */ ICalendar* inDate)
{
    AutoPtr<ISpannableStringBuilder> s;
    CSpannableStringBuilder::New(inFormat, (ISpannableStringBuilder**)&s);
    Char32 c;
    Int32 count;

    Int32 len;
    inFormat->GetLength(&len);

    for (Int32 i = 0; i < len; i += count) {
        Int32 temp;

        count = 1;
        s->GetCharAt(i, &c);

        if (c == Elastos::Droid::Text::Format::IDateFormat::QUOTE) {
            count = AppendQuotedText(s, i, len);
            s->GetLength(&len);
            continue;
        }

        Char32 cc;
        while ((i + count < len) && (s->GetCharAt(i + count, &cc), cc == c)) {
            count++;
        }

        String replacement;

        switch (c) {
            case Elastos::Droid::Text::Format::IDateFormat::AM_PM: {
                inDate->Get(ICalendar::AM_PM, &temp);
                AutoPtr<IDateUtils> du;
                CDateUtils::AcquireSingleton((IDateUtils**)&du);
                du->GetAMPMString(temp, &replacement);
                break;
            }
            case Elastos::Droid::Text::Format::IDateFormat::CAPITAL_AM_PM: {
                inDate->Get(ICalendar::AM_PM, &temp);
                AutoPtr<IDateUtils> du;
                CDateUtils::AcquireSingleton((IDateUtils**)&du);
                du->GetAMPMString(temp, &replacement);
                break;
            }
            case Elastos::Droid::Text::Format::IDateFormat::DATE: {
                inDate->Get(ICalendar::DATE, &temp);
                replacement = ZeroPad(temp, count);
                break;
            }
            case Elastos::Droid::Text::Format::IDateFormat::DAY: {
                inDate->Get(ICalendar::DAY_OF_WEEK, &temp);
                AutoPtr<IDateUtils> du;
                CDateUtils::AcquireSingleton((IDateUtils**)&du);
                du->GetDayOfWeekString(temp,
                        (count < 4 ? (IDateUtils::LENGTH_MEDIUM) : (IDateUtils::LENGTH_LONG)),
                        &replacement);
                break;
            }
            case Elastos::Droid::Text::Format::IDateFormat::HOUR: {
                inDate->Get(ICalendar::HOUR, &temp);

                if (0 == temp) {
                    temp = 12;
                }

                replacement = ZeroPad(temp, count);
                break;
            }
            case Elastos::Droid::Text::Format::IDateFormat::HOUR_OF_DAY: {
                inDate->Get(ICalendar::HOUR_OF_DAY, &temp);
                replacement = ZeroPad(temp, count);
                break;
            }
            case Elastos::Droid::Text::Format::IDateFormat::MINUTE: {
                inDate->Get(ICalendar::MINUTE, &temp);
                replacement = ZeroPad(temp, count);
                break;
            }
            case Elastos::Droid::Text::Format::IDateFormat::MONTH:
            case Elastos::Droid::Text::Format::IDateFormat::STANDALONE_MONTH: {
                replacement = GetMonthString(inDate, count, c);
                break;
            }
            case Elastos::Droid::Text::Format::IDateFormat::SECONDS: {
                inDate->Get(ICalendar::SECOND, &temp);
                replacement = ZeroPad(temp, count);
                break;

            }
            case Elastos::Droid::Text::Format::IDateFormat::TIME_ZONE: {
                replacement = GetTimeZoneString(inDate, count);
                break;
            }
            case Elastos::Droid::Text::Format::IDateFormat::YEAR: {
                replacement = GetYearString(inDate, count);
                break;
            }
            default:
                replacement = NULL;
                break;
        }

        if (!replacement.IsNull()) {
            AutoPtr<ICharSequence> cs;
            CStringWrapper::New(replacement, (ICharSequence**)&cs);
            s->Replace(i, i + count, cs);
            count = replacement.GetLength(); // CARE: count is used in the for loop above
            s->GetLength(&len);
        }
    }

    AutoPtr<ICharSequence> cseq;
    if (ISpanned::Probe(inFormat) != NULL) {
        AutoPtr<ISpannedString> ss;
        CSpannedString::New(s, (ISpannedString**)&ss);
        cseq = ICharSequence::Probe(ss.Get());
    }
    else {
        String str;
        s->ToString(&str);
        CStringWrapper::New(str, (ICharSequence**)&cseq);
    }
    return cseq;
}

String DateFormat::GetMonthString(
    /* [in] */ ICalendar* inDate,
    /* [in] */ Int32 count,
    /* [in] */ Int32 kind)
{
    Boolean standalone = (kind == Elastos::Droid::Text::Format::IDateFormat::STANDALONE_MONTH);
    Int32 month;
    inDate->Get(ICalendar::MONTH, &month);

    if (count >= 4) {
        AutoPtr<IDateUtils> du;
        CDateUtils::AcquireSingleton((IDateUtils**)&du);
        String str;
        if(standalone){
            du->GetStandaloneMonthString(month, IDateUtils::LENGTH_LONG, &str);
        }
        else {
            du->GetMonthString(month, IDateUtils::LENGTH_LONG, &str);
        }
        return str;
    }
    else if (count == 3) {
        AutoPtr<IDateUtils> du;
        CDateUtils::AcquireSingleton((IDateUtils**)&du);
        String str;
        if(standalone){
            du->GetStandaloneMonthString(month, IDateUtils::LENGTH_MEDIUM, &str);
        }
        else {
            du->GetMonthString(month, IDateUtils::LENGTH_MEDIUM, &str);
        }
        return str;
    }
    else {
        // Calendar.JANUARY == 0, so add 1 to month.
        return ZeroPad(month + 1, count);
    }
}

String DateFormat::GetTimeZoneString(
    /* [in] */ ICalendar* inDate,
    /* [in] */ Int32 count)
{
    AutoPtr<ITimeZone> tz;
    inDate->GetTimeZone((ITimeZone**)&tz);

    if (count < 2) { // FIXME: shouldn't this be <= 2 ?
        Int32 dstOffset, zoneOffset;
        inDate->Get(ICalendar::DST_OFFSET, &dstOffset);
        inDate->Get(ICalendar::ZONE_OFFSET, &zoneOffset);
        return FormatZoneOffset(dstOffset + zoneOffset, count);
    }
    else {
        Int32 dstOffset;
        inDate->Get(ICalendar::DST_OFFSET, &dstOffset);
        Boolean dst = dstOffset != 0;
        String str;
        tz->GetDisplayName(dst, ITimeZone::SHORT, &str);
        return str;
    }
}

String DateFormat::FormatZoneOffset(
    /* [in] */ Int32 offset,
    /* [in] */ Int32 count)
{
    offset /= 1000; // milliseconds to seconds
    StringBuilder tb;

    if (offset < 0) {
        tb.InsertChar(0, Char32("-"));
        offset = -offset;
    }
    else {
        tb.InsertChar(0, Char32("+"));
    }

    Int32 hours = offset / 3600;
    Int32 minutes = (offset % 3600) / 60;

    tb.AppendString(ZeroPad(hours, 2));
    tb.AppendString(ZeroPad(minutes, 2));
    return tb.ToString();
}

String DateFormat::GetYearString(
    /* [in] */ ICalendar* inDate,
    /* [in] */ Int32 count)
{
    Int32 year;
    inDate->Get(ICalendar::YEAR, &year);
    if(count <= 2) {
        return ZeroPad(year % 100, 2);
    }
    else {
        //String.format(Locale.getDefault(), "%d", year);
        return StringUtils::Int32ToString(year);
    }
}

Int32 DateFormat::AppendQuotedText(
    /* [in] */ ISpannableStringBuilder* s,
    /* [in] */ Int32 i,
    /* [in] */ Int32 len)
{
    Char32 cc;
    if (i + 1 < len && (s->GetCharAt(i + 1, &cc),
            cc == Elastos::Droid::Text::Format::IDateFormat::QUOTE)) {
        s->Delete(i, i + 1);
        return 1;
    }

    Int32 count = 0;

    // delete leading quote
    s->Delete(i, i + 1);
    len--;

    while (i < len) {
        Char32 c;
        s->GetCharAt(i, &c);

        if (c == Elastos::Droid::Text::Format::IDateFormat::QUOTE) {
            //  QUOTEQUOTE -> QUOTE
            if (i + 1 < len && (s->GetCharAt(i + 1, &cc),
                    cc == Elastos::Droid::Text::Format::IDateFormat::QUOTE)) {
                s->Delete(i, i + 1);
                len--;
                count++;
                i++;
            }
            else {
                //  Closing QUOTE ends quoted text copying
                s->Delete(i, i + 1);
                break;
            }
        }
        else {
            i++;
            count++;
        }
    }

    return count;
}

String DateFormat::ZeroPad(
    /* [in] */ Int32 inValue,
    /* [in] */ Int32 inMinDigits)
{
    String format("%0");
    format += StringUtils::Int32ToString(inMinDigits) + String("d");
    String value;
    value.AppendFormat(format.string(), inValue);
    return value;
}

} // namespace Format
} // namespace Text
} // namepsace Droid
} // namespace Elastos
