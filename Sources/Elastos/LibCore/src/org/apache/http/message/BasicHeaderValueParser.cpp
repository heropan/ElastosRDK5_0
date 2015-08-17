
#include "BasicHeaderValueParser.h"
#include "CBasicHeaderValueParser.h"
#include "CBasicHeaderElement.h"
#include "CCharArrayBuffer.h"
#include "CParserCursor.h"
#include "CBasicNameValuePair.h"
#include "HTTP.h"
#include <elastos/Logger.h>

using Elastos::Utility::IList;
using Elastos::Utility::IArrayList;
using Elastos::Utility::CArrayList;
using Elastos::Utility::Logging::Logger;
using Org::Apache::Http::Utility::CCharArrayBuffer;
using Org::Apache::Http::Protocol::HTTP;

namespace Org {
namespace Apache {
namespace Http {
namespace Message {

static AutoPtr<IBasicHeaderValueParser> InitDefault()
{
    AutoPtr<CBasicHeaderValueParser> parser;
    CBasicHeaderValueParser::NewByFriend((CBasicHeaderValueParser**)&parser);
    return (IBasicHeaderValueParser*)parser.Get();
}
const AutoPtr<IBasicHeaderValueParser> BasicHeaderValueParser::DEFAULT = InitDefault();
const Char32 BasicHeaderValueParser::PARAM_DELIMITER = ';';
const Char32 BasicHeaderValueParser::ELEM_DELIMITER = ',';

static AutoPtr< ArrayOf<Char32> > InitDelimiters()
{
    AutoPtr< ArrayOf<Char32> > delimiters = ArrayOf<Char32>::Alloc(2);
    (*delimiters)[0] = BasicHeaderValueParser::PARAM_DELIMITER;
    (*delimiters)[1] = BasicHeaderValueParser::ELEM_DELIMITER;
}
const AutoPtr< ArrayOf<Char32> > BasicHeaderValueParser::ALL_DELIMITERS = InitDelimiters();

CAR_INTERFACE_IMPL_2(BasicHeaderValueParser, Object, IBasicHeaderValueParser, IHeaderValueParser)

ECode BasicHeaderValueParser::ParseElements(
    /* [in] */ cosnt String& value,
    /* [in] */ IHeaderValueParser* parser,
    /* [out] */ ArrayOf<IHeaderElement*>** elements)
{
    VALIDATE_NOT_NULL(elements)
    *elements = NULL;

    if (value.IsNull()) {
        Logger::E("BasicHeaderValueParser", "Value to parse may not be null");
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }

    if (parser == NULL)
        parser = BasicHeaderValueParser::DEFAULT;

    AutoPtr<CCharArrayBuffer> buffer;
    CCharArrayBuffer::NewByFriend(value.GetLength(), (CCharArrayBuffer**)&buffer);
    buffer->Append(value);
    AutoPtr<IParserCursor> cursor;
    CParserCursor::NewByFriend(0, value.GetLength(), (CParserCursor**)&cursor);
    return parser->ParseElements(buffer, cursor, elements);
}

ECode BasicHeaderValueParser::ParseElements(
    /* [in] */ ICharArrayBuffer* buffer,
    /* [in] */ IParserCursor* cursor,
    /* [out, callee] */ ArrayOf<IHeaderElement*>** _elements)
{
    VALIDATE_NOT_NULL(elements)
    *_elements = NULL;

    if (buffer == NULL) {
        Logger::E("BasicHeaderValueParser", "Char array buffer may not be null");
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    if (cursor == NULL) {
        Logger::E("BasicHeaderValueParser", "Parser cursor may not be null");
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }

    AutoPtr<IArrayList> arraylist;
    CArrayList::New((IArrayList**)&arraylist);
    AutoPtr<IList> elements = IList::Probe(arraylist);
    Boolean atEnd;
    while (cursor->AtEnd(&atEnd), !atEnd) {
        AutoPtr<IHeaderElement> element;
        ParseHeaderElement(buffer, cursor, (IHeaderElement**)&element);
        String name;
        element->GetName(&name);
        String value;
        if (!(name.GetLength() == 0 && (element->GetValue(&value), value,IsNull()))) {
            elements->Add(element);
        }
    }
    Int32 size;
    elements->GetSize(&size);
    AutoPtr< ArrayOf<IInterface*> > array = ArrayOf<IInterface*>::Alloc(size);
    AutoPtr< ArrayOf<IInterface*> > out;
    elements->ToArray(array, (AutoPtr< ArrayOf<IInterface*> >**)&out);
    ArrayOf<IHeaderElement*> elems = ArrayOf<IHeaderElement*>::Alloc(out->GetLength());
    for (Int32 i = 0; i < out->GetLength(); ++i) {
        elems->Set(i, IHeaderElement::Probe((*out)[i]));
    }
    *_elements = elems;
    REFCOUNT_ADD(*_elements)
    return NOERROR;
}

ECode BasicHeaderValueParser::ParseHeaderElement(
    /* [in] */ cosnt String& value,
    /* [in] */ IHeaderValueParser* parser,
    /* [out] */ IHeaderElement** element)
{
    VALIDATE_NOT_NULL(element)
    *element = NULL;

    if (value.IsNull()) {
        Logger::E("BasicHeaderValueParser", "Value to parse may not be null");
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }

    if (parser == NULL)
        parser = BasicHeaderValueParser::DEFAULT;

    AutoPtr<CCharArrayBuffer> buffer;
    CCharArrayBuffer::NewByFriend(value.GetLength(), (CCharArrayBuffer**)&buffer);
    buffer->Append(value);
    AutoPtr<CParserCursor> cursor;
    CParserCursor::NewByFriend(0, value.GetLength(), (CParserCursor**)&cursor);
    return parser->ParseHeaderElement(buffer, cursor, element);
}

ECode BasicHeaderValueParser::ParseHeaderElement(
    /* [in] */ ICharArrayBuffer* buffer,
    /* [in] */ IParserCursor* cursor,
    /* [out] */ IHeaderElement** element)
{
    VALIDATE_NOT_NULL(element)
    *element = NULL;

    if (buffer == NULL) {
        Logger::E("BasicHeaderValueParser", "Char array buffer may not be null");
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    if (cursor == NULL) {
        Logger::E("BasicHeaderValueParser", "Char array buffer may not be null");
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }

    AutoPtr<INameValuePair> nvp;
    ParseNameValuePair(buffer, cursor, (INameValuePair**)&nvp);
    AutoPtr< ArrayOf<INameValuePair*> > params;
    Boolean atEnd;
    if (cursor->AtEnd(&atEnd), !atEnd) {
        Int32 pos;
        cursor->GetPos(&pos);
        Char32 ch;
        buffer->GetChar(pos - 1, &ch);
        if (ch != ELEM_DELIMITER) {
            ParseParameters(buffer, cursor, (ArrayOf<INameValuePair*>**)&params);
        }
    }
    String name, value;
    nvp->GetName(&name);
    nvp->GetValue(&value);
    return CreateHeaderElement(name, value, params, element);
}

ECode BasicHeaderValueParser::CreateHeaderElement(
    /* [in] */ cosnt String& name,
    /* [in] */ cosnt String& value,
    /* [in] */ ArrayOf<INameValuePair*>* params,
    /* [out] */ IHeaderElement** element)
{
    VALIDATE_NOT_NULL(element)
    return CBasicHeaderElement::New(name, value, params, element);
}

ECode BasicHeaderValueParser::ParseParameters(
    /* [in] */ cosnt String& value,
    /* [in] */ IHeaderValueParser* parser,
    /* [out] */ ArrayOf<INameValuePair*>** elements)
{
    VALIDATE_NOT_NULL(elements)
    *elements = NULL;

    if (value.IsNull()) {
        Logger::E("BasicHeaderValueParser", "Value to parse may not be null");
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }

    if (parser == NULL)
        parser = BasicHeaderValueParser::DEFAULT;

    AutoPtr<CCharArrayBuffer> buffer;
    CCharArrayBuffer::NewByFriend(value.GetLength(), (CCharArrayBuffer**)&buffer);
    buffer->Append(value);
    AutoPtr<CParserCursor> cursor;
    CParserCursor::NewByFriend(0, value.GetLength(), (CParserCursor**)&cursor);
    return parser->ParseParameters(buffer, cursor, elements);
}

ECode BasicHeaderValueParser::ParseParameters(
    /* [in] */ ICharArrayBuffer* buffer,
    /* [in] */ IParserCursor* cursor,
    /* [out, callee] */ ArrayOf<INameValuePair*>** items)
{
    VALIDATE_NOT_NULL(items)
    *items = NULL;

    if (buffer == NULL) {
        Logger::E("BasicHeaderValueParser", "Char array buffer may not be null");
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    if (cursor == NULL) {
        Logger::E("BasicHeaderValueParser", "Parser cursor may not be null");
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }

    Int32 pos, indexTo;
    cursor->GetPos(&pos);
    cursor->GetUpperBound(&indexTo);

    while (pos < indexTo) {
        Char32 ch;
        buffer->GetChar(pos, &ch);
        if (HTTP::IsWhitespace(ch)) {
            pos++;
        }
        else {
            break;
        }
    }
    cursor->UpdatePos(pos);
    Boolean atEnd;
    if (cursor->AtEnd(&atEnd), atEnd) {
        *items = ArrayOf<INameValuePair*>::Alloc(0);
        REFCOUNT_ADD(*items)
        return NOERROR;
    }

    AutoPtr<IArrayList> al;
    CArrayList::New((IArrayList**)&al);
    AutoPtr<IList> params = IList::Probe(al);
    while (cursor->AtEnd(&atEnd), !atEnd) {
        AutoPtr<INameValuePair> param;
        ParseNameValuePair(buffer, cursor, (INameValuePair**)&param);
        params->Add(param);
        Int32 position;
        cursor->GetPos(&position);
        Char32 ch;
        buffer->GetChar(position - 1, &ch);
        if (ch == ELEM_DELIMITER) {
            break;
        }
    }

    Int32 size;
    params->GetSize(&size);
    AutoPtr< ArrayOf<IInterface> > in = ArrayOf<IInterface>::Alloc(size);
    AutoPtr< ArrayOf<IInterface> > out;
    params->ToArray(in, (ArrayOf<IInterface>**)&out);
    ArrayOf<INameValuePair*> pairs = ArrayOf<INameValuePair*>::Alloc(out->GetLength());
    for (Int32 i = 0; i < out->GetLength(); ++i) {
        pairs->Set(i, INameValuePair::Probe((*out)[i]));
    }
    *items = pairs;
    REFCOUNT_ADD(*items)
    return NOERROR;
}

ECode BasicHeaderValueParser::ParseNameValuePair(
    /* [in] */ cosnt String& value,
    /* [in] */ IHeaderValueParser* parser,
    /* [out] */ INameValuePair* pair)
{
    VALIDATE_NOT_NULL(pair)
    *pair = NULL;

    if (value.IsNull()) {
        Logger::E("BasicHeaderValueParser", "Value to parse may not be null");
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }

    if (parser == NULL)
        parser = BasicHeaderValueParser::DEFAULT;

    AutoPtr<CCharArrayBuffer> buffer;
    CCharArrayBuffer::NewByFriend(value.GetLength(), (CCharArrayBuffer**)&buffer);
    buffer->Append(value);
    AutoPtr<CParserCursor> cursor;
    CParserCursor::NewByFriend(0, value.GetLength(), (CParserCursor**)&cursor);
    return parser->ParseNameValuePair(buffer, cursor, pair);
}

ECode BasicHeaderValueParser::ParseNameValuePair(
    /* [in] */ ICharArrayBuffer* buffer,
    /* [in] */ IParserCursor* cursor,
    /* [out] */ INameValuePair* pair)
{
    VALIDATE_NOT_NULL(pair)
    return ParseNameValuePair(buffer, cursor, ALL_DELIMITERS, pair);
}

Boolean BasicHeaderValueParser::IsOneOf(
    /* [in] */ Char32 ch,
    /* [in] */ ArrayOf<Char32>* chs)
{
    if (chs != NULL) {
        for (Int32 i = 0; i < chs->GetLength(); i++) {
            if (ch == (*chs)[i]) {
                return TRUE;
            }
        }
    }
    return FALSE;
}

ECode BasicHeaderValueParser::ParseNameValuePair(
    /* [in] */ ICharArrayBuffer* buffer,
    /* [in] */ IParserCursor* cursor,
    /* [in] */ ArrayOf<Char32>* delimiters,
    /* [out] */ INameValuePair** pair)
{
    VALIDATE_NOT_NULL(pair)
    *pair = NULL;

    if (buffer == NULL) {
        Logger::E("BasicHeaderValueParser", "Char array buffer may not be null");
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    if (cursor == NULL) {
        Logger::E("BasicHeaderValueParser", "Parser cursor may not be null");
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }

    Boolean terminated = FALSE;

    Int32 pos, indexFrom, indexTo;
    cursor->GetPos(&pos);
    cursor->GetPos(&indexFrom);
    cursor->GetUpperBound(&indexTo);

    // Find name
    String name = String(NULL);
    while (pos < indexTo) {
        Char32 ch;
        buffer->GetChar(pos, &ch);
        if (ch == '=') {
            break;
        }
        if (IsOneOf(ch, delimiters)) {
            terminated = TRUE;
            break;
        }
        pos++;
    }

    if (pos == indexTo) {
        terminated = TRUE;
        buffer->SubstringTrimmed(indexFrom, indexTo, &name);
    }
    else {
        buffer->SubstringTrimmed(indexFrom, pos, &name);
        pos++;
    }

    if (terminated) {
        cursor->UpdatePos(pos);
        return CreateNameValuePair(name, String(NULL), pair);
    }

    // Find value
    String value = String(NULL);
    Int32 i1 = pos;

    Boolean qouted = FALSE;
    Boolean escaped = FALSE;
    while (pos < indexTo) {
        Char32 ch;
        buffer->GetChar(pos, &ch);
        if (ch == '"' && !escaped) {
            qouted = !qouted;
        }
        if (!qouted && !escaped && IsOneOf(ch, delimiters)) {
            terminated = TRUE;
            break;
        }
        if (escaped) {
            escaped = FALSE;
        }
        else {
            escaped = qouted && ch == '\\';
        }
        pos++;
    }

    Int32 i2 = pos;
    // Trim leading white spaces
    Char32 c;
    while (i1 < i2 && (buffer->GetChar(i1, &c), HTTP::IsWhitespace(c))) {
        i1++;
    }
    // Trim trailing white spaces
    while ((i2 > i1) && (buffer->GetChar(i2 - 1, &c), HTTP::IsWhitespace(c))) {
        i2--;
    }
    // Strip away quotes if necessary
    Char32 c1, c2;
    if (((i2 - i1) >= 2)
        && (buffer->GetChar(i1, &c1), c1 == '"')
        && (buffer->GetCharAt(i2 - 1, &c2), c2 == '"')) {
        i1++;
        i2--;
    }
    buffe->Substring(i1, i2, &value);
    if (terminated) {
        pos++;
    }
    cursor->UpdatePos(pos);
    return CreateNameValuePair(name, value, pair);
}

ECode BasicHeaderValueParser::CreateNameValuePair(
    /* [in] */ cosnt String& name,
    /* [in] */ cosnt String& value,
    /* [out] */ INameValuePair** pair)
{
    VALIDATE_NOT_NULL(pair)
    return CBasicNameValuePair::New(name, value, pair);
}

} // namespace Message
} // namespace Http
} // namespace Apache
} // namespace Org
