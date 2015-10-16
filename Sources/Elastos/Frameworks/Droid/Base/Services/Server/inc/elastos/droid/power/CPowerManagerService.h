
#ifndef __ELASTOS_DROID_SERVER_POWER_CPOWERMANAGERSERVICE_H__
#define __ELASTOS_DROID_SERVER_POWER_CPOWERMANAGERSERVICE_H__

#include "_Elastos_Droid_Server_Power_CPowerManagerService.h"
#include "am/CActivityManagerService.h"
#include "LightsService.h"
#include "CBatteryService.h"
#include "TwilightService.h"
#include "power/DisplayPowerController.h"
#include "power/WirelessChargerDetector.h"
// #include "dreams/CDreamManagerService.h"
#include "elastos/droid/content/BroadcastReceiver.h"
#include "elastos/droid/database/ContentObserver.h"
#include "elastos/droid/os/Runnable.h"
#include "elastos/droid/os/HandlerBase.h"
#include <elastos/Core/Object.h>


using Elastos::Core::IRunnable;
using Elastos::IO::IPrintWriter;
using Elastos::IO::IFileDescriptor;
using Elastos::Droid::Internal::App::IIBatteryStats;
using Elastos::Droid::View::IWindowManagerPolicy;
using Elastos::Droid::Os::Runnable;
using Elastos::Droid::Os::IHandlerThread;
using Elastos::Droid::Os::IHandler;
using Elastos::Droid::Os::IPowerManager;
using Elastos::Droid::Os::IWorkSource;
using Elastos::Droid::Os::IBinder;
using Elastos::Droid::Content::BroadcastReceiver;
using Elastos::Droid::Content::IIntent;
using Elastos::Droid::Content::IContext;
using Elastos::Droid::Content::IContentResolver;
using Elastos::Droid::Database::ContentObserver;
using Elastos::Droid::Server::Am::CActivityManagerService;
using Elastos::Droid::Server::CBatteryService;

