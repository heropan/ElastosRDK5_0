#ifndef __ELASTOS_DROID_WIDGET_CRemoteViewsAdapterServiceConnection_H__
#define __ELASTOS_DROID_WIDGET_CRemoteViewsAdapterServiceConnection_H__

#include "_CRemoteViewsAdapterServiceConnection.h"
#include "os/Runnable.h"

using Elastos::Droid::Os::Runnable;
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
        MyRunnable(
            /* [in] */ IRemoteViewsAdapter* adapter);

        CARAPI Run();
    private:
        AutoPtr<IRemoteViewsAdapter> mAdapter;
    };

    class MyRunnableEx2 : public Runnable
    {
    public:
        MyRunnable(
            /* [in] */ IRemoteViewsAdapter* adapter);

        CARAPI Run();
    private:
        AutoPtr<IRemoteViewsAdapter> mAdapter;
    };
private:
    Object mLock;
    Boolean mIsConnected;
    Boolean mIsConnecting;
    IRemoteViewsAdapter* mAdapter;
    AutoPtr<IIRemoteViewsFactory> mRemoteViewsFactory;
};

} // namespace Widget
} // namespace Droid
} // namespace Elastos

#endif
