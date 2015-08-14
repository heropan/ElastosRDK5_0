
#ifndef __ELASTOS_DROID_WEBKIT_CONTENT_BROWSER_ACCESSIBILITY_BROWSERACCESSIBILITYMANAGER_H__
#define __ELASTOS_DROID_WEBKIT_CONTENT_BROWSER_ACCESSIBILITY_BROWSERACCESSIBILITYMANAGER_H__

// import android.content.Context;
// import android.graphics.Rect;
// import android.os.Build;
// import android.os.Bundle;
// import android.text.SpannableString;
// import android.text.style.URLSpan;
// import android.view.MotionEvent;
// import android.view.View;
// import android.view.ViewGroup;
// import android.view.ViewParent;
// import android.view.accessibility.AccessibilityEvent;
// import android.view.accessibility.AccessibilityManager;
// import android.view.accessibility.AccessibilityNodeInfo;
// import android.view.accessibility.AccessibilityNodeProvider;

// import org.chromium.base.CalledByNative;
// import org.chromium.base.JNINamespace;
// import org.chromium.content.browser.ContentViewCore;
// import org.chromium.content.browser.RenderCoordinates;

// import java.util.ArrayList;
// import java.util.List;
// import java.util.Locale;

namespace Elastos {
namespace Droid {
namespace Webkit {
namespace Content {
namespace Browser {
namespace Accessibility {

/**
 * Native accessibility for a {@link ContentViewCore}.
 *
 * This class is safe to load on ICS and can be used to run tests, but
 * only the subclass, JellyBeanBrowserAccessibilityManager, actually
 * has a AccessibilityNodeProvider implementation needed for native
 * accessibility.
 */
//@JNINamespace("content")
public class BrowserAccessibilityManager
{
public:
    /**
     * @return An AccessibilityNodeProvider on JellyBean, and null on previous versions.
     */
    CARAPI_(AutoPtr<IAccessibilityNodeProvider>) GetAccessibilityNodeProvider();

    /**
     * @see View#onHoverEvent(MotionEvent)
     */
    CARAPI_(Boolean) OnHoverEvent(
        /* [in] */ IMotionEvent* event);

    /**
     * Called by ContentViewCore to notify us when the frame info is initialized,
     * the first time, since until that point, we can't use mRenderCoordinates to transform
     * web coordinates to screen coordinates.
     */
    CARAPI_(void) NotifyFrameInfoInitialized();

protected:
    BrowserAccessibilityManager(
        /* [in] */ Int64 nativeBrowserAccessibilityManagerAndroid,
        /* [in] */ ContentViewCore* contentViewCore);

        /**
     * @see AccessibilityNodeProvider#createAccessibilityNodeInfo(int)
     */
    virtual CARAPI_(AutoPtr<IAccessibilityNodeInfo>) CreateAccessibilityNodeInfo(
        /* [in] */ Int32 virtualViewId);

    /**
     * @see AccessibilityNodeProvider#findAccessibilityNodeInfosByText(String, int)
     */
    virtual CARAPI_(List<AccessibilityNodeInfo>) FindAccessibilityNodeInfosByText(
        /* [in] */ String text,
        /* [in] */ Int32 virtualViewId);

    /**
     * @see AccessibilityNodeProvider#performAction(int, int, Bundle)
     */
    virtual CARAPI_(Boolean)  PerformAction(
        /* [in] */ Int32 virtualViewId,
        /* [in] */ Int32 action,
        /* [in] */ IBundle* arguments);

    //@CalledByNative
    virtual CARAPI_(void)  SetAccessibilityNodeInfoKitKatAttributes(
        /* [in] */ IAccessibilityNodeInfo* node,
        /* [in] */ Boolean canOpenPopup,
        /* [in] */ Boolean contentInvalid,
        /* [in] */ Boolean dismissable,
        /* [in] */ Boolean multiLine,
        /* [in] */ Int32 inputType,
        /* [in] */ Int32 liveRegion);

