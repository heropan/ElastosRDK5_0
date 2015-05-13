
#ifndef __CCALENDARCONTRACT_H__
#define __CCALENDARCONTRACT_H__

#include "_CCalendarContract.h"
#include "CalendarContract.h"

using Elastos::Droid::Net::IUri;

namespace Elastos {
namespace Droid {
namespace Provider {

CarClass(CCalendarContract)
{
public:
    CARAPI constructor();

    /**
     * The content:// style URL for the top-level calendar authority
     */
    CARAPI GetCONTENTURI(
        /* [out] */ IUri** uri);
};

} //Provider
} //Droid
} //Elastos

#endif //__CCALENDARCONTRACT_H__