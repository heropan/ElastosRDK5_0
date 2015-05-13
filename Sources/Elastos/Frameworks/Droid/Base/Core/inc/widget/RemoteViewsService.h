
#ifndef __REMOTEVIEWSSERVICE_H__
#define __REMOTEVIEWSSERVICE_H__

#include "ext/frameworkext.h"
#include "app/Service.h"
#include <elastos/Mutex.h>
#include <elastos/HashMap.h>

using Elastos::Utility::HashMap;
using Elastos::Core::Threading::Mutex;
using Elastos::Droid::Content::IIntentFilterComparison;
using Elastos::Droid::Widget::Internal::IIRemoteViewsFactory;

namespace Elastos {
namespace Droid {
namespace Widget {

class CRemoteViewsFactoryAdapter;

class RemoteViewsService : public Elastos::Droid::App::Service
{
public:
    virtual CARAPI OnBind(
        /* [in] */ IIntent* intent,
        /* [out] */ IBinder** binder);

    virtual CARAPI OnGetViewFactory(
        /* [in] */ IIntent* intent,
        /* [out] */ IRemoteViewsFactory** factory) = 0;

private:
    static const String TAG;

    static HashMap< AutoPtr<IIntentFilterComparison>, AutoPtr<IRemoteViewsFactory> > sRemoteViewFactories;
    static Mutex sLock;
    friend class CRemoteViewsFactoryAdapter;
};

} // namespace Widget
} // namespace Droid
} // namespace Elastos

#endif //__REMOTEVIEWSSERVICE_H__
