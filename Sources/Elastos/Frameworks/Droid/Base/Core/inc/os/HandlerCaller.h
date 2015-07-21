#ifndef __ELASTOS_DROID_OS_HANDLER_CALLER_H__
#define __ELASTOS_DROID_OS_HANDLER_CALLER_H__

#include "os/Handler.h"

using Elastos::Droid::Content::IContext;
using Elastos::Droid::Os::Handler;
using Elastos::Droid::Os::IHandlerCallerCallback;

namespace Elastos {
namespace Droid {
namespace Os {

class HandlerCaller
    : public Object
    , public IHandlerCaller
{
private:
    class MyHandler
        : public Handler
    {
    public:
        MyHandler(
            /* [in] */ ILooper* looper,
            /* [in] */ IWeakReference* host);

        CARAPI HandleMessage(
            /* [in] */ IMessage* msg);

    private:
        AutoPtr<IWeakReference> mWeakHost;
    };

protected:
    HandlerCaller();

public:
    CAR_INTERFACE_DECL()

    CARAPI GetWeakReference(
        /* [out] */ IWeakReference** weakReference);

    HandlerCaller(
        /* [in] */ IContext* context,
        /* [in] */ IHandlerCallerCallback* callback,
        /* [in] */ Boolean isStrong);

    HandlerCaller(
        /* [in] */ IContext* context,
        /* [in] */ ILooper* looper,
        /* [in] */ IHandlerCallerCallback* callback,
        /* [in] */ Boolean isStrong);

    constructor(
        /* [in] */ IContext* context,
        /* [in] */ IHandlerCallerCallback* callback,
        /* [in] */ Boolean isStrong);

    constructor(
        /* [in] */ IContext* context,
        /* [in] */ ILooper* looper,
        /* [in] */ IHandlerCallerCallback* callback,
        /* [in] */ Boolean isStrong);

    CARAPI GetContext(
        /* [out] */ IContext** ctx);

    CARAPI SetContext(
        /* [in] */ IContext* ctx);

    CARAPI ExecuteOrSendMessage(
        /* [in] */ IMessage* msg);

    CARAPI HasMessages(
        /* [in] */ Int32 what,
        /* [out] */ Boolean* result);

    CARAPI RemoveMessages(
        /* [in] */ Int32 what);

    CARAPI RemoveMessages(
        /* [in] */ Int32 what,
        /* [in] */ IInterface* obj);

    CARAPI SendMessage(
        /* [in] */ IMessage* msg);

    CARAPI ObtainMessage(
        /* [in] */ Int32 what,
        /* [out] */ IMessage** msg);

    CARAPI ObtainMessage(
        /* [in] */ Int32 what,
        /* [in] */ Boolean arg1,
        /* [in] */ IInterface* arg2,
        /* [out] */ IMessage** msg);

    CARAPI ObtainMessage(
        /* [in] */ Int32 what,
        /* [in] */ Boolean arg1,
        /* [in] */ IInterface* arg2,
        /* [in] */ IInterface* arg3,
        /* [out] */ IMessage** msg);

    CARAPI ObtainMessage(
        /* [in] */ Int32 what,
        /* [in] */ IInterface* arg1,
        /* [out] */ IMessage** msg);

    CARAPI ObtainMessage(
        /* [in] */ Int32 what,
        /* [in] */ Int32 arg1,
        /* [out] */ IMessage** msg);

    CARAPI ObtainMessage(
        /* [in] */ Int32 what,
        /* [in] */ Int32 arg1,
        /* [in] */ Int32 arg2,
        /* [out] */ IMessage** msg);

    CARAPI ObtainMessage(
        /* [in] */ Int32 what,
        /* [in] */ Int32 arg1,
        /* [in] */ IInterface* arg2,
        /* [out] */ IMessage** msg);

    CARAPI ObtainMessage(
        /* [in] */ Int32 what,
        /* [in] */ Int32 arg1,
        /* [in] */ Int32 arg2,
        /* [in] */ IInterface* arg3,
        /* [out] */ IMessage** msg);

    CARAPI ObtainMessage(
        /* [in] */ Int32 what,
        /* [in] */ Int32 arg1,
        /* [in] */ Int32 arg2,
        /* [in] */ IInterface* arg3,
        /* [in] */ IInterface* arg4,
        /* [out] */ IMessage** msg);

    CARAPI ObtainMessage(
        /* [in] */ Int32 what,
        /* [in] */ Int32 arg1,
        /* [in] */ IInterface* arg2,
        /* [in] */ IInterface* arg3,
        /* [out] */ IMessage** msg);

    CARAPI ObtainMessage(
        /* [in] */ Int32 what,
        /* [in] */ IInterface* arg1,
        /* [in] */ IInterface* arg2,
        /* [out] */ IMessage** msg);

    CARAPI ObtainMessage(
        /* [in] */ Int32 what,
        /* [in] */ IInterface* arg1,
        /* [in] */ IInterface* arg2,
        /* [in] */ IInterface* arg3,
        /* [out] */ IMessage** msg);

    CARAPI ObtainMessage(
        /* [in] */ Int32 what,
        /* [in] */ IInterface* arg1,
        /* [in] */ IInterface* arg2,
        /* [in] */ IInterface* arg3,
        /* [in] */ IInterface* arg4,
        /* [out] */ IMessage** msg);

    CARAPI ObtainMessage(
        /* [in] */ Int32 what,
        /* [in] */ Int32 arg1,
        /* [in] */ Int32 arg2,
        /* [in] */ Int32 arg3,
        /* [in] */ Int32 arg4,
        /* [out] */ IMessage** msg);

    CARAPI ObtainMessage(
        /* [in] */ Int32 what,
        /* [in] */ Int32 arg1,
        /* [in] */ Int32 arg2,
        /* [in] */ Int32 arg3,
        /* [in] */ Int32 arg4,
        /* [in] */ Int32 arg5,
        /* [out] */ IMessage** msg);

    CARAPI ObtainMessage(
        /* [in] */ Int32 what,
        /* [in] */ Int32 arg1,
        /* [in] */ Int32 arg2,
        /* [in] */ Int32 arg3,
        /* [in] */ Int32 arg4,
        /* [in] */ Int32 arg5,
        /* [in] */ Int32 arg6,
        /* [out] */ IMessage** msg);

    CARAPI ObtainMessage(
        /* [in] */ Int32 what,
        /* [in] */ Int32 arg1,
        /* [in] */ Int32 arg2,
        /* [in] */ Int32 arg3,
        /* [in] */ Int32 arg4,
        /* [in] */ IInterface* arg5,
        /* [out] */ IMessage** msg);

private:
    AutoPtr<IHandlerCallerCallback> GetCallback();

private:
    AutoPtr<IContext> mContext;

    AutoPtr<ILooper> mMainLooper;
    AutoPtr<MyHandler> mH;

    AutoPtr<IHandlerCallerCallback> mCallback;
    AutoPtr<IWeakReference> mWeakCallback;
};

} // namespace Os
} // namespace Droid
} // namespace Elastos

#endif //__ELASTOS_DROID_OS_HANDLER_CALLER_H__
