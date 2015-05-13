
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

ECode CCalendarContract::GetCONTENTURI(
    /* [out] */ IUri** uri)
{
    VALIDATE_NOT_NULL(uri);

    return CalendarContract::GetCONTENTURI(uri);
}

} //Provider
} //Droid
} //Elastos