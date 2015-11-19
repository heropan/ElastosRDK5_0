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
    REFCOUNT_ADD(*view);
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
    REFCOUNT_ADD(*filter);
    return NOERROR;
}

}// namespace Widget
}// namespace Droid
}// namespace Elastos
