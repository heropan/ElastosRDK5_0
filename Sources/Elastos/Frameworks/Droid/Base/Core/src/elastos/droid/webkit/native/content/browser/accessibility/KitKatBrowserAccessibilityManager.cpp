
#include "elastos/droid/webkit/native/content/browser/accessibility/KitKatBrowserAccessibilityManager.h"
#include "elastos/droid/webkit/native/content/browser/ContentViewCore.h"

using Elastos::Droid::View::Accessibility::ICollectionInfo;
using Elastos::Droid::View::Accessibility::ICollectionInfoHelper;

namespace Elastos {
namespace Droid {
namespace Webkit {
namespace Content {
namespace Browser {
namespace Accessibility {

KitKatBrowserAccessibilityManager::KitKatBrowserAccessibilityManager(
    /* [in] */ Int64 nativeBrowserAccessibilityManagerAndroid,
    /* [in] */ ContentViewCore* contentViewCore)
    : JellyBeanBrowserAccessibilityManager(nativeBrowserAccessibilityManagerAndroid, contentViewCore)
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
    assert(0);
    // TODO
    // node->SetLiveRegion(liveRegion);
}

//@Override
void KitKatBrowserAccessibilityManager::SetAccessibilityNodeInfoCollectionInfo(
    /* [in] */ IAccessibilityNodeInfo* node,
    /* [in] */ Int32 rowCount,
    /* [in] */ Int32 columnCount,
    /* [in] */ Boolean hierarchical)
{
    AutoPtr<ICollectionInfo> info;
    AutoPtr<ICollectionInfoHelper> help;
    assert(0);
    // TODO
    // CCollectionInfoHelper::AcquireSingleton((ICollectionInfoHelper**)&help);
    // help->Obtain(rowCount, columnCount, hierarchical, (ICollectionInfo**)&info);
    node->SetCollectionInfo(info);
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
    AutoPtr<ICollectionInfo> info;
    AutoPtr<ICollectionInfoHelper> help;
    assert(0);
    // TODO
    // CCollectionInfoHelper::AcquireSingleton((ICollectionInfoHelper**)&help);
    // help->Obtain(rowIndex, rowSpan, columnIndex, columnSpan, heading, (ICollectionInfo**)&info);
    // node->SetCollectionItemInfo(info);
}

//@Override
void KitKatBrowserAccessibilityManager::SetAccessibilityNodeInfoRangeInfo(
    /* [in] */ IAccessibilityNodeInfo* node,
    /* [in] */ Int32 rangeType,
    /* [in] */ Float min,
    /* [in] */ Float max,
    /* [in] */ Float current)
{
    AutoPtr<ICollectionInfo> info;
    AutoPtr<ICollectionInfoHelper> help;
    assert(0);
    // TODO
    // CCollectionInfoHelper::AcquireSingleton((ICollectionInfoHelper**)&help);
    // help->Obtain(rangeType, min, max, current, (ICollectionInfo**)&info);
    // node->SetRangeInfo(info);
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
