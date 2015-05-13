#include "widget/CHeaderViewListAdapter.h"

namespace Elastos{
namespace Droid{
namespace Widget{

ILISTADAPTER_METHODS_IMPL(CHeaderViewListAdapter, HeaderViewListAdapter)
IADAPTER_METHODS_IMPL(CHeaderViewListAdapter, HeaderViewListAdapter)


ECode CHeaderViewListAdapter::constructor(
    /* [in] */ ArrayOf<IFixedViewInfo*>* headerViewInfos,
    /* [in] */ ArrayOf<IFixedViewInfo*>* footerViewInfos,
    /* [in] */ IListAdapter* adapter)
{
    return HeaderViewListAdapter::Init(headerViewInfos, footerViewInfos, adapter);
}

ECode CHeaderViewListAdapter::GetHeadersCount(
    /* [out] */ Int32* count)
{
    VALIDATE_NOT_NULL(count);
    *count = HeaderViewListAdapter::GetHeadersCount();
    return NOERROR;
}

ECode CHeaderViewListAdapter::GetFootersCount(
    /* [out] */ Int32* count)
{
    VALIDATE_NOT_NULL(count);
    *count = HeaderViewListAdapter::GetFootersCount();
    return NOERROR;
}

ECode CHeaderViewListAdapter::RemoveHeader(
    /* [in] */ IView* v,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
    *result = HeaderViewListAdapter::RemoveHeader(v);
    return NOERROR;
}

ECode CHeaderViewListAdapter::RemoveFooter(
    /* [in] */ IView* v,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
    *result = HeaderViewListAdapter::RemoveFooter(v);
    return NOERROR;
}

ECode CHeaderViewListAdapter::GetFilter(
    /* [out] */ IFilter** filter)
{
    VALIDATE_NOT_NULL(filter);
    AutoPtr<IFilter> fTemp = HeaderViewListAdapter::GetFilter();
    *filter = fTemp;
    INTERFACE_ADDREF(*filter);
    return NOERROR;
}

ECode CHeaderViewListAdapter::GetWrappedAdapter(
    /* [out] */ IListAdapter** adapter)
{
    VALIDATE_NOT_NULL(adapter);
    AutoPtr<IListAdapter> aTemp = HeaderViewListAdapter::GetWrappedAdapter();
    *adapter = aTemp;
    INTERFACE_ADDREF(*adapter);
    return NOERROR;
}

}// namespace Widget
}// namespace Droid
}// namespace Elastos