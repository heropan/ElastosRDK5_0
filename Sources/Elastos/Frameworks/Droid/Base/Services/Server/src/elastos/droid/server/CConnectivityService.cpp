
#include "elastos/droid/server/CConnectivityService.h"
#include "elastos/droid/server/CNetworkPolicyListener.h"
#include "elastos/droid/server/CDataActivityObserver.h"
#include "elastos/droid/server/am/CBatteryStatsService.h"
#include "elastos/droid/net/DummyDataStateTracker.h"
#include "elastos/droid/net/ethernet/EthernetDataTracker.h"
#include "elastos/droid/net/NetworkUtils.h"
#include "elastos/droid/net/wifi/WifiStateTracker.h"
#include "elastos/droid/os/Binder.h"
#include "elastos/droid/os/FileUtils.h"
#include "elastos/droid/os/SystemClock.h"
#include "elastos/droid/text/TextUtils.h"
#include "elastos/droid/R.h"
#include "elastos/droid/Manifest.h"
#include <elastos/utility/logging/Slogger.h>
#include <elastos/core/StringUtils.h>
#include <elastos/core/StringBuilder.h>
#include <cutils/properties.h>

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

using Elastos::Core::StringUtils;
using Elastos::Core::ISystem;
using Elastos::Core::CSystem;
using Elastos::Core::StringBuilder;
using Elastos::Core::CObjectContainer;
using Elastos::Net::IInetAddress;
using Elastos::Net::IInetAddressHelper;
using Elastos::Net::CInetAddressHelper;
using Elastos::Utility::Logging::Slogger;
using Elastos::Security::IKeyStoreHelper;
using Elastos::Security::CKeyStoreHelper;

