
#ifndef __ELASTOS_DROID_WIDGET_SIMPLEEXPANDABLELISTADAPTER_H__
#define __ELASTOS_DROID_WIDGET_SIMPLEEXPANDABLELISTADAPTER_H__

#include "elastos/droid/widget/BaseExpandableListAdapter.h"

using Elastos::Utility::IObjectStringMap;
using Elastos::Utility::CObjectStringMap;
using Elastos::Droid::View::IView;
using Elastos::Droid::View::IViewGroup;
using Elastos::Droid::Content::IContext;
using Elastos::Droid::View::ILayoutInflater;

namespace Elastos {
namespace Droid {
namespace Widget {

/**
 * An easy adapter to map static data to group and child views defined in an XML
 * file. You can separately specify the data backing the group as a List of
 * Maps. Each entry in the ArrayList corresponds to one group in the expandable
 * list. The Maps contain the data for each row. You also specify an XML file
 * that defines the views used to display a group, and a mapping from keys in
 * the Map to specific views. This process is similar for a child, except it is
 * one-level deeper so the data backing is specified as a List<List<Map>>,
 * where the first List corresponds to the group of the child, the second List
 * corresponds to the position of the child within the group, and finally the
 * Map holds the data for that particular child.
 */
class SimpleExpandableListAdapter : public BaseExpandableListAdapter
{
public:
    /**
     * Constructor
     *
     * @param context The context where the {@link ExpandableListView}
     *            associated with this {@link SimpleExpandableListAdapter} is
     *            running
     * @param groupData A List of Maps. Each entry in the List corresponds to
     *            one group in the list. The Maps contain the data for each
     *            group, and should include all the entries specified in
     *            "groupFrom"
     * @param groupFrom A list of keys that will be fetched from the Map
     *            associated with each group.
     * @param groupTo The group views that should display column in the
     *            "groupFrom" parameter. These should all be TextViews. The
     *            first N views in this list are given the values of the first N
     *            columns in the groupFrom parameter.
     * @param groupLayout resource identifier of a view layout that defines the
     *            views for a group. The layout file should include at least
     *            those named views defined in "groupTo"
     * @param childData A List of List of Maps. Each entry in the outer List
     *            corresponds to a group (index by group position), each entry
     *            in the inner List corresponds to a child within the group
     *            (index by child position), and the Map corresponds to the data
     *            for a child (index by values in the childFrom array). The Map
     *            contains the data for each child, and should include all the
     *            entries specified in "childFrom"
     * @param childFrom A list of keys that will be fetched from the Map
     *            associated with each child.
     * @param childTo The child views that should display column in the
     *            "childFrom" parameter. These should all be TextViews. The
     *            first N views in this list are given the values of the first N
     *            columns in the childFrom parameter.
     * @param childLayout resource identifier of a view layout that defines the
     *            views for a child. The layout file should include at least
     *            those named views defined in "childTo"
     */
    SimpleExpandableListAdapter(
        /* [in] */ IContext* context,
        /* [in] */ IObjectContainer* groupData,
        /* [in] */ Int32 groupLayout,
        /* [in] */ ArrayOf<String>* groupFrom,
        /* [in] */ ArrayOf<Int32>* groupTo,
        /* [in] */ IObjectContainer* childData,
        /* [in] */ Int32 childLayout,
        /* [in] */ ArrayOf<String>* childFrom,
        /* [in] */ ArrayOf<Int32>* childTo);

