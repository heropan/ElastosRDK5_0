
#include "provider/CCalendarContractCalendarCache.h"
#include "net/Uri.h"

using Elastos::Droid::Net::Uri;

namespace Elastos {
namespace Droid {
namespace Provider {

ECode CCalendarContractCalendarCache::constructor()
{
    return NOERROR;
}

ECode CCalendarContractCalendarCache::GetURI(
    /* [out] */ IUri** uri)
{
    VALIDATE_NOT_NULL(uri);

    return Uri::Parse(String("content://") + ICalendarContract::AUTHORITY + String("/properties"), uri);
}

} //Provider
} //Droid
} //Elastos