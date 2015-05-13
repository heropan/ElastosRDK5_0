
#ifndef __BASEADAPTER_H__
#define __BASEADAPTER_H__

#include "ext/frameworkext.h"
#include "widget/Adapter.h"

using Elastos::Droid::Database::IDataSetObservable;

namespace Elastos{
namespace Droid{
namespace Widget{

/**
 * Common base class of common implementation for an {@link Adapter} that can be
 * used in both {@link ListView} (by implementing the specialized
 * {@link ListAdapter} interface} and {@link Spinner} (by implementing the
 * specialized {@link SpinnerAdapter} interface.
 */
class BaseAdapter : public ListAdapter
{
protected:
    BaseAdapter();

public:
    virtual CARAPI_(Boolean) HasStableIds();

    virtual CARAPI RegisterDataSetObserver(
        /* [in] */ IDataSetObserver* observer);

    virtual CARAPI UnregisterDataSetObserver(
        /* [in] */ IDataSetObserver* observer);

    virtual CARAPI NotifyDataSetChanged();

    virtual CARAPI NotifyDataSetInvalidated();

    virtual CARAPI_(Boolean) AreAllItemsEnabled();

    virtual CARAPI_(Boolean) IsEnabled(
        /* [in] */ Int32 position);

    virtual CARAPI_(AutoPtr<IView>) GetDropDownView(
        /* [in] */ Int32 position,
        /* [in] */ IView* convertView,
        /* [in] */ IViewGroup* parent);

    virtual CARAPI_(Int32) GetItemViewType(
        /* [in] */ Int32 position);

    virtual CARAPI_(Int32) GetViewTypeCount();

    virtual CARAPI_(Boolean) IsEmpty();

private:
    AutoPtr<IDataSetObservable> mDataSetObservable;
};

}// namespace Widget
}// namespace Droid
}// namespace Elastos

#endif //__BASEADAPTER_H__
