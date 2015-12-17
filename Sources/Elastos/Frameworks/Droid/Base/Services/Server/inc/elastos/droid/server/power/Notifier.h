#ifndef __ELASTOS_DROID_SERVER_POWER_NOTIFIER_H__
#define __ELASTOS_DROID_SERVER_POWER_NOTIFIER_H__

#include "elastos/droid/ext/frameworkdef.h"
#include "Elastos.Droid.Server.h"
#include "elastos/droid/content/BroadcastReceiver.h"
#include "elastos/droid/os/HandlerBase.h"

using Elastos::Droid::Os::HandlerBase;
using Elastos::Droid::Os::ILooper;
using Elastos::Droid::Os::IWorkSource;
using Elastos::Droid::Content::IBroadcastReceiver;
using Elastos::Droid::Content::BroadcastReceiver;
using Elastos::Droid::Content::IContext;
using Elastos::Droid::Content::IIntent;
using Elastos::Droid::View::IWindowManagerPolicy;
using Elastos::Droid::View::IScreenOnListener;
using Elastos::Droid::Internal::App::IIBatteryStats;

namespace Elastos {
namespace Droid {
namespace Server {
namespace Power {

/**
 * Sends broadcasts about important power state changes.
 * <p>
 * This methods of this class may be called by the power manager service while
 * its lock is being held.  Internally it takes care of sending broadcasts to
 * notify other components of the system or applications asynchronously.
 * </p><p>
 * The notifier is designed to collapse unnecessary broadcasts when it is not
 * possible for the system to have observed an intermediate state.
 * </p><p>
 * For example, if the device wakes up, goes to sleep, wakes up again and goes to
 * sleep again before the wake up notification is sent, then the system will
 * be told about only one wake up and sleep.  However, we always notify the
 * fact that at least one transition occurred.  It is especially important to
 * tell the system when we go to sleep so that it can lock the keyguard if needed.
 * </p>
 */
class Notifier : public ElRefBase
{
private:
    class NotifierHandler
        : public HandlerBase
    {
    public:
        NotifierHandler(
            /* [in] */ ILooper* looper,
            /* [in] */ Notifier* service)
            : HandlerBase(looper)
            , mHost(service)
        {}

        CARAPI HandleMessage(
            /* [in] */ IMessage* msg);

    private:
        Notifier* mHost;
    };

    class ScreenOnListener
        : public ElRefBase
        , public IScreenOnListener
    {
    public:
        ScreenOnListener(
            /* [in] */ Notifier* host);

        CAR_INTERFACE_DECL();

        CARAPI OnScreenOn();

    private:
        Notifier* mHost;
    };

    class WakeUpBroadcastDone : public BroadcastReceiver
    {
    public:
        WakeUpBroadcastDone(
            /* [in] */ Notifier* host);

        CARAPI OnReceive(
            /* [in] */ IContext* context,
            /* [in] */ IIntent* intent);

        CARAPI ToString(
            /* [out] */ String* info)
        {
            VALIDATE_NOT_NULL(info);
            *info = String("Notifier::WakeUpBroadcastDone: ");
            (*info).AppendFormat("%p", this);
            return NOERROR;
        }
    private:
        Notifier* mHost;
    };

    class GoToSleepBroadcastDone : public BroadcastReceiver
    {
    public:
        GoToSleepBroadcastDone(
            /* [in] */ Notifier* host);

        CARAPI OnReceive(
            /* [in] */ IContext* context,
            /* [in] */ IIntent* intent);

        CARAPI ToString(
            /* [out] */ String* info)
        {
            VALIDATE_NOT_NULL(info);
            *info = String("Notifier::GoToSleepBroadcastDone: ");
            (*info).AppendFormat("%p", this);
            return NOERROR;
        }
    private:
        Notifier* mHost;
    };

public:
    Notifier(
        /* [in] */ ILooper* looper,
        /* [in] */ IContext* context,
        /* [in] */ IIBatteryStats* batteryStats,
        /* [in] */ ISuspendBlocker* suspendBlocker,
        /* [in] */ IScreenOnBlocker* screenOnBlocker,
        /* [in] */ IWindowManagerPolicy* policy);

    /**
     * Called when a wake lock is acquired.
     */
    CARAPI_(void) OnWakeLockAcquired(
        /* [in] */ Int32 flags,
        /* [in] */ const String& tag,
        /* [in] */ Int32 ownerUid,
        /* [in] */ Int32 ownerPid,
        /* [in] */ IWorkSource* workSource);

    /**
     * Called when a wake lock is released.
     */
    CARAPI_(void) OnWakeLockReleased(
        /* [in] */ Int32 flags,
        /* [in] */ const String& tag,
        /* [in] */ Int32 ownerUid,
        /* [in] */ Int32 ownerPid,
        /* [in] */ IWorkSource* workSource);

