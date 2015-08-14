#ifndef __ELASTOS_DROID_UTILITY_CXML_H__
#define __ELASTOS_DROID_UTILITY_CXML_H__

#include "_CXml.h"

using Elastos::IO::IInputStream;
using Elastos::IO::IReader;
using Org::Xmlpull::V1::IXmlPullParser;
using Org::Xmlpull::V1::IXmlSerializer;
using Org::Xml::Sax::IContentHandler;

namespace Elastos{
namespace Droid{
namespace Utility{

CarClass(CXml)
{
public:
    CXml();

    ~CXml();

    CARAPI constructor();

    /**
     * Parses the given xml string and fires events on the given SAX handler.
     */
    CARAPI Parse(
        /* [in] */ const String& xml,
        /* [in] */ IContentHandler * contentHandler);

    /**
     * Parses xml from the given reader and fires events on the given SAX
     * handler.
     */
    CARAPI Parse(
        /* [in] */ IReader * in,
        /* [in] */ IContentHandler * contentHandler);

    /**
     * Parses xml from the given input stream and fires events on the given SAX
     * handler.
     */
    CARAPI Parse(
        /* [in] */ IInputStream * in,
        // /* [in] */ IEncoding * encoding,
        /* [in] */ IContentHandler * contentHandler);

    /**
     * Returns a new pull parser with namespace support.
     */
    CARAPI NewPullParser(
        /* [out] */ IXmlPullParser **out);

    /**
     * Creates a new xml serializer.
     */
    CARAPI NewSerializer(
        /* [out] */ IXmlSerializer **out);

    /**
     * Finds an encoding by name. Returns UTF-8 if you pass {@code null}.
     */
    // CARAPI FindEncodingByName(
    //     /* [in] */ const String& encodingName,
    //     /* [out] */ Encoding **out);

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
    CARAPI AsAttributeSet(
        /* [in] */ IXmlPullParser * parser,
        /* [out] */ IAttributeSet **attrSet);
};

}
}
}

#endif // __ELASTOS_DROID_UTILITY_CXML_H__
