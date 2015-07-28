
#ifndef __CCALENDARCONTRACTCANLENDARENTITY_H__
#define __CCALENDARCONTRACTCANLENDARENTITY_H__

#include "_CCalendarContractCalendarEntity.h"
#include "content/CursorEntityIterator.h"

using Elastos::Droid::Net::IUri;
using Elastos::Droid::Database::ICursor;
using Elastos::Droid::Content::IEntity;
using Elastos::Droid::Content::CursorEntityIterator;
using Elastos::Droid::Content::IEntityIterator;

namespace Elastos {
namespace Droid {
namespace Provider {

/**
 * Class that represents a Calendar Entity. There is one entry per calendar.
 * This is a helper class to make batch operations easier.
 */
CarClass(CCalendarContractCalendarEntity)
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
     * The default Uri used when creating a new calendar EntityIterator.
     */
    //@SuppressWarnings("hiding")
    CARAPI GetCONTENT_URI(
        /* [out] */ IUri** uri);

    /**
     * Creates an entity iterator for the given cursor. It assumes the
     * cursor contains a calendars query.
     *
     * @param cursor query on {@link #CONTENT_URI}
     * @return an EntityIterator of calendars
     */
    CARAPI NewEntityIterator(
        /* [in] */ ICursor* cursor,
        /* [out] */ IEntityIterator** iter);
};

} //Provider
} //Droid
} //Elastos

#endif //__CCALENDARCONTRACTCANLENDARENTITY_H__