
#include "ElStringByteSink.h"
#include "ICUUtil.h"
#include "CLocale.h"

#include <unicode/ucat.h>
#include <unicode/ureslocs.h>
#include <unicode/calendar.h>
#include <unicode/datefmt.h>
#include <unicode/dcfmtsym.h>
#include <unicode/decimfmt.h>
#include <unicode/dtfmtsym.h>
#include <unicode/gregocal.h>
#include <unicode/locid.h>
#include <unicode/numfmt.h>
#include <unicode/strenum.h>
#include <unicode/ubrk.h>
#include <unicode/ucal.h>
#include <unicode/uclean.h>
#include <unicode/ucol.h>
#include <unicode/ucurr.h>
#include <unicode/udat.h>
#include <unicode/ustring.h>
#include <unicode/unum.h>
#include <elastos/Logger.h>
#include <elastos/StringBuilder.h>
#include <elastos/UniquePtr.h>

using Elastos::Core::UniquePtr;

namespace Libcore {
namespace ICU {

// TODO: put this in a header file and use it everywhere!
// DISALLOW_COPY_AND_ASSIGN disallows the copy and operator= functions.
// It goes in the private: declarations in a class.
#define DISALLOW_COPY_AND_ASSIGN(TypeName) \
    TypeName(const TypeName&); \
    void operator=(const TypeName&)

class ScopedResourceBundle
{
public:
    ScopedResourceBundle(UResourceBundle* bundle)
        : mBundle(bundle)
    {}

    ~ScopedResourceBundle()
    {
        if (mBundle != NULL) {
            ures_close(mBundle);
        }
    }

    UResourceBundle* get()
    {
        return mBundle;
    }

