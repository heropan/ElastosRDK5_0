
#include "elastos/droid/ext/frameworkdef.h"
#include "elastos/droid/internal/utility/XmlUtils.h"
#include "elastos/droid/internal/utility/Xml.h"
#include <elastos/core/StringUtils.h>
#include <elastos/core/StringBuilder.h>
#include <elastos/etl/List.h>
#include <elastos/utility/logging/Slogger.h>

#ifdef DROID_CORE
#include "elastos/droid/internal/utility/CFastXmlSerializer.h"
#endif

using Elastos::Droid::Utility::CFastXmlSerializer;
using Elastos::Droid::Utility::IFastXmlSerializer;
using Elastos::Core::StringBuilder;
using Elastos::Core::StringUtils;
using Elastos::Core::CInteger32;
using Elastos::Core::IInteger32;
using Elastos::Core::CInteger64;
using Elastos::Core::IInteger64;
using Elastos::Core::CBoolean;
using Elastos::Core::IBoolean;
using Elastos::Core::CFloat;
using Elastos::Core::IFloat;
using Elastos::Core::CDouble;
using Elastos::Core::IDouble;
using Elastos::Core::IByte;
using Elastos::Core::CStringWrapper;
using Elastos::Core::CArrayOf;
using Elastos::Core::CObjectContainer;
using Elastos::Core::EIID_IInteger32;
using Elastos::Utility::CObjectMap;
using Elastos::Utility::Etl::List;
using Elastos::Utility::Logging::Slogger;
using Org::Kxml2::IO::CKXmlSerializer;

