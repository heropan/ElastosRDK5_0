
#include "elastos/droid/ext/frameworkdef.h"
#include "elastos/droid/view/accessibility/AccessibilityNodeProvider.h"

namespace Elastos {
namespace Droid {
namespace View {
namespace Accessibility {

ECode AccessibilityNodeProvider::CreateAccessibilityNodeInfo(
    /* [in] */ Int32 virtualViewId,
    /* [out] */ IAccessibilityNodeInfo** nodeInfo)
{
    VALIDATE_NOT_NULL(nodeInfo);
    *nodeInfo = NULL;
    return NOERROR;
}
ECode AccessibilityNodeProvider::PerformAction(
    /* [in] */ Int32 virtualViewId,
    /* [in] */ Int32 action,
    /* [in] */ IBundle* arguments,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
    *result = FALSE;
    return NOERROR;
}

ECode AccessibilityNodeProvider::FindAccessibilityNodeInfosByText(
    /* [in] */ const String& text,
    /* [in] */ Int32 virtualViewId,
    /* [out] */ IObjectContainer** nodeInfos)
{
    VALIDATE_NOT_NULL(nodeInfos);
    *nodeInfos = NULL;
    return NOERROR;
}

} // Accessibility
} // View
} // Droid
} // Elastos
