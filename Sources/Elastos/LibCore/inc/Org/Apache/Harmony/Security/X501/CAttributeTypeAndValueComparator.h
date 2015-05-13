
#ifndef __CATTRIBUTETYPEANDVALUECOMPARATOR_H__
#define __CATTRIBUTETYPEANDVALUECOMPARATOR_H__

#include "_CAttributeTypeAndValueComparator.h"

namespace Org {
namespace Apache {
namespace Harmony {
namespace Security {
namespace X501 {

CarClass(CAttributeTypeAndValueComparator)
{
public:
    CARAPI Compare(
        /* [in] */ IInterface * pLhs,
        /* [in] */ IInterface * pRhs,
        /* [out] */ Int32 * pResult);

private:
    // TODO: Add your private member variables here.
};

}
}
}
}
}

#endif // __CATTRIBUTETYPEANDVALUECOMPARATOR_H__
