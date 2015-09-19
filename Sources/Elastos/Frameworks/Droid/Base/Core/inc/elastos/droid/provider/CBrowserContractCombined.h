
#ifndef __ELASTOS_DROID_PROVIDER_CBROWSERCONTRACTCOMBINED_H__
#define __ELASTOS_DROID_PROVIDER_CBROWSERCONTRACTCOMBINED_H__

#include "_Elastos_Droid_Provider_CBrowserContractCombined.h"

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
    CARAPI GetCONTENT_URI(
        /* [out] */ IUri** uri);
};

}
}
}

#endif //__CBROWSERCONTRACTIMAGES_H__
