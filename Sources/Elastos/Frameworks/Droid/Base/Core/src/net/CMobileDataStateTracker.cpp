
#include "ext/frameworkext.h"
#include "net/CMobileDataStateTracker.h"
#include <StringBuffer.h>

namespace Elastos {
namespace Droid {
namespace Net {

/**
 * Create a new MobileDataStateTracker
 * @param netType the ConnectivityManager network type
 * @param tag the name of this network
 */
ECode CMobileDataStateTracker::constructor(
    /* [in] */ Int32 netType,
    /* [in] */ const String& tag)
{
    AutoPtr<ITelephonyManager> telManager;
    CTelephonyManager::GetDefault((ITelephonyManager**)&telManager);
    Int32 n;
    String str;
    telManager->GetNetworkType(&n);
    telManager->GetNetworkTypeName(&str);
    CNetworkInfo::New(netType, n, tag, str, (INetworkInfo**)&mNetworkInfo);
    mApnType = NetworkTypeToApnType(netType);
    return NOERROR;
}

/**
 * Begin monitoring data connectivity.
 *
 * @param context is the current Android context
 * @param target is the Hander to which to return the events.
 */
ECode CMobileDataStateTracker::StartMonitoring(
    /* [in] */ IContext* context,
    /* [in] */ Handler* target)
{
    mTarget = target;
    mContext = context;

    mHandler = new MdstHandler(target.getLooper(), this);

    AutoPtr<IIntentFilter> filter;
    CIntentFilter::New((IIntentFilter**)&filter);
    filter->AddAction(TelephonyIntents::ACTION_ANY_DATA_CONNECTION_STATE_CHANGED);
    filter->AddAction(TelephonyIntents::ACTION_DATA_CONNECTION_FAILED);
    filter->AddAction(DctConstants::ACTION_DATA_CONNECTION_TRACKER_MESSENGER);

    AutoPtr<MobileDataStateReceiver> dataReceiver = new MobileDataStateReceiver();
    AutoPtr<IIntent> intent;
    mContext->RegisterReceiver(dataReceiver, filter, (IIntent**)&intent);
    mMobileDataState = PhoneConstants::DataState.DISCONNECTED;
    return NOERROR;
}

CMobileDataStateTracker::MdstHandler::MdstHandler(
    /* [in] */ Looper* looper,
    /* [in] */ MobileDataStateTracker* mdst)
{
//    super(looper);
    mMdst = mdst;
}

//@Override
void CMobileDataStateTracker::MdstHandler::HandleMessage(
    /* [in] */ Message msg)
{
    switch (msg.what) {
        case AsyncChannel::CMD_CHANNEL_HALF_CONNECTED:
            if (msg.arg1 == AsyncChannel::STATUS_SUCCESSFUL) {
                if (VDBG) {
                    mMdst.Log("MdstHandler connected");
                }
                mMdst.mDataConnectionTrackerAc = (AsyncChannel*) msg.obj;
            } else {
                if (VDBG) {
                    mMdst.Log("MdstHandler %s NOT connected error=" + msg.arg1);
                }
            }
            break;
        case AsyncChannel::CMD_CHANNEL_DISCONNECTED:
            if (VDBG) mMdst.Log("Disconnected from DataStateTracker");
            mMdst.mDataConnectionTrackerAc = NULL;
            break;
        default: {
            if (VDBG) mMdst.Log("Ignorning unknown message=" + msg);
            break;
        }
    }
}


ECode CMobileDataStateTracker::IsPrivateDnsRouteSet(
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
    *result = mPrivateDnsRouteSet;
    return NOERROR;
}

ECode CMobileDataStateTracker::PrivateDnsRouteSet(
    /* [in] */ Boolean enabled)
{
    mPrivateDnsRouteSet = enabled;
    return NOERROR;
}

ECode CMobileDataStateTracker::GetNetworkInfo(
    /* [out] */ INetworkInfo* result)
{
    VALIDATE_NOT_NULL(result);
    *result = mNetworkInfo;
    REFCOUNT_ADD(*result);
    return NOERROR;
}

ECode CMobileDataStateTracker::IsDefaultRouteSet(
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
    *result = mDefaultRouteSet;
    return NOERROR;
}

ECode CMobileDataStateTracker::DefaultRouteSet(
    /* [in] */ Boolean enabled)
{
    mDefaultRouteSet = enabled;
    return NOERROR;
}

/**
 * This is not implemented.
 */
ECode CMobileDataStateTracker::ReleaseWakeLock()
{
    return NOERROR;
}

CMobileDataStateTracker::MobileDataStateReceiver::MobileDataStateReceiver(
    /* [in] */ CMobileDataStateTracker* host)
    : mHost(host)
{ }

void CMobileDataStateTracker::MobileDataStateReceiver::OnReceive(
    /* [in] */ IContext* context,
    /* [in] */ IIntent* intent)
{
    String act;
    intent->GetAction(&act);
    if (act.Equals(TelephonyIntents::ACTION_ANY_DATA_CONNECTION_STATE_CHANGED)) {
        String apnType;
        intent->GetStringExtra(PhoneConstants::DATA_APN_TYPE_KEY, &apnType);
        if (VDBG) {
            Log(String.format("Broadcast received: ACTION_ANY_DATA_CONNECTION_STATE_CHANGED"
                + "mApnType=%s %s received apnType=%s", mApnType,
                TextUtils::Equals(apnType, mApnType) ? "==" : "!=", apnType));
        }
        if (!TextUtils::Equals(apnType, mApnType)) {
            return;
        }

        Int32 oldSubtype;
        mNetworkInfo->GetSubtype(&oldSubtype);

        Int32 newSubType;
        AutoPtr<ITelephonyManager> telManager;
        CTelephonyManager::GetDefault((ITelephonyManager**)&telManager);
        telManager->GetNetworkType(&newSubType);

        String subTypeName;
        telManager->GetNetworkTypeName(&subTypeName);

        mNetworkInfo->SetSubtype(newSubType, subTypeName);
        Boolean bol;
        mNetworkInfo->IsConnected(&bol);
        if (newSubType != oldSubtype && bol) {
            AutoPtr<IMessage> msg;
            mTarget->ObtainMessage(EVENT_NETWORK_SUBTYPE_CHANGED,
                    oldSubtype, 0, mNetworkInfo, (IMessage**)&msg);
            msg->SendToTarget();
        }

//        PhoneConstants::DataState state = Enum.valueOf(PhoneConstants::DataState.class,
//                intent->GetStringExtra(PhoneConstants::STATE_KEY));
        String reason;
        intent->GetStringExtra(PhoneConstants::STATE_CHANGE_REASON_KEY, &reason);
        String apnName;
        intent->GetStringExtra(PhoneConstants::DATA_APN_KEY, apnName);
        intent->GetBooleanExtra(PhoneConstants::DATA_NETWORK_ROAMING_KEY, FALSE, &bol);
        mNetworkInfo->SetRoaming(bol);
//        if (VDBG) {
//            Log(mApnType + " setting isAvailable to " +
//                    intent.getBooleanExtra(PhoneConstants::NETWORK_UNAVAILABLE_KEY,FALSE));
//        }
        intent->GetBooleanExtra(PhoneConstants::NETWORK_UNAVAILABLE_KEY, FALSE, &bol);
        mNetworkInfo->SetIsAvailable(!bol);

//        if (DBG) {
//            Log("Received state=" + state + ", old=" + mMobileDataState +
//                ", reason=" + (reason == NULL ? "(unspecified)" : reason));
//        }
        if (mHost->mMobileDataState != state) {
            mHost->mMobileDataState = state;
            switch (state) {
                case DISCONNECTED:
                    Boolean bol;
                    mHost->IsTeardownRequested(&bol);
                    if(bol) {
                        mHost->SetTeardownRequested(FALSE);
                    }

                    mHost->SetDetailedState(DetailedStat::.DISCONNECTED, reason, apnName);
                    // can't do this here - ConnectivityService needs it to clear stuff
                    // it's ok though - just leave it to be refreshed next time
                    // we connect.
                    //if (DBG) Log("clearing mInterfaceName for "+ mApnType +
                    //        " as it DISCONNECTED");
                    //mInterfaceName = NULL;
                    break;
                case CONNECTING:
                    mHost->SetDetailedState(DetailedState.CONNECTING, reason, apnName);
                    break;
                case SUSPENDED:
                    mHost->SetDetailedState(DetailedState.SUSPENDED, reason, apnName);
                    break;
                case CONNECTED:
                    mHost->mLinkProperties = NULL;
                    intent->GetParcelableExtra(
                        PhoneConstants::DATA_LINK_PROPERTIES_KEY,
                        &(mHost->mLinkProperties));
                    if (mHost->mLinkProperties == NULL) {
                        Loge("CONNECTED event did not supply link properties.");
                        CLinkProperties::New((ILinkProperties**)&(mHost->mLinkProperties ));
                    }

                    mHost->mLinkCapabilities = NULL;
                    intent->GetParcelableExtra(
                            PhoneConstants::DATA_LINK_CAPABILITIES_KEY,
                            (ILinkCapabilities**)&(mHost->mLinkCapabilities));
                    if (mHost->mLinkCapabilities == NULL) {
                        Loge("CONNECTED event did not supply link capabilities.");
                        CLinkCapabilities::New((ILinkCapabilities**)&(mHost->mLinkCapabilities));
                    }
                    mHost->SetDetailedState(DetailedState.CONNECTED, reason, apnName);
                    break;
            }
        } else {
            // There was no state change. Check if LinkProperties has been updated.
            if (TextUtils::Equals(reason, PhoneConstants::REASON_LINK_PROPERTIES_CHANGED)) {
                mHost->mLinkProperties = NULL;
                intent->GetParcelableExtra(
                        PhoneConstants::DATA_LINK_PROPERTIES_KEY,
                        (ILinkCapabilities**)&(mHost->mLinkProperties));
                if (mHost->mLinkProperties == NULL) {
                    Loge("No link property in LINK_PROPERTIES change event.");
                    CLinkProperties::New((ILinkProperties**)&(mHost->mLinkProperties));
                }
                // Just update reason field in this NetworkInfo
                mHost->mNetworkInfo->SetDetailedState(mHost->mNetworkInfo->GetDetailedState(), reason,
                                              mHost->mNetworkInfo->GetExtraInfo());
                Message msg = mTarget.obtainMessage(EVENT_CONFIGURATION_CHANGED,
                                                    mNetworkInfo);
                msg.sendToTarget();
            }
        }
    } else if (intent.getAction().
            Equals(TelephonyIntents::ACTION_DATA_CONNECTION_FAILED)) {
        String apnType = intent->GetStringExtra(PhoneConstants::DATA_APN_TYPE_KEY);
        if (!TextUtils::Equals(apnType, mApnType)) {
            if (DBG) {
                Log(String.format(
                        "Broadcast received: ACTION_ANY_DATA_CONNECTION_FAILED ignore, " +
                        "mApnType=%s != received apnType=%s", mApnType, apnType));
            }
            return;
        }
        String reason;
        intent->GetStringExtra(PhoneConstants::FAILURE_REASON_KEY, &reason);
        String apnName;
        intent->GetStringExtra(PhoneConstants::DATA_APN_KEY, &apnName);
        if (DBG) {
            Log("Received " + intent.getAction() +
                        " broadcast" + reason == NULL ? "" : "(" + reason + ")");
        }
        mHost->SetDetailedState(DetailedState.FAILED, reason, apnName);
    } else if (intent.getAction().equals(DctConstants
            .ACTION_DATA_CONNECTION_TRACKER_MESSENGER)) {
        if (VDBG) Log(mApnType + " got ACTION_DATA_CONNECTION_TRACKER_MESSENGER");
        mMessenger =
            intent.getParcelableExtra(DctConstants.EXTRA_MESSENGER);
        AutoPtr<AsyncChannel> ac = new AsyncChannel();
        ac->Connect(mContext, MobileDataStateTracker.this.mHandler, mMessenger);
    } else {
//        if (DBG) Log("Broadcast received: ignore " + intent->GetAction());
    }
}


void CMobileDataStateTracker::GetPhoneService(
    /* [in] */ Boolean forceRefresh)
{
    if ((mPhoneService == NULL) || forceRefresh) {
        mPhoneService = ITelephony.Stub.asInterface(ServiceManager.getService("phone"));
    }
}

/**
 * Report whether data connectivity is possible.
 */
ECode CMobileDataStateTracker::IsAvailable(
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
    return mNetworkInfo->IsAvailable(result);
}

/**
 * Return the system properties name associated with the tcp buffer sizes
 * for this network.
 */
ECode CMobileDataStateTracker::GetTcpBufferSizesPropName(
    /* [out] */ String* result)
{
    VALIDATE_NOT_NULL(result);

    String networkTypeStr = "unknown";
    AutoPtr<ITelephonyManager> tm;
    CTelephonyManager::New(mContext, (ITelephonyManager**)&tm);
    //TODO We have to edit the parameter for getNetworkType regarding CDMA
    Int32 nType;
    tm->GetNetworkType(&nType);
    switch(nType) {
    case ITelephonyManager::NETWORK_TYPE_GPRS:
        networkTypeStr = "gprs";
        break;
    case ITelephonyManager::NETWORK_TYPE_EDGE:
        networkTypeStr = "edge";
        break;
    case ITelephonyManager::NETWORK_TYPE_UMTS:
        networkTypeStr = "umts";
        break;
    case ITelephonyManager::NETWORK_TYPE_HSDPA:
        networkTypeStr = "hsdpa";
        break;
    case ITelephonyManager::NETWORK_TYPE_HSUPA:
        networkTypeStr = "hsupa";
        break;
    case ITelephonyManager::NETWORK_TYPE_HSPA:
        networkTypeStr = "hspa";
        break;
    case ITelephonyManager::NETWORK_TYPE_HSPAP:
        networkTypeStr = "hspap";
        break;
    case ITelephonyManager::NETWORK_TYPE_CDMA:
        networkTypeStr = "cdma";
        break;
    case ITelephonyManager::NETWORK_TYPE_1xRTT:
        networkTypeStr = "1xrtt";
        break;
    case ITelephonyManager::NETWORK_TYPE_EVDO_0:
        networkTypeStr = "evdo";
        break;
    case ITelephonyManager::NETWORK_TYPE_EVDO_A:
        networkTypeStr = "evdo";
        break;
    case ITelephonyManager::NETWORK_TYPE_EVDO_B:
        networkTypeStr = "evdo";
        break;
    case ITelephonyManager::NETWORK_TYPE_IDEN:
        networkTypeStr = "iden";
        break;
    case ITelephonyManager::NETWORK_TYPE_LTE:
        networkTypeStr = "lte";
        break;
    case ITelephonyManager::NETWORK_TYPE_EHRPD:
        networkTypeStr = "ehrpd";
        break;
    default:
        Loge("unknown network type: " + tm.getNetworkType());
    }
    *result = "net.tcp.buffersize." + networkTypeStr;
    return NOERROR;
}

/**
 * Tear down mobile data connectivity, i.e., disable the ability to create
 * mobile data connections.
 * TODO - make async and return nothing?
 */
ECode CMobileDataStateTracker::Teardown(
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);

