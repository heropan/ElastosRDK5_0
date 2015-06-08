#include "CLocaleBuilder.h"

namespace Elastos {
namespace Utility {

CAR_INTERFACE_IMPL(CLocaleBuilder, Object, ILocaleBuilder)

CAR_OBJECT_IMPL(CLocaleBuilder)

CLocaleBuilder::CLocaleBuilder()
    : mLanguage("")
    , mRegion("")
    , mVariant("")
    , mScript("")
{
}

ECode CLocaleBuilder::constructor()
{
    // NOTE: We use sorted maps in the builder & the locale class itself
    // because serialized forms of the unicode locale extension (and
    // of the extension map itself) are specified to be in alphabetic
    // order of keys.
    // CTreeSet::New((ISet**)&mAttributes);
    // CTreeMap::New((IMap**)&mKeywords);
    // CTreeMap::New((IMap**)&mExtensions);
    return NOERROR;
}

ECode CLocaleBuilder::SetLanguage(
    /* [in] */ const String& language)
{
    mLanguage = NormalizeAndValidateLanguage(language, TRUE /* strict */);
    return NOERROR;
}

String CLocaleBuilder::NormalizeAndValidateLanguage(
    /* [in] */ const String& language,
    /* [in] */ Boolean strict)
{
   // if (language == null || language.isEmpty()) {
   //      return "";
   //  }

   //  final String lowercaseLanguage = language.toLowerCase(Locale.ROOT);
   //  if (!isValidBcp47Alpha(lowercaseLanguage, 2, 3)) {
   //      if (strict) {
   //          throw new IllformedLocaleException("Invalid language: " + language);
   //      } else {
   //          return UNDETERMINED_LANGUAGE;
   //      }
   //  }

   //  return lowercaseLanguage;
    return String("");
}

ECode CLocaleBuilder::SetLanguageTag(
    /* [in] */ const String& tag)
{
    // if (languageTag == null || languageTag.isEmpty()) {
    //     clear();
    //     return this;
    // }

    // final Locale fromIcu = forLanguageTag(languageTag, true /* strict */);
    // // When we ask ICU for strict parsing, it might return a null locale
    // // if the language tag is malformed.
    // if (fromIcu == null) {
    //     throw new IllformedLocaleException("Invalid languageTag: " + languageTag);
    // }

    // setLocale(fromIcu);
    // return this;
    return NOERROR;
}

ECode CLocaleBuilder::SetRegion(
    /* [in] */ const String& region)
{
    mRegion = NormalizeAndValidateRegion(region, TRUE /* strict */);
    return NOERROR;
}

String CLocaleBuilder::NormalizeAndValidateRegion(
    /* [in] */ const String& region,
    /* [in] */ Boolean strict)
{
    // if (region == null || region.isEmpty()) {
    //     return "";
    // }

    // final String uppercaseRegion = region.toUpperCase(Locale.ROOT);
    // if (!isValidBcp47Alpha(uppercaseRegion, 2, 2) &&
    //         !isUnM49AreaCode(uppercaseRegion)) {
    //     if (strict) {
    //         throw new IllformedLocaleException("Invalid region: " + region);
    //     } else {
    //         return "";
    //     }
    // }

    // return uppercaseRegion;
    return String("");
}

ECode CLocaleBuilder::SetVariant(
    /* [in] */ const String& variant)
{
    mVariant = NormalizeAndValidateVariant(variant);
    return NOERROR;
}

String CLocaleBuilder::NormalizeAndValidateVariant(
    /* [in] */ const String& region)
{
    // if (variant == null || variant.isEmpty()) {
    //     return "";
    // }

    // // Note that unlike extensions, we canonicalize to lower case alphabets
    // // and underscores instead of hyphens.
    // final String normalizedVariant = variant.replace('-', '_');
    // String[] subTags = normalizedVariant.split("_");

    // for (String subTag : subTags) {
    //     if (!isValidVariantSubtag(subTag)) {
    //         throw new IllformedLocaleException("Invalid variant: " + variant);
    //     }
    // }

    // return normalizedVariant;
    return String("");
}

Boolean CLocaleBuilder::IsValidVariantSubtag(
    /* [in] */ const String& subTag)
{
    // The BCP-47 spec states that :
    // - Subtags can be between [5, 8] alphanumeric chars in length.
    // - Subtags that start with a number are allowed to be 4 chars in length.
    // if (subTag.length() >= 5 && subTag.length() <= 8) {
    //     if (isAsciiAlphaNum(subTag)) {
    //         return true;
    //     }
    // } else if (subTag.length() == 4) {
    //     final char firstChar = subTag.charAt(0);
    //     if ((firstChar >= '0' && firstChar <= '9') && isAsciiAlphaNum(subTag)) {
    //         return true;
    //     }
    // }

    return FALSE;
}

ECode CLocaleBuilder::SetScript(
    /* [in] */ const String& script)
{
    mScript = NormalizeAndValidateScript(script, TRUE /* strict */);
    return NOERROR;
}

String CLocaleBuilder::NormalizeAndValidateScript(
    /* [in] */ const String& script,
    /* [in] */ Boolean strict)
{
    // if (script == null || script.isEmpty()) {
    //     return "";
    // }

    // if (!isValidBcp47Alpha(script, 4, 4)) {
    //     if (strict) {
    //         throw new IllformedLocaleException("Invalid script: " + script);
    //     } else {
    //         return "";
    //     }
    // }

    // return titleCaseAsciiWord(script);
    return String("");
}

ECode CLocaleBuilder::SetLocale(
    /* [in] */ ILocale* locale)
{
    // if (locale == null) {
    //     throw new NullPointerException("locale == null");
    // }

    // // Make copies of the existing values so that we don't partially
    // // update the state if we encounter an error.
    // final String backupLanguage = language;
    // final String backupRegion = region;
    // final String backupVariant = variant;

    // try {
    //     setLanguage(locale.getLanguage());
    //     setRegion(locale.getCountry());
    //     setVariant(locale.getVariant());
    // } catch (IllformedLocaleException ifle) {
    //     language = backupLanguage;
    //     region = backupRegion;
    //     variant = backupVariant;

    //     throw ifle;
    // }

    // // The following values can be set only via the builder class, so
    // // there's no need to normalize them or check their validity.

    // this.script = locale.getScript();

    // extensions.clear();
    // extensions.putAll(locale.extensions);

    // keywords.clear();
    // keywords.putAll(locale.unicodeKeywords);

    // attributes.clear();
    // attributes.addAll(locale.unicodeAttributes);
    return NOERROR;
}

ECode CLocaleBuilder::AddUnicodeLocaleAttribute(
    /* [in] */ const String& attribute)
{
    // if (attribute == null) {
    //     throw new NullPointerException("attribute == null");
    // }

    // final String lowercaseAttribute = attribute.toLowerCase(Locale.ROOT);
    // if (!isValidBcp47Alphanum(lowercaseAttribute, 3, 8)) {
    //     throw new IllformedLocaleException("Invalid locale attribute: " + attribute);
    // }

    // attributes.add(lowercaseAttribute);
    return NOERROR;
}

ECode CLocaleBuilder::RemoveUnicodeLocaleAttribute(
    /* [in] */ const String& attribute)
{
    // if (attribute == null) {
    //     throw new NullPointerException("attribute == null");
    // }

    // // Weirdly, remove is specified to check whether the attribute
    // // is valid, so we have to perform the full alphanumeric check here.
    // final String lowercaseAttribute = attribute.toLowerCase(Locale.ROOT);
    // if (!isValidBcp47Alphanum(lowercaseAttribute, 3, 8)) {
    //     throw new IllformedLocaleException("Invalid locale attribute: " + attribute);
    // }

    // attributes.remove(attribute);
    return NOERROR;
}

ECode CLocaleBuilder::SetExtension(
    /* [in] */ Char32 key,
    /* [in] */ const String& value)
{
    // if (value == null || value.isEmpty()) {
    //     extensions.remove(key);
    //     return this;
    // }

    // final String normalizedValue = value.toLowerCase(Locale.ROOT).replace('_', '-');
    // final String[] subtags = normalizedValue.split("-");

    // // Lengths for subtags in the private use extension should be [1, 8] chars.
    // // For all other extensions, they should be [2, 8] chars.
    // //
    // // http://www.rfc-editor.org/rfc/bcp/bcp47.txt
    // final int minimumLength = (key == PRIVATE_USE_EXTENSION) ? 1 : 2;
    // for (String subtag : subtags) {
    //     if (!isValidBcp47Alphanum(subtag, minimumLength, 8)) {
    //         throw new IllformedLocaleException(
    //                 "Invalid private use extension : " + value);
    //     }
    // }

    // // We need to take special action in the case of unicode extensions,
    // // since we claim to understand their keywords and attributes.
    // if (key == UNICODE_LOCALE_EXTENSION) {
    //     // First clear existing attributes and keywords.
    //     extensions.clear();
    //     attributes.clear();

    //     parseUnicodeExtension(subtags, keywords, attributes);
    // } else {
    //     extensions.put(key, normalizedValue);
    // }
    return NOERROR;
}

ECode CLocaleBuilder::ClearExtensions()
{
    // extensions.clear();
    // attributes.clear();
    // keywords.clear();
    return NOERROR;
}

ECode CLocaleBuilder::SetUnicodeLocaleKeyword(
    /* [in] */ const String& key,
    /* [in] */ const String& type)
{
    // if (key == null) {
    //     throw new NullPointerException("key == null");
    // }

    // if (type == null && keywords != null) {
    //     keywords.remove(key);
    //     return this;
    // }

    // final String lowerCaseKey = key.toLowerCase(Locale.ROOT);
    // // The key must be exactly two alphanumeric characters.
    // if (lowerCaseKey.length() != 2 || !isAsciiAlphaNum(lowerCaseKey)) {
    //     throw new IllformedLocaleException("Invalid unicode locale keyword: " + key);
    // }

    // // The type can be one or more alphanumeric strings of length [3, 8] characters,
    // // separated by a separator char, which is one of "_" or "-". Though the spec
    // // doesn't require it, we normalize all "_" to "-" to make the rest of our
    // // processing easier.
    // final String lowerCaseType = type.toLowerCase(Locale.ROOT).replace("_", "-");
    // if (!isValidTypeList(lowerCaseType)) {
    //     throw new IllformedLocaleException("Invalid unicode locale type: " + type);
    // }

    // // Everything checks out fine, add the <key, type> mapping to the list.
    // keywords.put(lowerCaseKey, lowerCaseType);
    return NOERROR;
}


ECode CLocaleBuilder::Clear()
{
    // clearExtensions();
    // language = region = variant = script = "";
    return NOERROR;
}

ECode CLocaleBuilder::Build(
    /* [out] */ ILocale** locale)
{
    // NOTE: We need to make a copy of attributes, keywords and extensions
    // because the RI allows this builder to reused.
    // return new Locale(language, region, variant, script,
    //         attributes, keywords, extensions,
    //         true /* has validated fields */);
    return NOERROR;
}


} // namespace Utility
} // namespace Elastos