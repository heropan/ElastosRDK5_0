
#include "provider/CContactsContractRawContactsEntity.h"
#include "provider/ContactsContract.h"
#include "provider/ContactsContractProfile.h"
#include "net/Uri.h"

using Elastos::Droid::Net::Uri;

namespace Elastos {
namespace Droid {
namespace Provider {

ECode CContactsContractRawContactsEntity::constructor()
{
    return NOERROR;
}

ECode CContactsContractRawContactsEntity::GetCONTENTURI(
    /* [out] */ IUri** uri)
{
    VALIDATE_NOT_NULL(uri);

    AutoPtr<IUri> auUri;
    FAIL_RETURN(ContactsContract::GetAUTHORITYURI((IUri**)&auUri))
    return Uri::WithAppendedPath(auUri, String("raw_contact_entities"), uri);
}

/**
 * The content:// style URI for this table, specific to the user's profile.
 */
ECode CContactsContractRawContactsEntity::GetPROFILECONTENTURI(
    /* [out] */ IUri** uri)
{
    VALIDATE_NOT_NULL(uri);

    AutoPtr<IUri> conUri;
    FAIL_RETURN(ContactsContractProfile::GetCONTENTURI((IUri**)&conUri))
    return Uri::WithAppendedPath(conUri, String("raw_contact_entities"), uri);
}

} //Provider
} //Droid
} //Elastos