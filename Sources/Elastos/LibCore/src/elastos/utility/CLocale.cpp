
#include "CLocale.h"
// #include "ICUUtil.h"
#include "StringBuilder.h"
#include "CSystem.h"
#include "CStringWrapper.h"
#include "CLocaleBuilder.h"

using Elastos::Core::CSystem;
using Elastos::Core::StringBuilder;
using Elastos::Core::EIID_ICloneable;
using Elastos::Core::CStringWrapper;
using Elastos::Core::ICharSequence;
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

static AutoPtr<ArrayOf<IObjectStreamField> > InitSerialPersistentFields()
{
    // new ObjectStreamField("country", String.class),
    // new ObjectStreamField("hashcode", int.class),
    // new ObjectStreamField("language", String.class),
    // new ObjectStreamField("variant", String.class),
    // new ObjectStreamField("script", String.class),
    // new ObjectStreamField("extensions", String.class),
}
AutoPtr<ArrayOf<IObjectStreamField> > CLocale::sSerialPersistentFields = InitSerialPersistentFields();

static AutoPtr<IMap> InitGRANDFATHERED_LOCALES()
{
    // GRANDFATHERED_LOCALES = new TreeMap<String, String>(String.CASE_INSENSITIVE_ORDER);

    // // From http://tools.ietf.org/html/bcp47
    // //
    // // grandfathered = irregular           ; non-redundant tags registered
    // //               / regular             ; during the RFC 3066 era
    // //  irregular =
    // GRANDFATHERED_LOCALES.put("en-GB-oed", "en-GB-x-oed");
    // GRANDFATHERED_LOCALES.put("i-ami", "ami");
    // GRANDFATHERED_LOCALES.put("i-bnn", "bnn");
    // GRANDFATHERED_LOCALES.put("i-default", "en-x-i-default");
    // GRANDFATHERED_LOCALES.put("i-enochian", "und-x-i-enochian");
    // GRANDFATHERED_LOCALES.put("i-hak", "hak");
    // GRANDFATHERED_LOCALES.put("i-klingon", "tlh");
    // GRANDFATHERED_LOCALES.put("i-lux", "lb");
    // GRANDFATHERED_LOCALES.put("i-mingo", "see-x-i-mingo");
    // GRANDFATHERED_LOCALES.put("i-navajo", "nv");
    // GRANDFATHERED_LOCALES.put("i-pwn", "pwn");
    // GRANDFATHERED_LOCALES.put("i-tao", "tao");
    // GRANDFATHERED_LOCALES.put("i-tay", "tay");
    // GRANDFATHERED_LOCALES.put("i-tsu", "tsu");
    // GRANDFATHERED_LOCALES.put("sgn-BE-FR", "sfb");
    // GRANDFATHERED_LOCALES.put("sgn-BE-NL", "vgt");
    // GRANDFATHERED_LOCALES.put("sgn-CH-DE", "sgg");

    // // regular =
    // GRANDFATHERED_LOCALES.put("art-lojban", "jbo");
    // GRANDFATHERED_LOCALES.put("cel-gaulish", "xtg-x-cel-gaulish");
    // GRANDFATHERED_LOCALES.put("no-bok", "nb");
    // GRANDFATHERED_LOCALES.put("no-nyn", "nn");
    // GRANDFATHERED_LOCALES.put("zh-guoyu", "cmn");
    // GRANDFATHERED_LOCALES.put("zh-hakka", "hak");
    // GRANDFATHERED_LOCALES.put("zh-min", "nan-x-zh-min");
    // GRANDFATHERED_LOCALES.put("zh-min-nan", "nan");
    // GRANDFATHERED_LOCALES.put("zh-xiang", "hsn");
}

AutoPtr<IMap> CLocale::GRANDFATHERED_LOCALES = InitGRANDFATHERED_LOCALES();

/**
 * The current default locale. It is temporarily assigned to US because we
 * need a default locale to lookup the real default locale.
 */
AutoPtr<ILocale> CLocale::sDefaultLocale = CLocale::US;
Boolean CLocale::sIsInited = FALSE;

CAR_INTERFACE_IMPL_3(CLocale, Object, ILocale, ICloneable, ISerializable)

CAR_OBJECT_IMPL(CLocale)

ECode CLocale::ForLanguageTag(
    /* [in] */ const String& languageTag,
    /* [out] */ ILocale** locale)
{
    VALIDATE_NOT_NULL(locale)
    *locale = NULL;

    if (languageTag.IsNull()) {
        return E_NULL_POINTER_EXCEPTION;
    }

    return ForLanguageTag(languageTag, FALSE /* strict */, locale);
}

CLocale::CLocale()
    : mHasValidatedFields(FALSE)
{
}

ECode CLocale::constructor(
    /* [in] */ Boolean hasValidatedFields,
    /* [in] */ const String& lowerCaseLanguageCode,
    /* [in] */ const String& upperCaseCountryCode)
{
    mLanguageCode = lowerCaseLanguageCode;
    mCountryCode = upperCaseCountryCode;
    mVariantCode = String("");
    mScriptCode = String("");

    // mUnicodeAttributes = Collections::EMPTY_SET;
    // mUnicodeKeywords = Collections::EMPTY_MAP;
    // mExtensions = Collections::EMPTY_MAP;

    mHasValidatedFields = hasValidatedFields;

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
    // return constructor(language, String(""), String(""), String(""),
    //     Collections::EMPTY_SET,
    //     Collections::EMPTY_MAP,
    //     Collections.EMPTY_MAP,
    //     FALSE /* has validated fields */);
}

