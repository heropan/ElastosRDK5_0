
#ifndef __CBROWSERCONTRACTSEARCHES_H__
#define __CBROWSERCONTRACTSEARCHES_H__

#include "_CBrowserContractSearches.h"

namespace Elastos {
namespace Droid {
namespace Provider {

CarClass(CBrowserContractSearches)
{
public:
    CARAPI constructor();

    /**
     * The content:// style URI for this table
     */
    CARAPI GetCONTENTURI(
        /* [out] */ IUri** uri);
}

}
}
}

#endif //__CBROWSERCONTRACTSEARCHES_H__