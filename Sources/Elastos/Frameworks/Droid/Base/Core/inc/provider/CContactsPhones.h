
#ifndef __CCONTACTSPHONES_H__
#define __CCONTACTSPHONES_H__

#include "_CContactsPhones.h"

using Elastos::Core::ICharSequence;
using Elastos::Droid::Net::IUri;
using Elastos::Droid::Content::IContext;

namespace Elastos {
namespace Droid {
namespace Provider {

CarClass(CContactsPhones)
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
     * The content:// style URL for filtering phone numbers
     * @deprecated see {@link android.provider.ContactsContract}
     */
    //@Deprecated
    CARAPI GetCONTENTFILTERURL(
        /* [out] */ IUri** uri);

    /**
     * @deprecated see {@link android.provider.ContactsContract}
     */
    // @Deprecated
    CARAPI GetDisplayLabel(
        /* [in] */ IContext* context,
        /* [in] */ Int32 type,
        /* [in] */ ICharSequence* sequence,
        /* [in] */ ArrayOf<ICharSequence*>* labelArray,
        /* [out] */ ICharSequence** label);

    /**
     * @deprecated see {@link android.provider.ContactsContract}
     */
    // @Deprecated
    CARAPI GetDisplayLabel(
        /* [in] */ IContext* context,
        /* [in] */ Int32 type,
        /* [in] */ ICharSequence* sequence,
        /* [out] */ ICharSequence** label);
};

} // Provider
} // Droid
} // Elastos

#endif //__CCONTACTSPHONES_H__