ECode CLocale::constructor(
    /* [in] */ const String& language,
    /* [in] */ const String& country)
{
    // return constructor(language, country, String(""), String(""),
    //     Collections::EMPTY_SET,
    //     Collections::EMPTY_MAP,
    //     Collections.EMPTY_MAP,
    //     FALSE /* has validated fields */);
}

ECode CLocale::constructor(
    /* [in] */ const String& language,
    /* [in] */ const String& country,
    /* [in] */ const String& variant)
{
    // return constructor(language, country, variant, String(""),
    //     Collections::EMPTY_SET,
    //     Collections::EMPTY_MAP,
    //     Collections.EMPTY_MAP,
    //     FALSE /* has validated fields */);
}

ECode CLocale::constructor(
    /* [in] */ const String& language,
    /* [in] */ const String& country,
    /* [in] */ const String& variant,
    /* [in] */ const String& scriptCode,
    /* [in] */ /* nonnull */ ISet* unicodeAttributes,
    /* [in] */ /* nonnull */ IMap* unicodeKeywords,
    /* [in] */ /* nonnull */ IMap* extensions,
    /* [in] */ Boolean hasValidatedFields)
{
    // if (language == null || country == null || variant == null) {
    //     throw new NullPointerException("language=" + language +
    //             ",country=" + country +
    //             ",variant=" + variant);
    // }

    // if (hasValidatedFields) {
    //     this.languageCode = adjustLanguageCode(language);
    //     this.countryCode = country;
    //     this.variantCode = variant;
    // } else {
    //     if (language.IsEmpty() && country.IsEmpty()) {
    //         languageCode = "";
    //         countryCode = "";
    //         variantCode = variant;
    //     } else {
    //         languageCode = adjustLanguageCode(language);
    //         countryCode = country.toUpperCase(Locale.US);
    //         variantCode = variant;
    //     }
    // }

    // this.scriptCode = scriptCode;

    // if (hasValidatedFields) {
    //     Set<String> attribsCopy = new TreeSet<String>(unicodeAttributes);
    //     Map<String, String> keywordsCopy = new TreeMap<String, String>(unicodeKeywords);
    //     Map<Character, String> extensionsCopy = new TreeMap<Character, String>(extensions);

    //     // We need to transform the list of attributes & keywords set on the
    //     // builder to a unicode locale extension. i.e, if we have any keywords
    //     // or attributes set, Locale#getExtension('u') should return a well
    //     // formed extension.
    //     addUnicodeExtensionToExtensionsMap(attribsCopy, keywordsCopy, extensionsCopy);

    //     this.unicodeAttributes = Collections.unmodifiableSet(attribsCopy);
    //     this.unicodeKeywords = Collections.unmodifiableMap(keywordsCopy);
    //     this.extensions = Collections.unmodifiableMap(extensionsCopy);
    // } else {
    //     this.unicodeAttributes = unicodeAttributes;
    //     this.unicodeKeywords = unicodeKeywords;
    //     this.extensions = extensions;
    // }

    // this.hasValidatedFields = hasValidatedFields;
    return NOERROR;
}

ECode CLocale::Clone(
    /* [out] */ IInterface** newObj)
{
    VALIDATE_NOT_NULL(newObj);
    AutoPtr<ILocale> locale;
    CLocale::New((ILocale**)&locale);

    CloneImpl(locale);

    *newObj = locale->Probe(EIID_IInterface);
    REFCOUNT_ADD(*newObj);
    return NOERROR;
}

ECode CLocale::CloneImpl(
    /* [in] */ ILocale* locale)
{
    assert(locale);

    CLocale* l = (CLocale*)locale;
    l->mCountryCode = mCountryCode;
    l->mLanguageCode = mLanguageCode;
    l->mVariantCode = mVariantCode;
    l->mScriptCode = mScriptCode;

    l->mUnicodeAttributes = mUnicodeAttributes;
    l->mUnicodeKeywords = mUnicodeKeywords;
    l->mExtensions = mExtensions;

    l->mHasValidatedFields = mHasValidatedFields;

    l->mCachedToStringResult = mCachedToStringResult;
    l->mCachedLanguageTag = mCachedLanguageTag;
    l->mCachedIcuLocaleId = mCachedIcuLocaleId;
}