    SetTeardownRequested(TRUE);
    *result = (SetEnableApn(mApnType, FALSE) != PhoneConstants::APN_REQUEST_FAILED);
    return NOERROR;
}

//@Override
ECode CMobileDataStateTracker::CaptivePortalCheckComplete()
{
    // not implemented
    return NOERROR;
}

/**
 * Record the detailed state of a network, and if it is a
 * change from the previous state, send a notification to
 * any listeners.
 * @param state the new {@code DetailedState}
 * @param reason a {@code String} indicating a reason for the state change,
 * if one was supplied. May be {@code NULL}.
 * @param extraInfo optional {@code String} providing extra information about the state change
 */
void CMobileDataStateTracker::SetDetailedState(
    /* [in] */ INetworkInfo.DetailedState state,
    /* [in] */ const String& reason,
    /* [in] */ const String& extraInfo)
{
    if (DBG) Log("setDetailed state, old ="
            + mNetworkInfo->GetDetailedState() + " and new state=" + state);
    if (state != mNetworkInfo->GetDetailedState()) {
        Boolean wasConnecting = (mNetworkInfo->GetState() == NetworkInfo.State.CONNECTING);
        String lastReason;
        mNetworkInfo->GetReason(&lastReason);
        /*
         * If a reason was supplied when the CONNECTING state was entered, and no
         * reason was supplied for entering the CONNECTED state, then retain the
         * reason that was supplied when going to CONNECTING.
         */
        if (wasConnecting && state == NetworkInfo.DetailedState.CONNECTED && reason == NULL
                && lastReason != NULL)
            reason = lastReason;
        mNetworkInfo->SetDetailedState(state, reason, extraInfo);
        Message msg = mTarget.obtainMessage(EVENT_STATE_CHANGED, new NetworkInfo(mNetworkInfo));
        msg.sendToTarget();
    }
}

ECode CMobileDataStateTracker::SetTeardownRequested(
    /* [in] */ Boolean isRequested)
{
    mTeardownRequested = isRequested;
    return NOERROR;
}

ECode CMobileDataStateTracker::IsTeardownRequested(
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
    *result = mTeardownRequested;
    return NOERROR;
}

/**
 * Re-enable mobile data connectivity after a {@link #teardown()}.
 * TODO - make async and always get a notification?
 */
ECode CMobileDataStateTracker::Reconnect(
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);

