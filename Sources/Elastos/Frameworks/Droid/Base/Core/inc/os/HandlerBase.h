#ifndef __ELASTOS_DROID_OS_HANDLER_BASE_H__
#define __ELASTOS_DROID_OS_HANDLER_BASE_H__

#include "ext/frameworkext.h"
#include "os/HandlerMacro.h"

namespace Elastos {
namespace Droid {
namespace Os {

/**
 * Base class for Handler
 */
class HandlerBase
    : public IHandler
    , public IWeakReferenceSource
    , public ElRefBase
    , public Handler
{
public:
    CAR_INTERFACE_DECL()

    IHANDLER_METHODS_DECL()

    HandlerBase(
        /* [in] */ Boolean async = FALSE);

    HandlerBase(
        /* [in] */ ILooper* looper,
        /* [in] */ Boolean async = FALSE);

    HandlerBase(
        /* [in] */ IHandlerCallback* callback,
        /* [in] */ Boolean takeStrongRefOfCallback,
        /* [in] */ Boolean async = FALSE);

    HandlerBase(
        /* [in] */ ILooper* looper,
        /* [in] */ IHandlerCallback* callback,
        /* [in] */ Boolean takeStrongRefOfCallback,
        /* [in] */ Boolean async = FALSE);

    virtual ~HandlerBase();

    CARAPI GetWeakReference(
        /* [out] */ IWeakReference** weakReference);

    virtual CARAPI HandleMessage(
        /* [in] */ IMessage* msg);
};


} // namespace Os
} // namespace Droid
} // namespace Elastos

#endif //__ELASTOS_DROID_OS_HANDLER_BASE_H__
