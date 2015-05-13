
#include "ext/frameworkdef.h"
#include "os/CPowerManagerHelper.h"
#include "os/CPowerManager.h"

namespace Elastos {
namespace Droid {
namespace Os {

ECode CPowerManagerHelper::UseScreenAutoBrightnessAdjustmentFeature(
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
    *result = CPowerManager::UseScreenAutoBrightnessAdjustmentFeature();
    return NOERROR;
}

ECode CPowerManagerHelper::UseTwilightAdjustmentFeature(
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
    *result = CPowerManager::UseTwilightAdjustmentFeature();
    return NOERROR;
}

ECode CPowerManagerHelper::ValidateWakeLockParameters(
    /* [in] */ Int32 levelAndFlags,
    /* [in] */ const String& tag)
{
    return CPowerManager::ValidateWakeLockParameters(levelAndFlags, tag);
}

} // namespace Os
} // namespace Droid
} // namespace Elastos
