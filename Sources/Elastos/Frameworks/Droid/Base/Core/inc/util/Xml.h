#ifndef __ELASTOS_DROID_UTILITY_XML_H__
#define __ELASTOS_DROID_UTILITY_XML_H__

#include "ext/frameworkext.h"
#include "Org.Kxml2.IO.h"

using Org::Xmlpull::V1::IXmlPullParser;

namespace Elastos {
namespace Droid {
namespace Utility {

class Xml
{
public:
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
    static CARAPI_(AutoPtr<IAttributeSet>) AsAttributeSet(
        /* [in] */ IXmlPullParser* parser);

    static CARAPI_(AutoPtr<IXmlPullParser>) NewPullParser();
};

} // namespace Utility
} // namespace Droid
} // namespace Elastos

#endif //__ELASTOS_DROID_UTILITY_XML_H__
