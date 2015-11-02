
#include "CConnectivityService.h"
#include "CNetworkPolicyListener.h"
#include "CDataActivityObserver.h"
#include "am/CBatteryStatsService.h"
#include "connectivity/CTethering.h"
#include "connectivity/Vpn.h"
#include "elastos/droid/net/DummyDataStateTracker.h"
#include "elastos/droid/net/ethernet/EthernetDataTracker.h"
#include "elastos/droid/net/NetworkUtils.h"
#include "elastos/droid/net/wifi/WifiStateTracker.h"
#include "elastos/droid/os/Binder.h"
#include "elastos/droid/os/FileUtils.h"
#include "elastos/droid/os/SystemClock.h"
#include "elastos/droid/text/TextUtils.h"
#include "R.h"
#include "Manifest.h"
#include <elastos/utility/logging/Slogger.h>
#include <elastos/core/StringUtils.h>
#include <elastos/core/StringBuilder.h>
#include <cutils/properties.h>

using Elastos::Core::StringUtils;
using Elastos::Core::ISystem;
using Elastos::Core::CSystem;
using Elastos::Core::StringBuilder;
using Elastos::Core::CObjectContainer;
using Elastos::Net::IInetAddress;
using Elastos::Net::IInetAddressHelper;
using Elastos::Net::CInetAddressHelper;
using Elastos::Utility::Logging::Slogger;
using Elastos::Droid::R;
using Elastos::Droid::Content::IContentResolver;
using Elastos::Droid::Content::IIntent;
using Elastos::Droid::Content::CIntent;
using Elastos::Droid::Content::IIntentFilter;
using Elastos::Droid::Content::CIntentFilter;
using Elastos::Droid::Content::Res::IResources;
using Elastos::Droid::Net::IConnectivityManager;
using Elastos::Droid::Net::ICompareResult;
using Elastos::Droid::Net::CCompareResult;
using Elastos::Droid::Net::ILinkAddress;
using Elastos::Droid::Net::ILinkCapabilities;
using Elastos::Droid::Net::INetworkPolicyManager;
using Elastos::Droid::Net::IRouteInfoHelper;
using Elastos::Droid::Net::CRouteInfoHelper;
using Elastos::Droid::Net::IConnectivityManagerHelper;
using Elastos::Droid::Net::CConnectivityManagerHelper;
using Elastos::Droid::Net::CNetworkConfig;
using Elastos::Droid::Net::CNetworkInfo;
using Elastos::Droid::Net::CNetworkState;
using Elastos::Droid::Net::CProxyProperties;
using Elastos::Droid::Net::DummyDataStateTracker;
using Elastos::Droid::Net::NetworkInfoState_CONNECTED;
using Elastos::Droid::Net::NetworkInfoState_DISCONNECTED;
using Elastos::Droid::Net::NetworkInfoState_SUSPENDED;
using Elastos::Droid::Net::NetworkInfoState_CONNECTING;
using Elastos::Droid::Net::NetworkInfoDetailedState_BLOCKED;
using Elastos::Droid::Net::NetworkInfoDetailedState_FAILED;
using Elastos::Droid::Net::NetworkInfoDetailedState_CAPTIVE_PORTAL_CHECK;
using Elastos::Droid::Net::NetworkInfoDetailedState_CONNECTED;
using Elastos::Droid::Net::NetworkUtils;
using Elastos::Droid::Net::Ethernet::EthernetDataTracker;
using Elastos::Droid::Wifi::WifiStateTracker;
using Elastos::Droid::Os::Binder;
using Elastos::Droid::Os::FileUtils;
using Elastos::Droid::Os::SystemClock;
using Elastos::Droid::Provider::CSettingsGlobal;
using Elastos::Droid::Provider::ISettingsGlobal;
using Elastos::Droid::Provider::CSettingsSecure;
using Elastos::Droid::Provider::ISettingsSecure;
using Elastos::Droid::Text::TextUtils;
using Elastos::Droid::Server::Am::CBatteryStatsService;
using Elastos::Droid::Server::Connectivity::CTethering;
using Elastos::Droid::Server::Connectivity::Vpn;

