
#include "provider/CalendarContractExtendedProperties.h"
#include "net/Uri.h"

using Elastos::Droid::Net::Uri;

namespace Elastos {
namespace Droid {
namespace Provider {

ECode CalendarContractExtendedProperties::GetCONTENTURI(
    /* [out] */ IUri** uri)
{
    VALIDATE_NOT_NULL(uri);

    return Uri::Parse(String("content://") + ICalendarContract::AUTHORITY + String("/extendedproperties"), uri);
}

} //Provider
} //Droid
} //Elastos