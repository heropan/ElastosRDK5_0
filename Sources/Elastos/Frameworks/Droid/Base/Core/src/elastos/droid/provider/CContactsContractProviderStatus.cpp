
#include "provider/CContactsContractProviderStatus.h"
#include "provider/ContactsContract.h"
#include "net/Uri.h"

using Elastos::Droid::Net::Uri;

namespace Elastos {
namespace Droid {
namespace Provider {

ECode CContactsContractProviderStatus::constructor()
{
    return NOERROR;
}

ECode CContactsContractProviderStatus::GetCONTENT_URI(
    /* [out] */ IUri** uri)
{
    VALIDATE_NOT_NULL(uri);

    AutoPtr<IUri> auUri;
    FAIL_RETURN(ContactsContract::GetAUTHORITY_URI((IUri**)&auUri))
    return Uri::WithAppendedPath(auUri, String("provider_status"), uri);
}

} //Provider
} //Droid
} //Elastos