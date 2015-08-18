
#ifndef __ELASTOS_DROID_PROVIDER_CCONTACTS_H__
#define __ELASTOS_DROID_PROVIDER_CCONTACTS_H__

#include "_Elastos_Droid_Provider_CContacts.h"

using Elastos::Droid::Net::IUri;

namespace Elastos {
namespace Droid {
namespace Provider {

CarClass(CContacts)
{
public:
    /**
     * no public constructor since this is a utility class
     */
    CARAPI constructor();

    /**
     * The content:// style URL for this provider
     * @deprecated see {@link android.provider.ContactsContract}
     */
    //@Deprecated
    CARAPI GetCONTENT_URI(
        /* [out] */ IUri** uri);

private:
    static const CString TAG;
};

} // Provider
} // Droid
} // Elastos

#endif //__ELASTOS_DROID_PROVIDER_CCONTACTS_H__
