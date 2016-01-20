
#include "elastos/droid/server/net/TrustAgentWrapper.h"

namespace Elastos {
namespace Droid {
namespace Server {
namespace Trust {

const String TrustAgentWrapper::EXTRA_COMPONENT_NAME("componentName");
const String TrustAgentWrapper::TRUST_EXPIRED_ACTION("android.server.trust.TRUST_EXPIRED_ACTION");
const String TrustAgentWrapper::PERMISSION = android.Manifest.permission.PROVIDE_TRUST_AGENT;
const Boolean TrustAgentWrapper::DEBUG = FALSE;
const String TrustAgentWrapper::TAG("TrustAgentWrapper");
const Int32 TrustAgentWrapper::MSG_GRANT_TRUST = 1;
const Int32 TrustAgentWrapper::MSG_REVOKE_TRUST = 2;
const Int32 TrustAgentWrapper::MSG_TRUST_TIMEOUT = 3;
const Int32 TrustAgentWrapper::MSG_RESTART_TIMEOUT = 4;
const Int32 TrustAgentWrapper::MSG_SET_TRUST_AGENT_FEATURES_COMPLETED = 5;
const Int32 TrustAgentWrapper::MSG_MANAGING_TRUST = 6;
const Int64 TrustAgentWrapper::RESTART_TIMEOUT_MILLIS = 5 * 60000;
const String TrustAgentWrapper::DATA_DURATION("duration");

ECode TrustAgentWrapper::BroadcastReceiver(
    /* [out] */ Inew** result)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
        @Override
        public void onReceive(Context context, Intent intent) {
            ComponentName component = intent.getParcelableExtra(EXTRA_COMPONENT_NAME);
            if (TRUST_EXPIRED_ACTION.equals(intent.getAction())
                    && mName.equals(component)) {
                mHandler.removeMessages(MSG_TRUST_TIMEOUT);
                mHandler.sendEmptyMessage(MSG_TRUST_TIMEOUT);
            }
        }

#endif
}

ECode TrustAgentWrapper::Handler(
    /* [out] */ Inew** result)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
        @Override
        public void handleMessage(Message msg) {
            switch (msg.what) {
                case MSG_GRANT_TRUST:
                    if (!isConnected()) {
                        Log.w(TAG, "Agent is not connected, cannot grant trust: "
                                + mName.flattenToShortString());
                        return;
                    }
                    mTrusted = true;
                    mMessage = (CharSequence) msg.obj;
                    boolean initiatedByUser = msg.arg1 != 0;
                    long durationMs = msg.getData().getLong(DATA_DURATION);
                    if (durationMs > 0) {
                        final long duration;
                        if (mMaximumTimeToLock != 0) {
                            // Enforce DevicePolicyManager timeout.  This is here as a safeguard to
                            // ensure trust agents are evaluating trust state at least as often as
                            // the policy dictates. Admins that want more guarantees should be using
                            // DevicePolicyManager#KEYGUARD_DISABLE_TRUST_AGENTS.
                            duration = Math.min(durationMs, mMaximumTimeToLock);
                            if (DEBUG) {
                                Log.v(TAG, "DPM lock timeout in effect. Timeout adjusted from "
                                    + durationMs + " to " + duration);
                            }
                        } else {
                            duration = durationMs;
                        }
                        long expiration = SystemClock.elapsedRealtime() + duration;
                        mAlarmPendingIntent = PendingIntent.getBroadcast(mContext, 0, mAlarmIntent,
                                PendingIntent.FLAG_CANCEL_CURRENT);
                        mAlarmManager.set(AlarmManager.ELAPSED_REALTIME_WAKEUP, expiration,
                                mAlarmPendingIntent);
                    }
                    mTrustManagerService.mArchive.logGrantTrust(mUserId, mName,
                            (mMessage != null ? mMessage.toString() : null),
                            durationMs, initiatedByUser);
                    mTrustManagerService.updateTrust(mUserId, initiatedByUser);
                    break;
                case MSG_TRUST_TIMEOUT:
                    if (DEBUG) Slog.v(TAG, "Trust timed out : " + mName.flattenToShortString());
                    mTrustManagerService.mArchive.logTrustTimeout(mUserId, mName);
                    onTrustTimeout();
                    // Fall through.
                case MSG_REVOKE_TRUST:
                    mTrusted = false;
                    mMessage = null;
                    mHandler.removeMessages(MSG_TRUST_TIMEOUT);
                    if (msg.what == MSG_REVOKE_TRUST) {
                        mTrustManagerService.mArchive.logRevokeTrust(mUserId, mName);
                    }
                    mTrustManagerService.updateTrust(mUserId, false);
                    break;
                case MSG_RESTART_TIMEOUT:
                    unbind();
                    mTrustManagerService.resetAgent(mName, mUserId);
                    break;
                case MSG_SET_TRUST_AGENT_FEATURES_COMPLETED:
                    IBinder token = (IBinder) msg.obj;
                    boolean result = msg.arg1 != 0;
                    if (mSetTrustAgentFeaturesToken == token) {
                        mSetTrustAgentFeaturesToken = null;
                        if (mTrustDisabledByDpm && result) {
                            if (DEBUG) Log.v(TAG, "Re-enabling agent because it acknowledged "
                                    + "enabled features: " + mName);
                            mTrustDisabledByDpm = false;
                            mTrustManagerService.updateTrust(mUserId, false);
                        }
                    } else {
                        if (DEBUG) Log.w(TAG, "Ignoring MSG_SET_TRUST_AGENT_FEATURES_COMPLETED "
                                + "with obsolete token: " + mName);
                    }
                    break;
                case MSG_MANAGING_TRUST:
                    mManagingTrust = msg.arg1 != 0;
                    if (!mManagingTrust) {
                        mTrusted = false;
                        mMessage = null;
                    }
                    mTrustManagerService.mArchive.logManagingTrust(mUserId, mName, mManagingTrust);
                    mTrustManagerService.updateTrust(mUserId, false);
                    break;
            }
        }

#endif
}

ECode TrustAgentWrapper::ITrustAgentServiceCallback.Stub(
    /* [out] */ Inew** result)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
        @Override
        public void grantTrust(CharSequence userMessage, long durationMs, boolean initiatedByUser) {
            if (DEBUG) Slog.v(TAG, "enableTrust(" + userMessage + ", durationMs = " + durationMs
                        + ", initiatedByUser = " + initiatedByUser + ")");
            Message msg = mHandler.obtainMessage(
                    MSG_GRANT_TRUST, initiatedByUser ? 1 : 0, 0, userMessage);
            msg.getData().putLong(DATA_DURATION, durationMs);
            msg.sendToTarget();
        }
        @Override
        public void revokeTrust() {
            if (DEBUG) Slog.v(TAG, "revokeTrust()");
            mHandler.sendEmptyMessage(MSG_REVOKE_TRUST);
        }
        @Override
        public void setManagingTrust(boolean managingTrust) {
            if (DEBUG) Slog.v(TAG, "managingTrust()");
            mHandler.obtainMessage(MSG_MANAGING_TRUST, managingTrust ? 1 : 0, 0).sendToTarget();
        }
        @Override
        public void onSetTrustAgentFeaturesEnabledCompleted(boolean result, IBinder token) {
            if (DEBUG) Slog.v(TAG, "onSetTrustAgentFeaturesEnabledCompleted(result=" + result);
            mHandler.obtainMessage(MSG_SET_TRUST_AGENT_FEATURES_COMPLETED,
                    result ? 1 : 0, 0, token).sendToTarget();
        }

#endif
}

ECode TrustAgentWrapper::ServiceConnection(
    /* [out] */ Inew** result)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
        @Override
        public void onServiceConnected(ComponentName name, IBinder service) {
            if (DEBUG) Log.v(TAG, "TrustAgent started : " + name.flattenToString());
            mHandler.removeMessages(MSG_RESTART_TIMEOUT);
            mTrustAgentService = ITrustAgentService.Stub.asInterface(service);
            mTrustManagerService.mArchive.logAgentConnected(mUserId, name);
            setCallback(mCallback);
            updateDevicePolicyFeatures();
        }
        @Override
        public void onServiceDisconnected(ComponentName name) {
            if (DEBUG) Log.v(TAG, "TrustAgent disconnected : " + name.flattenToShortString());
            mTrustAgentService = null;
            mManagingTrust = false;
            mSetTrustAgentFeaturesToken = null;
            mTrustManagerService.mArchive.logAgentDied(mUserId, name);
            mHandler.sendEmptyMessage(MSG_REVOKE_TRUST);
            if (mBound) {
                scheduleRestart();
            }
            // mTrustDisabledByDpm maintains state
        }

#endif
}

