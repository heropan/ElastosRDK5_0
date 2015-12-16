
#include "elastos/droid/net/UrlQuerySanitizer.h"
#include "elastos/droid/net/CUrlQuerySanitizerIllegalCharacterValueSanitizer.h"
#include <elastos/core/Math.h>

using Elastos::Utility::CHashMap;
using Elastos::Utility::CArrayList;

namespace Elastos {
namespace Droid {
namespace Net {

CAR_INTERFACE_IMPL(UrlQuerySanitizer, Object, IUrlQuerySanitizer)

const AutoPtr<IUrlQuerySanitizerValueSanitizer> UrlQuerySanitizer::sAllIllegal =
    CreateValueSanitizer(
            IUrlQuerySanitizerIllegalCharacterValueSanitizer::ALL_ILLEGAL);

const AutoPtr<IUrlQuerySanitizerValueSanitizer> UrlQuerySanitizer::sAllButNulLegal =
    CreateValueSanitizer(
            IUrlQuerySanitizerIllegalCharacterValueSanitizer::ALL_BUT_NUL_LEGAL);

const AutoPtr<IUrlQuerySanitizerValueSanitizer> UrlQuerySanitizer::sAllButWhitespaceLegal =
    CreateValueSanitizer(
            IUrlQuerySanitizerIllegalCharacterValueSanitizer::ALL_BUT_WHITESPACE_LEGAL);

const AutoPtr<IUrlQuerySanitizerValueSanitizer> UrlQuerySanitizer::sURLLegal =
    CreateValueSanitizer(
            IUrlQuerySanitizerIllegalCharacterValueSanitizer::URL_LEGAL);

const AutoPtr<IUrlQuerySanitizerValueSanitizer> UrlQuerySanitizer::sUrlAndSpaceLegal =
    CreateValueSanitizer(
            IUrlQuerySanitizerIllegalCharacterValueSanitizer::URL_AND_SPACE_LEGAL);

const AutoPtr<IUrlQuerySanitizerValueSanitizer> UrlQuerySanitizer::sAmpLegal =
    CreateValueSanitizer(
            IUrlQuerySanitizerIllegalCharacterValueSanitizer::AMP_LEGAL);

const AutoPtr<IUrlQuerySanitizerValueSanitizer> UrlQuerySanitizer::sAmpAndSpaceLegal =
    CreateValueSanitizer(
            IUrlQuerySanitizerIllegalCharacterValueSanitizer::AMP_AND_SPACE_LEGAL);

const AutoPtr<IUrlQuerySanitizerValueSanitizer> UrlQuerySanitizer::sSpaceLegal =
    CreateValueSanitizer(
            IUrlQuerySanitizerIllegalCharacterValueSanitizer::SPACE_LEGAL);

const AutoPtr<IUrlQuerySanitizerValueSanitizer> UrlQuerySanitizer::sAllButNulAndAngleBracketsLegal =
    CreateValueSanitizer(
            IUrlQuerySanitizerIllegalCharacterValueSanitizer::ALL_BUT_NUL_AND_ANGLE_BRACKETS_LEGAL);


UrlQuerySanitizer::UrlQuerySanitizer()
    : mSanitizers(CreateHashMap())
    , mEntries(CreateHashMap())
    , mEntriesList(CreateArrayList())
{
    GetAllIllegal((IUrlQuerySanitizerValueSanitizer**)&mUnregisteredParameterValueSanitizer);
}

AutoPtr<IHashMap> UrlQuerySanitizer::CreateHashMap()
{
    AutoPtr<IHashMap> rev;
    CHashMap::New((IHashMap**)&rev);
    return rev;
}

AutoPtr<IArrayList> UrlQuerySanitizer::CreateArrayList()
{
    AutoPtr<IArrayList> rev;
    CArrayList::New((IArrayList**)&rev);
    return rev;
}

AutoPtr<IUrlQuerySanitizerValueSanitizer> UrlQuerySanitizer::CreateValueSanitizer(
    /* [in] */ Int32 value)
{
    AutoPtr<IUrlQuerySanitizerValueSanitizer> rev;
    AutoPtr<IUrlQuerySanitizerIllegalCharacterValueSanitizer> tmp;
    CUrlQuerySanitizerIllegalCharacterValueSanitizer::New(value, (IUrlQuerySanitizerIllegalCharacterValueSanitizer**)&tmp);
    rev = IUrlQuerySanitizerValueSanitizer::Probe(tmp);
    return rev;
}

ECode UrlQuerySanitizer::GetUnregisteredParameterValueSanitizer(
    /* [out] */ IUrlQuerySanitizerValueSanitizer** result)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
        return mUnregisteredParameterValueSanitizer;
#endif
}

ECode UrlQuerySanitizer::SetUnregisteredParameterValueSanitizer(
    /* [in] */ IUrlQuerySanitizerValueSanitizer* sanitizer)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
        mUnregisteredParameterValueSanitizer = sanitizer;
#endif
}

ECode UrlQuerySanitizer::GetAllIllegal(
    /* [out] */ IUrlQuerySanitizerValueSanitizer** result)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
        return sAllIllegal;
#endif
}

