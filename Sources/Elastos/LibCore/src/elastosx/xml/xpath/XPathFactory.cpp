#include "XPathFactory.h"

namespace Elastosx {
namespace Xml {
namespace XPath {

const String XPathFactory::DEFAULT_PROPERTY_NAME = String("javax.xml.xpath.XPathFactory");
const String XPathFactory::DEFAULT_OBJECT_MODEL_URI = String("http://java.sun.com/jaxp/xpath/dom");

CAR_INTERFACE_IMPL(XPathFactory, Object, IXPathFactory)

CAR_OBJECT_IMPL(XPathFactory)

ECode XPathFactory::NewInstance(
    /* [out] */ IXPathFactory** instance)
{
    VALIDATE_NOT_NULL(instance)
    return NewInstance(DEFAULT_OBJECT_MODEL_URI, instance);
}

ECode XPathFactory::NewInstance(
    /* [in] */ const String& uri,
    /* [out] */ IXPathFactory** instance)
{
    VALIDATE_NOT_NULL(instance)
    if (uri.IsNull()) {
        return E_NULL_POINTER_EXCEPTION;
    }
    if (uri.GetLength() == 0) {
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
//    ClassLoader classLoader = Thread.currentThread().getContextClassLoader();
//    if (classLoader == null) {
        //use the current class loader
//        classLoader = XPathFactory.class.getClassLoader();
//    }
//    XPathFactory xpathFactory = new XPathFactoryFinder(classLoader).newFactory(uri);
//    if (xpathFactory == null) {
//        throw new XPathFactoryConfigurationException(
//            "No XPathFactory implementation found for the object model: "
//            + uri
//        );
//    }
//   return xpathFactory;
/*
    AutoPtr<IThread> thread = Thread::GetCurrentThread();
    AutoPtr<IClassLoader> classLoader;
    thread->GetContextClassLoader((IClassLoader**)&classLoader);
    if (classLoader == NULL) {
        //use the current class loader
//        classLoader = XPathFactory.class.getClassLoader();
    }
    AutoPtr<IXPathFactory> XPathFactory;
**/
    return NOERROR;
}

}// namespace XPath
}// namespace Xml
}// namespace Elastosx