
#ifndef __ELASTOS_DROID_HARDWARE_CAMERA2_DISPATCH_HANDLERDISPATCHER_H__
#define __ELASTOS_DROID_HARDWARE_CAMERA2_DISPATCH_HANDLERDISPATCHER_H__

#include "Elastos.Droid.Core_server.h"
#include "elastos/droid/ext/frameworkext.h"
#include "elastos/droid/os/Runnable.h"
#include <elastos/core/Object.h>

using Elastos::Droid::Os::Runnable;
using Elastos::Droid::Os::IHandler;
using Elastos::Core::Object;

namespace Elastos {
namespace Droid {
namespace Hardware {
namespace Camera2 {
namespace Dispatch {

class HandlerDispatcher
    : public Object
    , public IHandlerDispatcher
    , public IDispatchable
{
private:
    class MyRunnable
        : public Runnable
    {
    public:
        MyRunnable(
            /* [in] */ HandlerDispatcher* host,
            /* [in] */ IMethodInfo* method,
            /* [in] */ ArrayOf<IInterface*>* args);

        CARAPI Run();

    private:
        HandlerDispatcher* mHost;
        AutoPtr<IMethodInfo> mMethod;
        AutoPtr<ArrayOf<IInterface*> > mArgs;
    };

public:
    CAR_INTERFACE_DECL()

    HandlerDispatcher();

    virtual ~HandlerDispatcher() {}

    CARAPI constructor();

    /**
     * Create a dispatcher that forwards it's dispatch calls by posting
     * them onto the {@code handler} as a {@code Runnable}.
     *
     * @param dispatchTarget the destination whose method calls will be redirected into the handler
     * @param handler all calls into {@code dispatchTarget} will be posted onto this handler
     * @param <T> the type of the element you want to wrap.
     * @return a dispatcher that will forward it's dispatch calls to a handler
     */
    CARAPI constructor(
        /* [in] */ IDispatchable* dispatchTarget,
        /* [in] */ IHandler* handler);

    //@Override
    CARAPI Dispatch(
        /* [in] */ IMethodInfo* method,
        /* [in] */ ArrayOf<IInterface*>* args);

private:
    static const String TAG;

    AutoPtr<IDispatchable> mDispatchTarget;
    AutoPtr<IHandler> mHandler;
};

} // namespace Dispatch
} // namespace Camera2
} // namespace Hardware
} // namespace Droid
} // namespace Elastos

#endif //__ELASTOS_DROID_HARDWARE_CAMERA2_DISPATCH_HANDLERDISPATCHER_H__
