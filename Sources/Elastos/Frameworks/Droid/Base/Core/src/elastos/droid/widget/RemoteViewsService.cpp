
#include "widget/RemoteViewsService.h"
#include "widget/CRemoteViewsFactoryAdapter.h"
#include "content/CIntentFilterComparison.h"

using Elastos::Utility::Etl::Pair;
using Elastos::Droid::Content::CIntentFilterComparison;
using Elastos::Droid::Widget::Internal::IIRemoteViewsFactory;

namespace Elastos {
namespace Droid {
namespace Widget {

const String RemoteViewsService::TAG("RemoteViewsService");
Mutex RemoteViewsService::sLock;
HashMap< AutoPtr<IIntentFilterComparison>, AutoPtr<IRemoteViewsFactory> > RemoteViewsService::sRemoteViewFactories;

ECode RemoteViewsService::OnBind(
    /* [in] */ IIntent* intent,
    /* [out] */ IBinder** binder)
{
    VALIDATE_NOT_NULL(binder);
    AutoLock lock(sLock);

    AutoPtr<IIntentFilterComparison> fc;
    CIntentFilterComparison::New(intent, (IIntentFilterComparison**)&fc);
    AutoPtr<IRemoteViewsFactory> factory;
    Boolean isCreated = FALSE;
    if (sRemoteViewFactories.Find(fc) == sRemoteViewFactories.End()) {
        FAIL_RETURN(OnGetViewFactory(intent, (IRemoteViewsFactory**)&factory));
        sRemoteViewFactories[fc] = factory;
        factory->OnCreate();
        isCreated = FALSE;
    }
    else {
        factory = sRemoteViewFactories[fc];
        isCreated = TRUE;
    }

    return CRemoteViewsFactoryAdapter::New(factory, isCreated, binder);
}

} // namespace Widget
} // namespace Droid
} // namespace Elastos
