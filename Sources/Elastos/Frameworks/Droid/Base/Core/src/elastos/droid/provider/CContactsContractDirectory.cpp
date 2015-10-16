
#include "elastos/droid/provider/CContactsContractDirectory.h"
#include "elastos/droid/provider/ContactsContract.h"
#include "elastos/droid/net/Uri.h"
#include "elastos/droid/content/CContentValues.h"

using Elastos::Droid::Net::Uri;
using Elastos::Droid::Content::IContentValues;
using Elastos::Droid::Content::CContentValues;

namespace Elastos {
namespace Droid {
namespace Provider {

ECode CContactsContractDirectory::constructor()
{
    return NOERROR;
}

ECode CContactsContractDirectory::GetCONTENT_URI(
    /* [out] */ IUri** uri)
{
    VALIDATE_NOT_NULL(uri);

    AutoPtr<IUri> auUri;
    FAIL_RETURN(ContactsContract::GetAUTHORITY_URI((IUri**)&auUri))
    return Uri::WithAppendedPath(auUri, String("directories"), uri);
}

ECode CContactsContractDirectory::NotifyDirectoryChange(
    /* [in] */ IContentResolver* resolver)
{
    // This is done to trigger a query by Contacts Provider back to the directory provider.
    // No data needs to be sent back, because the provider can infer the calling
    // package from binder.
    AutoPtr<IContentValues> contentValues;
    FAIL_RETURN(CContentValues::New((IContentValues**)&contentValues))

    AutoPtr<IUri> auUri;
    FAIL_RETURN(GetCONTENT_URI((IUri**)&auUri))
    Int32 result;
    return resolver->Update(auUri, contentValues, String(NULL), NULL, &result);
}

} //Provider
} //Droid
} //Elastos