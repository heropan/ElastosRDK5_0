#ifndef __IO_LOCALEDATA_H__
#define __IO_LOCALEDATA_H__

#include "cmdef.h"
#include <Elastos.CoreLibrary_server.h>
#include <elastos/HashMap.h>
#include <elastos/Mutex.h>
#include "CInteger32.h"

using Elastos::Core::Mutex;
using Elastos::Core::IInteger32;
using Elastos::Core::CInteger32;
using Elastos::Utility::HashMap;

namespace Libcore {
namespace ICU {

/**
 * Passes locale-specific from ICU native code to Java.
 * <p>
 * Note that you share these; you must not alter any of the fields, nor their array elements
 * in the case of arrays. If you ever expose any of these things to user code, you must give
 * them a clone rather than the original.
 */
class LocaleData
{
public:
    LocaleData();

    ~LocaleData();

    static CARAPI_(AutoPtr<ILocaleData>) Get(
        /* [in] */ ILocale* locale);

    CARAPI_(String) ToString();

    CARAPI GetDateFormat(
        /* [in] */ DateFormat style,
        /* [out] */ String* format);

    CARAPI GetTimeFormat(
        /* [in] */ DateFormat style,
        /* [out] */ String* format);

    CARAPI GetFirstDayOfWeek(
        /* [out] */ IInteger32** day);

    CARAPI GetMinimalDaysInFirstWeek(
        /* [out] */ IInteger32** days);

    CARAPI GetAmPm(
        /* [out] */ ArrayOf<String>** amPm);

    CARAPI GetEras(
        /* [out] */ ArrayOf<String>** eras);

    CARAPI GetLongMonthNames(
        /* [out] */ ArrayOf<String>** names);

    CARAPI GetShortMonthNames(
        /* [out] */ ArrayOf<String>** names);

    CARAPI GetTinyMonthNames(
        /* [out] */ ArrayOf<String>** names);

    CARAPI GetLongStandAloneMonthNames(
        /* [out] */ ArrayOf<String>** names);

    CARAPI GetShortStandAloneMonthNames(
        /* [out] */ ArrayOf<String>** names);

    CARAPI GetTinyStandAloneMonthNames(
        /* [out] */ ArrayOf<String>** names);

    CARAPI GetLongWeekdayNames(
        /* [out] */ ArrayOf<String>** names);

    CARAPI GetShortWeekdayNames(
        /* [out] */ ArrayOf<String>** names);

    CARAPI GetTinyWeekdayNames(
        /* [out] */ ArrayOf<String>** names);

    CARAPI GetLongStandAloneWeekdayNames(
        /* [out] */ ArrayOf<String>** names);

    CARAPI GetShortStandAloneWeekdayNames(
        /* [out] */ ArrayOf<String>** names);

    CARAPI GetTinyStandAloneWeekdayNames(
        /* [out] */ ArrayOf<String>** names);

    CARAPI GetYesterday(
        /* [out] */ String* yesterday);

    CARAPI GetToday(
        /* [out] */ String* today);

    CARAPI GetTomorrow(
        /* [out] */ String* tomorrow);

    CARAPI GetZeroDigit(
        /* [out] */ Char32* zeroDigit);

    CARAPI GetDecimalSeparator(
        /* [out] */ Char32* decimalSeparator);

    CARAPI GetGroupingSeparator(
        /* [out] */ Char32* groupingSeparator);

    CARAPI GetPatternSeparator(
        /* [out] */ Char32* patternSeparator);

    CARAPI GetPercent(
        /* [out] */ Char32* percent);

    CARAPI GetPerMill(
        /* [out] */ Char32* perMill);

    CARAPI GetMonetarySeparator(
        /* [out] */ Char32* monetarySeparator);

    CARAPI GetMinusSign(
        /* [out] */ Char32* minusSign);

    CARAPI GetExponentSeparator(
        /* [out] */ String* exponentSeparator);

