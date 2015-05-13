
#ifndef __CATTRIBUTESIMPL_H__
#define __CATTRIBUTESIMPL_H__

#include "_CAttributesImpl.h"
#include "AttributesImpl.h"

namespace Org {
namespace Xml {
namespace Sax {
namespace Helpers {

CarClass(CAttributesImpl), public AttributesImpl
{
public:
    CARAPI GetLength(
        /* [out] */ Int32* length);

    CARAPI GetURI(
        /* [in] */ Int32 index,
        /* [out] */ String* URI);

    CARAPI GetLocalName(
        /* [in] */ Int32 index,
        /* [out] */ String* localName);

    CARAPI GetQName(
        /* [in] */ Int32 index,
        /* [out] */ String* qName);

    CARAPI GetType(
        /* [in] */ Int32 index,
        /* [out] */ String* type);

    CARAPI GetValue(
        /* [in] */ Int32 index,
        /* [out] */ String* value);

    CARAPI GetIndex(
        /* [in] */ const String& uri,
        /* [in] */ const String& localName,
        /* [out] */ Int32* index);

    CARAPI GetIndexEx(
        /* [in] */ const String& qName,
        /* [out] */ Int32* index);

    CARAPI GetTypeEx(
        /* [in] */ const String& uri,
        /* [in] */ const String& localName,
        /* [out] */ String* type);

    CARAPI GetTypeEx2(
        /* [in] */ const String& qName,
        /* [out] */ String* type);

    CARAPI GetValueEx(
        /* [in] */ const String& uri,
        /* [in] */ const String& localName,
        /* [out] */ String* value);

    CARAPI GetValueEx2(
        /* [in] */ const String& qName,
        /* [out] */ String* value);

    CARAPI Clear();

    CARAPI SetAttributes(
        /* [in] */ IAttributes* atts);

    CARAPI AddAttribute(
        /* [in] */ const String& uri,
        /* [in] */ const String& localName,
        /* [in] */ const String& qName,
        /* [in] */ const String& type,
        /* [in] */ const String& value);

    CARAPI SetAttribute(
        /* [in] */ Int32 index,
        /* [in] */ const String& uri,
        /* [in] */ const String& localName,
        /* [in] */ const String& qName,
        /* [in] */ const String& type,
        /* [in] */ const String& value);

    CARAPI RemoveAttribute(
        /* [in] */ Int32 index);

    CARAPI SetURI(
        /* [in] */ Int32 index,
        /* [in] */ const String& uri);

    CARAPI SetLocalName(
        /* [in] */ Int32 index,
        /* [in] */ const String& localName);

    CARAPI SetQName(
        /* [in] */ Int32 index,
        /* [in] */ const String& qName);

    CARAPI SetType(
        /* [in] */ Int32 index,
        /* [in] */ const String& type);

    CARAPI SetValue(
        /* [in] */ Int32 index,
        /* [in] */ const String& value);

    CARAPI constructor();

    CARAPI constructor(
        /* [in] */ IAttributes* atts);

private:
    // TODO: Add your private member variables here.
};

} // namespace Helpers
} // namespace Sax
} // namespace Xml
} // namespace Org

#endif // __CATTRIBUTESIMPL_H__
