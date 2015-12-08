#ifndef __ELASTOS_DROID_WIDGET_CHEADERVIEWLISTADAPTER_H__
#define __ELASTOS_DROID_WIDGET_CHEADERVIEWLISTADAPTER_H__

#include "_Elastos_Droid_Widget_CHeaderViewListAdapter.h"
#include "elastos/droid/widget/HeaderViewListAdapter.h"


namespace Elastos {
namespace Droid {
namespace Widget {

CarClass(CHeaderViewListAdapter), public HeaderViewListAdapter
{
public:

    ILISTADAPTER_METHODS_DECL()
    IADAPTER_METHODS_DECL()

    CARAPI constructor(
        /* [in] */ ArrayOf<IFixedViewInfo*>* headerViewInfos,
        /* [in] */ ArrayOf<IFixedViewInfo*>* footerViewInfos,
        /* [in] */ IListAdapter* adapter);

    CARAPI GetHeadersCount(
        /* [out] */ Int32* count);

    CARAPI GetFootersCount(
        /* [out] */ Int32* count);

    CARAPI RemoveHeader(
        /* [in] */ IView* v,
        /* [out] */ Boolean* result);

    CARAPI RemoveFooter(
        /* [in] */ IView* v,
        /* [out] */ Boolean* result);

    CARAPI GetFilter(
        /* [out] */ IFilter** filter);

    CARAPI GetWrappedAdapter(
        /* [out] */ IListAdapter** adapter);
};

} // namespace Widget
} // namespace Droid
} // namespace Elastos


#endif //__ELASTOS_DROID_WIDGET_CHEADERVIEWLISTADAPTER_H__