ECode UrlQuerySanitizer::GetAllButNulLegal(
    /* [out] */ IUrlQuerySanitizerValueSanitizer** result)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
        return sAllButNulLegal;
#endif
}

ECode UrlQuerySanitizer::GetAllButWhitespaceLegal(
    /* [out] */ IUrlQuerySanitizerValueSanitizer** result)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
        return sAllButWhitespaceLegal;
#endif
}

ECode UrlQuerySanitizer::GetUrlLegal(
    /* [out] */ IUrlQuerySanitizerValueSanitizer** result)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
        return sURLLegal;
#endif
}

ECode UrlQuerySanitizer::GetUrlAndSpaceLegal(
    /* [out] */ IUrlQuerySanitizerValueSanitizer** result)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
        return sUrlAndSpaceLegal;
#endif
}

ECode UrlQuerySanitizer::GetAmpLegal(
    /* [out] */ IUrlQuerySanitizerValueSanitizer** result)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
        return sAmpLegal;
#endif
}

ECode UrlQuerySanitizer::GetAmpAndSpaceLegal(
    /* [out] */ IUrlQuerySanitizerValueSanitizer** result)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
        return sAmpAndSpaceLegal;
#endif
}

ECode UrlQuerySanitizer::GetSpaceLegal(
    /* [out] */ IUrlQuerySanitizerValueSanitizer** result)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
        return sSpaceLegal;
#endif
}

ECode UrlQuerySanitizer::GetAllButNulAndAngleBracketsLegal(
    /* [out] */ IUrlQuerySanitizerValueSanitizer** result)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
        return sAllButNulAndAngleBracketsLegal;
#endif
}

ECode UrlQuerySanitizer::constructor()
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
#endif
}

ECode UrlQuerySanitizer::constructor(
    /* [in] */ const String& url)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
        setAllowUnregisteredParamaters(TRUE);
        parseUrl(url);
#endif
}

ECode UrlQuerySanitizer::ParseUrl(
    /* [in] */ const String& url)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
        Int32 queryIndex = url.indexOf('?');
        String query;
        if (queryIndex >= 0) {
            query = url.substring(queryIndex + 1);
        }
        else {
            query = "";
        }
        parseQuery(query);
#endif
}

ECode UrlQuerySanitizer::ParseQuery(
    /* [in] */ const String& query)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
        clear();
        // Split by '&'
        StringTokenizer tokenizer = new StringTokenizer(query, "&");
        while(tokenizer.hasMoreElements()) {
            String attributeValuePair = tokenizer.nextToken();
            if (attributeValuePair.length() > 0) {
                Int32 assignmentIndex = attributeValuePair.indexOf('=');
                if (assignmentIndex < 0) {
                    // No assignment found, treat as if empty value
                    parseEntry(attributeValuePair, "");
                }
                else {
                    parseEntry(attributeValuePair.substring(0, assignmentIndex),
                            attributeValuePair.substring(assignmentIndex + 1));
                }
            }
        }
#endif
}

ECode UrlQuerySanitizer::GetParameterSet(
    /* [out] */ ISet** result)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
        return mEntries.keySet();
#endif
}

ECode UrlQuerySanitizer::GetParameterList(
    /* [out] */ IList** result)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
        return mEntriesList;
#endif
}

ECode UrlQuerySanitizer::HasParameter(
    /* [in] */ const String& parameter,
    /* [out] */ Boolean* result)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
        return mEntries.containsKey(parameter);
#endif
}

ECode UrlQuerySanitizer::GetValue(
    /* [in] */ const String& parameter,
    /* [out] */ String* result)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
        return mEntries.get(parameter);
#endif
}

ECode UrlQuerySanitizer::RegisterParameter(
    /* [in] */ const String& parameter,
    /* [in] */ IUrlQuerySanitizerValueSanitizer* valueSanitizer)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
        if (valueSanitizer == NULL) {
            mSanitizers.remove(parameter);
        }
        mSanitizers.put(parameter, valueSanitizer);
#endif
}

ECode UrlQuerySanitizer::RegisterParameters(
    /* [in] */ ArrayOf<String>* parameters,
    /* [in] */ IUrlQuerySanitizerValueSanitizer* valueSanitizer)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
        Int32 length = parameters.length;
        for(Int32 i = 0; i < length; i++) {
            mSanitizers.put(parameters[i], valueSanitizer);
        }
