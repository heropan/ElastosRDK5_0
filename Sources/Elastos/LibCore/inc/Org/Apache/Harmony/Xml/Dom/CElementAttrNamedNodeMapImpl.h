
#ifndef __CELEMENTATTRNAMEDNODEMAPIMPL_H__
#define __CELEMENTATTRNAMEDNODEMAPIMPL_H__

#include "_CElementAttrNamedNodeMapImpl.h"

namespace Org {
namespace Apache {
namespace Harmony {
namespace Xml {
namespace Dom {

CarClass(CElementAttrNamedNodeMapImpl)
{
public:
    CARAPI GetNamedItem(
        /* [in] */ const String& name,
        /* [out] */ Org::W3c::Dom::INode ** ppNode);

    CARAPI SetNamedItem(
        /* [in] */ Org::W3c::Dom::INode * pArg,
        /* [out] */ Org::W3c::Dom::INode ** ppNode);

    CARAPI RemoveNamedItem(
        /* [in] */ const String& name,
        /* [out] */ Org::W3c::Dom::INode ** ppNode);

    CARAPI Item(
        /* [in] */ Int32 index,
        /* [out] */ Org::W3c::Dom::INode ** ppNode);

    CARAPI GetLength(
        /* [out] */ Int32 * pValue);

    CARAPI GetNamedItemNS(
        /* [in] */ const String& namespaceURI,
        /* [in] */ const String& localName,
        /* [out] */ Org::W3c::Dom::INode ** ppNode);

    CARAPI SetNamedItemNS(
        /* [in] */ Org::W3c::Dom::INode * pArg,
        /* [out] */ Org::W3c::Dom::INode ** ppNode);

    CARAPI RemoveNamedItemNS(
        /* [in] */ const String& namespaceURI,
        /* [in] */ const String& localName,
        /* [out] */ Org::W3c::Dom::INode ** ppNode);

private:
    // TODO: Add your private member variables here.
};

}
}
}
}
}

#endif // __CELEMENTATTRNAMEDNODEMAPIMPL_H__
