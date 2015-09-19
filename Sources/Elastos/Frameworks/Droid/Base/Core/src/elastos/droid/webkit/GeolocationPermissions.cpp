
#include "webkit/GeolocationPermissions.h"
#include "webkit/WebViewFactory.h"

namespace Elastos {
namespace Droid {
namespace Webkit {

AutoPtr<IGeolocationPermissions> GeolocationPermissions::GetInstance()
{
    AutoPtr<IGeolocationPermissions> gp;
    WebViewFactory::GetProvider()->GetGeolocationPermissions((IGeolocationPermissions**)&gp);
    return gp;
}

ECode GeolocationPermissions::GetOrigins(
    /* [in] */ IValueCallback* callback)
{
    return NOERROR;
}

ECode GeolocationPermissions::GetAllowed(
    /* [in] */ const String& origin,
    /* [in] */ IValueCallback* callback)
{
    return NOERROR;
}

ECode GeolocationPermissions::Clear(
    /* [in] */ const String& origin)
{
    return NOERROR;
}

ECode GeolocationPermissions::Allow(
    /* [in] */ const String& origin)
{
    return NOERROR;
}

ECode GeolocationPermissions::ClearAll()
{
    return NOERROR;
}

} // namespace Webkit
} // namepsace Droid
} // namespace Elastos