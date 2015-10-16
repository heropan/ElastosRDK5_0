
#include "elastos/droid/provider/CContactsContractRawContactsEntity.h"
#include "elastos/droid/provider/ContactsContract.h"
#include "elastos/droid/provider/ContactsContractProfile.h"
#include "elastos/droid/net/Uri.h"

using Elastos::Droid::Net::Uri;

namespace Elastos {
namespace Droid {
namespace Provider {

ECode CContactsContractRawContactsEntity::constructor()
{
    return NOERROR;
}

ECode CContactsContractRawContactsEntity::GetCONTENT_URI(
    /* [out] */ IUri** uri)
{
    VALIDATE_NOT_NULL(uri);

    AutoPtr<IUri> auUri;
    FAIL_RETURN(ContactsContract::GetAUTHORITY_URI((IUri**)&auUri))
    return Uri::WithAppendedPath(auUri, String("raw_contact_entities"), uri);
}

/**
 * The content:// style URI for this table, specific to the user's profile.
 */
ECode CContactsContractRawContactsEntity::GetPROFILE_CONTENT_URI(
    /* [out] */ IUri** uri)
{
    VALIDATE_NOT_NULL(uri);

    AutoPtr<IUri> conUri;
    FAIL_RETURN(ContactsContractProfile::GetCONTENT_URI((IUri**)&conUri))
    return Uri::WithAppendedPath(conUri, String("raw_contact_entities"), uri);
}

} //Provider
} //Droid
} //Elastos