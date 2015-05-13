#include "CArrayAdapter.h"

namespace Elastos {
namespace Droid {
namespace Widget {

PInterface CArrayAdapter::Probe(
    /* [in] */ REIID riid)
{
    if (riid == EIID_IAdapter) {
        return (IAdapter*)(IListAdapter*)this;
    }
    return _CArrayAdapter::Probe(riid);
}

ECode CArrayAdapter::constructor(
    /* [in] */ IContext* context,
    /* [in] */ Int32 textViewResourceId)
{
    return ArrayAdapter::Init(context, textViewResourceId, 0);
}

ECode CArrayAdapter::constructor(
    /* [in] */ IContext* context,
    /* [in] */ Int32 resource,
    /* [in] */ Int32 textViewResourceId)
{
    return ArrayAdapter::Init(context, resource, textViewResourceId);
}

ECode CArrayAdapter::constructor(
    /* [in] */ IContext* context,
    /* [in] */ Int32 textViewResourceId,
    /* [in] */ IObjectContainer* objects)
{
    return ArrayAdapter::Init(context, textViewResourceId, 0, objects);
}

ECode CArrayAdapter::constructor(
    /* [in] */ IContext* context,
    /* [in] */ Int32 resource,
    /* [in] */ Int32 textViewResourceId,
    /* [in] */ IObjectContainer* objects)
{
    return ArrayAdapter::Init(context, resource, textViewResourceId, objects);
}

ECode CArrayAdapter::RegisterDataSetObserver(
    /* [in] */ IDataSetObserver* observer)
{
    return ArrayAdapter::RegisterDataSetObserver(observer);
}

ECode CArrayAdapter::UnregisterDataSetObserver(
    /* [in] */ IDataSetObserver* observer)
{
    return ArrayAdapter::UnregisterDataSetObserver(observer);
}

ECode CArrayAdapter::GetCount(
    /* [out] */ Int32* count)
{
    VALIDATE_NOT_NULL(count);
    *count =  ArrayAdapter::GetCount();
    return NOERROR;
}

ECode CArrayAdapter::GetItem(
    /* [in] */ Int32 position,
    /* [out] */ IInterface** item)
{
    VALIDATE_NOT_NULL(item);
    AutoPtr<IInterface> temp = ArrayAdapter::GetItem(position);
    *item = temp;
    INTERFACE_ADDREF(*item);
    return NOERROR;
}

ECode CArrayAdapter::GetItemId(
    /* [in] */ Int32 position,
    /* [out] */ Int64* itemId)
{
    VALIDATE_NOT_NULL(itemId);
    *itemId = ArrayAdapter::GetItemId(position);
    return NOERROR;
}

ECode CArrayAdapter::HasStableIds(
    /* [out] */ Boolean* hasStableIds)
{
    VALIDATE_NOT_NULL(hasStableIds);
    *hasStableIds = ArrayAdapter::HasStableIds();
    return NOERROR;
}

ECode CArrayAdapter::GetView(
    /* [in] */ Int32 position,
    /* [in] */ IView* convertView,
    /* [in] */ IViewGroup* parent,
    /* [out] */ IView** view)
{
    VALIDATE_NOT_NULL(view);
    AutoPtr<IView> temp = ArrayAdapter::GetView(position, convertView, parent);
    *view = temp;
    INTERFACE_ADDREF(*view);
    return NOERROR;
}

ECode CArrayAdapter::GetItemViewType(
    /* [in] */ Int32 position,
    /* [out] */ Int32* viewType)
{
    VALIDATE_NOT_NULL(viewType);
    *viewType = ArrayAdapter::GetItemViewType(position);
    return NOERROR;
}

ECode CArrayAdapter::GetViewTypeCount(
    /* [out] */ Int32* count)
{
    VALIDATE_NOT_NULL(count);
    *count = ArrayAdapter::GetViewTypeCount();
    return NOERROR;
}

ECode CArrayAdapter::IsEmpty(
    /* [out] */ Boolean* isEmpty)
{
    VALIDATE_NOT_NULL(isEmpty);
    *isEmpty = ArrayAdapter::IsEmpty();
    return NOERROR;
}

ECode CArrayAdapter::NotifyDataSetChanged()
{
    return ArrayAdapter::NotifyDataSetChanged();
}

ECode CArrayAdapter::NotifyDataSetInvalidated()
{
    return ArrayAdapter::NotifyDataSetInvalidated();
}

ECode CArrayAdapter::Add(
    /* [in] */ IInterface* object)
{
    return ArrayAdapter::Add(object);
}
ECode CArrayAdapter::AddAll(
    /* [in] */ IObjectContainer* collection)
{
    return ArrayAdapter::AddAll(collection);
}

ECode CArrayAdapter::AddAllEx(
    /* [in] */ ArrayOf<IInterface* >* items)
{
    return ArrayAdapter::AddAllEx(items);
}

ECode CArrayAdapter::Insert(
    /* [in] */ IInterface* object,
    /* [in] */ Int32 index)
{
    return ArrayAdapter::Insert(object, index);
}

ECode CArrayAdapter::Remove(
    /* [in] */ IInterface* object)
{
    return ArrayAdapter::Remove(object);
}

ECode CArrayAdapter::Clear()
{
    return ArrayAdapter::Clear();
}

ECode CArrayAdapter::Sort(
    /* [in] */ IComparator* comparator)
{
    return ArrayAdapter::Sort(comparator);
}

ECode CArrayAdapter::SetNotifyOnChange(
    /* [in] */ Boolean notifyOnChange)
{
    return ArrayAdapter::SetNotifyOnChange(notifyOnChange);
}

ECode CArrayAdapter::GetContext(
    /* [out] */ IContext** context)
{
    VALIDATE_NOT_NULL(context);
    AutoPtr<IContext> temp = ArrayAdapter::GetContext();
    *context = temp;
    INTERFACE_ADDREF(*context);
    return NOERROR;
}

ECode CArrayAdapter::GetPosition(
    /* [in] */ IInterface* item,
    /* [out] */ Int32* position)
{
    VALIDATE_NOT_NULL(position);
    *position = ArrayAdapter::GetPosition(item);
    return NOERROR;
}

ECode CArrayAdapter::SetDropDownViewResource(
    /* [in] */ Int32 resource)
{
    return ArrayAdapter::SetDropDownViewResource(resource);
}

ECode CArrayAdapter::GetDropDownView(
    /* [in] */ Int32 position,
    /* [in] */ IView* convertView,
    /* [in] */ IViewGroup* parent,
    /* [out] */ IView** view)
{
    VALIDATE_NOT_NULL(view);
    AutoPtr<IView> temp = ArrayAdapter::GetDropDownView(position, convertView, parent);
    *view = temp;
    INTERFACE_ADDREF(*view);
    return NOERROR;
}

ECode CArrayAdapter::AreAllItemsEnabled(
    /* [out] */ Boolean* enable)
{
    VALIDATE_NOT_NULL(enable);
    *enable = ArrayAdapter::AreAllItemsEnabled();
    return NOERROR;
}

ECode CArrayAdapter::IsEnabled(
    /* [in] */ Int32 position,
    /* [out] */ Boolean* enable)
{
    VALIDATE_NOT_NULL(enable);
    *enable = ArrayAdapter::IsEnabled(position);
    return NOERROR;
}

ECode CArrayAdapter::GetFilter(
    /* [out] */ IFilter** filter)
{
    VALIDATE_NOT_NULL(filter);
    AutoPtr<IFilter> temp;
    temp = ArrayAdapter::GetFilter();
    *filter = temp;
    INTERFACE_ADDREF(*filter);
    return NOERROR;
}

}// namespace Widget
}// namespace Droid
}// namespace Elastos
