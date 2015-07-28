
#ifndef __CBROWSERCONTRACTIMAGES_H__
#define __CBROWSERCONTRACTIMAGES_H__

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

#endif //__CBROWSERCONTRACTIMAGES_H__