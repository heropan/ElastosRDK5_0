
#include "CLocaleData.h"

namespace Libcore {
namespace ICU {

ECode CLocaleData::GetDateFormat(
    /* [in] */ DateFormat style,
    /* [out] */ String* format)
{
    return LocaleData::GetDateFormat(style,format);
}

ECode CLocaleData::GetTimeFormat(
    /* [in] */ DateFormat style,
    /* [out] */ String* format)
{
    return LocaleData::GetTimeFormat(style,format);
}

ECode CLocaleData::GetFirstDayOfWeek(
    /* [out] */ IInteger32** day)
{
    return LocaleData::GetFirstDayOfWeek(day);
}

ECode CLocaleData::GetMinimalDaysInFirstWeek(
    /* [out] */ IInteger32** days)
{
    return LocaleData::GetMinimalDaysInFirstWeek(days);
}

ECode CLocaleData::GetAmPm(
    /* [out, callee] */ ArrayOf<String>** amPm)
{
    return LocaleData::GetAmPm(amPm);
}

ECode CLocaleData::GetEras(
    /* [out, callee] */ ArrayOf<String>** eras)
{
    return LocaleData::GetEras(eras);
}

ECode CLocaleData::GetLongMonthNames(
    /* [out, callee] */ ArrayOf<String>** names)
{
    return LocaleData::GetLongMonthNames(names);
}

ECode CLocaleData::GetShortMonthNames(
    /* [out, callee] */ ArrayOf<String>** names)
{
    return LocaleData::GetShortMonthNames(names);
}

ECode CLocaleData::GetTinyMonthNames(
    /* [out, callee] */ ArrayOf<String>** names)
{
    return LocaleData::GetTinyMonthNames(names);
}

ECode CLocaleData::GetLongStandAloneMonthNames(
    /* [out, callee] */ ArrayOf<String>** names)
{
    return LocaleData::GetLongStandAloneMonthNames(names);
}

ECode CLocaleData::GetShortStandAloneMonthNames(
    /* [out, callee] */ ArrayOf<String>** names)
{
    return LocaleData::GetShortStandAloneMonthNames(names);
}

ECode CLocaleData::GetTinyStandAloneMonthNames(
    /* [out, callee] */ ArrayOf<String>** names)
{
    return LocaleData::GetTinyStandAloneMonthNames(names);
}

ECode CLocaleData::GetLongWeekdayNames(
    /* [out, callee] */ ArrayOf<String>** names)
{
    return LocaleData::GetLongWeekdayNames(names);
}

ECode CLocaleData::GetShortWeekdayNames(
    /* [out, callee] */ ArrayOf<String>** names)
{
    return LocaleData::GetShortWeekdayNames(names);
}

ECode CLocaleData::GetTinyWeekdayNames(
    /* [out, callee] */ ArrayOf<String>** names)
{
    return LocaleData::GetTinyWeekdayNames(names);
}

ECode CLocaleData::GetLongStandAloneWeekdayNames(
    /* [out, callee] */ ArrayOf<String>** names)
{
    return LocaleData::GetLongStandAloneWeekdayNames(names);
}

ECode CLocaleData::GetShortStandAloneWeekdayNames(
    /* [out, callee] */ ArrayOf<String>** names)
{
    return LocaleData::GetShortStandAloneWeekdayNames(names);
}

ECode CLocaleData::GetTinyStandAloneWeekdayNames(
    /* [out, callee] */ ArrayOf<String>** names)
{
    return LocaleData::GetTinyStandAloneWeekdayNames(names);
}

ECode CLocaleData::GetYesterday(
    /* [out] */ String* yesterday)
{
    return LocaleData::GetYesterday(yesterday);
}

ECode CLocaleData::GetToday(
    /* [out] */ String* today)
{
    return LocaleData::GetToday(today);
}

ECode CLocaleData::GetTomorrow(
    /* [out] */ String* tomorrow)
{
    return LocaleData::GetTomorrow(tomorrow);
}

ECode CLocaleData::GetZeroDigit(
    /* [out] */ Char32* zeroDigit)
{
    return LocaleData::GetZeroDigit(zeroDigit);
}

ECode CLocaleData::GetDecimalSeparator(
    /* [out] */ Char32* decimalSeparator)
{
    return LocaleData::GetDecimalSeparator(decimalSeparator);
}

ECode CLocaleData::GetGroupingSeparator(
    /* [out] */ Char32* groupingSeparator)
{
    return LocaleData::GetGroupingSeparator(groupingSeparator);
}

ECode CLocaleData::GetPatternSeparator(
    /* [out] */ Char32* patternSeparator)
{
    return LocaleData::GetPatternSeparator(patternSeparator);
}

ECode CLocaleData::GetPercent(
    /* [out] */ Char32* percent)
{
    return LocaleData::GetPercent(percent);
}

ECode CLocaleData::GetPerMill(
    /* [out] */ Char32* perMill)
{
    return LocaleData::GetPerMill(perMill);
}

ECode CLocaleData::GetMonetarySeparator(
    /* [out] */ Char32* monetarySeparator)
{
    return LocaleData::GetMonetarySeparator(monetarySeparator);
}

ECode CLocaleData::GetMinusSign(
    /* [out] */ Char32* minusSign)
{
    return LocaleData::GetMinusSign(minusSign);
}

ECode CLocaleData::GetExponentSeparator(
    /* [out] */ String* exponentSeparator)
{
    return LocaleData::GetExponentSeparator(exponentSeparator);
}

ECode CLocaleData::GetInfinity(
    /* [out] */ String* infinity)
{
    return LocaleData::GetInfinity(infinity);
}

ECode CLocaleData::GetNaN(
    /* [out] */ String* naN)
{
    return LocaleData::GetNaN(naN);
}

ECode CLocaleData::GetCurrencySymbol(
    /* [out] */ String* currencySymbol)
{
    return LocaleData::GetCurrencySymbol(currencySymbol);
}

ECode CLocaleData::GetInternationalCurrencySymbol(
    /* [out] */ String* internationalCurrencySymbol)
{
    return LocaleData::GetInternationalCurrencySymbol(internationalCurrencySymbol);
}

ECode CLocaleData::GetNumberPattern(
    /* [out] */ String* numberPattern)
{
    return LocaleData::GetNumberPattern(numberPattern);
}

ECode CLocaleData::GetIntegerPattern(
    /* [out] */ String* integerPattern)
{
    return LocaleData::GetIntegerPattern(integerPattern);
}

ECode CLocaleData::GetCurrencyPattern(
    /* [out] */ String* currencyPattern)
{
    return LocaleData::GetCurrencyPattern(currencyPattern);
}

ECode CLocaleData::GetPercentPattern(
    /* [out] */ String* percentPattern)
{
    return LocaleData::GetPercentPattern(percentPattern);
}

} // namespace ICU
} // namespace Libcore
