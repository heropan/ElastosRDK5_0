
#include "provider/CContactsContractDisplayPhoto.h"
#include "provider/ContactsContract.h"
#include "net/Uri.h"

using Elastos::Droid::Net::Uri;

namespace Elastos {
namespace Droid {
namespace Provider {

ECode CContactsContractDisplayPhoto::constructor()
{
    return NOERROR;
}

ECode CContactsContractDisplayPhoto::GetCONTENTURI(
    /* [out] */ IUri** uri)
{
    VALIDATE_NOT_NULL(uri);

    AutoPtr<IUri> auUri;
    FAIL_RETURN(ContactsContract::GetAUTHORITYURI((IUri**)&auUri))
    return Uri::WithAppendedPath(auUri, String("display_photo"), uri);
}

ECode CContactsContractDisplayPhoto::GetCONTENTMAXDIMENSIONSURI(
    /* [out] */ IUri** uri)
{
    VALIDATE_NOT_NULL(uri);

    AutoPtr<IUri> auUri;
    FAIL_RETURN(ContactsContract::GetAUTHORITYURI((IUri**)&auUri))
    return Uri::WithAppendedPath(auUri, String("photo_dimensions"), uri);
}

} //Provider
} //Droid
} //Elastos