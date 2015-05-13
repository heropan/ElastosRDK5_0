
#include "PreferenceGroupAdapter.h"
#include "Preference.h"
#include "os/CHandler.h"
#include <elastos/Math.h>
#include <elastos/Slogger.h>

using Elastos::Core::Math;
using Elastos::Utility::ICollections;
using Elastos::Utility::CCollections;
using Elastos::Utility::Logging::Slogger;
using Elastos::Utility::IArrayList;
using Elastos::Utility::CArrayList;
using Elastos::Core::EIID_IComparable;
using Elastos::Core::EIID_IRunnable;
using Elastos::Droid::Os::CHandler;
using Elastos::Droid::Widget::IListView;
using Elastos::Droid::Widget::EIID_IBaseAdapter;
using Elastos::Droid::Widget::EIID_IListAdapter;
using Elastos::Droid::Widget::EIID_IAdapter;

namespace Elastos {
namespace Droid {
namespace Preference {

//====================================================
// PreferenceGroupAdapter::PreferenceLayout
//====================================================

CAR_INTERFACE_IMPL(PreferenceGroupAdapter::PreferenceLayout, IComparable)

ECode PreferenceGroupAdapter::PreferenceLayout::CompareTo(
    /* [in] */ IInterface* other,
    /* [in] */ Int32* result)
{
    VALIDATE_NOT_NULL(result)
    AutoPtr<PreferenceLayout> otherObj = (PreferenceLayout*)other;
    Int32 compareNames = mName.Compare(otherObj->mName);
    if (compareNames == 0) {
        if (mResId == otherObj->mResId) {
            if (mWidgetResId == otherObj->mWidgetResId) {
                *result = 0;
                return NOERROR;
            }
            else {
                *result = mWidgetResId - otherObj->mWidgetResId;
                return NOERROR;
            }
        }
        else {
            *result = mResId - otherObj->mResId;
            return NOERROR;
        }
    }

    *result = compareNames;
    return NOERROR;
}


//====================================================
// PreferenceGroupAdapter::SyncRunnable
//====================================================

PreferenceGroupAdapter::SyncRunnable::SyncRunnable(
    /* [in] */ PreferenceGroupAdapter* host)
    : mHost(host)
{}

ECode PreferenceGroupAdapter::SyncRunnable::Run()
{
    mHost->SyncMyPreferences();
    return NOERROR;
}


//====================================================
// PreferenceGroupAdapter
//====================================================

const String PreferenceGroupAdapter::TAG("PreferenceGroupAdapter");

PreferenceGroupAdapter::PreferenceGroupAdapter(
    /* [in] */ IPreferenceGroup* preferenceGroup)
    : mHasReturnedViewTypeCount(FALSE)
    , mIsSyncing(FALSE)
{
    AutoPtr<IWeakReferenceSource> wrs = IWeakReferenceSource::Probe(preferenceGroup);
    assert(wrs != NULL && "Error: Invalid PreferenceGroup interface, IWeakReferenceSource not implemented!");
    wrs->GetWeakReference((IWeakReference**)&mWeakPreferenceGroup);
    mPreferenceList = new List<AutoPtr<IPreference> >();
    mPreferenceLayouts = new List<AutoPtr<PreferenceLayout> >();
    mTempPreferenceLayout = new PreferenceLayout();
    CHandler::New((IHandler**)&mHandler);
    mSyncRunnable = new SyncRunnable(this);

    // If this group gets or loses any children, let us know
    reinterpret_cast<Preference*>(preferenceGroup->Probe(EIID_Preference))->SetOnPreferenceChangeInternalListener(
            (IPreferenceOnPreferenceChangeInternalListener*)this);

    SyncMyPreferences();
}

PInterface PreferenceGroupAdapter::Probe(
    /* [in] */REIID riid)
{
    if (riid == EIID_IInterface) {
        return (PInterface)(IBaseAdapter*)this;
    }
    else if (riid == EIID_IBaseAdapter) {
        return (IBaseAdapter*)this;
    }
    else if (riid == EIID_IListAdapter) {
        return (IListAdapter*)this;
    }
    else if (riid == EIID_IAdapter) {
        return (IAdapter*)this;
    }

    return NULL;
}

UInt32 PreferenceGroupAdapter::AddRef()
{
    return ElRefBase::AddRef();
}

UInt32 PreferenceGroupAdapter::Release()
{
    return ElRefBase::Release();
}

ECode PreferenceGroupAdapter::GetInterfaceID(
    /* [in] */ IInterface* pObject,
    /* [out] */ InterfaceID* pIID)
{
    VALIDATE_NOT_NULL(pIID)

    if (pObject == (IInterface *)(IBaseAdapter*)this) {
        *pIID = EIID_IBaseAdapter;
        return NOERROR;
    }
    else if (pObject == (IInterface *)(IListAdapter*)this) {
        *pIID = EIID_IListAdapter;
        return NOERROR;
    }
    else if (pObject == (IInterface *)(IAdapter*)this) {
        *pIID = EIID_IAdapter;
        return NOERROR;
    }
    return E_INVALID_ARGUMENT;
}

void PreferenceGroupAdapter::SyncMyPreferences()
{
    {
        Object::Autolock lock(mLock);
        if (mIsSyncing) {
            return;
        }

        mIsSyncing = TRUE;
    }

    AutoPtr< List<AutoPtr<IPreference> > > newPreferenceList = new List<AutoPtr<IPreference> >(mPreferenceList->GetSize());
    AutoPtr<IPreferenceGroup> group;
    mWeakPreferenceGroup->Resolve(EIID_IPreferenceGroup, (IInterface**)&group);
    FlattenPreferenceGroup(newPreferenceList, group);

    mPreferenceList = newPreferenceList;

    BaseAdapter::NotifyDataSetChanged();

    Object::Autolock lock(mLock);
    mIsSyncing = FALSE;
    mLock.NotifyAll();
}

void PreferenceGroupAdapter::FlattenPreferenceGroup(
    /* [in] */ List<AutoPtr<IPreference> >* preferences,
    /* [in] */ IPreferenceGroup* group)
{
    // TODO: shouldn't always?
    group->SortPreferences();

    Int32 groupSize;
    group->GetPreferenceCount(&groupSize);
    for (Int32 i = 0; i < groupSize; i++) {
        AutoPtr<IPreference> preference;
        group->GetPreference(i, (IPreference**)&preference);

        preferences->PushBack(preference);

        Boolean hasSpecifiedLayout;
        if (!mHasReturnedViewTypeCount && (preference->HasSpecifiedLayout(&hasSpecifiedLayout), !hasSpecifiedLayout)) {
            AddPreferenceClassName(preference);
        }

        if (IPreferenceGroup::Probe(preference) != NULL) {
            AutoPtr<IPreferenceGroup> preferenceGroup = IPreferenceGroup::Probe(preference);
            Boolean isOnSameScreenAsChildren;
            if (preferenceGroup->IsOnSameScreenAsChildren(&isOnSameScreenAsChildren), isOnSameScreenAsChildren) {
                FlattenPreferenceGroup(preferences, preferenceGroup);
            }
        }

        reinterpret_cast<Preference*>(preference->Probe(EIID_Preference))->SetOnPreferenceChangeInternalListener(this);
    }
}

AutoPtr<PreferenceGroupAdapter::PreferenceLayout> PreferenceGroupAdapter::CreatePreferenceLayout(
    /* [in] */ IPreference* preference,
    /* [in] */ PreferenceLayout* in)
{
    AutoPtr<PreferenceLayout> pl = in != NULL ? in : new PreferenceLayout();
    // EGuid clsId;
    // IObject::Probe(preference)->GetClassID(&clsId);
    // pl->mName = clsId.pUunm;
    preference->GetLayoutResource(&pl->mResId);
    preference->GetWidgetLayoutResource(&pl->mWidgetResId);
    return pl;
}

void PreferenceGroupAdapter::AddPreferenceClassName(
    /* [in] */ IPreference* preference)
{
    AutoPtr<PreferenceLayout> pl = CreatePreferenceLayout(preference, NULL);
    AutoPtr<ICollections> collections;
    CCollections::AcquireSingleton((ICollections**)&collections);
    AutoPtr<IArrayList> arrayList;
    CArrayList::New((IArrayList**)&arrayList);
    List<AutoPtr<PreferenceLayout> >::Iterator it = mPreferenceLayouts->Begin();
    for (; it != mPreferenceLayouts->End(); ++it) {
        Boolean isSuccess;
        arrayList->Add(*it, &isSuccess);
    }
    Int32 insertPos;
    collections->BinarySearch(arrayList, pl, &insertPos);

    // Only insert if it doesn't exist (when it is negative).
    if (insertPos < 0) {
        // Convert to insert index
        insertPos = insertPos * -1 - 1;
        mPreferenceLayouts->Insert(insertPos, pl);
    }
}

ECode PreferenceGroupAdapter::GetCount(
    /* [out] */ Int32* count)
{
    VALIDATE_NOT_NULL(count)
    *count = mPreferenceList->GetSize();
    return NOERROR;
}

ECode PreferenceGroupAdapter::GetItem(
    /* [in] */ Int32 position,
    /* [out] */ IInterface** item)
{
    VALIDATE_NOT_NULL(item)
    Int32 count;
    if (position < 0 || (GetCount(&count), position >= count)) {
        *item = NULL;
        return NOERROR;
    }
    *item = (IInterface*)(*mPreferenceList)[position];
    INTERFACE_ADDREF(*item)
    return NOERROR;
}

ECode PreferenceGroupAdapter::GetItemId(
    /* [in] */ Int32 position,
    /* [out] */ Int64* id)
{
    VALIDATE_NOT_NULL(id)
    Int32 count;
    if (position < 0 || (GetCount(&count), position >= count)) {
        *id = IListView::INVALID_ROW_ID;
        return NOERROR;
    }
    AutoPtr<IPreference> p;
    GetItem(position, (IInterface**)&p);
    *id = reinterpret_cast<Preference*>(p->Probe(EIID_Preference))->GetId();
    return NOERROR;
}

ECode PreferenceGroupAdapter::GetView(
    /* [in] */ Int32 position,
    /* [in] */ IView* convertView,
    /* [in] */ IViewGroup* parent,
    /* [out] */ IView** view)
{
    VALIDATE_NOT_NULL(view)

    AutoPtr<IPreference> preference;
    GetItem(position, (IInterface**)&preference);
    // Build a PreferenceLayout to compare with known ones that are cacheable.
    mTempPreferenceLayout = CreatePreferenceLayout(preference, mTempPreferenceLayout);

    // If it's not one of the cached ones, set the convertView to null so that
    // the layout gets re-created by the Preference.
    AutoPtr<ICollections> collections;
    CCollections::AcquireSingleton((ICollections**)&collections);
    AutoPtr<IArrayList> arrayList;
    CArrayList::New((IArrayList**)&arrayList);
    List<AutoPtr<PreferenceLayout> >::Iterator it = mPreferenceLayouts->Begin();
    for (; it != mPreferenceLayouts->End(); ++it) {
        Boolean isSuccess;
        arrayList->Add(*it, &isSuccess);
    }
    Int32 insertPos;
    collections->BinarySearch(arrayList, mTempPreferenceLayout, &insertPos);
    if (insertPos < 0) {
        convertView = NULL;
    }

    return preference->GetView(convertView, parent, view);
}

ECode PreferenceGroupAdapter::IsEnabled(
    /* [in] */ Int32 position,
    /* [out] */ Boolean* enabled)
{
    VALIDATE_NOT_NULL(enabled)
    Int32 count;
    if (position < 0 || (GetCount(&count), position >= count)) {
        *enabled = TRUE;
        return NOERROR;
    }
    AutoPtr<IPreference> p;
    GetItem(position, (IInterface**)&p);
    return p->IsSelectable(enabled);
}

ECode PreferenceGroupAdapter::AreAllItemsEnabled(
    /* [out] */ Boolean* enabled)
{
    VALIDATE_NOT_NULL(enabled)
    // There should always be a preference group, and these groups are always disabled
    *enabled = FALSE;
    return NOERROR;
}

ECode PreferenceGroupAdapter::OnPreferenceChange(
    /* [in] */ IPreference* preference)
{
    return NotifyDataSetChanged();
}

ECode PreferenceGroupAdapter::OnPreferenceHierarchyChange(
    /* [in] */ IPreference* preference)
{
    mHandler->RemoveCallbacks(mSyncRunnable);
    Boolean result;
    return mHandler->Post(mSyncRunnable, &result);
}

ECode PreferenceGroupAdapter::HasStableIds(
    /* [out] */ Boolean* hasStableIds)
{
    VALIDATE_NOT_NULL(hasStableIds)
    *hasStableIds = TRUE;
    return NOERROR;
}

ECode PreferenceGroupAdapter::GetItemViewType(
    /* [in] */ Int32 position,
    /* [out] */ Int32* type)
{
    VALIDATE_NOT_NULL(type)

    if (!mHasReturnedViewTypeCount) {
        mHasReturnedViewTypeCount = TRUE;
    }

    AutoPtr<IPreference> preference;
    GetItem(position, (IInterface**)&preference);
    Boolean hasSpecifiedLayout;
    if (preference->HasSpecifiedLayout(&hasSpecifiedLayout), hasSpecifiedLayout) {
        *type = IGNORE_ITEM_VIEW_TYPE;
        return NOERROR;
    }

    mTempPreferenceLayout = CreatePreferenceLayout(preference, mTempPreferenceLayout);

    AutoPtr<ICollections> collections;
    CCollections::AcquireSingleton((ICollections**)&collections);
    AutoPtr<IArrayList> arrayList;
    CArrayList::New((IArrayList**)&arrayList);
    List<AutoPtr<PreferenceLayout> >::Iterator it = mPreferenceLayouts->Begin();
    for (; it != mPreferenceLayouts->End(); ++it) {
        Boolean isSuccess;
        arrayList->Add(*it, &isSuccess);
    }
    Int32 viewType;
    collections->BinarySearch(arrayList, mTempPreferenceLayout, &viewType);
    if (viewType < 0) {
        // This is a class that was seen after we returned the count, so
        // don't recycle it.
        *type = IGNORE_ITEM_VIEW_TYPE;
    }
    else {
        *type = viewType;
    }
    return NOERROR;
}

ECode PreferenceGroupAdapter::GetViewTypeCount(
    /* [out] */ Int32* count)
{
    VALIDATE_NOT_NULL(count)
    if (!mHasReturnedViewTypeCount) {
        mHasReturnedViewTypeCount = TRUE;
    }

    *count = Elastos::Core::Math::Max(1, mPreferenceLayouts->GetSize());
    return NOERROR;
}

ECode PreferenceGroupAdapter::NotifyDataSetChanged()
{
    return BaseAdapter::NotifyDataSetChanged();
}

ECode PreferenceGroupAdapter::NotifyDataSetInvalidated()
{
    return BaseAdapter::NotifyDataSetInvalidated();
}

ECode PreferenceGroupAdapter::RegisterDataSetObserver(
    /* [in] */ IDataSetObserver* observer)
{
    return BaseAdapter::RegisterDataSetObserver(observer);
}

ECode PreferenceGroupAdapter::UnregisterDataSetObserver(
    /* [in] */ IDataSetObserver* observer)
{
    return BaseAdapter::UnregisterDataSetObserver(observer);
}

ECode PreferenceGroupAdapter::IsEmpty(
    /* [out] */ Boolean* isEmpty)
{
    VALIDATE_NOT_NULL(isEmpty)
    *isEmpty = BaseAdapter::IsEmpty();
    return NOERROR;
}

Int32 PreferenceGroupAdapter::GetCount()
{
    Int32 count;
    GetCount(&count);
    return count;
}

AutoPtr<IInterface> PreferenceGroupAdapter::GetItem(
    /* [in] */ Int32 position)
{
    AutoPtr<IInterface> item;
    GetItem(position, (IInterface**)&item);
    return item;
}

Int64 PreferenceGroupAdapter::GetItemId(
    /* [in] */ Int32 position)
{
    Int64 id;
    GetItemId(position, &id);
    return id;
}

AutoPtr<IView> PreferenceGroupAdapter::GetView(
    /* [in] */ Int32 position,
    /* [in] */ IView* convertView,
    /* [in] */ IViewGroup* parent)
{
    AutoPtr<IView> view;
    GetView(position, convertView, parent, (IView**)&view);
    return view;
}

}
}
}
