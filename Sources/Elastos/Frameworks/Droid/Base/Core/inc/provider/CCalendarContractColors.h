
#ifndef __CCALENDARCONTRACTCOLORS_H__
#define __CCALENDARCONTRACTCOLORS_H__

#include "_CCalendarContractColors.h"

using Elastos::Droid::Net::IUri;

namespace Elastos {
namespace Droid {
namespace Provider {

/**
 * Fields for accessing colors available for a given account. Colors are
 * referenced by {@link #COLOR_KEY} which must be unique for a given
 * account name/type. These values can only be updated by the sync
 * adapter. Only {@link #COLOR} may be updated after the initial insert. In
 * addition, a row can only be deleted once all references to that color
 * have been removed from the {@link Calendars} or {@link Events} tables.
 */
CarClass(CCalendarContractColors)
{
public:
    /**
     * This utility class cannot be instantiated
     */
    CARAPI constructor();

    /**
     * The content:// style URL for the top-level calendar authority
     */
    CARAPI GetCONTENT_URI(
        /* [out] */ IUri** uri);
};

} //Provider
} //Droid
} //Elastos

#endif //__CCALENDARCONTRACTCOLORS_H__