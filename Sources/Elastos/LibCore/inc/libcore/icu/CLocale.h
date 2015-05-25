
#ifndef __ICU_CLOCALE_H__
#define __ICU_CLOCALE_H__

#include "_CLocale.h"

namespace Libcore {
namespace ICU {

CarClass(CLocale)
{
public:
    /**
     * There's a circular dependency between toLowerCase/toUpperCase and
     * Locale.US. Work around this by avoiding these methods when constructing
     * the built-in locales.
     *
     * @param unused required for this constructor to have a unique signature
     */
    CARAPI constructor(
        /* [in] */ Boolean unused,
        /* [in] */ const String& lowerCaseLanguageCode,
        /* [in] */ const String& upperCaseCountryCode);

    CARAPI constructor();

    /**
     * Constructs a new {@code Locale} using the specified language.
     */
    CARAPI constructor(
        /* [in] */ const String& language);

    /**
     * Constructs a new {@code Locale} using the specified language and country codes.
     */
    CARAPI constructor(
        /* [in] */ const String& language,
        /* [in] */ const String& country);

    /**
     * Constructs a new {@code Locale} using the specified language, country,
     * and variant codes.
     */
    CARAPI constructor(
        /* [in] */ const String& language,
        /* [in] */ const String& country,
        /* [in] */ const String& variant);

    CARAPI Clone(
        /* [out] */ ILocale** newObj);

    CARAPI Equals(
        /* [in] */ IInterface* other,
        /* [out] */ Boolean* result);

    /**
     * Returns the system's installed locales. This array always includes {@code
     * Locale.US}, and usually several others. Most locale-sensitive classes
     * offer their own {@code getAvailableLocales} method, which should be
     * preferred over this general purpose method.
     *
     * @see java.text.BreakIterator#getAvailableLocales()
     * @see java.text.Collator#getAvailableLocales()
     * @see java.text.DateFormat#getAvailableLocales()
     * @see java.text.DateFormatSymbols#getAvailableLocales()
     * @see java.text.DecimalFormatSymbols#getAvailableLocales()
     * @see java.text.NumberFormat#getAvailableLocales()
     * @see java.util.Calendar#getAvailableLocales()
     */
    static CARAPI GetAvailableLocales(
        /* [out] */ ArrayOf<ILocale*>** locales);

    /**
     * Returns the country code for this locale, or {@code ""} if this locale
     * doesn't correspond to a specific country.
     */
    CARAPI GetCountry(
        /* [out] */ String* country);

    /**
     * Returns the user's preferred locale. This may have been overridden for
     * this process with {@link #setDefault}.
     *
     * <p>Since the user's locale changes dynamically, avoid caching this value.
     * Instead, use this method to look it up for each use.
     */
    static CARAPI_(AutoPtr<ILocale>) GetDefault();

    /**
     * Equivalent to {@code getDisplayCountry(Locale.getDefault())}.
     */
    CARAPI GetDisplayCountry(
        /* [out] */ String* country);

     /**
     * Returns the name of this locale's country, localized to {@code locale}.
     * Returns the empty string if this locale does not correspond to a specific
     * country.
     */
    CARAPI GetDisplayCountryEx(
        /* [in] */ ILocale* locale,
        /* [out] */ String* country);

    /**
     * Equivalent to {@code getDisplayLanguage(Locale.getDefault())}.
     */
    CARAPI GetDisplayLanguage(
        /* [out] */ String* language);

    /**
     * Returns the name of this locale's language, localized to {@code locale}.
     * If the language name is unknown, the language code is returned.
     */
    CARAPI GetDisplayLanguageEx(
        /* [in] */ ILocale* locale,
        /* [out] */ String* language);

    /**
     * Equivalent to {@code getDisplayName(Locale.getDefault())}.
     */
    CARAPI GetDisplayName(
        /* [out] */ String* name);

    /**
     * Returns this locale's language name, country name, and variant, localized
     * to {@code locale}. The exact output form depends on whether this locale
     * corresponds to a specific language, country and variant, such as:
     * {@code English}, {@code English (United States)}, {@code English (United
     * States,Computer)}, {@code anglais (&#x00c9;tats-Unis)}, {@code anglais
     * (&#x00c9;tats-Unis,informatique)}.
     */
    CARAPI GetDisplayNameEx(
        /* [in] */ ILocale* locale,
        /* [out] */ String* name);

    /**
     * Gets the full variant name in the default {@code Locale} for the variant code of
     * this {@code Locale}. If there is no matching variant name, the variant code is
     * returned.
     *
     * @return a variant name.
     */
    CARAPI GetDisplayVariant(
        /* [out] */ String* variantName);

    /**
     * Gets the full variant name in the specified {@code Locale} for the variant code
     * of this {@code Locale}. If there is no matching variant name, the variant code is
     * returned.
     *
     * @param locale
     *            the {@code Locale} for which the display name is retrieved.
     * @return a variant name.
     */
    CARAPI GetDisplayVariantEx(
        /* [in] */ ILocale* locale,
        /* [out] */ String* variantName);

     /**
     * Gets the three letter ISO country code which corresponds to the country
     * code for this {@code Locale}.
     *
     * @return a three letter ISO language code.
     * @throws MissingResourceException
     *                if there is no matching three letter ISO country code.
     */
    CARAPI GetISO3Country(
        /* [out] */ String* country);

    /**
     * Gets the three letter ISO language code which corresponds to the language
     * code for this {@code Locale}.
     *
     * @return a three letter ISO language code.
     * @throws MissingResourceException
     *                if there is no matching three letter ISO language code.
     */
    CARAPI GetISO3Language(
        /* [out] */ String* language);

    /**
     * Gets the list of two letter ISO country codes which can be used as the
     * country code for a {@code Locale}.
     *
     * @return an array of strings.
     */
    static CARAPI GetISOCountries(
        /* [out] */ ArrayOf<String>** codes);

    /**
     * Gets the list of two letter ISO language codes which can be used as the
     * language code for a {@code Locale}.
     *
     * @return an array of strings.
     */
    static CARAPI GetISOLanguages(
        /* [out] */ ArrayOf<String>** codes);

    /**
     * Gets the language code for this {@code Locale} or the empty string of no language
     * was set.
     *
     * @return a language code.
     */
    CARAPI GetLanguage(
        /* [out] */ String* language);

    /**
     * Gets the variant code for this {@code Locale} or an empty {@code String} if no variant
     * was set.
     *
     * @return a variant code.
     */
    CARAPI GetVariant(
        /* [out] */ String* variant);

    /**
     * Overrides the default locale. This does not affect system configuration,
     * and attempts to override the system-provided default locale may
     * themselves be overridden by actual changes to the system configuration.
     * Code that calls this method is usually incorrect, and should be fixed by
     * passing the appropriate locale to each locale-sensitive method that's
     * called.
     */
    static CARAPI SetDefault(
        /* [in] */ ILocale* locale);

    /**
     * Returns the string representation of this {@code Locale}. It consists of the
     * language code, country code and variant separated by underscores.
     * If the language is missing the string begins
     * with an underscore. If the country is missing there are 2 underscores
     * between the language and the variant. The variant cannot stand alone
     * without a language and/or country code: in this case this method would
     * return the empty string.
     *
     * <p>Examples: "en", "en_US", "_US", "en__POSIX", "en_US_POSIX"
     *
     * @return the string representation of this {@code Locale}.
     */
    //@Override
    CARAPI ToString(
        /* [out] */ String* str);

    //@Override
    CARAPI GetHashCode(
        /* [out] */ Int32* value);

public:
    /**
     * Locale constant for en_CA.
     */
    static const AutoPtr<ILocale> CANADA;

    /**
     * Locale constant for fr_CA.
     */
    static const AutoPtr<ILocale> CANADA_FRENCH;

    /**
     * Locale constant for zh_CN.
     */
    static const AutoPtr<ILocale> CHINA;

    /**
     * Locale constant for zh.
     */
    static const AutoPtr<ILocale> CHINESE;

    /**
     * Locale constant for en.
     */
    static const AutoPtr<ILocale> ENGLISH;

    /**
     * Locale constant for fr_FR.
     */
    static const AutoPtr<ILocale> FRANCE;

    /**
     * Locale constant for fr.
     */
    static const AutoPtr<ILocale> FRENCH;

    /**
     * Locale constant for de.
     */
    static const AutoPtr<ILocale> GERMAN;

    /**
     * Locale constant for de_DE.
     */
    static const AutoPtr<ILocale> GERMANY;

    /**
     * Locale constant for it.
     */
    static const AutoPtr<ILocale> ITALIAN;

    /**
     * Locale constant for it_IT.
     */
    static const AutoPtr<ILocale> ITALY;

    /**
     * Locale constant for ja_JP.
     */
    static const AutoPtr<ILocale> JAPAN;

    /**
     * Locale constant for ja.
     */
    static const AutoPtr<ILocale> JAPANESE;

    /**
     * Locale constant for ko_KR.
     */
    static const AutoPtr<ILocale> KOREA;

    /**
     * Locale constant for ko.
     */
    static const AutoPtr<ILocale> KOREAN;

    /**
     * Locale constant for zh_CN.
     */
    static const AutoPtr<ILocale> PRC;

    /**
     * Locale constant for the root locale. The root locale has an empty language,
     * country, and variant.
     *
     * @since 1.6
     */
    static const AutoPtr<ILocale> ROOT;

    /**
     * Locale constant for zh_CN.
     */
    static const AutoPtr<ILocale> SIMPLIFIED_CHINESE;

    /**
     * Locale constant for zh_TW.
     */
    static const AutoPtr<ILocale> TAIWAN;

    /**
     * Locale constant for zh_TW.
     */
    static const AutoPtr<ILocale> TRADITIONAL_CHINESE;

    /**
     * Locale constant for en_GB.
     */
    static const AutoPtr<ILocale> UK;

    /**
     * Locale constant for en_US.
     */
    static const AutoPtr<ILocale> US;

private:
    CARAPI_(String) ToNewString();

private:
    // Initialize a default which is used during static
    // initialization of the default for the platform.
    static AutoPtr<ILocale> sDefaultLocale;
    static Boolean sIsInited;

    String mCountryCode;
    String mLanguageCode;
    String mVariantCode;
    String mCachedToStringResult;
};

} // namespace ICU
} // namespace Libcore

#endif //__ICU_CLOCALE_H__
