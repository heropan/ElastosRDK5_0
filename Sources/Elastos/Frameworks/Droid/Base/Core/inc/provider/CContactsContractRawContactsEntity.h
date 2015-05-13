
#ifndef __CCONTACTSCONTRACTRAWCONTACTSENTITY_H__
#define __CCONTACTSCONTRACTRAWCONTACTSENTITY_H__

#include "_CContactsContractRawContactsEntity.h"

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
    CARAPI GetCONTENTURI(
        /* [out] */ IUri** uri);

    /**
     * The content:// style URI for this table, specific to the user's profile.
     */
    CARAPI GetPROFILECONTENTURI(
        /* [out] */ IUri** uri);
};

} //Provider
} //Droid
} //Elastos

#endif //__CCONTACTSCONTRACTRAWCONTACTSENTITY_H__