
#ifndef __CBASEEXPANDABLELISTADAPTER_H__
#define __CBASEEXPANDABLELISTADAPTER_H__

#include "_CBaseExpandableListAdapter.h"
#include "widget/BaseExpandableListAdapter.h"

CarClass(CBaseExpandableListAdapter), public BaseExpandableListAdapter
{
public:
    CARAPI NotifyDataSetInvalidated();

    CARAPI NotifyDataSetChanged();

    CARAPI RegisterDataSetObserver(
        /* [in] */ IDataSetObserver * pObserver);

    CARAPI UnregisterDataSetObserver(
        /* [in] */ IDataSetObserver * pObserver);

    CARAPI GetGroupCount(
        /* [out] */ Int32 * pCount);

    CARAPI GetChildrenCount(
        /* [in] */ Int32 groupPosition,
        /* [out] */ Int32 * pCount);

    CARAPI GetGroup(
        /* [in] */ Int32 groupPosition,
        /* [out] */ IInterface ** ppChild);

    CARAPI GetChild(
        /* [in] */ Int32 groupPosition,
        /* [in] */ Int32 childPosition,
        /* [out] */ IInterface ** ppChild);

    CARAPI GetGroupId(
        /* [in] */ Int32 groupPosition,
        /* [out] */ Int64 * pId);

    CARAPI GetChildId(
        /* [in] */ Int32 groupPosition,
        /* [in] */ Int32 childPosition,
        /* [out] */ Int64 * pId);

    CARAPI HasStableIds(
        /* [out] */ Boolean * pId);

    CARAPI GetGroupView(
        /* [in] */ Int32 groupPosition,
        /* [in] */ Boolean isExpanded,
        /* [in] */ IView * pConvertView,
        /* [in] */ IViewGroup * pParent,
        /* [out] */ IView ** ppView);

    CARAPI GetChildView(
        /* [in] */ Int32 groupPosition,
        /* [in] */ Int32 childPosition,
        /* [in] */ Boolean isLastChild,
        /* [in] */ IView * pConvertView,
        /* [in] */ IViewGroup * pParent,
        /* [out] */ IView ** ppView);

    CARAPI IsChildSelectable(
        /* [in] */ Int32 groupPosition,
        /* [in] */ Int32 childPosition,
        /* [out] */ Boolean * pSelectable);

    CARAPI AreAllItemsEnabled(
        /* [out] */ Boolean * pEnabled);

    CARAPI IsEmpty(
        /* [out] */ Boolean * pEmpty);

    CARAPI OnGroupExpanded(
        /* [in] */ Int32 groupPosition);

    CARAPI OnGroupCollapsed(
        /* [in] */ Int32 groupPosition);

    CARAPI GetCombinedChildId(
        /* [in] */ Int64 groupId,
        /* [in] */ Int64 childId,
        /* [out] */ Int64 * pId);

    CARAPI GetCombinedGroupId(
        /* [in] */ Int64 groupId,
        /* [out] */ Int64 * pId);

    CARAPI GetGroupType(
        /* [in] */ Int32 groupPosition,
        /* [out] */ Int32 * pType);

    CARAPI GetChildType(
        /* [in] */ Int32 groupPosition,
        /* [in] */ Int32 childPosition,
        /* [out] */ Int32 * pType);

    CARAPI GetGroupTypeCount(
        /* [out] */ Int32 * pCount);

    CARAPI GetChildTypeCount(
        /* [out] */ Int32 * pCount);

private:
    // TODO: Add your private member variables here.
};

#endif // __CBASEEXPANDABLELISTADAPTER_H__
