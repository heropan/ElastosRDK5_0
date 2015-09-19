
#include "provider/ContactsContractProfile.h"
#include "provider/ContactsContract.h"
#include "net/Uri.h"

using Elastos::Droid::Net::Uri;

namespace Elastos {
namespace Droid {
namespace Provider {

ECode ContactsContractProfile::GetCONTENT_URI(
    /* [out] */ IUri** uri)
{
    VALIDATE_NOT_NULL(uri);

    AutoPtr<IUri> auUri;
    FAIL_RETURN(ContactsContract::GetAUTHORITY_URI((IUri**)&auUri))
    return Uri::WithAppendedPath(auUri, String("profile"), uri);
}

ECode ContactsContractProfile::GetCONTENT_VCARD_URI(
    /* [out] */ IUri** uri)
{
    VALIDATE_NOT_NULL(uri);

    AutoPtr<IUri> _uri;
    FAIL_RETURN(GetCONTENT_URI((IUri**)&_uri))
    return Uri::WithAppendedPath(_uri, String("as_vcard"), uri);

}

ECode ContactsContractProfile::GetCONTENTRAWCONTACTSURI(
    /* [out] */ IUri** uri)
{
    VALIDATE_NOT_NULL(uri);

    AutoPtr<IUri> _uri;
    FAIL_RETURN(GetCONTENT_URI((IUri**)&_uri))
    return Uri::WithAppendedPath(_uri, String("raw_contacts"), uri);
}

ECode ContactsContractProfile::GetMINID(
    /* [out] */ Int64* id)
{
    VALIDATE_NOT_NULL(id);

    *id = 0x7FFFFFFFFFFFFFFFL/*Long.MAX_VALUE*/ - (Int64) 0x7FFFFFFF/*Integer.MAX_VALUE*/;
    return NOERROR;
}

} //Provider
} //Droid
} //Elastos