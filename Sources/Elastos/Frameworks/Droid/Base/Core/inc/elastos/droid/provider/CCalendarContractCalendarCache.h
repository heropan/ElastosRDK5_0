
#ifndef __ELASTOS_DROID_PROVIDER_CCALENDARCONTRACTCALENDARCACHE_H__
#define __ELASTOS_DROID_PROVIDER_CCALENDARCONTRACTCALENDARCACHE_H__

#include "_Elastos_Droid_Provider_CCalendarContractCalendarCache.h"

using Elastos::Droid::Net::IUri;

namespace Elastos {
namespace Droid {
namespace Provider {

/**
 * CalendarCache stores some settings for calendar including the current
 * time zone for the instances. These settings are stored using a key/value
 * scheme. A {@link #KEY} must be specified when updating these values.
 */
CarClass(CCalendarContractCalendarCache)
{
public:
    CARAPI constructor();

    /**
     * The URI to use for retrieving the properties from the Calendar db.
     */
    CARAPI GetURI(
        /* [out] */ IUri** uri);
};

} //Provider
} //Droid
} //Elastos

#endif //__ELASTOS_DROID_PROVIDER_CCALENDARCONTRACTCALENDARCACHE_H__
