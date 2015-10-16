
#ifndef __ELASTOS_DROID_WIDGET_BASEEXPANDABLELISTADAPTER_H__
#define __ELASTOS_DROID_WIDGET_BASEEXPANDABLELISTADAPTER_H__

#include "elastos/droid/ext/frameworkext.h"
#include "elastos/droid/database/CDataSetObservable.h"

using Elastos::Droid::Database::IDataSetObserver;
using Elastos::Droid::Database::IDataSetObservable;
using Elastos::Droid::Database::CDataSetObservable;
namespace Elastos {
namespace Droid {
namespace Widget {

/**
 * Base class for a {@link ExpandableListAdapter} used to provide data and Views
 * from some data to an expandable list view.
 * <p>
 * Adapters inheriting this class should verify that the base implementations of
 * {@link #getCombinedChildId(Int64, Int64)} and {@link #getCombinedGroupId(Int64)}
 * are correct in generating unique IDs from the group/children IDs.
 * <p>
 * @see SimpleExpandableListAdapter
 * @see SimpleCursorTreeAdapter
 */
class BaseExpandableListAdapter
{
public:
    BaseExpandableListAdapter();

    virtual CARAPI RegisterDataSetObserver(
        /* [in] */ IDataSetObserver* observer);

    virtual CARAPI UnregisterDataSetObserver(
        /* [in] */ IDataSetObserver* observer);

    /**
     * @see DataSetObservable#notifyInvalidated()
     */
    virtual CARAPI NotifyDataSetInvalidated();

    /**
     * @see DataSetObservable#notifyChanged()
     */
    virtual CARAPI NotifyDataSetChanged();

    virtual CARAPI_(Boolean) AreAllItemsEnabled();

    virtual CARAPI OnGroupCollapsed(
        /* [in] */ Int32 groupPosition);

    virtual CARAPI OnGroupExpanded(
        /* [in] */ Int32 groupPosition);

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
    virtual CARAPI_(Int64) GetCombinedChildId(
        /* [in] */ Int64 groupId,
        /* [in] */ Int64 childId);

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
    virtual CARAPI_(Int64) GetCombinedGroupId(
        /* [in] */ Int64 groupId);

    /**
     * {@inheritDoc}
     */
    virtual CARAPI_(Boolean) IsEmpty();


    /**
     * {@inheritDoc}
     * @return 0 for any group or child position, since only one child type count is declared.
     */
    virtual CARAPI_(Int32) GetChildType(
        /* [in] */ Int32 groupPosition,
        /* [in] */ Int32 childPosition);

    /**
     * {@inheritDoc}
     * @return 1 as a default value in BaseExpandableListAdapter.
     */
    virtual CARAPI_(Int32) GetChildTypeCount();

    /**
     * {@inheritDoc}
     * @return 0 for any groupPosition, since only one group type count is declared.
     */
    virtual CARAPI_(Int32) GetGroupType(
        /* [in] */ Int32 groupPosition);

    /**
     * {@inheritDoc}
     * @return 1 as a default value in BaseExpandableListAdapter.
     */
    virtual CARAPI_(Int32) GetGroupTypeCount();

    virtual CARAPI_(Int32) GetGroupCount() = 0;

private:
    AutoPtr<IDataSetObservable> mDataSetObservable ;

};

}// namespace Elastos
}// namespace Droid
}// namespace Widget

#endif
