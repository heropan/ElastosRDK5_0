
#include "elastos/droid/view/accessibility/CAccessibilityEvent.h"
#include "elastos/droid/view/accessibility/CAccessibilityRecord.h"
#include <elastos/utility/logging/Slogger.h>

using Elastos::Utility::Logging::Slogger;

namespace Elastos {
namespace Droid {
namespace View {
namespace Accessibility {

const String CAccessibilityEvent::TAG("AccessibilityEvent");
const Int32 CAccessibilityEvent::MAX_POOL_SIZE = 10;

AutoPtr<CAccessibilityEvent> CAccessibilityEvent::sPool = NULL;
Mutex CAccessibilityEvent::sPoolLock;
Int32 CAccessibilityEvent::sPoolSize = 0;

CAccessibilityEvent::CAccessibilityEvent()
    : mIsInPool(FALSE)
    , mEventType(0)
    , mEventTime(0)
    , mMovementGranularity(0)
    , mAction(0)
{}

PInterface CAccessibilityEvent::Probe(
    /* [in] */ REIID riid)
{
    if (riid == EIID_AccessibilityRecord) {
        return reinterpret_cast<PInterface>((AccessibilityRecord*)this);
    }
    return _CAccessibilityEvent::Probe(riid);
}

ECode CAccessibilityEvent::SetSource(
    /* [in] */ IView* source)
{
    return AccessibilityRecord::SetSource(source);
}

ECode CAccessibilityEvent::SetSource(
    /* [in] */ IView* root,
    /* [in] */ Int32 virtualDescendantId)
{
    return AccessibilityRecord::SetSource(root, virtualDescendantId);
}

ECode CAccessibilityEvent::GetSource(
    /* [out] */ IAccessibilityNodeInfo** info)
{
    VALIDATE_NOT_NULL(info);
    return AccessibilityRecord::GetSource(info);
}

ECode CAccessibilityEvent::SetWindowId(
    /* [in] */ Int32 windowId)
{
    return AccessibilityRecord::SetWindowId(windowId);
}

ECode CAccessibilityEvent::GetWindowId(
    /* [out] */ Int32* windowId)
{
    VALIDATE_NOT_NULL(windowId);
    return AccessibilityRecord::GetWindowId(windowId);
}

ECode CAccessibilityEvent::IsChecked(
    /* [out] */ Boolean* checked)
{
    VALIDATE_NOT_NULL(checked);
    return AccessibilityRecord::IsChecked(checked);
}

ECode CAccessibilityEvent::SetChecked(
    /* [in] */ Boolean isChecked)
{
    return AccessibilityRecord::SetChecked(isChecked);
}

ECode CAccessibilityEvent::IsEnabled(
    /* [out] */ Boolean* enabled)
{
    VALIDATE_NOT_NULL(enabled);
    return AccessibilityRecord::IsEnabled(enabled);
}

ECode CAccessibilityEvent::SetEnabled(
    /* [in] */ Boolean enabled)
{
    return AccessibilityRecord::SetEnabled(enabled);
}

ECode CAccessibilityEvent::IsPassword(
    /* [out] */ Boolean* password)
{
    VALIDATE_NOT_NULL(password);
    return AccessibilityRecord::IsPassword(password);
}

ECode CAccessibilityEvent::SetPassword(
    /* [in] */ Boolean password)
{
    return AccessibilityRecord::SetPassword(password);
}

ECode CAccessibilityEvent::IsFullScreen(
    /* [out] */ Boolean* isFull)
{
    VALIDATE_NOT_NULL(isFull);
    return AccessibilityRecord::IsFullScreen(isFull);
}

ECode CAccessibilityEvent::SetFullScreen(
    /* [in] */ Boolean full)
{
    return AccessibilityRecord::SetFullScreen(full);
}

ECode CAccessibilityEvent::IsScrollable(
    /* [out] */ Boolean* scrollable)
{
    VALIDATE_NOT_NULL(scrollable);
    return AccessibilityRecord::IsScrollable(scrollable);
}

ECode CAccessibilityEvent::SetScrollable(
    /* [in] */ Boolean scrollable)
{
    return AccessibilityRecord::SetScrollable(scrollable);
}

ECode CAccessibilityEvent::IsImportantForAccessibility(
    /* [out] */ Boolean* important)
{
    VALIDATE_NOT_NULL(important);
    return AccessibilityRecord::IsImportantForAccessibility(important);
}

ECode CAccessibilityEvent::GetItemCount(
    /* [out] */ Int32* count)
{
    VALIDATE_NOT_NULL(count);
    return AccessibilityRecord::GetItemCount(count);
}

ECode CAccessibilityEvent::SetItemCount(
    /* [in] */ Int32 count)
{
    return AccessibilityRecord::SetItemCount(count);
}

ECode CAccessibilityEvent::GetCurrentItemIndex(
    /* [out] */ Int32* index)
{
    VALIDATE_NOT_NULL(index);
    return AccessibilityRecord::GetCurrentItemIndex(index);
}

ECode CAccessibilityEvent::SetCurrentItemIndex(
    /* [in] */ Int32 index)
{
    return AccessibilityRecord::SetCurrentItemIndex(index);
}

ECode CAccessibilityEvent::GetFromIndex(
    /* [out] */ Int32* index)
{
    VALIDATE_NOT_NULL(index);
    return AccessibilityRecord::GetFromIndex(index);
}

ECode CAccessibilityEvent::SetFromIndex(
    /* [in] */ Int32 index)
{
    return AccessibilityRecord::SetFromIndex(index);
}

ECode CAccessibilityEvent::GetToIndex(
    /* [out] */ Int32* index)
{
    VALIDATE_NOT_NULL(index);
    return AccessibilityRecord::GetToIndex(index);
}

ECode CAccessibilityEvent::SetToIndex(
    /* [in] */ Int32 index)
{
    return AccessibilityRecord::SetToIndex(index);
}

ECode CAccessibilityEvent::GetScrollX(
    /* [out] */ Int32* x)
{
    VALIDATE_NOT_NULL(x);
    return AccessibilityRecord::GetScrollX(x);
}

ECode CAccessibilityEvent::SetScrollX(
    /* [in] */ Int32 x)
{
    return AccessibilityRecord::SetScrollX(x);
}

ECode CAccessibilityEvent::GetScrollY(
    /* [out] */ Int32* y)
{
    VALIDATE_NOT_NULL(y);
    return AccessibilityRecord::GetScrollY(y);
}

ECode CAccessibilityEvent::SetScrollY(
    /* [in] */ Int32 y)
{
    return AccessibilityRecord::SetScrollY(y);
}

ECode CAccessibilityEvent::GetMaxScrollX(
    /* [out] */ Int32* x)
{
    VALIDATE_NOT_NULL(x);
    return AccessibilityRecord::GetMaxScrollX(x);
}

ECode CAccessibilityEvent::SetMaxScrollX(
    /* [in] */ Int32 x)
{
    return AccessibilityRecord::SetMaxScrollX(x);
}

ECode CAccessibilityEvent::GetMaxScrollY(
    /* [out] */ Int32* y)
{
    VALIDATE_NOT_NULL(y);
    return AccessibilityRecord::GetMaxScrollY(y);
}

ECode CAccessibilityEvent::SetMaxScrollY(
    /* [in] */ Int32 y)
{
    return AccessibilityRecord::SetMaxScrollY(y);
}

ECode CAccessibilityEvent::GetAddedCount(
    /* [out] */ Int32* count)
{
    VALIDATE_NOT_NULL(count);
    return AccessibilityRecord::GetAddedCount(count);
}

ECode CAccessibilityEvent::SetAddedCount(
    /* [in] */ Int32 count)
{
    return AccessibilityRecord::SetAddedCount(count);
}

ECode CAccessibilityEvent::GetRemovedCount(
    /* [out] */ Int32* count)
{
    VALIDATE_NOT_NULL(count);
    return AccessibilityRecord::GetRemovedCount(count);
}

ECode CAccessibilityEvent::SetRemovedCount(
    /* [in] */ Int32 count)
{
    return AccessibilityRecord::SetRemovedCount(count);
}

ECode CAccessibilityEvent::GetClassName(
    /* [out] */ ICharSequence** name)
{
    VALIDATE_NOT_NULL(name);
    return AccessibilityRecord::GetClassName(name);
}

ECode CAccessibilityEvent::SetClassName(
    /* [in] */ ICharSequence* name)
{
    return AccessibilityRecord::SetClassName(name);
}

ECode CAccessibilityEvent::GetText(
    /* [out] */ IObjectContainer** container)
{
    VALIDATE_NOT_NULL(container);
    return AccessibilityRecord::GetText(container);
}

ECode CAccessibilityEvent::GetBeforeText(
    /* [out] */ ICharSequence** text)
{
    VALIDATE_NOT_NULL(text);
    return AccessibilityRecord::GetBeforeText(text);
}

ECode CAccessibilityEvent::SetBeforeText(
    /* [in] */ ICharSequence* text)
{
    return AccessibilityRecord::SetBeforeText(text);
}

ECode CAccessibilityEvent::GetContentDescription(
    /* [out] */ ICharSequence** contentDescription)
{
    VALIDATE_NOT_NULL(contentDescription);
    return AccessibilityRecord::GetContentDescription(contentDescription);
}

ECode CAccessibilityEvent::SetContentDescription(
    /* [in] */ ICharSequence* contentDescription)
{
    return AccessibilityRecord::SetContentDescription(contentDescription);
}

ECode CAccessibilityEvent::GetParcelableData(
    /* [out] */ IParcelable** parcelableData)
{
    VALIDATE_NOT_NULL(parcelableData);
    return AccessibilityRecord::GetParcelableData(parcelableData);
}

ECode CAccessibilityEvent::SetParcelableData(
    /* [in] */ IParcelable* parcelableData)
{
    return AccessibilityRecord::SetParcelableData(parcelableData);
}

ECode CAccessibilityEvent::GetSourceNodeId(
    /* [out] */ Int64* nodeId)
{
    VALIDATE_NOT_NULL(nodeId);
    return AccessibilityRecord::GetSourceNodeId(nodeId);
}

ECode CAccessibilityEvent::SetConnectionId(
    /* [in] */ Int32 connectionId)
{
    return AccessibilityRecord::SetConnectionId(connectionId);
}

ECode CAccessibilityEvent::SetSealed(
    /* [in] */ Boolean sealed)
{
    AccessibilityRecord::SetSealed(sealed);
    List<AutoPtr<IAccessibilityRecord> > records = mRecords;
    List<AutoPtr<IAccessibilityRecord> >::Iterator it = mRecords.Begin();
    for (; it != mRecords.End(); ++it) {
        AutoPtr<IAccessibilityRecord> record = *it;
        record->SetSealed(sealed);
    }
    return NOERROR;
}

ECode CAccessibilityEvent::Recycle()
{
    if (mIsInPool) {
        Slogger::E(TAG, "Event already recycled!");
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
        // throw new IllegalStateException("Event already recycled!");
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

void CAccessibilityEvent::Init(
    /* [in] */ IAccessibilityEvent* event)
{
    AccessibilityRecord* baseCls = reinterpret_cast<AccessibilityRecord*>(event->Probe(EIID_AccessibilityRecord));
    AccessibilityRecord::Init(baseCls);

    AutoPtr<CAccessibilityEvent> cls = (CAccessibilityEvent*)event;
    mEventType = cls->mEventType;
    mMovementGranularity = cls->mMovementGranularity;
    mAction = cls->mAction;
    mEventTime = cls->mEventTime;
    mPackageName = cls->mPackageName;
}

ECode CAccessibilityEvent::GetRecordCount(
    /* [out] */ Int32* count)
{
    VALIDATE_NOT_NULL(count);
    *count = mRecords.GetSize();
    return NOERROR;
}

ECode CAccessibilityEvent::AppendRecord(
    /* [in] */ IAccessibilityRecord* record)
{
    FAIL_RETURN(EnforceNotSealed());
    mRecords.PushBack(record);
    return NOERROR;
}

ECode CAccessibilityEvent::GetRecord(
    /* [in] */ Int32 index,
    /* [out] */ IAccessibilityRecord** record)
{
    VALIDATE_NOT_NULL(record);
    *record = mRecords[index];
    REFCOUNT_ADD(*record);
    return NOERROR;
}

ECode CAccessibilityEvent::GetEventType(
    /* [out] */ Int32* type)
{
    VALIDATE_NOT_NULL(type);
    *type = mEventType;
    return NOERROR;
}

ECode CAccessibilityEvent::SetEventType(
    /* [in] */ Int32 eventType)
{
    FAIL_RETURN(EnforceNotSealed());
    mEventType = eventType;
    return NOERROR;
}

ECode CAccessibilityEvent::GetEventTime(
    /* [out] */ Int64* eventTime)
{
    VALIDATE_NOT_NULL(eventTime);
    *eventTime = mEventTime;
    return NOERROR;
}

ECode CAccessibilityEvent::SetEventTime(
    /* [in] */ Int64 eventTime)
{
    FAIL_RETURN(EnforceNotSealed());
    mEventTime = eventTime;
    return NOERROR;
}

ECode CAccessibilityEvent::GetPackageName(
    /* [out] */ ICharSequence** packageName)
{
    VALIDATE_NOT_NULL(packageName);
    *packageName = mPackageName;
    REFCOUNT_ADD(*packageName);
    return NOERROR;
}

ECode CAccessibilityEvent::SetPackageName(
    /* [in] */ ICharSequence* packageName)
{
    FAIL_RETURN(EnforceNotSealed());
    mPackageName = packageName;
    return NOERROR;
}

ECode CAccessibilityEvent::SetMovementGranularity(
    /* [in] */ Int32 granularity)
{
    FAIL_RETURN(EnforceNotSealed());
    mMovementGranularity = granularity;
    return NOERROR;
}

ECode CAccessibilityEvent::GetMovementGranularity(
    /* [out] */ Int32* granularity)
{
    VALIDATE_NOT_NULL(granularity);
    *granularity = mMovementGranularity;
    return NOERROR;
}

ECode CAccessibilityEvent::SetAction(
    /* [in] */ Int32 action)
{
    FAIL_RETURN(EnforceNotSealed());
    mAction = action;
    return NOERROR;
}

ECode CAccessibilityEvent::GetAction(
    /* [out] */ Int32* action)
{
    VALIDATE_NOT_NULL(action);
    *action = mAction;
    return NOERROR;
}

ECode CAccessibilityEvent::Obtain(
    /* [in] */ Int32 eventType,
    /* [out] */ IAccessibilityEvent** _event)
{
    VALIDATE_NOT_NULL(_event);
    AutoPtr<IAccessibilityEvent> event;
    FAIL_RETURN(Obtain((IAccessibilityEvent**)&event));
    event->SetEventType(eventType);
    *_event = event;
    REFCOUNT_ADD(*_event);
    return NOERROR;
}

ECode CAccessibilityEvent::Obtain(
    /* [in] */ IAccessibilityEvent* otherEvent,
    /* [out] */ IAccessibilityEvent** event)
{
    VALIDATE_NOT_NULL(event);
    AutoPtr<IAccessibilityEvent> eventClone;
    FAIL_RETURN(Obtain((IAccessibilityEvent**)&eventClone));
    AutoPtr<CAccessibilityEvent> otherCls = (CAccessibilityEvent*)otherEvent;
    otherCls->Init(otherEvent);

    List<AutoPtr<IAccessibilityRecord> >::Iterator it = otherCls->mRecords.Begin();
    for (; it != otherCls->mRecords.End(); ++it) {
        AutoPtr<IAccessibilityRecord> record = *it;
        AutoPtr<IAccessibilityRecord> recordClone;
        FAIL_RETURN(CAccessibilityRecord::Obtain(record,
                (IAccessibilityRecord**)&recordClone));
        ((CAccessibilityEvent*)eventClone.Get())->mRecords.PushBack(recordClone);
    }

    *event = eventClone;
    REFCOUNT_ADD(*event);
    return NOERROR;
}

ECode CAccessibilityEvent::Obtain(
    /* [out] */ IAccessibilityEvent** event)
{
    VALIDATE_NOT_NULL(event);
    AutoLock lock(sPoolLock);
    if (sPool != NULL) {
        AutoPtr<CAccessibilityEvent> e = sPool;
        sPool = sPool->mNext;
        sPoolSize--;
        e->mNext = NULL;
        e->mIsInPool = FALSE;
        *event = (IAccessibilityEvent*)e;
        REFCOUNT_ADD(*event);
        return NOERROR;
    }
    AutoPtr<CAccessibilityEvent> cevent;
    CAccessibilityEvent::NewByFriend((CAccessibilityEvent**)&cevent);
    *event = cevent;
    REFCOUNT_ADD(*event);
    return NOERROR;
}

void CAccessibilityEvent::Clear()
{
    AccessibilityRecord::Clear();
    mEventType = 0;
    mMovementGranularity = 0;
    mAction = 0;
    mPackageName = NULL;
    mEventTime = 0;
    while (mRecords.Begin() != mRecords.End()) {
        AutoPtr<IAccessibilityRecord> record = *mRecords.Begin();
        record->Recycle();
        mRecords.Erase(mRecords.Begin());
    }
}

ECode CAccessibilityEvent::InitFromParcel(
    /* [in] */ IParcel* parcel)
{
    parcel->ReadBoolean(&mSealed);
    parcel->ReadInt32(&mEventType);
    parcel->ReadInt32(&mMovementGranularity);
    parcel->ReadInt32(&mAction);
    // mPackageName = TextUtils.CHAR_SEQUENCE_CREATOR.createFromParcel(parcel);
    parcel->ReadInt64(&mEventTime);
    parcel->ReadInt32(&mConnectionId);
    ReadAccessibilityRecordFromParcel((IAccessibilityRecord*)this, parcel);

    // Read the records.
    Int32 recordCount;
    parcel->ReadInt32(&recordCount);
    for (Int32 i = 0; i < recordCount; i++) {
        AutoPtr<IAccessibilityRecord> record;
        CAccessibilityRecord::Obtain((IAccessibilityRecord**)&record);
        ReadAccessibilityRecordFromParcel(record, parcel);
        ((CAccessibilityRecord*)record.Get())->mConnectionId = mConnectionId;
        mRecords.PushBack(record);
    }
    return NOERROR;
}

void CAccessibilityEvent::ReadAccessibilityRecordFromParcel(
    /* [in] */ IAccessibilityRecord* record,
    /* [in] */ IParcel* parcel)
{
    AccessibilityRecord* cls = reinterpret_cast<AccessibilityRecord*>(record->Probe(EIID_AccessibilityRecord));
    parcel->ReadInt32(&cls->mBooleanProperties);
    parcel->ReadInt32(&cls->mCurrentItemIndex);
    parcel->ReadInt32(&cls->mItemCount);
    parcel->ReadInt32(&cls->mFromIndex);
    parcel->ReadInt32(&cls->mToIndex);
    parcel->ReadInt32(&cls->mScrollX);
    parcel->ReadInt32(&cls->mScrollY);
    parcel->ReadInt32(&cls->mMaxScrollX);
    parcel->ReadInt32(&cls->mMaxScrollY);
    parcel->ReadInt32(&cls->mAddedCount);
    parcel->ReadInt32(&cls->mRemovedCount);
    // cls->mClassName = TextUtils.CHAR_SEQUENCE_CREATOR.createFromParcel(parcel);
    // cls->mContentDescription = TextUtils.CHAR_SEQUENCE_CREATOR.createFromParcel(parcel);
    // cls->mBeforeText = TextUtils.CHAR_SEQUENCE_CREATOR.createFromParcel(parcel);
    cls->mParcelableData = NULL;
    parcel->ReadInterfacePtr((Handle32*)(IParcelable**)&cls->mParcelableData);
    // parcel.readList(record.mText, null);
    Int32 count;
    parcel->ReadInt32(&count);
    for (Int32 i = 0; i < count; ++i) {
        AutoPtr<ICharSequence> r;
        parcel->ReadInterfacePtr((Handle32*)(ICharSequence**)&r);
        cls->mText.PushBack(r);
    }
    parcel->ReadInt32(&cls->mSourceWindowId);
    parcel->ReadInt64(&cls->mSourceNodeId);
    parcel->ReadBoolean(&cls->mSealed);
}

ECode CAccessibilityEvent::WriteToParcel(
    /* [in] */ IParcel* parcel)
{
    parcel->WriteBoolean(IsSealed() ? 1 : 0);
    parcel->WriteInt32(mEventType);
    parcel->WriteInt32(mMovementGranularity);
    parcel->WriteInt32(mAction);
    // TextUtils.writeToParcel(mPackageName, parcel, 0);
    parcel->WriteInt64(mEventTime);
    parcel->WriteInt32(mConnectionId);
    WriteAccessibilityRecordToParcel((IAccessibilityRecord*)this, parcel);

    // Write the records.
    Int32 recordCount;
    GetRecordCount(&recordCount);
    parcel->WriteInt32(recordCount);
    List<AutoPtr<IAccessibilityRecord> >::Iterator it = mRecords.Begin();
    for (; it != mRecords.End(); ++it) {
        WriteAccessibilityRecordToParcel(*it, parcel);
    }

    return NOERROR;
}

void CAccessibilityEvent::WriteAccessibilityRecordToParcel(
    /* [in] */ IAccessibilityRecord* record,
    /* [in] */ IParcel* parcel)
{
    AccessibilityRecord* cls = reinterpret_cast<AccessibilityRecord*>(record->Probe(EIID_AccessibilityRecord));
    parcel->WriteInt32(cls->mBooleanProperties);
    parcel->WriteInt32(cls->mCurrentItemIndex);
    parcel->WriteInt32(cls->mItemCount);
    parcel->WriteInt32(cls->mFromIndex);
    parcel->WriteInt32(cls->mToIndex);
    parcel->WriteInt32(cls->mScrollX);
    parcel->WriteInt32(cls->mScrollY);
    parcel->WriteInt32(cls->mMaxScrollX);
    parcel->WriteInt32(cls->mMaxScrollY);
    parcel->WriteInt32(cls->mAddedCount);
    parcel->WriteInt32(cls->mRemovedCount);
    // TextUtils.writeToParcel(record.mClassName, parcel, flags);
    // TextUtils.writeToParcel(record.mContentDescription, parcel, flags);
    // TextUtils.writeToParcel(record.mBeforeText, parcel, flags);
    // parcel->WriteParcelable(record.mParcelableData, flags);
    parcel->WriteInterfacePtr((IInterface*)cls->mParcelableData);
    parcel->WriteInt32(cls->mText.GetSize());
    List<AutoPtr<ICharSequence> >::Iterator it = mText.Begin();
    for (; it != mText.End(); ++it) {
        parcel->WriteInterfacePtr((IInterface*)(*it));
    }
    // parcel.writeList(record.mText);
    parcel->WriteInt32(cls->mSourceWindowId);
    parcel->WriteInt64(cls->mSourceNodeId);
    parcel->WriteBoolean(cls->mSealed);
}

ECode CAccessibilityEvent::ReadFromParcel(
    /* [in] */ IParcel* parcel)
{
    AutoPtr<IAccessibilityEvent> event;
    FAIL_RETURN(Obtain((IAccessibilityEvent**)&event));
    return event->InitFromParcel(parcel);
}

String CAccessibilityEvent::EventTypeToString(
    /* [in] */ Int32 eventType)
{
    switch (eventType) {
        case TYPE_VIEW_CLICKED:
            return String("TYPE_VIEW_CLICKED");
        case TYPE_VIEW_LONG_CLICKED:
            return String("TYPE_VIEW_LONG_CLICKED");
        case TYPE_VIEW_SELECTED:
            return String("TYPE_VIEW_SELECTED");
        case TYPE_VIEW_FOCUSED:
            return String("TYPE_VIEW_FOCUSED");
        case TYPE_VIEW_TEXT_CHANGED:
            return String("TYPE_VIEW_TEXT_CHANGED");
        case TYPE_WINDOW_STATE_CHANGED:
            return String("TYPE_WINDOW_STATE_CHANGED");
        case TYPE_VIEW_HOVER_ENTER:
            return String("TYPE_VIEW_HOVER_ENTER");
        case TYPE_VIEW_HOVER_EXIT:
            return String("TYPE_VIEW_HOVER_EXIT");
        case TYPE_NOTIFICATION_STATE_CHANGED:
            return String("TYPE_NOTIFICATION_STATE_CHANGED");
        case TYPE_TOUCH_EXPLORATION_GESTURE_START:
            return String("TYPE_TOUCH_EXPLORATION_GESTURE_START");
        case TYPE_TOUCH_EXPLORATION_GESTURE_END:
            return String("TYPE_TOUCH_EXPLORATION_GESTURE_END");
        case TYPE_WINDOW_CONTENT_CHANGED:
            return String("TYPE_WINDOW_CONTENT_CHANGED");
        case TYPE_VIEW_TEXT_SELECTION_CHANGED:
            return String("TYPE_VIEW_TEXT_SELECTION_CHANGED");
        case TYPE_VIEW_SCROLLED:
            return String("TYPE_VIEW_SCROLLED");
        case TYPE_ANNOUNCEMENT:
            return String("TYPE_ANNOUNCEMENT");
        case TYPE_VIEW_ACCESSIBILITY_FOCUSED:
            return String("TYPE_VIEW_ACCESSIBILITY_FOCUSED");
        case TYPE_VIEW_ACCESSIBILITY_FOCUS_CLEARED:
            return String("TYPE_VIEW_ACCESSIBILITY_FOCUS_CLEARED");
        case TYPE_VIEW_TEXT_TRAVERSED_AT_MOVEMENT_GRANULARITY:
            return String("TYPE_CURRENT_AT_GRANULARITY_MOVEMENT_CHANGED");
        case TYPE_GESTURE_DETECTION_START:
            return String("TYPE_GESTURE_DETECTION_START");
        case TYPE_GESTURE_DETECTION_END:
            return String("TYPE_GESTURE_DETECTION_END");
        case TYPE_TOUCH_INTERACTION_START:
            return String("TYPE_TOUCH_INTERACTION_START");
        case TYPE_TOUCH_INTERACTION_END:
            return String("TYPE_TOUCH_INTERACTION_END");
        default:
            return String(NULL);
    }
}

} // Accessibility
} // View
} // Droid
} // Elastos
