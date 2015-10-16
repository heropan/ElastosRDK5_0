
#include "elastos/droid/provider/CContactsExtensions.h"
#include "elastos/droid/net/Uri.h"

using Elastos::Droid::Net::Uri;

namespace Elastos {
namespace Droid {
namespace Provider {

ECode CContactsExtensions::constructor()
{
    return NOERROR;
}

ECode CContactsExtensions::GetCONTENT_URI(
    /* [out] */ IUri** uri)
{
    VALIDATE_NOT_NULL(uri);

    return Uri::Parse(String("content://contacts/extensions"), uri);
}

} // Provider
} // Droid
} // Elastos