    //@CalledByNative
    virtual CARAPI_(void)  SetAccessibilityNodeInfoCollectionInfo(
        /* [in] */ IAccessibilityNodeInfo* node,
        /* [in] */ Int32 rowCount,
        /* [in] */ Int32 columnCount,
        /* [in] */ Boolean hierarchical);

    //@CalledByNative
    virtual CARAPI_(void)  SetAccessibilityNodeInfoCollectionItemInfo(
        /* [in] */ IAccessibilityNodeInfo* node,
        /* [in] */ Int32 rowIndex,
        /* [in] */ Int32 rowSpan,
        /* [in] */ Int32 columnIndex,
        /* [in] */ Int32 columnSpan,
        /* [in] */ Boolean heading);

    //@CalledByNative
    virtual CARAPI_(void)  SetAccessibilityNodeInfoRangeInfo(
        /* [in] */ IAccessibilityNodeInfo* node,
        /* [in] */ Int32 rangeType,
        /* [in] */ Float min,
        /* [in] */ Float max,
        /* [in] */ Float current);

    //@CalledByNative
    virtual CARAPI_(void)  SetAccessibilityEventKitKatAttributes(
        /* [in] */ IAccessibilityEvent* event,
        /* [in] */ Boolean canOpenPopup,
        /* [in] */ Boolean contentInvalid,
        /* [in] */ Boolean dismissable,
        /* [in] */ Boolean multiLine,
        /* [in] */ Int32 inputType,
        /* [in] */ Int32 liveRegion);

    //@CalledByNative
    virtual CARAPI_(void)  SetAccessibilityEventCollectionInfo(
        /* [in] */ IAccessibilityEvent* event,
        /* [in] */ Int32 rowCount,
        /* [in] */ Int32 columnCount,
        /* [in] */ Boolean hierarchical);

    //@CalledByNative
    virtual CARAPI_(void)  SetAccessibilityEventHeadingFlag(
        /* [in] */ IAccessibilityEvent* event,
        /* [in] */ Boolean heading);

    //@CalledByNative
    virtual CARAPI_(void)  SetAccessibilityEventCollectionItemInfo(
        /* [in] */ IAccessibilityEvent* event,
        /* [in] */ Int32 rowIndex,
        /* [in] */ Int32 rowSpan,
        /* [in] */ Int32 columnIndex,
        /* [in] */ Int32 columnSpan);

    //@CalledByNative
    virtual CARAPI_(void)  SetAccessibilityEventRangeInfo(
        /* [in] */ IAccessibilityEvent* event,
        /* [in] */ Int32 rangeType,
        /* [in] */ Float min,
        /* [in] */ Float max,
        /* [in] */ Float current);

private:
    /**
     * Create a BrowserAccessibilityManager object, which is owned by the C++
     * BrowserAccessibilityManagerAndroid instance, and connects to the content view.
     * @param nativeBrowserAccessibilityManagerAndroid A pointer to the counterpart native
     *     C++ object that owns this object.
     * @param contentViewCore The content view that this object provides accessibility for.
     */
    //@CalledByNative
    static CARAPI_(AutoPtr<BrowserAccessibilityManager) Create(
        /* [in] */ Int64 nativeBrowserAccessibilityManagerAndroid,
        /* [in] */ ContentViewCore* contentViewCore);

    //@CalledByNative
    CARAPI_(void) OnNativeObjectDestroyed();

    CARAPI_(Boolean)  JumpToElementType(
        /* [in] */ String elementType,
        /* [in] */ Boolean forwards);

    CARAPI_(void) SendAccessibilityEvent(
        /* [in] */ Int32 virtualViewId,
        /* [in] */ Int32 eventType);

    CARAPI_(AutoPtr<IBundle>) GetOrCreateBundleForAccessibilityEvent(
        /* [in] */ IAccessibilityEvent* event);

    CARAPI_(AutoPtr<IAccessibilityNodeInfo>) CreateNodeForHost(
        /* [in] */ Int32 rootId);

    CARAPI_(Boolean)  IsFrameInfoInitialized();

