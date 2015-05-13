
#ifndef __CBROWSERCONTRACTCOMBINED_H__
#define __CBROWSERCONTRACTCOMBINED_H__

#include "_CBrowserContractCombined.h"

namespace Elastos {
namespace Droid {
namespace Provider {

CarClass(CBrowserContractCombined)
{
public:
    /**
     * This utility class cannot be instantiated
     */
    CARAPI constructor();

    /**
     * The content:// style URI for this table
     */
    CARAPI GetCONTENTURI(
        /* [out] */ IUri** uri);
};

}
}
}

#endif //__CBROWSERCONTRACTIMAGES_H__