ECode TrustAgentWrapper::constructor(
    /* [in] */ IContext* context,
    /* [in] */ ITrustManagerService* trustManagerService,
    /* [in] */ IIntent* intent,
    /* [in] */ IUserHandle* user)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
        mBroadcastReceiver = new InnerSub_BroadcastReceiver();
        mHandler = new InnerSub_Handler();
        mCallback = new InnerSub_InnerSub_ITrustAgentServiceCallback();

        mContext = context;
        mTrustManagerService = trustManagerService;
        mAlarmManager = (AlarmManager) mContext.getSystemService(Context.ALARM_SERVICE);
        mUserId = user.getIdentifier();
        mName = intent.getComponent();
        mAlarmIntent = new Intent(TRUST_EXPIRED_ACTION).putExtra(EXTRA_COMPONENT_NAME, mName);
        mAlarmIntent.setData(Uri.parse(mAlarmIntent.toUri(Intent.URI_INTENT_SCHEME)));
        mAlarmIntent.setPackage(context.getPackageName());
        final IntentFilter alarmFilter = new IntentFilter(TRUST_EXPIRED_ACTION);
        alarmFilter.addDataScheme(mAlarmIntent.getScheme());
        final String pathUri = mAlarmIntent.toUri(Intent.URI_INTENT_SCHEME);
        alarmFilter.addDataPath(pathUri, PatternMatcher.PATTERN_LITERAL);
        mContext.registerReceiver(mBroadcastReceiver, alarmFilter, PERMISSION, null);
        // Schedules a restart for when connecting times out. If the connection succeeds,
        // the restart is canceled in mCallback's onConnected.
        scheduleRestart();
        mBound = context.bindServiceAsUser(intent, mConnection, Context.BIND_AUTO_CREATE, user);
        if (!mBound) {
            Log.e(TAG, "Can't bind to TrustAgent " + mName.flattenToShortString());
        }

