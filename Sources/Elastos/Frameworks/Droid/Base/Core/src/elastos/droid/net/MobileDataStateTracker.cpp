
#include "elastos/droid/net/MobileDataStateTracker.h"

using Elastos::Droid::Telephony::ITelephonyManager;

namespace Elastos {
namespace Droid {
namespace Net {

CAR_INTERFACE_IMPL(MobileDataStateTracker, BaseNetworkStateTracker, IMobileDataStateTracker)

AutoPtr<ArrayOf<MobileDataStateTracker::NetworkDataEntry> > MobileDataStateTracker::CreateTheoreticalBWTable()
{
    AutoPtr<ArrayOf<NetworkDataEntry> > rev;
    NetworkDataEntry array[] = {
#if 0 // TODO: Waiting for ITelephonyManager
        NetworkDataEntry(ITelephonyManager::NETWORK_TYPE_EDGE,      237,     118, UNKNOWN),
        NetworkDataEntry(ITelephonyManager::NETWORK_TYPE_GPRS,       48,      40, UNKNOWN),
        NetworkDataEntry(ITelephonyManager::NETWORK_TYPE_UMTS,      384,      64, UNKNOWN),
        NetworkDataEntry(ITelephonyManager::NETWORK_TYPE_HSDPA,   14400, UNKNOWN, UNKNOWN),
        NetworkDataEntry(ITelephonyManager::NETWORK_TYPE_HSUPA,   14400,    5760, UNKNOWN),
        NetworkDataEntry(ITelephonyManager::NETWORK_TYPE_HSPA,    14400,    5760, UNKNOWN),
        NetworkDataEntry(ITelephonyManager::NETWORK_TYPE_HSPAP,   21000,    5760, UNKNOWN),
        NetworkDataEntry(ITelephonyManager::NETWORK_TYPE_CDMA,  UNKNOWN, UNKNOWN, UNKNOWN),
        NetworkDataEntry(ITelephonyManager::NETWORK_TYPE_1xRTT, UNKNOWN, UNKNOWN, UNKNOWN),
        NetworkDataEntry(ITelephonyManager::NETWORK_TYPE_EVDO_0,   2468,     153, UNKNOWN),
        NetworkDataEntry(ITelephonyManager::NETWORK_TYPE_EVDO_A,   3072,    1800, UNKNOWN),
        NetworkDataEntry(ITelephonyManager::NETWORK_TYPE_EVDO_B,  14700,    1800, UNKNOWN),
        NetworkDataEntry(ITelephonyManager::NETWORK_TYPE_IDEN,  UNKNOWN, UNKNOWN, UNKNOWN),
        NetworkDataEntry(ITelephonyManager::NETWORK_TYPE_LTE,    100000,   50000, UNKNOWN),
        NetworkDataEntry(ITelephonyManager::NETWORK_TYPE_EHRPD, UNKNOWN, UNKNOWN, UNKNOWN),
#endif
    };
    rev = ArrayOf<NetworkDataEntry>::Alloc(array, sizeof(array)/sizeof(NetworkDataEntry));
    return rev;
}
AutoPtr<ArrayOf<MobileDataStateTracker::NetworkDataEntry> > MobileDataStateTracker::mTheoreticalBWTable = MobileDataStateTracker::CreateTheoreticalBWTable();

const String MobileDataStateTracker::TAG = String("MobileDataStateTracker");
const Boolean MobileDataStateTracker::DBG = false;
const Boolean MobileDataStateTracker::VDBG = false;
const Int32 MobileDataStateTracker::UNKNOWN = ILinkQualityInfo::UNKNOWN_INT;

MobileDataStateTracker::MobileDataStateTracker()
    : mUserDataEnabled(true)
    , mPolicyDataEnabled(true)
    , mTeardownRequested(false)
    , mPrivateDnsRouteSet(false)
    , mDefaultRouteSet(false)
{
    // There are still some vars need to be initialized. Like "private final PhoneStateListener mPhoneStateListener = new PhoneStateListener() {"
}

ECode MobileDataStateTracker::constructor(
    /* [in] */ Int32 netType,
    /* [in] */ const String& tag)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translated before. Need check.
    AutoPtr<ITelephonyManager> telManager;
    CTelephonyManager::GetDefault((ITelephonyManager**)&telManager);
    Int32 n;
    String str;
    telManager->GetNetworkType(&n);
    telManager->GetNetworkTypeName(&str);
    CNetworkInfo::New(netType, n, tag, str, (INetworkInfo**)&mNetworkInfo);
    mApnType = NetworkTypeToApnType(netType);
    return NOERROR;
#endif
}

ECode MobileDataStateTracker::StartMonitoring(
    /* [in] */ IContext* context,
    /* [in] */ IHandler* target)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
        mTarget = target;
        mContext = context;
        mHandler = new MdstHandler(target.getLooper(), this);
        IntentFilter filter = new IntentFilter();
        filter.addAction(TelephonyIntents.ACTION_ANY_DATA_CONNECTION_STATE_CHANGED);
        filter.addAction(TelephonyIntents.ACTION_DATA_CONNECTION_CONNECTED_TO_PROVISIONING_APN);
        filter.addAction(TelephonyIntents.ACTION_DATA_CONNECTION_FAILED);
        mContext.registerReceiver(new MobileDataStateReceiver(), filter);
        mMobileDataState = PhoneConstants.DataState.DISCONNECTED;
        TelephonyManager tm = (TelephonyManager)mContext.getSystemService(
                Context.TELEPHONY_SERVICE);
        tm.listen(mPhoneStateListener, PhoneStateListener.LISTEN_SIGNAL_STRENGTHS);

#endif
}

ECode MobileDataStateTracker::IsPrivateDnsRouteSet(
    /* [out] */ Boolean* result)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translated before. Need check.
    VALIDATE_NOT_NULL(result);
    *result = mPrivateDnsRouteSet;
    return NOERROR;
#endif
}

