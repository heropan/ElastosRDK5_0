#ifndef __ELASTOS_DROID_SERVER_CONNECTIVITY_VPN_H__
#define __ELASTOS_DROID_SERVER_CONNECTIVITY_VPN_H__

#include "CConnectivityService.h"
#include <net/BaseNetworkStateTracker.h>

using Elastos::Security::IKeyStore;
using Elastos::Droid::Content::IComponentName;
using Elastos::Droid::Content::IServiceConnection;
using Elastos::Droid::Graphics::IBitmap;
using Elastos::Droid::Net::BaseNetworkStateTracker;
using Elastos::Droid::Net::ILinkProperties;
using Elastos::Droid::Net::ILocalSocket;
using Elastos::Droid::Net::NetworkInfoDetailedState;
using Elastos::Droid::Os::INetworkManagementService;
using Elastos::Droid::Internal::Net::IVpnConfig;
using Elastos::Droid::Internal::Net::IVpnProfile;
using Elastos::Droid::Internal::Net::ILegacyVpnInfo;

namespace Elastos {
namespace Droid {
namespace Server {
namespace Connectivity {

class Vpn : public BaseNetworkStateTracker
{
private:
    class Connection
        : public ElRefBase
        , public IServiceConnection
    {
    public:
        Connection(
            /* [in] */ Vpn* owner)
        : mOwner(owner)
        {}

        CAR_INTERFACE_DECL();

        CARAPI OnServiceConnected(
            /* [in] */ IComponentName* name,
            /* [in] */ IBinder* service);

        CARAPI OnServiceDisconnected(
            /* [in] */ IComponentName* name);

    private:
        Vpn* mOwner;
        AutoPtr<IBinder> mService;
    };

    class LegacyVpnRunner : public ThreadBase
    {
    public:
        LegacyVpnRunner(
            /* [in] */ IVpnConfig* config,
            /* [in] */ ArrayOf<String>* racoon,
            /* [in] */ ArrayOf<String>* mtpd,
            /* [in] */ Vpn* owner);

        CARAPI Check(
            /* [in] */ const String& interfaze);

        CARAPI Exit();

        CARAPI Run();

    private:
        CARAPI Checkpoint(
                /* [in] */ Boolean yield);

        CARAPI Execute();

        CARAPI_(void) MonitorDaemons();

    private:
        friend class Vpn;

        static const String TAG;
        static Object sTAGLock;

        AutoPtr<IVpnConfig> mConfig;
        AutoPtr<ArrayOf<String> > mDaemons;
        AutoPtr<ArrayOf<StringArray> > mArguments;
        AutoPtr<ArrayOf<ILocalSocket*> > mSockets;
        String mOuterInterface;

        Int64 mTimer;
        Vpn* mOwner;
    };

public:
    Vpn(
        /* [in] */ IContext* context,
        /* [in] */ CConnectivityService::VpnCallback* callback,
        /* [in] */ INetworkManagementService* netService);

    CARAPI SetEnableNotifications(
        /* [in] */ Boolean enableNotif);

    CARAPI Teardown(
        /* [out] */ Boolean* result);

    CARAPI Reconnect(
        /* [out] */ Boolean* result);

    CARAPI GetTcpBufferSizesPropName(
        /* [out] */ String* name);

    /**
     * Prepare for a VPN application. This method is designed to solve
     * race conditions. It first compares the current prepared package
     * with {@code oldPackage}. If they are the same, the prepared
     * package is revoked and replaced with {@code newPackage}. If
     * {@code oldPackage} is {@code null}, the comparison is omitted.
     * If {@code newPackage} is the same package or {@code null}, the
     * revocation is omitted. This method returns {@code true} if the
     * operation is succeeded.
     *
     * Legacy VPN is handled specially since it is not a real package.
     * It uses {@link VpnConfig#LEGACY_VPN} as its package name, and
     * it can be revoked by itself.
     *
     * @param oldPackage The package name of the old VPN application.
     * @param newPackage The package name of the new VPN application.
     * @return true if the operation is succeeded.
     */
    CARAPI Prepare(
        /* [in] */ const String& oldPackage,
        /* [in] */ const String& newPackage,
        /* [out] */ Boolean* result);

