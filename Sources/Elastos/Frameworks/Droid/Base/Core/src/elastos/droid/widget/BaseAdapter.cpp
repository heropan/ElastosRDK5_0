
#include "elastos/droid/widget/BaseAdapter.h"
#ifdef DROID_CORE
#include "Elastos.Droid.Core_server.h"
#include "elastos/droid/database/CDataSetObservable.h"
#else
#include "Elastos.Droid.Core.h"
#endif

using Elastos::Droid::Database::CDataSetObservable;

namespace Elastos{
namespace Droid{
namespace Widget{

BaseAdapter::BaseAdapter()
{
    CDataSetObservable::New((IDataSetObservable**)&mDataSetObservable);
}

Boolean BaseAdapter::HasStableIds()
{
    return FALSE;
}

ECode BaseAdapter::RegisterDataSetObserver(
    /* [in] */ IDataSetObserver* observer)
{
    return mDataSetObservable->RegisterObserver(observer);
}

ECode BaseAdapter::UnregisterDataSetObserver(
    /* [in] */ IDataSetObserver* observer)
{
    return mDataSetObservable->UnregisterObserver(observer);
}

/**
 * Notifies the attached View that the underlying data has been changed
 * and it should refresh itself.
 */
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

Boolean BaseAdapter::AreAllItemsEnabled()
{
    return TRUE;
}

Boolean BaseAdapter::IsEnabled(
    /* [in] */ Int32 position)
{
    return TRUE;
}

AutoPtr<IView> BaseAdapter::GetDropDownView(
    /* [in] */ Int32 position,
    /* [in] */ IView* convertView,
    /* [in] */ IViewGroup* parent)
{
    return GetView(position, convertView, parent);
}

Int32 BaseAdapter::GetItemViewType(
    /* [in] */ Int32 position)
{
    return 0;
}

Int32 BaseAdapter::GetViewTypeCount()
{
    return 1;
}

Boolean BaseAdapter::IsEmpty()
{
    return GetCount() == 0;
}

}// namespace Widget
}// namespace Droid
}// namespace Elastos