    //@CalledByNative
    CARAPI_(void) HandlePageLoaded(
        /* [in] */ Int32 id);

    //@CalledByNative
    CARAPI_(void) HandleFocusChanged(
        /* [in] */ Int32 id);

    //@CalledByNative
    CARAPI_(void) HandleCheckStateChanged(
        /* [in] */ Int32 id);

    //@CalledByNative
    CARAPI_(void) HandleTextSelectionChanged(
        /* [in] */ Int32 id);

    //@CalledByNative
    CARAPI_(void) HandleEditableTextChanged(
        /* [in] */ Int32 id);

    //@CalledByNative
    CARAPI_(void) HandleContentChanged(
        /* [in] */ Int32 id);

    //@CalledByNative
    CARAPI_(void) HandleNavigate();

    //@CalledByNative
    CARAPI_(void) HandleScrollPositionChanged(
        /* [in] */ Int32 id);

    //@CalledByNative
    CARAPI_(void) HandleScrolledToAnchor(
        /* [in] */ Int32 id);

    //@CalledByNative
    CARAPI_(void) HandleHover(
        /* [in] */ Int32 id);

    //@CalledByNative
    CARAPI_(void) AnnounceLiveRegionText(
        /* [in] */ String text);

    //@CalledByNative
    CARAPI_(void) SetAccessibilityNodeInfoParent(
        /* [in] */ IAccessibilityNodeInfo* node,
        /* [in] */ Int32 parentId);

    //@CalledByNative
    CARAPI_(void) AddAccessibilityNodeInfoChild(
        /* [in] */ IAccessibilityNodeInfo* node,
        /* [in] */ Int32 childId);

    //@CalledByNative
    CARAPI_(void) SetAccessibilityNodeInfoBooleanAttributes(
        /* [in] */ IAccessibilityNodeInfo* node,
        /* [in] */ Int32 virtualViewId,
        /* [in] */ Boolean checkable,
        /* [in] */ Boolean checked,
        /* [in] */ Boolean clickable,
        /* [in] */ Boolean enabled,
        /* [in] */ Boolean focusable,
        /* [in] */ Boolean focused,
        /* [in] */ Boolean password,
        /* [in] */ Boolean scrollable,
        /* [in] */ Boolean selected,
        /* [in] */ Boolean visibleToUser);

    //@CalledByNative
    CARAPI_(void) SetAccessibilityNodeInfoClassName(
        /* [in] */ IAccessibilityNodeInfo* node,
        /* [in] */ String className);

    //@CalledByNative
    CARAPI_(void) SetAccessibilityNodeInfoContentDescription(
        /* [in] */ IAccessibilityNodeInfo* node,
        /* [in] */ String contentDescription,
        /* [in] */ Boolean annotateAsLink);

    //@CalledByNative
    CARAPI_(void) SetAccessibilityNodeInfoLocation(
        /* [in] */ IAccessibilityNodeInfo* node,
        /* [in] */ Int32 absoluteLeft,
        /* [in] */ Int32 absoluteTop,
        /* [in] */ Int32 parentRelativeLeft,
        /* [in] */ Int32 parentRelativeTop,
        /* [in] */ Int32 width,
        /* [in] */ Int32 height,
        /* [in] */ Boolean isRootNode);

    //@CalledByNative
    CARAPI_(void) SetAccessibilityEventBooleanAttributes(
        /* [in] */ IAccessibilityEvent* event,
        /* [in] */ Boolean checked,
        /* [in] */ Boolean enabled,
        /* [in] */ Boolean password,
        /* [in] */ Boolean scrollable);

    //@CalledByNative
    CARAPI_(void) SetAccessibilityEventClassName(
        /* [in] */ IAccessibilityEvent* event,
        /* [in] */ String className);

    //@CalledByNative
    CARAPI_(void) SetAccessibilityEventListAttributes(
        /* [in] */ IAccessibilityEvent* event,
        /* [in] */ Int32 currentItemIndex,
        /* [in] */ Int32 itemCount);