    Boolean hasKey(const char* key) {
        UErrorCode status = U_ZERO_ERROR;
        ures_getStringByKey(mBundle, key, NULL, &status);
        return U_SUCCESS(status);
      }

private:
    UResourceBundle* mBundle;
    DISALLOW_COPY_AND_ASSIGN(ScopedResourceBundle);
};

Locale GetLocale(
    /* [in] */ const String& localeName)
{
    return Locale::createFromName(localeName.string());
}

AutoPtr<ArrayOf<String> > ICUUtil::sIsoLanguages = NULL;
AutoPtr<ArrayOf<String> > ICUUtil::sIsoCountries = NULL;
AutoPtr<ArrayOf<ILocale *> > ICUUtil::sAvailableLocalesCache = NULL;

ECode ICUUtil::GetISOLanguages(
    /* [out, callee] */ ArrayOf<String>** languages)
{
    VALIDATE_NOT_NULL(languages);

    if (sIsoLanguages == NULL) {
        sIsoLanguages = GetISOLanguagesNative();
    }
    *languages = sIsoLanguages->Clone();
    REFCOUNT_ADD(*languages);
    return NOERROR;
}

ECode ICUUtil::GetISOCountries(
    /* [out, callee] */ ArrayOf<String>** countries)
{
    VALIDATE_NOT_NULL(countries);

    if (sIsoCountries == NULL) {
        sIsoCountries = GetISOCountriesNative();
    }
    *countries = sIsoCountries->Clone();
    REFCOUNT_ADD(*countries);
    return NOERROR;
}

ECode ICUUtil::LocaleFromString(
    /* [in] */ const String& localeName,
    /* [out] */ ILocale** locale)
{
    VALIDATE_NOT_NULL(locale);
    *locale = NULL;

    Int32 first = localeName.IndexOf('_');
    Int32 second = localeName.IndexOf('_', first + 1);
    AutoPtr<CLocale> outloc;
    if (first == -1) {
        // Language only ("ja").
        FAIL_RETURN(CLocale::NewByFriend(localeName, (CLocale**)&outloc));
    }
    else if (second == -1) {
        // Language and country ("ja_JP").
        FAIL_RETURN(CLocale::NewByFriend(localeName.Substring(0, first),
                localeName.Substring(first + 1), (CLocale**)&outloc));
    }
    else {
        // Language and country and variant ("ja_JP_TRADITIONAL").
        FAIL_RETURN(CLocale::NewByFriend(localeName.Substring(0, first),
                localeName.Substring(first + 1, second), localeName.Substring(second + 1),
                (CLocale**)&outloc));
    }

    *locale = (ILocale*) outloc.Get();
    REFCOUNT_ADD(*locale)
    return NOERROR;
}

ECode ICUUtil::LocalesFromStrings(
    /* [in] */ const ArrayOf<String>& localeNames,
    /* [out, callee] */ ArrayOf<ILocale*>** locales)
{
    VALIDATE_NOT_NULL(locales);

    AutoPtr<ArrayOf<ILocale*> > temp = ArrayOf<ILocale*>::Alloc(localeNames.GetLength());
    for (Int32 i = 0; i < localeNames.GetLength(); ++i) {
        FAIL_RETURN(LocaleFromString(localeNames[i], &(*temp)[i]));
    }
    *locales = temp;
    REFCOUNT_ADD(*locales)
    return NOERROR;
}

ECode ICUUtil::GetAvailableLocales(
    /* [out, callee] */ ArrayOf<ILocale*>** locales)
{
    VALIDATE_NOT_NULL(locales);

    if (sAvailableLocalesCache == NULL) {
        AutoPtr<ArrayOf<String> > _locales;
        FAIL_RETURN(GetAvailableLocalesNative((ArrayOf<String>**)&_locales));
        FAIL_RETURN(LocalesFromStrings(*_locales, (ArrayOf<ILocale*>**)&sAvailableLocalesCache));
    }
    AutoPtr<ArrayOf<ILocale*> > temp = ArrayOf<ILocale*>::Alloc(sAvailableLocalesCache->GetLength());
    for (Int32 i = 0; i < sAvailableLocalesCache->GetLength(); i++) {
        temp->Set(i, (*sAvailableLocalesCache)[i]);
    }
    *locales = temp;
    REFCOUNT_ADD(*locales)
    return NOERROR;
}

ECode ICUUtil::GetAvailableBreakIteratorLocales(
    /* [out, callee] */ ArrayOf<ILocale*>** locales)
{
    VALIDATE_NOT_NULL(locales);

    AutoPtr<ArrayOf<String> > _locales;
    FAIL_RETURN(GetAvailableBreakIteratorLocalesNative((ArrayOf<String>**)&_locales));
    return LocalesFromStrings(*_locales, locales);
}

ECode ICUUtil::GetAvailableCalendarLocales(
    /* [out, callee] */ ArrayOf<ILocale*>** locales)
{
    VALIDATE_NOT_NULL(locales);

    AutoPtr<ArrayOf<String> > _locales;
    FAIL_RETURN(GetAvailableCalendarLocalesNative((ArrayOf<String>**)&_locales));
    return LocalesFromStrings(*_locales, locales);
}

ECode ICUUtil::GetAvailableCollatorLocales(
    /* [out, callee] */ ArrayOf<ILocale*>** locales)
{
    VALIDATE_NOT_NULL(locales);

    AutoPtr<ArrayOf<String> > _locales;
    FAIL_RETURN(GetAvailableCollatorLocalesNative((ArrayOf<String>**)&_locales));
    return LocalesFromStrings(*_locales, locales);
}

ECode ICUUtil::GetAvailableDateFormatLocales(
    /* [out, callee] */ ArrayOf<ILocale*>** locales)
{
    VALIDATE_NOT_NULL(locales);

    AutoPtr<ArrayOf<String> > _locales;
    FAIL_RETURN(GetAvailableDateFormatLocalesNative((ArrayOf<String>**)&_locales));
    return LocalesFromStrings(*_locales, locales);
}

ECode ICUUtil::GetAvailableDateFormatSymbolsLocales(
    /* [out, callee] */ ArrayOf<ILocale*>** locales)
{
    VALIDATE_NOT_NULL(locales);
    return GetAvailableDateFormatLocales(locales);
}

ECode ICUUtil::GetAvailableDecimalFormatSymbolsLocales(
    /* [out, callee] */ ArrayOf<ILocale*>** locales)
{
    VALIDATE_NOT_NULL(locales);
    return GetAvailableNumberFormatLocales(locales);
}

ECode ICUUtil::GetAvailableNumberFormatLocales(
    /* [out, callee] */ ArrayOf<ILocale*>** locales)
{
    VALIDATE_NOT_NULL(locales);

    AutoPtr<ArrayOf<String> > _locales;
    FAIL_RETURN(GetAvailableNumberFormatLocalesNative((ArrayOf<String>**)&_locales));
    return LocalesFromStrings(*_locales, locales);
}

static String versionString(
    /* [in] */ const UVersionInfo& version)
{
    char versionString[U_MAX_VERSION_STRING_LENGTH];
    u_versionToString(const_cast<UVersionInfo&>(version), &versionString[0]);
    return String(versionString);
}

String ICUUtil::GetIcuVersion()
{
    UVersionInfo icuVersion;
    u_getVersion(icuVersion);
    return versionString(icuVersion);
}

String ICUUtil::GetUnicodeVersion()
{
    UVersionInfo unicodeVersion;
    u_getUnicodeVersion(unicodeVersion);
    return versionString(unicodeVersion);
}

String ICUUtil::ToLowerCase(
    /* [in] */ const String& s,
    /* [in] */ ILocale* locale)
{
    assert(locale);
    String tag;
    locale->ToLanguageTag(&tag);
    UnicodeString us = UnicodeString::fromUTF8(s.string());
    UnicodeString original(us);
    us.toLower(Locale::createFromName(tag.string()));
    if (us == original) {
        return s;
    }
    else {
        String str("");
        ElStringByteSink sink(&str);
        us.toUTF8(sink);
        return str;
    }
}

String ICUUtil::ToUpperCase(
    /* [in] */ const String& s,
    /* [in] */ ILocale* locale)
{
    assert(locale);
    String tag;
    locale->ToLanguageTag(&tag);
    UnicodeString us = UnicodeString::fromUTF8(s.string());
    UnicodeString original(us);
    us.toUpper(Locale::createFromName(tag.string()));
    if (us == original) {
        return s;
    }
    else {
        String str("");
        ElStringByteSink sink(&str);
        us.toUTF8(sink);
        return str;
    }
}

template <typename Counter, typename Getter>
static ECode getAvailableLocales(
    /* [in] */ Counter* counter,
    /* [in] */ Getter* getter,
    /* [out, callee] */ ArrayOf<String>** locales)
{
    VALIDATE_NOT_NULL(locales)

    Int32 count = (*counter)();
    *locales = ArrayOf<String>::Alloc(count);
    REFCOUNT_ADD(*locales);
    for (int32_t i = 0; i < count; ++i) {
        (**locales)[i] = (*getter)(i);
    }
    return NOERROR;
}

ECode ICUUtil::GetAvailableBreakIteratorLocalesNative(
    /* [out, callee] */ ArrayOf<String>** locales)
{
    return getAvailableLocales(ubrk_countAvailable, ubrk_getAvailable, locales);
}

ECode ICUUtil::GetAvailableCalendarLocalesNative(
    /* [out, callee] */ ArrayOf<String>** locales)
{
    return getAvailableLocales(ucal_countAvailable, ucal_getAvailable, locales);
}

ECode ICUUtil::GetAvailableCollatorLocalesNative(
    /* [out, callee] */ ArrayOf<String>** locales)
{
    return getAvailableLocales(ucol_countAvailable, ucol_getAvailable, locales);
}

ECode ICUUtil::GetAvailableDateFormatLocalesNative(
    /* [out, callee] */ ArrayOf<String>** locales)
{
    return getAvailableLocales(udat_countAvailable, udat_getAvailable, locales);
}

ECode ICUUtil::GetAvailableLocalesNative(
    /* [out, callee] */ ArrayOf<String>** locales)
{
    return getAvailableLocales(uloc_countAvailable, uloc_getAvailable, locales);
}

ECode ICUUtil::GetAvailableNumberFormatLocalesNative(
    /* [out, callee] */ ArrayOf<String>** locales)
{
    VALIDATE_NOT_NULL(locales);

    return getAvailableLocales(unum_countAvailable, unum_getAvailable, locales);
}

static ECode maybeThrowIcuException(UErrorCode errorCode)
{
    const char* message = u_errorName(errorCode);
    if (errorCode <= U_ZERO_ERROR || errorCode >= U_ERROR_LIMIT) {
        return NOERROR;
    }

    switch (errorCode) {
        case U_ILLEGAL_ARGUMENT_ERROR:
            return E_ILLEGAL_ARGUMENT_EXCEPTION;
        case U_INDEX_OUTOFBOUNDS_ERROR:
        case U_BUFFER_OVERFLOW_ERROR:
            return E_ARRAY_INDEX_OUT_OF_BOUNDS_EXCEPTION;
        case U_UNSUPPORTED_ERROR:
            return E_UNSUPPORTED_OPERATION_EXCEPTION;
        default:
            return E_RUNTIME_EXCEPTION;
    }
}

class EnumerationCounter
{
public:
    const size_t count;
    EnumerationCounter(size_t count) : count(count) {}
    size_t operator()() { return count; }
};

class EnumerationGetter
{
public:
    UEnumeration* e;
    UErrorCode* status;
    EnumerationGetter(UEnumeration* e, UErrorCode* status) : e(e), status(status) {}
    const char* operator()(int32_t charCount)
    {
        UnicodeString us = uenum_unext(e, &charCount, status);
        String s("");
        ElStringByteSink sink(&s);
        us.toUTF8(sink);
        return (const char*)s;
    }
};

ECode ICUUtil::GetAvailableCurrencyCodes(
    /* [out, callee] */ ArrayOf<String>** codes)
{
    UErrorCode status = U_ZERO_ERROR;
    UEnumeration* e(ucurr_openISOCurrencies(UCURR_COMMON | UCURR_NON_DEPRECATED, &status));
    EnumerationCounter counter(uenum_count(e, &status));
    if (FAILED(maybeThrowIcuException(status))) {
        *codes = NULL;
        return NOERROR;
    }
    EnumerationGetter getter(e, &status);
    getAvailableLocales(&counter, &getter, codes);
    // jobject result = toStringArray16(env, &counter, &getter);
    ECode ec = maybeThrowIcuException(status);
    uenum_close(e);
    return ec;
}

String ICUUtil::GetCurrencyCode(
    /* [in] */ const String& locale)
{
    UErrorCode status = U_ZERO_ERROR;
    ScopedResourceBundle supplData(ures_openDirect(U_ICUDATA_CURR, "supplementalData", &status));
    if (U_FAILURE(status)) {
        return String(NULL);
    }

    ScopedResourceBundle currencyMap(ures_getByKey(supplData.get(), "CurrencyMap", NULL, &status));
    if (U_FAILURE(status)) {
        return String(NULL);
    }

    ScopedResourceBundle currency(ures_getByKey(currencyMap.get(), locale.string(), NULL, &status));
    if (U_FAILURE(status)) {
        return String(NULL);
    }

    ScopedResourceBundle currencyElem(ures_getByIndex(currency.get(), 0, NULL, &status));
    if (U_FAILURE(status)) {
        return String("None");
    }

    // check if there is a 'to' date. If there is, the currency isn't used anymore.
    ScopedResourceBundle currencyTo(ures_getByKey(currencyElem.get(), "to", NULL, &status));
    if (!U_FAILURE(status)) {
        // return and let the caller throw an exception
        return String(NULL);
    }
    // Ignore the failure to find a 'to' date.
    status = U_ZERO_ERROR;

    ScopedResourceBundle currencyId(ures_getByKey(currencyElem.get(), "id", NULL, &status));
    if (U_FAILURE(status)) {
        // No id defined for this country
        return String("None");
    }

    char buffer[256];
    Int32 length;
    ures_getUTF8String(currencyId.get(), buffer, &length, TRUE, &status);
    if (U_FAILURE(status) || length == 0) {
        return String("None");
    }
    else {
        buffer[length] = '\0';
        return String(buffer);
    }
}

String ICUUtil::GetCurrencyDisplayName(
    /* [in] */ const String& locale,
    /* [in] */ const String& currencyCode)
{
    UnicodeString icuCurrencyCode = UnicodeString::fromUTF8(currencyCode.string());
    UErrorCode status = U_ZERO_ERROR;
    UBool isChoiceFormat;
    int32_t charCount;
    const UChar* chars = ucurr_getName(icuCurrencyCode.getTerminatedBuffer(), locale.string(),
            UCURR_LONG_NAME, &isChoiceFormat, &charCount, &status);
    if (status == U_USING_DEFAULT_WARNING) {
        // ICUUtil's default is English. We want the ISO 4217 currency code instead.
        chars = icuCurrencyCode.getBuffer();
        charCount = icuCurrencyCode.length();
    }
    if (charCount == 0) {
        return String(NULL);
    }
    else {
        String s("");
        ElStringByteSink sink(&s);
        UnicodeString(chars).toUTF8(sink);
        return s;
    }
}

Int32 ICUUtil::GetCurrencyFractionDigits(
    /* [in] */ const String& currencyCode)
{
    UnicodeString icuCurrencyCode = UnicodeString::fromUTF8(currencyCode.string());
    UErrorCode status = U_ZERO_ERROR;
    return ucurr_getDefaultFractionDigits(icuCurrencyCode.getTerminatedBuffer(), &status);
}

String ICUUtil::GetCurrencySymbol(
    /* [in] */ const String& locale,
    /* [in] */ const String& currencyCode)
{
    // We can't use ucurr_getName because it doesn't distinguish between using data root from
    // the root locale and parroting back the input because it's never heard of the currency code.
    UErrorCode status = U_ZERO_ERROR;
    ScopedResourceBundle currLoc(ures_open(U_ICUDATA_CURR, locale.string(), &status));
    if (U_FAILURE(status)) {
        return String(NULL);
    }

    ScopedResourceBundle currencies(ures_getByKey(currLoc.get(), "Currencies", NULL, &status));
    if (U_FAILURE(status)) {
        return String(NULL);
    }

    ScopedResourceBundle currencyElems(ures_getByKey(currencies.get(), currencyCode.string(), NULL, &status));
    if (U_FAILURE(status)) {
        return String(NULL);
    }

    char buffer[256];
    Int32 length = 256;
    ures_getUTF8StringByIndex(currencyElems.get(), 0, buffer, &length, TRUE, &status);
    if (U_FAILURE(status) || length == 0) {
        return String(NULL);
    }
    else {
        buffer[length] = '\0';
        return String(buffer);
    }
}

String ICUUtil::GetDisplayCountry(
    /* [in] */ const String& countryCode,
    /* [in] */ const String& locale)
{
    Locale loc = GetLocale(locale);
    Locale targetLoc = GetLocale(countryCode);
    UnicodeString str;
    targetLoc.getDisplayCountry(loc, str);

    String s("");
    ElStringByteSink sink(&s);
    str.toUTF8(sink);
    return s;
}

String ICUUtil::GetDisplayLanguage(
    /* [in] */ const String& languageCode,
    /* [in] */ const String& locale)
{
    Locale loc = GetLocale(locale);
    Locale targetLoc = GetLocale(languageCode);
    UnicodeString str;
    targetLoc.getDisplayLanguage(loc, str);

    String s("");
    ElStringByteSink sink(&s);
    str.toUTF8(sink);
    return s;
}

String ICUUtil::GetDisplayVariant(
    /* [in] */ const String& variantCode,
    /* [in] */ const String& locale)
{
    Locale loc = GetLocale(locale);
    Locale targetLoc = GetLocale(variantCode);
    UnicodeString str;
    targetLoc.getDisplayVariant(loc, str);

    String s("");
    ElStringByteSink sink(&s);
    str.toUTF8(sink);
    return s;
}

String ICUUtil::GetISO3Country(
    /* [in] */ const String& locale)
{
    Locale loc = GetLocale(locale);
    return String(loc.getISO3Country());
}

String ICUUtil::GetISO3Language(
    /* [in] */ const String& locale)
{
    Locale loc = GetLocale(locale);
    return String(loc.getISO3Language());
}

String ICUUtil::AddLikelySubtags(
    /* [in] */ const String& locale)
{
    UErrorCode status = U_ZERO_ERROR;
    char maximizedLocaleID[ULOC_FULLNAME_CAPACITY];
    uloc_addLikelySubtags(locale.string(), maximizedLocaleID, sizeof(maximizedLocaleID), &status);
    if (U_FAILURE(status)) {
        return locale;
    }
    return String(maximizedLocaleID);
}

String ICUUtil::GetScript(
    /* [in] */ const String& locale)
{
    UErrorCode status = U_ZERO_ERROR;
    char script[ULOC_SCRIPT_CAPACITY];
    uloc_getScript(locale.string(), script, sizeof(script), &status);
    if (U_FAILURE(status)) {
        return String(NULL);
    }
    return String(script);
}

static AutoPtr<ArrayOf<String> > ToStringArray(const char* const* strings)
{
    Int32 count = 0;
    while (strings[count] != NULL) {
        ++count;
    }
    AutoPtr<ArrayOf<String> > result = ArrayOf<String>::Alloc(count);
    for (Int32 i = 0; i < count; ++i) {
        (*result)[i] = strings[i];
    }
    return result;
}

AutoPtr<ArrayOf<String> > ICUUtil::GetISOLanguagesNative()
{
    return ToStringArray(Locale::getISOLanguages());
}

AutoPtr<ArrayOf<String> > ICUUtil::GetISOCountriesNative()
{
    return ToStringArray(Locale::getISOCountries());
}

static void getIntegerField(
    /* [in] */ Int32 value,
    /* [out] */ IInteger32** field)
{
    CInteger32::New(value, field);
}

static void getStringField(
    /* [in] */ UResourceBundle* bundle,
    /* [in] */ Int32 index,
    /* [out] */ String* field)
{
    UErrorCode status = U_ZERO_ERROR;
    Int32 charCount;
    UnicodeString chars = ures_getStringByIndex(bundle, index, &charCount, &status);
    if (U_SUCCESS(status)) {
        String s("");
        ElStringByteSink sink(&s);
        chars.toUTF8(sink);
        *field = s;
    }
    else {
       PFL_EX("Error setting String field %d from ICUUtil resource: %s", index, u_errorName(status));
    }
}

static void getStringField(
    /* [in] */ UResourceBundle* bundle,
    /* [in] */ const char* key,
    /* [out] */ String* field)
{
    UErrorCode status = U_ZERO_ERROR;
    Int32 charCount;
    UnicodeString chars = ures_getStringByKey(bundle, key, &charCount, &status);
    if (U_SUCCESS(status)) {
        String s("");
        ElStringByteSink sink(&s);
        chars.toUTF8(sink);
        *field = s;
    }
    else {
       PFL_EX("Error setting String field %d from ICUUtil resource: %s", key, u_errorName(status));
    }
}

static void getStringArrayField(
    /* [in] */ const UnicodeString* valueArray,
    /* [in] */ int32_t size,
    /* [out] */ ArrayOf<String> ** field)
{
    AutoPtr<ArrayOf<String> > result = ArrayOf<String>::Alloc(size);
    for (int32_t i = 0; i < size ; i++) {
        String str("");
        ElStringByteSink sink(&str);
        valueArray[i].toUTF8(sink);
        (*result)[i] = str;
    }
    *field = result;
    REFCOUNT_ADD(*field);
}

static void getCharField(
    /* [in] */ const UnicodeString& value,
    /* [out] */ Char32* field)
{
    if (value.length() == 0) {
        return;
    }
    *field = (Char32)value.charAt(0);
}

static void getStringField(
    /* [in] */ const UnicodeString& value,
    /* [out] */ String* field)
{
    String str("");
    ElStringByteSink sink(&str);
    value.toUTF8(sink);
    *field = str;
}

static void setNumberPatterns(
    /* [in] */ LocaleData* localeData,
    /* [in] */ const String& localeName)
{
    UErrorCode status = U_ZERO_ERROR;
    Locale localeObj = GetLocale(localeName);

    UnicodeString pattern;
    UniquePtr<DecimalFormat> fmt(static_cast<DecimalFormat*>(
            NumberFormat::createInstance(localeObj, UNUM_CURRENCY, status)));
    pattern = fmt->toPattern(pattern.remove());
    getStringField(pattern, &localeData->mCurrencyPattern);

    fmt.reset(static_cast<DecimalFormat*>(NumberFormat::createInstance(localeObj, UNUM_DECIMAL, status)));
    pattern = fmt->toPattern(pattern.remove());
    getStringField(pattern, &localeData->mNumberPattern);

    fmt.reset(static_cast<DecimalFormat*>(NumberFormat::createInstance(localeObj, UNUM_PERCENT, status)));
    pattern = fmt->toPattern(pattern.remove());
    getStringField(pattern, &localeData->mPercentPattern);
}

static void setDecimalFormatSymbolsData(
    /* [in] */ LocaleData* localeData,
    /* [in] */ const String& localeName)
{
    UErrorCode status = U_ZERO_ERROR;
    Locale localeObj = GetLocale(localeName);
    DecimalFormatSymbols dfs(localeObj, status);

    getCharField(dfs.getSymbol(DecimalFormatSymbols::kDecimalSeparatorSymbol), &localeData->mDecimalSeparator);
    getCharField(dfs.getSymbol(DecimalFormatSymbols::kGroupingSeparatorSymbol), &localeData->mGroupingSeparator);
    getCharField(dfs.getSymbol(DecimalFormatSymbols::kPatternSeparatorSymbol), &localeData->mPatternSeparator);
    getCharField(dfs.getSymbol(DecimalFormatSymbols::kPercentSymbol), &localeData->mPercent);
    getCharField(dfs.getSymbol(DecimalFormatSymbols::kPerMillSymbol), &localeData->mPerMill);
    getCharField(dfs.getSymbol(DecimalFormatSymbols::kMonetarySeparatorSymbol), &localeData->mMonetarySeparator);
    getCharField(dfs.getSymbol(DecimalFormatSymbols:: kMinusSignSymbol), &localeData->mMinusSign);
    getStringField(dfs.getSymbol(DecimalFormatSymbols::kExponentialSymbol), &localeData->mExponentSeparator);
    getStringField(dfs.getSymbol(DecimalFormatSymbols::kInfinitySymbol), &localeData->mInfinity);
    getStringField(dfs.getSymbol(DecimalFormatSymbols::kNaNSymbol), &localeData->mNaN);
    getCharField(dfs.getSymbol(DecimalFormatSymbols::kZeroDigitSymbol), &localeData->mZeroDigit);
}

// Iterates up through the locale hierarchy. So "en_US" would return "en_US", "en", "".
class LocaleNameIterator {
 public:
  LocaleNameIterator(const char* locale_name, UErrorCode& status) : status_(status), has_next_(true) {
    strcpy(locale_name_, locale_name);
    locale_name_length_ = strlen(locale_name_);
  }