    /**
     * Constructor
     *
     * @param context The context where the {@link ExpandableListView}
     *            associated with this {@link SimpleExpandableListAdapter} is
     *            running
     * @param groupData A List of Maps. Each entry in the List corresponds to
     *            one group in the list. The Maps contain the data for each
     *            group, and should include all the entries specified in
     *            "groupFrom"
     * @param groupFrom A list of keys that will be fetched from the Map
     *            associated with each group.
     * @param groupTo The group views that should display column in the
     *            "groupFrom" parameter. These should all be TextViews. The
     *            first N views in this list are given the values of the first N
     *            columns in the groupFrom parameter.
     * @param expandedGroupLayout resource identifier of a view layout that
     *            defines the views for an expanded group. The layout file
     *            should include at least those named views defined in "groupTo"
     * @param collapsedGroupLayout resource identifier of a view layout that
     *            defines the views for a collapsed group. The layout file
     *            should include at least those named views defined in "groupTo"
     * @param childData A List of List of Maps. Each entry in the outer List
     *            corresponds to a group (index by group position), each entry
     *            in the inner List corresponds to a child within the group
     *            (index by child position), and the Map corresponds to the data
     *            for a child (index by values in the childFrom array). The Map
     *            contains the data for each child, and should include all the
     *            entries specified in "childFrom"
     * @param childFrom A list of keys that will be fetched from the Map
     *            associated with each child.
     * @param childTo The child views that should display column in the
     *            "childFrom" parameter. These should all be TextViews. The
     *            first N views in this list are given the values of the first N
     *            columns in the childFrom parameter.
     * @param childLayout resource identifier of a view layout that defines the
     *            views for a child. The layout file should include at least
     *            those named views defined in "childTo"
     */
    SimpleExpandableListAdapter(
        /* [in] */ IContext* context,
        /* [in] */ IObjectContainer* groupData,
        /* [in] */ Int32 expandedGroupLayout,
        /* [in] */ Int32 collapsedGroupLayout,
        /* [in] */ ArrayOf<String>* groupFrom,
        /* [in] */ ArrayOf<Int32>* groupTo,
        /* [in] */ IObjectContainer* childData,
        /* [in] */ Int32 childLayout,
        /* [in] */ ArrayOf<String>* childFrom,
        /* [in] */ ArrayOf<Int32>* childTo);


    /**
     * Constructor
     *
     * @param context The context where the {@link ExpandableListView}
     *            associated with this {@link SimpleExpandableListAdapter} is
     *            running
     * @param groupData A List of Maps. Each entry in the List corresponds to
     *            one group in the list. The Maps contain the data for each
     *            group, and should include all the entries specified in
     *            "groupFrom"
     * @param groupFrom A list of keys that will be fetched from the Map
     *            associated with each group.
     * @param groupTo The group views that should display column in the
     *            "groupFrom" parameter. These should all be TextViews. The
     *            first N views in this list are given the values of the first N
     *            columns in the groupFrom parameter.
     * @param expandedGroupLayout resource identifier of a view layout that
     *            defines the views for an expanded group. The layout file
     *            should include at least those named views defined in "groupTo"
     * @param collapsedGroupLayout resource identifier of a view layout that
     *            defines the views for a collapsed group. The layout file
     *            should include at least those named views defined in "groupTo"
     * @param childData A List of List of Maps. Each entry in the outer List
     *            corresponds to a group (index by group position), each entry
     *            in the inner List corresponds to a child within the group
     *            (index by child position), and the Map corresponds to the data
     *            for a child (index by values in the childFrom array). The Map
     *            contains the data for each child, and should include all the
     *            entries specified in "childFrom"
     * @param childFrom A list of keys that will be fetched from the Map
     *            associated with each child.
     * @param childTo The child views that should display column in the
     *            "childFrom" parameter. These should all be TextViews. The
     *            first N views in this list are given the values of the first N
     *            columns in the childFrom parameter.
     * @param childLayout resource identifier of a view layout that defines the
     *            views for a child (unless it is the last child within a group,
     *            in which case the lastChildLayout is used). The layout file
     *            should include at least those named views defined in "childTo"
     * @param lastChildLayout resource identifier of a view layout that defines
     *            the views for the last child within each group. The layout
     *            file should include at least those named views defined in
     *            "childTo"
     */
    SimpleExpandableListAdapter(
        /* [in] */ IContext* context,
        /* [in] */ IObjectContainer* groupData,
        /* [in] */ Int32 expandedGroupLayout,
        /* [in] */ Int32 collapsedGroupLayout,
        /* [in] */ ArrayOf<String>* groupFrom,
        /* [in] */ ArrayOf<Int32>* groupTo,
        /* [in] */ IObjectContainer* childData,
        /* [in] */ Int32 childLayout,
        /* [in] */ Int32 lastChildLayout,
        /* [in] */ ArrayOf<String>* childFrom,
        /* [in] */ ArrayOf<Int32>* childTo);

    CARAPI_(AutoPtr<IInterface>) GetChild(
        /* [in] */ Int32 groupPosition,
        /* [in] */ Int32 childPosition);

    CARAPI_(Int64) GetChildId(
        /* [in] */ Int32 groupPosition,
        /* [in] */ Int32 childPosition);

    CARAPI_(AutoPtr<IView>) GetChildView(
        /* [in] */ Int32 groupPosition,
        /* [in] */ Int32 childPosition,
        /* [in] */ Boolean isLastChild,
        /* [in] */ IView* convertView,
        /* [in] */ IViewGroup* parent);