    //@CalledByNative
    CARAPI_(void) SetAccessibilityEventScrollAttributes(
        /* [in] */ IAccessibilityEvent* event,
        /* [in] */ Int32 scrollX,
        /* [in] */ Int32 scrollY,
        /* [in] */ Int32 maxScrollX,
        /* [in] */ Int32 maxScrollY);

    //@CalledByNative
    CARAPI_(void) SetAccessibilityEventTextChangedAttrs(
        /* [in] */ IAccessibilityEvent* event,
        /* [in] */ Int32 fromIndex,
        /* [in] */ Int32 addedCount,
        /* [in] */ Int32 removedCount,
        /* [in] */ String beforeText,
        /* [in] */ String text);

    //@CalledByNative
    CARAPI_(void) SetAccessibilityEventSelectionAttrs(
        /* [in] */ IAccessibilityEvent* event,
        /* [in] */ Int32 fromIndex,
        /* [in] */ Int32 addedCount,
        /* [in] */ Int32 itemCount,
        /* [in] */ String text);

    CARAPI_(Int32) NativeGetRootId(
        /* [in] */ Int64 nativeBrowserAccessibilityManagerAndroid);

    CARAPI_(Boolean) NativeIsNodeValid(
        /* [in] */ Int64 nativeBrowserAccessibilityManagerAndroid,
        /* [in] */ Int32 id);

    CARAPI_(void) NativeHitTest(
        /* [in] */ Int64 nativeBrowserAccessibilityManagerAndroid,
        /* [in] */ Int32 x,
        /* [in] */ Int32 y);

    CARAPI_(Boolean) NativePopulateAccessibilityNodeInfo(
        /* [in] */ Int64 nativeBrowserAccessibilityManagerAndroid,
        /* [in] */ IAccessibilityNodeInfo* info,
        /* [in] */ Int32 id);

    CARAPI_(Boolean) NativePopulateAccessibilityEvent(
        /* [in] */ Int64 nativeBrowserAccessibilityManagerAndroid,
        /* [in] */ IAccessibilityEvent* event,
        /* [in] */ Int32 id,
        /* [in] */ Int32 eventType);

    CARAPI_(void) NativeClick(
        /* [in] */ Int64 nativeBrowserAccessibilityManagerAndroid,
        /* [in] */ Int32 id);

    CARAPI_(void) NativeFocus(
        /* [in] */ Int64 nativeBrowserAccessibilityManagerAndroid,
        /* [in] */ Int32 id);

    CARAPI_(void) NativeBlur(
        /* [in] */ Int64 nativeBrowserAccessibilityManagerAndroid);

    CARAPI_(void) NativeScrollToMakeNodeVisible(
        /* [in] */ Int64 nativeBrowserAccessibilityManagerAndroid,
        /* [in] */ Int32 id);

    CARAPI_(Int32) NativeFindElementType(
        /* [in] */ Int64 nativeBrowserAccessibilityManagerAndroid,
        /* [in] */ Int32 startId,
        /* [in] */ String elementType,
        /* [in] */ Boolean forwards);

private:
    static const String TAG;

    AutoPtr<ContentViewCore> mContentViewCore;
    AutoPtr<IAccessibilityManager> mAccessibilityManager;
    const AutoPtr<RenderCoordinates> mRenderCoordinates;
    Int64 mNativeObj;
    Int32 mAccessibilityFocusId;
    Boolean mIsHovering;
    Int32 mLastHoverId;
    Int32 mCurrentRootId;
    const AutoPtr< ArrayOf<Int32> mTempLocation;
    const AutoPtr<IViewGroup> mView;
    Boolean mUserHasTouchExplored;
    Boolean mPendingScrollToMakeNodeVisible;
};

} // namespace Accessibility
} // namespace Browser
} // namespace Content
} // namespace Webkit
} // namespace Droid
} // namespace Elastos

#endif//__ELASTOS_DROID_WEBKIT_CONTENT_BROWSER_ACCESSIBILITY_BROWSERACCESSIBILITYMANAGER_H__
