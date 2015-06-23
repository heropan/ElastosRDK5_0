
#include "provider/CalendarContract.h"
#include "net/Uri.h"

using Elastos::Droid::Net::Uri;

namespace Elastos {
namespace Droid {
namespace Provider {

const String CalendarContract::TAG("Calendar");

ECode CalendarContract::GetCONTENTURI(
    /* [out] */ IUri** uri)
{
    VALIDATE_NOT_NULL(uri);

    return Uri::Parse(String("content://") + ICalendarContract::AUTHORITY, uri);
}

} //Provider
} //Droid
} //Elastos