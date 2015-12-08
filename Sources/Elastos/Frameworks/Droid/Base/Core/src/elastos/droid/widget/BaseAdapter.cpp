
#include "elastos/droid/widget/BaseAdapter.h"
#ifdef DROID_CORE
#include "elastos/droid/database/CDataSetObservable.h"
#else
#include "Elastos.Droid.Core.h"
#endif

using Elastos::Droid::Database::CDataSetObservable;
using Elastos::Droid::Database::IObservable;

namespace Elastos {
namespace Droid {
namespace Widget {

CAR_INTERFACE_IMPL_4(BaseAdapter, Object, IBaseAdapter, IListAdapter, ISpinnerAdapter, IAdapter);
BaseAdapter::BaseAdapter()
{
    CDataSetObservable::New((IDataSetObservable**)&mDataSetObservable);
}

ECode BaseAdapter::HasStableIds(
    /* [out] */ Boolean* has)
{
    VALIDATE_NOT_NULL(has);
    *has = FALSE;
    return NOERROR;
}

ECode BaseAdapter::RegisterDataSetObserver(
    /* [in] */ IDataSetObserver* observer)
{
    return IObservable::Probe(mDataSetObservable)->RegisterObserver(observer);
}

ECode BaseAdapter::UnregisterDataSetObserver(
    /* [in] */ IDataSetObserver* observer)
{
    return IObservable::Probe(mDataSetObservable)->UnregisterObserver(observer);
}

ECode BaseAdapter::NotifyDataSetChanged()
{
    mDataSetObservable->NotifyChanged();
    return NOERROR;
}

ECode BaseAdapter::NotifyDataSetInvalidated()
{
    mDataSetObservable->NotifyInvalidated();
    return NOERROR;
}

ECode BaseAdapter::AreAllItemsEnabled(
    /* [out] */ Boolean* enabled)
{
    VALIDATE_NOT_NULL(enabled);
    *enabled = TRUE;
    return NOERROR;
}

ECode BaseAdapter::IsEnabled(
    /* [in] */ Int32 position,
    /* [out] */ Boolean* enabled)
{
    VALIDATE_NOT_NULL(enabled);
    *enabled = TRUE;
    return NOERROR;
}

ECode BaseAdapter::GetDropDownView(
    /* [in] */ Int32 position,
    /* [in] */ IView* convertView,
    /* [in] */ IViewGroup* parent,
    /* [out] */ IView** view)
{
    VALIDATE_NOT_NULL(view);
    return GetView(position, convertView, parent, view);
}

ECode BaseAdapter::GetItemViewType(
    /* [in] */ Int32 position,
    /* [out] */ Int32* type)
{
    VALIDATE_NOT_NULL(type);
    *type = 0;
    return NOERROR;
}

ECode BaseAdapter::GetViewTypeCount(
    /* [out] */ Int32* count)
{
    VALIDATE_NOT_NULL(count);
    *count = 1;
    return NOERROR;
}

ECode BaseAdapter::IsEmpty(
    /* [out] */ Boolean* empty)
{
    VALIDATE_NOT_NULL(empty);
    Int32 count = 0;
    *empty = (GetCount(&count), count) == 0;
    return NOERROR;
}

}// namespace Widget
}// namespace Droid
}// namespace Elastos
