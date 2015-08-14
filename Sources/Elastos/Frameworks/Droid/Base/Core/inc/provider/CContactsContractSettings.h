
#ifndef __ELASTOS_DROID_PROVIDER_CCONTACTSCONTRACTSETTINGS_H__
#define __ELASTOS_DROID_PROVIDER_CCONTACTSCONTRACTSETTINGS_H__

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
    CARAPI GetCONTENT_URI(
        /* [out] */ IUri** uri);
};

} //Provider
} //Droid
} //Elastos

#endif //__ELASTOS_DROID_PROVIDER_CCONTACTSCONTRACTSETTINGS_H__
