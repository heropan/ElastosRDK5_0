#ifndef __ELASTOS_DROID_OS_HANDLER_RUNNABLE_BASE_H__
#define __ELASTOS_DROID_OS_HANDLER_RUNNABLE_BASE_H__



namespace Elastos {
namespace Droid {
namespace Os {

/**
 * Base class for HandlerRunnable
 */
class HandlerRunnableBase
    : public IHandler
    , public IRunnable
    , public ElRefBase
    , public Handler
{
public:
    CAR_INTERFACE_DECL()

    IHANDLER_METHODS_DECL()

    HandlerRunnableBase(
        /* [in] */ Boolean async = FALSE);

    HandlerRunnableBase(
        /* [in] */ ILooper* looper,
        /* [in] */ Boolean async = FALSE);

    HandlerRunnableBase(
        /* [in] */ IHandlerCallback* callback,
        /* [in] */ Boolean takeStrongRefOfCallback,
        /* [in] */ Boolean async = FALSE);

    HandlerRunnableBase(
        /* [in] */ ILooper* looper,
        /* [in] */ IHandlerCallback* callback,
        /* [in] */ Boolean takeStrongRefOfCallback,
        /* [in] */ Boolean async = FALSE);

    virtual ~HandlerRunnableBase();

    virtual CARAPI HandleMessage(
        /* [in] */ IMessage* msg);
};


} // namespace Os
} // namespace Droid
} // namespace Elastos

#endif //__ELASTOS_DROID_OS_HANDLER_RUNNABLE_BASE_H__
