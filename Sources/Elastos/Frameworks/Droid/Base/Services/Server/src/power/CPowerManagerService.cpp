
#include "power/CPowerManagerService.h"
#include "power/ShutdownThread.h"
#include "os/SystemClock.h"
#include "os/Binder.h"
#include "os/Looper.h"
#include "R.h"
#include "Manifest.h"
#include <elastos/Slogger.h>
#include <elastos/Math.h>
#include <hardware/power.h>
#include <hardware_legacy/power.h>
#include <utils/Log.h>
#include <utils/Errors.h>
#include <cutils/android_reboot.h>
#include <suspend/autosuspend.h>
#include <fcntl.h>
#include <elastos/StringUtils.h>

using Elastos::Core::StringUtils;
using android::LogIfSlow;
using Elastos::Utility::Logging::Slogger;
using Elastos::Core::EIID_IRunnable;
using Elastos::IO::CFile;
using Elastos::IO::IFile;
using Elastos::IO::CFileReader;
using Elastos::IO::IFileReader;
using Elastos::IO::CBufferedReader;
using Elastos::IO::IBufferedReader;
using Elastos::IO::ICloseable;
using Elastos::IO::EIID_ICloseable;
using Elastos::Droid::Os::SystemClock;
using Elastos::Droid::Os::CHandlerThread;
using Elastos::Droid::Os::CHandler;
using Elastos::Droid::Os::CHandlerThread;
using Elastos::Droid::Os::IUserHandle;
using Elastos::Droid::Os::IPowerManagerHelper;
using Elastos::Droid::Os::Binder;
using Elastos::Droid::Os::ISystemProperties;
using Elastos::Droid::Os::CSystemProperties;
using Elastos::Droid::Os::CWorkSource;
using Elastos::Droid::Os::CPowerManagerHelper;
using Elastos::Droid::Provider::ISettingsSecure;
using Elastos::Droid::Provider::CSettingsSecure;
using Elastos::Droid::Provider::ISettingsGlobal;
using Elastos::Droid::Provider::CSettingsGlobal;
using Elastos::Droid::Provider::ISettingsSystem;
using Elastos::Droid::Provider::CSettingsSystem;
using Elastos::Droid::Database::IContentObserver;
using Elastos::Droid::Content::CIntentFilter;
using Elastos::Droid::Content::Pm::IPackageManager;
using Elastos::Droid::Hardware::CSystemSensorManager;
using Elastos::Droid::Hardware::ISystemSensorManager;
using Elastos::Droid::Media::IMediaPlayerHelper;
using Elastos::Droid::Media::CMediaPlayerHelper;