#endif
}

ECode TrustAgentWrapper::OnError(
    /* [in] */ ECode ec)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
        Slog.w(TAG , "Remote Exception", e);

#endif
}

ECode TrustAgentWrapper::OnTrustTimeout()
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
        try {
            if (mTrustAgentService != null) mTrustAgentService.onTrustTimeout();
        } catch (RemoteException e) {
            onError(e);
        }

#endif
}

ECode TrustAgentWrapper::OnUnlockAttempt(
    /* [in] */ Boolean successful)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
        try {
            if (mTrustAgentService != null) mTrustAgentService.onUnlockAttempt(successful);
        } catch (RemoteException e) {
            onError(e);
        }

#endif
}

ECode TrustAgentWrapper::SetCallback(
    /* [in] */ IITrustAgentServiceCallback* callback)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
        try {
            if (mTrustAgentService != null) {
                mTrustAgentService.setCallback(callback);
            }
        } catch (RemoteException e) {
            onError(e);
        }

#endif
}

ECode TrustAgentWrapper::UpdateDevicePolicyFeatures(
    /* [out] */ Boolean* result)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
        boolean trustDisabled = false;
        if (DEBUG) Slog.v(TAG, "updateDevicePolicyFeatures(" + mName + ")");
        try {
            if (mTrustAgentService != null) {
                DevicePolicyManager dpm =
                    (DevicePolicyManager) mContext.getSystemService(Context.DEVICE_POLICY_SERVICE);
                if ((dpm.getKeyguardDisabledFeatures(null)
                        & DevicePolicyManager.KEYGUARD_DISABLE_TRUST_AGENTS) != 0) {
                    List<String> features = dpm.getTrustAgentFeaturesEnabled(null, mName);
                    trustDisabled = true;
                    if (DEBUG) Slog.v(TAG, "Detected trust agents disabled. Features = "
                            + features);
                    if (features != null && features.size() > 0) {
                        Bundle bundle = new Bundle();
                        bundle.putStringArrayList(TrustAgentService.KEY_FEATURES,
                                (ArrayList<String>)features);
                        if (DEBUG) {
                            Slog.v(TAG, "TrustAgent " + mName.flattenToShortString()
                                    + " disabled until it acknowledges "+ features);
                        }
                        mSetTrustAgentFeaturesToken = new Binder();
                        mTrustAgentService.setTrustAgentFeaturesEnabled(bundle,
                                mSetTrustAgentFeaturesToken);
                    }
                }
                final long maxTimeToLock = dpm.getMaximumTimeToLock(null);
                if (maxTimeToLock != mMaximumTimeToLock) {
                    // If the timeout changes, cancel the alarm and send a timeout event to have
                    // the agent re-evaluate trust.
                    mMaximumTimeToLock = maxTimeToLock;
                    if (mAlarmPendingIntent != null) {
                        mAlarmManager.cancel(mAlarmPendingIntent);
                        mAlarmPendingIntent = null;
                        mHandler.sendEmptyMessage(MSG_TRUST_TIMEOUT);
                    }
                }
            }
        } catch (RemoteException e) {
            onError(e);
        }
        if (mTrustDisabledByDpm != trustDisabled) {
            mTrustDisabledByDpm = trustDisabled;
            mTrustManagerService.updateTrust(mUserId, false);
        }
        return trustDisabled;

