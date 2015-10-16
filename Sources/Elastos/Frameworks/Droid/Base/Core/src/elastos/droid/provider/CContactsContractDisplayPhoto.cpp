
#include "elastos/droid/provider/CContactsContractDisplayPhoto.h"
#include "elastos/droid/provider/ContactsContract.h"
#include "elastos/droid/net/Uri.h"

using Elastos::Droid::Net::Uri;

namespace Elastos {
namespace Droid {
namespace Provider {

ECode CContactsContractDisplayPhoto::constructor()
{
    return NOERROR;
}

ECode CContactsContractDisplayPhoto::GetCONTENT_URI(
    /* [out] */ IUri** uri)
{
    VALIDATE_NOT_NULL(uri);

    AutoPtr<IUri> auUri;
    FAIL_RETURN(ContactsContract::GetAUTHORITY_URI((IUri**)&auUri))
    return Uri::WithAppendedPath(auUri, String("display_photo"), uri);
}

ECode CContactsContractDisplayPhoto::GetCONTENTMAXDIMENSIONSURI(
    /* [out] */ IUri** uri)
{
    VALIDATE_NOT_NULL(uri);

    AutoPtr<IUri> auUri;
    FAIL_RETURN(ContactsContract::GetAUTHORITY_URI((IUri**)&auUri))
    return Uri::WithAppendedPath(auUri, String("photo_dimensions"), uri);
}

} //Provider
} //Droid
} //Elastos