#endif
}

ECode UrlQuerySanitizer::SetAllowUnregisteredParamaters(
    /* [in] */ Boolean allowUnregisteredParamaters)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
        mAllowUnregisteredParamaters = allowUnregisteredParamaters;
#endif
}

ECode UrlQuerySanitizer::GetAllowUnregisteredParamaters(
    /* [out] */ Boolean* result)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
        return mAllowUnregisteredParamaters;
#endif
}

ECode UrlQuerySanitizer::SetPreferFirstRepeatedParameter(
    /* [in] */ Boolean preferFirstRepeatedParameter)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
        mPreferFirstRepeatedParameter = preferFirstRepeatedParameter;
#endif
}

ECode UrlQuerySanitizer::GetPreferFirstRepeatedParameter(
    /* [out] */ Boolean* result)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
        return mPreferFirstRepeatedParameter;
#endif
}

ECode UrlQuerySanitizer::ParseEntry(
    /* [in] */ const String& parameter,
    /* [in] */ const String& value)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
        String unescapedParameter = unescape(parameter);
         ValueSanitizer valueSanitizer =
            getEffectiveValueSanitizer(unescapedParameter);
        if (valueSanitizer == NULL) {
            return NOERROR;
        }
        String unescapedValue = unescape(value);
        String sanitizedValue = valueSanitizer.sanitize(unescapedValue);
        addSanitizedEntry(unescapedParameter, sanitizedValue);
#endif
}

ECode UrlQuerySanitizer::AddSanitizedEntry(
    /* [in] */ const String& parameter,
    /* [in] */ const String& value)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
        mEntriesList.add(
                new ParameterValuePair(parameter, value));
        if (mPreferFirstRepeatedParameter) {
            if (mEntries.containsKey(parameter)) {
                return NOERROR;
            }
        }
        mEntries.put(parameter, value);
#endif
}

ECode UrlQuerySanitizer::GetValueSanitizer(
    /* [in] */ const String& parameter,
    /* [out] */ IUrlQuerySanitizerValueSanitizer** result)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
        return mSanitizers.get(parameter);
#endif
}

ECode UrlQuerySanitizer::GetEffectiveValueSanitizer(
    /* [in] */ const String& parameter,
    /* [out] */ IUrlQuerySanitizerValueSanitizer** result)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
        ValueSanitizer sanitizer = getValueSanitizer(parameter);
        if (sanitizer == NULL && mAllowUnregisteredParamaters) {
            sanitizer = getUnregisteredParameterValueSanitizer();
        }
        return sanitizer;
#endif
}

ECode UrlQuerySanitizer::Unescape(
    /* [in] */ const String& string,
    /* [out] */ String* result)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
        // Early exit if no escaped characters.
        Int32 firstEscape = string.indexOf('%');
        if ( firstEscape < 0) {
            firstEscape = string.indexOf('+');
            if (firstEscape < 0) {
                return string;
            }
        }
        Int32 length = string.length();
        StringBuilder stringBuilder = new StringBuilder(length);
        stringBuilder.append(string.substring(0, firstEscape));
        for (Int32 i = firstEscape; i < length; i++) {
            char c = string.charAt(i);
            if (c == '+') {
                c = ' ';
            }
            else if ( c == '%' && i + 2 < length) {
                char c1 = string.charAt(i + 1);
                char c2 = string.charAt(i + 2);
                if (isHexDigit(c1) && isHexDigit(c2)) {
                    c = (char) (decodeHexDigit(c1) * 16 + decodeHexDigit(c2));
                    i += 2;
                }
            }
            stringBuilder.append(c);
        }
        return stringBuilder.toString();
#endif
}

ECode UrlQuerySanitizer::IsHexDigit(
    /* [in] */ Char32 c,
    /* [out] */ Boolean* result)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
        return decodeHexDigit(c) >= 0;
#endif
}

ECode UrlQuerySanitizer::DecodeHexDigit(
    /* [in] */ Char32 c,
    /* [out] */ Int32* result)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
        if (c >= '0' && c <= '9') {
            return c - '0';
        }
        else if (c >= 'A' && c <= 'F') {
            return c - 'A' + 10;
        }
        else if (c >= 'a' && c <= 'f') {
            return c - 'a' + 10;
        }
        else {
            return -1;
        }
#endif
}

ECode UrlQuerySanitizer::Clear()
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
        mEntries.clear();
        mEntriesList.clear();
#endif
}

//=================================================================
// UrlQuerySanitizerParameterValuePair
//=================================================================
CAR_INTERFACE_IMPL(UrlQuerySanitizerParameterValuePair, Object, IUrlQuerySanitizerParameterValuePair)

