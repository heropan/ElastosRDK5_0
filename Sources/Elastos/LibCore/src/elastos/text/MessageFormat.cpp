#include "MessageFormat.h"
#include "CDecimalFormat.h"
#include "CSimpleDateFormat.h"
#include "CChoiceFormat.h"
#include "CMessageFormatField.h"
#include "CMessageFormat.h"
#include "CStringWrapper.h"
#include "CObjectContainer.h"
#include "CAttributedString.h"
#include "CDecimalFormatSymbols.h"
#include "CDouble.h"
#include "CInteger64.h"
#include "CDecimalFormatSymbols.h"
#include "CNativeDecimalFormat.h"
#include "CBigDecimal.h"

using Elastos::Core::ICharSequence;
using Elastos::Core::Character;
using Elastos::Core::CStringWrapper;
using Elastos::Core::IByte;
using Elastos::Core::IInteger16;
using Elastos::Core::IInteger32;
using Elastos::Core::IInteger64;
using Elastos::Core::CObjectContainer;
using Elastos::Utility::EIID_IDate;
using Elastos::Text::IAttributedString;
using Elastos::Text::CAttributedString;
using Elastos::Text::IDecimalFormatSymbols;
using Elastos::Text::CDecimalFormatSymbols;
using Elastos::Text::IFieldPosition;
using Elastos::Text::CFieldPosition;
using Elastos::Text::CParsePosition;

