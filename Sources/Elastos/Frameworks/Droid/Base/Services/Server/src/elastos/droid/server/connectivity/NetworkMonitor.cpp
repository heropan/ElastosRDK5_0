
#include <elastos/droid/server/connectivity/NetworkMonitor.h>


namespace Elastos {
namespace Droid {
namespace Server {
namespace Connectivity {


//==============================================================================
// NetworkMonitor::DefaultState
//==============================================================================

NetworkMonitor::DefaultState::DefaultState(
    /* [in] */ NetworkMonitor* host)
    : mHost(host)
{}

ECode NetworkMonitor::DefaultState::ProcessMessage(
    /* [in] */ IMessage* msg,
    /* [out] */ Boolean* result);
{
    if (DBG) log(getName() + message.toString());
    switch (message.what) {
        case CMD_NETWORK_LINGER:
            if (DBG) log("Lingering");
            transitionTo(mLingeringState);
            return HANDLED;
        case CMD_NETWORK_CONNECTED:
            if (DBG) log("Connected");
            transitionTo(mEvaluatingState);
            return HANDLED;
        case CMD_NETWORK_DISCONNECTED:
            if (DBG) log("Disconnected - quitting");
            quit();
            return HANDLED;
        case CMD_FORCE_REEVALUATION:
            if (DBG) log("Forcing reevaluation");
            mUidResponsibleForReeval = message.arg1;
            transitionTo(mEvaluatingState);
            return HANDLED;
        default:
            return HANDLED;
    }
}

//==============================================================================
// NetworkMonitor::OfflineState
//==============================================================================

NetworkMonitor::OfflineState::OfflineState(
    /* [in] */ NetworkMonitor* host)
    : mHost(host)
{}

ECode NetworkMonitor::OfflineState::Enter()
{
    mConnectivityServiceHandler.sendMessage(obtainMessage(EVENT_NETWORK_TESTED,
            NETWORK_TEST_RESULT_INVALID, 0, mNetworkAgentInfo));
}

ECode NetworkMonitor::OfflineState::ProcessMessage(
    /* [in] */ IMessage* msg,
    /* [out] */ Boolean* result);
{
    if (DBG) log(getName() + message.toString());
                switch (message.what) {
        case CMD_FORCE_REEVALUATION:
            // If the user has indicated they explicitly do not want to use this network,
            // don't allow a reevaluation as this will be pointless and could result in
            // the user being annoyed with repeated unwanted notifications.
            return mUserDoesNotWant ? HANDLED : NOT_HANDLED;
        default:
            return NOT_HANDLED;
    }
}

//==============================================================================
// NetworkMonitor::ValidatedState
//==============================================================================

NetworkMonitor::ValidatedState::ValidatedState(
    /* [in] */ NetworkMonitor* host)
    : mHost(host)
{}

ECode NetworkMonitor::ValidatedState::Enter()
{
    if (DBG) log("Validated");
    mConnectivityServiceHandler.sendMessage(obtainMessage(EVENT_NETWORK_TESTED,
            NETWORK_TEST_RESULT_VALID, 0, mNetworkAgentInfo));
}

ECode NetworkMonitor::ValidatedState::ProcessMessage(
    /* [in] */ IMessage* msg,
    /* [out] */ Boolean* result);
{
    if (DBG) log(getName() + message.toString());
    switch (message.what) {
        case CMD_NETWORK_CONNECTED:
            transitionTo(mValidatedState);
            return HANDLED;
        default:
            return NOT_HANDLED;
    }
}

//==============================================================================
// NetworkMonitor::EvaluatingState
//==============================================================================

NetworkMonitor::EvaluatingState::EvaluatingState(
    /* [in] */ NetworkMonitor* host)
    : mHost(host)
    , mRetries(0)
{}

ECode NetworkMonitor::EvaluatingState::Enter()
{
    mRetries = 0;
    sendMessage(CMD_REEVALUATE, ++mReevaluateToken, 0);
    if (mUidResponsibleForReeval != INVALID_UID) {
        TrafficStats.setThreadStatsUid(mUidResponsibleForReeval);
        mUidResponsibleForReeval = INVALID_UID;
    }
}

ECode NetworkMonitor::EvaluatingState::ProcessMessage(
    /* [in] */ IMessage* msg,
    /* [out] */ Boolean* result);
{
    if (DBG) log(getName() + message.toString());
    switch (message.what) {
        case CMD_REEVALUATE:
            if (message.arg1 != mReevaluateToken)
                return HANDLED;
            if (mNetworkAgentInfo.isVPN()) {
                transitionTo(mValidatedState);
                return HANDLED;
            }
            // If network provides no internet connectivity adjust evaluation.
            if (!mNetworkAgentInfo.networkCapabilities.hasCapability(
                    NetworkCapabilities.NET_CAPABILITY_INTERNET)) {
                // TODO: Try to verify something works.  Do all gateways respond to pings?
                transitionTo(mValidatedState);
                return HANDLED;
            }
            Int32 httpResponseCode = IsCaptivePortal();
            if (httpResponseCode == 204) {
                transitionTo(mValidatedState);
            } else if (httpResponseCode >= 200 && httpResponseCode <= 399) {
                transitionTo(mUserPromptedState);
            } else if (++mRetries > MAX_RETRIES) {
                transitionTo(mOfflineState);
            } else if (mReevaluateDelayMs >= 0) {
                Message msg = obtainMessage(CMD_REEVALUATE, ++mReevaluateToken, 0);
                sendMessageDelayed(msg, mReevaluateDelayMs);
            }
            return HANDLED;
        case CMD_FORCE_REEVALUATION:
            // Ignore duplicate requests.
            return HANDLED;
        default:
            return NOT_HANDLED;
    }
}

ECode NetworkMonitor::EvaluatingState::Exit()
{
    TrafficStats.clearThreadStatsUid();
}

//==============================================================================
// NetworkMonitor::CustomIntentReceiver
//==============================================================================

NetworkMonitor::CustomIntentReceiver::CustomIntentReceiver(
    /* [in] */ const String& action,
    /* [in] */ Int32 token,
    /* [in] */ Int32 message,
    /* [in] */ NetworkMonitor* host)
{
    mHost = host;
    mMessage = obtainMessage(message, token);
    mAction = action + "_" + mNetworkAgentInfo.network.netId + "_" + token;
    mContext.registerReceiver(this, new IntentFilter(mAction));
}

ECode NetworkMonitor::CustomIntentReceiver::GetPendingIntent(
    /* [out] */ IPendingIntent** pi)
{
    return PendingIntent.getBroadcast(mContext, 0, new Intent(mAction), 0);
}

ECode NetworkMonitor::CustomIntentReceiver::OnReceive(
    /* [in] */ IContext* context,
    /* [in] */ IIntent* intent)
{
    if (intent.getAction().equals(mAction)) sendMessage(mMessage);
}

//==============================================================================
// NetworkMonitor::UserPromptedState
//==============================================================================
// Intent broadcast when user selects sign-in notification.
const String NetworkMonitor::UserPromptedState::ACTION_SIGN_IN_REQUESTED("android.net.netmon.sign_in_requested");

NetworkMonitor::UserPromptedState::UserPromptedState(
    /* [in] */ NetworkMonitor* host)
    : mHost(host)
{}

ECode  NetworkMonitor::UserPromptedState::Enter() {
    mConnectivityServiceHandler.sendMessage(obtainMessage(EVENT_NETWORK_TESTED,
            NETWORK_TEST_RESULT_INVALID, 0, mNetworkAgentInfo));
    // Wait for user to select sign-in notifcation.
    mUserRespondedBroadcastReceiver = new CustomIntentReceiver(ACTION_SIGN_IN_REQUESTED,
            ++mUserPromptedToken, CMD_USER_WANTS_SIGN_IN);
    // Initiate notification to sign-in.
    Message message = obtainMessage(EVENT_PROVISIONING_NOTIFICATION, 1,
            mNetworkAgentInfo.network.netId,
            mUserRespondedBroadcastReceiver.getPendingIntent());
    mConnectivityServiceHandler.sendMessage(message);
}

ECode  NetworkMonitor::UserPromptedState::ProcessMessage(
    /* [in] */ IMessage* msg,
    /* [out] */ Boolean* result);
{
    if (DBG) log(getName() + message.toString());
    switch (message.what) {
        case CMD_USER_WANTS_SIGN_IN:
            if (message.arg1 != mUserPromptedToken)
                return HANDLED;
            transitionTo(mCaptivePortalState);
            return HANDLED;
        default:
            return NOT_HANDLED;
    }
}

ECode  NetworkMonitor::UserPromptedState::Exit() {
    Message message = obtainMessage(EVENT_PROVISIONING_NOTIFICATION, 0,
            mNetworkAgentInfo.network.netId, null);
    mConnectivityServiceHandler.sendMessage(message);
    mContext.unregisterReceiver(mUserRespondedBroadcastReceiver);
    mUserRespondedBroadcastReceiver = null;
}


//==============================================================================
// NetworkMonitor::CaptivePortalState
//==============================================================================
class CaptivePortalState : public State
{
    class CaptivePortalLoggedInBroadcastReceiver
        : public BroadcastReceiver
    {
    public:
        CaptivePortalLoggedInBroadcastReceiver(Int32 token)
        {
            mToken = token;
        }

        //@Override
        ECode OnReceive(
            /* [in] */ IContext* context,
            /* [in] */ IIntent* intent)
        {
            if (Integer.parseInt(intent.getStringExtra(Intent.EXTRA_TEXT)) ==
                    mNetworkAgentInfo.network.netId) {
                sendMessage(obtainMessage(CMD_CAPTIVE_PORTAL_LOGGED_IN, mToken,
                        Integer.parseInt(intent.getStringExtra(LOGGED_IN_RESULT))));
            }
        }
    private:
        Int32 mToken;
    };

//==============================================================================
// NetworkMonitor::CaptivePortalState
//==============================================================================
NetworkMonitor::CaptivePortalState::CaptivePortalState(
    /* [in] */ NetworkMonitor* host)
    : mHost(host)
{}

ECode NetworkMonitor::CaptivePortalState::Enter()
{
    Intent intent = new Intent(Intent.ACTION_SEND);
    intent.putExtra(Intent.EXTRA_TEXT, String.valueOf(mNetworkAgentInfo.network.netId));
    intent.setType("text/plain");
    intent.setComponent(new ComponentName("com.android.captiveportallogin",
            "com.android.captiveportallogin.CaptivePortalLoginActivity"));
    intent.setFlags(Intent.FLAG_ACTIVITY_BROUGHT_TO_FRONT | Intent.FLAG_ACTIVITY_NEW_TASK);

    // Wait for result.
    mCaptivePortalLoggedInBroadcastReceiver = new CaptivePortalLoggedInBroadcastReceiver(
            ++mCaptivePortalLoggedInToken);
    IntentFilter filter = new IntentFilter(ACTION_CAPTIVE_PORTAL_LOGGED_IN);
    mContext.registerReceiver(mCaptivePortalLoggedInBroadcastReceiver, filter);
    // Initiate app to log in.
    mContext.startActivityAsUser(intent, UserHandle.CURRENT);
}

ECode NetworkMonitor::CaptivePortalState::ProcessMessage(
    /* [in] */ IMessage* msg,
    /* [out] */ Boolean* result);
{
    if (DBG) log(getName() + message.toString());
    switch (message.what) {
        case CMD_CAPTIVE_PORTAL_LOGGED_IN:
            if (message.arg1 != mCaptivePortalLoggedInToken)
                return HANDLED;
            if (message.arg2 == 0) {
                mUserDoesNotWant = true;
                // TODO: Should teardown network.
                transitionTo(mOfflineState);
            } else {
                transitionTo(mValidatedState);
            }
            return HANDLED;
        default:
            return NOT_HANDLED;
    }
}

ECode NetworkMonitor::CaptivePortalState::Exit()
{
    mContext.unregisterReceiver(mCaptivePortalLoggedInBroadcastReceiver);
    mCaptivePortalLoggedInBroadcastReceiver = null;
}

//==============================================================================
// NetworkMonitor::LingeringState
//==============================================================================

const String NetworkMonitor::LingeringState::ACTION_LINGER_EXPIRED("android.net.netmon.lingerExpired");

NetworkMonitor::LingeringState::LingeringState(
    /* [in] */ NetworkMonitor* host)
    : mHost(host)
{}

ECode NetworkMonitor::LingeringState::Enter() {
    mBroadcastReceiver = new CustomIntentReceiver(ACTION_LINGER_EXPIRED, ++mLingerToken,
            CMD_LINGER_EXPIRED);
    mIntent = mBroadcastReceiver.getPendingIntent();
    long wakeupTime = SystemClock.elapsedRealtime() + mLingerDelayMs;
    mAlarmManager.setWindow(AlarmManager.ELAPSED_REALTIME_WAKEUP, wakeupTime,
            // Give a specific window so we aren't subject to unknown inexactitude.
            mLingerDelayMs / 6, mIntent);
}

ECode NetworkMonitor::LingeringState::ProcessMessage(
    /* [in] */ IMessage* msg,
    /* [out] */ Boolean* result);
{
    if (DBG) log(getName() + message.toString());
    switch (message.what) {
        case CMD_NETWORK_CONNECTED:
            // Go straight to active as we've already evaluated.
            transitionTo(mValidatedState);
            return HANDLED;
        case CMD_LINGER_EXPIRED:
            if (message.arg1 != mLingerToken)
                return HANDLED;
            mConnectivityServiceHandler.sendMessage(
                    obtainMessage(EVENT_NETWORK_LINGER_COMPLETE, mNetworkAgentInfo));
            return HANDLED;
        case CMD_FORCE_REEVALUATION:
            // Ignore reevaluation attempts when lingering.  A reevaluation could result
            // in a transition to the validated state which would abort the linger
            // timeout.  Lingering is the result of score assessment; validity is
            // irrelevant.
            return HANDLED;
        default:
            return NOT_HANDLED;
    }
}

ECode NetworkMonitor::LingeringState::Exit()
{
    mAlarmManager.cancel(mIntent);
    mContext.unregisterReceiver(mBroadcastReceiver);
}


//==============================================================================
// NetworkMonitor
//==============================================================================

const String NetworkMonitor::ACTION_NETWORK_CONDITIONS_MEASURED("android.net.conn.NETWORK_CONDITIONS_MEASURED");

const String NetworkMonitor::EXTRA_CONNECTIVITY_TYPE("extra_connectivity_type");
const String NetworkMonitor::EXTRA_NETWORK_TYPE("extra_network_type");
const String NetworkMonitor::EXTRA_RESPONSE_RECEIVED("extra_response_received");
const String NetworkMonitor::EXTRA_IS_CAPTIVE_PORTAL("extra_is_captive_portal");
const String NetworkMonitor::EXTRA_CELL_ID("extra_cellid");
const String NetworkMonitor::EXTRA_SSID("extra_ssid");
const String NetworkMonitor::EXTRA_BSSID("extra_bssid");
const String NetworkMonitor::EXTRA_REQUEST_TIMESTAMP_MS("extra_request_timestamp_ms");
const String NetworkMonitor::EXTRA_RESPONSE_TIMESTAMP_MS("extra_response_timestamp_ms");

const Int32 NetworkMonitor::NETWORK_TEST_RESULT_VALID = 0;
const Int32 NetworkMonitor::NETWORK_TEST_RESULT_INVALID = 1;

const Int32 NetworkMonitor::BASE = IProtocol::BASE_NETWORK_MONITOR;
const Int32 NetworkMonitor::CMD_NETWORK_CONNECTED = BASE + 1;
const Int32 NetworkMonitor::EVENT_NETWORK_TESTED = BASE + 2;
const Int32 NetworkMonitor::CMD_NETWORK_LINGER = BASE + 3;
const Int32 NetworkMonitor::CMD_LINGER_EXPIRED = BASE + 4;
const Int32 NetworkMonitor::EVENT_NETWORK_LINGER_COMPLETE = BASE + 5;
const Int32 NetworkMonitor::CMD_REEVALUATE = BASE + 6;
const Int32 NetworkMonitor::CMD_NETWORK_DISCONNECTED = BASE + 7;
const Int32 NetworkMonitor::CMD_FORCE_REEVALUATION = BASE + 8;
const Int32 NetworkMonitor::CMD_CAPTIVE_PORTAL_LOGGED_IN = BASE + 9;
const Int32 NetworkMonitor::CMD_USER_WANTS_SIGN_IN = BASE + 10;
const Int32 NetworkMonitor::EVENT_PROVISIONING_NOTIFICATION = BASE + 11;
const Int32 NetworkMonitor::EVENT_APP_BYPASSED_CAPTIVE_PORTAL = BASE + 12;
const Int32 NetworkMonitor::EVENT_NO_APP_RESPONSE = BASE + 13;
const Int32 NetworkMonitor::EVENT_APP_INDICATES_SIGN_IN_IMPOSSIBLE = BASE + 14;

const Boolean NetworkMonitor::DBG = TRUE;
const String NetworkMonitor::TAG("NetworkMonitor");
const String NetworkMonitor::DEFAULT_SERVER("clients3.google.com");
const Int32 NetworkMonitor::SOCKET_TIMEOUT_MS = 10000;

const String NetworkMonitor::PERMISSION_ACCESS_NETWORK_CONDITIONS("android.permission.ACCESS_NETWORK_CONDITIONS");
const String NetworkMonitor::ACTION_CAPTIVE_PORTAL_LOGGED_IN("android.net.netmon.captive_portal_logged_in");
const String NetworkMonitor::LOGGED_IN_RESULT("result");
const String NetworkMonitor::LINGER_DELAY_PROPERTY("persist.netmon.linger");

const Int32 NetworkMonitor::DEFAULT_LINGER_DELAY_MS = 30000;
const String NetworkMonitor::REEVALUATE_DELAY_PROPERTY("persist.netmon.reeval_delay");
const Int32 NetworkMonitor::DEFAULT_REEVALUATE_DELAY_MS = 5000;

const Int32 NetworkMonitor::MAX_RETRIES = 10;
const Int32 NetworkMonitor::INVALID_UID = -1;


NetworkMonitor::NetworkMonitor(
    /* [in] */ IContext* context,
    /* [in] */ IHandler* handler,
    /* [in] */ NetworkAgentInfo* networkAgentInfo)
    : systemReady(FALSE)
    , mLingerDelayMs(FALSE)
    , mLingerToken(FALSE)
    , mReevaluateDelayMs(FALSE)
    , mReevaluateToken(FALSE)
    , mUidResponsibleForReeval(INVALID_UID)
    , mCaptivePortalLoggedInToken(0)
    , mUserPromptedToken(0)
    , mIsCaptivePortalCheckEnabled(FALSE)
    , mUserDoesNotWant(FALSE)
{
    // Add suffix indicating which NetworkMonitor we're talking about.
    super(TAG + networkAgentInfo.name());

    mDefaultState = new DefaultState(this);
    mOfflineState = new OfflineState(this);
    mValidatedState = new ValidatedState(this);
    mEvaluatingState = new EvaluatingState(this);
    mUserPromptedState = new UserPromptedState(this);
    mCaptivePortalState = new CaptivePortalState(this);
    mLingeringState = new LingeringState(this);

    mContext = context;
    mConnectivityServiceHandler = handler;
    mNetworkAgentInfo = networkAgentInfo;
    mTelephonyManager = (TelephonyManager) context.getSystemService(Context.TELEPHONY_SERVICE);
    mWifiManager = (WifiManager) context.getSystemService(Context.WIFI_SERVICE);
    mAlarmManager = (AlarmManager) context.getSystemService(Context.ALARM_SERVICE);

    AddState(mDefaultState);
    AddState(mOfflineState, mDefaultState);
    AddState(mValidatedState, mDefaultState);
    AddState(mEvaluatingState, mDefaultState);
    AddState(mUserPromptedState, mDefaultState);
    AddState(mCaptivePortalState, mDefaultState);
    AddState(mLingeringState, mDefaultState);
    SetInitialState(mDefaultState);

    mServer = Settings.Global.getString(mContext.getContentResolver(),
            Settings.Global.CAPTIVE_PORTAL_SERVER);
    if (mServer == null) mServer = DEFAULT_SERVER;

    mLingerDelayMs = SystemProperties.getInt(LINGER_DELAY_PROPERTY, DEFAULT_LINGER_DELAY_MS);
    mReevaluateDelayMs = SystemProperties.getInt(REEVALUATE_DELAY_PROPERTY,
            DEFAULT_REEVALUATE_DELAY_MS);

    mIsCaptivePortalCheckEnabled = Settings.Global.getInt(mContext.getContentResolver(),
            Settings.Global.CAPTIVE_PORTAL_DETECTION_ENABLED, 1) == 1;

    start();
}

Int32 NetworkMonitor::IsCaptivePortal()
{
    if (!mIsCaptivePortalCheckEnabled) return 204;

    HttpURLConnection urlConnection = null;
    Int32 httpResponseCode = 599;
    try {
        URL url = new URL("http", mServer, "/generate_204");
        if (DBG) {
            log("Checking " + url.toString() + " on " +
                    mNetworkAgentInfo.networkInfo.getExtraInfo());
        }
        urlConnection = (HttpURLConnection) mNetworkAgentInfo.network.openConnection(url);
        urlConnection.setInstanceFollowRedirects(false);
        urlConnection.setConnectTimeout(SOCKET_TIMEOUT_MS);
        urlConnection.setReadTimeout(SOCKET_TIMEOUT_MS);
        urlConnection.setUseCaches(false);

        // Time how long it takes to get a response to our request
        long requestTimestamp = SystemClock.elapsedRealtime();

        urlConnection.getInputStream();

        // Time how long it takes to get a response to our request
        long responseTimestamp = SystemClock.elapsedRealtime();

        httpResponseCode = urlConnection.getResponseCode();
        if (DBG) {
            log("IsCaptivePortal: ret=" + httpResponseCode +
                    " headers=" + urlConnection.getHeaderFields());
        }
        // NOTE: We may want to consider an "HTTP/1.0 204" response to be a captive
        // portal.  The only example of this seen so far was a captive portal.  For
        // the time being go with prior behavior of assuming it's not a captive
        // portal.  If it is considered a captive portal, a different sign-in URL
        // is needed (i.e. can't browse a 204).  This could be the result of an HTTP
        // proxy server.

        // Consider 200 response with "Content-length=0" to not be a captive portal.
        // There's no point in considering this a captive portal as the user cannot
        // sign-in to an empty page.  Probably the result of a broken transparent proxy.
        // See http://b/9972012.
        if (httpResponseCode == 200 && urlConnection.getContentLength() == 0) {
            if (DBG) log("Empty 200 response interpreted as 204 response.");
            httpResponseCode = 204;
        }

        SendNetworkConditionsBroadcast(true /* response received */, httpResponseCode == 204,
                requestTimestamp, responseTimestamp);
    } catch (IOException e) {
        if (DBG) log("Probably not a portal: exception " + e);
        if (httpResponseCode == 599) {
            // TODO: Ping gateway and DNS server and log results.
        }
    } finally {
        if (urlConnection != null) {
            urlConnection.disconnect();
        }
    }
    return httpResponseCode;
}

void NetworkMonitor::SendNetworkConditionsBroadcast(
    /* [in] */ Boolean responseReceived,
    /* [in] */ Boolean IsCaptivePortal,
    /* [in] */ Int64 requestTimestampMs,
     /* [in] */ Int64 responseTimestampMs)
{
    if (Settings.Global.getInt(mContext.getContentResolver(),
            Settings.Global.WIFI_SCAN_ALWAYS_AVAILABLE, 0) == 0) {
        if (DBG) log("Don't send network conditions - lacking user consent.");
        return;
    }

    if (systemReady == false) return;

    Intent latencyBroadcast = new Intent(ACTION_NETWORK_CONDITIONS_MEASURED);
    switch (mNetworkAgentInfo.networkInfo.getType()) {
        case ConnectivityManager.TYPE_WIFI:
            WifiInfo currentWifiInfo = mWifiManager.getConnectionInfo();
            if (currentWifiInfo != null) {
                // NOTE: getSSID()'s behavior changed in API 17; before that, SSIDs were not
                // surrounded by double quotation marks (thus violating the Javadoc), but this
                // was changed to match the Javadoc in API 17. Since clients may have started
                // sanitizing the output of this method since API 17 was released, we should
                // not change it here as it would become impossible to tell whether the SSID is
                // simply being surrounded by quotes due to the API, or whether those quotes
                // are actually part of the SSID.
                latencyBroadcast.putExtra(EXTRA_SSID, currentWifiInfo.getSSID());
                latencyBroadcast.putExtra(EXTRA_BSSID, currentWifiInfo.getBSSID());
            } else {
                if (DBG) logw("network info is TYPE_WIFI but no ConnectionInfo found");
                return;
            }
            break;
        case ConnectivityManager.TYPE_MOBILE:
            latencyBroadcast.putExtra(EXTRA_NETWORK_TYPE, mTelephonyManager.getNetworkType());
            List<CellInfo> info = mTelephonyManager.getAllCellInfo();
            if (info == null) return;
            Int32 numRegisteredCellInfo = 0;
            for (CellInfo cellInfo : info) {
                if (cellInfo.isRegistered()) {
                    numRegisteredCellInfo++;
                    if (numRegisteredCellInfo > 1) {
                        if (DBG) log("more than one registered CellInfo.  Can't " +
                                "tell which is active.  Bailing.");
                        return;
                    }
                    if (cellInfo instanceof CellInfoCdma) {
                        CellIdentityCdma cellId = ((CellInfoCdma) cellInfo).getCellIdentity();
                        latencyBroadcast.putExtra(EXTRA_CELL_ID, cellId);
                    } else if (cellInfo instanceof CellInfoGsm) {
                        CellIdentityGsm cellId = ((CellInfoGsm) cellInfo).getCellIdentity();
                        latencyBroadcast.putExtra(EXTRA_CELL_ID, cellId);
                    } else if (cellInfo instanceof CellInfoLte) {
                        CellIdentityLte cellId = ((CellInfoLte) cellInfo).getCellIdentity();
                        latencyBroadcast.putExtra(EXTRA_CELL_ID, cellId);
                    } else if (cellInfo instanceof CellInfoWcdma) {
                        CellIdentityWcdma cellId = ((CellInfoWcdma) cellInfo).getCellIdentity();
                        latencyBroadcast.putExtra(EXTRA_CELL_ID, cellId);
                    } else {
                        if (DBG) logw("Registered cellinfo is unrecognized");
                        return;
                    }
                }
            }
            break;
        default:
            return;
    }
    latencyBroadcast.putExtra(EXTRA_CONNECTIVITY_TYPE, mNetworkAgentInfo.networkInfo.getType());
    latencyBroadcast.putExtra(EXTRA_RESPONSE_RECEIVED, responseReceived);
    latencyBroadcast.putExtra(EXTRA_REQUEST_TIMESTAMP_MS, requestTimestampMs);

    if (responseReceived) {
        latencyBroadcast.putExtra(EXTRA_IS_CAPTIVE_PORTAL, IsCaptivePortal);
        latencyBroadcast.putExtra(EXTRA_RESPONSE_TIMESTAMP_MS, responseTimestampMs);
    }
    mContext.sendBroadcastAsUser(latencyBroadcast, UserHandle.CURRENT,
            PERMISSION_ACCESS_NETWORK_CONDITIONS);
}


} // Connectivity
} // Server
} // Droid
} // Elastos
