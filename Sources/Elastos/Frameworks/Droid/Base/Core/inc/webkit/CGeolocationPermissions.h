
#ifndef __CGEOLOCATIONPERMISSIONS_H__
#define __CGEOLOCATIONPERMISSIONS_H__

#include "_CGeolocationPermissions.h"
#include "webkit/GeolocationPermissions.h"

namespace Elastos {
namespace Droid {
namespace Webkit {

CarClass(CGeolocationPermissions), public GeolocationPermissions
{
public:
    CARAPI constructor();

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

#endif // __CGEOLOCATIONPERMISSIONS_H__
