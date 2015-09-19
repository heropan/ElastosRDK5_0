#include "widget/CSimpleExpandableListAdapter.h"

namespace Elastos {
namespace Droid {
namespace Widget {

CSimpleExpandableListAdapter::CSimpleExpandableListAdapter()
{}

ECode CSimpleExpandableListAdapter::constructor(
    /* [in] */ IContext* context,
    /* [in] */ IObjectContainer* groupData,
    /* [in] */ Int32 groupLayout,
    /* [in] */ ArrayOf<String>* groupFrom,
    /* [in] */ ArrayOf<Int32>* groupTo,
    /* [in] */ IObjectContainer* childData,
    /* [in] */ Int32 childLayout,
    /* [in] */ ArrayOf<String>* childFrom,
    /* [in] */ ArrayOf<Int32>* childTo)
{
    return SimpleExpandableListAdapter::Init(context, groupData, groupLayout,
        groupFrom, groupTo, childData, childLayout, childFrom, childTo);
}

ECode CSimpleExpandableListAdapter::constructor(
    /* [in] */ IContext* context,
    /* [in] */ IObjectContainer* groupData,
    /* [in] */ Int32 expandedGroupLayout,
    /* [in] */ Int32 collapsedGroupLayout,
    /* [in] */ ArrayOf<String>* groupFrom,
    /* [in] */ ArrayOf<Int32>* groupTo,
    /* [in] */ IObjectContainer* childData,
    /* [in] */ Int32 childLayout,
    /* [in] */ ArrayOf<String>* childFrom,
    /* [in] */ ArrayOf<Int32>* childTo)
{
    return SimpleExpandableListAdapter::Init(context, groupData, expandedGroupLayout,
        collapsedGroupLayout, groupFrom, groupTo, childData, childLayout, childFrom, childTo);
}

ECode CSimpleExpandableListAdapter::constructor(
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
    /* [in] */ ArrayOf<Int32>* childTo)
{
    return SimpleExpandableListAdapter::Init(context, groupData, expandedGroupLayout,
        collapsedGroupLayout, groupFrom, groupTo, childData, childLayout, lastChildLayout, childFrom, childTo);
}

ECode CSimpleExpandableListAdapter::NewChildView(
    /* [in] */ Boolean isLastChild,
    /* [in] */ IViewGroup* parent,
    /* [out] */ IView** view)
{
    AutoPtr<IView> v = SimpleExpandableListAdapter::NewChildView(isLastChild, parent);
    *view = v;
    REFCOUNT_ADD(*view);
    return NOERROR;
}

ECode CSimpleExpandableListAdapter::GetGroupType(
    /* [in] */ Int32 groupPosition,
    /* [out] */ Int32* type)
{
    *type = SimpleExpandableListAdapter::GetGroupType(groupPosition);
    return NOERROR;
}

ECode CSimpleExpandableListAdapter::GetChildType(
    /* [in] */ Int32 groupPosition,
    /* [in] */ Int32 childPosition,
    /* [out] */ Int32* type)
{
    *type = SimpleExpandableListAdapter::GetChildType(groupPosition, childPosition);
    return NOERROR;
}

ECode CSimpleExpandableListAdapter::GetGroupTypeCount(
    /* [out] */ Int32* count)
{
    *count = SimpleExpandableListAdapter::GetGroupTypeCount();
    return NOERROR;
}

ECode CSimpleExpandableListAdapter::GetChildTypeCount(
    /* [out] */ Int32* count)
{
    *count = SimpleExpandableListAdapter::GetChildTypeCount();
    return NOERROR;
}

ECode CSimpleExpandableListAdapter::NotifyDataSetInvalidated()
{
    return SimpleExpandableListAdapter::NotifyDataSetInvalidated();
}

ECode CSimpleExpandableListAdapter::NotifyDataSetChanged()
{
    return SimpleExpandableListAdapter::NotifyDataSetChanged();
}


ECode CSimpleExpandableListAdapter::RegisterDataSetObserver(
    /* [in] */ IDataSetObserver* observer)
{
    return SimpleExpandableListAdapter::RegisterDataSetObserver(observer);
}

ECode CSimpleExpandableListAdapter::UnregisterDataSetObserver(
    /* [in] */ IDataSetObserver* observer)
{
    return SimpleExpandableListAdapter::UnregisterDataSetObserver(observer);
}

ECode CSimpleExpandableListAdapter::GetGroupCount(
    /* [out] */ Int32* count)
{
    VALIDATE_NOT_NULL(count);
    *count = SimpleExpandableListAdapter::GetGroupCount();
    return NOERROR;
}

ECode CSimpleExpandableListAdapter::GetChildrenCount(
    /* [in] */ Int32 groupPosition,
    /* [out] */ Int32* count)
{
    VALIDATE_NOT_NULL(count);
    *count = SimpleExpandableListAdapter::GetChildrenCount(groupPosition);
    return NOERROR;
}

ECode CSimpleExpandableListAdapter::GetGroup(
    /* [in] */ Int32 groupPosition,
    /* [out] */ IInterface** child)
{
    VALIDATE_NOT_NULL(child);
    AutoPtr<IInterface> temp = SimpleExpandableListAdapter::GetGroup(groupPosition);
    *child = temp;
    REFCOUNT_ADD(*child);
    return NOERROR;
}

ECode CSimpleExpandableListAdapter::GetChild(
    /* [in] */ Int32 groupPosition,
    /* [in] */ Int32 childPosition,
    /* [out] */ IInterface** child)
{
    VALIDATE_NOT_NULL(child);
    AutoPtr<IInterface> temp = SimpleExpandableListAdapter::GetChild(groupPosition, childPosition);
    *child = temp;
    REFCOUNT_ADD(*child);
    return NOERROR;
}

ECode CSimpleExpandableListAdapter::GetGroupId(
    /* [in] */ Int32 groupPosition,
    /* [out] */ Int64* id)
{
    VALIDATE_NOT_NULL(id);
    *id = SimpleExpandableListAdapter::GetGroupId(groupPosition);
    return NOERROR;
}

ECode CSimpleExpandableListAdapter::GetChildId(
    /* [in] */ Int32 groupPosition,
    /* [in] */ Int32 childPosition,
    /* [out] */ Int64* id)
{
    VALIDATE_NOT_NULL(id);
    *id = SimpleExpandableListAdapter::GetChildId(groupPosition, childPosition);
    return NOERROR;
}

ECode CSimpleExpandableListAdapter::HasStableIds(
    /* [out] */ Boolean* id)
{
    VALIDATE_NOT_NULL(id);
    *id = SimpleExpandableListAdapter::HasStableIds();
    return NOERROR;
}

ECode CSimpleExpandableListAdapter::GetGroupView(
    /* [in] */ Int32 groupPosition,
    /* [in] */ Boolean isExpanded,
    /* [in] */ IView* convertView,
    /* [in] */ IViewGroup* parent,
    /* [out] */ IView** view)
{
    VALIDATE_NOT_NULL(view);
    AutoPtr<IView> temp = SimpleExpandableListAdapter::GetGroupView(groupPosition, isExpanded, convertView, parent);
    *view = temp;
    REFCOUNT_ADD(*view);
    return NOERROR;
}

ECode CSimpleExpandableListAdapter::GetChildView(
    /* [in] */ Int32 groupPosition,
    /* [in] */ Int32 childPosition,
    /* [in] */ Boolean isLastChild,
    /* [in] */ IView* convertView,
    /* [in] */ IViewGroup* parent,
    /* [out] */ IView** view)
{
    VALIDATE_NOT_NULL(view);
    AutoPtr<IView> temp = SimpleExpandableListAdapter::GetChildView(groupPosition, childPosition, isLastChild, convertView, parent);
    *view = temp;
    REFCOUNT_ADD(*view);
    return NOERROR;
}

ECode CSimpleExpandableListAdapter::IsChildSelectable(
    /* [in] */ Int32 groupPosition,
    /* [in] */ Int32 childPosition,
    /* [out] */ Boolean* selectable)
{
    VALIDATE_NOT_NULL(selectable);
    *selectable = SimpleExpandableListAdapter::IsChildSelectable(groupPosition, childPosition);
    return NOERROR;
}

ECode CSimpleExpandableListAdapter::AreAllItemsEnabled(
    /* [out] */ Boolean* enabled)
{
    VALIDATE_NOT_NULL(enabled);
    *enabled = SimpleExpandableListAdapter::AreAllItemsEnabled();
    return NOERROR;
}

ECode CSimpleExpandableListAdapter::IsEmpty(
    /* [out] */ Boolean* empty)
{
    VALIDATE_NOT_NULL(empty);
    *empty = SimpleExpandableListAdapter::IsEmpty();
    return NOERROR;
}

ECode CSimpleExpandableListAdapter::OnGroupExpanded(
    /* [in] */ Int32 groupPosition)
{
    return SimpleExpandableListAdapter::OnGroupExpanded(groupPosition);
}

ECode CSimpleExpandableListAdapter::OnGroupCollapsed(
    /* [in] */ Int32 groupPosition)
{
    return SimpleExpandableListAdapter::OnGroupCollapsed(groupPosition);
}

ECode CSimpleExpandableListAdapter::GetCombinedChildId(
    /* [in] */ Int64 groupId,
    /* [in] */ Int64 childId,
    /* [out] */ Int64* id)
{
    return SimpleExpandableListAdapter::GetCombinedChildId(groupId, childId);
}

ECode CSimpleExpandableListAdapter::GetCombinedGroupId(
    /* [in] */ Int64 groupId,
    /* [out] */ Int64* id)
{
    VALIDATE_NOT_NULL(id);
    *id = SimpleExpandableListAdapter::GetCombinedGroupId(groupId);
    return NOERROR;
}

} // namespace Widget
} // namespace Droid
} // namespace Elastos
