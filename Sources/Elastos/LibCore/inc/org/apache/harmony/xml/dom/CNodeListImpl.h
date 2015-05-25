
#ifndef __CNODELISTIMPL_H__
#define __CNODELISTIMPL_H__

#include "_CNodeListImpl.h"

namespace Org {
namespace Apache {
namespace Harmony {
namespace Xml {
namespace Dom {

CarClass(CNodeListImpl)
{
public:
    CARAPI Item(
        /* [in] */ Int32 index,
        /* [out] */ Org::W3c::Dom::INode ** ppNode);

    CARAPI GetLength(
        /* [out] */ Int32 * pValue);

private:
    // TODO: Add your private member variables here.
};

}
}
}
}
}

#endif // __CNODELISTIMPL_H__