    /**
     * Instantiates a new View for a child.
     * @param isLastChild Whether the child is the last child within its group.
     * @param parent The eventual parent of this new View.
     * @return A new child View
     */
    CARAPI_(AutoPtr<IView>) NewChildView(
        /* [in] */ Boolean isLastChild,
        /* [in] */ IViewGroup* parent);

    CARAPI_(Int32) GetChildrenCount(
        /* [in] */ Int32 groupPosition);

    CARAPI_(AutoPtr<IInterface>) GetGroup(
        /* [in] */ Int32 groupPosition);

    CARAPI_(Int32) GetGroupCount();

    CARAPI_(Int64) GetGroupId(
        /* [in] */ Int32 groupPosition);

    CARAPI_(AutoPtr<IView>) GetGroupView(
        /* [in] */ Int32 groupPosition,
        /* [in] */ Boolean isExpanded,
        /* [in] */ IView* convertView,
        /* [in] */ IViewGroup* parent);

    /**
     * Instantiates a new View for a group.
     * @param isExpanded Whether the group is currently expanded.
     * @param parent The eventual parent of this new View.
     * @return A new group View
     */
    CARAPI_(AutoPtr<IView>) NewGroupView(
        /* [in] */ Boolean isExpanded,
        /* [in] */ IViewGroup* parent);

    CARAPI_(Boolean) IsChildSelectable(
        /* [in] */ Int32 groupPosition,
        /* [in] */ Int32 childPosition);

    CARAPI_(Boolean) HasStableIds();

protected:
    SimpleExpandableListAdapter();

    CARAPI Init(
        /* [in] */ IContext* context,
        /* [in] */ IObjectContainer* groupData,
        /* [in] */ Int32 groupLayout,
        /* [in] */ ArrayOf<String>* groupFrom,
        /* [in] */ ArrayOf<Int32>* groupTo,
        /* [in] */ IObjectContainer* childData,
        /* [in] */ Int32 childLayout,
        /* [in] */ ArrayOf<String>* childFrom,
        /* [in] */ ArrayOf<Int32>* childTo);

    CARAPI Init(
        /* [in] */ IContext* context,
        /* [in] */ IObjectContainer* groupData,
        /* [in] */ Int32 expandedGroupLayout,
        /* [in] */ Int32 collapsedGroupLayout,
        /* [in] */ ArrayOf<String>* groupFrom,
        /* [in] */ ArrayOf<Int32>* groupTo,
        /* [in] */ IObjectContainer* childData,
        /* [in] */ Int32 childLayout,
        /* [in] */ ArrayOf<String>* childFrom,
        /* [in] */ ArrayOf<Int32>* childTo);

    CARAPI Init(
        /* [in] */ IContext* context,
        /* [in] */ IObjectContainer* groupData,
        /* [in] */ Int32 expandedGroupLayout,
        /* [in] */ Int32 collapsedGroupLayout,
        /* [in] */ ArrayOf<String>* groupFrom,
        /* [in] */ ArrayOf<Int32>* groupTo,
        /* [in] */ IObjectContainer* childData,
        /* [in] */ Int32 childLayout,
        /* [in] */ Int32 lastChildLayout,
        /* [in] */ ArrayOf<String>* childFrom,
        /* [in] */ ArrayOf<Int32>* childTo);

private:
    CARAPI BindView(
        /* [in] */ IView* view,
        /* [in] */ IObjectStringMap* data,
        /* [in] */ ArrayOf<String>* from,
        /* [in] */ ArrayOf<Int32>* to);

    CARAPI_(AutoPtr<IInterface>) GetItemFromContainer(
        /* [in] */ IObjectContainer* container,
        /* [in] */ Int32 position);

private:
    AutoPtr<IObjectContainer> mGroupData;
    Int32 mExpandedGroupLayout;
    Int32 mCollapsedGroupLayout;
    AutoPtr<ArrayOf<String> > mGroupFrom;
    AutoPtr<ArrayOf<Int32> > mGroupTo;

    AutoPtr<IObjectContainer> mChildData;
    Int32 mChildLayout;
    Int32 mLastChildLayout;
    AutoPtr<ArrayOf<String> > mChildFrom;
    AutoPtr<ArrayOf<Int32> > mChildTo;

    AutoPtr<ILayoutInflater> mInflater;
};

} // namespace Widget
} // namespace Droid
} // namespace Elastos
#endif
