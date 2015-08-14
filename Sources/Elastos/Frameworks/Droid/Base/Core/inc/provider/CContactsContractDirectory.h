
#ifndef __ELASTOS_DROID_PROVIDER_CCONTACTSCONTRACTDIRECTORY_H__
#define __ELASTOS_DROID_PROVIDER_CCONTACTSCONTRACTDIRECTORY_H__

#include "_CContactsContractDirectory.h"

using Elastos::Droid::Net::IUri;
using Elastos::Droid::Content::IContentResolver;

namespace Elastos {
namespace Droid {
namespace Provider {

CarClass(CContactsContractDirectory)
{
public:
    /**
     * Not instantiable.
     */
    CARAPI constructor();

    /**
     * The content:// style URI for this table.  Requests to this URI can be
     * performed on the UI thread because they are always unblocking.
     */
    CARAPI GetCONTENT_URI(
        /* [out] */ IUri** uri);

    /**
     * Notifies the system of a change in the list of directories handled by
     * a particular directory provider. The Contacts provider will turn around
     * and send a query to the directory provider for the full list of directories,
     * which will replace the previous list.
     */
    CARAPI NotifyDirectoryChange(
        /* [in] */ IContentResolver* resolver);
};

} //Provider
} //Droid
} //Elastos

#endif //__ELASTOS_DROID_PROVIDER_CCONTACTSCONTRACTDIRECTORY_H__