    Boolean retValue = FALSE; //connected or expect to be?
    SetTeardownRequested(FALSE);
    switch (SetEnableApn(mApnType, TRUE)) {
        case PhoneConstants::APN_ALREADY_ACTIVE:
            // need to set self to CONNECTING so the below message is handled.
            retValue = TRUE;
            break;
        case PhoneConstants::APN_REQUEST_STARTED:
            // set IDLE here , avoid the following second FAILED not sent out
            mNetworkInfo.SetDetailedState(DetailedState.IDLE, NULL, NULL);
            retValue = TRUE;
            break;
        case PhoneConstants::APN_REQUEST_FAILED:
        case PhoneConstants::APN_TYPE_NOT_AVAILABLE:
            break;
        default:
            Loge("Error in reconnect - unexpected response.");
            break;
    }
    *result = retValue;
    return NOERROR;
}

/**
 * Turn on or off the mobile radio. No connectivity will be possible while the
 * radio is off. The operation is a no-op if the radio is already in the desired state.
 * @param turnOn {@code TRUE} if the radio should be turned on, {@code FALSE} if
 */
ECode CMobileDataStateTracker::SetRadio(
    /* [in] */ Boolean turnOn,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);

    GetPhoneService(FALSE);
    /*
     * If the phone process has crashed in the past, we'll get a
     * RemoteException and need to re-reference the service.
     */
    for (Int32 retry = 0; retry < 2; retry++) {
        if (mPhoneService == NULL) {
            Loge("Ignoring mobile radio request because could not acquire PhoneService");
            break;
        }

//        try {
            return mPhoneService->SetRadio(turnOn);
//        } catch (RemoteException e) {
//            if (retry == 0) GetPhoneService(TRUE);
//        }
    }

