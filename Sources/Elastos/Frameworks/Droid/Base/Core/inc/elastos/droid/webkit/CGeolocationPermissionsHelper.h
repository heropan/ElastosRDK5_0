
#ifndef __ELASTOS_DROID_WEBKIT_CGEOLOCATIONPERMISSIONSHELPER_H__
#define __ELASTOS_DROID_WEBKIT_CGEOLOCATIONPERMISSIONSHELPER_H__

#include "_Elastos_Droid_Webkit_CGeolocationPermissionsHelper.h"

namespace Elastos {
namespace Droid {
namespace Webkit {

CarClass(CGeolocationPermissionsHelper)
{
public:
    CARAPI GetInstance(
        /* [out] */ IGeolocationPermissions** instance);
};

} // namespace Webkit
} // namespace Droid
} // namespace Elastos

#endif // __ELASTOS_DROID_WEBKIT_CGEOLOCATIONPERMISSIONSHELPER_H__
