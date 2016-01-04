
#include "elastos/droid/server/net/CNetworkPolicyManagerService.h"
#include "elastos/droid/server/SystemConfig.h"
#include "elastos/droid/net/CProcessObserver.h"
#include "elastos/droid/net/CNetworkStatsService.h"
#include "elastos/droid/net/CNetworkPolicyManagerServiceAlertObserber.h"
#include "CNetworkManagementService.h"
#include "elastos/droid/os/Binder.h"
#include "elastos/droid/os/Handler.h"
#include "util/ArrayUtils.h"
#include "util/Xml.h"
#include "elastos/droid/R.h"
#include "elastos/droid/Manifest.h"
#include <elastos/utility/logging/Slogger.h>
#include <elastos/core/StringUtils.h>
#include <elastos/core/Math.h>

using Elastos::Droid::Server::SystemConfig;

using Elastos::Core::StringUtils;
using Elastos::Core::ICharSequence;
using Elastos::Core::CString;
using Elastos::Core::IBoolean;
using Elastos::Core::CBoolean;
using Elastos::Core::ISystem;
using Elastos::Core::CSystem;
using Elastos::Core::IInteger64;
using Elastos::Core::CInteger64;
using Elastos::Core::IBoolean;
using Elastos::Core::CArrayOf;
using Elastos::Core::EIID_ICharSequence;
using Elastos::IO::CFile;;
using Elastos::IO::IFileInputStream;
using Elastos::IO::IFileOutputStream;
using Elastos::IO::IIoUtils;
using Elastos::IO::CIoUtils;
using Elastos::Utility::Logging::Slogger;
using Elastos::Droid::App::IActivityManager;
using Elastos::Droid::Content::CIntent;
using Elastos::Droid::Content::IIntentFilter;
using Elastos::Droid::Content::CIntentFilter;
using Elastos::Droid::Content::IComponentName;
using Elastos::Droid::Content::CComponentName;
using Elastos::Droid::Content::Pm::IPackageManager;
using Elastos::Droid::Content::Pm::IUserInfo;
using Elastos::Droid::Content::Pm::IApplicationInfo;
using Elastos::Droid::Content::Res::IResources;
using Elastos::Droid::Net::CNetworkPolicy;
using Elastos::Droid::Net::INetworkTemplateHelper;
using Elastos::Droid::Net::CNetworkTemplateHelper;
using Elastos::Droid::Net::CNetworkTemplate;
using Elastos::Droid::Net::ILinkProperties;
using Elastos::Droid::Net::CNetworkIdentity;
using Elastos::Droid::Net::INetworkIdentityHelper;
using Elastos::Droid::Net::CNetworkIdentityHelper;
using Elastos::Droid::Net::INetworkInfo;
using Elastos::Droid::Net::INetworkPolicyManager;
using Elastos::Droid::Net::IConnectivityManager;
using Elastos::Droid::Net::INetworkPolicyManagerHelper;
using Elastos::Droid::Net::CNetworkPolicyManagerHelper;
using Elastos::Droid::Net::CNetworkQuotaInfo;
using Elastos::Droid::Net::IConnectivityManagerHelper;
using Elastos::Droid::Net::CConnectivityManagerHelper;
using Elastos::Droid::Utility::CPairHelper;
using Elastos::Droid::Utility::IPair;
using Elastos::Droid::Utility::IPairHelper;
using Elastos::Droid::Wifi::IWifiInfoHelper;
using Elastos::Droid::Wifi::IWifiInfo;
using Elastos::Droid::Wifi::IWifiInfoHelper;
using Elastos::Droid::Wifi::CWifiInfoHelper;
using Elastos::Droid::Wifi::IWifiConfiguration;
using Elastos::Droid::Wifi::IWifiManager;
using Elastos::Droid::Os::EIID_IHandlerCallback;
using Elastos::Droid::Os::Binder;
using Elastos::Droid::Os::IUserHandle;
using Elastos::Droid::Os::IUserManager;
using Elastos::Droid::Os::IIdleHandler;
using Elastos::Droid::Os::CRemoteCallbackList;
using Elastos::Droid::Os::IEnvironment;
using Elastos::Droid::Os::CEnvironment;
using Elastos::Droid::Os::IHandlerThread;
using Elastos::Droid::Provider::CSettingsSecure;
using Elastos::Droid::Provider::ISettingsSecure;
using Elastos::Droid::Text::Format::CFormatter;
using Elastos::Droid::Text::Format::IFormatter;
using Elastos::Droid::Text::Format::ITime;
using Elastos::Droid::Text::Format::CTime;
using Elastos::Droid::Text::Format::IDateUtils;
using Elastos::Droid::Text::Format::IFormatter;
using Elastos::Droid::Text::Format::CFormatter;
using Elastos::Droid::Telephony::CTelephonyManager;
using Elastos::Droid::Telephony::ITelephonyManager;
using Elastos::Droid::Telephony::CTelephonyManagerHelper;
using Elastos::Droid::Telephony::ITelephonyManagerHelper;
using Elastos::Droid::Utility::INtpTrustedTime;
using Elastos::Droid::Utility::INtpTrustedTimeHelper;
using Elastos::Droid::Utility::CNtpTrustedTimeHelper;
using Elastos::Droid::Utility::CAtomicFile;
using Elastos::Droid::Utility::Xml;
using Elastos::Droid::Utility::IFastXmlSerializer;
using Elastos::Droid::Utility::CFastXmlSerializer;
using Elastos::Droid::Utility::CSparseBooleanArray;
using Elastos::Droid::Internal::Utility::ArrayUtils;

