
namespace Elastos {
namespace Droid {
namespace Webkit {
namespace Content {
namespace Browser {
namespace Accessibility {

//=================================================================================
//       JellyBeanBrowserAccessibilityManager::InnerAccessibilityNodeProvider
//=================================================================================

JellyBeanBrowserAccessibilityManager::InnerAccessibilityNodeProvider::InnerAccessibilityNodeProvider(
    /* [in] */ JellyBeanBrowserAccessibilityManager* owner)
    : mOwner(owner)
{
}

//@Override
ECode JellyBeanBrowserAccessibilityManager::InnerAccessibilityNodeProvider::CreateAccessibilityNodeInfo(
    /* [in] */ Int32 virtualViewId,
    /* [out] */ IAccessibilityNodeInfo** info)
{
    VALIDATE_NOT_NULL(info);
    *info = mOwner->CreateAccessibilityNodeInfo(virtualViewId);
    REFCOUNT_ADD(*info);
    return NOERROR;
}

//@Override
ECode JellyBeanBrowserAccessibilityManager::InnerAccessibilityNodeProvider::FindAccessibilityNodeInfosByText(
    /* [in] */ const String& text,
    /* [in] */ Int32 virtualViewId,
    /* [out] */ IObjectContainer** nodeInfos)
{
    VALIDATE_NOT_NULL(nodeInfos);
    *nodeInfos = mOwner->FindAccessibilityNodeInfosByText(text, virtualViewId);
    REFCOUNT_ADD(*nodeInfos);
    return NOERROR;
}

//@Override
ECode JellyBeanBrowserAccessibilityManager::InnerAccessibilityNodeProvider::PerformAction(
    /* [in] */ Int32 virtualViewId,
    /* [in] */ Int32 action,
    /* [in] */ IBundle* arguments,
    /* [in] */ Boolean* result)
{
    VALIDATE_NOT_NULL(arguments);
    VALIDATE_NOT_NULL(result);
    *result = mOwner->PerformAction(virtualViewId, action, arguments);
    return NOERROR;
}

//===============================================================
//             JellyBeanBrowserAccessibilityManager
//===============================================================

JellyBeanBrowserAccessibilityManager::JellyBeanBrowserAccessibilityManager(
    /* [in] */ Int64 nativeBrowserAccessibilityManagerAndroid,
    /* [in] */ ContentViewCore* contentViewCore)
    : BrowserAccessibilityManager(nativeBrowserAccessibilityManagerAndroid, contentViewCore)
{
    mAccessibilityNodeProvider = new InnerAccessibilityNodeProvider(this);
}

//@Override
AutoPtr<IAccessibilityNodeProvider> JellyBeanBrowserAccessibilityManager::GetAccessibilityNodeProvider()
{
    return mAccessibilityNodeProvider;
}

} // namespace Accessibility
} // namespace Browser
} // namespace Content
} // namespace Webkit
} // namespace Droid
} // namespace Elastos
