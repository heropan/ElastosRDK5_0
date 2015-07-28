
#ifndef __CCONTACTSGROUPS_H__
#define __CCONTACTSGROUPS_H__

#include "_CContactsGroups.h"

using Elastos::Droid::Net::IUri;

namespace Elastos {
namespace Droid {
namespace Provider {

CarClass(CContactsGroups)
{
public:
    /**
     * no public constructor since this is a utility class
     */
    CARAPI constructor();

    /**
     * The content:// style URL for this table
     * @deprecated see {@link android.provider.ContactsContract}
     */
    //@Deprecated
    CARAPI GetCONTENT_URI(
        /* [out] */ IUri** uri);

    /**
     * The content:// style URL for the table that holds the deleted
     * groups.
     * @deprecated see {@link android.provider.ContactsContract}
     */
    //@Deprecated
    CARAPI GetDELETED_CONTENT_URI(
        /* [out] */ IUri** uri);
};

} // Provider
} // Droid
} // Elastos

#endif //__CCONTACTSPEOPLE_H__