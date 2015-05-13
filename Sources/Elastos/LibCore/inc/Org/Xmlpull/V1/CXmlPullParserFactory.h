
#ifndef __CXMLPULLPARSERFACTORY_H__
#define __CXMLPULLPARSERFACTORY_H__

#include "_CXmlPullParserFactory.h"
#include "XmlPullParserFactory.h"

namespace Org {
namespace Xmlpull {
namespace V1 {

CarClass(CXmlPullParserFactory), public XmlPullParserFactory
{
public:
    CARAPI constructor();

    CARAPI SetFeature(
        /* [in] */ const String& name,
        /* [in] */ Boolean state);

    CARAPI GetFeature(
        /* [in] */ const String& name,
        /* [out] */ Boolean* isGot);

    CARAPI SetNamespaceAware(
        /* [in] */ Boolean awareness);

    CARAPI IsNamespaceAware(
        /* [out] */ Boolean* isNspAware);

    CARAPI SetValidating(
        /* [in] */ Boolean validating);

    CARAPI IsValidating(
        /* [out] */ Boolean* isValidating);

    CARAPI NewPullParser(
        /* [out] */ IXmlPullParser** pullParser);

    CARAPI NewSerializer(
        /* [out] */ IXmlSerializer** serial);
};

} // namespace V1
} // namespace Xmlpull
} // namespace Org

#endif // __CXMLPULLPARSERFACTORY_H__
