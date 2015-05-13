
#include "view/accessibility/CAccessibilityRecord.h"

namespace Elastos {
namespace Droid {
namespace View {
namespace Accessibility {

PInterface CAccessibilityRecord::Probe(
    /* [in] */ REIID riid)
{
    if (riid == EIID_AccessibilityRecord) {
        return reinterpret_cast<PInterface>((AccessibilityRecord*)this);
    }
    return _CAccessibilityRecord::Probe(riid);
}

ECode CAccessibilityRecord::SetSource(
    /* [in] */ IView* source)
{
    return AccessibilityRecord::SetSource(source);
}

ECode CAccessibilityRecord::SetSourceEx(
    /* [in] */ IView* root,
    /* [in] */ Int32 virtualDescendantId)
{
    return AccessibilityRecord::SetSourceEx(root, virtualDescendantId);
}

ECode CAccessibilityRecord::GetSource(
    /* [out] */ IAccessibilityNodeInfo** info)
{
    VALIDATE_NOT_NULL(info);
    return AccessibilityRecord::GetSource(info);
}

ECode CAccessibilityRecord::SetWindowId(
    /* [in] */ Int32 windowId)
{
    return AccessibilityRecord::SetWindowId(windowId);
}

ECode CAccessibilityRecord::GetWindowId(
    /* [out] */ Int32* windowId)
{
    VALIDATE_NOT_NULL(windowId);
    return AccessibilityRecord::GetWindowId(windowId);
}

ECode CAccessibilityRecord::IsChecked(
    /* [out] */ Boolean* checked)
{
    VALIDATE_NOT_NULL(checked);
    return AccessibilityRecord::IsChecked(checked);
}

ECode CAccessibilityRecord::SetChecked(
    /* [in] */ Boolean isChecked)
{
    return AccessibilityRecord::SetChecked(isChecked);
}

ECode CAccessibilityRecord::IsEnabled(
    /* [out] */ Boolean* enabled)
{
    VALIDATE_NOT_NULL(enabled);
    return AccessibilityRecord::IsEnabled(enabled);
}

ECode CAccessibilityRecord::SetEnabled(
    /* [in] */ Boolean enabled)
{
    return AccessibilityRecord::SetEnabled(enabled);
}

ECode CAccessibilityRecord::IsPassword(
    /* [out] */ Boolean* password)
{
    VALIDATE_NOT_NULL(password);
    return AccessibilityRecord::IsPassword(password);
}

ECode CAccessibilityRecord::SetPassword(
    /* [in] */ Boolean password)
{
    return AccessibilityRecord::SetPassword(password);
}

ECode CAccessibilityRecord::IsFullScreen(
    /* [out] */ Boolean* isFull)
{
    VALIDATE_NOT_NULL(isFull);
    return AccessibilityRecord::IsFullScreen(isFull);
}

ECode CAccessibilityRecord::SetFullScreen(
    /* [in] */ Boolean full)
{
    return AccessibilityRecord::SetFullScreen(full);
}

ECode CAccessibilityRecord::IsScrollable(
    /* [out] */ Boolean* scrollable)
{
    VALIDATE_NOT_NULL(scrollable);
    return AccessibilityRecord::IsScrollable(scrollable);
}

ECode CAccessibilityRecord::SetScrollable(
    /* [in] */ Boolean scrollable)
{
    return AccessibilityRecord::SetScrollable(scrollable);
}

ECode CAccessibilityRecord::IsImportantForAccessibility(
    /* [out] */ Boolean* important)
{
    VALIDATE_NOT_NULL(important);
    return AccessibilityRecord::IsImportantForAccessibility(important);
}

ECode CAccessibilityRecord::GetItemCount(
    /* [out] */ Int32* count)
{
    VALIDATE_NOT_NULL(count);
    return AccessibilityRecord::GetItemCount(count);
}

ECode CAccessibilityRecord::SetItemCount(
    /* [in] */ Int32 count)
{
    return AccessibilityRecord::SetItemCount(count);
}

ECode CAccessibilityRecord::GetCurrentItemIndex(
    /* [out] */ Int32* index)
{
    VALIDATE_NOT_NULL(index);
    return AccessibilityRecord::GetCurrentItemIndex(index);
}

ECode CAccessibilityRecord::SetCurrentItemIndex(
    /* [in] */ Int32 index)
{
    return AccessibilityRecord::SetCurrentItemIndex(index);
}

ECode CAccessibilityRecord::GetFromIndex(
    /* [out] */ Int32* index)
{
    VALIDATE_NOT_NULL(index);
    return AccessibilityRecord::GetFromIndex(index);
}

ECode CAccessibilityRecord::SetFromIndex(
    /* [in] */ Int32 index)
{
    return AccessibilityRecord::SetFromIndex(index);
}

ECode CAccessibilityRecord::GetToIndex(
    /* [out] */ Int32* index)
{
    VALIDATE_NOT_NULL(index);
    return AccessibilityRecord::GetToIndex(index);
}

ECode CAccessibilityRecord::SetToIndex(
    /* [in] */ Int32 index)
{
    return AccessibilityRecord::SetToIndex(index);
}

ECode CAccessibilityRecord::GetScrollX(
    /* [out] */ Int32* x)
{
    VALIDATE_NOT_NULL(x);
    return AccessibilityRecord::GetScrollX(x);
}

ECode CAccessibilityRecord::SetScrollX(
    /* [in] */ Int32 x)
{
    return AccessibilityRecord::SetScrollX(x);
}

ECode CAccessibilityRecord::GetScrollY(
    /* [out] */ Int32* y)
{
    VALIDATE_NOT_NULL(y);
    return AccessibilityRecord::GetScrollY(y);
}

ECode CAccessibilityRecord::SetScrollY(
    /* [in] */ Int32 y)
{
    return AccessibilityRecord::SetScrollY(y);
}

ECode CAccessibilityRecord::GetMaxScrollX(
    /* [out] */ Int32* x)
{
    VALIDATE_NOT_NULL(x);
    return AccessibilityRecord::GetMaxScrollX(x);
}

ECode CAccessibilityRecord::SetMaxScrollX(
    /* [in] */ Int32 x)
{
    return AccessibilityRecord::SetMaxScrollX(x);
}

ECode CAccessibilityRecord::GetMaxScrollY(
    /* [out] */ Int32* y)
{
    VALIDATE_NOT_NULL(y);
    return AccessibilityRecord::GetMaxScrollY(y);
}

ECode CAccessibilityRecord::SetMaxScrollY(
    /* [in] */ Int32 y)
{
    return AccessibilityRecord::SetMaxScrollY(y);
}

ECode CAccessibilityRecord::GetAddedCount(
    /* [out] */ Int32* count)
{
    VALIDATE_NOT_NULL(count);
    return AccessibilityRecord::GetAddedCount(count);
}

ECode CAccessibilityRecord::SetAddedCount(
    /* [in] */ Int32 count)
{
    return AccessibilityRecord::SetAddedCount(count);
}

ECode CAccessibilityRecord::GetRemovedCount(
    /* [out] */ Int32* count)
{
    VALIDATE_NOT_NULL(count);
    return AccessibilityRecord::GetRemovedCount(count);
}

ECode CAccessibilityRecord::SetRemovedCount(
    /* [in] */ Int32 count)
{
    return AccessibilityRecord::SetRemovedCount(count);
}

ECode CAccessibilityRecord::GetClassName(
    /* [out] */ ICharSequence** name)
{
    VALIDATE_NOT_NULL(name);
    return AccessibilityRecord::GetClassName(name);
}

ECode CAccessibilityRecord::SetClassName(
    /* [in] */ ICharSequence* name)
{
    return AccessibilityRecord::SetClassName(name);
}

ECode CAccessibilityRecord::GetText(
    /* [out] */ IObjectContainer** container)
{
    VALIDATE_NOT_NULL(container);
    return AccessibilityRecord::GetText(container);
}

ECode CAccessibilityRecord::GetBeforeText(
    /* [out] */ ICharSequence** text)
{
    VALIDATE_NOT_NULL(text);
    return AccessibilityRecord::GetBeforeText(text);
}

ECode CAccessibilityRecord::SetBeforeText(
    /* [in] */ ICharSequence* text)
{
    return AccessibilityRecord::SetBeforeText(text);
}

ECode CAccessibilityRecord::GetContentDescription(
    /* [out] */ ICharSequence** contentDescription)
{
    VALIDATE_NOT_NULL(contentDescription);
    return AccessibilityRecord::GetContentDescription(contentDescription);
}

ECode CAccessibilityRecord::SetContentDescription(
    /* [in] */ ICharSequence* contentDescription)
{
    return AccessibilityRecord::SetContentDescription(contentDescription);
}

ECode CAccessibilityRecord::GetParcelableData(
    /* [out] */ IParcelable** parcelableData)
{
    VALIDATE_NOT_NULL(parcelableData);
    return AccessibilityRecord::GetParcelableData(parcelableData);
}

ECode CAccessibilityRecord::SetParcelableData(
    /* [in] */ IParcelable* parcelableData)
{
    return AccessibilityRecord::SetParcelableData(parcelableData);
}

ECode CAccessibilityRecord::GetSourceNodeId(
    /* [out] */ Int64* nodeId)
{
    VALIDATE_NOT_NULL(nodeId);
    return AccessibilityRecord::GetSourceNodeId(nodeId);
}

ECode CAccessibilityRecord::SetConnectionId(
    /* [in] */ Int32 connectionId)
{
    return AccessibilityRecord::SetConnectionId(connectionId);
}

ECode CAccessibilityRecord::SetSealed(
    /* [in] */ Boolean sealed)
{
    return AccessibilityRecord::SetSealed(sealed);
}

ECode CAccessibilityRecord::Obtain(
    /* [in] */ IAccessibilityRecord* record,
    /* [out] */ IAccessibilityRecord** newInstance)
{
    VALIDATE_NOT_NULL(newInstance);
    AutoPtr<IAccessibilityRecord> clone;
    ObtainEx((IAccessibilityRecord**)&clone);
    ((CAccessibilityRecord*)clone.Get())->Init((AccessibilityRecord*)record);
    *newInstance = clone;
    INTERFACE_ADDREF(*newInstance);
    return NOERROR;
}

ECode CAccessibilityRecord::ObtainEx(
    /* [out] */ IAccessibilityRecord** newInstance)
{
    VALIDATE_NOT_NULL(newInstance);
    Mutex::Autolock lock(sPoolLock);
    if (sPool != NULL) {
        AccessibilityRecord* record = sPool;
        sPool = sPool->mNext;
        sPoolSize--;
        record->mNext = NULL;
        record->mIsInPool = FALSE;
        *newInstance = (IAccessibilityRecord*)record->Probe(EIID_IAccessibilityRecord);
        INTERFACE_ADDREF(*newInstance);
        return NOERROR;
    }
    AutoPtr<CAccessibilityRecord> crecord;
    CAccessibilityRecord::NewByFriend((CAccessibilityRecord**)&crecord);
    *newInstance = crecord;
    INTERFACE_ADDREF(*newInstance);
    return NOERROR;
}

ECode CAccessibilityRecord::Recycle()
{
    return AccessibilityRecord::Recycle();
}


} // Accessibility
} // View
} // Droid
} // Elastos
