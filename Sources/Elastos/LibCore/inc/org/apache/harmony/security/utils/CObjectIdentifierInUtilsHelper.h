
#ifndef __COBJECTIDENTIFIERINUTILSHELPER_H__
#define __COBJECTIDENTIFIERINUTILSHELPER_H__

#include "_CObjectIdentifierInUtilsHelper.h"

namespace Org {
namespace Apache {
namespace Harmony {
namespace Security {
namespace Utils {

CarClass(CObjectIdentifierInUtilsHelper)
{
public:
    CARAPI ValidateOid(
        /* [in] */ ArrayOf<Int32> * pOid);

    CARAPI HashIntArray(
        /* [in] */ ArrayOf<Int32> * pArray,
        /* [out] */ Int32 * pHash);

private:
    // TODO: Add your private member variables here.
};

}
}
}
}
}

#endif // __COBJECTIDENTIFIERINUTILSHELPER_H__
