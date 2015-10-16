
#include "elastos/droid/webkit/CGeolocationPermissionsHelper.h"
#include "elastos/droid/webkit/GeolocationPermissions.h"

namespace Elastos {
namespace Droid {
namespace Webkit {

ECode CGeolocationPermissionsHelper::GetInstance(
    /* [out] */ IGeolocationPermissions** instance)
{
    VALIDATE_NOT_NULL(instance);
    AutoPtr<IGeolocationPermissions> _instance = GeolocationPermissions::GetInstance();
    *instance = _instance.Get();
    REFCOUNT_ADD(*instance);
    return NOERROR;
}

} // namespace Webkit
} // namepsace Droid
} // namespace Elastos