    Loge("Could not set radio power to " + (turnOn ? "on" : "off"));
    *result = FALSE;
    return NOERROR;
}

//@Override
ECode CMobileDataStateTracker::SetUserDataEnable(
    /* [in] */ Boolean enabled)
{
    if (DBG) Log("setUserDataEnable: E enabled=" + enabled);
    AsyncChannel channel = mDataConnectionTrackerAc;
    if (channel != NULL) {
        channel.sendMessage(DctConstants.CMD_SET_USER_DATA_ENABLE,
                enabled ? DctConstants.ENABLED : DctConstants.DISABLED);
        mUserDataEnabled = enabled;
    }
    if (VDBG) Log("setUserDataEnable: X enabled=" + enabled);
    return NOERROR;
}

//@Override
ECode CMobileDataStateTracker::SetPolicyDataEnable(
    /* [in] */ Boolean enabled)
{
    if (DBG) Log("setPolicyDataEnable(enabled=" + enabled + ")");
    final AsyncChannel channel = mDataConnectionTrackerAc;
    if (channel != NULL) {
        channel.sendMessage(DctConstants.CMD_SET_POLICY_DATA_ENABLE,
                enabled ? DctConstants.ENABLED : DctConstants.DISABLED);
        mPolicyDataEnabled = enabled;
    }
    return NOERROR;
}

