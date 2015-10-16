
#include "elastos/droid/view/accessibility/CAccessibilityNodeInfo.h"
#include "elastos/droid/view/accessibility/CAccessibilityInteractionClient.h"
#include "elastos/droid/graphics/CRect.h"
#include <elastos/utility/logging/Slogger.h>

using Elastos::Core::IInteger64;
using Elastos::Core::CInteger64;

using Elastos::Utility::Logging::Slogger;
using Elastos::Utility::CObjectInt32Map;
using Elastos::Droid::Graphics::CRect;

namespace Elastos {
namespace Droid {
namespace View {
namespace Accessibility {

const String CAccessibilityNodeInfo::TAG("AccessibilityNodeInfo");
const Boolean CAccessibilityNodeInfo::DEBUG = FALSE;
const Int32 CAccessibilityNodeInfo::PROPERTY_CHECKABLE = 0x00000001;
const Int32 CAccessibilityNodeInfo::PROPERTY_CHECKED = 0x00000002;
const Int32 CAccessibilityNodeInfo::PROPERTY_FOCUSABLE = 0x00000004;
const Int32 CAccessibilityNodeInfo::PROPERTY_FOCUSED = 0x00000008;
const Int32 CAccessibilityNodeInfo::PROPERTY_SELECTED = 0x00000010;
const Int32 CAccessibilityNodeInfo::PROPERTY_CLICKABLE = 0x00000020;
const Int32 CAccessibilityNodeInfo::PROPERTY_LONG_CLICKABLE = 0x00000040;
const Int32 CAccessibilityNodeInfo::PROPERTY_ENABLED = 0x00000080;
const Int32 CAccessibilityNodeInfo::PROPERTY_PASSWORD = 0x00000100;
const Int32 CAccessibilityNodeInfo::PROPERTY_SCROLLABLE = 0x00000200;
const Int32 CAccessibilityNodeInfo::PROPERTY_ACCESSIBILITY_FOCUSED = 0x00000400;
const Int32 CAccessibilityNodeInfo::PROPERTY_VISIBLE_TO_USER = 0x00000800;
const Int64 CAccessibilityNodeInfo::VIRTUAL_DESCENDANT_ID_MASK = 0xffffffff00000000ll;
const Int32 CAccessibilityNodeInfo::VIRTUAL_DESCENDANT_ID_SHIFT = 32;
const Int32 CAccessibilityNodeInfo::MAX_POOL_SIZE = 50;

AutoPtr<CAccessibilityNodeInfo> CAccessibilityNodeInfo::sPool = NULL;
Mutex CAccessibilityNodeInfo::sPoolLock;
Int32 CAccessibilityNodeInfo::sPoolSize = 0;

CAccessibilityNodeInfo::CAccessibilityNodeInfo()
    : mIsInPool(FALSE)
    , mSealed(FALSE)
    , mWindowId(UNDEFINED)
    , mSourceNodeId(ROOT_NODE_ID)
    , mParentNodeId(ROOT_NODE_ID)
    , mLabelForId(ROOT_NODE_ID)
    , mLabeledById(ROOT_NODE_ID)
    , mBooleanProperties(0)
    , mActions(0)
    , mMovementGranularities(0)
    , mConnectionId(UNDEFINED)
{
    ASSERT_SUCCEEDED(CRect::New((IRect**)&mBoundsInParent));
    ASSERT_SUCCEEDED(CRect::New((IRect**)&mBoundsInScreen));
}

Int32 CAccessibilityNodeInfo::GetAccessibilityViewId(
    /* [in] */ Int64 accessibilityNodeId)
{
    return (Int32)accessibilityNodeId;
}

Int32 CAccessibilityNodeInfo::GetVirtualDescendantId(
    /* [in] */ Int64 accessibilityNodeId)
{
    return (Int32)((accessibilityNodeId & VIRTUAL_DESCENDANT_ID_MASK)
            >> VIRTUAL_DESCENDANT_ID_SHIFT);
}

Int64 CAccessibilityNodeInfo::MakeNodeId(
    /* [in] */ Int32 accessibilityViewId,
    /* [in] */ Int32 virtualDescendantId)
{
    return (((Int64)virtualDescendantId) << VIRTUAL_DESCENDANT_ID_SHIFT)
            | accessibilityViewId;
}

ECode CAccessibilityNodeInfo::SetSource(
    /* [in] */ IView* source)
{
    return SetSource(source, UNDEFINED);
}

ECode CAccessibilityNodeInfo::SetSource(
    /* [in] */ IView* root,
    /* [in] */ Int32 virtualDescendantId)
{
    FAIL_RETURN(EnforceNotSealed());
    mWindowId = UNDEFINED;
    Int32 rootAccessibilityViewId = UNDEFINED;
    if (root != NULL) {
        root->GetAccessibilityWindowId(&mWindowId);
        root->GetAccessibilityViewId(&rootAccessibilityViewId);
    }
    mSourceNodeId = MakeNodeId(rootAccessibilityViewId, virtualDescendantId);
    return NOERROR;
}

ECode CAccessibilityNodeInfo::FindFocus(
    /* [in] */ Int32 focus,
    /* [out] */ IAccessibilityNodeInfo** info)
{
    VALIDATE_NOT_NULL(info);
    *info = NULL;

    FAIL_RETURN(EnforceSealed());
    FAIL_RETURN(EnforceValidFocusType(focus));
    if (!CanPerformRequestOverConnection(mSourceNodeId)) {

        return NOERROR;
    }

    AutoPtr<IAccessibilityInteractionClient> client = CAccessibilityInteractionClient::GetInstance();
    return client->FindFocus(mConnectionId, mWindowId, mSourceNodeId, focus, info);
}

ECode CAccessibilityNodeInfo::FocusSearch(
    /* [in] */ Int32 direction,
    /* [out] */ IAccessibilityNodeInfo** info)
{
    VALIDATE_NOT_NULL(info);
    *info = NULL;

    FAIL_RETURN(EnforceSealed());
    FAIL_RETURN(EnforceValidFocusDirection(direction));
    if (!CanPerformRequestOverConnection(mSourceNodeId)) {
        return NOERROR;
    }

    AutoPtr<IAccessibilityInteractionClient> client = CAccessibilityInteractionClient::GetInstance();
    return client->FocusSearch(mConnectionId, mWindowId, mSourceNodeId, direction, info);
}

ECode CAccessibilityNodeInfo::GetWindowId(
    /* [out] */ Int32* id)
{
    VALIDATE_NOT_NULL(id);
    *id = mWindowId;
    return NOERROR;
}

ECode CAccessibilityNodeInfo::GetChildNodeIds(
    /* [out] */ IObjectInt32Map** ids)
{
    VALIDATE_NOT_NULL(ids);

    AutoPtr<IObjectInt32Map> map;
    CObjectInt32Map::New((IObjectInt32Map**)&map);
    HashMap<Int32, Int64>::Iterator it = mChildNodeIds.Begin();
    for (; it != mChildNodeIds.End(); ++it) {
        AutoPtr<IInteger64> integer;
        CInteger64::New(it->mSecond, (IInteger64**)&integer);
        map->Put(it->mFirst, (IInterface*)integer);
    }
    *ids = map;
    REFCOUNT_ADD(*ids);
    return NOERROR;
}

ECode CAccessibilityNodeInfo::GetChildCount(
    /* [out] */ Int32* count)
{
    VALIDATE_NOT_NULL(count);
    *count = mChildNodeIds.GetSize();
    return NOERROR;
}

ECode CAccessibilityNodeInfo::GetChild(
    /* [in] */ Int32 index,
    /* [out] */ IAccessibilityNodeInfo** info)
{
    VALIDATE_NOT_NULL(info);
    *info = NULL;

    FAIL_RETURN(EnforceSealed());
    if (!CanPerformRequestOverConnection(mSourceNodeId)) {
        return NOERROR;
    }
    HashMap<Int32, Int64>::Iterator it = mChildNodeIds.Find(index);
    Int64 childId = 0;
    if (it != mChildNodeIds.End()) {
        childId = it->mSecond;
    }

    AutoPtr<IAccessibilityInteractionClient> client = CAccessibilityInteractionClient::GetInstance();
    return client->FindAccessibilityNodeInfoByAccessibilityId(mConnectionId, mWindowId,
        childId, FLAG_PREFETCH_DESCENDANTS, info);
}

ECode CAccessibilityNodeInfo::AddChild(
    /* [in] */ IView* child)
{
    return AddChild(child, UNDEFINED);
}

ECode CAccessibilityNodeInfo::AddChild(
    /* [in] */ IView* root,
    /* [in] */ Int32 virtualDescendantId)
{
    ASSERT_SUCCEEDED(EnforceNotSealed());
    Int32 index = mChildNodeIds.GetSize();
    Int32 rootAccessibilityViewId = UNDEFINED;
    if (root != NULL) {
        root->GetAccessibilityViewId(&rootAccessibilityViewId);
    }
    Int64 childNodeId = MakeNodeId(rootAccessibilityViewId, virtualDescendantId);
    mChildNodeIds[index] = childNodeId;

    return NOERROR;
}

ECode CAccessibilityNodeInfo::GetActions(
    /* [out] */ Int32* actions)
{
    VALIDATE_NOT_NULL(actions);
    *actions = mActions;
    return NOERROR;
}

ECode CAccessibilityNodeInfo::AddAction(
    /* [in] */ Int32 action)
{
    FAIL_RETURN(EnforceNotSealed());
    mActions |= action;
    return NOERROR;
}

ECode CAccessibilityNodeInfo::SetMovementGranularities(
    /* [in] */ Int32 granularities)
{
    FAIL_RETURN(EnforceNotSealed());
    mMovementGranularities = granularities;
    return NOERROR;
}

ECode CAccessibilityNodeInfo::GetMovementGranularities(
    /* [out] */ Int32* granularities)
{
    VALIDATE_NOT_NULL(granularities);
    *granularities = mMovementGranularities;
    return NOERROR;
}

ECode CAccessibilityNodeInfo::PerformAction(
    /* [in] */ Int32 action,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);

