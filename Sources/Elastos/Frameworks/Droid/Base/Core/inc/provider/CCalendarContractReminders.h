
#ifndef __CCALENDARCONTRACTREMENDERS_H__
#define __CCALENDARCONTRACTREMENDERS_H__

#include "_CCalendarContractReminders.h"
#include "CalendarContractReminders.h"

namespace Elastos {
namespace Droid {
namespace Provider {

/**
 * Fields and helpers for accessing reminders for an event. Each row of this
 * table represents a single reminder for an event. Calling
 * {@link #query(ContentResolver, long, String[])} will return a list of reminders for
 * the event with the given eventId. Both apps and sync adapters may write
 * to this table. There are three writable fields and all of them must be
 * included when inserting a new reminder. They are:
 * <ul>
 * <li>{@link #EVENT_ID}</li>
 * <li>{@link #MINUTES}</li>
 * <li>{@link #METHOD}</li>
 * </ul>
 */
CarClass(CCalendarContractReminders)
{
public:
    CARAPI constructor();

    /**
     * The content:// style URL for the top-level calendar authority
     */
    CARAPI GetCONTENT_URI(
        /* [out] */ IUri** uri);

    CARAPI Query(
    /* [in] */ IContentResolver* cr,
    /* [in] */ Int64 eventId,
    /* [in] */ ArrayOf<String>* projection,
    /* [out] */ ICursor** cursor);
};

} //Provider
} //Droid
} //Elastos

#endif //__CCALENDARCONTRACTREMENDERS_H__