
#include "provider/CCalendarContract.h"
#include "net/Uri.h"

using Elastos::Droid::Net::Uri;

namespace Elastos {
namespace Droid {
namespace Provider {

ECode CCalendarContract::constructor()
{
    return NOERROR;
}

ECode CCalendarContract::GetCONTENT_URI(
    /* [out] */ IUri** uri)
{
    VALIDATE_NOT_NULL(uri);

    return CalendarContract::GetCONTENT_URI(uri);
}

} //Provider
} //Droid
} //Elastos