namespace Elastos {
namespace Droid {
namespace Server {
namespace Net {

//==============================================================================
//              CNetworkPolicyManagerService::MyHandlerCallback
//==============================================================================
CAR_INTERFACE_IMPL(CNetworkPolicyManagerService::MyHandlerCallback, Object, IHandlerCallback)

ECode CNetworkPolicyManagerService::MyHandlerCallback::HandleMessage(
    /* [in] */ IMessage* msg,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
    *result = TRUE;

    Int32 what, arg1, arg2;
    msg->GetWhat(&what);
    msg->GetArg1(&arg1);
    msg->GetArg2(&arg2);

    switch(what) {
        case CNetworkPolicyManagerService::MSG_RULES_CHANGED: {
            mHost->HandleMsgRulesChanged(arg1, arg2);
            break;
        }
        case CNetworkPolicyManagerService::MSG_METERED_IFACES_CHANGED: {
            AutoPtr<IInterface> obj;
            msg->GetObj((IInterface**)&obj);
            IArrayOf* array = IArrayOf::Probe(obj);
            mHost->HandleMsgMeteredIfacesChanged(array);
            break;
        }
        case CNetworkPolicyManagerService::MSG_FOREGROUND_ACTIVITIES_CHANGED: {
            AutoPtr<IInterface> obj;
            msg->GetObj((IInterface**)&obj);
            IBoolean* bl = IBoolean::Probe(obj);
            Boolean value;
            bl->GetValue(&value);
            mHost->HandleMsgForegroundActivitiesChanged(arg1, arg2, value);
            break;
        }
        case CNetworkPolicyManagerService::MSG_PROCESS_DIED: {
            mHost->HandleMsgProcessDied(arg1, arg2);
            break;
        }
        case CNetworkPolicyManagerService::MSG_LIMIT_REACHED: {
            AutoPtr<IInterface> obj;
            msg->GetObj((IInterface**)&obj);
            ICharSequence* seq = ICharSequence::Probe(obj);
            String info;
            seq->ToString(&info);
            mHost->HandleMsgLimitReached(info);
            break;
        }
        case CNetworkPolicyManagerService::MSG_RESTRICT_BACKGROUND_CHANGED: {
            mHost->HandleMsgRestrictBackgroundChanged(arg1 != 0);
            break;
        }
        case CNetworkPolicyManagerService::MSG_ADVISE_PERSIST_THRESHOLD: {
            AutoPtr<IInterface> obj;
            msg->GetObj((IInterface**)&obj);
            IInteger64* iobj = IInteger64::Probe(obj);
            Int64 lowestRule = 0;
            iobj->GetValue(&lowestRule);
            mHost->HandleMsgAdvisePersistThreshold(lowestRule);
            break;
        }
        case CNetworkPolicyManagerService::MSG_SCREEN_ON_CHANGED: {
            mHost->HandleMsgScreenOnChanged();
            break;
        }
        default:
            *result = FALSE;
            break;
    }
    return NOERROR;
}

//==============================================================================
//              CNetworkPolicyManagerService::ScreenReceiver
//==============================================================================
ECode CNetworkPolicyManagerService::ScreenReceiver::OnReceive(
    /* [in] */ IContext* context,
    /* [in] */ IIntent* intent)
{
    // screen-related broadcasts are protected by system, no need
    // for permissions check.
    Boolean result;
    return mOwner->mHandler->SendEmptyMessage(
        CNetworkPolicyManagerService::MSG_SCREEN_ON_CHANGED,
        &result);
}


//==============================================================================
//              CNetworkPolicyManagerService::PackageReceiver
//==============================================================================
ECode CNetworkPolicyManagerService::PackageReceiver::OnReceive(
    /* [in] */ IContext* context,
    /* [in] */ IIntent* intent)
{
    // on background handler thread, and PACKAGE_ADDED is protected
    String action;
    intent->GetAction(&action);
    Int32 uid;
    intent->GetInt32Extra(IIntent::EXTRA_UID, -1, &uid);
    if (uid == -1) return NOERROR;

    if (IIntent::ACTION_PACKAGE_ADDED.Equals(action)) {
        // update rules for UID, since it might be subject to
        // global background data policy
        if (LOGV) Slogger::V(TAG, "ACTION_PACKAGE_ADDED for uid=%d", uid);
        {
            synchronized(mOwner->mRulesLock) {
                mOwner->UpdateRulesForUidLocked(uid);
            }
        }
    }
    return NOERROR;
}


//==============================================================================
//              CNetworkPolicyManagerService::UidRemovedReceiver
//==============================================================================
ECode CNetworkPolicyManagerService::UidRemovedReceiver::OnReceive(
    /* [in] */ IContext* context,
    /* [in] */ IIntent* intent)
{
    // on background handler thread, and UID_REMOVED is protected
    Int32 uid;
    intent->GetInt32Extra(IIntent::EXTRA_UID, -1, &uid);
    if (uid == -1) return NOERROR;

    // remove any policy and update rules to clean up
    if (LOGV) Slogger::V(TAG, "ACTION_UID_REMOVED for uid=%d", uid);
    {
        synchronized(mOwner->mRulesLock) {
            mOwner->mUidPolicy.Erase(uid);
            mOwner->UpdateRulesForUidLocked(uid);
            mOwner->WritePolicyLocked();
        }
    }
    return NOERROR;
}


//==============================================================================
//              CNetworkPolicyManagerService::UserReceiver
//==============================================================================
ECode CNetworkPolicyManagerService::UserReceiver::OnReceive(
    /* [in] */ IContext* context,
    /* [in] */ IIntent* intent)
{
    // on background handler thread, and USER_ADDED and USER_REMOVED
    // broadcasts are protected
    String action;
    intent->GetAction(&action);
    Int32 userId;
    intent->GetInt32Extra(IIntent::EXTRA_USER_HANDLE, -1, &userId);
    if (userId == -1) return NOERROR;

    // Update global restrict for new user
    synchronized(mOwner->mRulesLock) {
        // Remove any policies for given user; both cleaning up after a
        // USER_REMOVED, and one last sanity check during USER_ADDED
        mOwner->RemovePoliciesForUserLocked(userId);
        // Update global restrict for new user
        mOwner->UpdateRulesForGlobalChangeLocked(TRUE);
    }
    return NOERROR;
}


//==============================================================================
//              CNetworkPolicyManagerService::StatsReceiver
//==============================================================================
ECode CNetworkPolicyManagerService::StatsReceiver::OnReceive(
    /* [in] */ IContext* context,
    /* [in] */ IIntent* intent)
{
    // on background handler thread, and verified
    // READ_NETWORK_USAGE_HISTORY permission above.
    mOwner->MaybeRefreshTrustedTime();
    {
        synchronized(mOwner->mRulesLock) {
            mOwner->UpdateNetworkEnabledLocked();
            mOwner->UpdateNotificationsLocked();
        }
    }
    return NOERROR;
}


//==============================================================================
//              CNetworkPolicyManagerService::AllowReceiver
//==============================================================================
ECode CNetworkPolicyManagerService::AllowReceiver::OnReceive(
    /* [in] */ IContext* context,
    /* [in] */ IIntent* intent)
{
    // on background handler thread, and verified MANAGE_NETWORK_POLICY
    // permission above.
    return mOwner->SetRestrictBackground(FALSE);
}


//==============================================================================
//              CNetworkPolicyManagerService::SnoozeWarningReceiver
//==============================================================================
ECode CNetworkPolicyManagerService::SnoozeWarningReceiver::OnReceive(
    /* [in] */ IContext* context,
    /* [in] */ IIntent* intent)
{
    // on background handler thread, and verified MANAGE_NETWORK_POLICY
    // permission above.
    AutoPtr<IParcelable> parceable;
    intent->GetParcelableExtra(INetworkPolicyManager::EXTRA_NETWORK_TEMPLATE, (IParcelable**)&parceable);
    AutoPtr<INetworkTemplate> nwkTemplate = INetworkTemplate::Probe(parceable);
    mOwner->PerformSnooze(nwkTemplate, CNetworkPolicyManagerService::TYPE_WARNING);
    return NOERROR;
}


//==============================================================================
//              CNetworkPolicyManagerService::WifiConfigReceiver
//==============================================================================
ECode CNetworkPolicyManagerService::WifiConfigReceiver::OnReceive(
    /* [in] */ IContext* context,
    /* [in] */ IIntent* intent)
{
    // on background handler thread, and verified CONNECTIVITY_INTERNAL
    // permission above.

    Int32 reason;
    intent->GetInt32Extra(IWifiManager::EXTRA_CHANGE_REASON, IWifiManager::CHANGE_REASON_ADDED, &reason);
    if (reason == IWifiManager::CHANGE_REASON_REMOVED) {
        AutoPtr<IParcelable> parceable;
        intent->GetParcelableExtra(IWifiManager::EXTRA_WIFI_CONFIGURATION, (IParcelable**)&parceable);
        AutoPtr<IWifiConfiguration> config = IWifiConfiguration::Probe(parceable);
        String configSSID;
        config->GetSSID(&configSSID);
        if (!configSSID.IsNull()) {
            AutoPtr<INetworkTemplateHelper> netTempHelper;
            CNetworkTemplateHelper::AcquireSingleton((INetworkTemplateHelper**)&netTempHelper);
            AutoPtr<INetworkTemplate> networkTemp;
            netTempHelper->BuildTemplateWifi(configSSID, (INetworkTemplate**)&networkTemp);
            synchronized(mOwner->mRulesLock) {
                HashMap<AutoPtr<INetworkTemplate>, AutoPtr<INetworkPolicy> >::Iterator it =
                        mOwner->mNetworkPolicy.Find(networkTemp);
                if (it != mOwner->mNetworkPolicy.End()) {
                    mOwner->mNetworkPolicy.Erase(it);
                    mOwner->WritePolicyLocked();
                }
            }
        }
    }
    return NOERROR;
}


//==============================================================================
//              CNetworkPolicyManagerService::WifiStateReceiver
//==============================================================================
ECode CNetworkPolicyManagerService::WifiStateReceiver::OnReceive(
    /* [in] */ IContext* context,
    /* [in] */ IIntent* intent)
{
    // on background handler thread, and verified Permission::CONNECTIVITY_INTERNAL
    // permission above.

    // ignore when not connected
    AutoPtr<IParcelable> parcelable;
    intent->GetParcelableExtra(IWifiManager::EXTRA_NETWORK_INFO, (IParcelable**)&parcelable);
    AutoPtr<INetworkInfo> netInfo = INetworkInfo::Probe(parcelable);
    Boolean isConnected;
    netInfo->IsConnected(&isConnected);
    if (!isConnected) return NOERROR;

    parcelable = NULL;
    intent->GetParcelableExtra(IWifiManager::EXTRA_WIFI_INFO, (IParcelable**)&parcelable);
    AutoPtr<IWifiInfo> wifiInfo = IWifiInfo::Probe(parcelable);
    Boolean meteredHint;
    wifiInfo->GetMeteredHint(&meteredHint);

    AutoPtr<IWifiInfoHelper> wifiHelper;
    CWifiInfoHelper::AcquireSingleton((IWifiInfoHelper**)&wifiHelper);
    AutoPtr<INetworkTemplateHelper> netTempHelper;
    CNetworkTemplateHelper::AcquireSingleton((INetworkTemplateHelper**)&netTempHelper);

    String wifiSSID;
    wifiInfo->GetSSID(&wifiSSID);
    netTempHelper->BuildTemplateWifi(wifiSSID, (INetworkTemplate**)&networkTemp);
    synchronized(mOwner->mRulesLock){
        AutoPtr<INetworkPolicy> policy;
        HashMap< AutoPtr<INetworkTemplate>, AutoPtr<INetworkPolicy> >::Iterator it =
                mOwner->mNetworkPolicy.Find(networkTemp);
        if (it != mOwner->mNetworkPolicy.End()) {
            policy = it->mSecond;
        }
        Boolean isInferred;
        if (policy == NULL && meteredHint) {
            // policy doesn't exist, and AP is hinting that it's
            // metered: create an inferred policy.
            CNetworkPolicy::New(networkTemp, INetworkPolicy::CYCLE_NONE, ITime::TIMEZONE_UTC,
                    INetworkPolicy::WARNING_DISABLED, INetworkPolicy::LIMIT_DISABLED,
                    INetworkPolicy::SNOOZE_NEVER, INetworkPolicy::SNOOZE_NEVER, meteredHint, TRUE,
                    (INetworkPolicy**)&policy);
            mOwner->AddNetworkPolicyLocked(policy);

        }
        else if (policy != NULL && (policy->GetInferred(&isInferred), isInferred)) {
            // policy exists, and was inferred: update its current
            // metered state.
            policy->SetMetered(meteredHint);

            // since this is inferred for each wifi session, just update
            // rules without persisting.
            mOwner->UpdateNetworkRulesLocked();
        }
    }
    return NOERROR;
}


//==============================================================================
//              CNetworkPolicyManagerService::ConnReceiver
//==============================================================================
ECode CNetworkPolicyManagerService::ConnReceiver::OnReceive(
    /* [in] */ IContext* context,
    /* [in] */ IIntent* intent)
{
    // on background handler thread, and verified Permission::CONNECTIVITY_INTERNAL
    // permission above.

    mOwner->MaybeRefreshTrustedTime();
    {
        synchronized(mOwner->mRulesLock) {
            mOwner->EnsureActiveMobilePolicyLocked();
            mOwner->UpdateNetworkEnabledLocked();
            mOwner->UpdateNetworkRulesLocked();
            mOwner->UpdateNotificationsLocked();
        }
    }
    return NOERROR;
}


//==============================================================================
//              CNetworkPolicyManagerService::XmlUtils
//==============================================================================
ECode CNetworkPolicyManagerService::XmlUtils::ReadInt32Attribute(
    /* [in] */ IXmlPullParser* in,
    /* [in] */ const String& name,
    /* [out] */ Int32* attr)
{
    String value;
    in->GetAttributeValue(String(NULL), name, &value);
    //try {
    ECode ec = StringUtils::ParseInt32(value, attr);
    if (FAILED(ec)) return E_PROTOCOL_EXCEPTION;
    //} catch (NumberFormatException e) {
    //    throw new ProtocolException("problem parsing " + name + "=" + value + " as int");
    //}
    return NOERROR;
}

ECode CNetworkPolicyManagerService::XmlUtils::WriteInt32Attribute(
    /* [in] */ IXmlSerializer* out,
    /* [in] */ const String& name,
    /* [in] */ Int32 value)
{
    return out->WriteAttribute(String(NULL), name, StringUtils::Int32ToString(value));
}

ECode CNetworkPolicyManagerService::XmlUtils::ReadInt64Attribute(
    /* [in] */ IXmlPullParser* in,
    /* [in] */ const String& name,
    /* [out] */ Int64* attr)
{
    String value;
    in->GetAttributeValue(String(NULL), name, &value);
    //try {
    ECode ec = StringUtils::ParseInt64(value, attr);
    if (FAILED(ec)) return E_PROTOCOL_EXCEPTION;
    //} catch (NumberFormatException e) {
    //    throw new ProtocolException("problem parsing " + name + "=" + value + " as long");
    //}
    return NOERROR;
}

ECode CNetworkPolicyManagerService::XmlUtils::WriteInt64Attribute(
    /* [in] */ IXmlSerializer* out,
    /* [in] */ const String& name,
    /* [in] */ Int64 value)
{
    return out->WriteAttribute(String(NULL), name, StringUtils::Int64ToString(value));
}

ECode CNetworkPolicyManagerService::XmlUtils::ReadBooleanAttribute(
    /* [in] */ IXmlPullParser* in,
    /* [in] */ const String& name,
    /* [out] */ Boolean* attr)
{
    String value;
    in->GetAttributeValue(String(NULL), name, &value);
    *attr = value.EqualsIgnoreCase("TRUE") ? 1 : 0;
    return NOERROR;
}

ECode CNetworkPolicyManagerService::XmlUtils::WriteBooleanAttribute(
    /* [in] */ IXmlSerializer* out,
    /* [in] */ const String& name,
    /* [in] */ Boolean value)
{
    return out->WriteAttribute(String(NULL), name, StringUtils::BooleanToString(value));
}

//==============================================================================
//              CNetworkPolicyManagerService::InnerSub_LowPowerModeListener
//==============================================================================
CNetworkPolicyManagerService::InnerSub_LowPowerModeListener::InnerSub_LowPowerModeListener(
    /* [in] */ CNetworkPolicyManagerService* host,
    /* [in] */ IObject* rulesLock,
    /* [in] */ Boolean& restrictPower)
    : mHost(host)
    , mRulesLock(rulesLock)
    , mRestrictPower(restrictPower)
{}

ECode CNetworkPolicyManagerService::InnerSub_LowPowerModeListener::OnLowPowerModeChanged(
    /* [in] */ Boolean enabled)
{
    synchronized(mRulesLock) {
        if (mRestrictPower != enabled) {
            mRestrictPower = enabled;
            mHost->UpdateRulesForGlobalChangeLocked(TRUE);
        }
    }
    return NOERROR;
}

//==============================================================================
//              CNetworkPolicyManagerService::InnerSub_IProcessObserver
//==============================================================================
CNetworkPolicyManagerService::InnerSub_IProcessObserver::InnerSub_IProcessObserver(
    /* [in] */ CNetworkPolicyManagerService* host,
    /* [in] */ IObject* rulesLock,
    /* [in] */ ISparseArray* uidPidState)
    : mHost(host)
    , mRulesLock(rulesLock)
    , mUidPidState(uidPidState)
{}

ECode CNetworkPolicyManagerService::InnerSub_IProcessObserver::OnForegroundActivitiesChanged(
    /* [in] */ Int32 pid,
    /* [in] */ Int32 uid,
    /* [in] */ Boolean foregroundActivities)
{
    return NOERROR;
}

ECode CNetworkPolicyManagerService::InnerSub_IProcessObserver::OnProcessStateChanged(
    /* [in] */ Int32 pid,
    /* [in] */ Int32 uid,
    /* [in] */ Int32 procState)
{
    synchronized (mRulesLock) {
        // because a uid can have multiple pids running inside, we need to
        // remember all pid states and summarize foreground at uid level.

        // record foreground for this specific pid
        AutoPtr<ISparseIntArray> pidState;
        mUidPidState->Get(uid, (ISparseIntArray**)&pidState);
        if (pidState == NULL) {
            CSparseIntArray::New(2, (ISparseIntArray**)&pidState);
            mUidPidState->Put(uid, pidState);
        }
        pidState->Put(pid, procState);
        mHost->ComputeUidStateLocked(uid);
    }
    return NOERROR;
}

ECode CNetworkPolicyManagerService::InnerSub_IProcessObserver::OnProcessDied(
    /* [in] */ Int32 pid,
    /* [in] */ Int32 uid)
{
    synchronized (mRulesLock) {
        // clear records and recompute, when they exist
        AutoPtr<ISparseIntArray> pidState;
        mUidPidState->Get(uid, (ISparseIntArray**)&pidState);
        if (pidState != NULL) {
            pidState->Delete(pid);
            if (Ptr(pidState)->Func(pidState->GetSize)) <= 0) {
                mUidPidState->Remove(uid);
            }
            mHost->ComputeUidStateLocked(uid);
        }
    }
    return NOERROR;
}

//==============================================================================
//              CNetworkPolicyManagerService
//==============================================================================
const Int32 CNetworkPolicyManagerService::MSG_RULES_CHANGED = 1;
const Int32 CNetworkPolicyManagerService::MSG_METERED_IFACES_CHANGED = 2;
const Int32 CNetworkPolicyManagerService::MSG_LIMIT_REACHED = 5;
const Int32 CNetworkPolicyManagerService::MSG_RESTRICT_BACKGROUND_CHANGED = 6;
const Int32 CNetworkPolicyManagerService::MSG_ADVISE_PERSIST_THRESHOLD = 7;
const Int32 CNetworkPolicyManagerService::MSG_SCREEN_ON_CHANGED = 8;

const String CNetworkPolicyManagerService::TAG("NetworkPolicy");
const Boolean CNetworkPolicyManagerService::LOGD = FALSE;
const Boolean CNetworkPolicyManagerService::LOGV = FALSE;

const Int32 CNetworkPolicyManagerService::VERSION_INIT = 1;
const Int32 CNetworkPolicyManagerService::VERSION_ADDED_SNOOZE = 2;
const Int32 CNetworkPolicyManagerService::VERSION_ADDED_RESTRICT_BACKGROUND = 3;
const Int32 CNetworkPolicyManagerService::VERSION_ADDED_METERED = 4;
const Int32 CNetworkPolicyManagerService::VERSION_SPLIT_SNOOZE = 5;
const Int32 CNetworkPolicyManagerService::VERSION_ADDED_TIMEZONE = 6;
const Int32 CNetworkPolicyManagerService::VERSION_ADDED_INFERRED = 7;
const Int32 CNetworkPolicyManagerService::VERSION_SWITCH_APP_ID = 8;
const Int32 CNetworkPolicyManagerService::VERSION_ADDED_NETWORK_ID = 9;
const Int32 CNetworkPolicyManagerService::VERSION_SWITCH_UID = 10;
const Int32 CNetworkPolicyManagerService::VERSION_LATEST = VERSION_SWITCH_UID;

const Int32 CNetworkPolicyManagerService::TYPE_WARNING = 1;
const Int32 CNetworkPolicyManagerService::TYPE_LIMIT = 2;
const Int32 CNetworkPolicyManagerService::TYPE_LIMIT_SNOOZED = 3;

const String CNetworkPolicyManagerService::TAG_POLICY_LIST("policy-list");
const String CNetworkPolicyManagerService::TAG_NETWORK_POLICY("network-policy");
const String CNetworkPolicyManagerService::TAG_UID_POLICY("uid-policy");
const String CNetworkPolicyManagerService::TAG_APP_POLICY("app-policy");

const String CNetworkPolicyManagerService::ATTR_VERSION("version");
const String CNetworkPolicyManagerService::ATTR_RESTRICT_BACKGROUND("restrictBackground");
const String CNetworkPolicyManagerService::ATTR_NETWORK_TEMPLATE("networkTemplate");
const String CNetworkPolicyManagerService::ATTR_SUBSCRIBER_ID("subscriberId");
const String CNetworkPolicyManagerService::ATTR_NETWORK_ID("networkId");
const String CNetworkPolicyManagerService::ATTR_CYCLE_DAY("cycleDay");
const String CNetworkPolicyManagerService::ATTR_CYCLE_TIMEZONE("cycleTimezone");
const String CNetworkPolicyManagerService::ATTR_WARNING_BYTES("warningBytes");
const String CNetworkPolicyManagerService::ATTR_LIMIT_BYTES("limitBytes");
const String CNetworkPolicyManagerService::ATTR_LAST_SNOOZE("lastSnooze");
const String CNetworkPolicyManagerService::ATTR_LAST_WARNING_SNOOZE("lastWarningSnooze");
const String CNetworkPolicyManagerService::ATTR_LAST_LIMIT_SNOOZE("lastLimitSnooze");
const String CNetworkPolicyManagerService::ATTR_METERED("metered");
const String CNetworkPolicyManagerService::ATTR_INFERRED("inferred");
const String CNetworkPolicyManagerService::ATTR_UID("uid");
const String CNetworkPolicyManagerService::ATTR_APP_ID("appId");
const String CNetworkPolicyManagerService::ATTR_POLICY("policy");

const String CNetworkPolicyManagerService::TAG_ALLOW_BACKGROUND("NetworkPolicy:allowBackground");

const String CNetworkPolicyManagerService::ACTION_ALLOW_BACKGROUND("com.android.server.net.action.ALLOW_BACKGROUND");
const String CNetworkPolicyManagerService::ACTION_SNOOZE_WARNING("com.android.server.net.action.SNOOZE_WARNING");

const Int64 CNetworkPolicyManagerService::TIME_CACHE_MAX_AGE = IDateUtils::DAY_IN_MILLIS;

CAR_INTERFACE_IMPL(CNetworkPolicyManagerService, Object, IINetworkPolicyManager)

CNetworkPolicyManagerService::CNetworkPolicyManagerService()
    : mScreenOn(FALSE)
    , mRestrictBackground(FALSE)
    , mSuppressDefaultPolicy(FALSE)
    , mCurForegroundState(IActivityManager::PROCESS_STATE_TOP)
{
    ASSERT_SUCCEEDED(CRemoteCallbackList::New((IRemoteCallbackList**)&mListeners));
    ASSERT_SUCCEEDED(CProcessObserver::New((Handle32)this, (IProcessObserver**)&mProcessObserver));
    mScreenReceiver = new ScreenReceiver(this);
    mPackageReceiver = new PackageReceiver(this);
    mUidRemovedReceiver = new UidRemovedReceiver(this);
    mUserReceiver = new UserReceiver(this);
    mStatsReceiver = new StatsReceiver(this);
    mAllowReceiver = new AllowReceiver(this);
    mSnoozeWarningReceiver = new SnoozeWarningReceiver(this);
    mWifiConfigReceiver = new WifiConfigReceiver(this);
    mWifiStateReceiver = new WifiStateReceiver(this);
    CNetworkPolicyManagerServiceAlertObserber::New((Handle32)this, (INetworkManagementEventObserver**)&mAlertObserver);
    mConnReceiver = new ConnReceiver(this);
    CObject::New((IObject**)&mRulesLock);
    CArrayMap::New((IArrayMap**)&mNetworkPolicy);
    CArrayMap::New((IArrayMap**)&mNetworkRules);
    CSparseBooleanArray::New((ISparseBooleanArray**)&mPowerSaveWhitelistAppIds);
    CArraySet::New((IArraySet**)&mMeteredIfaces);
    CArraySet::New((IArraySet**)&mOverLimitNotified);
    CArraySet::New((IArraySet**)&mActiveNotifs);
    CSparseIntArray::New((ISparseIntArray**)&mUidState);
    CSparseArray::New((ISparseArray**)&mUidPidState);
}

CNetworkPolicyManagerService::~CNetworkPolicyManagerService()
{
    mUidPidForeground.Clear();
}

ECode CNetworkPolicyManagerService::constructor(
    /* [in] */ IContext* context,
    /* [in] */ IIActivityManager* activityManager,
    /* [in] */ IIPowerManager* powerManager,
    /* [in] */ INetworkStatsService* networkStats,
    /* [in] */ IINetworkManagementService* networkManagement)
{
    AutoPtr<INtpTrustedTimeHelper> helper;
    CNtpTrustedTimeHelper::AcquireSingleton((INtpTrustedTimeHelper**)&helper);
    AutoPtr<INtpTrustedTime> trustedTime;
    helper->GetInstance(context, (INtpTrustedTime**)&trustedTime);
    AutoPtr<IFile> systemdir = GetSystemDir();
    return constructor(context, activityManager, powerManager, networkStats, networkManagement,
            trustedTime, systemdir, FALSE);
}

ECode CNetworkPolicyManagerService::constructor(
    /* [in] */ IContext* context,
    /* [in] */ IIActivityManager* activityManager,
    /* [in] */ IIPowerManager* powerManager,
    /* [in] */ INetworkStatsService* networkStats,
    /* [in] */ IINetworkManagementService* networkManagement,
    /* [in] */ ITrustedTime* time,
    /* [in] */ IFile* systemDir,
    /* [in] */ Boolean suppressDefaultPolicy)
{
    VALIDATE_NOT_NULL(context);
    VALIDATE_NOT_NULL(activityManager);
    VALIDATE_NOT_NULL(powerManager);
    VALIDATE_NOT_NULL(networkStats);
    VALIDATE_NOT_NULL(networkManagement);
    VALIDATE_NOT_NULL(time);
    mContext = context;
    mActivityManager = activityManager;
    mPowerManager = powerManager;
    mNetworkStats = networkStats;
    mNetworkManager = networkManagement;
    mTime = time;

    AutoPtr<IHandlerThread> thread;
    CHandlerThread::New(TAG, (IHandlerThread**)&thread);
    thread->Start();
    AutoPtr<ILooper> looper;
    thread->GetLooper((ILooper**)&looper);
    mHandlerCallback = new MyHandlerCallback(this);
    CHandler::New(looper, mHandlerCallback, TRUE,(IHandler**)&mHandler);

    mSuppressDefaultPolicy = suppressDefaultPolicy;

    AutoPtr<IFile> file;
    CFile::New(systemDir, String("netpolicy.xml"), (IFile**)&file);
    CAtomicFile::New(file, (IAtomicFile**)&mPolicyFile);
    return NOERROR;
}

AutoPtr<IFile> CNetworkPolicyManagerService::GetSystemDir()
{
    AutoPtr<IFile> systemdir;
    AutoPtr<IEnvironment> env;
    CEnvironment::AcquireSingleton((IEnvironment**)&env);
    AutoPtr<IFile> dataDir;
    env->GetDataDirectory((IFile**)&dataDir);
    CFile::New(dataDir, String("system"), (IFile**)&systemdir);
    return systemdir;
}

ECode CNetworkPolicyManagerService::BindConnectivityManager(
    /* [in] */ IIConnectivityManager* connManager)
{
    VALIDATE_NOT_NULL(connManager);
    mConnManager = connManager;
    return NOERROR;
}

ECode CNetworkPolicyManagerService::BindNotificationManager(
    /* [in] */ IINotificationManager* notifManager)
{
    VALIDATE_NOT_NULL(notifManager);
    mNotifManager = notifManager;
    return NOERROR;
}

ECode CNetworkPolicyManagerService::SystemReady()
{
    if (!IsBandwidthControlEnabled()) {
        Slogger::W(TAG, String("bandwidth controls disabled, unable to enforce policy"));
        return NOERROR;
    }

    AutoPtr<IPackageManager> pm;
    mContext->GetPackageManager((IPackageManager**)&pm);

    synchronized(mRulesLock) {
            AutoPtr<ISystemConfig> sysConfig;
            SystemConfig::GetInstance((ISystemConfig**)&sysConfig);
            AutoPtr<IArraySet> allowPower;
            // TODO: Waiting for SystemConfig
            assert(0);
            // allowPower = sysConfig->GetAllowInPowerSave();
            for (Int32 i=0; i < Ptr(allowPower)->Func(allowPower->GetSize); i++) {
                AutoPtr<IInterface> obj;
                allowPower->ValueAt(i, (IInterface**)&obj);
                String pkg;
                ICharSequence::Probe(obj)->ToString(&pkg);
                // try {
                ECode ec;
                do {
                    AutoPtr<IApplicationInfo> ai;
                    if (FAILED(ec = pm->GetApplicationInfo(pkg, 0, (IApplicationInfo**)&ai))) break;
                    if ((Ptr(ai)->Func(ai->GetFlags) & IApplicationInfo::FLAG_SYSTEM) != 0) {
                        AutoPtr<IUserHandleHelper> helper;
                        CUserHandleHelper::AcquireSingleton((IUserHandleHelper**)&helper);
                        Int32 appId;
                        helper->GetAppID(Ptr(ai)->Func(ai->GetUid));
                        mPowerSaveWhitelistAppIds->Put(appId, TRUE);
                    }
                } while(FALSE);
                // } catch (PackageManager.NameNotFoundException e) {
                if (FAILED(ec)) {
                    if (ec != E_NAME_NOT_FOUND_EXCEPTION)
                        return ec;
                }
                // }
            }

            mPowerManagerInternal = LocalServices::GetService(EIID_IPowerManagerInternal);
            mPowerManagerInternal->RegisterLowPowerModeObserver(
                new InnerSub_LowPowerModeListener(this, mRulesLock, mRestrictPower));
            mPowerManagerInternal->GetLowPowerModeEnabled(&mRestrictPower);

        // read policy from disk
        ReadPolicyLocked();

        if (mRestrictBackground || mRestrictPower) {
            UpdateRulesForGlobalChangeLocked(TRUE);
            UpdateNotificationsLocked();
        }
    }

    UpdateScreenOn();

    // try {
    mActivityManager->RegisterProcessObserver(mProcessObserver);
    mNetworkManager->RegisterObserver(mAlertObserver);
    // } catch (RemoteException e) {
    //     // ignored; both services live in system_server
    // }

    // TODO: traverse existing processes to know foreground state, or have
    // activitymanager dispatch current state when new observer attached.

    AutoPtr<IIntentFilter> screenFilter;
    CIntentFilter::New((IIntentFilter**)&screenFilter);
    screenFilter->AddAction(IIntent::ACTION_SCREEN_ON);
    screenFilter->AddAction(IIntent::ACTION_SCREEN_OFF);
    AutoPtr<IIntent> intent;
    mContext->RegisterReceiver(mScreenReceiver, screenFilter, (IIntent**)&intent);

    // watch for network interfaces to be claimed
    AutoPtr<IIntentFilter> connFilter;
    CIntentFilter::New(IConnectivityManager::CONNECTIVITY_ACTION_IMMEDIATE, (IIntentFilter**)&connFilter);
    AutoPtr<IIntent> resIntent;
    mContext->RegisterReceiver(
        mConnReceiver, connFilter, Elastos::Droid::Manifest::permission::CONNECTIVITY_INTERNAL,
        mHandler, (IIntent**)&resIntent);

    // listen for package changes to update policy
    AutoPtr<IIntentFilter> packageFilter;
    CIntentFilter::New((IIntentFilter**)&packageFilter);
    packageFilter->AddAction(IIntent::ACTION_PACKAGE_ADDED);
    packageFilter->AddDataScheme(String("package"));
    resIntent = NULL;
    mContext->RegisterReceiver(
        mPackageReceiver, packageFilter, String(NULL), mHandler, (IIntent**)&resIntent);

    // listen for UID changes to update policy
    AutoPtr<IIntentFilter> acRemoved;
    CIntentFilter::New(IIntent::ACTION_UID_REMOVED, (IIntentFilter**)&acRemoved);
    resIntent = NULL;
    mContext->RegisterReceiver(
        mUidRemovedReceiver, acRemoved, String(NULL), mHandler, (IIntent**)&resIntent);

    // listen for user changes to update policy
    AutoPtr<IIntentFilter> userFilter;
    CIntentFilter::New((IIntentFilter**)&userFilter);
    userFilter->AddAction(IIntent::ACTION_USER_ADDED);
    userFilter->AddAction(IIntent::ACTION_USER_REMOVED);
    resIntent = NULL;
    mContext->RegisterReceiver(
        mUserReceiver, userFilter, String(NULL), mHandler, (IIntent**)&resIntent);

    // listen for stats update events
    AutoPtr<IIntentFilter> statsFilter;
    CIntentFilter::New(CNetworkStatsService::ACTION_NETWORK_STATS_UPDATED, (IIntentFilter**)&statsFilter);
    resIntent = NULL;
    mContext->RegisterReceiver(
        mStatsReceiver, statsFilter, Elastos::Droid::Manifest::permission::READ_NETWORK_USAGE_HISTORY,
        mHandler, (IIntent**)&resIntent);

    // listen for restrict background changes from notifications
    AutoPtr<IIntentFilter> allowFilter;
    CIntentFilter::New(ACTION_ALLOW_BACKGROUND, (IIntentFilter**)&allowFilter);
    resIntent = NULL;
    mContext->RegisterReceiver(
        mAllowReceiver, allowFilter, Elastos::Droid::Manifest::permission::MANAGE_NETWORK_POLICY,
        mHandler, (IIntent**)&resIntent);

    // listen for snooze warning from notifications
    AutoPtr<IIntentFilter> snoozeWarningFilter;
    CIntentFilter::New(ACTION_SNOOZE_WARNING, (IIntentFilter**)&snoozeWarningFilter);
    resIntent = NULL;
    mContext->RegisterReceiver(
        mSnoozeWarningReceiver, snoozeWarningFilter, Elastos::Droid::Manifest::permission::MANAGE_NETWORK_POLICY,
        mHandler, (IIntent**)&resIntent);

    // listen for configured wifi networks to be removed
    AutoPtr<IIntentFilter> wifiConfigFilter;
    CIntentFilter::New(IWifiManager::CONFIGURED_NETWORKS_CHANGED_ACTION, (IIntentFilter**)&wifiConfigFilter);
    resIntent = NULL;
    mContext->RegisterReceiver(
        mWifiConfigReceiver, wifiConfigFilter, NULL, mHandler, (IIntent**)&resIntent);

    // listen for wifi state changes to catch metered hint
    AutoPtr<IIntentFilter> wifiStateFilter;
    CIntentFilter::New(IWifiManager::NETWORK_STATE_CHANGED_ACTION, (IIntentFilter**)&wifiStateFilter);
    resIntent = NULL;
    mContext->RegisterReceiver(
        mWifiStateReceiver, wifiStateFilter, NULL, mHandler, (IIntent**)&resIntent);

    return NOERROR;
}