#endif
}

ECode TrustAgentWrapper::IsTrusted(
    /* [out] */ Boolean* result)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
        return mTrusted && mManagingTrust && !mTrustDisabledByDpm;

#endif
}

ECode TrustAgentWrapper::IsManagingTrust(
    /* [out] */ Boolean* result)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
        return mManagingTrust && !mTrustDisabledByDpm;

#endif
}

ECode TrustAgentWrapper::GetMessage(
    /* [out] */ ICharSequence** result)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
        return mMessage;

#endif
}

ECode TrustAgentWrapper::Unbind()
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
        if (!mBound) {
            return;
        }
        if (DEBUG) Log.v(TAG, "TrustAgent unbound : " + mName.flattenToShortString());
        mTrustManagerService.mArchive.logAgentStopped(mUserId, mName);
        mContext.unbindService(mConnection);
        mBound = false;
        mTrustAgentService = null;
        mSetTrustAgentFeaturesToken = null;
        mHandler.sendEmptyMessage(MSG_REVOKE_TRUST);
        mHandler.removeMessages(MSG_RESTART_TIMEOUT);

#endif
}

ECode TrustAgentWrapper::IsConnected(
    /* [out] */ Boolean* result)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
        return mTrustAgentService != null;

#endif
}

ECode TrustAgentWrapper::IsBound(
    /* [out] */ Boolean* result)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
        return mBound;

#endif
}

ECode TrustAgentWrapper::GetScheduledRestartUptimeMillis(
    /* [out] */ Int64* result)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
        return mScheduledRestartUptimeMillis;

#endif
}

ECode TrustAgentWrapper::ScheduleRestart()
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
        mHandler.removeMessages(MSG_RESTART_TIMEOUT);
        mScheduledRestartUptimeMillis = SystemClock.uptimeMillis() + RESTART_TIMEOUT_MILLIS;
        mHandler.sendEmptyMessageAtTime(MSG_RESTART_TIMEOUT, mScheduledRestartUptimeMillis);

#endif
}

} // namespace Trust
} // namespace Server
} // namespace Droid
} // namespace Elastos
