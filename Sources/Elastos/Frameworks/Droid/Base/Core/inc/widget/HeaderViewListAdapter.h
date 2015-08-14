#ifndef __ELASTOS_DROID_WIDGET_HEADERVIEWLISTADAPTER_H__
#define __ELASTOS_DROID_WIDGET_HEADERVIEWLISTADAPTER_H__

#include "ext/frameworkext.h"
#include "widget/ListView.h"
#include <elastos/utility/etl/List.h>

using Elastos::Utility::Etl::List;
using Elastos::Droid::View::IView;
using Elastos::Droid::View::IViewGroup;
using Elastos::Droid::Database::IDataSetObserver;

namespace Elastos{
namespace Droid{
namespace Widget{

class HeaderViewListAdapter
{

public:
    HeaderViewListAdapter();

    HeaderViewListAdapter(
        /* [in] */ ArrayOf<IFixedViewInfo*>* mHeaderViewInfos,
        /* [in] */ ArrayOf<IFixedViewInfo*>* mFooterViewInfos,
        /* [in] */ IListAdapter* adapter);

    ~HeaderViewListAdapter();

    CARAPI Init(
        /* [in] */ ArrayOf<IFixedViewInfo*>* mHeaderViewInfos,
        /* [in] */ ArrayOf<IFixedViewInfo*>* mFooterViewInfos,
        /* [in] */ IListAdapter* adapter);

    virtual CARAPI_(Int32) GetHeadersCount();

    virtual CARAPI_(Int32) GetFootersCount();

    virtual CARAPI_(Boolean) IsEmpty();

    CARAPI_(Boolean) RemoveHeader(
        /* [in] */ IView* v);

    CARAPI_(Boolean) RemoveFooter(
        /* [in] */ IView* v);

    CARAPI_(Int32) GetCount();

    CARAPI_(Boolean) AreAllItemsEnabled();

    CARAPI_(Boolean) IsEnabled(
        /* [in] */ Int32 position);

    CARAPI_(AutoPtr<IInterface>) GetItem(
        /* [in] */ Int32 position);

    CARAPI_(Int64) GetItemId(
        /* [in] */ Int32 position);

    CARAPI_(Boolean) HasStableIds();

    CARAPI_(AutoPtr<IView>) GetView(
        /* [in] */ Int32 position,
        /* [in] */ IView* convertView,
        /* [in] */ IViewGroup* parent);

    CARAPI_(Int32) GetItemViewType(
        /* [in] */ Int32 position);

    CARAPI_(Int32) GetViewTypeCount();

    CARAPI RegisterDataSetObserver(
        /* [in] */ IDataSetObserver* observer);

    CARAPI UnregisterDataSetObserver(
        /* [in] */ IDataSetObserver* observer);

    CARAPI_(AutoPtr<IFilter>) GetFilter();

    CARAPI_(AutoPtr<IListAdapter>) GetWrappedAdapter();

private:
    CARAPI_(Boolean) AreAllListInfosSelectable(
        /* [in] */ List<AutoPtr<IFixedViewInfo> >& info);

private:
    AutoPtr<IListAdapter> mAdapter;
    List<AutoPtr<IFixedViewInfo> > mHeaderViewInfos;
    List<AutoPtr<IFixedViewInfo> > mFooterViewInfos;
    Boolean mAreAllFixedViewsSelectable;
    Boolean mIsFilterable;

    typedef List<AutoPtr<IFixedViewInfo> >::Iterator Iterator;
};

}// namespace Widget
}// namespace Droid
}// namespace Elastos

#endif //__ELASTOS_DROID_WIDGET_HEADERVIEWLISTADAPTER_H__
