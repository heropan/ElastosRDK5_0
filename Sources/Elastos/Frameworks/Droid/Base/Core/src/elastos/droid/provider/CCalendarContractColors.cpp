
#include "provider/CCalendarContractColors.h"
#include "net/Uri.h"

using Elastos::Droid::Net::Uri;

namespace Elastos {
namespace Droid {
namespace Provider {

ECode CCalendarContractColors::constructor()
{
    return NOERROR;
}

ECode CCalendarContractColors::GetCONTENT_URI(
    /* [out] */ IUri** uri)
{
    VALIDATE_NOT_NULL(uri);

    return Uri::Parse(String("content://") + ICalendarContract::AUTHORITY + String("/colors"), uri);
}

} //Provider
} //Droid
} //Elastos