
#ifndef __CALENDARCONTRACT_H__
#define __CALENDARCONTRACT_H__

#include "ext/frameworkext.h"

using Elastos::Droid::Net::IUri;

namespace Elastos {
namespace Droid {
namespace Provider {

class CalendarContract
{
public:
    /**
     * The content:// style URL for the top-level calendar authority
     */
    static CARAPI GetCONTENTURI(
        /* [out] */ IUri** uri);

private:
    static const String TAG;
};

} //Provider
} //Droid
} //Elastos

#endif //__CALENDARCONTRACT_H__