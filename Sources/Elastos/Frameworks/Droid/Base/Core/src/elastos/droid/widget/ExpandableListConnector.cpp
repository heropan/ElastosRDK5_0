
#include "elastos/droid/widget/ExpandableListConnector.h"
#include "elastos/droid/widget/CExpandableListPosition.h"
#include "elastos/droid/os/SystemClock.h"
#include "elastos/droid/widget/AdapterView.h"
#include "elastos/droid/os/SystemClock.h"

using namespace Elastos::Core;
using Elastos::Droid::Os::SystemClock;

namespace Elastos {
namespace Droid {
namespace Widget {


ExpandableListConnector::ExpandableListConnector()
    : mExpandableListAdapter(NULL)
    , mTotalExpChildrenCount(0)
    , mMaxExpGroupCount(Elastos::Core::Math::INT32_MAX_VALUE)
{
}

ExpandableListConnector::ExpandableListConnector(
    /* [in] */ IExpandableListAdapter* expandableListAdapter)
    : mExpandableListAdapter(NULL)
    , mTotalExpChildrenCount(0)
    , mMaxExpGroupCount(Elastos::Core::Math::INT32_MAX_VALUE)
{
    SetExpandableListAdapter(expandableListAdapter);
}

ECode ExpandableListConnector::SetExpandableListAdapter(
    /* [in] */ IExpandableListAdapter* expandableListAdapter)
{
    if (mExpandableListAdapter != NULL) {
        FAIL_RETURN(mExpandableListAdapter->UnregisterDataSetObserver(mDataSetObserver));
    }

    mExpandableListAdapter = expandableListAdapter;
    return expandableListAdapter->RegisterDataSetObserver(mDataSetObserver);
}

AutoPtr<IPositionMetadata> ExpandableListConnector::GetUnflattenedPos(
    /* [in] */ Int32 flPos)
{
    List<AutoPtr<GroupMetadata> > egml = mExpGroupMetadataList;
    Int32 numExpGroups = egml.GetSize();
    Int32 leftExpGroupIndex = 0;
    Int32 rightExpGroupIndex = numExpGroups - 1;
    Int32 midExpGroupIndex = 0;
    AutoPtr<GroupMetadata> midExpGm;

    if (numExpGroups == 0) {
        return PositionMetadata::Obtain(flPos, ExpandableListPosition::GROUP, flPos,
                -1, NULL, 0);
    }

    while (leftExpGroupIndex <= rightExpGroupIndex) {
        midExpGroupIndex =
                (rightExpGroupIndex - leftExpGroupIndex) / 2
                        + leftExpGroupIndex;
        midExpGm = egml[midExpGroupIndex];

        if (flPos > midExpGm.Get()->mLastChildFlPos) {
            leftExpGroupIndex = midExpGroupIndex + 1;
        } else if (flPos < midExpGm.Get()->mFlPos) {
            rightExpGroupIndex = midExpGroupIndex - 1;
        } else if (flPos == midExpGm.Get()->mFlPos) {
            return PositionMetadata::Obtain(flPos, ExpandableListPosition::GROUP,
                    midExpGm.Get()->mGPos, -1, midExpGm, midExpGroupIndex);
        } else if (flPos <= (midExpGm.Get())->mLastChildFlPos) {
            Int32 childPos = flPos - (midExpGm.Get()->mFlPos + 1);
            return PositionMetadata::Obtain(flPos, ExpandableListPosition::CHILD,
                    midExpGm.Get()->mGPos, childPos, midExpGm, midExpGroupIndex);
        }
    }
    Int32 insertPosition = 0;
    Int32 groupPos = 0;
    if (leftExpGroupIndex > midExpGroupIndex) {

        AutoPtr<GroupMetadata> leftExpGm = egml[leftExpGroupIndex-1];

        insertPosition = leftExpGroupIndex;

        groupPos =
            (flPos - leftExpGm.Get()->mLastChildFlPos) + leftExpGm.Get()->mGPos;
    } else if (rightExpGroupIndex < midExpGroupIndex) {

        AutoPtr<GroupMetadata> rightExpGm = egml[++rightExpGroupIndex];

        insertPosition = rightExpGroupIndex;

        groupPos = rightExpGm.Get()->mGPos - (rightExpGm.Get()->mFlPos - flPos);
    } else {
        //return new runtimeException;
    }

    return PositionMetadata::Obtain(flPos, ExpandableListPosition::GROUP, groupPos, -1,
            NULL, insertPosition);
}

AutoPtr<IPositionMetadata> ExpandableListConnector::GetFlattenedPos(
    /* [in] */ IExpandableListPosition* pos)
{
    List<AutoPtr<GroupMetadata> > egml = mExpGroupMetadataList;
    Int32 numExpGroups = egml.GetSize();

    Int32 leftExpGroupIndex = 0;
    Int32 rightExpGroupIndex = numExpGroups - 1;
    Int32 midExpGroupIndex = 0;
    AutoPtr<GroupMetadata> midExpGm;

    if (numExpGroups == 0) {
        return PositionMetadata::Obtain(((CExpandableListPosition*)pos)->groupPos, ((CExpandableListPosition*)pos)->type,
                ((CExpandableListPosition*)pos)->groupPos, ((CExpandableListPosition*)pos)->childPos, NULL, 0);
    }
    while (leftExpGroupIndex <= rightExpGroupIndex) {
        midExpGroupIndex = (rightExpGroupIndex - leftExpGroupIndex)/2 + leftExpGroupIndex;
        midExpGm = egml[midExpGroupIndex];

        if (((CExpandableListPosition*)pos)->groupPos > midExpGm.Get()->mGPos) {
            leftExpGroupIndex = midExpGroupIndex + 1;
        } else if (((CExpandableListPosition*)pos)->groupPos < midExpGm.Get()->mGPos) {
            rightExpGroupIndex = midExpGroupIndex - 1;
        } else if (((CExpandableListPosition*)pos)->groupPos == midExpGm.Get()->mGPos) {
            if (((CExpandableListPosition*)pos)->type == ExpandableListPosition::GROUP) {
                return PositionMetadata::Obtain(midExpGm.Get()->mFlPos, ((CExpandableListPosition*)pos)->type,
                        ((CExpandableListPosition*)pos)->groupPos, ((CExpandableListPosition*)pos)->childPos, midExpGm, midExpGroupIndex);
            } else if (((CExpandableListPosition*)pos)->type == ExpandableListPosition::CHILD) {
                return PositionMetadata::Obtain(midExpGm.Get()->mFlPos + ((CExpandableListPosition*)pos)->childPos
                        + 1, ((CExpandableListPosition*)pos)->type, ((CExpandableListPosition*)pos)->groupPos, ((CExpandableListPosition*)pos)->childPos,
                        midExpGm, midExpGroupIndex);
            } else {
                return NULL;
            }
        }
    }

    if (((CExpandableListPosition*)pos)->type != ExpandableListPosition::GROUP) {
        return NULL;
    }

    if (leftExpGroupIndex > midExpGroupIndex) {

        AutoPtr<GroupMetadata> leftExpGm = egml[leftExpGroupIndex-1];
        Int32 flPos =
                leftExpGm.Get()->mLastChildFlPos
                        + (((CExpandableListPosition*)pos)->groupPos - leftExpGm.Get()->mGPos);
        return PositionMetadata::Obtain(flPos, ((CExpandableListPosition*)pos)->type, ((CExpandableListPosition*)pos)->groupPos,
                ((CExpandableListPosition*)pos)->childPos, NULL, leftExpGroupIndex);
    } else if (rightExpGroupIndex < midExpGroupIndex) {

        AutoPtr<GroupMetadata> rightExpGm = egml[++rightExpGroupIndex];
        Int32 flPos =
                rightExpGm.Get()->mFlPos - (rightExpGm.Get()->mGPos
                    - ((CExpandableListPosition*)pos)->groupPos);
        return PositionMetadata::Obtain(flPos, ((CExpandableListPosition*)pos)->type, ((CExpandableListPosition*)pos)->groupPos,
                ((CExpandableListPosition*)pos)->childPos, NULL, rightExpGroupIndex);
    } else {
        return NULL;
    }
}

Boolean ExpandableListConnector::AreAllItemsEnabled()
{
    Boolean enabled;
    mExpandableListAdapter->AreAllItemsEnabled(&enabled);
    return enabled;
    return FALSE;
}

Boolean ExpandableListConnector::IsEnabled(
    /* [in] */ Int32 flatListPos)
{
    AutoPtr<IPositionMetadata> metadata = GetUnflattenedPos(flatListPos);
    AutoPtr<IExpandableListPosition> pos = ((PositionMetadata*)(metadata.Get()))->mPosition;

    Boolean retValue;
    if (((CExpandableListPosition*)pos.Get())->type == ExpandableListPosition::CHILD) {
        mExpandableListAdapter->IsChildSelectable(((CExpandableListPosition*)pos.Get())->groupPos, ((CExpandableListPosition*)pos.Get())->childPos, &retValue);
    } else {
        retValue = TRUE;
    }

    metadata->Recycle();

    return retValue;
}

Int32 ExpandableListConnector::GetCount()
{
    Int32 count;
    mExpandableListAdapter->GetGroupCount(&count);

    return count + mTotalExpChildrenCount;
}

AutoPtr<IInterface> ExpandableListConnector::GetItem(
    /* [in] */ Int32 flatListPos)
{
    AutoPtr<IPositionMetadata> posMetadata = GetUnflattenedPos(flatListPos);

    AutoPtr<IInterface> retValue;
    if (((CExpandableListPosition*)((PositionMetadata*)posMetadata.Get())->mPosition.Get())->type == ExpandableListPosition::GROUP) {
        mExpandableListAdapter->GetGroup(((CExpandableListPosition*)((PositionMetadata*)posMetadata.Get())->mPosition.Get())->groupPos, (IInterface**)&retValue);
    }
    else if (((CExpandableListPosition*)((PositionMetadata*)posMetadata.Get())->mPosition.Get())->type == ExpandableListPosition::CHILD) {
        mExpandableListAdapter->GetChild(((CExpandableListPosition*)((PositionMetadata*)posMetadata.Get())->mPosition.Get())->groupPos,
                ((CExpandableListPosition*)(((PositionMetadata*)posMetadata.Get())->mPosition.Get()))->childPos, (IInterface**)&retValue);
    }
    else {
        // TODO: clean exit
        //throw new RuntimeException("Flat list position is of unknown type");
    }

    posMetadata->Recycle();

    return retValue;
}

Int64 ExpandableListConnector::GetItemId(
    /* [in] */ Int32 flatListPos)
{
    AutoPtr<IPositionMetadata> posMetadata = GetUnflattenedPos(flatListPos);
    Int64 groupId = 0;
    mExpandableListAdapter->GetGroupId(((CExpandableListPosition*)((PositionMetadata*)posMetadata.Get())->mPosition.Get())->groupPos, &groupId);

    Int64 retValue = 0;
    if (((CExpandableListPosition*)((PositionMetadata*)posMetadata.Get())->mPosition.Get())->type == ExpandableListPosition::GROUP) {
        mExpandableListAdapter->GetCombinedGroupId(groupId, &retValue);
    } else if (((CExpandableListPosition*)((PositionMetadata*)posMetadata.Get())->mPosition.Get())->type == ExpandableListPosition::CHILD) {
        Int64 childId;
        mExpandableListAdapter->GetChildId(((CExpandableListPosition*)((PositionMetadata*)posMetadata.Get())->mPosition.Get())->groupPos,
                ((CExpandableListPosition*)(((PositionMetadata*)posMetadata.Get())->mPosition.Get()))->childPos, &childId);
        mExpandableListAdapter->GetCombinedChildId(groupId, childId, &retValue);
    } else {
        // TODO: clean exit
        //throw new RuntimeException("Flat list position is of unknown type");
    }

    posMetadata->Recycle();

    return retValue;
}

AutoPtr<IView> ExpandableListConnector::GetView(
    /* [in] */ Int32 flatListPos,
    /* [in] */ IView* convertView,
    /* [in] */ IViewGroup* parent)
{
    AutoPtr<IPositionMetadata> posMetadata = GetUnflattenedPos(flatListPos);

    AutoPtr<IView> retValue = NULL;
    Boolean expanded = FALSE;
    if (((CExpandableListPosition*)((PositionMetadata*)posMetadata.Get())->mPosition.Get())->type == ExpandableListPosition::GROUP) {
        mExpandableListAdapter->GetGroupView(((CExpandableListPosition*)((PositionMetadata*)posMetadata.Get())->mPosition.Get())->groupPos,
                (((PositionMetadata*)posMetadata.Get())->IsExpanded(&expanded), expanded), convertView, parent, (IView**)&retValue);
    } else if (((CExpandableListPosition*)((PositionMetadata*)posMetadata.Get())->mPosition.Get())->type == ExpandableListPosition::CHILD) {
        Boolean isLastChild = ((GroupMetadata*)(((PositionMetadata*)posMetadata.Get())->mGroupMetadata.Get()))->mLastChildFlPos == flatListPos;

        mExpandableListAdapter->GetChildView(((CExpandableListPosition*)((PositionMetadata*)posMetadata.Get())->mPosition.Get())->groupPos,
                ((CExpandableListPosition*)((PositionMetadata*)posMetadata.Get())->mPosition.Get())->childPos, isLastChild, convertView, parent, (IView**)&retValue);
    } else {
        // TODO: clean exit
        //throw new RuntimeException("Flat list position is of unknown type");
    }

    posMetadata->Recycle();

    return retValue;
}

Int32 ExpandableListConnector::GetItemViewType(
    /* [in] */ Int32 flatListPos)
{

    AutoPtr<IPositionMetadata> metadata = GetUnflattenedPos(flatListPos);
    AutoPtr<IExpandableListPosition> pos = ((PositionMetadata*)(metadata.Get()))->mPosition;

    Int32 retValue = 0;
    AutoPtr<IHeterogeneousExpandableList> lh = (IHeterogeneousExpandableList*)IExpandableListAdapter::Probe(mExpandableListAdapter);
    if (lh != NULL) {
        AutoPtr<IHeterogeneousExpandableList> adapter =
                (IHeterogeneousExpandableList*) (mExpandableListAdapter);
        if (((CExpandableListPosition*)pos.Get())->type == ExpandableListPosition::GROUP) {
            adapter->GetGroupType(((CExpandableListPosition*)pos.Get())->groupPos, &retValue);
        } else {
            Int32 childType;
            adapter->GetChildType(((CExpandableListPosition*)pos.Get())->groupPos, ((CExpandableListPosition*)pos.Get())->childPos, &childType);
            adapter->GetGroupTypeCount(&retValue);
            retValue += childType;
        }
    } else {
        if (((CExpandableListPosition*)pos.Get())->type == ExpandableListPosition::GROUP) {
            retValue = 0;
        } else {
            retValue = 1;
        }
    }

    metadata->Recycle();

    return retValue;
}

Int32 ExpandableListConnector::GetViewTypeCount()
{
    if (mExpandableListAdapter->Probe(EIID_IHeterogeneousExpandableList)) {
        AutoPtr<IHeterogeneousExpandableList> adapter =
                (IHeterogeneousExpandableList*) mExpandableListAdapter;
        Int32 groupCount = 0;
        Int32 childCount = 0;
        adapter->GetGroupTypeCount(&groupCount);
        adapter->GetChildTypeCount(&childCount);
        return groupCount + childCount;
    } else {
        return 2;
    }
}

Boolean ExpandableListConnector::HasStableIds()
{
    Boolean ret = FALSE;
    mExpandableListAdapter->HasStableIds(&ret);
    return ret;
}

void ExpandableListConnector::RefreshExpGroupMetadataList(
    /* [in] */ Boolean forceChildrenCountRefresh,
    /* [in] */ Boolean syncGroupPositions)
{
    List<AutoPtr<GroupMetadata> > egml = mExpGroupMetadataList;
    Int32 egmlSize = egml.GetSize();
    Int32 curFlPos = 0;

    mTotalExpChildrenCount = 0;

    if (syncGroupPositions) {
        // We need to check whether any groups have moved positions
        Boolean positionsChanged = FALSE;

        for (Int32 i = egmlSize - 1; i >= 0; i--) {
            AutoPtr<GroupMetadata> curGm = egml[i];
            Int32 newGPos = FindGroupPosition(curGm.Get()->mGId, curGm.Get()->mGPos);
            if (newGPos != curGm.Get()->mGPos) {
                if (newGPos == IAdapterView::INVALID_POSITION) {
                    // Doh, just remove it from the list of expanded groups
                    egml.Remove(i);
                    egmlSize--;
                }

                curGm.Get()->mGPos = newGPos;
                if (!positionsChanged) positionsChanged = TRUE;
            }
        }

        if (positionsChanged) {
            //
            //jiazhenjiang
            //
            //Collections.sort(egml);
        }
    }

    Int32 gChildrenCount = 0;
    Int32 lastGPos = 0;
    for (Int32 i = 0; i < egmlSize; i++) {
        AutoPtr<GroupMetadata> curGm = egml[i];
        if ((curGm.Get()->mLastChildFlPos == GroupMetadata::REFRESH) || forceChildrenCountRefresh) {
            mExpandableListAdapter->GetChildrenCount(curGm.Get()->mGPos, &gChildrenCount);
        } else {
            gChildrenCount = curGm.Get()->mLastChildFlPos - curGm.Get()->mFlPos;
        }

        mTotalExpChildrenCount += gChildrenCount;

        curFlPos += (curGm.Get()->mGPos - lastGPos);
        lastGPos = curGm.Get()->mGPos;

        curGm.Get()->mFlPos = curFlPos;
        curFlPos += gChildrenCount;
        curGm.Get()->mLastChildFlPos = curFlPos;
    }
}

Boolean ExpandableListConnector::CollapseGroup(
    /* [in] */ Int32 groupPos)
{
    AutoPtr<IExpandableListPosition> elGroupPos = ExpandableListPosition::Obtain(
        ExpandableListPosition::GROUP, groupPos, -1, -1);
    AutoPtr<IPositionMetadata> pm = GetFlattenedPos(elGroupPos);
    elGroupPos->Recycle();

    if (pm == NULL) return FALSE;

    Boolean retValue = CollapseGroup(pm);
    pm->Recycle();
    return retValue;
}

Boolean ExpandableListConnector::CollapseGroup(
    /* [in] */ IPositionMetadata* posMetadata)
{
    if (((PositionMetadata*)posMetadata)->mGroupMetadata == NULL) return FALSE;

    // Remove the group from the list of expanded groups
    mExpGroupMetadataList.Remove((GroupMetadata*)(((PositionMetadata*)posMetadata)->mGroupMetadata.Get()));

    // Refresh the metadata
    RefreshExpGroupMetadataList(FALSE, FALSE);

    // Notify of change
    NotifyDataSetChanged();

    // Give the callback
    mExpandableListAdapter->OnGroupCollapsed(((GroupMetadata*)((PositionMetadata*)posMetadata)->mGroupMetadata.Get())->mGPos);

    return TRUE;
}

Boolean ExpandableListConnector::ExpandGroup(
    /* [in] */ Int32 groupPos)
{
    AutoPtr<IExpandableListPosition> elGroupPos = ExpandableListPosition::Obtain(
        ExpandableListPosition::GROUP, groupPos, -1, -1);
    AutoPtr<IPositionMetadata> pm = GetFlattenedPos(elGroupPos);
    elGroupPos->Recycle();

    Boolean retValue = ExpandGroup(pm);
    pm->Recycle();
    return retValue;
}

Boolean ExpandableListConnector::ExpandGroup(
    /* [in] */ IPositionMetadata* posMetadata)
{
    if (((CExpandableListPosition*)((PositionMetadata*)posMetadata)->mPosition.Get())->groupPos < 0) {
        // TODO clean exit
        //throw new RuntimeException("Need group");
    }

    if (mMaxExpGroupCount == 0) return FALSE;

    if (((PositionMetadata*)posMetadata)->mGroupMetadata != NULL) return FALSE;

    if (mExpGroupMetadataList.GetSize() >= mMaxExpGroupCount) {
        AutoPtr<GroupMetadata> collapsedGm = mExpGroupMetadataList[0];

        Int32 collapsedIndex = GetIndexOf(mExpGroupMetadataList, collapsedGm);

        CollapseGroup(collapsedGm.Get()->mGPos);

        if (((PositionMetadata*)posMetadata)->mGroupInsertIndex > collapsedIndex) {
            ((PositionMetadata*)posMetadata)->mGroupInsertIndex--;
        }
    }

     Int64 id = 0;

    AutoPtr<IGroupMetadata> expandedGm = GroupMetadata::Obtain(
        GroupMetadata::REFRESH,
        GroupMetadata::REFRESH,
            ((CExpandableListPosition*)((PositionMetadata*)posMetadata)->mPosition.Get())->groupPos,
            (mExpandableListAdapter->GetGroupId(((CExpandableListPosition*)((PositionMetadata*)posMetadata)->mPosition.Get())->groupPos, &id),id));

    AutoPtr<GroupMetadata> expandedGmAuto = (GroupMetadata*)expandedGm.Get();
    mExpGroupMetadataList.Insert(((PositionMetadata*)posMetadata)->mGroupInsertIndex, expandedGmAuto);

    // Refresh the metadata
    RefreshExpGroupMetadataList(FALSE, FALSE);

    // Notify of change
    NotifyDataSetChanged();

    // Give the callback
    mExpandableListAdapter->OnGroupExpanded(((GroupMetadata*)expandedGm.Get())->mGPos);

    return TRUE;
}

Boolean ExpandableListConnector::IsGroupExpanded(
    /* [in] */ Int32 groupPosition)
{
    AutoPtr<GroupMetadata> groupMetadata;
    for (Int32 i = mExpGroupMetadataList.GetSize() - 1; i >= 0; i--) {
        groupMetadata = mExpGroupMetadataList[i];

        if (groupMetadata.Get()->mGPos == groupPosition) {
            return TRUE;
        }
    }

    return FALSE;
}

ECode ExpandableListConnector::SetMaxExpGroupCount(
    /* [in] */ Int32 maxExpGroupCount)
{
    mMaxExpGroupCount = maxExpGroupCount;

    return NOERROR;
}

AutoPtr<IExpandableListAdapter> ExpandableListConnector::GetAdapter()
{
    return mExpandableListAdapter;
}

AutoPtr<IFilter> ExpandableListConnector::GetFilter()
{
    AutoPtr<IExpandableListAdapter> adapter = GetAdapter();
    if (adapter->Probe(EIID_IFilterable)) {

        AutoPtr<IFilter> filter = NULL;
        ((IFilterable*) adapter.Get())->GetFilter((IFilter**)&filter);
        return filter;
    } else {
        return NULL;
    }
}

List<AutoPtr<GroupMetadata> > ExpandableListConnector::GetExpandedGroupMetadataList()
{
    return mExpGroupMetadataList;
}

ECode ExpandableListConnector::SetExpandedGroupMetadataList(
    /* [in] */ List<AutoPtr<GroupMetadata> > expandedGroupMetadataList)
{

    if ((expandedGroupMetadataList.IsEmpty()) || (mExpandableListAdapter == NULL)) {
        return NOERROR;
    }

    Int32 numGroups = 0;
    mExpandableListAdapter->GetGroupCount(&numGroups);
    for (Int32 i = expandedGroupMetadataList.GetSize() - 1; i >= 0; i--) {
        if (((GroupMetadata*)expandedGroupMetadataList[i])->mGPos >= numGroups) {
            return NOERROR;
        }
    }

    mExpGroupMetadataList = expandedGroupMetadataList;
    RefreshExpGroupMetadataList(TRUE, FALSE);
    return NOERROR;
}

Boolean ExpandableListConnector::IsEmpty()
{
    Boolean empty = FALSE;
    AutoPtr<IExpandableListAdapter> adapter = GetAdapter();
    if (adapter) {
        adapter->IsEmpty(&empty);
    }
    return empty;
}

Int32 ExpandableListConnector::FindGroupPosition(
    /* [in] */ Int64 groupIdToMatch,
    /* [in] */ Int32 seedGroupPosition)
{
    Int32 count;
    mExpandableListAdapter->GetGroupCount(&count);

    if (count == 0) {
        return IAdapterView::INVALID_POSITION;
    }

    // If there isn't a selection don't hunt for it
    if (groupIdToMatch == IAdapterView::INVALID_ROW_ID) {
        return IAdapterView::INVALID_POSITION;
    }

    // Pin seed to reasonable values
    seedGroupPosition = Elastos::Core::Math::Max(0, seedGroupPosition);
    seedGroupPosition = Elastos::Core::Math::Min(count - 1, seedGroupPosition);

    Int64 endTime = SystemClock::GetUptimeMillis() + AdapterView::SYNC_MAX_DURATION_MILLIS;

    Int64 rowId;

    // first position scanned so far
    Int32 first = seedGroupPosition;

    // last position scanned so far
    Int32 last = seedGroupPosition;

    // True if we should move down on the next iteration
    Boolean next = FALSE;

    // True when we have looked at the first item in the data
    Boolean hitFirst;

    // True when we have looked at the last item in the data
    Boolean hitLast;

    // Get the item ID locally (instead of getItemIdAtPosition), so
    // we need the adapter
    AutoPtr<IExpandableListAdapter> adapter = GetAdapter();
    if (adapter.Get() == NULL) {
        return IAdapterView::INVALID_POSITION;
    }

    while (SystemClock::GetUptimeMillis() <= endTime) {
        adapter->GetGroupId(seedGroupPosition, &rowId);
        if (rowId == groupIdToMatch) {
            // Found it!
            return seedGroupPosition;
        }

        hitLast = last == count - 1;
        hitFirst = first == 0;

        if (hitLast && hitFirst) {
            // Looked at everything
            break;
        }

        if (hitFirst || (next && !hitLast)) {
            // Either we hit the top, or we are trying to move down
            last++;
            seedGroupPosition = last;
            // Try going up next time
            next = FALSE;
        } else if (hitLast || (!next && !hitFirst)) {
            // Either we hit the bottom, or we are trying to move up
            first--;
            seedGroupPosition = first;
            // Try going down next time
            next = TRUE;
        }

    }

    return IAdapterView::INVALID_POSITION;
}

ExpandableListConnector::MyDataSetObserver::MyDataSetObserver(
    /* [in] */ ExpandableListConnector* owner)
{
    mOwner = owner;
}
PInterface ExpandableListConnector::MyDataSetObserver::Probe(
    /* [in] */ REIID riid)
{
    return NULL;
}

UInt32 ExpandableListConnector::MyDataSetObserver::AddRef()
{
    return ElRefBase::AddRef();
}

UInt32 ExpandableListConnector::MyDataSetObserver::Release()
{
    return ElRefBase::Release();
}

ECode ExpandableListConnector::MyDataSetObserver::GetInterfaceID(
    /* [in] */ IInterface *pObject,
    /* [out] */ InterfaceID *pIID)
{
    return E_NOT_IMPLEMENTED;
}

ECode ExpandableListConnector::MyDataSetObserver::OnChanged()
{
    mOwner->RefreshExpGroupMetadataList(TRUE, TRUE);

    //NotifyDataSetChanged();
    return NOERROR;
}

ECode ExpandableListConnector::MyDataSetObserver::OnInvalidated()
{
    mOwner->RefreshExpGroupMetadataList(TRUE, TRUE);

    //NotifyDataSetInvalidated();
    return NOERROR;
}

Int32 ExpandableListConnector::GetIndexOf(
    /* [in] */ List<AutoPtr<GroupMetadata> > list,
    /* [in] */ AutoPtr<GroupMetadata> gm)
{
    Int32 count = 0;
    List<AutoPtr<GroupMetadata> >::Iterator iter = list.Begin();
    while(iter != list.End()) {
        if(*iter == gm){
            break;
        } else {
            ++count;
            ++iter;
        }
    }
    return count;
}

} // namespace Widget
} // namespace Droid
} // namespace Elastos
