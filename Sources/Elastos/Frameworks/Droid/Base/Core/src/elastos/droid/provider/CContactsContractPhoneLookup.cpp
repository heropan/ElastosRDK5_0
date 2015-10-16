
#include "elastos/droid/provider/CContactsContractPhoneLookup.h"
#include "elastos/droid/provider/ContactsContract.h"
#include "elastos/droid/net/Uri.h"

using Elastos::Droid::Net::Uri;

namespace Elastos {
namespace Droid {
namespace Provider {

ECode CContactsContractPhoneLookup::constructor()
{
    return NOERROR;
}

ECode CContactsContractPhoneLookup::GetCONTENT_FILTER_URI(
    /* [out] */ IUri** uri)
{
    VALIDATE_NOT_NULL(uri);

    AutoPtr<IUri> auUri;
    FAIL_RETURN(ContactsContract::GetAUTHORITY_URI((IUri**)&auUri))
    return Uri::WithAppendedPath(auUri, String("phone_lookup"), uri);
}

} //Provider
} //Droid
} //Elastos