namespace Elastos {
namespace Droid {
namespace Server {

const String CConnectivityService::TAG("ConnectivityService");

const Boolean CConnectivityService::DBG = FALSE;
const Boolean CConnectivityService::VDBG = FALSE;
const Boolean CConnectivityService::SAMPLE_DBG = FALSE;
const Boolean CConnectivityService::LOGD_RULES = FALSE;

const Int32 CConnectivityService::RESTORE_DEFAULT_NETWORK_DELAY = 1 * 60 * 1000;
const String CConnectivityService::NETWORK_RESTORE_DELAY_PROP_NAME("android.telephony.apn-restore");


const Int32 CConnectivityService::DEFAULT_FAIL_FAST_TIME_MS = 1 * 60 * 1000;
const String CConnectivityService::FAIL_FAST_TIME_MS("persist.radio.fail_fast_time_ms");
const String CConnectivityService::ACTION_PKT_CNT_SAMPLE_INTERVAL_ELAPSED \
    ("android.net.ConnectivityService.action.PKT_CNT_SAMPLE_INTERVAL_ELAPSED");

// Set network sampling interval at 12 minutes, this way, even if the timers get
// aggregated, it will fire at around 15 minutes, which should allow us to
// aggregate this timer with other timers (specially the socket keep alive timers)
const Int32 CConnectivityService::DEFAULT_SAMPLING_INTERVAL_IN_SECONDS = (SAMPLE_DBG ? 30 : 12 * 60);

// start network sampling a minute after booting ...
const Int32 CConnectivityService::DEFAULT_START_SAMPLING_INTERVAL_IN_SECONDS = (SAMPLE_DBG ? 30 : 60);

const Int32 SAMPLE_INTERVAL_ELAPSED_REQUEST_CODE = 0;

const Int32 CConnectivityService::ENABLED  = 1;
const Int32 CConnectivityService::DISABLED = 0;

const Int32 CConnectivityService::EVENT_CHANGE_MOBILE_DATA_ENABLED = 2;
const Int32 CConnectivityService::EVENT_CLEAR_NET_TRANSITION_WAKELOCK = 8;
const Int32 CConnectivityService::EVENT_APPLY_GLOBAL_HTTP_PROXY = 9;
const Int32 CConnectivityService::EVENT_SET_DEPENDENCY_MET = 10;
const Int32 CConnectivityService::EVENT_SEND_STICKY_BROADCAST_INTENT = 11;
const Int32 CConnectivityService::EVENT_SET_POLICY_DATA_ENABLE = 12;
const Int32 CConnectivityService::EVENT_ENABLE_FAIL_FAST_MOBILE_DATA = 14;
const Int32 CConnectivityService::EVENT_SAMPLE_INTERVAL_ELAPSED = 15;
const Int32 CConnectivityService::EVENT_PROXY_HAS_CHANGED = 16;
const Int32 CConnectivityService::EVENT_REGISTER_NETWORK_FACTORY = 17;
const Int32 CConnectivityService::EVENT_REGISTER_NETWORK_AGENT = 18;
const Int32 CConnectivityService::EVENT_REGISTER_NETWORK_REQUEST = 19;
const Int32 CConnectivityService::EVENT_TIMEOUT_NETWORK_REQUEST = 20;
const Int32 CConnectivityService::EVENT_REGISTER_NETWORK_LISTENER = 21;
const Int32 CConnectivityService::EVENT_RELEASE_NETWORK_REQUEST = 22;
const Int32 CConnectivityService::EVENT_UNREGISTER_NETWORK_FACTORY = 23;
const Int32 CConnectivityService::EVENT_EXPIRE_NET_TRANSITION_WAKELOCK = 24;
const Int32 CConnectivityService::EVENT_SYSTEM_READY = 25;

const Int32 CConnectivityService::INET_CONDITION_LOG_MAX_SIZE = 15;

const Int32 CConnectivityService::MIN_NET_ID = 100; // some reserved marks
const Int32 CConnectivityService::MAX_NET_ID = 65535;

const String CConnectivityService::DEFAULT_TCP_BUFFER_SIZES("4096,87380,110208,4096,16384,110208");

const String CConnectivityService::PROVISIONING_URL_PATH("/data/misc/radio/provisioning_urls.xml");

/** XML tag for root element. */
const String CConnectivityService::TAG_PROVISIONING_URLS("provisioningUrls");
/** XML tag for individual url */
const String CConnectivityService::TAG_PROVISIONING_URL("provisioningUrl");
/** XML tag for redirected url */
const String CConnectivityService::TAG_REDIRECTED_URL("redirectedUrl");
/** XML attribute for mcc */
const String CConnectivityService::ATTR_MCC("mcc");
/** XML attribute for mnc */
const String CConnectivityService::ATTR_MNC("mnc");

const Int32 CConnectivityService::REDIRECTED_PROVISIONING = 1;
const Int32 CConnectivityService::PROVISIONING = 2;

const String CConnectivityService::NOTIFICATION_ID("CaptivePortal.Notification");

//==============================================================================
// CConnectivityService::LegacyTypeTracker
//==============================================================================
const Boolean CConnectivityService::LegacyTypeTracker::DBG = TRUE;
const Boolean CConnectivityService::LegacyTypeTracker::VDBG = FALSE;
const String CConnectivityService::LegacyTypeTracker::TAG("CConnectivityService::LegacyTypeTracker");

CConnectivityService::LegacyTypeTracker::LegacyTypeTracker(
    /* [in] */ CConnectivityService* host)
{
    mTypeLists = ArrayOf<IArrayList*>::Alloc(IConnectivityManager::MAX_NETWORK_TYPE + 1);
}

ECode CConnectivityService::LegacyTypeTracker::AddSupportedType(
    /* [in] */ Int32 type)
{
    if ((&mTypeLists)[type] != NULL) {
        Slogger::E(TAG, "legacy list for type %d already initialized", type);
        return E_ILLEGAL_STATE_EXCEPTION;
    }

    AutoLock<IArrayList> list;
    CArrayList::New((IArrayList**)&list);
    mTypeLists->Set(type, list);
    return NOERROR;
}

Boolean CConnectivityService::LegacyTypeTracker::IsTypeSupported(
    /* [in] */ Int32 type)
{
    return mHost->IsNetworkTypeValid(type) && (mTypeLists*)[type] != NULL;
}

AutoPtr<INetworkAgentInfo> CConnectivityService::LegacyTypeTracker::GetNetworkForType(
    /* [in] */ Int32 type)
{
    AutoPtr<INetworkAgentInfo> info;
    Boolean empty;
    if (IsTypeSupported(type) && ((*mTypeLists)[type]->IsEmpty(&empty), !empty)) {
        AutoPtr<IInterface> obj;
        (*mTypeLists)[type]->Get(0, (IInterface**)&obj);
        info = INetworkAgentInfo::Probe(obj);
    }

    return info;
}

void CConnectivityService::LegacyTypeTracker::MaybeLogBroadcast(
    /* [in] */ INetworkAgentInfo* nai,
    /* [in] */ Boolean connected,
    /* [in] */ Int32 type)
{
    if (DBG) {
        String name;
        nai->Name(&name);
        Slogger::D(TAG, "Sending %s broadcast for type %d %s isDefaultNetwork=%d.",
            connected ? "connected" : "disconnected", type, name.string(), mHost->IsDefaultNetwork(nai));
    }
}

/** Adds the given network to the specified legacy type list. */
void CConnectivityService::LegacyTypeTracker::Add(
    /* [in] */ Int32 type,
    /* [in] */ INetworkAgentInfo* nai)
{
    if (!IsTypeSupported(type)) {
        return;  // Invalid network type.
    }
    if (VDBG) {
        Slogger::D(TAG, "Adding agent %s for legacy network type %d",
            Object::ToString(nai), type);
    }

    AutoPtr<IArrayList> list = (*mTypeLists)[type];
    Boolean bval;
    IInterface* naiObj = TO_IINTERFACE(nai);
    list->Contains(naiObj, &bval);
    if (bval) {
        Slogger::E(TAG, "Attempting to register duplicate agent for type %d: %s",
            type, Object::ToString(nai));
        return;
    }

    list->Add(naiObj);

    // Send a broadcast if this is the first network of its type or if it's the default.
    Int32 ival;
    list->GetSize(&ival);
    if (ival == 1 || IsDefaultNetwork(nai)) {
        MaybeLogBroadcast(nai, TRUE, type);
        SendLegacyNetworkBroadcast(nai, TRUE, type);
    }
}

void CConnectivityService::LegacyTypeTracker::Remove(
    /* [in] */ Int32 type,
    /* [in] */ INetworkAgentInfo* nai)
{
    AutoPtr<IArrayList> list = (*mTypeLists)[type];
    Boolean bval;
    if (list == NULL || list->IsEmpty(&bval)) {
        return;
    }

    AutoPtr<IInterface> obj;
    list->Get(0, (IInterface**)&obj);

    AutoPtr<IInterface> naiObj = TO_IINTERFACE(nai);
    Boolean wasFirstNetwork = Object::Equals(obj, naiObj);

    list->Remove(naiObj, &bval);
    if (!bval) {
        return;
    }

    if (wasFirstNetwork || IsDefaultNetwork(nai)) {
        MaybeLogBroadcast(nai, FALSE, type);
        SendLegacyNetworkBroadcast(nai, FALSE, type);
    }

    list->IsEmpty(&bval);
    if (!bval && wasFirstNetwork) {
        if (DBG) {
            Slogger::D(TAG, "Other network available for type %d, sending connected broadcast",
                type);
        }

        obj = NULL;
        list->Get(0, (IInterface**)&obj);
        INetworkAgentInfo* info = INetworkAgentInfo::Probe(obj);
        MaybeLogBroadcast(info, FALSE, type);
        SendLegacyNetworkBroadcast(info, FALSE, type);
    }
}

void CConnectivityService::LegacyTypeTracker::Remove(
    /* [in] */ INetworkAgentInfo* nai)
{
    if (VDBG) Slogger::D(TAG, "Removing agent %s", Object::ToString(nai).string());
    for (Int32 type = 0; type < mTypeLists->GetLength(); type++) {
        Remove(type, nai);
    }
}

String CConnectivityService::LegacyTypeTracker::NaiToString(
    /* [in] */ INetworkAgentInfo* nai)
{
    String name("NULL");
    if (nai != NULL) {
        nai->Name(&name);
    }

    NetworkAgentInfo* info = (NetworkAgentInfo*)nai;
    String state("???/???");
    if (info->mNetworkInfo != NULL) {
        NetworkInfoState state;
        NetworkInfoDetailedState detailedState;
        info->mNetworkInfo->GetState(&state);
        info->mNetworkInfo->GetDetailedState(&detailedState);
        StringBuilder sb;
        sb += (Int32)state;
        sb += "/";
        sb += (Int32)detailedState;
        state = sb.ToString();
    }

    StringBuilder sb(name);
    sb += " ";
    sb += state;
    return sb.ToString();
}

ECode CConnectivityService::LegacyTypeTracker::Dump(
    /* [in] */ IIndentingPrintWriter* pw)
{
    Int32 size;
    for (Int32 type = 0; type < mTypeLists->GetLength; type++) {
        if ((*mTypeLists)[type] == NULL) continue;
        pw->Print(type + " ");
        pw->IncreaseIndent();
        if ((*mTypeLists)[type]->GetSize(&size) == 0) pw->Println("none");
        for (Int32 i = 0; i < size; ++i) {
            AutoPtr<IInterface> obj;
            (*mTypeLists)[type]->Get(i, (IInterface**)&obj);
            INetworkAgentInfo* nai = INetworkAgentInfo::Probe(obj);
            pw->Println(NaiToString(nai));
        }
        pw->DecreaseIndent();
    }
    return NOERROR;
}

void CConnectivityService::LegacyTypeTracker::Log(
    /* [in] */ const String& s)
{
    Slogger::D(TAG, s.string());
}


//==============================================================================
// CConnectivityService::NetworkRequestInfo
//==============================================================================

static const Boolean CConnectivityService::NetworkRequestInfo::REQUEST = true;
static const Boolean CConnectivityService::NetworkRequestInfo::LISTEN = false;

CAR_INTERFACE_DECL(NetworkRequestInfo, Object, IProxyDeathRecipient)

CConnectivityService::NetworkRequestInfo::NetworkRequestInfo(
    /* [in] */ IMessenger* m,
    /* [in] */ INetworkRequest* r,
    /* [in] */ IBinder* binder,
    /* [in] */ Boolean isRequest,
    /* [in] */ CConnectivityService* host)
    : mHost(host)
{
    mMessenger = m;
    mRequest = r;
    mBinder = binder;
    mPid = GetCallingPid();
    mUid = GetCallingUid();
    mIsRequest = isRequest;

    ECode ec = mBinder->LinkToDeath((IProxyDeathRecipient*)this, 0);
    if (ec == E_REMOTE_EXCEPTION) {
        ProxyDied();
    }
}

void CConnectivityService::NetworkRequestInfo::UnlinkDeathRecipient()
{
    mBinder->UnlinkToDeath((IProxyDeathRecipient*)this, 0);
}

ECode CConnectivityService::NetworkRequestInfo::ProxyDied()
{
    Slogger::D("CConnectivityService", "ConnectivityService NetworkRequestInfo binderDied(%s, %)",
        Object::ToString(mRequest).string(), Object::ToString(mBinder).string());
    mHost->ReleaseNetworkRequest(mRequest);
    return NOERROR;
}

ECode CConnectivityService::NetworkRequestInfo::ToString(
    /* [out] */ String* str)
{
    VALIDATE_NOT_NULL(str)
    StringBuilder sb(isRequest ? "Request" : "Listen");
    sb += " from uid/pid:";
    sb += mUid;
    sb += "/";
    sb += mPid;
    sb += " for ";
    sb += Object::ToString(mRequest.string());
    return NOERROR;
}

//==============================================================================
// CConnectivityService::UserIntentReceiver
//==============================================================================
CConnectivityService::UserIntentReceiver::UserIntentReceiver(
    /* [in] */ CConnectivityService* host)
    : mHost(host)
{}

ECode CConnectivityService::UserIntentReceiver::OnReceive(
    /* [in] */ IContext* context,
    /* [in] */ IIntent* intent)
{
    String action;
    intent->GetAction(&action);
    Int32 userId;
    intent->GetInt32Extra(IIntent::EXTRA_USER_HANDLE, IUserHandle::USER_NULL);
    if (userId == IUserHandle::USER_NULL) return NOERROR;

    if (IIntent::ACTION_USER_STARTING.Equals(action)) {
        mHost->OnUserStart(userId);
    }
    else if (Intent.ACTION_USER_STOPPING.Equals(action)) {
        mHost->OnUserStop(userId);
    }
    return NOERROR;
}

//==============================================================================
// CConnectivityService::InternalBroadcastReciver
//==============================================================================
ECode CConnectivityService::InternalBroadcastReciver::OnReceive(
    /* [in] */ IContext* context,
    /* [in] */ IIntent* intent)
{
    // Try creating lockdown tracker, since user present usually means
    // unlocked keystore.
    Boolean update;
    if ((mHost->UpdateLockdownVpn(&update), update)) {
        mHost->mContext->UnregisterReceiver(this);
    }
    return NOERROR;
}

//==============================================================================
// CConnectivityService::PktCntSampleIntervalElapsedeceiver
//==============================================================================

CConnectivityService::PktCntSampleIntervalElapsedeceiver::PktCntSampleIntervalElapsedeceiver(
    /* [in] */ CConnectivityService* host)
    : mHost(host)
{}

//@Override
ECode CConnectivityService::PktCntSampleIntervalElapsedeceiver::OnReceive(
    /* [in] */ IContext* context,
    /* [in] */ IIntent* intent)
{
    String action;
    intent->GetAction(&action);
    if (action.Equals(CConnectivityService::ACTION_PKT_CNT_SAMPLE_INTERVAL_ELAPSED)) {
        AutoPtr<IMessage> msg;
        mHost->mHandler->ObtainMessage(CConnectivityService::EVENT_SAMPLE_INTERVAL_ELAPSED, (IMessage**)&msg);
        Boolean bval;
        mHost->mHandler->SendMessage(msg, &bval);
    }
}

//==============================================================================
// CConnectivityService::InternalHandler
//==============================================================================

ECode CConnectivityService::InternalHandler::HandleMessage(
    /* [in] */ IMessage* msg)
{
    Int32 what, arg1, arg2;
    msg->GetWhat(&what);
    msg->GetArg1(&arg1);
    msg->GetArg2(&arg2);
    AutoPtr<IInterface> obj;
    msg->GetObj((IInterface**)&obj);

    switch (what) {
        case CConnectivityService::EVENT_EXPIRE_NET_TRANSITION_WAKELOCK:
        case CConnectivityService::EVENT_CLEAR_NET_TRANSITION_WAKELOCK: {
            String causedBy;
            synchronized(mHost) {
                Boolean bval;
                if (arg1 == mNetTransitionWakeLockSerialNumber &&
                        (mNetTransitionWakeLock->IsHeld(&bval), bval)) {
                    mNetTransitionWakeLock->ReleaseLock();
                    causedBy = mNetTransitionWakeLockCausedBy;
                } else {
                    break;
                }
            }
            if (what == CConnectivityService::EVENT_EXPIRE_NET_TRANSITION_WAKELOCK) {
                Slogg::D("CConnectivityService", "Failed to find a new network"
                    " - expiring NetTransition Wakelock");
            }
            else {
                Slogg::D("CConnectivityService", "NetTransition Wakelock (%s)"
                    " cleared because we found a replacement network",
                    causedBy.IsNull() ? "unknown" : causedBy);
            }
            break;
        }
        case CConnectivityService::EVENT_APPLY_GLOBAL_HTTP_PROXY: {
            mHost->HandleDeprecatedGlobalHttpProxy();
            break;
        }
        case CConnectivityService::EVENT_SET_DEPENDENCY_MET: {
            Boolean met = (arg1 == CConnectivityService::ENABLED);
            mHost->HandleSetDependencyMet(arg2, met);
            break;
        }
        case CConnectivityService::EVENT_SEND_STICKY_BROADCAST_INTENT: {
            AutoPtr<IIntent> intent = IIntent::Probe(obj);
            mHost->SendStickyBroadcast(intent);
            break;
        }
        case CConnectivityService::EVENT_SET_POLICY_DATA_ENABLE: {
            Int32 networkType = arg1;
            Boolean enabled = arg2 == CConnectivityService::ENABLED;
            mHost->HandleSetPolicyDataEnable(networkType, enabled);
            break;
        }
        case CConnectivityService::EVENT_ENABLE_FAIL_FAST_MOBILE_DATA: {
            Int32 tag;
            mHost->mEnableFailFastMobileDataTag->Get(&tag);
            if (arg1 == tag) {
                AutoPtr<IMobileDataStateTracker> mobileDst;
                mobileDst = IMobileDataStateTracker::Probe((*mNetTrackers)[IConnectivityManager::TYPE_MOBILE]);
                if (mobileDst != NULL) {
                    mobileDst->SetEnableFailFastMobileData(arg2);
                }
            } else {
                Slogg::D("CConnectivityService", "EVENT_ENABLE_FAIL_FAST_MOBILE_DATA: stale arg1:%d != %d",
                    arg1, tag);
            }
            break;
        }
        case CConnectivityService::EVENT_SAMPLE_INTERVAL_ELAPSED: {
            mHost->HandleNetworkSamplingTimeout();
            break;
        }
        case CConnectivityService::EVENT_PROXY_HAS_CHANGED: {
            IProxyInfo* pi = IProxyInfo::Probe(obj);
            mHost->HandleApplyDefaultProxy(pi);
            break;
        }
        case CConnectivityService::EVENT_REGISTER_NETWORK_FACTORY: {
            INetworkFactoryInfo* nfi = INetworkFactoryInfo::Probe(obj);
            mHost->HandleRegisterNetworkFactory(nfi);
            break;
        }
        case CConnectivityService::EVENT_UNREGISTER_NETWORK_FACTORY: {
            IMessenger* msger = IMessenger::Probe(obj);
            mHost->HandleUnregisterNetworkFactory(msger);
            break;
        }
        case CConnectivityService::EVENT_REGISTER_NETWORK_AGENT: {
            INetworkAgentInfo* nai = INetworkAgentInfo::Probe(obj);
            mHost->HandleRegisterNetworkAgent(nai);
            break;
        }
        case CConnectivityService::EVENT_REGISTER_NETWORK_REQUEST:
        case CConnectivityService::EVENT_REGISTER_NETWORK_LISTENER: {
            mHost->HandleRegisterNetworkRequest(msg);
            break;
        }
        case CConnectivityService::EVENT_RELEASE_NETWORK_REQUEST: {
            INetworkRequest* nr = INetworkRequest::Probe(obj);
            mHost->HandleReleaseNetworkRequest(nr, arg1);
            break;
        }
        case CConnectivityService::EVENT_SYSTEM_READY: {
            AutoPtr<ICollection> values;
            mHost->mNetworkAgentInfos->GetValues((ICollection**)&values);
            AutoPtr<IIterator> it;
            values->GetIterator((IIterator**)&it);
            Boolean hasNext;
            while (it->HasNext(&hasNext), hasNext) {
                AutoPtr<IInterface> obj;
                it->GetNext((IInterface**)&obj);
                NetworkAgentInfo* nai = (NetworkAgentInfo*)INetworkAgentInfo::Probe(obj);
                nai->mNetworkMonitor->mSystemReady = TRUE;
            }
            break;
        }
    }
    return NOERROR;
}

//==============================================================================
// CConnectivityService::NetworkStateTrackerHandler
//==============================================================================

ECode CConnectivityService::NetworkStateTrackerHandler::HandleMessage(
    /* [in] */ IMessage* msg)
{
    AutoPtr<IInterface> obj;
    msg->GetObj((IInterface**)&obj);
    AutoPtr<INetworkInfo> info = INetworkInfo::Probe(obj);
    Int32 what;
    msg->GetWhat(&what);
    AutoPtr<IMessenger> replyToMsger;
    msg->GetReplyTo((IMessenger**)&replyToMsger);
    IInterface* replyTo = (IInterface*)replyToMsger;

    switch (what) {
        case IAsyncChannel::CMD_CHANNEL_HALF_CONNECTED: {
            mHost->HandleAsyncChannelHalfConnect(msg);
            break;
        }
        case IAsyncChannel::CMD_CHANNEL_DISCONNECT: {
            AutoPtr<IInterface> naiObj;
            mHost->mNetworkAgentInfos->Get(replyTo, (IInterface**)&naiObj);
            NetworkAgentInfo* nai = (NetworkAgentInfo*)INetworkAgentInfo::Probe(naiObj);
            if (nai != NULL) nai->mAsyncChannel->Disconnect();
            break;
        }
        case IAsyncChannel::CMD_CHANNEL_DISCONNECTED: {
            mHost->handleAsyncChannelDisconnected(msg);
            break;
        }
        case INetworkAgent::EVENT_NETWORK_CAPABILITIES_CHANGED: {
            AutoPtr<IInterface> naiObj;
            mHost->mNetworkAgentInfos->Get(replyTo, (IInterface**)&naiObj);
            NetworkAgentInfo* nai = (NetworkAgentInfo*)INetworkAgentInfo::Probe(naiObj);
            if (nai == NULL) {
                Slogg::E("CConnectivityService", "EVENT_NETWORK_CAPABILITIES_CHANGED from unknown NetworkAgent");
            } else {
                mHost->UpdateCapabilities(nai, INetworkCapabilities::Probe(obj));
            }
            break;
        }
        case INetworkAgent::EVENT_NETWORK_PROPERTIES_CHANGED: {
            AutoPtr<IInterface> naiObj;
            mHost->mNetworkAgentInfos->Get(replyTo, (IInterface**)&naiObj);
            NetworkAgentInfo* nai = (NetworkAgentInfo*)INetworkAgentInfo::Probe(naiObj);
            if (nai == NULL) {
                Slogg::E("CConnectivityService", "NetworkAgent not found for EVENT_NETWORK_PROPERTIES_CHANGED");
            } else {
                if (VDBG) {
                    Slogg::D("CConnectivityService", "Update of LinkProperties for %s; created=",
                        nai->Name().string(), nai->mCreated);
                }
                AutoPtr<ILinkProperties> oldLp = nai->mLinkProperties;
                synchronized(nai) {
                    nai->mLinkProperties = ILinkProperties::Probe(obj);
                }
                if (nai->mCreated) mHost->UpdateLinkProperties(nai, oldLp);
            }
            break;
        }
        case INetworkAgent::EVENT_NETWORK_INFO_CHANGED: {
            AutoPtr<IInterface> naiObj;
            mHost->mNetworkAgentInfos->Get(replyTo, (IInterface**)&naiObj);
            NetworkAgentInfo* nai = (NetworkAgentInfo*)INetworkAgentInfo::Probe(naiObj);
            if (nai == NULL) {
                Slogg::E("CConnectivityService", "EVENT_NETWORK_INFO_CHANGED from unknown NetworkAgent");
                break;
            }
            info = INetworkInfo::Probe(obj);
            mHost->UpdateNetworkInfo(nai, info);
            break;
        }
        case INetworkAgent::EVENT_NETWORK_SCORE_CHANGED: {
            AutoPtr<IInterface> naiObj;
            mHost->mNetworkAgentInfos->Get(replyTo, (IInterface**)&naiObj);
            NetworkAgentInfo* nai = (NetworkAgentInfo*)INetworkAgentInfo::Probe(naiObj);
            if (nai == NULL) {
                Slogg::E("CConnectivityService", "EVENT_NETWORK_SCORE_CHANGED from unknown NetworkAgent");
                break;
            }
            IInteger32 score = IInteger32::Probe(obj);
            if (score != NULL) {
                Int32 ival;
                score->GetValue(&ival);
                mHost->UpdateNetworkScore(nai, ival);
            }
            break;
        }
        case INetworkAgent::EVENT_UID_RANGES_ADDED: {
            AutoPtr<IInterface> naiObj;
            mHost->mNetworkAgentInfos->Get(replyTo, (IInterface**)&naiObj);
            NetworkAgentInfo* nai = (NetworkAgentInfo*)INetworkAgentInfo::Probe(naiObj);
            if (nai == NULL) {
                Slogg::E("CConnectivityService", "EVENT_UID_RANGES_ADDED from unknown NetworkAgent");
                break;
            }
            Int32 id;
            nai->mNetwork->GetNetId(&id);
            AutoPtr<ArrayOf<IUidRange*> > array;
            IArrayOf* arrObj = IArrayOf::Probe(obj);
            if (arrObj) {
                Int32 size;
                arrObj->GetLength(&size);
                array = ArrayOf<IUidRange*>::Alloc(size);
                for (Int32 i = 0; i < size; ++i) {
                    AutoPtr<IInterface> o;
                    arrObj->Get(i, (IInterface**)&o);
                    array->Set(i, IUidRange::Probe(o));
                }
            }
            ECode ec = mHost->mNetd->AddVpnUidRanges(id, array);
            if (FAILED(ec)) {
                // Never crash!
                Slogg::E("CConnectivityService", "Exception in addVpnUidRanges: %08x", ec);
            }
            break;
        }
        case INetworkAgent::EVENT_UID_RANGES_REMOVED: {
            AutoPtr<IInterface> naiObj;
            mHost->mNetworkAgentInfos->Get(replyTo, (IInterface**)&naiObj);
            NetworkAgentInfo* nai = (NetworkAgentInfo*)INetworkAgentInfo::Probe(naiObj);
            if (nai == NULL) {
                Slogg::E("CConnectivityService", "EVENT_UID_RANGES_REMOVED from unknown NetworkAgent");
                break;
            }

            Int32 id;
            nai->mNetwork->GetNetId(&id);
            AutoPtr<ArrayOf<IUidRange*> > array;
            IArrayOf* arrObj = IArrayOf::Probe(obj);
            if (arrObj) {
                Int32 size;
                arrObj->GetLength(&size);
                array = ArrayOf<IUidRange*>::Alloc(size);
                for (Int32 i = 0; i < size; ++i) {
                    AutoPtr<IInterface> o;
                    arrObj->Get(i, (IInterface**)&o);
                    array->Set(i, IUidRange::Probe(o));
                }
            }
            ECode ec = mNetd->RemoveVpnUidRanges(id, array);
            if (FAILED(ec)) {
                // Never crash!
                Slogg::E("CConnectivityService", "Exception in removeVpnUidRanges:　%08x", ec);
            }
            break;
        }
        case INetworkAgent::EVENT_SET_EXPLICITLY_SELECTED: {
            AutoPtr<IInterface> naiObj;
            mHost->mNetworkAgentInfos->Get(replyTo, (IInterface**)&naiObj);
            NetworkAgentInfo* nai = (NetworkAgentInfo*)INetworkAgentInfo::Probe(naiObj);
            if (nai == NULL) {
                Slogg::E("CConnectivityService", "EVENT_SET_EXPLICITLY_SELECTED from unknown NetworkAgent");
                break;
            }
            Boolean bval;
            nai->mNetworkMisc->GetExplicitlySelected(&bval);
            if (nai->mCreated && !bval) {
                Slogg::E("CConnectivityService", "ERROR: created network explicitly selected.");
            }
            nai->mNetworkMisc->SetExplicitlySelected(TRUE);
            break;
        }
        case INetworkMonitor::EVENT_NETWORK_TESTED: {
            AutoPtr<IInterface> naiObj;
            mHost->mNetworkAgentInfos->Get(replyTo, (IInterface**)&naiObj);
            NetworkAgentInfo* nai = (NetworkAgentInfo*)INetworkAgentInfo::Probe(naiObj);
            if (mHost->IsLiveNetworkAgent(nai, String("EVENT_NETWORK_VALIDATED"))) {
                Boolean valid = (arg1 == INetworkMonitor::NETWORK_TEST_RESULT_VALID);
                if (valid) {
                    if (DBG) Slogg::D("CConnectivityService", "Validated %s", nai->Name().string());
                    Boolean previouslyValidated = nai->mValidated;
                    Int32 previousScore, curScore;
                    nai->GetCurrentScore(&previousScore);
                    nai->mValidated = TRUE;
                    mHost->RematchNetworkAndRequests(nai, !previouslyValidated);
                    // If score has changed, rebroadcast to NetworkFactories. b/17726566
                    nai->GetCurrentScore(&curScore);
                    if (curScore != previousScore) {
                        mHost->SendUpdatedScoreToFactories(nai);
                    }
                }
                mHost->UpdateInetCondition(nai, valid);
                // Let the NetworkAgent know the state of its network
                nai->mAsyncChannel->SendMessage(
                    INetworkAgent::CMD_REPORT_NETWORK_STATUS,
                    (valid ? INetworkAgent::VALID_NETWORK : INetworkAgent::INVALID_NETWORK),
                    0, NULL);
            }
            break;
        }
        case INetworkMonitor::EVENT_NETWORK_LINGER_COMPLETE: {
            AutoPtr<IInterface> naiObj;
            mHost->mNetworkAgentInfos->Get(replyTo, (IInterface**)&naiObj);
            NetworkAgentInfo* nai = (NetworkAgentInfo*)INetworkAgentInfo::Probe(naiObj);
            if (IsLiveNetworkAgent(nai, String("EVENT_NETWORK_LINGER_COMPLETE"))) {
                mHost->HandleLingerComplete(nai);
            }
            break;
        }
        case INetworkMonitor::EVENT_PROVISIONING_NOTIFICATION: {
            if (arg1 == 0) {
                mHost->setProvNotificationVisibleIntent(FALSE, arg2, 0, NULL, NULL);
            } else {
                AutoPtr<INetworkAgentInfo> nai;
                ISynchronize* lockObj = ISynchronize::Probe(mNetworkForNetId);
                synchronized(mNetworkForNetId) {
                    AutoPtr<IInterface> obj;
                    mNetworkForNetId->Get(arg2, (IInterface**)&obj);;
                    nai = INetworkAgentInfo::Probe(obj);
                }
                if (nai == NULL) {
                    Slogg::E("CConnectivityService", "EVENT_PROVISIONING_NOTIFICATION from unknown NetworkMonitor");
                    break;
                }

                Int32 type;
                nai->mNetworkInfo->GetType(&type);
                String extraInfo;
                nai->mNetworkInfo->GetExtraInfo(&extraInfo);

                mHost->SetProvNotificationVisibleIntent(
                    TRUE, arg2, type, extraInfo, IPendingIntent::Probe(obj));
            }
            break;
        }
        case NetworkStateTracker.EVENT_STATE_CHANGED: {
            info = INetworkInfo::Probe(obj);
            NetworkInfoState state;
            info->GetState(&state);

            if (VDBG || (state == NetworkInfoState_CONNECTED) ||
                    (state == NetworkInfoState_DISCONNECTED) ||
                    (state == NetworkInfoState_SUSPENDED)) {
                String name;
                info->GetTypeName(&name);
                NetworkDetailedState detailedState;
                info->GetDetailedState(&detailedState);
                Slogg::D("CConnectivityService", "ConnectivityChange for %s: %d/%%d",
                    name.string(), state, detailedState)
            }

            // EventLogTags.writeConnectivityStateChanged(
            //         info.getType(), info.getSubtype(), info.getDetailedState().ordinal());

            Boolean bval;
            info->IsConnectedToProvisioningNetwork(&bval);
            if (bval) {
                /**
                 * TODO: Create ConnectivityManager.TYPE_MOBILE_PROVISIONING
                 * for now its an in between network, its a network that
                 * is actually a default network but we don't want it to be
                 * announced as such to keep background applications from
                 * trying to use it. It turns out that some still try so we
                 * take the additional step of clearing any default routes
                 * to the link that may have incorrectly setup by the lower
                 * levels.
                 */
                Int32 type;
                info->GetType(&type);
                AutoPtr<ILinkProperties> lp = mHost->GetLinkPropertiesForTypeInternal(type);
                if (DBG) {
                    Slogg::D("CConnectivityService", "EVENT_STATE_CHANGED: connected to provisioning network, lp=%s",
                        Object::ToString(lp));
                }

                // Clear any default routes setup by the radio so
                // any activity by applications trying to use this
                // connection will fail until the provisioning network
                // is enabled.
                /*
                for (RouteInfo r : lp.getRoutes()) {
                    removeRoute(lp, r, TO_DEFAULT_TABLE,
                                mNetTrackers[info.getType()].getNetwork().netId);
                }
                */
            } else if (state == NetworkInfoState_DISCONNECTED) {
            } else if (state == NetworkInfoState_SUSPENDED) {
            } else if (state == NetworkInfoState_CONNECTED) {
            //    handleConnect(info);
            }
            mHost->NotifyLockdownVpn(NULL);
            break;
        }
        case NetworkStateTracker.EVENT_CONFIGURATION_CHANGED: {
            // TODO: Temporary allowing network configuration
            //       change not resetting sockets.
            //       @see bug/4455071
            /*
            info = INetworkInfo::Probe(obj);
            handleConnectivityChange(info.getType(), mCurrentLinkProperties[info.getType()],
                    FALSE);
            */
            break;
        }
    }
    return NOERROR;
}

//==============================================================================
// CConnectivityService::SettingsObserver
//==============================================================================
CConnectivityService::SettingsObserver::SettingsObserver(
    /* [in] */ IHandler* handler,
    /* [in] */ Int32 what,
    /* [in] */ CConnectivityService* host)
    : ContentObserver(handler)
    , mHandler(handler)
    , mWhat(what)
    , mHost(host)
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
// CConnectivityService
//==============================================================================

CAR_INTERFACE_IMPL_2(CConnectivityService, Object, IIConnectivityManager, IBinder)

CAR_OBJECT_IMPL(CConnectivityService)

CConnectivityService::CConnectivityService()
    : mLockdownEnabled(FALSE)
    , mNetworkPreference(0)
    , mActiveDefaultNetwork(TYPE_NONE)
    , mDefaultInetConditionPublished(0)
    , mNumDnsEntries(0)
    , mTestMode(FALSE)
    , mSystemReady(FALSE)
    , mNetTransitionWakeLockCausedBy("")
    , mNetTransitionWakeLockSerialNumber(0)
    , mNetTransitionWakeLockTimeout(0)
    , mDefaultProxyDisabled(FALSE)
    , mNetworksDefined(0)
    , mNextNetId(MIN_NET_ID)
    , mNextNetworkRequestId(-1)
    , mIsNotificationVisible(FALSE)
    , mGlobalProxy(NULL)
    , mNetworksDefined(0)
{
    mVpnCallback = new VpnCallback(this);

    CLegacyTypeTracker::((ILegacyTypeTracker**)&mLegacyTypeTracker);
    CFile::New(PROVISIONING_URL_PATH, (IFile**)&mProvisioningUrlFile);
    CAtomicInteger32::New(0, (IAtomicInteger32**)&mEnableFailFastMobileDataTag);

    CHashMap::New((IHashMap**)&mNetworkFactoryInfos);
    CHashMap::New((IHashMap**)&mNetworkRequests);

    CSparseArray::New((ISparseArray**)&mNetworkForRequestId);
    CSparseArray::New((ISparseArray**)&mNetworkForNetId);

    CHashMap::New((IHashMap**)&mNetworkAgentInfos);

    CNetworkPolicyListener::New((Handle32)this, (INetworkPolicyListener**)&mPolicyListener);

    mUserPresentReceiver = new InternalBroadcastReciver(this);
    CDataActivityObserver::New(this, (INetworkManagementEventObserver**)&mDataActivityObserver);
}

CConnectivityService::~CConnectivityService()
{
    mNetRequestersPids = NULL;
    mInetLog = NULL;
}

ECode CConnectivityService::constructor(
    /* [in] */ IContext* context,
    /* [in] */ IINetworkManagementService* netd,
    /* [in] */ INetworkStatsService* statsService,
    /* [in] */ IINetworkPolicyManager* policyManager)
{
    if (DBG) Slogger::D(TAG, "ConnectivityService starting up!");

    AutoPtr<INetworkCapabilities> netCap;
    CNetworkCapabilities::New((INetworkCapabilities**)&netCap);
    netCap->AddCapability(INetworkCapabilities::NET_CAPABILITY_INTERNET);
    netCap->AddCapability(INetworkCapabilities::NET_CAPABILITY_NOT_RESTRICTED);
    CNetworkRequest::New(netCap, TYPE_NONE, NextNetworkRequestId(), (INetworkRequest**)&mDefaultRequest);
    AutoPtr<IBinder> binder;
    CBinder::New((IBinder**)&binder);
    AutoPtr<INetworkRequestInfo> nri;
    CNetworkRequestInfo::New(NULL, mDefaultRequest, binder,
        INetworkRequestInfo::REQUEST, (INetworkRequestInfo**)&nri);
    mNetworkRequests->Put(TO_IINTERFACE(mDefaultRequest), TO_IINTERFACE(nri));

    AutoPtr<IHandlerThread> handlerThread;
    CHandlerThread::New(String("ConnectivityServiceThread"), (IHandlerThread**)&handlerThread);
    handlerThread->Start();
    AutoPtr<ILooper> looper;
    handlerThread->GetLooper((ILooper**)&looper);
    mHandler = new InternalHandler(looper, this);
    mTrackerHandler = new NetworkStateTrackerHandler(looper, this);

    // setup our unique device name
    AutoPtr<IContentResolver> resolver;
    context->GetContentResolver((IContentResolver**)&resolver);
    AutoPtr<ISystemProperties> sysProp;
    CSystemProperties::AcquireSingleton((ISystemProperties**)&sysProp);
    String prop;
    sysProp->Get(String("net.hostname"), &prop);
    if (TextUtils::IsEmpty(prop)) {
        String id;
        AutoPtr<ISettingsSecure> settingsSecure;
        CSettingsSecure::AcquireSingleton((ISettingsSecure**)&settingsSecure);
        settingsSecure->GetString(resolver, ISettingsSystem::ANDROID_ID, &id);
        if (!id.IsNullOrEmpty()) {
            String name("android-");
            name += id;
            sysProp->Set(String("net.hostname"), name);
        }
    }

    // read our default dns server ip
    String dns;
    AutoPtr<ISettingsGlobal> settingsGlobal;
    CSettingsGlobal::AcquireSingleton((ISettingsGlobal**)&settingsGlobal);
    settingsGlobal->GetString(resolver, ISettingsGlobal::DEFAULT_DNS_SERVER, &dns);
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
    AutoPtr<IKeyStoreHelper> ksHelper;
    CKeyStoreHelper::AcquireSingleton((IKeyStoreHelper**)&ksHelper);
    ksHelper->GetInstance((IKeyStore**)&mKeyStore);
    AutoPtr<IInterface> service;
    mContext->GetSystemService(IContext::TELEPHONY_SERVICE, (IInterface**)&service);
    mTelephonyManager = ITelephonyManager::Probe(service);

    ec = mPolicyManager->RegisterListener(mPolicyListener);
    if (FAILED(ec)) {
        // ouch, no rules updates means some processes may never get network
        Slogger::E(TAG, "unable to register INetworkPolicyListener");
    }

    service = NULL;
    context->GetSystemService(IContext::POWER_SERVICE, (IInterface**)&service);
    AutoPtr<IPowerManager> powerManager = IPowerManager::Probe(service);
    powerManager->NewWakeLock(IPowerManager::PARTIAL_WAKE_LOCK, String(TAG), (IPowerManagerWakeLock**)&mNetTransitionWakeLock);
    AutoPtr<IResources> resources;
    mContext->GetResources((IResources**)&resources);
    resources->GetInteger(R::integer::config_networkTransitionTimeout, &mNetTransitionWakeLockTimeout);

    mNetTrackers = ArrayOf<INetworkStateTracker*>::Alloc(IConnectivityManager::MAX_NETWORK_TYPE + 1);

    mNetConfigs = ArrayOf<INetworkConfig*>::Alloc(IConnectivityManager::MAX_NETWORK_TYPE + 1);

    // TODO: What is the "correct" way to do determine if this is a wifi only device?
    Boolean wifiOnly = SystemProperties.getBoolean("ro.radio.noril", FALSE);
    Slogg::D("CConnectivityService", "wifiOnly=" + wifiOnly);

    AutoPtr<ArrayOf<String> > naStrings;
    resources->GetStringArray(R::array::networkAttributes, (ArrayOf<String>**)&naStrings);
    for (Int32 i = 0; i < naStrings->GetLength(); i++) {
        String naString = (*naStrings)[i];
        AutoPtr<INetworkConfig> n;
        CNetworkConfig::New(naString, (INetworkConfig**)&n);
        if (VDBG) Slogg::D("CConnectivityService", "naString=" + naString + " config=" + n);

        Int32 nType;
        n->GetType(&nType);
        if (nType > IConnectivityManager::MAX_NETWORK_TYPE) {
            Slogger::E(TAG, "Error in networkAttributes - ignoring attempt to define type %d", nType);
            continue;
        }

        if (wifiOnly && ConnectivityManager.isNetworkTypeMobile(n.type)) {
            Slogg::D("CConnectivityService", "networkAttributes - ignoring mobile as this dev is wifiOnly " +
                    n.type);
            continue;
        }

        if ((*mNetConfigs)[nType] != NULL) {
            Slogger::E(TAG, "Error in networkAttributes - ignoring attempt to redefine type %d", nType);
            continue;
        }

        mLegacyTypeTracker.addSupportedType(n.type);

        mNetConfigs->Set(nType, n);
        mNetworksDefined++;
    }

    if (VDBG) Slogg::D("CConnectivityService", "mNetworksDefined=" + mNetworksDefined);

    AutoPtr< ArrayOf<Int32> > protectedNetworks;
    resources->GetInt32Array(R::array::config_protectedNetworks, (ArrayOf<Int32>**)&protectedNetworks);
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

    String modeStr, typeStr;
    sysProp->Get(String("cm.test.mode"), &modeStr);
    sysProp->Get(String("ro.build.type");
    mTestMode = modeStr.Equals("TRUE") && typeStr.Equals("eng"));

    mTethering = new Tethering(mContext, mNetd, statsService, mHandler.getLooper());

    mPermissionMonitor = new PermissionMonitor(mContext, mNetd);

    //set up the listener for user state for creating user VPNs
    AutoPtr<IIntentFilter> intentFilter;
    CIntentFilter::New((IIntentFilter**)&intentFilter);
    intentFilter->AddAction(IIntent::ACTION_USER_STARTING);
    intentFilter->AddAction(IIntent::ACTION_USER_STOPPING);
    mContext->RegisterReceiverAsUser(
            mUserIntentReceiver, UserHandle::ALL, intentFilter, NULL, NULL);
    AutoPtr<IINetworkManagementEventObserver> clat;
    CNat464Xlat::New(mContext, mNetd, this, mTrackerHandler, (IINetworkManagementEventObserver**)&clat);
    mClat = (Nat464Xlat*)clat.Get();

    //try {
    mNetd->RegisterObserver(mTethering);
    mNetd->RegisterObserver(mDataActivityObserver);
    mNetd->RegisterObserver(mClat);
    //} catch (RemoteException e) {
    //    Loge("Error registering observer :" + e);
    //}

    if (DBG) {
        mInetLog = new List<String>();
    }

    mSettingsObserver = new SettingsObserver(mHandler, EVENT_APPLY_GLOBAL_HTTP_PROXY, this);
    mSettingsObserver->Observe(mContext);

    mDataConnectionStats = new DataConnectionStats(mContext);
    mDataConnectionStats->StartMonitoring();

    service = NULL;
    context->GetSystemService(IContext::ALARM_SERVICE, (IInterface**)&service);
    mAlarmManager = IAlarmManager::Probe(service);

    AutoPtr<IIntentFilter> filter, newFilter;
    CIntentFilter::New((IIntentFilter**)&filter);
    filter->AddAction(CConnectivityService::ACTION_PKT_CNT_SAMPLE_INTERVAL_ELAPSED);
    AutoPtr<IBroadcastReciver> receiver = new PktCntSampleIntervalElapsedeceiver(this);
    CIntentFilter::New(filter, (IIntentFilter**)&newFilter);
    AutoPtr<IIntent> stickyIntent;
    mContext->RegisterReceiver(receiver, newFilter, (IIntent**)&stickyIntent);

    mPacManager = new PacManager(mContext, mHandler, EVENT_PROXY_HAS_CHANGED);

    mUserManager = (UserManager) context.getSystemService(Context.USER_SERVICE);
    return NOERROR;
}


synchronized Int32 CConnectivityService::NextNetworkRequestId()
{
    return mNextNetworkRequestId++;
}

ECode CConnectivityService::AssignNextNetId(
    /* [in] */ INetworkAgentInfo* nai)
{
    synchronized(mNetworkForNetId) {
        for (Int32 i = MIN_NET_ID; i <= MAX_NET_ID; i++) {
            Int32 netId = mNextNetId;
            if (++mNextNetId > MAX_NET_ID) mNextNetId = MIN_NET_ID;
            // Make sure NetID unused.  http://b/16815182
            if (mNetworkForNetId.get(netId) == NULL) {
                nai.network = new Network(netId);
                mNetworkForNetId.put(netId, nai);
                return;
            }
        }
    }
    throw new IllegalStateException("No free netIds");
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

Boolean CConnectivityService::IsNetworkBlocked(
    /* [in] */ Int32 networkType,
    /* [in] */ Int32 uid)
{
    return isNetworkWithLinkPropertiesBlocked(getLinkPropertiesForType(networkType), uid);
}

Boolean CConnectivityService::IsNetworkBlocked(
    /* [in] */ INetworkAgentInfo* nai,
    /* [in] */ Int32 uid)
{
    return isNetworkWithLinkPropertiesBlocked(nai.linkProperties, uid);
}

Boolean CConnectivityService::IsNetworkWithLinkPropertiesBlocked(
    /* [in] */ ILinkProperties* lp,
    /* [in] */ Int32 uid)
{
    Boolean networkCostly = FALSE;
    Int32 uidRules;

    final String iface = (lp == NULL ? "" : lp.getInterfaceName());

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

AutoPtr<INetworkInfo> CConnectivityService::GetFilteredNetworkInfo(
    /* [in] */ Int32 networkType,
    /* [in] */ Int32 uid)
{
    NetworkInfo info = getNetworkInfoForType(networkType);
    return getFilteredNetworkInfo(info, networkType, uid);
}

/*
 * @deprecated Uses mLegacyTypeTracker; cannot deal with multiple Networks of the same type.
 */
AutoPtr<INetworkInfo> CConnectivityService::GetFilteredNetworkInfo(
    /* [in] */ INetworkInfo* info,
    /* [in] */ Int32 networkType,
    /* [in] */ Int32 uid)
{
    if (isNetworkBlocked(networkType, uid)) {
        // network is blocked; clone and override state
        info = new NetworkInfo(info);
        info.setDetailedState(DetailedState.BLOCKED, NULL, NULL);
        if (VDBG) Slogg::D("CConnectivityService", "returning Blocked NetworkInfo");
    }
    if (mLockdownTracker != NULL) {
        info = mLockdownTracker.augmentNetworkInfo(info);
        if (VDBG) Slogg::D("CConnectivityService", "returning Locked NetworkInfo");
    }
    return info;
}

AutoPtr<INetworkInfo> CConnectivityService::GetFilteredNetworkInfo(
    /* [in] */ INetworkAgentInfo* nai,
    /* [in] */ Int32 uid)
{
    NetworkInfo info = nai.networkInfo;
    if (isNetworkBlocked(nai, uid)) {
        // network is blocked; clone and override state
        info = new NetworkInfo(info);
        info.setDetailedState(DetailedState.BLOCKED, NULL, NULL);
        if (DBG) Slogg::D("CConnectivityService", "returning Blocked NetworkInfo");
    }
    if (mLockdownTracker != NULL) {
        info = mLockdownTracker.augmentNetworkInfo(info);
        if (DBG) Slogg::D("CConnectivityService", "returning Locked NetworkInfo");
    }
    return info;
}

ECode CConnectivityService::GetActiveNetworkInfo(
    /* [out] */ INetworkInfo** result)
{
    VALIDATE_NOT_NULL(result);
    *result = NULL;

    FAIL_RETURN(EnforceInternetPermission());
    Int32 uid = Binder::GetCallingUid();
    return GetNetworkInfo(mActiveDefaultNetwork, uid, result);
}

AutoPtr<INetworkInfo> CConnectivityService::GetProvisioningNetworkInfo()
{
    EnforceInternetPermission();

    // Find the first Provisioning Network
    NetworkInfo provNi = NULL;
    for (NetworkInfo ni : getAllNetworkInfo()) {
        if (ni.isConnectedToProvisioningNetwork()) {
            provNi = ni;
            break;
        }
    }
    if (DBG) Slogg::D("CConnectivityService", "getProvisioningNetworkInfo: X provNi=" + provNi);
    return provNi;
}

ECode GetProvisioningOrActiveNetworkInfo(
    /* [out] */ INetworkInfo** info)
{
    EnforceInternetPermission();

    NetworkInfo provNi = getProvisioningNetworkInfo();
    if (provNi == NULL) {
        final Int32 uid = Binder.getCallingUid();
        provNi = getNetworkInfo(mActiveDefaultNetwork, uid);
    }
    if (DBG) Slogg::D("CConnectivityService", "getProvisioningOrActiveNetworkInfo: X provNi=" + provNi);
    return provNi;
}


ECode CConnectivityService::GetActiveNetworkInfoUnfiltered(
    /* [out] */ INetworkInfo** info)
{
    VALIDATE_NOT_NULL(info);
    *info = NULL;

    FAIL_RETURN(EnforceInternetPermission());
    AutoPtr<IConnectivityManagerHelper> helper;
    CConnectivityManagerHelper::AcquireSingleton((IConnectivityManagerHelper**)&helper);
    Boolean isValid = FALSE;

    Boolean isValid = FALSE;
    IsNetworkTypeValid(mActiveDefaultNetwork, &isValid);
    if (isValid) {
        return GetNetworkInfoForType(mActiveDefaultNetwork);
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

    FAIL_RETURN(EnforceInternetPermission());
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

    Boolean isValid = FALSE;
    IsNetworkTypeValid(networkType, &isValid);
    if (isValid) {
        if (getNetworkInfoForType(networkType) != NULL) {
            info = getFilteredNetworkInfo(networkType, uid);
        }
    }
    return NOERROR;
}

ECode CConnectivityService::GetNetworkInfoForNetwork(
    /* [in] */ INetwork* network,
    /* [out] */ INetworkInfo** info)
{
    EnforceInternetPermission();
    if (network == NULL) return NULL;

    final Int32 uid = Binder.getCallingUid();
    NetworkAgentInfo nai = NULL;
    synchronized(mNetworkForNetId) {
        nai = mNetworkForNetId.get(network.netId);
    }
    if (nai == NULL) return NULL;
    synchronized(nai) {
        if (nai.networkInfo == NULL) return NULL;

        return getFilteredNetworkInfo(nai, uid);
    }
}

ECode CConnectivityService::GetAllNetworkInfo(
    /* [out, callee] */ ArrayOf<INetworkInfo*>** allInfo)
{
    VALIDATE_NOT_NULL(allInfo);
    FAIL_RETURN(EnforceInternetPermission());
    Int32 uid = Binder::GetCallingUid();
    List< AutoPtr<INetworkInfo> > result;

    for (Int32 networkType = 0; networkType <= ConnectivityManager.MAX_NETWORK_TYPE;
            networkType++) {
        if (getNetworkInfoForType(networkType) != NULL) {
            result.add(getFilteredNetworkInfo(networkType, uid));
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

ECode CConnectivityService::GetNetworkForType(
    /* [in] */ Int32 networkType,
    /* [out] */ INetwork** network)
{
    EnforceInternetPermission();
    final Int32 uid = Binder.getCallingUid();
    if (isNetworkBlocked(networkType, uid)) {
        return NULL;
    }
    NetworkAgentInfo nai = mLegacyTypeTracker.getNetworkForType(networkType);
    return (nai == NULL) ? NULL : nai.network;
}

ECode CConnectivityService::GetAllNetworks(
    /* [out, callee] */ ArrayOf<INetwork*>** networks)
{
    EnforceInternetPermission();
    final ArrayList<Network> result = new ArrayList();
    synchronized(mNetworkForNetId) {
        for (Int32 i = 0; i < mNetworkForNetId.size(); i++) {
            result.add(new Network(mNetworkForNetId.valueAt(i).network));
        }
    }
    return result.toArray(new Network[result.size()]);
}

ECode CConnectivityService::IsNetworkSupported(
    /* [in] */ Int32 networkType,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
    FAIL_RETURN(EnforceInternetPermission());
    AutoPtr<IConnectivityManagerHelper> helper;
    CConnectivityManagerHelper::AcquireSingleton((IConnectivityManagerHelper**)&helper);
    Boolean isValid = FALSE;
    helper->IsNetworkTypeValid(networkType, &isValid);
    AutoPtr<INetworkInfo> info = GetNetworkInfoForType(networkType);
    *result = (isValid && info != NULL);
    return NOERROR;
}

ECode CConnectivityService::GetActiveLinkProperties(
    /* [out] */ ILinkProperties** properties)
{
    return GetLinkPropertiesForType(mActiveDefaultNetwork, properties);
}

ECode CConnectivityService::GetLinkPropertiesForType(
    /* [in] */ Int32 networkType,
    /* [out] */ ILinkProperties** properties)
{
    EnforceInternetPermission();
    if (IsNetworkTypeValid(networkType)) {
        return getLinkPropertiesForTypeInternal(networkType);
    }
    return NULL;
}

// TODO - this should be ALL networks
ECode CConnectivityService::GetLinkProperties(
    /* [in] */ INetwork* network,
    /* [out] */ ILinkProperties** properties)
{
    EnforceInternetPermission();
    NetworkAgentInfo nai = NULL;
    synchronized(mNetworkForNetId) {
        nai = mNetworkForNetId.get(network.netId);
    }

    if (nai != NULL) {
        synchronized(nai) {
            return new LinkProperties(nai.linkProperties);
        }
    }
    return NULL;
}

ECode CConnectivityService::GetNetworkCapabilities(
    /* [in] */ INetwork* network,
    /* [out] */ INetworkCapabilities** result)
{
    EnforceInternetPermission();
    NetworkAgentInfo nai = NULL;
    synchronized(mNetworkForNetId) {
        nai = mNetworkForNetId.get(network.netId);
    }
    if (nai != NULL) {
        synchronized(nai) {
            return new NetworkCapabilities(nai.networkCapabilities);
        }
    }
    return NULL;
}

ECode CConnectivityService::GetAllNetworkState(
    /* [out, callee] */ ArrayOf<INetworkState*>** allStates)
{
    VALIDATE_NOT_NULL(allStates);
    *allStates = NULL;

    FAIL_RETURN(EnforceInternetPermission());
    Int32 uid = Binder::GetCallingUid();

    final ArrayList<NetworkState> result = Lists.newArrayList();
    for (Int32 networkType = 0; networkType <= ConnectivityManager.MAX_NETWORK_TYPE;
            networkType++) {
        if (getNetworkInfoForType(networkType) != NULL) {
            final NetworkInfo info = getFilteredNetworkInfo(networkType, uid);
            final LinkProperties lp = getLinkPropertiesForTypeInternal(networkType);
            final NetworkCapabilities netcap = getNetworkCapabilitiesForType(networkType);
            result.add(new NetworkState(info, lp, netcap));
        }
    }

    result.toArray(new NetworkState[result.size()]);

    // List< AutoPtr<INetworkState> > result;
    // AutoPtr< ArrayOf<INetworkState*> > stateArray = ArrayOf<INetworkState*>::Alloc(result.GetSize());
    // List< AutoPtr<INetworkState> >::Iterator it = result.Begin();
    // for (Int32 i = 0; it != result.End(); ++it, ++i) {
    //     AutoPtr<INetworkState> netState = *it;
    //     stateArray->Set(i, netState);
    // }
    // *allStates = stateArray;
    // REFCOUNT_ADD(*allStates);
    return NOERROR;
}

AutoPtr<INetworkState> CConnectivityService::GetNetworkStateUnchecked(
    /* [in] */ Int32 networkType)
{
    Boolean isValid = FALSE;
    IsNetworkTypeValid(networkType, &isValid);
    if (isValid) {
        NetworkInfo info = getNetworkInfoForType(networkType);
        if (info != NULL) {
            return new NetworkState(info,
                    getLinkPropertiesForTypeInternal(networkType),
                    getNetworkCapabilitiesForType(networkType));
        }
    }
    return NULL;
}

ECode CConnectivityService::GetActiveNetworkQuotaInfo(
    /* [out] */ INetworkQuotaInfo** info)
{
    VALIDATE_NOT_NULL(info);
    *info = NULL;

    FAIL_RETURN(EnforceInternetPermission());

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
    FAIL_RETURN(EnforceInternetPermission());
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

/**
* Ensure that a network route exists to deliver traffic to the specified
* host via the specified network interface.
* @param networkType the type of the network over which traffic to the
* specified host is to be routed
* @param hostAddress the IP address of the host to which the route is
* desired
* @return {@code TRUE} on success, {@code FALSE} on failure
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

    InetAddress addr;
    try {
        addr = InetAddress.getByAddress(hostAddress);
    } catch (UnknownHostException e) {
        if (DBG) Slogg::D("CConnectivityService", "requestRouteToHostAddress got " + e.toString());
        return FALSE;
    }

    AutoPtr<IConnectivityManagerHelper> helper;
    CConnectivityManagerHelper::AcquireSingleton((IConnectivityManagerHelper**)&helper);
    Boolean isValid;
    if ((helper->IsNetworkTypeValid(networkType, &isValid), !isValid)) {
        if (DBG) Slogger::D(TAG, "requestRouteToHostAddress on invalid network: %d", networkType);
        *result = FALSE;
        return NOERROR;
    }

    NetworkAgentInfo nai = mLegacyTypeTracker.getNetworkForType(networkType);
    if (nai == NULL) {
        if (mLegacyTypeTracker.IsTypeSupported(networkType) == FALSE) {
            if (DBG) Slogg::D("CConnectivityService", "requestRouteToHostAddress on unsupported network: " + networkType);
        } else {
            if (DBG) Slogg::D("CConnectivityService", "requestRouteToHostAddress on down network: " + networkType);
        }
        return FALSE;
    }

    DetailedState netState;
    synchronized(nai) {
        netState = nai.networkInfo.getDetailedState();
    }

    if (netState != DetailedState.CONNECTED && netState != DetailedState.CAPTIVE_PORTAL_CHECK) {
        if (VDBG) {
            Slogg::D("CConnectivityService", "requestRouteToHostAddress on down network "
                    + "(" + networkType + ") - dropped"
                    + " netState=" + netState);
        }
        return FALSE;
    }

    final Int32 uid = Binder.getCallingUid();
    final long token = Binder.clearCallingIdentity();
    try {
        LinkProperties lp;
        Int32 netId;
        synchronized(nai) {
            lp = nai.linkProperties;
            netId = nai.network.netId;
        }
        Boolean ok = addLegacyRouteToHost(lp, addr, netId, uid);
        if (DBG) Slogg::D("CConnectivityService", "requestRouteToHostAddress ok=" + ok);
        return ok;
    } finally {
        Binder.restoreCallingIdentity(token);
    }
    return NOERROR;
}

Boolean addLegacyRouteToHost(LinkProperties lp, InetAddress addr, Int32 netId, Int32 uid) {
    RouteInfo bestRoute = RouteInfo.selectBestRoute(lp.getAllRoutes(), addr);
    if (bestRoute == NULL) {
        bestRoute = RouteInfo.makeHostRoute(addr, lp.getInterfaceName());
    } else {
        String iface = bestRoute.getInterface();
        if (bestRoute.getGateway().Equals(addr)) {
            // if there is no better route, add the implied hostroute for our gateway
            bestRoute = RouteInfo.makeHostRoute(addr, iface);
        } else {
            // if we will connect to this through another route, add a direct route
            // to it's gateway
            bestRoute = RouteInfo.makeHostRoute(addr, bestRoute.getGateway(), iface);
        }
    }
    if (DBG) Slogg::D("CConnectivityService", "Adding " + bestRoute + " for interface " + bestRoute.getInterface());
    try {
        mNetd.addLegacyRouteForNetId(netId, bestRoute, uid);
    } catch (Exception e) {
        // never crash - catch them all
        if (DBG) Slogg::E("CConnectivityService", "Exception trying to add a route: " + e);
        return FALSE;
    }
    return TRUE;
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


void HandleAsyncChannelHalfConnect(
    /* [in] */ IMessage* msg)
{
    AsyncChannel ac = (AsyncChannel) msg.obj;
    if (mNetworkFactoryInfos.containsKey(msg.replyTo)) {
        if (msg.arg1 == IAsyncChannel::STATUS_SUCCESSFUL) {
            if (VDBG) Slogg::D("CConnectivityService", "NetworkFactory connected");
            // A network factory has connected.  Send it all current NetworkRequests.
            for (NetworkRequestInfo nri : mNetworkRequests.values()) {
                if (nri.isRequest == FALSE) continue;
                NetworkAgentInfo nai = mNetworkForRequestId.get(nri.request.requestId);
                ac.sendMessage(android.net.NetworkFactory.CMD_REQUEST_NETWORK,
                        (nai != NULL ? nai.getCurrentScore() : 0), 0, nri.request);
            }
        } else {
            Slogg::E("CConnectivityService", "Error connecting NetworkFactory");
            mNetworkFactoryInfos.remove(msg.obj);
        }
    } else if (mNetworkAgentInfos.containsKey(msg.replyTo)) {
        if (msg.arg1 == IAsyncChannel::STATUS_SUCCESSFUL) {
            if (VDBG) Slogg::D("CConnectivityService", "NetworkAgent connected");
            // A network agent has requested a connection.  Establish the connection.
            mNetworkAgentInfos.get(msg.replyTo).asyncChannel.
                    sendMessage(IAsyncChannel::CMD_CHANNEL_FULL_CONNECTION);
        } else {
            Slogg::E("CConnectivityService", "Error connecting NetworkAgent");
            NetworkAgentInfo nai = mNetworkAgentInfos.remove(msg.replyTo);
            if (nai != NULL) {
                synchronized(mNetworkForNetId) {
                    mNetworkForNetId.remove(nai.network.netId);
                }
                // Just in case.
                mLegacyTypeTracker.remove(nai);
            }
        }
    }
}

void HandleAsyncChannelDisconnected(
    /* [in] */ IMessage* msg)
{
    NetworkAgentInfo nai = mNetworkAgentInfos.get(msg.replyTo);
    if (nai != NULL) {
        if (DBG) {
            Slogg::D("CConnectivityService", nai.name() + " got DISCONNECTED, was satisfying " + nai.networkRequests.size());
        }
        // A network agent has disconnected.
        if (nai.created) {
            // Tell netd to clean up the configuration for this network
            // (routing rules, DNS, etc).
            try {
                mNetd.removeNetwork(nai.network.netId);
            } catch (Exception e) {
                Slogg::E("CConnectivityService", "Exception removing network: " + e);
            }
        }
        // TODO - if we move the logic to the network agent (have them disconnect
        // because they lost all their requests or because their score isn't good)
        // then they would disconnect organically, report their new state and then
        // disconnect the channel.
        if (nai.networkInfo.isConnected()) {
            nai.networkInfo.setDetailedState(NetworkInfo.DetailedState.DISCONNECTED,
                    NULL, NULL);
        }
        if (IsDefaultNetwork(nai)) {
            mDefaultInetConditionPublished = 0;
        }
        notifyNetworkCallbacks(nai, ConnectivityManager.CALLBACK_LOST);
        nai.networkMonitor.sendMessage(INetworkMonitor::CMD_NETWORK_DISCONNECTED);
        mNetworkAgentInfos.remove(msg.replyTo);
        updateClat(NULL, nai.linkProperties, nai);
        mLegacyTypeTracker.remove(nai);
        synchronized(mNetworkForNetId) {
            mNetworkForNetId.remove(nai.network.netId);
        }
        // Since we've lost the network, go through all the requests that
        // it was satisfying and see if any other factory can satisfy them.
        // TODO: This logic may be better replaced with a call to rematchAllNetworksAndRequests
        final ArrayList<NetworkAgentInfo> toActivate = new ArrayList<NetworkAgentInfo>();
        for (Int32 i = 0; i < nai.networkRequests.size(); i++) {
            NetworkRequest request = nai.networkRequests.valueAt(i);
            NetworkAgentInfo currentNetwork = mNetworkForRequestId.get(request.requestId);
            if (currentNetwork != NULL && currentNetwork.network.netId == nai.network.netId) {
                if (DBG) {
                    Slogg::D("CConnectivityService", "Checking for replacement network to handle request " + request );
                }
                mNetworkForRequestId.remove(request.requestId);
                sendUpdatedScoreToFactories(request, 0);
                NetworkAgentInfo alternative = NULL;
                for (Map.Entry entry : mNetworkAgentInfos.entrySet()) {
                    NetworkAgentInfo existing = (NetworkAgentInfo)entry.getValue();
                    if (existing.networkInfo.isConnected() &&
                            request.networkCapabilities.satisfiedByNetworkCapabilities(
                            existing.networkCapabilities) &&
                            (alternative == NULL ||
                             alternative.getCurrentScore() < existing.getCurrentScore())) {
                        alternative = existing;
                    }
                }
                if (alternative != NULL) {
                    if (DBG) Slogg::D("CConnectivityService", " found replacement in " + alternative.name());
                    if (!toActivate.contains(alternative)) {
                        toActivate.add(alternative);
                    }
                }
            }
        }
        if (nai.networkRequests.get(mDefaultRequest.requestId) != NULL) {
            removeDataActivityTracking(nai);
            mActiveDefaultNetwork = ConnectivityManager.TYPE_NONE;
            NotifyLockdownVpn(nai);
            requestNetworkTransitionWakelock(nai.name());
        }
        for (NetworkAgentInfo networkToActivate : toActivate) {
            networkToActivate.networkLingered.clear();
            networkToActivate.networkMonitor.sendMessage(INetworkMonitor::CMD_NETWORK_CONNECTED);
            rematchNetworkAndRequests(networkToActivate, FALSE);
        }
    }
}

void HandleRegisterNetworkRequest(
    /* [in] */ IMessage* msg)
{
    final NetworkRequestInfo nri = (NetworkRequestInfo) (msg.obj);
    final NetworkCapabilities newCap = nri.request.networkCapabilities;
    Int32 score = 0;

    // Check for the best currently alive network that satisfies this request
    NetworkAgentInfo bestNetwork = NULL;
    for (NetworkAgentInfo network : mNetworkAgentInfos.values()) {
        if (DBG) Slogg::D("CConnectivityService", "handleRegisterNetworkRequest checking " + network.name());
        if (newCap.satisfiedByNetworkCapabilities(network.networkCapabilities)) {
            if (DBG) Slogg::D("CConnectivityService", "apparently satisfied.  currentScore=" + network.getCurrentScore());
            if ((bestNetwork == NULL) ||
                    bestNetwork.getCurrentScore() < network.getCurrentScore()) {
                if (!nri.isRequest) {
                    // Not setting bestNetwork here as a listening NetworkRequest may be
                    // satisfied by multiple Networks.  Instead the request is added to
                    // each satisfying Network and notified about each.
                    network.addRequest(nri.request);
                    notifyNetworkCallback(network, nri);
                } else {
                    bestNetwork = network;
                }
            }
        }
    }
    if (bestNetwork != NULL) {
        if (DBG) Slogg::D("CConnectivityService", "using " + bestNetwork.name());
        if (bestNetwork.networkInfo.isConnected()) {
            // Cancel any lingering so the linger timeout doesn't teardown this network
            // even though we have a request for it.
            bestNetwork.networkLingered.clear();
            bestNetwork.networkMonitor.sendMessage(INetworkMonitor::CMD_NETWORK_CONNECTED);
        }
        // TODO: This logic may be better replaced with a call to rematchNetworkAndRequests
        bestNetwork.addRequest(nri.request);
        mNetworkForRequestId.put(nri.request.requestId, bestNetwork);
        notifyNetworkCallback(bestNetwork, nri);
        score = bestNetwork.getCurrentScore();
        if (nri.request.legacyType != TYPE_NONE) {
            mLegacyTypeTracker.add(nri.request.legacyType, bestNetwork);
        }
    }
    mNetworkRequests.put(nri.request, nri);
    if (nri.isRequest) {
        if (DBG) Slogg::D("CConnectivityService", "sending new NetworkRequest to factories");
        for (NetworkFactoryInfo nfi : mNetworkFactoryInfos.values()) {
            nfi.asyncChannel.sendMessage(android.net.NetworkFactory.CMD_REQUEST_NETWORK, score,
                    0, nri.request);
        }
    }
}

void HandleReleaseNetworkRequest(
    /* [in] */ INetworkRequest* request,
    /* [in] */ Int32 callingUid)
{
    NetworkRequestInfo nri = mNetworkRequests.get(request);
    if (nri != NULL) {
        if (Process.SYSTEM_UID != callingUid && nri.mUid != callingUid) {
            if (DBG) Slogg::D("CConnectivityService", "Attempt to release unowned NetworkRequest " + request);
            return;
        }
        if (DBG) Slogg::D("CConnectivityService", "releasing NetworkRequest " + request);
        nri.unlinkDeathRecipient();
        mNetworkRequests.remove(request);
        if (nri.isRequest) {
            // Find all networks that are satisfying this request and remove the request
            // from their request lists.
            for (NetworkAgentInfo nai : mNetworkAgentInfos.values()) {
                if (nai.networkRequests.get(nri.request.requestId) != NULL) {
                    nai.networkRequests.remove(nri.request.requestId);
                    if (DBG) {
                        Slogg::D("CConnectivityService", " Removing from current network " + nai.name() +
                                ", leaving " + nai.networkRequests.size() +
                                " requests.");
                    }
                    // check if has any requests remaining and if not,
                    // disconnect (unless it's a VPN).
                    Boolean keep = nai.isVPN();
                    for (Int32 i = 0; i < nai.networkRequests.size() && !keep; i++) {
                        NetworkRequest r = nai.networkRequests.valueAt(i);
                        if (isRequest(r)) keep = TRUE;
                    }
                    if (!keep) {
                        if (DBG) Slogg::D("CConnectivityService", "no live requests for " + nai.name() + "; disconnecting");
                        nai.asyncChannel.disconnect();
                    }
                }
            }

            // Maintain the illusion.  When this request arrived, we might have preteneded
            // that a network connected to serve it, even though the network was already
            // connected.  Now that this request has gone away, we might have to pretend
            // that the network disconnected.  LegacyTypeTracker will generate that
            // phatom disconnect for this type.
            NetworkAgentInfo nai = mNetworkForRequestId.get(nri.request.requestId);
            if (nai != NULL) {
                mNetworkForRequestId.remove(nri.request.requestId);
                if (nri.request.legacyType != TYPE_NONE) {
                    mLegacyTypeTracker.remove(nri.request.legacyType, nai);
                }
            }

            for (NetworkFactoryInfo nfi : mNetworkFactoryInfos.values()) {
                nfi.asyncChannel.sendMessage(android.net.NetworkFactory.CMD_CANCEL_REQUEST,
                        nri.request);
            }
        } else {
            // listens don't have a singular affectedNetwork.  Check all networks to see
            // if this listen request applies and remove it.
            for (NetworkAgentInfo nai : mNetworkAgentInfos.values()) {
                nai.networkRequests.remove(nri.request.requestId);
            }
        }
        callCallbackForRequest(nri, NULL, ConnectivityManager.CALLBACK_RELEASED);
    }
}

void CConnectivityService::HandleSetDependencyMet(
    /* [in] */ Int32 networkType,
    /* [in] */ Boolean met)
{
    if ((*mNetTrackers)[networkType] != NULL) {
        if (DBG) {
           Slogger::D(TAG, "HandleSetDependencyMet(%d, %s)", networkType, met ? "TRUE" : "FALSE");
        }
        (*mNetTrackers)[networkType]->SetDependencyMet(met);
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
    // TODO - handle this passing to factories
    // AutoPtr<IConnectivityManagerHelper> helper;
    // CConnectivityManagerHelper::AcquireSingleton((IConnectivityManagerHelper**)&helper);
    // Boolean isValid = FALSE;
    // if ((helper->IsNetworkTypeValid(networkType, &isValid), isValid)) {
    //     AutoPtr<INetworkStateTracker> tracker = (*mNetTrackers)[networkType];
    //     if (tracker != NULL) {
    //         tracker->SetPolicyDataEnable(enabled);
    //     }
    // }
}

ECode CConnectivityService::EnforceInternetPermission()
{
    return mContext->EnforceCallingOrSelfPermission(
            Elastos::Droid::Manifest::permission::INTERNET,
            String("ConnectivityService"));
}

ECode CConnectivityService::EnforceChangePermission()
{
    return mContext->EnforceCallingOrSelfPermission(
            Elastos::Droid::Manifest::permission::ACCESS_NETWORK_STATE,
            String("ConnectivityService"));
}

// TODO Make this a special check when it goes public
ECode CConnectivityService::EnforceChangePermission()
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

ECode CConnectivityService::SendConnectedBroadcast(
    /* [in] */ INetworkInfo* info)
{
    FAIL_RETURN(EnforceConnectivityInternalPermission())
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
    /* [in] */ Boolean active,
    /* [in] */ Int64 tsNanos)
{
    AutoPtr<IIntent> intent;
    CIntent::New(IConnectivityManager::ACTION_DATA_ACTIVITY_CHANGE, (IIntent**)&intent);
    intent->PutExtra(IConnectivityManager::EXTRA_DEVICE_TYPE, deviceType);
    intent->PutBooleanExtra(IConnectivityManager::EXTRA_IS_ACTIVE, active);
    intent->PutExtra(IConnectivityManager::EXTRA_REALTIME_NS, tsNanos);
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

void CConnectivityService::SendStickyBroadcast(
    /* [in] */ IIntent* intent)
{
    AutoLock lock(this);
    if (!mSystemReady) {
        mInitialBroadcast = NULL;
        CIntent::New(intent, (IIntent**)&mInitialBroadcast);
    }
    intent->AddFlags(IIntent::FLAG_RECEIVER_REGISTERED_ONLY_BEFORE_BOOT);
    if (DBG) {
       String action;
       intent->GetAction(&action);
       Slogger::V(TAG, "SendStickyBroadcast: action=%s", action.string());
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
    // start network sampling ..
    Intent intent = new Intent(ACTION_PKT_CNT_SAMPLE_INTERVAL_ELAPSED);
    intent.setPackage(mContext.getPackageName());

    mSampleIntervalElapsedIntent = PendingIntent.getBroadcast(mContext,
            SAMPLE_INTERVAL_ELAPSED_REQUEST_CODE, intent, 0);
    setAlarm(DEFAULT_START_SAMPLING_INTERVAL_IN_SECONDS * 1000, mSampleIntervalElapsedIntent);

    loadGlobalProxy();

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


    mHandler.sendMessage(mHandler.obtainMessage(EVENT_SYSTEM_READY));

    mPermissionMonitor.startMonitoring();
}


/** @hide */
ECode CConnectivityService::CaptivePortalCheckCompleted(
    /* [in] */ INetworkInfo* info,
    /* [in] */ Boolean isCaptivePortal)
{
    enforceConnectivityInternalPermission();
    if (DBG) Slogg::D("CConnectivityService", "captivePortalCheckCompleted: ni=" + info + " captive=" + isCaptivePortal);
//        mNetTrackers[info.getType()].captivePortalCheckCompleted(isCaptivePortal);
}


void CConnectivityService::SetupDataActivityTracking(
    /* [in] */ INetworkAgentInfo* networkAgent)
{
    final String iface = networkAgent.linkProperties.getInterfaceName();

    final Int32 timeout;
    Int32 type = ConnectivityManager.TYPE_NONE;

    if (networkAgent.networkCapabilities.hasTransport(
            NetworkCapabilities.TRANSPORT_CELLULAR)) {
        timeout = Settings.Global.getInt(mContext.getContentResolver(),
                                         Settings.Global.DATA_ACTIVITY_TIMEOUT_MOBILE,
                                         5);
        type = ConnectivityManager.TYPE_MOBILE;
    } else if (networkAgent.networkCapabilities.hasTransport(
            NetworkCapabilities.TRANSPORT_WIFI)) {
        timeout = Settings.Global.getInt(mContext.getContentResolver(),
                                         Settings.Global.DATA_ACTIVITY_TIMEOUT_WIFI,
                                         0);
        type = ConnectivityManager.TYPE_WIFI;
    } else {
        // do not track any other networks
        timeout = 0;
    }

    if (timeout > 0 && iface != NULL && type != ConnectivityManager.TYPE_NONE) {
        try {
            mNetd.addIdleTimer(iface, timeout, type);
        } catch (Exception e) {
            // You shall not crash!
            Slogg::E("CConnectivityService", "Exception in setupDataActivityTracking " + e);
        }
    }
}

void CConnectivityService::RemoveDataActivityTracking(
    /* [in] */ INetworkAgentInfo* networkAgent)
{
    final String iface = networkAgent.linkProperties.getInterfaceName();
    final NetworkCapabilities caps = networkAgent.networkCapabilities;

    if (iface != NULL && (caps.hasTransport(NetworkCapabilities.TRANSPORT_CELLULAR) ||
                          caps.hasTransport(NetworkCapabilities.TRANSPORT_WIFI))) {
        try {
            // the call fails silently if no idletimer setup for this interface
            mNetd.removeIdleTimer(iface);
        } catch (Exception e) {
            Slogg::E("CConnectivityService", "Exception in removeDataActivityTracking " + e);
        }
    }
}

void CConnectivityService::UpdateMtu(
        /* [in] */ ILinkProperties* newLp,
        /* [in] */ ILinkProperties* oldLp)
{
    final String iface = newLp.getInterfaceName();
    final Int32 mtu = newLp.getMtu();
    if (oldLp != NULL && newLp.isIdenticalMtu(oldLp)) {
        if (VDBG) Slogg::D("CConnectivityService", "identical MTU - not setting");
        return;
    }

    if (LinkProperties.isValidMtu(mtu, newLp.hasGlobalIPv6Address()) == FALSE) {
        Slogg::E("CConnectivityService", "Unexpected mtu value: " + mtu + ", " + iface);
        return;
    }

    // Cannot set MTU without interface name
    if (TextUtils.isEmpty(iface)) {
        Slogg::E("CConnectivityService", "Setting MTU size with NULL iface.");
        return;
    }

    try {
        if (DBG) Slogg::D("CConnectivityService", "Setting MTU size: " + iface + ", " + mtu);
        mNetd.setMtu(iface, mtu);
    } catch (Exception e) {
        Slog.e(TAG, "exception in setMtu()" + e);
    }
}

void CConnectivityService::UpdateTcpBufferSizes(
    /* [in] */ INetworkAgentInfo* nai)
{
    if (IsDefaultNetwork(nai) == FALSE) {
        return;
    }

    String tcpBufferSizes = nai.linkProperties.getTcpBufferSizes();
    String[] values = NULL;
    if (tcpBufferSizes != NULL) {
        values = tcpBufferSizes.split(",");
    }

    if (values == NULL || values.length != 6) {
        if (DBG) Slogg::D("CConnectivityService", "Invalid tcpBufferSizes string: " + tcpBufferSizes +", using defaults");
        tcpBufferSizes = DEFAULT_TCP_BUFFER_SIZES;
        values = tcpBufferSizes.split(",");
    }

    if (tcpBufferSizes.Equals(mCurrentTcpBufferSizes)) return;

    try {
        if (DBG) Slog.d(TAG, "Setting tx/rx TCP buffers to " + tcpBufferSizes);

        final String prefix = "/sys/kernel/ipv4/tcp_";
        FileUtils.stringToFile(prefix + "rmem_min", values[0]);
        FileUtils.stringToFile(prefix + "rmem_def", values[1]);
        FileUtils.stringToFile(prefix + "rmem_max", values[2]);
        FileUtils.stringToFile(prefix + "wmem_min", values[3]);
        FileUtils.stringToFile(prefix + "wmem_def", values[4]);
        FileUtils.stringToFile(prefix + "wmem_max", values[5]);
        mCurrentTcpBufferSizes = tcpBufferSizes;
    } catch (IOException e) {
        Slogg::E("CConnectivityService", "Can't set TCP buffer sizes:" + e);
    }

    final String defaultRwndKey = "net.tcp.default_init_rwnd";
    Int32 defaultRwndValue = SystemProperties.getInt(defaultRwndKey, 0);
    Integer rwndValue = Settings.Global.getInt(mContext.getContentResolver(),
        Settings.Global.TCP_DEFAULT_INIT_RWND, defaultRwndValue);
    final String sysctlKey = "sys.sysctl.tcp_def_init_rwnd";
    if (rwndValue != 0) {
        SystemProperties.set(sysctlKey, rwndValue.toString());
    }
}

void CConnectivityService::FlushVmDnsCache()
{
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

ECode CConnectivityService::GetRestoreDefaultNetworkDelay(
    /* [in] */ Int32 networkType,
    /* [out] */ Int32* result)
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
    // final IndentingPrintWriter pw = new IndentingPrintWriter(writer, "  ");
    // if (mContext.checkCallingOrSelfPermission(
    //         android.Manifest.permission.DUMP)
    //         != PackageManager.PERMISSION_GRANTED) {
    //     pw.println("Permission Denial: can't dump ConnectivityService " +
    //             "from from pid=" + Binder.getCallingPid() + ", uid=" +
    //             Binder.getCallingUid());
    //     return;
    // }

    // pw.println("NetworkFactories for:");
    // pw.increaseIndent();
    // for (NetworkFactoryInfo nfi : mNetworkFactoryInfos.values()) {
    //     pw.println(nfi.name);
    // }
    // pw.decreaseIndent();
    // pw.println();

    // NetworkAgentInfo defaultNai = mNetworkForRequestId.get(mDefaultRequest.requestId);
    // pw.print("Active default network: ");
    // if (defaultNai == NULL) {
    //     pw.println("none");
    // } else {
    //     pw.println(defaultNai.network.netId);
    // }
    // pw.println();

    // pw.println("Current Networks:");
    // pw.increaseIndent();
    // for (NetworkAgentInfo nai : mNetworkAgentInfos.values()) {
    //     pw.println(nai.toString());
    //     pw.increaseIndent();
    //     pw.println("Requests:");
    //     pw.increaseIndent();
    //     for (Int32 i = 0; i < nai.networkRequests.size(); i++) {
    //         pw.println(nai.networkRequests.valueAt(i).toString());
    //     }
    //     pw.decreaseIndent();
    //     pw.println("Lingered:");
    //     pw.increaseIndent();
    //     for (NetworkRequest nr : nai.networkLingered) pw.println(nr.toString());
    //     pw.decreaseIndent();
    //     pw.decreaseIndent();
    // }
    // pw.decreaseIndent();
    // pw.println();

    // pw.println("Network Requests:");
    // pw.increaseIndent();
    // for (NetworkRequestInfo nri : mNetworkRequests.values()) {
    //     pw.println(nri.toString());
    // }
    // pw.println();
    // pw.decreaseIndent();

    // pw.print("mActiveDefaultNetwork: " + mActiveDefaultNetwork);
    // if (mActiveDefaultNetwork != TYPE_NONE) {
    //     NetworkInfo activeNetworkInfo = getActiveNetworkInfo();
    //     if (activeNetworkInfo != NULL) {
    //         pw.print(" " + activeNetworkInfo.getState() +
    //                  "/" + activeNetworkInfo.getDetailedState());
    //     }
    // }
    // pw.println();

    // pw.println("mLegacyTypeTracker:");
    // pw.increaseIndent();
    // mLegacyTypeTracker.dump(pw);
    // pw.decreaseIndent();
    // pw.println();

    // synchronized(this) {
    //     pw.println("NetworkTransitionWakeLock is currently " +
    //             (mNetTransitionWakeLock.isHeld() ? "" : "not ") + "held.");
    //     pw.println("It was last requested for "+mNetTransitionWakeLockCausedBy);
    // }
    // pw.println();

    // mTethering.dump(fd, pw, args);

    // if (mInetLog != NULL) {
    //     pw.println();
    //     pw.println("Inet condition reports:");
    //     pw.increaseIndent();
    //     for(Int32 i = 0; i < mInetLog.size(); i++) {
    //         pw.println(mInetLog.get(i));
    //     }
    //     pw.decreaseIndent();
    // }
}

Boolean CConnectivityService::IsLiveNetworkAgent(
    /* [in] */ INetworkAgentInfo* nai,
    /* [in] */ const String& msg)
{
    if (nai.network == NULL) return FALSE;
    final NetworkAgentInfo officialNai;
    synchronized(mNetworkForNetId) {
        officialNai = mNetworkForNetId.get(nai.network.netId);
    }
    if (officialNai != NULL && officialNai.Equals(nai)) return TRUE;
    if (officialNai != NULL || VDBG) {
        Slogg::E("CConnectivityService", msg + " - IsLiveNetworkAgent found mismatched netId: " + officialNai +
            " - " + nai);
    }
    return FALSE;
}

Boolean CConnectivityService::IsRequest(
    /* [in] */ INetworkRequest* request)
{
    return mNetworkRequests.get(request).isRequest;
}

ECode CConnectivityService::Tether(
    /* [in] */ const String& iface,
    /* [out] */ Int32* status)
{
    VALIDATE_NOT_NULL(status);

    ConnectivityManager.enforceTetherChangePermission(mContext);

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
    ConnectivityManager.enforceTetherChangePermission(mContext);

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
    ConnectivityManager.enforceTetherChangePermission(mContext);
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

ECode CConnectivityService::GetTetheredDhcpRanges(
    /* [out, callee] */ ArrayOf<String>** ifaces)
{
    VALIDATE_NOT_NULL(ifaces);
    FAIL_RETURN(EnforceTetherAccessPermission());
    return mTethering->GetTetheredDhcpRanges(ifaces);
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


ECode CConnectivityService::IsTetheringSupported(
    /* [out] */ Boolean* status)
{
    // VALIDATE_NOT_NULL(status);
    // FAIL_RETURN(EnforceTetherAccessPermission());

    // AutoPtr<ISystemProperties> sysProp;
    // CSystemProperties::AcquireSingleton((ISystemProperties**)&sysProp);
    // String denied;
    // sysProp->Get(String("ro.tether.denied"), &denied);
    // Int32 defaultVal =  denied.Equals("TRUE") ? 0 : 1;

    // AutoPtr<IContentResolver> resolver;
    // mContext->GetContentResolver((IContentResolver**)&resolver);
    // Int32 value;
    // AutoPtr<ISettingsGlobal> settingsGlobal;
    // CSettingsGlobal::AcquireSingleton((ISettingsGlobal**)&settingsGlobal);
    // settingsGlobal->GetInt32(resolver, ISettingsGlobal::TETHER_SUPPORTED, defaultVal, &value);
    // Boolean tetherEnabledInSettings = (value != 0);
    // *status = tetherEnabledInSettings;
    // return NOERROR;

    enforceTetherAccessPermission();
    Int32 defaultVal = (SystemProperties.get("ro.tether.denied").Equals("TRUE") ? 0 : 1);
    Boolean tetherEnabledInSettings = (Settings.Global.getInt(mContext.getContentResolver(),
            Settings.Global.TETHER_SUPPORTED, defaultVal) != 0)
            && !mUserManager.hasUserRestriction(UserManager.DISALLOW_CONFIG_TETHERING);
    return tetherEnabledInSettings && ((mTethering.getTetherableUsbRegexs().length != 0 ||
            mTethering.getTetherableWifiRegexs().length != 0 ||
            mTethering.getTetherableBluetoothRegexs().length != 0) &&
            mTethering.getUpstreamIfaceTypes().length != 0);
}

ECode CConnectivityService::RequestNetworkTransitionWakelock(
    /* [in] */ const String& forWhom)
{
    // FAIL_RETURN(EnforceConnectivityInternalPermission());
    // {
    //     AutoLock lock(this);

    //     Boolean  isHeld = FALSE;
    //     if ((mNetTransitionWakeLock->IsHeld(&isHeld), isHeld)) {
    //         return NOERROR;
    //     }
    //     mNetTransitionWakeLockSerialNumber++;
    //     mNetTransitionWakeLock->AcquireLock();
    //     mNetTransitionWakeLockCausedBy = forWhom;
    // }

    // AutoPtr<IMessage> msg;
    // mHandler->ObtainMessage(EVENT_CLEAR_NET_TRANSITION_WAKELOCK,
    //     mNetTransitionWakeLockSerialNumber, 0, (IMessage**)&msg);
    // Boolean result;
    // return mHandler->SendMessageDelayed(msg, mNetTransitionWakeLockTimeout, &result);

        Int32 serialNum = 0;
        synchronized(this) {
            if (mNetTransitionWakeLock.isHeld()) return;
            serialNum = ++mNetTransitionWakeLockSerialNumber;
            mNetTransitionWakeLock.acquire();
            mNetTransitionWakeLockCausedBy = forWhom;
        }
        mHandler.sendMessageDelayed(mHandler.obtainMessage(
                EVENT_EXPIRE_NET_TRANSITION_WAKELOCK, serialNum, 0),
                mNetTransitionWakeLockTimeout);
        return;

}

// 100 percent is full good, 0 is full bad.
ECode CConnectivityService::ReportInetCondition(
    /* [in] */ Int32 networkType,
    /* [in] */ Int32 percentage)
{
    if (percentage > 50) return;  // don't handle good network reports
    NetworkAgentInfo nai = mLegacyTypeTracker.getNetworkForType(networkType);
    if (nai != NULL) reportBadNetwork(nai.network);
}

ECode CConnectivityService::ReportBadNetwork(
    /* [in] */ INetwork* network)
{
    enforceAccessPermission();
    enforceInternetPermission();

    if (network == NULL) return;

    final Int32 uid = Binder.getCallingUid();
    NetworkAgentInfo nai = NULL;
    synchronized(mNetworkForNetId) {
        nai = mNetworkForNetId.get(network.netId);
    }
    if (nai == NULL) return;
    if (DBG) Slogg::D("CConnectivityService", "reportBadNetwork(" + nai.name() + ") by " + uid);
    synchronized(nai) {
        // Validating an uncreated network could result in a call to rematchNetworkAndRequests()
        // which isn't meant to work on uncreated networks.
        if (!nai.created) return;

        if (isNetworkBlocked(nai, uid)) return;

        nai.networkMonitor.sendMessage(INetworkMonitor::CMD_FORCE_REEVALUATION, uid);
    }
}

ECode CConnectivityService::GetProxy(
    /* [out] */ IProxyInfo** proxyInfo)
{
    // VALIDATE_NOT_NULL(proxyProperties);
    // AutoLock lock(mProxyLock);
    // *proxyProperties = mDefaultProxyDisabled ? NULL : mDefaultProxy;
    // REFCOUNT_ADD(*proxyProperties);
    // return NOERROR;

    // this information is already available as a world read/writable jvm property
    // so this API change wouldn't have a benifit.  It also breaks the passing
    // of proxy info to all the JVMs.
    // enforceAccessPermission();
    synchronized(mProxyLock) {
        ProxyInfo ret = mGlobalProxy;
        if ((ret == NULL) && !mDefaultProxyDisabled) ret = mDefaultProxy;
        return ret;
    }
}

ECode CConnectivityService::SetGlobalProxy(
    /* [in] */ IProxyInfo* proxyInfo)
{
    // FAIL_RETURN(EnforceChangePermission());
    // {
    //     AutoLock lock(mGlobalProxyLock);
    //     if (proxyProperties == mGlobalProxy) {
    //         return NOERROR;
    //     }
    //     Boolean isEqual = FALSE;
    //     if (proxyProperties != NULL &&
    //         (proxyProperties->Equals(mGlobalProxy, &isEqual), isEqual)) {
    //         return NOERROR;
    //     }
    //     if (mGlobalProxy != NULL &&
    //         (mGlobalProxy->Equals(proxyProperties, &isEqual), isEqual)) {
    //         return NOERROR;
    //     }

    //     String host("");
    //     Int32 port = 0;
    //     String exclList("");
    //     if (proxyProperties != NULL) {
    //         String hozt;
    //         proxyProperties->GetHost(&hozt);
    //         if (!TextUtils::IsEmpty(hozt)) {
    //             mGlobalProxy = NULL;
    //             CProxyProperties::New(proxyProperties, (IProxyProperties**)&mGlobalProxy);
    //             mGlobalProxy->GetHost(&host);
    //             mGlobalProxy->GetPort(&port);
    //             mGlobalProxy->GetExclusionList(&exclList);
    //         }
    //         else {
    //             mGlobalProxy = NULL;
    //         }
    //     }
    //     else {
    //         mGlobalProxy = NULL;
    //     }
    //     AutoPtr<IContentResolver> res;
    //     mContext->GetContentResolver((IContentResolver**)&res);
    //     Boolean bol;
    //     AutoPtr<ISettingsGlobal> settingsGlobal;
    //     CSettingsGlobal::AcquireSingleton((ISettingsGlobal**)&settingsGlobal);
    //     settingsGlobal->PutString(res, ISettingsGlobal::GLOBAL_HTTP_PROXY_HOST, host, &bol);
    //     settingsGlobal->PutInt32(res, ISettingsGlobal::GLOBAL_HTTP_PROXY_PORT, port, &bol);
    //     settingsGlobal->PutString(res, ISettingsGlobal::GLOBAL_HTTP_PROXY_EXCLUSION_LIST,
    //             exclList, &bol);
    // }

    // if (mGlobalProxy == NULL) {
    //     proxyProperties = mDefaultProxy;
    // }
    // //sendProxyBroadcast(proxyProperties);
    // return NOERROR;

    enforceConnectivityInternalPermission();

    synchronized(mProxyLock) {
        if (proxyProperties == mGlobalProxy) return;
        if (proxyProperties != NULL && proxyProperties.Equals(mGlobalProxy)) return;
        if (mGlobalProxy != NULL && mGlobalProxy.Equals(proxyProperties)) return;

        String host = "";
        Int32 port = 0;
        String exclList = "";
        String pacFileUrl = "";
        if (proxyProperties != NULL && (!TextUtils.isEmpty(proxyProperties.getHost()) ||
                !Uri.EMPTY.Equals(proxyProperties.getPacFileUrl()))) {
            if (!proxyProperties.isValid()) {
                if (DBG)
                    Slogg::D("CConnectivityService", "Invalid proxy properties, ignoring: " + proxyProperties.toString());
                return;
            }
            mGlobalProxy = new ProxyInfo(proxyProperties);
            host = mGlobalProxy.getHost();
            port = mGlobalProxy.getPort();
            exclList = mGlobalProxy.getExclusionListAsString();
            if (!Uri.EMPTY.Equals(proxyProperties.getPacFileUrl())) {
                pacFileUrl = proxyProperties.getPacFileUrl().toString();
            }
        } else {
            mGlobalProxy = NULL;
        }
        ContentResolver res = mContext.getContentResolver();
        final long token = Binder.clearCallingIdentity();
        try {
            Settings.Global.putString(res, Settings.Global.GLOBAL_HTTP_PROXY_HOST, host);
            Settings.Global.putInt(res, Settings.Global.GLOBAL_HTTP_PROXY_PORT, port);
            Settings.Global.putString(res, Settings.Global.GLOBAL_HTTP_PROXY_EXCLUSION_LIST,
                    exclList);
            Settings.Global.putString(res, Settings.Global.GLOBAL_HTTP_PROXY_PAC, pacFileUrl);
        } finally {
            Binder.restoreCallingIdentity(token);
        }

        if (mGlobalProxy == NULL) {
            proxyProperties = mDefaultProxy;
        }
        sendProxyBroadcast(proxyProperties);
    }
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
    String pacFileUrl = Settings.Global.getString(res, Settings.Global.GLOBAL_HTTP_PROXY_PAC);
    if (!TextUtils.isEmpty(host) || !TextUtils.isEmpty(pacFileUrl)) {
        ProxyInfo proxyProperties;
        if (!TextUtils.isEmpty(pacFileUrl)) {
            proxyProperties = new ProxyInfo(pacFileUrl);
        } else {
            proxyProperties = new ProxyInfo(host, port, exclList);
        }
        if (!proxyProperties.isValid()) {
            if (DBG) Slogg::D("CConnectivityService", "Invalid proxy properties, ignoring: " + proxyProperties.toString());
            return;
        }

        synchronized(mProxyLock) {
            mGlobalProxy = proxyProperties;
        }
    }
}

ECode CConnectivityService::GetGlobalProxy(
    /* [out] */ IProxyInfo** proxyProperties)
{
    // this information is already available as a world read/writable jvm property
    // so this API change wouldn't have a benifit.  It also breaks the passing
    // of proxy info to all the JVMs.
    // enforceAccessPermission();
    synchronized(mProxyLock) {
        return mGlobalProxy;
    }
}

void CConnectivityService::HandleApplyDefaultProxy(
    /* [in] */ IProxyInfo* proxy)
{
    if (proxy != NULL && TextUtils.isEmpty(proxy.getHost())
            && Uri.EMPTY.Equals(proxy.getPacFileUrl())) {
        proxy = NULL;
    }
    synchronized(mProxyLock) {
        if (mDefaultProxy != NULL && mDefaultProxy.Equals(proxy)) return;
        if (mDefaultProxy == proxy) return; // catches repeated nulls
        if (proxy != NULL &&  !proxy.isValid()) {
            if (DBG) Slogg::D("CConnectivityService", "Invalid proxy properties, ignoring: " + proxy.toString());
            return;
        }

        // This call could be coming from the PacManager, containing the port of the local
        // proxy.  If this new proxy matches the global proxy then copy this proxy to the
        // global (to get the correct local port), and send a broadcast.
        // TODO: Switch PacManager to have its own message to send back rather than
        // reusing EVENT_HAS_CHANGED_PROXY and this call to HandleApplyDefaultProxy.
        if ((mGlobalProxy != NULL) && (proxy != NULL)
                && (!Uri.EMPTY.Equals(proxy.getPacFileUrl()))
                && proxy.getPacFileUrl().Equals(mGlobalProxy.getPacFileUrl())) {
            mGlobalProxy = proxy;
            sendProxyBroadcast(mGlobalProxy);
            return;
        }
        mDefaultProxy = proxy;

        if (mGlobalProxy != NULL) return;
        if (!mDefaultProxyDisabled) {
            sendProxyBroadcast(proxy);
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
        if (data.length == 0) {
            return;
        }

        String proxyHost = (*data)[0];
        Int32 proxyPort = 8080;
        if (data->GetLength() > 1) {
            ECode ec = StringUtils::ParseInt32((*data)[1], &proxyPort);
            if (FAILED(ec)) return;
        }

        AutoPtr<IProxyInfo> p;
        IProxyInfo::New((*data)[0], proxyPort, String(""), (IProxyInfo**)&p);
        SetGlobalProxy(p);
    }
}

void CConnectivityService::SendProxyBroadcast(
    /* [in] */ IProxyInfo* proxy)
{
    AutoPtr<IProxyInfo> proxyProp = proxy;
    if (proxyProp == NULL) {
        CProxyInfo::New(String(""), 0, String(""), (IProxyInfo**)&proxyProp);
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
        else if (TextUtils::Equals(feature, String("enableEMERGENCY")/*Phone::FEATURE_ENABLE_EMERGENCY*/)) {
            usedNetworkType = IConnectivityManager::TYPE_MOBILE_EMERGENCY;
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

ECode CConnectivityService::PrepareVpn(
    /* [in] */ const String& oldPackage,
    /* [in] */ const String& newPackage,
    /* [out] */ Boolean* status)
{
    // VALIDATE_NOT_NULL(status);
    // *status = FALSE;
        throwIfLockdownEnabled();
        Int32 user = UserHandle.getUserId(Binder.getCallingUid());
        synchronized(mVpns) {
            return mVpns.get(user).prepare(oldPackage, newPackage);
        }
}

ECode CConnectivityService::SetVpnPackageAuthorization(
    /* [in] */ Boolean authorized)
{
    Int32 user = UserHandle.getUserId(Binder.getCallingUid());
    synchronized(mVpns) {
        mVpns.get(user).setPackageAuthorization(authorized);
    }
}

ECode CConnectivityService::EstablishVpn(
    /* [in] */ IVpnConfig* config,
    /* [out] */ IParcelFileDescriptor** fd)
{
    VALIDATE_NOT_NULL(fd);
    *fd = NULL;

    Int32 user = UserHandle.getUserId(Binder.getCallingUid());
    synchronized(mVpns) {
        return mVpns.get(user).establish(config);
    }
}

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

    Int32 user = UserHandle.getUserId(Binder.getCallingUid());
    synchronized(mVpns) {
        mVpns.get(user).startLegacyVpn(profile, mKeyStore, egress);
    }
}

ECode CConnectivityService::GetLegacyVpnInfo(
    /* [out] */ ILegacyVpnInfo** info)
{
    VALIDATE_NOT_NULL(info);
    *info = NULL;

    FAIL_RETURN(ThrowIfLockdownEnabled());
    Int32 user = UserHandle.getUserId(Binder.getCallingUid());
    synchronized(mVpns) {
        return mVpns.get(user).getLegacyVpnInfo();
    }
}

ECode CConnectivityService::GetVpnConfig(
    /* [out] */ IVpnConfig** config)
{
    Int32 user = UserHandle.getUserId(Binder.getCallingUid());
    synchronized(mVpns) {
        return mVpns.get(user).getVpnConfig();
    }
}

ECode CConnectivityService::UpdateLockdownVpn(
    /* [out] */ Boolean* status)
{
    VALIDATE_NOT_NULL(status);
    *status = FALSE;

    if (Binder.getCallingUid() != Process.SYSTEM_UID) {
        Slog.w(TAG, "Lockdown VPN only available to AID_SYSTEM");
        return FALSE;
    }

    FAIL_RETURN(EnforceSystemUid());

    // Tear down existing lockdown if profile was removed
    mLockdownEnabled = LockdownVpnTracker::IsEnabled();
    if (mLockdownEnabled) {
        assert(0);
        // if (!mKeyStore.isUnlocked()) {
        //     Slogger::W(TAG, "KeyStore locked; unable to create LockdownTracker");
        //     *status = FALSE;
        //     return NOERROR;
        // }

        // String profileName(mKeyStore->Get(Credentials::LOCKDOWN_VPN));
        // AutoPtr<IVpnProfile> profile;
        // VpnProfile::Decode(profileName, mKeyStore->Get(Credentials::VPN + profileName));
        // Int32 user = UserHandle.getUserId(Binder.getCallingUid());
        // synchronized(mVpns) {
        //     setLockdownTracker(new LockdownVpnTracker(mContext, mNetd, this, mVpns.get(user),
        //                 profile));
        // }
    }
    else {
        SetLockdownTracker(NULL);
    }

    *status = TRUE;
    return NOERROR;
}

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
        mNetd.setFirewallInterfaceRule("lo", TRUE);
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
