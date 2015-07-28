
#ifndef __CCALENDARCONTRACTCANLENDARENTITY_H__
#define __CCALENDARCONTRACTCANLENDARENTITY_H__

#include "_CCalendarContractEventsEntity.h"
#include "content/CursorEntityIterator.h"

using Elastos::Droid::Net::IUri;
using Elastos::Droid::Database::ICursor;
using Elastos::Droid::Content::IEntity;
using Elastos::Droid::Content::CursorEntityIterator;
using Elastos::Droid::Content::IEntityIterator;
using Elastos::Droid::Content::IContentResolver;
using Elastos::Droid::Content::IContentProviderClient;

namespace Elastos {
namespace Droid {
namespace Provider {

CarClass(CCalendarContractEventsEntity)
{
private:
    class EntityIteratorImpl
        : public ElRefBase
        , public CursorEntityIterator
    {
    public:
        EntityIteratorImpl(
            /* [in] */ ICursor* cursor,
            /* [in] */ IContentResolver* resolver);

        EntityIteratorImpl(
            /* [in] */ ICursor* cursor,
            /* [in] */ IContentProviderClient* provider);

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
        AutoPtr<IContentResolver> mResolver;
        AutoPtr<IContentProviderClient> mProvider;

        static AutoPtr<ArrayOf<String> > REMINDERS_PROJECTION;
        static Int32 COLUMN_MINUTES;
        static Int32 COLUMN_METHOD;

        static AutoPtr<ArrayOf<String> > ATTENDEES_PROJECTION;
        static Int32 COLUMN_ATTENDEE_NAME;
        static Int32 COLUMN_ATTENDEE_EMAIL;
        static Int32 COLUMN_ATTENDEE_RELATIONSHIP;
        static Int32 COLUMN_ATTENDEE_TYPE;
        static Int32 COLUMN_ATTENDEE_STATUS;
        static Int32 COLUMN_ATTENDEE_IDENTITY;
        static Int32 COLUMN_ATTENDEE_ID_NAMESPACE;

        static AutoPtr<ArrayOf<String> > EXTENDED_PROJECTION;
        static Int32 COLUMN_ID;
        static Int32 COLUMN_NAME;
        static Int32 COLUMN_VALUE;

        static String WHERE_EVENT_ID;
    };

public:
    /**
     * This utility class cannot be instantiated
     */
    CARAPI constructor();

    /**
     * The content:// style URL for this table
     */
    CARAPI GetCONTENT_URI(
        /* [out] */ IUri** uri);

    /**
     * Creates a new iterator for events
     *
     * @param cursor An event query
     * @param resolver For performing additional queries
     * @return an EntityIterator containing one entity per event in the
     *         cursor
     */
    CARAPI NewEntityIterator(
        /* [in] */ ICursor* cursor,
        /* [in] */ IContentResolver* resolver,
        /* [out] */ IEntityIterator** iterator);

    /**
     * Creates a new iterator for events
     *
     * @param cursor An event query
     * @param provider For performing additional queries
     * @return an EntityIterator containing one entity per event in the
     *         cursor
     */
    CARAPI NewEntityIterator(
        /* [in] */ ICursor* cursor,
        /* [in] */ IContentProviderClient* provider,
        /* [out] */ IEntityIterator** iterator);
};

} //Provider
} //Droid
} //Elastos

#endif //__CCALENDARCONTRACTCANLENDARENTITY_H__