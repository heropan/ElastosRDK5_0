
#ifndef __CCONTACTSORGANIZATIONS_H__
#define __CCONTACTSORGANIZATIONS_H__

#include "_CContactsOrganizations.h"

using Elastos::Core::ICharSequence;
using Elastos::Droid::Net::IUri;
using Elastos::Droid::Content::IContext;

namespace Elastos {
namespace Droid {
namespace Provider {

CarClass(CContactsOrganizations)
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
     * @deprecated see {@link android.provider.ContactsContract}
     */
    // @Deprecated
    CARAPI GetDisplayLabel(
        /* [in] */ IContext* context,
        /* [in] */ Int32 type,
        /* [in] */ ICharSequence* label,
        /* [out] */ ICharSequence** lb);
};

} // Provider
} // Droid
} // Elastos

#endif //__CCONTACTSORGANIZATIONS_H__