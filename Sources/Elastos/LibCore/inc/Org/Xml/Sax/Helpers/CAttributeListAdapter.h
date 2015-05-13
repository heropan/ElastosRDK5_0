
#ifndef __CATTRIBUTELISTADAPTER_H__
#define __CATTRIBUTELISTADAPTER_H__

#include "_CAttributeListAdapter.h"

namespace Org {
namespace Xml {
namespace Sax {
namespace Helpers {

CarClass(CAttributeListAdapter)
{
public:
    CARAPI GetLength(
        /* [out] */ Int32 * pLength);

    CARAPI GetURI(
        /* [in] */ Int32 index,
        /* [out] */ String * pURI);

    CARAPI GetLocalName(
        /* [in] */ Int32 index,
        /* [out] */ String * pLocalName);

    CARAPI GetQName(
        /* [in] */ Int32 index,
        /* [out] */ String * pQName);

    CARAPI GetType(
        /* [in] */ Int32 index,
        /* [out] */ String * pType);

    CARAPI GetValue(
        /* [in] */ Int32 index,
        /* [out] */ String * pValue);

    CARAPI GetIndex(
        /* [in] */ const String& uri,
        /* [in] */ const String& localName,
        /* [out] */ Int32 * pIndex);

    CARAPI GetIndexEx(
        /* [in] */ const String& qName,
        /* [out] */ Int32 * pIndex);

    CARAPI GetTypeEx(
        /* [in] */ const String& uri,
        /* [in] */ const String& localName,
        /* [out] */ String * pType);

    CARAPI GetTypeEx2(
        /* [in] */ const String& qName,
        /* [out] */ String * pType);

    CARAPI GetValueEx(
        /* [in] */ const String& uri,
        /* [in] */ const String& localName,
        /* [out] */ String * pValue);

    CARAPI GetValueEx2(
        /* [in] */ const String& qName,
        /* [out] */ String * pValue);

    CARAPI constructor();

    CARAPI constructor(
        /* [in] */ IAttributeList * pParent);

private:
    // TODO: Add your private member variables here.
};

} // namespace Helpers
} // namespace Sax
} // namespace Xml
} // namespace Org

#endif // __CATTRIBUTELISTADAPTER_H__
