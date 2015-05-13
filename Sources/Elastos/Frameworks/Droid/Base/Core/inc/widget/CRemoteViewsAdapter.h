#ifndef __CREMOTEVIEWSADAPTER_H__
#define __CREMOTEVIEWSADAPTER_H__

#include "_CRemoteViewsAdapter.h"
#include "RemoteViewsAdapter.h"
#include "widget/AdapterMacro.h"

using Elastos::Droid::Content::IContext;
using Elastos::Droid::Content::IIntent;
using Elastos::Droid::Database::IDataSetObserver;
using Elastos::Droid::View::IView;
using Elastos::Droid::View::IViewGroup;

namespace Elastos {
namespace Droid {
namespace Widget {

CarClass(CRemoteViewsAdapter) , public RemoteViewsAdapter
{
public:
    IBASEADAPTER_METHODS_DECL()
    ILISTADAPTER_METHODS_DECL()
    ISPINNERADAPTER_METHODS_DECL()
    IADAPTER_METHODS_DECL()

    CRemoteViewsAdapter();

    CARAPI constructor(
        /* [in] */ IContext* ctx,
        /* [in] */ IIntent* intent,
        /* [in] */ IRemoteAdapterConnectionCallback* cb);

    CARAPI_(PInterface) Probe(
        /* [in] */ REIID riid);

    CARAPI IsDataReady(
        /* [out] */ Boolean* res);

    CARAPI SetRemoteViewsOnClickHandler(
        /* [in] */ IRemoteViewsOnClickHandler* handler);

    CARAPI SaveRemoteViewsCache();

    CARAPI GetRemoteViewsServiceIntent(
        /* [out] */ IIntent** intent);

    /**
     * This method allows an AdapterView using this Adapter to provide information about which
     * views are currently being displayed. This allows for certain optimizations and preloading
     * which  wouldn't otherwise be possible.
     */
    CARAPI SetVisibleRangeHint(
        /* [in] */ Int32 lowerBound,
        /* [in] */ Int32 upperBound);

    CARAPI SuperNotifyDataSetChanged();

    CARAPI HandleMessage(
        /* [in] */ IMessage* msg,
        /* [out] */ Boolean* result);

};

} // namespace Widget
} // namespace Droid
} // namespace Elastos

#endif
