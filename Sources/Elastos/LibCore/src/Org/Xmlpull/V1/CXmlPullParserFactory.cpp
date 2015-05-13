
#include "CXmlPullParserFactory.h"

namespace Org {
namespace Xmlpull {
namespace V1 {

ECode CXmlPullParserFactory::constructor()
{
    return NOERROR;
}

ECode CXmlPullParserFactory::SetFeature(
        /* [in] */ const String& name,
        /* [in] */ Boolean state)
{
    return XmlPullParserFactory::SetFeature(name, state);
}

ECode CXmlPullParserFactory::GetFeature(
        /* [in] */ const String& name,
        /* [out] */ Boolean* isGot)
{
    return XmlPullParserFactory::GetFeature(name, isGot);
}

ECode CXmlPullParserFactory::SetNamespaceAware(
        /* [in] */ Boolean awareness)
{
    return XmlPullParserFactory::SetNamespaceAware(awareness);
}

ECode CXmlPullParserFactory::IsNamespaceAware(
        /* [out] */ Boolean* isNspAware)
{
    return XmlPullParserFactory::IsNamespaceAware(isNspAware);
}

ECode CXmlPullParserFactory::SetValidating(
        /* [in] */ Boolean validating)
{
    return XmlPullParserFactory::SetValidating(validating);
}

ECode CXmlPullParserFactory::IsValidating(
        /* [out] */ Boolean* isValidating)
{
    return XmlPullParserFactory::IsValidating(isValidating);
}

ECode CXmlPullParserFactory::NewPullParser(
        /* [out] */ IXmlPullParser** pullParser)
{
    return XmlPullParserFactory::NewPullParser(pullParser);
}

ECode CXmlPullParserFactory::NewSerializer(
        /* [out] */ IXmlSerializer** serial)
{
    return XmlPullParserFactory::NewSerializer(serial);
}

} // namespace V1
} // namespace Xmlpull
} // namespace Org
