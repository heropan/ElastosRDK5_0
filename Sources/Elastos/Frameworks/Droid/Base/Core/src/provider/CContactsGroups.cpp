
#include "provider/CContactsGroups.h"
#include "net/Uri.h"

using Elastos::Droid::Net::Uri;

namespace Elastos {
namespace Droid {
namespace Provider {

ECode CContactsGroups::constructor()
{
    return NOERROR;
}

ECode CContactsGroups::GetCONTENTURI(
    /* [out] */ IUri** uri)
{
    VALIDATE_NOT_NULL(uri);

    return Uri::Parse(String("content://contacts/groups"), uri);
}

ECode CContactsGroups::GetDELETEDCONTENTURI(
    /* [out] */ IUri** uri)
{
    VALIDATE_NOT_NULL(uri);

    return Uri::Parse(String("content://contacts/deleted_groups"), uri);
}

} // Provider
} // Droid
} // Elastos