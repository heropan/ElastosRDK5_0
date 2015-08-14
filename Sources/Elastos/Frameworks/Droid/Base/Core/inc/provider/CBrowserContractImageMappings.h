
#ifndef __ELASTOS_DROID_PROVIDER_CBROWSERCONTRACTIMAGEMAPPINGS_H__
#define __ELASTOS_DROID_PROVIDER_CBROWSERCONTRACTIMAGEMAPPINGS_H__

#include "_CBrowserContractImageMappings.h"

namespace Elastos {
namespace Droid {
namespace Provider {

CarClass(CBrowserContractImageMappings)
{
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
