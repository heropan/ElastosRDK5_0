
#ifndef __ELASTOS_DROID_SERVER_TRUST_TRUSTAGENTWRAPPER_H__
#define __ELASTOS_DROID_SERVER_TRUST_TRUSTAGENTWRAPPER_H__

#include "_Elastos.Droid.Server.h"
#include "elastos/droid/ext/frameworkext.h"
#include <elastos/core/Object.h>

namespace Elastos {
namespace Droid {
namespace Server {
namespace Trust {

/**
 * A wrapper around a TrustAgentService interface. Coordinates communication between
 * TrustManager and the actual TrustAgent.
 */
class TrustAgentWrapper
{
private:
    class InnerSub_BroadcastReceiver
        : public BroadcastReceiver
    {
        @Override
        public void onReceive(Context context, Intent intent) {
            ComponentName component = intent.getParcelableExtra(EXTRA_COMPONENT_NAME);
            if (TRUST_EXPIRED_ACTION.equals(intent.getAction())
                    && mName.equals(component)) {
                mHandler.removeMessages(MSG_TRUST_TIMEOUT);
                mHandler.sendEmptyMessage(MSG_TRUST_TIMEOUT);
            }
        }
    };

    class InnerSub_Handler
        : public Handler
    {
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
    };

    class InnerSub_ITrustAgentServiceCallback
        : public Object
        , public IITrustAgentServiceCallback
    {
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
    };

    class InnerSub_ServiceConnection
        : public ServiceConnection
    {
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
    };
public:
    CARAPI constructor(
        /* [in] */ IContext* context,
        /* [in] */ ITrustManagerService* trustManagerService,
        /* [in] */ IIntent* intent,
        /* [in] */ IUserHandle* user);

    /**
     * @see android.service.trust.TrustAgentService#onUnlockAttempt(boolean)
     */
    CARAPI OnUnlockAttempt(
        /* [in] */ Boolean successful);

    CARAPI UpdateDevicePolicyFeatures(
        /* [out] */ Boolean* result);

    CARAPI IsTrusted(
        /* [out] */ Boolean* result);

    CARAPI IsManagingTrust(
        /* [out] */ Boolean* result);

    CARAPI GetMessage(
        /* [out] */ ICharSequence** result);

    CARAPI Unbind();

    CARAPI IsConnected(
        /* [out] */ Boolean* result);

    CARAPI IsBound(
        /* [out] */ Boolean* result);

    /**
     * If not connected, returns the time at which the agent is restarted.
     *
     * @return restart time in uptime millis.
     */
    CARAPI GetScheduledRestartUptimeMillis(
        /* [out] */ Int64* result);

private:
    CARAPI OnError(
        /* [in] */ ECode ec);

    CARAPI OnTrustTimeout();

    CARAPI SetCallback(
        /* [in] */ IITrustAgentServiceCallback* callback);

    CARAPI ScheduleRestart();

private:
    static const String EXTRA_COMPONENT_NAME;

    static const String TRUST_EXPIRED_ACTION;

    static const String PERMISSION;

    static const Boolean DEBUG;

    static const String TAG;

    static const Int32 MSG_GRANT_TRUST;

    static const Int32 MSG_REVOKE_TRUST;

    static const Int32 MSG_TRUST_TIMEOUT;

    static const Int32 MSG_RESTART_TIMEOUT;

    static const Int32 MSG_SET_TRUST_AGENT_FEATURES_COMPLETED;

    static const Int32 MSG_MANAGING_TRUST;

    /**
     * Time in uptime millis that we wait for the service connection, both when starting
     * and when the service disconnects.
     */
    static const Int64 RESTART_TIMEOUT_MILLIS;

    /**
     * Long extra for {@link #MSG_GRANT_TRUST}
     */
    static const String DATA_DURATION;

    const ITrustManagerService* mTrustManagerService;

    const Int32 mUserId;

    const IContext* mContext;

    const IComponentName* mName;

    IITrustAgentService* mTrustAgentService;

    Boolean mBound;

    Int64 mScheduledRestartUptimeMillis;

    Int64 mMaximumTimeToLock;

    // Trust state
    Boolean mTrusted;

    ICharSequence* mMessage;

    Boolean mTrustDisabledByDpm;

    Boolean mManagingTrust;

    IIBinder* mSetTrustAgentFeaturesToken;

    IAlarmManager* mAlarmManager;

    const IIntent* mAlarmIntent;

    IPendingIntent* mAlarmPendingIntent;

    IBroadcastReceiver* mBroadcastReceiver;

    IHandler* mHandler;

    IITrustAgentServiceCallback* mCallback;

    IServiceConnection* mConnection;
};

} // namespace Trust
} // namespace Server
} // namespace Droid
} // namespace Elastos

#endif // __ELASTOS_DROID_SERVER_TRUST_TRUSTAGENTWRAPPER_H__