    FAIL_RETURN(EnforceSealed());
    if (!CanPerformRequestOverConnection(mSourceNodeId)) {
        *result = FALSE;
        return NOERROR;
    }

    AutoPtr<IAccessibilityInteractionClient> client = CAccessibilityInteractionClient::GetInstance();
    return client->PerformAccessibilityAction(mConnectionId, mWindowId, mSourceNodeId,
        action, NULL, result);
}

ECode CAccessibilityNodeInfo::PerformAction(
    /* [in] */ Int32 action,
    /* [in] */ IBundle* arguments,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);

    FAIL_RETURN(EnforceSealed());
    if (!CanPerformRequestOverConnection(mSourceNodeId)) {
        *result = FALSE;
        return NOERROR;
    }

    AutoPtr<IAccessibilityInteractionClient> client = CAccessibilityInteractionClient::GetInstance();
    return client->PerformAccessibilityAction(mConnectionId, mWindowId, mSourceNodeId,
        action, arguments, result);
}

ECode CAccessibilityNodeInfo::FindAccessibilityNodeInfosByText(
    /* [in] */ const String& text,
    /* [out] */ IObjectContainer** container)
{
    VALIDATE_NOT_NULL(container);

    FAIL_RETURN(EnforceSealed());
    if (!CanPerformRequestOverConnection(mSourceNodeId)) {
        return CObjectContainer::New(container);
    }

    AutoPtr<IAccessibilityInteractionClient> client = CAccessibilityInteractionClient::GetInstance();
    return client->FindAccessibilityNodeInfosByText(
        mConnectionId, mWindowId, mSourceNodeId, text, container);

}

ECode CAccessibilityNodeInfo::GetParent(
    /* [out] */ IAccessibilityNodeInfo** parent)
{
    VALIDATE_NOT_NULL(parent);

    FAIL_RETURN(EnforceSealed());
    if (!CanPerformRequestOverConnection(mParentNodeId)) {
        *parent = NULL;
        return NOERROR;
    }

    AutoPtr<IAccessibilityInteractionClient> client = CAccessibilityInteractionClient::GetInstance();
    return client->FindAccessibilityNodeInfoByAccessibilityId(
        mConnectionId, mWindowId, mParentNodeId,
        FLAG_PREFETCH_DESCENDANTS | FLAG_PREFETCH_SIBLINGS, parent);
}

ECode CAccessibilityNodeInfo::GetParentNodeId(
    /* [out] */ Int64* id)
{
    VALIDATE_NOT_NULL(id);
    *id = mParentNodeId;
    return NOERROR;
}

ECode CAccessibilityNodeInfo::SetParent(
    /* [in] */ IView* parent)
{
    return SetParent(parent, UNDEFINED);
}

ECode CAccessibilityNodeInfo::SetParent(
    /* [in] */ IView* root,
    /* [in] */ Int32 virtualDescendantId)
{
    FAIL_RETURN(EnforceNotSealed());
    Int32 rootAccessibilityViewId = UNDEFINED;
    if (root != NULL) {
        root->GetAccessibilityViewId(&rootAccessibilityViewId);
    }
    mParentNodeId = MakeNodeId(rootAccessibilityViewId, virtualDescendantId);
    return NOERROR;
}

ECode CAccessibilityNodeInfo::GetBoundsInParent(
    /* [in] */ IRect* outBounds)
{
    VALIDATE_NOT_NULL(outBounds);
    Int32 left, top, right, bottom;
    mBoundsInParent->GetLeft(&left);
    mBoundsInParent->GetTop(&top);
    mBoundsInParent->GetRight(&right);
    mBoundsInParent->GetBottom(&bottom);
    outBounds->Set(left, top, right, bottom);
    return NOERROR;
}

ECode CAccessibilityNodeInfo::SetBoundsInParent(
    /* [in] */ IRect* bounds)
{
    FAIL_RETURN(EnforceNotSealed());
    Int32 left, top, right, bottom;
    bounds->GetLeft(&left);
    bounds->GetTop(&top);
    bounds->GetRight(&right);
    bounds->GetBottom(&bottom);
    return mBoundsInParent->Set(left, top, right, bottom);
}

ECode CAccessibilityNodeInfo::GetBoundsInScreen(
    /* [in] */ IRect* outBounds)
{
    VALIDATE_NOT_NULL(outBounds);

    Int32 left, top, right, bottom;
    mBoundsInScreen->GetLeft(&left);
    mBoundsInScreen->GetTop(&top);
    mBoundsInScreen->GetRight(&right);
    mBoundsInScreen->GetBottom(&bottom);
    outBounds->Set(left, top, right, bottom);
    return NOERROR;
}

ECode CAccessibilityNodeInfo::SetBoundsInScreen(
    /* [in] */ IRect* bounds)
{
    FAIL_RETURN(EnforceNotSealed());
    Int32 left, top, right, bottom;
    bounds->GetLeft(&left);
    bounds->GetTop(&top);
    bounds->GetRight(&right);
    bounds->GetBottom(&bottom);
    return mBoundsInScreen->Set(left, top, right, bottom);
}

ECode CAccessibilityNodeInfo::IsCheckable(
    /* [out] */ Boolean* checkable)
{
    VALIDATE_NOT_NULL(checkable);
    *checkable = GetBooleanProperty(PROPERTY_CHECKABLE);
    return NOERROR;
}

ECode CAccessibilityNodeInfo::SetCheckable(
    /* [in] */ Boolean checkable)
{
    return SetBooleanProperty(PROPERTY_CHECKABLE, checkable);
}

ECode CAccessibilityNodeInfo::IsChecked(
    /* [out] */ Boolean* checked)
{
    VALIDATE_NOT_NULL(checked);
    *checked = GetBooleanProperty(PROPERTY_CHECKED);
    return NOERROR;
}

ECode CAccessibilityNodeInfo::SetChecked(
    /* [in] */ Boolean checked)
{
    return SetBooleanProperty(PROPERTY_CHECKED, checked);
}

ECode CAccessibilityNodeInfo::IsFocusable(
    /* [out] */ Boolean* focusable)
{
    VALIDATE_NOT_NULL(focusable);
    *focusable = GetBooleanProperty(PROPERTY_FOCUSABLE);
    return NOERROR;
}

ECode CAccessibilityNodeInfo::SetFocusable(
    /* [in] */ Boolean focusable)
{
    return SetBooleanProperty(PROPERTY_FOCUSABLE, focusable);
}

ECode CAccessibilityNodeInfo::IsFocused(
    /* [out] */ Boolean* focused)
{
    VALIDATE_NOT_NULL(focused);
    *focused = GetBooleanProperty(PROPERTY_FOCUSED);
    return NOERROR;
}

ECode CAccessibilityNodeInfo::SetFocused(
    /* [in] */ Boolean focused)
{
    return SetBooleanProperty(PROPERTY_FOCUSED, focused);
}

ECode CAccessibilityNodeInfo::IsVisibleToUser(
    /* [out] */ Boolean* visible)
{
    VALIDATE_NOT_NULL(visible);
    *visible = GetBooleanProperty(PROPERTY_VISIBLE_TO_USER);
    return NOERROR;
}

ECode CAccessibilityNodeInfo::SetVisibleToUser(
    /* [in] */ Boolean visible)
{
    return SetBooleanProperty(PROPERTY_VISIBLE_TO_USER, visible);
}

ECode CAccessibilityNodeInfo::IsAccessibilityFocused(
    /* [out] */ Boolean* focused)
{
    VALIDATE_NOT_NULL(focused);
    *focused = GetBooleanProperty(PROPERTY_ACCESSIBILITY_FOCUSED);
    return NOERROR;
}

ECode CAccessibilityNodeInfo::SetAccessibilityFocused(
    /* [in] */ Boolean focused)
{
    return SetBooleanProperty(PROPERTY_ACCESSIBILITY_FOCUSED, focused);
}

ECode CAccessibilityNodeInfo::IsSelected(
    /* [out] */ Boolean* selected)
{
    VALIDATE_NOT_NULL(selected);
    *selected = GetBooleanProperty(PROPERTY_SELECTED);
    return NOERROR;
}

ECode CAccessibilityNodeInfo::SetSelected(
    /* [in] */ Boolean selected)
{
    return SetBooleanProperty(PROPERTY_SELECTED, selected);
}

ECode CAccessibilityNodeInfo::IsClickable(
    /* [out] */ Boolean* clickable)
{
    VALIDATE_NOT_NULL(clickable);
    *clickable = GetBooleanProperty(PROPERTY_CLICKABLE);
    return NOERROR;
}

ECode CAccessibilityNodeInfo::SetClickable(
    /* [in] */ Boolean clickable)
{
    return SetBooleanProperty(PROPERTY_CLICKABLE, clickable);
}

ECode CAccessibilityNodeInfo::IsLongClickable(
    /* [out] */ Boolean* longClickable)
{
    VALIDATE_NOT_NULL(longClickable);
    *longClickable = GetBooleanProperty(PROPERTY_LONG_CLICKABLE);
    return NOERROR;
}

ECode CAccessibilityNodeInfo::SetLongClickable(
    /* [in] */ Boolean longClickable)
{
    return SetBooleanProperty(PROPERTY_LONG_CLICKABLE, longClickable);
}

ECode CAccessibilityNodeInfo::IsEnabled(
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
    *result = GetBooleanProperty(PROPERTY_ENABLED);
    return NOERROR;
}

ECode CAccessibilityNodeInfo::SetEnabled(
    /* [in] */ Boolean enabled)
{
    return SetBooleanProperty(PROPERTY_ENABLED, enabled);
}

ECode CAccessibilityNodeInfo::IsPassword(
    /* [out] */ Boolean* isPassword)
{
    VALIDATE_NOT_NULL(isPassword);
    *isPassword = GetBooleanProperty(PROPERTY_PASSWORD);
    return NOERROR;
}

ECode CAccessibilityNodeInfo::SetPassword(
    /* [in] */ Boolean isPassword)
{
    return SetBooleanProperty(PROPERTY_PASSWORD, isPassword);
}

ECode CAccessibilityNodeInfo::IsScrollable(
    /* [out] */ Boolean* scrollable)
{
    VALIDATE_NOT_NULL(scrollable);
    *scrollable = GetBooleanProperty(PROPERTY_SCROLLABLE);
    return NOERROR;
}

ECode CAccessibilityNodeInfo::SetScrollable(
    /* [in] */ Boolean scrollable)
{
    FAIL_RETURN(EnforceNotSealed());
    return SetBooleanProperty(PROPERTY_SCROLLABLE, scrollable);
}

ECode CAccessibilityNodeInfo::GetPackageName(
    /* [out] */ ICharSequence** packageName)
{
    VALIDATE_NOT_NULL(packageName);
    *packageName = mPackageName;
    REFCOUNT_ADD(*packageName);
    return NOERROR;
}

ECode CAccessibilityNodeInfo::SetPackageName(
    /* [in] */ ICharSequence* packageName)
{
    FAIL_RETURN(EnforceNotSealed());
    mPackageName = packageName;
    return NOERROR;
}

ECode CAccessibilityNodeInfo::GetClassName(
    /* [out] */ ICharSequence** className)
{
    VALIDATE_NOT_NULL(className);
    *className = mClassName;
    REFCOUNT_ADD(*className);
    return NOERROR;
}

ECode CAccessibilityNodeInfo::SetClassName(
    /* [in] */ ICharSequence* className)
{
    FAIL_RETURN(EnforceNotSealed());
    mClassName = className;
    return NOERROR;
}

ECode CAccessibilityNodeInfo::GetText(
    /* [out] */ ICharSequence** text)
{
    VALIDATE_NOT_NULL(text);
    *text = mText;
    REFCOUNT_ADD(*text);
    return NOERROR;
}

ECode CAccessibilityNodeInfo::SetText(
    /* [in] */ ICharSequence* text)
{
    FAIL_RETURN(EnforceNotSealed());
    mText = text;
    return NOERROR;
}

ECode CAccessibilityNodeInfo::GetContentDescription(
    /* [out] */ ICharSequence** description)
{
    VALIDATE_NOT_NULL(description);
    *description = mContentDescription;
    REFCOUNT_ADD(*description);
    return NOERROR;
}

ECode CAccessibilityNodeInfo::SetContentDescription(
    /* [in] */ ICharSequence* description)
{
    FAIL_RETURN(EnforceNotSealed());
    mContentDescription = description;
    return NOERROR;
}

ECode CAccessibilityNodeInfo::SetLabelFor(
    /* [in] */ IView* labeled)
{
    return SetLabelFor(labeled, UNDEFINED);
}

ECode CAccessibilityNodeInfo::SetLabelFor(
    /* [in] */ IView* root,
    /* [in] */ Int32 virtualDescendantId)
{
    FAIL_RETURN(EnforceNotSealed());
    Int32 rootAccessibilityViewId = UNDEFINED;
    if (root != NULL) {
        root->GetAccessibilityViewId(&rootAccessibilityViewId);
    }
    mLabelForId = MakeNodeId(rootAccessibilityViewId, virtualDescendantId);
    return NOERROR;
}

ECode CAccessibilityNodeInfo::GetLabelFor(
    /* [out] */ IAccessibilityNodeInfo** info)
{
    VALIDATE_NOT_NULL(info);
    FAIL_RETURN(EnforceSealed());
    if (!CanPerformRequestOverConnection(mLabelForId)) {
        *info = NULL;
        return NOERROR;
    }

    AutoPtr<IAccessibilityInteractionClient> client = CAccessibilityInteractionClient::GetInstance();
    return client->FindAccessibilityNodeInfoByAccessibilityId(
        mConnectionId, mWindowId, mLabelForId,
        FLAG_PREFETCH_DESCENDANTS | FLAG_PREFETCH_SIBLINGS, info);
}

ECode CAccessibilityNodeInfo::SetLabeledBy(
    /* [in] */ IView* label)
{
    return SetLabeledBy(label, UNDEFINED);
}

ECode CAccessibilityNodeInfo::SetLabeledBy(
    /* [in] */ IView* root,
    /* [in] */ Int32 virtualDescendantId)
{
    FAIL_RETURN(EnforceNotSealed());
    Int32 rootAccessibilityViewId = UNDEFINED;
    if (root != NULL) {
        root->GetAccessibilityViewId(&rootAccessibilityViewId);
    }
    mLabeledById = MakeNodeId(rootAccessibilityViewId, virtualDescendantId);
    return NOERROR;
}

ECode CAccessibilityNodeInfo::GetLabeledBy(
    /* [out] */ IAccessibilityNodeInfo** info)
{
    VALIDATE_NOT_NULL(info);
    *info = NULL;

    FAIL_RETURN(EnforceSealed());
    if (!CanPerformRequestOverConnection(mLabeledById)) {

        return NOERROR;
    }

    AutoPtr<IAccessibilityInteractionClient> client = CAccessibilityInteractionClient::GetInstance();
    return client->FindAccessibilityNodeInfoByAccessibilityId(
        mConnectionId, mWindowId, mLabeledById,
        FLAG_PREFETCH_DESCENDANTS | FLAG_PREFETCH_SIBLINGS, info);
}

Boolean CAccessibilityNodeInfo::GetBooleanProperty(
    /* [in] */ Int32 property)
{
    return (mBooleanProperties & property) != 0;
}

ECode CAccessibilityNodeInfo::SetBooleanProperty(
    /* [in] */ Int32 property,
    /* [in] */ Boolean value)
{
    FAIL_RETURN(EnforceNotSealed());
    if (value) {
        mBooleanProperties |= property;
    }
    else {
        mBooleanProperties &= ~property;
    }
    return NOERROR;
}

ECode CAccessibilityNodeInfo::SetConnectionId(
    /* [in] */ Int32 connectionId)
{
    FAIL_RETURN(EnforceNotSealed());
    mConnectionId = connectionId;
    return NOERROR;
}

ECode CAccessibilityNodeInfo::DescribeContents(
    /* [out] */ Int32* contents)
{
    VALIDATE_NOT_NULL(contents);
    *contents = 0;
    return NOERROR;
}

ECode CAccessibilityNodeInfo::GetSourceNodeId(
    /* [out] */ Int64* id)
{
    VALIDATE_NOT_NULL(id);
    *id = mSourceNodeId;
    return NOERROR;
}

ECode CAccessibilityNodeInfo::SetSealed(
    /* [in] */ Boolean sealed)
{
    mSealed = sealed;
    return NOERROR;
}

ECode CAccessibilityNodeInfo::IsSealed(
    /* [out] */ Boolean* sealed)
{
    VALIDATE_NOT_NULL(sealed);
    *sealed = mSealed;
    return NOERROR;
}

ECode CAccessibilityNodeInfo::EnforceSealed()
{
    Boolean sealed;
    if (IsSealed(&sealed), !sealed) {
        Slogger::E(TAG, "Cannot perform this action on a not sealed instance.");
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
        // throw new IllegalStateException("Cannot perform this "
        //         + "action on a not sealed instance.");
    }
    return NOERROR;
}

ECode CAccessibilityNodeInfo::EnforceValidFocusDirection(
    /* [in] */ Int32 direction)
{
    switch (direction) {
        case IView::FOCUS_DOWN:
        case IView::FOCUS_UP:
        case IView::FOCUS_LEFT:
        case IView::FOCUS_RIGHT:
        case IView::FOCUS_FORWARD:
        case IView::FOCUS_BACKWARD:
            return NOERROR;
        default:
            Slogger::E(TAG, "Unknown direction: %d", direction);
            return E_ILLEGAL_ARGUMENT_EXCEPTION;
            // throw new IllegalArgumentException("Unknown direction: " + direction);
    }
}

ECode CAccessibilityNodeInfo::EnforceValidFocusType(
    /* [in] */ Int32 focusType)
{
    switch (focusType) {
        case FOCUS_INPUT:
        case FOCUS_ACCESSIBILITY:
            return NOERROR;
        default:
            Slogger::E(TAG, "Unknown focus type: %d", focusType);
            return E_ILLEGAL_ARGUMENT_EXCEPTION;
            // throw new IllegalArgumentException("Unknown focus type: " + focusType);
    }
}

ECode CAccessibilityNodeInfo::EnforceNotSealed()
{
    Boolean sealed;
    if (IsSealed(&sealed), sealed) {
        Slogger::E(TAG, "Cannot perform this action on a sealed instance.");
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
        // throw new IllegalStateException("Cannot perform this "
        //         + "action on a sealed instance.");
    }
    return NOERROR;
}

ECode CAccessibilityNodeInfo::Obtain(
    /* [in] */ IView* source,
    /* [out] */ IAccessibilityNodeInfo** info)
{
    VALIDATE_NOT_NULL(info);
    FAIL_RETURN(Obtain(info));
    return (*info)->SetSource(source);
}

ECode CAccessibilityNodeInfo::Obtain(
    /* [in] */ IView* root,
    /* [in] */ Int32 virtualDescendantId,
    /* [out] */ IAccessibilityNodeInfo** info)
{
    VALIDATE_NOT_NULL(info);
    FAIL_RETURN(Obtain(info));
    return (*info)->SetSource(root, virtualDescendantId);
}

ECode CAccessibilityNodeInfo::Obtain(
    /* [out] */ IAccessibilityNodeInfo** info)
{
    VALIDATE_NOT_NULL(info);

    AutoLock lock(sPoolLock);
    if (sPool != NULL) {
        AutoPtr<CAccessibilityNodeInfo> infoCls = sPool;
        sPool = sPool->mNext;
        sPoolSize--;
        infoCls->mNext = NULL;
        infoCls->mIsInPool = FALSE;
        *info = (IAccessibilityNodeInfo*)infoCls;
        REFCOUNT_ADD(*info);
        return NOERROR;
    }
    AutoPtr<CAccessibilityNodeInfo> cinfo;
    CAccessibilityNodeInfo::NewByFriend((CAccessibilityNodeInfo**)&cinfo);
    *info = cinfo;
    REFCOUNT_ADD(*info);
    return NOERROR;
}

ECode CAccessibilityNodeInfo::Obtain(
    /* [in] */ IAccessibilityNodeInfo* info,
    /* [out] */ IAccessibilityNodeInfo** infoClone)
{
    VALIDATE_NOT_NULL(infoClone);
    FAIL_RETURN(Obtain(infoClone));
    ((CAccessibilityNodeInfo*)(*infoClone))->Init((CAccessibilityNodeInfo*)info);
    return NOERROR;
}

ECode CAccessibilityNodeInfo::Recycle()
{
    if (mIsInPool) {
        Slogger::E(TAG, "Info already recycled!");
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
        // throw new IllegalStateException("Info already recycled!");
    }
    Clear();
    AutoLock lock(sPoolLock);
    if (sPoolSize <= MAX_POOL_SIZE) {
        mNext = sPool;
        sPool = this;
        mIsInPool = TRUE;
        sPoolSize++;
    }
    return NOERROR;
}

ECode CAccessibilityNodeInfo::WriteToParcel(
    /* [in] */ IParcel* dest)
{
    Boolean sealed;
    IsSealed(&sealed);
    dest->WriteBoolean(sealed);
    dest->WriteInt64(mSourceNodeId);
    dest->WriteInt32(mWindowId);
    dest->WriteInt64(mParentNodeId);
    dest->WriteInt64(mLabelForId);
    dest->WriteInt64(mLabeledById);
    dest->WriteInt32(mConnectionId);

    HashMap<Int32, Int64> childIds = mChildNodeIds;
    Int32 childIdsSize = childIds.GetSize();
    dest->WriteInt32(childIdsSize);
    HashMap<Int32, Int64>::Iterator it = childIds.Begin();
    for (; it != childIds.End(); ++it) {
        dest->WriteInt64(it->mSecond);
    }

    AutoPtr<CRect> inParentCls = (CRect*)mBoundsInParent.Get();
    AutoPtr<CRect> inScreenCls = (CRect*)mBoundsInScreen.Get();
    dest->WriteInt32(inParentCls->mTop);
    dest->WriteInt32(inParentCls->mBottom);
    dest->WriteInt32(inParentCls->mLeft);
    dest->WriteInt32(inParentCls->mRight);

    dest->WriteInt32(inScreenCls->mTop);
    dest->WriteInt32(inScreenCls->mBottom);
    dest->WriteInt32(inScreenCls->mLeft);
    dest->WriteInt32(inScreenCls->mRight);

    dest->WriteInt32(mActions);

    dest->WriteInt32(mMovementGranularities);

    dest->WriteInt32(mBooleanProperties);

    dest->WriteInterfacePtr((IInterface*)mPackageName);
    dest->WriteInterfacePtr((IInterface*)mClassName);
    dest->WriteInterfacePtr((IInterface*)mText);
    dest->WriteInterfacePtr((IInterface*)mContentDescription);

    // Since instances of this class are fetched via synchronous i.e. blocking
    // calls in IPCs we always recycle as soon as the instance is marshaled.
    return Recycle();
}

ECode CAccessibilityNodeInfo::ReadFromParcel(
    /* [in] */ IParcel* source)
{
    AutoPtr<IAccessibilityNodeInfo> info;
    Obtain((IAccessibilityNodeInfo**)&info);
    ((CAccessibilityNodeInfo*)info.Get())->InitFromParcel(source);
    return NOERROR;
}

void CAccessibilityNodeInfo::Init(
    /* [in] */ CAccessibilityNodeInfo* other)
{
    mSealed = other->mSealed;
    mSourceNodeId = other->mSourceNodeId;
    mParentNodeId = other->mParentNodeId;
    mLabelForId = other->mLabelForId;
    mLabeledById = other->mLabeledById;
    mWindowId = other->mWindowId;
    mConnectionId = other->mConnectionId;
    mBoundsInParent->Set(other->mBoundsInParent);
    mBoundsInScreen->Set(other->mBoundsInScreen);
    mPackageName = other->mPackageName;
    mClassName = other->mClassName;
    mText = other->mText;
    mContentDescription = other->mContentDescription;
    mActions= other->mActions;
    mBooleanProperties = other->mBooleanProperties;
    mMovementGranularities = other->mMovementGranularities;
    HashMap<Int32, Int64>::Iterator it = other->mChildNodeIds.Begin();
    for (Int32 i = 0; it != other->mChildNodeIds.End(); ++it, ++i) {
        mChildNodeIds[i] = it->mSecond;
    }
}

void CAccessibilityNodeInfo::InitFromParcel(
    /* [in] */ IParcel* parcel)
{
    parcel->ReadBoolean(&mSealed);
    parcel->ReadInt64(&mSourceNodeId);
    parcel->ReadInt32(&mWindowId);
    parcel->ReadInt64(&mParentNodeId);
    parcel->ReadInt64(&mLabelForId);
    parcel->ReadInt64(&mLabeledById);
    parcel->ReadInt32(&mConnectionId);

    Int32 childrenSize;
    parcel->ReadInt32(&childrenSize);
    for (Int32 i = 0; i < childrenSize; i++) {
        Int64 childId;
        parcel->ReadInt64(&childId);
        mChildNodeIds[i] = childId;
    }

    AutoPtr<CRect> inParentCls = (CRect*)mBoundsInParent.Get();
    AutoPtr<CRect> inScreenCls = (CRect*)mBoundsInScreen.Get();
    parcel->ReadInt32(&inParentCls->mTop);
    parcel->ReadInt32(&inParentCls->mBottom);
    parcel->ReadInt32(&inParentCls->mLeft);
    parcel->ReadInt32(&inParentCls->mRight);

    parcel->ReadInt32(&inScreenCls->mTop);
    parcel->ReadInt32(&inScreenCls->mBottom);
    parcel->ReadInt32(&inScreenCls->mLeft);
    parcel->ReadInt32(&inScreenCls->mRight);

    parcel->ReadInt32(&mActions);

    parcel->ReadInt32(&mMovementGranularities);

    parcel->ReadInt32(&mBooleanProperties);

    AutoPtr<IInterface> obj;
    parcel->ReadInterfacePtr((Handle32*)&obj);
    mPackageName = ICharSequence::Probe(obj);
    parcel->ReadInterfacePtr((Handle32*)&obj);
    mClassName = ICharSequence::Probe(obj);
    parcel->ReadInterfacePtr((Handle32*)&obj);
    mText = ICharSequence::Probe(obj);
    parcel->ReadInterfacePtr((Handle32*)&obj);
    mContentDescription = ICharSequence::Probe(obj);
}

void CAccessibilityNodeInfo::Clear()
{
    mSealed = FALSE;
    mSourceNodeId = ROOT_NODE_ID;
    mParentNodeId = ROOT_NODE_ID;
    mLabelForId = ROOT_NODE_ID;
    mLabeledById = ROOT_NODE_ID;
    mWindowId = UNDEFINED;
    mConnectionId = UNDEFINED;
    mMovementGranularities = 0;
    mChildNodeIds.Clear();
    mBoundsInParent->Set(0, 0, 0, 0);
    mBoundsInScreen->Set(0, 0, 0, 0);
    mBooleanProperties = 0;
    mPackageName = NULL;
    mClassName = NULL;
    mText = NULL;
    mContentDescription = NULL;
    mActions = 0;
}

String CAccessibilityNodeInfo::GetActionSymbolicName(
    /* [in] */ Int32 action)
{
    switch (action) {
        case ACTION_FOCUS:
            return String("ACTION_FOCUS");
        case ACTION_CLEAR_FOCUS:
            return String("ACTION_CLEAR_FOCUS");
        case ACTION_SELECT:
            return String("ACTION_SELECT");
        case ACTION_CLEAR_SELECTION:
            return String("ACTION_CLEAR_SELECTION");
        case ACTION_CLICK:
            return String("ACTION_CLICK");
        case ACTION_LONG_CLICK:
            return String("ACTION_LONG_CLICK");
        case ACTION_ACCESSIBILITY_FOCUS:
            return String("ACTION_ACCESSIBILITY_FOCUS");
        case ACTION_CLEAR_ACCESSIBILITY_FOCUS:
            return String("ACTION_CLEAR_ACCESSIBILITY_FOCUS");
        case ACTION_NEXT_AT_MOVEMENT_GRANULARITY:
            return String("ACTION_NEXT_AT_MOVEMENT_GRANULARITY");
        case ACTION_PREVIOUS_AT_MOVEMENT_GRANULARITY:
            return String("ACTION_PREVIOUS_AT_MOVEMENT_GRANULARITY");
        case ACTION_NEXT_HTML_ELEMENT:
            return String("ACTION_NEXT_HTML_ELEMENT");
        case ACTION_PREVIOUS_HTML_ELEMENT:
            return String("ACTION_PREVIOUS_HTML_ELEMENT");
        case ACTION_SCROLL_FORWARD:
            return String("ACTION_SCROLL_FORWARD");
        case ACTION_SCROLL_BACKWARD:
            return String("ACTION_SCROLL_BACKWARD");
        default:
            Slogger::E(TAG, "Unknown action: %d", action);
            return String(NULL);
            // throw new IllegalArgumentException("Unknown action: " + action);
    }
}

String CAccessibilityNodeInfo::GetMovementGranularitySymbolicName(
    /* [in] */ Int32 granularity)
{
    switch (granularity) {
        case MOVEMENT_GRANULARITY_CHARACTER:
            return String("MOVEMENT_GRANULARITY_CHARACTER");
        case MOVEMENT_GRANULARITY_WORD:
            return String("MOVEMENT_GRANULARITY_WORD");
        case MOVEMENT_GRANULARITY_LINE:
            return String("MOVEMENT_GRANULARITY_LINE");
        case MOVEMENT_GRANULARITY_PARAGRAPH:
            return String("MOVEMENT_GRANULARITY_PARAGRAPH");
        case MOVEMENT_GRANULARITY_PAGE:
            return String("MOVEMENT_GRANULARITY_PAGE");
        default:
            Slogger::E(TAG, "Unknown movement granularity: %d", granularity);
            return String(NULL);
            // throw new IllegalArgumentException("Unknown movement granularity: " + granularity);
    }
}

Boolean CAccessibilityNodeInfo::CanPerformRequestOverConnection(
    /* [in] */ Int64 accessibilityNodeId)
{
    return (mWindowId != UNDEFINED
            && GetAccessibilityViewId(accessibilityNodeId) != UNDEFINED
            && mConnectionId != UNDEFINED);
}

ECode CAccessibilityNodeInfo::Equals(
    /* [in] */ IInterface* other,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
    *result = FALSE;

    IAccessibilityNodeInfo* info = IAccessibilityNodeInfo::Probe(other);
    if (info == NULL)
        return NOERROR;

    CAccessibilityNodeInfo* node = (CAccessibilityNodeInfo*)info;
    if (node == this) {
        *result = TRUE;
        return NOERROR;
    }

    if (mSourceNodeId != node->mSourceNodeId) {
        return NOERROR;
    }
    if (mWindowId != node->mWindowId) {
        return NOERROR;
    }

    *result = TRUE;
    return NOERROR;
}

ECode CAccessibilityNodeInfo::GetHashCode(
    /* [out] */ Int32* hash)
{
    VALIDATE_NOT_NULL(hash);

    const Int32 prime = 31;
    Int32 result = 1;
    result = prime * result + GetAccessibilityViewId(mSourceNodeId);
    result = prime * result + GetVirtualDescendantId(mSourceNodeId);
    result = prime * result + mWindowId;
    *hash = result;
    return NOERROR;
}

ECode CAccessibilityNodeInfo::ToString(
    /* [out] */ String* s)
{
    VALIDATE_NOT_NULL(s);

    // StringBuilder builder = new StringBuilder();
    // builder.append(super.toString());

    // if (DEBUG) {
    //     builder.append("; accessibilityViewId: " + getAccessibilityViewId(mSourceNodeId));
    //     builder.append("; virtualDescendantId: " + getVirtualDescendantId(mSourceNodeId));
    //     builder.append("; mParentNodeId: " + mParentNodeId);

    //     int granularities = mMovementGranularities;
    //     builder.append("; MovementGranularities: [");
    //     while (granularities != 0) {
    //         final int granularity = 1 << Integer.numberOfTrailingZeros(granularities);
    //         granularities &= ~granularity;
    //         builder.append(getMovementGranularitySymbolicName(granularity));
    //         if (granularities != 0) {
    //             builder.append(", ");
    //         }
    //     }
    //     builder.append("]");

    //     SparseLongArray childIds = mChildNodeIds;
    //     builder.append("; childAccessibilityIds: [");
    //     for (int i = 0, count = childIds.size(); i < count; i++) {
    //         builder.append(childIds.valueAt(i));
    //         if (i < count - 1) {
    //             builder.append(", ");
    //         }
    //     }
    //     builder.append("]");
    // }

    // builder.append("; boundsInParent: " + mBoundsInParent);
    // builder.append("; boundsInScreen: " + mBoundsInScreen);

    // builder.append("; packageName: ").append(mPackageName);
    // builder.append("; className: ").append(mClassName);
    // builder.append("; text: ").append(mText);
    // builder.append("; contentDescription: ").append(mContentDescription);

    // builder.append("; checkable: ").append(isCheckable());
    // builder.append("; checked: ").append(isChecked());
    // builder.append("; focusable: ").append(isFocusable());
    // builder.append("; focused: ").append(isFocused());
    // builder.append("; selected: ").append(isSelected());
    // builder.append("; clickable: ").append(isClickable());
    // builder.append("; longClickable: ").append(isLongClickable());
    // builder.append("; enabled: ").append(isEnabled());
    // builder.append("; password: ").append(isPassword());
    // builder.append("; scrollable: " + isScrollable());

    // builder.append("; [");
    // for (int actionBits = mActions; actionBits != 0;) {
    //     final int action = 1 << Integer.numberOfTrailingZeros(actionBits);
    //     actionBits &= ~action;
    //     builder.append(getActionSymbolicName(action));
    //     if (actionBits != 0) {
    //         builder.append(", ");
    //     }
    // }
    // builder.append("]");

    // return builder.toString();
    assert(0);
    return E_NOT_IMPLEMENTED;
}


} // Accessibility
} // View
} // Droid
} // Elastos
