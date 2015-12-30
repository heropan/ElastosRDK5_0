
#include "JSONTokener.h"
#include "JSON.h"
#include "CJSONArray.h"
#include "CJSONObject.h"
#include "JSONObject.h"
#include "CoreUtils.h"
#include "StringUtils.h"
#include "Math.h"
#include "utility/Arrays.h"
#include "elastos/utility/logging/Logger.h"

using Elastos::Core::CoreUtils;
using Elastos::Core::ICharSequence;
using Elastos::Core::StringUtils;
using Elastos::Core::IString;
using Elastos::Utility::Logging::Logger;

namespace Org {
namespace Json {

CAR_INTERFACE_IMPL(JSONTokener, Object, IJSONTokener);

JSONTokener::JSONTokener()
    : mIn(NULL)
    , mPos(0)
{}

JSONTokener::~JSONTokener()
{}

ECode JSONTokener::constructor(
    /* [in] */ const String& in)
{
    String tmp = in;
    // consume an optional byte order mark (BOM) if it exists
    if (!tmp.IsNull() && tmp.StartWith("\ufeff")) {
        String str = tmp.Substring(1);
        tmp = str;
    }

    mIn = tmp;
    return NOERROR;
}

ECode JSONTokener::NextValue(
    /* [out] */ IInterface** obj)
{
    VALIDATE_NOT_NULL(obj);
    *obj = NULL;

    Int32 c;
    NextCleanInternal(&c);
    switch (c) {
        case -1:
            return SyntaxError(String("End of input"));

        case '{': {
            AutoPtr<IJSONObject> json;
            ReadObject((IJSONObject**)&json);
            *obj = json;
            REFCOUNT_ADD(*obj);
            return NOERROR;
        }

        case '[':{
            AutoPtr<IJSONArray> json;
            ReadArray((IJSONArray**)&json);
            *obj = json;
            REFCOUNT_ADD(*obj);
            return NOERROR;
        }

        case '\'':
        case '"': {
            String str;
            NextString((Char32) c, &str);
            AutoPtr<ICharSequence> cs = CoreUtils::Convert(str);
            *obj = cs;
            REFCOUNT_ADD(*obj);
            return NOERROR;
        }

        default:
            mPos--;
            return ReadLiteral(obj);
    }

    return NOERROR;
}

ECode JSONTokener::NextCleanInternal(
    /* [out] */ Int32* result)
{
    VALIDATE_NOT_NULL(result);
    *result = -1;

    while (mPos < mIn.GetLength()) {
        Int32 c = (Int32)mIn.GetChar(mPos++);
        switch (c) {
            case '\t':
            case ' ':
            case '\n':
            case '\r':
                continue;

            case '/': {
                if (mPos == mIn.GetLength()) {
                    *result = c;
                    return NOERROR;
                }

                Char32 peek = mIn.GetChar(mPos);
                switch (peek) {
                    case '*': {
                        // skip a /* c-style comment */
                        mPos++;
                        Int32 commentEnd = mIn.IndexOf("*/", mPos);
                        if (commentEnd == -1) {
                            return SyntaxError(String("Unterminated comment"));
                        }
                        mPos = commentEnd + 2;
                        continue;
                    }

                    case '/':
                        // skip a // end-of-line comment
                        mPos++;
                        SkipToEndOfLine();
                        continue;

                    default:
                        *result = c;
                        return NOERROR;
                }
            }

            case '#':
                /*
                 * Skip a # hash end-of-line comment. The JSON RFC doesn't
                 * specify this behavior, but it's required to parse
                 * existing documents. See http://b/2571423.
                 */
                SkipToEndOfLine();
                continue;

            default:
                *result = c;
                return NOERROR;
        }
    }

    return NOERROR;
}

void JSONTokener::SkipToEndOfLine()
{
    for (; mPos < mIn.GetLength(); mPos++) {
        Char32 c = mIn.GetChar(mPos);
        if (c == '\r' || c == '\n') {
            mPos++;
            break;
        }
    }
}

ECode JSONTokener::NextString(
    /* [in] */ Char32 quote,
    /* [out] */ String* result)
{
    VALIDATE_NOT_NULL(result);
    *result = String(NULL);

    /*
     * For strings that are free of escape sequences, we can just extract
     * the result as a substring of the input. But if we encounter an escape
     * sequence, we need to use a StringBuilder to compose the result.
     */
    AutoPtr<StringBuilder> builder;

    /* the index of the first character not yet appended to the builder. */
    Int32 start = mPos;

    while (mPos < mIn.GetLength()) {
        Int32 c = (Int32)mIn.GetChar(mPos++);
        if ((Char32)c == quote) {
            if (builder == NULL) {
                // a new string avoids leaking memory
                String str = mIn.Substring(start, mPos - 1);
                *result = str;
                return NOERROR;
            }
            else {
                AutoPtr<ArrayOf<Char32> > args = mIn.GetChars();
                builder->Append(*args, start, mPos - 1);
                *result = builder->ToString();
                return NOERROR;
            }
        }

        if ((Char32)c == '\\') {
            if (mPos == mIn.GetLength()) {
                return SyntaxError(String("Unterminated escape sequence"));
            }
            if (builder == NULL) {
                builder = new StringBuilder();
            }
            AutoPtr<ArrayOf<Char32> > args = mIn.GetChars();
            builder->Append(*args, start, mPos - 1);
            Char32 cd;
            ReadEscapeCharacter(&cd);
            builder->AppendChar(cd);
            start = mPos;
        }
    }

    return SyntaxError(String("Unterminated string"));
}

ECode JSONTokener::ReadEscapeCharacter(
    /* [out] */ Char32* c)
{
    VALIDATE_NOT_NULL(c);

    Char32 escaped = mIn.GetChar(mPos++);
    switch (escaped) {
        case 'u': {
            if (mPos + 4 > mIn.GetLength()) {
                return SyntaxError(String("Unterminated escape sequence"));
            }
            String hex = mIn.Substring(mPos, mPos + 4);
            mPos += 4;
            *c = (Char32)StringUtils::ParseInt32(hex, 16);
            return NOERROR;
        }

        case 't':
            *c = '\t';
            return NOERROR;

        case 'b':
            *c = '\b';
            return NOERROR;

        case 'n':
            *c = '\n';
            return NOERROR;

        case 'r':
            *c = '\r';
            return NOERROR;

        case 'f':
            *c = '\f';
            return NOERROR;

        case '\'':
        case '"':
        case '\\':
        default:
            *c = escaped;
            return NOERROR;
    }
}

ECode JSONTokener::ReadLiteral(
    /* [out] */ IInterface** obj)
{
    VALIDATE_NOT_NULL(obj);
    *obj = NULL;

    String literal = NextToInternal(String("{}[]/\\:,=;# \t\f"));

    if (literal.GetLength() == 0) {
        return SyntaxError(String("Expected literal value"));
    }
    else if (String("null").EqualsIgnoreCase(literal)) {
        *obj = JSONObject::Object_NULL;
        REFCOUNT_ADD(*obj);
        return NOERROR;
    }
    else if (String("true").EqualsIgnoreCase(literal)) {
        AutoPtr<IBoolean> bObj = CoreUtils::Convert(TRUE);
        *obj = bObj;
        REFCOUNT_ADD(*obj);
        return NOERROR;
    }
    else if (String("false").EqualsIgnoreCase(literal)) {
        AutoPtr<IBoolean> bObj = CoreUtils::Convert(FALSE);
        *obj = bObj;
        REFCOUNT_ADD(*obj);
        return NOERROR;
    }

    /* try to parse as an integral type... */
    if (literal.IndexOf('.') == -1) {
        Int32 base = 10;
        String number = literal;
        if (number.StartWith("0x") || number.StartWith("0X")) {
            number = number.Substring(2);
            base = 16;
        }
        else if (number.StartWith("0") && number.GetLength() > 1) {
            number = number.Substring(1);
            base = 8;
        }
        // try {
        Int64 longValue = StringUtils::ParseInt64(number, base);
        using Elastos::Core::Math;
        if (longValue <= Math::INT32_MAX_VALUE && longValue >= Math::INT32_MIN_VALUE) {
            AutoPtr<IInteger32> iObj = CoreUtils::Convert((Int32)longValue);
            *obj = iObj;
            REFCOUNT_ADD(*obj);
            return NOERROR;
        }
        else {
            AutoPtr<IInteger64> lObj = CoreUtils::Convert(longValue);
            *obj = lObj;
            REFCOUNT_ADD(*obj);
            return NOERROR;
        }
        //} catch (NumberFormatException e) {
            /*
             * This only happens for integral numbers greater than
             * Long.MAX_VALUE, numbers in exponential form (5e-10) and
             * unquoted strings. Fall through to try floating point.
             */
        //}
    }

    /* ...next try to parse as a floating point... */
    // try {
    Double data = StringUtils::ParseDouble(literal);
    AutoPtr<IDouble> dObj = CoreUtils::Convert(data);
    *obj = dObj;
    REFCOUNT_ADD(*obj);
    return NOERROR;
    // } catch (NumberFormatException ignored) {
    // }

    /* ... finally give up. We have an unquoted string */
    AutoPtr<ICharSequence> cs = CoreUtils::Convert(literal); // a new string avoids leaking memory
    *obj = cs;
    REFCOUNT_ADD(*obj);
    return NOERROR;
}

String JSONTokener::NextToInternal(
    /* [in] */ const String& excluded)
{
    Int32 start = mPos;
    for (; mPos < mIn.GetLength(); mPos++) {
        Char32 c = mIn.GetChar(mPos);
        if (c == '\r' || c == '\n' || excluded.IndexOf(c) != -1) {
            return mIn.Substring(start, mPos);
        }
    }
    return mIn.Substring(start);
}

ECode JSONTokener::ReadObject(
    /* [out] */ IJSONObject** obj)
{
    VALIDATE_NOT_NULL(obj);
    *obj = NULL;

    AutoPtr<IJSONObject> result;
    CJSONObject::New((IJSONObject**)&result);

    /* Peek to see if this is the empty object. */
    Int32 first;
    NextCleanInternal(&first);
    if (first == '}') {
        *obj = result;
        REFCOUNT_ADD(*obj);
        return NOERROR;
    }
    else if (first != -1) {
        mPos--;
    }

    while (TRUE) {
        AutoPtr<IInterface> name;
        NextValue((IInterface**)&name);
        if (IString::Probe(name) == NULL) {
            if (name == NULL) {
                return SyntaxError(String("Names cannot be null"));
            }
            else {
                StringBuilder builder;
                builder += "Names must be strings, but ";
                builder += name;
                builder += " is of type ";
                builder += "?";//
                return SyntaxError(builder.ToString());
                // throw syntaxError("Names must be strings, but " + name
                //         + " is of type " + name.getClass().getName());
            }
        }

        /*
         * Expect the name/value separator to be either a colon ':', an
         * equals sign '=', or an arrow "=>". The last two are bogus but we
         * include them because that's what the original implementation did.
         */
        Int32 separator;
        NextCleanInternal(&separator);
        if (separator != ':' && separator != '=') {;
            StringBuilder builder;
            builder += "Expected ':' after ";
            builder += name;
            return SyntaxError(builder.ToString());
            // throw syntaxError("Expected ':' after " + name);
        }
        if (mPos < mIn.GetLength() && mIn.GetChar(mPos) == '>') {
            mPos++;
        }

        AutoPtr<IInterface> object;
        NextValue((IInterface**)&object);
        String str;
        ICharSequence::Probe(name)->ToString(&str);
        result->Put(str, object);

        Int32 data;
        NextCleanInternal(&data);
        switch (data) {
            case '}':
                *obj = result;
                REFCOUNT_ADD(*obj);
                return NOERROR;
            case ';':
            case ',':
                continue;
            default:
                return SyntaxError(String("Unterminated object"));
        }
    }
    return NOERROR;
}

ECode JSONTokener::ReadArray(
    /* [out] */ IJSONArray** obj)
{
    VALIDATE_NOT_NULL(obj);
    *obj = NULL;

    AutoPtr<IJSONArray> result;
    CJSONArray::New((IJSONArray**)&result);

    /* to cover input that ends with ",]". */
    Boolean hasTrailingSeparator = FALSE;

    while (TRUE) {
        Int32 data;
        NextCleanInternal(&data);
        switch (data) {
            case -1:
                return SyntaxError(String("Unterminated array"));
            case ']':
                if (hasTrailingSeparator) {
                    result->Put((IInterface*)NULL);
                }
                *obj = result;
                REFCOUNT_ADD(*obj);
                return NOERROR;
            case ',':
            case ';':
                /* A separator without a value first means "null". */
                result->Put((IInterface*)NULL);
                hasTrailingSeparator = TRUE;
                continue;
            default:
                mPos--;
        }

        AutoPtr<IInterface> object;
        NextValue((IInterface**)&object);
        result->Put(object);

        Int32 other;
        NextCleanInternal(&other);
        switch (other) {
            case ']':
                *obj = result;
                REFCOUNT_ADD(*obj);
                return NOERROR;
            case ',':
            case ';':
                hasTrailingSeparator = TRUE;
                continue;
            default:
                return SyntaxError(String("Unterminated array"));
        }
    }
    return NOERROR;
}

ECode JSONTokener::SyntaxError(
    /* [in] */ const String& message)
{
    Logger::E("JSONTokener", message);
    return E_JSON_EXCEPTION;
}

ECode JSONTokener::ToString(
    /* [out] */ String* str)
{
    VALIDATE_NOT_NULL(str);
    // consistent with the original implementation
    StringBuilder builder;
    builder += " at character ";
    builder += mPos;
    builder += " of ";
    builder += mIn;

    *str = builder.ToString();
    return NOERROR;
}

ECode JSONTokener::More(
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
    *result = mPos < mIn.GetLength();
    return NOERROR;
}

ECode JSONTokener::Next(
    /* [out] */ Char32* ca)
{
    VALIDATE_NOT_NULL(ca);
    *ca = '\0';

    if (mPos < mIn.GetLength()) {
        *ca = mIn.GetChar(mPos++);
    }
    return NOERROR;
}

ECode JSONTokener::Next(
    /* [in] */ Char32 c,
    /* [out] */ Char32* ca)
{
    VALIDATE_NOT_NULL(ca);
    Char32 result;
    Next(&result);
    if (result != c) {
        StringBuilder builder;
        builder += "Expected ";
        builder += c;
        builder += " but was ";
        builder += result;
        return SyntaxError(builder.ToString());
        // throw syntaxError("Expected " + c + " but was " + result);
    }
    *ca = result;
    return NOERROR;
}

ECode JSONTokener::NextClean(
    /* [out] */ Char32* ca)
{
    VALIDATE_NOT_NULL(ca);
    *ca = '\0';

    Int32 nextCleanInt;
    NextCleanInternal(&nextCleanInt);
    if (nextCleanInt != -1) {
        *ca = (Char32)nextCleanInt;
    }
    return NOERROR;
}

ECode JSONTokener::Next(
    /* [in] */ Int32 length,
    /* [out] */ String* str)
{
    VALIDATE_NOT_NULL(str);
    *str = String(NULL);

    if (mPos + length > mIn.GetLength()) {
        StringBuilder builder;
        builder += length;
        builder += " is out of bounds";
        return SyntaxError(builder.ToString());
        // return SyntaxError(length + " is out of bounds");
    }

    String result = mIn.Substring(mPos, mPos + length);
    mPos += length;

    *str = result;
    return NOERROR;
}

ECode JSONTokener::NextTo(
    /* [in] */ const String& excluded,
    /* [out] */ String* str)
{
    VALIDATE_NOT_NULL(str);
    *str = String(NULL);

    if (excluded.IsNull()) {
        Logger::E("JSONTokener", "excluded == null");
        return E_NULL_POINTER_EXCEPTION;
        // throw new NullPointerException("excluded == null");
    }
    *str = NextToInternal(excluded).Trim();
    return NOERROR;
}

ECode JSONTokener::NextTo(
    /* [in] */ Char32 excluded,
    /* [out] */ String* str)
{
    VALIDATE_NOT_NULL(str);

    String strExcluded = StringUtils::ToString((Int32)excluded);
    *str = NextToInternal(strExcluded).Trim();
    return NOERROR;
}

ECode JSONTokener::SkipPast(
    /* [in] */ const String& thru)
{
    Int32 thruStart = mIn.IndexOf(thru, mPos);
    mPos = thruStart == -1 ? mIn.GetLength() : (thruStart + thru.GetLength());
    return NOERROR;
}

ECode JSONTokener::SkipTo(
    /* [in] */ Char32 to,
    /* [out] */ Char32* ca)
{
    VALIDATE_NOT_NULL(ca);

    Int32 index = mIn.IndexOf(to, mPos);
    if (index != -1) {
        mPos = index;
        *ca = to;
        return NOERROR;
    }
    else {
        *ca = '\0';
        return NOERROR;
    }
}

ECode JSONTokener::Back()
{
    if (--mPos == -1) {
        mPos = 0;
    }
    return NOERROR;
}

Int32 JSONTokener::Dehexchar(
    /* [in] */ Char32 hex)
{
    if (hex >= '0' && hex <= '9') {
        return hex - '0';
    }
    else if (hex >= 'A' && hex <= 'F') {
        return hex - 'A' + 10;
    }
    else if (hex >= 'a' && hex <= 'f') {
        return hex - 'a' + 10;
    }
    else {
        return -1;
    }
}

} //namespace Json
} //namespace Org
