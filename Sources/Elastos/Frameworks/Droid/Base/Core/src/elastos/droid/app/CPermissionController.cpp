
#include "elastos/droid/ext/frameworkext.h"
#include "elastos/droid/app/CPermissionController.h"

using Elastos::Droid::Content::Pm::IPackageManager;

namespace Elastos {
namespace Droid {
namespace App {

ECode CPermissionController::CheckPermission(
    /* [in] */ const String& permission,
    /* [in] */ Int32 pid,
    /* [in] */ Int32 uid,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);

    Int32 checkResult;
    ECode ec = mActivityManagerService->CheckPermission(permission, pid, uid, &checkResult);

    if (checkResult == IPackageManager::PERMISSION_GRANTED) {
        *result = TRUE;
    }
    else {
        *result = FALSE;
    }

    return ec;
}

ECode CPermissionController::constructor(
    /* [in] */ IIActivityManager* activityManagerService)
{
    VALIDATE_NOT_NULL(activityManagerService);

    mActivityManagerService = activityManagerService;
    return NOERROR;
}

} // namespace App
} // namespace Droid
} // namespace Elastos
