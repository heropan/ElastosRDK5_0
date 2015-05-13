
#include "provider/CContacts.h"
#include "net/Uri.h"

using Elastos::Droid::Net::Uri;

namespace Elastos {
namespace Droid {
namespace Provider {

const CString CContacts::TAG = "Contacts";

ECode CContacts::constructor()
{
    return NOERROR;
}

ECode CContacts::GetCONTENTURI(
    /* [out] */ IUri** uri)
{
    VALIDATE_NOT_NULL(uri);

    return Uri::Parse(String("content://") + IContacts::AUTHORITY, uri);
}

} // Provider
} // Droid
} // Elastos