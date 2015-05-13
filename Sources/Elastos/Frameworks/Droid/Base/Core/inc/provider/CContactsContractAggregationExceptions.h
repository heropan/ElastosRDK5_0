
#ifndef __CCONTACTSCONTRACTAGGREGATIONEXCEPTIONS_H__
#define __CCONTACTSCONTRACTAGGREGATIONEXCEPTIONS_H__

#include "_CContactsContractAggregationExceptions.h"

namespace Elastos {
namespace Droid {
namespace Provider {

using Elastos::Droid::Net::IUri;

CarClass(CContactsContractAggregationExceptions)
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

} //Provider
} //Droid
} //Elastos

#endif //__CCONTACTSCONTRACTAGGREGATIONEXCEPTIONS_H__