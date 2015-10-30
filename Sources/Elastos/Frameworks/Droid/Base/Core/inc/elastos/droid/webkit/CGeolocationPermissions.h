
#ifndef __ELASTOS_DROID_WEBKIT_CGEOLOCATIONPERMISSIONS_H__
#define __ELASTOS_DROID_WEBKIT_CGEOLOCATIONPERMISSIONS_H__

#include "_Elastos_Droid_Webkit_CGeolocationPermissions.h"
#include "elastos/droid/webkit/GeolocationPermissions.h"

namespace Elastos {
namespace Droid {
namespace Webkit {

CarClass(CGeolocationPermissions), public GeolocationPermissions
{
public:
    CAR_OBJECT_DECL();

    CARAPI GetOrigins(
        /* [in] */ IValueCallback* callBack);

    CARAPI GetAllowed(
        /* [in] */ const String& origin,
        /* [in] */ IValueCallback* callBack);

    CARAPI Clear(
        /* [in] */ const String& origin);

    CARAPI Allow(
        /* [in] */ const String& origin);

    CARAPI ClearAll();
};

} // namespace Webkit
} // namespace Droid
} // namespace Elastos

#endif // __ELASTOS_DROID_WEBKIT_CGEOLOCATIONPERMISSIONS_H__
