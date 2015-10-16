
#include "elastos/droid/provider/CContacts.h"
#include "elastos/droid/net/Uri.h"

using Elastos::Droid::Net::Uri;

namespace Elastos {
namespace Droid {
namespace Provider {

const CString CContacts::TAG = "Contacts";

ECode CContacts::constructor()
{
    return NOERROR;
}

ECode CContacts::GetCONTENT_URI(
    /* [out] */ IUri** uri)
{
    VALIDATE_NOT_NULL(uri);

    return Uri::Parse(String("content://") + IContacts::AUTHORITY, uri);
}

} // Provider
} // Droid
} // Elastos