ECode CLocale::Equals(
    /* [in] */ IInterface* other,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
    *result = FALSE;
    if (ILocale::Probe(other) == NULL) {
        return NOERROR;
    }

    if (other == this->Probe(EIID_IInterface)) {
        *result = TRUE;
        return NOERROR;
    }

    Boolean equals;
    CLocale* o = (CLocale*)ILocale::Probe(other);
    *result = mLanguageCode.Equals(o->mLanguageCode)
            && mCountryCode.Equals(o->mCountryCode)
            && mVariantCode.Equals(o->mVariantCode)
            && (mExtensions->Equals(o->mExtensions, &equals), equals);
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

    String normalizedRegion;
    ECode ec = CLocaleBuilder::NormalizeAndValidateRegion(mCountryCode, FALSE /* strict */, &normalizedRegion);
    FAIL_RETURN(ec)

    if (normalizedRegion.IsEmpty()) {
        *country = mCountryCode;
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

    // Hacks for backward compatibility.
    //
    // Our language tag will contain "und" if the languageCode is invalid
    // or missing. ICU will then return "langue indéterminée" or the equivalent
    // display language for the indeterminate language code.
    //
    // Sigh... ugh... and what not.
    String normalizedLanguage;
    ECode ec = CLocaleBuilder::NormalizeAndValidateLanguage(mLanguageCode, FALSE /* strict */, &normalizedLanguage);
    FAIL_RETURN(ec);
    if (UNDETERMINED_LANGUAGE.Equals(normalizedLanguage)) {
        *language = mLanguageCode;
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
    if (!mScriptCode.IsEmpty()) {
        if (count == 1) {
            buffer.Append(" (");
        }
        String displayScript;
        GetDisplayScript(locale, &displayScript);
        buffer.Append(displayScript.IsEmpty() ? mScriptCode : displayScript);
        ++count;
    }
    if (!mCountryCode.IsEmpty()) {
        if (count == 1) {
            buffer.Append(" (");
        }
        else if (count == 2) {
            buffer.Append(",");
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
        else if (count == 2 || count == 3) {
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
    return GetDisplayVariant(GetDefault(), variantName);;
}

ECode CLocale::GetDisplayVariant(
    /* [in] */ ILocale* locale,
    /* [out] */ String* variantName)
{
    VALIDATE_NOT_NULL(variantName);

    if (mVariantCode.IsEmpty()) {
        *variantName = String("");
        return NOERROR;
    }

    // try {
    String result;
    ECode ec = CLocaleBuilder::NormalizeAndValidateVariant(mVariantCode, &result);
    if (ec = (ECode)E_ILLFORMED_LOCALE_EXCEPTION) {
        *variantName = mVariantCode;
        return NOERROR;
    }
    // } catch (IllformedLocaleException ilfe) {
    //      return mVariantCode;
    // }

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

    // The results of getISO3Country do not depend on the languageCode,
    // so we pass an arbitrarily selected language code here. This guards
    // against errors caused by malformed or invalid language codes.
    String cc("en-");
    cc += mCountryCode;
    // String code = ICUUtil::GetISO3Country(cc);
    // if (!mCountryCode.IsEmpty() && code.IsEmpty()) {
    //     ALOGE("CLocale::GetISO3Country: MissingResourceException: No 3-letter country code for locale: %s", mCountryCode.string());
    //     return E_MISSING_RESOURCE_EXCEPTION;
    // }
    // *country = code;
    // return NOERROR;
}

ECode CLocale::GetISO3Language(
    /* [out] */ String* language)
{
    VALIDATE_NOT_NULL(language);

    if (mLanguageCode.IsNullOrEmpty()) {
        *language = String("");
        return NOERROR;
    }

    // The results of getISO3Language do not depend on the country code
    // or any of the other locale fields, so we pass just the language here.
    // String code = ICUUtil.GetISO3Language(languageCode);
    // if (!mLanguageCode.IsEmpty() && code.IsEmpty()) {
    //     ALOGE("CLocale::GetISO3Language: MissingResourceException: No 3-letter country code for locale: %s", mCountryCode.string());
    //     return E_MISSING_RESOURCE_EXCEPTION;
    // }
    // *country = code;
    // return NOERROR;
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
    // We only need to revalidate the language, country and variant because
    // the rest of the fields can only be set via the builder which validates
    // them anyway.
    String language("");
    String region("");
    String variant("");
    String illFormedVariantSubtags("");

    if (mHasValidatedFields) {
        language = mLanguageCode;
        region = mCountryCode;
        // Note that we are required to normalize hyphens to underscores
        // in the builder, but we must use hyphens in the BCP-47 language tag.
        variant = mVariantCode.Replace('_', '-');
    }
    else {
        CLocaleBuilder::NormalizeAndValidateLanguage(mLanguageCode, FALSE /* strict */, &language);
        CLocaleBuilder::NormalizeAndValidateRegion(mCountryCode, FALSE /* strict */, &region);

        // try {
        ECode ec = CLocaleBuilder::NormalizeAndValidateVariant(mVariantCode, &variant);
        // } catch (IllformedLocaleException ilfe)
        if (ec == (ECode)E_ILLFORMED_LOCALE_EXCEPTION) {
            // If our variant is ill formed, we must attempt to split it into
            // its constituent subtags and preserve the well formed bits and
            // move the rest to the private use extension (if they're well
            // formed extension subtags).
            AutoPtr<ArrayOf<String> > split = SplitIllformedVariant(mVariantCode);

            variant = (*split)[0];
            illFormedVariantSubtags = (*split)[1];
        }
    }

    if (language.IsEmpty()) {
        language = UNDETERMINED_LANGUAGE;
    }

    if (language.Equals("no") && region.Equals("NO") && variant.Equals("NY")) {
        language = "nn";
        region = "NO";
        variant = "";
    }

    StringBuilder sb(16);
    sb.Append(language);

    if (!mScriptCode.IsEmpty()) {
        sb.AppendChar('-');
        sb.Append(mScriptCode);
    }

    if (!region.IsEmpty()) {
        sb.AppendChar('-');
        sb.Append(region);
    }

    if (!variant.IsEmpty()) {
        sb.AppendChar('-');
        sb.Append(variant);
    }

    // Extensions (optional, omitted if empty). Note that we don't
    // emit the private use extension here, but add it in the end.
    AutoPtr<ISet> entrySet;
    mExtensions->EntrySet((ISet**)&entrySet);
    AutoPtr<IIterator> it;
    IIterable::Probe(entrySet)->GetIterator((IIterator**)&it);
    Boolean hasNext;
    String str;
    AutoPtr<IMapEntry> extension;
    AutoPtr<ICharSequence> csq;
    AutoPtr<IInterface> obj;
    while (it->HasNext(&hasNext), hasNext) {
        it->Next((IInterface**)&obj);
        extension = IMapEntry::Probe(obj);

        obj = NULL;
        extension->GetKey((IInterface**)&obj);
        csq = ICharSequence::Probe(obj);
        csq->ToString(&str);
        if (!str.Equals("x")) {
            sb.AppendChar('-');
            sb.Append(str);
            sb.AppendChar('-');

            obj = NULL;
            extension->GetValue((IInterface**)&obj);
            csq = ICharSequence::Probe(obj);
            csq->ToString(&str);
            sb.Append(str);
        }

        obj = NULL;
    }

    // The private use extension comes right at the very end.
    csq = NULL;
    CStringWrapper::New(String("x"), (ICharSequence**)&csq);
    mExtensions->Get(csq, (IInterface**)&obj);
    String privateUse;
    ICharSequence::Probe(obj)->ToString(&privateUse);
    if (!privateUse.IsNull()) {
        sb.Append("-x-");
        sb.Append(privateUse);
    }

    // If we have any ill-formed variant subtags, we append them to the
    // private use extension (or add a private use extension if one doesn't
    // exist).
    if (!illFormedVariantSubtags.IsEmpty()) {
        if (!privateUse.IsNull()) {
            sb.Append("-x-lvariant-");
        } else {
            sb.AppendChar('-');
        }
        sb.Append(illFormedVariantSubtags);
    }

    return sb.ToString();
}

AutoPtr<ArrayOf<String> > CLocale::SplitIllformedVariant(
    /* [in] */ const String& variant)
{
    assert(0 && "TODO");
    // String normalizedVariant = variant.replace('_', '-');
    // String[] subTags = normalizedVariant.split("-");

    // String[] split = new String[] { "", "" };

    // // First go through the list of variant subtags and check if they're
    // // valid private use extension subtags. If they're not, we will omit
    // // the first such subtag and all subtags after.
    // //
    // // NOTE: |firstInvalidSubtag| is the index of the first variant
    // // subtag we decide to omit altogether, whereas |firstIllformedSubtag| is the
    // // index of the first subtag we decide to append to the private use extension.
    // //
    // // In other words:
    // // [0, firstIllformedSubtag) => expressed as variant subtags.
    // // [firstIllformedSubtag, firstInvalidSubtag) => expressed as private use
    // // extension subtags.
    // // [firstInvalidSubtag, subTags.length) => omitted.
    // Int32 firstInvalidSubtag = subTags.length;
    // for (Int32 i = 0; i < subTags.length; ++i) {
    //     if (!isValidBcp47Alphanum(subTags[i], 1, 8)) {
    //         firstInvalidSubtag = i;
    //         break;
    //     }
    // }

    // if (firstInvalidSubtag == 0) {
    //     return split;
    // }

    // // We now consider each subtag that could potentially be appended to
    // // the private use extension and check if it's valid.
    // Int32 firstIllformedSubtag = firstInvalidSubtag;
    // for (Int32 i = 0; i < firstInvalidSubtag; ++i) {
    //     String subTag = subTags[i];
    //     // The BCP-47 spec states that :
    //     // - Subtags can be between [5, 8] alphanumeric chars in length.
    //     // - Subtags that start with a number are allowed to be 4 chars in length.
    //     if (subTag.length() >= 5 && subTag.length() <= 8) {
    //         if (!isAsciiAlphaNum(subTag)) {
    //             firstIllformedSubtag = i;
    //         }
    //     } else if (subTag.length() == 4) {
    //         char firstChar = subTag.charAt(0);
    //         if (!(firstChar >= '0' && firstChar <= '9') || !isAsciiAlphaNum(subTag)) {
    //             firstIllformedSubtag = i;
    //         }
    //     } else {
    //         firstIllformedSubtag = i;
    //     }
    // }

    // split[0] = concatenateRange(subTags, 0, firstIllformedSubtag);
    // split[1] = concatenateRange(subTags, firstIllformedSubtag, firstInvalidSubtag);

    // return split;
}

String CLocale::ConcatenateRange(
    /* [in] */ ArrayOf<String>* array,
    /* [in] */ Int32 start,
    /* [in] */ Int32 end)
{
    StringBuilder builder(32);
    for (Int32 i = start; i < end; ++i) {
        if (i != start) {
            builder.AppendChar('-');
        }
        builder.Append((*array)[i]);
    }

    return builder.ToString();
}

ECode CLocale::GetExtensionKeys(
    /* [out] */ ISet** keys)
{
    VALIDATE_NOT_NULL(keys);
    // return extensions.keySet();
    return NOERROR;
}

ECode CLocale::GetExtension(
    /* [in] */ Char32 extensionKey,
    /* [out] */ String* extension)
{
    VALIDATE_NOT_NULL(extension);
    // return extensions.get(extensionKey);
    return NOERROR;
}

ECode CLocale::GetUnicodeLocaleType(
    /* [in] */ const String& keyWord,
    /* [out] */ String* type)
{
    VALIDATE_NOT_NULL(type);
    // return unicodeKeywords.get(keyWord);
    return NOERROR;
}

ECode CLocale::GetUnicodeLocaleAttributes(
    /* [out] */ ISet** keys)
{
    VALIDATE_NOT_NULL(keys);
    // return unicodeAttributes;
}

ECode CLocale::GetUnicodeLocaleKeys(
    /* [out] */ ISet** keys)
{
    VALIDATE_NOT_NULL(keys);
    // return unicodeKeywords.keySet();
    return NOERROR;
}

ECode CLocale::GetHashCode(
    /* [out] */ Int32* value)
{
    VALIDATE_NOT_NULL(value)
    // *value = mCountryCode.GetHashCode() + mLanguageCode.GetHashCode() + mVariantCode.GetHashCode();
    //         return countryCode.hashCode()
    //             + languageCode.hashCode() + variantCode.hashCode()
    //             + scriptCode.hashCode() + extensions.hashCode();
    return NOERROR;
}

ECode CLocale::SetDefault(
    /* [in] */ ILocale* locale)
{
    if (locale == NULL) {
        return E_NULL_POINTER_EXCEPTION;
    }

    // String languageTag = locale.toLanguageTag();
    // defaultLocale = locale;
    // ICU.setDefaultLocale(languageTag);
}

ECode CLocale::ToString(
    /* [out] */ String* str)
{
    VALIDATE_NOT_NULL(str);
    String result = mCachedToStringResult;
    *str = result.IsNull() ? (mCachedToStringResult = ToNewString(
        mLanguageCode, mCountryCode, mVariantCode, mScriptCode, mExtensions)) : result;
    return NOERROR;
}

String CLocale::ToNewString(
    /* [in] */ const String& languageCode,
    /* [in] */ const String& countryCode,
    /* [in] */ const String& variantCode,
    /* [in] */ const String& scriptCode,
    /* [in] */ IMap* extensions)
{
    // The string form of a locale that only has a variant is the empty string.
    if (languageCode.IsNullOrEmpty() || countryCode.IsNullOrEmpty()) {
        return String("");
    }

    // Otherwise, the output format is "ll_cc_variant", where language and country are always
    // two letters, but the variant is an arbitrary length. A size of 11 characters has room
    // for "en_US_POSIX", the largest "common" value. (In practice, the string form is almost
    // always 5 characters: "ll_cc".)
    StringBuilder result(11);
    result.Append(languageCode);

    Boolean empty;
    Boolean hasScriptOrExtensions = !scriptCode.IsNullOrEmpty() || !(extensions->IsEmpty(&empty), empty);

    if (!countryCode.IsNullOrEmpty() || !variantCode.IsNullOrEmpty() || hasScriptOrExtensions) {
         result.AppendChar('_');
    }
    result.Append(countryCode);

    if (!variantCode.IsNullOrEmpty() || hasScriptOrExtensions) {
        result.AppendChar('_');
    }
    result.Append(variantCode);

    if (hasScriptOrExtensions) {
        if (!variantCode.IsNullOrEmpty()) {
            result.AppendChar('_');
        }

        // Note that this is notably different from the BCP-47 spec (for
        // backwards compatibility). We are forced to append a "#" before the script tag.
        // and also put the script code right at the end.
        result.AppendChar("#");
        if (!scriptCode.IsNullOrEmpty() ) {
            result.Append(scriptCode);
        }

        // Note the use of "-" instead of "_" before the extensions.
        if (!(extensions->IsEmpty(&empty), empty)) {
            if (!scriptCode.IsNullOrEmpty()) {
                result.AppendChar('-');
            }
            result.Append(SerializeExtensions(extensions));
        }
    }

    return result.ToString();
}


ECode CLocale::WriteObject(
    /* [in] */ IObjectOutputStream* stream)
{
    assert(0 && "TODO");
    // ObjectOutputStream.PutField fields = stream.putFields();
    // fields.put("country", countryCode);
    // fields.put("hashcode", -1);
    // fields.put("language", languageCode);
    // fields.put("variant", variantCode);
    // fields.put("script", scriptCode);

    // if (!extensions.isEmpty()) {
    //     fields.put("extensions", serializeExtensions(extensions));
    // }

    // stream.writeFields();
    return NOERROR;
}

ECode CLocale::ReadObject(
    /* [in] */ IObjectOutputStream* stream)
{
    assert(0 && "TODO");
    // ObjectInputStream.GetField fields = stream.readFields();
    // countryCode = (String) fields.get("country", "");
    // languageCode = (String) fields.get("language", "");
    // variantCode = (String) fields.get("variant", "");
    // scriptCode = (String) fields.get("script", "");

    // this.unicodeKeywords = Collections.EMPTY_MAP;
    // this.unicodeAttributes = Collections.EMPTY_SET;
    // this.extensions = Collections.EMPTY_MAP;

    // String extensions = (String) fields.get("extensions", null);
    // if (extensions != null) {
    //     readExtensions(extensions);
    // }
    return NOERROR;
}

ECode CLocale::ReadExtensions(
    /* [in] */ const String& extensions)
{
    assert(0 && "TODO");
    // Map<Character, String> extensionsMap = new TreeMap<Character, String>();
    // parseSerializedExtensions(extensions, extensionsMap);
    // this.extensions = Collections.unmodifiableMap(extensionsMap);

    // if (extensionsMap.containsKey(UNICODE_LOCALE_EXTENSION)) {
    //     String unicodeExtension = extensionsMap.get(UNICODE_LOCALE_EXTENSION);
    //     String[] subTags = unicodeExtension.split("-");

    //     Map<String, String> unicodeKeywords = new TreeMap<String, String>();
    //     Set<String> unicodeAttributes = new TreeSet<String>();
    //     parseUnicodeExtension(subTags, unicodeKeywords, unicodeAttributes);

    //     this.unicodeKeywords = Collections.unmodifiableMap(unicodeKeywords);
    //     this.unicodeAttributes = Collections.unmodifiableSet(unicodeAttributes);
    // }
    return NOERROR;
}

String CLocale::SerializeExtensions(
    /* [in] */ IMap* extensionsMap)
{
    assert(0 && "TODO");
    // Iterator<Map.Entry<Character, String>> entryIterator = extensionsMap.entrySet().iterator();
    StringBuilder sb(64);

    // while (true) {
    //     Map.Entry<Character, String> entry = entryIterator.next();
    //     sb.append(entry.getKey());
    //     sb.append('-');
    //     sb.append(entry.getValue());

    //     if (entryIterator.hasNext()) {
    //         sb.append('-');
    //     } else {
    //         break;
    //     }
    // }

    return sb.ToString();
}

ECode CLocale::ParseSerializedExtensions(
        /* [in] */ const String& extString,
        /* [in] */ IMap* outputMap)
{
    assert(0 && "TODO");
    // This probably isn't the most efficient approach, but it's the
    // most straightforward to code.
    //
    // Start by splitting the string on "-". We will then keep track of
    // where each of the extension keys (single characters) appear in the
    // original string and then use those indices to construct substrings
    // representing the values.
    // String[] subTags = extString.split("-");
    // int[] typeStartIndices = new int[subTags.length / 2];

    // Int32 length = 0;
    // Int32 count = 0;
    // for (String subTag : subTags) {
    //     if (subTag.length() > 0) {
    //         // Account for the length of the "-" at the end of each subtag.
    //         length += (subTag.length() + 1);
    //     }

    //     if (subTag.length() == 1) {
    //         typeStartIndices[count++] = length;
    //     }
    // }

    // for (Int32 i = 0; i < count; ++i) {
    //     Int32 valueStart = typeStartIndices[i];
    //     // Since the start Index points to the beginning of the next type
    //     // ....prev-k-next.....
    //     //            |_ here
    //     // (idx - 2) is the index of the next key
    //     // (idx - 3) is the (non inclusive) end of the previous type.
    //     Int32 valueEnd = (i == (count - 1)) ?
    //             extString.length() : (typeStartIndices[i + 1] - 3);

    //     outputMap.put(extString.charAt(typeStartIndices[i] - 2),
    //             extString.substring(valueStart, valueEnd));
    // }
}

Boolean CLocale::IsUnM49AreaCode(
    /* [in] */ const String& code)
{
    assert(0 && "TODO");
    // if (code.length() != 3) {
    //     return false;
    // }

    // for (Int32 i = 0; i < 3; ++i) {
    //     char character = code.charAt(i);
    //     if (!(character >= '0' && character <= '9')) {
    //         return false;
    //     }
    // }

    return true;
}

Boolean CLocale::IsAsciiAlphaNum(
    /* [in] */ const String& string)
{
    assert(0 && "TODO");
    // for (Int32 i = 0; i < string.length(); i++) {
    //     char character = string.charAt(i);
    //     if (!(character >= 'a' && character <= 'z' ||
    //             character >= 'A' && character <= 'Z' ||
    //             character >= '0' && character <= '9')) {
    //         return false;
    //     }
    // }

    return true;
}

Boolean CLocale::IsValidBcp47Alpha(
    /* [in] */ const String& attributeOrType,
    /* [in] */ Int32 lowerBound,
    /* [in] */ Int32 upperBound)
{
    // final int length = string.length();
    // if (length < lowerBound || length > upperBound) {
    //     return false;
    // }

    // for (int i = 0; i < length; ++i) {
    //     final char character = string.charAt(i);
    //     if (!(character >= 'a' && character <= 'z' ||
    //             character >= 'A' && character <= 'Z')) {
    //         return false;
    //     }
    // }

    // return true;
}

Boolean CLocale::IsValidBcp47Alphanum(
    /* [in] */ const String& attributeOrType,
    /* [in] */ Int32 lowerBound,
    /* [in] */ Int32 upperBound)
{
    assert(0 && "TODO");
    // if (attributeOrType.length() < lowerBound || attributeOrType.length() > upperBound) {
    //     return false;
    // }

    return IsAsciiAlphaNum(attributeOrType);
}

String CLocale::TitleCaseAsciiWord(
    /* [in] */ const String& word)
{
    assert(0 && "TODO");
    // try {
    //     byte[] chars = word.toLowerCase(Locale.ROOT).getBytes(StandardCharsets.US_ASCII);
    //     chars[0] = (byte) ((int) chars[0] + 'A' - 'a');
    //     return new String(chars, StandardCharsets.US_ASCII);
    // } catch (UnsupportedOperationException uoe) {
    //     throw new AssertionError(uoe);
    // }
}

Boolean CLocale::IsValidTypeList(
    /* [in] */ const String& lowerCaseTypeList)
{
    assert(0 && "TODO");
    // String[] splitList = lowerCaseTypeList.split("-");
    // for (String type : splitList) {
    //     if (!isValidBcp47Alphanum(type, 3, 8)) {
    //         return false;
    //     }
    // }

    return true;
}

void CLocale::AddUnicodeExtensionToExtensionsMap(
    /* [in] */ ISet* attributes,
    /* [in] */ IMap* keywords,
    /* [in] */ IMap* extensions)
{
    assert(0 && "TODO");
    // if (attributes.isEmpty() && keywords.isEmpty()) {
    //     return;
    // }

    // // Assume that the common case is a low number of keywords & attributes
    // // (usually one or two).
    // StringBuilder sb = new StringBuilder(32);

    // // All attributes must appear before keywords, in lexical order.
    // if (!attributes.isEmpty()) {
    //     Iterator<String> attributesIterator = attributes.iterator();
    //     while (true) {
    //         sb.append(attributesIterator.next());
    //         if (attributesIterator.hasNext()) {
    //             sb.append('-');
    //         } else {
    //             break;
    //         }
    //     }
    // }

    // if (!keywords.isEmpty()) {
    //     if (!attributes.isEmpty()) {
    //         sb.append('-');
    //     }

    //     Iterator<Map.Entry<String, String>> keywordsIterator = keywords.entrySet().iterator();
    //     while (true) {
    //         Map.Entry<String, String> keyWord = keywordsIterator.next();
    //         sb.append(keyWord.getKey());
    //         if (!keyWord.getValue().isEmpty()) {
    //             sb.append('-');
    //             sb.append(keyWord.getValue());
    //         }
    //         if (keywordsIterator.hasNext()) {
    //             sb.append('-');
    //         } else {
    //             break;
    //         }
    //     }
    // }

    // extensions.put(UNICODE_LOCALE_EXTENSION, sb.toString());
}

void CLocale::ParseUnicodeExtension(
    /* [in] */ ArrayOf<String>* subtags,
    /* [in] */ IMap* keywords,
    /* [in] */ ISet* attributes)
{
    assert(0 && "TODO");
    // String lastKeyword = null;
    // List<String> subtagsForKeyword = new ArrayList<String>();
    // for (String subtag : subtags) {
    //     if (subtag.length() == 2) {
    //         if (subtagsForKeyword.size() > 0) {
    //             keywords.put(lastKeyword, joinBcp47Subtags(subtagsForKeyword));
    //             subtagsForKeyword.clear();
    //         }

    //         lastKeyword = subtag;
    //     } else if (subtag.length() > 2) {
    //         if (lastKeyword == null) {
    //             attributes.add(subtag);
    //         } else {
    //             subtagsForKeyword.add(subtag);
    //         }
    //     }
    // }

    // if (subtagsForKeyword.size() > 0) {
    //     keywords.put(lastKeyword, joinBcp47Subtags(subtagsForKeyword));
    // } else if (lastKeyword != null) {
    //     keywords.put(lastKeyword, "");
    // }
}

String CLocale::JoinBcp47Subtags(
    /* [in] */ IList* strings)
{
    assert(0 && "TODO");
    // Int32 size = strings.size();

    // StringBuilder sb = new StringBuilder(strings.get(0).length());
    // for (Int32 i = 0; i < size; ++i) {
    //     sb.append(strings.get(i));
    //     if (i != size - 1) {
    //         sb.append('-');
    //     }
    // }

    // return sb.toString();
}

String CLocale::AdjustLanguageCode(
    /* [in] */ const String& languageCode)
{
    assert(0 && "TODO");
    // String adjusted = languageCode.toLowerCase(Locale.US);
    // // Map new language codes to the obsolete language
    // // codes so the correct resource bundles will be used.
    // if (languageCode.equals("he")) {
    //     adjusted = "iw";
    // } else if (languageCode.equals("id")) {
    //     adjusted = "in";
    // } else if (languageCode.equals("yi")) {
    //     adjusted = "ji";
    // }

    // return adjusted;
}

String CLocale::ConvertGrandfatheredTag(
    /* [in] */ const String& original)
{
    assert(0 && "TODO");
    // String converted = GRANDFATHERED_LOCALES.get(original);
    // return converted != null ? converted : original;
}

void CLocale::ExtractVariantSubtags(
    /* [in] */ ArrayOf<String>* subtags,
    /* [in] */ Int32 startIndex,
    /* [in] */ Int32 endIndex,
    /* [in] */ IList* normalizedVariants)
{
    assert(0 && "TODO");
    // for (Int32 i = startIndex; i < endIndex; i++) {
    //     String subtag = subtags[i];

    //     if (Builder.isValidVariantSubtag(subtag)) {
    //         normalizedVariants.add(subtag);
    //     } else {
    //         break;
    //     }
    // }
}

Int32 CLocale::ExtractExtensions(
    /* [in] */ ArrayOf<String>* subtags,
    /* [in] */ Int32 startIndex,
    /* [in] */ Int32 endIndex,
    /* [in] */ IMap* extensions)
{
    assert(0 && "TODO");
    // Int32 privateUseExtensionIndex = -1;
    // Int32 extensionKeyIndex = -1;

    // Int32 i = startIndex;
    // for (; i < endIndex; i++) {
    //     String subtag = subtags[i];

    //     boolean parsingPrivateUse = (privateUseExtensionIndex != -1) &&
    //             (extensionKeyIndex == privateUseExtensionIndex);

    //     // Note that private use extensions allow subtags of length 1.
    //     // Private use extensions *must* come last, so there's no ambiguity
    //     // in that case.
    //     if (subtag.length() == 1 && !parsingPrivateUse) {
    //         // Emit the last extension we encountered if any. First check
    //         // whether we encountered two keys in a row (which is an error).
    //         // Also checks if we already have an extension with the same key,
    //         // which is again an error.
    //         if (extensionKeyIndex != -1) {
    //             if ((i - 1) == extensionKeyIndex) {
    //                 return extensionKeyIndex;
    //             }

    //             String key = subtags[extensionKeyIndex];
    //             if (extensions.containsKey(key.charAt(0))) {
    //                 return extensionKeyIndex;
    //             }

    //             String value = concatenateRange(subtags, extensionKeyIndex + 1, i);
    //             extensions.put(key.charAt(0), value.toLowerCase(Locale.ROOT));
    //         }

    //         // Mark the start of the next extension. Also keep track of whether this
    //         // is a private use extension, and throw an error if it doesn't come last.
    //         extensionKeyIndex = i;
    //         if ("x".equals(subtag)) {
    //             privateUseExtensionIndex = i;
    //         } else if (privateUseExtensionIndex != -1) {
    //             // The private use extension must come last.
    //             return privateUseExtensionIndex;
    //         }
    //     } else if (extensionKeyIndex != -1) {
    //         // We must have encountered a valid key in order to start parsing
    //         // its subtags.
    //         if (!isValidBcp47Alphanum(subtag, parsingPrivateUse ? 1 : 2, 8)) {
    //             return i;
    //         }
    //     } else {
    //         // Encountered a value without a preceding key.
    //         return i;
    //     }
    // }

    // if (extensionKeyIndex != -1) {
    //     if ((i - 1) == extensionKeyIndex) {
    //         return extensionKeyIndex;
    //     }

    //     String key = subtags[extensionKeyIndex];
    //     if (extensions.containsKey(key.charAt(0))) {
    //         return extensionKeyIndex;
    //     }

    //     String value = concatenateRange(subtags, extensionKeyIndex + 1, i);
    //     extensions.put(key.charAt(0), value.toLowerCase(Locale.ROOT));
    // }

    // return i;
}

ECode CLocale::ForLanguageTag(
    /* @Nonnull */ /* [in] */ const String& tag,
    /* [in] */ Boolean strict,
    /* [out] */ ILocale** locale)
{
    assert(0 && "TODO");
    // String converted = convertGrandfatheredTag(tag);
    // String[] subtags = converted.split("-");

    // Int32 lastSubtag = subtags.length;
    // for (Int32 i = 0; i < subtags.length; ++i) {
    //     String subtag = subtags[i];
    //     if (subtag.isEmpty() || subtag.length() > 8) {
    //         if (strict) {
    //             throw new IllformedLocaleException("Invalid subtag at index: " + i
    //                     + " in tag: " + tag);
    //         } else {
    //             lastSubtag = (i - 1);
    //         }

    //         break;
    //     }
    // }

    // String languageCode = Builder.normalizeAndValidateLanguage(subtags[0], strict);
    // String scriptCode = "";
    // Int32 nextSubtag = 1;
    // if (lastSubtag > nextSubtag) {
    //     scriptCode = Builder.normalizeAndValidateScript(subtags[nextSubtag], false /* strict */);
    //     if (!scriptCode.isEmpty()) {
    //         nextSubtag++;
    //     }
    // }

    // String regionCode = "";
    // if (lastSubtag > nextSubtag) {
    //     regionCode = Builder.normalizeAndValidateRegion(subtags[nextSubtag], false /* strict */);
    //     if (!regionCode.isEmpty()) {
    //         nextSubtag++;
    //     }
    // }

    // List<String> variants = null;
    // if (lastSubtag > nextSubtag) {
    //     variants = new ArrayList<String>();
    //     extractVariantSubtags(subtags, nextSubtag, lastSubtag, variants);
    //     nextSubtag += variants.size();
    // }

    // Map<Character, String> extensions = Collections.EMPTY_MAP;
    // if (lastSubtag > nextSubtag) {
    //     extensions = new TreeMap<Character, String>();
    //     nextSubtag = extractExtensions(subtags, nextSubtag, lastSubtag, extensions);
    // }

    // if (nextSubtag != lastSubtag) {
    //     if (strict) {
    //         throw new IllformedLocaleException("Unparseable subtag: " + subtags[nextSubtag]
    //                 + " from language tag: " + tag);
    //     }
    // }

    // Set<String> unicodeKeywords = Collections.EMPTY_SET;
    // Map<String, String> unicodeAttributes = Collections.EMPTY_MAP;
    // if (extensions.containsKey(UNICODE_LOCALE_EXTENSION)) {
    //     unicodeKeywords = new TreeSet<String>();
    //     unicodeAttributes = new TreeMap<String, String>();
    //     parseUnicodeExtension(extensions.get(UNICODE_LOCALE_EXTENSION).split("-"),
    //             unicodeAttributes, unicodeKeywords);
    // }

    // String variantCode = "";
    // if (variants != null && !variants.isEmpty()) {
    //     StringBuilder variantsBuilder = new StringBuilder(variants.size() * 8);
    //     for (Int32 i = 0; i < variants.size(); ++i) {
    //         if (i != 0) {
    //             variantsBuilder.append('_');
    //         }
    //         variantsBuilder.append(variants.get(i));
    //     }
    //     variantCode = variantsBuilder.toString();
    // }

    // return new Locale(languageCode, regionCode, variantCode, scriptCode,
    //         unicodeKeywords, unicodeAttributes, extensions, true /* has validated fields */);
    return NOERROR;
}

} // namespace Utility
} // namespace Elastos