ECode UrlQuerySanitizerParameterValuePair::constructor(
    /* [in] */ const String& parameter,
    /* [in] */ const String& value)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
                mParameter = parameter;
                mValue = value;
#endif
}

ECode UrlQuerySanitizerParameterValuePair::GetParameter(
    /* [out] */ String* result)
{
    VALIDATE_NOT_NULL(result)

    *result = mParameter;
    return NOERROR;
}

ECode UrlQuerySanitizerParameterValuePair::SetParameter(
    /* [in] */ const String& mParameter)
{
    this->mParameter = mParameter;
    return NOERROR;
}

ECode UrlQuerySanitizerParameterValuePair::GetValue(
    /* [out] */ String* result)
{
    VALIDATE_NOT_NULL(result)

    *result = mValue;
    return NOERROR;
}

ECode UrlQuerySanitizerParameterValuePair::SetValue(
    /* [in] */ const String& mValue)
{
    this->mValue = mValue;
    return NOERROR;
}

//=============================================================
// UrlQuerySanitizerIllegalCharacterValueSanitizer
//=============================================================

CAR_INTERFACE_IMPL_2(UrlQuerySanitizerIllegalCharacterValueSanitizer, Object, IUrlQuerySanitizerValueSanitizer, IUrlQuerySanitizerIllegalCharacterValueSanitizer)

const String UrlQuerySanitizerIllegalCharacterValueSanitizer::JAVASCRIPT_PREFIX("javascript:");
const String UrlQuerySanitizerIllegalCharacterValueSanitizer::VBSCRIPT_PREFIX("vbscript:");
const Int32 UrlQuerySanitizerIllegalCharacterValueSanitizer::MIN_SCRIPT_PREFIX_LENGTH = Elastos::Core::Math::Min(JAVASCRIPT_PREFIX.GetLength(), VBSCRIPT_PREFIX.GetLength());

ECode UrlQuerySanitizerIllegalCharacterValueSanitizer::constructor(
    /* [in] */ Int32 flags)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
                mFlags = flags;
#endif
}

ECode UrlQuerySanitizerIllegalCharacterValueSanitizer::Sanitize(
    /* [in] */ const String& value,
    /* [out] */ String* result)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
                if (value == NULL) {
                    return NULL;
                }
                Int32 length = value.length();
                if ((mFlags & SCRIPT_URL_OK) != 0) {
                    if (length >= MIN_SCRIPT_PREFIX_LENGTH) {
                        String asLower = value.toLowerCase(Locale.ROOT);
                        if (asLower.StartWith(JAVASCRIPT_PREFIX)  ||
                            asLower.StartWith(VBSCRIPT_PREFIX)) {
                            return "";
                        }
                    }
                }
                // If whitespace isn't OK, get rid of whitespace at beginning
                // and end of value.
                if ( (mFlags & ALL_WHITESPACE_OK) == 0) {
                    value = trimWhitespace(value);
                    // The length could have changed, so we need to correct
                    // the length variable.
                    length = value.length();
                }
                StringBuilder stringBuilder = new StringBuilder(length);
                for(Int32 i = 0; i < length; i++) {
                    char c = value.charAt(i);
                    if (!characterIsLegal(c)) {
                        if ((mFlags & SPACE_OK) != 0) {
                            c = ' ';
                        }
                        else {
                            c = '_';
                        }
                    }
                    stringBuilder.append(c);
                }
                return stringBuilder.toString();
#endif
}

ECode UrlQuerySanitizerIllegalCharacterValueSanitizer::TrimWhitespace(
    /* [in] */ const String& value,
    /* [out] */ String* result)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
                Int32 start = 0;
                Int32 last = value.length() - 1;
                Int32 end = last;
                while (start <= end && isWhitespace(value.charAt(start))) {
                    start++;
                }
                while (end >= start && isWhitespace(value.charAt(end))) {
                    end--;
                }
                if (start == 0 && end == last) {
                    return value;
                }
                return value.substring(start, end + 1);
#endif
}

ECode UrlQuerySanitizerIllegalCharacterValueSanitizer::IsWhitespace(
    /* [in] */ Char32 c,
    /* [out] */ Boolean* result)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
                switch(c) {
                case ' ':
                case '\t':
                case '\f':
                case '\n':
                case '\r':
                case 11: /* VT */
                    return TRUE;
                default:
                    return FALSE;
                }
#endif
}

