
#ifndef __CCONTACTSPHOTOS_H__
#define __CCONTACTSPHOTOS_H__

#include "_CContactsPhotos.h"

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
    CARAPI GetCONTENTURI(
        /* [out] */ IUri** uri);
};

} // Provider
} // Droid
} // Elastos

#endif //__CCONTACTSPHOTOS_H__