    /**
     * Called when the screen is turned on.
     */
    CARAPI_(void) OnScreenOn();

    /**
     * Called when the screen is turned off.
     */
    CARAPI_(void) OnScreenOff();

    /**
     * Called when the screen changes brightness.
     */
    CARAPI_(void) OnScreenBrightness(
        /* [in] */ Int32 brightness);

    /**
     * Called when the device is waking up from sleep and the
     * display is about to be turned on.
     */
    CARAPI_(void) OnWakeUpStarted();

    /**
     * Called when the device has finished waking up from sleep
     * and the display has been turned on.
     */
    CARAPI_(void) OnWakeUpFinished();

    /**
     * Called when the device is going to sleep.
     */
    CARAPI_(void) OnGoToSleepStarted(
        /* [in] */ Int32 reason);

    /**
     * Called when the device has finished going to sleep and the
     * display has been turned off.
     *
     * This is a good time to make transitions that we don't want the user to see,
     * such as bringing the key guard to focus.  There's no guarantee for this,
     * however because the user could turn the device on again at any time.
     * Some things may need to be protected by other mechanisms that defer screen on.
     */
    CARAPI_(void) OnGoToSleepFinished();

    /**
     * Called when there has been user activity.
     */
    CARAPI_(void) OnUserActivity(
        /* [in] */ Int32 event,
        /* [in] */ Int32 uid);

    CARAPI_(void) OnBootFastWake();

    CARAPI_(void) OnBootFastSleep();

    CARAPI_(void) OnWirelessChargingStarted();

private:
    static CARAPI_(Int32) GetBatteryStatsWakeLockMonitorType(
        /* [in] */ Int32 flags);

    CARAPI_(void) UpdatePendingBroadcastLocked();

    CARAPI_(void) FinishPendingBroadcastLocked();

    CARAPI_(void) SendUserActivity();

    CARAPI_(void) SendBootFastWake();

    CARAPI_(void) SendBootFastSleep();

    CARAPI_(void) SendNextBroadcast();

    CARAPI_(void) SendWakeUpBroadcast();

    CARAPI_(void) SendGoToSleepBroadcast(
        /* [in] */ Int32 reason);

    CARAPI_(void) PlayWirelessChargingStartedSound();

private:
    static const String TAG;

    static const Boolean DEBUG;

    static const Int32 POWER_STATE_UNKNOWN;
    static const Int32 POWER_STATE_AWAKE;
    static const Int32 POWER_STATE_ASLEEP;

    static const Int32 MSG_USER_ACTIVITY;// = 1;
    static const Int32 MSG_BROADCAST;// = 2;
    static const Int32 MSG_WIRELESS_CHARGING_STARTED;// = 3;
    static const Int32 MSG_BOOT_FAST_WAKE;// = 4;
    static const Int32 MSG_BOOT_FAST_SLEEP;// = 5;

    Object mLock;

    AutoPtr<IContext> mContext;
    AutoPtr<IIBatteryStats> mBatteryStats;
    AutoPtr<ISuspendBlocker> mSuspendBlocker;
    AutoPtr<IScreenOnBlocker> mScreenOnBlocker;
    AutoPtr<IWindowManagerPolicy> mPolicy;

    AutoPtr<IHandler> mHandler;
    AutoPtr<IIntent> mScreenOnIntent;
    AutoPtr<IIntent> mScreenOffIntent;

    // The current power state.
    Int32 mActualPowerState;
    Int32 mLastGoToSleepReason;

    // True if there is a pending transition that needs to be reported.
    Boolean mPendingWakeUpBroadcast;
    Boolean mPendingGoToSleepBroadcast;

    // The currently broadcasted power state.  This reflects what other parts of the
    // system have observed.
    Int32 mBroadcastedPowerState;
    Boolean mBroadcastInProgress;
    Int64 mBroadcastStartTime;

    // True if a user activity message should be sent.
    Boolean mUserActivityPending;

    Boolean mBootFastWakePending;

    Boolean mBootFastSleepPending;

    // True if the screen on blocker has been acquired.
    Boolean mScreenOnBlockerAcquired;

    AutoPtr<IScreenOnListener> mScreenOnListener;
    AutoPtr<IBroadcastReceiver> mWakeUpBroadcastDone;
    AutoPtr<IBroadcastReceiver> mGoToSleepBroadcastDone;

    friend class ScreenOnListener;
    friend class WakeUpBroadcastDone;
    friend class GoToSleepBroadcastDone;
};

} // namespace Power
} // namespace Server
} // namespace Droid
} // namespace Elastos

#endif // __ELASTOS_DROID_SERVER_POWER_NOTIFIER_H__
