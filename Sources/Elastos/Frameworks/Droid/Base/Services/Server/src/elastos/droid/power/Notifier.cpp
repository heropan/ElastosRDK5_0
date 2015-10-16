
#include "power/Notifier.h"
#include "power/MobileDirectController.h"
#include "elastos/droid/app/ActivityManagerNative.h"
#include "elastos/droid/os/SystemClock.h"
#include "elastos/droid/os/Handler.h"
#include "elastos/droid/os/BatteryStats.h"
#include <elastos/utility/logging/Slogger.h>

using Elastos::Utility::Logging::Slogger;
using Elastos::Droid::View::EIID_IScreenOnListener;
using Elastos::Droid::Os::CHandler;
using Elastos::Droid::Os::IPowerManager;
using Elastos::Droid::Os::IUserHandleHelper;
using Elastos::Droid::Os::CUserHandleHelper;
using Elastos::Droid::Os::IUserHandle;
using Elastos::Droid::Os::SystemClock;
using Elastos::Droid::Os::BatteryStats;
using Elastos::Droid::Content::CIntent;
using Elastos::Droid::Content::IContentResolver;
using Elastos::Droid::App::ActivityManagerNative;
using Elastos::Droid::Net::IUriHelper;
using Elastos::Droid::Net::CUriHelper;
using Elastos::Droid::Net::IUri;
using Elastos::Droid::Media::IRingtone;
using Elastos::Droid::Media::IRingtoneManagerHelper;
using Elastos::Droid::Media::CRingtoneManagerHelper;
using Elastos::Droid::Media::IAudioManager;
using Elastos::Droid::Provider::CSettingsGlobal;
using Elastos::Droid::Provider::ISettingsGlobal;

