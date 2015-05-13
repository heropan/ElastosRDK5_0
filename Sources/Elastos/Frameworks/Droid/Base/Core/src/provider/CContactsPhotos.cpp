
#include "provider/CContactsPhotos.h"
#include "net/Uri.h"

using Elastos::Droid::Net::Uri;

namespace Elastos {
namespace Droid {
namespace Provider {

ECode CContactsPhotos::constructor()
{
    return NOERROR;
}

ECode CContactsPhotos::GetCONTENTURI(
    /* [out] */ IUri** uri)
{
    VALIDATE_NOT_NULL(uri);

    return Uri::Parse(String("content://contacts/photos"), uri);
}

} // Provider
} // Droid
} // Elastos