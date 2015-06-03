
#include "CLocale.h"
#include "ICUUtil.h"
#include <elastos/StringBuilder.h>
#include <elastos/CSystem.h>

using Elastos::Core::CSystem;
using Elastos::Core::StringBuilder;

namespace Libcore {
namespace ICU {

static AutoPtr<ILocale> CreateLocale(
    /* [in] */ const String& language,
    /* [in] */ const String& country)
{
    AutoPtr<CLocale> l;
    ASSERT_SUCCEEDED(CLocale::NewByFriend(TRUE, language, country, (CLocale**)&l));
    return (ILocale*)l.Get();
}

const AutoPtr<ILocale> CLocale::CANADA = CreateLocale(String("en"), String("CA"));
const AutoPtr<ILocale> CLocale::CANADA_FRENCH = CreateLocale(String("fr"), String("CA"));
const AutoPtr<ILocale> CLocale::CHINA = CreateLocale(String("zh"), String("CN"));
const AutoPtr<ILocale> CLocale::CHINESE = CreateLocale(String("zh"), String(""));
const AutoPtr<ILocale> CLocale::ENGLISH = CreateLocale(String("en"), String(""));
const AutoPtr<ILocale> CLocale::FRANCE = CreateLocale(String("fr"), String("FR"));
const AutoPtr<ILocale> CLocale::FRENCH = CreateLocale(String("fr"), String(""));
const AutoPtr<ILocale> CLocale::GERMAN = CreateLocale(String("de"), String(""));
const AutoPtr<ILocale> CLocale::GERMANY = CreateLocale(String("de"), String("DE"));
const AutoPtr<ILocale> CLocale::ITALIAN = CreateLocale(String("it"), String(""));
const AutoPtr<ILocale> CLocale::ITALY = CreateLocale(String("it"), String("IT"));
const AutoPtr<ILocale> CLocale::JAPAN = CreateLocale(String("ja"), String("JP"));
const AutoPtr<ILocale> CLocale::JAPANESE = CreateLocale(String("ja"), String(""));
const AutoPtr<ILocale> CLocale::KOREA = CreateLocale(String("ko"), String("KR"));
const AutoPtr<ILocale> CLocale::KOREAN = CreateLocale(String("ko"), String(""));
const AutoPtr<ILocale> CLocale::PRC = CreateLocale(String("zh"), String("CN"));
const AutoPtr<ILocale> CLocale::ROOT = CreateLocale(String(""), String(""));
const AutoPtr<ILocale> CLocale::SIMPLIFIED_CHINESE = CreateLocale(String("zh"), String("CN"));
const AutoPtr<ILocale> CLocale::TAIWAN = CreateLocale(String("zh"), String("TW"));
const AutoPtr<ILocale> CLocale::TRADITIONAL_CHINESE = CreateLocale(String("zh"), String("TW"));
const AutoPtr<ILocale> CLocale::UK = CreateLocale(String("en"), String("GB"));
const AutoPtr<ILocale> CLocale::US = CreateLocale(String("en"), String("US"));

AutoPtr<ILocale> CLocale::sDefaultLocale = CLocale::US;

Boolean CLocale::sIsInited = FALSE;

ECode CLocale::constructor(
    /* [in] */ Boolean unused,
    /* [in] */ const String& lowerCaseLanguageCode,
    /* [in] */ const String& upperCaseCountryCode)
{
    mLanguageCode = lowerCaseLanguageCode;
    mCountryCode = upperCaseCountryCode;
    mVariantCode = String("");
    return NOERROR;
}

ECode CLocale::constructor()
{
    mLanguageCode = "en";
    mCountryCode = "US";
    mVariantCode = "";
    return NOERROR;
}

ECode CLocale::constructor(
    /* [in] */ const String& language)
{
    return constructor(language, String(""), String(""));
}

ECode CLocale::constructor(
    /* [in] */ const String& language,
    /* [in] */ const String& country)
{
    return constructor(language, country, String(""));
}

ECode CLocale::constructor(
    /* [in] */ const String& language,
    /* [in] */ const String& country,
    /* [in] */ const String& variant)
{
    if (language.IsNull() || country.IsNull() || variant.IsNull()) {
        // throw new NullPointerException();
        return E_NULL_POINTER_EXCEPTION;
    }
    if (language.IsEmpty() && country.IsEmpty()){
        mLanguageCode = "";
        mCountryCode = "";
        mVariantCode = variant;
        return NOERROR;
    }

    StringBuilder sbl(language);
    sbl.ToASCIILowerCase();
    mLanguageCode = sbl.ToString();
    // Map new language codes to the obsolete language
    // codes so the correct resource bundles will be used.
    if (mLanguageCode.Equals("he")) {
        mLanguageCode = "iw";
    }
    else if (mLanguageCode.Equals("id")) {
        mLanguageCode = "in";
    }
    else if (mLanguageCode.Equals("yi")) {
        mLanguageCode = "ji";
    }

    // countryCode = country.toUpperCase(Locale.US);
    StringBuilder sbc(country);
    sbc.ToASCIIUpperCase();
    mCountryCode = sbc.ToString();

    // Work around for be compatible with RI
    mVariantCode = variant;
    return NOERROR;
}

ECode CLocale::Clone(
    /* [out] */ ILocale** newObj)
{
    VALIDATE_NOT_NULL(newObj);
    return CLocale::New(mLanguageCode, mCountryCode, mVariantCode, newObj);
}

ECode CLocale::Equals(
    /* [in] */ IInterface* other,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
    *result = FALSE;
    if (other == NULL) {
        return NOERROR;
    }

    if (ILocale::Probe(other) == NULL) return NOERROR;

    if ((CLocale*)ILocale::Probe(other) == this) {
        *result = TRUE;
        return NOERROR;
    }

    CLocale* o = (CLocale*)ILocale::Probe(other);
    *result = mLanguageCode == o->mLanguageCode
            && mCountryCode == o->mCountryCode
            && mVariantCode == o->mVariantCode;
    return NOERROR;
}

ECode CLocale::GetAvailableLocales(
    /* [out] */ ArrayOf<ILocale*>** locales)
{
    VALIDATE_NOT_NULL(locales);
    return ICUUtil::GetAvailableLocales(locales);
}

ECode CLocale::GetCountry(
    /* [out] */ String* country)
{
    VALIDATE_NOT_NULL(country);
    *country = mCountryCode;
    return NOERROR;
}

AutoPtr<ILocale> CLocale::GetDefault()
{
    if (!sIsInited) {
        sIsInited = TRUE;

        AutoPtr<Elastos::Core::CSystem> cs;
        Elastos::Core::CSystem::AcquireSingletonByFriend((Elastos::Core::CSystem**)&cs);

        String language, region, variant;
        cs->GetPropertyEx(String("user.language"), String("en"), &language);
        cs->GetPropertyEx(String("user.region"), String("US"), &region);
        cs->GetPropertyEx(String("user.variant"), String(""), &variant);

        AutoPtr<CLocale> l;
        ASSERT_SUCCEEDED(CLocale::NewByFriend(language, region, variant, (CLocale**)&l));
        sDefaultLocale = (ILocale*)l.Get();
    }

    return sDefaultLocale;
}

ECode CLocale::GetDisplayCountry(
    /* [out] */ String* country)
{
    VALIDATE_NOT_NULL(country);
    return GetDisplayCountryEx(GetDefault(), country);
}

ECode CLocale::GetDisplayCountryEx(
    /* [in] */ ILocale* locale,
    /* [out] */ String* country)
{
    VALIDATE_NOT_NULL(country);

    if (mCountryCode.IsEmpty()) {
        *country = String("");
        return NOERROR;
    }
    String thisStr, locStr;
    ToString(&thisStr);
    locale->ToString(&locStr);
    String result = ICUUtil::GetDisplayCountry(thisStr, locStr);
    if (result.IsNull()) { // TODO: do we need to do this, or does ICU do it for us?
        GetDefault()->ToString(&locStr);
        result = ICUUtil::GetDisplayCountry(thisStr, locStr);
    }
    *country = result;
    return NOERROR;
}

ECode CLocale::GetDisplayLanguage(
    /* [out] */ String* language)
{
    VALIDATE_NOT_NULL(language);
    return GetDisplayLanguageEx(GetDefault(), language);
}

ECode CLocale::GetDisplayLanguageEx(
    /* [in] */ ILocale* locale,
    /* [out] */ String* language)
{
    VALIDATE_NOT_NULL(language);

    if (mLanguageCode.IsEmpty()) {
        *language = String("");
        return NOERROR;
    }

    // Last-minute workaround for http://b/7291355 in jb-mr1.
    // This isn't right for all languages, but it's right for en and tl.
    // We should have more CLDR data in a future release, but we'll still
    // probably want to have frameworks/base translate the obsolete tl and
    // tl-rPH locales to fil and fil-rPH at runtime, at which point
    // libcore and icu4c will just do the right thing.
    if (mLanguageCode.Equals("tl")) {
        *language = String("Filipino");
        return NOERROR;
    }

    String thisStr, locStr;
    ToString(&thisStr);
    locale->ToString(&locStr);
    String result = ICUUtil::GetDisplayLanguage(thisStr, locStr);
    if (result.IsNull()) { // TODO: do we need to do this, or does ICU do it for us?
        GetDefault()->ToString(&locStr);
        result = ICUUtil::GetDisplayLanguage(thisStr, locStr);
    }
    *language = result;
    return NOERROR;
}

ECode CLocale::GetDisplayName(
    /* [out] */ String* name)
{
    VALIDATE_NOT_NULL(name);
    return GetDisplayNameEx(GetDefault(), name);;
}

ECode CLocale::GetDisplayNameEx(
    /* [in] */ ILocale* locale,
    /* [out] */ String* name)
{
    VALIDATE_NOT_NULL(name);

    Int32 count = 0;
    StringBuilder buffer;
    if (!mLanguageCode.IsEmpty()) {
        String displayLanguage;
        GetDisplayLanguageEx(locale, &displayLanguage);
        buffer.AppendString(displayLanguage.IsEmpty() ? mLanguageCode : displayLanguage);
        ++count;
    }
    if (!mCountryCode.IsEmpty()) {
        if (count == 1) {
            buffer.AppendCStr(" (");
        }
        String displayCountry;
        GetDisplayCountryEx(locale, &displayCountry);
        buffer.AppendString(displayCountry.IsEmpty() ? mCountryCode : displayCountry);
        ++count;
    }
    if (!mVariantCode.IsEmpty()) {
        if (count == 1) {
            buffer.AppendCStr(" (");
        }
        else if (count == 2) {
            buffer.AppendCStr(",");
        }
        String displayVariant;
        GetDisplayVariantEx(locale, &displayVariant);
        buffer.AppendString(displayVariant.IsEmpty() ? mVariantCode : displayVariant);
        ++count;
    }
    if (count > 1) {
        buffer.AppendCStr(")");
    }
    return buffer.ToString(name);
}

ECode CLocale::GetDisplayVariant(
    /* [out] */ String* variantName)
{
    VALIDATE_NOT_NULL(variantName);
    return GetDisplayVariantEx(GetDefault(), variantName);;
}

ECode CLocale::GetDisplayVariantEx(
    /* [in] */ ILocale* locale,
    /* [out] */ String* variantName)
{
    VALIDATE_NOT_NULL(variantName);

    if (mVariantCode.GetByteLength() == 0) {
        *variantName = mVariantCode;
        return NOERROR;
    }
    String thisStr, locStr;
    ToString(&thisStr);
    locale->ToString(&locStr);
    String result = ICUUtil::GetDisplayVariant(thisStr, locStr);
    if (result.IsNull()) { // TODO: do we need to do this, or does ICU do it for us?
        GetDefault()->ToString(&locStr);
        result = ICUUtil::GetDisplayVariant(thisStr, locStr);
    }
    *variantName = result;
    return NOERROR;
}

ECode CLocale::GetISO3Country(
    /* [out] */ String* country)
{
    VALIDATE_NOT_NULL(country);

    if (mCountryCode.GetByteLength() == 0) {
        *country = mCountryCode;
        return NOERROR;
    }
    String thisStr;
    ToString(&thisStr);
    *country = ICUUtil::GetISO3Country(thisStr);
    return NOERROR;
}

ECode CLocale::GetISO3Language(
    /* [out] */ String* language)
{
    VALIDATE_NOT_NULL(language);

    if (mLanguageCode.IsNullOrEmpty()) {
        *language = mLanguageCode;
        return NOERROR;
    }
    String thisStr;
    ToString(&thisStr);
    *language = ICUUtil::GetISO3Language(thisStr);
    return NOERROR;
}

ECode CLocale::GetISOCountries(
    /* [out] */ ArrayOf<String>** codes)
{
    VALIDATE_NOT_NULL(codes);
    return ICUUtil::GetISOCountries(codes);
}

ECode CLocale::GetISOLanguages(
    /* [out] */ ArrayOf<String>** codes)
{
    VALIDATE_NOT_NULL(codes);
    return ICUUtil::GetISOLanguages(codes);
}

ECode CLocale::GetLanguage(
    /* [out] */ String* language)
{
    VALIDATE_NOT_NULL(language);
    *language = mLanguageCode;
    return NOERROR;
}

ECode CLocale::GetVariant(
    /* [out] */ String* variant)
{
    VALIDATE_NOT_NULL(variant);
    *variant = mVariantCode;
    return NOERROR;
}

ECode CLocale::SetDefault(
    /* [in] */ ILocale* locale)
{
    if (locale == NULL) {
        return E_NULL_POINTER_EXCEPTION;
    }
    sDefaultLocale = locale;
    sIsInited = TRUE;
    return NOERROR;
}

ECode CLocale::ToString(
    /* [out] */ String* str)
{
    VALIDATE_NOT_NULL(str);
    String result = mCachedToStringResult;
    *str = result.IsNull() ? (mCachedToStringResult = ToNewString()) : result;
    return NOERROR;
}

ECode CLocale::GetHashCode(
    /* [out] */ Int32* value)
{
    VALIDATE_NOT_NULL(value)
    *value = mCountryCode.GetHashCode() + mLanguageCode.GetHashCode() + mVariantCode.GetHashCode();
    return NOERROR;
}

String CLocale::ToNewString()
{
    // The string form of a locale that only has a variant is the empty string.
    if ((!mLanguageCode.IsNull() && mLanguageCode.GetByteLength() == 0) &&
          (!mCountryCode.IsNull() && mCountryCode.GetByteLength() == 0)) {
        return String("");
    }
    // Otherwise, the output format is "ll_cc_variant", where language and country are always
    // two letters, but the variant is an arbitrary length. A size of 11 characters has room
    // for "en_US_POSIX", the largest "common" value. (In practice, the string form is almost
    // always 5 characters: "ll_cc".)
    StringBuilder result(11);
    result.AppendString(mLanguageCode);
    if ((!mCountryCode.IsNull() && mCountryCode.GetByteLength() > 0) ||
        (!mVariantCode.IsNull() && mVariantCode.GetByteLength() > 0)) {
         result.AppendChar('_');
    }

    if (!mCountryCode.IsNull()) {
        result.AppendString(mCountryCode);
    }

    if (!mVariantCode.IsNull() && mVariantCode.GetByteLength() > 0) {
        result.AppendChar('_');
    }

    if (!mVariantCode.IsNull()) {
        result.AppendString(mVariantCode);
    }

    return result.ToString();
}

} // namespace ICU
} // namespace Libcore
