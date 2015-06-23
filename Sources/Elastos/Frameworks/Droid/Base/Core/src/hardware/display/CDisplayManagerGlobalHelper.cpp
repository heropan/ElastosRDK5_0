
#include "ext/frameworkdef.h"
#include "hardware/display/CDisplayManagerGlobalHelper.h"
#include "hardware/display/DisplayManagerGlobal.h"

namespace Elastos {
namespace Droid {
namespace Hardware {
namespace Display {

ECode CDisplayManagerGlobalHelper::GetInstance(
    /* [out] */ IDisplayManagerGlobal** global)
{
    VALIDATE_NOT_NULL(global);
    AutoPtr<IDisplayManagerGlobal> temp = DisplayManagerGlobal::GetInstance();
    *global = temp;
    REFCOUNT_ADD(*global);
    return NOERROR;
}

} // namespace Display
} // namespace Hardware
} // namespace Droid
} // namespace Elastos
