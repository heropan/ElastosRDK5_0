
#include "XmlPullParserFactory.h"

namespace Org {
namespace Xmlpull {
namespace V1 {

const String XmlPullParserFactory::PROPERTY_NAME = String("org.xmlpull.v1.XmlPullParserFactory");
const String XmlPullParserFactory::RESOURCE_NAME = String("/META-INF/services/") + PROPERTY_NAME;

ECode XmlPullParserFactory::SetFeature(
    /* [in] */ const String& name,
    /* [in] */ Boolean state)
{
    mFeatures[name] = state;
    return NOERROR;
}

ECode XmlPullParserFactory::GetFeature(
    /* [in] */ const String& name,
    /* [out] */ Boolean* isGot)
{
    VALIDATE_NOT_NULL(isGot)

    HashMap<String, Boolean>::Iterator iter = mFeatures.Find(name);
    *isGot = iter != mFeatures.End() ? iter->mSecond : FALSE;
    return NOERROR;
}

ECode XmlPullParserFactory::SetNamespaceAware(
    /* [in] */ Boolean awareness)
{
    mFeatures[IXmlPullParser::FEATURE_PROCESS_NAMESPACES] = awareness;
    return NOERROR;
}

ECode XmlPullParserFactory::IsNamespaceAware(
    /* [out] */ Boolean* isNspAware)
{
    VALIDATE_NOT_NULL(isNspAware)

    return GetFeature(IXmlPullParser::FEATURE_PROCESS_NAMESPACES, isNspAware);
}

ECode XmlPullParserFactory::SetValidating(
    /* [in] */ Boolean validating)
{
    mFeatures[IXmlPullParser::FEATURE_VALIDATION] = validating;
    return NOERROR;
}

ECode XmlPullParserFactory::IsValidating(
    /* [out] */ Boolean* isValidating)
{
    VALIDATE_NOT_NULL(isValidating)

    return GetFeature(IXmlPullParser::FEATURE_VALIDATION, isValidating);
}

ECode XmlPullParserFactory::NewPullParser(
    /* [out] */ IXmlPullParser** pullParser)
{
    assert(0 && "TODO");
    // if (parserClasses == null) throw new XmlPullParserException
    //         ("Factory initialization was incomplete - has not tried "+classNamesLocation);

    // if (parserClasses.size() == 0) throw new XmlPullParserException
    //         ("No valid parser classes found in "+classNamesLocation);

    // final StringBuilder issues = new StringBuilder();

    // for (int i = 0; i < parserClasses.size(); i++) {
    //     final Class ppClass = (Class) parserClasses.get(i);
    //     try {
    //         final XmlPullParser pp = (XmlPullParser) ppClass.newInstance();

    //         for (Iterator iter = features.keySet().iterator(); iter.hasNext(); ) {
    //             final String key = (String) iter.next();
    //             final Boolean value = (Boolean) features.get(key);
    //             if(value != null && value.booleanValue()) {
    //                 pp.setFeature(key, true);
    //             }
    //         }
    //         return pp;

    //     } catch(Exception ex) {
    //         issues.append (ppClass.getName () + ": "+ ex.toString ()+"; ");
    //     }
    // }

    // throw new XmlPullParserException ("could not create parser: "+issues);
    return NOERROR;
}

ECode XmlPullParserFactory::NewSerializer(
    /* [out] */ IXmlSerializer** serial)
{
    assert(0 && "TODO");
    // if (serializerClasses == null) {
    //     throw new XmlPullParserException
    //         ("Factory initialization incomplete - has not tried "+classNamesLocation);
    // }
    // if(serializerClasses.size() == 0) {
    //     throw new XmlPullParserException
    //         ("No valid serializer classes found in "+classNamesLocation);
    // }

    // final StringBuilder issues = new StringBuilder ();

    // for (int i = 0; i < serializerClasses.size (); i++) {
    //     final Class ppClass = (Class) serializerClasses.get(i);
    //     try {
    //         final XmlSerializer ser = (XmlSerializer) ppClass.newInstance();

    //         return ser;

    //     } catch(Exception ex) {
    //         issues.append (ppClass.getName () + ": "+ ex.toString ()+"; ");
    //     }
    // }

    // throw new XmlPullParserException ("could not create serializer: "+issues);
    return NOERROR;
}

ECode XmlPullParserFactory::NewInstance(
    /* [out] */ IXmlPullParserFactory** xmlPullParserFac)
{
    VALIDATE_NOT_NULL(xmlPullParserFac)

    return NewInstance(String(NULL), xmlPullParserFac);
}

ECode XmlPullParserFactory::NewInstance(
    /* [in] */ const String& classNames,
 // /* [in] */  Class context,
    /* [out] */ IXmlPullParserFactory** xmlPullParserFac)
{
    assert(0 && "TODO");
    /*
    if (context == null) {
        //NOTE: make sure context uses the same class loader as API classes
        //      this is the best we can do without having access to context classloader in J2ME
        //      if API is in the same classloader as implementation then this will work
        context = referenceContextClass;
    }

    String  classNamesLocation = null;

    if (classNames == null || classNames.length() == 0 || "DEFAULT".equals(classNames)) {
        try {
            InputStream is = context.getResourceAsStream (RESOURCE_NAME);

            if (is == null) throw new XmlPullParserException
                    ("resource not found: "+RESOURCE_NAME
                         +" make sure that parser implementing XmlPull API is available");
            final StringBuilder sb = new StringBuilder();

            while (true) {
                final int ch = is.read();
                if (ch < 0) break;
                else if (ch > ' ')
                    sb.append((char) ch);
            }
            is.close ();

            classNames = sb.toString ();
        }
        catch (Exception e) {
            throw new XmlPullParserException (null, null, e);
        }
        classNamesLocation = "resource "+RESOURCE_NAME+" that contained '"+classNames+"'";
    } else {
        classNamesLocation =
            "parameter classNames to newInstance() that contained '"+classNames+"'";
    }
    */
    // classNames = "org.kxml2.io.KXmlParser,org.kxml2.io.KXmlSerializer";

    // XmlPullParserFactory factory = null;
    // final ArrayList parserClasses = new ArrayList();
    // final ArrayList serializerClasses = new ArrayList();
    // int pos = 0;

    // while (pos < classNames.length ()) {
    //     int cut = classNames.indexOf (',', pos);

    //     if (cut == -1) cut = classNames.length ();
    //     final String name = classNames.substring (pos, cut);

    //     Class candidate = null;
    //     Object instance = null;

    //     try {
    //         candidate = Class.forName (name);
    //         // necessary because of J2ME .class issue
    //         instance = candidate.newInstance ();
    //     }
    //     catch (Exception e) {}

    //     if (candidate != null) {
    //         boolean recognized = false;
    //         if (instance instanceof XmlPullParser) {
    //             parserClasses.add(candidate);
    //             recognized = true;
    //         }
    //         if (instance instanceof XmlSerializer) {
    //             serializerClasses.add(candidate);
    //             recognized = true;
    //         }
    //         if (instance instanceof XmlPullParserFactory) {
    //             if (factory == null) {
    //                 factory = (XmlPullParserFactory) instance;
    //             }
    //             recognized = true;
    //         }
    //         if (!recognized) {
    //             throw new XmlPullParserException ("incompatible class: "+name);
    //         }
    //     }
    //     pos = cut + 1;
    // }

    // if (factory == null) {
    //     factory = new XmlPullParserFactory ();
    // }
    // factory.parserClasses = parserClasses;
    // factory.serializerClasses = serializerClasses;
    // factory.classNamesLocation = "org.kxml2.io.kXmlParser,org.kxml2.io.KXmlSerializer";
    // return factory;
    return NOERROR;
}

} // namespace V1
} // namespace Xmlpull
} // namespace Org