    /**
     * Protect a socket from routing changes by binding it to the given
     * interface. The socket is NOT closed by this method.
     *
     * @param socket The socket to be bound.
     * @param interfaze The name of the interface.
     */
    CARAPI Protect(
        /* [in] */ IParcelFileDescriptor *socket,
        /* [in] */ const String& interfaze);

    /**
     * Establish a VPN network and return the file descriptor of the VPN
     * interface. This methods returns {@code null} if the application is
     * revoked or not prepared.
     *
     * @param config The parameters to configure the network.
     * @return The file descriptor of the VPN interface.
     */
    CARAPI Establish(
        /* [in] */ IVpnConfig* config,
        /* [out] */ IParcelFileDescriptor** tun);

    CARAPI InterfaceStatusChanged(
        /* [in] */ const String& iface,
        /* [in] */ Boolean up);

    /**
     * Start legacy VPN, controlling native daemons as needed. Creates a
     * secondary thread to perform connection work, returning quickly.
     */
    CARAPI StartLegacyVpn(
        /* [in] */ IVpnProfile* socket,
        /* [in] */ IKeyStore* interfaze,
        /* [in] */ ILinkProperties* egress);

    CARAPI StopLegacyVpn();

    CARAPI GetLegacyVpnInfo(
        /* [out] */ ILegacyVpnInfo** info);

    CARAPI GetLegacyVpnConfig(
        /* [out] */ IVpnConfig** config);

protected:
    CARAPI_(void) StartMonitoringInternal();

private:
    /**
     * Update current state, dispaching event to listeners.
     */
    CARAPI_(void) UpdateState(
        /* [in] */ NetworkInfoDetailedState detailedState,
        /* [in] */ const String& reason);

    CARAPI EnforceControlPermission();

    CARAPI_(void) ShowNotification(
        /* [in] */ IVpnConfig* config,
        /* [in] */ const String& label,
        /* [in] */ IBitmap* icon);

    CARAPI_(void) HideNotification();

    CARAPI NativeCreate(
        /* [in] */ Int32 mtu,
        /* [out] */ Int32* fd);

    CARAPI NativeGetName(
        /* [in] */ Int32 tun,
        /* [out] */ String* vpnName);

    CARAPI_(Int32) NativeSetAddresses(
        /* [in] */ const String& interfaze,
        /* [in] */ const String& addresses);

    CARAPI_(Int32) NativeSetRoutes(
        /* [in] */ const String& interfaze,
        /* [in] */ const String& routes);

    CARAPI NativeReset(
        /* [in] */ const String& interfaze);

    CARAPI_(Int32) NativeCheck(
        /* [in] */ const String& interfaze);

    CARAPI NativeProtect(
        /* [in] */ Int32 socket,
        /* [in] */ const String& interfaze);

    static CARAPI FindLegacyVpnGateway(
        /* [in] */ ILinkProperties* prop,
        /* [out] */ String* address);

    CARAPI_(void) StartLegacyVpn(
        /* [in] */ IVpnConfig* config,
        /* [in] */ ArrayOf<String>* racoon,
        /* [in] */ ArrayOf<String>* mtpd);

private:
    friend class CVpnObserver;
    static const String TAG;
    static const Boolean LOGD;

    AutoPtr<CConnectivityService::VpnCallback> mCallback;

    String mPackage;
    String mInterface;
    AutoPtr<Connection> mConnection;
    AutoPtr<LegacyVpnRunner> mLegacyVpnRunner;
    AutoPtr<IPendingIntent> mStatusIntent;
    Boolean mEnableNotif;
    AutoPtr<INetworkManagementEventObserver> mObserver;
    Object mLock;
};

} // Connectivity
} // Server
} // Droid
} // Elastos

#endif