namespace Elastos {
namespace Droid {
namespace Server {
namespace Power {

static struct power_module* gPowerModule = NULL;
static Mutex gPowerManagerLock;
static Boolean gScreenOn = FALSE;
static Boolean gScreenBright = FALSE;

const String CPowerManagerService::TAG("PowerManagerService");
const Boolean CPowerManagerService::DEBUG = FALSE;
const Boolean CPowerManagerService::DEBUG_SPEW = DEBUG && FALSE;
const Boolean CPowerManagerService::DEBUG_BOOTFAST = FALSE;
const Int32 CPowerManagerService::DIRTY_WAKE_LOCKS = 1 << 0;
const Int32 CPowerManagerService::DIRTY_WAKEFULNESS = 1 << 1;
const Int32 CPowerManagerService::DIRTY_USER_ACTIVITY = 1 << 2;
const Int32 CPowerManagerService::DIRTY_ACTUAL_DISPLAY_POWER_STATE_UPDATED = 1 << 3;
const Int32 CPowerManagerService::DIRTY_BOOT_COMPLETED = 1 << 4;
const Int32 CPowerManagerService::DIRTY_SETTINGS = 1 << 5;
const Int32 CPowerManagerService::DIRTY_IS_POWERED = 1 << 6;
const Int32 CPowerManagerService::DIRTY_STAY_ON = 1 << 7;
const Int32 CPowerManagerService::DIRTY_BATTERY_STATE = 1 << 8;
const Int32 CPowerManagerService::DIRTY_PROXIMITY_POSITIVE = 1 << 9;
const Int32 CPowerManagerService::DIRTY_SCREEN_ON_BLOCKER_RELEASED = 1 << 10;
const Int32 CPowerManagerService::DIRTY_DOCK_STATE = 1 << 11;
const Int32 CPowerManagerService::WAKEFULNESS_ASLEEP = 0;
const Int32 CPowerManagerService::WAKEFULNESS_AWAKE = 1;
const Int32 CPowerManagerService::WAKEFULNESS_NAPPING = 2;
const Int32 CPowerManagerService::WAKEFULNESS_DREAMING = 3;
const Int32 CPowerManagerService::WAKEFULNESS_BOOTFAST = 4;
const Int32 CPowerManagerService::WAKE_LOCK_CPU = 1 << 0;
const Int32 CPowerManagerService::WAKE_LOCK_SCREEN_BRIGHT = 1 << 1;
const Int32 CPowerManagerService::WAKE_LOCK_SCREEN_DIM = 1 << 2;
const Int32 CPowerManagerService::WAKE_LOCK_BUTTON_BRIGHT = 1 << 3;
const Int32 CPowerManagerService::WAKE_LOCK_PROXIMITY_SCREEN_OFF = 1 << 4;
const Int32 CPowerManagerService::WAKE_LOCK_STAY_AWAKE = 1 << 5;
const Int32 CPowerManagerService::USER_ACTIVITY_SCREEN_BRIGHT = 1 << 0;
const Int32 CPowerManagerService::USER_ACTIVITY_SCREEN_DIM = 1 << 1;
const Int32 CPowerManagerService::DEFAULT_SCREEN_OFF_TIMEOUT = 15 * 1000;
const Int32 CPowerManagerService::MINIMUM_SCREEN_OFF_TIMEOUT = 10 * 1000;
const Int32 CPowerManagerService::SCREEN_DIM_DURATION = 7 * 1000;
const Float CPowerManagerService::MAXIMUM_SCREEN_DIM_RATIO = 0.2f;
const String CPowerManagerService::BOOT_ANIMATION_SERVICE("bootanim");
const Int32 CPowerManagerService::BOOT_ANIMATION_POLL_INTERVAL = 200;
const Int32 CPowerManagerService::DREAM_BATTERY_LEVEL_DRAIN_CUTOFF = 5;
const Int32 CPowerManagerService::MSG_USER_ACTIVITY_TIMEOUT = 1;
const Int32 CPowerManagerService::MSG_SANDMAN = 2;
const Int32 CPowerManagerService::MSG_SCREEN_ON_BLOCKER_RELEASED = 3;
const Int32 CPowerManagerService::MSG_CHECK_IF_BOOT_ANIMATION_FINISHED = 4;
const Int32 CPowerManagerService::MSG_TEMP_WAKUP_RESLEEP = 5;

ECode CPowerManagerService::PowerManagerHandler::HandleMessage(
    /* [in] */ IMessage* msg)
{
    Int32 what;
    msg->GetWhat(&what);

    switch(what) {
        case CPowerManagerService::MSG_USER_ACTIVITY_TIMEOUT:
            mHost->HandleUserActivityTimeout();
            break;
        case CPowerManagerService::MSG_SANDMAN:
            mHost->HandleSandman();
            break;
        case CPowerManagerService::MSG_SCREEN_ON_BLOCKER_RELEASED:
            mHost->HandleScreenOnBlockerReleased();
            break;
        case CPowerManagerService::MSG_CHECK_IF_BOOT_ANIMATION_FINISHED:
            mHost->CheckIfBootAnimationFinished();
            break;
        case CPowerManagerService::MSG_TEMP_WAKUP_RESLEEP:
            mHost->HandleTempWakupResleep();
            break;
    }

    return NOERROR;
}

CPowerManagerService::HdmiPluggedReceiver::HdmiPluggedReceiver(
    /* [in] */ CPowerManagerService* host)
    : mHost(host)
{}

ECode CPowerManagerService::HdmiPluggedReceiver::OnReceive(
    /* [in] */ IContext* context,
    /* [in] */ IIntent* intent)
{
    Mutex::Autolock lock(mHost->_m_syncLock);
    Int32 wasPlugged = mHost->mHdmiPlugged ? 1 : 0;
    Int32 value = wasPlugged;
    intent->GetInt32Extra(String("hdmistatus")/*DisplayManagerAw::EXTRA_HDMISTATUS*/, wasPlugged, &value);
    mHost->mHdmiPlugged = value > 0 ? TRUE : FALSE;
    Slogger::E(TAG, "no CMediaPlayerHelper to get function setHdmiState!!!");
    // MediaPlayer.setHdmiState(mHdmiPlugged);
    return NOERROR;
}

CAR_INTERFACE_IMPL(CPowerManagerService::MyDisplayPowerControllerCallbacks, IInterface)

CPowerManagerService::MyDisplayPowerControllerCallbacks::MyDisplayPowerControllerCallbacks(
    /* [in] */ CPowerManagerService* host)
    : mHost(host)
{}

void CPowerManagerService::MyDisplayPowerControllerCallbacks::OnStateChanged()
{
    mHost->mDirty |= CPowerManagerService::DIRTY_ACTUAL_DISPLAY_POWER_STATE_UPDATED;
    mHost->UpdatePowerStateLocked();
}

void CPowerManagerService::MyDisplayPowerControllerCallbacks::OnProximityPositive()
{
    mHost->mProximityPositive = TRUE;
    mHost->mDirty |= CPowerManagerService::DIRTY_PROXIMITY_POSITIVE;
    mHost->UpdatePowerStateLocked();
}

void CPowerManagerService::MyDisplayPowerControllerCallbacks::OnProximityNegative()
{
    mHost->mProximityPositive = FALSE;
    mHost->mDirty |= CPowerManagerService::DIRTY_PROXIMITY_POSITIVE;
    mHost->UserActivityNoUpdateLocked(SystemClock::GetUptimeMillis(),
            IPowerManager::USER_ACTIVITY_EVENT_OTHER, 0, IProcess::SYSTEM_UID);
    mHost->UpdatePowerStateLocked();
}


CPowerManagerService::BatteryReceiver::BatteryReceiver(
    /* [in] */ CPowerManagerService* host)
    : mHost(host)
{}

ECode CPowerManagerService::BatteryReceiver::OnReceive(
    /* [in] */ IContext* context,
    /* [in] */ IIntent* intent)
{
    Mutex::Autolock lock(mHost->_m_syncLock);
    mHost->HandleBatteryStateChangedLocked();
    return NOERROR;
}


CPowerManagerService::BootCompletedReceiver::BootCompletedReceiver(
    /* [in] */ CPowerManagerService* host)
    : mHost(host)
{}

ECode CPowerManagerService::BootCompletedReceiver::OnReceive(
    /* [in] */ IContext* context,
    /* [in] */ IIntent* intent)
{
    // This is our early signal that the system thinks it has finished booting.
    // However, the boot animation may still be running for a few more seconds
    // since it is ultimately in charge of when it terminates.
    // Defer transitioning into the boot completed state until the animation exits.
    // We do this so that the screen does not start to dim prematurely before
    // the user has actually had a chance to interact with the device.
    mHost->StartWatchingForBootAnimationFinished();
    return NOERROR;
}


CPowerManagerService::DreamReceiver::DreamReceiver(
    /* [in] */ CPowerManagerService* host)
    : mHost(host)
{}

ECode CPowerManagerService::DreamReceiver::OnReceive(
    /* [in] */ IContext* context,
    /* [in] */ IIntent* intent)
{
    Mutex::Autolock lock(mHost->_m_syncLock);
    mHost->ScheduleSandmanLocked();
    return NOERROR;
}


CPowerManagerService::UserSwitchedReceiver::UserSwitchedReceiver(
    /* [in] */ CPowerManagerService* host)
    : mHost(host)
{}

ECode CPowerManagerService::UserSwitchedReceiver::OnReceive(
    /* [in] */ IContext* context,
    /* [in] */ IIntent* intent)
{
    Mutex::Autolock lock(mHost->_m_syncLock);
    mHost->HandleSettingsChangedLocked();
    return NOERROR;
}

CPowerManagerService::DockReceiver::DockReceiver(
    /* [in] */ CPowerManagerService* host)
    : mHost(host)
{}

ECode CPowerManagerService::DockReceiver::OnReceive(
    /* [in] */ IContext* context,
    /* [in] */ IIntent* intent)
{
    Mutex::Autolock lock(mHost->_m_syncLock);
    Int32 dockState;
    intent->GetInt32Extra(IIntent::EXTRA_DOCK_STATE,
            IIntent::EXTRA_DOCK_STATE_UNDOCKED, &dockState);
    if (mHost->mDockState != dockState) {
        mHost->mDockState = dockState;
        mHost->mDirty |= DIRTY_DOCK_STATE;
        mHost->UpdatePowerStateLocked();
    }

    return NOERROR;
}


CPowerManagerService::SettingsObserver::SettingsObserver(
    /* [in] */ IHandler* handler,
    /* [in] */ CPowerManagerService* host)
    : ContentObserver(handler)
    , mHost(host)
{}

ECode CPowerManagerService::SettingsObserver::OnChange(
    /* [in] */ Boolean selfChange,
    /* [in] */ IUri* uri)
{
    Mutex::Autolock lock(mHost->_m_syncLock);
    mHost->HandleSettingsChangedLocked();
    return NOERROR;
}


CPowerManagerService::WakeLock::WakeLock(
    /* [in] */ IBinder* lock,
    /* [in] */ Int32 flags,
    /* [in] */ const String& tag,
    /* [in] */ IWorkSource* workSource,
    /* [in] */ Int32 ownerUid,
    /* [in] */ Int32 ownerPid,
    /* [in] */ CPowerManagerService* host)
    : mLock(lock)
    , mFlags(flags)
    , mTag(tag)
    , mOwnerUid(ownerUid)
    , mOwnerPid(ownerPid)
    , mHost(host)
{
    mWorkSource = CPowerManagerService::CopyWorkSource(workSource);
}

CAR_INTERFACE_IMPL(CPowerManagerService::WakeLock, IProxyDeathRecipient);

ECode CPowerManagerService::WakeLock::ProxyDied()
{
    PFL_EX("ProxyDied()");
    mHost->HandleWakeLockDeath(this);
    return NOERROR;
}

Boolean CPowerManagerService::WakeLock::HasSameProperties(
    /* [in] */ Int32 flags,
    /* [in] */ const String& tag,
    /* [in] */ IWorkSource* workSource,
    /* [in] */ Int32 ownerUid,
    /* [in] */ Int32 ownerPid)
{
    return mFlags == flags
            && mTag.Equals(tag)
            && HasSameWorkSource(workSource)
            && mOwnerUid == ownerUid
            && mOwnerPid == ownerPid;
}

void CPowerManagerService::WakeLock::UpdateProperties(
    /* [in] */ Int32 flags,
    /* [in] */ const String& tag,
    /* [in] */ IWorkSource* workSource,
    /* [in] */ Int32 ownerUid,
    /* [in] */ Int32 ownerPid)
{
    mFlags = flags;
    mTag = tag;
    UpdateWorkSource(workSource);
    mOwnerUid = ownerUid;
    mOwnerPid = ownerPid;
}

Boolean CPowerManagerService::WakeLock::HasSameWorkSource(
    /* [in] */ IWorkSource* workSource)
{
    return mWorkSource.Get() == workSource;
}

void CPowerManagerService::WakeLock::UpdateWorkSource(
    /* [in] */ IWorkSource* workSource)
{
    mWorkSource = CPowerManagerService::CopyWorkSource(workSource);
}

String CPowerManagerService::WakeLock::ToString()
{
    return GetLockLevelString()
            + String(" '") + mTag + String("'") + GetLockFlagsString()
            + String(" (uid=") + StringUtils::Int32ToString(mOwnerUid) + String(", pid=")
            + StringUtils::Int32ToString(mOwnerPid) + String(", ws=") /*+ mWorkSource */+ String(")");
}

String CPowerManagerService::WakeLock::GetLockLevelString()
{
    switch (mFlags & IPowerManager::WAKE_LOCK_LEVEL_MASK) {
        case IPowerManager::FULL_WAKE_LOCK:
            return String("FULL_WAKE_LOCK                ");
        case IPowerManager::SCREEN_BRIGHT_WAKE_LOCK:
            return String("SCREEN_BRIGHT_WAKE_LOCK       ");
        case IPowerManager::SCREEN_DIM_WAKE_LOCK:
            return String("SCREEN_DIM_WAKE_LOCK          ");
        case IPowerManager::PARTIAL_WAKE_LOCK:
            return String("PARTIAL_WAKE_LOCK             ");
        case IPowerManager::PROXIMITY_SCREEN_OFF_WAKE_LOCK:
            return String("PROXIMITY_SCREEN_OFF_WAKE_LOCK");
        default:
            return String("???                           ");
    }
}

String CPowerManagerService::WakeLock::GetLockFlagsString()
{
    String result("");
    if ((mFlags & IPowerManager::ACQUIRE_CAUSES_WAKEUP) != 0) {
        result += String(" ACQUIRE_CAUSES_WAKEUP");
    }
    if ((mFlags & IPowerManager::ON_AFTER_RELEASE) != 0) {
        result += String(" ON_AFTER_RELEASE");
    }
    return result;
}


CPowerManagerService::SuspendBlockerImpl::SuspendBlockerImpl(
    /* [in] */ const String& name)
    : mName(name)
    , mReferenceCount(0)
{}

CPowerManagerService::SuspendBlockerImpl::~SuspendBlockerImpl()
{
    // try {
    if (mReferenceCount != 0) {
        Slogger::E(TAG, "Suspend blocker \"%s\" was finalized without being released!",
                (const char*)mName);
        // Log.wtf(TAG, "Suspend blocker \"" + mName
        //         + "\" was finalized without being released!");
        mReferenceCount = 0;
        CPowerManagerService::NativeReleaseSuspendBlocker(mName);
    }
    // } finally {
    //     super.finalize();
    // }
}

CAR_INTERFACE_IMPL(CPowerManagerService::SuspendBlockerImpl, ISuspendBlocker);

ECode CPowerManagerService::SuspendBlockerImpl::Acquire()
{
    Mutex::Autolock lock(mLock);
    mReferenceCount += 1;
    if (mReferenceCount == 1) {
        CPowerManagerService::NativeAcquireSuspendBlocker(mName);
    }
    return NOERROR;
}

ECode CPowerManagerService::SuspendBlockerImpl::ReleaseEx()
{
    Mutex::Autolock lock(mLock);
    mReferenceCount -= 1;
    if (mReferenceCount == 0) {
        CPowerManagerService::NativeReleaseSuspendBlocker(mName);
    }
    else if (mReferenceCount < 0) {
        Slogger::E(TAG, "Suspend blocker \"%s\" was released without being acquired!",
                mName.string());
        // Log.wtf(TAG, "Suspend blocker \"" + mName
        //         + "\" was released without being acquired!", new Throwable());
        mReferenceCount = 0;
        return E_INVALID_ARGUMENT;
    }
    return NOERROR;
}

ECode CPowerManagerService::SuspendBlockerImpl::BootFastSleepRelease()
{
    if(mReferenceCount > 0){
        if(DEBUG_BOOTFAST)
            Slogger::D(TAG, "boot fast mode force release!");
        CPowerManagerService::NativeReleaseSuspendBlocker(mName);
    }
    return NOERROR;
}

ECode CPowerManagerService::SuspendBlockerImpl::BootFastWakeAcquire()
{
    if (mReferenceCount > 0) {
        if(DEBUG_BOOTFAST)
            Slogger::D(TAG, "boot fast mode resume wakelock!");
       CPowerManagerService::NativeAcquireSuspendBlocker(mName);
    }
    return NOERROR;
}


CPowerManagerService::ScreenOnBlockerImpl::ScreenOnBlockerImpl(
    /* [in] */ CPowerManagerService* host)
    : mNestCount(0)
    , mHost(host)
{}

CAR_INTERFACE_IMPL(CPowerManagerService::ScreenOnBlockerImpl, IScreenOnBlocker);

Boolean CPowerManagerService::ScreenOnBlockerImpl::IsHeld()
{
    Mutex::Autolock lock(mLock);
    return mNestCount != 0;
}

ECode CPowerManagerService::ScreenOnBlockerImpl::Acquire()
{
    Mutex::Autolock lock(mLock);
    mNestCount += 1;
    if (CPowerManagerService::DEBUG) {
        Slogger::D(CPowerManagerService::TAG, "Screen on blocked: mNestCount=%d", mNestCount);
    }
    return NOERROR;
}

ECode CPowerManagerService::ScreenOnBlockerImpl::ReleaseEx()
{
    Mutex::Autolock lock(mLock);
    mNestCount -= 1;
    if (mNestCount < 0) {
        Slogger::E(CPowerManagerService::TAG,
                "Screen on blocker was released without being acquired!");
        // Log.wtf(TAG, "Screen on blocker was released without being acquired!",
        //         new Throwable());
        mNestCount = 0;
    }
    if (mNestCount == 0) {
        Boolean result;
        mHost->mHandler->SendEmptyMessage(MSG_SCREEN_ON_BLOCKER_RELEASED, &result);
    }
    if (CPowerManagerService::DEBUG) {
        Slogger::D(CPowerManagerService::TAG, "Screen on unblocked: mNestCount=%d", mNestCount);
    }
    return NOERROR;
}

CPowerManagerService::DisplayBlankerImpl::DisplayBlankerImpl(
    /* [in] */ CPowerManagerService* host)
    : mBlanked(FALSE)
    , mHost(host)
{}

CAR_INTERFACE_IMPL(CPowerManagerService::DisplayBlankerImpl, IDisplayBlanker);

ECode CPowerManagerService::DisplayBlankerImpl::BlankAllDisplays()
{
    Mutex::Autolock lock(mLock);
    mBlanked = TRUE;
    mHost->mDisplayManagerService->BlankAllDisplaysFromPowerManager();
    CPowerManagerService::NativeSetInteractive(FALSE);
    if(mHost->mBootFastStats)
        CPowerManagerService::NativeGoToBootFastSleep();
    else
        CPowerManagerService::NativeSetAutoSuspend(TRUE);
    return NOERROR;
}

ECode CPowerManagerService::DisplayBlankerImpl::UnblankAllDisplays()
{
    Mutex::Autolock lock(mLock);
    CPowerManagerService::NativeSetAutoSuspend(FALSE);
    CPowerManagerService::NativeSetInteractive(TRUE);
    mHost->mDisplayManagerService->UnblankAllDisplaysFromPowerManager();
    mBlanked = false;
    return NOERROR;
}


CPowerManagerService::CPowerManagerService()
    : mDirty(0)
    , mWakefulness(0)
    , mSandmanScheduled(FALSE)
    , mWakeLockSummary(0)
    , mRequestWaitForNegativeProximity(FALSE)
    , mLastWakeTime(0)
    , mLastSleepTime(0)
    , mSendWakeUpFinishedNotificationWhenReady(FALSE)
    , mSendGoToSleepFinishedNotificationWhenReady(FALSE)
    , mLastUserActivityTime(0)
    , mLastUserActivityTimeNoChangeLights(0)
    , mUserActivitySummary(0)
    , mLastScreenOffEventElapsedRealTime(0)
    , mDisplayReady(FALSE)
    , mHoldingWakeLockSuspendBlocker(FALSE)
    , mSystemReady(FALSE)
    , mBootCompleted(FALSE)
    , mIsPowered(FALSE)
    , mPlugType(0)
    , mBatteryLevel(0)
    , mHdmiPlugged(FALSE)
    , mBatteryLevelWhenDreamStarted(0)
    , mDockState(IIntent::EXTRA_DOCK_STATE_UNDOCKED)
    , mWakeUpWhenPluggedOrUnpluggedConfig(FALSE)
    , mDreamsSupportedConfig(FALSE)
    , mDreamsEnabledByDefaultConfig(FALSE)
    , mDreamsActivatedOnSleepByDefaultConfig(FALSE)
    , mDreamsActivatedOnDockByDefaultConfig(FALSE)
    , mDreamsEnabledSetting(FALSE)
    , mDreamsActivateOnSleepSetting(FALSE)
    , mDreamsActivateOnDockSetting(FALSE)
    , mScreenOffTimeoutSetting(0)
    , mMaximumScreenOffTimeoutFromDeviceAdmin(0x7FFFFFFF)
    , mStayOnWhilePluggedInSetting(0)
    , mStayOn(FALSE)
    , mProximityPositive(FALSE)
    , mBootFastStats(FALSE)
    , mBootFastSleepClockStart(0)
    , mBootFastSleepTimeStart(0)
    , mBootFastTempWakeStatus(FALSE)
    , mBootFastWakeLockToggle(FALSE)
    , mScreenBrightnessSettingMinimum(0)
    , mScreenBrightnessSettingMaximum(0)
    , mScreenBrightnessSettingDefault(0)
    , mScreenBrightnessSetting(0)
    , mScreenAutoBrightnessAdjustmentSetting(0.0f)
    , mScreenBrightnessModeSetting(0)
    , mScreenBrightnessOverrideFromWindowManager(-1)
    , mUserActivityTimeoutOverrideFromWindowManager(-1)
    , mTemporaryScreenBrightnessSettingOverride(-1)
    , mTemporaryScreenAutoBrightnessAdjustmentSettingOverride(Elastos::Core::Math::FLOAT_NAN)
    , mLastWarningAboutUserActivityPermission(Elastos::Core::Math::INT64_MAX_VALUE)
{
    mDisplayPowerRequest = new DisplayPowerRequest();
    mDisplayPowerControllerCallbacks =
            (DisplayPowerController::IDisplayPowerControllerCallbacks*)new MyDisplayPowerControllerCallbacks(this);
}

void CPowerManagerService::NativeInit()
{
    Int32 err = hw_get_module(POWER_HARDWARE_MODULE_ID,
            (hw_module_t const**)&gPowerModule);
    if (!err) {
        gPowerModule->init(gPowerModule);
    }
    else {
        ALOGE("Couldn't load %s module (%s)", POWER_HARDWARE_MODULE_ID,
                (const char*)StringUtils::Int32ToString(-err));
    }
}

static void writeSysFs(const char *path, const char *val)
{
    int fd;

    if((fd = open(path, O_RDWR)) < 0) {
        ALOGE("writeSysFs, open %s fail.", path);
        goto exit;
    }

    ALOGI("write %s, val:%s\n", path, val);

    write(fd, val, strlen(val));

exit:
    close(fd);
}

static void readSysFs(const char *path, char *buf, int count)
{
    int fd, r;

    if( NULL == buf ) {
        ALOGE("buf is NULL");
        return;
    }

    if((fd = open(path, O_RDONLY)) < 0) {
        ALOGE("readSysFs, open %s fail.", path);
        goto exit;
    }

    r = read(fd, buf, count);
    if (r < 0) {
        ALOGE("read error: %s, %s\n", path, (const char*)StringUtils::Int32ToString(errno));
    }

    ALOGI("read %s, val:%s\n", path, buf);

exit:
    close(fd);
}

// tellen add 2012/08/27 for first close cpu1 when shutdown or reboot
static Boolean closeCPU1()
{
    const char *CPU1_ONLINE = "/sys/devices/system/cpu/cpu1/online";
    const char *CPU_ONLINE = "/sys/devices/system/cpu/online";

    Boolean ret = TRUE;
    char online[10] = {0};

    readSysFs(CPU_ONLINE, (char*)online, 10);
    if( ( '0' == online[0]) && ( 10 == online[1]) ) {
        ALOGI("CPU1 has been closed");
    }
    else {
        memset(online, 0, 10);
        writeSysFs(CPU1_ONLINE, "0");
        readSysFs(CPU_ONLINE, online, 10);
        if( ( '0' == online[0]) && ( 10 == online[1]) ) {
            ALOGI("close CPU1 done");
        }
        else {
            ALOGE("close CPU1 fail");
            ret = FALSE;
        }
    }

    return ret;
}

void CPowerManagerService::NativeShutdown()
{
    // if (!closeCPU1())
    //     return;
    android_reboot(ANDROID_RB_POWEROFF, 0, 0);
}

ECode CPowerManagerService::NativeReboot(
    /* [in] */ const String& reason)
{
    // if (!closeCPU1())
    //     return NOERROR;
    if (reason == NULL) {
        android_reboot(ANDROID_RB_RESTART, 0, 0);
    } else {
        const char *chars = reason.string();
        android_reboot(ANDROID_RB_RESTART2, 0, (char *) chars);
    }
    // jniThrowIOException(env, errno);
    Slogger::E(TAG, "NativeReboot errorno: %d", errno);
    return E_IO_EXCEPTION;
}

void CPowerManagerService::NativeSetPowerState(
    /* [in] */ Boolean screenOn,
    /* [in] */ Boolean screenBright)
{
    Mutex::Autolock lock(gPowerManagerLock);
    gScreenOn = screenOn;
    gScreenBright = screenBright;
}

void CPowerManagerService::NativeAcquireSuspendBlocker(
    /* [in] */ const String& name)
{
    acquire_wake_lock(PARTIAL_WAKE_LOCK, name.string());
}

void CPowerManagerService::NativeReleaseSuspendBlocker(
    /* [in] */ const String& name)
{
    release_wake_lock(name.string());
}

void CPowerManagerService::NativeSetInteractive(
    /* [in] */ Boolean enable)
{
    if (gPowerModule) {
        if (enable) {
            ALOGD_IF_SLOW(20, "Excessive delay in setInteractive(true) while turning screen on");
            gPowerModule->setInteractive(gPowerModule, TRUE);
        }
        else {
            ALOGD_IF_SLOW(20, "Excessive delay in setInteractive(false) while turning screen off");
            gPowerModule->setInteractive(gPowerModule, FALSE);
        }
    }
}

void CPowerManagerService::NativeSetAutoSuspend(
    /* [in] */ Boolean enable)
{
    if (enable) {
        ALOGD_IF_SLOW(100, "Excessive delay in autosuspend_enable() while turning screen off");
        autosuspend_enable();
    }
    else {
        ALOGD_IF_SLOW(100, "Excessive delay in autosuspend_disable() while turning screen on");
        autosuspend_disable();
    }
}

void CPowerManagerService::NativeGoToBootFastSleep()
{
    Slogger::D(TAG, "nativeGoToBootFastSleep");
    autosuspend_bootfast();
    return;
}

void CPowerManagerService::Init(
    /* [in] */ IContext* context,
    /* [in] */ LightsService* ls,
    /* [in] */ CActivityManagerService* am,
    /* [in] */ CBatteryService* bs,
    /* [in] */ IIBatteryStats* bss,
    /* [in] */ CDisplayManagerService* dm)
{
    mContext = context;
    mLightsService = ls;
    mBatteryService = bs;
    mBatteryStats = bss;
    mDisplayManagerService = dm;
    ASSERT_SUCCEEDED(CHandlerThread::New(TAG, (IHandlerThread**)&mHandlerThread));
    mHandlerThread->Start();
    AutoPtr<ILooper> looper;
    ASSERT_SUCCEEDED(mHandlerThread->GetLooper((ILooper**)&looper));
    mHandler = new PowerManagerHandler(looper, this);

    AutoPtr<IMediaPlayerHelper> helper;
    CMediaPlayerHelper::AcquireSingleton((IMediaPlayerHelper**)&helper);
    helper->SetHdmiState(FALSE);

    // Watchdog.getInstance().addMonitor(this);

    // Forcibly turn the screen on at boot so that it is in a known power state.
    // We do this in init() rather than in the constructor because setting the
    // screen state requires a call into surface flinger which then needs to call back
    // into the activity manager to check permissions.  Unfortunately the
    // activity manager is not running when the constructor is called, so we
    // have to defer setting the screen state until this point.
    mDisplayBlanker->UnblankAllDisplays();
}

void CPowerManagerService::SetPolicy(
    /* [in] */ IWindowManagerPolicy* policy)
{
    Mutex::Autolock lock(_m_syncLock);
    mPolicy = policy;
}

ECode CPowerManagerService::SystemReady(
    /* [in] */ TwilightService* twilight)
    ///* [in] */ CDreamManagerService* dreamManager)
{
    Mutex::Autolock lock(_m_syncLock);
    mSystemReady = TRUE;
    // mDreamManager = dreamManager;

    AutoPtr<IInterface> obj;
    ASSERT_SUCCEEDED(mContext->GetSystemService(IContext::POWER_SERVICE, (IInterface**)&obj));
    AutoPtr<IPowerManager> pm = IPowerManager::Probe(obj);
    pm->GetMinimumScreenBrightnessSetting(&mScreenBrightnessSettingMinimum);
    pm->GetMaximumScreenBrightnessSetting(&mScreenBrightnessSettingMaximum);
    pm->GetDefaultScreenBrightnessSetting(&mScreenBrightnessSettingDefault);

    AutoPtr<ILooper> looper;
    mHandler->GetLooper((ILooper**)&looper);
    AutoPtr<ISystemSensorManager> systemSensorManager;
    CSystemSensorManager::New(mContext, looper, (ISystemSensorManager**)&systemSensorManager);
    AutoPtr<ISensorManager> sensorManager = ISensorManager::Probe(systemSensorManager.Get());

    // The notifier runs on the system server's main looper so as not to interfere
    // with the animations and other critical functions of the power manager.
    mNotifier = new Notifier(Looper::GetMainLooper(), mContext, mBatteryStats,
            CreateSuspendBlockerLocked(String("PowerManagerService.Broadcasts")),
            mScreenOnBlocker, mPolicy);

    // The display power controller runs on the power manager service's
    // own handler thread.
    mDisplayPowerController = new DisplayPowerController(looper, mContext,
            mNotifier, mLightsService, twilight, sensorManager, mDisplayManagerService, mDisplayBlanker,
            mDisplayPowerControllerCallbacks, mHandler);

    mWirelessChargerDetector = new WirelessChargerDetector(sensorManager,
            CreateSuspendBlockerLocked(String("PowerManagerService.WirelessChargerDetector")));

    mSettingsObserver = new SettingsObserver(mHandler, this);
//    mAttentionLight = mLightsService->GetLight(LightsService::_LIGHT_ID_ATTENTION);

    // Register for broadcasts from other components of the system.
    AutoPtr<IIntentFilter> filter;
    ASSERT_SUCCEEDED(CIntentFilter::New((IIntentFilter**)&filter));
    filter->AddAction(IIntent::ACTION_HDMISTATUS_CHANGED);
    AutoPtr<IBroadcastReceiver> receiver = (IBroadcastReceiver*)new HdmiPluggedReceiver(this);
    AutoPtr<IIntent> intent;
    mContext->RegisterReceiver(receiver, filter, (IIntent**)&intent);
    filter = NULL;
    ASSERT_SUCCEEDED(CIntentFilter::New((IIntentFilter**)&filter));
    filter->AddAction(IIntent::ACTION_BATTERY_CHANGED);
    AutoPtr<IBroadcastReceiver> batteryR = (IBroadcastReceiver*)new BatteryReceiver(this);
    intent = NULL;
    mContext->RegisterReceiverEx(batteryR, filter, String(NULL), mHandler, (IIntent**)&intent);

    filter = NULL;
    ASSERT_SUCCEEDED(CIntentFilter::New((IIntentFilter**)&filter));
    filter->AddAction(IIntent::ACTION_BOOT_COMPLETED);
    AutoPtr<IBroadcastReceiver> bootR = (IBroadcastReceiver*)new BootCompletedReceiver(this);
    intent = NULL;
    mContext->RegisterReceiverEx(bootR, filter, String(NULL), mHandler, (IIntent**)&intent);

    filter = NULL;
    ASSERT_SUCCEEDED(CIntentFilter::New((IIntentFilter**)&filter));
    filter->AddAction(IIntent::ACTION_DREAMING_STARTED);
    filter->AddAction(IIntent::ACTION_DREAMING_STOPPED);
    AutoPtr<IBroadcastReceiver> dreamR = (IBroadcastReceiver*)new DreamReceiver(this);
    intent = NULL;
    mContext->RegisterReceiverEx(dreamR, filter, String(NULL), mHandler, (IIntent**)&intent);

    filter = NULL;
    ASSERT_SUCCEEDED(CIntentFilter::New((IIntentFilter**)&filter));
    filter->AddAction(IIntent::ACTION_USER_SWITCHED);
    AutoPtr<IBroadcastReceiver> userR = (IBroadcastReceiver*)new UserSwitchedReceiver(this);
    intent = NULL;
    mContext->RegisterReceiverEx(userR, filter, String(NULL), mHandler, (IIntent**)&intent);

    filter = NULL;
    ASSERT_SUCCEEDED(CIntentFilter::New((IIntentFilter**)&filter));
    filter->AddAction(IIntent::ACTION_DOCK_EVENT);
    AutoPtr<IBroadcastReceiver> dockR = (IBroadcastReceiver*)new DockReceiver(this);
    intent = NULL;
    mContext->RegisterReceiverEx(dockR, filter, String(NULL), mHandler, (IIntent**)&intent);

    // Register for settings changes.
    AutoPtr<IContentResolver> resolver;
    ASSERT_SUCCEEDED(mContext->GetContentResolver((IContentResolver**)&resolver));

    AutoPtr<IUri> uri;
    AutoPtr<ISettingsSecure> settingsSecure;
    CSettingsSecure::AcquireSingleton((ISettingsSecure**)&settingsSecure);
    settingsSecure->GetUriForEx(ISettingsSecure::SCREENSAVER_ENABLED, (IUri**)&uri);
    resolver->RegisterContentObserverEx(uri, FALSE,
        (IContentObserver*)mSettingsObserver, IUserHandle::USER_ALL);

    uri = NULL;
    settingsSecure->GetUriForEx(ISettingsSecure::SCREENSAVER_ACTIVATE_ON_SLEEP, (IUri**)&uri);
    resolver->RegisterContentObserverEx(uri, FALSE,
        (IContentObserver*)mSettingsObserver, IUserHandle::USER_ALL);

    uri = NULL;
    settingsSecure->GetUriForEx(ISettingsSecure::SCREENSAVER_ACTIVATE_ON_DOCK, (IUri**)&uri);
    resolver->RegisterContentObserverEx(uri, FALSE,
        (IContentObserver*)mSettingsObserver, IUserHandle::USER_ALL);

    AutoPtr<ISettingsSystem> settingsSystem;
    CSettingsSystem::AcquireSingleton((ISettingsSystem**)&settingsSystem);
    uri = NULL;
    settingsSystem->GetUriForEx(ISettingsSystem::SCREEN_OFF_TIMEOUT, (IUri**)&uri);
    resolver->RegisterContentObserverEx(uri, FALSE,
        (IContentObserver*)mSettingsObserver, IUserHandle::USER_ALL);

    AutoPtr<ISettingsGlobal> settingsGlobal;
    CSettingsGlobal::AcquireSingleton((ISettingsGlobal**)&settingsGlobal);
    uri = NULL;
    settingsGlobal->GetUriForEx(ISettingsGlobal::STAY_ON_WHILE_PLUGGED_IN, (IUri**)&uri);
    resolver->RegisterContentObserverEx(uri, FALSE,
         (IContentObserver*)mSettingsObserver, IUserHandle::USER_ALL);

    uri = NULL;
    settingsSystem->GetUriForEx(ISettingsSystem::SCREEN_BRIGHTNESS, (IUri**)&uri);
    resolver->RegisterContentObserverEx(uri, FALSE,
        (IContentObserver*)mSettingsObserver, IUserHandle::USER_ALL);

    uri = NULL;
    settingsSystem->GetUriForEx(ISettingsSystem::SCREEN_BRIGHTNESS_MODE, (IUri**)&uri);
    resolver->RegisterContentObserverEx(uri, FALSE,
        (IContentObserver*)mSettingsObserver, IUserHandle::USER_ALL);

    // Go.
    ReadConfigurationLocked();
    UpdateSettingsLocked();
    mDirty |= DIRTY_BATTERY_STATE;
    UpdatePowerStateLocked();
    return NOERROR;
}

void CPowerManagerService::ReadConfigurationLocked()
{
    AutoPtr<IResources> resources;
    ASSERT_SUCCEEDED(mContext->GetResources((IResources**)&resources));
    resources->GetBoolean(R::bool_::config_unplugTurnsOnScreen,
            &mWakeUpWhenPluggedOrUnpluggedConfig);
    resources->GetBoolean(R::bool_::config_dreamsSupported,
            &mDreamsSupportedConfig);
    resources->GetBoolean(R::bool_::config_dreamsEnabledByDefault,
            &mDreamsEnabledByDefaultConfig);
    resources->GetBoolean(R::bool_::config_dreamsActivatedOnSleepByDefault,
            &mDreamsActivatedOnSleepByDefaultConfig);
    resources->GetBoolean(R::bool_::config_dreamsActivatedOnDockByDefault,
            &mDreamsActivatedOnDockByDefaultConfig);
}

void CPowerManagerService::UpdateSettingsLocked()
{
    AutoPtr<IContentResolver> resolver;
    ASSERT_SUCCEEDED(mContext->GetContentResolver((IContentResolver**)&resolver));

    Int32 value;
    AutoPtr<ISettingsSecure> settingsSecure;
    CSettingsSecure::AcquireSingleton((ISettingsSecure**)&settingsSecure);
    settingsSecure->GetInt32ForUser(resolver,
            ISettingsSecure::SCREENSAVER_ENABLED,
            mDreamsEnabledByDefaultConfig ? 1 : 0, IUserHandle::USER_CURRENT, &value);
    mDreamsEnabledSetting = value != 0;
    settingsSecure->GetInt32ForUser(resolver,
            ISettingsSecure::SCREENSAVER_ACTIVATE_ON_SLEEP,
            mDreamsActivatedOnSleepByDefaultConfig ? 1 : 0,
            IUserHandle::USER_CURRENT, &value);
    mDreamsActivateOnSleepSetting = value != 0;
    settingsSecure->GetInt32ForUser(resolver,
            ISettingsSecure::SCREENSAVER_ACTIVATE_ON_DOCK,
            mDreamsActivatedOnDockByDefaultConfig ? 1 : 0,
            IUserHandle::USER_CURRENT, &value);
    mDreamsActivateOnDockSetting = value != 0;

    AutoPtr<ISettingsSystem> settingsSystem;
    CSettingsSystem::AcquireSingleton((ISettingsSystem**)&settingsSystem);
    settingsSystem->GetInt32ForUser(resolver,
            ISettingsSystem::SCREEN_OFF_TIMEOUT,
            DEFAULT_SCREEN_OFF_TIMEOUT, IUserHandle::USER_CURRENT,
            &mScreenOffTimeoutSetting);

    AutoPtr<ISettingsGlobal> settingsGlobal;
    CSettingsGlobal::AcquireSingleton((ISettingsGlobal**)&settingsGlobal);
    settingsGlobal->GetInt32(resolver,
            ISettingsGlobal::STAY_ON_WHILE_PLUGGED_IN,
            IBatteryManager::BATTERY_PLUGGED_AC, &mStayOnWhilePluggedInSetting);

    Int32 oldScreenBrightnessSetting = mScreenBrightnessSetting;
    settingsSystem->GetInt32ForUser(resolver,
            ISettingsSystem::SCREEN_BRIGHTNESS,
            mScreenBrightnessSettingDefault, IUserHandle::USER_CURRENT,
            &mScreenBrightnessSetting);
    if (oldScreenBrightnessSetting != mScreenBrightnessSetting) {
        mTemporaryScreenBrightnessSettingOverride = -1;
    }

    Float oldScreenAutoBrightnessAdjustmentSetting = mScreenAutoBrightnessAdjustmentSetting;
    settingsSystem->GetFloatForUser(resolver,
            ISettingsSystem::SCREEN_AUTO_BRIGHTNESS_ADJ, 0.0f,
            IUserHandle::USER_CURRENT, &mScreenAutoBrightnessAdjustmentSetting);
    if (oldScreenAutoBrightnessAdjustmentSetting != mScreenAutoBrightnessAdjustmentSetting) {
        mTemporaryScreenAutoBrightnessAdjustmentSettingOverride = Elastos::Core::Math::FLOAT_NAN;
    }

    settingsSystem->GetInt32ForUser(resolver,
            ISettingsSystem::SCREEN_BRIGHTNESS_MODE,
            ISettingsSystem::SCREEN_BRIGHTNESS_MODE_MANUAL,
            IUserHandle::USER_CURRENT, &mScreenBrightnessModeSetting);

    mDirty |= DIRTY_SETTINGS;
}

void CPowerManagerService::HandleSettingsChangedLocked()
{
    UpdateSettingsLocked();
    UpdatePowerStateLocked();
}

ECode CPowerManagerService::AcquireWakeLock(
    /* [in] */ IBinder* lock,
    /* [in] */ Int32 flags,
    /* [in] */ const String& tag,
    /* [in] */ IWorkSource* ws)
{
    if (lock == NULL) {
        Slogger::E(TAG, "TODO: lock must not be null");
        return E_ILLEGAL_STATE_EXCEPTION;
        // throw new IllegalArgumentException("lock must not be null");
    }

    AutoPtr<IPowerManagerHelper> helper;
    FAIL_RETURN(CPowerManagerHelper::AcquireSingleton((IPowerManagerHelper**)&helper));
    FAIL_RETURN(helper->ValidateWakeLockParameters(flags, tag));
    FAIL_RETURN(mContext->EnforceCallingOrSelfPermission(
            Elastos::Droid::Manifest::Permission::WAKE_LOCK, String(NULL)));
    Int32 size;
    if (ws != NULL && (ws->GetSize(&size), size != 0)) {
        FAIL_RETURN(mContext->EnforceCallingOrSelfPermission(
                Elastos::Droid::Manifest::Permission::UPDATE_DEVICE_STATS, String(NULL)));
    }
    else {
        ws = NULL;
    }
    Int32 uid = Binder::GetCallingUid();
    Int32 pid = Binder::GetCallingPid();
    Int64 ident = Binder::ClearCallingIdentity();
    // try {
    ECode ec = AcquireWakeLockInternal(lock, flags, tag, ws, uid, pid);
    Binder::RestoreCallingIdentity(ident);
    return ec;
    // } finally {
    //     Binder.restoreCallingIdentity(ident);
    // }
}

ECode CPowerManagerService::AcquireWakeLockInternal(
    /* [in] */ IBinder* _lock,
    /* [in] */ Int32 flags,
    /* [in] */ const String& tag,
    /* [in] */ IWorkSource* ws,
    /* [in] */ Int32 uid,
    /* [in] */ Int32 pid)
{
    Mutex::Autolock lock(_m_syncLock);
    if (DEBUG_SPEW) {
        Slogger::D(TAG, "acquireWakeLockInternal: lock=, flags=0x%08x, tag=\"%s\", ws=%p, uid=%d, pid=%d"
                /*+ Objects.hashCode(lock)*/, flags, (const char*)tag, ws, uid, pid);
    }

    AutoPtr<WakeLock> wakeLock;
    List< AutoPtr<WakeLock> >::Iterator it = FindWakeLockIndexLocked(_lock);
    if (it != mWakeLocks.End()) {
        wakeLock = *it;
        if (!wakeLock->HasSameProperties(flags, tag, ws, uid, pid)) {
            // Update existing wake lock.  This shouldn't happen but is harmless.
            NotifyWakeLockReleasedLocked(wakeLock);
            wakeLock->UpdateProperties(flags, tag, ws, uid, pid);
            NotifyWakeLockAcquiredLocked(wakeLock);
        }
    }
    else {
        wakeLock = new WakeLock(_lock, flags, tag, ws, uid, pid, this);
        AutoPtr<IProxy> proxy = (IProxy*)_lock->Probe(EIID_IProxy);
        if (proxy != NULL && FAILED(proxy->LinkToDeath(wakeLock, 0))) {
            Slogger::E(TAG, "Wake lock is already dead.");
            return E_ILLEGAL_ARGUMENT_EXCEPTION;
        }
        NotifyWakeLockAcquiredLocked(wakeLock);
        mWakeLocks.PushBack(wakeLock);
    }

    ApplyWakeLockFlagsOnAcquireLocked(wakeLock);
    mDirty |= DIRTY_WAKE_LOCKS;
    UpdatePowerStateLocked();
    return NOERROR;
}

Boolean CPowerManagerService::IsScreenLock(
    /* [in] */ WakeLock* wakeLock)
{
    switch (wakeLock->mFlags & IPowerManager::WAKE_LOCK_LEVEL_MASK) {
        case IPowerManager::FULL_WAKE_LOCK:
        case IPowerManager::SCREEN_BRIGHT_WAKE_LOCK:
        case IPowerManager::SCREEN_DIM_WAKE_LOCK:
            return TRUE;
    }
    return FALSE;
}

void CPowerManagerService::ApplyWakeLockFlagsOnAcquireLocked(
    /* [in] */ WakeLock* wakeLock)
{
    if ((wakeLock->mFlags & IPowerManager::ACQUIRE_CAUSES_WAKEUP) != 0 &&
            IsScreenLock(wakeLock)) {
        WakeUpNoUpdateLocked(SystemClock::GetUptimeMillis());
    }
}

ECode CPowerManagerService::ReleaseWakeLock(
    /* [in] */ IBinder* lock,
    /* [in] */ Int32 flags)
{
    if (lock == NULL) {
        Slogger::E(TAG, "lock must not be null");
        return E_ILLEGAL_STATE_EXCEPTION;
        // throw new IllegalArgumentException("lock must not be null");
    }

    FAIL_RETURN(mContext->EnforceCallingOrSelfPermission(
        Elastos::Droid::Manifest::Permission::WAKE_LOCK, String(NULL)));

    Int64 ident = Binder::ClearCallingIdentity();
    // try {
    ReleaseWakeLockInternal(lock, flags);
    // } finally {
    Binder::RestoreCallingIdentity(ident);
    // }
    return NOERROR;
}

void CPowerManagerService::ReleaseWakeLockInternal(
    /* [in] */ IBinder* _lock,
    /* [in] */ Int32 flags)
{
    Mutex::Autolock lock(_m_syncLock);
    if (DEBUG_SPEW) {
        Slogger::D(TAG, "releaseWakeLockInternal: lock=, flags=0x%08x"
                /* + Objects.hashCode(lock)*/, flags);
    }

    List< AutoPtr<WakeLock> >::Iterator it = FindWakeLockIndexLocked(_lock);
    if (it == mWakeLocks.End()) {
        return;
    }

    AutoPtr<WakeLock> wakeLock = *it;
    mWakeLocks.Erase(it);
    NotifyWakeLockReleasedLocked(wakeLock);
    AutoPtr<IProxy> proxy = (IProxy*)wakeLock->mLock->Probe(EIID_IProxy);
    if (proxy != NULL) {
        Boolean res;
        proxy->UnlinkToDeath(wakeLock, 0, &res);
    }

    if ((flags & IPowerManager::WAIT_FOR_PROXIMITY_NEGATIVE) != 0) {
        mRequestWaitForNegativeProximity = TRUE;
    }

    ApplyWakeLockFlagsOnReleaseLocked(wakeLock);
    mDirty |= DIRTY_WAKE_LOCKS;
    UpdatePowerStateLocked();
}

void CPowerManagerService::HandleWakeLockDeath(
    /* [in] */ WakeLock* wakeLock)
{
    Mutex::Autolock lock(_m_syncLock);
    if (DEBUG_SPEW) {
        Slogger::D(TAG, "handleWakeLockDeath: lock="/* + Objects.hashCode(wakeLock.mLock)*/);
    }

    List< AutoPtr<WakeLock> >::Iterator it = mWakeLocks.Begin();
    for (; it != mWakeLocks.End(); ++it) {
        if ((*it).Get() == wakeLock) {
            break;
        }
    }
    if (it == mWakeLocks.End()) {
        return;
    }

    mWakeLocks.Erase(it);
    NotifyWakeLockReleasedLocked(wakeLock);

    ApplyWakeLockFlagsOnReleaseLocked(wakeLock);
    mDirty |= DIRTY_WAKE_LOCKS;
    UpdatePowerStateLocked();
}

void CPowerManagerService::ApplyWakeLockFlagsOnReleaseLocked(
    /* [in] */ WakeLock* wakeLock)
{
    if ((wakeLock->mFlags & IPowerManager::ON_AFTER_RELEASE) != 0 && (wakeLock->mFlags & IPowerManager::PARTIAL_WAKE_LOCK) == 0) {
        UserActivityNoUpdateLocked(SystemClock::GetUptimeMillis(),
                IPowerManager::USER_ACTIVITY_EVENT_OTHER,
                IPowerManager::USER_ACTIVITY_FLAG_NO_CHANGE_LIGHTS,
                wakeLock->mOwnerUid);
    }
}

ECode CPowerManagerService::UpdateWakeLockWorkSource(
    /* [in] */ IBinder* _lock,
    /* [in] */ IWorkSource* ws)
{
    if (_lock == NULL) {
        Slogger::E(TAG, "lock must not be null");
        return E_ILLEGAL_STATE_EXCEPTION;
        // throw new IllegalArgumentException("lock must not be null");
    }

    FAIL_RETURN(mContext->EnforceCallingOrSelfPermission(
            Elastos::Droid::Manifest::Permission::WAKE_LOCK, String(NULL)));
    Int32 size;
    if (ws != NULL && (ws->GetSize(&size), size != 0)) {
        FAIL_RETURN(mContext->EnforceCallingOrSelfPermission(
                Elastos::Droid::Manifest::Permission::UPDATE_DEVICE_STATS, String(NULL)));
    }
    else {
        ws = NULL;
    }

    Int64 ident = Binder::ClearCallingIdentity();
    // try {
    ECode ec = UpdateWakeLockWorkSourceInternal(_lock, ws);
    // } finally {
    Binder::RestoreCallingIdentity(ident);
    // }
    return ec;
}

ECode CPowerManagerService::UpdateWakeLockWorkSourceInternal(
    /* [in] */IBinder* _lock,
    /* [in] */IWorkSource* ws)
{
    Mutex::Autolock lock(_m_syncLock);
    List< AutoPtr<WakeLock> >::Iterator it = FindWakeLockIndexLocked(_lock);
    if (it == mWakeLocks.End()) {
        Slogger::E(TAG, "Wake lock not active");
        return E_ILLEGAL_STATE_EXCEPTION;
        // throw new IllegalArgumentException("Wake lock not active");
    }

    AutoPtr<WakeLock> wakeLock = *it;
    if (!wakeLock->HasSameWorkSource(ws)) {
        NotifyWakeLockReleasedLocked(wakeLock);
        wakeLock->UpdateWorkSource(ws);
        NotifyWakeLockAcquiredLocked(wakeLock);
    }
    return NOERROR;
}

List< AutoPtr<CPowerManagerService::WakeLock> >::Iterator
CPowerManagerService::FindWakeLockIndexLocked(
    /* [in] */IBinder* _lock)
{
    List< AutoPtr<WakeLock> >::Iterator it = mWakeLocks.Begin();
    for (; it != mWakeLocks.End(); ++it) {
        if ((*it)->mLock.Get() == _lock) {
            return it;
        }
    }
    return mWakeLocks.End();
}

void CPowerManagerService::NotifyWakeLockAcquiredLocked(
    /* [in] */ WakeLock* wakeLock)
{
    if (mSystemReady) {
        mNotifier->OnWakeLockAcquired(wakeLock->mFlags, wakeLock->mTag,
                wakeLock->mOwnerUid, wakeLock->mOwnerPid,
                wakeLock->mWorkSource);
    }
}

void CPowerManagerService::NotifyWakeLockReleasedLocked(
    /* [in] */ WakeLock* wakeLock)
{
    if (mSystemReady) {
        mNotifier->OnWakeLockReleased(wakeLock->mFlags, wakeLock->mTag,
                wakeLock->mOwnerUid, wakeLock->mOwnerPid,
                wakeLock->mWorkSource);
    }
}

ECode CPowerManagerService::IsWakeLockLevelSupported(
    /* [in] */ Int32 level,
    /* [out] */ Boolean* res)
{
    VALIDATE_NOT_NULL(res);
    Int64 ident = Binder::ClearCallingIdentity();
    // try {
    *res = IsWakeLockLevelSupportedInternal(level);
    // } finally {
    Binder::RestoreCallingIdentity(ident);
    // }
    return NOERROR;
}

Boolean CPowerManagerService::IsWakeLockLevelSupportedInternal(
    /* [in] */ Int32 level)
{
    Mutex::Autolock lock(_m_syncLock);
    switch (level) {
        case IPowerManager::PARTIAL_WAKE_LOCK:
        case IPowerManager::SCREEN_DIM_WAKE_LOCK:
        case IPowerManager::SCREEN_BRIGHT_WAKE_LOCK:
        case IPowerManager::FULL_WAKE_LOCK:
            return TRUE;

        case IPowerManager::PROXIMITY_SCREEN_OFF_WAKE_LOCK:
            return mSystemReady && mDisplayPowerController->IsProximitySensorAvailable();

        default:
            return FALSE;
    }
}

ECode CPowerManagerService::UserActivity(
    /* [in] */ Int64 eventTime,
    /* [in] */ Int32 event,
    /* [in] */ Int32 flags)
{
    Int64 now = SystemClock::GetUptimeMillis();
    Int32 result;
    FAIL_RETURN(mContext->CheckCallingOrSelfPermission(Elastos::Droid::Manifest::Permission::DEVICE_POWER, &result));
    if (result != IPackageManager::PERMISSION_GRANTED) {
        // Once upon a time applications could call userActivity().
        // Now we require the DEVICE_POWER permission.  Log a warning and ignore the
        // request instead of throwing a SecurityException so we don't break old apps.
        Mutex::Autolock lock(_m_syncLock);
        if (now >= mLastWarningAboutUserActivityPermission + (5 * 60 * 1000)) {
            mLastWarningAboutUserActivityPermission = now;
            Slogger::W(TAG, "Ignoring call to PowerManager.userActivity() because the caller does not have DEVICE_POWER permission.  Please fix your app!   pid=%d uid=%d"
                    , Binder::GetCallingPid(), Binder::GetCallingUid());
        }
        return NOERROR;
    }

    if (eventTime > SystemClock::GetUptimeMillis()) {
        Slogger::E(TAG, "event time must not be in the future");
        return E_ILLEGAL_STATE_EXCEPTION;
        // throw new IllegalArgumentException("event time must not be in the future");
    }

    Int32 uid = Binder::GetCallingUid();
    Int64 ident = Binder::ClearCallingIdentity();
    // try {
    UserActivityInternal(eventTime, event, flags, uid);
    // } finally {
    Binder::RestoreCallingIdentity(ident);
    // }
    return NOERROR;
}

void CPowerManagerService::UserActivityFromNative(
    /* [in] */ Int64 eventTime,
    /* [in] */ Int32 event,
    /* [in] */ Int32 flags)
{
    if(!mBootFastStats){
        UserActivityInternal(eventTime, event, flags, IProcess::SYSTEM_UID);
    }
    else{
        if(DEBUG_BOOTFAST)
            Slogger::D(TAG, "boot fast not allow userActivityFromNative");
    }
}

void CPowerManagerService::UserActivityInternal(
    /* [in] */ Int64 eventTime,
    /* [in] */ Int32 event,
    /* [in] */ Int32 flags,
    /* [in] */ Int32 uid)
{
    if(mBootFastStats){
        if(DEBUG_BOOTFAST)
            Slogger::D(TAG, "in boot fast mode not allow userActivity");
            return;
    }
    Mutex::Autolock lock(_m_syncLock);
    if (UserActivityNoUpdateLocked(eventTime, event, flags, uid)) {
        UpdatePowerStateLocked();
    }
}

Boolean CPowerManagerService::UserActivityNoUpdateLocked(
    /* [in] */ Int64 eventTime,
    /* [in] */ Int32 event,
    /* [in] */ Int32 flags,
    /* [in] */ Int32 uid)
{
    if (DEBUG_SPEW) {
        Slogger::D(TAG, "userActivityNoUpdateLocked: eventTime=%d, event=%d, flags=0x%08x, uid=%d"
                , eventTime, event, flags, uid);
    }

    if (eventTime < mLastSleepTime || eventTime < mLastWakeTime
            || mWakefulness == WAKEFULNESS_ASLEEP || !mBootCompleted || !mSystemReady) {
        return FALSE;
    }

    mNotifier->OnUserActivity(event, uid);

    if ((flags & IPowerManager::USER_ACTIVITY_FLAG_NO_CHANGE_LIGHTS) != 0) {
        if (eventTime > mLastUserActivityTimeNoChangeLights
                && eventTime > mLastUserActivityTime) {
            mLastUserActivityTimeNoChangeLights = eventTime;
            mDirty |= DIRTY_USER_ACTIVITY;
            return TRUE;
        }
    }
    else {
        if (eventTime > mLastUserActivityTime) {
            mLastUserActivityTime = eventTime;
            mDirty |= DIRTY_USER_ACTIVITY;
            return TRUE;
        }
    }
    return FALSE;
}

ECode CPowerManagerService::WakeUp(
    /* [in] */ Int64 eventTime)
{
    if (eventTime > SystemClock::GetUptimeMillis()) {
        Slogger::E(TAG, "event time must not be in the future");
        return E_ILLEGAL_STATE_EXCEPTION;
        // throw new IllegalArgumentException("event time must not be in the future");
    }

    FAIL_RETURN(mContext->EnforceCallingOrSelfPermission(
        Elastos::Droid::Manifest::Permission::DEVICE_POWER, String(NULL)));

    Int64 ident = Binder::ClearCallingIdentity();
    // try {
    if(!mBootFastStats) {
        WakeUpInternal(eventTime);
    }
    else{
        if(DEBUG_BOOTFAST)
            Slogger::D(TAG, "boot fast status not allow wakeup");
    }
    // } finally {
    Binder::RestoreCallingIdentity(ident);
    // }
    return NOERROR;
}

void CPowerManagerService::WakeUpFromNative(
    /* [in] */ Int64 eventTime)
{
    if(!mBootFastStats)
        WakeUpInternal(eventTime);
    else{
        if(DEBUG_BOOTFAST)
            Slogger::D(TAG, "boot fast status not allow wakeUpFromNative");
    }
}

void CPowerManagerService::WakeUpInternal(
    /* [in] */ Int64 eventTime)
{
    Mutex::Autolock lock(_m_syncLock);
    if (WakeUpNoUpdateLocked(eventTime)) {
        UpdatePowerStateLocked();
    }
}

Boolean CPowerManagerService::WakeUpNoUpdateLocked(
    /* [in] */ Int64 eventTime)
{
    if (DEBUG_SPEW) {
        Slogger::D(TAG, "wakeUpNoUpdateLocked: eventTime=%d", eventTime);
    }

    if (eventTime < mLastSleepTime || mWakefulness == WAKEFULNESS_AWAKE
            || !mBootCompleted || !mSystemReady) {
        return FALSE;
    }

    /*add by chenjd, 20130515*/
    /* when wake up, turn on the status led*/
    ChanceLedStatus(1);

    switch (mWakefulness) {
        case WAKEFULNESS_ASLEEP:
            Slogger::I(TAG, "Waking up from sleep...");
            SendPendingNotificationsLocked();
            mNotifier->OnWakeUpStarted();
            mSendWakeUpFinishedNotificationWhenReady = TRUE;
            break;
        case WAKEFULNESS_DREAMING:
            Slogger::I(TAG, "Waking up from dream...");
            break;
        case WAKEFULNESS_NAPPING:
            Slogger::I(TAG, "Waking up from nap...");
            break;
    }

    mLastWakeTime = eventTime;
    mWakefulness = WAKEFULNESS_AWAKE;
    mDirty |= DIRTY_WAKEFULNESS;

    UserActivityNoUpdateLocked(
            eventTime, IPowerManager::USER_ACTIVITY_EVENT_OTHER, 0, IProcess::SYSTEM_UID);
    return TRUE;
}

ECode CPowerManagerService::GoToSleep(
    /* [in] */ Int64 eventTime,
    /* [in] */ Int32 reason)
{
    if (eventTime > SystemClock::GetUptimeMillis()) {
        Slogger::E(TAG, "event time must not be in the future");
        return E_ILLEGAL_STATE_EXCEPTION;
        // throw new IllegalArgumentException("event time must not be in the future");
    }

    FAIL_RETURN(mContext->EnforceCallingOrSelfPermission(
            Elastos::Droid::Manifest::Permission::DEVICE_POWER, String(NULL)));

    Int64 ident = Binder::ClearCallingIdentity();
    // try {
    GoToSleepInternal(eventTime, reason);
    // } finally {
    Binder::RestoreCallingIdentity(ident);
    // }
    return NOERROR;
}

void CPowerManagerService::GoToSleepFromNative(
    /* [in] */ Int64 eventTime,
    /* [in] */ Int32 reason)
{
    GoToSleepInternal(eventTime, reason);
}

void CPowerManagerService::GoToSleepInternal(
    /* [in] */ Int64 eventTime,
    /* [in] */ Int32 reason)
{
    Mutex::Autolock lock(_m_syncLock);
    if (GoToSleepNoUpdateLocked(eventTime, reason)) {
        UpdatePowerStateLocked();
    }
}

void CPowerManagerService::ChanceLedStatus(
    /* [in] */ Int32 status)
{
    Char32 portType = 'h';
    Int32 portNum = 20;
    Slogger::E(TAG, "no Gpio!!!!!!!!!!!");
    assert(0);
    // Gpio::WriteGpio(portType, portNum, status);
}

Boolean CPowerManagerService::GoToSleepNoUpdateLocked(
    /* [in] */ Int64 eventTime,
    /* [in] */ Int32 reason)
{
    if (DEBUG_SPEW) {
        Slogger::D(TAG, "goToSleepNoUpdateLocked: eventTime=%d, reason=%d"
                , eventTime, reason);
    }

    if (eventTime < mLastWakeTime || mWakefulness == WAKEFULNESS_ASLEEP
            || !mBootCompleted || !mSystemReady || mBootFastStats) {
        return FALSE;
    }

    /*add by chenjd, 20130515*/
    /* when go to sleep, turn off the status led*/
    ChanceLedStatus(0);

    switch (reason) {
        case IPowerManager::GO_TO_SLEEP_REASON_DEVICE_ADMIN:
            Slogger::I(TAG, "Going to sleep due to device administration policy...");
            break;
        case IPowerManager::GO_TO_SLEEP_REASON_TIMEOUT:
            Slogger::I(TAG, "Going to sleep due to screen timeout...");
            break;
        default:
            Slogger::I(TAG, "Going to sleep by user request...");
            reason = IPowerManager::GO_TO_SLEEP_REASON_USER;
            break;
    }

    SendPendingNotificationsLocked();
    mNotifier->OnGoToSleepStarted(reason);
    mSendGoToSleepFinishedNotificationWhenReady = TRUE;

    mLastSleepTime = eventTime;
    mDirty |= DIRTY_WAKEFULNESS;
    mWakefulness = WAKEFULNESS_ASLEEP;

    // Report the number of wake locks that will be cleared by going to sleep.
    Int32 numWakeLocksCleared = 0;
    List< AutoPtr<WakeLock> >::Iterator it = mWakeLocks.Begin();
    for (; it != mWakeLocks.End(); ++it) {
        AutoPtr<WakeLock> wakeLock = *it;
        switch (wakeLock->mFlags & IPowerManager::WAKE_LOCK_LEVEL_MASK) {
            case IPowerManager::FULL_WAKE_LOCK:
            case IPowerManager::SCREEN_BRIGHT_WAKE_LOCK:
            case IPowerManager::SCREEN_DIM_WAKE_LOCK:
                numWakeLocksCleared += 1;
                break;
        }
    }
    // EventLog.writeEvent(EventLogTags.POWER_SLEEP_REQUESTED, numWakeLocksCleared);
    return TRUE;
}

ECode CPowerManagerService::Nap(
    /* [in] */ Int64 eventTime)
{
    if (eventTime > SystemClock::GetUptimeMillis()) {
        Slogger::E(TAG, "event time must not be in the future");
        return E_ILLEGAL_STATE_EXCEPTION;
        // throw new IllegalArgumentException("event time must not be in the future");
    }

    FAIL_RETURN(mContext->EnforceCallingOrSelfPermission(
        Elastos::Droid::Manifest::Permission::DEVICE_POWER, String(NULL)));

    Int64 ident = Binder::ClearCallingIdentity();
    // try {
    NapInternal(eventTime);
    // } finally {
    Binder::RestoreCallingIdentity(ident);
    // }
    return NOERROR;
}

void CPowerManagerService::NapInternal(
    /* [in] */ Int64 eventTime)
{
    Mutex::Autolock lock(_m_syncLock);
    if (NapNoUpdateLocked(eventTime)) {
        UpdatePowerStateLocked();
    }
}

Boolean CPowerManagerService::NapNoUpdateLocked(
    /* [in] */ Int64 eventTime)
{
    if (DEBUG_SPEW) {
        Slogger::D(TAG, "napNoUpdateLocked: eventTime=%d", eventTime);
    }

    if (eventTime < mLastWakeTime || mWakefulness != WAKEFULNESS_AWAKE
            || !mBootCompleted || !mSystemReady) {
        return FALSE;
    }

    Slogger::I(TAG, "Nap time...");

    mDirty |= DIRTY_WAKEFULNESS;
    mWakefulness = WAKEFULNESS_NAPPING;
    return TRUE;
}

void CPowerManagerService::UpdatePowerStateLocked()
{
    if (!mSystemReady || mDirty == 0) {
        return;
    }

    // Phase 0: Basic state updates.
    UpdateIsPoweredLocked(mDirty);
    UpdateStayOnLocked(mDirty);

    // Phase 1: Update wakefulness.
    // Loop because the wake lock and user activity computations are influenced
    // by changes in wakefulness.
    Int64 now = SystemClock::GetUptimeMillis();
    Int32 dirtyPhase2 = 0;
    for (;;) {
        Int32 dirtyPhase1 = mDirty;
        dirtyPhase2 |= dirtyPhase1;
        mDirty = 0;

        UpdateWakeLockSummaryLocked(dirtyPhase1);
        UpdateUserActivitySummaryLocked(now, dirtyPhase1);
        if (!UpdateWakefulnessLocked(dirtyPhase1)) {
            break;
        }
    }

    // Phase 2: Update dreams and display power state.
    UpdateDreamLocked(dirtyPhase2);
    UpdateDisplayPowerStateLocked(dirtyPhase2);

    // Phase 3: Send notifications, if needed.
    if (mDisplayReady) {
        SendPendingNotificationsLocked();
    }

    // Phase 4: Update suspend blocker.
    // Because we might release the last suspend blocker here, we need to make sure
    // we finished everything else first!
    UpdateSuspendBlockerLocked();
    UpdateBootFastSuspendBlockerLocked();
}

void CPowerManagerService::SendPendingNotificationsLocked()
{
    if (mSendWakeUpFinishedNotificationWhenReady) {
        mSendWakeUpFinishedNotificationWhenReady = FALSE;
        mNotifier->OnWakeUpFinished();
    }
    if (mSendGoToSleepFinishedNotificationWhenReady) {
        mSendGoToSleepFinishedNotificationWhenReady = FALSE;
        mNotifier->OnGoToSleepFinished();
    }
}

void CPowerManagerService::UpdateIsPoweredLocked(
    /* [in] */ Int32 dirty)
{
    if ((dirty & DIRTY_BATTERY_STATE) != 0) {
        Boolean wasPowered = mIsPowered;
        Int32 oldPlugType = mPlugType;
        mIsPowered = mBatteryService->IsPowered(IBatteryManager::BATTERY_PLUGGED_ANY);
        mPlugType = mBatteryService->GetPlugType();
        mBatteryLevel = mBatteryService->GetBatteryLevel();

        if (DEBUG) {
            Slogger::D(TAG, "updateIsPoweredLocked: wasPowered=%d, mIsPowered=%d, oldPlugType=%d, mBatteryLevel=%d"
                    , wasPowered, mIsPowered, oldPlugType, mPlugType, mBatteryLevel);
        }

        if (wasPowered != mIsPowered || oldPlugType != mPlugType) {
            mDirty |= DIRTY_IS_POWERED;

            // Update wireless dock detection state.
            Boolean dockedOnWirelessCharger = mWirelessChargerDetector->Update(
                    mIsPowered, mPlugType, mBatteryLevel);

            // Treat plugging and unplugging the devices as a user activity.
            // Users find it disconcerting when they plug or unplug the device
            // and it shuts off right away.
            // Some devices also wake the device when plugged or unplugged because
            // they don't have a charging LED.
            Int64 now = SystemClock::GetUptimeMillis();
            if (ShouldWakeUpWhenPluggedOrUnpluggedLocked(wasPowered, oldPlugType, dockedOnWirelessCharger)) {
                if(mBootFastStats&&!wasPowered&&mIsPowered){
                    TempWakeUp();
                }
                else{
                    if(!mBootFastStats)
                        WakeUpNoUpdateLocked(now);
                }
            }
            UserActivityNoUpdateLocked(
                    now, IPowerManager::USER_ACTIVITY_EVENT_OTHER, 0, IProcess::SYSTEM_UID);

            // Tell the notifier whether wireless charging has started so that
            // it can provide feedback to the user.
            if (dockedOnWirelessCharger) {
                mNotifier->OnWirelessChargingStarted();
            }
        }
    }
}

Boolean CPowerManagerService::ShouldWakeUpWhenPluggedOrUnpluggedLocked(
    /* [in] */ Boolean wasPowered,
    /* [in] */ Int32 oldPlugType,
    /* [in] */ Boolean dockedOnWirelessCharger)
{
    // Don't wake when powered unless configured to do so.
    if (!mWakeUpWhenPluggedOrUnpluggedConfig) {
        return FALSE;
    }

    // Don't wake when undocked from wireless charger.
    // See WirelessChargerDetector for justification.
    if (wasPowered && !mIsPowered
            /*&& oldPlugType == IBatteryManager::BATTERY_PLUGGED_WIRELESS*/) {
        return FALSE;
    }

    // Don't wake when docked on wireless charger unless we are certain of it.
    // See WirelessChargerDetector for justification.
    if (!wasPowered && mIsPowered
            /*&& mPlugType == IBatteryManager::BATTERY_PLUGGED_WIRELESS*/
            && !dockedOnWirelessCharger) {
        return FALSE;
    }

    // If already dreaming and becoming powered, then don't wake.
    if (mIsPowered && (mWakefulness == WAKEFULNESS_NAPPING
            || mWakefulness == WAKEFULNESS_DREAMING)) {
        return FALSE;
    }

    // Otherwise wake up!
    return TRUE;
}

void CPowerManagerService::UpdateStayOnLocked(
    /* [in] */ Int32 dirty)
{
    if ((dirty & (DIRTY_BATTERY_STATE | DIRTY_SETTINGS)) != 0) {
        Boolean wasStayOn = mStayOn;
        if (mStayOnWhilePluggedInSetting != 0
                && !IsMaximumScreenOffTimeoutFromDeviceAdminEnforcedLocked()) {
            mStayOn = mBatteryService->IsPowered(mStayOnWhilePluggedInSetting);
        }
        else {
            mStayOn = FALSE;
        }

        if (mStayOn != wasStayOn) {
            mDirty |= DIRTY_STAY_ON;
        }
    }
}

void CPowerManagerService::UpdateWakeLockSummaryLocked(
    /* [in] */ Int32 dirty)
{
    if ((dirty & (DIRTY_WAKE_LOCKS | DIRTY_WAKEFULNESS)) != 0) {
        mWakeLockSummary = 0;

        List< AutoPtr<WakeLock> >::Iterator it = mWakeLocks.Begin();
        for (; it != mWakeLocks.End(); ++it) {
            AutoPtr<WakeLock> wakeLock = *it;
            switch (wakeLock->mFlags & IPowerManager::WAKE_LOCK_LEVEL_MASK) {
                case IPowerManager::PARTIAL_WAKE_LOCK:
                    mWakeLockSummary |= WAKE_LOCK_CPU;
                    break;
                case IPowerManager::FULL_WAKE_LOCK:
                    if (mWakefulness != WAKEFULNESS_ASLEEP) {
                        mWakeLockSummary |= WAKE_LOCK_CPU
                                | WAKE_LOCK_SCREEN_BRIGHT | WAKE_LOCK_BUTTON_BRIGHT;
                        if (mWakefulness == WAKEFULNESS_AWAKE) {
                            mWakeLockSummary |= WAKE_LOCK_STAY_AWAKE;
                        }
                    }
                    break;
                case IPowerManager::SCREEN_BRIGHT_WAKE_LOCK:
                    if (mWakefulness != WAKEFULNESS_ASLEEP) {
                        mWakeLockSummary |= WAKE_LOCK_CPU | WAKE_LOCK_SCREEN_BRIGHT;
                        if (mWakefulness == WAKEFULNESS_AWAKE) {
                            mWakeLockSummary |= WAKE_LOCK_STAY_AWAKE;
                        }
                    }
                    break;
                case IPowerManager::SCREEN_DIM_WAKE_LOCK:
                    if (mWakefulness != WAKEFULNESS_ASLEEP) {
                        mWakeLockSummary |= WAKE_LOCK_CPU | WAKE_LOCK_SCREEN_DIM;
                        if (mWakefulness == WAKEFULNESS_AWAKE) {
                            mWakeLockSummary |= WAKE_LOCK_STAY_AWAKE;
                        }
                    }
                    break;
                case IPowerManager::PROXIMITY_SCREEN_OFF_WAKE_LOCK:
                    if (mWakefulness != WAKEFULNESS_ASLEEP) {
                        mWakeLockSummary |= WAKE_LOCK_CPU | WAKE_LOCK_PROXIMITY_SCREEN_OFF;
                    }
                    break;
            }
        }

        if (DEBUG_SPEW) {
            Slogger::D(TAG, "updateWakeLockSummaryLocked: mWakefulness=%s, mWakeLockSummary=0x%08x"
                    , (const char*)WakefulnessToString(mWakefulness), mWakeLockSummary);
        }
    }
}

void CPowerManagerService::UpdateUserActivitySummaryLocked(
    /* [in] */ Int64 now,
    /* [in] */ Int32 dirty)
{
    // Update the status of the user activity timeout timer.
    if ((dirty & (DIRTY_USER_ACTIVITY | DIRTY_WAKEFULNESS | DIRTY_SETTINGS)) != 0) {
        mHandler->RemoveMessages(MSG_USER_ACTIVITY_TIMEOUT);

        Int64 nextTimeout = 0;
        if (mWakefulness != WAKEFULNESS_ASLEEP) {
            Int32 screenOffTimeout = GetScreenOffTimeoutLocked();
            Int32 screenDimDuration = GetScreenDimDurationLocked(screenOffTimeout);

            mUserActivitySummary = 0;
            if (mLastUserActivityTime >= mLastWakeTime) {
                nextTimeout = mLastUserActivityTime
                        + screenOffTimeout - screenDimDuration;
                if (now < nextTimeout) {
                    mUserActivitySummary |= USER_ACTIVITY_SCREEN_BRIGHT;
                }
                else {
                    nextTimeout = mLastUserActivityTime + screenOffTimeout;
                    if (now < nextTimeout) {
                        mUserActivitySummary |= USER_ACTIVITY_SCREEN_DIM;
                    }
                }
            }
            if (mUserActivitySummary == 0
                    && mLastUserActivityTimeNoChangeLights >= mLastWakeTime) {
                nextTimeout = mLastUserActivityTimeNoChangeLights + screenOffTimeout;
                if (now < nextTimeout
                        && mDisplayPowerRequest->mScreenState
                                != DisplayPowerRequest::SCREEN_STATE_OFF) {
                    mUserActivitySummary = mDisplayPowerRequest->mScreenState
                            == DisplayPowerRequest::SCREEN_STATE_BRIGHT ?
                            USER_ACTIVITY_SCREEN_BRIGHT : USER_ACTIVITY_SCREEN_DIM;
                }
            }
            if (mUserActivitySummary != 0) {
                AutoPtr<IMessage> msg;
                mHandler->ObtainMessage(MSG_USER_ACTIVITY_TIMEOUT, (IMessage**)&msg);
                msg->SetAsynchronous(TRUE);
                Boolean result;
                mHandler->SendMessageAtTime(msg, nextTimeout, &result);
            }
        }
        else {
            mUserActivitySummary = 0;
        }

        if (DEBUG_SPEW) {
            Slogger::D(TAG, "updateUserActivitySummaryLocked: mWakefulness=%s, mUserActivitySummary=0x%08x, nextTimeout="
                    , (const char*)WakefulnessToString(mWakefulness), mUserActivitySummary
                    /*, TimeUtils.formatUptime(nextTimeout)*/);
        }
    }
}

void CPowerManagerService::HandleUserActivityTimeout()
{
    // runs on handler thread
    Mutex::Autolock lock(_m_syncLock);
    if (DEBUG_SPEW) {
        Slogger::D(TAG, "handleUserActivityTimeout");
    }

    mDirty |= DIRTY_USER_ACTIVITY;
    UpdatePowerStateLocked();
}

Int32 CPowerManagerService::GetScreenOffTimeoutLocked()
{
    Int32 timeout = mScreenOffTimeoutSetting;
    if (IsMaximumScreenOffTimeoutFromDeviceAdminEnforcedLocked()) {
        timeout = Elastos::Core::Math::Min(timeout, mMaximumScreenOffTimeoutFromDeviceAdmin);
    }
    if (mUserActivityTimeoutOverrideFromWindowManager >= 0) {
        timeout = (Int32)Elastos::Core::Math::Min((Int64)timeout, mUserActivityTimeoutOverrideFromWindowManager);
    }
    /*add by chenjd, 2013-04-18, start {{---------------*/
    /* negative number means stay on as long as possible */
    Boolean screenoffEnable = FALSE;
    AutoPtr<ISystemProperties> sysProp;
    CSystemProperties::AcquireSingleton((ISystemProperties**)&sysProp);
    sysProp->GetBoolean(String("ro.sw.screenoffEnable"), FALSE, &screenoffEnable);
    if (timeout == -1 || !screenoffEnable){
        timeout = mMaximumScreenOffTimeoutFromDeviceAdmin;
    }
    /* add by chenjd, end -------------------------}} */

    return Elastos::Core::Math::Max(timeout, MINIMUM_SCREEN_OFF_TIMEOUT);
}

Int32 CPowerManagerService::GetScreenDimDurationLocked(
    /* [in] */ Int32 screenOffTimeout)
{
    return Elastos::Core::Math::Min(SCREEN_DIM_DURATION,
            (Int32)(screenOffTimeout * MAXIMUM_SCREEN_DIM_RATIO));
}

Boolean CPowerManagerService::UpdateWakefulnessLocked(
    /* [in] */ Int32 dirty)
{
    Boolean changed = FALSE;
    if ((dirty & (DIRTY_WAKE_LOCKS | DIRTY_USER_ACTIVITY | DIRTY_BOOT_COMPLETED
            | DIRTY_WAKEFULNESS | DIRTY_STAY_ON | DIRTY_PROXIMITY_POSITIVE
            | DIRTY_DOCK_STATE)) != 0) {
        if (mWakefulness == WAKEFULNESS_AWAKE && IsItBedTimeYetLocked()) {
            if (DEBUG_SPEW) {
                Slogger::D(TAG, "updateWakefulnessLocked: Bed time...");
            }
            Int64 time = SystemClock::GetUptimeMillis();
            if (ShouldNapAtBedTimeLocked()) {
                changed = NapNoUpdateLocked(time);
            }
            else {
                changed = GoToSleepNoUpdateLocked(time,
                        IPowerManager::GO_TO_SLEEP_REASON_TIMEOUT);
            }
        }
    }
    return changed;
}

Boolean CPowerManagerService::ShouldNapAtBedTimeLocked()
{
    return mDreamsActivateOnSleepSetting
            || (mDreamsActivateOnDockSetting
                    && mDockState != IIntent::EXTRA_DOCK_STATE_UNDOCKED);
}

Boolean CPowerManagerService::IsItBedTimeYetLocked()
{
    return mBootCompleted && !IsBeingKeptAwakeLocked();
}

Boolean CPowerManagerService::IsBeingKeptAwakeLocked()
{
    return mStayOn
            || mProximityPositive
            || (mWakeLockSummary & WAKE_LOCK_STAY_AWAKE) != 0
            || (mUserActivitySummary & (USER_ACTIVITY_SCREEN_BRIGHT
                    | USER_ACTIVITY_SCREEN_DIM)) != 0;
}

void CPowerManagerService::UpdateDreamLocked(
    /* [in] */ Int32 dirty)
{
    if ((dirty & (DIRTY_WAKEFULNESS
            | DIRTY_USER_ACTIVITY
            | DIRTY_WAKE_LOCKS
            | DIRTY_BOOT_COMPLETED
            | DIRTY_SETTINGS
            | DIRTY_IS_POWERED
            | DIRTY_STAY_ON
            | DIRTY_PROXIMITY_POSITIVE
            | DIRTY_BATTERY_STATE)) != 0) {
        ScheduleSandmanLocked();
    }
}

void CPowerManagerService::ScheduleSandmanLocked()
{
    if (!mSandmanScheduled) {
        mSandmanScheduled = TRUE;
        AutoPtr<IMessage> msg;
        mHandler->ObtainMessage(MSG_SANDMAN, (IMessage**)&msg);
        msg->SetAsynchronous(TRUE);
        Boolean result;
        mHandler->SendMessage(msg, &result);
    }
}

void CPowerManagerService::HandleSandman()
{
    // Handle preconditions.
    Boolean startDreaming = FALSE;
    {
        Mutex::Autolock lock(_m_syncLock);
        mSandmanScheduled = FALSE;
        Boolean canDream = CanDreamLocked();
        if (DEBUG_SPEW) {
            Slogger::D(TAG, "handleSandman: canDream=%d mWakefulness=%d"
                    , canDream, mWakefulness);
        }

        if (canDream && mWakefulness == WAKEFULNESS_NAPPING) {
            startDreaming = TRUE;
        }
    }

    // Start dreaming if needed.
    // We only control the dream on the handler thread, so we don't need to worry about
    // concurrent attempts to start or stop the dream.
    Boolean isDreaming = FALSE;
    // if (mDreamManager != NULL) {
    //     if (startDreaming) {
    //         ASSERT_SUCCEEDED(mDreamManager->StartDream());
    //     }
    //     mDreamManager->IsDreaming(&isDreaming);
    // }

    // Update dream state.
    // We might need to stop the dream again if the preconditions changed.
    Boolean continueDreaming = FALSE;
    {
        Mutex::Autolock lock(_m_syncLock);
        if (isDreaming && CanDreamLocked()) {
            if (mWakefulness == WAKEFULNESS_NAPPING) {
                mWakefulness = WAKEFULNESS_DREAMING;
                mDirty |= DIRTY_WAKEFULNESS;
                mBatteryLevelWhenDreamStarted = mBatteryLevel;
                UpdatePowerStateLocked();
                continueDreaming = TRUE;
            }
            else if (mWakefulness == WAKEFULNESS_DREAMING) {
                if (!IsBeingKeptAwakeLocked()
                        && mBatteryLevel < mBatteryLevelWhenDreamStarted
                                - DREAM_BATTERY_LEVEL_DRAIN_CUTOFF) {
                    // If the user activity timeout expired and the battery appears
                    // to be draining faster than it is charging then stop dreaming
                    // and go to sleep.
                    Slogger::I(TAG, "Stopping dream because the battery appears to be draining faster than it is charging.  Battery level when dream started: %d%%.  Battery level now: %d%%."
                            , mBatteryLevelWhenDreamStarted, mBatteryLevel);
                }
                else {
                    continueDreaming = TRUE;
                }
            }
        }
        if (!continueDreaming) {
            HandleDreamFinishedLocked();
        }
    }
    // Stop dreaming if needed.
    // It's possible that something else changed to make us need to start the dream again.
    // If so, then the power manager will have posted another message to the handler
    // to take care of it later.
    // if (mDreamManager != NULL) {
    //     if (!continueDreaming) {
    //         ASSERT_SUCCEEDED(mDreamManager->StopDream());
    //     }
    // }
}

Boolean CPowerManagerService::CanDreamLocked()
{
    return mDreamsSupportedConfig
            && mDreamsEnabledSetting
            && mDisplayPowerRequest->mScreenState != DisplayPowerRequest::SCREEN_STATE_OFF
            && mBootCompleted
            && (mIsPowered || IsBeingKeptAwakeLocked());
}

void CPowerManagerService::HandleDreamFinishedLocked()
{
    if (mWakefulness == WAKEFULNESS_NAPPING
            || mWakefulness == WAKEFULNESS_DREAMING) {
        if (IsItBedTimeYetLocked()) {
            GoToSleepNoUpdateLocked(SystemClock::GetUptimeMillis(),
                    IPowerManager::GO_TO_SLEEP_REASON_TIMEOUT);
            UpdatePowerStateLocked();
        }
        else {
            WakeUpNoUpdateLocked(SystemClock::GetUptimeMillis());
            UpdatePowerStateLocked();
        }
    }
}

void CPowerManagerService::HandleScreenOnBlockerReleased()
{
    Mutex::Autolock lock(_m_syncLock);
    mDirty |= DIRTY_SCREEN_ON_BLOCKER_RELEASED;
    UpdatePowerStateLocked();
}

void CPowerManagerService::UpdateDisplayPowerStateLocked(
    /* [in] */ Int32 dirty)
{
    if ((dirty & (DIRTY_WAKE_LOCKS | DIRTY_USER_ACTIVITY | DIRTY_WAKEFULNESS
            | DIRTY_ACTUAL_DISPLAY_POWER_STATE_UPDATED | DIRTY_BOOT_COMPLETED
            | DIRTY_SETTINGS | DIRTY_SCREEN_ON_BLOCKER_RELEASED)) != 0) {
        Int32 newScreenState = GetDesiredScreenPowerStateLocked();
        if (newScreenState != mDisplayPowerRequest->mScreenState) {
            if (newScreenState == DisplayPowerRequest::SCREEN_STATE_OFF
                    && mDisplayPowerRequest->mScreenState
                            != DisplayPowerRequest::SCREEN_STATE_OFF) {
                mLastScreenOffEventElapsedRealTime = SystemClock::GetElapsedRealtime();
            }

            mDisplayPowerRequest->mScreenState = newScreenState;
            NativeSetPowerState(
                    newScreenState != DisplayPowerRequest::SCREEN_STATE_OFF,
                    newScreenState == DisplayPowerRequest::SCREEN_STATE_BRIGHT);
        }

        Int32 screenBrightness = mScreenBrightnessSettingDefault;
        Float screenAutoBrightnessAdjustment = 0.0f;
        Boolean autoBrightness = (mScreenBrightnessModeSetting == ISettingsSystem::SCREEN_BRIGHTNESS_MODE_AUTOMATIC);
        if (IsValidBrightness(mScreenBrightnessOverrideFromWindowManager)) {
            screenBrightness = mScreenBrightnessOverrideFromWindowManager;
            autoBrightness = FALSE;
        }
        else if (IsValidBrightness(mTemporaryScreenBrightnessSettingOverride)) {
            screenBrightness = mTemporaryScreenBrightnessSettingOverride;
        }
        else if (IsValidBrightness(mScreenBrightnessSetting)) {
            screenBrightness = mScreenBrightnessSetting;
        }
        if (autoBrightness) {
            screenBrightness = mScreenBrightnessSettingDefault;
            if (IsValidAutoBrightnessAdjustment(
                    mTemporaryScreenAutoBrightnessAdjustmentSettingOverride)) {
                screenAutoBrightnessAdjustment =
                        mTemporaryScreenAutoBrightnessAdjustmentSettingOverride;
            }
            else if (IsValidAutoBrightnessAdjustment(
                    mScreenAutoBrightnessAdjustmentSetting)) {
                screenAutoBrightnessAdjustment = mScreenAutoBrightnessAdjustmentSetting;
            }
        }
        screenBrightness = Elastos::Core::Math::Max(Elastos::Core::Math::Min(screenBrightness,
                mScreenBrightnessSettingMaximum), mScreenBrightnessSettingMinimum);
        screenAutoBrightnessAdjustment = Elastos::Core::Math::Max(Elastos::Core::Math::Min(
                screenAutoBrightnessAdjustment, 1.0f), -1.0f);
        mDisplayPowerRequest->mScreenBrightness = screenBrightness;
        mDisplayPowerRequest->mScreenAutoBrightnessAdjustment =
                screenAutoBrightnessAdjustment;
        mDisplayPowerRequest->mUseAutoBrightness = autoBrightness;
        mDisplayPowerRequest->mUseProximitySensor = ShouldUseProximitySensorLocked();
        mDisplayPowerRequest->mBlockScreenOn = mScreenOnBlocker->IsHeld();
        mDisplayReady = mDisplayPowerController->RequestPowerState(mDisplayPowerRequest,
                mRequestWaitForNegativeProximity);
        mRequestWaitForNegativeProximity = FALSE;
        if (DEBUG_SPEW) {
            Slogger::D(TAG, "updateScreenStateLocked: mDisplayReady=%d, newScreenState=%d" \
                ", mWakefulness=%d, mWakeLockSummary=0x%08x, mUserActivitySummary=0x%08x, mBootCompleted=%d"
                , mDisplayReady, newScreenState, mWakefulness
                , mWakeLockSummary, mUserActivitySummary, mBootCompleted);
        }
    }
}

Boolean CPowerManagerService::IsValidBrightness(
    /* [in] */ Int32 value)
{
    return value >= 0 && value <= 255;
}

Boolean CPowerManagerService::IsValidAutoBrightnessAdjustment(
    /* [in] */ Float value)
{
    // Handles NaN by always returning false.
    return value >= -1.0f && value <= 1.0f;
}

Int32 CPowerManagerService::GetDesiredScreenPowerStateLocked()
{
    if (mWakefulness == WAKEFULNESS_ASLEEP || mWakefulness == WAKEFULNESS_BOOTFAST) {
        return DisplayPowerRequest::SCREEN_STATE_OFF;
    }

    if ((mWakeLockSummary & WAKE_LOCK_SCREEN_BRIGHT) != 0
            || (mUserActivitySummary & USER_ACTIVITY_SCREEN_BRIGHT) != 0
            || !mBootCompleted) {
        return DisplayPowerRequest::SCREEN_STATE_BRIGHT;
    }

    return DisplayPowerRequest::SCREEN_STATE_DIM;
}

Boolean CPowerManagerService::ShouldUseProximitySensorLocked()
{
    return (mWakeLockSummary & WAKE_LOCK_PROXIMITY_SCREEN_OFF) != 0;
}

void CPowerManagerService::UpdateSuspendBlockerLocked()
{
    Boolean wantCpu = IsCpuNeededLocked();
    if (wantCpu != mHoldingWakeLockSuspendBlocker) {
        mHoldingWakeLockSuspendBlocker = wantCpu;
        if (wantCpu) {
            if (DEBUG) {
                Slogger::D(TAG, "updateSuspendBlockerLocked: Acquiring suspend blocker.");
            }
            mWakeLockSuspendBlocker->Acquire();
        }
        else {
            if (DEBUG) {
                Slogger::D(TAG, "updateSuspendBlockerLocked: Releasing suspend blocker.");
            }
            mWakeLockSuspendBlocker->ReleaseEx();
        }
    }
}

void CPowerManagerService::UpdateBootFastSuspendBlockerLocked()
{
    if(mWakeLockSuspendBlocker!= NULL){
        if(mBootFastStats){
            if(mBootFastWakeLockToggle)
                mWakeLockSuspendBlocker->BootFastSleepRelease();
            else
                mWakeLockSuspendBlocker->BootFastWakeAcquire();
        }
    }
}

Boolean CPowerManagerService::IsCpuNeededLocked()
{
    return !mBootCompleted
            || mWakeLockSummary != 0
            || mUserActivitySummary != 0
            || mDisplayPowerRequest->mScreenState != DisplayPowerRequest::SCREEN_STATE_OFF
            || !mDisplayReady;
}

ECode CPowerManagerService::IsScreenOn(
    /* [out] */ Boolean* isScreenOn)
{
    VALIDATE_NOT_NULL(isScreenOn);
    Int64 ident = Binder::ClearCallingIdentity();
    // try {
    *isScreenOn = IsScreenOnInternal();
    // } finally {
    Binder::RestoreCallingIdentity(ident);
    // }
    return NOERROR;
}

Boolean CPowerManagerService::IsScreenOnInternal()
{
    Mutex::Autolock lock(_m_syncLock);
    return !mSystemReady
            || mDisplayPowerRequest->mScreenState != DisplayPowerRequest::SCREEN_STATE_OFF;
}

void CPowerManagerService::HandleBatteryStateChangedLocked()
{
    mDirty |= DIRTY_BATTERY_STATE;
    UpdatePowerStateLocked();
}

void CPowerManagerService::StartWatchingForBootAnimationFinished()
{
    Boolean result;
    mHandler->SendEmptyMessage(MSG_CHECK_IF_BOOT_ANIMATION_FINISHED, &result);
}

void CPowerManagerService::CheckIfBootAnimationFinished()
{
    if (DEBUG) {
        Slogger::D(TAG, "Check if boot animation finished...");
    }

    AutoPtr<ISystemService> systemService;
    CSystemService::AcquireSingleton((ISystemService**)&systemService);
    Boolean isRunning;
    if (systemService->IsRunning(BOOT_ANIMATION_SERVICE, &isRunning), isRunning) {
        Boolean result;
        mHandler->SendEmptyMessageDelayed(
            MSG_CHECK_IF_BOOT_ANIMATION_FINISHED,
            BOOT_ANIMATION_POLL_INTERVAL,
            &result);
        return;
    }

    Mutex::Autolock lock(_m_syncLock);
    if (!mBootCompleted) {
        Slogger::I(TAG, "Boot animation finished.");
        HandleBootCompletedLocked();
    }
}

void CPowerManagerService::HandleBootCompletedLocked()
{
    Int64 now = SystemClock::GetUptimeMillis();
    mBootCompleted = TRUE;
    mDirty |= DIRTY_BOOT_COMPLETED;
    UserActivityNoUpdateLocked(
            now, IPowerManager::USER_ACTIVITY_EVENT_OTHER, 0, IProcess::SYSTEM_UID);
    UpdatePowerStateLocked();
}

ECode CPowerManagerService::Reboot(
    /* [in] */ Boolean confirm,
    /* [in] */ const String& reason,
    /* [in] */ Boolean wait)
{
    FAIL_RETURN(mContext->EnforceCallingOrSelfPermission(
            Elastos::Droid::Manifest::Permission::REBOOT, String(NULL)));

    Int64 ident = Binder::ClearCallingIdentity();
    // try {
    ECode ec = ShutdownOrRebootInternal(FALSE, confirm, reason, wait);
    // } finally {
    Binder::RestoreCallingIdentity(ident);
    // }
    return ec;
}

ECode CPowerManagerService::Shutdown(
    /* [in] */ Boolean confirm,
    /* [in] */ Boolean wait)
{
    FAIL_RETURN(mContext->EnforceCallingOrSelfPermission(
            Elastos::Droid::Manifest::Permission::REBOOT, String(NULL)));

    Int64 ident = Binder::ClearCallingIdentity();
    // try {
    ECode ec = ShutdownOrRebootInternal(TRUE, confirm, String(NULL), wait);
    // } finally {
    Binder::RestoreCallingIdentity(ident);
    // }
    return ec;
}


CPowerManagerService::ShutdownOrRebootRunnable::ShutdownOrRebootRunnable(
    /* [in] */ Boolean shutdown,
    /* [in] */ Boolean confirm,
    /* [in] */ const String& reason,
    /* [in] */ IContext* ctx)
    : mShutdown(shutdown)
    , mConfirm(confirm)
    , mReason(reason)
    , mContext(ctx)
{}

ECode CPowerManagerService::ShutdownOrRebootRunnable::Run()
{
    Lock();
    if (mShutdown) {
        //ShutdownThread.shutdown(mContext, confirm);\
        ShutdownThread::Shutdown(mContext, mConfirm, mPolicy);
    }
    else {
        ShutdownThread::Reboot(mContext, mReason, mConfirm);
    }
    Unlock();
    return NOERROR;
}


ECode CPowerManagerService::ShutdownOrRebootInternal(
    /* [in] */ const Boolean shutdown,
    /* [in] */ const Boolean confirm,
    /* [in] */ const String& reason,
    /* [in] */ Boolean wait)
{
    if (mHandler == NULL || !mSystemReady) {
        Slogger::E(TAG, "Too early to call shutdown() or reboot()");
        return E_ILLEGAL_STATE_EXCEPTION;
        // throw new IllegalStateException("Too early to call shutdown() or reboot()");
    }

    // ShutdownThread must run on a looper capable of displaying the UI.
    AutoPtr<ShutdownOrRebootRunnable> runnable = new ShutdownOrRebootRunnable(
            shutdown, confirm, reason, mContext);
    Boolean result;
    mHandler->Post(runnable, &result);

    // PowerManager.reboot() is documented not to return so just wait for the inevitable.
    if (wait) {
        runnable->Lock();
        while (TRUE) {
            // try {
            runnable->Wait();
            // } catch (InterruptedException e) {
            // }
        }
        runnable->Unlock();
    }
    return NOERROR;
}

ECode CPowerManagerService::Crash(
    /* [in] */ const String& message)
{
    FAIL_RETURN(mContext->EnforceCallingOrSelfPermission(
            Elastos::Droid::Manifest::Permission::REBOOT, String(NULL)));

    Int64 ident = Binder::ClearCallingIdentity();
    // try {
    ECode ec = CrashInternal(message);
    // } finally {
    Binder::RestoreCallingIdentity(ident);
    // }
    return ec;
}


CPowerManagerService::CrashThread::CrashThread(
    /* [in] */ const String& s)
{
    Thread::Init(s);
}

ECode CPowerManagerService::CrashThread::Run()
{
    Slogger::E(String("CPowerManagerService"), "PowerManagerService.crash()");
    return E_RUNTIME_EXCEPTION;
}


ECode CPowerManagerService::CrashInternal(
    /* [in] */ const String& message)
{
    AutoPtr<IThread> t = (IThread*)new CrashThread(String("PowerManagerService.crash()"));
    // try {
    FAIL_RETURN(t->Start());
    return t->Join();
    // } catch (InterruptedException e) {
    //     Log.wtf(TAG, e);
    // }
}

ECode CPowerManagerService::SetStayOnSetting(
    /* [in] */ Int32 val)
{
    FAIL_RETURN(mContext->EnforceCallingOrSelfPermission(
        Elastos::Droid::Manifest::Permission::WRITE_SETTINGS, String(NULL)));

    Int64 ident = Binder::ClearCallingIdentity();
    // try {
    SetStayOnSettingInternal(val);
    // } finally {
    Binder::RestoreCallingIdentity(ident);
    // }
    return NOERROR;
}

void CPowerManagerService::SetStayOnSettingInternal(
    /* [in] */ Int32 val)
{
    AutoPtr<IContentResolver> resolver;
    mContext->GetContentResolver((IContentResolver**)&resolver);
    Boolean result;
    AutoPtr<ISettingsGlobal> settingsGlobal;
    CSettingsGlobal::AcquireSingleton((ISettingsGlobal**)&settingsGlobal);
    settingsGlobal->PutInt32(resolver,
            ISettingsGlobal::STAY_ON_WHILE_PLUGGED_IN,
            val, &result);
}

ECode CPowerManagerService::SetMaximumScreenOffTimeoutFromDeviceAdmin(
    /* [in] */ Int32 timeMs)
{
    Int64 ident = Binder::ClearCallingIdentity();
    // try {
    SetMaximumScreenOffTimeoutFromDeviceAdminInternal(timeMs);
    // } finally {
    Binder::RestoreCallingIdentity(ident);
    // }
    return NOERROR;
}

void CPowerManagerService::SetMaximumScreenOffTimeoutFromDeviceAdminInternal(
    /* [in] */ Int32 timeMs)
{
    Mutex::Autolock lock(_m_syncLock);
    mMaximumScreenOffTimeoutFromDeviceAdmin = timeMs;
    mDirty |= DIRTY_SETTINGS;
    UpdatePowerStateLocked();
}

Boolean CPowerManagerService::IsMaximumScreenOffTimeoutFromDeviceAdminEnforcedLocked()
{
    return mMaximumScreenOffTimeoutFromDeviceAdmin >= 0
            && mMaximumScreenOffTimeoutFromDeviceAdmin < 0x7FFFFFFF /*Integer.MAX_VALUE*/;
}

ECode CPowerManagerService::SetAttentionLight(
    /* [in] */ Boolean on,
    /* [in] */ Int32 color)
{
    FAIL_RETURN(mContext->EnforceCallingOrSelfPermission(
            Elastos::Droid::Manifest::Permission::DEVICE_POWER, String(NULL)));

    Int64 ident = Binder::ClearCallingIdentity();
    // try {
    SetAttentionLightInternal(on, color);
    // } finally {
    Binder::RestoreCallingIdentity(ident);
    // }
    return NOERROR;
}

void CPowerManagerService::SetAttentionLightInternal(
    /* [in] */ Boolean on,
    /* [in] */ Int32 color)
{
    AutoPtr<LightsService::Light> light;
    {
        Mutex::Autolock lock(_m_syncLock);
        if (!mSystemReady) {
            return;
        }
        light = mAttentionLight;
    }

    // Control light outside of lock.
    light->SetFlashing(color, LightsService::_LIGHT_FLASH_HARDWARE,
            (on ? 3 : 0), 0);
}

ECode CPowerManagerService::GoToBootFastSleep(
    /* [in] */ Int64 eventTime)
{
    if(DEBUG_BOOTFAST)
        Slogger::D(TAG, "goToBootFastSleep");
    if (eventTime > SystemClock::GetUptimeMillis()) {
        Slogger::E(TAG, "event time must not be in the future");
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
        // throw new IllegalArgumentException("event time must not be in the future");
    }

    FAIL_RETURN(mContext->EnforceCallingOrSelfPermission(
            Elastos::Droid::Manifest::Permission::DEVICE_POWER, String(NULL)));

    Int64 ident = Binder::ClearCallingIdentity();
    // try {
    GoToBootFastSleepInternal(eventTime);
    // } finally {
    Binder::RestoreCallingIdentity(ident);
    // }
    return NOERROR;
}

void CPowerManagerService::GoToBootFastSleepInternal(
    /* [in] */ Int64 eventTime)
{
    if(DEBUG_BOOTFAST)
        Slogger::D(TAG, "goToBootFastSleepInternal");
    Mutex::Autolock lock(_m_syncLock);
    if (GoToBootFastSleepNoUpdateLocked(eventTime)) {
        mPolicy->HideScreen(TRUE);
        UpdatePowerStateLocked();
    }
}

Boolean CPowerManagerService::GoToBootFastSleepNoUpdateLocked(
    /* [in] */ Int64 eventTime)
{
    if(DEBUG_BOOTFAST)
        Slogger::D(TAG, "goToSleepNoUpdateLocked");

    if(mBatteryLevel <= 3){
        Slogger::D(TAG, "goToBootFastSleep Battery too low real shutdown");
        NativeShutdown();
    }

    mDirty |= DIRTY_WAKEFULNESS;
    mWakefulness = WAKEFULNESS_BOOTFAST;

    //sendPendingNotificationsLocked();
    //mNotifier.onGoToSleepStarted(PowerManager.GO_TO_SLEEP_REASON_USER);
    //mSendGoToSleepFinishedNotificationWhenReady = true;

    mNotifier->OnBootFastSleep();

    AutoPtr<ISystem> system;
    Elastos::Core::CSystem::AcquireSingleton((ISystem**)&system);
    Int64 now;
    system->GetCurrentTimeMillis(&now);

    mBootFastStats = TRUE;
    mBootFastWakeLockToggle = TRUE;
    mBootFastSleepClockStart = now;
    mBootFastSleepTimeStart = SystemClock::GetUptimeMillis();
    return TRUE;
}

ECode CPowerManagerService::BootFastWake(
    /* [in] */ Int64 eventTime)
{
    if(DEBUG_BOOTFAST)
        Slogger::D(TAG, "bootFastWake");

    AutoPtr<ISystem> system;
    Elastos::Core::CSystem::AcquireSingleton((ISystem**)&system);
    Int64 now;
    system->GetCurrentTimeMillis(&now);
    if (eventTime > now) {
        Slogger::E(TAG, "event time must not be in the future");
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
        // throw new IllegalArgumentException("event time must not be in the future");
    }

    FAIL_RETURN(mContext->EnforceCallingOrSelfPermission(
            Elastos::Droid::Manifest::Permission::DEVICE_POWER, String(NULL)));

    Int64 ident = Binder::ClearCallingIdentity();
    // try {
    BootFastWakeInternal(eventTime);
    // } finally {
    Binder::RestoreCallingIdentity(ident);
    // }
    return NOERROR;
}

void CPowerManagerService::BootFastWakeInternal(
    /* [in] */ Int64 eventTime)
{
    if(DEBUG_BOOTFAST)
        Slogger::D(TAG, "bootFastWakeInternal");
    Mutex::Autolock lock(_m_syncLock);
    if (BootFastWakeNoUpdateLocked(eventTime)) {
        UpdatePowerStateLocked();
    }
}

Boolean CPowerManagerService::BootFastWakeNoUpdateLocked(
    /* [in] */ Int64 eventTime)
{
    if(DEBUG_BOOTFAST)
        Slogger::D(TAG, "bootFastWakeNoUpdateLocked");
    mDirty |= DIRTY_WAKEFULNESS;
    mWakefulness = WAKEFULNESS_AWAKE;
    mNotifier->OnBootFastWake();
    if(mBootFastTempWakeStatus) {
        mHandler->RemoveMessages(MSG_TEMP_WAKUP_RESLEEP);
    }
    UserActivityNoUpdateLocked(
            eventTime, IPowerManager::USER_ACTIVITY_EVENT_OTHER, 0, IProcess::SYSTEM_UID);
    mBootFastStats = FALSE;
    mBootFastWakeLockToggle = FALSE;
    return TRUE;
}

void CPowerManagerService::HandleTempWakupResleep()
{
    mPolicy->HideScreen(TRUE);
    mBootFastTempWakeStatus= FALSE;
    GoToBootFastSleep(SystemClock::GetUptimeMillis());
}

ECode CPowerManagerService::IsBootFastStatus(
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result)
    if(DEBUG_BOOTFAST)
        Slogger::D(TAG, "isBootFastStatus = %d", mBootFastStats);
    *result = mBootFastStats;
    return NOERROR;
}

ECode CPowerManagerService::IsBootFastWakeFromStandby(
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result)
    *result = FALSE;

