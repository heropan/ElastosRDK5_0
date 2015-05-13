
#include "util/Xml.h"

#ifdef DROID_CORE
#include "util/CXmlPullAttributes.h"
#else
#include "Elastos.Droid.Core.h"
#endif

using Org::Kxml2::IO::IKXmlParser;
using Org::Kxml2::IO::CKXmlParser;

namespace Elastos {
namespace Droid {
namespace Utility {

AutoPtr<IXmlPullParser> Xml::NewPullParser()
{
    //try {
    AutoPtr<IKXmlParser> parser;
    CKXmlParser::New((IKXmlParser**)&parser);
    parser->SetFeature(IXmlPullParser::FEATURE_PROCESS_DOCDECL, TRUE);
    parser->SetFeature(IXmlPullParser::FEATURE_PROCESS_NAMESPACES, TRUE);
    return (IXmlPullParser*)parser.Get();
    //} catch (XmlPullParserException e) {
    //    throw new AssertionError();
    //}
}

AutoPtr<IAttributeSet> Xml::AsAttributeSet(
    /* [in] */ IXmlPullParser* parser)
{
    assert(parser != NULL);
    AutoPtr<IAttributeSet> attrSet = IAttributeSet::Probe(parser);
    if (attrSet == NULL) {
        CXmlPullAttributes::New(parser, (IXmlPullAttributes**)&attrSet);
    }
    return attrSet;
}

} // namespace Utility
} // namespace Droid
} // namespace Elastos
