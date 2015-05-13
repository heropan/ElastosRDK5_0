
#include "net/CTrafficStats.h"

#include <dirent.h>
#include <errno.h>
#include <fcntl.h>
#include <elstrtoken.h>
#include <elastos/StringBuilder.h>

namespace Elastos {
namespace Droid {
namespace Net {


//Virtual function
Ecode CValueSanitizer::Sanitize(
    /* [in] */ const String& value,
    /* [out] */ String *result)
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
    INTERFACE_ADDREF(*result);
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
    INTERFACE_ADDREF(*result);
    return NOERROR;
}

Ecode CUrlQuerySanitizer::GetAllButNulLegal(
    /* [out] */ IValueSanitizer** result)
{
    VALIDATE_NOT_NULL(result);
    *result = sAllButNulLegal;
    INTERFACE_ADDREF(*result);
    return NOERROR;
}

Ecode CUrlQuerySanitizer::GetAllButWhitespaceLegal(
    /* [out] */ IValueSanitizer** result)
{
    VALIDATE_NOT_NULL(result);
    *result = sAllButWhitespaceLegal;
    INTERFACE_ADDREF(*result);
    return NOERROR;
}

Ecode CUrlQuerySanitizer::GetUrlLegal(
    /* [out] */ IValueSanitizer** result)
{
    VALIDATE_NOT_NULL(result);
    *result = sURLLegal;
    INTERFACE_ADDREF(*result);
    return NOERROR;
}

Ecode CUrlQuerySanitizer::GetUrlAndSpaceLegal(
    /* [out] */ IValueSanitizer** result)
{
    VALIDATE_NOT_NULL(result);
    *result = sUrlAndSpaceLegal;
    INTERFACE_ADDREF(*result);
    return NOERROR;
}

Ecode CUrlQuerySanitizer::GetAmpLegal(
    /* [out] */ IValueSanitizer** result)
{
    VALIDATE_NOT_NULL(result);
    *result = sAmpLegal;
    INTERFACE_ADDREF(*result);
    return NOERROR;
}

Ecode CUrlQuerySanitizer::GetAmpAndSpaceLegal(
    /* [out] */ IValueSanitizer** result)
{
    VALIDATE_NOT_NULL(result);
    *result = sAmpAndSpaceLegal;
    INTERFACE_ADDREF(*result);
    return NOERROR;
}

Ecode CUrlQuerySanitizer::GetSpaceLegal(
    /* [out] */ IValueSanitizer** result)
{
    VALIDATE_NOT_NULL(result);
    *result = sSpaceLegal;
    INTERFACE_ADDREF(*result);
    return NOERROR;
}

Ecode CUrlQuerySanitizer::GetAllButNulAndAngleBracketsLegal(
    /* [out] */ IValueSanitizer** result)
{
    VALIDATE_NOT_NULL(result);
    *result = sAllButNulAndAngleBracketsLegal;
    INTERFACE_ADDREF(*result);
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
    INTERFACE_ADDREF(*result);
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
    INTERFACE_ADDREF(*result);
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
    if (valueSanitizer == null) {
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
        INTERFACE_ADDREF(*result);
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
    INTERFACE_ADDREF(*result);
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
    for(int i = 0; i < length; i++) {
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
