
#include "ext/frameworkdef.h"
#include "text/format/CTime.h"
#include "text/format/DateUtils.h"
#include "content/res/CResources.h"
#include "R.h"
#include <elastos/core/Math.h>
#include <cutils/tztime.h>
#include <utils/String8.h>

using Libcore::ICU::ILocale;
using Libcore::ICU::ILocaleHelper;
using Libcore::ICU::ILocaleData;
using Libcore::ICU::ILocaleDataHelper;
using Libcore::ICU::CLocale;
using Libcore::ICU::CLocaleHelper;
using Libcore::ICU::CLocaleDataHelper;
using Elastos::Utility::CTimeZoneHelper;
using Elastos::Utility::ITimeZoneHelper;
using Elastos::Utility::ITimeZone;
using Elastos::Droid::Content::Res::IResources;
using Elastos::Droid::Content::Res::CResources;

namespace Elastos {
namespace Droid {
namespace Text {
namespace Format {

const String CTime::Y_M_D_T_H_M_S_000("%Y-%m-%dT%H:%M:%S.000");
const String CTime::Y_M_D_T_H_M_S_000_Z("%Y-%m-%dT%H:%M:%S.000Z");
const String CTime::Y_M_D("%Y-%m-%d");

/*
 * The Locale for which date formatting strings have been loaded.
 */
AutoPtr<ILocale> CTime::sLocale;
AutoPtr< ArrayOf<String> > CTime::sShortMonths;
AutoPtr< ArrayOf<String> > CTime::sLongMonths;
AutoPtr< ArrayOf<String> > CTime::sLongStandaloneMonths;
AutoPtr< ArrayOf<String> > CTime::sShortWeekdays;
AutoPtr< ArrayOf<String> > CTime::sLongWeekdays;
String CTime::sTimeOnlyFormat;
String CTime::sDateOnlyFormat;
String CTime::sDateTimeFormat;
String CTime::sAm;
String CTime::sPm;
Char32 CTime::sZeroDigit;

// Referenced by native code.
String CTime::sDateCommand("%a %b %e %H:%M:%S %Z %Y");

const Int32 CTime::DAYS_PER_MONTH[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

/**
 * This array is indexed by the weekDay field (SUNDAY=0, MONDAY=1, etc.)
 * and gives a number that can be added to the yearDay to give the
 * closest Thursday yearDay.
 */
const Int32 CTime::sThursdayOffset[] = {-3, 3, 2, 1, 0, -1, -2};

Mutex CTime::sTimeClassLock;

char tochar(Int32 n)
{
    return (n >= 0 && n <= 9) ? ('0'+ n) : ' ';
}

char next_char(Int32 *m, Int32 k)
{
    Int32 n = *m / k;
    *m = *m % k;
    return tochar(n);
}

String format_ex(const char* format, const struct strftime_locale *locale, struct tm* t)
{
    char buf[257];
    int n = strftime_tz(buf, 257, format, t, locale);
    if (n > 0) {
        return String(buf);
    }
    else {
        return String("");
    }
}

Boolean ctime2tm(CTime* time, struct tm* tm)
{
    tm->tm_sec = time->mSecond;
    tm->tm_min = time->mMinute;
    tm->tm_hour = time->mHour;
    tm->tm_mday = time->mMonthDay;
    tm->tm_mon = time->mMonth;
    tm->tm_year = time->mYear - 1900;
    tm->tm_wday = time->mWeekDay;
    tm->tm_yday = time->mYearDay;
    tm->tm_isdst = time->mIsDst;
    tm->tm_gmtoff = time->mGmtoff;
    Boolean allDay = time->mAllDay;
    if (allDay &&
        ((tm->tm_sec != 0) || (tm->tm_min != 0) || (tm->tm_hour != 0))) {
        // jniThrowException(env, "java/lang/IllegalArgumentException",
        //                   "allDay is true but sec, min, hour are not 0.");
        return FALSE;
    }
    return TRUE;
}

void tm2ctime(const struct tm* tm, CTime* time)
{
    time->mSecond = tm->tm_sec;
    time->mMinute = tm->tm_min;
    time->mHour = tm->tm_hour;
    time->mMonthDay = tm->tm_mday;
    time->mMonth = tm->tm_mon;
    time->mYear = tm->tm_year + 1900;
    time->mWeekDay = tm->tm_wday;
    time->mYearDay = tm->tm_yday;
    time->mIsDst = tm->tm_isdst;
    time->mGmtoff = tm->tm_gmtoff;
}

void format2445_ex(char* buf, Boolean hasTime, struct tm* tm, const String& timezone)
{
    Int32 n;

    n = tm->tm_year + 1900;
    buf[0] = next_char(&n, 1000);
    buf[1] = next_char(&n, 100);
    buf[2] = next_char(&n, 10);
    buf[3] = tochar(n);

    n = tm->tm_mon + 1;
    buf[4] = next_char(&n, 10);
    buf[5] = tochar(n);

    n = tm->tm_mday;
    buf[6] = next_char(&n, 10);
    buf[7] = tochar(n);

    if (hasTime) {
        buf[8] = 'T';

        n = tm->tm_hour;
        buf[9] = next_char(&n, 10);
        buf[10] = tochar(n);

        n = tm->tm_min;
        buf[11] = next_char(&n, 10);
        buf[12] = tochar(n);

        n = tm->tm_sec;
        buf[13] = next_char(&n, 10);
        buf[14] = tochar(n);
        if (timezone.Equals("UTC")) {
            buf[15] = 'Z';
        }
    }
}

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
    String strId;
    tz->GetID(&strId);
    return constructor(strId);
}

ECode CTime::constructor(
    /* [in] */ const String& timezone)
{
    if (timezone.IsNull()) {
       // throw new NullPointerException("timezone is null!");
        return E_NULL_POINTER_EXCEPTION;
    }
    mTimezone = timezone;
    mYear = 1970;
    mMonthDay = 1;
    // Set the daylight-saving indicator to the unknown value -1 so that
    // it will be recomputed.
    mIsDst = -1;
    return NOERROR;
}

ECode CTime::constructor(
    /* [in] */ ITime* other)
{
    return Set(other);
}

ECode CTime::Normalize(
    /* [in] */ Boolean ignoreDst,
    /* [out] */ Int64* ret)
{
    VALIDATE_NOT_NULL(ret);
    struct tm t;
    if (!ctime2tm(this, &t)) {
        *ret = 0;
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }

    if (ignoreDst) {
        t.tm_isdst = -1;
    }
    int64_t r = mktime_tz(&t, mTimezone.string());
    if (r == -1) {
        *ret = -1;
        return NOERROR;
    }
    *ret = r * 1000;

    tm2ctime(&t, this);
    return NOERROR;
}

ECode CTime::SwitchTimezone(
    /* [in] */ const String& timezone)
{
    struct tm t;
    if (!ctime2tm(this, &t)) return E_ILLEGAL_ARGUMENT_EXCEPTION;

    time_t seconds = mktime_tz(&t, mTimezone.string());
    localtime_tz(&seconds, &t, timezone.string());

    tm2ctime(&t, this);

    mTimezone = timezone;
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
    mTimezone = timezone;
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
    *result = NativeCompare(a, b);
    return NOERROR;
}

#define COMPARE_FIELD(field) do { \
        Int32 diff = aObj->field - bObj->field; \
        if (diff != 0) return diff; \
    } while(0)

Int32 CTime::NativeCompare(
    /* [in] */ ITime* a,
    /* [in] */ ITime* b)
{
    CTime* aObj = (CTime*)a;
    CTime* bObj = (CTime*)b;
    if (aObj->mTimezone.Equals(bObj->mTimezone)) {
        // if the timezones are the same, we can easily compare the two
        // times.  Otherwise, convert to milliseconds and compare that.
        // This requires that object be normalized.
        COMPARE_FIELD(mYear);
        COMPARE_FIELD(mMonth);
        COMPARE_FIELD(mMonthDay);
        COMPARE_FIELD(mHour);
        COMPARE_FIELD(mMinute);
        COMPARE_FIELD(mSecond);
        return 0;
    }
    else {
        Int64 am;
        a->ToMillis(FALSE, &am /* use isDst */);
        Int64 bm;
        b->ToMillis(FALSE, &bm /* use isDst */);
        Int64 diff = am - bm;
        return (diff < 0) ? -1 : ((diff > 0) ? 1 : 0);
    }
}

ECode CTime::Format(
    /* [in] */ const String& format,
    /* [out] */ String* ret)
{
    VALIDATE_NOT_NULL(ret);
    AutoLock lock(sTimeClassLock);

    AutoPtr<ILocaleHelper> localeHelp;
    CLocaleHelper::AcquireSingleton((ILocaleHelper**)&localeHelp);
    AutoPtr<ILocale> locale;
    localeHelp->GetDefault((ILocale**)&locale);

    Boolean isEquals;
    if (sLocale == NULL || locale == NULL || (locale->Equals(sLocale, &isEquals), !isEquals)) {
        AutoPtr<ILocaleDataHelper> ldh;
        CLocaleDataHelper::AcquireSingleton((ILocaleDataHelper**)&ldh);
        AutoPtr<ILocaleData> localeData;
        ldh->Get(locale, (ILocaleData**)&localeData);

        AutoPtr< ArrayOf<String> > amPm;
        localeData->GetAmPm((ArrayOf<String>**)&amPm);
        sAm = (*amPm)[0];
        sPm = (*amPm)[1];
        localeData->GetZeroDigit(&sZeroDigit);

        sShortMonths = NULL;
        sLongMonths = NULL;
        sLongStandaloneMonths = NULL;
        sShortWeekdays = NULL;
        sLongWeekdays = NULL;
        localeData->GetShortMonthNames((ArrayOf<String>**)&sShortMonths);
        localeData->GetLongMonthNames((ArrayOf<String>**)&sLongMonths);
        localeData->GetLongStandAloneMonthNames((ArrayOf<String>**)&sLongStandaloneMonths);
        localeData->GetShortWeekdayNames((ArrayOf<String>**)&sShortWeekdays);
        localeData->GetLongWeekdayNames((ArrayOf<String>**)&sLongWeekdays);

        AutoPtr<IResources> r = CResources::GetSystem();
        r->GetString(R::string::time_of_day, &sTimeOnlyFormat);
        r->GetString(R::string::month_day_year, &sDateOnlyFormat);
        r->GetString(R::string::date_and_time, &sDateTimeFormat);

        sLocale = locale;
    }

    String result;
    FAIL_RETURN(Format1(format, &result));
    if (sZeroDigit != '0') {
        result = LocalizeDigits(result);
    }
    *ret  = result;
    return NOERROR;
}

ECode CTime::Format1(
    /* [in] */ const String& format,
    /* [out] */ String* result)
{
    // We only teardown and setup our 'locale' struct and other state
    // when the Java-side locale changed.  This is safe to do here
    // without locking because we're always called from Java code
    // synchronized on the class instance.
    VALIDATE_NOT_NULL(result);
    struct tm t;
    if (!ctime2tm(this, &t)) {
        *result = "";
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }

    AutoPtr<ILocaleHelper> localeHelp;
    CLocaleHelper::AcquireSingleton((ILocaleHelper**)&localeHelp);
    AutoPtr<ILocale> tmp_locale;
    localeHelp->GetDefault((ILocale**)&tmp_locale);
    AutoPtr<ILocaleDataHelper> ldh;
    CLocaleDataHelper::AcquireSingleton((ILocaleDataHelper**)&ldh);
    AutoPtr<ILocaleData> localeData;
    ldh->Get(tmp_locale, (ILocaleData**)&localeData);

    static AutoPtr<ILocale> localePrevious;
    static struct strftime_locale locale;
    if (localePrevious != sLocale) {
        localePrevious = sLocale;

        for (Int32 i = 0; i < 12; i++) {
            //Calendar.JANUARY == 0
            locale.mon[i] = (*sShortMonths)[i].string();
            locale.month[i] = (*sLongMonths)[i].string();
            locale.standalone_month[i] = (*sLongStandaloneMonths)[i].string();
        }

        for (Int32 i = 0; i < 7; ++i) {
            // Calendar.SUNDAY == 1, and there's an empty string in element 0.
            locale.wday[i] = (*sShortWeekdays)[i].string();
            locale.weekday[i] = (*sLongWeekdays)[i].string();
        }

        locale.X_fmt = sTimeOnlyFormat.string();
        locale.x_fmt = sDateOnlyFormat.string();
        locale.c_fmt = sDateTimeFormat.string();

        locale.am = sAm.string();
        locale.pm = sPm.string();

        locale.date_fmt = sDateCommand.string();
    }

    *result = format_ex(format.string(), &locale, &t);
    return NOERROR;
}

String CTime::LocalizeDigits(
    /* [in] */ const String& s)
{
    AutoPtr<ArrayOf<Char32> > chars = s.GetChars();
    Int32 charCount = chars->GetLength();
    Char32 ch;
    Int32 offsetToLocalizedDigits = sZeroDigit - '0';
    StringBuilder result;
    for (Int32 i = 0; i < charCount; ++i) {
        ch = (*chars)[i];
        if (ch >= '0' && ch <= '9') {
            ch += offsetToLocalizedDigits;
        }
        result.AppendChar(ch);
    }
    return result.ToString();
}

ECode CTime::ToString(
    /* [out] */ String* ret)
{
    VALIDATE_NOT_NULL(ret);
    struct tm t;
    if (!ctime2tm(this, &t)) {
        *ret = "";
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }

    char s[150];
#ifdef HAVE_TM_GMTOFF
    Int64 tm_gmtoff = t.tm_gmtoff;
#else
    Int64 tm_gmtoff = 0;
#endif
    int64_t r = mktime_tz(&t, mTimezone.string());
    if (r != -1) r *= 1000;
    sprintf(s, "%04d%02d%02dT%02d%02d%02d%s(%d,%d,%lld,%d,%d)",
            t.tm_year + 1900, t.tm_mon + 1, t.tm_mday, t.tm_hour, t.tm_min,
            t.tm_sec, mTimezone.string(), t.tm_wday, t.tm_yday, tm_gmtoff, t.tm_isdst,
            (int)(r / 1000));
    *ret = s;
    return NOERROR;
}

ECode CTime::Parse(
    /* [in] */ const String& s,
    /* [out] */ Boolean* ret)
{
    VALIDATE_NOT_NULL(ret);
    if (s.IsNull()) {
        //throw new NullPointerException("time string is null");
        *ret = FALSE;
        return  E_NULL_POINTER_EXCEPTION;
    }
    FAIL_RETURN(NativeParse(s, ret));
    if (*ret) {
        mTimezone = ITime::TIMEZONE_UTC;
    }
    return NOERROR;
}

// ============================================================================
// Just do this here because it's not worth recreating the strings
Int32 get_char(const String& s, Char32 c, Int32 mul, Boolean* thrown)
{
    if (c >= '0' && c <= '9') {
        return (c - '0') * mul;
    }
    else {
        if (!*thrown) {
           // jniThrowExceptionFmt(env, "android/util/TimeFormatException",
           //                       "Parse error at pos=%d", spos);
            *thrown = TRUE;
        }
        return 0;
    }
}

Boolean check_char(const String& s, Char32 c, Char32 expected)
{
    if (c != expected) {
        // jniThrowExceptionFmt(env, "android/util/TimeFormatException",
        //                      "Unexpected character 0x%02x at pos=%d.  Expected %c.",
        //                      c, spos, expected);
        return FALSE;
    }
    return TRUE;
}

ECode CTime::NativeParse(
    /* [in] */ const String& s,
    /* [out] */ Boolean* result)
{
    AutoPtr<ArrayOf<Char32> > chars = s.GetChars();
    UInt32 len = chars->GetLength();
    if (len < 8) {
        // jniThrowException(env, "android/util/TimeFormatException",
        //                   "String too short -- expected at least 8 characters.");
        *result = FALSE;
        return E_TIME_FORMAT_EXCEPTION;
    }

    Boolean inUtc = FALSE;

    // year
    Int32 n;
    Boolean thrown = FALSE;
    n = get_char(s, (*chars)[0], 1000, &thrown);
    n += get_char(s, (*chars)[1], 100, &thrown);
    n += get_char(s, (*chars)[2], 10, &thrown);
    n += get_char(s, (*chars)[3], 1, &thrown);
    if (thrown) {
        *result = FALSE;
        return E_TIME_FORMAT_EXCEPTION;
    }
    mYear = n;

    // month
    n = get_char(s, (*chars)[4], 10, &thrown);
    n += get_char(s, (*chars)[5], 1, &thrown);
    n--;
    if (thrown) {
        *result = FALSE;
        return E_TIME_FORMAT_EXCEPTION;
    }
    mMonth = n;

    //day of month
    n = get_char(s, (*chars)[6], 10, &thrown);
    n += get_char(s, (*chars)[7], 1, &thrown);
    if (thrown) {
        *result = FALSE;
        return E_TIME_FORMAT_EXCEPTION;
    }
    mMonthDay = n;

    if (len > 8) {
        // T
        if (!check_char(s, (*chars)[8], 'T')) {
            *result = FALSE;
            return E_TIME_FORMAT_EXCEPTION;
        }
        mAllDay = FALSE;

        //hour
        n = get_char(s, (*chars)[9], 10, &thrown);
        n += get_char(s, (*chars)[10], 1, &thrown);
        if (thrown) {
            *result = FALSE;
            return E_TIME_FORMAT_EXCEPTION;
        }
        mHour = n;

        // min
        n = get_char(s, (*chars)[11], 10, &thrown);
        n = get_char(s, (*chars)[12], 1, &thrown);
        if (thrown) {
            *result = FALSE;
            return E_TIME_FORMAT_EXCEPTION;
        }
        mMinute = n;

        //sec
        n = get_char(s, (*chars)[13], 10, &thrown);
        n += get_char(s, (*chars)[14], 1, &thrown);
        if (thrown) {
            *result = FALSE;
            return E_TIME_FORMAT_EXCEPTION;
        }
        mSecond = n;

        if (len > 15) {
            // Z
            if (!check_char(s, (*chars)[15], 'Z')) {
                *result = FALSE;
                return E_TIME_FORMAT_EXCEPTION;
            }
            inUtc = TRUE;
        }
    }
    else {
        mAllDay = FALSE;
        mHour = 0;
        mMinute = 0;
        mSecond = 0;
    }

    mWeekDay = 0;
    mYearDay = 0;
    mIsDst = -1;
    mGmtoff = 0;

    *result = inUtc;
    return NOERROR;
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
    FAIL_RETURN(NativeParse3339(s, ret));
    if (*ret) {
        mTimezone = ITime::TIMEZONE_UTC;
    }
    return NOERROR;
}

ECode CTime::NativeParse3339(
    /* [in] */ const String& s,
    /* [out] */ Boolean* result)
{
    AutoPtr<ArrayOf<Char32> > chars = s.GetChars();
    UInt32 len = chars->GetLength();
    if (len < 10) {
        // jniThrowException(env, "android/util/TimeFormatException",
        //                   "String too short --- expected at least 10 characters.");
        *result = FALSE;
        return E_TIME_FORMAT_EXCEPTION;
    }

    Boolean inUtc = FALSE;

    // year
    Int32 n;
    Boolean thrown = FALSE;
    n = get_char(s, (*chars)[0], 1000, &thrown);
    n += get_char(s, (*chars)[1], 100, &thrown);
    n += get_char(s, (*chars)[2], 10, &thrown);
    n += get_char(s, (*chars)[3], 1, &thrown);
    if (thrown) {
        *result = FALSE;
        return E_TIME_FORMAT_EXCEPTION;
    }
    mYear = n;

    // -
    if (!check_char(s, (*chars)[4], '-')) {
        *result = FALSE;
        return E_TIME_FORMAT_EXCEPTION;
    }

    // month
    n = get_char(s, (*chars)[5], 10, &thrown);
    n += get_char(s, (*chars)[6], 1, &thrown);
    --n;
    if (thrown) {
        *result = FALSE;
        return E_TIME_FORMAT_EXCEPTION;
    }
    mMonth = n;

    // -
    if (!check_char(s, (*chars)[7], '-')) {
        *result = FALSE;
        return E_TIME_FORMAT_EXCEPTION;
    }

    //day
    n = get_char(s, (*chars)[8], 10, &thrown);
    n += get_char(s, (*chars)[9], 1, &thrown);
    if (thrown) {
        *result = FALSE;
        return E_TIME_FORMAT_EXCEPTION;
    }
    mMonthDay = n;

    if (len >= 19) {
        // T
        if (!check_char(s, (*chars)[10], 'T')) {
            *result = FALSE;
            return E_TIME_FORMAT_EXCEPTION;
        }

        mAllDay = FALSE;
        //hour
        n = get_char(s, (*chars)[11], 10, &thrown);
        n += get_char(s, (*chars)[12], 1, &thrown);
        if (thrown) {
            *result = FALSE;
            return E_TIME_FORMAT_EXCEPTION;
        }
        Int32 hour = n;

        // :
        if (!check_char(s, (*chars)[13], ':')) {
            *result = FALSE;
            return E_TIME_FORMAT_EXCEPTION;
        }

        // minute
        n = get_char(s, (*chars)[14], 10, &thrown);
        n += get_char(s, (*chars)[15], 1, &thrown);
        if (thrown) {
            *result = FALSE;
            return E_TIME_FORMAT_EXCEPTION;
        }
        Int32 minute = n;

        // :
        if (!check_char(s, (*chars)[16], ':')) {
            *result = FALSE;
            return E_TIME_FORMAT_EXCEPTION;
        }

        //second
        n = get_char(s, (*chars)[17], 10, &thrown);
        n += get_char(s, (*chars)[18], 1, &thrown);
        if (thrown) {
            *result = FALSE;
            return E_TIME_FORMAT_EXCEPTION;
        }
        mSecond = n;

        // skip the '.XYZ' -- we don't care about subsecond precision.
        Int32 tz_index = 19;
        if (tz_index < len && (*chars)[tz_index] == '.') {
            do {
                 tz_index++;
            } while(tz_index < len
                 && (*chars)[tz_index] >= '0'
                 && (*chars)[tz_index] <= '9');
        }

        Int32 offset = 0;
        if (len > tz_index) {
            Char32 c = (*chars)[tz_index];

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
                // jniThrowExceptionFmt(env, "android/util/TimeFormatException",
                //                      "Unexpected character 0x%02x at position %d.  Expected + or -",
                //                      c, tz_index);
                *result = FALSE;
                return E_TIME_FORMAT_EXCEPTION;
           }
           inUtc = TRUE;

           if (offset != 0) {
                if (len < tz_index + 6) {
                     // jniThrowExceptionFmt(env, "android/util/TimeFormatException",
                     //                     "Unexpected length; should be %d characters",
                     //                     tz_index + 6);
                    *result = FALSE;
                    return E_TIME_FORMAT_EXCEPTION;
                }

                //hour
                n = get_char(s, (*chars)[tz_index + 1], 10, &thrown);
                n += get_char(s, (*chars)[tz_index + 2], 1, &thrown);
                if (thrown) {
                    *result = FALSE;
                    return E_TIME_FORMAT_EXCEPTION;
                }
                n *= offset;
                hour += n;

                // :
                if (!check_char(s, (*chars)[tz_index + 3], ':')) {
                    *result = FALSE;
                    return E_TIME_FORMAT_EXCEPTION;
                }

                //minute
                n = get_char(s, (*chars)[tz_index + 4], 10, &thrown);
                n += get_char(s, (*chars)[tz_index + 5], 1, &thrown);
                if (thrown) {
                    *result = FALSE;
                    return E_TIME_FORMAT_EXCEPTION;
                }
                n *= offset;
                minute += n;
            }
        }
        mHour = hour;
        mMinute = minute;

        if (offset != 0) {
            // we need to normalize after applying the hour and minute offsets
            Int64 itmp;
            Normalize(FALSE, &itmp/* use isdst */);
            // The timezone is set to UTC in the calling Java code.
        }
    }
    else {
        mAllDay = FALSE;
        mHour = 0;
        mMinute = 0;
        mSecond = 0;
    }