/**
 * carrier dependency is met/unmet
 * @param met
 */
ECode CMobileDataStateTracker::SetDependencyMet(
    /* [in] */ Boolean met)
{
    Bundle bundle = Bundle.forPair(DctConstants.APN_TYPE_KEY, mApnType);
    if (DBG) Log("setDependencyMet: E met=" + met);
    Message msg = Message.obtain();
    msg.what = DctConstants.CMD_SET_DEPENDENCY_MET;
    msg.arg1 = (met ? DctConstants.ENABLED : DctConstants.DISABLED);
    msg.setData(bundle);
    mDataConnectionTrackerAc.sendMessage(msg);
    if (VDBG) Log("setDependencyMet: X met=" + met);
    return NOERROR;
}

//@Override
ECode CMobileDataStateTracker::ToString(
    /* [out] */ String* result)
{
    VALIDATE_NOT_NULL(result);

    CharArrayWriter writer = new CharArrayWriter();
    AutoPtr<IPrintWriter> pw;
    CPrintWriter::New(writer, (IPrintWriter**)&pw);
    pw->Print("Mobile data state: "); pw->Println(mMobileDataState);
    pw->Print("Data enabled: user="); pw->Print(mUserDataEnabled);
    pw->Print(", policy="); pw->Println(mPolicyDataEnabled);
    *result = writer.toString();
    return NOERROR;
}