namespace Elastos {
namespace Droid {
namespace Server {

const String CConnectivityService::TAG("ConnectivityService");

const Boolean CConnectivityService::DBG = FALSE;
const Boolean CConnectivityService::VDBG = FALSE;
const Boolean CConnectivityService::LOGD_RULES = FALSE;

const Int32 CConnectivityService::RESTORE_DEFAULT_NETWORK_DELAY = 1 * 60 * 1000;
const String CConnectivityService::NETWORK_RESTORE_DELAY_PROP_NAME("android.telephony.apn-restore");
const Int32 CConnectivityService::MAX_HOSTROUTE_CYCLE_COUNT = 10;

const Int32 CConnectivityService::ENABLED  = 1;
const Int32 CConnectivityService::DISABLED = 0;

const Boolean CConnectivityService::ADD = TRUE;
const Boolean CConnectivityService::REMOVE = FALSE;

const Boolean CConnectivityService::TO_DEFAULT_TABLE = TRUE;
const Boolean CConnectivityService::TO_SECONDARY_TABLE = FALSE;

const Int32 CConnectivityService::INET_CONDITION_LOG_MAX_SIZE = 15;

const Int32 CConnectivityService::EVENT_RESTORE_DEFAULT_NETWORK = 1;
const Int32 CConnectivityService::EVENT_CHANGE_MOBILE_DATA_ENABLED = 2;
const Int32 CConnectivityService::EVENT_SET_NETWORK_PREFERENCE = 3;
const Int32 CConnectivityService::EVENT_INET_CONDITION_CHANGE = 4;
const Int32 CConnectivityService::EVENT_INET_CONDITION_HOLD_END = 5;
const Int32 CConnectivityService::EVENT_SET_MOBILE_DATA = 7;
const Int32 CConnectivityService::EVENT_CLEAR_NET_TRANSITION_WAKELOCK = 8;
const Int32 CConnectivityService::EVENT_APPLY_GLOBAL_HTTP_PROXY = 9;
const Int32 CConnectivityService::EVENT_SET_DEPENDENCY_MET = 10;
const Int32 CConnectivityService::EVENT_RESTORE_DNS = 11;
const Int32 CConnectivityService::EVENT_SEND_STICKY_BROADCAST_INTENT = 12;
const Int32 CConnectivityService::EVENT_SET_POLICY_DATA_ENABLE = 13;
const Int32 CConnectivityService::EVENT_VPN_STATE_CHANGED = 14;

//==============================================================================
//              CConnectivityService::RadioAttributes
//==============================================================================
CConnectivityService::RadioAttributes::RadioAttributes(
    /* [in] */ const String& init)
{
    AutoPtr< ArrayOf<String> > fragments;
    StringUtils::Split(init, String(","), (ArrayOf<String>**)&fragments);
    mType = StringUtils::ParseInt32((*fragments)[0]);
    mSimultaneity = StringUtils::ParseInt32((*fragments)[1]);
}


//==============================================================================
//              CConnectivityService::DefaultNetworkFactory
//==============================================================================
CConnectivityService::DefaultNetworkFactory::DefaultNetworkFactory(
    /* [in] */ IContext* context,
    /* [in] */ IHandler* trackerHandler,
    /* [in] */ CConnectivityService* owner)
    : mContext(context)
    , mTrackerHandler(trackerHandler)
    , mOwner(owner)
{}

PInterface CConnectivityService::DefaultNetworkFactory::Probe(
    /* [in] */ REIID riid)
{
    assert(0);
    return NULL;
}

UInt32 CConnectivityService::DefaultNetworkFactory::AddRef()
{
    return ElRefBase::AddRef();
}

UInt32 CConnectivityService::DefaultNetworkFactory::Release()
{
    return ElRefBase::Release();
}

ECode CConnectivityService::DefaultNetworkFactory::GetInterfaceID(
    /* [in] */ IInterface* object,
    /* [out] */ InterfaceID* iid)
{
    assert(0);
    return E_NOT_IMPLEMENTED;
}

ECode CConnectivityService::DefaultNetworkFactory::CreateTracker(
    /* [in] */ Int32 targetNetworkType,
    /* [in] */ INetworkConfig* config,
    /* [out] */ INetworkStateTracker** tracker)
{
    VALIDATE_NOT_NULL(tracker);
    *tracker = NULL;

    Int32 radio;
    config->GetRadio(&radio);
    switch (radio) {
        case IConnectivityManager::TYPE_WIFI: {
            String name;
            config->GetName(&name);
            *tracker = new WifiStateTracker(targetNetworkType, name);
            REFCOUNT_ADD(*tracker);
            return NOERROR;
        }
        case IConnectivityManager::TYPE_MOBILE: {
            // return new MobileDataStateTracker(targetNetworkType, config.name);
            PFL_EX("TODO")
            return E_NOT_IMPLEMENTED;
        }
        case IConnectivityManager::TYPE_DUMMY: {
            String name;
            config->GetName(&name);
            *tracker = new DummyDataStateTracker(targetNetworkType, name);
            REFCOUNT_ADD(*tracker);
            return NOERROR;
        }
        case IConnectivityManager::TYPE_BLUETOOTH: {
            // return BluetoothTetheringDataTracker.getInstance();
            assert(0);
            return E_NOT_IMPLEMENTED;
        }
        case IConnectivityManager::TYPE_WIMAX: {
            AutoPtr<INetworkStateTracker> t = MakeWimaxStateTracker(mContext, mTrackerHandler);
            *tracker = t;
            REFCOUNT_ADD(*tracker);
            return NOERROR;
        }
        case IConnectivityManager::TYPE_ETHERNET: {
            AutoPtr<INetworkStateTracker> t = EthernetDataTracker::GetInstance();
            *tracker = t;
            REFCOUNT_ADD(*tracker);
            return NOERROR;
        }
        default:
            // throw new IllegalArgumentException(
            //         "Trying to create a NetworkStateTracker for an unknown radio type: "
            //         + config.radio);
            return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
}


//==============================================================================
//              CConnectivityService::FeatureUser
//==============================================================================
/**
 * Used to notice when the calling process dies so we can self-expire
 *
 * Also used to know if the process has cleaned up after itself when
 * our auto-expire timer goes off.  The timer has a link to an object.
 *
 */
CConnectivityService::FeatureUser::FeatureUser(
    /* [in] */ Int32 type,
    /* [in] */ const String& feature,
    /* [in] */ IBinder* binder,
    /* [in] */ CConnectivityService* owner)
    : mNetworkType(type)
    , mFeature(feature)
    , mBinder(binder)
    , mOwner(owner)
{
    mPid = Binder::GetCallingPid();
    mUid = Binder::GetCallingUid();
    AutoPtr<ISystem> system;
    Elastos::Core::CSystem::AcquireSingleton((ISystem**)&system);
    system->GetCurrentTimeMillis(&mCreateTime);

//    try {
    AutoPtr< ::IProxy > proxy = (::IProxy*)mBinder->Probe(EIID_IProxy);
    if (proxy != NULL) {
        proxy->LinkToDeath(this, 0);
    }
//    } catch (RemoteException e) {
//        binderDied();
//    }
}

CAR_INTERFACE_IMPL(CConnectivityService::FeatureUser, IProxyDeathRecipient)

void CConnectivityService::FeatureUser::UnlinkDeathRecipient()
{
    AutoPtr< ::IProxy > proxy = (::IProxy*)mBinder->Probe(EIID_IProxy);
    if (proxy != NULL) {
        Boolean result;
        proxy->UnlinkToDeath(this, 0, &result);
    }
}

ECode CConnectivityService::FeatureUser::ProxyDied()
{
    AutoPtr<ISystem> system;
    Elastos::Core::CSystem::AcquireSingleton((ISystem**)&system);
    Int64 millis;
    system->GetCurrentTimeMillis(&millis);
    Slogger::D(TAG, "ConnectivityService FeatureUser binderDied(%d, %s, %p), created %lld mSec ago",
        mNetworkType, mFeature.string(), mBinder.Get(), millis - mCreateTime);
    mOwner->StopUsingNetworkFeature(this, FALSE);
    return NOERROR;
}

ECode CConnectivityService::FeatureUser::Expire()
{
    if (VDBG) {
        AutoPtr<ISystem> system;
        Elastos::Core::CSystem::AcquireSingleton((ISystem**)&system);
        Int64 millis;
        system->GetCurrentTimeMillis(&millis);
        Slogger::V(TAG, "ConnectivityService FeatureUser expire(%d, %s, %p), created %lld mSec ago",
            mNetworkType, mFeature.string(), mBinder.Get(), millis - mCreateTime);
    }
    mOwner->StopUsingNetworkFeature(this, FALSE);
    return NOERROR;
}

Boolean CConnectivityService::FeatureUser::IsSameUser(
    /* [in] */ FeatureUser* u)
{
    if (u == NULL) return FALSE;

    return IsSameUser(u->mPid, u->mUid, u->mNetworkType, u->mFeature);
}

Boolean CConnectivityService::FeatureUser::IsSameUser(
    /* [in] */ Int32 pid,
    /* [in] */ Int32 uid,
    /* [in] */ Int32 networkType,
    /* [in] */ const String& feature)
{
    if ((mPid == pid) && (mUid == uid) && (mNetworkType == networkType) &&
        TextUtils::Equals(mFeature, feature)) {
        return TRUE;
    }
    return FALSE;
}

String CConnectivityService::FeatureUser::ToString()
{
    AutoPtr<ISystem> system;
    Elastos::Core::CSystem::AcquireSingleton((ISystem**)&system);
    Int64 millis;
    system->GetCurrentTimeMillis(&millis);

    StringBuilder sb("FeatureUser(");
    sb += mNetworkType;
    sb += ",";
    sb += mFeature;
    sb += ",";
    sb += mPid;
    sb += ",";
    sb += mUid;
    sb += "), created ";
    sb += millis - mCreateTime;
    sb += " mSec ago";
    return sb.ToString();
}


//==============================================================================
//              CConnectivityService::InternalBroadcastReciver
//==============================================================================
ECode CConnectivityService::InternalBroadcastReciver::OnReceive(
    /* [in] */ IContext* context,
    /* [in] */ IIntent* intent)
{
    // Try creating lockdown tracker, since user present usually means
    // unlocked keystore.
    Boolean update;
    if ((mOwner->UpdateLockdownVpn(&update), update)) {
        mOwner->mContext->UnregisterReceiver(this);
    }
    return NOERROR;
}

//==============================================================================
//              CConnectivityService::InternalHandler
//==============================================================================

ECode CConnectivityService::InternalHandler::HandleMessage(
    /* [in] */ IMessage* msg)
{
    Int32 what, arg1, arg2;
    msg->GetWhat(&what);
    msg->GetArg1(&arg1);
    msg->GetArg2(&arg2);
    switch (what) {
        case CConnectivityService::EVENT_CLEAR_NET_TRANSITION_WAKELOCK:
            mOwner->HandleEventClearNetTransitionWakelock(arg1);
            break;

        case CConnectivityService::EVENT_RESTORE_DEFAULT_NETWORK: {
            AutoPtr<IInterface> obj;
            msg->GetObj((IInterface**)&obj);
            FeatureUser * u = (FeatureUser*)obj.Get();
            u->Expire();
            break;
        }

        case CConnectivityService::EVENT_INET_CONDITION_CHANGE: {
            mOwner->HandleInetConditionChange(arg1, arg2);
            break;
        }
        case CConnectivityService::EVENT_INET_CONDITION_HOLD_END: {
            mOwner->HandleInetConditionHoldEnd(arg1, arg2);
            break;
        }
        case CConnectivityService::EVENT_SET_NETWORK_PREFERENCE: {
            mOwner->HandleSetNetworkPreference(arg1);
            break;
        }
        case CConnectivityService::EVENT_SET_MOBILE_DATA: {
            Boolean enabled = (arg1 == ENABLED);
            mOwner->HandleSetMobileData(enabled);
            break;
        }
        case CConnectivityService::EVENT_APPLY_GLOBAL_HTTP_PROXY: {
            mOwner->HandleDeprecatedGlobalHttpProxy();
            break;
        }
        case CConnectivityService::EVENT_SET_DEPENDENCY_MET: {
            Boolean met = (arg1 == ENABLED);
            mOwner->HandleSetDependencyMet(arg2, met);
            break;
        }
        case CConnectivityService::EVENT_RESTORE_DNS: {
            if (mOwner->mActiveDefaultNetwork != -1) {
                mOwner->HandleDnsConfigurationChange(mOwner->mActiveDefaultNetwork);
            }
            break;
        }
        case CConnectivityService::EVENT_SEND_STICKY_BROADCAST_INTENT: {
            AutoPtr<IInterface> obj;
            msg->GetObj((IInterface**)&obj);
            IIntent* intent = IIntent::Probe(obj);
            mOwner->SendStickyBroadcast(intent);
            break;
        }
        case CConnectivityService::EVENT_SET_POLICY_DATA_ENABLE: {
            Boolean enabled = arg2 == ENABLED;
            mOwner->HandleSetPolicyDataEnable(arg1, enabled);
            break;
        }
        case CConnectivityService::EVENT_VPN_STATE_CHANGED: {
            if (mOwner->mLockdownTracker != NULL) {
                AutoPtr<IInterface> obj;
                msg->GetObj((IInterface**)&obj);
                INetworkInfo* ni = INetworkInfo::Probe(obj);
                mOwner->mLockdownTracker->OnVpnStateChanged(ni);
            }
            break;
        }
    }
    return NOERROR;
}

//==============================================================================
//              CConnectivityService::NetworkStateTrackerHandler
//==============================================================================

ECode CConnectivityService::NetworkStateTrackerHandler::HandleMessage(
    /* [in] */ IMessage* msg)
{
    AutoPtr<IInterface> obj;
    msg->GetObj((IInterface**)&obj);
    AutoPtr<INetworkInfo> info = INetworkInfo::Probe(obj);
    Int32 what;
    msg->GetWhat(&what);
    switch (what) {
        case INetworkStateTracker::EVENT_STATE_CHANGED: {
            Int32 type, subtype;
            info->GetType(&type);
            info->GetSubtype(&subtype);
            NetworkInfoState state;
            info->GetState(&state);
            String typeName;
            info->GetTypeName(&typeName);
            NetworkInfoDetailedState detailedState;
            info->GetDetailedState(&detailedState);

            if (VDBG || (state == NetworkInfoState_CONNECTED) ||
                    (state == NetworkInfoState_DISCONNECTED)) {
                Slogger::V(TAG, "ConnectivityChange for %s: state %d, detailedState %d",
                    typeName.string(), state, detailedState);
            }

            // EventLogTags.writeConnectivityStateChanged(
            //                 info.getType(), info.getSubtype(), info.getDetailedState().ordinal());

            if (detailedState == NetworkInfoDetailedState_FAILED) {
                mOwner->HandleConnectionFailure(info);
            }
            else if (detailedState == NetworkInfoDetailedState_CAPTIVE_PORTAL_CHECK) {
                mOwner->HandleCaptivePortalTrackerCheck(info);
            }
            else if (state == NetworkInfoState_DISCONNECTED) {
                mOwner->HandleDisconnect(info);
            }
            else if (state == NetworkInfoState_SUSPENDED) {
                // TODO: need to think this over.
                // the logic here is, handle SUSPENDED the same as
                // DISCONNECTED. The only difference being we are
                // broadcasting an intent with NetworkInfo that's
                // suspended. This allows the applications an
                // opportunity to handle DISCONNECTED and SUSPENDED
                // differently, or not.
                mOwner->HandleDisconnect(info);
            }
            else if (state == NetworkInfoState_CONNECTED) {
                mOwner->HandleConnect(info);
            }

            if (mOwner->mLockdownTracker != NULL) {
                mOwner->mLockdownTracker->OnNetworkInfoChanged(info);
            }
            break;
        }
        case INetworkStateTracker::EVENT_CONFIGURATION_CHANGED: {
            // TODO: Temporary allowing network configuration
            //       change not resetting sockets.
            //       @see bug/4455071
            Int32 type;
            info->GetType(&type);
            mOwner->HandleConnectivityChange(type, FALSE);
            break;
        }
        case INetworkStateTracker::EVENT_NETWORK_SUBTYPE_CHANGED: {
            Int32 type;
            info->GetType(&type);
            mOwner->UpdateNetworkSettings((*mOwner->mNetTrackers)[type]);
            break;
        }
    }
    return NOERROR;
}


//==============================================================================
//              CConnectivityService::SettingsObserver
//==============================================================================
CConnectivityService::SettingsObserver::SettingsObserver(
    /* [in] */ IHandler* handler,
    /* [in] */ Int32 what,
    /* [in] */ CConnectivityService* owner)
    : ContentObserver(handler)
    , mHandler(handler)
    , mWhat(what)
    , mOwner(owner)
{}

void CConnectivityService::SettingsObserver::Observe(
    /* [in] */ IContext* context)
{
    AutoPtr<IContentResolver> resolver;
    context->GetContentResolver((IContentResolver**)&resolver);
    AutoPtr<IUri> uri;
    AutoPtr<ISettingsGlobal> settingsGlobal;
    CSettingsGlobal::AcquireSingleton((ISettingsGlobal**)&settingsGlobal);
    settingsGlobal->GetUriFor(ISettingsGlobal::HTTP_PROXY, (IUri**)&uri);
    resolver->RegisterContentObserver(uri, FALSE, (IContentObserver*)this);
}

ECode CConnectivityService::SettingsObserver::OnChange(
    /* [in] */ Boolean selfChange)
{
    AutoPtr<IMessage> msg;
    mHandler->ObtainMessage(mWhat, (IMessage**)&msg);
    return msg->SendToTarget();
}


//==============================================================================
//              CConnectivityService::VpnCallback
//==============================================================================
/**
 * Callback for VPN subsystem. Currently VPN is not adapted to the service
 * through NetworkStateTracker since it works differently. For example, it
 * needs to override DNS servers but never takes the default routes. It
 * relies on another data network, and it could keep existing connections
 * alive after reconnecting, switching between networks, or even resuming
 * from deep sleep. Calls from applications should be done synchronously
 * to avoid race conditions. As these are all hidden APIs, refactoring can
 * be done whenever a better abstraction is developed.
 */
ECode CConnectivityService::VpnCallback::OnStateChanged(
    /* [in] */ INetworkInfo* info)
{
    AutoPtr<IMessage> msg;
    mOwner->mHandler->ObtainMessage(CConnectivityService::EVENT_VPN_STATE_CHANGED, info, (IMessage**)&msg);
    return msg->SendToTarget();
}

ECode CConnectivityService::VpnCallback::Override(
    /* [in] */ IObjectContainer* dnsServers,
    /* [in] */ IObjectContainer* searchDomains)
{
    if (dnsServers == NULL) {
        Restore();
        return NOERROR;
    }

    // Convert DNS servers into addresses.
    List< AutoPtr<IInetAddress> > addresses;
    AutoPtr<IInetAddressHelper> helper;
    CInetAddressHelper::AcquireSingleton((IInetAddressHelper**)&helper);
    AutoPtr<IObjectEnumerator> dnsServersIt;
    dnsServers->GetObjectEnumerator((IObjectEnumerator**)&dnsServersIt);
    Boolean hasNext;
    while (dnsServersIt->MoveNext(&hasNext), hasNext) {
        AutoPtr<ICharSequence> dnsServer;
        dnsServersIt->Current((IInterface**)&dnsServer);
        String address;
        dnsServer->ToString(&address);
        // Double check the addresses and remove invalid ones.
        // try {
        AutoPtr<IInetAddress> addr;
        helper->ParseNumericAddress(address, (IInetAddress**)&addr);
        addresses.PushBack(addr);
        // } catch (Exception e) {
        //     // ignore
        // }
    }
    if (addresses.IsEmpty()) {
        Restore();
        return NOERROR;
    }

    // Concatenate search domains into a string.
    StringBuilder buffer;
    if (searchDomains != NULL) {
        AutoPtr<IObjectEnumerator> searchDomainsIt;
        searchDomains->GetObjectEnumerator((IObjectEnumerator**)&searchDomainsIt);
        Boolean hasNext;
        while (searchDomainsIt->MoveNext(&hasNext), hasNext) {
            AutoPtr<ICharSequence> csDomain;
            searchDomainsIt->Current((IInterface**)&csDomain);
            String domain;
            csDomain->ToString(&domain);
            buffer.AppendString(domain);
            buffer.AppendChar(' ');
        }
    }
    String domains = buffer.ToString();
    domains.Trim();

    // Apply DNS changes.
    Boolean changed = FALSE;
    {
        AutoLock lock(mOwner->mDnsLock);
        changed = mOwner->UpdateDns(String("VPN"), String("VPN"), addresses, domains);
        mOwner->mDnsOverridden = TRUE;
    }
    if (changed) {
        mOwner->BumpDns();
    }

    // Temporarily disable the default proxy.
    {
        AutoLock lock(mOwner->mDefaultProxyLock);
        mOwner->mDefaultProxyDisabled = TRUE;
        if (mOwner->mDefaultProxy != NULL) {
            mOwner->SendProxyBroadcast(NULL);
        }
    }

    // TODO: support proxy per network.
    return NOERROR;
}

ECode CConnectivityService::VpnCallback::Restore()
{
    {
        AutoLock lock(mOwner->mDnsLock);
        if (mOwner->mDnsOverridden) {
            mOwner->mDnsOverridden = FALSE;
            Boolean result;
            mOwner->mHandler->SendEmptyMessage(CConnectivityService::EVENT_RESTORE_DNS, &result);
        }
    }

    {
        AutoLock lock(mOwner->mDefaultProxyLock);
        mOwner->mDefaultProxyDisabled = FALSE;
        if (mOwner->mDefaultProxy != NULL) {
            mOwner->SendProxyBroadcast(mOwner->mDefaultProxy);
        }
    }
    return NOERROR;
}


//==============================================================================
//              CConnectivityService
//==============================================================================

CConnectivityService::CConnectivityService()
    : mTetheringConfigValid(FALSE)
    , mLockdownEnabled(FALSE)
    , mNetworkPreference(0)
    , mActiveDefaultNetwork(-1)
    , mDefaultInetCondition(0)
    , mDefaultInetConditionPublished(0)
    , mInetConditionChangeInFlight(FALSE)
    , mDefaultConnectionSequence(0)
    , mNumDnsEntries(0)
    , mDnsOverridden(FALSE)
    , mTestMode(FALSE)
    , mSystemReady(FALSE)
    , mNetTransitionWakeLockCausedBy("")
    , mNetTransitionWakeLockSerialNumber(0)
    , mNetTransitionWakeLockTimeout(0)
    , mDefaultProxyDisabled(FALSE)
    , mGlobalProxy(NULL)
    , mNetworksDefined(0)
{
    mVpnCallback = new VpnCallback(this);

    CNetworkPolicyListener::New((Handle32)this, (INetworkPolicyListener**)&mPolicyListener);

    mUserPresentReceiver = new InternalBroadcastReciver(this);
    CDataActivityObserver::New((Handle32)this, (INetworkManagementEventObserver**)&mDataActivityObserver);
}

CConnectivityService::~CConnectivityService()
{
    mNetRequestersPids = NULL;
    mInetLog = NULL;
}

ECode CConnectivityService::constructor(
    /* [in] */ IContext* context,
    /* [in] */ INetworkManagementService* netd,
    /* [in] */ INetworkStatsService* statsService,
    /* [in] */ IINetworkPolicyManager* policyManager)
{
    // Currently, omitting a NetworkFactory will create one internally
    // TODO: create here when we have cleaner WiMAX support
    return constructor(context, netd, statsService, policyManager, 0);
}

ECode CConnectivityService::constructor(
    /* [in] */ IContext* context,
    /* [in] */ INetworkManagementService* netManager,
    /* [in] */ INetworkStatsService* statsService,
    /* [in] */ IINetworkPolicyManager* policyManager,
    /* [in] */ Handle32 netFac)
{
    if (DBG) Slogger::D(TAG, "ConnectivityService starting up!");

    AutoPtr<IHandlerThread> handlerThread;
    CHandlerThread::New(String("ConnectivityServiceThread"), (IHandlerThread**)&handlerThread);
    handlerThread->Start();
    AutoPtr<ILooper> looper;
    handlerThread->GetLooper((ILooper**)&looper);
    mHandler = new InternalHandler(looper, this);
    mTrackerHandler = new NetworkStateTrackerHandler(looper, this);

    AutoPtr<INetworkFactory> netFactory = (INetworkFactory*)netFac;
    if (netFactory == NULL) {
        netFactory = new DefaultNetworkFactory(context, mTrackerHandler, this);
    }

    // setup our unique device name
    AutoPtr<IContentResolver> resolver;
    context->GetContentResolver((IContentResolver**)&resolver);
    AutoPtr<ISystemProperties> sysProp;
    CSystemProperties::AcquireSingleton((ISystemProperties**)&sysProp);
    String prop;
    sysProp->Get(String("net.hostname"), &prop);
    if (TextUtils::IsEmpty(prop)) {
        String id;
        // TODO:
        // AutoPtr<ISettingsSecure> settingsSecure;
        // CSettingsSecure::AcquireSingleton((ISettingsSecure**)&settingsSecure);
        // settingsSecure->GetString(resolver, ISettingsSystem::ANDROID_ID, &id);
        if (!id.IsNullOrEmpty()) {
            String name("android-");
            name += id;
            sysProp->Set(String("net.hostname"), name);
        }
    }

    // read our default dns server ip
    String dns;
    // AutoPtr<ISettingsGlobal> settingsGlobal;
    // CSettingsGlobal::AcquireSingleton((ISettingsGlobal**)&settingsGlobal);
    // settingsGlobal->GetString(resolver, ISettingsGlobal::DEFAULT_DNS_SERVER, &dns);
    if (dns.IsNullOrEmpty()) {
        AutoPtr<IResources> resources;
        context->GetResources((IResources**)&resources);
        resources->GetString(R::string::config_default_dns_server, &dns);
    }
    ECode ec = NetworkUtils::NumericToInetAddress(dns, (IInetAddress**)&mDefaultDns);
    if (FAILED(ec)) {
        Slogger::E(TAG, "Error setting defaultDns using %s", dns.string());
    }

    mContext = context;
    assert(mContext != NULL);
    mNetd = netManager;
    assert(mNetd != NULL);
    mPolicyManager = policyManager;
    assert(mPolicyManager != NULL);
    PFL_EX("TODO: mKeyStore is not ready")
    // mKeyStore = KeyStore::GetInstance();

    ec = mPolicyManager->RegisterListener(mPolicyListener);
    if (FAILED(ec)) {
        // ouch, no rules updates means some processes may never get network
        Slogger::E(TAG, "unable to register INetworkPolicyListener");
    }

    AutoPtr<IInterface> powerManagerObj;
    context->GetSystemService(IContext::POWER_SERVICE, (IInterface**)&powerManagerObj);
    AutoPtr<IPowerManager> powerManager = IPowerManager::Probe(powerManagerObj);
    powerManager->NewWakeLock(IPowerManager::PARTIAL_WAKE_LOCK, String(TAG), (IPowerManagerWakeLock**)&mNetTransitionWakeLock);
    AutoPtr<IResources> resources;
    mContext->GetResources((IResources**)&resources);
    resources->GetInteger(R::integer::config_networkTransitionTimeout, &mNetTransitionWakeLockTimeout);

    mNetTrackers = ArrayOf<INetworkStateTracker*>::Alloc(IConnectivityManager::MAX_NETWORK_TYPE + 1);
    mCurrentLinkProperties = ArrayOf<ILinkProperties*>::Alloc(IConnectivityManager::MAX_NETWORK_TYPE + 1);

    mNetworkPreference = GetPersistedNetworkPreference();

    mRadioAttributes = ArrayOf<RadioAttributes*>::Alloc(IConnectivityManager::MAX_RADIO_TYPE + 1);
    mNetConfigs = ArrayOf<INetworkConfig*>::Alloc(IConnectivityManager::MAX_NETWORK_TYPE + 1);

    // Load device network attributes from resources
    AutoPtr< ArrayOf<String> > raStrings;
    resources->GetStringArray(R::array::radioAttributes, (ArrayOf<String>**)&raStrings);
    for (Int32 i = 0; i < raStrings->GetLength(); i++) {
        String raString = (*raStrings)[i];
        AutoPtr<RadioAttributes> r = new RadioAttributes(raString);
        if (r->mType > IConnectivityManager::MAX_RADIO_TYPE) {
            Slogger::E(TAG, "Error in radioAttributes - ignoring attempt to define type %d", r->mType);
            continue;
        }
        if ((*mRadioAttributes)[r->mType] != NULL) {
            Slogger::E(TAG, "Error in radioAttributes - ignoring attempt to redefine type %d", r->mType);
            continue;
        }
        mRadioAttributes->Set(r->mType, r);
    }

    AutoPtr<ArrayOf<String> > naStrings;
    resources->GetStringArray(R::array::networkAttributes, (ArrayOf<String>**)&naStrings);
    for (Int32 i = 0; i < naStrings->GetLength(); i++) {
        String naString = (*naStrings)[i];
        AutoPtr<INetworkConfig> n;
        CNetworkConfig::New(naString, (INetworkConfig**)&n);
        Int32 nType;
        n->GetType(&nType);
        if (nType > IConnectivityManager::MAX_NETWORK_TYPE) {
            Slogger::E(TAG, "Error in networkAttributes - ignoring attempt to define type %d", nType);
            continue;
        }
        if ((*mNetConfigs)[nType] != NULL) {
            Slogger::E(TAG, "Error in networkAttributes - ignoring attempt to redefine type %d", nType);
            continue;
        }
        Int32 nRadio;
        n->GetRadio(&nRadio);
        if ((*mRadioAttributes)[nRadio] == NULL && (nType != IConnectivityManager::TYPE_ETHERNET)) {
            Slogger::E(TAG, "Error in networkAttributes - ignoring attempt to use undefined radio %d in network type %d",
                    nRadio, nType);
            continue;
        }
        mNetConfigs->Set(nType, n);
        mNetworksDefined++;
    }

    AutoPtr< ArrayOf<Int32> > protectedNetworks;
    resources->GetIntArray(R::array::config_protectedNetworks, (ArrayOf<Int32>**)&protectedNetworks);
    for (Int32 i = 0; i < protectedNetworks->GetLength(); i++) {
        Int32 p = (*protectedNetworks)[i];
        if ((*mNetConfigs)[p] != NULL &&
                (Find(mProtectedNetworks.Begin(), mProtectedNetworks.End(), p) == mProtectedNetworks.End())) {
                mProtectedNetworks.PushBack(p);
        }
        else {
            if (DBG) Slogger::E(TAG, "Ignoring protectedNetwork %d", p);
        }
    }

    // high priority first
    mPriorityList = ArrayOf<Int32>::Alloc(mNetworksDefined);
    {
        Int32 insertionPoint = mNetworksDefined - 1;
        Int32 currentLowest = 0;
        Int32 nextLowest = 0;
        while (insertionPoint > -1) {
            for (Int32 i = 0; i < mNetConfigs->GetLength(); i++) {
                AutoPtr<INetworkConfig> na = (*mNetConfigs)[i];
                if (na == NULL) continue;
                Int32 priority;
                na->GetPriority(&priority);
                if (priority < currentLowest) continue;
                if (priority > currentLowest) {
                    if (priority < nextLowest || nextLowest == 0) {
                        nextLowest = priority;
                    }
                    continue;
                }
                Int32 type;
                na->GetType(&type);
                (*mPriorityList)[insertionPoint--] = type;
            }
            currentLowest = nextLowest;
            nextLowest = 0;
        }
    }

    mNetRequestersPids = ArrayOf< AutoPtr<List<Int32> > >::Alloc(IConnectivityManager::MAX_NETWORK_TYPE + 1);
    for(Int32 i = 0; i < mPriorityList->GetLength(); i++) {
        (*mNetRequestersPids)[i] = new List<Int32>();
    }

    mNumDnsEntries = 0;

    String modeStr, typeStr;
    mTestMode = (sysProp->Get(String("cm.test.mode"), &modeStr), String("true").Equals(modeStr))
            && (sysProp->Get(String("ro.build.type"), &typeStr), String("eng").Equals(typeStr));

    AutoPtr<IConnectivityManagerHelper> helper;
    CConnectivityManagerHelper::AcquireSingleton((IConnectivityManagerHelper**)&helper);

    // Create and start trackers for hard-coded networks
    for (Int32 i = 0; i < mPriorityList->GetLength(); i++) {
        Int32 targetNetworkType = (*mPriorityList)[i];
        AutoPtr<INetworkConfig> config = (*mNetConfigs)[targetNetworkType];
        AutoPtr<INetworkStateTracker> tracker;
//        try {
        if (FAILED(netFactory->CreateTracker(targetNetworkType, config,
                (INetworkStateTracker**)&tracker))) {
            String networkTypeName;
            helper->GetNetworkTypeName(targetNetworkType, &networkTypeName);
            Slogger::E(TAG, "Problem creating %s tracker", networkTypeName.string());
            continue;
        }
        mNetTrackers->Set(targetNetworkType, tracker);
//        } catch (IllegalArgumentException e) {
//            Slog.e(TAG, "Problem creating " + getNetworkTypeName(targetNetworkType)
//                    + " tracker: " + e);
//            continue;
//        }

        tracker->StartMonitoring(context, mTrackerHandler);
        Boolean result;
        if (config->IsDefault(&result), result) {
            tracker->Reconnect(&result);
        }
    }

    AutoPtr<ILooper> looper2;
    mHandler->GetLooper((ILooper**)&looper2);
    CTethering::NewByFriend(mContext, mNetd, statsService, this, looper2, (CTethering**)&mTethering);
    AutoPtr< ArrayOf<String> > usbRegexs;
    AutoPtr< ArrayOf<String> > wifiRegexs;
    AutoPtr< ArrayOf<String> > btRegexs;
    mTethering->GetTetherableUsbRegexs((ArrayOf<String>**)&usbRegexs);
    mTethering->GetTetherableWifiRegexs((ArrayOf<String>**)&wifiRegexs);
    mTethering->GetTetherableBluetoothRegexs((ArrayOf<String>**)&btRegexs);
    AutoPtr< ArrayOf<Int32> > ifaceTypes = mTethering->GetUpstreamIfaceTypes();
    mTetheringConfigValid = ((usbRegexs->GetLength() != 0 ||
                              wifiRegexs->GetLength() != 0 ||
                              btRegexs->GetLength() != 0) &&
                                ifaceTypes->GetLength() != 0);

    mVpn = new Vpn(mContext, mVpnCallback, mNetd);
    mVpn->StartMonitoring(mContext, mTrackerHandler);

    //try {
    mNetd->RegisterObserver(mTethering);
    mNetd->RegisterObserver(mDataActivityObserver);
    //} catch (RemoteException e) {
    //    Loge("Error registering observer :" + e);
    //}

    if (DBG) {
        mInetLog = new List<String>();
    }

    mSettingsObserver = new SettingsObserver(mHandler, EVENT_APPLY_GLOBAL_HTTP_PROXY, this);
    mSettingsObserver->Observe(mContext);

    mCaptivePortalTracker = CaptivePortalTracker::MakeCaptivePortalTracker(mContext, (IIConnectivityManager*)this);
    LoadGlobalProxy();
    return NOERROR;
}

/**
 * Loads external WiMAX library and registers as system service, returning a
 * {@link NetworkStateTracker} for WiMAX. Caller is still responsible for
 * invoking {@link NetworkStateTracker#startMonitoring(Context, Handler)}.
 */
AutoPtr<INetworkStateTracker> CConnectivityService::MakeWimaxStateTracker(
    /* [in] */ IContext* context,
    /* [in] */ IHandler* trackerHandler)
{
    // Initialize Wimax
    // DexClassLoader wimaxClassLoader;
    // Class wimaxStateTrackerClass = null;
    // Class wimaxServiceClass = null;
    // Class wimaxManagerClass;
    // String wimaxJarLocation;
    // String wimaxLibLocation;
    // String wimaxManagerClassName;
    // String wimaxServiceClassName;
    // String wimaxStateTrackerClassName;

    // NetworkStateTracker wimaxStateTracker = null;

    // boolean isWimaxEnabled = context.getResources().getBoolean(
    //         com.android.internal.R.bool.config_wimaxEnabled);

    // if (isWimaxEnabled) {
    //     try {
    //         wimaxJarLocation = context.getResources().getString(
    //                 com.android.internal.R.string.config_wimaxServiceJarLocation);
    //         wimaxLibLocation = context.getResources().getString(
    //                 com.android.internal.R.string.config_wimaxNativeLibLocation);
    //         wimaxManagerClassName = context.getResources().getString(
    //                 com.android.internal.R.string.config_wimaxManagerClassname);
    //         wimaxServiceClassName = context.getResources().getString(
    //                 com.android.internal.R.string.config_wimaxServiceClassname);
    //         wimaxStateTrackerClassName = context.getResources().getString(
    //                 com.android.internal.R.string.config_wimaxStateTrackerClassname);

    //         if (DBG) log("wimaxJarLocation: " + wimaxJarLocation);
    //         wimaxClassLoader =  new DexClassLoader(wimaxJarLocation,
    //                 new ContextWrapper(context).getCacheDir().getAbsolutePath(),
    //                 wimaxLibLocation, ClassLoader.getSystemClassLoader());

    //         try {
    //             wimaxManagerClass = wimaxClassLoader.loadClass(wimaxManagerClassName);
    //             wimaxStateTrackerClass = wimaxClassLoader.loadClass(wimaxStateTrackerClassName);
    //             wimaxServiceClass = wimaxClassLoader.loadClass(wimaxServiceClassName);
    //         } catch (ClassNotFoundException ex) {
    //             loge("Exception finding Wimax classes: " + ex.toString());
    //             return null;
    //         }
    //     } catch(Resources.NotFoundException ex) {
    //         loge("Wimax Resources does not exist!!! ");
    //         return null;
    //     }

    //     try {
    //         if (DBG) log("Starting Wimax Service... ");

    //         Constructor wmxStTrkrConst = wimaxStateTrackerClass.getConstructor
    //                 (new Class[] {Context.class, Handler.class});
    //         wimaxStateTracker = (NetworkStateTracker) wmxStTrkrConst.newInstance(
    //                 context, trackerHandler);

    //         Constructor wmxSrvConst = wimaxServiceClass.getDeclaredConstructor
    //                 (new Class[] {Context.class, wimaxStateTrackerClass});
    //         wmxSrvConst.setAccessible(true);
    //         IBinder svcInvoker = (IBinder)wmxSrvConst.newInstance(context, wimaxStateTracker);
    //         wmxSrvConst.setAccessible(false);

    //         ServiceManager.addService(WimaxManagerConstants.WIMAX_SERVICE, svcInvoker);

    //     } catch(Exception ex) {
    //         loge("Exception creating Wimax classes: " + ex.toString());
    //         return null;
    //     }
    // } else {
    //     loge("Wimax is not enabled or not added to the network attributes!!! ");
    //     return null;
    // }

    // return wimaxStateTracker;
    assert(0);
    return NULL;
}

/**
* Sets the preferred network.
* @param preference the new preference
*/
ECode CConnectivityService::SetNetworkPreference(
    /* [in] */ Int32 preference)
{
    FAIL_RETURN(EnforceChangePermission());

    AutoPtr<IMessage> msg;
    mHandler->ObtainMessage(EVENT_SET_NETWORK_PREFERENCE, preference, 0, (IMessage**)&msg);
    Boolean result;
    return mHandler->SendMessage(msg, &result);
}

ECode CConnectivityService::GetNetworkPreference(
    /* [out] */ Int32* preference)
{
    VALIDATE_NOT_NULL(preference);
    FAIL_RETURN(EnforceAccessPermission());
    {
        AutoLock lock(this);
        *preference = mNetworkPreference;
    }
    return NOERROR;
}

void CConnectivityService::HandleSetNetworkPreference(
    /* [in] */ Int32 preference)
{
    Boolean isValid = FALSE;
    Boolean result = FALSE;
    AutoPtr<IConnectivityManagerHelper> helper;
    CConnectivityManagerHelper::AcquireSingleton((IConnectivityManagerHelper**)&helper);
    if ((helper->IsNetworkTypeValid(preference, &isValid), isValid) &&
            ((*mNetConfigs)[preference] != NULL) &&
            ((*mNetConfigs)[preference]->IsDefault(&result), result)) {
        if (mNetworkPreference != preference) {
            AutoPtr<IContentResolver> cr;
            mContext->GetContentResolver((IContentResolver**)&cr);
            // TODO:
            Boolean bol;
            AutoPtr<ISettingsGlobal> settingsGlobal;
            CSettingsGlobal::AcquireSingleton((ISettingsGlobal**)&settingsGlobal);
            settingsGlobal->PutInt32(cr, ISettingsGlobal::NETWORK_PREFERENCE, preference, &bol);
            {
                AutoLock lock(this);
                mNetworkPreference = preference;
            }
            EnforcePreference();
        }
    }
}

Int32 CConnectivityService::GetConnectivityChangeDelay()
{
    AutoPtr<IContentResolver> cr;
    mContext->GetContentResolver((IContentResolver**)&cr);

    /** Check system properties for the default value then use secure settings value, if any. */
    AutoPtr<ISystemProperties> sysProp;
    CSystemProperties::AcquireSingleton((ISystemProperties**)&sysProp);
    Int32 defaultDelay;
    sysProp->GetInt32(String("conn.") + ISettingsGlobal::CONNECTIVITY_CHANGE_DELAY,
            IConnectivityManager::CONNECTIVITY_CHANGE_DELAY_DEFAULT, &defaultDelay);
    Int32 val;
    AutoPtr<ISettingsGlobal> settingsGlobal;
    CSettingsGlobal::AcquireSingleton((ISettingsGlobal**)&settingsGlobal);
    settingsGlobal->GetInt32(cr, ISettingsGlobal::CONNECTIVITY_CHANGE_DELAY,
            defaultDelay, &val);
    return val;
}

Int32 CConnectivityService::GetPersistedNetworkPreference()
{
    AutoPtr<IContentResolver> cr;
    mContext->GetContentResolver((IContentResolver**)&cr);

    Int32 networkPrefSetting;
    AutoPtr<ISettingsGlobal> settingsGlobal;
    CSettingsGlobal::AcquireSingleton((ISettingsGlobal**)&settingsGlobal);
    settingsGlobal->GetInt32(cr, ISettingsGlobal::NETWORK_PREFERENCE, -1, &networkPrefSetting);
    if (networkPrefSetting != -1) {
        return networkPrefSetting;
    }

    return IConnectivityManager::DEFAULT_NETWORK_PREFERENCE;
}

/**
 * Make the state of network connectivity conform to the preference settings
 * In this method, we only tear down a non-preferred network. Establishing
 * a connection to the preferred network is taken care of when we handle
 * the disconnect event from the non-preferred network
 * (see {@link #handleDisconnect(NetworkInfo)}).
 */
void CConnectivityService::EnforcePreference()
{
    AutoPtr<INetworkInfo> info;
    (*mNetTrackers)[mNetworkPreference]->GetNetworkInfo((INetworkInfo**)&info);
    Boolean result = FALSE;
    if (info->IsConnected(&result), result) {
        return;
    }

    if ((*mNetTrackers)[mNetworkPreference]->IsAvailable(&result), !result) {
        return;
    }

    for (Int32 t = 0; t <= IConnectivityManager::MAX_RADIO_TYPE; t++) {
        if (t != mNetworkPreference && (*mNetTrackers)[t] != NULL) {
            AutoPtr<INetworkInfo> info;
            (*mNetTrackers)[t]->GetNetworkInfo((INetworkInfo**)&info);
            Boolean connect = FALSE;
            if ((info->IsConnected(&connect), connect)) {
                if (DBG) {
                    Slogger::D(TAG, "tearing down %p in enforcePreference", info.Get());
                }
                Teardown((*mNetTrackers)[t]);
            }
        }
    }
}

Boolean CConnectivityService::Teardown(
    /* [in] */ INetworkStateTracker* netTracker)
{
    Boolean result;
    if (netTracker->Teardown(&result), result) {
        netTracker->SetTeardownRequested(TRUE);
        return TRUE;
    }
    else {
        return FALSE;
    }
}

/**
 * Check if UID should be blocked from using the network represented by the
 * given {@link NetworkStateTracker}.
 */
Boolean CConnectivityService::IsNetworkBlocked(
    /* [in] */ INetworkStateTracker* tracker,
    /* [in] */ Int32 uid)
{
    AutoPtr<ILinkProperties> properties;
    tracker->GetLinkProperties((ILinkProperties**)&properties);
    String iface;
    properties->GetInterfaceName(&iface);

    Boolean networkCostly = FALSE;
    Int32 uidRules;
    {
        AutoLock lock(mRulesLock);

        networkCostly = !iface.IsNull() && mMeteredIfaces.Find(iface) != mMeteredIfaces.End();
        HashMap<Int32, Int32>::Iterator it = mUidRules.Find(uid);
        uidRules = (it != mUidRules.End()) ? it->mSecond : INetworkPolicyManager::RULE_ALLOW_ALL;
    }

    if (networkCostly && (uidRules & INetworkPolicyManager::RULE_REJECT_METERED) != 0) {
        return TRUE;
    }

    // no restrictive rules; network is visible
    return FALSE;
}

/**
 * Return a filtered {@link NetworkInfo}, potentially marked
 * {@link DetailedState#BLOCKED} based on
 * {@link #isNetworkBlocked(NetworkStateTracker, int)}.
 */
AutoPtr<INetworkInfo> CConnectivityService::GetFilteredNetworkInfo(
    /* [in] */ INetworkStateTracker* tracker,
    /* [in] */ Int32 uid)
{
    AutoPtr<INetworkInfo> info;
    tracker->GetNetworkInfo((INetworkInfo**)&info);
    if (IsNetworkBlocked(tracker, uid)) {
        // network is blocked; clone and override state
        AutoPtr<INetworkInfo> info2;
        CNetworkInfo::New(info, (INetworkInfo**)&info2);
        info2->SetDetailedState(NetworkInfoDetailedState_BLOCKED, String(NULL), String(NULL));
        info = info2;
    }
    if (mLockdownTracker != NULL) {
        info = mLockdownTracker->AugmentNetworkInfo(info);
    }
    return info;
}

/**
* Return NetworkInfo for the active (i.e., connected) network interface.
* It is assumed that at most one network is active at a time. If more
* than one is active, it is indeterminate which will be returned.
* @return the info for the active network, or {@code NULL} if none is
* active
*/
ECode CConnectivityService::GetActiveNetworkInfo(
    /* [out] */ INetworkInfo** result)
{
    VALIDATE_NOT_NULL(result);
    *result = NULL;

    FAIL_RETURN(EnforceAccessPermission());
    Int32 uid = Binder::GetCallingUid();
    AutoPtr<IContentResolver> cr;
    mContext->GetContentResolver((IContentResolver**)&cr);
    Int32 retVal;
    AutoPtr<ISettingsSecure> settingsSecure;
    CSettingsSecure::AcquireSingleton((ISettingsSecure**)&settingsSecure);
    settingsSecure->GetInt32(cr, ISettingsSecure::WIFI_CHEAT_ON, 0, &retVal);
    if (retVal != 0) {
        AutoPtr<INetworkInfo> info;
        GetNetworkInfo(mActiveDefaultNetwork, uid, (INetworkInfo**)&info);
        //Cheat ethernet to wifi --Begin--
        if (info != NULL) {
            Int32 type;
            info->GetType(&type);
            if (type == IConnectivityManager::TYPE_ETHERNET) {
                AutoPtr<INetworkInfo> notTrueWifi;
                (*mNetTrackers)[IConnectivityManager::TYPE_WIFI]->GetNetworkInfo((INetworkInfo**)&notTrueWifi);
                Int32 state;
                info->GetDetailedState(&state);
                String reason, extraInfo;
                info->GetReason(&reason);
                info->GetExtraInfo(&extraInfo);
                notTrueWifi->SetDetailedState(state, reason, extraInfo);
                Boolean available;
                info->IsAvailable(&available);
                notTrueWifi->SetIsAvailable(available);
                *result = notTrueWifi;
                REFCOUNT_ADD(*result);
                return NOERROR;
            }
        }
        *result = info;
        REFCOUNT_ADD(*result);
        //Cheat ethernet to wifi --End--
        return NOERROR;
    }
    return GetNetworkInfo(mActiveDefaultNetwork, uid, result);
}

ECode CConnectivityService::GetActiveNetworkInfoUnfiltered(
    /* [out] */ INetworkInfo** info)
{
    VALIDATE_NOT_NULL(info);
    *info = NULL;

    FAIL_RETURN(EnforceAccessPermission());
    AutoPtr<IConnectivityManagerHelper> helper;
    CConnectivityManagerHelper::AcquireSingleton((IConnectivityManagerHelper**)&helper);
    Boolean isValid = FALSE;
    if ((helper->IsNetworkTypeValid(mActiveDefaultNetwork, &isValid), isValid)) {
        AutoPtr<INetworkStateTracker> tracker = (*mNetTrackers)[mActiveDefaultNetwork];
        if (tracker != NULL) {
            return tracker->GetNetworkInfo(info);
        }
    }

    return NOERROR;
}

ECode CConnectivityService::GetActiveNetworkInfoForUid(
    /* [in] */ Int32 uid,
    /* [out] */ INetworkInfo** info)
{
    VALIDATE_NOT_NULL(info);
    *info = NULL;

    FAIL_RETURN(EnforceConnectivityInternalPermission());
    return GetNetworkInfo(mActiveDefaultNetwork, uid, info);
}

ECode CConnectivityService::GetNetworkInfo(
    /* [in] */ Int32 networkType,
    /* [out] */ INetworkInfo** result)
{
    VALIDATE_NOT_NULL(result);
    *result = NULL;

    AutoPtr<IContentResolver> cr;
    mContext->GetContentResolver((IContentResolver**)&cr);
    Int32 retVal;
    AutoPtr<ISettingsSecure> settingsSecure;
    CSettingsSecure::AcquireSingleton((ISettingsSecure**)&settingsSecure);
    settingsSecure->GetInt32(cr, ISettingsSecure::WIFI_CHEAT_ON, 0, &retVal);
    if (retVal != 0) {
        if (networkType != IConnectivityManager::TYPE_ETHERNET) {
            FAIL_RETURN(EnforceAccessPermission());
        }

        Int32 uid = Binder::GetCallingUid();
        AutoPtr<INetworkInfo> info;
        GetNetworkInfo(networkType, uid, (INetworkInfo**)&info);
        //Cheat ethernet to wifi --Begin--
        if (info != NULL) {
            Int32 type;
            info->GetType(&type);
            if (mActiveDefaultNetwork == IConnectivityManager::TYPE_ETHERNET
                && type == IConnectivityManager::TYPE_WIFI) {
                AutoPtr<INetworkInfo> ethInfo;
                (*mNetTrackers)[IConnectivityManager::TYPE_ETHERNET]->GetNetworkInfo((INetworkInfo**)&ethInfo);
                Int32 state;
                ethInfo->GetDetailedState(&state);
                String reason, extraInfo;
                ethInfo->GetReason(&reason);
                ethInfo->GetExtraInfo(&extraInfo);
                info->SetDetailedState(state, reason, extraInfo);
                Boolean available;
                ethInfo->IsAvailable(&available);
                info->SetIsAvailable(available);
            }
        }
        *result = info;
        REFCOUNT_ADD(*result);
        //Cheat ethernet to wifi --End--
        return NOERROR;
    }

    FAIL_RETURN(EnforceAccessPermission());
    Int32 uid = Binder::GetCallingUid();
    return GetNetworkInfo(networkType, uid, result);
}

ECode CConnectivityService::GetNetworkInfo(
    /* [in] */ Int32 networkType,
    /* [in] */ Int32 uid,
    /* [out] */ INetworkInfo** info)
{
    VALIDATE_NOT_NULL(info);
    *info = NULL;
    AutoPtr<IConnectivityManagerHelper> helper;
    CConnectivityManagerHelper::AcquireSingleton((IConnectivityManagerHelper**)&helper);
    Boolean isValid = FALSE;
    if ((helper->IsNetworkTypeValid(networkType, &isValid), isValid)) {
        AutoPtr<INetworkStateTracker> tracker = (*mNetTrackers)[networkType];
        if (tracker != NULL) {
            AutoPtr<INetworkInfo> i = GetFilteredNetworkInfo(tracker, uid);
            *info = i;
            REFCOUNT_ADD(*info);
        }
    }
    return NOERROR;
}

ECode CConnectivityService::GetAllNetworkInfo(
    /* [out, callee] */ ArrayOf<INetworkInfo*>** allInfo)
{
    VALIDATE_NOT_NULL(allInfo);
    FAIL_RETURN(EnforceAccessPermission());
    Int32 uid = Binder::GetCallingUid();
    List< AutoPtr<INetworkInfo> > result;
    {
        AutoLock lock(mRulesLock);
        for ( Int32 i = 0; i < mNetTrackers->GetLength(); i++ ) {
            if ((*mNetTrackers)[i] != NULL) {
                AutoPtr<INetworkInfo> info = GetFilteredNetworkInfo((*mNetTrackers)[i], uid);
                result.PushBack(info);
            }
        }
    }

    AutoPtr< ArrayOf<INetworkInfo*> > infoArray = ArrayOf<INetworkInfo*>::Alloc(result.GetSize());
    List< AutoPtr<INetworkInfo> >::Iterator it = result.Begin();
    for (Int32 i = 0; it != result.End(); ++it, ++i) {
        AutoPtr<INetworkInfo> netInfo = *it;
        infoArray->Set(i, netInfo);
    }
    *allInfo = infoArray;
    REFCOUNT_ADD(*allInfo);
    return NOERROR;
}

ECode CConnectivityService::IsNetworkSupported(
    /* [in] */ Int32 networkType,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
    FAIL_RETURN(EnforceAccessPermission());
    AutoPtr<IConnectivityManagerHelper> helper;
    CConnectivityManagerHelper::AcquireSingleton((IConnectivityManagerHelper**)&helper);
    Boolean isValid = FALSE;
    helper->IsNetworkTypeValid(networkType, &isValid);
    *result = (isValid && ((*mNetTrackers)[networkType] != NULL));
    return NOERROR;
}

/**
* Return LinkProperties for the active (i.e., connected) default
* network interface.  It is assumed that at most one default network
* is active at a time. If more than one is active, it is indeterminate
* which will be returned.
* @return the ip properties for the active network, or {@code NULL} if
* none is active
*/
ECode CConnectivityService::GetActiveLinkProperties(
    /* [out] */ ILinkProperties** properties)
{
    return GetLinkProperties(mActiveDefaultNetwork, properties);
}

ECode CConnectivityService::GetLinkProperties(
    /* [in] */ Int32 networkType,
    /* [out] */ ILinkProperties** properties)
{
    VALIDATE_NOT_NULL(properties);
    *properties = NULL;

    AutoPtr<IContentResolver> cr;
    mContext->GetContentResolver((IContentResolver**)&cr);
    Int32 retVal;
    AutoPtr<ISettingsSecure> settingsSecure;
    CSettingsSecure::AcquireSingleton((ISettingsSecure**)&settingsSecure);
    settingsSecure->GetInt32(cr, ISettingsSecure::WIFI_CHEAT_ON, 0, &retVal);
    if (retVal != 0) {
        if (networkType != IConnectivityManager::TYPE_ETHERNET) {
            FAIL_RETURN(EnforceAccessPermission());
        }
    }
    else {
        FAIL_RETURN(EnforceAccessPermission());
    }
    AutoPtr<IConnectivityManagerHelper> helper;
    CConnectivityManagerHelper::AcquireSingleton((IConnectivityManagerHelper**)&helper);
    Boolean isValid = FALSE;
    if ((helper->IsNetworkTypeValid(networkType, &isValid), isValid)) {
        AutoPtr<INetworkStateTracker> tracker = (*mNetTrackers)[networkType];
        if (tracker != NULL) {
            return tracker->GetLinkProperties(properties);
        }
    }

    return NOERROR;
}

ECode CConnectivityService::GetAllNetworkState(
    /* [out, callee] */ ArrayOf<INetworkState*>** allStates)
{
    VALIDATE_NOT_NULL(allStates);
    *allStates = NULL;

    FAIL_RETURN(EnforceAccessPermission());
    Int32 uid = Binder::GetCallingUid();
    List< AutoPtr<INetworkState> > result;
    {
        AutoLock lock(mRulesLock);
        for (Int32 i = 0; i < mNetTrackers->GetLength(); i++) {
            AutoPtr<INetworkStateTracker> tracker = (*mNetTrackers)[i];
            if(tracker != NULL) {
                AutoPtr<INetworkInfo> info = GetFilteredNetworkInfo(tracker, uid);
                AutoPtr<ILinkProperties> properties;
                tracker->GetLinkProperties((ILinkProperties**)&properties);
                AutoPtr<ILinkCapabilities> capabilities;
                tracker->GetLinkCapabilities((ILinkCapabilities**)&capabilities);
                AutoPtr<INetworkState> state;
                CNetworkState::New(info, properties, capabilities, (INetworkState**)&state);
                result.PushBack(state);
            }
        }
    }
    AutoPtr< ArrayOf<INetworkState*> > stateArray = ArrayOf<INetworkState*>::Alloc(result.GetSize());
    List< AutoPtr<INetworkState> >::Iterator it = result.Begin();
    for (Int32 i = 0; it != result.End(); ++it, ++i) {
        AutoPtr<INetworkState> netState = *it;
        stateArray->Set(i, netState);
    }
    *allStates = stateArray;
    REFCOUNT_ADD(*allStates);
    return NOERROR;
}

AutoPtr<INetworkState> CConnectivityService::GetNetworkStateUnchecked(
    /* [in] */ Int32 networkType)
{
    AutoPtr<IConnectivityManagerHelper> helper;
    CConnectivityManagerHelper::AcquireSingleton((IConnectivityManagerHelper**)&helper);
    Boolean isValid = FALSE;
    if ((helper->IsNetworkTypeValid(networkType, &isValid), isValid)) {
        AutoPtr<INetworkStateTracker> tracker = (*mNetTrackers)[networkType];
        if (tracker != NULL) {
            AutoPtr<INetworkInfo> info;
            tracker->GetNetworkInfo((INetworkInfo**)&info);
            AutoPtr<ILinkProperties> properties;
            tracker->GetLinkProperties((ILinkProperties**)&properties);
            AutoPtr<ILinkCapabilities> capabilities;
            tracker->GetLinkCapabilities((ILinkCapabilities**)&capabilities);
            AutoPtr<INetworkState> state;
            CNetworkState::New(info, properties, capabilities, (INetworkState**)&state);
            return state;
        }
    }
    return NULL;
}

ECode CConnectivityService::GetActiveNetworkQuotaInfo(
    /* [out] */ INetworkQuotaInfo** info)
{
    VALIDATE_NOT_NULL(info);
    *info = NULL;

    FAIL_RETURN(EnforceAccessPermission());

    Int64 token = Binder::ClearCallingIdentity();
    AutoPtr<INetworkState> state = GetNetworkStateUnchecked(mActiveDefaultNetwork);
    if (state != NULL) {
        ECode ec = mPolicyManager->GetNetworkQuotaInfo(state, info);
        Binder::RestoreCallingIdentity(token);
        if (FAILED(ec)) *info = NULL;
        return NOERROR;
    }
    Binder::RestoreCallingIdentity(token);
    *info = NULL;
    return NOERROR;
}

ECode CConnectivityService::IsActiveNetworkMetered(
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
    FAIL_RETURN(EnforceAccessPermission());
    Int64 token = Binder::ClearCallingIdentity();
    *result = IsNetworkMeteredUnchecked(mActiveDefaultNetwork);
    Binder::RestoreCallingIdentity(token);
    return NOERROR;
}

Boolean CConnectivityService::IsNetworkMeteredUnchecked(
    /* [in] */ Int32 networkType)
{
    AutoPtr<INetworkState> state = GetNetworkStateUnchecked(networkType);
    if (state != NULL) {
        Boolean result;
        ECode ec = mPolicyManager->IsNetworkMetered(state, &result);
        if (SUCCEEDED(ec)) return result;
    }
    return FALSE;
}

ECode CConnectivityService::SetRadios(
    /* [in] */ Boolean turnOn,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
    *result = TRUE;
    FAIL_RETURN(EnforceChangePermission());
    for (Int32 i = 0; i < mNetTrackers->GetLength(); i++) {
        AutoPtr<INetworkStateTracker> tracker = (*mNetTrackers)[i];
        if (tracker != NULL) {
            Boolean b;
            tracker->SetRadio(turnOn, &b);
            *result = b && *result;
        }
    }
    return NOERROR;
}

ECode CConnectivityService::SetRadio(
    /* [in] */ Int32 netType,
    /* [in] */ Boolean turnOn,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
    FAIL_RETURN(EnforceChangePermission());
    Boolean isValid = FALSE;
    AutoPtr<IConnectivityManagerHelper> helper;
    CConnectivityManagerHelper::AcquireSingleton((IConnectivityManagerHelper**)&helper);
    if ((helper->IsNetworkTypeValid(netType, &isValid), !isValid)) {
        *result = FALSE;
        return NOERROR;
    }
    AutoPtr<INetworkStateTracker> tracker = (*mNetTrackers)[netType];
    if (tracker != NULL) {
        return tracker->SetRadio(turnOn, result);
    }
    *result = FALSE;
    return NOERROR;
}

ECode CConnectivityService::StartUsingNetworkFeature(
    /* [in] */ Int32 networkType,
    /* [in] */ const String& feature,
    /* [in] */ IBinder* binder,
    /* [out] */ Int32* status)
{
    VALIDATE_NOT_NULL(status);
    Int64 startTime = 0;
    if (DBG) {
        startTime = SystemClock::GetElapsedRealtime();
    }
    if (VDBG) {
        Slogger::V(TAG, "startUsingNetworkFeature for net %d : %s, uid=%d", networkType,
                feature.string(), Binder::GetCallingUid());
    }
    FAIL_RETURN(EnforceChangePermission());

    AutoPtr<IConnectivityManagerHelper> helper;
    CConnectivityManagerHelper::AcquireSingleton((IConnectivityManagerHelper**)&helper);
    Boolean isValid = TRUE;
    if ((helper->IsNetworkTypeValid(networkType, &isValid), !isValid) ||
            (*mNetConfigs)[networkType] == NULL) {
        if (DBG) {
            Int64 execTime = SystemClock::GetElapsedRealtime() - startTime;
            if (execTime > 250) {
               Slogger::E(TAG, "startUsingNetworkFeature took too long: %dms", execTime);
            }
            else {
               if (VDBG) Slogger::E(TAG, "startUsingNetworkFeature took %dms", execTime);
            }
        }
        // TODO:
        *status = 3;//*status = PhoneConstants::APN_REQUEST_FAILED;
        return NOERROR;
    }

    AutoPtr<FeatureUser> fuser = new FeatureUser(networkType, feature, binder, this);

    // TODO - move this into individual networktrackers
    Int32 usedNetworkType = ConvertFeatureToNetworkType(networkType, feature);

    if (mLockdownEnabled) {
        // Since carrier APNs usually aren't available from VPN
        // endpoint, mark them as unavailable.
        if (DBG) {
            Int64 execTime = SystemClock::GetElapsedRealtime() - startTime;
            if (execTime > 250) {
               Slogger::E(TAG, "startUsingNetworkFeature took too long: %dms", execTime);
            }
            else {
               if (VDBG) Slogger::E(TAG, "startUsingNetworkFeature took %dms", execTime);
            }
        }
        // TODO:
        *status = 2;//*status = IPhoneConstants::APN_TYPE_NOT_AVAILABLE;
        return NOERROR;
    }

    if (Find(mProtectedNetworks.Begin(), mProtectedNetworks.End(), usedNetworkType)
            != mProtectedNetworks.End()) {
        FAIL_RETURN(EnforceConnectivityInternalPermission());
    }

    // if UID is restricted, don't allow them to bring up metered APNs
    Boolean networkMetered = IsNetworkMeteredUnchecked(usedNetworkType);
    Int32 uidRules;
    {
        AutoLock lock(mRulesLock);
        HashMap<Int32, Int32>::Iterator it = mUidRules.Find(Binder::GetCallingUid());
        if (it != mUidRules.End()) {
            uidRules = it->mSecond;
        }
        else {
            uidRules = INetworkPolicyManager::RULE_ALLOW_ALL;
        }
    }
    if (networkMetered && (uidRules & INetworkPolicyManager::RULE_REJECT_METERED) != 0) {
        if (DBG) {
            Int64 execTime = SystemClock::GetElapsedRealtime() - startTime;
            if (execTime > 250) {
               Slogger::E(TAG, "startUsingNetworkFeature took too long: %dms", execTime);
            }
            else {
               if (VDBG) Slogger::E(TAG, "startUsingNetworkFeature took %dms", execTime);
            }
        }
        // TODO:
        *status = 3;//*status = IPhoneConstants::APN_REQUEST_FAILED;
        return NOERROR;
    }

    AutoPtr<INetworkStateTracker> network = (*mNetTrackers)[usedNetworkType];
    if (network != NULL) {
        Int32 currentPid = Binder::GetCallingPid();
        if (usedNetworkType != networkType) {
            AutoPtr<INetworkInfo> ni;
            network->GetNetworkInfo((INetworkInfo**)&ni);

            Boolean avail;
            if (ni->IsAvailable(&avail), !avail) {
                // TODO:
                if (!TextUtils::Equals(feature, String("enableDUNAlways")/*IPhone::FEATURE_ENABLE_DUN_ALWAYS*/)) {
                    if (DBG) {
                        String typeName;
                        ni->GetTypeName(&typeName);
                        Slogger::D(TAG, "special network not available ni = %s",
                                typeName.string());
                    }
                    if (DBG) {
                        Int64 execTime = SystemClock::GetElapsedRealtime() - startTime;
                        if (execTime > 250) {
                           Slogger::E(TAG, "startUsingNetworkFeature took too long: %dms", execTime);
                        }
                        else {
                           if (VDBG) Slogger::E(TAG, "startUsingNetworkFeature took %dms", execTime);
                        }
                    }
                    // TODO:
                    *status = 2;//*status = IPhoneConstants::APN_TYPE_NOT_AVAILABLE;
                    return NOERROR;
                }
                else {
                    // else make the attempt anyway - probably giving REQUEST_STARTED below
                    if (DBG) {
                        String typeName;
                        ni->GetTypeName(&typeName);
                        Slogger::D(TAG, "special network not available, but try anyway ni = %s",
                                typeName.string());
                    }
                }
            }

            Int32 restoreTimer = GetRestoreDefaultNetworkDelay(usedNetworkType);

            {
                AutoLock lock(this);
                Boolean addToList = TRUE;
                if (restoreTimer < 0) {
                    // In case there is no timer is specified for the feature,
                    // make sure we don't add duplicate entry with the same request.
                    List< AutoPtr<FeatureUser> >::Iterator it;
                    for (it = mFeatureUsers.Begin(); it != mFeatureUsers.End(); ++it) {
                        AutoPtr<FeatureUser> u = *it;
                        if (u->IsSameUser(fuser)) {
                            addToList = FALSE;
                            break;
                        }
                    }
                }

                if (addToList) {
                    mFeatureUsers.PushBack(fuser);
                }

                AutoPtr< List<Int32> > netRequestersPid = (*mNetRequestersPids)[usedNetworkType];
                if (Find(netRequestersPid->Begin(), netRequestersPid->End(), currentPid)
                        == netRequestersPid->End()) {
                    // this gets used for per-pid dns when connected
                    netRequestersPid->PushBack(currentPid);
                }
            }

            if (restoreTimer >= 0) {
                AutoPtr<IMessage> msg;
                mHandler->ObtainMessage(EVENT_RESTORE_DEFAULT_NETWORK, fuser, (IMessage**)&msg);
                Boolean result;
                return mHandler->SendMessageDelayed(msg, restoreTimer, &result);
            }

            Boolean result;
            if ((ni->IsConnectedOrConnecting(&result), result) &&
                    (network->IsTeardownRequested(&result), !result)) {
                if (ni->IsConnected(&result), result) {
                    Int64 token = Binder::ClearCallingIdentity();
                    // add the pid-specific dns
                    HandleDnsConfigurationChange(usedNetworkType);
                    if (VDBG) Slogger::V(TAG, "special network already active");
                    Binder::RestoreCallingIdentity(token);
                    if (DBG) {
                        Int64 execTime = SystemClock::GetElapsedRealtime() - startTime;
                        if (execTime > 250) {
                           Slogger::E(TAG, "startUsingNetworkFeature took too long: %dms", execTime);
                        }
                        else {
                           if (VDBG) Slogger::E(TAG, "startUsingNetworkFeature took %dms", execTime);
                        }
                    }
                    // TODO:
                    *status = 0 ;//*status = IPhoneConstants::APN_ALREADY_ACTIVE;
                    return NOERROR;
                }
                if (VDBG) Slogger::V(TAG, "special network already connecting");
                if (DBG) {
                    Int64 execTime = SystemClock::GetElapsedRealtime() - startTime;
                    if (execTime > 250) {
                       Slogger::E(TAG, "startUsingNetworkFeature took too long: %dms", execTime);
                    }
                    else {
                       if (VDBG) Slogger::E(TAG, "startUsingNetworkFeature took %dms", execTime);
                    }
                }
                // TODO:
                *status = 1;//*status = IPhoneConstants::APN_REQUEST_STARTED;
                return NOERROR;
            }

            // check if the radio in play can make another contact
            // assume if cannot for now

            if (DBG) {
                Slogger::D(TAG, "startUsingNetworkFeature reconnecting to %d: %s ",
                        networkType, feature.string());
            }
            network->Reconnect(&result);
            if (DBG) {
                Int64 execTime = SystemClock::GetElapsedRealtime() - startTime;
                if (execTime > 250) {
                   Slogger::E(TAG, "startUsingNetworkFeature took too long: %dms", execTime);
                }
                else {
                   if (VDBG) Slogger::E(TAG, "startUsingNetworkFeature took %dms", execTime);
                }
            }
            // TODO:
            *status = 1;//*status = IPhoneConstants::APN_REQUEST_STARTED;
            return NOERROR;
        }
        else {
            // need to remember this unsupported request so we respond appropriately on stop
            {
                AutoLock lock(this);
                mFeatureUsers.PushBack(fuser);
                AutoPtr< List<Int32> > netRequestersPid = (*mNetRequestersPids)[usedNetworkType];
                if (Find(netRequestersPid->Begin(), netRequestersPid->End(), currentPid)
                        == netRequestersPid->End()) {
                    // this gets used for per-pid dns when connected
                    netRequestersPid->PushBack(currentPid);
                }
            }
            if (DBG) {
                Int64 execTime = SystemClock::GetElapsedRealtime() - startTime;
                if (execTime > 250) {
                   Slogger::E(TAG, "startUsingNetworkFeature took too long: %dms", execTime);
                }
                else {
                   if (VDBG) Slogger::E(TAG, "startUsingNetworkFeature took %dms", execTime);
                }
            }
            *status = -1;
            return NOERROR;
        }
    }
    if (DBG) {
        Int64 execTime = SystemClock::GetElapsedRealtime() - startTime;
        if (execTime > 250) {
           Slogger::E(TAG, "startUsingNetworkFeature took too long: %dms", execTime);
        }
        else {
           if (VDBG) Slogger::E(TAG, "startUsingNetworkFeature took %dms", execTime);
        }
    }
    // TODO:
    *status = 2;//*status = IPhoneConstants::APN_TYPE_NOT_AVAILABLE;
    return NOERROR;
}

ECode CConnectivityService::StopUsingNetworkFeature(
    /* [in] */ Int32 networkType,
    /* [in] */ const String& feature,
    /* [out] */ Int32* status)
{
    VALIDATE_NOT_NULL(status);
    FAIL_RETURN(EnforceChangePermission());
    Int32 pid = Binder::GetCallingPid();
    Int32 uid = Binder::GetCallingUid();

    AutoPtr<FeatureUser> fuser;
    Boolean found = FALSE;

    {
        AutoLock lock(this);
        List< AutoPtr<FeatureUser> >::Iterator it;
        for (it = mFeatureUsers.Begin(); it != mFeatureUsers.End(); ++it) {
            AutoPtr<FeatureUser> fu = *it;
            if (fu->IsSameUser(pid, uid, networkType, feature)) {
                fuser = fu;
                found = TRUE;
                break;
            }
        }
    }
    if (found && (fuser != NULL)) {
        // stop regardless of how many other time this proc had called start
        *status = StopUsingNetworkFeature(fuser, TRUE);
        return NOERROR;
    }
    else {
        // none found!
        if (VDBG) {
           Slogger::V(TAG, "stopUsingNetworkFeature - not a live request, ignoring");
        }
        *status = 1;
        return NOERROR;
    }
}

Int32 CConnectivityService::StopUsingNetworkFeature(
    /* [in] */ FeatureUser* u,
    /* [in] */ Boolean ignoreDups)
{
    Int32 networkType = u->mNetworkType;
    String feature = u->mFeature;
    Int32 pid = u->mPid;
    Int32 uid = u->mUid;

    AutoPtr<INetworkStateTracker> tracker;
    Boolean callTeardown = FALSE;  // used to carry our decision outside of sync block

    if (VDBG) {
       Slogger::V(TAG, "stopUsingNetworkFeature: net %d: %s", networkType, feature.string());
    }

    AutoPtr<IConnectivityManagerHelper> helper;
    CConnectivityManagerHelper::AcquireSingleton((IConnectivityManagerHelper**)&helper);
    Boolean isValid = FALSE;
    if ((helper->IsNetworkTypeValid(networkType, &isValid), !isValid)) {
        if (DBG) {
            Slogger::D(TAG, "stopUsingNetworkFeature: net %d: %s, net is invalid",
                    networkType, feature.string());
        }
        return -1;
    }

    // need to link the mFeatureUsers list with the mNetRequestersPids state in this
    // sync block
    {
        AutoLock lock(this);
        // check if this process still has an outstanding start request
        if (Find(mFeatureUsers.Begin(), mFeatureUsers.End(), AutoPtr<FeatureUser>(u))
                == mFeatureUsers.End()) {
            if (VDBG) {
                Slogger::V(TAG, "stopUsingNetworkFeature: this process has no outstanding requests, ignoring");
            }
            return -1;
        }
        u->UnlinkDeathRecipient();
        mFeatureUsers.Remove(u);
        // If we care about duplicate requests, check for that here.
        //
        // This is done to support the extension of a request - the app
        // can request we start the network feature again and renew the
        // auto-shutoff delay.  Normal "stop" calls from the app though
        // do not pay attention to duplicate requests - in effect the
        // API does not refcount and a single stop will counter multiple starts.
        if (ignoreDups == FALSE) {
            List< AutoPtr<FeatureUser> >::Iterator it;
            for(it = mFeatureUsers.Begin(); it != mFeatureUsers.End(); ++it) {
                AutoPtr<FeatureUser> fu = *it;
                if (fu->IsSameUser(u)) {
                    if (VDBG) Slogger::V(TAG, "stopUsingNetworkFeature: dup is found, ignoring");
                    return 1;
                }
            }
        }

        // TODO - move to individual network trackers
        Int32 usedNetworkType = ConvertFeatureToNetworkType(networkType, feature);

        tracker = (*mNetTrackers)[usedNetworkType];
        if (tracker == NULL) {
            if (DBG) {
                Slogger::D(TAG, "stopUsingNetworkFeature: net %d: %s \
                        no known tracker for used net type %d", networkType, feature.string(), usedNetworkType);
            }
            return -1;
        }
        if (usedNetworkType != networkType) {
            Int32 currentPid = pid;
            (*mNetRequestersPids)[usedNetworkType]->Remove(currentPid);
            ReassessPidDns(pid, TRUE);
            if ((*mNetRequestersPids)[usedNetworkType]->IsEmpty() == FALSE) {
                if (VDBG) {
                   Slogger::V(TAG, "stopUsingNetworkFeature: net %d: %s \
                             others still using it", networkType, feature.string());
                }
                return 1;
            }
            callTeardown = TRUE;
        }
        else {
            if (DBG) {
               Slogger::D(TAG, "stopUsingNetworkFeature: net %d: %s \
                        not a known feature - dropping", networkType, feature.string());
            }
        }
    }

    if (callTeardown) {
        if (DBG) {
           Slogger::D(TAG, "stopUsingNetworkFeature: teardown net %d: %s", networkType, feature.string());
        }
        Boolean result;
        tracker->Teardown(&result);
        return 1;
    }
    else {
        return -1;
    }
}

/**
* @deprecated use requestRouteToHostAddress instead
*
* Ensure that a network route exists to deliver traffic to the specified
* host via the specified network interface.
* @param networkType the type of the network over which traffic to the
* specified host is to be routed
* @param hostAddress the IP address of the host to which the route is
* desired
* @return {@code true} on success, {@code false} on failure
*/
ECode CConnectivityService::RequestRouteToHost(
    /* [in] */ Int32 networkType,
    /* [in] */ Int32 hostAddress,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
    AutoPtr<IInetAddress> inetAddress;
    NetworkUtils::Int32ToInetAddress(hostAddress, (IInetAddress**)&inetAddress);

    if (inetAddress == NULL) {
        *result = FALSE;
        return NOERROR;
    }
    AutoPtr< ArrayOf<Byte> > address;
    inetAddress->GetAddress((ArrayOf<Byte>**)&address);
    return RequestRouteToHostAddress(networkType, address, result);
}

/**
* Ensure that a network route exists to deliver traffic to the specified
* host via the specified network interface.
* @param networkType the type of the network over which traffic to the
* specified host is to be routed
* @param hostAddress the IP address of the host to which the route is
* desired
* @return {@code true} on success, {@code false} on failure
*/
ECode CConnectivityService::RequestRouteToHostAddress(
    /* [in] */ Int32 networkType,
    /* [in] */ ArrayOf<Byte>* hostAddress,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
    FAIL_RETURN(EnforceChangePermission());

    if (Find(mProtectedNetworks.Begin(), mProtectedNetworks.End(), networkType)
            != mProtectedNetworks.End()) {
        FAIL_RETURN(EnforceConnectivityInternalPermission());
    }

    AutoPtr<IConnectivityManagerHelper> helper;
    CConnectivityManagerHelper::AcquireSingleton((IConnectivityManagerHelper**)&helper);
    Boolean isValid;
    if ((helper->IsNetworkTypeValid(networkType, &isValid), !isValid)) {
        if (DBG) Slogger::D(TAG, "requestRouteToHostAddress on invalid network: %d", networkType);
        *result = FALSE;
        return NOERROR;
    }
    AutoPtr<INetworkStateTracker> tracker = (*mNetTrackers)[networkType];
    AutoPtr<INetworkInfo> info;
    tracker->GetNetworkInfo((INetworkInfo**)&info);
    NetworkInfoDetailedState netState;
    info->GetDetailedState(&netState);

    Boolean requested;
    if (tracker == NULL || (netState != NetworkInfoDetailedState_CONNECTED &&
            netState != NetworkInfoDetailedState_CAPTIVE_PORTAL_CHECK) ||
            (tracker->IsTeardownRequested(&requested), requested)) {
        if (VDBG) {
           Slogger::V(TAG, "requestRouteToHostAddress on down network (%d) - dropped", networkType);
        }
        *result = FALSE;
        return NOERROR;
    }
    Int64 token = Binder::ClearCallingIdentity();
    AutoPtr<IInetAddressHelper> addrHelper;
    CInetAddressHelper::AcquireSingleton((IInetAddressHelper**)&addrHelper);
    AutoPtr<IInetAddress> addr;
    ECode ec = addrHelper->GetByAddress(hostAddress, (IInetAddress**)&addr);
    if (FAILED(ec)) {
        if (DBG) Slogger::D(TAG, "requestRouteToHostAddress got %d" + ec);
        Binder::RestoreCallingIdentity(token);
        *result = FALSE;
        return NOERROR;
    }
    AutoPtr<ILinkProperties> lp;
    tracker->GetLinkProperties((ILinkProperties**)&lp);
    *result = AddRouteToAddress(lp, addr);
    Binder::RestoreCallingIdentity(token);
    return NOERROR;
}

Boolean CConnectivityService::AddRoute(
    /* [in] */ ILinkProperties* p,
    /* [in] */ IRouteInfo* r,
    /* [in] */ Boolean toDefaultTable)
{
    String name;
    p->GetInterfaceName(&name);
    return ModifyRoute(name, p, r, 0, ADD, toDefaultTable);
}

Boolean CConnectivityService::RemoveRoute(
    /* [in] */ ILinkProperties* p,
    /* [in] */ IRouteInfo* r,
    /* [in] */ Boolean toDefaultTable)
{
    String name;
    p->GetInterfaceName(&name);
    return ModifyRoute(name, p, r, 0, REMOVE, toDefaultTable);
}

Boolean CConnectivityService::AddRouteToAddress(
    /* [in] */ ILinkProperties* lp,
    /* [in] */ IInetAddress* addr)
{
    return ModifyRouteToAddress(lp, addr, ADD, TO_DEFAULT_TABLE);
}

Boolean CConnectivityService::RemoveRouteToAddress(
    /* [in] */ ILinkProperties* lp,
    /* [in] */ IInetAddress* addr)
{
    return ModifyRouteToAddress(lp, addr, REMOVE, TO_DEFAULT_TABLE);
}

Boolean CConnectivityService::ModifyRouteToAddress(
    /* [in] */ ILinkProperties* lp,
    /* [in] */ IInetAddress* addr,
    /* [in] */ Boolean doAdd,
    /* [in] */ Boolean toDefaultTable)
{
    AutoPtr<IRouteInfoHelper> helper;
    CRouteInfoHelper::AcquireSingleton((IRouteInfoHelper**)&helper);
    AutoPtr<IObjectContainer> routes;
    lp->GetRoutes((IObjectContainer**)&routes);
    AutoPtr<IRouteInfo> bestRoute;
    helper->SelectBestRoute(routes, addr, (IRouteInfo**)&bestRoute);
    if (bestRoute == NULL) {
        helper->MakeHostRoute(addr, (IRouteInfo**)&bestRoute);
    }
    else {
        AutoPtr<IInetAddress> address;
        bestRoute->GetGateway((IInetAddress**)&address);
        Boolean equal = FALSE;
        if ((address->Equals(addr, &equal), equal)) {
            // if there is no better route, add the implied hostroute for our gateway
            bestRoute = NULL;
            helper->MakeHostRoute(addr, (IRouteInfo**)&bestRoute);
        }
        else {
            // if we will connect to this through another route, add a direct route
            // to it's gateway
            bestRoute = NULL;
            helper->MakeHostRoute(addr, address, (IRouteInfo**)&bestRoute);
        }
    }
    String name;
    lp->GetInterfaceName(&name);
    return ModifyRoute(name, lp, bestRoute, 0, doAdd, toDefaultTable);
}

Boolean CConnectivityService::ModifyRoute(
    /* [in] */ const String& ifaceName,
    /* [in] */ ILinkProperties* lp,
    /* [in] */ IRouteInfo* r,
    /* [in] */ Int32 cycleCount,
    /* [in] */ Boolean doAdd,
    /* [in] */ Boolean toDefaultTable)
{
    if ((ifaceName.IsNull()) || (lp == NULL) || (r == NULL)) {
//        if (DBG) Slogger::D(TAG, "modifyRoute got unexpected NULL: " + ifaceName + ", " + lp + ", " + r);
        return FALSE;
    }

    if (cycleCount > MAX_HOSTROUTE_CYCLE_COUNT) {
        Slogger::E(TAG, "Error modifying route - too much recursion");
        return FALSE;
    }

    Boolean isHostRoute;
    r->IsHostRoute(&isHostRoute);
    if (isHostRoute == FALSE) {
        AutoPtr<IObjectContainer> routes;
        lp->GetRoutes((IObjectContainer**)&routes);
        AutoPtr<IInetAddress> gateway;
        r->GetGateway((IInetAddress**)&gateway);
        AutoPtr<IRouteInfoHelper> helper;
        CRouteInfoHelper::AcquireSingleton((IRouteInfoHelper**)&helper);
        AutoPtr<IRouteInfo> bestRoute;
        helper->SelectBestRoute(routes, gateway, (IRouteInfo**)&bestRoute);
        if (bestRoute != NULL) {
            AutoPtr<IInetAddress> address;
            bestRoute->GetGateway((IInetAddress**)&address);
            Boolean equal = FALSE;
            if ((address->Equals(gateway, &equal), equal)) {
                // if there is no better route, add the implied hostroute for our gateway
                bestRoute = NULL;
                helper->MakeHostRoute(gateway, (IRouteInfo**)&bestRoute);
            }
            else {
                // if we will connect to our gateway through another route, add a direct
                // route to it's gateway
                bestRoute = NULL;
                helper->MakeHostRoute(gateway, address, (IRouteInfo**)&bestRoute);
            }
            ModifyRoute(ifaceName, lp, bestRoute, cycleCount + 1, doAdd, toDefaultTable);
        }
    }
    if (doAdd) {
        // if (VDBG) Slogger::V(TAG, "Adding " + r + " for interface " + ifaceName);
        ECode ec = NOERROR;
        if (toDefaultTable) {
            mAddedRoutes.PushBack(r);  // only track default table - only one apps can effect
            ec = mNetd->AddRoute(ifaceName, r);
        }
        else {
            ec = mNetd->AddSecondaryRoute(ifaceName, r);
        }
        if (FAILED(ec)) {
            // never crash - catch them all
            if (DBG) Slogger::E(TAG, "Exception trying to add a route: %d", ec);
            return FALSE;
        }
    }
    else {
        // if we remove this one and there are no more like it, then refcount==0 and
        // we can remove it from the table
        if (toDefaultTable) {
            mAddedRoutes.Remove(r);
            if (Find(mAddedRoutes.Begin(), mAddedRoutes.End(), AutoPtr<IRouteInfo>(r))
                    == mAddedRoutes.End()) {
//                if (VDBG) Slogger::V(TAG, "Removing " + r + " for interface " + ifaceName);
                ECode ec = mNetd->RemoveRoute(ifaceName, r);
                if (FAILED(ec)) {
                    // never crash - catch them all
                    if (VDBG) Slogger::E(TAG, "Exception trying to remove a route: %d", ec);
                    return FALSE;
                }
            }
            else {
//                if (VDBG) Slogger::V(TAG, "not removing " + r + " as it's still in use");
            }
        }
        else {
//            if (VDBG) Slogger::V(TAG, "Removing " + r + " for interface " + ifaceName);
            ECode ec = mNetd->RemoveSecondaryRoute(ifaceName, r);
            if (FAILED(ec)) {
                // never crash - catch them all
                if (VDBG) Slogger::E(TAG, "Exception trying to remove a route: %d", ec);
                return FALSE;
            }
        }
    }
    return TRUE;
}

/**
* @see ConnectivityManager#getMobileDataEnabled()
*/
ECode CConnectivityService::GetMobileDataEnabled(
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
    // TODO: This detail should probably be in DataConnectionTracker's
    //       which is where we store the value and maybe make this
    //       asynchronous.
    FAIL_RETURN(EnforceAccessPermission());
    Int32 retVal;
    AutoPtr<IContentResolver> resolver;
    mContext->GetContentResolver((IContentResolver**)&resolver);
    AutoPtr<ISettingsGlobal> settingsGlobal;
    CSettingsGlobal::AcquireSingleton((ISettingsGlobal**)&settingsGlobal);
    settingsGlobal->GetInt32(resolver, ISettingsGlobal::MOBILE_DATA, 1, &retVal);
    if (VDBG) Slogger::V(TAG, "getMobileDataEnabled returning %d", retVal);
    *result = (retVal == 1);
    return NOERROR;
}

ECode CConnectivityService::SetDataDependency(
    /* [in] */ Int32 networkType,
    /* [in] */ Boolean met)
{
    FAIL_RETURN(EnforceConnectivityInternalPermission());

    AutoPtr<IMessage> msg;
    mHandler->ObtainMessage(EVENT_SET_DEPENDENCY_MET,
        (met ? ENABLED : DISABLED), networkType, (IMessage**)&msg);
    Boolean result;
    return mHandler->SendMessage(msg, &result);
}

void CConnectivityService::HandleSetDependencyMet(
    /* [in] */ Int32 networkType,
    /* [in] */ Boolean met)
{
    if ((*mNetTrackers)[networkType] != NULL) {
        if (DBG) {
           Slogger::D(TAG, "handleSetDependencyMet(%d, %s)", networkType, met ? "true" : "false");
        }
        (*mNetTrackers)[networkType]->SetDependencyMet(met);
    }
}

/**
* @see ConnectivityManager#setMobileDataEnabled(boolean)
*/
ECode CConnectivityService::SetMobileDataEnabled(
    /* [in] */ Boolean enabled)
{
    FAIL_RETURN(EnforceChangePermission());
    if (DBG) Slogger::D(TAG, "setMobileDataEnabled(%s)", enabled ? "true" : "false");

    AutoPtr<IMessage> msg;
    mHandler->ObtainMessage(EVENT_SET_MOBILE_DATA,
        (enabled ? ENABLED : DISABLED), 0, (IMessage**)&msg);
    Boolean result;
    return mHandler->SendMessage(msg, &result);
}

void CConnectivityService::HandleSetMobileData(
    /* [in] */ Boolean enabled)
{
    if ((*mNetTrackers)[IConnectivityManager::TYPE_MOBILE] != NULL) {
//        if (VDBG) {
//            //String strTemp;
//            //(*mNetTrackers)[IConnectivityManager::TYPE_MOBILE]->ToString(&strTemp);
//            //Log(strTemp + enabled);
//        }
        (*mNetTrackers)[IConnectivityManager::TYPE_MOBILE]->SetUserDataEnable(enabled);
    }
    if ((*mNetTrackers)[IConnectivityManager::TYPE_WIMAX] != NULL) {
//        if (VDBG) {
//            //String strTemp;
//            //((*mNetTrackers)[IConnectivityManager::TYPE_WIMAX]->ToString(&strTemp);
//            //Log(strTemp + enabled);
//        }
        (*mNetTrackers)[IConnectivityManager::TYPE_WIMAX]->SetUserDataEnable(enabled);
    }
}

ECode CConnectivityService::SetPolicyDataEnable(
    /* [in] */ Int32 networkType,
    /* [in] */ Boolean enabled)
{
    // only someone like NPMS should only be calling us
    FAIL_RETURN(mContext->EnforceCallingOrSelfPermission(
            Elastos::Droid::Manifest::permission::MANAGE_NETWORK_POLICY, TAG));

    AutoPtr<IMessage> msg;
    mHandler->ObtainMessage(EVENT_SET_POLICY_DATA_ENABLE,
        networkType, (enabled ? ENABLED : DISABLED), (IMessage**)&msg);
    Boolean result;
    return mHandler->SendMessage(msg, &result);
}

void CConnectivityService::HandleSetPolicyDataEnable(
    /* [in] */ Int32 networkType,
    /* [in] */ Boolean enabled)
{
    AutoPtr<IConnectivityManagerHelper> helper;
    CConnectivityManagerHelper::AcquireSingleton((IConnectivityManagerHelper**)&helper);
    Boolean isValid = FALSE;
    if ((helper->IsNetworkTypeValid(networkType, &isValid), isValid)) {
        AutoPtr<INetworkStateTracker> tracker = (*mNetTrackers)[networkType];
        if (tracker != NULL) {
            tracker->SetPolicyDataEnable(enabled);
        }
    }
}

ECode CConnectivityService::EnforceAccessPermission()
{
    return mContext->EnforceCallingOrSelfPermission(
            Elastos::Droid::Manifest::permission::ACCESS_NETWORK_STATE,
            String("ConnectivityService"));
}

ECode CConnectivityService::EnforceChangePermission()
{
    return mContext->EnforceCallingOrSelfPermission(
            Elastos::Droid::Manifest::permission::CHANGE_NETWORK_STATE,
            String("ConnectivityService"));
}

// TODO Make this a special check when it goes public
ECode CConnectivityService::EnforceTetherChangePermission()
{
    return mContext->EnforceCallingOrSelfPermission(
            Elastos::Droid::Manifest::permission::CHANGE_NETWORK_STATE,
            String("ConnectivityService"));
}

ECode CConnectivityService::EnforceTetherAccessPermission()
{
    return mContext->EnforceCallingOrSelfPermission(
            Elastos::Droid::Manifest::permission::ACCESS_NETWORK_STATE,
            String("ConnectivityService"));
}

ECode CConnectivityService::EnforceConnectivityInternalPermission()
{
    return mContext->EnforceCallingOrSelfPermission(
            Elastos::Droid::Manifest::permission::CONNECTIVITY_INTERNAL,
            String("ConnectivityService"));
}

/**
 * Handle a {@code DISCONNECTED} event. If this pertains to the non-active
 * network, we ignore it. If it is for the active network, we send out a
 * broadcast. But first, we check whether it might be possible to connect
 * to a different network.
 * @param info the {@code NetworkInfo} for the network
 */
void CConnectivityService::HandleDisconnect(
    /* [in] */ INetworkInfo* info)
{
    Int32 prevNetType;
    info->GetType(&prevNetType);

    (*mNetTrackers)[prevNetType]->SetTeardownRequested(FALSE);

    // Remove idletimer previously setup in {@code handleConnect}
    RemoveDataActivityTracking(prevNetType);

    /*
     * If the disconnected network is not the active one, then don't report
     * this as a loss of connectivity. What probably happened is that we're
     * getting the disconnect for a network that we explicitly disabled
     * in accordance with network preference policies.
     */
    Boolean isdefault = TRUE;
    if (((*mNetConfigs)[prevNetType]->IsDefault(&isdefault), !isdefault)) {
        AutoPtr<List<Int32> > pids = (*mNetRequestersPids)[prevNetType];
        List<Int32>::Iterator it;
        for (it = pids->Begin(); it != pids->End(); ++it){
            ReassessPidDns(*it, FALSE);
        }
    }

    AutoPtr<IIntent> intent;
    CIntent::New(IConnectivityManager::CONNECTIVITY_ACTION, (IIntent**)&intent);
    AutoPtr<INetworkInfo> newInfo;
    CNetworkInfo::New(info, (INetworkInfo**)&newInfo);
    intent->PutExtra(IConnectivityManager::EXTRA_NETWORK_INFO, IParcelable::Probe(newInfo));
    Int32 nowNetType;
    info->GetType(&nowNetType);
    intent->PutExtra(IConnectivityManager::EXTRA_NETWORK_TYPE, nowNetType);

    Boolean fallOver;
    if ((info->IsFailover(&fallOver), fallOver)) {
        intent->PutBooleanExtra(IConnectivityManager::EXTRA_IS_FAILOVER, TRUE);
        info->SetFailover(FALSE);
    }
    String reason;
    if (info->GetReason(&reason), !reason.IsNull()) {
        intent->PutExtra(IConnectivityManager::EXTRA_REASON, reason);
    }
    String extraInfo;
    if (info->GetExtraInfo(&extraInfo), !extraInfo.IsNull()) {
        intent->PutExtra(IConnectivityManager::EXTRA_EXTRA_INFO, extraInfo);
    }

    if (((*mNetConfigs)[prevNetType]->IsDefault(&isdefault), isdefault)) {
        TryFailover(prevNetType);
        if (mActiveDefaultNetwork != -1) {
            AutoPtr<INetworkInfo> switchTo;
            (*mNetTrackers)[mActiveDefaultNetwork]->GetNetworkInfo((INetworkInfo**)&switchTo);
            intent->PutExtra(IConnectivityManager::EXTRA_OTHER_NETWORK_INFO, IParcelable::Probe(switchTo));
        }
        else {
            mDefaultInetConditionPublished = 0; // we're not connected anymore
            intent->PutBooleanExtra(IConnectivityManager::EXTRA_NO_CONNECTIVITY, TRUE);
        }
    }
    intent->PutExtra(IConnectivityManager::EXTRA_INET_CONDITION, mDefaultInetConditionPublished);

    // Reset interface if no other connections are using the same interface
    Boolean doReset = TRUE;
    AutoPtr<ILinkProperties> linkProperties;
    (*mNetTrackers)[prevNetType]->GetLinkProperties((ILinkProperties**)&linkProperties);
    if (linkProperties != NULL) {
        String oldIface;
        linkProperties->GetInterfaceName(&oldIface);
        if (TextUtils::IsEmpty(oldIface) == FALSE) {
            for (Int32 i = 0; i < mNetTrackers->GetLength(); i++) {
                AutoPtr<INetworkStateTracker> networkStateTracker = (*mNetTrackers)[i];
                if (networkStateTracker == NULL) {
                    continue;
                }
                AutoPtr<INetworkInfo> networkInfo;
                networkStateTracker->GetNetworkInfo((INetworkInfo**)&networkInfo);
                Boolean connected;
                networkInfo->IsConnected(&connected);
                Int32 type;
                networkInfo->GetType(&type);
                if (connected && type != prevNetType) {
                    AutoPtr<ILinkProperties> l;
                    networkStateTracker->GetLinkProperties((ILinkProperties**)&l);
                    if (l == NULL) continue;
                    String ifName;
                    l->GetInterfaceName(&ifName);
                    if (oldIface.Equals(ifName)) {
                        doReset = FALSE;
                        break;
                    }
                }
            }
        }
    }

    // do this before we broadcast the change
    HandleConnectivityChange(prevNetType, doReset);

    AutoPtr<IIntent> immediateIntent;
    CIntent::New(intent, (IIntent**)&immediateIntent);
    immediateIntent->SetAction(IConnectivityManager::CONNECTIVITY_ACTION_IMMEDIATE);
    SendStickyBroadcast(immediateIntent);
    SendStickyBroadcastDelayed(intent, GetConnectivityChangeDelay());
    /*
     * If the failover network is already connected, then immediately send
     * out a followup broadcast indicating successful failover
     */
    if (mActiveDefaultNetwork != -1) {
        AutoPtr<INetworkInfo> netinfo;
        (*mNetTrackers)[mActiveDefaultNetwork]->GetNetworkInfo((INetworkInfo**)&netinfo);
        SendConnectedBroadcastDelayed(netinfo, GetConnectivityChangeDelay());
    }

    //For popocloud, when network is disconnected, set net.state 0.
    property_set("net.state", "0"); //popobox
}

void CConnectivityService::TryFailover(
    /* [in] */ Int32 prevNetType)
{
    /*
     * If this is a default network, check if other defaults are available.
     * Try to reconnect on all available and let them hash it out when
     * more than one connects.
     */
    Boolean isdefault;
    if (((*mNetConfigs)[prevNetType]->IsDefault(&isdefault), isdefault)) {
        if (mActiveDefaultNetwork == prevNetType) {
            mActiveDefaultNetwork = -1;
        }

        // don't signal a reconnect for anything lower or equal priority than our
        // current connected default
        // TODO - don't filter by priority now - nice optimization but risky
        //int currentPriority = -1;
        //if (mActiveDefaultNetwork != -1) {
        //    currentPriority = mNetConfigs[mActiveDefaultNetwork].mPriority;
        //}
        for (Int32 checkType = 0; checkType <= IConnectivityManager::MAX_NETWORK_TYPE; checkType++) {
            if (checkType == prevNetType) continue;
            if ((*mNetConfigs)[checkType] == NULL) continue;
            if ((*mNetConfigs)[checkType]->IsDefault(&isdefault), !isdefault) continue;
            if ((*mNetTrackers)[checkType] == NULL) continue;

// Enabling the isAvailable() optimization caused mobile to not get
// selected if it was in the middle of error handling. Specifically
// a moble connection that took 30 seconds to complete the DEACTIVATE_DATA_CALL
// would not be available and we wouldn't get connected to anything.
// So removing the isAvailable() optimization below for now. TODO: This
// optimization should work and we need to investigate why it doesn't work.
// This could be related to how DEACTIVATE_DATA_CALL is reporting its
// complete before it is really complete.
//                if (!mNetTrackers[checkType].isAvailable()) continue;

//                if (currentPriority >= mNetConfigs[checkType].mPriority) continue;

            AutoPtr<INetworkStateTracker> checkTracker = (*mNetTrackers)[checkType];
            AutoPtr<INetworkInfo> checkInfo;
            checkTracker->GetNetworkInfo((INetworkInfo**)&checkInfo);
            Boolean connect, requested;
            if ((checkInfo->IsConnectedOrConnecting(&connect), !connect) ||
                    (checkTracker->IsTeardownRequested(&requested), requested)) {
                checkInfo->SetFailover(TRUE);
                checkTracker->Reconnect(&connect);
            }
            if (DBG) {
               String typeName;
               checkInfo->GetTypeName(&typeName);
               Slogger::D(TAG, "Attempting to switch to %s", typeName.string());
            }
        }
    }
}

ECode CConnectivityService::SendConnectedBroadcast(
    /* [in] */ INetworkInfo* info)
{
    SendGeneralBroadcast(info, IConnectivityManager::CONNECTIVITY_ACTION_IMMEDIATE);
    SendGeneralBroadcast(info, IConnectivityManager::CONNECTIVITY_ACTION);
    return NOERROR;
}

void CConnectivityService::SendConnectedBroadcastDelayed(
    /* [in] */ INetworkInfo* info,
    /* [in] */ Int32 delayMs)
{
    SendGeneralBroadcast(info, IConnectivityManager::CONNECTIVITY_ACTION_IMMEDIATE);
    SendGeneralBroadcastDelayed(info, IConnectivityManager::CONNECTIVITY_ACTION, delayMs);
}

void CConnectivityService::SendInetConditionBroadcast(
    /* [in] */ INetworkInfo* info)
{
    SendGeneralBroadcast(info, IConnectivityManager::INET_CONDITION_ACTION);
}

AutoPtr<IIntent> CConnectivityService::MakeGeneralIntent(
    /* [in] */ INetworkInfo* info,
    /* [in] */ const String& bcastType)
{
    AutoPtr<INetworkInfo> egressInfo = info;
    if (mLockdownTracker != NULL) {
        egressInfo = mLockdownTracker->AugmentNetworkInfo(egressInfo);
    }

    AutoPtr<IIntent> intent;
    CIntent::New(bcastType, (IIntent**)&intent);
    AutoPtr<INetworkInfo> infoNew;
    CNetworkInfo::New(egressInfo, (INetworkInfo**)&infoNew);
    intent->PutExtra(IConnectivityManager::EXTRA_NETWORK_INFO, IParcelable::Probe(infoNew));
    Int32 type;
    egressInfo->GetType(&type);
    intent->PutExtra(IConnectivityManager::EXTRA_NETWORK_TYPE, type);

    Boolean failover;
    if ((egressInfo->IsFailover(&failover), failover)) {
        intent->PutBooleanExtra(IConnectivityManager::EXTRA_IS_FAILOVER, TRUE);
        egressInfo->SetFailover(FALSE);
    }
    String reason;
    if (egressInfo->GetReason(&reason), !reason.IsNull()) {
        intent->PutExtra(IConnectivityManager::EXTRA_REASON, reason);
    }
    String extraInfo;
    if (egressInfo->GetExtraInfo(&extraInfo), !extraInfo.IsNull()) {
        intent->PutExtra(IConnectivityManager::EXTRA_EXTRA_INFO, extraInfo);
    }
    intent->PutExtra(IConnectivityManager::EXTRA_INET_CONDITION, mDefaultInetConditionPublished);
    return intent;
}

void CConnectivityService::SendGeneralBroadcast(
    /* [in] */ INetworkInfo* info,
    /* [in] */ const String& bcastType)
{
    AutoPtr<IIntent> intent = MakeGeneralIntent(info, bcastType);
    SendStickyBroadcast(intent);
}

void CConnectivityService::SendGeneralBroadcastDelayed(
    /* [in] */ INetworkInfo* info,
    /* [in] */ const String& bcastType,
    /* [in] */ Int32 delayMs)
{
    AutoPtr<IIntent> intent = MakeGeneralIntent(info, bcastType);
    SendStickyBroadcastDelayed(intent, delayMs);
}

void CConnectivityService::SendDataActivityBroadcast(
    /* [in] */ Int32 deviceType,
    /* [in] */ Boolean active)
{
    AutoPtr<IIntent> intent;
    CIntent::New(IConnectivityManager::ACTION_DATA_ACTIVITY_CHANGE, (IIntent**)&intent);
    intent->PutExtra(IConnectivityManager::EXTRA_DEVICE_TYPE, deviceType);
    intent->PutBooleanExtra(IConnectivityManager::EXTRA_IS_ACTIVE, active);
    Int64 ident = Binder::ClearCallingIdentity();
    AutoPtr<IUserHandleHelper> helper;
    CUserHandleHelper::AcquireSingleton((IUserHandleHelper**)&helper);
    AutoPtr<IUserHandle> ALL;
    helper->GetALL((IUserHandle**)&ALL);
    mContext->SendOrderedBroadcastAsUser(intent, ALL,
                Elastos::Droid::Manifest::permission::RECEIVE_DATA_ACTIVITY_CHANGE,
                NULL, NULL, 0, String(NULL), NULL);
    Binder::RestoreCallingIdentity(ident);
}

/**
 * Called when an attempt to fail over to another network has failed.
 * @param info the {@link NetworkInfo} for the failed network
 */
void CConnectivityService::HandleConnectionFailure(
    /* [in] */ INetworkInfo* info)
{
    Int32 type;
    info->GetType(&type);
    (*mNetTrackers)[type]->SetTeardownRequested(FALSE);

    String reason;
    info->GetReason(&reason);
    String extraInfo;
    info->GetExtraInfo(&extraInfo);

    String reasonText;
    if (reason.IsNull()) {
        reasonText = ".";
    }
    else {
        reasonText = " (";
        reasonText += reason;
        reasonText += String(").");
    }
    String typeName;
    info->GetTypeName(&typeName);
    Slogger::E("Attempt to connect to %s failed %s", typeName.string(), reasonText.string());

    AutoPtr<IIntent> intent;
    CIntent::New(IConnectivityManager::CONNECTIVITY_ACTION, (IIntent**)&intent);
    AutoPtr<INetworkInfo> infoNew;
    CNetworkInfo::New(info, (INetworkInfo**)&infoNew);
    intent->PutExtra(IConnectivityManager::EXTRA_NETWORK_INFO, IParcelable::Probe(infoNew));
    Int32 typeNow;
    info->GetType(&typeNow);
    intent->PutExtra(IConnectivityManager::EXTRA_NETWORK_TYPE, typeNow);

    AutoPtr<INetworkInfo> actNetInfo;
    GetActiveNetworkInfo((INetworkInfo**)&actNetInfo);
    if (actNetInfo == NULL) {
        intent->PutBooleanExtra(IConnectivityManager::EXTRA_NO_CONNECTIVITY, TRUE);
    }
    if (!reason.IsNull()) {
        intent->PutExtra(IConnectivityManager::EXTRA_REASON, reason);
    }
    if (!extraInfo.IsNull()) {
        intent->PutExtra(IConnectivityManager::EXTRA_EXTRA_INFO, extraInfo);
    }
    Boolean failOver;
    if ((info->IsFailover(&failOver), failOver)) {
        intent->PutBooleanExtra(IConnectivityManager::EXTRA_IS_FAILOVER, TRUE);
        info->SetFailover(FALSE);
    }

    Int32 typeNow2;
    info->GetType(&typeNow2);
    Boolean isDefault;
    if ((*mNetConfigs)[typeNow2]->IsDefault(&isDefault), isDefault) {
        TryFailover(typeNow2);
        if (mActiveDefaultNetwork != -1) {
            AutoPtr<INetworkInfo> switchTo;
            (*mNetTrackers)[mActiveDefaultNetwork]->GetNetworkInfo((INetworkInfo**)&switchTo);
            intent->PutExtra(IConnectivityManager::EXTRA_OTHER_NETWORK_INFO, IParcelable::Probe(switchTo));
        }
        else {
            mDefaultInetConditionPublished = 0;
            intent->PutBooleanExtra(IConnectivityManager::EXTRA_NO_CONNECTIVITY, TRUE);
        }
    }

    intent->PutExtra(IConnectivityManager::EXTRA_INET_CONDITION, mDefaultInetConditionPublished);

    AutoPtr<IIntent> immediateIntent;
    CIntent::New(intent, (IIntent**)&immediateIntent);
    immediateIntent->SetAction(IConnectivityManager::CONNECTIVITY_ACTION_IMMEDIATE);
    SendStickyBroadcast(immediateIntent);
    SendStickyBroadcast(intent);
    /*
     * If the failover network is already connected, then immediately send
     * out a followup broadcast indicating successful failover
     */
    if (mActiveDefaultNetwork != -1) {
        AutoPtr<INetworkInfo> netInfo;
        (*mNetTrackers)[mActiveDefaultNetwork]->GetNetworkInfo((INetworkInfo**)&netInfo);
        SendConnectedBroadcast(netInfo);
    }
}

void CConnectivityService::SendStickyBroadcast(
    /* [in] */ IIntent* intent)
{
    AutoLock lock(this);
    if (!mSystemReady) {
        mInitialBroadcast = NULL;
        CIntent::New(intent, (IIntent**)&mInitialBroadcast);
    }
    intent->AddFlags(IIntent::FLAG_RECEIVER_REGISTERED_ONLY_BEFORE_BOOT);
    if (VDBG) {
       String action;
       intent->GetAction(&action);
       Slogger::V(TAG, "sendStickyBroadcast: action=%s", action.string());
    }

    Int64 ident = Binder::ClearCallingIdentity();
    AutoPtr<IUserHandleHelper> helper;
    CUserHandleHelper::AcquireSingleton((IUserHandleHelper**)&helper);
    AutoPtr<IUserHandle> ALL;
    helper->GetALL((IUserHandle**)&ALL);
    mContext->SendStickyBroadcastAsUser(intent, ALL);
    Binder::RestoreCallingIdentity(ident);
}

void CConnectivityService::SendStickyBroadcastDelayed(
    /* [in] */ IIntent* intent,
    /* [in] */ Int32 delayMs)
{
    if (delayMs <= 0) {
        SendStickyBroadcast(intent);
    }
    else {
        if (VDBG) {
           String action;
           intent->GetAction(&action);
           Slogger::V(TAG, "sendStickyBroadcastDelayed: delayMs=%d, action=%s", delayMs, action.string());
        }

        AutoPtr<IMessage> msg;
        mHandler->ObtainMessage(EVENT_SEND_STICKY_BROADCAST_INTENT,
            intent, (IMessage**)&msg);
        Boolean result;
        mHandler->SendMessageDelayed(msg, delayMs, &result);
    }
}

void CConnectivityService::SystemReady()
{
    {
        AutoLock lock(this);
        mSystemReady = TRUE;
        if (mInitialBroadcast != NULL) {
            AutoPtr<IUserHandleHelper> helper;
            CUserHandleHelper::AcquireSingleton((IUserHandleHelper**)&helper);
            AutoPtr<IUserHandle> ALL;
            helper->GetALL((IUserHandle**)&ALL);
            mContext->SendStickyBroadcastAsUser(mInitialBroadcast, ALL);
            mInitialBroadcast = NULL;
        }
    }
    // load the global proxy at startup
    //send EVENT_APPLY_GLOBAL_HTTP_PROXY
    Boolean result;
    mHandler->SendEmptyMessage(EVENT_APPLY_GLOBAL_HTTP_PROXY, &result);

    // Try bringing up tracker, but if KeyStore isn't ready yet, wait
    // for user to unlock device.
    Boolean update;
    if ((UpdateLockdownVpn(&update), !update)) {
        AutoPtr<IIntentFilter> filter;
        CIntentFilter::New(IIntent::ACTION_USER_PRESENT, (IIntentFilter**)&filter);
        AutoPtr<IIntent> intent;
        mContext->RegisterReceiver(mUserPresentReceiver, filter, (IIntent**)&intent);
    }
}

Boolean CConnectivityService::IsNewNetTypePreferredOverCurrentNetType(
    /* [in] */ Int32 type)
{
    Int32 prop1, prop2;
    (*mNetConfigs)[mActiveDefaultNetwork]->GetPriority(&prop1);
    (*mNetConfigs)[type]->GetPriority(&prop2);
    if ((type != mNetworkPreference && prop1 > prop2) ||
            mNetworkPreference == mActiveDefaultNetwork) {
        return FALSE;
    }
    return TRUE;
}

void CConnectivityService::HandleConnect(
    /* [in] */ INetworkInfo* info)
{
    Int32 newNetType;
    info->GetType(&newNetType);
    SetupDataActivityTracking(newNetType);

    // snapshot isFailover, because sendConnectedBroadcast() resets it
    Boolean isFailover;
    info->IsFailover(&isFailover);
    AutoPtr<INetworkStateTracker> thisNet = (*mNetTrackers)[newNetType];
    AutoPtr<ILinkProperties> properties;
    thisNet->GetLinkProperties((ILinkProperties**)&properties);
    String thisIface;
    properties->GetInterfaceName(&thisIface);

    // if this is a default net and other default is running
    // kill the one not preferred
    Boolean isDefault;
    (*mNetConfigs)[newNetType]->IsDefault(&isDefault);
    if (isDefault) {
        if (mActiveDefaultNetwork != -1 && mActiveDefaultNetwork != newNetType) {
            if (IsNewNetTypePreferredOverCurrentNetType(newNetType)) {
                // tear down the other
                AutoPtr<INetworkStateTracker> otherNet =
                        (*mNetTrackers)[mActiveDefaultNetwork];
                if (DBG) {
                    AutoPtr<INetworkInfo> otherNetInfo;
                    otherNet->GetNetworkInfo((INetworkInfo**)&otherNetInfo);
                    String otherNetTypeName;
                    otherNetInfo->GetTypeName(&otherNetTypeName);
                    Slogger::D(TAG, "Policy requires %s teardown", otherNetTypeName.string());
                }
                if (!Teardown(otherNet)) {
                    Slogger::E(TAG, "Network declined teardown request");
                    Teardown(thisNet);
                    return;
                }
            }
            else {
                // don't accept this one
                if (VDBG) {
                    String typeName;
                    info->GetTypeName(&typeName);
                    Slogger::V(TAG, "Not broadcasting CONNECT_ACTION \
                            to torn down network %s", typeName.string());
                }
                Teardown(thisNet);
                return;
            }
        }

        {
            AutoLock lock(this);
            // have a new default network, release the transition wakelock in a second
            // if it's held.  The second pause is to allow apps to reconnect over the
            // new network
            Boolean isHeld;
            if ((mNetTransitionWakeLock->IsHeld(&isHeld), isHeld)) {
                AutoPtr<IMessage> msg;
                mHandler->ObtainMessage(EVENT_CLEAR_NET_TRANSITION_WAKELOCK,
                    mNetTransitionWakeLockSerialNumber, 0, (IMessage**)&msg);
                Boolean result;
                mHandler->SendMessageDelayed(msg, 1000, &result);
            }
        }
        mActiveDefaultNetwork = newNetType;
        // this will cause us to come up initially as unconnected and switching
        // to connected after our normal pause unless somebody reports us as reall
        // disconnected
        mDefaultInetConditionPublished = 0;
        mDefaultConnectionSequence++;
        mInetConditionChangeInFlight = FALSE;
        // Don't do this - if we never sign in stay, grey
        //reportNetworkCondition(mActiveDefaultNetwork, 100);
    }

    thisNet->SetTeardownRequested(FALSE);
    UpdateNetworkSettings(thisNet);
    HandleConnectivityChange(newNetType, FALSE);
    SendConnectedBroadcastDelayed(info, GetConnectivityChangeDelay());

    // notify battery stats service about this network
    if (!thisIface.IsNull()) {
        //try {
        CBatteryStatsService::GetService()->NoteNetworkInterfaceType(thisIface, newNetType);
        //} catch (RemoteException e) {
            // ignored; service lives in system_server
        //}
    }

    //For popocloud, when network is connected, set net.state 1.
    property_set("net.state", "1"); //popobox
}

void CConnectivityService::HandleCaptivePortalTrackerCheck(
    /* [in] */ INetworkInfo* info)
{
    if (DBG) Slogger::D(TAG, "Captive portal check %p", info);
    Int32 type;
    info->GetType(&type);
    AutoPtr<INetworkStateTracker> thisNet = (*mNetTrackers)[type];
    Boolean isDefault;
    if (((*mNetConfigs)[type]->IsDefault(&isDefault), isDefault)) {
        if (mActiveDefaultNetwork != -1 && mActiveDefaultNetwork != type) {
            if (IsNewNetTypePreferredOverCurrentNetType(type)) {
                if (DBG) {
                   String typeName;
                   info->GetTypeName(&typeName);
                   Slogger::D(TAG, "Captive check on %s", typeName.string());
                }
                AutoPtr<INetworkInfo> infoNew;
                CNetworkInfo::New(info, (INetworkInfo**)&infoNew);
                mCaptivePortalTracker->DetectCaptivePortal(infoNew);
                return;
            }
            else {
                if (DBG) {
                   String typeName;
                   info->GetTypeName(&typeName);
                   Slogger::D(TAG, "Tear down low priority net %s", typeName.string());
                }
                Teardown(thisNet);
                return;
            }
        }
    }

    thisNet->CaptivePortalCheckComplete();
}

/** @hide */
ECode CConnectivityService::CaptivePortalCheckComplete(
    /* [in] */ INetworkInfo* info)
{
    Int32 type;
    info->GetType(&type);
    return (*mNetTrackers)[type]->CaptivePortalCheckComplete();
}

/**
 * Setup data activity tracking for the given network interface.
 *
 * Every {@code setupDataActivityTracking} should be paired with a
 * {@link removeDataActivityTracking} for cleanup.
 */
void CConnectivityService::SetupDataActivityTracking(
    /* [in] */ Int32 type)
{
    AutoPtr<INetworkStateTracker> thisNet = (*mNetTrackers)[type];
    AutoPtr<ILinkProperties> properties;
    thisNet->GetLinkProperties((ILinkProperties**)&properties);
    String iface;
    properties->GetInterfaceName(&iface);

    Int32 timeout;

    AutoPtr<IContentResolver> resolver;
    mContext->GetContentResolver((IContentResolver**)&resolver);
    AutoPtr<IConnectivityManagerHelper> helper;
    CConnectivityManagerHelper::AcquireSingleton((IConnectivityManagerHelper**)&helper);
    Boolean isTypeMobile;
    if ((helper->IsNetworkTypeMobile(type, &isTypeMobile), isTypeMobile)) {
        AutoPtr<ISettingsGlobal> settingsGlobal;
        CSettingsGlobal::AcquireSingleton((ISettingsGlobal**)&settingsGlobal);
        settingsGlobal->GetInt32(resolver, ISettingsGlobal::DATA_ACTIVITY_TIMEOUT_MOBILE, 0, &timeout);
        // Canonicalize mobile network type
        type = IConnectivityManager::TYPE_MOBILE;
    }
    else if (IConnectivityManager::TYPE_WIFI == type) {
        AutoPtr<ISettingsGlobal> settingsGlobal;
        CSettingsGlobal::AcquireSingleton((ISettingsGlobal**)&settingsGlobal);
        settingsGlobal->GetInt32(resolver, ISettingsGlobal::DATA_ACTIVITY_TIMEOUT_WIFI, 0, &timeout);
    }
    else {
        // do not track any other networks
        timeout = 0;
    }

    if (timeout > 0 && !iface.IsNull()) {
        mNetd->AddIdleTimer(iface, timeout, StringUtils::Int32ToString(type));
    }
}

/**
 * Remove data activity tracking when network disconnects.
 */
void CConnectivityService::RemoveDataActivityTracking(
    /* [in] */ Int32 type)
{
    AutoPtr<INetworkStateTracker> net = (*mNetTrackers)[type];
    AutoPtr<ILinkProperties> properties;
    net->GetLinkProperties((ILinkProperties**)&properties);
    String iface;
    properties->GetInterfaceName(&iface);

    AutoPtr<IConnectivityManagerHelper> helper;
    CConnectivityManagerHelper::AcquireSingleton((IConnectivityManagerHelper**)&helper);
    Boolean isTypeMobile = FALSE;
    if (!iface.IsNull() &&
        ((helper->IsNetworkTypeMobile(type, &isTypeMobile), isTypeMobile) ||
            IConnectivityManager::TYPE_WIFI == type)) {
            // the call fails silently if no idletimer setup for this interface
            mNetd->RemoveIdleTimer(iface);
    }
}

/**
 * After a change in the connectivity state of a network. We're mainly
 * concerned with making sure that the list of DNS servers is set up
 * according to which networks are connected, and ensuring that the
 * right routing table entries exist.
 */
void CConnectivityService::HandleConnectivityChange(
    /* [in] */ Int32 netType,
    /* [in] */ Boolean doReset)
{
    Int32 resetMask = doReset ? NetworkUtils::RESET_ALL_ADDRESSES : 0;
    AutoPtr<INetworkStateTracker> tracker = (*mNetTrackers)[netType];
    /*
     * If a non-default network is enabled, add the host routes that
     * will allow it's DNS servers to be accessed.
     */
    HandleDnsConfigurationChange(netType);

    AutoPtr<ILinkProperties> curLp = (*mCurrentLinkProperties)[netType];
    AutoPtr<ILinkProperties> newLp;

    AutoPtr<INetworkInfo> info;
    tracker->GetNetworkInfo((INetworkInfo**)&info);
    Boolean isConnected = FALSE;
    info->IsConnected(&isConnected);
    if (isConnected) {
        tracker->GetLinkProperties((ILinkProperties**)&newLp);
        if (VDBG) {
            String curLpStr, newLpStr;
            if (curLp) curLp->ToString(&curLpStr);
            if (newLp) newLp->ToString(&newLpStr);
            Slogger::V(TAG, "handleConnectivityChange: changed linkProperty[%d]:"
                " doReset=%d resetMask=%d\n    curLp=%s\n    newlp=%s",
                netType, doReset, resetMask, curLpStr.string(), newLpStr.string());
        }

        if (curLp != NULL) {
            Boolean isIdentical = FALSE;
            curLp->IsIdenticalInterfaceName(newLp, &isIdentical);
            if (isIdentical) {
                AutoPtr<ICompareResult> car;
                curLp->CompareAddresses(newLp, (ICompareResult**)&car);
                AutoPtr<IObjectContainer> objCtnRemoved;
                car->GetRemoved((IObjectContainer**)&objCtnRemoved);
                AutoPtr<IObjectContainer> objCtnAdded;
                car->GetAdded((IObjectContainer**)&objCtnAdded);
                Int32 sizeRemoved, sizeAdded;
                objCtnRemoved->GetObjectCount(&sizeRemoved);
                objCtnAdded->GetObjectCount(&sizeAdded);
                if (VDBG) Slogger::V(TAG, "sizeRemoved %d sizeAdded %d", sizeRemoved, sizeAdded);

                if ((sizeRemoved != 0) || (sizeAdded != 0)) {
                    AutoPtr<IObjectEnumerator> objEmu;
                    objCtnRemoved->GetObjectEnumerator((IObjectEnumerator**)&objEmu);
                    Boolean hasNext;
                    while (objEmu->MoveNext(&hasNext), hasNext) {
                        AutoPtr<IInterface> obj;
                        objEmu->Current((IInterface**)&obj);
                        ILinkAddress* linkAddr = ILinkAddress::Probe(obj);
                        AutoPtr<IInetAddress> netAddr;
                        linkAddr->GetAddress((IInetAddress**)&netAddr);
                        if (IInet4Address::Probe(netAddr) != NULL) {
                            resetMask |= NetworkUtils::RESET_IPV4_ADDRESSES;
                        }
                        if (IInet6Address::Probe(netAddr) != NULL) {
                            resetMask |= NetworkUtils::RESET_IPV6_ADDRESSES;
                        }
                    }

                    if (VDBG) {
                        String rstr;
                        if (car) car->ToString(&rstr);
                        Slogger::V(TAG, "handleConnectivityChange: addresses changed"
                            " linkProperty[%d]: resetMask=%d\n   car=%s",  netType, resetMask, rstr.string());
                    }
                }
                else {
                    if (VDBG) {
                       Slogger::V(TAG, "handleConnectivityChange: address are the same reset per doReset"
                            " linkProperty[%d]: resetMask=%d",  netType, resetMask);
                    }
                }
            }
            else {
                resetMask = NetworkUtils::RESET_ALL_ADDRESSES;
                if (VDBG) {
                   Slogger::V(TAG, "handleConnectivityChange: interface not not equivalent reset both"
                       " linkProperty[%d]: resetMask=%d",  netType, resetMask);
                }
            }
        }

        Boolean isDefault = FALSE;
        (*mNetConfigs)[netType]->IsDefault(&isDefault);
        if (isDefault) {
            AutoPtr<IProxyProperties> proxyProperties;
            newLp->GetHttpProxy((IProxyProperties**)&proxyProperties);
            HandleApplyDefaultProxy(proxyProperties);
        }
    }
    else {
        if (VDBG) {
            String curLpStr;
            if (curLp) curLp->ToString(&curLpStr);
            Slogger::V(TAG, "handleConnectivityChange: changed linkProperty[%d]:"
                " doReset=%d resetMask=%d\n    curLp=%s\n    newlp=NULL",
                netType, doReset, resetMask, curLpStr.string());
        }
    }

    mCurrentLinkProperties->Set(netType, newLp);

    Boolean isDefault1;
    (*mNetConfigs)[netType]->IsDefault(&isDefault1);
    Boolean resetDns = FALSE;
    resetDns = UpdateRoutes(newLp, curLp, isDefault1);

    if (resetMask != 0 || resetDns) {
        AutoPtr<ILinkProperties> linkProperties;
        tracker->GetLinkProperties((ILinkProperties**)&linkProperties);
        if (linkProperties != NULL) {
            String iface;
            linkProperties->GetInterfaceName(&iface);
            if (TextUtils::IsEmpty(iface) == FALSE) {
                if (resetMask != 0) {
                    if (VDBG) Slogger::V(TAG, "resetConnections(%s, %d)", iface.string(), resetMask);
                    NetworkUtils::ResetConnections(iface, resetMask);

                    // Tell VPN the interface is down. It is a temporary
                    // but effective fix to make VPN aware of the change.
                    if ((resetMask & NetworkUtils::RESET_IPV4_ADDRESSES) != 0) {
                        mVpn->InterfaceStatusChanged(iface, FALSE);
                    }
                }
                if (resetDns) {
                   if (VDBG) Slogger::V(TAG, "resetting DNS cache for %s", iface.string());
                    ECode ec = mNetd->FlushInterfaceDnsCache(iface);
                    if (FAILED(ec)) {
                        // never crash - catch them all
                        if (DBG) Slogger::E(TAG, "Exception resetting dns cache: %d", ec);
                    }
                }
            }
        }
    }

    // TODO: Temporary notifying upstread change to Tethering.
    //       @see bug/4455071
    /** Notify TetheringService if interface name has been changed. */
    AutoPtr<INetworkInfo> info2;
    tracker->GetNetworkInfo((INetworkInfo**)&info2);
    String reason;
    info2->GetReason(&reason);
    if (TextUtils::Equals(reason, String("linkPropertiesChanged"/*PhoneConstants::REASON_LINK_PROPERTIES_CHANGED*/))) {
        Boolean supported;
        if (IsTetheringSupported(&supported), supported) {
            mTethering->HandleTetherIfaceChange();
        }
    }
}

/**
 * Add and remove routes using the old properties (NULL if not previously connected),
 * new properties (NULL if becoming disconnected).  May even be double NULL, which
 * is a noop.
 * Uses isLinkDefault to determine if default routes should be set or conversely if
 * host routes should be set to the dns servers
 * returns a boolean indicating the routes changed
 */
Boolean CConnectivityService::UpdateRoutes(
    /* [in] */ ILinkProperties* newLp,
    /* [in] */ ILinkProperties* curLp,
    /* [in] */ Boolean isLinkDefault)
{
    if (VDBG) {
        String curLpStr, newLpStr;
        if (curLp) curLp->ToString(&curLpStr);
        if (newLp) newLp->ToString(&newLpStr);
        Slogger::V(TAG, "CConnectivityService::UpdateRoutes\n    curLp:%s\n    newLp:%s\n    isLinkDefault: %d.",
            curLpStr.string(), newLpStr.string(), isLinkDefault);
    }

    // Collection<RouteInfo> routesToAdd = NULL;
    AutoPtr<ICompareResult> routeDiff;//= new CompareResult<RouteInfo>();
    AutoPtr<ICompareResult> dnsDiff;//= new CompareResult<InetAddress>();
    if (curLp != NULL) {
        // check for the delta between the current set and the new
        curLp->CompareRoutes(newLp, (ICompareResult**)&routeDiff);
        curLp->CompareDnses(newLp, (ICompareResult**)&dnsDiff);
    }
    else if (newLp != NULL) {
        CCompareResult::New((ICompareResult**)&routeDiff);
        CCompareResult::New((ICompareResult**)&dnsDiff);

        AutoPtr<IObjectContainer> routes;
        newLp->GetRoutes((IObjectContainer**)&routes);
        AutoPtr<IObjectEnumerator> routesIt;
        routes->GetObjectEnumerator((IObjectEnumerator**)&routesIt);
        Boolean hasNext;
        while (routesIt->MoveNext(&hasNext), hasNext) {
            AutoPtr<IInterface> obj;
            routesIt->Current((IInterface**)&obj);
            routeDiff->AddAdded(obj);
        }

        AutoPtr<IObjectContainer> dnses;
        newLp->GetDnses((IObjectContainer**)&dnses);
        AutoPtr<IObjectEnumerator> dnsesIt;
        dnses->GetObjectEnumerator((IObjectEnumerator**)&dnsesIt);
        while (dnsesIt->MoveNext(&hasNext), hasNext) {
            AutoPtr<IInterface> obj;
            dnsesIt->Current((IInterface**)&obj);
            dnsDiff->AddAdded(obj);
        }
    }
    else {
        CCompareResult::New((ICompareResult**)&routeDiff);
        CCompareResult::New((ICompareResult**)&dnsDiff);
    }

    Int32 sizeDiffRmvd = 0;
    AutoPtr<IObjectContainer> routeDiffRmvd;
    routeDiff->GetRemoved((IObjectContainer**)&routeDiffRmvd);
    routeDiffRmvd->GetObjectCount(&sizeDiffRmvd);
    Int32 sizeDiffAdded = 0;
    AutoPtr<IObjectContainer> routeDiffAdded;
    routeDiff->GetAdded((IObjectContainer**)&routeDiffAdded);
    routeDiffAdded->GetObjectCount(&sizeDiffAdded);
    Boolean routesChanged = ((sizeDiffRmvd != 0) ||(sizeDiffAdded != 0));

    if (!routesChanged) {
        if (VDBG) Slogger::V(TAG, "renew ip, routesChanged is false.");
        if (newLp != NULL) {
            if (VDBG) Slogger::V(TAG, "renew ip, newLp != null ");
            AutoPtr<IObjectContainer> routes, dnses;
            newLp->GetRoutes((IObjectContainer**)&routes);
            AutoPtr<IObjectEnumerator> routesIt;
            routes->GetObjectEnumerator((IObjectEnumerator**)&routesIt);
            Boolean hasNext;
            while (routesIt->MoveNext(&hasNext), hasNext) {
                AutoPtr<IInterface> obj;
                routesIt->Current((IInterface**)&obj);
                routeDiff->AddAdded(obj);
            }

            newLp->GetDnses((IObjectContainer**)&dnses);
            AutoPtr<IObjectEnumerator> dnsesIt;
            dnses->GetObjectEnumerator((IObjectEnumerator**)&dnsesIt);
            while (dnsesIt->MoveNext(&hasNext), hasNext) {
                AutoPtr<IInterface> obj;
                dnsesIt->Current((IInterface**)&obj);
                dnsDiff->AddAdded(obj);
            }
        }
    }

    AutoPtr<IObjectEnumerator> objEmu1;
    routeDiffRmvd->GetObjectEnumerator((IObjectEnumerator**)&objEmu1);
    Boolean hasNext;
    while (objEmu1->MoveNext(&hasNext), hasNext) {
        AutoPtr<IInterface> obj;
        objEmu1->Current((IInterface**)&obj);
        AutoPtr<IRouteInfo> rInfo = IRouteInfo::Probe(obj);
        Boolean isDefault;
        if (isLinkDefault || (rInfo->IsDefaultRoute(&isDefault), !isDefault)) {
            RemoveRoute(curLp, rInfo, TO_DEFAULT_TABLE);
        }
        if (isLinkDefault == FALSE) {
            // remove from a secondary route table
            RemoveRoute(curLp, rInfo, TO_SECONDARY_TABLE);
        }
    }

    AutoPtr<IObjectEnumerator> objEmu2;
    routeDiffAdded->GetObjectEnumerator((IObjectEnumerator**)&objEmu2);
    while (objEmu2->MoveNext(&hasNext), hasNext) {
        AutoPtr<IInterface> obj;
        objEmu2->Current((IInterface**)&obj);
        AutoPtr<IRouteInfo> rInfo = IRouteInfo::Probe(obj);
        Boolean isDefault;
        if (isLinkDefault || (rInfo->IsDefaultRoute(&isDefault), !isDefault)) {
            AddRoute(newLp, rInfo, TO_DEFAULT_TABLE);
        }
        else {
            // add to a secondary route table
            AddRoute(newLp, rInfo, TO_SECONDARY_TABLE);

            // many radios add a default route even when we don't want one.
            // remove the default route unless somebody else has asked for it
            String ifaceName;
            newLp->GetInterfaceName(&ifaceName);
            Boolean cnts;
            if (TextUtils::IsEmpty(ifaceName) == FALSE &&
                (Find(mAddedRoutes.Begin(), mAddedRoutes.End(), rInfo) == mAddedRoutes.End())) {
                if (VDBG) Slogger::V(TAG, "Removing IRouteInfo %p for interface %s",
                    rInfo.Get(), ifaceName.string());
                ECode ec = mNetd->RemoveRoute(ifaceName, rInfo);
                if (FAILED(ec)) {
                    // never crash - catch them all
                    if (DBG) Slogger::E(TAG, "Exception trying to remove a route: %d", ec);
                }
            }
        }
    }

    if (!isLinkDefault) {
        // handle DNS routes
        if (routesChanged) {
            // routes changed - remove all old dns entries and add new
            if (curLp != NULL) {
                AutoPtr<IObjectContainer> curDnses;
                curLp->GetDnses((IObjectContainer**)&curDnses);
                AutoPtr<IObjectEnumerator> curDnsesIt;
                curDnses->GetObjectEnumerator((IObjectEnumerator**)&curDnsesIt);
                Boolean hasNext;
                while (curDnsesIt->MoveNext(&hasNext), hasNext) {
                    AutoPtr<IInterface> obj;
                    curDnsesIt->Current((IInterface**)&obj);
                    AutoPtr<IInetAddress> oldDns = IInetAddress::Probe(obj);
                    RemoveRouteToAddress(curLp, oldDns);
                }
            }
            if (newLp != NULL) {
                AutoPtr<IObjectContainer> newDnses;
                newLp->GetDnses((IObjectContainer**)&newDnses);
                AutoPtr<IObjectEnumerator> newDnsesIt;
                newDnses->GetObjectEnumerator((IObjectEnumerator**)&newDnsesIt);
                Boolean hasNext;
                while (newDnsesIt->MoveNext(&hasNext), hasNext) {
                    AutoPtr<IInterface> obj;
                    newDnsesIt->Current((IInterface**)&obj);
                    AutoPtr<IInetAddress> newDns = IInetAddress::Probe(obj);
                    AddRouteToAddress(newLp, newDns);
                }
            }
        }
        else {
            AutoPtr<IObjectContainer> dnsDiffRmvd;
            dnsDiff->GetRemoved((IObjectContainer**)&dnsDiffRmvd);
            AutoPtr<IObjectEnumerator> dnsDiffRmvdIt;
            dnsDiffRmvd->GetObjectEnumerator((IObjectEnumerator**)&dnsDiffRmvdIt);
            Boolean hasNext;
            while (dnsDiffRmvdIt->MoveNext(&hasNext), hasNext) {
                AutoPtr<IInterface> obj;
                dnsDiffRmvdIt->Current((IInterface**)&obj);
                AutoPtr<IInetAddress> oldDns = IInetAddress::Probe(obj);
                RemoveRouteToAddress(curLp, oldDns);
            }
            AutoPtr<IObjectContainer> dnsDiffAdded;
            dnsDiff->GetRemoved((IObjectContainer**)&dnsDiffAdded);
            AutoPtr<IObjectEnumerator> dnsDiffAddedIt;
            dnsDiffAdded->GetObjectEnumerator((IObjectEnumerator**)&dnsDiffAddedIt);
            while (dnsDiffAddedIt->MoveNext(&hasNext), hasNext) {
                AutoPtr<IInterface> obj;
                dnsDiffAddedIt->Current((IInterface**)&obj);
                AutoPtr<IInetAddress> newDns = IInetAddress::Probe(obj);
                AddRouteToAddress(newLp, newDns);
            }
        }
    }
    return routesChanged;
}

/**
* Reads the network specific TCP buffer sizes from SystemProperties
* net.tcp.buffersize.[default|wifi|umts|edge|gprs] and set them for system
* wide use
*/
ECode CConnectivityService::UpdateNetworkSettings(
    /* [in] */ INetworkStateTracker* nt)
{
    String key;
    nt->GetTcpBufferSizesPropName(&key);
    String bufferSizes;
    if (!key.IsNull()) {
        AutoPtr<ISystemProperties> sysProp;
        CSystemProperties::AcquireSingleton((ISystemProperties**)&sysProp);
        sysProp->Get(key, &bufferSizes);
    }

    if (TextUtils::IsEmpty(bufferSizes)) {
        if (VDBG) Slogger::V(TAG, "%s not found in system properties. Using defaults", key.string());

        // Setting to default values so we won't be stuck to previous values
        key = "net.tcp.buffersize.default";
        AutoPtr<ISystemProperties> sysProp;
        CSystemProperties::AcquireSingleton((ISystemProperties**)&sysProp);
        sysProp->Get(key, &bufferSizes);
    }

    // Set values in kernel
    if (!bufferSizes.IsNullOrEmpty()) {
        if (VDBG) {
           Slogger::V(TAG, "Setting TCP values: [%s] which comes from [%s]",
                    bufferSizes.string(), key.string());
        }
        SetBufferSize(bufferSizes);
    }
    return NOERROR;
}

/**
 * Writes TCP buffer sizes to /sys/kernel/ipv4/tcp_[r/w]mem_[min/def/max]
 * which maps to /proc/sys/net/ipv4/tcp_rmem and tcpwmem
 *
 * @param bufferSizes in the format of "readMin, readInitial, readMax,
 *        writeMin, writeInitial, writeMax"
 */
void CConnectivityService::SetBufferSize(
    /* [in] */ const String& bufferSizes)
{
    AutoPtr< ArrayOf<String> > values;
    StringUtils::Split(bufferSizes, String(","), (ArrayOf<String>**)&values);

    if (values->GetLength() == 6) {
        String prefix("/sys/kernel/ipv4/tcp_");
        FileUtils::StringToFile(prefix + String("rmem_min"), (*values)[0]);
        FileUtils::StringToFile(prefix + String("rmem_def"), (*values)[1]);
        FileUtils::StringToFile(prefix + String("rmem_max"), (*values)[2]);
        FileUtils::StringToFile(prefix + String("wmem_min"), (*values)[3]);
        FileUtils::StringToFile(prefix + String("wmem_def"), (*values)[4]);
        FileUtils::StringToFile(prefix + String("wmem_max"), (*values)[5]);
    }
    else {
//            loge("Invalid buffersize string: " + bufferSizes);
    }
}

/**
 * Adjust the per-process dns entries (net.dns<x>.<pid>) based
 * on the highest priority active net which this process requested.
 * If there aren't any, clear it out
 */
void CConnectivityService::ReassessPidDns(
    /* [in] */ Int32 myPid,
    /* [in] */ Boolean doBump)
{
    if (VDBG) Slogger::V(TAG, "reassessPidDns for pid %d", myPid);
    for (Int32 i = 0; i < mPriorityList->GetLength(); i++) {
        Boolean isDefault = FALSE;
        if (((*mNetConfigs)[i]->IsDefault(&isDefault), isDefault)) {
            continue;
        }
        AutoPtr<INetworkStateTracker> nt = (*mNetTrackers)[i];
        AutoPtr<INetworkInfo> info;
        nt->GetNetworkInfo((INetworkInfo**)&info);
        Boolean result;
        if ((info->IsConnected(&result), result) && (nt->IsTeardownRequested(&result), !result)) {
            AutoPtr<ILinkProperties> p;
            nt->GetLinkProperties((ILinkProperties**)&p);
            if (p == NULL) continue;
            AutoPtr<List<Int32> > pids = (*mNetRequestersPids)[i];
            List<Int32>::Iterator it;
            for (it = pids->Begin(); it != pids->End(); ++it) {
                if (*it == myPid) {
                    AutoPtr<IObjectContainer> dnses;
                    p->GetDnses((IObjectContainer**)&dnses);
                    WritePidDns(dnses, myPid);
                    if (doBump) {
                        BumpDns();
                    }
                    return;
                }
            }
       }
    }
    // nothing found - delete
    for (Int32 i = 1; ; i++) {
        String prop("net.dns");
        prop += StringUtils::ToString(i);
        prop += String(".");
        prop += StringUtils::ToString(myPid);
        AutoPtr<ISystemProperties> sysProp;
        CSystemProperties::AcquireSingleton((ISystemProperties**)&sysProp);
        String value;
        if (sysProp->Get(prop, &value), value.IsNullOrEmpty()) {
            if (doBump) {
                BumpDns();
            }
            return;
        }
        sysProp->Set(prop, String(""));
    }
}

Boolean CConnectivityService::WritePidDns(
    /* [in] */ IObjectContainer* dnses,
    /* [in] */ Int32 pid)
{
    Int32 j = 1;
    Boolean changed = FALSE;

    AutoPtr<IObjectEnumerator> dnsesIt;
    dnses->GetObjectEnumerator((IObjectEnumerator**)&dnsesIt);
    Boolean hasNext;
    while(dnsesIt->MoveNext(&hasNext), hasNext) {
        AutoPtr<IInterface> obj;
        dnsesIt->Current((IInterface**)&obj);
        AutoPtr<IInetAddress> dns = IInetAddress::Probe(obj);
        String dnsString;
        dns->GetHostAddress(&dnsString);
        String prop("net.dns");
        prop += StringUtils::ToString(j);
        prop += String(".");
        prop += StringUtils::ToString(pid);
        AutoPtr<ISystemProperties> sysProp;
        CSystemProperties::AcquireSingleton((ISystemProperties**)&sysProp);
        String value;
        sysProp->Get(prop, &value);
        if (changed || !dnsString.Equals(value)) {
            changed = TRUE;
            sysProp->Set(prop, dnsString);
        }
        j++;
    }
    return changed;
}

void CConnectivityService::BumpDns()
{
    /*
     * Bump the property that tells the name resolver library to reread
     * the DNS server list from the properties.
     */
    AutoPtr<ISystemProperties> sysProp;
    CSystemProperties::AcquireSingleton((ISystemProperties**)&sysProp);
    String propVal;
    sysProp->Get(String("net.dnschange"), &propVal);
    Int32 n = 0;
    if (!propVal.IsNullOrEmpty()) {
        n = StringUtils::ParseInt32(propVal);
    }
    String prop("");
    prop += StringUtils::ToString(n + 1);
    sysProp->Set(String("net.dnschange"), prop);
    /*
     * Tell the VMs to toss their DNS caches
     */
    AutoPtr<IIntent> intent;
    CIntent::New(IIntent::ACTION_CLEAR_DNS_CACHE, (IIntent**)&intent);
    intent->AddFlags(IIntent::FLAG_RECEIVER_REPLACE_PENDING);
    /*
     * Connectivity events can happen before boot has completed ...
     */
    intent->AddFlags(IIntent::FLAG_RECEIVER_REGISTERED_ONLY_BEFORE_BOOT);
    Int64 ident = Binder::ClearCallingIdentity();
    AutoPtr<IUserHandleHelper> helper;
    CUserHandleHelper::AcquireSingleton((IUserHandleHelper**)&helper);
    AutoPtr<IUserHandle> ALL;
    helper->GetALL((IUserHandle**)&ALL);
    mContext->SendBroadcastAsUser(intent, ALL);
    Binder::RestoreCallingIdentity(ident);
}

// Caller must grab mDnsLock.
Boolean CConnectivityService::UpdateDns(
    /* [in] */ const String& network,
    /* [in] */ const String& iface,
    /* [in] */ List< AutoPtr<IInetAddress> >& dnses,
    /* [in] */ const String& domains)
{
    Boolean changed = FALSE;
    Int32 last = 0;
    AutoPtr<ISystemProperties> sysProp;
    CSystemProperties::AcquireSingleton((ISystemProperties**)&sysProp);
    if (dnses.IsEmpty() && mDefaultDns != NULL) {
        ++last;
        String value;
        mDefaultDns->GetHostAddress(&value);
        String prop;
        sysProp->Get(String("net.dns1"), &prop);
        if (!value.Equals(prop)) {
            if (DBG) {
                Slogger::E(TAG, "no dns provided for %s - using %s", network.string(), value.string());
            }
            changed = TRUE;
            sysProp->Set(String("net.dns1"), value);
        }
    }
    else {
        List< AutoPtr<IInetAddress> >::Iterator it;
        for (it = dnses.Begin(); it != dnses.End(); ++it) {
            AutoPtr<IInetAddress> dns = *it;
            ++last;
            String key("net.dns");
            key += StringUtils::ToString(last);
            String value;
            dns->GetHostAddress(&value);
            String str;
            if (!changed && (sysProp->Get(key, &str), value.Equals(str))) {
                continue;
            }
            if (VDBG) {
                Slogger::V(TAG, "adding dns %s for %s", value.string(), network.string());
            }
            changed = TRUE;
            sysProp->Set(key, value);
        }
    }
    for (Int32 i = last + 1; i <= mNumDnsEntries; ++i) {
        String key("net.dns");
        key += StringUtils::ToString(i);
        if (VDBG) Slogger::E(TAG, "erasing %s", key.string());
        changed = TRUE;
        sysProp->Set(key, String(""));
    }
    mNumDnsEntries = last;

    if (changed) {
        //try {
        AutoPtr<IObjectContainer> _dnses;
        CObjectContainer::New((IObjectContainer**)&_dnses);
        List< AutoPtr<IInetAddress> >::Iterator it;
        for (it = dnses.Begin(); it != dnses.End(); ++it) {
            _dnses->Add(*it);
        }
        AutoPtr<ArrayOf<String> > dnsStrings = NetworkUtils::MakeStrings(_dnses);
        mNetd->SetDnsServersForInterface(iface, dnsStrings);
        mNetd->SetDefaultInterfaceForDns(iface);
        //} catch (Exception e) {
        //    if (DBG) loge("exception setting default dns interface: " + e);
        //}
    }
    String str;
    if (sysProp->Get(String("net.dns.search"), &str), !domains.Equals(str)) {
        sysProp->Set(String("net.dns.search"), domains);
        changed = TRUE;
    }
    return changed;
}

void CConnectivityService::HandleDnsConfigurationChange(
    /* [in] */ Int32 netType)
{
    // add default net's dns entries
    AutoPtr<INetworkStateTracker> nt = (*mNetTrackers)[netType];
    if (nt != NULL) {
        AutoPtr<INetworkInfo> info;
        nt->GetNetworkInfo((INetworkInfo**)&info);
        Boolean result;
        if ((info->IsConnected(&result), result) && (nt->IsTeardownRequested(&result), !result)) {
            AutoPtr<ILinkProperties> p;
            nt->GetLinkProperties((ILinkProperties**)&p);
            if (p == NULL) return;
            AutoPtr<IObjectContainer> dnses;
            p->GetDnses((IObjectContainer**)&dnses);
            Boolean changed = FALSE;
            Boolean isDefault;
            if ((*mNetConfigs)[netType]->IsDefault(&isDefault), isDefault) {
                String network;
                info->GetTypeName(&network);
                {
                    AutoLock lock(mDnsLock);
                    if (!mDnsOverridden) {
                        String iface;
                        p->GetInterfaceName(&iface);
                        List< AutoPtr<IInetAddress> > _dnses;
                        AutoPtr<IObjectEnumerator> dnsesIt;
                        dnses->GetObjectEnumerator((IObjectEnumerator**)&dnsesIt);
                        Boolean hasNext;
                        while (dnsesIt->MoveNext(&hasNext), hasNext) {
                            AutoPtr<IInterface> obj;
                            dnsesIt->Current((IInterface**)&obj);
                            _dnses.PushBack(IInetAddress::Probe(obj));
                        }
                        changed = UpdateDns(network, iface, _dnses, String(""));
                    }
                }
            }
            else {
                //try {
                String iface;
                p->GetInterfaceName(&iface);
                AutoPtr<ArrayOf<String> > dnsStrings = NetworkUtils::MakeStrings(dnses);
                mNetd->SetDnsServersForInterface(iface, dnsStrings);
                //} catch (Exception e) {
                //    if (DBG) loge("exception setting dns servers: " + e);
                //}
                // set per-pid dns for attached secondary nets
                AutoPtr<List<Int32> > pids = (*mNetRequestersPids)[netType];
                List<Int32>::Iterator it;
                for(it = pids->Begin(); it != pids->End(); ++it) {
                    Int32 pid = *it;
                    changed = WritePidDns(dnses, pid);
                }
            }
            if (changed) BumpDns();
        }
    }
}

Int32 CConnectivityService::GetRestoreDefaultNetworkDelay(
    /* [in] */ Int32 networkType)
{
    AutoPtr<ISystemProperties> sysProp;
    CSystemProperties::AcquireSingleton((ISystemProperties**)&sysProp);
    String restoreDefaultNetworkDelayStr;
    sysProp->Get(NETWORK_RESTORE_DELAY_PROP_NAME, &restoreDefaultNetworkDelayStr);
    if (!restoreDefaultNetworkDelayStr.IsNullOrEmpty()) {
        Int32 v;
        ECode ec = StringUtils::ParseInt32(restoreDefaultNetworkDelayStr, &v);
        if (SUCCEEDED(ec)) return v;
    }
    // if the system property isn't set, use the value for the apn type
    Int32 ret = RESTORE_DEFAULT_NETWORK_DELAY;

    if ((networkType <= IConnectivityManager::MAX_NETWORK_TYPE) &&
            ((*mNetConfigs)[networkType] != NULL)) {
        (*mNetConfigs)[networkType]->GetRestoreTime(&ret);
    }
    return ret;
}

void CConnectivityService::Dump(
    /* [in] */ IFileDescriptor* fd,
    /* [in] */ IPrintWriter* writer,
    /* [in] */ const ArrayOf<String>& args)
{
    assert(0);
//    AP<IIndentingPrintWriter> pw;
//    CIndentingPrintWriter::New(writer, "  ", (IIndentingPrintWriter**)&pw);
//
//    Int32 permission;
//    mContext->CheckCallingOrSelfPermission(
//            Elastos::Droid::Manifest::permission::DUMP, &permission);
//    if (permission!= IPackageManager::PERMISSION_GRANTED) {
//        AutoPtr<IBinderHelper> binderHelper;
//        CBinderHelper::AcquireSingleton((IBinderHelper**)&binderHelper);
//        Int32 pid;
//        binderHelper->GetCallingPid(&pid);
//        Int32 uid;
//        binderHelper->GetCallingUid(&uid);
//
//        pw->Println(String("Permission Denial: can't dump ConnectivityService ") +
//                String("from from pid=") + pid + String(", uid=") + uid);
//        return;
//    }
//
//    // TODO: add locking to get atomic snapshot
//    pw->Println();
//    for (Int32 i = 0; i < (*mNetTrackers).GetLength(); i++) {
//        AutoPtr<INetworkStateTracker> nst = (*mNetTrackers)[i];
//        if (nst != NULL) {
//            String name;
//            IConnectivityManager::GetNetworkTypeName(i, &name);
//            pw->Println(String("NetworkStateTracker for ") + name + String(":"));
//            pw>IncreaseIndent();
//            AutoPtr<INetworkInfo> info = nst->GetNetworkInfo();
//            Boolean isConnected;
//            info->IsConnected(&isConnected);
//            String typeName;
//            info->GetTypeName(&typeName);
//            if (isConnected == TRUE) {
//                pw->Println(String("Active network: ") + typeName);
//            }
//            pw->Println(info);
//            AutoPtr<ILinkProperties> properties = nst->GetLinkProperties();
//            pw->Println(properties);
//            pw->Println(nst);
//            pw->Println();
//            pw->DecreaseIndent();
//        }
//    }
//
//    pw->Println(String("Network Requester Pids:"));
//    pw->IncreaseIndent();
//
//    for(Int32 net = 0; net < mPriorityList->GetLength(); net++) {
//        String pidString = StringUtils::ToString(net) + String(": ");
//        AutoPtr<List<Int32> > plist = (*mNetRequestersPids)[net];
//        List<Int32>::Iterator iter;
//        for(iter = plist->Begin(); iter != plist->End(); iter++) {
//            pidString = pidString + StringUtils::Int32ToString(*iter) + ", ";
//        }
//
//    }
//
//    pw->Println();
//    pw->DecreaseIndent();
//
//    pw->Println(String("FeatureUsers:"));
//    pw->IncreaseIndent();
//
//    List<FeatureUser*>::Iterator it;
//    for( it = mFeatureUsers.Begin(); it != mFeatureUsers.End(); it++ ) {
//        pw->Println(*it->ToString());//FeatureUser::ToString()
//    }
//
//    pw->Println();
//    pw->DecreaseIndent();
//
//    {
//        AutoLock lock(this);
//
//        Boolean isHeld;
//        mNetTransitionWakeLock->IsHeld(&isHeld);
//        pw->Println(String("NetworkTranstionWakeLock is currently ") +
//                (isHeld ? "" : String("not ")) + String("held."));
//        pw->Println(String("It was last requested for ")+mNetTransitionWakeLockCausedBy);
//    }
//    pw->Println();
//
//    mTethering->Dump(fd, pw, args);
//
//    if (mInetLog != NULL) {
//        pw->Println();
//        pw->Println(String("Inet condition reports:"));
//        pw->IncreaseIndent();
//
//        List<String>::Iterator iter;
//        for(iter = mInetLog->Begin(); iter != mInetLog->End(); iter++) {
//            pw->Println(*iter);
//        }
//        pw->DecreaseIndent();
//    }
}

ECode CConnectivityService::Tether(
    /* [in] */ const String& iface,
    /* [out] */ Int32* status)
{
    VALIDATE_NOT_NULL(status);
    FAIL_RETURN(EnforceTetherChangePermission());

    Boolean supported;
    if (IsTetheringSupported(&supported), supported) {
        return mTethering->Tether(iface, status);
    }
    else {
        *status = IConnectivityManager::TETHER_ERROR_UNSUPPORTED;
        return NOERROR;
    }
}

ECode CConnectivityService::Untether(
    /* [in] */ const String& iface,
    /* [out] */ Int32* status)
{
    VALIDATE_NOT_NULL(status);
    FAIL_RETURN(EnforceTetherChangePermission());

    Boolean supported;
    if (IsTetheringSupported(&supported), supported) {
        return mTethering->Untether(iface, status);
    }
    else {
        *status = IConnectivityManager::TETHER_ERROR_UNSUPPORTED;
        return NOERROR;
    }
}

ECode CConnectivityService::GetLastTetherError(
    /* [in] */ const String& iface,
    /* [out] */ Int32* status)
{
    VALIDATE_NOT_NULL(status);
    FAIL_RETURN(EnforceTetherAccessPermission());

    Boolean supported;
    if (IsTetheringSupported(&supported), supported) {
        return mTethering->GetLastTetherError(iface, status);
    }
    else {
        *status = IConnectivityManager::TETHER_ERROR_UNSUPPORTED;
        return NOERROR;
    }
}

// TODO - proper iface API for selection by property, inspection, etc
ECode CConnectivityService::GetTetherableUsbRegexs(
    /* [out, callee] */ ArrayOf<String>** regexs)
{
    VALIDATE_NOT_NULL(regexs);
    FAIL_RETURN(EnforceTetherAccessPermission());
    Boolean supported;
    if (IsTetheringSupported(&supported), supported) {
        return mTethering->GetTetherableUsbRegexs(regexs);
    }
    else {
        AutoPtr< ArrayOf<String> > strArray = ArrayOf<String>::Alloc(0);
        *regexs = strArray;
        REFCOUNT_ADD(*regexs);
        return NOERROR;
    }
}

ECode CConnectivityService::GetTetherableWifiRegexs(
    /* [out, callee] */ ArrayOf<String>** regexs)
{
    VALIDATE_NOT_NULL(regexs);
    FAIL_RETURN(EnforceTetherAccessPermission());
    Boolean supported;
    if (IsTetheringSupported(&supported), supported) {
        return mTethering->GetTetherableWifiRegexs(regexs);
    }
    else {
        AutoPtr< ArrayOf<String> > strArray = ArrayOf<String>::Alloc(0);
        *regexs = strArray;
        REFCOUNT_ADD(*regexs);
        return NOERROR;
    }
}

ECode CConnectivityService::GetTetherableBluetoothRegexs(
    /* [out, callee] */ ArrayOf<String>** regexs)
{
    VALIDATE_NOT_NULL(regexs);
    FAIL_RETURN(EnforceTetherAccessPermission());
    Boolean supported;
    if (IsTetheringSupported(&supported), supported) {
        return mTethering->GetTetherableBluetoothRegexs(regexs);
    }
    else {
        AutoPtr< ArrayOf<String> > strArray = ArrayOf<String>::Alloc(0);
        *regexs = strArray;
        REFCOUNT_ADD(*regexs);
        return NOERROR;
    }
}

ECode CConnectivityService::SetUsbTethering(
    /* [in] */ Boolean enable,
    /* [out] */ Int32* status)
{
    VALIDATE_NOT_NULL(status);
    FAIL_RETURN(EnforceTetherAccessPermission());
    Boolean supported;
    if (IsTetheringSupported(&supported), supported) {
        return mTethering->SetUsbTethering(enable, status);
    }
    else {
        *status = IConnectivityManager::TETHER_ERROR_UNSUPPORTED;
        return NOERROR;
    }
}

// TODO - move iface listing, queries, etc to new module
ECode CConnectivityService::GetTetherableIfaces(
    /* [out, callee] */ ArrayOf<String>** ifaces)
{
    VALIDATE_NOT_NULL(ifaces);
    FAIL_RETURN(EnforceTetherAccessPermission());
    return mTethering->GetTetherableIfaces(ifaces);
}

ECode CConnectivityService::GetTetheredIfaces(
    /* [out, callee] */ ArrayOf<String>** ifaces)
{
    VALIDATE_NOT_NULL(ifaces);
    FAIL_RETURN(EnforceTetherAccessPermission());
    return mTethering->GetTetheredIfaces(ifaces);
}

ECode CConnectivityService::GetTetheredIfacePairs(
    /* [out, callee] */ ArrayOf<String>** ifaces)
{
    VALIDATE_NOT_NULL(ifaces);
    FAIL_RETURN(EnforceTetherAccessPermission());
    return mTethering->GetTetheredIfacePairs(ifaces);
}

ECode CConnectivityService::GetTetheringErroredIfaces(
    /* [out, callee] */ ArrayOf<String>** ifaces)
{
    VALIDATE_NOT_NULL(ifaces);
    FAIL_RETURN(EnforceTetherAccessPermission());
    AutoPtr< ArrayOf<String> > i = mTethering->GetErroredIfaces();
    *ifaces = i.Get();
    REFCOUNT_ADD(*ifaces);
    return NOERROR;
}

// if ro.tether.denied = true we default to no tethering
// gservices could set the secure setting to 1 though to enable it on a build where it
// had previously been turned off.
ECode CConnectivityService::IsTetheringSupported(
    /* [out] */ Boolean* status)
{
    VALIDATE_NOT_NULL(status);
    FAIL_RETURN(EnforceTetherAccessPermission());

    AutoPtr<ISystemProperties> sysProp;
    CSystemProperties::AcquireSingleton((ISystemProperties**)&sysProp);
    String denied;
    sysProp->Get(String("ro.tether.denied"), &denied);
    Int32 defaultVal =  denied.Equals("true") ? 0 : 1;

    AutoPtr<IContentResolver> resolver;
    mContext->GetContentResolver((IContentResolver**)&resolver);
    Int32 value;
    AutoPtr<ISettingsGlobal> settingsGlobal;
    CSettingsGlobal::AcquireSingleton((ISettingsGlobal**)&settingsGlobal);
    settingsGlobal->GetInt32(resolver, ISettingsGlobal::TETHER_SUPPORTED, defaultVal, &value);
    Boolean tetherEnabledInSettings = (value != 0);
    *status = tetherEnabledInSettings && mTetheringConfigValid;
    return NOERROR;
}

// An API NetworkStateTrackers can call when they lose their network.
// This will automatically be cleared after X seconds or a network becomes CONNECTED,
// whichever happens first.  The timer is started by the first caller and not
// restarted by subsequent callers.
ECode CConnectivityService::RequestNetworkTransitionWakelock(
    /* [in] */ const String& forWhom)
{
    FAIL_RETURN(EnforceConnectivityInternalPermission());
    {
        AutoLock lock(this);

        Boolean  isHeld = FALSE;
        if ((mNetTransitionWakeLock->IsHeld(&isHeld), isHeld)) {
            return NOERROR;
        }
        mNetTransitionWakeLockSerialNumber++;
        mNetTransitionWakeLock->AcquireLock();
        mNetTransitionWakeLockCausedBy = forWhom;
    }

    AutoPtr<IMessage> msg;
    mHandler->ObtainMessage(EVENT_CLEAR_NET_TRANSITION_WAKELOCK,
        mNetTransitionWakeLockSerialNumber, 0, (IMessage**)&msg);
    Boolean result;
    return mHandler->SendMessageDelayed(msg, mNetTransitionWakeLockTimeout, &result);
}

// 100 percent is full good, 0 is full bad.
ECode CConnectivityService::ReportInetCondition(
    /* [in] */ Int32 networkType,
    /* [in] */ Int32 percentage)
{
    if (VDBG) Slogger::V(TAG, "reportNetworkCondition(%d, %d)", networkType, percentage);
    FAIL_RETURN(mContext->EnforceCallingOrSelfPermission(
            Elastos::Droid::Manifest::permission::STATUS_BAR,
            String("ConnectivityService")));

//    if (DBG) {
//        Int32 pid;
//        Int32 uid;
//        AutoPtr<IBinderHelper> binderHelper;
//        CBinderHelper::AcquireSingleton((IBinderHelper**)&binderHelper);
//        binderHelper->GetCallingPid(&pid);
//        binderHelper->GetCallingUid(&uid);
//
//        AutoPtr<IGregorianCalendar> calendar;
//        CGregorianCalendar::New((IGregorianCalendar**)&calendar);
//        String s = StringUtils::ToString(pid) + "(" + StringUtils::Int32ToString(uid) + ") reports inet is " +
//            (percentage > 50 ? "connected" : "disconnected") + " (" + StringUtils::Int32ToString(percentage) + ") on " +
//            "network Type " + StringUtils::Int32ToString(networkType) + " at " + calendar->GetInstance()->GetTime();
//        mInetLog->PushBack(s);
//        while(mInetLog->GetSize() > INET_CONDITION_LOG_MAX_SIZE) {
//            mInetLog->Erase(0);
//        }
//    }

    AutoPtr<IMessage> msg;
    mHandler->ObtainMessage(EVENT_INET_CONDITION_CHANGE,
        networkType, percentage, (IMessage**)&msg);
    Boolean result;
    return mHandler->SendMessage(msg, &result);
}

void CConnectivityService::HandleInetConditionChange(
    /* [in] */ Int32 netType,
    /* [in] */ Int32 condition)
{
    if (mActiveDefaultNetwork == -1) {
        if (DBG) Slogger::D(TAG, "handleInetConditionChange: no active default network - ignore");
        return;
    }
    if (mActiveDefaultNetwork != netType) {
        if (DBG) {
            Slogger::D(TAG, "handleInetConditionChange: net=%d != default=%d - ignore", netType, mActiveDefaultNetwork);
        }
        return;
    }
    if (VDBG) {
        Slogger::V(TAG, "handleInetConditionChange: net=%d, condition=%d,mActiveDefaultNetwork=%d",
                netType, condition, mActiveDefaultNetwork);
    }
    mDefaultInetCondition = condition;
    Int32 delay;
    if (mInetConditionChangeInFlight == FALSE) {
        if (VDBG) Slogger::V(TAG, "handleInetConditionChange: starting a change hold");
        // setup a new hold to debounce this
        AutoPtr<IContentResolver> resolver;
        mContext->GetContentResolver((IContentResolver**)&resolver);
        AutoPtr<ISettingsGlobal> settingsGlobal;
        CSettingsGlobal::AcquireSingleton((ISettingsGlobal**)&settingsGlobal);
        if (mDefaultInetCondition > 50) {
            settingsGlobal->GetInt32(resolver, ISettingsGlobal::INET_CONDITION_DEBOUNCE_UP_DELAY, 500, &delay);
        }
        else {
            settingsGlobal->GetInt32(resolver, ISettingsGlobal::INET_CONDITION_DEBOUNCE_DOWN_DELAY, 3000, &delay);
        }
        mInetConditionChangeInFlight = TRUE;

        AutoPtr<IMessage> msg;
        mHandler->ObtainMessage(EVENT_INET_CONDITION_HOLD_END,
            mActiveDefaultNetwork, mDefaultConnectionSequence, (IMessage**)&msg);
        Boolean result;
        mHandler->SendMessageDelayed(msg, delay, &result);
    }
    else {
        // we've set the new condition, when this hold ends that will get picked up
        if (VDBG) Slogger::V(TAG, "handleInetConditionChange: currently in hold - not setting new end evt");
    }
}

void CConnectivityService::HandleInetConditionHoldEnd(
    /* [in] */ Int32 netType,
    /* [in] */ Int32 sequence)
{
    if (DBG) {
        Slogger::D(TAG, "handleInetConditionHoldEnd: net=%d , condition=%d, publicsh condition= %d",
            netType, mDefaultInetCondition, mDefaultInetConditionPublished);
    }
    mInetConditionChangeInFlight = FALSE;

    if (mActiveDefaultNetwork == -1) {
        if (DBG) Slogger::D(TAG, "handleInetConditionHoldEnd: no active default network - ignoring");
        return;
    }
    if (mDefaultConnectionSequence != sequence) {
        if (DBG) Slogger::D(TAG, "handleInetConditionHoldEnd: event hold for obsolete network - ignoring");
        return;
    }
    // TODO: Figure out why this optimization sometimes causes a
    //       change in mDefaultInetCondition to be missed and the
    //       UI to not be updated.
    // if (mDefaultInetConditionPublished == mDefaultInetCondition) {
    //    if (DBG) Slogger::D(TAG, "no change in condition - aborting");
    //    return;
    // }
    AutoPtr<INetworkInfo> networkInfo;
    (*mNetTrackers)[mActiveDefaultNetwork]->GetNetworkInfo((INetworkInfo**)&networkInfo);
    Boolean isConnected;
    if ((networkInfo->IsConnected(&isConnected), isConnected == FALSE)) {
        if (DBG) Slogger::D(TAG, "handleInetConditionHoldEnd: default network not connected - ignoring");
        return;
    }
    mDefaultInetConditionPublished = mDefaultInetCondition;
    SendInetConditionBroadcast(networkInfo);
    return;
}

ECode CConnectivityService::GetProxy(
    /* [out] */ IProxyProperties** proxyProperties)
{
    VALIDATE_NOT_NULL(proxyProperties);
    AutoLock lock(mDefaultProxyLock);
    *proxyProperties = mDefaultProxyDisabled ? NULL : mDefaultProxy;
    REFCOUNT_ADD(*proxyProperties);
    return NOERROR;
}

ECode CConnectivityService::SetGlobalProxy(
    /* [in] */ IProxyProperties* proxyProperties)
{
    FAIL_RETURN(EnforceChangePermission());
    {
        AutoLock lock(mGlobalProxyLock);
        if (proxyProperties == mGlobalProxy) {
            return NOERROR;
        }
        Boolean isEqual = FALSE;
        if (proxyProperties != NULL &&
            (proxyProperties->Equals(mGlobalProxy, &isEqual), isEqual)) {
            return NOERROR;
        }
        if (mGlobalProxy != NULL &&
            (mGlobalProxy->Equals(proxyProperties, &isEqual), isEqual)) {
            return NOERROR;
        }

        String host("");
        Int32 port = 0;
        String exclList("");
        if (proxyProperties != NULL) {
            String hozt;
            proxyProperties->GetHost(&hozt);
            if (!TextUtils::IsEmpty(hozt)) {
                mGlobalProxy = NULL;
                CProxyProperties::New(proxyProperties, (IProxyProperties**)&mGlobalProxy);
                mGlobalProxy->GetHost(&host);
                mGlobalProxy->GetPort(&port);
                mGlobalProxy->GetExclusionList(&exclList);
            }
            else {
                mGlobalProxy = NULL;
            }
        }
        else {
            mGlobalProxy = NULL;
        }
        AutoPtr<IContentResolver> res;
        mContext->GetContentResolver((IContentResolver**)&res);
        Boolean bol;
        AutoPtr<ISettingsGlobal> settingsGlobal;
        CSettingsGlobal::AcquireSingleton((ISettingsGlobal**)&settingsGlobal);
        settingsGlobal->PutString(res, ISettingsGlobal::GLOBAL_HTTP_PROXY_HOST, host, &bol);
        settingsGlobal->PutInt32(res, ISettingsGlobal::GLOBAL_HTTP_PROXY_PORT, port, &bol);
        settingsGlobal->PutString(res, ISettingsGlobal::GLOBAL_HTTP_PROXY_EXCLUSION_LIST,
                exclList, &bol);
    }

    if (mGlobalProxy == NULL) {
        proxyProperties = mDefaultProxy;
    }
    //sendProxyBroadcast(proxyProperties);
    return NOERROR;
}

void CConnectivityService::LoadGlobalProxy()
{
    AutoPtr<IContentResolver> res;
    mContext->GetContentResolver((IContentResolver**)&res);
    String host;
    AutoPtr<ISettingsGlobal> settingsGlobal;
    CSettingsGlobal::AcquireSingleton((ISettingsGlobal**)&settingsGlobal);
    settingsGlobal->GetString(res, ISettingsGlobal::GLOBAL_HTTP_PROXY_HOST, &host);
    Int32 port;
    settingsGlobal->GetInt32(res, ISettingsGlobal::GLOBAL_HTTP_PROXY_PORT, 0, &port);
    String exclList;
    settingsGlobal->GetString(res, ISettingsGlobal::GLOBAL_HTTP_PROXY_EXCLUSION_LIST, &exclList);
    if (!TextUtils::IsEmpty(host)) {
        AutoPtr<IProxyProperties> proxyProperties;
        CProxyProperties::New(host, port, exclList, (IProxyProperties**)&proxyProperties);
        {
            AutoLock lock(mGlobalProxyLock);
            mGlobalProxy = proxyProperties;
        }
    }
}

ECode CConnectivityService::GetGlobalProxy(
    /* [out] */ IProxyProperties** proxyProperties)
{
    VALIDATE_NOT_NULL(proxyProperties);
    {
        AutoLock lock(mGlobalProxyLock);
        *proxyProperties = mGlobalProxy;
        REFCOUNT_ADD(*proxyProperties);
    }
    return NOERROR;
}

void CConnectivityService::HandleApplyDefaultProxy(
    /* [in] */ IProxyProperties* proxy)
{
    if (proxy != NULL) {
        String host;
        proxy->GetHost(&host);
        if (TextUtils::IsEmpty(host)) {
            proxy = NULL;
        }
    }
    {
        AutoLock lock(mDefaultProxyLock);
        Boolean isEqual = FALSE;
        if (mDefaultProxy != NULL &&
            (mDefaultProxy->Equals(proxy, &isEqual), isEqual)) {
            return;
        }
        if (mDefaultProxy.Get() == proxy) return;
        mDefaultProxy = proxy;

        if (!mDefaultProxyDisabled) {
            SendProxyBroadcast(proxy);
        }
    }
}

void CConnectivityService::HandleDeprecatedGlobalHttpProxy()
{
    AutoPtr<IContentResolver> resolver;
    mContext->GetContentResolver((IContentResolver**)&resolver);
    String proxy;
    AutoPtr<ISettingsGlobal> settingsGlobal;
    CSettingsGlobal::AcquireSingleton((ISettingsGlobal**)&settingsGlobal);
    settingsGlobal->GetString(resolver, ISettingsGlobal::HTTP_PROXY, &proxy);
    if (!TextUtils::IsEmpty(proxy)) {
        AutoPtr< ArrayOf<String> > data;
        StringUtils::Split(proxy, String(":"), (ArrayOf<String>**)&data);
        String proxyHost = (*data)[0];
        Int32 proxyPort = 8080;
        if (data->GetLength() > 1) {
            ECode ec = StringUtils::ParseInt32((*data)[1], &proxyPort);
            if (FAILED(ec)) return;
        }
        AutoPtr<IProxyProperties> p;
        CProxyProperties::New((*data)[0], proxyPort, String(""), (IProxyProperties**)&p);
        SetGlobalProxy(p);
    }
}

void CConnectivityService::SendProxyBroadcast(
    /* [in] */ IProxyProperties* proxy)
{
    AutoPtr<IProxyProperties> proxyProp = proxy;
    if (proxyProp == NULL) {
        CProxyProperties::New(String(""), 0, String(""), (IProxyProperties**)&proxyProp);
    }
    if (DBG) Slogger::D(TAG, "sending Proxy Broadcast for %p", proxy);
    AutoPtr<IIntent> intent;
    CIntent::New(Elastos::Droid::Net::IProxy::PROXY_CHANGE_ACTION, (IIntent**)&intent);
    intent->AddFlags(IIntent::FLAG_RECEIVER_REPLACE_PENDING |
        IIntent::FLAG_RECEIVER_REGISTERED_ONLY_BEFORE_BOOT);
    intent->PutExtra(Elastos::Droid::Net::IProxy::EXTRA_PROXY_INFO, IParcelable::Probe(proxyProp));
    Int64 ident = Binder::ClearCallingIdentity();
    AutoPtr<IUserHandleHelper> helper;
    CUserHandleHelper::AcquireSingleton((IUserHandleHelper**)&helper);
    AutoPtr<IUserHandle> ALL;
    helper->GetALL((IUserHandle**)&ALL);
    mContext->SendStickyBroadcastAsUser(intent, ALL);
    Binder::RestoreCallingIdentity(ident);
}

Int32 CConnectivityService::ConvertFeatureToNetworkType(
    /* [in] */ Int32 networkType,
    /* [in] */ const String& feature)
{
    Int32 usedNetworkType = networkType;

    if (networkType == IConnectivityManager::TYPE_MOBILE) {
        if (TextUtils::Equals(feature, String("enableMMS")/*Phone::FEATURE_ENABLE_MMS*/)) {
            usedNetworkType = IConnectivityManager::TYPE_MOBILE_MMS;
        }
        else if (TextUtils::Equals(feature, String("enableSUPL")/*Phone::FEATURE_ENABLE_SUPL*/)) {
            usedNetworkType = IConnectivityManager::TYPE_MOBILE_SUPL;
        }
        else if (TextUtils::Equals(feature, String("enableDUN")/*Phone::FEATURE_ENABLE_DUN*/) ||
                TextUtils::Equals(feature, String("enableDUNAlways")/*Phone::FEATURE_ENABLE_DUN_ALWAYS*/)) {
            usedNetworkType = IConnectivityManager::TYPE_MOBILE_DUN;
        }
        else if (TextUtils::Equals(feature, String("enableHIPRI")/*Phone::FEATURE_ENABLE_HIPRI*/)) {
            usedNetworkType = IConnectivityManager::TYPE_MOBILE_HIPRI;
        }
        else if (TextUtils::Equals(feature, String("enableFOTA")/*Phone::FEATURE_ENABLE_FOTA*/)) {
            usedNetworkType = IConnectivityManager::TYPE_MOBILE_FOTA;
        }
        else if (TextUtils::Equals(feature, String("enableIMS")/*Phone::FEATURE_ENABLE_IMS*/)) {
            usedNetworkType = IConnectivityManager::TYPE_MOBILE_IMS;
        }
        else if (TextUtils::Equals(feature, String("enableCBS")/*Phone::FEATURE_ENABLE_CBS*/)) {
            usedNetworkType = IConnectivityManager::TYPE_MOBILE_CBS;
        }
        else {
            Slogger::E(TAG, "Can't match any mobile netTracker!");
        }
    }
    else if (networkType == IConnectivityManager::TYPE_WIFI) {
        if (TextUtils::Equals(feature, String("p2p"))) {
            usedNetworkType = IConnectivityManager::TYPE_WIFI_P2P;
        }
        else {
            Slogger::E(TAG, "Can't match any wifi netTracker!");
        }
    }
    else {
        Slogger::E(TAG, "Unexpected network type");
    }
    return usedNetworkType;
}

/**
 * Protect a socket from VPN routing rules. This method is used by
 * VpnBuilder and not available in ConnectivityManager. Permissions
 * are checked in Vpn class.
 * @hide
 */
ECode CConnectivityService::ProtectVpn(
    /* [in] */ IParcelFileDescriptor* socket,
    /* [out] */ Boolean* status)
{
    VALIDATE_NOT_NULL(status);
    *status = FALSE;
    FAIL_RETURN(ThrowIfLockdownEnabled());

    //try {
    Int32 type = mActiveDefaultNetwork;
    AutoPtr<IConnectivityManagerHelper> helper;
    CConnectivityManagerHelper::AcquireSingleton((IConnectivityManagerHelper**)&helper);
    Boolean isValid = FALSE;
    if (helper->IsNetworkTypeValid(type, &isValid), isValid) {
        AutoPtr<ILinkProperties> linkProperties;
        (*mNetTrackers)[type]->GetLinkProperties((ILinkProperties**)&linkProperties);
        String iface;
        linkProperties->GetInterfaceName(&iface);
        ECode ec = mVpn->Protect(socket, iface);
        if (SUCCEEDED(ec)) *status = TRUE;
        else *status = FALSE;
        socket->Close();
        return NOERROR;
    }
    socket->Close();
    return NOERROR;
}

/**
 * Prepare for a VPN application. This method is used by VpnDialogs
 * and not available in ConnectivityManager. Permissions are checked
 * in Vpn class.
 * @hide
 */
ECode CConnectivityService::PrepareVpn(
    /* [in] */ const String& oldPackage,
    /* [in] */ const String& newPackage,
    /* [out] */ Boolean* status)
{
    VALIDATE_NOT_NULL(status);
    *status = FALSE;

    FAIL_RETURN(ThrowIfLockdownEnabled());
    return mVpn->Prepare(oldPackage, newPackage, status);
}

/**
 * Configure a TUN interface and return its file descriptor. Parameters
 * are encoded and opaque to this class. This method is used by VpnBuilder
 * and not available in ConnectivityManager. Permissions are checked in
 * Vpn class.
 * @hide
 */
ECode CConnectivityService::EstablishVpn(
    /* [in] */ IVpnConfig* config,
    /* [out] */ IParcelFileDescriptor** fd)
{
    VALIDATE_NOT_NULL(fd);
    *fd = NULL;

    FAIL_RETURN(ThrowIfLockdownEnabled());
    return mVpn->Establish(config, fd);
}

/**
 * Start legacy VPN, controlling native daemons as needed. Creates a
 * secondary thread to perform connection work, returning quickly.
 */
ECode CConnectivityService::StartLegacyVpn(
    /* [in] */ IVpnProfile* profile)
{
    FAIL_RETURN(ThrowIfLockdownEnabled());
    AutoPtr<ILinkProperties> egress;
    GetActiveLinkProperties((ILinkProperties**)&egress);
    if (egress == NULL) {
        //throw new IllegalStateException("Missing active network connection");
        return E_ILLEGAL_STATE_EXCEPTION;
    }
    return mVpn->StartLegacyVpn(profile, mKeyStore, egress);
}

/**
 * Return the information of the ongoing legacy VPN. This method is used
 * by VpnSettings and not available in ConnectivityManager. Permissions
 * are checked in Vpn class.
 * @hide
 */
ECode CConnectivityService::GetLegacyVpnInfo(
    /* [out] */ ILegacyVpnInfo** info)
{
    VALIDATE_NOT_NULL(info);
    *info = NULL;

    FAIL_RETURN(ThrowIfLockdownEnabled());
    return mVpn->GetLegacyVpnInfo(info);
}

ECode CConnectivityService::UpdateLockdownVpn(
    /* [out] */ Boolean* status)
{
    VALIDATE_NOT_NULL(status);
    *status = FALSE;

    FAIL_RETURN(EnforceSystemUid());

    // Tear down existing lockdown if profile was removed
    mLockdownEnabled = LockdownVpnTracker::IsEnabled();
    if (mLockdownEnabled) {
        assert(0);
        // if (mKeyStore->State() != KeyStoreState::UNLOCKED) {
        //     Slogger::W(TAG, "KeyStore locked; unable to create LockdownTracker");
        //     *status = FALSE;
        //     return NOERROR;
        // }

        // String profileName(mKeyStore->Get(Credentials::LOCKDOWN_VPN));
        // AutoPtr<IVpnProfile> profile;
        // VpnProfile::Decode(profileName, mKeyStore->Get(Credentials::VPN + profileName));
        // SetLockdownTracker(new CLockdownVpnTracker(mContext, mNetd, this, mVpn, profile));
    }
    else {
        SetLockdownTracker(NULL);
    }

    *status = TRUE;
    return NOERROR;
}

/**
 * Internally set new {@link LockdownVpnTracker}, shutting down any existing
 * {@link LockdownVpnTracker}. Can be {@code NULL} to disable lockdown.
 */
void CConnectivityService::SetLockdownTracker(
    /* [in] */ LockdownVpnTracker* tracker)
{
    // Shutdown any existing tracker
    AutoPtr<LockdownVpnTracker> existing = mLockdownTracker;
    mLockdownTracker = NULL;
    if (existing != NULL) {
        existing->Shutdown();
    }

    //try {
    if (tracker != NULL) {
        mNetd->SetFirewallEnabled(TRUE);
        mLockdownTracker = tracker;
        mLockdownTracker->Init();
    }
    else {
        mNetd->SetFirewallEnabled(FALSE);
    }
    //} catch (RemoteException e) {
        // ignored; NMS lives inside system_server
    //}
}

ECode CConnectivityService::ThrowIfLockdownEnabled()
{
    if (mLockdownEnabled) {
        //throw new IllegalStateException("Unavailable in lockdown mode");
        return E_ILLEGAL_STATE_EXCEPTION;
    }
    return NOERROR;
}

ECode CConnectivityService::EnforceSystemUid()
{
    Int32 uid = Binder::GetCallingUid();
    if (uid != IProcess::SYSTEM_UID) {
        //throw new SecurityException("Only available to AID_SYSTEM");
        return E_SECURITY_EXCEPTION;
    }
    return NOERROR;
}

void CConnectivityService::HandleEventClearNetTransitionWakelock(
    /* [in] */ Int32 arg1)
{
    String causedBy;
    {
        AutoLock lock(this);
        Boolean held = FALSE;
        if (arg1 == mNetTransitionWakeLockSerialNumber &&
                (mNetTransitionWakeLock->IsHeld(&held), held)) {
            mNetTransitionWakeLock->ReleaseLock();
            causedBy = mNetTransitionWakeLockCausedBy;
        }
    }
    if (!causedBy.IsNull()) {
        Slogger::D(TAG, "NetTransition Wakelock for %s released by timeout", causedBy.string());
    }
}

void CConnectivityService::HandleEventRestoreDns()
{
    if (mActiveDefaultNetwork != -1) {
        HandleDnsConfigurationChange(mActiveDefaultNetwork);
    }
}

void CConnectivityService::HandleEventVpnStateChanged(
    /* [in] */ INetworkInfo* info)
{
    if (mLockdownTracker != NULL) {
        mLockdownTracker->OnVpnStateChanged(info);
    }
}

} //namespace Server
} //namespace Droid
} //namespace Elastos
