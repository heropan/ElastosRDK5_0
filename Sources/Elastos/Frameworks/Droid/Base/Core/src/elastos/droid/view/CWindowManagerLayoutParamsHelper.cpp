
#include "view/CWindowManagerLayoutParamsHelper.h"
#include "view/CWindowManagerLayoutParams.h"
#include "ext/frameworkdef.h"

namespace Elastos {
namespace Droid {
namespace View {

CWindowManagerLayoutParamsHelper::MayUseInputMethodHelper(
    /* [in] */ Int32 flags,
    /* [out] */ Boolean* mayUse)
{
    VALIDATE_NOT_NULL(mayUse);
    *mayUse = CWindowManagerLayoutParams::MayUseInputMethod(flags);
    return NOERROR;
}

} // namespace View
} // namepsace Droid
} // namespace Elastos
