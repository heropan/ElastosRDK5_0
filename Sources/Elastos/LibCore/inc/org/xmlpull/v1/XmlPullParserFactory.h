
#ifndef __XMLPULLPARSERFACTORY_H__
#define __XMLPULLPARSERFACTORY_H__

#include <elastos/core/Object.h>
#include "elastos/HashMap.h"
#include "Org.Xmlpull.V1_server.h"

using Elastos::Utility::HashMap;
using Org::Xmlpull::V1::IXmlPullParserFactory;

namespace Org {
namespace Xmlpull {
namespace V1 {

class XmlPullParserFactory
    : public Object
    , public IXmlPullParserFactory
{
public:
    CAR_INTERFACE_DECL()

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

public:
    static CARAPI NewInstance(
        /* [out] */ IXmlPullParserFactory** xmlPullParserFac);

    static CARAPI NewInstance(
        /* [in] */ const String& classNames,
     // /* [in] */  Class context,
        /* [out] */ IXmlPullParserFactory** xmlPullParserFac);

public:
    static const String PROPERTY_NAME; // = "org.xmlpull.v1.XmlPullParserFactory";


protected:
    String mClassNamesLocation;

    // ArrayList mParserClasses;
    // ArrayList mSerializerClasses;

    // features are kept there
    HashMap<String, Boolean> mFeatures;
private:
    // /** used as default class to server as context class in newInstance() */
    // final static Class referenceContextClass;

    // static {
    //     XmlPullParserFactory f = new XmlPullParserFactory();
    //     referenceContextClass = f.getClass();
    // }

    static const String RESOURCE_NAME; // = "/META-INF/services/" + PROPERTY_NAME;
};

} // namespace V1
} // namespace Xmlpull
} // namespace Org

#endif // __XMLPULLPARSERFACTORY_H__
