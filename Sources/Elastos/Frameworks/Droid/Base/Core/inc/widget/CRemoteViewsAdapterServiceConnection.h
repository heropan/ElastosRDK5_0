#ifndef __CRemoteViewsAdapterServiceConnection_h__
#define __CRemoteViewsAdapterServiceConnection_h__

#include "_CRemoteViewsAdapterServiceConnection.h"
#include "os/Runnable.h"
#include <elastos/Mutex.h>

using Elastos::Droid::Os::Runnable;
using Elastos::Core::Threading::Mutex;
using Elastos::Droid::Content::IContext;
using Elastos::Droid::Content::IIntent;
using Elastos::Droid::Widget::Internal::IIRemoteViewsFactory;

namespace Elastos {
namespace Droid {
namespace Widget {

CarClass(CRemoteViewsAdapterServiceConnection)
{
public:
    CRemoteViewsAdapterServiceConnection();

    CARAPI constructor(
        /* [in] */ IRemoteViewsAdapter* adapter);

    CARAPI Bind(
        /* [in] */ IContext* ctx,
        /* [in] */ Int32 appWidgetId,
        /* [in] */ IIntent* intent);

    CARAPI Unbind(
        /* [in] */ IContext* ctx,
        /* [in] */ Int32 appWidgetId,
        /* [in] */ IIntent* intent);

    CARAPI GetRemoteViewsFactory(
        /* [out] */ IIRemoteViewsFactory** factory);

    CARAPI IsConnected(
        /* [out] */ Boolean* isConnected);

    CARAPI OnServiceConnected(
        /* [in] */ IBinder* service);

    CARAPI OnServiceDisconnected();

    CARAPI ToString(
        /* [out] */ String* str);
private:
    class MyRunnable : public Runnable
    {
    public:
        MyRunnable(
            /* [in] */ IRemoteViewsAdapter* adapter);

        CARAPI Run();
    private:
        AutoPtr<IRemoteViewsAdapter> mAdapter;
    };

    class MyRunnableEx : public Runnable
    {
    public:
        MyRunnableEx(
            /* [in] */ IRemoteViewsAdapter* adapter);

        CARAPI Run();
    private:
        AutoPtr<IRemoteViewsAdapter> mAdapter;
    };

    class MyRunnableEx2 : public Runnable
    {
    public:
        MyRunnableEx2(
            /* [in] */ IRemoteViewsAdapter* adapter);

        CARAPI Run();
    private:
        AutoPtr<IRemoteViewsAdapter> mAdapter;
    };
private:
    Mutex mLock;
    Boolean mIsConnected;
    Boolean mIsConnecting;
    IRemoteViewsAdapter* mAdapter;
    AutoPtr<IIRemoteViewsFactory> mRemoteViewsFactory;
};

} // namespace Widget
} // namespace Droid
} // namespace Elastos

#endif