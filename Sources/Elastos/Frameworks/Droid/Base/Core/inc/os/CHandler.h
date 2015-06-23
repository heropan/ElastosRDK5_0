
#ifndef __ELASTOS_DROID_OS_CHANDLER_H__
#define __ELASTOS_DROID_OS_CHANDLER_H__

#include "_CHandler.h"


namespace Elastos {
namespace Droid {
namespace Os {

CarClass(CHandler), public Handler
{
public:
    IHANDLER_METHODS_DECL()

    CARAPI constructor();

    CARAPI constructor(
        /* [in] */ IHandlerCallback* callback,
        /* [in] */ Boolean takeStrongRefOfCallback);

    CARAPI constructor(
        /* [in] */ ILooper* looper);

    CARAPI constructor(
        /* [in] */ ILooper* looper,
        /* [in] */ IHandlerCallback* callback,
        /* [in] */ Boolean takeStrongRefOfCallback);

    CARAPI constructor(
        /* [in] */ Boolean async);

    CARAPI constructor(
        /* [in] */ IHandlerCallback* callback,
        /* [in] */ Boolean takeStrongRefOfCallback,
        /* [in] */ Boolean async);

    CARAPI constructor(
        /* [in] */ ILooper* looper,
        /* [in] */ IHandlerCallback* callback,
        /* [in] */ Boolean takeStrongRefOfCallback,
        /* [in] */ Boolean async);

    CARAPI_(PInterface) Probe(
        /* [in] */ REIID riid);

    CARAPI HandleMessage(
        /* [in] */ IMessage* msg);
};

} // namespace Os
} // namespace Droid
} // namespace Elastos

#endif //__ELASTOS_DROID_OS_CHANDLER_H__
