
#ifndef __ELASTOS_DROID_PACKAGES_SYSTEMUI_STATUSBAR_SERVICEMONITOR_H__
#define __ELASTOS_DROID_PACKAGES_SYSTEMUI_STATUSBAR_SERVICEMONITOR_H__

#include "_SystemUI.h"
#include "Elastos.Droid.Content.h"
#include <elastos/droid/content/BroadcastReceiver.h>
#include <elastos/droid/database/ContentObserver.h>
#include <elastos/droid/os/Handler.h>
#include <elastos/core/Object.h>

using Elastos::Droid::Content::BroadcastReceiver;
using Elastos::Droid::Content::IComponentName;
using Elastos::Droid::Content::IServiceConnection;
using Elastos::Droid::Database::ContentObserver;
using Elastos::Droid::Os::Handler;
using Elastos::Core::Object;

namespace Elastos {
namespace Droid {
namespace Packages {
namespace SystemUI {
namespace StatusBar {

/**
 * Manages a persistent connection to a service component defined in a secure setting.
 *
 * <p>If a valid service component is specified in the secure setting, starts it up and keeps it
 * running; handling setting changes, package updates, component disabling, and unexpected
 * process termination.
 *
 * <p>Clients can listen for important events using the supplied {@link Callbacks}.
 */
class ServiceMonitor: public Object
{
public:
    class MonitorHandler: public Handler
    {
    public:
        MonitorHandler(
            /* [in] */ ServiceMonitor* host);

        CARAPI HandleMessage(
            /* [in] */ IMessage* msg);

    private:
        ServiceMonitor* mHost;
    };

    class SettingObserver : public ContentObserver
    {
    public:
        SettingObserver(
            /* [in] */ IHandler* handler,
            /* [in] */ ServiceMonitor* host);

        CARAPI OnChange(
            /* [in] */ Boolean selfChange);

        CARAPI OnChange(
            /* [in] */ Boolean selfChange,
            /* [in] */ IUri* uri);

    private:
        ServiceMonitor* mHost;
    };

    class SC
        : public Object
        , public IServiceConnection
        , public IProxyDeathRecipient
    {
    public:
        CAR_INTERFACE_DECL()

        SC(
            /* [in] */ ServiceMonitor* host);

        CARAPI OnServiceConnected(
            /* [in] */ IComponentName* name,
            /* [in] */ IBinder* service);

        CARAPI OnServiceDisconnected(
            /* [in] */ IComponentName* name);

        CARAPI ProxyDied();

    private:
        AutoPtr<IComponentName> mName;
        AutoPtr<IBinder> mService;
        ServiceMonitor* mHost;
    };

    class MonitorBroadcastReceiver: public BroadcastReceiver
    {
    public:
        MonitorBroadcastReceiver(
            /* [in] */ ServiceMonitor* host);

        CARAPI OnReceive(
            /* [in] */ IContext* context,
            /* [in] */ IIntent* intent);

    private:
        ServiceMonitor* mHost;
    };

public:
    ServiceMonitor(
        /* [in] */ const String& ownerTag,
        /* [in] */ Boolean debug,
        /* [in] */ IContext* context,
        /* [in] */ const String& settingKey,
        /* [in] */ IServiceMonitorCallbacks* callbacks);

    CARAPI_(void) Start();

private:
    CARAPI_(AutoPtr<IComponentName>) GetComponentNameFromSetting();

    // everything below is called on the handler
    CARAPI_(void) PackageIntent(
        /* [in] */ IIntent* intent);

    CARAPI_(void) StopService();

    CARAPI_(void) StartService();

    CARAPI_(void) ContinueStartService();

    CARAPI_(void) ServiceDisconnected(
        /* [in] */ IComponentName* serviceName);

    CARAPI_(void) CheckBound();

    CARAPI_(void) ScheduleCheckBound();

    static CARAPI_(String) BundleToString(
        /* [in] */ IBundle* bundle);

public:
    // internal handler + messages used to serialize access to internal state
    static const Int32 MSG_START_SERVICE;
    static const Int32 MSG_CONTINUE_START_SERVICE;
    static const Int32 MSG_STOP_SERVICE;
    static const Int32 MSG_PACKAGE_INTENT;
    static const Int32 MSG_CHECK_BOUND;
    static const Int32 MSG_SERVICE_DISCONNECTED;

private:
    static const Int32 RECHECK_DELAY;
    static const Int32 WAIT_FOR_STOP;
    AutoPtr<IHandler> mHandler;
    AutoPtr<IContentObserver> mSettingObserver;
    AutoPtr<IBroadcastReceiver> mBroadcastReceiver;

    String mTag;
    Boolean mDebug;

    AutoPtr<IContext> mContext;
    String mSettingKey;
    AutoPtr<IServiceMonitorCallbacks> mCallbacks;

    AutoPtr<IComponentName> mServiceName;
    AutoPtr<SC> mServiceConnection;
    Boolean mBound;
};

} // namespace StatusBar
} // namespace SystemUI
} // namespace Packages
} // namespace Droid
} // namespace Elastos

#endif // __ELASTOS_DROID_PACKAGES_SYSTEMUI_STATUSBAR_SERVICEMONITOR_H__
