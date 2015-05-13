
#include "provider/CContactsContractSettings.h"
#include "provider/ContactsContract.h"
#include "net/Uri.h"

using Elastos::Droid::Net::Uri;

namespace Elastos {
namespace Droid {
namespace Provider {

ECode CContactsContractSettings::constructor()
{
    return NOERROR;
}

ECode CContactsContractSettings::GetCONTENTURI(
    /* [out] */ IUri** uri)
{
    VALIDATE_NOT_NULL(uri);

    AutoPtr<IUri> auUri;
    FAIL_RETURN(ContactsContract::GetAUTHORITYURI((IUri**)&auUri))
    return Uri::WithAppendedPath(auUri, String("settings"), uri);
}

} //Provider
} //Droid
} //Elastos