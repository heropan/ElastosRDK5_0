
#ifndef __ELASTOS_DROID_PROVIDER_CCONTACTSCONTRACTRAWCONTACTS_H__
#define __ELASTOS_DROID_PROVIDER_CCONTACTSCONTRACTRAWCONTACTS_H__

#include "_Elastos_Droid_Provider_CContactsContractRawContacts.h"
#include "elastos/droid/content/CursorEntityIterator.h"
#include "elastos/droid/net/Uri.h"

using Elastos::Droid::Content::CursorEntityIterator;
using Elastos::Droid::Content::IContentResolver;
using Elastos::Droid::Content::IEntity;
using Elastos::Droid::Content::IEntityIterator;
using Elastos::Droid::Net::IUri;

namespace Elastos {
namespace Droid {
namespace Provider {

CarClass(CContactsContractRawContacts)
{
private:
    class EntityIteratorImpl
        : public ElRefBase
        , public CursorEntityIterator
    {
    public:
        EntityIteratorImpl(
            /* [in] */ ICursor* cursor);

        ~EntityIteratorImpl();

        CARAPI_(PInterface) Probe(
            /* [in]  */ REIID riid);

        CARAPI_(UInt32) AddRef();

        CARAPI_(UInt32) Release();

        CARAPI GetInterfaceID(
            /* [in] */ IInterface *pObject,
            /* [out] */ InterfaceID *pIID);

        // @Override
        CARAPI GetEntityAndIncrementCursor(
            /* [in] */ ICursor* cursor,
            /* [out] */ IEntity** entity);

    private:
        static AutoPtr<ArrayOf<String> > DATA_KEYS;

    };

public:
    /**
     * This utility class cannot be instantiated
     */
    CARAPI constructor();

    /**
     * The content:// style URI for this table, which requests a directory of
     * raw contact rows matching the selection criteria.
     */
    CARAPI GetCONTENT_URI(
        /* [out] */ IUri** uri);

    /**
     * Build a {@link android.provider.ContactsContract.Contacts#CONTENT_LOOKUP_URI}
     * style {@link Uri} for the parent {@link android.provider.ContactsContract.Contacts}
     * entry of the given {@link RawContacts} entry.
     */
    CARAPI GetContactLookupUri(
        /* [in] */ IContentResolver* resolver,
        /* [in] */ IUri* rawContactUri,
        /* [out] */ IUri** uri);

    /**
     * TODO: javadoc
     * @param cursor
     * @return
     */
    CARAPI NewEntityIterator(
        /* [in] */ ICursor* cursor,
        /* [out] */ IEntityIterator** iterator);
};

} //Provider
} //Droid
} //Elastos

#endif //__ELASTOS_DROID_PROVIDER_CCONTACTSCONTRACTRAWCONTACTS_H__
