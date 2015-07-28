
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

ECode CContactsGroups::GetCONTENT_URI(
    /* [out] */ IUri** uri)
{
    VALIDATE_NOT_NULL(uri);

    return Uri::Parse(String("content://contacts/groups"), uri);
}

ECode CContactsGroups::GetDELETED_CONTENT_URI(
    /* [out] */ IUri** uri)
{
    VALIDATE_NOT_NULL(uri);

    return Uri::Parse(String("content://contacts/deleted_groups"), uri);
}

} // Provider
} // Droid
} // Elastos