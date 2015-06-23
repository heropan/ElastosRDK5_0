#ifndef __CACCESSIBILITYNODEINFOHELPER_H__
#define __CACCESSIBILITYNODEINFOHELPER_H__

#include "_CAccessibilityNodeInfoHelper.h"

namespace Elastos {
namespace Droid {
namespace View {
namespace Accessibility {

CarClass(CAccessibilityNodeInfoHelper)
{
public:
    CARAPI GetAccessibilityViewId(
        /* [in] */ Int64 accessibilityNodeId,
        /* [out] */ Int32* viewId);

    CARAPI GetVirtualDescendantId(
        /* [in] */ Int64 accessibilityNodeId,
        /* [out] */ Int32* descendantId);

    CARAPI MakeNodeId(
        /* [in] */ Int32 accessibilityViewId,
        /* [in] */ Int32 virtualDescendantId,
        /* [out] */ Int64* nodeId);

    CARAPI Obtain(
        /* [in] */ IView* source,
        /* [out] */ IAccessibilityNodeInfo** info);

    CARAPI Obtain(
        /* [in] */ IView* root,
        /* [in] */ Int32 virtualDescendantId,
        /* [out] */ IAccessibilityNodeInfo** info);

    CARAPI Obtain(
        /* [out] */ IAccessibilityNodeInfo** info);

    CARAPI Obtain(
        /* [in] */ IAccessibilityNodeInfo* info,
        /* [out] */ IAccessibilityNodeInfo** infoClone);
};

} // Accessibility
} // View
} // Droid
} // Elastos

#endif //__CAccessibilityNodeInfoHelper_H__
