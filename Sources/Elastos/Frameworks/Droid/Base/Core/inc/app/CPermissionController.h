
#ifndef __ELASTOS_DROID_APP_CPERMISSIONCONTROLLER_H__
#define __ELASTOS_DROID_APP_CPERMISSIONCONTROLLER_H__

#include "_CPermissionController.h"

namespace Elastos {
namespace Droid {
namespace App {

CarClass(CPermissionController)
{
public:
    CARAPI CheckPermission(
        /* [in] */ const String& permission,
        /* [in] */ Int32 pid,
        /* [in] */ Int32 uid,
        /* [out] */ Boolean* result);

    CARAPI constructor(
        /* [in] */ IIActivityManager* activityManagerService);

private:
    // TODO: Add your private member variables here.
    IIActivityManager* mActivityManagerService;
};

} // namespace App
} // namespace Droid
} // namespace Elastos

#endif // __ELASTOS_DROID_APP_CPERMISSIONCONTROLLER_H__
