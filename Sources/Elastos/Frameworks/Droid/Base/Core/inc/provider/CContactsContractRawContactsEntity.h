
#ifndef __ELASTOS_DROID_PROVIDER_CCONTACTSCONTRACTRAWCONTACTSENTITY_H__
#define __ELASTOS_DROID_PROVIDER_CCONTACTSCONTRACTRAWCONTACTSENTITY_H__

#include "_Elastos_Droid_Provider_CContactsContractRawContactsEntity.h"

using Elastos::Droid::Net::IUri;

namespace Elastos {
namespace Droid {
namespace Provider {

CarClass(CContactsContractRawContactsEntity)
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

    /**
     * The content:// style URI for this table, specific to the user's profile.
     */
    CARAPI GetPROFILE_CONTENT_URI(
        /* [out] */ IUri** uri);
};

} //Provider
} //Droid
} //Elastos

#endif //__ELASTOS_DROID_PROVIDER_CCONTACTSCONTRACTRAWCONTACTSENTITY_H__
