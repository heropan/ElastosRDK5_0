
#include "elastos/droid/provider/CContactsContractAggregationExceptions.h"
#include "elastos/droid/provider/ContactsContract.h"
#include "elastos/droid/net/Uri.h"

using Elastos::Droid::Net::Uri;

namespace Elastos {
namespace Droid {
namespace Provider {

ECode CContactsContractAggregationExceptions::constructor()
{
    return NOERROR;
}

ECode CContactsContractAggregationExceptions::GetCONTENT_URI(
    /* [out] */ IUri** uri)
{
    VALIDATE_NOT_NULL(uri);

    AutoPtr<IUri> auUri;
    FAIL_RETURN(ContactsContract::GetAUTHORITY_URI((IUri**)&auUri))
    return Uri::WithAppendedPath(auUri, String("aggregation_exceptions"), uri);
}

} //Provider
} //Droid
} //Elastos