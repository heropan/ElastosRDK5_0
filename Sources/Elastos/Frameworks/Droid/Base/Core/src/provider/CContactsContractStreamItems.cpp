
#include "provider/CContactsContractStreamItems.h"
#include "provider/ContactsContract.h"
#include "net/Uri.h"

using Elastos::Droid::Net::Uri;

namespace Elastos {
namespace Droid {
namespace Provider {

ECode CContactsContractStreamItems::constructor()
{
    return NOERROR;
}

ECode CContactsContractStreamItems::GetCONTENTURI(
    /* [out] */ IUri** uri)
{
    VALIDATE_NOT_NULL(uri);

    AutoPtr<IUri> auUri;
    FAIL_RETURN(ContactsContract::GetAUTHORITYURI((IUri**)&auUri))
    return Uri::WithAppendedPath(auUri, String("stream_items"), uri);
}

ECode CContactsContractStreamItems::GetCONTENTPHOTOURI(
    /* [out] */ IUri** uri)
{
    VALIDATE_NOT_NULL(uri);

    AutoPtr<IUri> _uri;
    FAIL_RETURN(GetCONTENTURI((IUri**)&_uri))
    return Uri::WithAppendedPath(_uri, String("photo"), uri);
}

ECode CContactsContractStreamItems::GetCONTENTLIMITURI(
    /* [out] */ IUri** uri)
{
    VALIDATE_NOT_NULL(uri);

    AutoPtr<IUri> auUri;
    FAIL_RETURN(ContactsContract::GetAUTHORITYURI((IUri**)&auUri))
    return Uri::WithAppendedPath(auUri, String("stream_items_limit"), uri);
}

} //Provider
} //Droid
} //Elastos