namespace Elastos {
namespace Droid {
namespace Server {
namespace Power {

/**
 * The power manager service is responsible for coordinating power management
 * functions on the device.
 */
CarClass(CPowerManagerService)
{
private:
    class PowerManagerHandler : public HandlerBase
    {
    public:
        PowerManagerHandler(
            /* [in] */ ILooper* looper,
            /* [in] */ CPowerManagerService* service)
            : HandlerBase(looper, NULL, TRUE)
            , mHost(service)
        {}

        CARAPI HandleMessage(
            /* [in] */ IMessage* msg);

    private:
        CPowerManagerService* mHost;
    };

    class HdmiPluggedReceiver : public BroadcastReceiver
    {
    public:
        HdmiPluggedReceiver(
            /* [in] */ CPowerManagerService* host);

        CARAPI OnReceive(
            /* [in] */ IContext* context,
            /* [in] */ IIntent* intent);

        CARAPI ToString(
            /* [out] */ String* info)
        {
            VALIDATE_NOT_NULL(info);
            *info = String("CPowerManagerService::HdmiPluggedReceiver: ");
            (*info).AppendFormat("%p", this);
            return NOERROR;
        }
    private:
        CPowerManagerService* mHost;
    };

    class MyDisplayPowerControllerCallbacks
        : public ElRefBase
        , public DisplayPowerController::IDisplayPowerControllerCallbacks
    {
    public:
        CAR_INTERFACE_DECL()

        MyDisplayPowerControllerCallbacks(
            /* [in] */ CPowerManagerService* host);

        CARAPI_(void) OnStateChanged();

        CARAPI_(void) OnProximityPositive();

        CARAPI_(void) OnProximityNegative();

    private:
        CPowerManagerService* mHost;
    };

    class BatteryReceiver : public BroadcastReceiver
    {
    public:
        BatteryReceiver(
            /* [in] */ CPowerManagerService* powerMagService);

        CARAPI OnReceive(
            /* [in] */ IContext* context,
            /* [in] */ IIntent* intent);

        CARAPI ToString(
            /* [out] */ String* info)
        {
            VALIDATE_NOT_NULL(info);
            *info = String("CPowerManagerService::BatteryReceiver: ");
            (*info).AppendFormat("%p", this);
            return NOERROR;
        }
    private:
        CPowerManagerService* mHost;
    };

    class BootCompletedReceiver : public BroadcastReceiver
    {
    public:
        BootCompletedReceiver(
            /* [in] */ CPowerManagerService* host);

        CARAPI OnReceive(
            /* [in] */ IContext* context,
            /* [in] */ IIntent* intent);

        CARAPI ToString(
            /* [out] */ String* info)
        {
            VALIDATE_NOT_NULL(info);
            *info = String("CPowerManagerService::BootCompletedReceiver: ");
            (*info).AppendFormat("%p", this);
            return NOERROR;
        }
    private:
        CPowerManagerService* mHost;
    };

    class DreamReceiver : public BroadcastReceiver
    {
    public:
        DreamReceiver(
            /* [in] */ CPowerManagerService* host);

        CARAPI OnReceive(
            /* [in] */ IContext* context,
            /* [in] */ IIntent* intent);

        CARAPI ToString(
            /* [out] */ String* info)
        {
            VALIDATE_NOT_NULL(info);
            *info = String("CPowerManagerService::DreamReceiver: ");
            (*info).AppendFormat("%p", this);
            return NOERROR;
        }
    private:
        CPowerManagerService* mHost;
    };

    class UserSwitchedReceiver : public BroadcastReceiver
    {
    public:
        UserSwitchedReceiver(
            /* [in] */ CPowerManagerService* host);

        CARAPI OnReceive(
            /* [in] */ IContext* context,
            /* [in] */ IIntent* intent);

        CARAPI ToString(
            /* [out] */ String* info)
        {
            VALIDATE_NOT_NULL(info);
            *info = String("CPowerManagerService::UserSwitchedReceiver: ");
            (*info).AppendFormat("%p", this);
            return NOERROR;
        }
    private:
        CPowerManagerService* mHost;
    };

    class DockReceiver : public BroadcastReceiver
    {
    public:
        DockReceiver(
            /* [in] */ CPowerManagerService* host);

        CARAPI OnReceive(
            /* [in] */ IContext* context,
            /* [in] */ IIntent* intent);

        CARAPI ToString(
            /* [out] */ String* info)
        {
            VALIDATE_NOT_NULL(info);
            *info = String("CPowerManagerService::DockReceiver: ");
            (*info).AppendFormat("%p", this);
            return NOERROR;
        }
    private:
        CPowerManagerService* mHost;
    };

    class SettingsObserver : public ContentObserver
    {
    public:
        SettingsObserver(
            /* [in] */ IHandler* handler,
            /* [in] */ CPowerManagerService* host);

        CARAPI OnChange(
            /* [in] */ Boolean selfChange,
            /* [in] */ IUri* uri);

    private:
        CPowerManagerService* mHost;
    };

    /**
     * Represents a wake lock that has been acquired by an application.
     */
    class WakeLock
        : public ElRefBase
        , public IProxyDeathRecipient
    {
    public:
        WakeLock(
            /* [in] */ IBinder* lock,
            /* [in] */ Int32 flags,
            /* [in] */ const String& tag,
            /* [in] */ IWorkSource* workSource,
            /* [in] */ Int32 ownerUid,
            /* [in] */ Int32 ownerPid,
            /* [in] */ CPowerManagerService* host);

        CAR_INTERFACE_DECL();

        CARAPI ProxyDied();

        CARAPI_(Boolean) HasSameProperties(
            /* [in] */ Int32 flags,
            /* [in] */ const String& tag,
            /* [in] */ IWorkSource* workSource,
            /* [in] */ Int32 ownerUid,
            /* [in] */ Int32 ownerPid);

        CARAPI_(void) UpdateProperties(
            /* [in] */ Int32 flags,
            /* [in] */ const String& tag,
            /* [in] */ IWorkSource* workSource,
            /* [in] */ Int32 ownerUid,
            /* [in] */ Int32 ownerPid);

        CARAPI_(Boolean) HasSameWorkSource(
            /* [in] */ IWorkSource* workSource);

        CARAPI_(void) UpdateWorkSource(
            /* [in] */ IWorkSource* workSource);

        CARAPI_(String) ToString();

    private:
        CARAPI_(String) GetLockLevelString();
        CARAPI_(String) GetLockFlagsString();

    public:
         AutoPtr<IBinder> mLock;
         Int32  mFlags;
         String mTag;
         AutoPtr<IWorkSource> mWorkSource;
         Int32 mOwnerUid;
         Int32 mOwnerPid;

    private:
        CPowerManagerService* mHost;
    };

    class SuspendBlockerImpl
        : public ElRefBase
        , public ISuspendBlocker
    {
    public:
        SuspendBlockerImpl(
            /* [in] */ const String& name);

        ~SuspendBlockerImpl();

        CAR_INTERFACE_DECL();

        CARAPI Acquire();

        CARAPI Release();

        CARAPI BootFastSleepRelease();

        CARAPI BootFastWakeAcquire();

    private:
        String mName;
        Int32 mReferenceCount;
        Object mLock;
    };

    class ScreenOnBlockerImpl
        : public ElRefBase
        , public IScreenOnBlocker
    {
    public:
        ScreenOnBlockerImpl(
            /* [in] */ CPowerManagerService* host);

        CAR_INTERFACE_DECL();

        CARAPI_(Boolean) IsHeld();

        CARAPI Acquire();

        CARAPI Release();

    private:
        Int32 mNestCount;
        Object mLock;
        CPowerManagerService* mHost;
    };

    class DisplayBlankerImpl
        : public ElRefBase
        , public IDisplayBlanker
    {
    public:
        DisplayBlankerImpl(
            /* [in] */ CPowerManagerService* host);

        CAR_INTERFACE_DECL();

        CARAPI BlankAllDisplays();

        CARAPI UnblankAllDisplays();

    private:
        Boolean mBlanked;
        Object mLock;
        CPowerManagerService* mHost;
    };

    class ShutdownOrRebootRunnable
        : public Runnable
        , public Object
    {
    public:
        ShutdownOrRebootRunnable(
            /* [in] */ Boolean shutdown,
            /* [in] */ Boolean confirm,
            /* [in] */ const String& reason,
            /* [in] */ IContext* ctx);

        CARAPI Run();

    private:
        Boolean mShutdown;
        Boolean mConfirm;
        String mReason;
        AutoPtr<IContext> mContext;
    };

    class CrashThread
        : public ThreadBase
    {
    public:
        CrashThread(
            /* [in] */ const String& s);

        virtual CARAPI Run();
    };

public:
    CPowerManagerService();

    /**
     * Initialize the power manager.
     * Must be called before any other functions within the power manager are called.
     */
    CARAPI_(void) Init(
        /* [in] */ IContext* context,
        /* [in] */ LightsService* ls,
        /* [in] */ CActivityManagerService* am,
        /* [in] */ CBatteryService* bs,
        /* [in] */ IIBatteryStats* bss,
        /* [in] */ CDisplayManagerService* dm);

    CARAPI_(void) SetPolicy(
        /* [in] */ IWindowManagerPolicy* policy);

    CARAPI SystemReady(
        /* [in] */ TwilightService* twilight);
        ///* [in] */ CDreamManagerService* dreamManager);

    CARAPI AcquireWakeLock(
        /* [in] */ IBinder* lock,
        /* [in] */ Int32 flags,
        /* [in] */ const String& tag,
        /* [in] */ IWorkSource* ws);

    CARAPI ReleaseWakeLock(
        /* [in] */ IBinder* lock,
        /* [in] */ Int32 flags);

    CARAPI UpdateWakeLockWorkSource(
        /* [in] */ IBinder* lock,
        /* [in] */ IWorkSource* ws);

    CARAPI IsWakeLockLevelSupported(
        /* [in] */ Int32 level,
        /* [out] */ Boolean* res);

    CARAPI UserActivity(
        /* [in] */ Int64 eventTime,
        /* [in] */ Int32 event,
        /* [in] */ Int32 flags);

    CARAPI WakeUp(
        /* [in] */ Int64 eventTime);

    CARAPI GoToSleep(
        /* [in] */ Int64 eventTime,
        /* [in] */ Int32 reason);

    CARAPI Nap(
        /* [in] */ Int64 eventTime);

    //@Override // Binder call
    CARAPI IsScreenOn(
        /* [out] */ Boolean* isScreenOn);

    /**
     * Reboots the device.
     *
     * @param confirm If true, shows a reboot confirmation dialog.
     * @param reason The reason for the reboot, or null if none.
     * @param wait If true, this call waits for the reboot to complete and does not return.
     */
    //@Override // Binder call
    CARAPI Reboot(
        /* [in] */ Boolean confirm,
        /* [in] */ const String& reason,
        /* [in] */ Boolean wait);

    /**
     * Shuts down the device.
     *
     * @param confirm If true, shows a shutdown confirmation dialog.
     * @param wait If true, this call waits for the shutdown to complete and does not return.
     */
    //@Override // Binder call
    CARAPI Shutdown(
        /* [in] */ Boolean confirm,
        /* [in] */ Boolean wait);

    /**
     * Crash the runtime (causing a complete restart of the Android framework).
     * Requires REBOOT permission.  Mostly for testing.  Should not return.
     */
    //@Override // Binder call
    CARAPI Crash(
        /* [in] */ const String& message);

    /**
     * Set the setting that determines whether the device stays on when plugged in.
     * The argument is a bit string, with each bit specifying a power source that,
     * when the device is connected to that source, causes the device to stay on.
     * See {@link android.os.BatteryManager} for the list of power sources that
     * can be specified. Current values include {@link android.os.BatteryManager#BATTERY_PLUGGED_AC}
     * and {@link android.os.BatteryManager#BATTERY_PLUGGED_USB}
     *
     * Used by "adb shell svc power stayon ..."
     *
     * @param val an {@code int} containing the bits that specify which power sources
     * should cause the device to stay on.
     */
    //@Override // Binder call
    CARAPI SetStayOnSetting(
        /* [in] */ Int32 val);

    /**
     * Used by device administration to set the maximum screen off timeout.
     *
     * This method must only be called by the device administration policy manager.
     */
    //@Override // Binder call
    CARAPI SetMaximumScreenOffTimeoutFromDeviceAdmin(
        /* [in] */ Int32 timeMs);

    /**
     * Used by the phone application to make the attention LED flash when ringing.
     */
    //@Override // Binder call
    CARAPI SetAttentionLight(
        /* [in] */ Boolean on,
        /* [in] */ Int32 color);

    CARAPI GoToBootFastSleep(
        /* [in] */ Int64 time);

    CARAPI BootFastWake(
        /* [in] */ Int64 time);

    CARAPI IsBootFastStatus(
        /* [out] */ Boolean* result);

    CARAPI IsBootFastWakeFromStandby(
        /* [out] */ Boolean* result);

    /**
     * Used by the Watchdog.
     */
    CARAPI TimeSinceScreenWasLastOn(
        /* [out] */ Int64* time);

    /**
     * Used by the window manager to override the screen brightness based on the
     * current foreground activity.
     *
     * This method must only be called by the window manager.
     *
     * @param brightness The overridden brightness, or -1 to disable the override.
     */
    CARAPI SetScreenBrightnessOverrideFromWindowManager(
        /* [in] */ Int32 brightness);

    /**
     * Used by the window manager to override the button brightness based on the
     * current foreground activity.
     *
     * This method must only be called by the window manager.
     *
     * @param brightness The overridden brightness, or -1 to disable the override.
     */
    CARAPI SetButtonBrightnessOverrideFromWindowManager(
        /* [in] */ Int32 brightness);

    /**
     * Used by the window manager to override the user activity timeout based on the
     * current foreground activity.  It can only be used to make the timeout shorter
     * than usual, not longer.
     *
     * This method must only be called by the window manager.
     *
     * @param timeoutMillis The overridden timeout, or -1 to disable the override.
     */
    CARAPI SetUserActivityTimeoutOverrideFromWindowManager(
        /* [in] */ Int64 timeoutMillis);

    /**
     * Used by the settings application and brightness control widgets to
     * temporarily override the current screen brightness setting so that the
     * user can observe the effect of an intended settings change without applying
     * it immediately.
     *
     * The override will be canceled when the setting value is next updated.
     *
     * @param brightness The overridden brightness.
     *
     * @see Settings.System#SCREEN_BRIGHTNESS
     */
    //@Override // Binder call
    CARAPI SetTemporaryScreenBrightnessSettingOverride(
        /* [in] */ Int32 brightness);

    /**
     * Used by the settings application and brightness control widgets to
     * temporarily override the current screen auto-brightness adjustment setting so that the
     * user can observe the effect of an intended settings change without applying
     * it immediately.
     *
     * The override will be canceled when the setting value is next updated.
     *
     * @param adj The overridden brightness, or Float.NaN to disable the override.
     *
     * @see Settings.System#SCREEN_AUTO_BRIGHTNESS_ADJ
     */
    //@Override // Binder call
    CARAPI SetTemporaryScreenAutoBrightnessAdjustmentSettingOverride(
        /* [in] */ Float adj);

    /**
     * Low-level function turn the device off immediately, without trying
     * to be clean.  Most people should use {@link ShutdownThread} for a clean shutdown.
     */
    static CARAPI_(void) LowLevelShutdown();

    /**
     * Low-level function to reboot the device.
     *
     * @param reason code to pass to the kernel (e.g. "recovery"), or null.
     * @throws IOException if reboot fails for some reason (eg, lack of
     *         permission)
     */
    static CARAPI LowLevelReboot(
        /* [in] */ const String& reason);

    //@Override // Watchdog.Monitor implementation
    // CARAPI Monitor();

    CARAPI constructor();

    // method for NativeInputManager
    static CARAPI_(bool) isScreenOn();

    static CARAPI_(bool) isScreenBright();

    static CARAPI_(bool) isBootFastStatus();

    static CARAPI_(bool) isPowered();

    static CARAPI_(void) tempWakeuUp(
        /* [in] */ nsecs_t eventTime);

    static CARAPI_(void) userActivity(
        /* [in] */ nsecs_t eventTime,
        /* [in] */ int32_t eventType);

    static CARAPI_(void) wakeUp(
        /* [in] */ nsecs_t eventTime);

    static CARAPI_(void) goToSleep(
        /* [in] */ nsecs_t eventTime);

protected:
    // CARAPI_(void) Dump(
    //     /* [in] */ IFileDescriptor* fd,
    //     /* [in] */ IPrintWriter* pw,
    //     /* [in] */ const ArrayOf<String>& args);

private:
    CARAPI_(void) NativeInit();

    static CARAPI_(void) NativeShutdown();

    static CARAPI NativeReboot(
        /* [in] */ const String& reason);

    static CARAPI_(void) NativeSetPowerState(
        /* [in] */ Boolean screenOn,
        /* [in] */ Boolean screenBright);

    static CARAPI_(void) NativeAcquireSuspendBlocker(
        /* [in] */ const String& name);

    static CARAPI_(void) NativeReleaseSuspendBlocker(
        /* [in] */ const String& name);

    static CARAPI_(void) NativeSetInteractive(
        /* [in] */ Boolean enable);

    static CARAPI_(void) NativeSetAutoSuspend(
        /* [in] */ Boolean enable);

    static CARAPI_(void) NativeGoToBootFastSleep();

    CARAPI_(void) ReadConfigurationLocked();

    CARAPI_(void) UpdateSettingsLocked();

    CARAPI_(void) HandleSettingsChangedLocked();

    CARAPI AcquireWakeLockInternal(
        /* [in] */ IBinder* lock,
        /* [in] */ Int32 flags,
        /* [in] */ const String& tag,
        /* [in] */ IWorkSource* ws,
        /* [in] */ Int32 uid,
        /* [in] */ Int32 pid);

    static CARAPI_(Boolean) IsScreenLock(
        /* [in] */ WakeLock* wakeLock);

    CARAPI_(void) ApplyWakeLockFlagsOnAcquireLocked(
        /* [in] */ WakeLock* wakeLock);

    CARAPI_(void) ReleaseWakeLockInternal(
        /* [in] */ IBinder* lock,
        /* [in] */ Int32 flags);

    CARAPI_(void) HandleWakeLockDeath(
        /* [in] */ WakeLock* wakeLock);

    CARAPI_(void) ApplyWakeLockFlagsOnReleaseLocked(
        /* [in] */ WakeLock* wakeLock);

    CARAPI UpdateWakeLockWorkSourceInternal(
        /* [in] */ IBinder* lock,
        /* [in] */ IWorkSource* ws);

    CARAPI_(List< AutoPtr<WakeLock> >::Iterator) FindWakeLockIndexLocked(
        /* [in] */ IBinder* lock);

    CARAPI_(void) NotifyWakeLockAcquiredLocked(
        /* [in] */ WakeLock* wakeLock);

    CARAPI_(void) NotifyWakeLockReleasedLocked(
        /* [in] */ WakeLock* wakeLock);

    CARAPI_(Boolean) IsWakeLockLevelSupportedInternal(
        /* [in] */ Int32 level);

    CARAPI_(void) UserActivityFromNative(
        /* [in] */ Int64 eventTime,
        /* [in] */ Int32 event,
        /* [in] */ Int32 flags);

    CARAPI_(void) UserActivityInternal(
        /* [in] */ Int64 eventTime,
        /* [in] */ Int32 event,
        /* [in] */ Int32 flags,
        /* [in] */ Int32 uid);

    CARAPI_(Boolean) UserActivityNoUpdateLocked(
        /* [in] */ Int64 eventTime,
        /* [in] */ Int32 event,
        /* [in] */ Int32 flags,
        /* [in] */ Int32 uid);

    // Called from native code.
    CARAPI_(void) WakeUpFromNative(
        /* [in] */ Int64 eventTime);

    CARAPI_(void) WakeUpInternal(
        /* [in] */ Int64 eventTime);

    CARAPI_(Boolean) WakeUpNoUpdateLocked(
        /* [in] */ Int64 eventTime);

    CARAPI_(void) GoToBootFastSleepInternal(
        /* [in] */ Int64 eventTime);

    CARAPI_(Boolean) GoToBootFastSleepNoUpdateLocked(
        /* [in] */ Int64 eventTime);

    CARAPI_(void) BootFastWakeInternal(
        /* [in] */ Int64 eventTime);

    CARAPI_(Boolean) BootFastWakeNoUpdateLocked(
        /* [in] */ Int64 eventTime);

    CARAPI_(void) HandleTempWakupResleep();

    CARAPI_(void) TempWakeUp();

    CARAPI_(void) TempWakeUpFromNative(
        /* [in] */ Int64 eventTime);

    // Called from native code.
    CARAPI_(void) GoToSleepFromNative(
        /* [in] */ Int64 eventTime,
        /* [in] */ Int32 reason);

    CARAPI_(void) GoToSleepInternal(
        /* [in] */ Int64 eventTime,
        /* [in] */ Int32 reason);

    CARAPI_(void) ChanceLedStatus(
        /* [in] */ Int32 status);

    CARAPI_(Boolean) GoToSleepNoUpdateLocked(
        /* [in] */ Int64 eventTime,
        /* [in] */ Int32 reason);

    CARAPI_(void) NapInternal(
        /* [in] */ Int64 eventTime);

    CARAPI_(Boolean) NapNoUpdateLocked(
        /* [in] */ Int64 eventTime);

    /**
     * Updates the global power state based on dirty bits recorded in mDirty.
     *
     * This is the main function that performs power state transitions.
     * We centralize them here so that we can recompute the power state completely
     * each time something important changes, and ensure that we do it the same
     * way each time.  The point is to gather all of the transition logic here.
     */
    CARAPI_(void) UpdatePowerStateLocked();

    CARAPI_(void) SendPendingNotificationsLocked();

    /**
      * Updates the value of mIsPowered.
      * Sets DIRTY_IS_POWERED if a change occurred.
      */
    CARAPI_(void) UpdateIsPoweredLocked(
        /* [in] */ Int32 dirty);

    CARAPI_(Boolean) ShouldWakeUpWhenPluggedOrUnpluggedLocked(
        /* [in] */ Boolean wasPowered,
        /* [in] */ Int32 oldPlugType,
        /* [in] */ Boolean dockedOnWirelessCharger);

    /**
     * Updates the value of mStayOn.
     * Sets DIRTY_STAY_ON if a change occurred.
     */
    CARAPI_(void) UpdateStayOnLocked(
        /* [in] */ Int32 dirty);

    /**
     * Updates the value of mWakeLockSummary to summarize the state of all active wake locks.
     * Note that most wake-locks are ignored when the system is asleep.
     *
     * This function must have no other side-effects.
     */
    CARAPI_(void) UpdateWakeLockSummaryLocked(
        /* [in] */ Int32 dirty);

    /**
     * Updates the value of mUserActivitySummary to summarize the user requested
     * state of the system such as whether the screen should be bright or dim.
     * Note that user activity is ignored when the system is asleep.
     *
     * This function must have no other side-effects.
     */
    CARAPI_(void) UpdateUserActivitySummaryLocked(
        /* [in] */ Int64 now,
        /* [in] */ Int32 dirty);

    /**
     * Called when a user activity timeout has occurred.
     * Simply indicates that something about user activity has changed so that the new
     * state can be recomputed when the power state is updated.
     *
     * This function must have no other side-effects besides setting the dirty
     * bit and calling update power state.  Wakefulness transitions are handled elsewhere.
     */
    CARAPI_(void) HandleUserActivityTimeout();

    CARAPI_(Int32) GetScreenOffTimeoutLocked();

    CARAPI_(Int32) GetScreenDimDurationLocked(
        /* [in] */ Int32 screenOffTimeout);

    /**
     * Updates the wakefulness of the device.
     *
     * This is the function that decides whether the device should start napping
     * based on the current wake locks and user activity state.  It may modify mDirty
     * if the wakefulness changes.
     *
     * Returns true if the wakefulness changed and we need to restart power state calculation.
     */
    CARAPI_(Boolean) UpdateWakefulnessLocked(
        /* [in] */ Int32 dirty);

    /**
     * Returns true if the device should automatically nap and start dreaming when the user
     * activity timeout has expired and it's bedtime.
     */
    CARAPI_(Boolean) ShouldNapAtBedTimeLocked();

    /**
     * Returns true if the device should go to sleep now.
     * Also used when exiting a dream to determine whether we should go back
     * to being fully awake or else go to sleep for good.
     */
    CARAPI_(Boolean) IsItBedTimeYetLocked();

    /**
     * Returns true if the device is being kept awake by a wake lock, user activity
     * or the stay on while powered setting.
     */
    CARAPI_(Boolean) IsBeingKeptAwakeLocked();

    /**
     * Determines whether to post a message to the sandman to update the dream state.
     */
    CARAPI_(void) UpdateDreamLocked(
            /* [in] */ Int32 dirty);

    CARAPI_(void) ScheduleSandmanLocked();

    /**
     * Called when the device enters or exits a napping or dreaming state.
     *
     * We do this asynchronously because we must call out of the power manager to start
     * the dream and we don't want to hold our lock while doing so.  There is a risk that
     * the device will wake or go to sleep in the meantime so we have to handle that case.
     */
    CARAPI_(void) HandleSandman(); // runs on handler thread

    /**
     * Returns true if the device is allowed to dream in its current state
     * assuming that it is currently napping or dreaming.
     */
    CARAPI_(Boolean) CanDreamLocked();

    /**
     * Called when a dream is ending to figure out what to do next.
     */
    CARAPI_(void) HandleDreamFinishedLocked();

    CARAPI_(void) HandleScreenOnBlockerReleased();

    /**
     * Updates the display power state asynchronously.
     * When the update is finished, mDisplayReady will be set to true.  The display
     * controller posts a message to tell us when the actual display power state
     * has been updated so we come back here to double-check and finish up.
     *
     * This function recalculates the display power state each time.
     */
    CARAPI_(void) UpdateDisplayPowerStateLocked(
        /* [in] */ Int32 dirty);

    static CARAPI_(Boolean) IsValidBrightness(
        /* [in] */ Int32 value);

    static CARAPI_(Boolean) IsValidAutoBrightnessAdjustment(
        /* [in] */ Float value);

    CARAPI_(Int32) GetDesiredScreenPowerStateLocked();

    CARAPI_(Boolean) ShouldUseProximitySensorLocked();

    /**
     * Updates the suspend blocker that keeps the CPU alive.
     *
     * This function must have no other side-effects.
     */
    CARAPI_(void) UpdateSuspendBlockerLocked();

    CARAPI_(void) UpdateBootFastSuspendBlockerLocked();

    CARAPI_(Boolean) IsCpuNeededLocked();

    CARAPI_(Boolean) IsScreenOnInternal();

    CARAPI_(void) HandleBatteryStateChangedLocked();

    CARAPI_(void) StartWatchingForBootAnimationFinished();

    CARAPI_(void) CheckIfBootAnimationFinished();

    CARAPI_(void) HandleBootCompletedLocked();

    CARAPI ShutdownOrRebootInternal(
        /* [in] */ Boolean shutdown,
        /* [in] */ Boolean confirm,
        /* [in] */ const String& reason,
        /* [in] */ Boolean wait);

    CARAPI CrashInternal(
        /* [in] */ const String& message);

    CARAPI_(void) SetStayOnSettingInternal(
        /* [in] */ Int32 val);

    CARAPI_(void) SetMaximumScreenOffTimeoutFromDeviceAdminInternal(
        /* [in] */ Int32 timeMs);

    CARAPI_(Boolean) IsMaximumScreenOffTimeoutFromDeviceAdminEnforcedLocked();

    CARAPI_(void) SetAttentionLightInternal(
        /* [in] */ Boolean on,
        /* [in] */ Int32 color);

    CARAPI_(void) SetScreenBrightnessOverrideFromWindowManagerInternal(
        /* [in] */ Int32 brightness);

    CARAPI_(void) SetUserActivityTimeoutOverrideFromWindowManagerInternal(
        /* [in] */ Int64 timeoutMillis);

    CARAPI_(void) SetTemporaryScreenBrightnessSettingOverrideInternal(
        /* [in] */ Int32 brightness);

    CARAPI_(void) SetTemporaryScreenAutoBrightnessAdjustmentSettingOverrideInternal(
        /* [in] */ Float adj);

    CARAPI_(AutoPtr<ISuspendBlocker>) CreateSuspendBlockerLocked(
        /* [in] */ const String& name);

    static CARAPI_(String) WakefulnessToString(
        /* [in] */ Int32 wakefulness);

    static CARAPI_(AutoPtr<IWorkSource>) CopyWorkSource(
        /* [in] */ IWorkSource* workSource);

private:
    static const String TAG;

    static const Boolean DEBUG;
    static const Boolean DEBUG_SPEW;
    static const Boolean DEBUG_BOOTFAST;

    // Message: Sent when a user activity timeout occurs to update the power state.
    static const Int32 MSG_USER_ACTIVITY_TIMEOUT;// = 1;
    // Message: Sent when the device enters or exits a napping or dreaming state.
    static const Int32 MSG_SANDMAN;// = 2;
    // Message: Sent when the screen on blocker is released.
    static const Int32 MSG_SCREEN_ON_BLOCKER_RELEASED;// = 3;
    // Message: Sent to poll whether the boot animation has terminated.
    static const Int32 MSG_CHECK_IF_BOOT_ANIMATION_FINISHED;// = 4;

    static const Int32 MSG_TEMP_WAKUP_RESLEEP;// = 5;

    // Dirty bit: mWakeLocks changed
    static const Int32 DIRTY_WAKE_LOCKS;
    // Dirty bit: mWakefulness changed
    static const Int32 DIRTY_WAKEFULNESS;
    // Dirty bit: user activity was poked or may have timed out
    static const Int32 DIRTY_USER_ACTIVITY;
    // Dirty bit: actual display power state was updated asynchronously
    static const Int32 DIRTY_ACTUAL_DISPLAY_POWER_STATE_UPDATED;
    // Dirty bit: mBootCompleted changed
    static const Int32 DIRTY_BOOT_COMPLETED;
    // Dirty bit: settings changed
    static const Int32 DIRTY_SETTINGS;
    // Dirty bit: mIsPowered changed
    static const Int32 DIRTY_IS_POWERED;
    // Dirty bit: mStayOn changed
    static const Int32 DIRTY_STAY_ON;
    // Dirty bit: battery state changed
    static const Int32 DIRTY_BATTERY_STATE;
    // Dirty bit: proximity state changed
    static const Int32 DIRTY_PROXIMITY_POSITIVE;
    // Dirty bit: screen on blocker state became held or unheld
    static const Int32 DIRTY_SCREEN_ON_BLOCKER_RELEASED;
    // Dirty bit: dock state changed
    static const Int32 DIRTY_DOCK_STATE;

    // Wakefulness: The device is asleep and can only be awoken by a call to wakeUp().
    // The screen should be off or in the process of being turned off by the display controller.
    static const Int32 WAKEFULNESS_ASLEEP;
    // Wakefulness: The device is fully awake.  It can be put to sleep by a call to goToSleep().
    // When the user activity timeout expires, the device may start napping or go to sleep.
    static const Int32 WAKEFULNESS_AWAKE;
    // Wakefulness: The device is napping.  It is deciding whether to dream or go to sleep
    // but hasn't gotten around to it yet.  It can be awoken by a call to wakeUp(), which
    // ends the nap. User activity may brighten the screen but does not end the nap.
    static const Int32 WAKEFULNESS_NAPPING;
    // Wakefulness: The device is dreaming.  It can be awoken by a call to wakeUp(),
    // which ends the dream.  The device goes to sleep when goToSleep() is called, when
    // the dream ends or when unplugged.
    // User activity may brighten the screen but does not end the dream.
    static const Int32 WAKEFULNESS_DREAMING;

    //Wakefulnes: The device is not shutdown but deep sleep , pass long key wake up and show boot
    //logo and boot animation,we call it boot fast
    static const Int32 WAKEFULNESS_BOOTFAST;

    // Summarizes the state of all active wakelocks.
    static const Int32 WAKE_LOCK_CPU;
    static const Int32 WAKE_LOCK_SCREEN_BRIGHT;
    static const Int32 WAKE_LOCK_SCREEN_DIM;
    static const Int32 WAKE_LOCK_BUTTON_BRIGHT;
    static const Int32 WAKE_LOCK_PROXIMITY_SCREEN_OFF;
    static const Int32 WAKE_LOCK_STAY_AWAKE; // only set if already awake

    // Summarizes the user activity state.
    static const Int32 USER_ACTIVITY_SCREEN_BRIGHT;
    static const Int32 USER_ACTIVITY_SCREEN_DIM;

    // Default and minimum screen off timeout in milliseconds.
    static const Int32 DEFAULT_SCREEN_OFF_TIMEOUT;
    static const Int32 MINIMUM_SCREEN_OFF_TIMEOUT;

    // The screen dim duration, in milliseconds.
    // This is subtracted from the end of the screen off timeout so the
    // minimum screen off timeout should be longer than this.
    static const Int32 SCREEN_DIM_DURATION;

    // The maximum screen dim time expressed as a ratio relative to the screen
    // off timeout.  If the screen off timeout is very short then we want the
    // dim timeout to also be quite short so that most of the time is spent on.
    // Otherwise the user won't get much screen on time before dimming occurs.
    static const Float MAXIMUM_SCREEN_DIM_RATIO;

    // The name of the boot animation service in init.rc.
    static const String BOOT_ANIMATION_SERVICE;

    // Poll interval in milliseconds for watching boot animation finished.
    static const Int32 BOOT_ANIMATION_POLL_INTERVAL;

    // If the battery level drops by this percentage and the user activity timeout
    // has expired, then assume the device is receiving insufficient current to charge
    // effectively and terminate the dream.
    static const Int32 DREAM_BATTERY_LEVEL_DRAIN_CUTOFF;

    AutoPtr<IContext> mContext;
    AutoPtr<LightsService> mLightsService;
    AutoPtr<CBatteryService> mBatteryService;
    AutoPtr<CDisplayManagerService> mDisplayManagerService;
    AutoPtr<IIBatteryStats> mBatteryStats;
    AutoPtr<IHandlerThread> mHandlerThread;
    AutoPtr<PowerManagerHandler> mHandler;
    AutoPtr<IWindowManagerPolicy> mPolicy;
    AutoPtr<Notifier> mNotifier;
    AutoPtr<DisplayPowerController> mDisplayPowerController;
    AutoPtr<WirelessChargerDetector> mWirelessChargerDetector;
    AutoPtr<SettingsObserver> mSettingsObserver;
    // AutoPtr<CDreamManagerService> mDreamManager;
    AutoPtr<LightsService::Light> mAttentionLight;

    // A bitfield that indicates what parts of the power state have
    // changed and need to be recalculated.
    Int32 mDirty;

    // Indicates whether the device is awake or asleep or somewhere in between.
    // This is distinct from the screen power state, which is managed separately.
    Int32 mWakefulness;

    // True if MSG_SANDMAN has been scheduled.
    Boolean mSandmanScheduled;

    // Table of all suspend blockers.
    // There should only be a few of these.
    List< AutoPtr<ISuspendBlocker> > mSuspendBlockers;

    // Table of all wake locks acquired by applications.
    List< AutoPtr<WakeLock> > mWakeLocks;

    // A bitfield that summarizes the state of all active wakelocks.
    Int32 mWakeLockSummary;

    // If true, instructs the display controller to wait for the proximity sensor to
    // go negative before turning the screen on.
    Boolean mRequestWaitForNegativeProximity;

    // Timestamp of the last time the device was awoken or put to sleep.
    Int64 mLastWakeTime;
    Int64 mLastSleepTime;

    // True if we need to send a wake up or go to sleep finished notification
    // when the display is ready.
    Boolean mSendWakeUpFinishedNotificationWhenReady;
    Boolean mSendGoToSleepFinishedNotificationWhenReady;

    // Timestamp of the last call to user activity.
    Int64 mLastUserActivityTime;
    Int64 mLastUserActivityTimeNoChangeLights;

    // A bitfield that summarizes the effect of the user activity timer.
    // A zero value indicates that the user activity timer has expired.
    Int32 mUserActivitySummary;

    // The desired display power state.  The actual state may lag behind the
    // requested because it is updated asynchronously by the display power controller.
    AutoPtr<DisplayPowerRequest> mDisplayPowerRequest;

    // The time the screen was last turned off, in elapsedRealtime() timebase.
    Int64 mLastScreenOffEventElapsedRealTime;

    // True if the display power state has been fully applied, which means the display
    // is actually on or actually off or whatever was requested.
    Boolean mDisplayReady;

    // True if holding a wake-lock to block suspend of the CPU.
    Boolean mHoldingWakeLockSuspendBlocker;

    // The suspend blocker used to keep the CPU alive when wake locks have been acquired.
    AutoPtr<ISuspendBlocker> mWakeLockSuspendBlocker;

    // The screen on blocker used to keep the screen from turning on while the lock
    // screen is coming up.
    AutoPtr<ScreenOnBlockerImpl> mScreenOnBlocker;

    // The display blanker used to turn the screen on or off.
    AutoPtr<DisplayBlankerImpl> mDisplayBlanker;

    // True if systemReady() has been called.
    Boolean mSystemReady;

    // True if boot completed occurred.  We keep the screen on until this happens.
    Boolean mBootCompleted;

    // True if the device is plugged into a power source.
    Boolean mIsPowered;

    // The current plug type, such as BatteryManager.BATTERY_PLUGGED_WIRELESS.
    Int32 mPlugType;

    // The current battery level percentage.
    Int32 mBatteryLevel;

    Boolean mHdmiPlugged;

    // The battery level percentage at the time the dream started.
    // This is used to terminate a dream and go to sleep if the battery is
    // draining faster than it is charging and the user activity timeout has expired.
    Int32 mBatteryLevelWhenDreamStarted;

    // The current dock state.
    Int32 mDockState;

    // True if the device should wake up when plugged or unplugged.
    Boolean mWakeUpWhenPluggedOrUnpluggedConfig;

    // True if dreams are supported on this device.
    Boolean mDreamsSupportedConfig;

    // Default value for dreams enabled
    Boolean mDreamsEnabledByDefaultConfig;

    // Default value for dreams activate-on-sleep
    Boolean mDreamsActivatedOnSleepByDefaultConfig;

    // Default value for dreams activate-on-dock
    Boolean mDreamsActivatedOnDockByDefaultConfig;

    // True if dreams are enabled by the user.
    Boolean mDreamsEnabledSetting;

    // True if dreams should be activated on sleep.
    Boolean mDreamsActivateOnSleepSetting;

    // True if dreams should be activated on dock.
    Boolean mDreamsActivateOnDockSetting;

    // The screen off timeout setting value in milliseconds.
    Int32 mScreenOffTimeoutSetting;

    // The maximum allowable screen off timeout according to the device
    // administration policy.  Overrides other settings.
    Int32 mMaximumScreenOffTimeoutFromDeviceAdmin;//Integer.MAX_VALUE;

    // The stay on while plugged in setting.
    // A bitfield of battery conditions under which to make the screen stay on.
    Int32 mStayOnWhilePluggedInSetting;

    // True if the device should stay on.
    Boolean mStayOn;

    // True if the proximity sensor reads a positive result.
    Boolean mProximityPositive;
    Boolean mBootFastStats;

    Int64 mBootFastSleepClockStart;

    Int64 mBootFastSleepTimeStart;

    Boolean mBootFastTempWakeStatus;

    Boolean mBootFastWakeLockToggle;

    // Screen brightness setting limits.
    Int32 mScreenBrightnessSettingMinimum;
    Int32 mScreenBrightnessSettingMaximum;
    Int32 mScreenBrightnessSettingDefault;

    // The screen brightness setting, from 0 to 255.
    // Use -1 if no value has been set.
    Int32 mScreenBrightnessSetting;

    // The screen auto-brightness adjustment setting, from -1 to 1.
    // Use 0 if there is no adjustment.
    Float mScreenAutoBrightnessAdjustmentSetting;

    // The screen brightness mode.
    // One of the Settings.System.SCREEN_BRIGHTNESS_MODE_* constants.
    Int32 mScreenBrightnessModeSetting;

    // The screen brightness setting override from the window manager
    // to allow the current foreground activity to override the brightness.
    // Use -1 to disable.
    Int32 mScreenBrightnessOverrideFromWindowManager;

    // The user activity timeout override from the window manager
    // to allow the current foreground activity to override the user activity timeout.
    // Use -1 to disable.
    Int64 mUserActivityTimeoutOverrideFromWindowManager;

    // The screen brightness setting override from the settings application
    // to temporarily adjust the brightness until next updated,
    // Use -1 to disable.
    Int32 mTemporaryScreenBrightnessSettingOverride;

    // The screen brightness adjustment setting override from the settings
    // application to temporarily adjust the auto-brightness adjustment factor
    // until next updated, in the range -1..1.
    // Use NaN to disable.
    Float mTemporaryScreenAutoBrightnessAdjustmentSettingOverride;

    // Time when we last logged a warning about calling userActivity() without permission.
    Int64 mLastWarningAboutUserActivityPermission;

    AutoPtr<DisplayPowerController::IDisplayPowerControllerCallbacks> mDisplayPowerControllerCallbacks;

    friend class MyDisplayPowerControllerCallbacks;
    friend class WakeLock;
    friend class SuspendBlockerImpl;
    friend class ScreenOnBlockerImpl;
    friend class DisplayBlankerImpl;
    friend class HdmiPluggedReceiver;
};

} // namespace Power
} // namespace Server
} // namespace Droid
} // namespace Elastos

#endif // __ELASTOS_DROID_SERVER_POWER_CPOWERMANAGERSERVICE_H__
