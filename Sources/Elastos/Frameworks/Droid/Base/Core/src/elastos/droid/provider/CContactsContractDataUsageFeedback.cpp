
#include "elastos/droid/provider/CContactsContractDataUsageFeedback.h"
#include "elastos/droid/provider/ContactsContractData.h"
#include "elastos/droid/provider/ContactsContractContacts.h"
#include "elastos/droid/net/Uri.h"

using Elastos::Droid::Net::Uri;

namespace Elastos {
namespace Droid {
namespace Provider {

ECode CContactsContractDataUsageFeedback::constructor()
{
    return NOERROR;
}

ECode CContactsContractDataUsageFeedback::GetFEEDBACKURI(
    /* [out] */ IUri** uri)
{
    VALIDATE_NOT_NULL(uri);

    AutoPtr<IUri> conUri;
    FAIL_RETURN(ContactsContractData::GetCONTENT_URI((IUri**)&conUri))
    return Uri::WithAppendedPath(conUri, String("usagefeedback"), uri);
}

ECode CContactsContractDataUsageFeedback::GetDELETEUSAGEURI(
    /* [out] */ IUri** uri)
{
    VALIDATE_NOT_NULL(uri);

    AutoPtr<IUri> conUri;
    FAIL_RETURN(ContactsContractContacts::GetCONTENT_URI((IUri**)&conUri))
    return Uri::WithAppendedPath(conUri, String("delete_usage"), uri);
}

} //Provider
} //Droid
} //Elastos