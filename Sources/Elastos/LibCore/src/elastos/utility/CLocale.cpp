
#include "CLocale.h"
// #include "ICUUtil.h"
#include "StringBuilder.h"
#include "CSystem.h"

using Elastos::Core::CSystem;
using Elastos::Core::StringBuilder;
using Elastos::Core::EIID_ICloneable;
using Elastos::IO::EIID_ISerializable;
// using Libcore::ICU::ICUUtil;

namespace Elastos {
namespace Utility {

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

String CLocale::UNDETERMINED_LANGUAGE("und");

/**
 * The current default locale. It is temporarily assigned to US because we
 * need a default locale to lookup the real default locale.
 */
AutoPtr<ILocale> CLocale::sDefaultLocale = CLocale::US;
Boolean CLocale::sIsInited = FALSE;

CAR_INTERFACE_IMPL_3(CLocale, Object, ILocale, ICloneable, ISerializable)

CAR_OBJECT_IMPL(CLocale)

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
    /* [out] */ IInterface** newObj)
{
    VALIDATE_NOT_NULL(newObj);
    AutoPtr<ILocale> locale;
    CLocale::New(mLanguageCode, mCountryCode, mVariantCode, (ILocale**)&locale);
    *newObj = locale->Probe(EIID_IInterface);
    REFCOUNT_ADD(*newObj);
    return NOERROR;
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

    if (other == this->Probe(EIID_IInterface)) {
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
    // return ICUUtil::GetAvailableLocales(locales);
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
        cs->GetProperty(String("user.language"), String("en"), &language);
        cs->GetProperty(String("user.region"), String("US"), &region);
        cs->GetProperty(String("user.variant"), String(""), &variant);

        AutoPtr<CLocale> l;
        ASSERT_SUCCEEDED(CLocale::NewByFriend(language, region, variant, (CLocale**)&l));
        sDefaultLocale = ILocale::Probe(l);
    }

    return sDefaultLocale;
}

ECode CLocale::GetDisplayCountry(
    /* [out] */ String* country)
{
    VALIDATE_NOT_NULL(country);
    return GetDisplayCountry(GetDefault(), country);
}

ECode CLocale::GetDisplayCountry(
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
    // String result = ICUUtil::GetDisplayCountry(thisStr, locStr);
    // if (result.IsNull()) { // TODO: do we need to do this, or does ICU do it for us?
    //     GetDefault()->ToString(&locStr);
    //     result = ICUUtil::GetDisplayCountry(thisStr, locStr);
    // }
    // *country = result;
    return NOERROR;
}

ECode CLocale::GetDisplayLanguage(
    /* [out] */ String* language)
{
    VALIDATE_NOT_NULL(language);
    return GetDisplayLanguage(GetDefault(), language);
}

ECode CLocale::GetDisplayLanguage(
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
    // String result = ICUUtil::GetDisplayLanguage(thisStr, locStr);
    // if (result.IsNull()) { // TODO: do we need to do this, or does ICU do it for us?
    //     GetDefault()->ToString(&locStr);
    //     result = ICUUtil::GetDisplayLanguage(thisStr, locStr);
    // }
    // *language = result;
    return NOERROR;
}

ECode CLocale::GetDisplayName(
    /* [out] */ String* name)
{
    VALIDATE_NOT_NULL(name);
    return GetDisplayName(GetDefault(), name);;
}

ECode CLocale::GetDisplayName(
    /* [in] */ ILocale* locale,
    /* [out] */ String* name)
{
    VALIDATE_NOT_NULL(name);

    Int32 count = 0;
    StringBuilder buffer;
    if (!mLanguageCode.IsEmpty()) {
        String displayLanguage;
        GetDisplayLanguage(locale, &displayLanguage);
        buffer.Append(displayLanguage.IsEmpty() ? mLanguageCode : displayLanguage);
        ++count;
    }
    if (!mCountryCode.IsEmpty()) {
        if (count == 1) {
            buffer.Append(" (");
        }
        String displayCountry;
        GetDisplayCountry(locale, &displayCountry);
        buffer.Append(displayCountry.IsEmpty() ? mCountryCode : displayCountry);
        ++count;
    }
    if (!mVariantCode.IsEmpty()) {
        if (count == 1) {
            buffer.Append(" (");
        }
        else if (count == 2) {
            buffer.Append(",");
        }
        String displayVariant;
        GetDisplayVariant(locale, &displayVariant);
        buffer.Append(displayVariant.IsEmpty() ? mVariantCode : displayVariant);
        ++count;
    }
    if (count > 1) {
        buffer.Append(")");
    }
    return buffer.ToString(name);
}

ECode CLocale::GetDisplayVariant(
    /* [out] */ String* variantName)
{
    VALIDATE_NOT_NULL(variantName);
    return GetDisplayVariant(GetDefault(), variantName);;
}

ECode CLocale::GetDisplayVariant(
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
    // String result = ICUUtil::GetDisplayVariant(thisStr, locStr);
    // if (result.IsNull()) { // TODO: do we need to do this, or does ICU do it for us?
    //     GetDefault()->ToString(&locStr);
    //     result = ICUUtil::GetDisplayVariant(thisStr, locStr);
    // }
    // *variantName = result;
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
    // *country = ICUUtil::GetISO3Country(thisStr);
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
    // *language = ICUUtil::GetISO3Language(thisStr);
    return NOERROR;
}

ECode CLocale::GetISOCountries(
    /* [out] */ ArrayOf<String>** codes)
{
    VALIDATE_NOT_NULL(codes);
    // return ICUUtil::GetISOCountries(codes);
}

ECode CLocale::GetISOLanguages(
    /* [out] */ ArrayOf<String>** codes)
{
    VALIDATE_NOT_NULL(codes);
    // return ICUUtil::GetISOLanguages(codes);
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

ECode CLocale::GetScript(
    /* [out] */ String* script)
{
    VALIDATE_NOT_NULL(script);
    *script = mScriptCode;
    return NOERROR;
}

ECode CLocale::GetDisplayScript(
    /* [out] */ String* script)
{
    VALIDATE_NOT_NULL(script);
    AutoPtr<ILocale> locale = GetDefault();
    return GetDisplayScript(locale, script);
}

ECode CLocale::GetDisplayScript(
    /* [in] */ ILocale* locale,
    /* [out] */ String* script)
{
    VALIDATE_NOT_NULL(script);
    *script = String("");

    if (mScriptCode.IsEmpty()) {
        return NOERROR;
    }

    // String result = ICUUtil::GetDisplayScript(THIS_PROBE(ILocale), locale);
    // if (result.IsNull()) { // TODO: do we need to do this, or does ICU do it for us?
    //     AutoPtr<ILocale> locale = GetDefault();
    //     result = ICUUtil::GetDisplayScript(THIS_PROBE(ILocale),locale);
    // }

    // *script = result;
    return NOERROR;
}

ECode CLocale::ToLanguageTag(
    /* [out] */ String* tag)
{
    VALIDATE_NOT_NULL(tag);
    if (mCachedLanguageTag.IsNull()) {
        mCachedLanguageTag = MakeLanguageTag();
    }

    *tag = mCachedLanguageTag;
    return NOERROR;
}

String CLocale::MakeLanguageTag()
{
    assert(0 && "TODO");
    return String("");
    // // We only need to revalidate the language, country and variant because
    // // the rest of the fields can only be set via the builder which validates
    // // them anyway.
    // String language = "";
    // String region = "";
    // String variant = "";
    // String illFormedVariantSubtags = "";

    // if (hasValidatedFields) {
    //     language = languageCode;
    //     region = countryCode;
    //     // Note that we are required to normalize hyphens to underscores
    //     // in the builder, but we must use hyphens in the BCP-47 language tag.
    //     variant = variantCode.replace('_', '-');
    // } else {
    //     language = Builder.normalizeAndValidateLanguage(languageCode, false /* strict */);
    //     region = Builder.normalizeAndValidateRegion(countryCode, false /* strict */);

    //     try {
    //         variant = Builder.normalizeAndValidateVariant(variantCode);
    //     } catch (IllformedLocaleException ilfe) {
    //         // If our variant is ill formed, we must attempt to split it into
    //         // its constituent subtags and preserve the well formed bits and
    //         // move the rest to the private use extension (if they're well
    //         // formed extension subtags).
    //         String split[] = splitIllformedVariant(variantCode);

    //         variant = split[0];
    //         illFormedVariantSubtags = split[1];
    //     }
    // }

    // if (language.isEmpty()) {
    //     language = UNDETERMINED_LANGUAGE;
    // }

    // if ("no".equals(language) && "NO".equals(region) && "NY".equals(variant)) {
    //     language = "nn";
    //     region = "NO";
    //     variant = "";
    // }

    // final StringBuilder sb = new StringBuilder(16);
    // sb.append(language);

    // if (!scriptCode.isEmpty()) {
    //     sb.append('-');
    //     sb.append(scriptCode);
    // }

    // if (!region.isEmpty()) {
    //     sb.append('-');
    //     sb.append(region);
    // }

    // if (!variant.isEmpty()) {
    //     sb.append('-');
    //     sb.append(variant);
    // }

    // // Extensions (optional, omitted if empty). Note that we don't
    // // emit the private use extension here, but add it in the end.
    // for (Map.Entry<Character, String> extension : extensions.entrySet()) {
    //     if (!extension.getKey().equals('x')) {
    //         sb.append('-').append(extension.getKey());
    //         sb.append('-').append(extension.getValue());
    //     }
    // }

    // // The private use extension comes right at the very end.
    // final String privateUse = extensions.get('x');
    // if (privateUse != null) {
    //     sb.append("-x-");
    //     sb.append(privateUse);
    // }

    // // If we have any ill-formed variant subtags, we append them to the
    // // private use extension (or add a private use extension if one doesn't
    // // exist).
    // if (!illFormedVariantSubtags.isEmpty()) {
    //     if (privateUse == null) {
    //         sb.append("-x-lvariant-");
    //     } else {
    //         sb.append('-');
    //     }
    //     sb.append(illFormedVariantSubtags);
    // }

    // return sb.toString();
}

ECode CLocale::GetExtensionKeys(
    /* [out] */ ISet** keys)
{
    VALIDATE_NOT_NULL(keys);

    return NOERROR;
}

ECode CLocale::GetExtension(
    /* [in] */ Char32 extensionKey,
    /* [out] */ String* extension)
{
    VALIDATE_NOT_NULL(extension);

    return NOERROR;
}

ECode CLocale::GetUnicodeLocaleType(
    /* [in] */ const String& keyWord,
    /* [out] */ String* type)
{
    VALIDATE_NOT_NULL(type);
    return NOERROR;
}

ECode CLocale::GetUnicodeLocaleAttributes(
    /* [out] */ ISet** keys)
{
    VALIDATE_NOT_NULL(keys);
    // *variant = mVariantCode;
    return NOERROR;
}

ECode CLocale::GetUnicodeLocaleKeys(
    /* [out] */ ISet** keys)
{
    VALIDATE_NOT_NULL(keys);
    // *variant = mVariantCode;
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
    result.Append(mLanguageCode);
    if ((!mCountryCode.IsNull() && mCountryCode.GetByteLength() > 0) ||
        (!mVariantCode.IsNull() && mVariantCode.GetByteLength() > 0)) {
         result.AppendChar('_');
    }

    if (!mCountryCode.IsNull()) {
        result.Append(mCountryCode);
    }

    if (!mVariantCode.IsNull() && mVariantCode.GetByteLength() > 0) {
        result.AppendChar('_');
    }

    if (!mVariantCode.IsNull()) {
        result.Append(mVariantCode);
    }

    return result.ToString();
}

} // namespace Utility
} // namespace Elastos