ECode MobileDataStateTracker::PrivateDnsRouteSet(
    /* [in] */ Boolean enabled)
{
    mPrivateDnsRouteSet = enabled;
    return NOERROR;
}

ECode MobileDataStateTracker::GetNetworkInfo(
    /* [out] */ INetworkInfo** result)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translated before. Need check.
    VALIDATE_NOT_NULL(result);
    *result = mNetworkInfo;
    REFCOUNT_ADD(*result);
    return NOERROR;
#endif
}

ECode MobileDataStateTracker::IsDefaultRouteSet(
    /* [out] */ Boolean* result)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translated before. Need check.
    VALIDATE_NOT_NULL(result);
    *result = mDefaultRouteSet;
    return NOERROR;
#endif
}

ECode MobileDataStateTracker::DefaultRouteSet(
    /* [in] */ Boolean enabled)
{
    mDefaultRouteSet = enabled;
    return NOERROR;
}

ECode MobileDataStateTracker::ReleaseWakeLock()
{
    return NOERROR;
}

ECode MobileDataStateTracker::UpdateLinkProperitesAndCapatilities(
    /* [in] */ IIntent* intent)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
        mLinkProperties = intent.getParcelableExtra(
                PhoneConstants.DATA_LINK_PROPERTIES_KEY);
        if (mLinkProperties == null) {
            loge("CONNECTED event did not supply link properties.");
            mLinkProperties = new LinkProperties();
        }
        mLinkProperties.setMtu(mContext.getResources().getInteger(
                com.android.internal.R.integer.config_mobile_mtu));
        mNetworkCapabilities = intent.getParcelableExtra(
                PhoneConstants.DATA_NETWORK_CAPABILITIES_KEY);
        if (mNetworkCapabilities == null) {
            loge("CONNECTED event did not supply network capabilities.");
            mNetworkCapabilities = new NetworkCapabilities();
        }

#endif
}

ECode MobileDataStateTracker::GetPhoneService(
    /* [in] */ Boolean forceRefresh)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translated before. Need check.
    if ((mPhoneService == NULL) || forceRefresh) {
        mPhoneService = ITelephony.Stub.asInterface(ServiceManager.getService("phone"));
    }
