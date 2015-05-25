
#ifndef __ICU_CICUHELPER_H__
#define __ICU_CICUHELPER_H__

#include "_CICUHelper.h"

namespace Libcore {
namespace ICU {

CarClass(CICUHelper)
{
public:
    CARAPI GetISOLanguages(
        /* [out, callee] */ ArrayOf<String>** languages);

    CARAPI GetISOCountries(
        /* [out, callee] */ ArrayOf<String>** countries);

    CARAPI LocaleFromString(
        /* [in] */ const String& localeName,
        /* [out] */ ILocale** locale);

    CARAPI LocalesFromStrings(
        /* [in] */ const ArrayOf<String>& localeNames,
        /* [out, callee] */ ArrayOf<ILocale*>** locales);

    CARAPI GetAvailableLocales(
        /* [out, callee] */ ArrayOf<ILocale*>** locales);

    CARAPI GetAvailableBreakIteratorLocales(
        /* [out, callee] */ ArrayOf<ILocale*>** locales);

    CARAPI GetAvailableCalendarLocales(
        /* [out, callee] */ ArrayOf<ILocale*>** locales);

    CARAPI GetAvailableCollatorLocales(
        /* [out, callee] */ ArrayOf<ILocale*>** locales);

    CARAPI GetAvailableDateFormatLocales(
        /* [out, callee] */ ArrayOf<ILocale*>** locales);

    CARAPI GetAvailableDateFormatSymbolsLocales(
        /* [out, callee] */ ArrayOf<ILocale*>** locales);

    CARAPI GetAvailableDecimalFormatSymbolsLocales(
        /* [out, callee] */ ArrayOf<ILocale*>** locales);

    CARAPI GetAvailableNumberFormatLocales(
        /* [out, callee] */ ArrayOf<ILocale*>** locales);

    /**
     * Returns the ICU version in use. This is "4.4" for gingerbread, for example.
     */
    CARAPI GetIcuVersion(
        /* [out] */ String* icuVersion);

    /**
     * Returns the Unicode version our ICU supports. This is "5.2" for gingerbread, for example.
     */
    CARAPI GetUnicodeVersion(
        /* [out] */ String* unicodeVersion);

    CARAPI ToLowerCase(
        /* [in] */ const String& s,
        /* [in] */ const String& localname,
        /* [out] */ String* ls);

    CARAPI ToUpperCase(
        /* [in] */ const String& s,
        /* [in] */ const String& localname,
        /* [out] */ String* us);

    CARAPI GetAvailableCurrencyCodes(
        /* [out, callee] */ ArrayOf<String>** codes);

    CARAPI GetCurrencyCode(
        /* [in] */ const String& locale,
        /* [out] */ String* currencyCode);

    CARAPI GetCurrencyDisplayName(
        /* [in] */ const String& locale,
        /* [in] */ const String& currencyCode,
        /* [out] */ String* displayName);

    CARAPI GetCurrencyFractionDigits(
        /* [in] */ const String& currencyCode,
        /* [out] */ Int32* currencyFractionDigits);

    CARAPI GetCurrencySymbol(
        /* [in] */ const String& locale,
        /* [in] */ const String& currencyCode,
        /* [out] */ String* currencySymbol);

    CARAPI GetDisplayCountry(
        /* [in] */ const String& countryCode,
        /* [in] */ const String& locale,
        /* [out] */ String* displayCountry);

    CARAPI GetDisplayLanguage(
        /* [in] */ const String& languageCode,
        /* [in] */ const String& locale,
        /* [out] */ String* displayLanguage);

    CARAPI GetDisplayVariant(
        /* [in] */ const String& variantCode,
        /* [in] */ const String& locale,
        /* [out] */ String* displayVariant);

    CARAPI GetISO3Country(
        /* [in] */ const String& locale,
        /* [out] */ String* ISO3country);

    CARAPI GetISO3Language(
        /* [in] */ const String& locale,
        /* [out] */ String* ISO3Language);

    CARAPI AddLikelySubtags(
        /* [in] */ const String& locale,
        /* [out] */ String* result);

    CARAPI GetScript(
        /* [in] */ const String& locale,
        /* [out] */ String* script);
};

} // namespace ICU
} // namespace Libcore

#endif // __ICU_CICUHELPER_H__