namespace Elastos {
namespace Text {

static AutoPtr<IMessageFormatField> sInit(const String& name)
{
    AutoPtr<CMessageFormatField> field;
    CMessageFormatField::NewByFriend(name, (CMessageFormatField**)&field);
    return (IMessageFormatField*)field;
}

const AutoPtr<IMessageFormatField> MessageFormat::MessageFormatField::ARGUMENT
    = sInit(String("message argument field"));

ECode MessageFormat::MessageFormatField::Init(
    /* [in] */ const String& fieldName)
{
    return Field::Init(fieldName);
}

MessageFormat::~MessageFormat()
{
}

ECode MessageFormat::Init(
    /* [in] */ const String& tem,
    /* [in] */ ILocale* locale)
{
    mLocale = locale;
    return ApplyPattern(tem);
}

ECode MessageFormat::Init(
    /* [in] */ const String& tem)
{
    AutoPtr<ILocaleHelper> pLocaleHelper;
    CLocaleHelper::AcquireSingleton((ILocaleHelper**)&pLocaleHelper);
    AutoPtr<ILocale> pLocale;
    pLocaleHelper->GetDefault((ILocale**)&pLocale);
    return Init(tem, (ILocale*)pLocale);
}

ECode MessageFormat::ApplyPattern(
    /* [in] */ const String& tem)
{
    if (tem.IsNull()) {
        return NOERROR;
    }

    AutoPtr<ArrayOf<Char32> > charArray = tem.GetChars();
    Int32 length = charArray->GetLength();
    StringBuffer buffer("");
    AutoPtr<IParsePosition> position;
    CParsePosition::New(0, (IParsePosition**)&position);
    List<String> localStrings;
    Int32 argCount = 0;
    AutoPtr<ArrayOf<Int32> > args = ArrayOf<Int32>::Alloc(10);
    Int32 maxArg = -1;
    List< AutoPtr<IFormat> > localFormats;
    Int32 index, arg, offset;
    position->GetIndex(&index);
    Boolean succeeded;
    Char32 ch;

    while (index < length) {
        if (Format::UpTo(tem, position, buffer, '{', &succeeded), succeeded) {
            arg = 0;
            position->GetIndex(&offset);
            if (offset >= length) {
                return E_ILLEGAL_ARGUMENT_EXCEPTION;
            }

            // Get argument number
            while ((ch = (*charArray)[offset++]) != '}' && ch != ',') {
                if (ch < '0' && ch > '9') {
                    return E_ILLEGAL_ARGUMENT_EXCEPTION;
                }

                arg = arg * 10 + (ch - '0');

                if (arg < 0 || offset >= length) {
                    return E_ILLEGAL_ARGUMENT_EXCEPTION;
                }
            }

            offset--;
            position->SetIndex(offset);
            AutoPtr<IFormat> f;
            FAIL_RETURN(ParseVariable(tem, position, (IFormat**)&f));
            localFormats.PushBack(f);
            if (argCount >= args->GetLength()) {
                AutoPtr<ArrayOf<Int32> > newArgs = ArrayOf<Int32>::Alloc(args->GetLength() * 2);
                newArgs->Copy(args);
                args = newArgs;
            }
            (*args)[argCount++] = arg;
            if (arg > maxArg) {
                maxArg = arg;
            }
        }

        String outstr;
        buffer.SubstringEx(0, buffer.GetLength(), &outstr);
        localStrings.PushBack(outstr);
        buffer.Reset();
        position->GetIndex(&index);
    }

    mStrings = ArrayOf<String>::Alloc(localStrings.GetSize());
    List<String>::Iterator it = localStrings.Begin();
    for (Int32 i = 0; i < (Int32)(localStrings.GetSize()); ++i, ++it) {
        (*mStrings)[i] = *it;
    }

    mArgumentNumbers = args;
    mFormats = ArrayOf<IFormat* >::Alloc(argCount);
    List< AutoPtr<IFormat> >::Iterator ite = localFormats.Begin();
    for (Int32 i = 0; i < argCount; ++i, ++ite) {
        mFormats->Set(i, *ite);
    }

    mMaxOffset = argCount - 1;
    mMaxArgumentIndex = maxArg;
    return NOERROR;
}

ECode MessageFormat::FormatToCharacterIterator(
    /* [in] */ IInterface* object,
    /* [out] */ IAttributedCharacterIterator** characterIterator)
{
    VALIDATE_NOT_NULL(characterIterator);
    *characterIterator = NULL;
    VALIDATE_NOT_NULL(object);

    StringBuffer buffer;
    List<AutoPtr<FieldContainer> > fields;

    // format the message, and find fields
    AutoPtr<IFieldPosition> position;
    CFieldPosition::New(0, (IFieldPosition**)&position);
    AutoPtr<IStringBuffer > sb;
    FormatImpl((ArrayOf< IInterface* >*) object, &buffer, position, &fields, (IStringBuffer **)&sb);

    // create an AttributedString with the formatted buffer
    AutoPtr<IAttributedString> as;
    String outstr;
    buffer.SubstringEx(0, buffer.GetLength(),&outstr);
    CAttributedString::New(outstr, (IAttributedString**)&as);

    // add MessageFormat field attributes and values to the AttributedString
    List<AutoPtr<FieldContainer> >::Iterator fc = fields.Begin();
    for ( ; fc != fields.End(); ++fc) {
        FAIL_RETURN(as->AddAttributeEx((*fc)->mAttribute, (*fc)->mValue, (*fc)->mStart, (*fc)->mEnd));
    }

    // return the CharacterIterator from AttributedString
    return as->GetIterator(characterIterator);
}

ECode MessageFormat::FormatObjects(
    /* [in] */ ArrayOf< IInterface* >* objects,
    /* [in] */ IStringBuffer * buffer,
    /* [in] */ IFieldPosition* field,
    /* [out] */ IStringBuffer ** value)
{
    return FormatImpl(objects, buffer, field, NULL, value);
}

ECode MessageFormat::FormatImpl(
    /* [in] */ ArrayOf< IInterface* >* objects,
    /* [in] */ IStringBuffer* inbuffer,
    /* [in] */ IFieldPosition* position,
    /* [in] */ List<AutoPtr<FieldContainer> >* fields,
    /* [out] */ IStringBuffer ** value)
{
    VALIDATE_NOT_NULL(value);
    *value = NULL;
    VALIDATE_NOT_NULL(inbuffer);

    StringBuffer * buffer = (StringBuffer *)inbuffer;
    AutoPtr<IFieldPosition> passedField;
    CFieldPosition::New(0, (IFieldPosition**)&passedField);

    Int32 begin;
    String result;
    for (Int32 i = 0; i <= mMaxOffset; i++) {
        *buffer += (*mStrings)[i];
        begin = buffer->GetLength();
        AutoPtr<IInterface> arg;
        if (objects != NULL && (*mArgumentNumbers)[i] < objects->GetLength()) {
            arg = (*objects)[(*mArgumentNumbers)[i]];
        } else {
            *buffer += '{';
            *buffer += (*mArgumentNumbers)[i];
            *buffer += '}';
            HandleArgumentField(begin, buffer->GetLength(), (*mArgumentNumbers)[i],
                    position, fields);
            continue;
        }

        AutoPtr<IFormat> format = (*mFormats)[i];
        if (format == NULL || arg == NULL) {
            if (INumber::Probe(arg) != NULL) {
                format = NULL;
                NumberFormat::GetInstance((INumberFormat**)&format);
            }
            else if (IDate::Probe(arg) != NULL) {
                format = NULL;
                DateFormat::GetInstance((IDateFormat**)&format);
            }
            else {
                *buffer += arg;
                HandleArgumentField(begin, buffer->GetLength(),
                        (*mArgumentNumbers)[i], position, fields);
                continue;
            }
        }

        if (IChoiceFormat::Probe(format) != NULL) {
            format->FormatObject(arg, &result);
            AutoPtr<IMessageFormat> mf;
            CMessageFormat::New(result, (IMessageFormat**)&mf);
            mf->SetLocale(mLocale);
            AutoPtr<IStringBuffer > outres;
            mf->FormatObjects(objects, buffer , passedField, (IStringBuffer **)&outres);
            HandleArgumentField(begin, buffer->GetLength(), (*mArgumentNumbers)[i], position, fields);
            HandleFormat(format, arg, begin, fields);
        }
        else {
            AutoPtr<IStringBuffer> outres;
            format->FormatObjectEx(arg, buffer, passedField, (IStringBuffer **)&outres);
            HandleArgumentField(begin, buffer->GetLength(), (*mArgumentNumbers)[i], position, fields);
            HandleFormat(format, arg, begin, fields);
        }
    }
    if (mMaxOffset + 1 < mStrings->GetLength()) {
        (*buffer) += (*mStrings)[mMaxOffset + 1];
    }

    *value = buffer;
    REFCOUNT_ADD(*value);
    return NOERROR;
}

ECode MessageFormat::HandleArgumentField(
    /* [in] */ Int32 begin,
    /* [in] */ Int32 end,
    /* [in] */ Int32 argIndex,
    /* [in] */ IFieldPosition* position,
    /* [in] */ List<AutoPtr<FieldContainer> >* fields)
{
    if (fields != NULL) {
        AutoPtr<FieldContainer> fc = new FieldContainer(begin, end,
            (IAttributedCharacterIteratorAttribute*)MessageFormatField::ARGUMENT,
            (IInterface*)argIndex);
        fields->PushBack(fc);
    }
    else if (position != NULL) {
        Int32 endIndex;
        position->GetEndIndex(&endIndex);
        AutoPtr<IFormatField> fa;
        position->GetFieldAttribute((IAttributedCharacterIteratorAttribute**)&fa);
        if (fa == MessageFormatField::ARGUMENT && endIndex == 0) {
            position->SetBeginIndex(begin);
            position->SetEndIndex(end);
        }
    }

    return NOERROR;
}

MessageFormat::FieldContainer::FieldContainer(
    /* [in] */ Int32 start,
    /* [in] */ Int32 end,
    /* [in] */ IAttributedCharacterIteratorAttribute* attribute,
    /* [in] */ IInterface* value)
{
    mStart = start;
    mEnd = end;
    mAttribute = attribute;
    mValue = value;
}

ECode MessageFormat::HandleFormat(
    /* [in] */ IFormat* format,
    /* [in] */ IInterface* arg,
    /* [in] */ Int32 begin,
    /* [in] */ List<AutoPtr<FieldContainer> >* fields)
{
    VALIDATE_NOT_NULL(fields);
    VALIDATE_NOT_NULL(format);

    AutoPtr<IAttributedCharacterIterator> iterator;
    format->FormatToCharacterIterator(arg, (IAttributedCharacterIterator**)&iterator);
    Int32 index, endIndex, start, end;
    Char32 character;
    iterator->GetIndex(&index);
    iterator->GetEndIndex(&endIndex);
    while (index != endIndex) {
        iterator->GetRunStart(&start);
        iterator->GetRunLimit(&end);

//            Iterator<?> it = iterator.getAttributes().keySet().iterator();
//            while (it.hasNext()) {
//                AttributedCharacterIterator.Attribute attribute = (AttributedCharacterIterator.Attribute) it
//                        .next();
//                Object value = iterator.getAttribute(attribute);
//                fields.add(new FieldContainer(begin + start, begin + end,
//                        attribute, value));
//            }
        iterator->SetIndex(end, &character);
    }
    return NOERROR;
}

ECode MessageFormat::FormatObjectEx(
    /* [in] */ IInterface* object,
    /* [in] */ IStringBuffer * buffer,
    /* [in] */ IFieldPosition* field,
    /* [out] */ IStringBuffer ** value)
{
    return FormatObjects((ArrayOf< IInterface* >*) object, buffer, field, value);
}

ECode MessageFormat::GetFormats(
    /* [out] */ ArrayOf< IFormat* >** arrayOfInstances)
{
    VALIDATE_NOT_NULL(arrayOfInstances);
    *arrayOfInstances = mFormats->Clone();
    REFCOUNT_ADD(*arrayOfInstances);
    return NOERROR;
}

ECode MessageFormat::GetFormatsByArgumentIndex(
    /* [out] */ ArrayOf< IFormat* >** arrayOfInstances)
{
    VALIDATE_NOT_NULL(arrayOfInstances);
    AutoPtr<ArrayOf< IFormat* > > answer = ArrayOf< IFormat* >::Alloc(mMaxArgumentIndex + 1);
    for (Int32 i = 0; i < mMaxOffset + 1; i++) {
        answer->Set((*mArgumentNumbers)[i], (*mFormats)[i]);
    }
    *arrayOfInstances = answer;
    REFCOUNT_ADD(*arrayOfInstances)
    return NOERROR;
}

ECode MessageFormat::SetFormatByArgumentIndex(
    /* [in] */ Int32 argIndex,
    /* [in] */ IFormat* format)
{
    for (Int32 i = 0; i < mMaxOffset + 1; i++) {
        if ((*mArgumentNumbers)[i] == argIndex) {
            mFormats->Set(i, format);
        }
    }
    return NOERROR;
}

ECode MessageFormat::SetFormatsByArgumentIndexEx(
    /* [in] */ ArrayOf< IFormat* >* formats)
{
    for (Int32 j = 0; j < mFormats->GetLength(); j++) {
        for (Int32 i = 0; i < mMaxOffset + 1; i++) {
            if ((*mArgumentNumbers)[i] == j) {
                mFormats->Set(i, (*formats)[j]);
            }
        }
    }
    return NOERROR;
}

ECode MessageFormat::GetLocale(
    /* [out] */ ILocale** locale)
{
    VALIDATE_NOT_NULL(locale);
    *locale = mLocale;
    REFCOUNT_ADD(*locale);
    return NOERROR;
}

ECode MessageFormat::Parse(
    /* [in] */ const String& string,
    /* [out, callee] */ ArrayOf< IInterface* >** value)
{
    VALIDATE_NOT_NULL(value);
    *value = NULL;

    AutoPtr<IParsePosition> position;
    CParsePosition::New(0, (IParsePosition**)&position);
    AutoPtr<ArrayOf< IInterface* > > result;
    ParseEx(string, position, (ArrayOf<IInterface*>**)&result);

    Int32 index;
    position->GetIndex(&index);
    if (index == 0) {
        return E_PARSE_EXCEPTION;
    }

    if (result) {
        *value = result;
        REFCOUNT_ADD(*value)
    }
    return NOERROR;
}

ECode MessageFormat::ParseEx(
    /* [in] */ const String& string,
    /* [in] */ IParsePosition* position,
    /* [out, callee] */ ArrayOf< IInterface* >** value)
{
    VALIDATE_NOT_NULL(value);
    *value = NULL;

    if (string.IsNull()) {
        // *value = EmptyArray::OBJECT;
        return NOERROR;
    }

    AutoPtr<IParsePosition> internalPos;
    CParsePosition::New(0, (IParsePosition**)&internalPos);
    Int32 offset, next, errorIndex;
    position->GetIndex(&offset);
    AutoPtr<ArrayOf< IInterface* > > result = ArrayOf< IInterface* >::Alloc(mMaxArgumentIndex + 1);

    String sub, str, target;
    for (Int32 i = 0; i <= mMaxOffset; i++) {
        sub = (*mStrings)[i];
        if (!string.RegionMatches(offset, sub, 0, (Int32)(sub.GetLength()))) {
            position->SetErrorIndex(offset);
            *value = NULL;
            return NOERROR;
        }

        offset += (Int32)(sub.GetLength());
        AutoPtr<IInterface> parse;
        AutoPtr<IFormat> format = (*mFormats)[i];

        if (format == NULL) {
            if (i + 1 < mStrings->GetLength()) {
                next = string.IndexOf((*mStrings)[i + 1], offset);
                if (next == -1) {
                    position->SetErrorIndex(offset);
                    *value = NULL;
                    return NOERROR;
                }

                target = string.Substring(offset, next);
                offset = next;
            }
            else {
                target = string.Substring(offset);
                offset = (Int32)(string.GetLength());
            }

            CStringWrapper::New(target, (ICharSequence**)&parse);
        }
        else {
            internalPos->SetIndex(offset);
            format->ParseObjectEx(string, internalPos, (IInterface**)&parse);
            internalPos->GetErrorIndex(&errorIndex);
            if (errorIndex != -1) {
                position->SetErrorIndex(offset);
                *value = NULL;
                return NOERROR;
            }
            internalPos->GetIndex(&offset);
        }

        result->Set((*mArgumentNumbers)[i], parse);
    }

    if (mMaxOffset + 1 < mStrings->GetLength()) {
        sub = (*mStrings)[mMaxOffset + 1];
        if (!string.RegionMatches(offset, sub, 0, (Int32)(sub.GetLength()))) {
            position->SetErrorIndex(offset);
            *value = NULL;
            return NOERROR;
        }
        offset += (Int32)(sub.GetLength());
    }
    position->SetIndex(offset);

    *value = result;
    REFCOUNT_ADD(*value)
    return NOERROR;
}

ECode MessageFormat::ParseObjectEx(
    /* [in] */ const String& string,
    /* [in] */ IParsePosition* position,
    /* [out] */ IInterface** result)
{
    VALIDATE_NOT_NULL(result);
    *result = NULL;

    AutoPtr<ArrayOf<IInterface*> > objects;
    FAIL_RETURN(ParseEx(string, position, (ArrayOf<IInterface*>**)&objects));
    if (objects != NULL) {
        AutoPtr<IObjectContainer> bc;
        CObjectContainer::New((IObjectContainer**)&bc);
        for (Int32 i = 0; i < objects->GetLength(); ++i) {
            bc->Add((*objects)[i]);
        }

        *result = bc->Probe(EIID_IInterface);
        REFCOUNT_ADD(*result);
    }

    return NOERROR;
}

ECode MessageFormat::Match(
    /* [in] */ const String& string,
    /* [in] */ IParsePosition* position,
    /* [in] */ Boolean last,
    /* [in] */ ArrayOf<String>* tokens,
    /* [out] */ Int32* value)
{
    VALIDATE_NOT_NULL(value);
    *value = -1;
    VALIDATE_NOT_NULL(position);
    VALIDATE_NOT_NULL(tokens);

    if (string.IsNull()) return NOERROR;

    AutoPtr<ArrayOf<Char32> > charArray = string.GetChars();
    Int32 length = charArray->GetLength();
    Int32 offset;
    position->GetIndex(&offset);
    Int32 token = -1;
    while (offset < length && Character::IsWhitespace((*charArray)[offset])) {
        offset++;
    }

    for (Int32 i = tokens->GetLength() - 1; i >= 0; --i) {
        if (string.RegionMatchesIgnoreCase(offset, (*tokens)[i], 0, (*tokens)[i].GetLength())) {
            token = i;
            break;
        }
    }

    if (token == -1) {
        return NOERROR;
    }
    offset += (*tokens)[token].GetLength();
    while (offset < length && Character::IsWhitespace((*charArray)[offset])) {
        offset++;
    }
    Char32 ch;
    if (offset < length
            && ((ch = (*charArray)[offset]) == '}' || (!last && ch == ','))) {
        position->SetIndex(offset + 1);
        *value = token;
        return NOERROR;
    }

    return NOERROR;
}

ECode MessageFormat::ParseVariable(
    /* [in] */ const String& string,
    /* [in] */ IParsePosition* position,
    /* [out] */ IFormat** value)
{
    VALIDATE_NOT_NULL(value);
    *value = NULL;

    AutoPtr<ArrayOf<Char32> > charArray = string.GetChars();
    Int32 length = charArray->GetLength();
    Int32 offset;
    position->GetIndex(&offset);
    Char32 ch;
    if (offset >= length || ((ch = (*charArray)[offset++]) != '}' && ch != ',')) {
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    position->SetIndex(offset);
    if (ch == '}') {
        return NOERROR;
    }

    Int32 type;
    AutoPtr<ArrayOf<String> > tokens1 = ArrayOf<String>::Alloc(4);
    AutoPtr<ArrayOf<String> > tokens2 = ArrayOf<String>::Alloc(4);
    (*tokens1)[0] = String("time");
    (*tokens1)[1] = String("date");
    (*tokens1)[2] = String("number");
    (*tokens1)[3] = String("choice");

    Match(string, position, FALSE, tokens1, &type);
    if (type == -1) {
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }

    StringBuffer buffer("");
    Int32 index;
    position->GetIndex(&index);
    ch = (*charArray)[index - 1];

    AutoPtr<IDateFormat> dv;
    AutoPtr<INumberFormat> nv;
    switch (type) {
    case 0: // time
    case 1: // date
        if (ch == '}') {
            if (type == 1) {
                DateFormat::GetDateInstanceEx2(
                    IDateFormat::DEFAULT, mLocale, (IDateFormat**)&dv);
            }
            else {
                DateFormat::GetTimeInstanceEx2(
                    IDateFormat::DEFAULT, mLocale, (IDateFormat**)&dv);
            }

            *value = dv;
            REFCOUNT_ADD(*value);
            return NOERROR;
        }
        (*tokens2)[0] = String("full");
        (*tokens2)[1] = String("long");
        (*tokens2)[2] = String("medium");
        (*tokens2)[3] = String("short");

        Int32 dateStyle;
        Match(string, position, TRUE, tokens2, &dateStyle);
        if (dateStyle == -1) {
            Boolean succeeded;
            Format::UpToWithQuotes(string, position, buffer, '}', '{', &succeeded);
            AutoPtr<ISimpleDateFormat> sdf;
            String outstr;
            buffer.SubstringEx(0, buffer.GetLength(),&outstr);
            CSimpleDateFormat::New(outstr, mLocale.Get(), (ISimpleDateFormat**)&sdf);
            *value = sdf.Get();
            REFCOUNT_ADD(*value);
            return NOERROR;
        }

        switch (dateStyle) {
            case 0:
                dateStyle = IDateFormat::FULL;
                break;
            case 1:
                dateStyle = IDateFormat::LONG;
                break;
            case 2:
                dateStyle = IDateFormat::MEDIUM;
                break;
            case 3:
                dateStyle = IDateFormat::SHORT;
                break;
        }

        if (type == 1) {
            DateFormat::GetDateInstanceEx2(dateStyle, mLocale.Get(), (IDateFormat**)&dv);
        }
        else {
            DateFormat::GetTimeInstanceEx2(dateStyle, mLocale.Get(), (IDateFormat**)&dv);
        }
        *value = dv;
        REFCOUNT_ADD(*value);
        return NOERROR;

    case 2: // number
        if (ch == '}') {
            NumberFormat::GetInstance(mLocale, (INumberFormat**)value);
            return NOERROR;
        }
        Int32 numberStyle;
        AutoPtr<ArrayOf<String> > tokens3 = ArrayOf<String>::Alloc(3);
        (*tokens3)[0] = String("currency");
        (*tokens3)[1] = String("percent");
        (*tokens3)[2] = String("integer");
        Match(string, position, TRUE, tokens3, &numberStyle);
        if (numberStyle == -1) {
            Boolean succeeded;
            FAIL_RETURN(Format::UpToWithQuotes(string, position, buffer, '}', '{', &succeeded));
            AutoPtr<IDecimalFormatSymbols> dfs;
            CDecimalFormatSymbols::New(mLocale, (IDecimalFormatSymbols**)&dfs);
            AutoPtr<IDecimalFormat> df;
            String outstr;
            buffer.SubstringEx(0, buffer.GetLength(),&outstr);
            CDecimalFormat::New(outstr, dfs, (IDecimalFormat**)&df);
            *value = df.Get();
            REFCOUNT_ADD(*value);
            return NOERROR;
        }

        switch (numberStyle) {
            case 0: // currency
                NumberFormat::GetCurrencyInstance(mLocale, (INumberFormat**)&nv);
                break;
            case 1: // percent
                NumberFormat::GetPercentInstance(mLocale, (INumberFormat**)&nv);
                break;
            default:
                NumberFormat::GetIntegerInstance(mLocale, (INumberFormat**)&nv);
                break;
        }
        *value = nv.Get();
        REFCOUNT_ADD(*value);
        return NOERROR;
    }

    Boolean succeeded;
    Format::UpToWithQuotes(string, position, buffer, '}', '{', &succeeded);
    AutoPtr<IChoiceFormat> cf;
    String outstr;
    buffer.SubstringEx(0, buffer.GetLength(), &outstr);
    CChoiceFormat::New(outstr, (IChoiceFormat**)&cf);
    *value = cf.Get();
    REFCOUNT_ADD(*value);
    return NOERROR;
}

ECode MessageFormat::SetFormat(
    /* [in] */ Int32 offset,
    /* [in] */ IFormat* format)
{
    if (offset < 0) {
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    mFormats->Set(offset, format);
    return NOERROR;
}

ECode MessageFormat::SetFormats(
    /* [in] */ ArrayOf< IFormat* >* formats)
{
    Int32 min = mFormats->GetLength();
    if (formats->GetLength() < min) {
        min = formats->GetLength();
    }
    for (Int32 i = 0; i < min; i++) {
        mFormats->Set(i, (*formats)[i]);
    }
    return NOERROR;
}

ECode MessageFormat::SetLocale(
    /* [in] */ ILocale* locale)
{
    mLocale = locale;
    for (Int32 i = 0; i <= mMaxOffset; i++) {
        AutoPtr<IFormat> format = (*mFormats)[i];
        // java specification undefined for null argument, change into
        // a more tolerant implementation
        if (IDecimalFormat::Probe(format) != NULL) {
            String pattern;
            IDecimalFormat* df = IDecimalFormat::Probe(format);
            assert(df != NULL);
            df->ToPattern(&pattern);
            AutoPtr<IDecimalFormatSymbols> dfs;
            CDecimalFormatSymbols::New(mLocale, (IDecimalFormatSymbols**)&dfs);
            AutoPtr<IDecimalFormat> dff;
            CDecimalFormat::New(pattern, dfs, (IDecimalFormat**)&dff);
            mFormats->Set(i, dff);
        }
        else if (ISimpleDateFormat::Probe(format) != NULL) {
            String pattern;
            ISimpleDateFormat* sdf = ISimpleDateFormat::Probe(format);
            sdf->ToPattern(&pattern);
            AutoPtr<ISimpleDateFormat> sdff;
            CSimpleDateFormat::New(pattern, mLocale, (ISimpleDateFormat**)&sdff);
            mFormats->Set(i, sdff);
        }
    }
    return NOERROR;
}

ECode MessageFormat::DecodeDecimalFormat(
    /* [in] */ StringBuffer* buffer,
    /* [in] */ IFormat* format,
    /* [out] */ String* value)
{
    VALIDATE_NOT_NULL(value);
    *value = String(NULL);
    VALIDATE_NOT_NULL(buffer);
    VALIDATE_NOT_NULL(format);

    *buffer += ",number";

    INumberFormat* nff = INumberFormat::Probe(format);
    AutoPtr<INumberFormat> nfn, nfi, nfc, nfp;
    NumberFormat::GetNumberInstance(mLocale, (INumberFormat**)&nfn);
    NumberFormat::GetIntegerInstance(mLocale, (INumberFormat**)&nfi);
    NumberFormat::GetCurrencyInstance(mLocale, (INumberFormat**)&nfc);
    NumberFormat::GetPercentInstance(mLocale, (INumberFormat**)&nfp);

    Boolean result = FALSE;
    if (nff->Equals(nfn, &result), result) {
        // Empty block
    } else if (nff->Equals(nfi, &result), result) {
        *buffer += ",integer";
    } else if (nff->Equals(nfc, &result), result) {
        *buffer += ",currency";
    } else if (nff->Equals(nfp, &result), result) {
        *buffer += ",percent";
    } else {
        *buffer += ',';
        IDecimalFormat* nfff = IDecimalFormat::Probe(format);
        nfff->ToPattern(value);
    }
    return NOERROR;
}

ECode MessageFormat::DecodeSimpleDateFormat(
    /* [in] */ StringBuffer* buffer,
    /* [in] */ IFormat* format,
    /* [out] */ String* value)
{
    VALIDATE_NOT_NULL(value);
    *value = String(NULL);
    VALIDATE_NOT_NULL(format);
    VALIDATE_NOT_NULL(buffer);

    IDateFormat* dateFormat = IDateFormat::Probe(format);

    AutoPtr<IDateFormat> dft, dft2, dft3, dft4, dfd, dfd2, dfd3, dfd4;
    DateFormat::GetTimeInstanceEx2(IDateFormat::DEFAULT, mLocale, (IDateFormat**)&dft);
    DateFormat::GetDateInstanceEx2(IDateFormat::DEFAULT, mLocale, (IDateFormat**)&dfd);
    DateFormat::GetTimeInstanceEx2(IDateFormat::SHORT, mLocale, (IDateFormat**)&dft2);
    DateFormat::GetDateInstanceEx2(IDateFormat::SHORT, mLocale, (IDateFormat**)&dfd2);
    DateFormat::GetTimeInstanceEx2(IDateFormat::LONG, mLocale, (IDateFormat**)&dft3);
    DateFormat::GetDateInstanceEx2(IDateFormat::LONG, mLocale, (IDateFormat**)&dfd3);
    DateFormat::GetTimeInstanceEx2(IDateFormat::FULL, mLocale, (IDateFormat**)&dft4);
    DateFormat::GetDateInstanceEx2(IDateFormat::FULL, mLocale, (IDateFormat**)&dfd4);

    Boolean result = FALSE;
    if (dateFormat->Equals(dft, &result), result) {
        *buffer += ",time";
    }
    else if (dateFormat->Equals(dfd, &result), result) {
        *buffer += ",date";
    }
    else if (dateFormat->Equals(dft2, &result), result) {
        *buffer += ",time,short";
    }
    else if (dateFormat->Equals(dfd2, &result), result) {
        *buffer += ",date,short";
    }
    else if (dateFormat->Equals(dft3, &result), result) {
        *buffer += ",time,long";
    }
    else if (dateFormat->Equals(dfd3, &result), result) {
        *buffer += ",date,long";
    }
    else if (dateFormat->Equals(dft4, &result), result) {
        *buffer += ",time,full";
    }
    else if (dateFormat->Equals(dfd4, &result), result) {
        *buffer += ",date,full";
    }
    else {
        *buffer += ",date,";
        ISimpleDateFormat* sdf = ISimpleDateFormat::Probe(format);
        sdf->ToPattern(value);
    }

    return NOERROR;
}

ECode MessageFormat::ToPattern(
    /* [out] */ String* value)
{
    VALIDATE_NOT_NULL(value);

    StringBuffer buffer;
    String pattern;
    Int32 index = 0, count = 0, length;
    Char32 ch;
    Boolean quote;

    for (Int32 i = 0; i <= mMaxOffset; i++) {
        AppendQuoted(&buffer, (*mStrings)[i]);
        buffer += "{";
        buffer += (*mArgumentNumbers)[i];
        AutoPtr<IFormat> format = (*mFormats)[i];
        if (IChoiceFormat::Probe(format) != NULL) {
            buffer += ",choice,";
            IChoiceFormat* cf = IChoiceFormat::Probe(format);
            cf->ToPattern(&pattern);
        }
        else if (IDecimalFormat::Probe(format) != NULL) {
            DecodeDecimalFormat(&buffer, format, &pattern);
        }
        else if (ISimpleDateFormat::Probe(format) != NULL) {
            DecodeSimpleDateFormat(&buffer, format, &pattern);
        }
        else if (format != NULL) {
            return E_ILLEGAL_ARGUMENT_EXCEPTION;
        }

        if (!pattern.IsNull()) {
            quote = FALSE;
            index = 0, count = 0;
            AutoPtr<ArrayOf<Char32> > charArray = pattern.GetChars();
            length = charArray->GetLength();
            while (index < length) {
                ch = (*charArray)[index++];
                if (ch == '\'') {
                    quote = !quote;
                }
                if (!quote) {
                    if (ch == '{') {
                        count++;
                    }
                    if (ch == '}') {
                        if (count > 0) {
                            count--;
                        } else {
                            buffer += "'}";
                            ch = '\'';
                        }
                    }
                }
                buffer += ch;
            }
        }
        buffer += "}";
    }
    if (mMaxOffset + 1 < mStrings->GetLength()) {
        AppendQuoted(&buffer, (*mStrings)[mMaxOffset + 1]);
    }
    buffer.SubstringEx(0, buffer.GetLength(),value);
    return NOERROR;
}

ECode MessageFormat::AppendQuoted(
    /* [in] */ StringBuffer* buffer,
    /* [in] */ const String& string)
{
    VALIDATE_NOT_NULL(buffer);

    if (string.IsNull()) return NOERROR;

    AutoPtr<ArrayOf<Char32> > charArray = string.GetChars();
    Int32 length = charArray->GetLength();
    Char32 ch;
    for (Int32 i = 0; i < length; i++) {
        ch = (*charArray)[i];
        if (ch == '{' || ch == '}') {
            *buffer += '\'';
            *buffer += ch;
            *buffer += '\'';
        }
        else {
            *buffer += ch;
        }
    }
    return NOERROR;
}

ECode MessageFormat::GetMaxOffset(
    /* [out] */ Int32* maxOffset)
{
    VALIDATE_NOT_NULL(maxOffset)
    *maxOffset = mMaxOffset;
    return NOERROR;
}

ECode MessageFormat::GetArgumentNumbers(
    /* [out,callee] */ ArrayOf<Int32>** argumentNumbers)
{
    VALIDATE_NOT_NULL(*argumentNumbers)
    *argumentNumbers = mArgumentNumbers->Clone();
    REFCOUNT_ADD(*argumentNumbers);
    return NOERROR;
}

ECode MessageFormat::GetStrings(
    /* [out,callee] */ ArrayOf<String>** strings)
{
    VALIDATE_NOT_NULL(*strings)
    *strings = mStrings->Clone();
    REFCOUNT_ADD(*strings);
    return NOERROR;
}

ECode MessageFormat::Equals(
    /* [in] */ IInterface* object,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result)
    if (this->Probe(EIID_IMessageFormat) == IMessageFormat::Probe(object)) {
        *result = TRUE;
        return NOERROR;
    }
    if (object->Probe(EIID_IMessageFormat) == NULL) {
        *result = FALSE;
        return NOERROR;
    }
    AutoPtr<IMessageFormat> format = (IMessageFormat*)object;
    Int32 mo;
    format->GetMaxOffset(&mo);
    if (mMaxOffset != mo) {
        *result = FALSE;
        return NOERROR;
    }
    // Must use a loop since the lengths may be different due
    // to serialization cross-loading
    AutoPtr<ArrayOf<Int32> > an;
    format->GetArgumentNumbers((ArrayOf<Int32>**)&an);
    for (Int32 i = 0; i < mMaxOffset; i++) {
        if ((*mArgumentNumbers)[i] != (*an)[i]) {
            *result = FALSE;
            return NOERROR;
        }
    }
    AutoPtr<ILocale> locale;
    format->GetLocale((ILocale**)&locale);
    Boolean res1, res2, res3;
    mLocale->Equals(locale, &res1);
    AutoPtr<ArrayOf<String> > strings;
    format->GetStrings((ArrayOf<String>**)&strings);
    res2 = mStrings->Equals(strings);
    AutoPtr<ArrayOf<IFormat*> > formats;
    format->GetFormats((ArrayOf<IFormat*>**)&formats);
    res3 = mFormats->Equals(formats);
    *result = res1 && res2 && res3;
    return NOERROR;
}

} // namespace Text
} // namespace Elastos
