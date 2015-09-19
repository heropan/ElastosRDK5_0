#include "utility/CXml.h"
#include "utility/CXmlPullAttributes.h"
#include "ext/frameworkext.h"

using Org::Xml::Sax::IXMLReader;
using Org::Xml::Sax::IInputSource;
using Org::Xml::Sax::CInputSource;
using Elastos::IO::IStringReader;
using Elastos::IO::CStringReader;


namespace Elastos{
namespace Droid{
namespace Utility{

CXml::CXml()
{
}

CXml::~CXml()
{
}

ECode CXml::constructor()
{
    return NOERROR;
}

/**
 * Parses the given xml string and fires events on the given SAX handler.
 */
ECode CXml::Parse(
    /* [in] */ const String& xml,
    /* [in] */ IContentHandler *contentHandler)
{
    assert(0);

    //try {
    // AutoPtr<IXMLReader> reader;
    // CExpatReader::New(&reader);
    // reader->SetContentHandler(contentHandler);
    // AutoPtr<IStringReader> sr;
    // CStringReader::New(xml, &sr);
    // AutoPtr<IInputSource> inputSource;
    // CInputSource::New(sr, &inputSource);
    // reader->Parse(inputSource);
    return NOERROR;
    //} catch (IOException e) {
    //    throw new AssertionError(e);
    //}
}

/**
 * Parses xml from the given reader and fires events on the given SAX
 * handler.
 */
ECode CXml::Parse(
    /* [in] */ IReader *in,
    /* [in] */ IContentHandler *contentHandler)
{
    assert(0);
    AutoPtr<IXMLReader> reader;
    // CExpatReader::New(&reader);
    // reader->SetContentHandler(contentHandler);
    AutoPtr<IInputSource> inputSource;
    CInputSource::New(in, (IInputSource**)&inputSource);
    reader->Parse(inputSource);
    return NOERROR;
}

/**
 * Parses xml from the given input stream and fires events on the given SAX
 * handler.
 */
ECode CXml::Parse(
    /* [in] */ IInputStream *in,
    // /* [in] */ IEncoding *encoding,
    /* [in] */ IContentHandler *contentHandler)
{
    assert(0);
    AutoPtr<IXMLReader> reader;
    // CExpatReader::New(&reader);
    // reader->SetContentHandler(contentHandler);
    AutoPtr<IInputSource> inputSource;
    // CInputSource::New(in, (IInputSource**)&inputSource);
    // inputSource->SetEncoding(encoding->mExpatName);
    reader->Parse(inputSource);
    return NOERROR;
}

/**
 * Returns a new pull parser with namespace support.
 */
ECode CXml::NewPullParser(
    /* [out] */ IXmlPullParser **out)
{
    VALIDATE_NOT_NULL(out);
    //try {
    assert(0);
    // CXmlPullParser::New(&out);
    // out->SetFeature(IXmlPullParser::FEATURE_PROCESS_DOCDECL, TRUE);
    // out->SetFeature(IXmlPullParser::FEATURE_PROCESS_NAMESPACES, TRUE);
    return NOERROR;
    //} catch (XmlPullParserException e) {
    //    throw new AssertionError();
    //}
}

/**
 * Creates a new xml serializer.
 */
ECode CXml::NewSerializer(
    /* [out] */ IXmlSerializer **out)
{
    VALIDATE_NOT_NULL(out);
        assert(0);
    //try {
    //    XmlSerializerFactory.instance.newSerializer();
    //} catch (XmlPullParserException e) {
    //    throw new AssertionError(e);
    //}
    return E_NOT_IMPLEMENTED;
}

/**
 * Finds an encoding by name. Returns UTF-8 if you pass {@code null}.
 */
// ECode CXml::FindEncodingByName(
//     /* [in] */ const String& encodingName,
//     /* [out] */ Encoding **out)
// {
//     if (encodingName == NULL) {
//         return Encoding::UTF_8;
//     }

//     for (Encoding encoding : Encoding.values()) {
//         if (encoding.expatName.equalsIgnoreCase(encodingName))
//             return encoding;
//     }
//     //throw new UnsupportedEncodingException(encodingName);
//     return E_UNSUPPORTED_ENCODING_EXCEPTION;
// }

/**
 * Return an AttributeSet interface for use with the given XmlPullParser.
 * If the given parser itself implements AttributeSet, that implementation
 * is simply returned.  Otherwise a wrapper class is
 * instantiated on top of the XmlPullParser, as a proxy for retrieving its
 * attributes, and returned to you.
 *
 * @param parser The existing parser for which you would like an
 *               AttributeSet.
 *
 * @return An AttributeSet you can use to retrieve the
 *         attribute values at each of the tags as the parser moves
 *         through its XML document.
 *
 * @see AttributeSet
 */
ECode CXml::AsAttributeSet(
    /* [in] */ IXmlPullParser *parser,
    /* [out] */ IAttributeSet **attrSet)
{
    VALIDATE_NOT_NULL(attrSet);
    *attrSet = (IAttributeSet*)parser->Probe(EIID_IAttributeSet);
    REFCOUNT_ADD(*attrSet);

    if (*attrSet == NULL) {
        // CXmlPullAttributes::New(parser, attrSet);
    }
    return NOERROR;
}

}
}
}
