#include "widget/HeaderViewListAdapter.h"

namespace Elastos{
namespace Droid{
namespace Widget{

HeaderViewListAdapter::HeaderViewListAdapter(
    /* [in] */ ArrayOf<IFixedViewInfo*>* headerViewInfos,
    /* [in] */ ArrayOf<IFixedViewInfo*>* footerViewInfos,
    /* [in] */ IListAdapter* adapter)
{
    Init(headerViewInfos, footerViewInfos, adapter);
}

ECode HeaderViewListAdapter::Init(
    /* [in] */ ArrayOf<IFixedViewInfo*>* headerViewInfos,
    /* [in] */ ArrayOf<IFixedViewInfo*>* footerViewInfos,
    /* [in] */ IListAdapter* adapter)
{
    mAdapter = adapter;
    if(IFilterable::Probe(adapter)){
        mIsFilterable = TRUE;
    } else {
        mIsFilterable = FALSE;
    }

    if(headerViewInfos != NULL) {
        for(Int32 i = 0; i < headerViewInfos->GetLength(); i++) {
            mHeaderViewInfos.PushBack((*headerViewInfos)[i]);
        }
    }

    if(footerViewInfos != NULL) {
        for(Int32 i = 0; i < footerViewInfos->GetLength(); i++) {
            mFooterViewInfos.PushBack((*footerViewInfos)[i]);
        }
    }

    mAreAllFixedViewsSelectable = AreAllListInfosSelectable(mHeaderViewInfos)
        && AreAllListInfosSelectable(mFooterViewInfos);
    return NOERROR;
}
HeaderViewListAdapter::HeaderViewListAdapter()
    : mAreAllFixedViewsSelectable(FALSE)
    , mIsFilterable(FALSE)
{}

HeaderViewListAdapter::~HeaderViewListAdapter()
{
    if(mHeaderViewInfos.IsEmpty() == FALSE) {
        mHeaderViewInfos.Clear();
    }
    if(mFooterViewInfos.IsEmpty() == FALSE) {
        mFooterViewInfos.Clear();
    }
}

Int32 HeaderViewListAdapter::GetHeadersCount()
{
    return mHeaderViewInfos.GetSize();
}

Int32 HeaderViewListAdapter::GetFootersCount()
{
    return mFooterViewInfos.GetSize();
}

Boolean HeaderViewListAdapter::IsEmpty()
{
    Boolean res = FALSE;
    return  mAdapter == NULL || (mAdapter->IsEmpty(&res), res);
}

Boolean HeaderViewListAdapter::RemoveHeader(
    /* [in] */ IView* v)
{
    Iterator it = mHeaderViewInfos.Begin();
    while(it != mHeaderViewInfos.End())
    {
        AutoPtr<IFixedViewInfo> info = *it;
        AutoPtr<IView> vInfo;
        info->GetView((IView**)&vInfo);
        if(v == vInfo)
        {
            it = mHeaderViewInfos.Erase(it);
            mAreAllFixedViewsSelectable = AreAllListInfosSelectable(mHeaderViewInfos)
                && AreAllListInfosSelectable(mFooterViewInfos);
            return TRUE;
        }
        it++;
    }
    return FALSE;
}

Boolean HeaderViewListAdapter::RemoveFooter(
    /* [in] */ IView* v)
{
    Iterator it = mFooterViewInfos.Begin();
    while(it != mFooterViewInfos.End())
    {
        AutoPtr<IFixedViewInfo> info = *it;
        AutoPtr<IView> vInfo;
        info->GetView((IView**)&vInfo);
        if(v == vInfo)
        {
            it = mFooterViewInfos.Erase(it);
            mAreAllFixedViewsSelectable = AreAllListInfosSelectable(mHeaderViewInfos)
                && AreAllListInfosSelectable(mFooterViewInfos);
            return TRUE;
        }
        it++;
    }
    return FALSE;
}

Int32 HeaderViewListAdapter::GetCount()
{
    if(mAdapter != NULL) {
        Int32 count = 0;
        mAdapter->GetCount(&count);
        return GetFootersCount() + GetHeadersCount() + count;
    } else {
        return GetFootersCount() + GetHeadersCount();
    }
}

Boolean HeaderViewListAdapter::AreAllItemsEnabled()
{
    if(mAdapter != NULL) {
        Boolean res = FALSE;
        mAdapter->AreAllItemsEnabled(&res);
        return mAreAllFixedViewsSelectable && res;
    } else {
        return TRUE;
    }
}

Boolean HeaderViewListAdapter::IsEnabled(
    /* [in] */ Int32 position)
{
    Int32 numHeaders = GetHeadersCount();
    if(position < numHeaders) {
        Boolean selectable = FALSE;
        mHeaderViewInfos[position]->GetSelectable(&selectable);
        return selectable;
    }

    Int32 adjPosition = position - numHeaders;
    Int32 adapterCount = 0;
    if(mAdapter != NULL) {
        mAdapter->GetCount(&adapterCount);
        if(adjPosition < adapterCount) {
            Boolean isEnabled = FALSE;
            mAdapter->IsEnabled(adjPosition, &isEnabled);
            return isEnabled;
        }
    }
    if (adjPosition - adapterCount >= GetFootersCount())
        return FALSE;
    Boolean selectable;
    IFixedViewInfo* temp = mFooterViewInfos[adjPosition - adapterCount];
    temp->GetSelectable(&selectable);
    return selectable;
}

AutoPtr<IInterface> HeaderViewListAdapter::GetItem(
    /* [in] */ Int32 position)
{
    Int32 numHeaders = GetHeadersCount();
    if(position < numHeaders) {
        AutoPtr<IInterface> data;
        mHeaderViewInfos[position]->GetData((IInterface**)&data);
        return data;
    }
    Int32 adjPosition = position - numHeaders;
    Int32 adapterCount = 0;
    if(mAdapter != NULL) {
        mAdapter->GetCount(&adapterCount);
        if(adjPosition < adapterCount) {
            AutoPtr<IInterface> inter;
            mAdapter->GetItem(adjPosition, (IInterface**)&inter);
            return inter;
        }
    }
    if (adjPosition - adapterCount >= GetFootersCount())
        return NULL;
    AutoPtr<IInterface> data;
    mFooterViewInfos[adjPosition - adapterCount]->GetData((IInterface**)&data);
    return data;
}

Int64 HeaderViewListAdapter::GetItemId(
    /* [in] */ Int32 position)
{
    Int32 numHeaders = GetHeadersCount();
    if(mAdapter != NULL && position >= numHeaders) {
        Int32 adjPosition = position - numHeaders;
        Int32 adapterCount = 0;
        mAdapter->GetCount(&adapterCount);
        if(adjPosition < adapterCount) {
            Int64 id = 0;
            mAdapter->GetItemId(adjPosition, &id);
            return id;
        }
    }
    return -1;
}

Boolean HeaderViewListAdapter::HasStableIds()
{
    if(mAdapter != NULL) {
        Boolean res = FALSE;
        mAdapter->HasStableIds(&res);
        return res;
    }
    return FALSE;
}

AutoPtr<IView> HeaderViewListAdapter::GetView(
    /* [in] */ Int32 position,
    /* [in] */ IView* convertView,
    /* [in] */ IViewGroup* parent)
{
    Int32 numHeaders = GetHeadersCount();
    if(position < numHeaders) {
        AutoPtr<IView> data;
        mHeaderViewInfos[position]->GetView((IView**)&data);
        return data;
    }

    Int32 adjPosition = position - numHeaders;
    Int32 adapterCount = 0;
    if(mAdapter != NULL) {
        mAdapter->GetCount(&adapterCount);
        if(adjPosition < adapterCount) {
            AutoPtr<IView> v;
            mAdapter->GetView(adjPosition, convertView, parent, (IView**)&v);
            return v;
        }
    }
    if (adjPosition - adapterCount >= GetFootersCount())
        return NULL;
    AutoPtr<IView> v;
    mFooterViewInfos[adjPosition - adapterCount]->GetView((IView**)&v);
    return v;
}

Int32 HeaderViewListAdapter::GetItemViewType(
    /* [in] */Int32 position)
{
    Int32 numHeaders = GetHeadersCount();
    if(mAdapter != NULL && position >= numHeaders) {
        Int32 adjPosition = position - numHeaders;
        Int32 adapterCount = 0;
        mAdapter->GetCount(&adapterCount);
        if(adjPosition < adapterCount) {
            Int32 res = 0;
            mAdapter->GetItemViewType(adjPosition, &res);
        }
    }
    return IAdapterView::ITEM_VIEW_TYPE_HEADER_OR_FOOTER;
}

Int32 HeaderViewListAdapter::GetViewTypeCount()
{
    if(mAdapter != NULL) {
        Int32 count = 0;
        mAdapter->GetViewTypeCount(&count);
        return count;
    }
    return 1;
}

ECode HeaderViewListAdapter::RegisterDataSetObserver(
    /* [in] */ IDataSetObserver* observer)
{
    if(mAdapter != NULL) {
        mAdapter->RegisterDataSetObserver(observer);
    }
    return NOERROR;
}

ECode HeaderViewListAdapter::UnregisterDataSetObserver(
    /* [in] */ IDataSetObserver* observer)
{
    if(mAdapter != NULL) {
        mAdapter->UnregisterDataSetObserver(observer);
    }
    return NOERROR;
}

AutoPtr<IFilter> HeaderViewListAdapter::GetFilter()
{
    if(mIsFilterable) {
        AutoPtr<IFilterable> filterable = IFilterable::Probe(mAdapter);
        AutoPtr<IFilter> filter;
        filterable->GetFilter((IFilter**)&filter);
        return filter;
    }
    return NULL;
}

AutoPtr<IListAdapter> HeaderViewListAdapter::GetWrappedAdapter()
{
    return mAdapter;
}

Boolean HeaderViewListAdapter::AreAllListInfosSelectable(
        /* [in] */ List<AutoPtr<IFixedViewInfo> >& infos)
{
    if(!infos.IsEmpty()) {
        Iterator it = infos.Begin();
        for(; it != infos.End(); it++) {
            AutoPtr<IFixedViewInfo> info = *it;
            Boolean selectable;
            info->GetSelectable(&selectable);
            if(!selectable) {
                return FALSE;
            }
        }
    }
    return TRUE;
}

}// amespace Widget
}// namespace Droid
}// namespace Elastos
