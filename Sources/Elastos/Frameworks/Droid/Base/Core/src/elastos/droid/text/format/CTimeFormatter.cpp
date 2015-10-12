#include "CTimeFormatter.h"

namespace Elastos {
namespace Droid {
namespace Text {
namespace Format {

const Int32 CTimeFormatter::FORCE_LOWER_CASE = -1;
const Int32 CTimeFormatter::SECSPERMIN = 60;
const Int32 CTimeFormatter::MINSPERHOUR = 60;
const Int32 CTimeFormatter::DAYSPERWEEK = 7;
const Int32 CTimeFormatter::MONSPERYEAR = 12;
const Int32 CTimeFormatter::HOURSPERDAY = 24;
const Int32 CTimeFormatter::DAYSPERLYEAR = 366;
const Int32 CTimeFormatter::DAYSPERNYEAR = 365;

CAR_INTERFACE_IMPL(CTimeFormatter, Object, ITimeFormatter)

CAR_OBJECT_IMPL(CTimeFormatter)

CTimeFormatter::CTimeFormatter()
{}

CTimeFormatter::~CTimeFormatter()
{}

ECode CTimeFormatter::constructor()
{
    // synchronized (mLock) {
    //     AutoPtr<ILocale> locale_;
    //     CLocale::New((ILocale**)&locale_);
    //     AutoPtr<ILocale> locale = locale_->GetDefault();

    //     Boolean isEqual = FALSE;
    //     locale->Equals(IInterface::Probe(sLocale), &isEqual);
    //     if (sLocale == null || !isEqual) {
    //         sLocale = locale;
    //         AutoPtr<ILocaleDataHelper> localeDataHelper;
    //         CLocaleDataHelper::AcquireSingleton((ILocaleDataHelper**)&localeDataHelper);
    //         localeDataHelper->Get(locale, (ILocale**)&sLocaleData);

    //         AutoPtr<IResources> r;
    //         AutoPtr<IResourcesHelper> resourcesHelper;
    //         CResourcesHelper::AcquireSingleton((resourcesHelper**)&resourcesHelper);
    //         resourcesHelper->GetSystem((IResources**)&r);

    //         r->GetString(com.android.internal.R.string.time_of_day, &sTimeOnlyFormat)
    //         r->GetString(com.android.internal.R.string.month_day_year, &sDateOnlyFormat)
    //         r->GetString(com.android.internal.R.string.date_and_time, &sDateTimeFormat)
    //     }

    //     mDateTimeFormat = sDateTimeFormat;
    //     mTimeOnlyFormat = sTimeOnlyFormat;
    //     mDateOnlyFormat = sDateOnlyFormat;
    //     mLocaleData = sLocaleData;
    // }
    return NOERROR;
}

String CTiemFormatter::Format(
    /* [in] */ const String& pattern,
    /* [in] */ IZoneInfoWallTime* wallTime,
    /* [in] */ IZoneInfo* zoneInfo,
    /* [out] */ String* result)
{
    VALIDATE_NOT_NULL(result)
    // try {
    //   StringBuilder stringBuilder;

    //   mOutputBuilder = stringBuilder;
    //   // This uses the US locale because number localization is handled separately (see below)
    //   // and locale sensitive strings are output directly using outputBuilder.
    //   CFormatter::New(IAppendable::Probe(stringBuilder), CLocale::US, (IFormatter**)&mNumberFormatter);

    //   FormatInternal(pattern, wallTime, zoneInfo);
    //   String result;
    //   stringBuilder.ToString(&result);
    //   // This behavior is the source of a bug since some formats are defined as being
    //   // in ASCII and not localized.
    //   if (mLocaleData->mZeroDigit != '0') {
    //     result = LocalizeDigits(result);
    //   }
    //   return result;
    // } finally {
    //   mOutputBuilder = NULL;
    //   mNumberFormatter = NULL;
    // }

    return String("");
}

String CTiemFormatter::LocalizeDigits(
    /* [in] */ const String& s)
{
    Int32 length = s.GetLength();
    Int32 offsetToLocalizedDigits = mLocaleData->mZeroDigit - '0';
    AutoPtr<StringBuilder> result = new StringBuilder(length);
    for (Int32 i = 0; i < length; ++i) {
        Char32 ch = s.GetChar(i);
        if (ch >= '0' && ch <= '9') {
            ch += offsetToLocalizedDigits;
        }
        result->Append(ch);
    }
    return result->ToString();

}

ECode CTiemFormatter::FormatInternal(
    /* [in] */ const String& pattern,
    /* [in] */ IZoneInfoWallTime* wallTime,
    /* [in] */ IZoneInfo* zoneInfo)
{
    AutoPtr<ICharBufferHelper> formatBufferHelper;
    CCharBufferHelper::AcquireSingleton((ICharBufferHelper**)&formatBufferHelper);
    AutoPtr<ArrayOf<Char32> > pattern_ = pattern.GetChars(0);
    AutoPtr<ICharBuffer> formatBuffer;
    formatBufferHelper->Wrap(pattern_, (ICharBuffer**)&formatBuffer);
    Int32 remaining;
    IBuffer::Probe(formatBuffer)->GetRemaining(&remaining);
    Int32 position;
    Char32 currentChar;
    while (remaining > 0) {
        Boolean outputCurrentChar = TRUE;
        IBuffer::Probe(formatBuffer)->GetPosition(&position);
        formatBuffer->GetCharAt(position, &currentChar);
        if (currentChar == '%') {
            outputCurrentChar = HandleToken(formatBuffer, wallTime, zoneInfo);
        }

        if (outputCurrentChar) {
            IBuffer::Probe(formatBuffer)->GetPosition(&position);
            formatBuffer->GetCharAt(position, &currentChar);
            mOutputBuilder->Append(currentChar);
        }
        formatBuffer->GetPosition(&position);
        AutoPtr<IBuffer> out;
        return formatBuffer->Position(position + 1, (IBuffer**)&out);
    }
}

Boolean CTiemFormatter::HandleToken(
    /* [in] */ ICharBuffer* formatBuffer,
    /* [in] */ IZoneInfoWallTime* wallTime,
    /* [in] */ IZoneInfo* zoneInfo)
{
    // The char at formatBuffer.position() is expected to be '%' at this point.
    Int32 modifier = 0;
    Int32 remaining;
    formatBuffer->GetRemaining(&remaining);
    while (remaining > 1) {
        // Increment the position then get the new current char.
        Int32 position;
        formatBuffer->GetPosition(&position);
        AutoPtr<IBuffer> buffer;
        formatBuffer->Position(position + 1, (IBuffer**)&buffer);
        formatBuffer->GetPosition(&position);
        Char32 currentChar;
        formatBuffer->Get(position, &currentChar);
        Int32 weekDay;
        wallTime->GetWeekDay(&weekDay);
        Int32 month;
        wallTime->GetMonth(&month);
        Int32 year;
        wallTime->GetYear(&year);
        Int32 monthDay;
        wallTime->GetMonthDay(&monthDay);
        Int32 hour;
        wallTime->GetHour(&hour);
        Int32 minute;
        wallTime->GetMinute(&minute);
        Int32 second;
        wallTime->GetSecond(&second);
        Int32 yearDay;
        wallTime->GetYearDay(&yearDay);
        Int32 iDst;
        wallTime->GetIsDst(&iDst);
        String format;
        switch (currentChar) {
            case 'A':
                assert(0 && "TODO"); //Localedata
                ModifyAndAppend((weekDay < 0
                                || weekDay >= DAYSPERWEEK)
                                ? "?" : (*mLocaleData).longWeekdayNames[weekDay + 1],
                        modifier);
                return FALSE;
            case 'a':
                ModifyAndAppend((weekDay < 0
                               || weekDay >= DAYSPERWEEK)
                               ? "?" : (*mLocaleData).shortWeekdayNames[weekDay + 1],
                        modifier);
                return FALSE;
            case 'B':
                if (modifier == '-') {
                    ModifyAndAppend((month < 0
                                   || month >= MONSPERYEAR)
                                   ? "?"
                                   : (*mLocaleData).longStandAloneMonthNames[month],
                            modifier);
               } else {
                    ModifyAndAppend((month < 0
                                   || month >= MONSPERYEAR)
                                   ? "?" : (*mLocaleData).longMonthNames[month],
                            modifier);
               }
               return FALSE;
           case 'b':
           case 'h':
                ModifyAndAppend((month < 0 || month >= MONSPERYEAR)
                               ? "?" : (*mLocaleData).shortMonthNames[month],
                            modifier);
                return FALSE;
           case 'C':
                OutputYear(year, TRUE, FALSE, modifier);
                return FALSE;
           case 'c':
                FormatInternal(dateTimeFormat, wallTime, zoneInfo);
                return FALSE;
           case 'D':
                FormatInternal("%m/%d/%y", wallTime, zoneInfo);
                return FALSE;
           case 'd':
                format = GetFormat(modifier, "%02d", "%2d", "%d", "%02d");
                mNumberFormatter->Format(format, monthDay);
                return FALSE;
           case 'E':
           case 'O':
                // C99 locale modifiers are not supported.
                continue;
           case '_':
           case '-':
           case '0':
           case '^':
           case '#':
                modifier = currentChar;
                continue;
           case 'e':
                format = GetFormat(modifier, "%2d", "%2d", "%d", "%02d");
                mNumberFormatter->Format(format, monthDay);
                return FALSE;
           case 'F':
                FormatInternal("%Y-%m-%d", wallTime, zoneInfo);
                return FALSE;
           case 'H':
                format = GetFormat(modifier, "%02d", "%2d", "%d", "%02d");
                mNumberFormatter->Format(format, hour);
                return FALSE;
           case 'I':
                Int32 hour = (hour % 12 != 0) ? (hour % 12) : 12;
                format = GetFormat(modifier, "%02d", "%2d", "%d", "%02d");
                mNumberFormatter->Format(format, hour);
               return FALSE;
           case 'j':
                Int32 yearDay_ = yearDay + 1;
                format = GetFormat(modifier, "%03d", "%3d", "%d", "%03d");
                mNumberFormatter->Format(format, yearDay_);
                return FALSE;
           case 'k':
                format = GetFormat(modifier, "%2d", "%2d", "%d", "%02d");
                mNumberFormatter->Format(format, hour);
                return FALSE;
           case 'l':
                Int32 n2 = (hour % 12 != 0) ? (hour % 12) : 12;
                format = GetFormat(modifier, "%2d", "%2d", "%d", "%02d");
                mNumberFormatter->Format(format, n2);
                return FALSE;
           case 'M':
                format = GetFormat(modifier, "%02d", "%2d", "%d", "%02d");
                mNumberFormatter->Format(format, minute);
                return FALSE;
           case 'm':
                format = GetFormat(modifier, "%02d", "%2d", "%d", "%02d");
                mNumberFormatter->Format(format, month + 1);
                return FALSE;
           case 'n':
                mOutputBuilder->Append('\n');
                return FALSE;
           case 'p':
                ModifyAndAppend((hour >= (HOURSPERDAY / 2)) ? (*mLocaleData).amPm[1]
                       : (*mLocaleData).amPm[0], modifier);
                return FALSE;
           case 'P':
                ModifyAndAppend((hour >= (HOURSPERDAY / 2)) ? (*mLocaleData).amPm[1]
                       : (*mLocaleData).amPm[0], FORCE_LOWER_CASE);
                return FALSE;
           case 'R':
                FormatInternal("%H:%M", wallTime, zoneInfo);
                return FALSE;
           case 'r':
                FormatInternal("%I:%M:%S %p", wallTime, zoneInfo);
                return FALSE;
           case 'S':
                format = GetFormat(modifier, "%02d", "%2d", "%d", "%02d");
                mNumberFormatter->Format(format, second);
                return FALSE;
           case 's':
                Int32 timeInSeconds;
                wallTime->Mktime(zoneInfo, &timeInSeconds);
                mOutputBuilder->Append(Integer.toString(timeInSeconds));
                return FALSE;
           case 'T':
                FormatInternal("%H:%M:%S", wallTime, zoneInfo);
                return FALSE;
           case 't':
                mOutputBuilder->Append('\t');
                return FALSE;
           case 'U':
                format = GetFormat(modifier, "%02d", "%2d", "%d", "%02d");
                mNumberFormatter->Format(format, (yearDay + DAYSPERWEEK - weekDay) / DAYSPERWEEK);
                return FALSE;
           case 'u':
                Int32 day = (weekDay == 0) ? DAYSPERWEEK : weekDay;
                mNumberFormatter->Format("%d", day);
                return FALSE;
           case 'V':   /* ISO 8601 week number */
           case 'G':   /* ISO 8601 year (four digits) */
           case 'g':   /* ISO 8601 year (two digits) */
           {
               Int32 yday;
               Int32 wday;
               wallTime->GetYearDay(&yday);
               wallTime->GetWeekDay(&wday);
               Int32 w;
               while (TRUE) {
                   Int32 len = IsLeap(year) ? DAYSPERLYEAR : DAYSPERNYEAR;
                   // What yday (-3 ... 3) does the ISO year begin on?
                   Int32 bot = ((yday + 11 - wday) % DAYSPERWEEK) - 3;
                   // What yday does the NEXT ISO year begin on?
                   Int32 top = bot - (len % DAYSPERWEEK);
                   if (top < -3) {
                       top += DAYSPERWEEK;
                   }
                   top += len;
                   if (yday >= top) {
                       ++year;
                       w = 1;
                       break;
                   }
                   if (yday >= bot) {
                       w = 1 + ((yday - bot) / DAYSPERWEEK);
                       break;
                   }
                   --year;
                   yday += IsLeap(year) ? DAYSPERLYEAR : DAYSPERNYEAR;
               }
               if (currentChar == 'V') {
                   format = GetFormat(modifier, "%02d", "%2d", "%d", "%02d");
                   mNumberFormatter->Format(format, w);
               } else if (currentChar == 'g') {
                   OutputYear(year, FALSE, TRUE, modifier);
               } else {
                   OutputYear(year, TRUE, TRUE, modifier);
               }
               return FALSE;
           }
           case 'v':
               FormatInternal("%e-%b-%Y", wallTime, zoneInfo);
               return FALSE;
           case 'W':
               Int32 n = (yearDay + DAYSPERWEEK - (
                               weekDay != 0 ? (weekDay - 1)
                                       : (DAYSPERWEEK - 1))) / DAYSPERWEEK;
               format = GetFormat(modifier, "%02d", "%2d", "%d", "%02d");
               mNumberFormatter->Format(format, n);
               return FALSE;
           case 'w':
               mNumberFormatter->Format("%d", weekDay);
               return FALSE;
           case 'X':
               FormatInternal(timeOnlyFormat, wallTime, zoneInfo);
               return FALSE;
           case 'x':
               FormatInternal(dateOnlyFormat, wallTime, zoneInfo);
               return FALSE;
           case 'y':
               OutputYear(year, FALSE, TRUE, modifier);
               return FALSE;
           case 'Y':
               OutputYear(year, TRUE, TRUE, modifier);
               return FALSE;
           case 'Z':
               if (iDst < 0) {
                   return FALSE;
               }
               boolean isDst = isDst != 0;
               ModifyAndAppend(ITimeZone::Probe(zoneInfo)->GetDisplayName(isDst, TimeZone.SHORT), modifier);
               return FALSE;
           case 'z': {
               if (iDst < 0) {
                   return FALSE;
               }
               Int32 diff;
               wallTime->GetGmtOffset(&diff);
               Char32 sign;
               if (diff < 0) {
                   sign = '-';
                   diff = -diff;
               } else {
                   sign = '+';
               }
               mOutputBuilder->Append(sign);
               diff /= SECSPERMIN;
               diff = (diff / MINSPERHOUR) * 100 + (diff % MINSPERHOUR);
               format = GetFormat(modifier, "%04d", "%4d", "%d", "%04d");
               mNumberFormatter->Format(format, diff);
               return FALSE;
           }
           case '+':
               FormatInternal("%a %b %e %H:%M:%S %Z %Y", wallTime, zoneInfo);
               return FALSE;
           case '%':
               // If conversion char is undefined, behavior is undefined. Print out the
               // character itself.
           default:
               return TRUE;
       }
   }
   return TRUE;
}

ECode CTimeFormatter::ModifyAndAppend(
    /* [in] */ ICharSequence* str,
    /* [in] */ Int32 modifier)
{
    VALIDATE_NOT_NULL(str)
    Int32 length;
    str->GetLength(&length);
    Char32 c;
    switch (modifier) {
    case FORCE_LOWER_CASE:
        for (Int32 i = 0; i < length; i++) {
            str->GetCharAt(i,&c);
            mOutputBuilder.Append(BrokenToLower(c));
        }
        break;
    case '^':
        for (Int32 i = 0; i < length; i++) {
            mOutputBuilder.Append(BrokenToUpper(c));
        }
        break;
    case '#':
        for (Int32 i = 0; i < length; i++) {
            str->GetCharAt(i, &c);
            if (BrokenIsUpper(c)) {
                c = BrokenToLower(c);
            } else if (BrokenIsLower(c)) {
                c = BrokenToUpper(c);
            }
            mOutputBuilder.Append(c);
        }
        break;
    default:
        mOutputBuilder.Append(str);
    }
}

ECode CTimeFormatter::OutputYear(
    /* [in] */ Int32 value,
    /* [in] */ Boolean outputTop,
    /* [in] */ Boolean outputBottom,
    /* [in] */ Int32 modifier)
{
    Int32 lead;
    Int32 trail;

    const Int32 DIVISOR = 100;
    trail = value % DIVISOR;
    lead = value / DIVISOR + trail / DIVISOR;
    trail %= DIVISOR;
    if (trail < 0 && lead > 0) {
        trail += DIVISOR;
        --lead;
    } else if (lead < 0 && trail > 0) {
        trail -= DIVISOR;
        ++lead;
    }
    if (outputTop) {
        if (lead == 0 && trail < 0) {
            mOutputBuilder->Append("-0");
        } else {
            mNumberFormatter->Format(GetFormat(modifier, "%02d", "%2d", "%d", "%02d"), lead);
        }
    }
    if (outputBottom) {
        Int32 n = ((trail < 0) ? -trail : trail);
        mNumberFormatter->Format(GetFormat(modifier, "%02d", "%2d", "%d", "%02d"), n);
    }
}

String CTimeFormatter::GetFormat(
    /* [in] */ Int32 modifier,
    /* [in] */ const String& normal,
    /* [in] */ const String& underscore,
    /* [in] */ const String& dash,
    /* [in] */ const String& zero)
{
    switch (modifier) {
        case '_':
            return underscore;
        case '-':
            return dash;
        case '0':
            return zero;
    }
    return normal;
}

Boolean CTimeFormatter::IsLeap(
    /* [in] */ Int32 year)
{
    return (((year) % 4) == 0 && (((year) % 100) != 0 || ((year) % 400) == 0));
}

Boolean CTimeFormatter::BrokenIsUpper(
    /* [in] */ Char32 toCheck)
{
    return toCheck >= 'A' && toCheck <= 'Z';
}

Boolean CTimeFormatter::BrokenIsLower(
    /* [in] */ Char32 toCheck)
{
    return toCheck >= 'a' && toCheck <= 'z';
}

Char32 CTimeFormatter::BrokenToLower(
    /* [in] */ Char32 input)
{
    if (input >= 'A' && input <= 'Z') {
        return (Char32) (input - 'A' + 'a');
    }
    return input;
}

Char32 CTimeFormatter::BrokenToUpper(
    /* [in] */ Char32 input)
{
    if (input >= 'a' && input <= 'z') {
        return (Char32) (input - 'a' + 'A');
    }
    return input;
}

} // namespace Elastos
} // namespace Droid
} // namespace Text
} // namespace Format

