
#ifndef __ELASTOS_DROID_SERVER_LOCKDOWNVPNTRACKER_H__
#define __ELASTOS_DROID_SERVER_LOCKDOWNVPNTRACKER_H__

#include "Elastos.Droid.Server.h"
#include "elastos/droid/ext/frameworkext.h"
#include "elastos/droid/content/BroadcastReceiver.h"

using Elastos::Droid::App::IPendingIntent;
using Elastos::Droid::Content::BroadcastReceiver;
using Elastos::Droid::Content::IBroadcastReceiver;
using Elastos::Droid::Content::IContext;
using Elastos::Droid::Content::IIntent;
using Elastos::Droid::Net::INetworkInfo;
using Elastos::Droid::Os::INetworkManagementService;
using Elastos::Droid::Internal::Net::IVpnProfile;

namespace Elastos {
namespace Droid {
namespace Server {

class CConnectivityService;

namespace Connectivity {
class Vpn;
}

namespace Net {

/**
 * State tracker for lockdown mode. Watches for normal {@link NetworkInfo} to be
 * connected and kicks off VPN connection, managing any required {@code netd}
 * firewall rules.
 */
class LockdownVpnTracker : public ElRefBase
{
private:
    class TrackerBroadcastReceiver : public BroadcastReceiver
    {
    public:
        TrackerBroadcastReceiver(
            /* [in] */ LockdownVpnTracker* owner) : mOwner(owner)
        {}

        CARAPI OnReceive(
            /* [in] */ IContext* context,
            /* [in] */ IIntent* intent);

        CARAPI ToString(
            /* [out] */ String* info)
        {
            VALIDATE_NOT_NULL(info);
            *info = String("LockdownVpnTracker::TrackerBroadcastReceiver: ");
            (*info).AppendFormat("%p", this);
            return NOERROR;
        }
    private:
        LockdownVpnTracker* mOwner;
    };

public:
    LockdownVpnTracker(
        /* [in] */ IContext* context,
        /* [in] */ INetworkManagementService* netService,
        /* [in] */ CConnectivityService* connService,
        /* [in] */ Elastos::Droid::Server::Connectivity::Vpn* vpn,
        /* [in] */ IVpnProfile* profile);

    static CARAPI_(Boolean) IsEnabled();

    CARAPI Init();

    CARAPI Shutdown();

    CARAPI Reset();

    CARAPI OnNetworkInfoChanged(
        /* [in] */ INetworkInfo* info);

    CARAPI OnVpnStateChanged(
        /* [in] */ INetworkInfo* info);

    CARAPI_(AutoPtr<INetworkInfo>) AugmentNetworkInfo(
        /* [in] */ INetworkInfo* inInfo);

private:
    CARAPI HandleStateChangedLocked();

    CARAPI InitLocked();

    CARAPI ShutdownLocked();

    CARAPI ClearSourceRulesLocked();

    CARAPI_(void) ShowNotification(
        /* [in] */ Int32 titleRes,
        /* [in] */ Int32 iconRes);

    CARAPI_(void) HideNotification();

private:
    static const String TAG;

    /** Number of VPN attempts before waiting for user intervention. */
    static const Int32 MAX_ERROR_COUNT;

    static const String ACTION_LOCKDOWN_RESET;
    static const String ACTION_VPN_SETTINGS;

    AutoPtr<IContext> mContext;
    AutoPtr<INetworkManagementService> mNetService;
    AutoPtr<CConnectivityService> mConnService;
    AutoPtr<Elastos::Droid::Server::Connectivity::Vpn> mVpn;
    AutoPtr<IVpnProfile> mProfile;

    Object mStateLock;

    AutoPtr<IPendingIntent> mResetIntent;

    String mAcceptedEgressIface;
    String mAcceptedIface;
    String mAcceptedSourceAddr;

    Int32 mErrorCount;

    AutoPtr<IBroadcastReceiver> mResetReceiver;
};

} // namespace Net
} // namespace Server
} // namepsace Droid
} // namespace Elastos

#endif // __ELASTOS_DROID_SERVER_LOCKDOWNVPNTRACKER_H__
