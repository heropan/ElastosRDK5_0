
#ifndef __ELASTOS_DROID_WIDGET_EXPANDABLELISTCONNECTOR_H__
#define __ELASTOS_DROID_WIDGET_EXPANDABLELISTCONNECTOR_H__

#include "elastos/droid/ext/frameworkext.h"
#include "widget/ExpandableListPosition.h"
#include "widget/BaseAdapter.h"
#include "widget/GroupMetadata.h"
#include "widget/PositionMetadata.h"
#include <elastos/core/Math.h>

namespace Elastos {
namespace Droid {
namespace Widget {

/*
 * Implementation notes:
 *
 * <p>
 * Terminology:
 * <li> flPos - Flat list position, the position used by ListView
 * <li> gPos - Group position, the position of a group among all the groups
 * <li> cPos - Child position, the position of a child among all the children
 * in a group
 */

/**
 * A {@link BaseAdapter} that provides data/Views in an expandable list (offers
 * features such as collapsing/expanding groups containing children). By
 * itself, this adapter has no data and is a connector to a
 * {@link ExpandableListAdapter} which provides the data.
 * <p>
 * Internally, this connector translates the flat list position that the
 * ListAdapter expects to/from group and child positions that the ExpandableListAdapter
 * expects.
 */
class ExpandableListConnector : public BaseAdapter
{
public:
    ExpandableListConnector();
    /**
     * Constructs the connector
     */
    ExpandableListConnector(
        /* [in] */ IExpandableListAdapter* expandableListAdapter);

    /**
     * Point to the {@link ExpandableListAdapter} that will give us data/Views
     *
     * @param expandableListAdapter the adapter that supplies us with data/Views
     */
    virtual CARAPI SetExpandableListAdapter(
        /* [in] */ IExpandableListAdapter* expandableListAdapter);

    /**
     * Translates a flat list position to either a) group pos if the specified
     * flat list position corresponds to a group, or b) child pos if it
     * corresponds to a child.  Performs a binary search on the expanded
     * groups list to find the flat list pos if it is an exp group, otherwise
     * finds where the flat list pos fits in between the exp groups.
     *
     * @param flPos the flat list position to be translated
     * @return the group position or child position of the specified flat list
     *         position encompassed in a {@link PositionMetadata} object
     *         that contains additional useful info for insertion, etc.
     */
    virtual CARAPI_(AutoPtr<IPositionMetadata>) GetUnflattenedPos(
        /* [in] */ Int32 flPos);

    /**
     * Translates either a group pos or a child pos (+ group it belongs to) to a
     * flat list position.  If searching for a child and its group is not expanded, this will
     * return NULL since the child isn't being shown in the ListView, and hence it has no
     * position.
     *
     * @param pos a {@link ExpandableListPosition} representing either a group position
     *        or child position
     * @return the flat list position encompassed in a {@link PositionMetadata}
     *         object that contains additional useful info for insertion, etc., or NULL.
     */
    virtual CARAPI_(AutoPtr<IPositionMetadata>) GetFlattenedPos(
        /* [in] */ IExpandableListPosition* pos);

    virtual CARAPI_(Boolean) AreAllItemsEnabled();

    virtual CARAPI_(Boolean) IsEnabled(
        /* [in] */ Int32 flatListPos);

    virtual CARAPI_(Int32) GetCount();

    virtual CARAPI_(AutoPtr<IInterface>) GetItem(
        /* [in] */ Int32 flatListPos);

    virtual CARAPI_(Int64) GetItemId(
        /* [in] */ Int32 flatListPos);

    virtual CARAPI_(AutoPtr<IView>) GetView(
        /* [in] */ Int32 flatListPos,
        /* [in] */ IView* convertView,
        /* [in] */ IViewGroup* parent);

    virtual CARAPI_(Int32) GetItemViewType(
        /* [in] */ Int32 flatListPos);

    virtual CARAPI_(Int32) GetViewTypeCount();

