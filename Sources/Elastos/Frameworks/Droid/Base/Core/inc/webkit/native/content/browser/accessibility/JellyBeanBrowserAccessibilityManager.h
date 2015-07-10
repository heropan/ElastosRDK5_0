
#ifndef __JELLYBEANBROWSERACCESSIBILITYMANAGER_H__
#define __JELLYBEANBROWSERACCESSIBILITYMANAGER_H__

// import android.os.Bundle;
// import android.view.accessibility.AccessibilityNodeInfo;
// import android.view.accessibility.AccessibilityNodeProvider;

// import org.chromium.base.JNINamespace;
// import org.chromium.content.browser.ContentViewCore;

// import java.util.List;

namespace Elastos {
namespace Droid {
namespace Webkit {
namespace Content {
namespace Browser {
namespace Accessibility {

/**
 * Subclass of BrowserAccessibilityManager for JellyBean that creates an
 * AccessibilityNodeProvider and delegates its implementation to this object.
 */
//@JNINamespace("content")
class JellyBeanBrowserAccessibilityManager : public BrowserAccessibilityManager
{
private:
    class InnerAccessibilityNodeProvider
        : public Object
        , public IAccessibilityNodeProvider
    {
    public:
        InnerAccessibilityNodeProvider(
            /* [in] */ JellyBeanBrowserAccessibilityManager* owner);

        //@Override
        CARAPI CreateAccessibilityNodeInfo(
            /* [in] */ Int32 virtualViewId,
            /* [out] */ IAccessibilityNodeInfo** info);

        //@Override
        CARAPI FindAccessibilityNodeInfosByText(
            /* [in] */ String text,
            /* [in] */ Int32 virtualViewId,
            /* [out] */ IObjectContainer** nodeInfos);

        //@Override
        CARAPI PerformAction(
            /* [in] */ Int32 virtualViewId,
            /* [in] */ Int32 action,
            /* [in] */ IBundle* arguments,
            /* [in] */ Boolean* result);

    private:
        JellyBeanBrowserAccessibilityManager* mOwner;
    };

public:
    JellyBeanBrowserAccessibilityManager(
        /* [in] */ Int64 nativeBrowserAccessibilityManagerAndroid,
        /* [in] */ ContentViewCore* contentViewCore);

    //@Override
    CARAPI_(AutoPtr<IAccessibilityNodeProvider>) GetAccessibilityNodeProvider();

private:
    AutoPtr<IAccessibilityNodeProvider> mAccessibilityNodeProvider;
};

} // namespace Accessibility
} // namespace Browser
} // namespace Content
} // namespace Webkit
} // namespace Droid
} // namespace Elastos

#endif//__JELLYBEANBROWSERACCESSIBILITYMANAGER_H__