    using Elastos::Core::Math;

    if(DEBUG_BOOTFAST)
        Slogger::D(TAG,"isBootFastWakeFromStandby");
    AutoPtr<ISystem> system;
    Elastos::Core::CSystem::AcquireSingleton((ISystem**)&system);
    Int64 bootFastSleepClockEnd;
    system->GetCurrentTimeMillis(&bootFastSleepClockEnd);
    Int64 bootFastSleepTimeEnd = SystemClock::GetUptimeMillis();

    if(!mBootFastStats) {
        *result = FALSE;
    }
    else{
        Int64 time = Math::Abs(Math::Abs(bootFastSleepClockEnd - mBootFastSleepClockStart) -
            Math::Abs(bootFastSleepTimeEnd - mBootFastSleepTimeStart));
        mBootFastSleepTimeStart = SystemClock::GetUptimeMillis();
        system->GetCurrentTimeMillis(&mBootFastSleepClockStart);
        if(DEBUG_BOOTFAST)
               Slogger::D(TAG, "standby time = %d", time);
        if(time > 2000) {
            *result = TRUE;
        }
        else {
            *result = FALSE;
        }
    }
    return NOERROR;
}

void CPowerManagerService::TempWakeUp()
{
    mBootFastTempWakeStatus = TRUE;
    if(DEBUG_BOOTFAST)
        Slogger::D(TAG, "tempWakeUp current ");
    //mPolicy.hideScreen(true);
    WakeUpInternal(SystemClock::GetUptimeMillis());
    mPolicy->ShowPowerCharge(mBatteryLevel);
    AutoPtr<ISystem> system;
    Elastos::Core::CSystem::AcquireSingleton((ISystem**)&system);
    Int64 now;
    system->GetCurrentTimeMillis(&now);
    mBootFastSleepClockStart = now;
    mBootFastSleepTimeStart = SystemClock::GetUptimeMillis();
    Boolean result;
    mHandler->SendEmptyMessageDelayed(MSG_TEMP_WAKUP_RESLEEP, 4500, &result);
    //goToBootFastSleep(SystemClock.uptimeMillis());
    return;
}

void CPowerManagerService::TempWakeUpFromNative(
    /* [in] */ Int64 eventTime)
{
    if(DEBUG_BOOTFAST)
        Slogger::D(TAG, "tempWakeUpFromNative");
    TempWakeUp();
}

ECode CPowerManagerService::TimeSinceScreenWasLastOn(
    /* [out] */ Int64* time)
{
    VALIDATE_NOT_NULL(time);
    Mutex::Autolock lock(_m_syncLock);
    if (mDisplayPowerRequest->mScreenState != DisplayPowerRequest::SCREEN_STATE_OFF) {
        *time = 0;
        return NOERROR;
    }

    *time = SystemClock::GetElapsedRealtime() - mLastScreenOffEventElapsedRealTime;
    return NOERROR;
}

ECode CPowerManagerService::SetScreenBrightnessOverrideFromWindowManager(
    /* [in] */ Int32 brightness)
{
    FAIL_RETURN(mContext->EnforceCallingOrSelfPermission(
            Elastos::Droid::Manifest::Permission::DEVICE_POWER, String(NULL)));

    Int64 ident = Binder::ClearCallingIdentity();
    // try {
    SetScreenBrightnessOverrideFromWindowManagerInternal(brightness);
    // } finally {
    Binder::RestoreCallingIdentity(ident);
    // }
    return NOERROR;
}

void CPowerManagerService::SetScreenBrightnessOverrideFromWindowManagerInternal(
        /* [in] */ Int32 brightness)
{
    Mutex::Autolock lock(_m_syncLock);
    if (mScreenBrightnessOverrideFromWindowManager != brightness) {
        mScreenBrightnessOverrideFromWindowManager = brightness;
        mDirty |= DIRTY_SETTINGS;
        UpdatePowerStateLocked();
    }
}

ECode CPowerManagerService::SetButtonBrightnessOverrideFromWindowManager(
    /* [in] */ Int32 brightness)
{
    // Do nothing.
    // Button lights are not currently supported in the new implementation.
    return mContext->EnforceCallingOrSelfPermission(
            Elastos::Droid::Manifest::Permission::DEVICE_POWER, String(NULL));
}

ECode CPowerManagerService::SetUserActivityTimeoutOverrideFromWindowManager(
    /* [in] */ Int64 timeoutMillis)
{
    FAIL_RETURN(mContext->EnforceCallingOrSelfPermission(
            Elastos::Droid::Manifest::Permission::DEVICE_POWER, String(NULL)));

    Int64 ident = Binder::ClearCallingIdentity();
    // try {
    SetUserActivityTimeoutOverrideFromWindowManagerInternal(timeoutMillis);
    // } finally {
    Binder::RestoreCallingIdentity(ident);
    // }
    return NOERROR;
}

void CPowerManagerService::SetUserActivityTimeoutOverrideFromWindowManagerInternal(
    /* [in] */ Int64 timeoutMillis)
{
    Mutex::Autolock lock(_m_syncLock);
    if (mUserActivityTimeoutOverrideFromWindowManager != timeoutMillis) {
        mUserActivityTimeoutOverrideFromWindowManager = timeoutMillis;
        mDirty |= DIRTY_SETTINGS;
        UpdatePowerStateLocked();
    }
}

ECode CPowerManagerService::SetTemporaryScreenBrightnessSettingOverride(
    /* [in] */ Int32 brightness)
{
    FAIL_RETURN(mContext->EnforceCallingOrSelfPermission(
            Elastos::Droid::Manifest::Permission::DEVICE_POWER, String(NULL)));

    Int64 ident = Binder::ClearCallingIdentity();
    // try {
    SetTemporaryScreenBrightnessSettingOverrideInternal(brightness);
    // } finally {
    Binder::RestoreCallingIdentity(ident);
    // }
    return NOERROR;
}

void CPowerManagerService::SetTemporaryScreenBrightnessSettingOverrideInternal(
    /* [in] */ Int32 brightness)
{
    Mutex::Autolock lock(_m_syncLock);
    if (mTemporaryScreenBrightnessSettingOverride != brightness) {
        mTemporaryScreenBrightnessSettingOverride = brightness;
        mDirty |= DIRTY_SETTINGS;
        UpdatePowerStateLocked();
    }
}

ECode CPowerManagerService::SetTemporaryScreenAutoBrightnessAdjustmentSettingOverride(
    /* [in] */ Float adj)
{
    FAIL_RETURN(mContext->EnforceCallingOrSelfPermission(
            Elastos::Droid::Manifest::Permission::DEVICE_POWER, String(NULL)));

    Int64 ident = Binder::ClearCallingIdentity();
    // try {
    SetTemporaryScreenAutoBrightnessAdjustmentSettingOverrideInternal(adj);
    // } finally {
    Binder::RestoreCallingIdentity(ident);
    // }
    return NOERROR;
}

void CPowerManagerService::SetTemporaryScreenAutoBrightnessAdjustmentSettingOverrideInternal(
    /* [in] */ Float adj)
{
    Mutex::Autolock lock(_m_syncLock);
    // Note: This condition handles NaN because NaN is not equal to any other
    // value, including itself.
    if (mTemporaryScreenAutoBrightnessAdjustmentSettingOverride != adj) {
        mTemporaryScreenAutoBrightnessAdjustmentSettingOverride = adj;
        mDirty |= DIRTY_SETTINGS;
        UpdatePowerStateLocked();
    }
}

void CPowerManagerService::LowLevelShutdown()
{
    NativeShutdown();
}

ECode CPowerManagerService::LowLevelReboot(
    /* [in] */ const String& reason)
{
    return NativeReboot(reason);
}

// ECode CPowerManagerService::Monitor()
// {
//     // Grab and release lock for watchdog monitor to detect deadlocks.
//     Mutex::Autolock lock(_m_syncLock);
//     return NOERROR;
// }

// void CPowerManagerService::Dump(
//     /* [in] */ IFileDescriptor* fd,
//     /* [in] */ IPrintWriter* pw,
//     /* [in] */ ArrayOf<String>* args)
// {
//     if (mContext.checkCallingOrSelfPermission(Elastos::Droid::Manifest::Permission::DUMP)
//             != PackageManager.PERMISSION_GRANTED) {
//         pw.println("Permission Denial: can't dump PowerManager from from pid="
//                 + Binder.getCallingPid()
//                 + ", uid=" + Binder.getCallingUid());
//         return;
//     }

//     pw.println("POWER MANAGER (dumpsys power)\n");

//     final DisplayPowerController dpc;
//     final WirelessChargerDetector wcd;
//     synchronized (mLock) {
//         pw.println("Power Manager State:");
//         pw.println("  mDirty=0x" + Integer.toHexString(mDirty));
//         pw.println("  mWakefulness=" + wakefulnessToString(mWakefulness));
//         pw.println("  mIsPowered=" + mIsPowered);
//         pw.println("  mPlugType=" + mPlugType);
//         pw.println("  mBatteryLevel=" + mBatteryLevel);
//         pw.println("  mBatteryLevelWhenDreamStarted=" + mBatteryLevelWhenDreamStarted);
//         pw.println("  mDockState=" + mDockState);
//         pw.println("  mStayOn=" + mStayOn);
//         pw.println("  mBootFastStats=" + mBootFastStats);
//         pw.println("  mProximityPositive=" + mProximityPositive);
//         pw.println("  mBootCompleted=" + mBootCompleted);
//         pw.println("  mSystemReady=" + mSystemReady);
//         pw.println("  mWakeLockSummary=0x" + Integer.toHexString(mWakeLockSummary));
//         pw.println("  mUserActivitySummary=0x" + Integer.toHexString(mUserActivitySummary));
//         pw.println("  mRequestWaitForNegativeProximity=" + mRequestWaitForNegativeProximity);
//         pw.println("  mSandmanScheduled=" + mSandmanScheduled);
//         pw.println("  mLastWakeTime=" + TimeUtils.formatUptime(mLastWakeTime));
//         pw.println("  mLastSleepTime=" + TimeUtils.formatUptime(mLastSleepTime));
//         pw.println("  mSendWakeUpFinishedNotificationWhenReady="
//                 + mSendWakeUpFinishedNotificationWhenReady);
//         pw.println("  mSendGoToSleepFinishedNotificationWhenReady="
//                 + mSendGoToSleepFinishedNotificationWhenReady);
//         pw.println("  mLastUserActivityTime=" + TimeUtils.formatUptime(mLastUserActivityTime));
//         pw.println("  mLastUserActivityTimeNoChangeLights="
//                 + TimeUtils.formatUptime(mLastUserActivityTimeNoChangeLights));
//         pw.println("  mDisplayReady=" + mDisplayReady);
//         pw.println("  mHoldingWakeLockSuspendBlocker=" + mHoldingWakeLockSuspendBlocker);

//         pw.println();
//         pw.println("Settings and Configuration:");
//         pw.println("  mDreamsSupportedConfig=" + mDreamsSupportedConfig);
//         pw.println("  mDreamsEnabledSetting=" + mDreamsEnabledSetting);
//         pw.println("  mDreamsActivateOnSleepSetting=" + mDreamsActivateOnSleepSetting);
//         pw.println("  mDreamsActivateOnDockSetting=" + mDreamsActivateOnDockSetting);
//         pw.println("  mScreenOffTimeoutSetting=" + mScreenOffTimeoutSetting);
//         pw.println("  mMaximumScreenOffTimeoutFromDeviceAdmin="
//                 + mMaximumScreenOffTimeoutFromDeviceAdmin + " (enforced="
//                 + isMaximumScreenOffTimeoutFromDeviceAdminEnforcedLocked() + ")");
//         pw.println("  mStayOnWhilePluggedInSetting=" + mStayOnWhilePluggedInSetting);
//         pw.println("  mScreenBrightnessSetting=" + mScreenBrightnessSetting);
//         pw.println("  mScreenAutoBrightnessAdjustmentSetting="
//                 + mScreenAutoBrightnessAdjustmentSetting);
//         pw.println("  mScreenBrightnessModeSetting=" + mScreenBrightnessModeSetting);
//         pw.println("  mScreenBrightnessOverrideFromWindowManager="
//                 + mScreenBrightnessOverrideFromWindowManager);
//         pw.println("  mUserActivityTimeoutOverrideFromWindowManager="
//                 + mUserActivityTimeoutOverrideFromWindowManager);
//         pw.println("  mTemporaryScreenBrightnessSettingOverride="
//                 + mTemporaryScreenBrightnessSettingOverride);
//         pw.println("  mTemporaryScreenAutoBrightnessAdjustmentSettingOverride="
//                 + mTemporaryScreenAutoBrightnessAdjustmentSettingOverride);
//         pw.println("  mScreenBrightnessSettingMinimum=" + mScreenBrightnessSettingMinimum);
//         pw.println("  mScreenBrightnessSettingMaximum=" + mScreenBrightnessSettingMaximum);
//         pw.println("  mScreenBrightnessSettingDefault=" + mScreenBrightnessSettingDefault);

//         final int screenOffTimeout = getScreenOffTimeoutLocked();
//         final int screenDimDuration = getScreenDimDurationLocked(screenOffTimeout);
//         pw.println();
//         pw.println("Screen off timeout: " + screenOffTimeout + " ms");
//         pw.println("Screen dim duration: " + screenDimDuration + " ms");

//         pw.println();
//         pw.println("Wake Locks: size=" + mWakeLocks.size());
//         for (WakeLock wl : mWakeLocks) {
//             pw.println("  " + wl);
//         }

//         pw.println();
//         pw.println("Suspend Blockers: size=" + mSuspendBlockers.size());
//         for (SuspendBlocker sb : mSuspendBlockers) {
//             pw.println("  " + sb);
//         }

//         pw.println();
//         pw.println("Screen On Blocker: " + mScreenOnBlocker);

//         pw.println();
//         pw.println("Display Blanker: " + mDisplayBlanker);

//         dpc = mDisplayPowerController;
//         wcd = mWirelessChargerDetector;
//     }

//     if (dpc != null) {
//         dpc.dump(pw);
//     }
//      if (wcd != null) {
//          wcd.dump(pw);
//      }
// }

AutoPtr<ISuspendBlocker> CPowerManagerService::CreateSuspendBlockerLocked(
    /* [in] */ const String& name)
{
    AutoPtr<ISuspendBlocker> suspendBlocker = (ISuspendBlocker*)new SuspendBlockerImpl(name);
    mSuspendBlockers.PushBack(suspendBlocker);
    return suspendBlocker;
}

String CPowerManagerService::WakefulnessToString(
    /* [in] */ Int32 wakefulness)
{
    switch (wakefulness) {
        case WAKEFULNESS_ASLEEP:
            return String("Asleep");
        case WAKEFULNESS_AWAKE:
            return String("Awake");
        case WAKEFULNESS_DREAMING:
            return String("Dreaming");
        case WAKEFULNESS_NAPPING:
            return String("Napping");
        case WAKEFULNESS_BOOTFAST:
            return String("Bootfast");
        default:
            return StringUtils::Int32ToString(wakefulness);
    }
}

AutoPtr<IWorkSource> CPowerManagerService::CopyWorkSource(
    /* [in] */ IWorkSource* workSource)
{
    if (workSource != NULL) {
        AutoPtr<IWorkSource> newWorkSource;
        ASSERT_SUCCEEDED(CWorkSource::New(workSource, (IWorkSource**)&newWorkSource));
        return newWorkSource;
    }
    else {
        return NULL;
    }
}

ECode CPowerManagerService::constructor()
{
    {
        Mutex::Autolock lock(_m_syncLock);
        mWakeLockSuspendBlocker = CreateSuspendBlockerLocked(String("PowerManagerService"));
        mWakeLockSuspendBlocker->Acquire();
        mScreenOnBlocker = new ScreenOnBlockerImpl(this);
        mDisplayBlanker = new DisplayBlankerImpl(this);
        mHoldingWakeLockSuspendBlocker = TRUE;
        mWakefulness = WAKEFULNESS_AWAKE;
    }
    NativeInit();
    NativeSetPowerState(TRUE, TRUE);
    return NOERROR;
}


} // namespace Power
} // namespace Server
} // namespace Droid
} // namespace Elastos
