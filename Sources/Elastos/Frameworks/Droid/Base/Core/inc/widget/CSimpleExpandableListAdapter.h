#ifndef __CSIMPLEEXPANDABLELISTADAPTER_H__
#define __CSIMPLEEXPANDABLELISTADAPTER_H__
#include "_CSimpleExpandableListAdapter.h"
#include "widget/SimpleExpandableListAdapter.h"

namespace Elastos {
namespace Droid {
namespace Widget {

CarClass(CSimpleExpandableListAdapter) , public SimpleExpandableListAdapter
{
public:
    CSimpleExpandableListAdapter();

    CARAPI constructor(
        /* [in] */ IContext* context,
        /* [in] */ IObjectContainer* groupData,
        /* [in] */ Int32 groupLayout,
        /* [in] */ ArrayOf<String>* groupFrom,
        /* [in] */ ArrayOf<Int32>* groupTo,
        /* [in] */ IObjectContainer* childData,
        /* [in] */ Int32 childLayout,
        /* [in] */ ArrayOf<String>* childFrom,
        /* [in] */ ArrayOf<Int32>* childTo);

    CARAPI constructor(
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

    CARAPI constructor(
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

    CARAPI NewChildView(
        /* [in] */ Boolean isLastChild,
        /* [in] */ IViewGroup* parent,
        /* [out] */ IView** view);

    CARAPI GetGroupType(
        /* [in] */ Int32 groupPosition,
        /* [out] */ Int32* type);

    CARAPI GetChildType(
        /* [in] */ Int32 groupPosition,
        /* [in] */ Int32 childPosition,
        /* [out] */ Int32* type);

    CARAPI GetGroupTypeCount(
        /* [out] */ Int32* count);

    CARAPI GetChildTypeCount(
        /* [out] */ Int32* count);

    CARAPI NotifyDataSetInvalidated();

    CARAPI NotifyDataSetChanged();

    CARAPI RegisterDataSetObserver(
        /* [in] */ IDataSetObserver* observer);

    CARAPI UnregisterDataSetObserver(
        /* [in] */ IDataSetObserver* observer);

    CARAPI GetGroupCount(
        /* [out] */ Int32* count);

    CARAPI GetChildrenCount(
        /* [in] */ Int32 groupPosition,
        /* [out] */ Int32* count);

    CARAPI GetGroup(
        /* [in] */ Int32 groupPosition,
        /* [out] */ IInterface** child);

    CARAPI GetChild(
        /* [in] */ Int32 groupPosition,
        /* [in] */ Int32 childPosition,
        /* [out] */ IInterface** child);

    CARAPI GetGroupId(
        /* [in] */ Int32 groupPosition,
        /* [out] */ Int64* id);

    CARAPI GetChildId(
        /* [in] */ Int32 groupPosition,
        /* [in] */ Int32 childPosition,
        /* [out] */ Int64* id);

    CARAPI HasStableIds(
        /* [out] */ Boolean* id);

    CARAPI GetGroupView(
        /* [in] */ Int32 groupPosition,
        /* [in] */ Boolean isExpanded,
        /* [in] */ IView* convertView,
        /* [in] */ IViewGroup* parent,
        /* [out] */ IView** view);

    CARAPI GetChildView(
        /* [in] */ Int32 groupPosition,
        /* [in] */ Int32 childPosition,
        /* [in] */ Boolean isLastChild,
        /* [in] */ IView* convertView,
        /* [in] */ IViewGroup* parent,
        /* [out] */ IView** view);

    CARAPI IsChildSelectable(
        /* [in] */ Int32 groupPosition,
        /* [in] */ Int32 childPosition,
        /* [out] */ Boolean* selectable);

    CARAPI AreAllItemsEnabled(
        /* [out] */ Boolean* enabled);

    CARAPI IsEmpty(
        /* [out] */ Boolean* empty);

    CARAPI OnGroupExpanded(
        /* [in] */ Int32 groupPosition);

    CARAPI OnGroupCollapsed(
        /* [in] */ Int32 groupPosition);

    CARAPI GetCombinedChildId(
        /* [in] */ Int64 groupId,
        /* [in] */ Int64 childId,
        /* [out] */ Int64* id);

    CARAPI GetCombinedGroupId(
        /* [in] */ Int64 groupId,
        /* [out] */ Int64* id);
};

} // namespace Widget
} // namespace Droid
} // namespace Elastos
#endif