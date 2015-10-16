#include "elastos/droid/widget/CRemoteViewsAdapter.h"

namespace Elastos {
namespace Droid {
namespace Widget {

IBASEADAPTER_METHODS_IMPL(CRemoteViewsAdapter, RemoteViewsAdapter)
ILISTADAPTER_METHODS_IMPL(CRemoteViewsAdapter, RemoteViewsAdapter)
ISPINNERADAPTER_METHODS_IMPL(CRemoteViewsAdapter, RemoteViewsAdapter)
IADAPTER_METHODS_IMPL(CRemoteViewsAdapter, RemoteViewsAdapter)

CRemoteViewsAdapter::CRemoteViewsAdapter()
{}

PInterface CRemoteViewsAdapter::Probe(
    /* [in] */ REIID riid)
{
    if (riid == EIID_IAdapter) {
        return (IAdapter*)(IBaseAdapter*)this;
    }
    else if (riid == EIID_RemoteViewsAdapter) {
        return reinterpret_cast<PInterface>((RemoteViewsAdapter*)this);
    }
    return _CRemoteViewsAdapter::Probe(riid);
}

ECode CRemoteViewsAdapter::constructor(
    /* [in] */ IContext* ctx,
    /* [in] */ IIntent* intent,
    /* [in] */ IRemoteAdapterConnectionCallback* cb)
{
    return RemoteViewsAdapter::Init(ctx, intent, cb);
}

ECode CRemoteViewsAdapter::IsDataReady(
    /* [out] */ Boolean* res)
{
    *res = RemoteViewsAdapter::IsDataReady();
    return NOERROR;
}


ECode CRemoteViewsAdapter::SetRemoteViewsOnClickHandler(
    /* [in] */ IRemoteViewsOnClickHandler* handler)
{
    return RemoteViewsAdapter::SetRemoteViewsOnClickHandler(handler);
}


ECode CRemoteViewsAdapter::SaveRemoteViewsCache()
{
    return RemoteViewsAdapter::SaveRemoteViewsCache();
}


ECode CRemoteViewsAdapter::GetRemoteViewsServiceIntent(
    /* [out] */ IIntent** intent)
{
    AutoPtr<IIntent> temp = RemoteViewsAdapter::GetRemoteViewsServiceIntent();
    *intent = temp;
    REFCOUNT_ADD(*intent);
    return NOERROR;
}


/**
 * This method allows an AdapterView using this Adapter to provide information about which
 * views are currently being displayed. This allows for certain optimizations and preloading
 * which  wouldn't otherwise be possible.
 */
ECode CRemoteViewsAdapter::SetVisibleRangeHint(
    /* [in] */ Int32 lowerBound,
    /* [in] */ Int32 upperBound)
{
    return RemoteViewsAdapter::SetVisibleRangeHint(lowerBound, upperBound);
}


ECode CRemoteViewsAdapter::SuperNotifyDataSetChanged()
{
    return RemoteViewsAdapter::SuperNotifyDataSetChanged();
}

ECode CRemoteViewsAdapter::HandleMessage(
    /* [in] */ IMessage* msg,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
    *result =  RemoteViewsAdapter::HandleMessage(msg);
    return NOERROR;
}

} // namespace Widget
} // namespace Droid
} // namespace Elastos
