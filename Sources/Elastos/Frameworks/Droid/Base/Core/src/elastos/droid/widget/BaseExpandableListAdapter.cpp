
#include "elastos/droid/widget/BaseExpandableListAdapter.h"

namespace Elastos {
namespace Droid {
namespace Widget {

BaseExpandableListAdapter::BaseExpandableListAdapter()
{
    CDataSetObservable::New((IDataSetObservable**)&mDataSetObservable);
}

ECode BaseExpandableListAdapter::RegisterDataSetObserver(
    /* [in] */ IDataSetObserver* observer)
{
    mDataSetObservable->RegisterObserver(observer);
    return NOERROR;
}

ECode BaseExpandableListAdapter::UnregisterDataSetObserver(
    /* [in] */ IDataSetObserver* observer)
{
    return mDataSetObservable->UnregisterObserver(observer);
}

/**
 * @see DataSetObservable#notifyInvalidated()
 */
ECode BaseExpandableListAdapter::NotifyDataSetInvalidated()
{
    return mDataSetObservable->NotifyInvalidated();
}

/**
 * @see DataSetObservable#notifyChanged()
 */
ECode BaseExpandableListAdapter::NotifyDataSetChanged()
{
    return mDataSetObservable->NotifyChanged();
}

Boolean BaseExpandableListAdapter::AreAllItemsEnabled()
{
    return TRUE;
}

ECode BaseExpandableListAdapter::OnGroupCollapsed(
    /* [in] */ Int32 groupPosition)
{
    return NOERROR;
}

ECode BaseExpandableListAdapter::OnGroupExpanded(
    /* [in] */ Int32 groupPosition)
{
    return NOERROR;
}

/**
 * Override this method if you foresee a clash in IDs based on this scheme:
 * <p>
 * Base implementation returns a Int64:
 * <li> bit 0: Whether this ID points to a child (unset) or group (set), so for this method
 *             this bit will be 1.
 * <li> bit 1-31: Lower 31 bits of the groupId
 * <li> bit 32-63: Lower 32 bits of the childId.
 * <p>
 * {@inheritDoc}
 */
Int64 BaseExpandableListAdapter::GetCombinedChildId(
    /* [in] */ Int64 groupId,
    /* [in] */ Int64 childId)
{
    return 0x8000000000000000L | ((groupId & 0x7FFFFFFF) << 32) | (childId & 0xFFFFFFFF);
}

/**
 * Override this method if you foresee a clash in IDs based on this scheme:
 * <p>
 * Base implementation returns a Int64:
 * <li> bit 0: Whether this ID points to a child (unset) or group (set), so for this method
 *             this bit will be 0.
 * <li> bit 1-31: Lower 31 bits of the groupId
 * <li> bit 32-63: Lower 32 bits of the childId.
 * <p>
 * {@inheritDoc}
 */
Int64 BaseExpandableListAdapter::GetCombinedGroupId(
    /* [in] */ Int64 groupId)
{
    return (groupId & 0x7FFFFFFF) << 32;
}

/**
 * {@inheritDoc}
 */
Boolean BaseExpandableListAdapter::IsEmpty()
{
    return GetGroupCount() == 0;
}


/**
 * {@inheritDoc}
 * @return 0 for any group or child position, since only one child type count is declared.
 */
Int32 BaseExpandableListAdapter::GetChildType(
    /* [in] */ Int32 groupPosition,
    /* [in] */ Int32 childPosition)
{
    return 0;
}

/**
 * {@inheritDoc}
 * @return 1 as a default value in BaseExpandableListAdapter.
 */
Int32 BaseExpandableListAdapter::GetChildTypeCount()
{
    return 1;
}

/**
 * {@inheritDoc}
 * @return 0 for any groupPosition, since only one group type count is declared.
 */
Int32 BaseExpandableListAdapter::GetGroupType(
    /* [in] */ Int32 groupPosition)
{
    return 0;
}

/**
 * {@inheritDoc}
 * @return 1 as a default value in BaseExpandableListAdapter.
 */
Int32 BaseExpandableListAdapter::GetGroupTypeCount()
{
    return 1;
}

}// namespace Elastos
}// namespace Droid
}// namespace Widget
