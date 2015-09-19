
#include "provider/CContactsContractCommonDataKindsCallable.h"
#include "provider/ContactsContractData.h"
#include "net/Uri.h"

using Elastos::Droid::Net::Uri;

namespace Elastos {
namespace Droid {
namespace Provider {

ECode CContactsContractCommonDataKindsCallable::constructor()
{
    return NOERROR;
}

ECode CContactsContractCommonDataKindsCallable::GetCONTENT_URI(
    /* [out] */ IUri** uri)
{
    VALIDATE_NOT_NULL(uri);

    AutoPtr<IUri> auUri;
    FAIL_RETURN(ContactsContractData::GetCONTENT_URI((IUri**)&auUri))
    return Uri::WithAppendedPath(auUri, String("callables"), uri);
}

ECode CContactsContractCommonDataKindsCallable::GetCONTENT_FILTER_URI(
    /* [out] */ IUri** uri)
{
    VALIDATE_NOT_NULL(uri);

    AutoPtr<IUri> _uri;
    FAIL_RETURN(GetCONTENT_URI((IUri**)&_uri))
    return Uri::WithAppendedPath(_uri, String("filter"), uri);
}

} //Provider
} //Droid
} //Elastos