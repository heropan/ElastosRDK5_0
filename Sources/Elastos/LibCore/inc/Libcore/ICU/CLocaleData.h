
#ifndef __ICU_CLOCALEDATA_H__
#define __ICU_CLOCALEDATA_H__

#include "_CLocaleData.h"
#include "LocaleData.h"

using Elastos::Core::IInteger32;

namespace Libcore {
namespace ICU {

CarClass(CLocaleData) , public LocaleData
{
public:
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
        /* [out, callee] */ ArrayOf<String>** amPm);

    CARAPI GetEras(
        /* [out, callee] */ ArrayOf<String>** eras);

    CARAPI GetLongMonthNames(
        /* [out, callee] */ ArrayOf<String>** names);

    CARAPI GetShortMonthNames(
        /* [out, callee] */ ArrayOf<String>** names);

    CARAPI GetTinyMonthNames(
        /* [out, callee] */ ArrayOf<String>** names);

    CARAPI GetLongStandAloneMonthNames(
        /* [out, callee] */ ArrayOf<String>** names);

    CARAPI GetShortStandAloneMonthNames(
        /* [out, callee] */ ArrayOf<String>** names);

    CARAPI GetTinyStandAloneMonthNames(
        /* [out, callee] */ ArrayOf<String>** names);

    CARAPI GetLongWeekdayNames(
        /* [out, callee] */ ArrayOf<String>** names);

    CARAPI GetShortWeekdayNames(
        /* [out, callee] */ ArrayOf<String>** names);

    CARAPI GetTinyWeekdayNames(
        /* [out, callee] */ ArrayOf<String>** names);

    CARAPI GetLongStandAloneWeekdayNames(
        /* [out, callee] */ ArrayOf<String>** names);

    CARAPI GetShortStandAloneWeekdayNames(
        /* [out, callee] */ ArrayOf<String>** names);

    CARAPI GetTinyStandAloneWeekdayNames(
        /* [out, callee] */ ArrayOf<String>** names);

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
};

} // namespace ICU
} // namespace Libcore

#endif //__ICU_CLOCALEDATA_H__

