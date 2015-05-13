
#include "provider/CContactsContractAggregationExceptions.h"
#include "provider/ContactsContract.h"
#include "net/Uri.h"

using Elastos::Droid::Net::Uri;

namespace Elastos {
namespace Droid {
namespace Provider {

ECode CContactsContractAggregationExceptions::constructor()
{
    return NOERROR;
}

ECode CContactsContractAggregationExceptions::GetCONTENTURI(
    /* [out] */ IUri** uri)
{
    VALIDATE_NOT_NULL(uri);

    AutoPtr<IUri> auUri;
    FAIL_RETURN(ContactsContract::GetAUTHORITYURI((IUri**)&auUri))
    return Uri::WithAppendedPath(auUri, String("aggregation_exceptions"), uri);
}

} //Provider
} //Droid
} //Elastos