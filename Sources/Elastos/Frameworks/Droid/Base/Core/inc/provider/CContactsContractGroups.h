
#ifndef __CCONTACTSCONTRACTGROUPS_H__
#define __CCONTACTSCONTRACTGROUPS_H__

#include "_CContactsContractGroups.h"
#include "content/CursorEntityIterator.h"

using Elastos::Droid::Net::IUri;
using Elastos::Droid::Database::ICursor;
using Elastos::Droid::Content::IEntity;
using Elastos::Droid::Content::CursorEntityIterator;
using Elastos::Droid::Content::IEntityIterator;

namespace Elastos {
namespace Droid {
namespace Provider {

CarClass(CContactsContractGroups)
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
    };

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
     * The content:// style URI for this table joined with details data from
     * {@link ContactsContract.Data}.
     */
    CARAPI GetCONTENTSUMMARYURI(
        /* [out] */ IUri** uri);

    CARAPI NewEntityIterator(
        /* [in] */ ICursor* cursor,
        /* [out] */ IEntityIterator** iterator);
};

} //Provider
} //Droid
} //Elastos

#endif //__CCONTACTSCONTRACTGROUPS_H__