  const char* Get() {
      return locale_name_;
  }

  bool HasNext() {
    return has_next_;
  }

  void Up() {
    locale_name_length_ = uloc_getParent(locale_name_, locale_name_, sizeof(locale_name_), &status_);
    if (locale_name_length_ == 0) {
      has_next_ = false;
    }
  }

 private:
  UErrorCode& status_;
  bool has_next_;
  char locale_name_[ULOC_FULLNAME_CAPACITY];
  int32_t locale_name_length_;

  DISALLOW_COPY_AND_ASSIGN(LocaleNameIterator);
};

static Boolean getDateTimePatterns(
    /* [in] */ LocaleData* localeData,
    /* [in] */ const char* locale_name)
{
  UErrorCode status = U_ZERO_ERROR;
  ScopedResourceBundle root(ures_open(NULL, locale_name, &status));
  if (U_FAILURE(status)) {
    return FALSE;
  }
  ScopedResourceBundle calendar(ures_getByKey(root.get(), "calendar", NULL, &status));
  if (U_FAILURE(status)) {
    return FALSE;
  }
  ScopedResourceBundle gregorian(ures_getByKey(calendar.get(), "gregorian", NULL, &status));
  if (U_FAILURE(status)) {
    return FALSE;
  }
  ScopedResourceBundle dateTimePatterns(ures_getByKey(gregorian.get(), "DateTimePatterns", NULL, &status));
  if (U_FAILURE(status)) {
    return FALSE;
  }
  getStringField(dateTimePatterns.get(), 0, &localeData->mFullTimeFormat);
  getStringField(dateTimePatterns.get(), 1, &localeData->mLongTimeFormat);
  getStringField(dateTimePatterns.get(), 2, &localeData->mMediumTimeFormat);
  getStringField(dateTimePatterns.get(), 3, &localeData->mShortTimeFormat);
  getStringField(dateTimePatterns.get(), 4, &localeData->mFullDateFormat);
  getStringField(dateTimePatterns.get(), 5, &localeData->mLongDateFormat);
  getStringField(dateTimePatterns.get(), 6, &localeData->mMediumDateFormat);
  getStringField(dateTimePatterns.get(), 7, &localeData->mShortDateFormat);
  return TRUE;
}

static Boolean getYesterdayTodayAndTomorrow(
    /* [in] */ LocaleData* localeData,
    /* [in] */ const char* locale_name)
{
  UErrorCode status = U_ZERO_ERROR;
  ScopedResourceBundle root(ures_open(NULL, locale_name, &status));
  if (U_FAILURE(status)) {
    return FALSE;
  }
  ScopedResourceBundle calendar(ures_getByKey(root.get(), "calendar", NULL, &status));
  if (U_FAILURE(status)) {
    return FALSE;
  }
  ScopedResourceBundle gregorian(ures_getByKey(calendar.get(), "gregorian", NULL, &status));
  if (U_FAILURE(status)) {
    return FALSE;
  }
  ScopedResourceBundle fields(ures_getByKey(gregorian.get(), "fields", NULL, &status));
  if (U_FAILURE(status)) {
    return FALSE;
  }
  ScopedResourceBundle day(ures_getByKey(fields.get(), "day", NULL, &status));
  if (U_FAILURE(status)) {
    return FALSE;
  }
  ScopedResourceBundle relative(ures_getByKey(day.get(), "relative", NULL, &status));
  if (U_FAILURE(status)) {
    return FALSE;
  }
  // bn_BD only has a "-2" entry.
  if (relative.hasKey("-1") && relative.hasKey("0") && relative.hasKey("1")) {
    getStringField(relative.get(), "-1", &localeData->mYesterday);
    getStringField(relative.get(), "0", &localeData->mToday);
    getStringField(relative.get(), "1", &localeData->mTomorrow);
    return TRUE;
  }
  return FALSE;
}

Boolean ICUUtil::InitLocaleDataImpl(
    /* [in] */ const String& localeName,
    /* [in] */ LocaleData* localeData)
{
    if (localeName.string() == NULL) {
        return FALSE;
    }
    if (localeName.GetByteLength() >= ULOC_FULLNAME_CAPACITY) {
        return FALSE; // ICUUtil has a fixed-length limit.
    }

    // Get the DateTimePatterns.
    UErrorCode status = U_ZERO_ERROR;
    Boolean foundDateTimePatterns = FALSE;
    for (LocaleNameIterator it(localeName.string(), status); it.HasNext(); it.Up()) {
      if (getDateTimePatterns(localeData, it.Get())) {
          foundDateTimePatterns = TRUE;
          break;
      }
    }
    if (!foundDateTimePatterns) {
        // ALOGE("Couldn't find ICUUtil DateTimePatterns for %s", localeName.c_str());
        return FALSE;
    }

    // Get the "Yesterday", "Today", and "Tomorrow" strings.
    Boolean foundYesterdayTodayAndTomorrow = FALSE;
    for (LocaleNameIterator it(localeName.string(), status); it.HasNext(); it.Up()) {
      if (getYesterdayTodayAndTomorrow(localeData, it.Get())) {
        foundYesterdayTodayAndTomorrow = TRUE;
        break;
      }
    }
    if (!foundYesterdayTodayAndTomorrow) {
      // ALOGE("Couldn't find ICUUtil yesterday/today/tomorrow for %s", localeName.c_str());
      return FALSE;
    }

    status = U_ZERO_ERROR;
    Locale localeObj = GetLocale(localeName);
    UniquePtr<Calendar> cal(Calendar::createInstance(localeObj, status));
    if (U_FAILURE(status)) {
        return FALSE;
    }

    localeData->mMinimalDaysInFirstWeek = NULL;
    localeData->mFirstDayOfWeek = NULL;
    getIntegerField(cal->getFirstDayOfWeek(), (IInteger32**)&localeData->mFirstDayOfWeek);
    getIntegerField(cal->getMinimalDaysInFirstWeek(),
            (IInteger32**)&localeData->mMinimalDaysInFirstWeek);

    // Get DateFormatSymbols.
    status = U_ZERO_ERROR;
    DateFormatSymbols dateFormatSym(localeObj, status);
    if (U_FAILURE(status)) {
        return FALSE;
    }

    // Get AM/PM and BC/AD.
    int32_t count = 0;
    const UnicodeString* amPmStrs = dateFormatSym.getAmPmStrings(count);
    localeData->mAmPm = NULL;
    getStringArrayField(amPmStrs, count, (ArrayOf<String> **)&(localeData->mAmPm));

    const UnicodeString* erasStrs = dateFormatSym.getEras(count);
    localeData->mEras = NULL;
    getStringArrayField(erasStrs, count, (ArrayOf<String> **)&localeData->mEras);

    localeData->mLongMonthNames = NULL;
    const UnicodeString* longMonthNames =
            dateFormatSym.getMonths(count, DateFormatSymbols::FORMAT, DateFormatSymbols::WIDE);
    getStringArrayField(longMonthNames, count, (ArrayOf<String> **)&localeData->mLongMonthNames);

    localeData->mShortMonthNames = NULL;
    const UnicodeString* shortMonthNames =
            dateFormatSym.getMonths(count, DateFormatSymbols::FORMAT, DateFormatSymbols::ABBREVIATED);
    getStringArrayField(shortMonthNames, count, (ArrayOf<String> **)&localeData->mShortMonthNames);

    localeData->mTinyMonthNames = NULL;
    const UnicodeString* tinyMonthNames =
            dateFormatSym.getMonths(count, DateFormatSymbols::FORMAT, DateFormatSymbols::NARROW);
    getStringArrayField(tinyMonthNames, count, (ArrayOf<String> **)&localeData->mTinyMonthNames);

    localeData->mLongWeekdayNames = NULL;
    const UnicodeString* longWeekdayNames =
            dateFormatSym.getWeekdays(count, DateFormatSymbols::FORMAT, DateFormatSymbols::WIDE);
    getStringArrayField(longWeekdayNames, count, (ArrayOf<String> **)&localeData->mLongWeekdayNames);

    localeData->mShortWeekdayNames = NULL;
    const UnicodeString* shortWeekdayNames =
            dateFormatSym.getWeekdays(count, DateFormatSymbols::FORMAT, DateFormatSymbols::ABBREVIATED);
    getStringArrayField(shortWeekdayNames, count, (ArrayOf<String> **)&localeData->mShortWeekdayNames);

    localeData->mTinyWeekdayNames = NULL;
    const UnicodeString* tinyWeekdayNames =
            dateFormatSym.getWeekdays(count, DateFormatSymbols::FORMAT, DateFormatSymbols::NARROW);
    getStringArrayField(tinyWeekdayNames, count, (ArrayOf<String> **)&localeData->mTinyWeekdayNames);

    localeData->mLongStandAloneMonthNames = NULL;
    const UnicodeString* longStandAloneMonthNames =
            dateFormatSym.getMonths(count, DateFormatSymbols::STANDALONE, DateFormatSymbols::WIDE);
    getStringArrayField(longStandAloneMonthNames, count, (ArrayOf<String> **)&localeData->mLongStandAloneMonthNames);

    localeData->mShortStandAloneMonthNames = NULL;
    const UnicodeString* shortStandAloneMonthNames =
            dateFormatSym.getMonths(count, DateFormatSymbols::STANDALONE, DateFormatSymbols::ABBREVIATED);
    getStringArrayField(shortStandAloneMonthNames, count, (ArrayOf<String> **)&localeData->mShortStandAloneMonthNames);

    localeData->mTinyStandAloneMonthNames = NULL;
    const UnicodeString* tinyStandAloneMonthNames =
            dateFormatSym.getMonths(count, DateFormatSymbols::STANDALONE, DateFormatSymbols::NARROW);
    getStringArrayField(tinyStandAloneMonthNames, count, (ArrayOf<String> **)&localeData->mTinyStandAloneMonthNames);

    localeData->mLongStandAloneWeekdayNames = NULL;
    const UnicodeString* longStandAloneWeekdayNames =
            dateFormatSym.getWeekdays(count, DateFormatSymbols::STANDALONE, DateFormatSymbols::WIDE);
    getStringArrayField(longStandAloneWeekdayNames, count, (ArrayOf<String> **)&localeData->mLongStandAloneWeekdayNames);

    localeData->mShortStandAloneWeekdayNames = NULL;
    const UnicodeString* shortStandAloneWeekdayNames =
            dateFormatSym.getWeekdays(count, DateFormatSymbols::STANDALONE, DateFormatSymbols::ABBREVIATED);
    getStringArrayField(shortStandAloneWeekdayNames, count, (ArrayOf<String> **)&localeData->mShortStandAloneWeekdayNames);

    localeData->mTinyStandAloneWeekdayNames = NULL;
    const UnicodeString* tinyStandAloneWeekdayNames =
            dateFormatSym.getWeekdays(count, DateFormatSymbols::STANDALONE, DateFormatSymbols::NARROW);
    getStringArrayField(tinyStandAloneWeekdayNames, count, (ArrayOf<String> **)&localeData->mTinyStandAloneWeekdayNames);

    status = U_ZERO_ERROR;

    // For numberPatterns and symbols.
    setNumberPatterns(localeData, localeName);
    setDecimalFormatSymbolsData(localeData, localeName);

    String countryCode(Locale::createFromName(localeName.string()).getCountry());
    String internationalCurrencySymbol = ICUUtil::GetCurrencyCode(countryCode);

    String currencySymbol = String(NULL);
    if (!internationalCurrencySymbol.IsNull()) {
        currencySymbol = ICUUtil::GetCurrencySymbol(localeName, internationalCurrencySymbol);
    }
    else {
        internationalCurrencySymbol = String("None");
    }
    if (currencySymbol.IsNull()) {
        // This is the UTF-8 encoding of U+00A4 (CURRENCY SIGN).
        currencySymbol = String("\xc2\xa4");
    }
    localeData->mCurrencySymbol = currencySymbol;
    localeData->mInternationalCurrencySymbol = internationalCurrencySymbol;

    return TRUE;
}

} // namespace ICU
} // namespace Libcore
