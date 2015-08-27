// wuweizuo automatic build .cpp file from .java file.

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
    /*
    super(context, cvc);
    */
}

Boolean JellyBeanContentView::PerformAccessibilityAction(
    /* [in] */ Int32 action,
    /* [in] */ IBundle* arguments)
{
    /*
    if (mContentViewCore.supportsAccessibilityAction(action)) {
        return mContentViewCore.performAccessibilityAction(action, arguments);
    }


    return super.performAccessibilityAction(action, arguments);
    */
}

AutoPtr<IAccessibilityNodeProvider> JellyBeanContentView::GetAccessibilityNodeProvider()
{
    /*
    AccessibilityNodeProvider provider = mContentViewCore.getAccessibilityNodeProvider();
    if (provider != null) {
        return provider;
    } else {
        return super.getAccessibilityNodeProvider();
    }
    */
}

} // namespace Browser
} // namespace Content
} // namespace Webkit
} // namespace Droid
} // namespace Elastos


