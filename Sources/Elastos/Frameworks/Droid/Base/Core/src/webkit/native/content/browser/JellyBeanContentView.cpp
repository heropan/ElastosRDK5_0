// wuweizuo automatic build .cpp file from .java file.

#include "JellyBeanContentView.h"

namespace Elastos {
namespace Droid {
namespace Webkit {
namespace Content {
namespace Browser {

//=====================================================================
//                         JellyBeanContentView
//=====================================================================
JellyBeanContentView::JellyBeanContentView(
    /* [in] */ IContext* context,
    /* [in] */ ContentViewCore* cvc)
{
    // ==================before translated======================
    // super(context, cvc);
}

Boolean JellyBeanContentView::PerformAccessibilityAction(
    /* [in] */ Int32 action,
    /* [in] */ IBundle* arguments)
{
    // ==================before translated======================
    // if (mContentViewCore.supportsAccessibilityAction(action)) {
    //     return mContentViewCore.performAccessibilityAction(action, arguments);
    // }
    //
    // return super.performAccessibilityAction(action, arguments);
    assert(0);
    return FALSE;
}

AutoPtr<IAccessibilityNodeProvider> JellyBeanContentView::GetAccessibilityNodeProvider()
{
    // ==================before translated======================
    // AccessibilityNodeProvider provider = mContentViewCore.getAccessibilityNodeProvider();
    // if (provider != null) {
    //     return provider;
    // } else {
    //     return super.getAccessibilityNodeProvider();
    // }
    assert(0);
    AutoPtr<IAccessibilityNodeProvider> empty;
    return empty;
}

} // namespace Browser
} // namespace Content
} // namespace Webkit
} // namespace Droid
} // namespace Elastos