ECode UrlQuerySanitizerIllegalCharacterValueSanitizer::CharacterIsLegal(
    /* [in] */ Char32 c,
    /* [out] */ Boolean* result)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
                switch(c) {
                case ' ' : return (mFlags & SPACE_OK) != 0;
                case '\t': case '\f': case '\n': case '\r': case 11: /* VT */
                  return (mFlags & OTHER_WHITESPACE_OK) != 0;
                case '\"': return (mFlags & DQUOTE_OK) != 0;
                case '\'': return (mFlags & SQUOTE_OK) != 0;
                case '<' : return (mFlags & LT_OK) != 0;
                case '>' : return (mFlags & GT_OK) != 0;
                case '&' : return (mFlags & AMP_OK) != 0;
                case '%' : return (mFlags & PCT_OK) != 0;
                case '\0': return (mFlags & NUL_OK) != 0;
                default  : return (c >= 32 && c < 127) ||
                    ((c >= 128) && ((mFlags & NON_7_BIT_ASCII_OK) != 0));
                }
#endif
}

} // namespace Net
} // namespace Droid
} // namespace Elastos

#if 0 // old CUrlQuerySanitizer.cpp
#include "net/CTrafficStats.h"

#include <dirent.h>
#include <errno.h>
#include <fcntl.h>
#include <elstrtoken.h>
#include <elastos/core/StringBuilder.h>

