
namespace Elastos {
namespace Droid {
namespace Webkit {
namespace Content {
namespace Browser {
namespace Accessibility {

KitKatBrowserAccessibilityManager::KitKatBrowserAccessibilityManager(
    /* [in] */ Int64 nativeBrowserAccessibilityManagerAndroid,
    /* [in] */ ContentViewCore* contentViewCore)
    : JellyBeanBrowserAccessibilityManager(nativeBrowserAccessibilityManagerAndroid, contentViewCore);
{
}

//@Override
void KitKatBrowserAccessibilityManager::SetAccessibilityNodeInfoKitKatAttributes(
    /* [in] */ IAccessibilityNodeInfo* node,
    /* [in] */ Boolean canOpenPopup,
    /* [in] */ Boolean contentInvalid,
    /* [in] */ Boolean dismissable,
    /* [in] */ Boolean multiLine,
    /* [in] */ Int32 inputType,
    /* [in] */ Int32 liveRegion)
{
    node->SetCanOpenPopup(canOpenPopup);
    node->SetContentInvalid(contentInvalid);
    node->SetDismissable(contentInvalid);
    node->SetMultiLine(multiLine);
    node->SetInputType(inputType);
    node->SetLiveRegion(liveRegion);
}

//@Override
void KitKatBrowserAccessibilityManager::SetAccessibilityNodeInfoCollectionInfo(
    /* [in] */ IAccessibilityNodeInfo* node,
    /* [in] */ Int32 rowCount,
    /* [in] */ Int32 columnCount,
    /* [in] */ Boolean hierarchical)
{
    node->SetCollectionInfo(AccessibilityNodeInfo.CollectionInfo.obtain(
            rowCount, columnCount, hierarchical));
}

//@Override
void KitKatBrowserAccessibilityManager::SetAccessibilityNodeInfoCollectionItemInfo(
    /* [in] */ IAccessibilityNodeInfo* node,
    /* [in] */ Int32 rowIndex,
    /* [in] */ Int32 rowSpan,
    /* [in] */ Int32 columnIndex,
    /* [in] */ Int32 columnSpan,
    /* [in] */ Boolean heading)
{
    node->SetCollectionItemInfo(AccessibilityNodeInfo.CollectionItemInfo.obtain(
            rowIndex, rowSpan, columnIndex, columnSpan, heading));
}

//@Override
void KitKatBrowserAccessibilityManager::SetAccessibilityNodeInfoRangeInfo(
    /* [in] */ IAccessibilityNodeInfo* node,
    /* [in] */ Int32 rangeType,
    /* [in] */ Float min,
    /* [in] */ Float max,
    /* [in] */ Float current)
{
    node->SetRangeInfo(AccessibilityNodeInfo.RangeInfo.obtain(
            rangeType, min, max, current));
}

//@Override
void KitKatBrowserAccessibilityManager::SetAccessibilityEventKitKatAttributes(
    /* [in] */ IAccessibilityEvent* event,
    /* [in] */ Boolean canOpenPopup,
    /* [in] */ Boolean contentInvalid,
    /* [in] */ Boolean dismissable,
    /* [in] */ Boolean multiLine,
    /* [in] */ Int32 inputType,
    /* [in] */ Int32 liveRegion)
{
    // This is just a fallback for pre-KitKat systems.
    // Do nothing on KitKat and higher.
}

//@Override
void KitKatBrowserAccessibilityManager::SetAccessibilityEventCollectionInfo(
    /* [in] */ IAccessibilityEvent* event,
    /* [in] */ Int32 rowCount,
    /* [in] */ Int32 columnCount,
    /* [in] */ Boolean hierarchical)
{
    // This is just a fallback for pre-KitKat systems.
    // Do nothing on KitKat and higher.
}

//@Override
void KitKatBrowserAccessibilityManager::SetAccessibilityEventHeadingFlag(
    /* [in] */ IAccessibilityEvent* event,
    /* [in] */ Boolean heading)
{
    // This is just a fallback for pre-KitKat systems.
    // Do nothing on KitKat and higher.
}

//@Override
void KitKatBrowserAccessibilityManager::SetAccessibilityEventCollectionItemInfo(
    /* [in] */ IAccessibilityEvent* event,
    /* [in] */ Int32 rowIndex,
    /* [in] */ Int32 rowSpan,
    /* [in] */ Int32 columnIndex,
    /* [in] */ Int32 columnSpan)
{
    // This is just a fallback for pre-KitKat systems.
    // Do nothing on KitKat and higher.
}

//@Override
void KitKatBrowserAccessibilityManager::SetAccessibilityEventRangeInfo(
    /* [in] */ IAccessibilityEvent* event,
    /* [in] */ Int32 rangeType,
    /* [in] */ Float min,
    /* [in] */ Float max,
    /* [in] */ Float current)
{
    // This is just a fallback for pre-KitKat systems.
    // Do nothing on KitKat and higher.
}

} // namespace Accessibility
} // namespace Browser
} // namespace Content
} // namespace Webkit
} // namespace Droid
} // namespace Elastos
