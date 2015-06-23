
#ifndef __CDEFAULTDATAHANDLER_H__
#define __CDEFAULTDATAHANDLER_H__

#include "_CDefaultDataHandler.h"
#include <ext/frameworkext.h>
#include <elastos/Stack.h>

using Elastos::Utility::Stack;
using Elastos::Droid::Net::IUri;
using Elastos::IO::IInputStream;
using Org::Xml::Sax::IAttributes;
using Org::Xml::Sax::ILocator;

namespace Elastos {
namespace Droid {
namespace Content {

CarClass(CDefaultDataHandler)
{
public:
    CDefaultDataHandler();

    ~CDefaultDataHandler();

    CARAPI SetDocumentLocator(
        /* [in] */ ILocator* locator);

    CARAPI StartDocument();

    CARAPI EndDocument();

    CARAPI StartPrefixMapping(
        /* [in] */ const String& prefix,
        /* [in] */ const String& uri);

    CARAPI EndPrefixMapping(
        /* [in] */ const String& prefix);

    CARAPI StartElement(
        /* [in] */ const String& uri,
        /* [in] */ const String& localName,
        /* [in] */ const String& qName,
        /* [in] */ IAttributes* atts);

    CARAPI EndElement(
        /* [in] */ const String& uri,
        /* [in] */ const String& localName,
        /* [in] */ const String& qName);

    CARAPI Characters(
        /* [out] */ ArrayOf<Char32>* ch,
        /* [in] */ Int32 start,
        /* [in] */ Int32 length);

    CARAPI IgnorableWhitespace(
        /* [out] */ ArrayOf<Char32>* ch,
        /* [in] */ Int32 start,
        /* [in] */ Int32 length);

    CARAPI ProcessingInstruction(
        /* [in] */ const String& target,
        /* [in] */ const String& data);

    CARAPI SkippedEntity(
        /* [in] */ const String& name);

    CARAPI Insert(
        /* [in] */ IContentResolver* contentResolver,
        /* [in] */ IInputStream* inStream);

    CARAPI Insert(
        /* [in] */ IContentResolver* contentResolver,
        /* [in] */ const String& inStr);

private:
    CARAPI ParseRow(
        /* [in] */ IAttributes* atts);

    CARAPI InsertRow(
        /* [out] */ IUri** uri);

private:
    static const String ROW;
    static const String COL;
    static const String URI_STR;
    static const String POSTFIX;
    static const String DEL;
    static const String SELECT;
    static const String ARG;

private:
    AutoPtr<Stack<AutoPtr<IUri> > > mUris;
    AutoPtr<IContentValues> mValues;
    AutoPtr<IContentResolver> mContentResolver;

};

}
}
}

#endif // __CDEFAULTDATAHANDLER_H__
