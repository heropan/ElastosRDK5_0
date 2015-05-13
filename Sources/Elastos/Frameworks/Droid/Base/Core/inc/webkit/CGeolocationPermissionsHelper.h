
#ifndef __CGEOLOCATIONPERMISSIONSHELPER_H__
#define __CGEOLOCATIONPERMISSIONSHELPER_H__

#include "_CGeolocationPermissionsHelper.h"

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

#endif // __CGEOLOCATIONPERMISSIONSHELPER_H__