    mWeekDay = 0;
    mYearDay = 0;
    mIsDst = -1;
    mGmtoff = 0;

    *result = inUtc;
    return NOERROR;
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
    mAllDay = FALSE;
    struct tm t;
    time_t seconds;
    time(&seconds);
    localtime_tz(&seconds, &t, mTimezone.string());

    tm2ctime(&t,this);
    return NOERROR;
}

ECode CTime::ToMillis(
    /* [in] */ Boolean ignoreDst,
    /* [out] */ Int64* ret)
{
    struct tm t;
    if (!ctime2tm(this, &t)) {
        *ret = 0;
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }

    if (ignoreDst) {
        t.tm_isdst = -1;
    }
    int64_t r = mktime_tz(&t, mTimezone.string());
    if (r == -1) {
        *ret = - 1;
        return NOERROR;
    }
    *ret = r * 1000;
    return NOERROR;
}

ECode CTime::Set(
    /* [in] */ Int64 millis)
{
    mAllDay = FALSE;
    struct tm t;

    time_t seconds = millis / 1000;
    localtime_tz(&seconds, &t, mTimezone);

    tm2ctime(&t, this);
    return NOERROR;
}

ECode CTime::Format2445(
    /* [out] */ String* ret)
{
    VALIDATE_NOT_NULL(ret);
    struct tm t;
    if (!ctime2tm(this, &t)) {
        *ret = "";
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    };

    if (!mAllDay) {
        char buf[16];
        format2445_ex(buf, TRUE, &t, mTimezone);
        if (mTimezone.Equals("UTC")) {
            // The letter 'Z' is appended to the end so allow for one
            // more character in the buffer.
            *ret = String(buf, 16);
            return NOERROR;
        }
        else {
            *ret = String(buf, 15);
            return NOERROR;
        }
    }
    else {
        char buf[8];
        format2445_ex(buf, FALSE, &t, mTimezone);
        *ret = String(buf, 8);
        return NOERROR;
    }
}

ECode CTime::Set(
    /* [in] */ ITime* that)
{
    CTime* thatTime = (CTime*)that;
    mTimezone = thatTime->mTimezone;
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

CTime::Set(
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
    VALIDATE_NOT_NULL(ret);
    Int32 result;
    FAIL_RETURN(Compare(this, that, &result));
    *ret = result < 0;
    return NOERROR;
}

ECode CTime::After(
    /* [in] */ ITime* that,
    /* [out] */ Boolean* ret)
{
    VALIDATE_NOT_NULL(ret);
    Int32 result;
    FAIL_RETURN(Compare(this, that, &result));
    *ret = result > 0;
    return NOERROR;
}

ECode CTime::GetWeekNumber(
    /* [out] */ Int32* ret)
{
    VALIDATE_NOT_NULL(ret);
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
    if (allDay) {
        return Format(Y_M_D, ret);
    }
    else if ((ITime::TIMEZONE_UTC).Equals(mTimezone)) {
        return Format(Y_M_D_T_H_M_S_000_Z, ret);
    }
    else {
        String base;
        FAIL_RETURN(Format(Y_M_D_T_H_M_S_000, &base));
        String sign = (mGmtoff < 0) ? String("-") : String("+");
        Int32 offset = (Int32)Elastos::Core::Math::Abs(mGmtoff);
        Int32 minutes = (offset % 3600) / 60;
        Int32 hours = offset / 3600;

        assert(0);
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
    mTimezone = timezone;
    return NOERROR;
}

ECode CTime::GetTimeZone(
    /* [out] */ String* timezone)
{
    VALIDATE_NOT_NULL(timezone);
    *timezone = mTimezone;
    return NOERROR;
}

} // namespace Format
} // namespace Text
} // namepsace Droid
} // namespace Elastos