    CARAPI GetInfinity(
        /* [out] */ String* infinity);

    CARAPI GetNaN(
        /* [out] */ String* naN);

    CARAPI GetCurrencySymbol(
        /* [out] */ String* currencySymbol);

    CARAPI GetInternationalCurrencySymbol(
        /* [out] */ String* internationalCurrencySymbol);

    CARAPI GetNumberPattern(
        /* [out] */ String* numberPattern);

    CARAPI GetIntegerPattern(
        /* [out] */ String* integerPattern);

    CARAPI GetCurrencyPattern(
        /* [out] */ String* currencyPattern);

    CARAPI GetPercentPattern(
        /* [out] */ String* percentPattern);

private:
    static CARAPI_(AutoPtr<ILocaleData>) InitLocaleData(
        /* [in] */ ILocale* locale);

    static Boolean sIsInited;

    static Boolean Init();
public:
    // Used by Calendar.
    AutoPtr<IInteger32> mFirstDayOfWeek;
    AutoPtr<IInteger32> mMinimalDaysInFirstWeek;

    // Used by DateFormatSymbols.
    AutoPtr<ArrayOf<String> > mAmPm; // "AM", "PM".
    AutoPtr<ArrayOf<String> > mEras; // "BC", "AD".

    AutoPtr<ArrayOf<String> > mLongMonthNames; // "January", ...
    AutoPtr<ArrayOf<String> > mShortMonthNames; // "Jan", ...
    AutoPtr<ArrayOf<String> > mTinyMonthNames; // "J", ...
    AutoPtr<ArrayOf<String> > mLongStandAloneMonthNames; // "January", ...
    AutoPtr<ArrayOf<String> > mShortStandAloneMonthNames; // "Jan", ...
    AutoPtr<ArrayOf<String> > mTinyStandAloneMonthNames; // "J", ...

    AutoPtr<ArrayOf<String> > mLongWeekdayNames; // "Sunday", ...
    AutoPtr<ArrayOf<String> > mShortWeekdayNames; // "Sun", ...
    AutoPtr<ArrayOf<String> > mTinyWeekdayNames; // "S", ...
    AutoPtr<ArrayOf<String> > mLongStandAloneWeekdayNames; // "Sunday", ...
    AutoPtr<ArrayOf<String> > mShortStandAloneWeekdayNames; // "Sun", ...
    AutoPtr<ArrayOf<String> > mTinyStandAloneWeekdayNames; // "S", ...

    // Used by frameworks/base DateSorter and DateUtils.
    String mYesterday; // "Yesterday".
    String mToday; // "Today".
    String mTomorrow; // "Tomorrow".

    String mFullTimeFormat;
    String mLongTimeFormat;
    String mMediumTimeFormat;
    String mShortTimeFormat;

    String mFullDateFormat;
    String mLongDateFormat;
    String mMediumDateFormat;
    String mShortDateFormat;

    // Used by DecimalFormatSymbols.
    Char32 mZeroDigit;
    Char32 mDecimalSeparator;
    Char32 mGroupingSeparator;
    Char32 mPatternSeparator;
    Char32 mPercent;
    Char32 mPerMill;
    Char32 mMonetarySeparator;
    Char32 mMinusSign;
    String mExponentSeparator;
    String mInfinity;
    String mNaN;
    // Also used by Currency.
    String mCurrencySymbol;
    String mInternationalCurrencySymbol;

    // Used by DecimalFormat and NumberFormat.
    String mNumberPattern;
    String mIntegerPattern;
    String mCurrencyPattern;
    String mPercentPattern;

    // A cache for the locale-specific data.
private:
    static HashMap< String, AutoPtr<ILocaleData> > sLocaleDataCache;
    static Mutex sLocaleDataCacheLock;
};

} // namespace ICU
} // namespace Libcore

#endif //__ICU_LOCALEDATA_H__