#endif
}

ECode MobileDataStateTracker::IsAvailable(
    /* [out] */ Boolean* result)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translated before. Need check.
    VALIDATE_NOT_NULL(result);
    return mNetworkInfo->IsAvailable(result);
#endif
}

ECode MobileDataStateTracker::GetTcpBufferSizesPropName(
    /* [out] */ String* result)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translated before. Need check.
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
#endif
}

ECode MobileDataStateTracker::Teardown(
    /* [out] */ Boolean* result)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translated before. Need check.
    VALIDATE_NOT_NULL(result);

    SetTeardownRequested(TRUE);
    *result = (SetEnableApn(mApnType, FALSE) != PhoneConstants::APN_REQUEST_FAILED);
    return NOERROR;
#endif
}

ECode MobileDataStateTracker::IsReady(
    /* [out] */ Boolean* result)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
        return mDataConnectionTrackerAc != null;

#endif
}

ECode MobileDataStateTracker::CaptivePortalCheckCompleted(
    /* [in] */ Boolean isCaptivePortal)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
        if (mIsCaptivePortal.getAndSet(isCaptivePortal) != isCaptivePortal) {
            // Captive portal change enable/disable failing fast
            setEnableFailFastMobileData(
                    isCaptivePortal ? DctConstants.ENABLED : DctConstants.DISABLED);
        }

#endif
}

ECode MobileDataStateTracker::SetDetailedState(
    /* [in] */ NetworkInfoDetailedState state,
    /* [in] */ const String& reason,
    /* [in] */ const String& extraInfo)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
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
#endif
}

ECode MobileDataStateTracker::SetTeardownRequested(
    /* [in] */ Boolean isRequested)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translated before. Need check.
    mTeardownRequested = isRequested;
    return NOERROR;
#endif
}

ECode MobileDataStateTracker::IsTeardownRequested(
    /* [out] */ Boolean* result)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translated before. Need check.
    VALIDATE_NOT_NULL(result);
    *result = mTeardownRequested;
    return NOERROR;
#endif
}

ECode MobileDataStateTracker::Reconnect(
    /* [out] */ Boolean* result)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translated before. Need check.
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
#endif
}

ECode MobileDataStateTracker::SetRadio(
    /* [in] */ Boolean turnOn,
    /* [out] */ Boolean* result)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translated before. Need check.
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
#endif
}

ECode MobileDataStateTracker::SetInternalDataEnable(
    /* [in] */ Boolean enabled)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
        if (DBG) log("setInternalDataEnable: E enabled=" + enabled);
        final AsyncChannel channel = mDataConnectionTrackerAc;
        if (channel != null) {
            channel.sendMessage(DctConstants.EVENT_SET_INTERNAL_DATA_ENABLE,
                    enabled ? DctConstants.ENABLED : DctConstants.DISABLED);
        }
        if (VDBG) log("setInternalDataEnable: X enabled=" + enabled);

#endif
}

ECode MobileDataStateTracker::SetUserDataEnable(
    /* [in] */ Boolean enabled)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translated before. Need check.
    if (DBG) Log("setUserDataEnable: E enabled=" + enabled);
    AsyncChannel channel = mDataConnectionTrackerAc;
    if (channel != NULL) {
        channel.sendMessage(DctConstants.CMD_SET_USER_DATA_ENABLE,
                enabled ? DctConstants.ENABLED : DctConstants.DISABLED);
        mUserDataEnabled = enabled;
    }
    if (VDBG) Log("setUserDataEnable: X enabled=" + enabled);
    return NOERROR;
#endif
}

ECode MobileDataStateTracker::SetPolicyDataEnable(
    /* [in] */ Boolean enabled)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translated before. Need check.
    if (DBG) Log("setPolicyDataEnable(enabled=" + enabled + ")");
    final AsyncChannel channel = mDataConnectionTrackerAc;
    if (channel != NULL) {
        channel.sendMessage(DctConstants.CMD_SET_POLICY_DATA_ENABLE,
                enabled ? DctConstants.ENABLED : DctConstants.DISABLED);
        mPolicyDataEnabled = enabled;
    }
    return NOERROR;