namespace Elastos {
namespace Droid {
namespace Internal {
namespace Utility {

const String XmlUtils::TAG("XmlUtils");

ECode XmlUtils::SkipCurrentTag(
    /* [in] */ IXmlPullParser* parser)
{
    Int32 outerDepth = 0;
    parser->GetDepth(&outerDepth);
    Int32 type = 0;
    Int32 depth = 0;
    while ((parser->Next(&type), type) != IXmlPullParser::END_DOCUMENT
           && (type != IXmlPullParser::END_TAG
                   || (parser->GetDepth(&depth), depth) > outerDepth)) {
    }
    return NOERROR;
}

Int32 XmlUtils::ConvertValueToList(
    /* [in] */ ICharSequence* value,
    /* [in] */ const ArrayOf<String>& options,
    /* [in] */ Int32 defaultValue)
{
    if (NULL != value) {
        String str;
        value->ToString(&str);

        for (Int32 i = 0; i < options.GetLength(); i++) {
            if (str.Equals(options[i]))
                return i;
        }
    }

    return defaultValue;
}

Boolean XmlUtils::ConvertValueToBoolean(
    /* [in] */ ICharSequence* value,
    /* [in] */ Boolean defaultValue)
{
    Boolean result = FALSE;

    if (NULL == value) return defaultValue;

    String str;
    value->ToString(&str);
    if (str.Equals("1") ||  str.Equals("true") ||  str.Equals("TRUE")) {
        result = TRUE;
    }

    return result;
}

Int32 XmlUtils::ConvertValueToInt32(
    /* [in] */ ICharSequence* charSeq,
    /* [in] */ Int32 defaultValue)
{
    if (NULL == charSeq) return defaultValue;

    String nm;
    charSeq->ToString(&nm);

    // XXX This code is copied from Integer.decode() so we don't
    // have to instantiate an Integer!

    Int32 sign = 1;
    Int32 index = 0;
    AutoPtr<ArrayOf<Char32> > charArray = nm.GetChars();
    Int32 len = charArray->GetLength();
    Int32 base = 10;

    if ('-' == (*charArray)[0]) {
        sign = -1;
        index++;
    }

    if ('0' == (*charArray)[index]) {
        //  Quick check for a zero by itself
        if (index == (len - 1)) return 0;

        Char32 c = (*charArray)[index + 1];

        if ('x' == c || 'X' == c) {
            index += 2;
            base = 16;
        }
        else {
            index++;
            base = 8;
        }
    }
    else if ('#' == (*charArray)[index]) {
        index++;
        base = 16;
    }

    return StringUtils::ParseInt32(nm.Substring(index), base) * sign;
}

Int32 XmlUtils::ConvertValueToUnsignedInt(
    /* [in] */ const String& value,
    /* [in] */ Int32 defaultValue)
{
    if (value.IsNull()) return defaultValue;

    return ParseUnsignedIntAttribute(value);
}

Int32 XmlUtils::ParseUnsignedIntAttribute(
    /* [in] */ const String& charSeq)
{
    String value = charSeq;

    // Int64 bits;
    Int32 index = 0;
    Int32 len = value.GetLength();
    Int32 base = 10;

    if ('0' == value.GetChar(index)) {
        //  Quick check for zero by itself
        if (index == (len - 1)) {
            return 0;
        }

        Char32 c = value.GetChar(index + 1);

        if ('x' == c || 'X' == c) {     //  check for hex
            index += 2;
            base = 16;
        }
        else {                        //  check for octal
            index++;
            base = 8;
        }
    }
    else if ('#' == value.GetChar(index)) {
        index++;
        base = 16;
    }

    return (Int32) StringUtils::ParseInt64(value.Substring(index), base);
}

ECode XmlUtils::WriteMapXml(
    /* [in] */ IMap* val,
    /* [in] */ IOutputStream* out) //throws XmlPullParserException, java.io.IOException
{
    AutoPtr<IFastXmlSerializer> serializer;
    CFastXmlSerializer::New((IFastXmlSerializer**)&serializer);
    FAIL_RETURN(serializer->SetOutput(out, String("utf-8")));
    FAIL_RETURN(serializer->StartDocument(String(NULL), TRUE));
    FAIL_RETURN(serializer->SetFeature(String("http://xmlpull.org/v1/doc/features.html#indent-output"), TRUE));
    FAIL_RETURN(WriteMapXml(val, String(NULL), serializer));
    return serializer->EndDocument();
}

ECode XmlUtils::WriteListXml(
    /* [in] */ IList* val,
    /* [in] */ IOutputStream* out) //throws XmlPullParserException, java.io.IOException
{
    AutoPtr<IXmlSerializer> serializer;
    Xml::NewSerializer((IXmlSerializer**)&serializer);
    // CKXmlSerializer::New((IXmlSerializer**)&serializer);
    FAIL_RETURN(serializer->SetOutput(out, String("utf-8")));
    FAIL_RETURN(serializer->StartDocument(String(NULL), TRUE));
    FAIL_RETURN(serializer->SetFeature(String("http://xmlpull.org/v1/doc/features.html#indent-output"), TRUE));
    FAIL_RETURN(WriteListXml(val, String(NULL), serializer));
    return serializer->EndDocument();
}

ECode XmlUtils::WriteMapXml(
    /* [in] */ IMap* val,
    /* [in] */ const String& name,
    /* [in] */ IXmlSerializer* out) //throws XmlPullParserException, java.io.IOException
{
    return WriteMapXml(val, name, out, NULL);
}

ECode XmlUtils::WriteMapXml(
    /* [in] */ IMap* val,
    /* [in] */ const String& name,
    /* [in] */ IXmlSerializer* out,
    /* [in] */ WriteMapCallback* callback) //throws XmlPullParserException, java.io.IOException
{
    if (val == NULL) {
        FAIL_RETURN(out->WirteStartTag(String(NULL), String("NULL"))
        FAIL_RETURN(out->WirteEndTag(String(NULL), String("NULL"))
        return NOERROR;
    }

    FAIL_RETURN(out->WirteStartTag(String(NULL), String("map"))
    if (name != NULL) {
        FAIL_RETURN(out->WirteAttribute(String(NULL), String("name"), name)
    }

    FAIL_RETURN(WriteMapXml(val, out, callback))

    return out->WirteEndTag(String(NULL), String("map");
}

ECode XmlUtils::WriteMapXml(
    /* [in] */ IMap* val,
    /* [in] */ IXmlSerializer* out,
    /* [in] */ WriteMapCallback* callback) //throws XmlPullParserException, java.io.IOException
{
    if (val == NULL) {
        return NOERROR;
    }

    AutoPtr<ISet> s;
    val->GetEntrySet((ISet**)&s);
    AutoPtr<IIterator> i;
    s->GetIterator((IIterator**)&i);

    Boolean hasNext;
    while (i->HasNext(&hasNext), hasNext) {
        AutoPtr<IInterface> object;
        i->GetNext((IInterface**)&object);
        AutoPtr<IMapEntry> e = IMapEntry::Probe(object);
        AutoPtr<IInterface> value, key;
        e->GetValue((IInterface**)&value);
        e->GetKey((IInterface**)&key);
        String k;
        assert(IString::Probe(key) != NULL);
        ICharSequence::Probe(key)->ToString(&k);
        FAIL_RETURN(WriteValueXml(value, k, out, callback))
    }
    return NOERROR;
}

ECode XmlUtils::WriteListXml(
    /* [in] */ IList* val,
    /* [in] */ const String& name,
    /* [in] */ IXmlSerializer* out) //throws XmlPullParserException, java.io.IOException
{
    if (val == NULL) {
        FAIL_RETURN(out->WriteStartTag(String(NULL), String("NULL")));
        return out->WriteEndTag(String(NULL), String("NULL"));
    }

    FAIL_RETURN(out->WriteStartTag(String(NULL), String("list")));
    if (!name.IsNull()) {
        FAIL_RETURN(out->WriteAttribute(String(NULL), String("name"), name));
    }

    Int32 N;
    val->GetSize((Int32*)&N);
    Int32 i = 0;
    while (i < N) {
        AutoPtr<IInterface> value;
        val->Get(i, (IInterface**)&value);
        FAIL_RETURN(WriteValueXml(value, String(NULL), out))
        i++;
    }

    return out->WriteEndTag(String(NULL), String("list"));
}

ECode XmlUtils::WriteSetXml(
    /* [in] */ ISet* val,
    /* [in] */ const String& name,
    /* [in] */ IXmlSerializer* out) //throws XmlPullParserException, java.io.IOException
{
    if (val == NULL) {
        FAIL_RETURN(out->WriteStartTag(String(NULL), String("NULL")));
        return out->WriteEndTag(String(NULL), String("NULL"));
    }

    FAIL_RETURN(out->WriteStartTag(String(NULL), String("set")));
    if (!name.IsNull()) {
        FAIL_RETURN(out->WriteAttribute(String(NULL), String("name"), name));
    }

    AutoPtr<IIterator> iter;
    val->GetIterator((IIterator**)&iter);
    Boolean hasNext = FALSE;
    while (iter->HasNext(&hasNext), hasNext) {
        AutoPtr<IInterface> value;
        iter->GetNext((IInterface**)&value);
        FAIL_RETURN(WriteValueXml(value, String(NULL), out))
    }

    return out->WriteEndTag(String(NULL), String("set"));
}

ECode XmlUtils::WriteByteArrayXml(
    /* [in] */ IArrayOf* val,
    /* [in] */ const String& name,
    /* [in] */ IXmlSerializer* out) //throws XmlPullParserException, java.io.IOException
{
    if (val == NULL) {
        FAIL_RETURN(out->WriteStartTag(String(NULL), String("NULL")));
        return out->WriteEndTag(String(NULL), String("NULL"));
    }

    FAIL_RETURN(out->WriteStartTag(String(NULL), String("byte-array")));
    if (!name.IsNull()) {
        FAIL_RETURN(out->WriteAttribute(String(NULL), String("name"), name));
    }

    Int32 N = 0;
    val->GetLength(&N);
    FAIL_RETURN(out->WriteAttribute(String(NULL), String("num"), StringUtils::Int32ToString(N)));

    AutoPtr<StringBuilder> sb = new StringBuilder(N * 2);
    for (Int32 i = 0; i < N; i++) {
        AutoPtr<IInterface> obj;
        val->Get(i, (IInterface**)&obj);
        assert(IByte::Probe(obj) != NULL);

        Byte bv;
        IByte::Probe(obj)->GetValue(&bv);
        Int32 b = bv;
        Int32 h = b >> 4;
        sb->AppendInt32(h >= 10 ? ('a' + h - 10) : ('0' + h));
        h = b & 0xff;
        sb->AppendInt32(h >= 10 ? ('a' + h - 10) : ('0' + h));
    }

    FAIL_RETURN(out->WriteText(sb->ToString()));

    return out->WriteEndTag(String(NULL), String("byte-array"));
}

ECode XmlUtils::WriteIntArrayXml(
    /* [in] */ IArrayOf* val,
    /* [in] */ const String& name,
    /* [in] */ IXmlSerializer* out)//throws XmlPullParserException, java.io.IOException
{
    if (val == NULL) {
        FAIL_RETURN(out->WriteStartTag(String(NULL), String("NULL")));
        return out->WriteEndTag(String(NULL), String("NULL"));
    }

    FAIL_RETURN(out->WriteStartTag(String(NULL), String("Int32-array")));
    if (!name.IsNull()) {
        FAIL_RETURN(out->WriteAttribute(String(NULL), String("name"), name));
    }

    Int32 N = 0;
    val->GetLength(&N);
    FAIL_RETURN(out->WriteAttribute(String(NULL), String("num"), StringUtils::Int32ToString(N)));

    for (Int32 i = 0; i < N; i++) {
        FAIL_RETURN(out->WriteStartTag(String(NULL), String("item")));

        AutoPtr<IInterface> value;
        val->Get(i, (IInterface**)&value);
        assert(IInteger32::Probe(value) != NULL);
        Int32 v = 0;
        IInteger32::Probe(value)->GetValue(&v);
        FAIL_RETURN(out->WriteAttribute(String(NULL), String("value"), StringUtils::Int32ToString(v)));
        FAIL_RETURN(out->WriteEndTag(String(NULL), String("item")));
    }

    return out->WriteEndTag(String(NULL), String("Int32-array"));
}

ECode XmlUtils::WriteValueXml(
    /* [in] */ IInterface* v,
    /* [in] */ const String& name,
    /* [in] */ IXmlSerializer* out) //throws XmlPullParserException, java.io.IOException
{
    String typeStr, value;
    if (v == NULL) {
        FAIL_RETURN(out->WriteStartTag(String(NULL), String("NULL")));
        if (!name.IsNull()) {
            FAIL_RETURN(out->WriteAttribute(String(NULL), String("name"), name));
        }
        return out->WriteEndTag(String(NULL), String("NULL"));
    }
    else if (IInteger32::Probe(v) != NULL) {
        typeStr = "Int32";
        Int32 tmp = 0;
        IInteger32::Probe(v)->GetValue(&tmp);
        value = StringUtils::Int32ToString(tmp);
    }
    else if (IInteger64::Probe(v) != NULL) {
        typeStr = "Int64";
        Int64 tmp = 0;
        IInteger64::Probe(v)->GetValue(&tmp);
        value = StringUtils::Int64ToString(tmp);
    }
    else if (IFloat::Probe(v) != NULL) {
        typeStr = "Float";
        Float tmp;
        IFloat::Probe(v)->GetValue(&tmp);
        value = StringUtils::FloatToString(tmp);
    }
    else if (IDouble::Probe(v) != NULL) {
        typeStr = "Double";
        Double tmp = 0;
        IDouble::Probe(v)->GetValue(&tmp);
        value = StringUtils::DoubleToString(tmp);
    }
    else if (IBoolean::Probe(v) != NULL) {
        typeStr = "Boolean";
        Boolean tmp = FALSE;
        IBoolean::Probe(v)->GetValue(&tmp);
        value = StringUtils::BooleanToString(tmp);
    }
    //Combine byte[] and int [].
    else if (IArrayOf::Probe(v) != NULL) {
        Int32 size = 0;
        AutoPtr<IArrayOf> av = IArrayOf::Probe(v);
        av->GetLength(&size);
        if (size > 0) {
            AutoPtr<IInterface> value;
            IArrayOf::Probe(v)->Get(0, (IInterface**)&value);
            //byte []
            if (IByte::Probe(value) != NULL) {
                return WriteByteArrayXml(av, name, out);
            }
            //int []
            else {
                return WriteIntArrayXml(av, name, out);
            }
        }

        return NOERROR;
    }
    else if (IMap::Probe(v) != NULL) {
        return WriteMapXml(IMap::Probe(v), name, out);
    }
    // TODO: Combine list and set' process.
    else if (IObjectContainer::Probe(v) != NULL) {
        //Using list's process.
        return WriteListXml(IObjectContainer::Probe(v), name, out);

        //Using Set's process.
        //return WriteSetXml(IObjectContainer::Probe(v), name, out);
    }
    else if (ICharSequence::Probe(v) != NULL) {
        // XXX This is to allow us to at least write something if
        // we encounter styled text...  but it means we will drop all
        // of the styling information. :(
        FAIL_RETURN(out->WriteStartTag(String(NULL), String("string")));
        if (!name.IsNull()) {
            FAIL_RETURN(out->WriteAttribute(String(NULL), String("name"), name));
        }
        String value;
        FAIL_RETURN(out->WriteText((ICharSequence::Probe(v)->ToString(&value), value)));
        return out->WriteEndTag(String(NULL), String("string"));
    }
    else {
        // throw new RuntimeException("writeValueXml: unable to write value " + v);
        return E_RUNTIME_EXCEPTION;
    }

    FAIL_RETURN(out->WriteStartTag(String(NULL), typeStr));
    if (!name.IsNull()) {
        FAIL_RETURN(out->WriteAttribute(String(NULL), String("name"), name));
    }
    out->WriteAttribute(String(NULL), String("value"), value);
    return out->WriteEndTag(String(NULL), typeStr);
}

AutoPtr<IMap> XmlUtils::ReadMapXml(
    /* [in] */ IInputStream* in)//throws XmlPullParserException, java.io.IOException
{
    AutoPtr<IXmlPullParser> parser = Xml::NewPullParser();
    parser->SetInputEx(in, String(NULL));
    AutoPtr<ArrayOf<String> > name = ArrayOf<String>::Alloc(1);
    AutoPtr<IInterface> obj;
    ReadValueXml(parser, name, (IInterface**)&obj);
    return IMap::Probe(obj);
}

AutoPtr<IArrayOf> XmlUtils::ReadListXml(
    /* [in] */ IInputStream* in)//throws XmlPullParserException, java.io.IOException
{
    AutoPtr<IXmlPullParser> parser = Xml::NewPullParser();
    parser->SetInputEx(in, String(NULL));
    AutoPtr<ArrayOf<String> > name = ArrayOf<String>::Alloc(1);
    AutoPtr<IInterface> obj;
    ReadValueXml(parser, name, (IInterface**)&obj);
    return IArrayOf::Probe(obj);
}

AutoPtr<ISet> XmlUtils::ReadSetXml(
    /* [in] */ IInputStream* in)//throws XmlPullParserException, java.io.IOException
{
    AutoPtr<IXmlPullParser> parser = Xml::NewPullParser();
    parser->SetInputEx(in, String(NULL));
    AutoPtr<ArrayOf<String> > name = ArrayOf<String>::Alloc(1);
    AutoPtr<IInterface> obj;
    ReadValueXml(parser, name, (IInterface**)&obj);
    return ISet::Probe(obj);
}

ECode XmlUtils::ReadThisMapXml(
    /* [in] */ IXmlPullParser* parser,
    /* [in] */ const String& endTag,
    /* [in] */ ArrayOf<String>* name,
    /* [out] */ IMap** map)//throws XmlPullParserException, java.io.IOException
{
    VALIDATE_NOT_NULL(map);
    CObjectMap::New(map);

    Int32 eventType = 0;
    parser->GetEventType(&eventType);
    do {
        if (eventType == IXmlPullParser::START_TAG) {
            AutoPtr<IInterface> val;
            FAIL_RETURN(ReadThisValueXml(parser, name, (IInterface**)&val));
            if (!(*name)[0].IsNull()) {
                //System.out.println("Adding to map: " + name + " -> " + val);
                AutoPtr<ICharSequence> key;
                CStringWrapper::New((*name)[0], (ICharSequence**)&key);
                (*map)->Put(key, val);
            }
            else {
                String n;
                parser->GetName(&n);
                Slogger::D(TAG, "Map value without name attribute: %s", n.string());
                return E_XML_PULL_PARSER_EXCEPTION;
            }
        }
        else if (eventType == IXmlPullParser::END_TAG) {
            String n;
            parser->GetName(&n);
            if (n.Equals(endTag)) {
                return NOERROR;
            }
            Slogger::D(TAG, "Expected %s end tag at: %s", endTag.string(), n.string());
            return E_XML_PULL_PARSER_EXCEPTION;
        }
        parser->Next(&eventType);
    } while (eventType != IXmlPullParser::END_DOCUMENT);

    Slogger::D(TAG, "Document ended before %s end tag", endTag.string());
    return E_XML_PULL_PARSER_EXCEPTION;
}

ECode XmlUtils::ReadThisListXml(
    /* [in] */ IXmlPullParser* parser,
    /* [in] */ const String& endTag,
    /* [in] */ ArrayOf<String>* name,
    /* [out] */ IArrayOf** array)//throws XmlPullParserException, java.io.IOException
{
    VALIDATE_NOT_NULL(array);
    *array = NULL;

    Int32 eventType = 0;
    parser->GetEventType(&eventType);
    List<AutoPtr<IInterface> > list;
    do {
        if (eventType == IXmlPullParser::START_TAG) {
            AutoPtr<IInterface> val;
            FAIL_RETURN(ReadThisValueXml(parser, name, (IInterface**)&val));
            list.PushBack(val);
            //System.out.println("Adding to list: " + val);
        }
        else if (eventType == IXmlPullParser::END_TAG) {
            String n;
            FAIL_RETURN(parser->GetName(&n));
            if (n.Equals(endTag)) {
                Int32 size = list.GetSize();
                CArrayOf::New(EIID_IInterface, size, array);
                List<AutoPtr<IInterface> >::Iterator ator = list.Begin();
                for (Int32 i = 0; ator != list.End() && i < size; ++ator, i++) {
                    (*array)->Put(i, *ator);
                }
                return NOERROR;
            }
            Slogger::D(TAG, "Expected %s end tag at: %s", endTag.string(), n.string());
            return E_XML_PULL_PARSER_EXCEPTION;
        }
        FAIL_RETURN(parser->Next(&eventType));
    } while (eventType != IXmlPullParser::END_DOCUMENT);

    Slogger::D(TAG, "Document ended before %s end tag", endTag.string());
    return E_XML_PULL_PARSER_EXCEPTION;
}

ECode XmlUtils::ReadThisSetXml(
    /* [in] */ IXmlPullParser* parser,
    /* [in] */ const String& endTag,
    /* [in] */ ArrayOf<String>* name,
    /* [out] */ ISet** container)//throws XmlPullParserException, java.io.IOException
{
    VALIDATE_NOT_NULL(container);
    CObjectContainer::New(container);

    Int32 eventType = 0;
    FAIL_RETURN(parser->GetEventType(&eventType));
    do {
        if (eventType == IXmlPullParser::START_TAG) {
            AutoPtr<IInterface> val;
            FAIL_RETURN(ReadThisValueXml(parser, name, (IInterface**)&val));
            (*container)->Add(val);
            //System.out.println("Adding to set: " + val);
        }
        else if (eventType == IXmlPullParser::END_TAG) {
            String n;
            FAIL_RETURN(parser->GetName(&n));
            if (n.Equals(endTag)) {
                return NOERROR;
            }
            Slogger::D(TAG, "Expected %s end tag at: %s", endTag.string(), n.string());
            return E_XML_PULL_PARSER_EXCEPTION;
        }
        FAIL_RETURN(parser->Next(&eventType));
    } while (eventType != IXmlPullParser::END_DOCUMENT);

    Slogger::D(TAG, "Document ended before %s end tag", endTag.string());
    return E_XML_PULL_PARSER_EXCEPTION;
}

ECode XmlUtils::ReadThisIntArrayXml(
    /* [in] */ IXmlPullParser* parser,
    /* [in] */ const String& endTag,
    /* [in] */ ArrayOf<String>* name,
    /* [out] */ IArrayOf** array)//throws XmlPullParserException, java.io.IOException
{
    VALIDATE_NOT_NULL(array);
    String value;
    FAIL_RETURN(parser->GetAttributeValueEx(String(NULL), String("num"), &value));
    Int32 num = StringUtils::ParseInt32(value);;

    CArrayOf::New(EIID_IInteger32, num, array);
    Int32 i = 0;

    Int32 eventType = 0;
    FAIL_RETURN(parser->GetEventType(&eventType));
    do {
        if (eventType == IXmlPullParser::START_TAG) {
            String n;
            FAIL_RETURN(parser->GetName(&n));
            if (n.Equals("item")) {
                FAIL_RETURN(parser->GetAttributeValueEx(String(NULL), String("value"), &value));
                AutoPtr<IInteger32> iVal;
                CInteger32::New(StringUtils::ParseInt32(value), (IInteger32**)&iVal);
                (*array)->Put(i, iVal);
            }
            else {
                Slogger::D(TAG, "Expected item tag at: %s", n.string());
                return E_XML_PULL_PARSER_EXCEPTION;
            }
        }
        else if (eventType == IXmlPullParser::END_TAG) {
            String n;
            FAIL_RETURN(parser->GetName(&n));
            if (n.Equals(endTag)) {
                return NOERROR;
            }
            else if (n.Equals("item")) {
                i++;
            }
            else {
                Slogger::D(TAG, "Expected %s end tag at: %s", endTag.string(), n.string());
                return E_XML_PULL_PARSER_EXCEPTION;
            }
        }
        parser->Next(&eventType);
    } while (eventType != IXmlPullParser::END_DOCUMENT);

    Slogger::D(TAG, "Document ended before %s end tag", endTag.string());
    return E_XML_PULL_PARSER_EXCEPTION;
}

ECode XmlUtils::ReadValueXml(
    /* [in] */ IXmlPullParser* parser,
    /* [in] */ ArrayOf<String>* name,
    /* [out] */ IInterface** value) //throws XmlPullParserException, java.io.IOException
{
    VALIDATE_NOT_NULL(value);
    Int32 eventType = 0;
    FAIL_RETURN(parser->GetEventType(&eventType));
    do {
        if (eventType == IXmlPullParser::START_TAG) {
            return ReadThisValueXml(parser, name, value);
        }
        else if (eventType == IXmlPullParser::END_TAG) {
            String n;
            FAIL_RETURN(parser->GetName(&n));
            Slogger::D(TAG, "Unexpected end tag at: %s", n.string());
            return E_XML_PULL_PARSER_EXCEPTION;
        }
        else if (eventType == IXmlPullParser::TEXT) {
            String n;
            FAIL_RETURN(parser->GetName(&n));
            Slogger::D(TAG, "Unexpected text: %s", n.string());
            return E_XML_PULL_PARSER_EXCEPTION;
        }
        parser->Next(&eventType);
    } while (eventType != IXmlPullParser::END_DOCUMENT);

    Slogger::D(TAG, "Unexpected end of document");
    return E_XML_PULL_PARSER_EXCEPTION;
}

ECode XmlUtils::ReadThisValueXml(
    /* [in] */ IXmlPullParser* parser,
    /* [in] */ ArrayOf<String>* name,
    /* [out] */ IInterface** ret)//throws XmlPullParserException, java.io.IOException
{
    VALIDATE_NOT_NULL(ret);
    String valueName;
    FAIL_RETURN(parser->GetAttributeValueEx(String(NULL), String("name"), &valueName));
    String tagName;
    FAIL_RETURN(parser->GetName(&tagName));

    //System.out.println("Reading this value tag: " + tagName + ", name=" + valueName);

    AutoPtr<IInterface> res;

    if (tagName.Equals("NULL")) {
        res = NULL;
    }
    else if (tagName.Equals("string")) {
        String value = String("");
        Int32 eventType;
        while ((parser->Next(&eventType), eventType) != IXmlPullParser::END_DOCUMENT) {
            if (eventType == IXmlPullParser::END_TAG) {
                String n;
                FAIL_RETURN(parser->GetName(&n));
                if (n.Equals("string")) {
                    (*name)[0] = valueName;
                    //System.out.println("Returning value for " + valueName + ": " + value);
                    AutoPtr<ICharSequence> v;
                    CStringWrapper::New(value, (ICharSequence**)&v);
                    *ret = v;
                    INTERFACE_ADDREF(*ret);
                    return NOERROR;
                }

                Slogger::D(TAG, "Unexpected end tag in <string>: %s", n.string());
                return E_XML_PULL_PARSER_EXCEPTION;
            }
            else if (eventType == IXmlPullParser::TEXT) {
                String t;
                FAIL_RETURN(parser->GetText(&t));
                value += t;
            }
            else if (eventType == IXmlPullParser::START_TAG) {
                String n;
                FAIL_RETURN(parser->GetName(&n));
                Slogger::D(TAG, "Unexpected start tag in <string>: %s", n.string());
                return E_XML_PULL_PARSER_EXCEPTION;
            }
        }

        Slogger::D(TAG, "Unexpected end of document in <string>");
        return E_XML_PULL_PARSER_EXCEPTION;
    }
    else if (tagName.Equals("Int32")) {
        String value;
        FAIL_RETURN(parser->GetAttributeValueEx(String(NULL), String("value"), &value));
        AutoPtr<IInteger32> iv;
        CInteger32::New(StringUtils::ParseInt32(value), (IInteger32**)&iv);
        res = iv;
    }
    else if (tagName.Equals("Int64")) {
        String value;
        FAIL_RETURN(parser->GetAttributeValueEx(String(NULL), String("value"), &value));
        AutoPtr<IInteger64> iv;
        CInteger64::New(StringUtils::ParseInt64(value), (IInteger64**)&iv);
        res = iv;
    }
    else if (tagName.Equals("Float")) {
        String value;
        FAIL_RETURN(parser->GetAttributeValueEx(String(NULL), String("value"), &value));
        AutoPtr<IFloat> fv;
        CFloat::New(StringUtils::ParseFloat(value), (IFloat**)&fv);
        res = fv;
    }
    else if (tagName.Equals("Double")) {
        String value;
        FAIL_RETURN(parser->GetAttributeValueEx(String(NULL), String("value"), &value));
        AutoPtr<IDouble> dv;
        CDouble::New(StringUtils::ParseDouble(value), (IDouble**)&dv);
        res = dv;
    }
    else if (tagName.Equals("Boolean")) {
        String value;
        FAIL_RETURN(parser->GetAttributeValueEx(String(NULL), String("value"), &value));
        AutoPtr<IBoolean> bv;
        CBoolean::New(value.EqualsIgnoreCase("true"), (IBoolean**)&bv);
        res = bv;
    }
    else if (tagName.Equals("Int32-array")) {
        Int32 t = 0;
        FAIL_RETURN(parser->Next(&t));
        AutoPtr<IArrayOf> av;
        FAIL_RETURN(ReadThisIntArrayXml(parser, String("Int32-array"), name, (IArrayOf**)&av));
        (*name)[0] = valueName;
        //System.out.println("Returning value for " + valueName + ": " + res);
        *ret = av;
        INTERFACE_ADDREF(*ret);
        return NOERROR;
    }
    else if (tagName.Equals("map")) {
        Int32 t = 0;
        FAIL_RETURN(parser->Next(&t));
        AutoPtr<IMap> mv;
        FAIL_RETURN(ReadThisMapXml(parser, String("map"), name, (IMap**)&mv));
        (*name)[0] = valueName;
        *ret = mv;
        INTERFACE_ADDREF(*ret);
        //System.out.println("Returning value for " + valueName + ": " + res);
        return NOERROR;
    }
    else if (tagName.Equals("list")) {
        Int32 t = 0;
        FAIL_RETURN(parser->Next(&t));
        AutoPtr<IArrayOf> av;
        FAIL_RETURN(ReadThisListXml(parser, String("list"), name, (IArrayOf**)&av));
        (*name)[0] = valueName;
        //System.out.println("Returning value for " + valueName + ": " + res);
        *ret = av;
        INTERFACE_ADDREF(*ret);
        return NOERROR;
    }
    else if (tagName.Equals("set")) {
        Int32 t = 0;
        FAIL_RETURN(parser->Next(&t));
        AutoPtr<ISet> oc;
        FAIL_RETURN(ReadThisSetXml(parser, String("set"), name, (ISet**)&oc));
        (*name)[0] = valueName;
        //System.out.println("Returning value for " + valueName + ": " + res);
        *ret = oc;
        INTERFACE_ADDREF(*ret);
        return NOERROR;
    }
    else {
        Slogger::D(TAG, "Unknown tag: %s", tagName.string());
        return E_XML_PULL_PARSER_EXCEPTION;
    }

    // Skip through to end tag.
    Int32 eventType;
    while ((parser->Next(&eventType), eventType) != IXmlPullParser::END_DOCUMENT) {
        String n;
        FAIL_RETURN(parser->GetName(&n));
        if (eventType == IXmlPullParser::END_TAG) {
            if (n.Equals(tagName)) {
                (*name)[0] = valueName;
                //System.out.println("Returning value for " + valueName + ": " + res);
                *ret = res;
                INTERFACE_ADDREF(*ret);
                return NOERROR;
            }

            Slogger::D(TAG, "Unexpected end tag in <%s>: %s", tagName.string(), n.string());
            return E_XML_PULL_PARSER_EXCEPTION;
        }
        else if (eventType == IXmlPullParser::TEXT) {
            Slogger::D(TAG, "Unexpected text in <%s>: %s", tagName.string(), n.string());
            return E_XML_PULL_PARSER_EXCEPTION;
        }
        else if (eventType == IXmlPullParser::START_TAG) {
            Slogger::D(TAG, "Unexpected start tag in <%s>: %s", tagName.string(), n.string());
            return E_XML_PULL_PARSER_EXCEPTION;
        }
    }

    Slogger::D(TAG, "Unexpected end of document in <%s>", tagName.string());
    return E_XML_PULL_PARSER_EXCEPTION;
}

ECode XmlUtils::BeginDocument(
    /* [in] */ IXmlPullParser* parser,
    /* [in] */ const String& firstElementName)
{
    Int32 type;
    while ((parser->Next(&type), type) != IXmlPullParser::START_TAG
               && type != IXmlPullParser::END_DOCUMENT) {
        ;
    }

    if (type != IXmlPullParser::START_TAG) {
        // throw new XmlPullParserException("No start tag found");
        return E_XML_PULL_PARSER_EXCEPTION;
    }

    String name;
    if (parser->GetName(&name), !name.Equals(firstElementName)) {
        // throw new XmlPullParserException("Unexpected start tag: found " + parser.getName() +
        //         ", expected " + firstElementName);
        return E_XML_PULL_PARSER_EXCEPTION;
    }
    return NOERROR;
}

ECode XmlUtils::NextElement(
    /* [in] */ IXmlPullParser* parser)
{
    Int32 type;
    while ((parser->Next(&type), type) != IXmlPullParser::START_TAG
               && type != IXmlPullParser::END_DOCUMENT) {
        ;
    }
    return NOERROR;
}

Boolean XmlUtils::NextElementWithin(
    /* [in] */ IXmlPullParser* parser,
    /* [in] */ Int32 outerDepth)
{
    assert(0);
    return FALSE;
}

} // namespace Utility
} // namespace Internal
} // namespace Droid
} // namespace Elastos
