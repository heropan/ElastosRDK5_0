
#ifndef __CCONTACTSCONTRACTSETTINGS_H__
#define __CCONTACTSCONTRACTSETTINGS_H__

#include "_CContactsContractSettings.h"

using Elastos::Droid::Net::IUri;

namespace Elastos {
namespace Droid {
namespace Provider {

CarClass(CContactsContractSettings)
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

#endif //__CCONTACTSCONTRACTSETTINGS_H__