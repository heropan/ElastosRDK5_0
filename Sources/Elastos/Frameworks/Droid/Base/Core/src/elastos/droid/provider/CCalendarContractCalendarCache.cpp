
#include "elastos/droid/provider/CCalendarContractCalendarCache.h"
#include "elastos/droid/net/Uri.h"
#include <elastos/core/StringBuilder.h>

using Elastos::Core::StringBuilder;
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
    StringBuilder builder;
    builder += "content://";
    builder += ICalendarContract::AUTHORITY;
    builder += "/properties";
    String str = builder.ToString();
    return Uri::Parse(str, uri);
}

} //Provider
} //Droid
} //Elastos