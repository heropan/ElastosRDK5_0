
#ifndef __ELASTOS_DROID_WIDGET_CREMOTEVIEWSFACTORYADAPTER_H__
#define __ELASTOS_DROID_WIDGET_CREMOTEVIEWSFACTORYADAPTER_H__

#include "_CRemoteViewsFactoryAdapter.h"

using Elastos::Droid::Content::IIntent;

namespace Elastos {
namespace Droid {
namespace Widget {

CarClass(CRemoteViewsFactoryAdapter)
{
public:
    CRemoteViewsFactoryAdapter();

    CARAPI constructor(
        /* [in] */ IRemoteViewsFactory* factory,
        /* [in] */ Boolean isCreated);

    CARAPI IsCreated(
        /* [out] */ Boolean* result);

    CARAPI OnDataSetChanged();

    CARAPI OnDataSetChangedAsync();

    CARAPI GetCount(
        /* [out] */ Int32* count);

    CARAPI GetViewAt(
        /* [in] */ Int32 position,
        /* [out] */ IParcelable** views);

    CARAPI GetLoadingView(
        /* [out] */ IParcelable** views);


    CARAPI GetViewTypeCount(
        /* [out] */ Int32* count);

    CARAPI GetItemId(
        /* [in] */ Int32 position,
        /* [out] */ Int64* id);

    CARAPI HasStableIds(
        /* [out] */ Boolean* res);

    CARAPI OnDestroy(
        /* [in] */ IIntent* intent);

    CARAPI ToString(
        /* [out] */ String* str);

private:
    AutoPtr<IRemoteViewsFactory> mFactory;
    Boolean mIsCreated;
};

} // namespace Widget
} // namespace Droid
} // namespace Elastos

#endif //__ELASTOS_DROID_WIDGET_CREMOTEVIEWSFACTORYADAPTER_H__

