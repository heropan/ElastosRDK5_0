
#ifndef __ELASTOS_DROID_PROVIDER_CBROWSERCONTRACTHISTORY_H__
#define __ELASTOS_DROID_PROVIDER_CBROWSERCONTRACTHISTORY_H__

#include "_Elastos_Droid_Provider_CBrowserContractHistory.h"

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
    CARAPI GetCONTENT_URI(
        /* [out] */ IUri** uri);
};

}
}
}

#endif //__ELASTOS_DROID_PROVIDER_CBROWSERCONTRACTHISTORY_H__