    virtual CARAPI_(Boolean) HasStableIds();

private:
    /**
     * Traverses the expanded group metadata list and fills in the flat list
     * positions.
     *
     * @param forceChildrenCountRefresh Forces refreshing of the children count
     *        for all expanded groups.
     * @param syncGroupPositions Whether to search for the group positions
     *         based on the group IDs. This should only be needed when calling
     *         this from an onChanged callback.
     */
    virtual CARAPI_(void) RefreshExpGroupMetadataList(
        /* [in] */ Boolean forceChildrenCountRefresh,
        /* [in] */ Boolean syncGroupPositions);

    CARAPI_(Int32) GetIndexOf(
        /* [in] */ List<AutoPtr<GroupMetadata> > list,
        /* [in] */ AutoPtr<GroupMetadata> gm);

public:
    /**
     * Collapse a group in the grouped list view
     *
     * @param groupPos position of the group to collapse
     */
    virtual CARAPI_(Boolean) CollapseGroup(
        /* [in] */ Int32 groupPos);

    virtual CARAPI_(Boolean) CollapseGroup(
        /* [in] */ IPositionMetadata* posMetadata);

    /**
     * Expand a group in the grouped list view
     * @param groupPos the group to be expanded
     */
    virtual CARAPI_(Boolean) ExpandGroup(
        /* [in] */ Int32 groupPos);

    virtual CARAPI_(Boolean) ExpandGroup(
        /* [in] */ IPositionMetadata* posMetadata);

    /**
     * Whether the given group is currently expanded.
     * @param groupPosition The group to check.
     * @return Whether the group is currently expanded.
     */
    virtual CARAPI_(Boolean) IsGroupExpanded(
        /* [in] */ Int32 groupPosition);

    /**
     * Set the maximum number of groups that can be expanded at any given time
     */
    virtual CARAPI SetMaxExpGroupCount(
        /* [in] */ Int32 maxExpGroupCount);

    virtual CARAPI_(AutoPtr<IExpandableListAdapter>) GetAdapter();

    virtual CARAPI_(AutoPtr<IFilter>) GetFilter();

    virtual CARAPI_(List<AutoPtr<GroupMetadata> >) GetExpandedGroupMetadataList();

    virtual CARAPI SetExpandedGroupMetadataList(
        /* [in] */ List<AutoPtr<GroupMetadata> > expandedGroupMetadataList);

    virtual CARAPI_(Boolean) IsEmpty();

    /**
     * Searches the expandable list adapter for a group position matching the
     * given group ID. The search starts at the given seed position and then
     * alternates between moving up and moving down until 1) we find the right
     * position, or 2) we run out of time, or 3) we have looked at every
     * position
     *
     * @return Position of the row that matches the given row ID, or
     *         {@link AdapterView#INVALID_POSITION} if it can't be found
     * @see AdapterView#findSyncPosition()
     */
    virtual CARAPI_(Int32) FindGroupPosition(
        /* [in] */ Int64 groupIdToMatch,
        /* [in] */ Int32 seedGroupPosition);

    class MyDataSetObserver
        : public ElRefBase
        , public IDataSetObserver
    {
    public:
        MyDataSetObserver(
            /* [in] */ ExpandableListConnector* owner);

        CAR_INTERFACE_DECL()

        virtual CARAPI OnChanged();

        virtual CARAPI OnInvalidated();

        ExpandableListConnector* mOwner;
    };

private:
    /**
     * The ExpandableListAdapter to fetch the data/Views for this expandable list
     */
    // expandableListAdapter usually holds adapter, we use weak-reference here.
    IExpandableListAdapter* mExpandableListAdapter;

    /**
     * List of metadata for the currently expanded groups. The metadata consists
     * of data essential for efficiently translating between flat list positions
     * and group/child positions. See {@link GroupMetadata}.
     */
    List<AutoPtr<GroupMetadata> > mExpGroupMetadataList;

    /** The number of children from all currently expanded groups */
    Int32 mTotalExpChildrenCount;

    /** The maximum number of allowable expanded groups. Defaults to 'no limit' */
    Int32 mMaxExpGroupCount;// = Math::INT32_MAX_VALUE;

    /** Change observer used to have ExpandableListAdapter changes pushed to us */
    AutoPtr<IDataSetObserver> mDataSetObserver;// = new MyDataSetObserver();
};

} // namespace Widget
} // namespace Droid
} // namespace Elastos

#endif //__ELASTOS_DROID_WIDGET_EXPANDABLELISTCONNECTOR_H__