#endif
}

ECode MobileDataStateTracker::SetEnableFailFastMobileData(
    /* [in] */ Int32 enabled)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
        if (DBG) log("setEnableFailFastMobileData(enabled=" + enabled + ")");
        final AsyncChannel channel = mDataConnectionTrackerAc;
        if (channel != null) {
            channel.sendMessage(DctConstants.CMD_SET_ENABLE_FAIL_FAST_MOBILE_DATA, enabled);
        }

#endif
}

ECode MobileDataStateTracker::SetDependencyMet(
    /* [in] */ Boolean met)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translated before. Need check.
    Bundle bundle = Bundle.forPair(DctConstants.APN_TYPE_KEY, mApnType);
    if (DBG) Log("setDependencyMet: E met=" + met);
    Message msg = Message.obtain();
    msg.what = DctConstants.CMD_SET_DEPENDENCY_MET;
    msg.arg1 = (met ? DctConstants.ENABLED : DctConstants.DISABLED);
    msg.setData(bundle);
    mDataConnectionTrackerAc.sendMessage(msg);
    if (VDBG) Log("setDependencyMet: X met=" + met);
    return NOERROR;
#endif
}

ECode MobileDataStateTracker::EnableMobileProvisioning(
    /* [in] */ const String& url)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
        if (DBG) log("enableMobileProvisioning(url=" + url + ")");
        final AsyncChannel channel = mDataConnectionTrackerAc;
        if (channel != null) {
            Message msg = Message.obtain();
            msg.what = DctConstants.CMD_ENABLE_MOBILE_PROVISIONING;
            msg.setData(Bundle.forPair(DctConstants.PROVISIONING_URL_KEY, url));
            channel.sendMessage(msg);
        }

#endif
}

ECode MobileDataStateTracker::IsProvisioningNetwork(
    /* [out] */ Boolean* result)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
        boolean retVal;
        try {
            Message msg = Message.obtain();
            msg.what = DctConstants.CMD_IS_PROVISIONING_APN;
            msg.setData(Bundle.forPair(DctConstants.APN_TYPE_KEY, mApnType));
            Message result = mDataConnectionTrackerAc.sendMessageSynchronously(msg);
            retVal = result.arg1 == DctConstants.ENABLED;
        } catch (NullPointerException e) {
            loge("isProvisioningNetwork: X " + e);
            retVal = false;
        }
        if (DBG) log("isProvisioningNetwork: retVal=" + retVal);
        return retVal;

#endif
}

ECode MobileDataStateTracker::AddStackedLink(
    /* [in] */ ILinkProperties* link)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
        mLinkProperties.addStackedLink(link);

#endif
}

ECode MobileDataStateTracker::RemoveStackedLink(
    /* [in] */ ILinkProperties* link)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
        mLinkProperties.removeStackedLink(link);

#endif
}

ECode MobileDataStateTracker::ToString(
    /* [out] */ String* result)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translated before. Need check.
    VALIDATE_NOT_NULL(result);

    CharArrayWriter writer = new CharArrayWriter();
    AutoPtr<IPrintWriter> pw;
    CPrintWriter::New(writer, (IPrintWriter**)&pw);
    pw->Print("Mobile data state: "); pw->Println(mMobileDataState);
    pw->Print("Data enabled: user="); pw->Print(mUserDataEnabled);
    pw->Print(", policy="); pw->Println(mPolicyDataEnabled);
    *result = writer.toString();
    return NOERROR;
#endif
}