namespace Elastos {
namespace Droid {
namespace Server {
namespace Power {

Notifier::ScreenOnListener::ScreenOnListener(
    /* [in] */ Notifier* host)
    : mHost(host)
{}

CAR_INTERFACE_IMPL(Notifier::ScreenOnListener, IScreenOnListener);

ECode Notifier::ScreenOnListener::OnScreenOn()
{
    AutoLock lock(mHost->mLock);
    if (mHost->mScreenOnBlockerAcquired && !mHost->mPendingWakeUpBroadcast) {
        mHost->mScreenOnBlockerAcquired = FALSE;
        mHost->mScreenOnBlocker->Release();
    }
    return NOERROR;
}


Notifier::WakeUpBroadcastDone::WakeUpBroadcastDone(
    /* [in] */ Notifier* host)
    : mHost(host)
{}

ECode Notifier::WakeUpBroadcastDone::OnReceive(
    /* [in] */ IContext* context,
    /* [in] */ IIntent* intent)
{
    // EventLog.writeEvent(EventLogTags.POWER_SCREEN_BROADCAST_DONE, 1,
    //         SystemClock.uptimeMillis() - mBroadcastStartTime, 1);
    mHost->SendNextBroadcast();
    return NOERROR;
}


Notifier::GoToSleepBroadcastDone::GoToSleepBroadcastDone(
    /* [in] */ Notifier* host)
    : mHost(host)
{}

ECode Notifier::GoToSleepBroadcastDone::OnReceive(
    /* [in] */ IContext* context,
    /* [in] */ IIntent* intent)
{
    // EventLog.writeEvent(EventLogTags.POWER_SCREEN_BROADCAST_DONE, 0,
    //         SystemClock.uptimeMillis() - mBroadcastStartTime, 1);
    mHost->SendNextBroadcast();
    return NOERROR;
}


const String Notifier::TAG("PowerManagerNotifier");
const Boolean Notifier::DEBUG = FALSE;
const Int32 Notifier::POWER_STATE_UNKNOWN = 0;
const Int32 Notifier::POWER_STATE_AWAKE = 1;
const Int32 Notifier::POWER_STATE_ASLEEP = 2;

const Int32 Notifier::MSG_USER_ACTIVITY = 1;
const Int32 Notifier::MSG_BROADCAST = 2;
const Int32 Notifier::MSG_WIRELESS_CHARGING_STARTED = 3;
const Int32 Notifier::MSG_BOOT_FAST_WAKE = 4;
const Int32 Notifier::MSG_BOOT_FAST_SLEEP = 5;

ECode Notifier::NotifierHandler::HandleMessage(
    /* [in] */ IMessage* msg)
{
    Int32 what;
    msg->GetWhat(&what);

    switch(what) {
        case Notifier::MSG_USER_ACTIVITY:
            mHost->SendUserActivity();
            break;

        case Notifier::MSG_BROADCAST:
            mHost->SendNextBroadcast();
            break;
        case Notifier::MSG_BOOT_FAST_WAKE:
            mHost->SendBootFastWake();
            break;
        case Notifier::MSG_BOOT_FAST_SLEEP:
            mHost->SendBootFastSleep();
            break;
        case Notifier::MSG_WIRELESS_CHARGING_STARTED:
            mHost->PlayWirelessChargingStartedSound();
            break;
    }

    return NOERROR;
}

Notifier::Notifier(
    /* [in] */ ILooper* looper,
    /* [in] */ IContext* context,
    /* [in] */ IIBatteryStats* batteryStats,
    /* [in] */ ISuspendBlocker* suspendBlocker,
    /* [in] */ IScreenOnBlocker* screenOnBlocker,
    /* [in] */ IWindowManagerPolicy* policy)
    : mContext(context)
    , mBatteryStats(batteryStats)
    , mSuspendBlocker(suspendBlocker)
    , mScreenOnBlocker(screenOnBlocker)
    , mPolicy(policy)
    , mActualPowerState(0)
    , mLastGoToSleepReason(0)
    , mPendingWakeUpBroadcast(FALSE)
    , mPendingGoToSleepBroadcast(FALSE)
    , mBroadcastedPowerState(0)
    , mBroadcastInProgress(FALSE)
    , mBroadcastStartTime(0)
    , mUserActivityPending(FALSE)
    , mBootFastWakePending(FALSE)
    , mBootFastSleepPending(FALSE)
    , mScreenOnBlockerAcquired(FALSE)
{
    mScreenOnListener = (IScreenOnListener*)new ScreenOnListener(this);
    mWakeUpBroadcastDone = (IBroadcastReceiver*)new WakeUpBroadcastDone(this);
    mGoToSleepBroadcastDone = (IBroadcastReceiver*)new GoToSleepBroadcastDone(this);

    mHandler = new NotifierHandler(looper, this);
    ASSERT_SUCCEEDED(CIntent::New(IIntent::ACTION_SCREEN_ON, (IIntent**)&mScreenOnIntent));
    mScreenOnIntent->AddFlags(
        IIntent::FLAG_RECEIVER_REGISTERED_ONLY | IIntent::FLAG_RECEIVER_FOREGROUND);
    ASSERT_SUCCEEDED(CIntent::New(IIntent::ACTION_SCREEN_OFF, (IIntent**)&mScreenOffIntent));
    mScreenOffIntent->AddFlags(
        IIntent::FLAG_RECEIVER_REGISTERED_ONLY | IIntent::FLAG_RECEIVER_FOREGROUND);
}

void Notifier::OnWakeLockAcquired(
    /* [in] */ Int32 flags,
    /* [in] */ const String& tag,
    /* [in] */ Int32 ownerUid,
    /* [in] */ Int32 ownerPid,
    /* [in] */ IWorkSource* workSource)
{
    if (DEBUG) {
        Slogger::D(TAG, "onWakeLockAcquired: flags=%d, tag=\"%s\", ownerUid=%d, ownerPid=%d, workSource=%p"
            , flags, (const char*)tag, ownerUid, ownerPid, workSource);
    }

    // try {
    Int32 monitorType = GetBatteryStatsWakeLockMonitorType(flags);
    if (workSource != NULL) {
        mBatteryStats->NoteStartWakelockFromSource(workSource, ownerPid, tag, monitorType);
    }
    else {
        mBatteryStats->NoteStartWakelock(ownerUid, ownerPid, tag, monitorType);
    }
    // } catch (RemoteException ex) {
    //     // Ignore
    // }
}

void Notifier::OnWakeLockReleased(
    /* [in] */ Int32 flags,
    /* [in] */ const String& tag,
    /* [in] */ Int32 ownerUid,
    /* [in] */ Int32 ownerPid,
    /* [in] */ IWorkSource* workSource)
{
    if (DEBUG) {
        Slogger::D(TAG, "onWakeLockReleased: flags=%d, tag=\"%s\", ownerUid=%d, ownerPid=%d, workSource=%p"
                , flags, (const char*)tag, ownerUid, ownerPid, workSource);
    }

    // try {
    Int32 monitorType = GetBatteryStatsWakeLockMonitorType(flags);
    if (workSource != NULL) {
        mBatteryStats->NoteStopWakelockFromSource(workSource, ownerPid, tag, monitorType);
    }
    else {
        mBatteryStats->NoteStopWakelock(ownerUid, ownerPid, tag, monitorType);
    }
    // } catch (RemoteException ex) {
    //     // Ignore
    // }
}

Int32 Notifier::GetBatteryStatsWakeLockMonitorType(
    /* [in] */ Int32 flags)
{
    switch (flags & IPowerManager::WAKE_LOCK_LEVEL_MASK) {
        case IPowerManager::PARTIAL_WAKE_LOCK:
        case IPowerManager::PROXIMITY_SCREEN_OFF_WAKE_LOCK:
            return BatteryStats::WAKE_TYPE_PARTIAL;
        default:
            return BatteryStats::WAKE_TYPE_FULL;
    }
    return 0;
}

void Notifier::OnScreenOn()
{
    if (DEBUG) {
        Slogger::D(TAG, "onScreenOn");
    }

    // try {

    mBatteryStats->NoteScreenOn();
    // } catch (RemoteException ex) {
    //     // Ignore
    // }
}

void Notifier::OnScreenOff()
{
    if (DEBUG) {
        Slogger::D(TAG, "onScreenOff");
    }

    // try {
    mBatteryStats->NoteScreenOff();
    // } catch (RemoteException ex) {
    //     // Ignore
    // }
}

void Notifier::OnScreenBrightness(
    /* [in] */ Int32 brightness)
{
    if (DEBUG) {
        Slogger::D(TAG, "onScreenBrightness: brightness=%d", brightness);
    }

    // try {
    mBatteryStats->NoteScreenBrightness(brightness);
    // } catch (RemoteException ex) {
    //     // Ignore
    // }
}

void Notifier::OnWakeUpStarted()
{
    if (DEBUG) {
        Slogger::D(TAG, "onWakeUpStarted");
    }

    AutoLock lock(mLock);
    if (mActualPowerState != POWER_STATE_AWAKE) {
        mActualPowerState = POWER_STATE_AWAKE;
        mPendingWakeUpBroadcast = TRUE;
        if (!mScreenOnBlockerAcquired) {
            mScreenOnBlockerAcquired = TRUE;
            mScreenOnBlocker->Acquire();
        }
        UpdatePendingBroadcastLocked();
    }
}

void Notifier::OnWakeUpFinished()
{
    if (DEBUG) {
        Slogger::D(TAG, "onWakeUpFinished");
    }
}

void Notifier::OnGoToSleepStarted(
    /* [in] */ Int32 reason)
{
    if (DEBUG) {
        Slogger::D(TAG, "onGoToSleepStarted");
    }

    AutoLock lock(mLock);
    mLastGoToSleepReason = reason;
}

void Notifier::OnGoToSleepFinished()
{
    if (DEBUG) {
        Slogger::D(TAG, "onGoToSleepFinished");
    }

    AutoLock lock(mLock);
    if (mActualPowerState != POWER_STATE_ASLEEP) {
        mActualPowerState = POWER_STATE_ASLEEP;
        mPendingGoToSleepBroadcast = TRUE;
        if (mUserActivityPending) {
            mUserActivityPending = FALSE;
            mHandler->RemoveMessages(MSG_USER_ACTIVITY);
        }
        UpdatePendingBroadcastLocked();
    }
}

void Notifier::OnUserActivity(
    /* [in] */ Int32 event,
    /* [in] */ Int32 uid)
{
    if (DEBUG) {
        Slogger::D(TAG, "onUserActivity: event=%d, uid=%d", event, uid);
    }

    // try {
    mBatteryStats->NoteUserActivity(uid, event);
    // } catch (RemoteException ex) {
    //     // Ignore
    // }

    AutoLock lock(mLock);
    if (!mUserActivityPending) {
        mUserActivityPending = TRUE;

        AutoPtr<IMessage> msg;
        mHandler->ObtainMessage(MSG_USER_ACTIVITY, (IMessage**)&msg);
        msg->SetAsynchronous(TRUE);
        Boolean result;
        mHandler->SendMessage(msg, &result);
    }
}

void Notifier::OnBootFastWake()
{
    AutoLock lock(mLock);
    if (!mBootFastWakePending) {
        mBootFastWakePending = TRUE;

        AutoPtr<IMessage> msg;
        mHandler->ObtainMessage(MSG_BOOT_FAST_WAKE, (IMessage**)&msg);
        msg->SetAsynchronous(TRUE);
        Boolean result;
        mHandler->SendMessage(msg, &result);
    }
}

void Notifier::OnBootFastSleep()
{
    AutoLock lock(mLock);
    if (!mBootFastSleepPending) {
        mBootFastSleepPending = TRUE;

        AutoPtr<IMessage> msg;
        mHandler->ObtainMessage(MSG_BOOT_FAST_SLEEP, (IMessage**)&msg);
        msg->SetAsynchronous(TRUE);
        Boolean result;
        mHandler->SendMessage(msg, &result);
    }
}

void Notifier::OnWirelessChargingStarted()
{
    if (DEBUG) {
        Slogger::D(TAG, "onWirelessChargingStarted");
    }

    mSuspendBlocker->Acquire();
    AutoPtr<IMessage> msg;
    mHandler->ObtainMessage(MSG_WIRELESS_CHARGING_STARTED, (IMessage**)&msg);
    msg->SetAsynchronous(TRUE);
    Boolean result;
    mHandler->SendMessage(msg, &result);
}

void Notifier::UpdatePendingBroadcastLocked()
{
    if (!mBroadcastInProgress
            && mActualPowerState != POWER_STATE_UNKNOWN
            && (mPendingWakeUpBroadcast || mPendingGoToSleepBroadcast
                    || mActualPowerState != mBroadcastedPowerState)) {
        mBroadcastInProgress = TRUE;
        mSuspendBlocker->Acquire();

        mSuspendBlocker->Acquire();
        AutoPtr<IMessage> msg;
        mHandler->ObtainMessage(MSG_BROADCAST, (IMessage**)&msg);
        msg->SetAsynchronous(TRUE);
        Boolean result;
        mHandler->SendMessage(msg, &result);
    }
}

void Notifier::FinishPendingBroadcastLocked()
{
    mBroadcastInProgress = FALSE;
    mSuspendBlocker->Release();
}

void Notifier::SendUserActivity()
{
    {
        AutoLock lock(mLock);
        if (!mUserActivityPending) {
            return;
        }
        mUserActivityPending = FALSE;
    }

    mPolicy->UserActivity();
}

void Notifier::SendBootFastWake()
{
    {
        AutoLock lock(mLock);
        if(!mBootFastWakePending){
            return;
        }
        mBootFastWakePending = FALSE;
    }

    AutoPtr<IIntent> intent;
    CIntent::New(IIntent::ACTION_BOOT_FAST, (IIntent**)&intent);
    intent->AddFlags(IIntent::FLAG_RECEIVER_REGISTERED_ONLY | IIntent::FLAG_RECEIVER_REPLACE_PENDING);
    intent->PutExtra(IIntent::EXTRA_BOOT_FAST, 1);

    ActivityManagerNative::BroadcastStickyIntent(intent, String(NULL), IUserHandle::USER_ALL);
    AutoPtr<MobileDirectController> controller = MobileDirectController::GetInstance();
    if (controller->IsMobileModeAvailable()) {
        controller->SetNetworkEnable(TRUE);
    }
    mPolicy->ScreenTurningOn(NULL);
}

void Notifier::SendBootFastSleep()
{
    {
        AutoLock lock(mLock);
        if(!mBootFastWakePending){
            return;
        }
        mBootFastWakePending = FALSE;
    }

    AutoPtr<IIntent> intent;
    CIntent::New(IIntent::ACTION_BOOT_FAST, (IIntent**)&intent);
    intent->AddFlags(IIntent::FLAG_RECEIVER_REGISTERED_ONLY | IIntent::FLAG_RECEIVER_REPLACE_PENDING);
    intent->PutExtra(IIntent::EXTRA_BOOT_FAST, 0);
    ActivityManagerNative::BroadcastStickyIntent(intent, String(NULL), IUserHandle::USER_ALL);

    mPolicy->ScreenTurnedOff(0);
    mPolicy->HideScreen(TRUE);
}

void Notifier::SendNextBroadcast()
{
    Int32 powerState;
    Int32 goToSleepReason;
    {
        AutoLock lock(mLock);
        if (mBroadcastedPowerState == POWER_STATE_UNKNOWN) {
            // Broadcasted power state is unknown.  Send wake up.
            mPendingWakeUpBroadcast = FALSE;
            mBroadcastedPowerState = POWER_STATE_AWAKE;
        }
        else if (mBroadcastedPowerState == POWER_STATE_AWAKE) {
            // Broadcasted power state is awake.  Send asleep if needed.
            if (mPendingWakeUpBroadcast || mPendingGoToSleepBroadcast
                    || mActualPowerState == POWER_STATE_ASLEEP) {
                mPendingGoToSleepBroadcast = FALSE;
                mBroadcastedPowerState = POWER_STATE_ASLEEP;
            }
            else {
                FinishPendingBroadcastLocked();
                return;
            }
        }
        else {
            // Broadcasted power state is asleep.  Send awake if needed.
            if (mPendingWakeUpBroadcast || mPendingGoToSleepBroadcast
                    || mActualPowerState == POWER_STATE_AWAKE) {
                mPendingWakeUpBroadcast = FALSE;
                mBroadcastedPowerState = POWER_STATE_AWAKE;
            }
            else {
                FinishPendingBroadcastLocked();
                return;
            }
        }

        mBroadcastStartTime = SystemClock::GetUptimeMillis();
        powerState = mBroadcastedPowerState;
        goToSleepReason = mLastGoToSleepReason;
    }

    // EventLog.writeEvent(EventLogTags.POWER_SCREEN_BROADCAST_SEND, 1);

    if (powerState == POWER_STATE_AWAKE) {
        SendWakeUpBroadcast();
    }
    else {
        SendGoToSleepBroadcast(goToSleepReason);
    }
}

void Notifier::SendWakeUpBroadcast()
{
    if (DEBUG) {
        Slogger::D(TAG, "Sending wake up broadcast.");
    }

    // EventLog.writeEvent(EventLogTags.POWER_SCREEN_STATE, 1, 0, 0, 0);

    mPolicy->ScreenTurningOn(mScreenOnListener);

    // try {
    ActivityManagerNative::GetDefault()->WakingUp();
    // } catch (RemoteException e) {
    //     // ignore it
    // }

    // if (ActivityManagerNative.isSystemReady()) {
    AutoPtr<IUserHandleHelper> handleHelper;
    ASSERT_SUCCEEDED(CUserHandleHelper::AcquireSingleton((IUserHandleHelper**)&handleHelper));
    AutoPtr<IUserHandle> all;
    handleHelper->GetALL((IUserHandle**)&all);
    mContext->SendOrderedBroadcastAsUser(mScreenOnIntent, all, String(NULL),
            mWakeUpBroadcastDone, mHandler, 0, String(NULL), NULL);
    // } else {
    //     EventLog.writeEvent(EventLogTags.POWER_SCREEN_BROADCAST_STOP, 2, 1);
    //     SendNextBroadcast();
    // }
}

void Notifier::SendGoToSleepBroadcast(
    /* [in] */ Int32 reason)
{
    if (DEBUG) {
        Slogger::D(TAG, "Sending go to sleep broadcast.");
    }

    Int32 why = IWindowManagerPolicy::OFF_BECAUSE_OF_USER;
    switch (reason) {
        case IPowerManager::GO_TO_SLEEP_REASON_DEVICE_ADMIN:
            why = IWindowManagerPolicy::OFF_BECAUSE_OF_ADMIN;
            break;
        case IPowerManager::GO_TO_SLEEP_REASON_TIMEOUT:
            why = IWindowManagerPolicy::OFF_BECAUSE_OF_TIMEOUT;
            break;
    }

    // EventLog.writeEvent(EventLogTags.POWER_SCREEN_STATE, 0, why, 0, 0);

    mPolicy->ScreenTurnedOff(why);
    // try {
    ActivityManagerNative::GetDefault()->GoingToSleep();
    // } catch (RemoteException e) {
    //     // ignore it.
    // }

    // if (ActivityManagerNative.isSystemReady()) {
    AutoPtr<IUserHandleHelper> handleHelper;
    ASSERT_SUCCEEDED(CUserHandleHelper::AcquireSingleton((IUserHandleHelper**)&handleHelper));
    AutoPtr<IUserHandle> all;
    handleHelper->GetALL((IUserHandle**)&all);
    mContext->SendOrderedBroadcastAsUser(mScreenOffIntent, all, String(NULL),
            mGoToSleepBroadcastDone, mHandler, 0, String(NULL), NULL);
    // } else {
    //     EventLog.writeEvent(EventLogTags.POWER_SCREEN_BROADCAST_STOP, 3, 1);
    //     sendNextBroadcast();
    // }
}

void Notifier::PlayWirelessChargingStartedSound()
{
    AutoPtr<IContentResolver> cr;
    mContext->GetContentResolver((IContentResolver**)&cr);
    String soundPath;
    AutoPtr<ISettingsGlobal> settingsGlobal;
    CSettingsGlobal::AcquireSingleton((ISettingsGlobal**)&settingsGlobal);
    settingsGlobal->GetString(cr,
            ISettingsGlobal::WIRELESS_CHARGING_STARTED_SOUND, &soundPath);
    if (!soundPath.IsNull()) {
        AutoPtr<IUriHelper> helper;
        CUriHelper::AcquireSingleton((IUriHelper**)&helper);
        AutoPtr<IUri> soundUri;
        helper->Parse(String("file://") + soundPath, (IUri**)&soundPath);
        if (soundUri != NULL) {
            AutoPtr<IRingtoneManagerHelper> rmHelper;
            CRingtoneManagerHelper::AcquireSingleton((IRingtoneManagerHelper**)&rmHelper);
            AutoPtr<IRingtone> sfx;
            rmHelper->GetRingtone(mContext, soundUri, (IRingtone**)&sfx);
            if (sfx != NULL) {
                sfx->SetStreamType(IAudioManager::STREAM_SYSTEM);
                sfx->Play();
            }
        }
    }

    mSuspendBlocker->Release();
}

} // namespace Power
} // namespace Server
} // namespace Droid
} // namespace Elastos