namespace Elastos {
namespace Droid {
namespace Net {


//Virtual function
Ecode CValueSanitizer::Sanitize(
    /* [in] */ const String& value,
    /* [out] */ String* result)
{
    return NOERROR;
}

Ecode ParameterValuePair::ParameterValuePair(
    /* [in] */ const String& parameter,
    /* [in] */ const String& value)
{
    mParameter = parameter;
    mValue = value;
    return NOERROR;
}

CUrlQuerySanitizer::CUrlQuerySanitizer()
{
    sAllIllegal = new IllegalCharacterValueSanitizer(
                IllegalCharacterValueSanitizer.ALL_ILLEGAL);
    sAllButNulLegal = new IllegalCharacterValueSanitizer(
                IllegalCharacterValueSanitizer.ALL_BUT_NUL_LEGAL);
    sAllButWhitespaceLegal = new IllegalCharacterValueSanitizer(
                IllegalCharacterValueSanitizer.ALL_BUT_WHITESPACE_LEGAL);
    sURLLegal = new IllegalCharacterValueSanitizer(
                IllegalCharacterValueSanitizer.URL_LEGAL);
    sUrlAndSpaceLegal = new IllegalCharacterValueSanitizer(
                IllegalCharacterValueSanitizer.URL_AND_SPACE_LEGAL);
    sAmpLegal = new IllegalCharacterValueSanitizer(
                IllegalCharacterValueSanitizer.AMP_LEGAL);
    sAmpAndSpaceLegal = new IllegalCharacterValueSanitizer(
                IllegalCharacterValueSanitizer.AMP_AND_SPACE_LEGAL);
    sSpaceLegal = new IllegalCharacterValueSanitizer(
                IllegalCharacterValueSanitizer.SPACE_LEGAL);
    sAllButNulAndAngleBracketsLegal = new IllegalCharacterValueSanitizer(
                IllegalCharacterValueSanitizer.ALL_BUT_NUL_AND_ANGLE_BRACKETS_LEGAL);
}

CUrlQuerySanitizer::~CUrlQuerySanitizer()
{
}

Ecode CUrlQuerySanitizer::GetUnregisteredParameterValueSanitizer(
    /* [out] */ IValueSanitizer** result)
{
    VALIDATE_NOT_NULL(result);
    *result = mUnregisteredParameterValueSanitizer;
    REFCOUNT_ADD(*result);
    return NOERROR;
}

Ecode CUrlQuerySanitizer::SetUnregisteredParameterValueSanitizer(
    /* [in] */ IValueSanitizer* sanitizer)
{
    mUnregisteredParameterValueSanitizer = sanitizer;
    return NOERROR;
}

Ecode CUrlQuerySanitizer::GetAllIllegal(
    /* [out] */ IValueSanitizer** result)
{
    VALIDATE_NOT_NULL(result);
    *result = sAllIllegal;
    REFCOUNT_ADD(*result);
    return NOERROR;
}

Ecode CUrlQuerySanitizer::GetAllButNulLegal(
    /* [out] */ IValueSanitizer** result)
{
    VALIDATE_NOT_NULL(result);
    *result = sAllButNulLegal;
    REFCOUNT_ADD(*result);
    return NOERROR;
}

Ecode CUrlQuerySanitizer::GetAllButWhitespaceLegal(
    /* [out] */ IValueSanitizer** result)
{
    VALIDATE_NOT_NULL(result);
    *result = sAllButWhitespaceLegal;
    REFCOUNT_ADD(*result);
    return NOERROR;
}

Ecode CUrlQuerySanitizer::GetUrlLegal(
    /* [out] */ IValueSanitizer** result)
{
    VALIDATE_NOT_NULL(result);
    *result = sURLLegal;
    REFCOUNT_ADD(*result);
    return NOERROR;
}

Ecode CUrlQuerySanitizer::GetUrlAndSpaceLegal(
    /* [out] */ IValueSanitizer** result)
{
    VALIDATE_NOT_NULL(result);
    *result = sUrlAndSpaceLegal;
    REFCOUNT_ADD(*result);
    return NOERROR;
}

Ecode CUrlQuerySanitizer::GetAmpLegal(
    /* [out] */ IValueSanitizer** result)
{
    VALIDATE_NOT_NULL(result);
    *result = sAmpLegal;
    REFCOUNT_ADD(*result);
    return NOERROR;
}

Ecode CUrlQuerySanitizer::GetAmpAndSpaceLegal(
    /* [out] */ IValueSanitizer** result)
{
    VALIDATE_NOT_NULL(result);
    *result = sAmpAndSpaceLegal;
    REFCOUNT_ADD(*result);
    return NOERROR;
}

Ecode CUrlQuerySanitizer::GetSpaceLegal(
    /* [out] */ IValueSanitizer** result)
{
    VALIDATE_NOT_NULL(result);
    *result = sSpaceLegal;
    REFCOUNT_ADD(*result);
    return NOERROR;
}

Ecode CUrlQuerySanitizer::GetAllButNulAndAngleBracketsLegal(
    /* [out] */ IValueSanitizer** result)
{
    VALIDATE_NOT_NULL(result);
    *result = sAllButNulAndAngleBracketsLegal;
    REFCOUNT_ADD(*result);
    return NOERROR;
}

Ecode CUrlQuerySanitizer::ParseUrl(
    /* [in] */ const String& url)
{
    VALIDATE_NOT_NULL(url);

    Int32 queryIndex = url.IndexOf('?');
    String query;
    if (queryIndex >= 0) {
        query = url.Substring(queryIndex + 1);
    }
    else {
        query = "";
    }
    parseQuery(query);

    return NOERROR;
}

Ecode CUrlQuerySanitizer::ParseQuery(
    /* [in] */ const String& query)
{
    Clear();

    AutoPtr<ArrayOf<String> > values;
    StringUtils::Split(query, String("&"), (ArrayOf<String>**)&values);

    Int32 tokens = values ? values->GetLength() : 0;
    for (Int32 i = 0; i < tokens; ++i) {
        String attributeValuePair = (*values)[i];
        if (attributeValuePair.GetLength() > 0) {
            Int32 assignmentIndex = attributeValuePair.IndexOf('=');
            if (assignmentIndex < 0) {
                // No assignment found, treat as if empty value
                ParseEntry(attributeValuePair, "");
            }
            else {
                ParseEntry(attributeValuePair.Substring(0, assignmentIndex),
                        attributeValuePair.Substring(assignmentIndex + 1));
            }
        }
    }

    return NOERROR;
}

Ecode CUrlQuerySanitizer::GetParameterSet(
    /* [out] */ IObjectContainer** result)
{
    VALIDATE_NOT_NULL(result);
    AutoPtr<IObjectContainer> out;
    CObjectContainer::New((IObjectContainer**)&out);

    Set<String>::Iterator it = mEntries.keySet()->Begin();
    for (; it != mHeaders->End();++it) {
        out->Add(*it);
    }

    *result = out->Get();
    REFCOUNT_ADD(*result);
    return NOERROR;
}

Ecode CUrlQuerySanitizer::GetParameterList(
    /* [out] */ IObjectContainer** result)
{
    VALIDATE_NOT_NULL(result);
    AutoPtr<IObjectContainer> out;
    CObjectContainer::New((IObjectContainer**)&out);

    List<ParameterValuePair>::Iterator it = mEntriesList.Begin();
    for (; it != mHeaders->End(); ++it) {
        out->Add(*it);
    }

    *result = out->Get();
    REFCOUNT_ADD(*result);
    return NOERROR;
}

Ecode CUrlQuerySanitizer::HasParameter(
    /* [in] */ const String& parameter,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
    HashMap<String, String>::Iterator iter = mEntries.Find(parameter);
    *result = (iter != mEntries.End());
    return NOERROR;
}

Ecode CUrlQuerySanitizer::GetValue(
    /* [in] */ const String& parameter,
    /* [out] */ String* result)
{
    VALIDATE_NOT_NULL(result);
    *result = NULL;

    HashMap<String, String>::Iterator iter = mEntries.Find(parameter);
    if (iter != mEntries.End())
        result = iter->mSecond;

    return NOERROR;
}

Ecode CUrlQuerySanitizer::RegisterParameter(
    /* [in] */ const String& parameter,
    /* [in] */ IValueSanitizer* valueSanitizer)
{
    if (valueSanitizer == NULL) {
        mSanitizers.Erase(parameter);
    }
    mSanitizers.Insert(parameter, valueSanitizer);
    return NOERROR;
}

Ecode CUrlQuerySanitizer::RegisterParameters(
    /* [in] */ ArrayOf<String>* parameters,
    /* [in] */ IValueSanitizer* valueSanitizer)
{
    Int32 length = parameters->GetLength();
    for(Int32 i = 0; i < length; i++) {
        mSanitizers.Insert(parameters[i], valueSanitizer);
    }
    return NOERROR;
}

Ecode CUrlQuerySanitizer::SetAllowUnregisteredParamaters(
    /* [in] */ Boolean allowUnregisteredParamaters)
{
    mAllowUnregisteredParamaters = allowUnregisteredParamaters;
    return NOERROR;
}

Ecode CUrlQuerySanitizer::GetAllowUnregisteredParamaters(
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
    *result = mAllowUnregisteredParamaters;
    return NOERROR;
}

Ecode CUrlQuerySanitizer::SetPreferFirstRepeatedParameter(
    /* [in] */ Boolean preferFirstRepeatedParameter)
{
    mPreferFirstRepeatedParameter = preferFirstRepeatedParameter;
    return NOERROR;
}

Ecode CUrlQuerySanitizer::GetPreferFirstRepeatedParameter(
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
    *result = mPreferFirstRepeatedParameter;
    return NOERROR;
}

Ecode CUrlQuerySanitizer::GetValueSanitizer(
    /* [in] */ const String& parameter,
    /* [out] */ IValueSanitizer** result)
{
    VALIDATE_NOT_NULL(result);
    *result = NULL;

    HashMap<String, AutoPtr<IValueSanitizer> >::Iterator iter = mSanitizers.Find(parameter);
    if (iter != mSanitizers.End()) {
        *result = iter->mSecond;
        REFCOUNT_ADD(*result);
    }

    return NOERROR;
}

Ecode CUrlQuerySanitizer::GetEffectiveValueSanitizer(
    /* [in] */ const String& parameter,
    /* [out] */ IValueSanitizer** result)
{
    VALIDATE_NOT_NULL(result);
    AutoPtr<IValueSanitizer> sanitizer;
    GetValueSanitizer(parameter, (IValueSanitizer**)&sanitizer);
    if (sanitizer == NULL && mAllowUnregisteredParamaters) {
        GetUnregisteredParameterValueSanitizer(&sanitizer);
    }
    *result = sanitizer;
    REFCOUNT_ADD(*result);
    return NOERROR;
}

Ecode CUrlQuerySanitizer::Unescape(
    /* [in] */ const String& string,
    /* [out] */ String* result)
{
    VALIDATE_NOT_NULL(result);
    *result = NULL;

    Int32 firstEscape = string.IndexOf('%');
    if ( firstEscape < 0) {
        firstEscape = string.IndexOf('+');
        if (firstEscape < 0) {
            *result = string;
            return NOERROR;
        }
    }

    Int32 length = string.GetLength();
    StringBuilder stringBuilder(length);
    stringBuilder += (string.Substring(0, firstEscape));
    for (Int32 i = firstEscape; i < length; i++) {
        char c = string.GetChar(i);
        if (c == '+') {
            c = ' ';
        }
        else if ( c == '%' && i + 2 < length) {
            char c1 = string.GetChar(i + 1);
            char c2 = string.GetChar(i + 2);
            if (IsHexDigit(c1) && IsHexDigit(c2)) {
                c = (char) (DecodeHexDigit(c1) * 16 + DecodeHexDigit(c2));
                i += 2;
        }
        stringBuilder += (c);

    }

    *result = stringBuilder.ToString();
    return NOERROR;
}

void CUrlQuerySanitizer::ParseEntry(
    /* [in] */ const String& parameter,
    /* [in] */ const String& value)
{
    String unescapedParameter;
    Unescape(parameter,&unescapedParameter);

    AutoPtr<IValueSanitizer> valueSanitizer;
    GetEffectiveValueSanitizer(unescapedParameter, &valueSanitizer);
    if (valueSanitizer == NULL) {
        return;
    }
    String unescapedValue;
    Unescape(value, &unescapedValue);
    String sanitizedValue;
    valueSanitizer->Sanitize(unescapedValue, &sanitizedValue);

    AddSanitizedEntry(unescapedParameter, sanitizedValue);
}

void CUrlQuerySanitizer::AddSanitizedEntry(
    /* [in] */ const String& parameter,
    /* [in] */ const String& value)
{
    AutoPtr<ParameterValuePair> t1 = new ParameterValuePair(parameter, value));
    mEntriesList.Insert(t1);

    if (mPreferFirstRepeatedParameter){
         HashMap<String, String>::Iterator iter = mEntries.Find(parameter);
         if (iter != mEntries.End())
             return;
    }
    mEntries.Insert(parameter, value);
}


Boolean CUrlQuerySanitizer::IsHexDigit(
    /* [in] */ Char16 c)
{
    if (DecodeHexDigit(c) >= 0)
        return TRUE;
    else
        return FALSE;
}

Int32 CUrlQuerySanitizer::DecodeHexDigit(
    /* [in] */ Char16 c)
{
    if (c >= '0' && c <= '9') {
        return c - '0';
    }
    else if (c >= 'A' && c <= 'F') {
        return c - 'A' + 10;
    }
    else if (c >= 'a' && c <= 'f') {
        return c - 'a' + 10;
    }
    else {
        return -1;
    }
}

void CUrlQuerySanitizer::Clear()
{
    mEntries.Clear();
    mEntriesList.Clear();
}

CIllegalCharacterValueSanitizer::CIllegalCharacterValueSanitizer(
    /* [in] */ Int32 flags)
{
    mFlags = flags;
}


Ecode CIllegalCharacterValueSanitizer::Sanitize(
   /* [in] */ const String& value,
   /* [out] */ String* result)
{
    if (value == NULL) {
        result = NULL;
        return NOERROR;
    }

    Int32 length = value.GetLength();
    if ((mFlags & SCRIPT_URL_OK) != 0) {
        if (length >= MIN_SCRIPT_PREFIX_LENGTH) {
            String asLower = value.ToLowerCase();
            if (asLower.StartsWith(JAVASCRIPT_PREFIX)  ||
                asLower.StartsWith(VBSCRIPT_PREFIX)) {
                 result = "";
                 return NOERROR;
             }
        }
    }

    // If whitespace isn't OK, get rid of whitespace at beginning
    // and end of value.
    if ( (mFlags & ALL_WHITESPACE_OK) == 0) {
        value = TrimWhitespace(value);
        // The length could have changed, so we need to correct
        // the length variable.
        length = value.GetLength();
    }

    AutoPtr<StringBuilder> stringBuilder = new StringBuilder(length);
    for(Int32 i = 0; i < length; i++) {
        char c = value.GetChar(i);
        if (!CharacterIsLegal(c)) {
            if ((mFlags & SPACE_OK) != 0) {
                c = ' ';
            }
            else {
                c = '_';
            }
        }
        stringBuilder->AppendChar(c);
    }

    result = stringBuilder->toString();
    return NOERROR;
}

String CIllegalCharacterValueSanitizer::TrimWhitespace(
    /* [in] */ const String& value)
{
    Int32 start = 0;
    Int32 last = value.GetLength() - 1;
    Int32 end = last;

    while (start <= end && isWhitespace(value.GetChar(start))) {
        start++;
    }
    while (end >= start && isWhitespace(value.GetChar(end))) {
        end--;
    }

    if (start == 0 && end == last) {
        return value;
    }
    return value.Substring(start, end + 1);
}

Boolean CIllegalCharacterValueSanitizer::IsWhitespace(
    /* [in] */ Char16 c)
{
    Boolean result;

    switch(c) {
        case ' ':
        case '\t':
        case '\f':
        case '\n':
        case '\r':
        case 11: /* VT */
            result = TRUE;
        break;
        default:
            result = FALSE;
        break;
    }
    return result;
}

Boolean CIllegalCharacterValueSanitizer::CharacterIsLegal(
    /* [in] */ Char16 c)
{
    Boolean result;

    switch(c) {
        case ' ' :
            result =  (mFlags & SPACE_OK) != 0;
        break;
        case '\t':
        case '\f':
        case '\n':
        case '\r':
        case 11: /* VT */
            result =  ((mFlags & OTHER_WHITESPACE_OK) != 0 );
        break;
        case '\"':
            result = ( (mFlags & DQUOTE_OK) != 0 );
        break;
        case '\'':
            result = ( (mFlags & SQUOTE_OK) != 0 );
        break;
        case '<' :
            result = ( (mFlags & LT_OK) != 0 );
        break;
        case '>' :
            result = ( (mFlags & GT_OK) != 0 );
        break;
        case '&' :
            result = ( (mFlags & AMP_OK) != 0 );
        break;
        case '%' :
            result = ( (mFlags & PCT_OK) != 0 );
        break;
        case '\0':
            result = ( (mFlags & NUL_OK) != 0);
        break;
        default  :
            result = ( (c >= 32 && c < 127) ||
            ((c >= 128) && ((mFlags & NON_7_BIT_ASCII_OK) != 0)) );
        break;
    }
    return result;
}

} // namespace Net
} // namepsace Droid
} // namespace Elastos
#endif