 /**
 * Check {@link NetworkPolicy} against current {@link INetworkStatsService}
 * to show visible notifications as needed.
 */
void CNetworkPolicyManagerService::UpdateNotificationsLocked()
{
    if (LOGV) Slogger::V(TAG, "updateNotificationsLocked()");

    // keep track of previously active notifications
    AutoPtr<IArraySet> beforeNotifs;
    CArraySet::New(mActiveNotifs, (IArraySet**)&beforeNotifs);
    mActiveNotifs->Clear();

    // TODO: when switching to kernel notifications, compute next future
    // cycle boundary to recompute notifications.

    // examine stats for each active policy
    Int64 currentTime = CurrentTimeMillis();
    HashMap< AutoPtr<INetworkTemplate>, AutoPtr<INetworkPolicy> >::Iterator npIt;
    for(Int32 i = Ptr(mNetworkPolicy)->Func(mNetworkPolicy->GetSize) - 1; i >= 0; --i) {
        AutoPtr<INetworkPolicy> policy;
        mNetworkPolicy->ValueAt(i, (INetworkPolicy**)&policy);
        // ignore policies that aren't relevant to user
        AutoPtr<INetworkTemplate> netTmpl;
        policy->GetTemplate((INetworkTemplate**)&netTmpl);
        if (!IsTemplateRelevant(netTmpl)) continue;
        Boolean hasCycle;
        policy->HasCycle(&hasCycle);
        if (!hasCycle) continue;

        AutoPtr<INetworkPolicyManagerHelper> helper;
        CNetworkPolicyManagerHelper::AcquireSingleton((INetworkPolicyManagerHelper**)&helper);
        Int64 start;
        helper->ComputeLastCycleBoundary(currentTime, policy, &start);
        Int64 end = currentTime;
        Int64 totalBytes = GetTotalBytes(netTmpl, start, end);

        Boolean overLimit;
        if (policy->IsOverLimit(totalBytes, &overLimit), overLimit) {
            Int64 lastLimitSnooze;
            policy->GetLastLimitSnooze(&lastLimitSnooze);
            if (lastLimitSnooze >= start) {
                EnqueueNotification(policy, TYPE_LIMIT_SNOOZED, totalBytes);
            }
            else {
                EnqueueNotification(policy, TYPE_LIMIT, totalBytes);
                NotifyOverLimitLocked(netTmpl);
            }
        }
        else {
            NotifyUnderLimitLocked(netTmpl);

            Boolean isOverWarning;
            policy->IsOverWarning(totalBytes, &isOverWarning);
            Int64 lastWarningSnooze;
            policy->GetLastWarningSnooze(&lastWarningSnooze);
            if (isOverWarning && lastWarningSnooze < start) {
                EnqueueNotification(policy, TYPE_WARNING, totalBytes);
            }
        }
    }

    // ongoing notification when restricting background data
    if (mRestrictBackground) {
        EnqueueRestrictedNotification(TAG_ALLOW_BACKGROUND);
    }

    // cancel stale notifications that we didn't renew above
    for (Int32 i = Ptr(beforeNotifs)->Func(beforeNotifs->GetSize) - 1; i >= 0; --i) {
        AutoPtr<IInterface> obj;
        beforeNotifs->ValueAt(i, (IInterface**)&obj);
        String tag;
        ICharSequence::Probe(obj)->ToString(&tag);
        if (mActiveNotifs.Find(tag) == mActiveNotifs.End()) {
            CancelNotification(tag);
        }
    }
}

/**
 * Test if given {@link NetworkTemplate} is relevant to user based on
 * current device state, such as when
 * {@link TelephonyManager#getSubscriberId()} matches. This is regardless of
 * data connection status.
 */
Boolean CNetworkPolicyManagerService::IsTemplateRelevant(
    /* [in] */ INetworkTemplate* templ)
{
    AutoPtr<ITelephonyManagerHelper> teleHelper;
    CTelephonyManagerHelper::AcquireSingleton((ITelephonyManagerHelper**)&teleHelper);
    AutoPtr<ITelephonyManager> tele;
    teleHelper->From(mContext, (ITelephonyManager**)&tele);
    Int32 rule;
    templ->GetMatchRule(&rule);
    switch (rule) {
        case INetworkTemplate::MATCH_MOBILE_3G_LOWER:
        case INetworkTemplate::MATCH_MOBILE_4G:
        case INetworkTemplate::MATCH_MOBILE_ALL:
            // mobile templates are relevant when SIM is ready and
            // subscriberId matches.
            Int32 state;
            tele->GetSimState(&state);
            if (state == ITelephonyManager::SIM_STATE_READY) {
                String templateSubscriberId;
                templ->GetSubscriberId(&templateSubscriberId);
                String teleSubscriberId;
                tele->GetSubscriberId(&teleSubscriberId);
                return teleSubscriberId.Equals(templateSubscriberId);
            } else {
                return FALSE;
            }
    }
    return TRUE;
}

/**
 * Notify that given {@link NetworkTemplate} is over
 * {@link NetworkPolicy#limitBytes}, potentially showing dialog to user.
 */
void CNetworkPolicyManagerService::NotifyOverLimitLocked(
    /* [in] */ INetworkTemplate* templ)
{
    if (mOverLimitNotified.Find(templ) == mOverLimitNotified.End()) {
        mContext->StartActivity(BuildNetworkOverLimitIntent(templ));
        mOverLimitNotified.Insert(templ);
    }
}

void CNetworkPolicyManagerService::NotifyUnderLimitLocked(
    /* [in] */ INetworkTemplate* templ)
{
    mOverLimitNotified.Erase(templ);
}

/**
 * Build unique tag that identifies an active {@link NetworkPolicy}
 * notification of a specific type, like {@link #TYPE_LIMIT}.
 */
String CNetworkPolicyManagerService::BuildNotificationTag(
    /* [in] */ INetworkPolicy* policy,
    /* [in] */ Int32 type)
{
    AutoPtr<INetworkTemplate> netTmpl;
    policy->GetTemplate((INetworkTemplate**)&netTmpl);
    Int32 hashCode;
    netTmpl->GetHashCode(&hashCode);
    StringBuilder sb(TAG);
    sb += ":";
    sb += hashCode;
    sb += ":";
    sb += type;
    return sb.ToString();
}

/**
 * Show notification for combined {@link NetworkPolicy} and specific type,
 * like {@link #TYPE_LIMIT}. Okay to call multiple times.
 */
void CNetworkPolicyManagerService::EnqueueNotification(
    /* [in] */ INetworkPolicy* policy,
    /* [in] */ Int32 type,
    /* [in] */ Int64 totalBytes)
{
    String tag = BuildNotificationTag(policy, type);
    AutoPtr<INotificationBuilder> builder;
    CNotificationBuilder::New(mContext, (INotificationBuilder**)&builder);
    builder->SetOnlyAlertOnce(TRUE);
    builder->SetWhen(0);
    Int32 color;
    Ptr(mContext)->Func(mContext->GetResources)->GetColor(R::color::system_notification_accent_color, &color);
    builder->SetColor(color);

    AutoPtr<IResources> res;
    mContext->GetResources((IResources**)&res);
    switch (type) {
        case TYPE_WARNING:
        {
            AutoPtr<ICharSequence> title;
            res->GetText(R::string::data_usage_warning_title, (ICharSequence**)&title);
            AutoPtr<ICharSequence> body;
            res->GetText(R::string::data_usage_warning_body, (ICharSequence**)&body);

            builder->SetSmallIcon(R::drawable::stat_notify_error);
            builder->SetTicker(title);
            builder->SetContentTitle(title);
            builder->SetContentText(body);

            AutoPtr<INetworkTemplate> netTmpl;
            policy->GetTemplate((INetworkTemplate**)&netTmpl);
            AutoPtr<IIntent> snoozeIntent = BuildSnoozeWarningIntent(netTmpl);

            AutoPtr<IPendingIntentHelper> pendingIntentHelper;
            CPendingIntentHelper::AcquireSingleton((IPendingIntentHelper**)&pendingIntentHelper);
            AutoPtr<IPendingIntent> pendingIntentDel;
            pendingIntentHelper->GetBroadcast(mContext, 0, snoozeIntent, IPendingIntent::FLAG_UPDATE_CURRENT, (IPendingIntent**)&pendingIntentDel);
            builder->SetDeleteIntent(pendingIntentDel);

            AutoPtr<IIntent> viewIntent = BuildViewDataUsageIntent(netTmpl);
            AutoPtr<IPendingIntent> pendingIntent;
            pendingIntentHelper->GetActivity(mContext, 0, viewIntent, IPendingIntent::FLAG_UPDATE_CURRENT, (IPendingIntent**)&pendingIntent);
            builder->SetContentIntent(pendingIntent);

            break;
        }
        case TYPE_LIMIT:
        {
            AutoPtr<ICharSequence> body;
            res->GetText(R::string::data_usage_limit_body, (ICharSequence**)&body);

            AutoPtr<ICharSequence> title;
            Int32 icon = R::drawable::stat_notify_disabled_data;
            AutoPtr<INetworkTemplate> netTmpl;
            policy->GetTemplate((INetworkTemplate**)&netTmpl);
            Int32 rule;
            netTmpl->GetMatchRule(&rule);
            switch (rule)
            {
                case INetworkTemplate::MATCH_MOBILE_3G_LOWER:
                    res->GetText(R::string::data_usage_3g_limit_title, (ICharSequence**)&title);
                    break;
                case INetworkTemplate::MATCH_MOBILE_4G:
                    res->GetText(R::string::data_usage_4g_limit_title, (ICharSequence**)&title);
                    break;
                case INetworkTemplate::MATCH_MOBILE_ALL:
                    res->GetText(R::string::data_usage_mobile_limit_title, (ICharSequence**)&title);
                    break;
                case INetworkTemplate::MATCH_WIFI:
                    res->GetText(R::string::data_usage_wifi_limit_title, (ICharSequence**)&title);
                    icon = R::drawable::stat_notify_error;
                    break;
                default:
                    title = NULL;
                    break;
            }

            builder->SetOngoing(TRUE);
            builder->SetSmallIcon(icon);
            builder->SetSmallIcon(R::drawable::stat_notify_disabled);
            builder->SetTicker(title);
            builder->SetContentTitle(title);
            builder->SetContentText(body);

            AutoPtr<IIntent> intent = BuildNetworkOverLimitIntent(netTmpl);
            AutoPtr<IPendingIntentHelper> pendingIntentHelper;
            CPendingIntentHelper::AcquireSingleton((IPendingIntentHelper**)&pendingIntentHelper);
            AutoPtr<IPendingIntent> pendingIntent;
            pendingIntentHelper->GetActivity(mContext, 0, intent, IPendingIntent::FLAG_UPDATE_CURRENT, (IPendingIntent**)&pendingIntent);
            builder->SetContentIntent(pendingIntent);
            break;
        }
        case TYPE_LIMIT_SNOOZED:
        {
            Int64 limitBytes;
            policy->GetLimitBytes(&limitBytes);
            Int64 overBytes = totalBytes - limitBytes;

            AutoPtr<IFormatter> formatter;
            CFormatter::AcquireSingleton((IFormatter**)&formatter);
            String strFormatter;
            formatter->FormatFileSize(mContext, overBytes, &strFormatter);

            AutoPtr< ArrayOf<IInterface*> > args = ArrayOf<IInterface*>::Alloc(1);
            AutoPtr<ICharSequence> csFormatter;
            CString::New(strFormatter, (ICharSequence**)&csFormatter);
            args->Set(0, csFormatter.Get());

            String strbody;
            res->GetString(R::string::data_usage_limit_snoozed_body, args, &strbody);
            AutoPtr<ICharSequence> body;
            CString::New(strbody, (ICharSequence**)&body);

            AutoPtr<ICharSequence> title;
            AutoPtr<INetworkTemplate> netTmpl;
            policy->GetTemplate((INetworkTemplate**)&netTmpl);
            Int32 rule;
            netTmpl->GetMatchRule(&rule);
            switch (rule)
            {
                case INetworkTemplate::MATCH_MOBILE_3G_LOWER:
                    res->GetText(R::string::data_usage_3g_limit_snoozed_title, (ICharSequence**)&title);
                    break;
                case INetworkTemplate::MATCH_MOBILE_4G:
                    res->GetText(R::string::data_usage_4g_limit_snoozed_title, (ICharSequence**)&title);
                    break;
                case INetworkTemplate::MATCH_MOBILE_ALL:
                    res->GetText(R::string::data_usage_mobile_limit_snoozed_title, (ICharSequence**)&title);
                    break;
                case INetworkTemplate::MATCH_WIFI:
                    res->GetText(R::string::data_usage_wifi_limit_snoozed_title, (ICharSequence**)&title);
                    break;
                default:
                    title = NULL;
                    break;
            }

            builder->SetOngoing(TRUE);
            builder->SetSmallIcon(R::drawable::stat_notify_error);
            builder->SetTicker(title);
            builder->SetContentTitle(title);
            builder->SetContentText(body);

            AutoPtr<IIntent> intent = BuildViewDataUsageIntent(netTmpl);
            AutoPtr<IPendingIntentHelper> pendingIntentHelper;
            CPendingIntentHelper::AcquireSingleton((IPendingIntentHelper**)&pendingIntentHelper);
            AutoPtr<IPendingIntent> pendingIntent;
            pendingIntentHelper->GetActivity(mContext, 0, intent, IPendingIntent::FLAG_UPDATE_CURRENT, (IPendingIntent**)&pendingIntent);
            builder->SetContentIntent(pendingIntent);
            break;
        }
    }

    // TODO: move to NotificationManager once we can mock it
    // XXX what to do about multi-user?
    String packageName;
    mContext->GetPackageName(&packageName);
    AutoPtr< ArrayOf<Int32> > idReceived;
    AutoPtr<INotification> notification;
    builder->GetNotification((INotification**)&notification);
    mNotifManager->EnqueueNotificationWithTag(packageName, packageName, tag,
            0x0, notification, (ArrayOf<Int32>**)&idReceived, IUserHandle::USER_OWNER);
    mActiveNotifs.Insert(tag);
}

/**
 * Show ongoing notification to reflect that {@link #mRestrictBackground}
 * has been enabled.
 */
void CNetworkPolicyManagerService::EnqueueRestrictedNotification(
    /* [in] */ const String& tag)
{
    AutoPtr<IResources> res;
    mContext->GetResources((IResources**)&res);
    AutoPtr<INotificationBuilder> builder;
    CNotificationBuilder::New(mContext, (INotificationBuilder**)&builder);

    AutoPtr<ICharSequence> title;
    res->GetText(R::string::data_usage_restricted_title, (ICharSequence**)&title);
    AutoPtr<ICharSequence> body;
    res->GetText(R::string::data_usage_restricted_body, (ICharSequence**)&body);

    builder->SetOnlyAlertOnce(TRUE);
    builder->SetOngoing(TRUE);
    builder->SetSmallIcon(R::drawable::stat_notify_error);
    builder->SetTicker(title);
    builder->SetContentTitle(title);
    builder->SetContentText(body);
    Int32 color;
    Ptr(mContext)->Func(mContext->GetResources)->GetColor(R::color::system_notification_accent_color, &color);

    AutoPtr<IIntent> intent = BuildAllowBackgroundDataIntent();
    AutoPtr<IPendingIntentHelper> helper;
    CPendingIntentHelper::AcquireSingleton((IPendingIntentHelper**)&helper);
    AutoPtr<IPendingIntent> pendingIntent;
    helper->GetBroadcast(mContext, 0, intent, IPendingIntent::FLAG_UPDATE_CURRENT, (IPendingIntent**)&pendingIntent);
    builder->SetContentIntent(pendingIntent);

    // TODO: move to NotificationManager once we can mock it
    // XXX what to do about multi-user?
    String packageName;
    mContext->GetPackageName(&packageName);
    AutoPtr<ArrayOf<Int32> > idReceived;
    AutoPtr<INotification> notification;
    builder->GetNotification((INotification**)&notification);
    mNotifManager->EnqueueNotificationWithTag(packageName, packageName, tag,
            0x0, notification, (ArrayOf<Int32>**)&idReceived, IUserHandle::USER_OWNER);
    mActiveNotifs.Insert(tag);
}

void CNetworkPolicyManagerService::CancelNotification(
    /* [in] */ const String& tag)
{
    // TODO: move to NotificationManager once we can mock it
    // XXX what to do about multi-user?
    String packageName;
    mContext->GetPackageName(&packageName);
    mNotifManager->CancelNotificationWithTag(
            packageName, tag, 0x0, IUserHandle::USER_OWNER);
}

/**
 * Proactively control network data connections when they exceed
 * {@link NetworkPolicy#limitBytes}.
 */
void CNetworkPolicyManagerService::UpdateNetworkEnabledLocked()
{
    if (LOGV) Slogger::V(TAG, "updateNetworkEnabledLocked()");

    // TODO: reset any policy-disabled networks when any policy is removed
    // completely, which is currently rare case.

    Int64 currentTime = CurrentTimeMillis();
    for (Int32 i = Ptr(mNetworkPolicy)->Func(mNetworkPolicy->GetSize) - 1; i >= 0; --i) {
        AutoPtr<INetworkPolicy> policy;
        mNetworkPolicy->ValueAt(i, (INetworkPolicy**)&policy);
        AutoPtr<INetworkTemplate> netTempl;
        policy->GetTemplate((INetworkTemplate**)&netTempl);
        // shortcut when policy has no limit
        Int64 limitBytes;
        policy->GetLimitBytes(&limitBytes);
        Boolean hasCycle;
        policy->HasCycle(&hasCycle);
        if (limitBytes == INetworkPolicy::LIMIT_DISABLED || hasCycle == FALSE) {
            SetNetworkTemplateEnabled(netTempl, TRUE);
            continue;
        }

        AutoPtr<INetworkPolicyManagerHelper> helper;
        CNetworkPolicyManagerHelper::AcquireSingleton((INetworkPolicyManagerHelper**)&helper);
        Int64 start;
        helper->ComputeLastCycleBoundary(currentTime, policy, &start);
        Int64 end = currentTime;
        Int64 totalBytes = GetTotalBytes(netTempl, start, end);

        // disable data connection when over limit and not snoozed
        Boolean isOverLimit;
        policy->IsOverLimit(totalBytes, &isOverLimit);
        Int64 lastLimitSnooze;
        policy->GetLastLimitSnooze(&lastLimitSnooze);
        Boolean overLimitWithoutSnooze = isOverLimit && (lastLimitSnooze < start);
        Boolean networkEnabled = !overLimitWithoutSnooze;

        SetNetworkTemplateEnabled(netTempl, networkEnabled);
    }
}

/**
 * Control {@link IConnectivityManager#setPolicyDataEnable(int, boolean)}
 * for the given {@link NetworkTemplate}.
 */
void CNetworkPolicyManagerService::SetNetworkTemplateEnabled(
    /* [in] */  INetworkTemplate* templ,
    /* [in] */  Boolean enabled)
{
    AutoPtr<ITelephonyManagerHelper> teleHelper;
    CTelephonyManagerHelper::AcquireSingleton((ITelephonyManagerHelper**)&teleHelper);
    AutoPtr<ITelephonyManager> tele;
    teleHelper->From(mContext, (ITelephonyManager**)&tele);
    Int32 rule;
    templ->GetMatchRule(&rule);
    switch (rule) {
        case INetworkTemplate::MATCH_MOBILE_3G_LOWER:
        case INetworkTemplate::MATCH_MOBILE_4G:
        case INetworkTemplate::MATCH_MOBILE_ALL: {
            // TODO: offer more granular control over radio states once
            // 4965893 is available.
            Int32 state;
            tele->GetSimState(&state);
            String templateSubscriberId;
            templ->GetSubscriberId(&templateSubscriberId);
            String teleSubscriberId;
            tele->GetSubscriberId(&teleSubscriberId);
            Boolean isEquals = teleSubscriberId.Equals(templateSubscriberId);
            if (state == ITelephonyManager::SIM_STATE_READY && isEquals == TRUE)
            {
                SetPolicyDataEnable(IConnectivityManager::TYPE_MOBILE, enabled);
                SetPolicyDataEnable(IConnectivityManager::TYPE_WIMAX, enabled);
            }
            break;
        }
        case INetworkTemplate::MATCH_WIFI:
            SetPolicyDataEnable(IConnectivityManager::TYPE_WIFI, enabled);
            break;
        case INetworkTemplate::MATCH_ETHERNET:
            SetPolicyDataEnable(IConnectivityManager::TYPE_ETHERNET, enabled);
            break;
        default:
            //throw new IllegalArgumentException("unexpected template");
            break;
    }
}

/**
 * Examine all connected {@link NetworkState}, looking for
 * {@link NetworkPolicy} that need to be enforced. When matches found, set
 * remaining quota based on usage cycle and historical stats.
 */
void CNetworkPolicyManagerService::UpdateNetworkRulesLocked()
{
    if (LOGV) Slogger::V(TAG, "updateIfacesLocked()");

    AutoPtr< ArrayOf<INetworkState*> > states;
    ECode ec = mConnManager->GetAllNetworkState((ArrayOf<INetworkState*>**)&states);
    if (FAILED(ec)) {
        // ignored; service lives in system_server
        return;
    }

    // If we are in restrict power mode, we want to treat all interfaces
    // as metered, to restrict access to the network by uid.  However, we
    // will not have a bandwidth limit.  Also only do this if restrict
    // background data use is *not* enabled, since that takes precendence
    // use over those networks can have a cost associated with it).
    Boolean powerSave = mRestrictPower && !mRestrictBackground;

    // First, generate identities of all connected networks so we can
    // quickly compare them against all defined policies below.
    AutoPtr<IArrayList> connIdents;
    CArrayList::New(states->GetLength(), (IArrayList**)&connIdents);
    AutoPtr<IArraySet> connIfaces;
    CArraySet::New(states->GetLength(), (IArrayList**)&connIfaces);
    for(Int32 i = 0; i < states->GetLength(); i++) {
        AutoPtr<INetworkState> state = (*states)[i];
        AutoPtr<INetworkInfo> info;
        state->GetNetworkInfo((INetworkInfo**)&info);
        Boolean isConnected;
        if (info->IsConnected(&isConnected), isConnected) {
            AutoPtr<INetworkIdentityHelper> helper;
            CNetworkIdentityHelper::AcquireSingleton((INetworkIdentityHelper**)&helper);
            AutoPtr<INetworkIdentity> ident;
            helper->BuildNetworkIdentity(mContext, state, (INetworkIdentity**)&ident);

            AutoPtr<ILinkProperties> prop;
            state->GetLinkProperties((ILinkProperties**)&prop);
            String baseIface;
            prop->GetInterfaceName(&baseIface);
            AutoPtr<IPairHelper> pairHelper;
            CPairHelper::AcquireSingleton((IPairHelper**)&pairHelper);
            if (!baseIface.IsNull()) {
                AutoPtr<IPair> pair;
                pairHelper->Create(StringUtils::ParseCharSequence(baseIface), ident, (IPair**)&pair);
                connIdents->Add(pair);
                if (powerSave) {
                    connIfaces->Add(StringUtils::ParseCharSequence(baseIface));
                }
            }

            // Stacked interfaces are considered to have same identity as
            // their parent network.
            AutoPtr<IList> stackedLinks;
            Ptr(state)->Func(state->GetLinkProperties)->GetStackedLinks((IList**)&stackedLinks);
            FOR_EACH(iter, stackedLinks) {
                AutoPtr<ILinkProperties> stackedLink = ILinkProperties::Probe(Ptr(iter)->Func(iter->GetNext));
                String stackedIface;
                stackedLink->GetInterfaceName(&stackedIface);
                if (!stackedIface.IsNull()) {
                    AutoPtr<IPair> pair;
                    pairHelper->Create(StringUtils::ParseCharSequence(stackedIface), ident, (IPair**)&pair);
                    connIdents->Add(pair);
                    if (powerSave) {
                        connIfaces->Add(StringUtils::ParseCharSequence(stackedIface));
                    }
                }
            }
        }
    }

    // Apply policies against all connected interfaces found above
    mNetworkRules.Clear();
    AutoPtr<IArrayList> ifaceList;
    CArrayList::New((IArrayList**)&ifaceList);
    for (Int32 i = Ptr(mNetworkPolicy)->Func(mNetworkPolicy->GetSize) - 1; i >= 0; --i) {
        AutoPtr<INetworkPolicy> policy;
        mNetworkPolicy->ValueAt(i, (INetworkPolicy**)policy);
        ifaceList.Clear();
        for (Int32 j = Ptr(connIdents)->Func(connIdents->GetSize) - 1; j >= 0; j--) {
            AutoPtr<IInterface> obj;
            connIdents->Get(j, (IInterface**)&obj);
            AutoPtr<IPair> ident = IPair::Probe(obj);
            Boolean isMatches;
            Ptr(policy)->Func(policy->GetTemplate)->Matches(Ptr(ident)->Func(ident->GetSecond), &isMatches);
            if (isMatches) {
                ifaceList->Add(ident.first);
            }
        }

        if (Ptr(ifaceList)->Func(ifaceList->GetSize) > 0) {
            AutoPtr<ArrayOf<IInterface*> > ifacesArray;
            ifaceList->ToArray((ArrayOf<IInterface*>**)&ifacesArray);
            AutoPtr<IArrayOf> ifaces;
            CArrayOf::New(EIID_ICharSequence, ifacesArray->GetLength(), (IArrayOf**)&ifaces);
            for (Int32 k = 0; k < ifacesArray->GetLength(); ++i) {
                ifaces->Set(k, (*ifacesArray)[k]);
            }
            mNetworkRules->Put(policy, ifaces);
        }
    }

    Int64 lowestRule = Elastos::Core::Math::INT64_MAX_VALUE;
    AutoPtr<IArraySet> newMeteredIfaces;
    CArraySet::New(states->GetLength(), (IArraySet**)&newMeteredIfaces);

    // apply each policy that we found ifaces for; compute remaining data
    // based on current cycle and historical stats, and push to kernel.
    Int64 currentTime = CurrentTimeMillis();
    for (Int32 i = Ptr(mNetworkRules)->Func(mNetworkRules->GetSize) - 1; i >= 0; --i) {
        AutoPtr<IInterface> obj;
        mNetworkRules->GetKeyAt(i, (IInterface**)&obj);
        AutoPtr<INetworkPolicy> policy = INetworkPolicy::Probe(obj);
        obj = NULL;
        mNetworkRules->GetValueAt(i, (IInterface**)&obj);
        AutoPtr<IArrayOf> ifaces = IArrayOf::Probe(obj);

        Int64 start, totalBytes;
        Boolean hasCycle;
        if (policy->HasCycle(&hasCycle), hasCycle) {
            AutoPtr<INetworkPolicyManagerHelper> helper;
            CNetworkPolicyManagerHelper::AcquireSingleton((INetworkPolicyManagerHelper**)&helper);
            helper->ComputeLastCycleBoundary(currentTime, policy, &start);
            AutoPtr<INetworkTemplate> temp;
            policy->GetTemplate((INetworkTemplate**)&temp);
            totalBytes = GetTotalBytes(temp, start, currentTime);
        }
        else {
            start = Elastos::Core::Math::INT64_MAX_VALUE;
            totalBytes = 0;
        }

        if (LOGD) {
//            Slogger::D(TAG, "applying policy " + policy.toString() + " to ifaces "
//                    + Arrays.toString(ifaces));
            Slogger::D(TAG, "applying policy");
        }

        Int64 waringBytes;
        policy->GetWarningBytes(&waringBytes);
        Boolean hasWarning = (waringBytes != INetworkPolicy::LIMIT_DISABLED);
        Int64 limitBytes;
        policy->GetLimitBytes(&limitBytes);
        Boolean hasLimit = (limitBytes != INetworkPolicy::LIMIT_DISABLED);
        Boolean metered;
        if (hasLimit || (policy->GetMetered(&metered), metered)) {
            Int64 quotaBytes;
            Int64 lastLimitSnooze;
            if (!hasLimit) {
                // metered network, but no policy limit; we still need to
                // restrict apps, so push really high quota.
                quotaBytes = Elastos::Core::Math::INT64_MAX_VALUE;
            }
            else if (policy->GetLastLimitSnooze(&lastLimitSnooze), lastLimitSnooze >= start) {
                // snoozing past quota, but we still need to restrict apps,
                // so push really high quota.
                quotaBytes = Elastos::Core::Math::INT64_MAX_VALUE;
            }
            else {
                // remaining "quota" bytes are based on total usage in
                // current cycle. kernel doesn't like 0-byte rules, so we
                // set 1-byte quota and disable the radio later.
                quotaBytes = Elastos::Core::Math::Max((Int64)1, limitBytes - totalBytes);
            }

            if (Ptr(ifaces)->Func(ifaces->GetLength) > 1) {
                // TODO: switch to shared quota once NMS supports
                Slogger::W(TAG, "shared quota unsupported; generating rule for each iface");
            }

            for(Int32 i = 0; i < Ptr(ifaces)->Func(ifaces->GetLength); i++) {
                AutoPtr<IInterface> obj;
                ifaces->Get(i, (IInterface**)&obj);
                String iface;
                ICharSequence::Probe(obj)->ToString(&iface);
                RemoveInterfaceQuota(iface);
                SetInterfaceQuota(iface, quotaBytes);
                newMeteredIfaces->Add(Obj);
                if (powerSave) {
                    connIfaces->Remove(obj);
                }
            }
        }

        // keep track of lowest warning or limit of active policies
        if (hasWarning && waringBytes < lowestRule) {
            lowestRule = waringBytes;
        }
        if (hasLimit && limitBytes < lowestRule) {
            lowestRule = limitBytes;
        }
    }

        for (Int32 i = Ptr(connIfaces)->Func(GetSize)-1; i >= 0; i--) {
            AutoPtr<IInterface> obj;
            connIfaces->GetValueAt(i, (IInterface**)&obj);
            String iface;
            ICharSequence::Probe(obj)->ToString(iface);
            RemoveInterfaceQuota(iface);
            SetInterfaceQuota(iface, Math::INT64_MAX_VALUE);
            newMeteredIfaces->Add(Obj);
        }

    AutoPtr<IInteger64> iobj;
    CInteger64::New(lowestRule, (IInteger64**)&iobj);
    AutoPtr<IMessage> msg;
    mHandler->ObtainMessage(MSG_ADVISE_PERSIST_THRESHOLD, (IMessage**)&msg);
    msg->SetObj(iobj);
    Boolean result;
    mHandler->SendMessage(msg, &result);

    // remove quota on any trailing interfaces
    for (Int32 i = Ptr(mMeteredIfaces)->Func(mMeteredIfaces->GetSize) - 1; i >= 0; i--) {
        AutoPtr<IInterface> obj;
        mMeteredIfaces->GetValueAt(i, (IInterface**)&obj);
        String iface;
        ICharSequence::Probe(obj)->ToString(&iface);
        Boolean bContains;
        newMeteredIfaces->Contains(obj, &bContains);
        if (!bContains) {
            RemoveInterfaceQuota(obj);
        }
    }
    mMeteredIfaces.Clear();
    mMeteredIfaces.Insert(newMeteredIfaces.Begin(), newMeteredIfaces.End());

    AutoPtr<IArrayOf> meteredIfaces;
    CArrayOf::New(EIID_ICharSequence, mMeteredIfaces.GetSize(), (IArrayOf**)&meteredIfaces);
    Int32 i;
    for (i = 0, miIt = mMeteredIfaces.Begin(); miIt != mMeteredIfaces.End(); ++miIt) {
        AutoPtr<ICharSequence> seq;
        CString::New(*miIt, (ICharSequence**)&seq);
        meteredIfaces->Put(i, seq);
    }

    AutoPtr<IMessage> msg2;
    mHandler->ObtainMessage(MSG_METERED_IFACES_CHANGED, (IMessage**)&msg2);
    msg2->SetObj(meteredIfaces);
    mHandler->SendMessage(msg2, &result);
}

/**
 * Once any {@link #mNetworkPolicy} are loaded from disk, ensure that we
 * have at least a default mobile policy defined.
 */
void CNetworkPolicyManagerService::EnsureActiveMobilePolicyLocked()
{
    if (LOGV) {
        Slogger::V(TAG, "ensureActiveMobilePolicyLocked()");
    }

    if (mSuppressDefaultPolicy) {
        return;
    }

    AutoPtr<ITelephonyManagerHelper> teleHelper;
    CTelephonyManagerHelper::AcquireSingleton((ITelephonyManagerHelper**)&teleHelper);
    AutoPtr<ITelephonyManager> tele;
    teleHelper->From(mContext, (ITelephonyManager**)&tele);

    // avoid creating policy when SIM isn't ready
    Int32 state;
    tele->GetSimState(&state);
    if (state != ITelephonyManager::SIM_STATE_READY) {
        return;
    }

    String subscriberId;
    tele->GetSubscriberId(&subscriberId);
    AutoPtr<INetworkIdentity> probeIdent;
    CNetworkIdentity::New(IConnectivityManager::TYPE_MOBILE, ITelephonyManager::NETWORK_TYPE_UNKNOWN,
                            subscriberId, String(NULL), FALSE, (INetworkIdentity**)&probeIdent);

    // examine to see if any policy is defined for active mobile
    Boolean mobileDefined = FALSE;

    for (Int32 i = Ptr(mNetworkPolicy)->Func(mNetworkPolicy->GetSize)-1; i >= 0; i--) {
        AutoPtr<IInterface> obj;
        mNetworkPolicy->GetValueAt(i, (IInterface**)&obj);
        Boolean isMatches;
        AutoPtr<INetworkTemplate> tp;
        INetworkPolicy::Probe(obj)->GetTemplate((INetworkTemplate**)&tp);
        tp->Matches(probeIdent, &isMatches);
        if (isMatches) {
            mobileDefined = TRUE;
            break;
        }
    }

    if (FALSE == mobileDefined) {
        Slogger::I(TAG, "no policy for active mobile network; generating default policy");

        // build default mobile policy, and assume usage cycle starts today
        AutoPtr<IResources> res;
        mContext->GetResources((IResources**)&res);
        Int32 defaultWarning;
        res->GetInteger(R::integer::config_networkPolicyDefaultWarning, &defaultWarning);
        Int64 warningBytes = defaultWarning * ITrafficStats::MB_IN_BYTES;

        AutoPtr<ITime> time;
        CTime::New((ITime**)&time);
        time->SetToNow();

        Int32 cycleDay;
        time->GetMonthDay(&cycleDay);
        String cycleTimezone;
        time->GetTimeZone(&cycleTimezone);

        AutoPtr<INetworkTemplateHelper> ntHelper;
        CNetworkTemplateHelper::AcquireSingleton((INetworkTemplateHelper**)&ntHelper);
        AutoPtr<INetworkTemplate> nwkTemplate;
        ntHelper->BuildTemplateMobileAll(subscriberId, (INetworkTemplate**)&nwkTemplate);
        AutoPtr<INetworkPolicy> policy;
        CNetworkPolicy::New(nwkTemplate, cycleDay, cycleTimezone, warningBytes,
                            INetworkPolicy::LIMIT_DISABLED, INetworkPolicy::SNOOZE_NEVER,
                            INetworkPolicy::SNOOZE_NEVER, TRUE, TRUE, (INetworkPolicy**)&policy);
        AddNetworkPolicyLocked(policy);
    }
}

void CNetworkPolicyManagerService::ReadPolicyLocked()
{
    if (LOGV) Slogger::V(TAG, "readPolicyLocked()");

    // clear any existing policy and read from disk
    mNetworkPolicy.Clear();
    mUidPolicy.Clear();

    AutoPtr<IFileInputStream> fis;
//     //try {
    AutoPtr<IXmlPullParser> in;
    Int32 type, version;
    FAIL_GOTO(mPolicyFile->OpenRead((IFileInputStream**)&fis), ERROR);
    in = Xml::NewPullParser();
    in->SetInput((IInputStream*)fis.Get(), String(NULL));

    version = VERSION_INIT;
    while (in->Next(&type), type != IXmlPullParser::END_DOCUMENT) {
        String tag;
        in->GetName(&tag);
        if (type == IXmlPullParser::START_TAG) {
            if (TAG_POLICY_LIST.Equals(tag)) {
                FAIL_GOTO(XmlUtils::ReadInt32Attribute(in, ATTR_VERSION, &version), ERROR);
                if (version >= VERSION_ADDED_RESTRICT_BACKGROUND) {
                    FAIL_GOTO(XmlUtils::ReadBooleanAttribute(
                            in, ATTR_RESTRICT_BACKGROUND, &mRestrictBackground), ERROR);
                }
                else {
                    mRestrictBackground = FALSE;
                }
            }
            else if (TAG_NETWORK_POLICY.Equals(tag)) {
                Int32 networkTemplate;
                String subscriberId;
                String networkId;
                Int32 cycleDay;
                String cycleTimezone;
                Int64 warningBytes;
                Int64 limitBytes;
                Int64 lastLimitSnooze;
                Boolean metered;
                Int64 lastWarningSnooze;
                Boolean inferred;
                AutoPtr<INetworkTemplate> nwkTemplate;
                AutoPtr<INetworkPolicy> policy;
                FAIL_GOTO(XmlUtils::ReadInt32Attribute(in, ATTR_NETWORK_TEMPLATE, &networkTemplate), ERROR);
                FAIL_GOTO(in->GetAttributeValue(String(NULL), ATTR_SUBSCRIBER_ID, &subscriberId), ERROR);
                if (version >= VERSION_ADDED_NETWORK_ID) {
                    FAIL_GOTO(in->GetAttributeValue(String(NULL), ATTR_NETWORK_ID, &networkId), ERROR);
                }
                else {
                    networkId = NULL;
                }
                FAIL_GOTO(XmlUtils::ReadInt32Attribute(in, ATTR_CYCLE_DAY, &cycleDay), ERROR);
                if (version >= VERSION_ADDED_TIMEZONE) {
                    FAIL_GOTO(in->GetAttributeValue(String(NULL), ATTR_CYCLE_TIMEZONE, &cycleTimezone), ERROR);
                }
                else {
                    cycleTimezone = ITime::TIMEZONE_UTC;
                }
                FAIL_GOTO(XmlUtils::ReadInt64Attribute(in, ATTR_WARNING_BYTES, &warningBytes), ERROR);
                FAIL_GOTO(XmlUtils::ReadInt64Attribute(in, ATTR_LIMIT_BYTES, &limitBytes), ERROR);
                if (version >= VERSION_SPLIT_SNOOZE) {
                    FAIL_GOTO(XmlUtils::ReadInt64Attribute(in, ATTR_LAST_LIMIT_SNOOZE, &lastLimitSnooze), ERROR);
                }
                else if (version >= VERSION_ADDED_SNOOZE) {
                    FAIL_GOTO(XmlUtils::ReadInt64Attribute(in, ATTR_LAST_SNOOZE, &lastLimitSnooze), ERROR);
                }
                else {
                    lastLimitSnooze = INetworkPolicy::SNOOZE_NEVER;
                }
                if (version >= VERSION_ADDED_METERED) {
                    FAIL_GOTO(XmlUtils::ReadBooleanAttribute(in, ATTR_METERED, &metered), ERROR);
                }
                else {
                    switch (networkTemplate) {
                        case INetworkTemplate::MATCH_MOBILE_3G_LOWER:
                        case INetworkTemplate::MATCH_MOBILE_4G:
                        case INetworkTemplate::MATCH_MOBILE_ALL:
                            metered = TRUE;
                            break;
                        default:
                            metered = FALSE;
                    }
                }
                if (version >= VERSION_SPLIT_SNOOZE) {
                    FAIL_GOTO(XmlUtils::ReadInt64Attribute(in, ATTR_LAST_WARNING_SNOOZE, &lastWarningSnooze), ERROR);
                }
                else {
                    lastWarningSnooze = INetworkPolicy::SNOOZE_NEVER;
                }
                if (version >= VERSION_ADDED_INFERRED) {
                    FAIL_GOTO(XmlUtils::ReadBooleanAttribute(in, ATTR_INFERRED, &inferred), ERROR);
                }
                else {
                    inferred = FALSE;
                }

                CNetworkTemplate::New(networkTemplate, subscriberId, networkId, (INetworkTemplate**)&nwkTemplate);
                CNetworkPolicy::New(nwkTemplate, cycleDay,
                        cycleTimezone, warningBytes, limitBytes, lastWarningSnooze,
                        lastLimitSnooze, metered, inferred, (INetworkPolicy**)&policy);
                mNetworkPolicy[nwkTemplate] = policy;
            }
            else if (TAG_UID_POLICY.Equals(tag)) {
                Int32 uid;
                Int32 policy;
                AutoPtr<IUserHandleHelper> helper;
                Boolean isapp;
                FAIL_GOTO(XmlUtils::ReadInt32Attribute(in, ATTR_UID, &uid), ERROR);
                FAIL_GOTO(XmlUtils::ReadInt32Attribute(in, ATTR_POLICY, &policy), ERROR);

                CUserHandleHelper::AcquireSingleton((IUserHandleHelper**)&helper);
                helper->IsApp(uid, &isapp);
                if (isapp == TRUE) {
                    SetUidPolicyUncheckedLocked(uid, policy, FALSE);
                }
                else {
                    Slogger::W(TAG, "unable to apply policy to UID %d; ignoring", uid);
                }
            }
            else if (TAG_APP_POLICY.Equals(tag)) {
                Int32 appId;
                Int32 policy;
                AutoPtr<IUserHandleHelper> helper;
                Int32 uid;
                Boolean isapp;
                FAIL_GOTO(XmlUtils::ReadInt32Attribute(in, ATTR_APP_ID, &appId), ERROR);
                FAIL_GOTO(XmlUtils::ReadInt32Attribute(in, ATTR_POLICY, &policy), ERROR);

                // TODO: set for other users during upgrade
                CUserHandleHelper::AcquireSingleton((IUserHandleHelper**)&helper);
                helper->GetUid(IUserHandle::USER_OWNER, appId, &uid);
                helper->IsApp(uid, &isapp);
                if (isapp == TRUE) {
                    SetUidPolicyUncheckedLocked(uid, policy, FALSE);
                }
                else {
                    Slogger::W(TAG, "unable to apply policy to UID %d; ignoring", uid);
                }
            }
        }
    }
ERROR:
// //    } catch (FileNotFoundException e) {
// //        // missing policy is okay, probably first boot
// //        upgradeLegacyBackgroundData();
// //    } catch (IOException e) {
// //        Log.wtf(TAG, "problem reading network policy", e);
// //    } catch (XmlPullParserException e) {
// //        Log.wtf(TAG, "problem reading network policy", e);
// //    } finally {
    AutoPtr<IIoUtils> ioUtils;
    CIoUtils::AcquireSingleton((IIoUtils**)&ioUtils);
    ioUtils->CloseQuietly(fis);
// //    }
}

/**
 * Upgrade legacy background data flags, notifying listeners of one last
 * change to always-true.
 */
void CNetworkPolicyManagerService::UpgradeLegacyBackgroundData()
{
    AutoPtr<IContentResolver> resolver;
    mContext->GetContentResolver((IContentResolver**)&resolver);
    AutoPtr<ISettingsSecure> settingsSecure;
    CSettingsSecure::AcquireSingleton((ISettingsSecure**)&settingsSecure);
    Int32 data = 0;
    settingsSecure->GetInt32(resolver, ISettingsSecure::BACKGROUND_DATA, 1, &data);
    mRestrictBackground = data != 1;

    // kick off one last broadcast if restricted
    if (mRestrictBackground == TRUE) {
        AutoPtr<IIntent> broadcast;
        CIntent::New(IConnectivityManager::ACTION_BACKGROUND_DATA_SETTING_CHANGED, (IIntent**)&broadcast);
        AutoPtr<IUserHandleHelper> helper;
        CUserHandleHelper::AcquireSingleton((IUserHandleHelper**)&helper);
        AutoPtr<IUserHandle> ALL;
        helper->GetALL((IUserHandle**)&ALL);
        mContext->SendBroadcastAsUser(broadcast, ALL);
    }
}

void CNetworkPolicyManagerService::WritePolicyLocked()
{
    //if (LOGV) Slog.v(TAG, "writePolicyLocked()");

    AutoPtr<IFileOutputStream> fos;
    AutoPtr<IXmlSerializer> out;
    AutoPtr<IBoolean> value;
    HashMap<Int32, Int32>::Iterator uidPolicyIt;
    // //try {
    FAIL_GOTO(mPolicyFile->StartWrite((IFileOutputStream**)&fos), ERROR);

    CFastXmlSerializer::New((IFastXmlSerializer**)&out);
    FAIL_GOTO(out->SetOutput((IOutputStream*)fos.Get(), String("utf-8")), ERROR);
    CBoolean::New(TRUE, (IBoolean**)&value);
    FAIL_GOTO(out->StartDocument(String(NULL), value), ERROR);

    FAIL_GOTO(out->WriteStartTag(String(NULL), TAG_POLICY_LIST), ERROR);
    FAIL_GOTO(XmlUtils::WriteInt32Attribute(out, ATTR_VERSION, VERSION_LATEST), ERROR);
    FAIL_GOTO(XmlUtils::WriteBooleanAttribute(out, ATTR_RESTRICT_BACKGROUND, mRestrictBackground), ERROR);

    // write all known network policies
    for (Int32 i = 0; i < Ptr(mNetworkPolicy)->Func(mNetworkPolicy->GetSize); i++) {
        AutoPtr<IInterface> obj;
        mNetworkPolicy->GetValueAt(i, (IInterface**)&obj);
        AutoPtr<INetworkPolicy> policy = INetworkPolicy::Probe(obj);
        AutoPtr<INetworkTemplate> nwkTemplate;
        Int32 rule;
        String subscriberId;
        String networkId;
        Int32 cycleDay;
        String cycleTimezone;
        Int64 waringBytes, limitBytes, lastWarningSnooze, lastLimitSnooze;
        Boolean metered, inferred;
        policy->GetTemplate((INetworkTemplate**)&nwkTemplate);

        FAIL_GOTO(out->WriteStartTag(String(NULL), TAG_NETWORK_POLICY), ERROR);
        nwkTemplate->GetMatchRule(&rule);
        FAIL_GOTO(XmlUtils::WriteInt32Attribute(out, ATTR_NETWORK_TEMPLATE, rule), ERROR);
        nwkTemplate->GetSubscriberId(&subscriberId);
        if (!subscriberId.IsNull()) {
            FAIL_GOTO(out->WriteAttribute(String(NULL), ATTR_SUBSCRIBER_ID, subscriberId), ERROR);
        }
        nwkTemplate->GetNetworkId(&networkId);
        if (!networkId.IsNull()) {
            FAIL_GOTO(out->WriteAttribute(String(NULL), ATTR_NETWORK_ID, networkId), ERROR);
        }
        policy->GetCycleDay(&cycleDay);
        FAIL_GOTO(XmlUtils::WriteInt32Attribute(out, ATTR_CYCLE_DAY, cycleDay), ERROR);
        policy->GetCycleTimezone(&cycleTimezone);
        FAIL_GOTO(out->WriteAttribute(String(NULL), ATTR_CYCLE_TIMEZONE, cycleTimezone), ERROR);
        policy->GetWarningBytes(&waringBytes);
        FAIL_GOTO(XmlUtils::WriteInt64Attribute(out, ATTR_WARNING_BYTES, waringBytes), ERROR);
        policy->GetLimitBytes(&limitBytes);
        FAIL_GOTO(XmlUtils::WriteInt64Attribute(out, ATTR_LIMIT_BYTES, limitBytes), ERROR);
        policy->GetLastWarningSnooze(&lastWarningSnooze);
        FAIL_GOTO(XmlUtils::WriteInt64Attribute(out, ATTR_LAST_WARNING_SNOOZE, lastWarningSnooze), ERROR);
        policy->GetLastLimitSnooze(&lastLimitSnooze);
        FAIL_GOTO(XmlUtils::WriteInt64Attribute(out, ATTR_LAST_LIMIT_SNOOZE, lastLimitSnooze), ERROR);
        policy->GetMetered(&metered);
        FAIL_GOTO(XmlUtils::WriteBooleanAttribute(out, ATTR_METERED, metered), ERROR);
        policy->GetInferred(&inferred);
        FAIL_GOTO(XmlUtils::WriteBooleanAttribute(out, ATTR_INFERRED, inferred), ERROR);
        FAIL_GOTO(out->WriteEndTag(String(NULL), TAG_NETWORK_POLICY), ERROR);
    }

    // write all known uid policies
    for(uidPolicyIt = mUidPolicy.Begin(); uidPolicyIt != mUidPolicy.End(); ++uidPolicyIt) {
        Int32 uid = uidPolicyIt->mFirst;
        Int32 policy = uidPolicyIt->mSecond;

        // skip writing empty policies
        if (policy == INetworkPolicyManager::POLICY_NONE) {
            continue;
        }

        FAIL_GOTO(out->WriteStartTag(String(NULL), TAG_UID_POLICY), ERROR);
        FAIL_GOTO(XmlUtils::WriteInt32Attribute(out, ATTR_UID, uid), ERROR);
        FAIL_GOTO(XmlUtils::WriteInt32Attribute(out, ATTR_POLICY, policy), ERROR);
        FAIL_GOTO(out->WriteEndTag(String(NULL), TAG_UID_POLICY), ERROR);
    }

    FAIL_GOTO(out->WriteEndTag(String(NULL), TAG_POLICY_LIST), ERROR);
    FAIL_GOTO(out->EndDocument(), ERROR);

    FAIL_GOTO(mPolicyFile->FinishWrite(fos), ERROR);
    return;
    // //} catch (IOException e) {
ERROR:
    if (fos != NULL) {
        mPolicyFile->FailWrite(fos);
    }
    // //}
}

ECode CNetworkPolicyManagerService::SetUidPolicy(
    /* [in] */ Int32 uid,
    /* [in] */ Int32 policy)
{
    FAIL_RETURN(mContext->EnforceCallingOrSelfPermission(Elastos::Droid::Manifest::permission::MANAGE_NETWORK_POLICY, TAG));

    AutoPtr<IUserHandleHelper> helper;
    CUserHandleHelper::AcquireSingleton((IUserHandleHelper**)&helper);
    Boolean isapp;
    helper->IsApp(uid, &isapp);
    if (!isapp) {
        //throw new IllegalArgumentException("cannot apply policy to UID " + uid);
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }

    synchronized(mRulesLock) {
        Int32 oldPolicy;
        mUidPolicy->Get(uid, POLICY_NONE, &oldPolicy);
        if (oldPolicy != policy) {
            SetUidPolicyUncheckedLocked(uid, policy, TRUE);
        }
    }
    return NOERROR;
}

void CNetworkPolicyManagerService::SetUidPolicyUncheckedLocked(
    /* [in] */ Int32 uid,
    /* [in] */ Int32 policy,
    /* [in] */ Boolean persist)
{
    mUidPolicy[uid] = policy;

    // uid policy changed, recompute rules and persist policy.
    UpdateRulesForUidLocked(uid);
    if (persist) {
        WritePolicyLocked();
    }
}

ECode CNetworkPolicyManagerService::GetUidPolicy(
    /* [in] */  Int32 uid,
    /* [out] */ Int32* policy)
{
    VALIDATE_NOT_NULL(policy);

    FAIL_RETURN(mContext->EnforceCallingOrSelfPermission(Elastos::Droid::Manifest::permission::MANAGE_NETWORK_POLICY, TAG));

    synchronized(mRulesLock) {
        HashMap<Int32, Int32>::Iterator it = mUidPolicy.Find(uid);
        if (it != mUidPolicy.End()) {
            *policy = it->mSecond;
        }
        else *policy = INetworkPolicyManager::POLICY_NONE;
    }
    return NOERROR;
}

ECode CNetworkPolicyManagerService::GetUidsWithPolicy(
    /* [in] */ Int32 policy,
    /* [out, callee] */ ArrayOf<Int32>** _uids)
{
    VALIDATE_NOT_NULL(_uids);
    FAIL_RETURN(mContext->EnforceCallingOrSelfPermission(Elastos::Droid::Manifest::permission::MANAGE_NETWORK_POLICY, TAG));

    AutoPtr<ArrayOf<Int32> > uids = ArrayOf<Int32>::Alloc(0);
    synchronized(mRulesLock) {
        HashMap<Int32, Int32>::Iterator it;
        for(it = mUidPolicy.Begin(); it != mUidPolicy.End(); ++it) {
            Int32 uid = it->mFirst;
            Int32 uidPolicy = it->mSecond;
            if (uidPolicy == policy) {
                uids = ArrayUtils::AppendInt32(uids, uid);
            }
        }
    }
    *_uids = uids;
    REFCOUNT_ADD(*_uids);
    return NOERROR;
}

/**
 * Remove any policies associated with given {@link UserHandle}, persisting
 * if any changes are made.
 */
void CNetworkPolicyManagerService::RemovePoliciesForUserLocked(
    /* [in] */ Int32 userId)
{
    if (LOGV) {
        Slogger::V(TAG, "removePoliciesForUserLocked()");
    }

    AutoPtr<IUserHandleHelper> uhhelper;
    CUserHandleHelper::AcquireSingleton((IUserHandleHelper**)&uhhelper);
    AutoPtr<ArrayOf<Int32> > uids = ArrayOf<Int32>::Alloc(0);
    HashMap<Int32, Int32>::Iterator it;
    for(it = mUidPolicy.Begin(); it != mUidPolicy.End(); ++it) {
        Int32 uid = it->mFirst;
        Int32 uidNew;
        uhhelper->GetUserId(uid, &uidNew);
        if (uidNew == userId) {
            uids = ArrayUtils::AppendInt32(uids, uid);
        }
    }

    if (uids->GetLength() > 0) {
        for(Int32 i = 0; i < uids->GetLength(); i++) {
            Int32 uid = (*uids)[i];
            mUidPolicy.Erase(uid);
            UpdateRulesForUidLocked(uid);
        }
        WritePolicyLocked();
    }
}

ECode CNetworkPolicyManagerService::RegisterListener(
    /* [in] */ INetworkPolicyListener* listener)
{
    // TODO: create permission for observing network policy
    FAIL_RETURN(mContext->EnforceCallingOrSelfPermission(
            Elastos::Droid::Manifest::permission::CONNECTIVITY_INTERNAL, TAG));

    Boolean result;
    return mListeners->Register(listener, &result);

    // TODO: consider dispatching existing rules to new listeners
}

ECode CNetworkPolicyManagerService::UnregisterListener(
    /* [in] */ INetworkPolicyListener* listener)
{
    // TODO: create permission for observing network policy
    FAIL_RETURN(mContext->EnforceCallingOrSelfPermission(
            Elastos::Droid::Manifest::permission::CONNECTIVITY_INTERNAL, TAG));

    Boolean result;
    return mListeners->Unregister(listener, &result);
}

ECode CNetworkPolicyManagerService::SetNetworkPolicies(
    /* [in] */ ArrayOf<INetworkPolicy*>* policies)
{
    FAIL_RETURN(mContext->EnforceCallingOrSelfPermission(
            Elastos::Droid::Manifest::permission::MANAGE_NETWORK_POLICY, TAG));

    MaybeRefreshTrustedTime();
    synchronized(mRulesLock) {
        mNetworkPolicy.Clear();
        for (Int32 i = 0; i <  policies->GetLength(); i++) {
            AutoPtr<INetworkPolicy> policy = (*policies)[i];
            AutoPtr<INetworkTemplate> nwkTemplate;
            policy->GetTemplate((INetworkTemplate**)&nwkTemplate);
            mNetworkPolicy[nwkTemplate] = policy;
        }

        UpdateNetworkEnabledLocked();
        UpdateNetworkRulesLocked();
        UpdateNotificationsLocked();
        WritePolicyLocked();
    }
    return NOERROR;
}

void CNetworkPolicyManagerService::AddNetworkPolicyLocked(
    /* [in] */ INetworkPolicy* policy)
{
    AutoPtr<INetworkTemplate> nwkTemplate;
    policy->GetTemplate((INetworkTemplate**)&nwkTemplate);
    mNetworkPolicy[nwkTemplate] = policy;

    UpdateNetworkEnabledLocked();
    UpdateNetworkRulesLocked();
    UpdateNotificationsLocked();
    WritePolicyLocked();
}

ECode CNetworkPolicyManagerService::GetNetworkPolicies(
    /* [out, callee] */ ArrayOf<INetworkPolicy*>** policies)
{
    VALIDATE_NOT_NULL(policies);

    FAIL_RETURN(mContext->EnforceCallingOrSelfPermission(
            Elastos::Droid::Manifest::permission::MANAGE_NETWORK_POLICY, TAG));
    FAIL_RETURN(mContext->EnforceCallingOrSelfPermission(
            Elastos::Droid::Manifest::permission::READ_PHONE_STATE, TAG));

    synchronized(mRulesLock) {
        *policies = ArrayOf<INetworkPolicy*>::Alloc(mNetworkPolicy.GetSize());
        REFCOUNT_ADD(*policies);
        Int32 index = 0;
        HashMap<AutoPtr<INetworkTemplate>, AutoPtr<INetworkPolicy> >::Iterator it;
        for (it = mNetworkPolicy.Begin(); it != mNetworkPolicy.End(); ++it, ++index) {
            (*policies)->Set(index, it->mSecond);
        }
    }
    return NOERROR;
}

ECode CNetworkPolicyManagerService::SnoozeLimit(
    /* [in] */  INetworkTemplate* templ)
{
    FAIL_RETURN(mContext->EnforceCallingOrSelfPermission(
            Elastos::Droid::Manifest::permission::MANAGE_NETWORK_POLICY, TAG));

    const Int64 token = Binder::ClearCallingIdentity();
    //try {
    ECode ec = PerformSnooze(templ, TYPE_LIMIT);
    //} finally {
    Binder::RestoreCallingIdentity(token);
    //}
    return ec;
}

ECode CNetworkPolicyManagerService::PerformSnooze(
    /* [in] */ INetworkTemplate* templ,
    /* [in] */ Int32 type)
{
    MaybeRefreshTrustedTime();
    Int64 currentTime = CurrentTimeMillis();
    synchronized(mRulesLock) {
        // find and snooze local policy that matches
        AutoPtr<INetworkPolicy> policy;
        HashMap< AutoPtr<INetworkTemplate>, AutoPtr<INetworkPolicy> >::Iterator it
                = mNetworkPolicy.Find(templ);
        if (it != mNetworkPolicy.End()) {
            policy = it->mSecond;
        }
        if (policy == NULL) {
            //throw new IllegalArgumentException("unable to find policy for " + templ);
            return E_ILLEGAL_ARGUMENT_EXCEPTION;
        }

        switch (type) {
            case TYPE_WARNING:
                policy->SetLastWarningSnooze(currentTime);
                break;
            case TYPE_LIMIT:
                policy->SetLastLimitSnooze(currentTime);
                break;
            default:
                //throw new IllegalArgumentException("unexpected type");
                return E_ILLEGAL_ARGUMENT_EXCEPTION;
        }

        UpdateNetworkEnabledLocked();
        UpdateNetworkRulesLocked();
        UpdateNotificationsLocked();
        WritePolicyLocked();
    }
    return NOERROR;
}

ECode CNetworkPolicyManagerService::SetRestrictBackground(
    /* [in] */ Boolean restrictBackground)
{
    FAIL_RETURN(mContext->EnforceCallingOrSelfPermission(
            Elastos::Droid::Manifest::permission::MANAGE_NETWORK_POLICY, TAG));

    MaybeRefreshTrustedTime();
    synchronized(mRulesLock) {
        mRestrictBackground = restrictBackground;
        UpdateRulesForRestrictBackgroundLocked();
        UpdateNotificationsLocked();
        WritePolicyLocked();
    }

    AutoPtr<IMessage> msg;
    mHandler->ObtainMessage(MSG_RESTRICT_BACKGROUND_CHANGED, (IMessage**)&msg);
    msg->SetArg1(restrictBackground ? 1 : 0);
    Boolean result;
    return mHandler->SendMessage(msg, &result);
}

ECode CNetworkPolicyManagerService::GetRestrictBackground(
    /* [out] */ Boolean* restrictBackground)
{
    VALIDATE_NOT_NULL(restrictBackground);

    FAIL_RETURN(mContext->EnforceCallingOrSelfPermission(
            Elastos::Droid::Manifest::permission::MANAGE_NETWORK_POLICY, TAG));

    synchronized(mRulesLock) {
        *restrictBackground = mRestrictBackground;
    }
    return NOERROR;
}

AutoPtr<INetworkPolicy> CNetworkPolicyManagerService::FindPolicyForNetworkLocked(
    /* [in] */ INetworkIdentity* ident)
{
    HashMap<AutoPtr<INetworkTemplate>, AutoPtr<INetworkPolicy> >::Iterator it;
    for(it = mNetworkPolicy.Begin(); it != mNetworkPolicy.End(); ++it) {
        AutoPtr<INetworkPolicy> policy = it->mSecond;
        AutoPtr<INetworkTemplate> nwkTemplate;
        policy->GetTemplate((INetworkTemplate**)&nwkTemplate);
        Boolean isMatched;
        nwkTemplate->Matches(ident, &isMatched);
        if (isMatched) {
            return policy;
        }
    }
    return NULL;
}

ECode CNetworkPolicyManagerService::GetNetworkQuotaInfo(
    /* [in] */  INetworkState* state,
    /* [out] */ INetworkQuotaInfo** info)
{
    VALIDATE_NOT_NULL(info);
    *info = NULL;

    FAIL_RETURN(mContext->EnforceCallingOrSelfPermission(
            Elastos::Droid::Manifest::permission::ACCESS_NETWORK_STATE, TAG));

    // only returns usage summary, so we don't require caller to have
    // READ_NETWORK_USAGE_HISTORY.
    const Int64 token = Binder::ClearCallingIdentity();
    //try {
    AutoPtr<INetworkQuotaInfo> _info = GetNetworkQuotaInfoUnchecked(state);
    *info = _info;
    REFCOUNT_ADD(*info);
    //} finally {
    Binder::RestoreCallingIdentity(token);
    //}
    return NOERROR;
}

AutoPtr<INetworkQuotaInfo> CNetworkPolicyManagerService::GetNetworkQuotaInfoUnchecked(
    /* [in] */  INetworkState* state)
{
    AutoPtr<INetworkIdentityHelper> helper;
    CNetworkIdentityHelper::AcquireSingleton((INetworkIdentityHelper**)&helper);
    AutoPtr<INetworkIdentity> ident;
    helper->BuildNetworkIdentity(mContext, state, (INetworkIdentity**)&ident);

    AutoPtr<INetworkPolicy> policy;
    synchronized(mRulesLock) {
        policy = FindPolicyForNetworkLocked(ident);
    }

    Boolean hasCycle;
    if (policy == NULL || (policy->HasCycle(&hasCycle), !hasCycle)) {
        // missing policy means we can't derive useful quota info
        return NULL;
    }

    Int64 currentTime = CurrentTimeMillis();

    // find total bytes used under policy
    AutoPtr<INetworkPolicyManagerHelper> npmHelper;
    CNetworkPolicyManagerHelper::AcquireSingleton((INetworkPolicyManagerHelper**)&npmHelper);
    Int64 start;
    npmHelper->ComputeLastCycleBoundary(currentTime, policy, &start);
    Int64 end = currentTime;
    AutoPtr<INetworkTemplate> nwkTemplate;
    policy->GetTemplate((INetworkTemplate**)&nwkTemplate);
    Int64 totalBytes = GetTotalBytes(nwkTemplate, start, end);

    // report soft and hard limits under policy
    Int64 waringBytes, limitBytes;
    policy->GetWarningBytes(&waringBytes);
    policy->GetLimitBytes(&limitBytes);
    Int64 softLimitBytes = waringBytes != INetworkPolicy::WARNING_DISABLED ? waringBytes
            : INetworkQuotaInfo::NO_LIMIT;
    Int64 hardLimitBytes = limitBytes != INetworkPolicy::LIMIT_DISABLED ? limitBytes
            : INetworkQuotaInfo::NO_LIMIT;

    AutoPtr<INetworkQuotaInfo> info;
    CNetworkQuotaInfo::New(totalBytes, softLimitBytes, hardLimitBytes, (INetworkQuotaInfo**)&info);
    return info;
}

ECode CNetworkPolicyManagerService::IsNetworkMetered(
    /* [in] */  INetworkState* state,
    /* [out] */ Boolean* metered)
{
    VALIDATE_NOT_NULL(metered);

    AutoPtr<INetworkIdentityHelper> helper;
    CNetworkIdentityHelper::AcquireSingleton((INetworkIdentityHelper**)&helper);
    AutoPtr<INetworkIdentity> ident;
    helper->BuildNetworkIdentity(mContext, state, (INetworkIdentity**)&ident);

    // roaming networks are always considered metered
    Boolean roaming;
    ident->GetRoaming(&roaming);
    if (roaming) {
        *metered = TRUE;
        return NOERROR;
    }

    AutoPtr<INetworkPolicy> policy;
    synchronized(mRulesLock) {
        policy = FindPolicyForNetworkLocked(ident);
    }

    if (policy != NULL) {
        return policy->GetMetered(metered);
    }
    else {
        AutoPtr<INetworkInfo> info;
        state->GetNetworkInfo((INetworkInfo**)&info);
        Int32 type;
        info->GetType(&type);
        AutoPtr<IConnectivityManagerHelper> helper;
        CConnectivityManagerHelper::AcquireSingleton((IConnectivityManagerHelper**)&helper);
        Boolean result;
        if ((helper->IsNetworkTypeMobile(type, &result), result) || type == IConnectivityManager::TYPE_WIMAX) {
            *metered = TRUE;
            return NOERROR;
        }
        *metered = FALSE;
        return NOERROR;
    }
}

void CNetworkPolicyManagerService::Dump(
    /* [in] */ IFileDescriptor* fd,
    /* [in] */ IPrintWriter* writer,
    /* [in] */ const ArrayOf<String>& args)
{
    FAIL_RETURN(mContext->EnforceCallingOrSelfPermission(Elastos::Droid::Manifest::permission::DUMP, TAG));

    AutoPtr<IIndentingPrintWriter> fout;
    CIndentingPrintWriter::New(writer, String("  "), (IIndentingPrintWriter**)&fout);
    AutoPtr<IPrintWriter> fout;
    CPrintWriter::New(writer, String("  "), (IPrintWriter**)&fout);

    AutoPtr<HashSet<String> > argSet = new HashSet<String>();

    for(Int32 i = 0; i < args.GetLength(); i++) {
        String arg = args[i];
        argSet->Insert(arg);
    }

    synchronized(mRulesLock) {
        AutoPtr<HashSet<String> >::Iterator iter1 = argSet->Find(String("--unsnooze"));
        if (iter1 != argSet->End()) {
            HashMap<AutoPtr<INetworkTemplate>, AutoPtr<INetworkPolicy> >::Iterator iter2;
            for(iter2 = mNetworkPolicy->Begin(); iter2 != mNetworkPolicy->End(); iter2++) {
                AutoPtr<INetworkPolicy> policy = iter2->mSecond;
                policy->ClearSnooze();
            }

            UpdateNetworkEnabledLocked();
            UpdateNetworkRulesLocked();
            UpdateNotificationsLocked();
            WritePolicyLocked();

            fout->PrintStringln(String("Cleared snooze timestamps"));
            return;
        }

        fout->PrintString(String("Restrict background: "));
        fout->PrintBooleanln(mRestrictBackground);
        fout->PrintStringln(String("Network policies:"));
        fout->IncreaseIndent();

        HashMap<AutoPtr<INetworkTemplate>, AutoPtr<INetworkPolicy> >::Iterator iter3;
        for(iter3 = mNetworkPolicy->Begin(); iter3 != mNetworkPolicy->End(); iter3++) {
            AutoPtr<INetworkPolicy> policy = iter3->mSecond;
            String result;
            policy->ToString(&result);
            fout->PrintStringln(result);
        }

        fout->DecreaseIndent();

        fout->PrintStringln(String("Policy for UIDs:"));
        fout->IncreaseIndent();

        HashMap<Int32, Int32>::Iterator iter4 = mUidPolicy->Begin();
        for (; iter4 != mUidPolicy->End(); iter4++) {
            Int32 uid = iter4->mFirst;
            Int32 policy = iter4->mSecond;
            fout->PrintString(String("UID="));
            fout->PrintInt32(uid);
            fout->PrintString(String(" policy="));
            CNetworkPolicyManager::DumpPolicy(fout, policy);
            fout->Println();
        }
        fout->DecreaseIndent();

        AutoPtr<HashSet<Int32> > knownUids = new HashSet<Int32>();
        CollectKeys(mUidForeground, knownUids);//HashMap<Int32, Boolean>
        CollectKeys(mUidRules, knownUids);//HashMap<Int32, Int32>

        fout->PrintStringln(String("Status for known UIDs:"));
        fout->IncreaseIndent();

        size = knownUids->Size();
        HashSet<Int32>::Iterator iter5 = knownUids->Begin();
        for (Int32 i = 0; i < size; i++)
        for(; iter5 != knownUids->End(); iter5++) {
            Int32 uid = *iter5;
            fout->PrintString(String("UID="));
            fout->PrintInt32(uid);

            fout->PrintString(String(" foreground="));
            Int32 foregroundIndex = mUidPidForeground.IndexOfKey(uid);
            HashMap<Int32, AutoPtr<HashMap<Int32, Boolean> > >::Iterator iter6 = mUidPidForeground.Find(uid);
            if (iter6 == mUidPidForeground.End())
            {
                fout->PrintString(String("UNKNOWN"));
            }
            else
            {
                DumpSparseBooleanArray(fout, iter6->mSecond);
                DumpHashMap(fout, iter6->mSecond);
            }

            fout->PrintString(String(" rules="));
            Int32 rulesIndex = mUidRules->IndexOfKey(uid);
            HashMap<Int32, Int32>::Iterator iter7 = mUidRules->Find(uid);
            if (iter7 == mUidRules->End())
            {
                fout->PrintString(String("UNKNOWN"));
            }
            else
            {
                CNetworkPolicyManager::DumpRules(fout, iter7->mSecond);//??
            }

            fout->Println();
        }
        fout->DecreaseIndent();
    }
}

ECode CNetworkPolicyManagerService::IsUidForeground(
    /* [in] */  Int32 uid,
    /* [out] */ Boolean* isUidForeground)
{
    VALIDATE_NOT_NULL(isUidForeground);

    FAIL_RETURN(mContext->EnforceCallingOrSelfPermission(
            Elastos::Droid::Manifest::permission::MANAGE_NETWORK_POLICY, TAG));

    synchronized(mRulesLock) {
        // only really in foreground when screen is also on
        //*isUidForeground = (mUidForeground->Get(uid, FALSE) && mScreenOn);
        Boolean isForeground = FALSE;
        Int32BooleanMapIterator it = mUidForeground.Find(uid);
        if (it != mUidForeground.End()) {
            isForeground = it->mSecond;
        }
        *isUidForeground = isForeground && mScreenOn;
    }
    return NOERROR;
}

/**
 * Foreground for PID changed; recompute foreground at UID level. If
 * changed, will trigger {@link #updateRulesForUidLocked(int)}.
 */
void CNetworkPolicyManagerService::ComputeUidForegroundLocked(
    /* [in] */  Int32 uid)
{
    UidPidForegroundMapIterator it = mUidPidForeground.Find(uid);
    assert(it != mUidPidForeground.End());

    AutoPtr<Int32BooleanMap> pidForeground = it->mSecond;
    // current pid is dropping foreground; examine other pids
    Boolean uidForeground = FALSE;
    Int32BooleanMapIterator it2;
    for (it2 = pidForeground->Begin(); it2 != pidForeground->End(); ++it2) {
        if (it2->mSecond == TRUE) {
            uidForeground = TRUE;
            break;
        }
    }

    Boolean oldUidForeground = FALSE;
    it2 = mUidForeground.Find(uid);
    if(it2 != mUidForeground.End()) {
        oldUidForeground = it2->mSecond;
    }
    if (oldUidForeground != uidForeground) {
        // foreground changed, push updated rules
        mUidForeground[uid] = uidForeground;
        UpdateRulesForUidLocked(uid);
    }
}

void CNetworkPolicyManagerService::UpdateScreenOn()
{
    synchronized(mRulesLock) {
        //try {
        mPowerManager->IsScreenOn(&mScreenOn);
        //} catch (RemoteException e) {
            // ignored; service lives in system_server
        //}
        UpdateRulesForScreenLocked();
        //}
    }
}

/**
 * Update rules that might be changed by {@link #mScreenOn} value.
 */
void CNetworkPolicyManagerService::UpdateRulesForScreenLocked()
{
    // only update rules for anyone with foreground activities
    Int32BooleanMapIterator it;
    for(it = mUidForeground.Begin(); it != mUidForeground.End(); ++it) {
        if (it->mSecond == TRUE) {
            Int32 uid = it->mFirst;
            UpdateRulesForUidLocked(uid);
        }
    }
}

/**
* Update rules that might be changed by {@link #mRestrictBackground} value.
*/
void CNetworkPolicyManagerService::UpdateRulesForRestrictBackgroundLocked()
{
    AutoPtr<IPackageManager> pm;
    mContext->GetPackageManager((IPackageManager**)&pm);
    AutoPtr<IInterface> obj;
    mContext->GetSystemService(IContext::USER_SERVICE, (IInterface**)&obj);
    AutoPtr<IUserManager> um = IUserManager::Probe(obj);

    // update rules for all installed applications
    AutoPtr<IObjectContainer> users;
    um->GetUsers((IObjectContainer**)&users);
    AutoPtr<IObjectContainer> apps;
    Int32 flags = IPackageManager::GET_UNINSTALLED_PACKAGES | IPackageManager::GET_DISABLED_COMPONENTS;
    pm->GetInstalledApplications(flags , (IObjectContainer**)&apps);

    AutoPtr<IUserHandleHelper> helper;
    CUserHandleHelper::AcquireSingleton((IUserHandleHelper**)&helper);

    AutoPtr<IObjectEnumerator> usersIt;
    users->GetObjectEnumerator((IObjectEnumerator**)&usersIt);
    Boolean usersNext;
    while ((usersIt->MoveNext(&usersNext), usersNext)) {
        AutoPtr<IUserInfo> user;
        usersIt->Current((IInterface**)&user);
        Int32 id;
        user->GetId(&id);
        AutoPtr<IObjectEnumerator> appsIt;
        apps->GetObjectEnumerator((IObjectEnumerator**)&appsIt);
        Boolean appNext;
        while ((appsIt->MoveNext(&appNext), appNext)) {
            AutoPtr<IApplicationInfo> app;
            appsIt->Current((IInterface**)&app);
            Int32 uid;
            app->GetUid(&uid);
            helper->GetUid(id, uid, &uid);
            UpdateRulesForUidLocked(uid);
        }
    }

    // limit data usage for some internal system services
    UpdateRulesForUidLocked(IProcess::MEDIA_UID);
    UpdateRulesForUidLocked(IProcess::DRM_UID);
}

Boolean CNetworkPolicyManagerService::IsUidValidForRules(
    /* [in] */  Int32 uid)
{
    AutoPtr<IUserHandleHelper> helper;
    CUserHandleHelper::AcquireSingleton((IUserHandleHelper**)&helper);
    // allow rules on specific system services, and any apps
    Boolean isapp;
    if (uid == IProcess::MEDIA_UID
        || uid == IProcess::DRM_UID
        || (helper->IsApp(uid, &isapp), isapp)) {
        return TRUE;
    }

    return FALSE;
}

void CNetworkPolicyManagerService::UpdateRulesForUidLocked(
    /* [in] */  Int32 uid)
{
    if (!IsUidValidForRules(uid)) return;

    Int32 uidPolicy;
    GetUidPolicy(uid, &uidPolicy);
    Boolean uidForeground;
    IsUidForeground(uid, &uidForeground);

    // derive active rules based on policy and active state
    Int32 uidRules = INetworkPolicyManager::RULE_ALLOW_ALL;
    if (!uidForeground && (uidPolicy & INetworkPolicyManager::POLICY_REJECT_METERED_BACKGROUND) != 0) {
        // uid in background, and policy says to block metered data
        uidRules = INetworkPolicyManager::RULE_REJECT_METERED;
    }
    if (!uidForeground && mRestrictBackground) {
        // uid in background, and global background disabled
        uidRules = INetworkPolicyManager::RULE_REJECT_METERED;
    }

    // TODO: only dispatch when rules actually change

    if (uidRules == INetworkPolicyManager::RULE_ALLOW_ALL) {
        mUidRules.Erase(uid);
    }
    else {
        mUidRules[uid] = uidRules;
    }

    Boolean rejectMetered = (uidRules & INetworkPolicyManager::RULE_REJECT_METERED) != 0;
    SetUidNetworkRules(uid, rejectMetered);

    // dispatch changed rule to existing listeners
    AutoPtr<IMessage> msg;
    mHandler->ObtainMessage(MSG_RULES_CHANGED, (IMessage**)&msg);
    msg->SetArg1(uid);
    msg->SetArg2(uidRules);
    Boolean result;
    mHandler->SendMessage(msg, &result);

    //try {
        // adjust stats accounting based on foreground status
    mNetworkStats->SetUidForeground(uid, uidForeground);
    //} catch (RemoteException e) {
        // ignored; service lives in system_server
    //}
}

void CNetworkPolicyManagerService::SetInterfaceQuota(
    /* [in] */ const String& iface,
    /* [in] */ Int64 quotaBytes)
{
    //try {
    mNetworkManager->SetInterfaceQuota(iface, quotaBytes);
    //} catch (IllegalStateException e) {
    //    Log.wtf(TAG, "problem setting interface quota", e);
    //} catch (RemoteException e) {
        // ignored; service lives in system_server
    //}
}

void CNetworkPolicyManagerService::RemoveInterfaceQuota(
    /* [in] */ const String& iface)
{
    //try {
    mNetworkManager->RemoveInterfaceQuota(iface);
    //} catch (IllegalStateException e) {
    //    Log.wtf(TAG, "problem removing interface quota", e);
    //} catch (RemoteException e) {
        // ignored; service lives in system_server
    //}
}

void CNetworkPolicyManagerService::SetUidNetworkRules(
    /* [in] */ Int32 uid,
    /* [in] */ Boolean rejectOnQuotaInterfaces)
{
//    try {
    mNetworkManager->SetUidNetworkRules(uid, rejectOnQuotaInterfaces);
//    } catch (IllegalStateException e) {
//        Log.wtf(TAG, "problem setting uid rules", e);
//    } catch (RemoteException e) {
        // ignored; service lives in system_server
//    }
}

/**
 * Control {@link IConnectivityManager#setPolicyDataEnable(int, boolean)}.
 */
void CNetworkPolicyManagerService::SetPolicyDataEnable(
    /* [in] */ Int32 networkType,
    /* [in] */ Boolean enabled)
{
//    try {
    mConnManager->SetPolicyDataEnable(networkType, enabled);
//    } catch (RemoteException e) {
        // ignored; service lives in system_server
//    }
}

Int64 CNetworkPolicyManagerService::GetTotalBytes(
    /* [in] */ INetworkTemplate* templ,
    /* [in] */ Int64 start,
    /* [in] */ Int64 end)
{
    //try {
    Int64 totalBytes;
    ECode ec = mNetworkStats->GetNetworkTotalBytes(templ, start, end, &totalBytes);
    if (FAILED(ec)) return 0;
    return totalBytes;
    //} catch (RuntimeException e) {
    //    Slog.w(TAG, "problem reading network stats: " + e);
    //    return 0;
    //} catch (RemoteException e) {
        // ignored; service lives in system_server
    //    return 0;
    //}
}

Boolean CNetworkPolicyManagerService::IsBandwidthControlEnabled()
{
    Int64 token = Binder::ClearCallingIdentity();
    // try {
    Boolean result;
    ECode ec = mNetworkManager->IsBandwidthControlEnabled(&result);
    if (FAILED(ec)) result = FALSE;
    // } catch (RemoteException e) {
    //     ignored; service lives in system_server
    //    return false;
    // } finally {
    Binder::RestoreCallingIdentity(token);
    // }
    return result;
}

/**
 * Try refreshing {@link #mTime} when stale.
 */
void CNetworkPolicyManagerService::MaybeRefreshTrustedTime()
{
    Int64 cacheAge;
    mTime->GetCacheAge(&cacheAge);
    if (cacheAge > TIME_CACHE_MAX_AGE) {
        Boolean result;
        mTime->ForceRefresh(&result);
    }
}

Int64 CNetworkPolicyManagerService::CurrentTimeMillis()
{
    Boolean hasCache;
    mTime->HasCache(&hasCache);
    Int64 timeMillis;
    if(hasCache) {
        mTime->GetCurrentTimeMillis(&timeMillis);
    }
    else {
        AutoPtr<ISystem> system;
        Elastos::Core::CSystem::AcquireSingleton((ISystem**)&system);
        system->GetCurrentTimeMillis(&timeMillis);
    }
    return timeMillis;
}

AutoPtr<IIntent> CNetworkPolicyManagerService::BuildAllowBackgroundDataIntent()
{
    AutoPtr<IIntent> intent;
    CIntent::New(ACTION_ALLOW_BACKGROUND, (IIntent**)&intent);
    return intent;
}

AutoPtr<IIntent> CNetworkPolicyManagerService::BuildSnoozeWarningIntent(
    /* [in] */ INetworkTemplate* templ)
{
    AutoPtr<IIntent> intent;
    CIntent::New(ACTION_SNOOZE_WARNING, (IIntent**)&intent);
    intent->PutExtra(INetworkPolicyManager::EXTRA_NETWORK_TEMPLATE, IParcelable::Probe(templ));
    return intent;
}

AutoPtr<IIntent> CNetworkPolicyManagerService::BuildNetworkOverLimitIntent(
    /* [in] */ INetworkTemplate* templ)
{
    AutoPtr<IIntent> intent;
    CIntent::New((IIntent**)&intent);
    AutoPtr<IComponentName> component;
    CComponentName::New(String("com.android.systemui"),
            String("com.android.systemui.net.NetworkOverLimitActivity"), (IComponentName**)&component);
    intent->SetComponent(component);
    intent->AddFlags(IIntent::FLAG_ACTIVITY_NEW_TASK);
    intent->PutExtra(INetworkPolicyManager::EXTRA_NETWORK_TEMPLATE, IParcelable::Probe(templ));
    return intent;
}

AutoPtr<IIntent> CNetworkPolicyManagerService::BuildViewDataUsageIntent(
    /* [in] */ INetworkTemplate* templ)
{
    AutoPtr<IIntent> intent;
    CIntent::New((IIntent**)&intent);
    AutoPtr<IComponentName> component;
    CComponentName::New(String("com.android.settings"),
            String("com.android.settings.Settings$DataUsageSummaryActivity"), (IComponentName**)&component);
    intent->SetComponent(component);
    intent->AddFlags(IIntent::FLAG_ACTIVITY_NEW_TASK);
    intent->PutExtra(INetworkPolicyManager::EXTRA_NETWORK_TEMPLATE, IParcelable::Probe(templ));
    return intent;
}

// @VisibleForTesting
ECode CNetworkPolicyManagerService::AddIdleHandler(
    /* [in] */  IIdleHandler* handler)
{
    AutoPtr<ILooper> looper;
    mHandler->GetLooper((ILooper**)&looper);
    AutoPtr<IMessageQueue> queue;
    looper->GetQueue((IMessageQueue**)&queue);
    return queue->AddIdleHandler(handler);
}

void CNetworkPolicyManagerService::CollectKeys(
    /* [in] */ HashMap<Int32, Int32>* source,
    /* [in] */ HashMap<Int32, Boolean>* target)
{
    HashMap<Int32, Int32>::Iterator srcIt;
    for(srcIt = source->Begin(); srcIt != source->End(); ++srcIt) {
        Int32 key = srcIt->mFirst;
        (*target)[key] = TRUE;
    }
}

void CNetworkPolicyManagerService::CollectKeys(
    /* [in] */ HashMap<Int32, Boolean>* source,
    /* [in] */ HashMap<Int32, Boolean>* target)
{
    Int32BooleanMapIterator srcIt;
    for(srcIt = source->Begin(); srcIt != source->End(); ++srcIt) {
        Int32 key = srcIt->mFirst;
        (*target)[key] = TRUE;
    }
}

void CNetworkPolicyManagerService::DumpHashMap(
    /* [in] */ IPrintWriter* fout,
    /* [in] */ HashMap<Int32, Boolean>* value)
{
    fout->PrintString(String("["));
    Int32 i = 0;
    Int32 count = value->GetSize();
    Int32BooleanMapIterator iter;
    for (iter = value->Begin(); iter != value->End(); ++iter, ++i) {
        fout->PrintString(StringUtils::Int32ToString(iter->mFirst) + "=" + StringUtils::BooleanToString(iter->mSecond));
        if (i < count - 1) {
            fout->PrintString(String(","));
        }
    }
    fout->PrintString(String("]"));
}

void CNetworkPolicyManagerService::HandleMsgRulesChanged(
    /* [in] */ Int32 uid,
    /* [in] */ Int32 uidRules)
{
    Int32 length;
    mListeners->BeginBroadcast(&length);
    for (Int32 i = 0; i < length; i++) {
        AutoPtr<IInterface> obj;
        mListeners->GetBroadcastItem(i, (IInterface**)&obj);
        AutoPtr<INetworkPolicyListener> listener = INetworkPolicyListener::Probe(obj);
        if (listener != NULL) {
            listener->OnUidRulesChanged(uid, uidRules);
        }
    }
    mListeners->FinishBroadcast();
}

void CNetworkPolicyManagerService::HandleMsgMeteredIfacesChanged(
    /* [in] */ IArrayOf* meteredIfaces)
{
    AutoPtr<ArrayOf<String> > array;
    if (meteredIfaces != NULL) {
        Int32 size;
        meteredIfaces->GetLength(&size);
        array = ArrayOf<String>::Alloc(size);

        for (Int32 i = 0; i < size; i++) {
            AutoPtr<ICharSequence> seq;
            meteredIfaces->Get(i, (IInterface**)&seq);
            String info;
            seq->ToString(&info);
            (*array)[i] = info;
        }
    }
    else {
        array = ArrayOf<String>::Alloc(0);
    }

    Int32 length;
    mListeners->BeginBroadcast(&length);
    for (Int32 i = 0; i < length; i++) {
        AutoPtr<IInterface> obj;
        mListeners->GetBroadcastItem(i, (IInterface**)&obj);
        AutoPtr<INetworkPolicyListener> listener = INetworkPolicyListener::Probe(obj);
        if (listener != NULL) {
            listener->OnMeteredIfacesChanged(array);
        }
    }
    mListeners->FinishBroadcast();
}

void CNetworkPolicyManagerService::HandleMsgForegroundActivitiesChanged(
    /* [in] */ Int32 pid,
    /* [in] */ Int32 uid,
    /* [in] */ Boolean foregroundActivities)
{
    synchronized(mRulesLock) {
        // because a uid can have multiple pids running inside, we need to
        // remember all pid states and summarize foreground at uid level.

        // record foreground for this specific pid
        AutoPtr<Int32BooleanMap> pidForeground;
        UidPidForegroundMapIterator it = mUidPidForeground.Find(uid);
        if (it != mUidPidForeground.End()) {
            pidForeground = it->mSecond;
        }
        if (pidForeground == NULL) {
            pidForeground = new Int32BooleanMap();
            mUidPidForeground[uid] = pidForeground;
        }
        (*pidForeground)[pid] = foregroundActivities;
        ComputeUidForegroundLocked(uid);
    }
}

void CNetworkPolicyManagerService::HandleMsgProcessDied(
    /* [in] */ Int32 pid,
    /* [in] */ Int32 uid)
{
    synchronized(mRulesLock) {
        // clear records and recompute, when they exist
        AutoPtr<Int32BooleanMap> pidForeground;
        UidPidForegroundMapIterator it = mUidPidForeground.Find(uid);
        if (it != mUidPidForeground.End()) {
            pidForeground = it->mSecond;
        }
        if (pidForeground != NULL) {
            pidForeground->Erase(pid);
            ComputeUidForegroundLocked(uid);
        }
    }
}

void CNetworkPolicyManagerService::HandleMsgLimitReached(
    /* [in] */ const String& iface)
{
    MaybeRefreshTrustedTime();
    synchronized(mRulesLock) {
        if (mMeteredIfaces.Find(iface) != mMeteredIfaces.End()) {
            // force stats update to make sure we have
            // numbers that caused alert to trigger.
            mNetworkStats->ForceUpdate();

            UpdateNetworkEnabledLocked();
            UpdateNotificationsLocked();
        }
    }
}

void CNetworkPolicyManagerService::HandleMsgRestrictBackgroundChanged(
    /* [in] */ Boolean restrictBackground)
{
    Int32 length;
    mListeners->BeginBroadcast(&length);
    for (Int32 i = 0; i < length; i++) {
        AutoPtr<INetworkPolicyListener> listener;
        mListeners->GetBroadcastItem(i, (IInterface**)&listener);
        if (listener != NULL) {
            listener->OnRestrictBackgroundChanged(restrictBackground);
        }
    }
    mListeners->FinishBroadcast();
}

void CNetworkPolicyManagerService::HandleMsgAdvisePersistThreshold(
    /* [in] */ Int64 lowestRule)
{
    // make sure stats are recorded frequently enough; we aim
    // for 2MB threshold for 2GB/month rules.
    Int64 persistThreshold = lowestRule / 1000;
    mNetworkStats->AdvisePersistThreshold(persistThreshold);
}

void CNetworkPolicyManagerService::HandleMsgScreenOnChanged()
{
    UpdateScreenOn();
}

} // namespace Net
} // namespace Server
} // namepsace Droid
} // namespace Elastos
