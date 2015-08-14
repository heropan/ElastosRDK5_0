
#ifndef __ELASTOS_DROID_PROVIDER_CBROWSERCONTRACTSEARCHES_H__
#define __ELASTOS_DROID_PROVIDER_CBROWSERCONTRACTSEARCHES_H__

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
    CARAPI GetCONTENT_URI(
        /* [out] */ IUri** uri);
}

}
}
}

#endif //__ELASTOS_DROID_PROVIDER_CBROWSERCONTRACTSEARCHES_H__
