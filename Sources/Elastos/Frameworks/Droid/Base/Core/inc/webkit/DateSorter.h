
#ifndef __ELASTOS_DROID_WEBKIT_DATESORTER_H__
#define __ELASTOS_DROID_WEBKIT_DATESORTER_H__

#include "ext/frameworkext.h"

using Elastos::Droid::Content::IContext;
using Elastos::Utility::ICalendar;

namespace Elastos {
namespace Droid {
namespace Webkit {

/**
 * Sorts dates into the following groups:
 *   Today
 *   Yesterday
 *   seven days ago
 *   one month ago
 *   older than a month ago
 */
class DateSorter
{
public:
    /**
     * @param context Application context
     */
    DateSorter(
        /* [in] */ IContext* context);

    /**
     * @param time time since the Epoch in milliseconds, such as that
     * returned by Calendar.getTimeInMillis()
     * @return an index from 0 to (DAY_COUNT - 1) that identifies which
     * date bin this date belongs to
     */
    virtual CARAPI_(Int32) GetIndex(
        /* [in] */ Int64 time);

    /**
     * @param index date bin index as returned by getIndex()
     * @return string label suitable for display to user
     */
    virtual CARAPI_(String) GetLabel(
        /* [in] */ Int32 index);

    /**
     * @param index date bin index as returned by getIndex()
     * @return date boundary at given index
     */
    virtual CARAPI_(Int64) GetBoundary(
        /* [in] */ Int32 index);

protected:
    DateSorter();

    CARAPI_(void) Init(
        /* [in] */ IContext* context);

private:
    /**
     * Calcuate 12:00am by zeroing out hour, minute, second, millisecond
     */
    CARAPI_(void) BeginningOfDay(
        /* [in] */ ICalendar* c);

private:
    static const String LOGTAG;
    static const Int32 NUM_DAYS_AGO = 7;
    AutoPtr< ArrayOf<Int64> > mBins;
    AutoPtr< ArrayOf<String> > mLabels;
};

} // namespace Webkit
} // namespace Droid
} // namespace Elastos

#endif //__ELASTOS_DROID_WEBKIT_DATESORTER_H__
