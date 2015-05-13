
#ifndef __CBROWSERCONTRACTIMAGEMAPPINGS_H__
#define __CBROWSERCONTRACTIMAGEMAPPINGS_H__

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
    CARAPI GetCONTENTURI(
        /* [out] */ IUri** uri);
};

}
}
}

#endif //__CBROWSERCONTRACTIMAGES_H__