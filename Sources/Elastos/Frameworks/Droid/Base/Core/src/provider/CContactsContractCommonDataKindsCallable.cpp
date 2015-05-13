
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

ECode CContactsContractCommonDataKindsCallable::GetCONTENTURI(
    /* [out] */ IUri** uri)
{
    VALIDATE_NOT_NULL(uri);

    AutoPtr<IUri> auUri;
    FAIL_RETURN(ContactsContractData::GetCONTENTURI((IUri**)&auUri))
    return Uri::WithAppendedPath(auUri, String("callables"), uri);
}

ECode CContactsContractCommonDataKindsCallable::GetCONTENTFILTERURI(
    /* [out] */ IUri** uri)
{
    VALIDATE_NOT_NULL(uri);

    AutoPtr<IUri> _uri;
    FAIL_RETURN(GetCONTENTURI((IUri**)&_uri))
    return Uri::WithAppendedPath(_uri, String("filter"), uri);
}

} //Provider
} //Droid
} //Elastos