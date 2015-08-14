
#ifndef __ELASTOS_DROID_PROVIDER_CBROWSERCONTRACTIMAGES_H__
#define __ELASTOS_DROID_PROVIDER_CBROWSERCONTRACTIMAGES_H__

#include "_CBrowserContractImages.h"

namespace Elastos {
namespace Droid {
namespace Provider {

CarClass(CBrowserContractImages)
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

#endif //__ELASTOS_DROID_PROVIDER_CBROWSERCONTRACTIMAGES_H__
