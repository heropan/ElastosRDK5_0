#include "elastos/droid/ext/frameworkext.h"
#include "elastos/droid/widget/CExpandableListConnector.h"

namespace Elastos {
namespace Droid {
namespace Widget {

PInterface CExpandableListConnector::Probe(
    /* [in] */ REIID riid)
{
    return _CExpandableListConnector::Probe(riid);
}

ECode CExpandableListConnector::SetExpandableListAdapter(
    /* [in] */ IExpandableListAdapter* expandableListAdapter)
{
    return ExpandableListConnector::SetExpandableListAdapter(expandableListAdapter);
}

ECode CExpandableListConnector::GetUnflattenedPos(
    /* [in] */ Int32 flPos,
    /* [out] */ IPositionMetadata** positionMetadata)
{
    VALIDATE_NOT_NULL(positionMetadata);
    *positionMetadata = ExpandableListConnector::GetUnflattenedPos(flPos);
    return NOERROR;
}

ECode CExpandableListConnector::IsGroupExpanded(
    /* [in] */ Int32 groupPosition,
    /* [out] */ Boolean* expanded)
{
    VALIDATE_NOT_NULL(expanded);
    *expanded = ExpandableListConnector::IsGroupExpanded(groupPosition);
    return NOERROR;
}

ECode CExpandableListConnector::SetMaxExpGroupCount(
    /* [in] */ Int32 maxExpGroupCount)
{
    return ExpandableListConnector::SetMaxExpGroupCount(maxExpGroupCount);
}

ECode CExpandableListConnector::GetFlattenedPos(
    /* [in] */ IExpandableListPosition* pos,
    /* [out] */ IPositionMetadata** data)
{
    VALIDATE_NOT_NULL(data);
    *data = ExpandableListConnector::GetFlattenedPos(pos);
    return NOERROR;
}

ECode CExpandableListConnector::ExpandGroup(
    /* [in] */ Int32 groupPos,
    /* [out] */ Boolean* res)
{
    VALIDATE_NOT_NULL(res);
    *res = ExpandableListConnector::ExpandGroup(groupPos);
    return NOERROR;
}

ECode CExpandableListConnector::ExpandGroup(
    /* [in] */ IPositionMetadata* posMetadata,
    /* [out] */ Boolean* res)
{
    VALIDATE_NOT_NULL(res);
    *res = ExpandableListConnector::ExpandGroup(posMetadata);
    return NOERROR;
}

ECode CExpandableListConnector::CollapseGroup(
    /* [in] */ Int32 groupPos,
    /* [out] */ Boolean* res)
{
    VALIDATE_NOT_NULL(res);
    *res = ExpandableListConnector::CollapseGroup(groupPos);
    return NOERROR;
}

ECode CExpandableListConnector::CollapseGroup(
    /* [in] */ IPositionMetadata* posMetadata,
    /* [out] */ Boolean* res)
{
    VALIDATE_NOT_NULL(res);
    *res = ExpandableListConnector::CollapseGroup(posMetadata);
    return NOERROR;
}

ECode CExpandableListConnector::RegisterDataSetObserver(
    /* [in] */ IDataSetObserver* observer)
{
    return ExpandableListConnector::RegisterDataSetObserver(observer);
}

ECode CExpandableListConnector::UnregisterDataSetObserver(
    /* [in] */ IDataSetObserver* observer)
{
    return ExpandableListConnector::UnregisterDataSetObserver(observer);
}

ECode CExpandableListConnector::GetCount(
    /* [out] */ Int32* count)
{
    VALIDATE_NOT_NULL(count);
    *count = ExpandableListConnector::GetCount();
    return NOERROR;
}

ECode CExpandableListConnector::GetItem(
    /* [in] */ Int32 position,
    /* [out] */ IInterface** item)
{
    VALIDATE_NOT_NULL(item);
    AutoPtr<IInterface> temp = ExpandableListConnector::GetItem(position);
    *item = temp;
    REFCOUNT_ADD(*item);
    return NOERROR;
}

ECode CExpandableListConnector::GetItemId(
    /* [in] */ Int32 position,
    /* [out] */ Int64* itemId)
{
    VALIDATE_NOT_NULL(itemId);
    *itemId = ExpandableListConnector::GetItemId(position);
    return NOERROR;
}

ECode CExpandableListConnector::HasStableIds(
    /* [out] */ Boolean* hasStableIds)
{
    VALIDATE_NOT_NULL(hasStableIds);
    *hasStableIds = ExpandableListConnector::HasStableIds();
    return NOERROR;
}

ECode CExpandableListConnector::GetView(
    /* [in] */ Int32 position,
    /* [in] */ IView* convertView,
    /* [in] */ IViewGroup* parent,
    /* [out] */ IView** view)
{
    VALIDATE_NOT_NULL(view);
    *view = ExpandableListConnector::GetView(position, convertView, parent);
    return NOERROR;
}

ECode CExpandableListConnector::GetItemViewType(
    /* [in] */ Int32 position,
    /* [out] */ Int32* viewType)
{
    VALIDATE_NOT_NULL(viewType);
    *viewType = ExpandableListConnector::GetItemViewType(position);
    return NOERROR;
}

ECode CExpandableListConnector::GetViewTypeCount(
    /* [out] */ Int32* count)
{
    VALIDATE_NOT_NULL(count);
    *count = ExpandableListConnector::GetViewTypeCount();
    return NOERROR;
}

ECode CExpandableListConnector::IsEmpty(
    /* [out] */ Boolean* isEmpty)
{
    VALIDATE_NOT_NULL(isEmpty);
    *isEmpty = ExpandableListConnector::IsEmpty();
    return NOERROR;
}

ECode CExpandableListConnector::IsEnabled(
    /* [in] */ Int32 position,
    /* [out] */ Boolean* enabled)
{
    VALIDATE_NOT_NULL(enabled);
    *enabled = ExpandableListConnector::IsEnabled(position);
    return NOERROR;
}

ECode CExpandableListConnector::AreAllItemsEnabled(
    /* [out] */ Boolean* enabled)
{
    VALIDATE_NOT_NULL(enabled);
    *enabled = ExpandableListConnector::AreAllItemsEnabled();
    return NOERROR;
}

ECode CExpandableListConnector::NotifyDataSetChanged()
{
    return ExpandableListConnector::NotifyDataSetChanged();
}

ECode CExpandableListConnector::NotifyDataSetInvalidated()
{
    return ExpandableListConnector::NotifyDataSetInvalidated();
}

ECode CExpandableListConnector::GetDropDownView(
    /* [in] */ Int32 position,
    /* [in] */ IView* convertView,
    /* [in] */ IViewGroup* parent,
    /* [out] */ IView** view)
{
    VALIDATE_NOT_NULL(view);
    AutoPtr<IView> v = ExpandableListConnector::GetDropDownView(position, convertView, parent);
    *view = v;
    REFCOUNT_ADD(*view);
    return NOERROR;
}

ECode CExpandableListConnector::GetFilter(
    /* [out] */ IFilter** filter)
{
    VALIDATE_NOT_NULL(filter);
    AutoPtr<IFilter> f = ExpandableListConnector::GetFilter();
    *filter = f;
    REFCOUNT_ADD(*filter);
    return NOERROR;
}

}// namespace Widget
}// namespace Droid
}// namespace Elastos
