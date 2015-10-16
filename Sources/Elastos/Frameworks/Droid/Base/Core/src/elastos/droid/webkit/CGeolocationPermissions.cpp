
#include "elastos/droid/webkit/CGeolocationPermissions.h"

namespace Elastos {
namespace Droid {
namespace Webkit {

ECode CGeolocationPermissions::constructor()
{
    return NOERROR;
}

ECode CGeolocationPermissions::GetOrigins(
    /* [in] */ IValueCallback* callBack)
{
    return GeolocationPermissions::GetOrigins(callBack);
}

ECode CGeolocationPermissions::GetAllowed(
    /* [in] */ const String& origin,
    /* [in] */ IValueCallback* callBack)
{
    return GeolocationPermissions::GetAllowed(origin, callBack);
}

ECode CGeolocationPermissions::Clear(
    /* [in] */ const String& origin)
{
    return GeolocationPermissions::Clear(origin);
}

ECode CGeolocationPermissions::Allow(
    /* [in] */ const String& origin)
{
    return GeolocationPermissions::Allow(origin);
}

ECode CGeolocationPermissions::ClearAll()
{
    return GeolocationPermissions::ClearAll();
}

} // namespace Webkit
} // namepsace Droid
} // namespace Elastos