ECode MobileDataStateTracker::SetEnableApn(
    /* [in] */ const String& apnType,
    /* [in] */ Boolean enable,
    /* [out] */ Int32* result)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translated before. Need check.
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
//            if (enable) {
//                return mPhoneService->enableApnType(apnType);
//            } else {
//                return mPhoneService->disableApnType(apnType);
//            }
//        } catch (RemoteException e) {
//            if (retry == 0) GetPhoneService(TRUE);
//        }
    }

    Loge("Could not " + (enable ? "enable" : "disable") + " APN type \"" + apnType + "\"");
    return PhoneConstants::APN_REQUEST_FAILED;
#endif
}

ECode MobileDataStateTracker::NetworkTypeToApnType(
    /* [in] */ Int32 netType,
    /* [out] */ String* result)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translated before. Need check.
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
        case ConnectivityManager::TYPE_MOBILE_IA:
            return PhoneConstants::APN_TYPE_IA;
        case ConnectivityManager::TYPE_MOBILE_EMERGENCY:
            return PhoneConstants::APN_TYPE_EMERGENCY;
        default:
            sLoge("Error mapping networkType " + netType + " to apnType.");
            return NULL;
    }
#endif
}

ECode MobileDataStateTracker::GetLinkProperties(
    /* [out] */ ILinkProperties** result)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translated before. Need check.
    VALIDATE_NOT_NULL(result);

    AutoPtr<ILinkProperties> linkProp;
    CLinkProperties::New(mLinkProperties, (ILinkProperties**)&linkProp);
    *result = linkProp;
    REFCOUNT_ADD(*result);
    return NOERROR;
#endif
}

ECode MobileDataStateTracker::SupplyMessenger(
    /* [in] */ IMessenger* messenger)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
        if (VDBG) log(mApnType + " got supplyMessenger");
        AsyncChannel ac = new AsyncChannel();
        ac.connect(mContext, MobileDataStateTracker.this.mHandler, messenger);

#endif
}

ECode MobileDataStateTracker::Log(
    /* [in] */ const String& s)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
        Slog.d(TAG, mApnType + ": " + s);

#endif
}

ECode MobileDataStateTracker::Loge(
    /* [in] */ const String& s)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
        Slog.e(TAG, mApnType + ": " + s);

#endif
}

ECode MobileDataStateTracker::Sloge(
    /* [in] */ const String& s)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
        Slog.e(TAG, s);

#endif
}

ECode MobileDataStateTracker::GetLinkQualityInfo(
    /* [out] */ ILinkQualityInfo** result)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
        if (mNetworkInfo == null || mNetworkInfo.getType() == ConnectivityManager.TYPE_NONE) {
            // no data available yet; just return
            return null;
        }
        MobileLinkQualityInfo li = new MobileLinkQualityInfo();
        li.setNetworkType(mNetworkInfo.getType());
        mSamplingDataTracker.setCommonLinkQualityInfoFields(li);
        if (mNetworkInfo.getSubtype() != TelephonyManager.NETWORK_TYPE_UNKNOWN) {
            li.setMobileNetworkType(mNetworkInfo.getSubtype());
            NetworkDataEntry entry = getNetworkDataEntry(mNetworkInfo.getSubtype());
            if (entry != null) {
                li.setTheoreticalRxBandwidth(entry.downloadBandwidth);
                li.setTheoreticalRxBandwidth(entry.uploadBandwidth);
                li.setTheoreticalLatency(entry.latency);
            }
            if (mSignalStrength != null) {
                li.setNormalizedSignalStrength(getNormalizedSignalStrength(
                        li.getMobileNetworkType(), mSignalStrength));
            }
        }
        SignalStrength ss = mSignalStrength;
        if (ss != null) {
            li.setRssi(ss.getGsmSignalStrength());
            li.setGsmErrorRate(ss.getGsmBitErrorRate());
            li.setCdmaDbm(ss.getCdmaDbm());
            li.setCdmaEcio(ss.getCdmaEcio());
            li.setEvdoDbm(ss.getEvdoDbm());
            li.setEvdoEcio(ss.getEvdoEcio());
            li.setEvdoSnr(ss.getEvdoSnr());
            li.setLteSignalStrength(ss.getLteSignalStrength());
            li.setLteRsrp(ss.getLteRsrp());
            li.setLteRsrq(ss.getLteRsrq());
            li.setLteRssnr(ss.getLteRssnr());
            li.setLteCqi(ss.getLteCqi());
        }
        if (VDBG) {
            Slog.d(TAG, "Returning LinkQualityInfo with"
                    + " MobileNetworkType = " + String.valueOf(li.getMobileNetworkType())
                    + " Theoretical Rx BW = " + String.valueOf(li.getTheoreticalRxBandwidth())
                    + " gsm Signal Strength = " + String.valueOf(li.getRssi())
                    + " cdma Signal Strength = " + String.valueOf(li.getCdmaDbm())
                    + " evdo Signal Strength = " + String.valueOf(li.getEvdoDbm())
                    + " Lte Signal Strength = " + String.valueOf(li.getLteSignalStrength()));
        }
        return li;

