
#include "provider/CContactsGroupMembership.h"
#include "net/Uri.h"

using Elastos::Droid::Net::Uri;

namespace Elastos {
namespace Droid {
namespace Provider {

ECode CContactsGroupMembership::constructor()
{
    return NOERROR;
}

ECode CContactsGroupMembership::GetCONTENTURI(
    /* [out] */ IUri** uri)
{
    VALIDATE_NOT_NULL(uri);

    return Uri::Parse(String("content://contacts/groupmembership"), uri);
}

ECode CContactsGroupMembership::GetRAWCONTENTURI(
    /* [out] */ IUri** uri)
{
    VALIDATE_NOT_NULL(uri);

    return Uri::Parse(String("content://contacts/groupmembershipraw"), uri);
}

} // Provider
} // Droid
} // Elastos