
#include "ext/frameworkdef.h"
#include "view/accessibility/AccessibilityRecord.h"
#include "view/accessibility/CAccessibilityNodeInfo.h"
#include "view/accessibility/CAccessibilityInteractionClient.h"
#include <elastos/utility/logging/Slogger.h>
#include <elastos/core/StringBuilder.h>
#include <elastos/core/StringUtils.h>

using Elastos::Core::StringUtils;
using Elastos::Core::CObjectContainer;
using Elastos::Utility::Logging::Slogger;
using Elastos::Core::StringBuilder;

namespace Elastos {
namespace Droid {
namespace View {
namespace Accessibility {

// {657EA1A2-91E4-4414-9514-CE42594D9BC5}
extern const InterfaceID EIID_AccessibilityRecord =
        { 0x657ea1a2, 0x91e4, 0x4414, { 0x95, 0x14, 0xce, 0x42, 0x59, 0x4d, 0x9b, 0xc5 } };

const String AccessibilityRecord::TAG("AccessibilityRecord");
const Int32 AccessibilityRecord::UNDEFINED = -1;
const Int32 AccessibilityRecord::PROPERTY_CHECKED = 0x00000001;
const Int32 AccessibilityRecord::PROPERTY_ENABLED = 0x00000002;
const Int32 AccessibilityRecord::PROPERTY_PASSWORD = 0x00000004;
const Int32 AccessibilityRecord::PROPERTY_FULL_SCREEN = 0x00000080;
const Int32 AccessibilityRecord::PROPERTY_SCROLLABLE = 0x00000100;
const Int32 AccessibilityRecord::PROPERTY_IMPORTANT_FOR_ACCESSIBILITY = 0x00000200;
const Int32 AccessibilityRecord::GET_SOURCE_PREFETCH_FLAGS =
        IAccessibilityNodeInfo::FLAG_PREFETCH_PREDECESSORS
        | IAccessibilityNodeInfo::FLAG_PREFETCH_SIBLINGS
        | IAccessibilityNodeInfo::FLAG_PREFETCH_DESCENDANTS;
const Int32 AccessibilityRecord::MAX_POOL_SIZE = 10;

AccessibilityRecord* AccessibilityRecord::sPool = NULL;
Mutex AccessibilityRecord::sPoolLock;
Int32 AccessibilityRecord::sPoolSize = 0;

AccessibilityRecord::AccessibilityRecord()
    : mIsInPool(FALSE)
    , mSealed(FALSE)
    , mBooleanProperties(PROPERTY_IMPORTANT_FOR_ACCESSIBILITY)
    , mCurrentItemIndex(UNDEFINED)
    , mItemCount(UNDEFINED)
    , mFromIndex(UNDEFINED)
    , mToIndex(UNDEFINED)
    , mScrollX(UNDEFINED)
    , mScrollY(UNDEFINED)
    , mMaxScrollX(UNDEFINED)
    , mMaxScrollY(UNDEFINED)
    , mAddedCount(UNDEFINED)
    , mRemovedCount(UNDEFINED)
    , mSourceNodeId(UNDEFINED)
    , mSourceWindowId(UNDEFINED)
    , mConnectionId(UNDEFINED)
{
    mSourceNodeId = CAccessibilityNodeInfo::MakeNodeId(UNDEFINED, UNDEFINED);
}

ECode AccessibilityRecord::SetSource(
    /* [in] */ IView* source)
{
    return SetSource(source, UNDEFINED);
}

ECode AccessibilityRecord::SetSource(
    /* [in] */ IView* root,
    /* [in] */ Int32 virtualDescendantId)
{
    FAIL_RETURN(EnforceNotSealed());
    Boolean important = TRUE;
    if (virtualDescendantId == UNDEFINED) {
        if (root != NULL) {
            root->IsImportantForAccessibility(&important);
        }
    }
    SetBooleanProperty(PROPERTY_IMPORTANT_FOR_ACCESSIBILITY, important);
    mSourceWindowId = UNDEFINED;
    if (root != NULL) {
        root->GetAccessibilityWindowId(&mSourceWindowId);
    }
    Int32 rootViewId = UNDEFINED;
    if (root != NULL) {
        root->GetAccessibilityViewId(&rootViewId);
    }
    mSourceNodeId = CAccessibilityNodeInfo::MakeNodeId(rootViewId, virtualDescendantId);

    return NOERROR;
}

ECode AccessibilityRecord::GetSource(
    /* [out] */ IAccessibilityNodeInfo** info)
{
    VALIDATE_NOT_NULL(info);
    *info = NULL;

    FAIL_RETURN(EnforceSealed());
    if (mConnectionId == UNDEFINED || mSourceWindowId == UNDEFINED
            || CAccessibilityNodeInfo::GetAccessibilityViewId(mSourceNodeId) == UNDEFINED) {
        return NOERROR;
    }

    AutoPtr<IAccessibilityInteractionClient> client = CAccessibilityInteractionClient::GetInstance();
    return client->FindAccessibilityNodeInfoByAccessibilityId(
        mConnectionId, mSourceWindowId,
        mSourceNodeId, GET_SOURCE_PREFETCH_FLAGS, info);
}

ECode AccessibilityRecord::SetWindowId(
    /* [in] */ Int32 windowId)
{
    mSourceWindowId = windowId;
    return NOERROR;
}

ECode AccessibilityRecord::GetWindowId(
    /* [out] */ Int32* windowId)
{
    VALIDATE_NOT_NULL(windowId);
    *windowId = mSourceWindowId;
    return NOERROR;
}

ECode AccessibilityRecord::IsChecked(
    /* [out] */ Boolean* checked)
{
    VALIDATE_NOT_NULL(checked);
    *checked = GetBooleanProperty(PROPERTY_CHECKED);
    return NOERROR;
}

ECode AccessibilityRecord::SetChecked(
    /* [in] */ Boolean isChecked)
{
    FAIL_RETURN(EnforceNotSealed());
    SetBooleanProperty(PROPERTY_CHECKED, isChecked);
    return NOERROR;
}

ECode AccessibilityRecord::IsEnabled(
    /* [out] */ Boolean* enabled)
{
    VALIDATE_NOT_NULL(enabled);
    *enabled = GetBooleanProperty(PROPERTY_ENABLED);
    return NOERROR;
}

ECode AccessibilityRecord::SetEnabled(
    /* [in] */ Boolean enabled)
{
    FAIL_RETURN(EnforceNotSealed());
    SetBooleanProperty(PROPERTY_ENABLED, enabled);
    return NOERROR;
}

ECode AccessibilityRecord::IsPassword(
    /* [out] */ Boolean* password)
{
    VALIDATE_NOT_NULL(password);
    *password = GetBooleanProperty(PROPERTY_PASSWORD);
    return NOERROR;
}

ECode AccessibilityRecord::SetPassword(
    /* [in] */ Boolean password)
{
    FAIL_RETURN(EnforceNotSealed());
    SetBooleanProperty(PROPERTY_PASSWORD, password);
    return NOERROR;
}

ECode AccessibilityRecord::IsFullScreen(
    /* [out] */ Boolean* isFull)
{
    VALIDATE_NOT_NULL(isFull);
    *isFull = GetBooleanProperty(PROPERTY_FULL_SCREEN);
    return NOERROR;
}

ECode AccessibilityRecord::SetFullScreen(
    /* [in] */ Boolean full)
{
    FAIL_RETURN(EnforceNotSealed());
    SetBooleanProperty(PROPERTY_FULL_SCREEN, full);
    return NOERROR;
}

ECode AccessibilityRecord::IsScrollable(
    /* [out] */ Boolean* scrollable)
{
    VALIDATE_NOT_NULL(scrollable);
    *scrollable = GetBooleanProperty(PROPERTY_SCROLLABLE);
    return NOERROR;
}

ECode AccessibilityRecord::SetScrollable(
    /* [in] */ Boolean scrollable)
{
    FAIL_RETURN(EnforceNotSealed());
    SetBooleanProperty(PROPERTY_SCROLLABLE, scrollable);
    return NOERROR;
}

ECode AccessibilityRecord::IsImportantForAccessibility(
    /* [out] */ Boolean* important)
{
    VALIDATE_NOT_NULL(important);
    *important = GetBooleanProperty(PROPERTY_IMPORTANT_FOR_ACCESSIBILITY);
    return NOERROR;
}

ECode AccessibilityRecord::GetItemCount(
    /* [out] */ Int32* count)
{
    VALIDATE_NOT_NULL(count);
    *count = mItemCount;
    return NOERROR;
}

ECode AccessibilityRecord::SetItemCount(
    /* [in] */ Int32 count)
{
    FAIL_RETURN(EnforceNotSealed());
    mItemCount = count;
    return NOERROR;
}

ECode AccessibilityRecord::GetCurrentItemIndex(
    /* [out] */ Int32* index)
{
    VALIDATE_NOT_NULL(index);
    *index = mCurrentItemIndex;
    return NOERROR;
}

ECode AccessibilityRecord::SetCurrentItemIndex(
    /* [in] */ Int32 index)
{
    FAIL_RETURN(EnforceNotSealed());
    mCurrentItemIndex = index;
    return NOERROR;
}

ECode AccessibilityRecord::GetFromIndex(
    /* [out] */ Int32* index)
{
    VALIDATE_NOT_NULL(index);
    *index = mFromIndex;
    return NOERROR;
}

ECode AccessibilityRecord::SetFromIndex(
    /* [in] */ Int32 index)
{
    FAIL_RETURN(EnforceNotSealed());
    mFromIndex = index;
    return NOERROR;
}

ECode AccessibilityRecord::GetToIndex(
    /* [out] */ Int32* index)
{
    VALIDATE_NOT_NULL(index);
    *index = mToIndex;
    return NOERROR;
}

ECode AccessibilityRecord::SetToIndex(
    /* [in] */ Int32 index)
{
    FAIL_RETURN(EnforceNotSealed());
    mToIndex = index;
    return NOERROR;
}

ECode AccessibilityRecord::GetScrollX(
    /* [out] */ Int32* x)
{
    VALIDATE_NOT_NULL(x);
    *x = mScrollX;
    return NOERROR;
}

ECode AccessibilityRecord::SetScrollX(
    /* [in] */ Int32 x)
{
    FAIL_RETURN(EnforceNotSealed());
    mScrollX = x;
    return NOERROR;
}

ECode AccessibilityRecord::GetScrollY(
    /* [out] */ Int32* y)
{
    VALIDATE_NOT_NULL(y);
    *y = mScrollY;
    return NOERROR;
}

ECode AccessibilityRecord::SetScrollY(
    /* [in] */ Int32 y)
{
    FAIL_RETURN(EnforceNotSealed());
    mScrollY = y;
    return NOERROR;
}

ECode AccessibilityRecord::GetMaxScrollX(
    /* [out] */ Int32* x)
{
    VALIDATE_NOT_NULL(x);
    *x = mMaxScrollX;
    return NOERROR;
}

ECode AccessibilityRecord::SetMaxScrollX(
    /* [in] */ Int32 x)
{
    FAIL_RETURN(EnforceNotSealed());
    mMaxScrollX = x;
    return NOERROR;
}

ECode AccessibilityRecord::GetMaxScrollY(
    /* [out] */ Int32* y)
{
    VALIDATE_NOT_NULL(y);
    *y = mMaxScrollY;
    return NOERROR;
}

ECode AccessibilityRecord::SetMaxScrollY(
    /* [in] */ Int32 y)
{
    FAIL_RETURN(EnforceNotSealed());
    mMaxScrollY = y;
    return NOERROR;
}

ECode AccessibilityRecord::GetAddedCount(
    /* [out] */ Int32* count)
{
    VALIDATE_NOT_NULL(count);
    *count = mAddedCount;
    return NOERROR;
}

ECode AccessibilityRecord::SetAddedCount(
    /* [in] */ Int32 count)
{
    FAIL_RETURN(EnforceNotSealed());
    mAddedCount = count;
    return NOERROR;
}

ECode AccessibilityRecord::GetRemovedCount(
    /* [out] */ Int32* count)
{
    VALIDATE_NOT_NULL(count);
    *count = mRemovedCount;
    return NOERROR;
}

ECode AccessibilityRecord::SetRemovedCount(
    /* [in] */ Int32 count)
{
    FAIL_RETURN(EnforceNotSealed());
    mRemovedCount = count;
    return NOERROR;
}

ECode AccessibilityRecord::GetClassName(
    /* [out] */ ICharSequence** name)
{
    VALIDATE_NOT_NULL(name);
    *name = mClassName;
    REFCOUNT_ADD(*name);
    return NOERROR;
}

ECode AccessibilityRecord::SetClassName(
    /* [in] */ ICharSequence* name)
{
    FAIL_RETURN(EnforceNotSealed());
    mClassName = name;
    return NOERROR;
}

ECode AccessibilityRecord::GetText(
    /* [out] */ IObjectContainer** container)
{
    VALIDATE_NOT_NULL(container);
    AutoPtr<IObjectContainer> text;
    CObjectContainer::New((IObjectContainer**)&text);
    List<AutoPtr<ICharSequence> >::Iterator it = mText.Begin();
    for (; it != mText.End(); ++it) {
        text->Add((IInterface*)(*it));
    }
    *container = text;
    REFCOUNT_ADD(*container);
    return NOERROR;
}

ECode AccessibilityRecord::GetBeforeText(
    /* [out] */ ICharSequence** text)
{
    VALIDATE_NOT_NULL(text);
    *text = mBeforeText;
    REFCOUNT_ADD(*text);
    return NOERROR;
}

ECode AccessibilityRecord::SetBeforeText(
    /* [in] */ ICharSequence* text)
{
    FAIL_RETURN(EnforceNotSealed());
    mBeforeText = text;
    return NOERROR;
}

ECode AccessibilityRecord::GetContentDescription(
    /* [out] */ ICharSequence** contentDescription)
{
    VALIDATE_NOT_NULL(contentDescription);
    *contentDescription = mContentDescription;
    REFCOUNT_ADD(*contentDescription);
    return NOERROR;
}

ECode AccessibilityRecord::SetContentDescription(
    /* [in] */ ICharSequence* contentDescription)
{
    FAIL_RETURN(EnforceNotSealed());
    mContentDescription = contentDescription;
    return NOERROR;
}

ECode AccessibilityRecord::GetParcelableData(
    /* [out] */ IParcelable** parcelableData)
{
    VALIDATE_NOT_NULL(parcelableData);
    *parcelableData = mParcelableData;
    REFCOUNT_ADD(*parcelableData);
    return NOERROR;
}

ECode AccessibilityRecord::SetParcelableData(
    /* [in] */ IParcelable* parcelableData)
{
    FAIL_RETURN(EnforceNotSealed());
    mParcelableData = parcelableData;
    return NOERROR;
}

ECode AccessibilityRecord::GetSourceNodeId(
    /* [out] */ Int64* nodeId)
{
    VALIDATE_NOT_NULL(nodeId);
    *nodeId = mSourceNodeId;
    return NOERROR;
}

ECode AccessibilityRecord::SetConnectionId(
    /* [in] */ Int32 connectionId)
{
    FAIL_RETURN(EnforceNotSealed());
    mConnectionId = connectionId;
    return NOERROR;
}

ECode AccessibilityRecord::SetSealed(
    /* [in] */ Boolean sealed)
{
    FAIL_RETURN(EnforceNotSealed());
    mSealed = sealed;
    return NOERROR;
}

Boolean AccessibilityRecord::IsSealed()
{
    return mSealed;
}

ECode AccessibilityRecord::EnforceSealed()
{
    if (!IsSealed()) {
        Slogger::E(TAG, "Cannot perform this action on a not sealed instance.");
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
        // throw new IllegalStateException("Cannot perform this "
        //         + "action on a not sealed instance.");
    }
    return NOERROR;
}

ECode AccessibilityRecord::EnforceNotSealed()
{
    if (IsSealed()) {
        Slogger::E(TAG, "Cannot perform this action on a sealed instance.");
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
        // throw new IllegalStateException("Cannot perform this "
        //         + "action on a sealed instance.");
    }
    return NOERROR;
}

Boolean AccessibilityRecord::GetBooleanProperty(
    /* [in] */ Int32 property)
{
    return (mBooleanProperties & property) == property;
}

void AccessibilityRecord::SetBooleanProperty(
    /* [in] */ Int32 property,
    /* [in] */ Boolean value)
{
    if (value) {
        mBooleanProperties |= property;
    }
    else {
        mBooleanProperties &= ~property;
    }
}

ECode AccessibilityRecord::Recycle()
{
    if (mIsInPool) {
        Slogger::E(TAG, "Record already recycled!");
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
        // throw new IllegalStateException("Record already recycled!");
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

void AccessibilityRecord::Init(
    /* [in] */ AccessibilityRecord* record)
{
    mSealed = record->mSealed;
    mBooleanProperties = record->mBooleanProperties;
    mCurrentItemIndex = record->mCurrentItemIndex;
    mItemCount = record->mItemCount;
    mFromIndex = record->mFromIndex;
    mToIndex = record->mToIndex;
    mScrollX = record->mScrollX;
    mScrollY = record->mScrollY;
    mMaxScrollX = record->mMaxScrollX;
    mMaxScrollY = record->mMaxScrollY;
    mAddedCount = record->mAddedCount;
    mRemovedCount = record->mRemovedCount;
    mClassName = record->mClassName;
    mContentDescription = record->mContentDescription;
    mBeforeText = record->mBeforeText;
    mParcelableData = record->mParcelableData;
    mText = record->mText;
    mSourceWindowId = record->mSourceWindowId;
    mSourceNodeId = record->mSourceNodeId;
    mConnectionId = record->mConnectionId;
}

void AccessibilityRecord::Clear()
{
    mSealed = FALSE;
    mBooleanProperties = PROPERTY_IMPORTANT_FOR_ACCESSIBILITY;
    mCurrentItemIndex = UNDEFINED;
    mItemCount = UNDEFINED;
    mFromIndex = UNDEFINED;
    mToIndex = UNDEFINED;
    mScrollX = UNDEFINED;
    mScrollY = UNDEFINED;
    mMaxScrollX = UNDEFINED;
    mMaxScrollY = UNDEFINED;
    mAddedCount = UNDEFINED;
    mRemovedCount = UNDEFINED;
    mClassName = NULL;
    mContentDescription = NULL;
    mBeforeText = NULL;
    mParcelableData = NULL;
    mText.Clear();
    mSourceNodeId = CAccessibilityNodeInfo::MakeNodeId(UNDEFINED, UNDEFINED);
    mSourceWindowId = UNDEFINED;
    mConnectionId = UNDEFINED;
}

String AccessibilityRecord::ToString()
{
    StringBuilder builder;
    String className, contentDescription;
    mClassName->ToString(&className);
    mContentDescription->ToString(&contentDescription);
    builder.AppendString(String(" [ ClassName: ") + className);
    // builder.AppendString(String("; Text: ") + mText);
    builder.AppendString(String("; ContentDescription: ") + contentDescription);
    builder.AppendString(String("; ItemCount: ") + StringUtils::Int32ToString(mItemCount));
    builder.AppendString(String("; CurrentItemIndex: ")
            + StringUtils::Int32ToString(mCurrentItemIndex));
    builder.AppendString(String("; IsEnabled: ") +
            StringUtils::BooleanToString(GetBooleanProperty(PROPERTY_ENABLED)));
    builder.AppendString(String("; IsPassword: ") +
            StringUtils::BooleanToString(GetBooleanProperty(PROPERTY_PASSWORD)));
    builder.AppendString(String("; IsChecked: ") +
            StringUtils::BooleanToString(GetBooleanProperty(PROPERTY_CHECKED)));
    builder.AppendString(String("; IsFullScreen: ") +
            StringUtils::BooleanToString(GetBooleanProperty(PROPERTY_FULL_SCREEN)));
    builder.AppendString(String("; Scrollable: ") +
            StringUtils::BooleanToString(GetBooleanProperty(PROPERTY_SCROLLABLE)));
    String beforeText;
    mBeforeText->ToString(&beforeText);
    builder.AppendString(String("; BeforeText: ") + beforeText);
    builder.AppendString(String("; FromIndex: ") + StringUtils::Int32ToString(mFromIndex));
    builder.AppendString(String("; ToIndex: ") + StringUtils::Int32ToString(mToIndex));
    builder.AppendString(String("; ScrollX: ") + StringUtils::Int32ToString(mScrollX));
    builder.AppendString(String("; ScrollY: ") + StringUtils::Int32ToString(mScrollY));
    builder.AppendString(String("; MaxScrollX: ") + StringUtils::Int32ToString(mMaxScrollX));
    builder.AppendString(String("; MaxScrollY: ") + StringUtils::Int32ToString(mMaxScrollY));
    builder.AppendString(String("; AddedCount: ") + StringUtils::Int32ToString(mAddedCount));
    builder.AppendString(String("; RemovedCount: ") + StringUtils::Int32ToString(mRemovedCount));
    // builder.AppendString(String("; ParcelableData: ") + mParcelableData);
    builder.AppendCStr(" ]");
    return builder.ToString();
}


} // Accessibility
} // View
} // Droid
} // Elastos
