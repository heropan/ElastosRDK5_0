
#ifndef __CCONTACTSGROUPMEMBERSHIP_H__
#define __CCONTACTSGROUPMEMBERSHIP_H__

#include "_CContactsGroupMembership.h"

using Elastos::Droid::Net::IUri;

namespace Elastos {
namespace Droid {
namespace Provider {

CarClass(CContactsGroupMembership)
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
    CARAPI GetCONTENTURI(
        /* [out] */ IUri** uri);

    /**
     * The content:// style URL for this table
     * @deprecated see {@link android.provider.ContactsContract}
     */
    //@Deprecated
    CARAPI GetRAWCONTENTURI(
        /* [out] */ IUri** uri);
};

} // Provider
} // Droid
} // Elastos

#endif //__CCONTACTSGROUPMEMBERSHIP_H__