
#ifndef __ELASTOS_DROID_PROVIDER_CCONTACTSPHOTOS_H__
#define __ELASTOS_DROID_PROVIDER_CCONTACTSPHOTOS_H__

#include "_Elastos_Droid_Provider_CContactsPhotos.h"

using Elastos::Droid::Net::IUri;

namespace Elastos {
namespace Droid {
namespace Provider {

CarClass(CContactsPhotos)
{
public:
    CARAPI constructor();

    /**
     * The content:// style URL for this table
     * @deprecated see {@link android.provider.ContactsContract}
     */
    //@Deprecated
    CARAPI GetCONTENT_URI(
        /* [out] */ IUri** uri);
};

} // Provider
} // Droid
} // Elastos

#endif //__ELASTOS_DROID_PROVIDER_CCONTACTSPHOTOS_H__
