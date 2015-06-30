
#include "CICUHelper.h"
#include "ICUUtil.h"

namespace Libcore {
namespace ICU {

CAR_SINGLETON_IMPL(CICUHelper)

CAR_INTERFACE_IMPL(CICUHelper, Singleton, IICUHelper)

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

ECode CICUHelper::LocaleFromIcuLocaleId(
    /* [in] */ const String& localeId,
    /* [out] */ ILocale** locale)
{
    VALIDATE_NOT_NULL(locale);

    return ICUUtil::LocaleFromIcuLocaleId(localeId, locale);
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

ECode CICUHelper::GetBestDateTimePattern(
    /* [in] */ const String& skeleton,
    /* [in] */ ILocale* locale,
    /* [out] */ String* rst)
{
    VALIDATE_NOT_NULL(locale);
    VALIDATE_NOT_NULL(rst);

    return ICUUtil::GetBestDateTimePattern(skeleton, locale, rst);
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
    /* [in] */ const String& languageTag,
    /* [out] */ String* ls)
{
    VALIDATE_NOT_NULL(ls);

    *ls = ICUUtil::ToLowerCase(s, languageTag);
    return NOERROR;
}

ECode CICUHelper::ToUpperCase(
    /* [in] */ const String& s,
    /* [in] */ const String& languageTag,
    /* [out] */ String* us)
{
    VALIDATE_NOT_NULL(us);

    *us = ICUUtil::ToUpperCase(s, languageTag);
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
    /* [in] */ ILocale* locale,
    /* [in] */ const String& currencyCode,
    /* [out] */ String* displayName)
{
    VALIDATE_NOT_NULL(locale);
    VALIDATE_NOT_NULL(displayName);
    
    return ICUUtil::GetCurrencyDisplayName(locale, currencyCode, displayName);
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
        /* [in] */ ILocale* locale,
        /* [in] */ const String& currencyCode,
        /* [out] */ String* currencySymbol)
{
    VALIDATE_NOT_NULL(locale);
    VALIDATE_NOT_NULL(currencySymbol);

    return ICUUtil::GetCurrencySymbol(locale, currencyCode, currencySymbol);
}

ECode CICUHelper::GetDisplayCountry(
        /* [in] */ ILocale* targetLocale,
        /* [in] */ ILocale* locale,
        /* [out] */ String* displayCountry)
{
    VALIDATE_NOT_NULL(targetLocale);
    VALIDATE_NOT_NULL(locale);
    VALIDATE_NOT_NULL(displayCountry);

    return ICUUtil::GetDisplayCountry(targetLocale, locale, displayCountry);
}

ECode CICUHelper::GetDisplayLanguage(
        /* [in] */ ILocale* targetLocale,
        /* [in] */ ILocale* locale,
        /* [out] */ String* displayLanguage)
{
    VALIDATE_NOT_NULL(targetLocale);
    VALIDATE_NOT_NULL(locale);
    VALIDATE_NOT_NULL(displayLanguage);

    return ICUUtil::GetDisplayLanguage(targetLocale, locale, displayLanguage);
}

ECode CICUHelper::GetDisplayVariant(
    /* [in] */ ILocale* variant,
    /* [in] */ ILocale* locale,
    /* [out] */ String* displayVariant)
{
    VALIDATE_NOT_NULL(variant)
    VALIDATE_NOT_NULL(locale)
    VALIDATE_NOT_NULL(displayVariant)

    *displayVariant = ICUUtil::GetDisplayVariant(variant, locale);
    return NOERROR;
}

ECode CICUHelper::GetDisplayScript(
    /* [in] */ ILocale* targetLocale,
    /* [in] */ ILocale* locale,
    /* [out] */ String* script)
{
    VALIDATE_NOT_NULL(targetLocale)
    VALIDATE_NOT_NULL(locale)
    VALIDATE_NOT_NULL(script)

    *script = ICUUtil::GetDisplayScript(targetLocale, locale);
    return NOERROR;
}

ECode CICUHelper::GetISO3Country(
    /* [in] */ const String& languageTag,
    /* [out] */ String* ISO3country)
{
    VALIDATE_NOT_NULL(ISO3country);

    *ISO3country = ICUUtil::GetISO3Country(languageTag);
    return NOERROR;
}

ECode CICUHelper::GetISO3Language(
    /* [in] */ const String& languageTag,
    /* [out] */ String* ISO3Language)
{
    VALIDATE_NOT_NULL(ISO3Language);

    *ISO3Language = ICUUtil::GetISO3Language(languageTag);
    return NOERROR;
}

ECode CICUHelper::AddLikelySubtags(
    /* [in] */ const String& locale,
    /* [out] */ String* result)
{
    VALIDATE_NOT_NULL(result)

    *result = ICUUtil::AddLikelySubtags(locale);
}

ECode CICUHelper::AddLikelySubtags(
    /* [in] */ ILocale* locale,
    /* [out] */ ILocale** target)
{
    VALIDATE_NOT_NULL(locale)
    VALIDATE_NOT_NULL(*target);

    *target = ICUUtil::AddLikelySubtags(locale);
    return NOERROR;
}

ECode CICUHelper::GetDateFormatOrder(
    /* [in] */ const String& pattern,
    /* [out, callee] */ ArrayOf<Char32>** locales)
{
    VALIDATE_NOT_NULL(*locales);
    return ICUUtil::GetDateFormatOrder(pattern, locales);
}

ECode CICUHelper::GetCldrVersion(
    /* [out] */ String* cldrVersion)
{
    VALIDATE_NOT_NULL(cldrVersion);
    return ICUUtil::GetCldrVersion(cldrVersion);
}

ECode CICUHelper::ToLowerCase(
        /* [in] */ const String& s,
        /* [in] */ ILocale* locale,
        /* [out] */ String* ls)
{
    VALIDATE_NOT_NULL(locale);
    VALIDATE_NOT_NULL(ls);

    String languageTag;
    locale->ToLanguageTag(&languageTag);
    return ToLowerCase(s, languageTag, ls);
}

ECode CICUHelper::ToUpperCase(
    /* [in] */ const String& s,
    /* [in] */ ILocale* locale,
    /* [out] */ String* us)
{
    VALIDATE_NOT_NULL(locale);
    VALIDATE_NOT_NULL(us);

    String languageTag;
    locale->ToLanguageTag(&languageTag);
    return ToUpperCase(s, languageTag, us);
}

ECode CICUHelper::GetCurrencyNumericCode(
    /* [in] */ const String& currencyCode,
    /* [out] */ Int32* currencyFractionDigits)
{
    VALIDATE_NOT_NULL(currencyFractionDigits)

    return GetCurrencyNumericCode(currencyCode, currencyFractionDigits);
}

ECode CICUHelper::GetScript(
    /* [in] */ const String& locale,
    /* [out] */ String* script)
{
    VALIDATE_NOT_NULL(script);
    *script = ICUUtil::GetScript(locale);
    return NOERROR;
}

ECode CICUHelper::SetDefaultLocale(
    /* [in] */ const String& languageTag)
{
    return ICUUtil::SetDefaultLocale(languageTag);
}

ECode CICUHelper::GetDefaultLocale(
    /* [out] */ String* defaultLocale)
{
    VALIDATE_NOT_NULL(defaultLocale)

    return ICUUtil::GetDefaultLocale(defaultLocale);
}

ECode CICUHelper::LocalesFromStrings(
    /* [in] */ ArrayOf<String>* localeNames,
    /* [out, callee] */ ArrayOf<ILocale*>** locales)
{
    VALIDATE_NOT_NULL(localeNames)
    VALIDATE_NOT_NULL(*locales)

    return ICUUtil::LocalesFromStrings(*localeNames, locales);
}

} // namespace ICU
} // namespace Libcore
