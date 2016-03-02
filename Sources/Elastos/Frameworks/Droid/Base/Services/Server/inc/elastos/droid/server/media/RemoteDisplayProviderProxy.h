
#ifndef __ELASTOS_DROID_SERVER_MEDIA_REMOTEDISPLAYPROVIDERPROXY_H__
#define __ELASTOS_DROID_SERVER_MEDIA_REMOTEDISPLAYPROVIDERPROXY_H__

#include "elastos/droid/server/media/CProviderCallback.h"
#include "elastos/droid/os/Runnable.h"

using Elastos::Droid::Content::IServiceConnection;
using Elastos::Droid::Content::IComponentName;
using Elastos::Droid::Media::IRemoteDisplayState;
using Elastos::Droid::Media::IIRemoteDisplayProvider;
using Elastos::Droid::Os::IBinder;
using Elastos::Droid::Os::Runnable;

namespace Elastos {
namespace Droid {
namespace Server {
namespace Media {

extern "C" const InterfaceID EIID_IRemoteDisplayProviderProxyCallbacks;

/**
 * Maintains a connection to a particular remote display provider service.
 */
class RemoteDisplayProviderProxy
    : public Object
    , public IServiceConnection
{
public:
    interface ICallbacks : public IInterface
    {
    public:
        virtual CARAPI OnDisplayStateChanged(
            /* [in] */ RemoteDisplayProviderProxy* provider,
            /* [in] */ IRemoteDisplayState* state) = 0;
    };

private:
    class DisplayStateChangedRunnable : public Runnable
    {
    public:
        DisplayStateChangedRunnable(
            /* [in] */ RemoteDisplayProviderProxy* host)
            : mHost(host)
        {}

        CARAPI Run();

    private:
        RemoteDisplayProviderProxy* mHost;
    };

    class Connection
        : public Object
        , public IProxyDeathRecipient
    {
    private:
        class RegisterRunnable : public Runnable
        {
        public:
            RegisterRunnable(
                /* [in] */ Connection* host)
                : mHost(host)
            {}

            CARAPI Run();

        private:
            Connection* mHost;
        };

        class ProxyDiedRunnable : public Runnable
        {
        public:
            ProxyDiedRunnable(
                /* [in] */ Connection* host)
                : mHost(host)
            {}

            CARAPI Run();

        private:
            Connection* mHost;
        };

        class PostStateChangedRunnable : public Runnable
        {
        public:
            PostStateChangedRunnable(
                /* [in] */ Connection* host,
                /* [in] */ IRemoteDisplayState* state)
                : mHost(host)
                , mState(state)
            {}

            CARAPI Run();

        private:
            Connection* mHost;
            AutoPtr<IRemoteDisplayState> mState;
        };

    public:
        Connection(
            /* [in] */ IIRemoteDisplayProvider* provider,
            /* [in] */ RemoteDisplayProviderProxy* host);

        CAR_INTERFACE_DECL()

        CARAPI_(Boolean) Register();

        CARAPI_(void) Dispose();

        CARAPI_(void) SetDiscoveryMode(
            /* [in] */ Int32 mode);

        CARAPI_(void) Connect(
            /* [in] */ const String& id);

        CARAPI_(void) Disconnect(
            /* [in] */ const String& id);

        CARAPI_(void) SetVolume(
            /* [in] */ const String& id,
            /* [in] */ Int32 volume);

        CARAPI_(void) AdjustVolume(
            /* [in] */ const String& id,
            /* [in] */ Int32 volume);

        // @Override
        CARAPI ProxyDied();

        CARAPI_(void) PostStateChanged(
            /* [in] */ IRemoteDisplayState* state);

    private:
        AutoPtr<IIRemoteDisplayProvider> mProvider;
        AutoPtr<CProviderCallback> mCallback;
        RemoteDisplayProviderProxy* mHost;
    };

public:
    RemoteDisplayProviderProxy(
        /* [in] */ IContext* context,
        /* [in] */ IComponentName* componentName,
        /* [in] */ Int32 userId);

    CAR_INTERFACE_DECL()

    // CARAPI_(void) dump(PrintWriter pw, String prefix);

    CARAPI_(void) SetCallback(
        /* [in] */ ICallback* callback);

    CARAPI_(AutoPtr<IRemoteDisplayState>) GetDisplayState();

    CARAPI_(void) SetDiscoveryMode(
        /* [in] */ Int32 mode);

    CARAPI_(void) SetSelectedDisplay(
        /* [in] */ const String& id);

    CARAPI_(void) SetDisplayVolume(
        /* [in] */ Int32 volume);

    CARAPI_(void) AdjustDisplayVolume(
        /* [in] */ Int32 delta);

    CARAPI_(Boolean) HasComponentName(
        /* [in] */ const String& packageName,
        /* [in] */ const String& className);

    CARAPI_(String) GetFlattenedComponentName();

    CARAPI_(void) Start();

    CARAPI_(void) Stop();

    CARAPI_(void) RebindIfDisconnected();

    CARAPI OnServiceConnected(
        /* [in] */ IComponentName* name,
        /* [in] */ IBinder* service);

    CARAPI OnServiceDisconnected(
        /* [in] */ IComponentName* name);

    CARAPI ToString(
        /* [out] */ String* str);

private:
    CARAPI_(void) UpdateBinding();

    CARAPI_(Boolean) ShouldBind();

    CARAPI_(void) Bind();

    CARAPI_(void) Unbind();

    CARAPI_(void) OnConnectionReady(
        /* [in] */ Connection* connection);

    CARAPI_(void) OnConnectionDied(
        /* [in] */ Connection* connection);

    CARAPI_(void) OnDisplayStateChanged(
        /* [in] */ Connection* connection,
        /* [in] */ IRemoteDisplayState* state);

    CARAPI_(void) Disconnect();

    CARAPI_(void) SetDisplayState(
        /* [in] */ IRemoteDisplayState* state);

private:
    static const String TAG;  // max. 23 chars
    static const Boolean DEBUG = FALSE;

    AutoPtr<IContext> mContext;
    AutoPtr<IComponentName> mComponentName;
    Int32 mUserId;
    AutoPtr<IHandler> mHandler;

    AutoPtr<ICallback> mDisplayStateCallback;

    // Connection state
    Boolean mRunning;
    Boolean mBound;
    AutoPtr<Connection> mActiveConnection;
    Boolean mConnectionReady;

    // Logical state
    Int32 mDiscoveryMode;
    String mSelectedDisplayId;
    AutoPtr<IRemoteDisplayState> mDisplayState;
    Boolean mScheduledDisplayStateChangedCallback;

    AutoPtr<IRunnable> mDisplayStateChanged;
};

} // namespace Media
} // namespace Server
} // namespace Droid
} // namespace Elastos

#endif // __ELASTOS_DROID_SERVER_MEDIA_REMOTEDISPLAYPROVIDERPROXY_H__
