
#ifndef __CBROWSERCONTRACTHISTORY_H__
#define __CBROWSERCONTRACTHISTORY_H__

#include "_CBrowserContractHistory.h"

namespace Elastos {
namespace Droid {
namespace Provider {

CarClass(CBrowserContractHistory)
{
public:
     /**
     * This utility class cannot be instantiated.
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

#endif //__CBROWSERCONTRACTHISTORY_H__