
#include "ext/frameworkdef.h"
#include "view/accessibility/CAccessibilityNodeInfoHelper.h"
#include "view/accessibility/CAccessibilityNodeInfo.h"

namespace Elastos {
namespace Droid {
namespace View {
namespace Accessibility {

ECode CAccessibilityNodeInfoHelper::GetAccessibilityViewId(
    /* [in] */ Int64 accessibilityNodeId,
    /* [out] */ Int32* viewId)
{
    VALIDATE_NOT_NULL(viewId);
    *viewId = CAccessibilityNodeInfo::GetAccessibilityViewId(accessibilityNodeId);
    return NOERROR;
}

ECode CAccessibilityNodeInfoHelper::GetVirtualDescendantId(
    /* [in] */ Int64 accessibilityNodeId,
    /* [out] */ Int32* descendantId)
{
    VALIDATE_NOT_NULL(descendantId);
    *descendantId = CAccessibilityNodeInfo::GetVirtualDescendantId(accessibilityNodeId);
    return NOERROR;
}

ECode CAccessibilityNodeInfoHelper::MakeNodeId(
    /* [in] */ Int32 accessibilityViewId,
    /* [in] */ Int32 virtualDescendantId,
    /* [out] */ Int64* nodeId)
{
    VALIDATE_NOT_NULL(nodeId);
    *nodeId = CAccessibilityNodeInfo::MakeNodeId(accessibilityViewId, virtualDescendantId);
    return NOERROR;
}

ECode CAccessibilityNodeInfoHelper::Obtain(
    /* [in] */ IView* source,
    /* [out] */ IAccessibilityNodeInfo** info)
{
    VALIDATE_NOT_NULL(info);
    return CAccessibilityNodeInfo::Obtain(source, info);
}

ECode CAccessibilityNodeInfoHelper::ObtainEx(
    /* [in] */ IView* root,
    /* [in] */ Int32 virtualDescendantId,
    /* [out] */ IAccessibilityNodeInfo** info)
{
    VALIDATE_NOT_NULL(info);
    return CAccessibilityNodeInfo::ObtainEx(root, virtualDescendantId, info);
}

ECode CAccessibilityNodeInfoHelper::ObtainEx2(
    /* [out] */ IAccessibilityNodeInfo** info)
{
    VALIDATE_NOT_NULL(info);
    return CAccessibilityNodeInfo::ObtainEx2(info);
}

ECode CAccessibilityNodeInfoHelper::ObtainEx3(
    /* [in] */ IAccessibilityNodeInfo* info,
    /* [out] */ IAccessibilityNodeInfo** infoClone)
{
    VALIDATE_NOT_NULL(infoClone);
    return CAccessibilityNodeInfo::ObtainEx3(info, infoClone);
}

} // Accessibility
} // View
} // Droid
} // Elastos
