
#include "elastos/droid/net/LockdownVpnTracker.h"
#include "connectivity/Vpn.h"
#include "CConnectivityService.h"
#include "elastos/droid/app/NotificationManager.h"
#include "elastos/droid/R.h"
#include "elastos/droid/Manifest.h"
#include <elastos/utility/logging/Slogger.h>

using Elastos::Core::ICharSequence;
using Elastos::Core::CString;
using Elastos::Utility::Logging::Slogger;
using Elastos::Droid::R;
using Elastos::Droid::Content::CIntent;
using Elastos::Droid::Content::IIntentFilter;
using Elastos::Droid::Content::CIntentFilter;
using Elastos::Droid::Net::NetworkInfoState;
using Elastos::Droid::Net::NetworkInfoState_DISCONNECTED;
using Elastos::Droid::Net::NetworkInfoDetailedState_FAILED;
using Elastos::Droid::Net::CNetworkInfo;
using Elastos::Droid::Server::Connectivity::Vpn;

namespace Elastos {
namespace Droid {
namespace Server {
namespace Net {

ECode LockdownVpnTracker::TrackerBroadcastReceiver::OnReceive(
    /* [in] */ IContext* context,
    /* [in] */ IIntent* intent)
{
    return mOwner->Reset();
}

const String LockdownVpnTracker::TAG("LockdownVpnTracker");
const Int32 LockdownVpnTracker::MAX_ERROR_COUNT = 4;
const String LockdownVpnTracker::ACTION_LOCKDOWN_RESET("com.android.server.action.LOCKDOWN_RESET");
const String LockdownVpnTracker::ACTION_VPN_SETTINGS("android.net.vpn.SETTINGS");

/**
 * State tracker for lockdown mode. Watches for normal {@link NetworkInfo} to be
 * connected and kicks off VPN connection, managing any required {@code netd}
 * firewall rules.
 */
LockdownVpnTracker::LockdownVpnTracker(
    /* [in] */ IContext* context,
    /* [in] */ INetworkManagementService* netService,
    /* [in] */ CConnectivityService* connService,
    /* [in] */ Elastos::Droid::Server::Connectivity::Vpn* vpn,
    /* [in] */ IVpnProfile* profile)
    : mContext(context)
    , mNetService(netService)
    , mConnService(connService)
    , mVpn(vpn)
    , mProfile(profile)
    , mErrorCount(0)
{
    mResetReceiver = new TrackerBroadcastReceiver(this);

    AutoPtr<IIntent> resetIntent;
    CIntent::New(ACTION_LOCKDOWN_RESET, (IIntent**)&resetIntent);
    resetIntent->AddFlags(IIntent::FLAG_RECEIVER_REGISTERED_ONLY);
    AutoPtr<IPendingIntentHelper> helper;
    CPendingIntentHelper::AcquireSingleton((IPendingIntentHelper**)&helper);
    helper->GetBroadcast(mContext, 0, resetIntent, 0, (IPendingIntent**)&mResetIntent);
}

/**
 * State tracker for lockdown mode. Watches for normal {@link NetworkInfo} to be
 * connected and kicks off VPN connection, managing any required {@code netd}
 * firewall rules.
 */
Boolean LockdownVpnTracker::IsEnabled()
{
    // return KeyStore.getInstance().contains(Credentials.LOCKDOWN_VPN);
    return FALSE;
}

/**
 * Watch for state changes to both active egress network, kicking off a VPN
 * connection when ready, or setting firewall rules once VPN is connected.
 */
ECode LockdownVpnTracker::HandleStateChangedLocked()
{
    Slogger::D(TAG, "handleStateChanged()");

    AutoPtr<INetworkInfo> egressInfo;
    mConnService->GetActiveNetworkInfoUnfiltered((INetworkInfo**)&egressInfo);
    AutoPtr<ILinkProperties> egressProp;
    mConnService->GetActiveLinkProperties((ILinkProperties**)&egressProp);

    AutoPtr<INetworkInfo> vpnInfo;
    mVpn->GetNetworkInfo((INetworkInfo**)&vpnInfo);
    AutoPtr<IVpnConfig> vpnConfig;
    mVpn->GetLegacyVpnConfig((IVpnConfig**)&vpnConfig);

    // Restart VPN when egress network disconnected or changed
    NetworkInfoState state;
    Boolean egressDisconnected = egressInfo == NULL
            || NetworkInfoState_DISCONNECTED == (egressInfo->GetState(&state), state);
    String ifaceName;
    Boolean egressChanged = egressProp == NULL
            || (egressProp->GetInterfaceName(&ifaceName), mAcceptedEgressIface.Equals(ifaceName));
    if (egressDisconnected || egressChanged) {
        ClearSourceRulesLocked();
        mAcceptedEgressIface = NULL;
        mVpn->StopLegacyVpn();
    }
    if (egressDisconnected) return NOERROR;

    Int32 egressType;
    egressInfo->GetType(&egressType);
    NetworkInfoDetailedState dstate;
    vpnInfo->GetDetailedState(&dstate);
    if (dstate == NetworkInfoDetailedState_FAILED) {
        //EventLogTags.writeLockdownVpnError(egressType);
    }

    Boolean isConnected;
    if (mErrorCount > MAX_ERROR_COUNT) {
        ShowNotification(R::string::vpn_lockdown_error, R::drawable::vpn_disconnected);
    }
    else if ((egressInfo->IsConnected(&isConnected), isConnected) &&
            (vpnInfo->IsConnectedOrConnecting(&isConnected), !isConnected)) {
        Boolean isValid;
        mProfile->IsValidLockdownProfile(&isValid);
        if (isValid) {
            Slogger::D(TAG, "Active network connected; starting VPN");
            //EventLogTags.writeLockdownVpnConnecting(egressType);
            ShowNotification(R::string::vpn_lockdown_connecting, R::drawable::vpn_disconnected);

            egressProp->GetInterfaceName(&mAcceptedEgressIface);
            assert(0);
            // mVpn.startLegacyVpn(mProfile, KeyStore.getInstance(), egressProp);
            mVpn->StartLegacyVpn(mProfile, NULL, egressProp);
        }
        else {
            Slogger::E(TAG, "Invalid VPN profile; requires IP-based server and DNS");
            ShowNotification(R::string::vpn_lockdown_error, R::drawable::vpn_disconnected);
        }
    }
    else if ((vpnInfo->IsConnected(&isConnected), isConnected) && vpnConfig != NULL) {
        String iface;
        String sourceAddr;
        vpnConfig->GetInterfaze(&iface);
        vpnConfig->GetAddresses(&sourceAddr);

        if (((iface.IsNull() && mAcceptedIface.IsNull()) || iface.Equals(mAcceptedIface))
                && ((sourceAddr.IsNull() && mAcceptedSourceAddr.IsNull()) || sourceAddr.Equals(mAcceptedSourceAddr))) {
            return NOERROR;
        }

        Slogger::D(TAG, "VPN connected using iface=%s, sourceAddr=%s", iface.string(), sourceAddr.string());
        //EventLogTags.writeLockdownVpnConnected(egressType);
        ShowNotification(R::string::vpn_lockdown_connected, R::drawable::vpn_connected);

        // try {
        ClearSourceRulesLocked();

        ECode ec = mNetService->SetFirewallInterfaceRule(iface, TRUE);
        if (FAILED(ec)) return E_RUNTIME_EXCEPTION;
        ec = mNetService->SetFirewallEgressSourceRule(sourceAddr, TRUE);
        if (FAILED(ec)) return E_RUNTIME_EXCEPTION;

        mErrorCount = 0;
        mAcceptedIface = iface;
        mAcceptedSourceAddr = sourceAddr;
        // } catch (RemoteException e) {
        //     throw new RuntimeException("Problem setting firewall rules", e);
        // }

        AutoPtr<INetworkInfo> info = AugmentNetworkInfo(egressInfo);
        mConnService->SendConnectedBroadcast(info);
    }
    return NOERROR;
}

ECode LockdownVpnTracker::Init()
{
    AutoLock lock(mStateLock);
    return InitLocked();
}

ECode LockdownVpnTracker::InitLocked()
{
    Slogger::D(TAG, "initLocked()");

    mVpn->SetEnableNotifications(FALSE);

    AutoPtr<IIntentFilter> resetFilter;
    CIntentFilter::New(ACTION_LOCKDOWN_RESET, (IIntentFilter**)&resetFilter);
    AutoPtr<IIntent> intent;
    mContext->RegisterReceiver(mResetReceiver, resetFilter,
            Elastos::Droid::Manifest::permission::CONNECTIVITY_INTERNAL, NULL, (IIntent**)&intent);

    // try {
    // TODO: support non-standard port numbers
    String server;
    mProfile->GetServer(&server);
    ECode ec = mNetService->SetFirewallEgressDestRule(server, 500, TRUE);
    if (SUCCEEDED(ec)) {
        ec = mNetService->SetFirewallEgressDestRule(server, 4500, TRUE);
    }
    if (FAILED(ec)) return E_RUNTIME_EXCEPTION;
    // } catch (RemoteException e) {
    //     throw new RuntimeException("Problem setting firewall rules", e);
    // }

    {
        AutoLock lock(mStateLock);
        HandleStateChangedLocked();
    }
    return NOERROR;
}

ECode LockdownVpnTracker::Shutdown()
{
    AutoLock lock(mStateLock);
    return ShutdownLocked();
}

ECode LockdownVpnTracker::ShutdownLocked()
{
    Slogger::D(TAG, "shutdownLocked()");

    mAcceptedEgressIface = NULL;
    mErrorCount = 0;

    mVpn->StopLegacyVpn();
    //try {
    String server;
    mProfile->GetServer(&server);
    ECode ec = mNetService->SetFirewallEgressDestRule(server, 500, FALSE);
    if (SUCCEEDED(ec)) {
        ec = mNetService->SetFirewallEgressDestRule(server, 4500, FALSE);
    }
    if (FAILED(ec)) {
        return E_RUNTIME_EXCEPTION;
    }
    //} catch (RemoteException e) {
    //    throw new RuntimeException("Problem setting firewall rules", e);
    //}
    ClearSourceRulesLocked();
    HideNotification();

    mContext->UnregisterReceiver(mResetReceiver);
    mVpn->SetEnableNotifications(TRUE);
    return NOERROR;
}

ECode LockdownVpnTracker::Reset()
{
    AutoLock lock(mStateLock);
    // cycle tracker, reset error count, and trigger retry
    ShutdownLocked();
    InitLocked();
    HandleStateChangedLocked();
    return NOERROR;
}

ECode LockdownVpnTracker::ClearSourceRulesLocked()
{
    //try {
    ECode ec = NOERROR;
    if (!mAcceptedIface.IsNull()) {
        ec = mNetService->SetFirewallInterfaceRule(mAcceptedIface, FALSE);
        if (FAILED(ec)) return E_RUNTIME_EXCEPTION;
        mAcceptedIface = NULL;
    }
    if (!mAcceptedSourceAddr.IsNull()) {
        ec = mNetService->SetFirewallEgressSourceRule(mAcceptedSourceAddr, FALSE);
        if (FAILED(ec)) return E_RUNTIME_EXCEPTION;
        mAcceptedSourceAddr = NULL;
    }
    //} catch (RemoteException e) {
    //    throw new RuntimeException("Problem setting firewall rules", e);
    //}
    return NOERROR;
}

ECode LockdownVpnTracker::OnNetworkInfoChanged(
    /* [in] */ INetworkInfo* info)
{
    AutoLock lock(mStateLock);
    return HandleStateChangedLocked();
}

ECode LockdownVpnTracker::OnVpnStateChanged(
    /* [in] */ INetworkInfo* info)
{
    NetworkInfoDetailedState state;
    info->GetDetailedState(&state);
    if (state == NetworkInfoDetailedState_FAILED) {
        mErrorCount++;
    }

    {
        AutoLock lock(mStateLock);
        return HandleStateChangedLocked();
    }
}

AutoPtr<INetworkInfo> LockdownVpnTracker::AugmentNetworkInfo(
    /* [in] */ INetworkInfo* _info)
{
    AutoPtr<INetworkInfo> info = _info;
    Boolean isConnected;
    info->IsConnected(&isConnected);
    if (isConnected) {
        AutoPtr<INetworkInfo> vpnInfo;
        mVpn->GetNetworkInfo((INetworkInfo**)&vpnInfo);
        info = NULL;
        CNetworkInfo::New(_info, (INetworkInfo**)&info);
        NetworkInfoDetailedState state;
        vpnInfo->GetDetailedState(&state);
        String reason;
        vpnInfo->GetReason(&reason);
        info->SetDetailedState(state, reason, String(NULL));
    }
    return info;
}

void LockdownVpnTracker::ShowNotification(
    /* [in] */ Int32 titleRes,
    /* [in] */ Int32 iconRes)
{
    AutoPtr<INotificationBuilder> builder;
    CNotificationBuilder::New(mContext, (INotificationBuilder**)&builder);
    builder->SetWhen(0);
    builder->SetSmallIcon(iconRes);
    String value;
    mContext->GetString(titleRes, &value);
    AutoPtr<ICharSequence> csValue;
    CString::New(value, (ICharSequence**)&csValue);
    builder->SetContentTitle(csValue);
    mContext->GetString(R::string::vpn_lockdown_reset, &value);
    csValue = NULL;
    CString::New(value, (ICharSequence**)&csValue);
    builder->SetContentText(csValue);
    builder->SetContentIntent(mResetIntent);
    builder->SetPriority(INotification::PRIORITY_LOW);
    builder->SetOngoing(TRUE);

    AutoPtr<INotification> notification;
    builder->Build((INotification**)&notification);

    NotificationManager::From(mContext)->Notify(TAG, 0, notification);
}

void LockdownVpnTracker::HideNotification()
{
    NotificationManager::From(mContext)->Cancel(TAG, 0);
}

} // namespace Net
} // namespace Server
} // namepsace Droid
} // namespace Elastos
