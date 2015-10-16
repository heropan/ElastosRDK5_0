#include "elastos/droid/view/accessibility/CAccessibilityManagerHelper.h"
#include "elastos/droid/view/accessibility/CAccessibilityManager.h"

namespace Elastos {
namespace Droid {
namespace View {
namespace Accessibility {

ECode CAccessibilityManagerHelper::CreateAsSharedAcrossUsers(
    /* [in] */ IContext* context)
{
    return CAccessibilityManager::CreateAsSharedAcrossUsers(context);
}

ECode CAccessibilityManagerHelper::GetInstance(
    /* [in] */ IContext* context,
    /* [out] */ IAccessibilityManager** manager)
{
    VALIDATE_NOT_NULL(manager);
    return CAccessibilityManager::GetInstance(context, manager);
}

} // Accessibility
} // View
} // Droid
} // Elastos