/**
 * Internal method supporting the ENABLE_MMS feature.
 * @param apnType the type of APN to be enabled or disabled (e.g., mms)
 * @param enable {@code TRUE} to enable the specified APN type,
 * {@code FALSE} to disable it.
 * @return an integer value representing the outcome of the request.
 */
Int32 CMobileDataStateTracker::SetEnableApn(
    /* [in] */ const String& apnType,
    /* [in] */ Boolean enable)
{
    GetPhoneService(FALSE);
    /*
     * If the phone process has crashed in the past, we'll get a
     * RemoteException and need to re-reference the service.
     */
    for (Int32 retry = 0; retry < 2; retry++) {
        if (mPhoneService == NULL) {
            Loge("Ignoring feature request because could not acquire PhoneService");
            break;
        }

//        try {
            if (enable) {
                return mPhoneService->enableApnType(apnType);
            } else {
                return mPhoneService->disableApnType(apnType);
            }
//        } catch (RemoteException e) {
//            if (retry == 0) GetPhoneService(TRUE);
//        }
    }

    Loge("Could not " + (enable ? "enable" : "disable") + " APN type \"" + apnType + "\"");
    return PhoneConstants::APN_REQUEST_FAILED;
}

static String CMobileDataStateTracker::NetworkTypeToApnType(
    /* [in] */ Int32 netType)
{
    switch(netType) {
        case ConnectivityManager::TYPE_MOBILE:
            return PhoneConstants::APN_TYPE_DEFAULT;  // TODO - use just one of these
        case ConnectivityManager::TYPE_MOBILE_MMS:
            return PhoneConstants::APN_TYPE_MMS;
        case ConnectivityManager::TYPE_MOBILE_SUPL:
            return PhoneConstants::APN_TYPE_SUPL;
        case ConnectivityManager::TYPE_MOBILE_DUN:
            return PhoneConstants::APN_TYPE_DUN;
        case ConnectivityManager::TYPE_MOBILE_HIPRI:
            return PhoneConstants::APN_TYPE_HIPRI;
        case ConnectivityManager::TYPE_MOBILE_FOTA:
            return PhoneConstants::APN_TYPE_FOTA;
        case ConnectivityManager::TYPE_MOBILE_IMS:
            return PhoneConstants::APN_TYPE_IMS;
        case ConnectivityManager::TYPE_MOBILE_CBS:
            return PhoneConstants::APN_TYPE_CBS;
        default:
            sLoge("Error mapping networkType " + netType + " to apnType.");
            return NULL;
    }
}

/**
 * @see android.net.NetworkStateTracker#getLinkProperties()
 */
ECode CMobileDataStateTracker::GetLinkProperties(
    /* [out] */ ILinkProperties* result)
{
    VALIDATE_NOT_NULL(result);

    AutoPtr<ILinkProperties> linkProp;
    CLinkProperties::New(mLinkProperties, (ILinkProperties**)&linkProp);
    *result = linkProp;
    REFCOUNT_ADD(*result);
    return NOERROR;
}

/**
 * @see android.net.NetworkStateTracker#getLinkCapabilities()
 */
ECode CMobileDataStateTracker::GetLinkCapabilities(
    /* [out] */ ILinkCapabilities* result)
{
    VALIDATE_NOT_NULL(result);

    AutoPtr<ILinkCapabilities> linkCap;
    CLinkCapabilities::New(mLinkCapabilities, (ILinkCapabilities**)&linkCap);
    *result = linkCap;
    REFCOUNT_ADD(*result);
    return NOERROR;
}

void CMobileDataStateTracker::Log(
    /* [in] */ const String& s)
{
//    SLog.d(TAG, mApnType + ": " + s);
}

void CMobileDataStateTracker::Loge(
    /* [in] */ const String& s)
{
//    SLog.e(TAG, mApnType + ": " + s);
}

static void CMobileDataStateTracker::SLoge(
    /* [in] */ const String& s)
{
//    SLog.e(TAG, s);
}

} // namespace Net
} // namepsace Droid
} // namespace Elastos