#endif
}

ECode MobileDataStateTracker::GetNetworkDataEntry(
    /* [in] */ Int32 networkType,
    /* [out] */ NetworkDataEntry** result)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
        for (NetworkDataEntry entry : mTheoreticalBWTable) {
            if (entry.networkType == networkType) {
                return entry;
            }
        }
        Slog.e(TAG, "Could not find Theoretical BW entry for " + String.valueOf(networkType));
        return null;

#endif
}

ECode MobileDataStateTracker::GetNormalizedSignalStrength(
    /* [in] */ Int32 networkType,
    /* [in] */ ISignalStrength* ss,
    /* [out] */ Int32* result)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
        int level;
        switch(networkType) {
            case TelephonyManager.NETWORK_TYPE_EDGE:
            case TelephonyManager.NETWORK_TYPE_GPRS:
            case TelephonyManager.NETWORK_TYPE_UMTS:
            case TelephonyManager.NETWORK_TYPE_HSDPA:
            case TelephonyManager.NETWORK_TYPE_HSUPA:
            case TelephonyManager.NETWORK_TYPE_HSPA:
            case TelephonyManager.NETWORK_TYPE_HSPAP:
                level = ss.getGsmLevel();
                break;
            case TelephonyManager.NETWORK_TYPE_CDMA:
            case TelephonyManager.NETWORK_TYPE_1xRTT:
                level = ss.getCdmaLevel();
                break;
            case TelephonyManager.NETWORK_TYPE_EVDO_0:
            case TelephonyManager.NETWORK_TYPE_EVDO_A:
            case TelephonyManager.NETWORK_TYPE_EVDO_B:
                level = ss.getEvdoLevel();
                break;
            case TelephonyManager.NETWORK_TYPE_LTE:
                level = ss.getLteLevel();
                break;
            case TelephonyManager.NETWORK_TYPE_IDEN:
            case TelephonyManager.NETWORK_TYPE_EHRPD:
            default:
                return UNKNOWN;
        }
        return (level * LinkQualityInfo.NORMALIZED_SIGNAL_STRENGTH_RANGE) /
                SignalStrength.NUM_SIGNAL_STRENGTH_BINS;

#endif
}

ECode MobileDataStateTracker::StartSampling(
    /* [in] */ ISamplingDataTrackerSamplingSnapshot* s)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
        mSamplingDataTracker.startSampling(s);

#endif
}

ECode MobileDataStateTracker::StopSampling(
    /* [in] */ ISamplingDataTrackerSamplingSnapshot* s)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
        mSamplingDataTracker.stopSampling(s);

#endif
}

//==============================================================
// MobileDataStateTracker::MdstHandler
//==============================================================
MobileDataStateTracker::MdstHandler::MdstHandler(
    /* [in] */ ILooper* looper,
    /* [in] */ IMobileDataStateTracker* mdst)
    : Handler(looper)
{
    mMdst = mdst;
}

ECode MobileDataStateTracker::MdstHandler::HandleMessage(
    /* [in] */ AutoPtr<IMessage> msg)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translated before. Need check.
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
#endif
}

