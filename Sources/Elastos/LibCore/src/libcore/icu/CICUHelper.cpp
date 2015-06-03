
#include "CICUHelper.h"
#include "ICUUtil.h"

namespace Libcore {
namespace ICU {


ECode CICUHelper::GetISOLanguages(
    /* [out, callee] */ ArrayOf<String>** languages)
{
    VALIDATE_NOT_NULL(languages);

    return ICUUtil::GetISOLanguages(languages);
}

ECode CICUHelper::GetISOCountries(
    /* [out, callee] */ ArrayOf<String>** countries)
{
    VALIDATE_NOT_NULL(countries);

    return ICUUtil::GetISOCountries(countries);
}

ECode CICUHelper::LocaleFromString(
    /* [in] */ const String& localeName,
    /* [out] */ ILocale** locale)
{
    VALIDATE_NOT_NULL(locale);

    return ICUUtil::LocaleFromString(localeName, locale);
}

ECode CICUHelper::LocalesFromStrings(
    /* [in] */ const ArrayOf<String>& localeNames,
    /* [out, callee] */ ArrayOf<ILocale*>** locales)
{
    VALIDATE_NOT_NULL(locales);

    return ICUUtil::LocalesFromStrings(localeNames, locales);
}

ECode CICUHelper::GetAvailableLocales(
    /* [out, callee] */ ArrayOf<ILocale*>** locales)
{
    VALIDATE_NOT_NULL(locales);

    return ICUUtil::GetAvailableLocales(locales);
}

ECode CICUHelper::GetAvailableBreakIteratorLocales(
    /* [out, callee] */ ArrayOf<ILocale*>** locales)
{
    VALIDATE_NOT_NULL(locales);

    return ICUUtil::GetAvailableBreakIteratorLocales(locales);
}

ECode CICUHelper::GetAvailableCalendarLocales(
    /* [out, callee] */ ArrayOf<ILocale*>** locales)
{
    VALIDATE_NOT_NULL(locales);

    return ICUUtil::GetAvailableCalendarLocales(locales);
}

ECode CICUHelper::GetAvailableCollatorLocales(
    /* [out, callee] */ ArrayOf<ILocale*>** locales)
{
    VALIDATE_NOT_NULL(locales);

    return ICUUtil::GetAvailableCollatorLocales(locales);
}

ECode CICUHelper::GetAvailableDateFormatLocales(
    /* [out, callee] */ ArrayOf<ILocale*>** locales)
{
    VALIDATE_NOT_NULL(locales);

    return ICUUtil::GetAvailableDateFormatLocales(locales);
}

ECode CICUHelper::GetAvailableDateFormatSymbolsLocales(
    /* [out, callee] */ ArrayOf<ILocale*>** locales)
{
    VALIDATE_NOT_NULL(locales);

    return ICUUtil::GetAvailableDateFormatSymbolsLocales(locales);
}

ECode CICUHelper::GetAvailableDecimalFormatSymbolsLocales(
    /* [out, callee] */ ArrayOf<ILocale*>** locales)
{
    VALIDATE_NOT_NULL(locales);

    return ICUUtil::GetAvailableDateFormatSymbolsLocales(locales);
}

ECode CICUHelper::GetAvailableNumberFormatLocales(
    /* [out, callee] */ ArrayOf<ILocale*>** locales)
{
    VALIDATE_NOT_NULL(locales);

    return ICUUtil::GetAvailableNumberFormatLocales(locales);
}

ECode CICUHelper::GetIcuVersion(
    /* [out] */ String* icuVersion)
{
    VALIDATE_NOT_NULL(icuVersion);
    *icuVersion = ICUUtil::GetIcuVersion();
    return NOERROR;
}

ECode CICUHelper::GetUnicodeVersion(
    /* [out] */ String* unicodeVersion)
{
    VALIDATE_NOT_NULL(unicodeVersion);
    *unicodeVersion = ICUUtil::GetUnicodeVersion();
    return NOERROR;
}

ECode CICUHelper::ToLowerCase(
    /* [in] */ const String& s,
    /* [in] */ const String& localname,
    /* [out] */ String* ls)
{
    VALIDATE_NOT_NULL(ls);

    *ls = ICUUtil::ToLowerCase(s, localname);
    return NOERROR;
}

ECode CICUHelper::ToUpperCase(
    /* [in] */ const String& s,
    /* [in] */ const String& localname,
    /* [out] */ String* us)
{
    VALIDATE_NOT_NULL(us);

    *us = ICUUtil::ToUpperCase(s, localname);
    return NOERROR;
}

ECode CICUHelper::GetAvailableCurrencyCodes(
    /* [out] */ ArrayOf<String>** codes)
{
    VALIDATE_NOT_NULL(codes);
    return ICUUtil::GetAvailableCurrencyCodes(codes);
}

ECode CICUHelper::GetCurrencyCode(
    /* [in] */ const String& locale,
    /* [out] */ String* currencyCode)
{
    VALIDATE_NOT_NULL(currencyCode);

    *currencyCode = ICUUtil::GetCurrencyCode(locale);
    return NOERROR;
}

ECode CICUHelper::GetCurrencyDisplayName(
    /* [in] */ const String& locale,
    /* [in] */ const String& currencyCode,
    /* [out] */ String* displayName)
{
    VALIDATE_NOT_NULL(displayName);
    *displayName = ICUUtil::GetCurrencyDisplayName(locale, currencyCode);
    return NOERROR;
}

ECode CICUHelper::GetCurrencyFractionDigits(
    /* [in] */ const String& currencyCode,
    /* [out] */ Int32* currencyFractionDigits)
{
    VALIDATE_NOT_NULL(currencyFractionDigits);

    *currencyFractionDigits = ICUUtil::GetCurrencyFractionDigits(currencyCode);
    return NOERROR;
}

ECode CICUHelper::GetCurrencySymbol(
    /* [in] */ const String& locale,
    /* [in] */ const String& currencyCode,
    /* [out] */ String* currencySymbol)
{
    VALIDATE_NOT_NULL(currencySymbol);

    *currencySymbol = ICUUtil::GetCurrencySymbol(locale, currencyCode);
    return NOERROR;
}

ECode CICUHelper::GetDisplayCountry(
    /* [in] */ const String& countryCode,
    /* [in] */ const String& locale,
    /* [out] */ String* displayCountry)
{
    VALIDATE_NOT_NULL(displayCountry);

    *displayCountry = ICUUtil::GetDisplayCountry(countryCode, locale);
    return NOERROR;
}

ECode CICUHelper::GetDisplayLanguage(
    /* [in] */ const String& languageCode,
    /* [in] */ const String& locale,
    /* [out] */ String* displayLanguage)
{
    VALIDATE_NOT_NULL(displayLanguage);

    *displayLanguage = ICUUtil::GetDisplayLanguage(languageCode, locale);
    return NOERROR;
}

ECode CICUHelper::GetDisplayVariant(
    /* [in] */ const String& variantCode,
    /* [in] */ const String& locale,
    /* [out] */ String* displayVariant)
{
    VALIDATE_NOT_NULL(displayVariant);

    *displayVariant = ICUUtil::GetDisplayVariant(variantCode, locale);
    return NOERROR;
}

ECode CICUHelper::GetISO3Country(
    /* [in] */ const String& locale,
    /* [out] */ String* ISO3country)
{
    VALIDATE_NOT_NULL(ISO3country);

    *ISO3country = ICUUtil::GetISO3Country(locale);
    return NOERROR;
}

ECode CICUHelper::GetISO3Language(
    /* [in] */ const String& locale,
    /* [out] */ String* ISO3Language)
{
    VALIDATE_NOT_NULL(ISO3Language);

    *ISO3Language = ICUUtil::GetISO3Language(locale);
    return NOERROR;
}

ECode CICUHelper::AddLikelySubtags(
    /* [in] */ const String& locale,
    /* [out] */ String* result)
{
    VALIDATE_NOT_NULL(result);
    *result = ICUUtil::AddLikelySubtags(locale);
    return NOERROR;
}

ECode CICUHelper::GetScript(
    /* [in] */ const String& locale,
    /* [out] */ String* script)
{
    VALIDATE_NOT_NULL(script);
    *script = ICUUtil::GetScript(locale);
    return NOERROR;
}

} // namespace ICU
} // namespace Libcore