//==============================================================
// MobileDataStateTracker::MobileDataStateReceiver
//==============================================================
ECode MobileDataStateTracker::MobileDataStateReceiver::OnReceive(
    /* [in] */ IContext* context,
    /* [in] */ IIntent* intent)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
                if (intent.getAction().equals(TelephonyIntents.
                        ACTION_DATA_CONNECTION_CONNECTED_TO_PROVISIONING_APN)) {
                    String apnName = intent.getStringExtra(PhoneConstants.DATA_APN_KEY);
                    String apnType = intent.getStringExtra(PhoneConstants.DATA_APN_TYPE_KEY);
                    if (!TextUtils.equals(mApnType, apnType)) {
                        return;
                    }
                    if (DBG) {
                        log("Broadcast received: " + intent.getAction() + " apnType=" + apnType
                                + " apnName=" + apnName);
                    }
                    // Make us in the connecting state until we make a new TYPE_MOBILE_PROVISIONING
                    mMobileDataState = PhoneConstants.DataState.CONNECTING;
                    updateLinkProperitesAndCapatilities(intent);
                    mNetworkInfo.setIsConnectedToProvisioningNetwork(true);
                    // Change state to SUSPENDED so setDetailedState
                    // sends EVENT_STATE_CHANGED to connectivityService
                    setDetailedState(DetailedState.SUSPENDED, "", apnName);
                } else if (intent.getAction().equals(TelephonyIntents.
                        ACTION_ANY_DATA_CONNECTION_STATE_CHANGED)) {
                    String apnType = intent.getStringExtra(PhoneConstants.DATA_APN_TYPE_KEY);
                    if (VDBG) {
                        log(String.format("Broadcast received: ACTION_ANY_DATA_CONNECTION_STATE_CHANGED"
                            + "mApnType=%s %s received apnType=%s", mApnType,
                            TextUtils.equals(apnType, mApnType) ? "==" : "!=", apnType));
                    }
                    if (!TextUtils.equals(apnType, mApnType)) {
                        return;
                    }
                    // Assume this isn't a provisioning network.
                    mNetworkInfo.setIsConnectedToProvisioningNetwork(false);
                    if (DBG) {
                        log("Broadcast received: " + intent.getAction() + " apnType=" + apnType);
                    }
                    int oldSubtype = mNetworkInfo.getSubtype();
                    int newSubType = TelephonyManager.getDefault().getNetworkType();
                    String subTypeName = TelephonyManager.getDefault().getNetworkTypeName();
                    mNetworkInfo.setSubtype(newSubType, subTypeName);
                    if (newSubType != oldSubtype && mNetworkInfo.isConnected()) {
                        Message msg = mTarget.obtainMessage(EVENT_NETWORK_SUBTYPE_CHANGED,
                                                            oldSubtype, 0, mNetworkInfo);
                        msg.sendToTarget();
                    }
                    PhoneConstants.DataState state = Enum.valueOf(PhoneConstants.DataState.class,
                            intent.getStringExtra(PhoneConstants.STATE_KEY));
                    String reason = intent.getStringExtra(PhoneConstants.STATE_CHANGE_REASON_KEY);
                    String apnName = intent.getStringExtra(PhoneConstants.DATA_APN_KEY);
                    mNetworkInfo.setRoaming(intent.getBooleanExtra(
                            PhoneConstants.DATA_NETWORK_ROAMING_KEY, false));
                    if (VDBG) {
                        log(mApnType + " setting isAvailable to " +
                                intent.getBooleanExtra(PhoneConstants.NETWORK_UNAVAILABLE_KEY,false));
                    }
                    mNetworkInfo.setIsAvailable(!intent.getBooleanExtra(
                            PhoneConstants.NETWORK_UNAVAILABLE_KEY, false));
                    if (DBG) {
                        log("Received state=" + state + ", old=" + mMobileDataState +
                            ", reason=" + (reason == null ? "(unspecified)" : reason));
                    }
                    if (mMobileDataState != state) {
                        mMobileDataState = state;
                        switch (state) {
                            case DISCONNECTED:
                                if(isTeardownRequested()) {
                                    setTeardownRequested(false);
                                }
                                setDetailedState(DetailedState.DISCONNECTED, reason, apnName);
                                // can't do this here - ConnectivityService needs it to clear stuff
                                // it's ok though - just leave it to be refreshed next time
                                // we connect.
                                //if (DBG) log("clearing mInterfaceName for "+ mApnType +
                                //        " as it DISCONNECTED");
                                //mInterfaceName = null;
                                break;
                            case CONNECTING:
                                setDetailedState(DetailedState.CONNECTING, reason, apnName);
                                break;
                            case SUSPENDED:
                                setDetailedState(DetailedState.SUSPENDED, reason, apnName);
                                break;
                            case CONNECTED:
                                updateLinkProperitesAndCapatilities(intent);
                                setDetailedState(DetailedState.CONNECTED, reason, apnName);
                                break;
                        }
                        if (VDBG) {
                            Slog.d(TAG, "TelephonyMgr.DataConnectionStateChanged");
                            if (mNetworkInfo != null) {
                                Slog.d(TAG, "NetworkInfo = " + mNetworkInfo);
                                Slog.d(TAG, "subType = " + mNetworkInfo.getSubtype());
                                Slog.d(TAG, "subType = " + mNetworkInfo.getSubtypeName());
                            }
                            if (mLinkProperties != null) {
                                Slog.d(TAG, "LinkProperties = " + mLinkProperties);
                            } else {
                                Slog.d(TAG, "LinkProperties = " );
                            }
                            if (mNetworkCapabilities != null) {
                                Slog.d(TAG, mNetworkCapabilities.toString());
                            } else {
                                Slog.d(TAG, "NetworkCapabilities = " );
                            }
                        }
                        /* lets not sample traffic data across state changes */
                        mSamplingDataTracker.resetSamplingData();
                    } else {
                        // There was no state change. Check if LinkProperties has been updated.
                        if (TextUtils.equals(reason, PhoneConstants.REASON_LINK_PROPERTIES_CHANGED)) {
                            mLinkProperties = intent.getParcelableExtra(
                                    PhoneConstants.DATA_LINK_PROPERTIES_KEY);
                            if (mLinkProperties == null) {
                                loge("No link property in LINK_PROPERTIES change event.");
                                mLinkProperties = new LinkProperties();
                            }
                            // Just update reason field in this NetworkInfo
                            mNetworkInfo.setDetailedState(mNetworkInfo.getDetailedState(), reason,
                                                          mNetworkInfo.getExtraInfo());
                            Message msg = mTarget.obtainMessage(EVENT_CONFIGURATION_CHANGED,
                                                                mNetworkInfo);
                            msg.sendToTarget();
                        }
                    }
                } else if (intent.getAction().
                        equals(TelephonyIntents.ACTION_DATA_CONNECTION_FAILED)) {
                    String apnType = intent.getStringExtra(PhoneConstants.DATA_APN_TYPE_KEY);
                    if (!TextUtils.equals(apnType, mApnType)) {
                        if (DBG) {
                            log(String.format(
                                    "Broadcast received: ACTION_ANY_DATA_CONNECTION_FAILED ignore, " +
                                    "mApnType=%s != received apnType=%s", mApnType, apnType));
                        }
                        return;
                    }
                    // Assume this isn't a provisioning network.
                    mNetworkInfo.setIsConnectedToProvisioningNetwork(false);
                    String reason = intent.getStringExtra(PhoneConstants.FAILURE_REASON_KEY);
                    String apnName = intent.getStringExtra(PhoneConstants.DATA_APN_KEY);
                    if (DBG) {
                        log("Broadcast received: " + intent.getAction() +
                                    " reason=" + reason == null ? "null" : reason);
                    }
                    setDetailedState(DetailedState.FAILED, reason, apnName);
                } else {
                    if (DBG) log("Broadcast received: ignore " + intent.getAction());
                }

#endif
}

} // namespace Net
} // namespace Droid
} // namespace Elastos

#if 0 // old CMobileDataStateTracker.cpp
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

#endif