
#include "ext/frameworkext.h"
#include "os/CPowerManager.h"
#include "os/SystemProperties.h"
#include "R.h"
#include <elastos/Slogger.h>

using Elastos::Utility::Logging::Slogger;
using Elastos::Droid::Content::Res::IResources;

namespace Elastos {
namespace Droid {
namespace Os {

const String CPowerManager::TAG("PowerManager");

ECode CPowerManager::GetMinimumScreenBrightnessSetting(
    /* [out] */ Int32* screenBrightness)
{
    VALIDATE_NOT_NULL(screenBrightness);
    AutoPtr<IResources> res;
    FAIL_RETURN(mContext->GetResources((IResources**)&res));
    return res->GetInteger(R::integer::config_screenBrightnessSettingMinimum, screenBrightness);
}

ECode CPowerManager::GetMaximumScreenBrightnessSetting(
    /* [out] */ Int32* screenBrightness)
{
    VALIDATE_NOT_NULL(screenBrightness);
    AutoPtr<IResources> res;
    FAIL_RETURN(mContext->GetResources((IResources**)&res));
    return res->GetInteger(R::integer::config_screenBrightnessSettingMaximum, screenBrightness);
}

ECode CPowerManager::GetDefaultScreenBrightnessSetting(
    /* [out] */ Int32* screenBrightness)
{
    VALIDATE_NOT_NULL(screenBrightness);
    AutoPtr<IResources> res;
    FAIL_RETURN(mContext->GetResources((IResources**)&res));
    return res->GetInteger(R::integer::config_screenBrightnessSettingDefault, screenBrightness);
}

Boolean CPowerManager::UseScreenAutoBrightnessAdjustmentFeature()
{
    return SystemProperties::GetBoolean(String("persist.power.useautobrightadj"), FALSE);
}

Boolean CPowerManager::UseTwilightAdjustmentFeature()
{
    return SystemProperties::GetBoolean(String("persist.power.usetwilightadj"), FALSE);
}

ECode CPowerManager::NewWakeLock(
    /* [in] */ Int32 levelAndFlags,
    /* [in] */ const String& tag,
    /* [out] */ IPowerManagerWakeLock** wakeLock)
{
    VALIDATE_NOT_NULL(wakeLock);
    FAIL_RETURN(ValidateWakeLockParameters(levelAndFlags, tag));
    return CPowerManagerWakeLock::New(levelAndFlags, tag, (Handle32)this, wakeLock);
}

ECode CPowerManager::ValidateWakeLockParameters(
    /* [in] */ Int32 levelAndFlags,
    /* [in] */ const String& tag)
{
    switch (levelAndFlags & WAKE_LOCK_LEVEL_MASK) {
        case PARTIAL_WAKE_LOCK:
        case SCREEN_DIM_WAKE_LOCK:
        case SCREEN_BRIGHT_WAKE_LOCK:
        case FULL_WAKE_LOCK:
        case PROXIMITY_SCREEN_OFF_WAKE_LOCK:
            break;
        default:
            Slogger::E(TAG, "Must specify a valid wake lock level.");
            return E_ILLEGAL_ARGUMENT_EXCEPTION;
            // throw new IllegalArgumentException("Must specify a valid wake lock level.");
    }
    if (tag.IsNull()) {
        Slogger::E(TAG, "The tag must not be null.");
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
        // throw new IllegalArgumentException("The tag must not be null.");
    }
    return NOERROR;
}

ECode CPowerManager::UserActivity(
    /* [in] */ Int64 when,
    /* [in] */ Boolean noChangeLights)
{
    // try {
    return mService->UserActivity(when, USER_ACTIVITY_EVENT_OTHER,
            noChangeLights ? USER_ACTIVITY_FLAG_NO_CHANGE_LIGHTS : 0);
    // } catch (RemoteException e) {
    // }
}

ECode CPowerManager::GoToSleep(
    /* [in] */ Int64 eventTime)
{
    // try {
    return mService->GoToSleep(eventTime, GO_TO_SLEEP_REASON_USER);
    // } catch (RemoteException e) {
    // }
}

ECode CPowerManager::WakeUp(
    /* [in] */ Int64 eventTime)
{
    // try {
    return mService->WakeUp(eventTime);
    // } catch (RemoteException e) {
    // }
}

ECode CPowerManager::Nap(
    /* [in] */ Int64 time)
{
    // try {
    return mService->Nap(time);
    // } catch (RemoteException e) {
    // }
}

ECode CPowerManager::GoToBootFastSleep(
    /* [in] */ Int64 time)
{
    return mService->GoToBootFastSleep(time);
}

ECode CPowerManager::BootFastWake(
    /* [in] */ Int64 time)
{
    return mService->BootFastWake(time);
}

ECode CPowerManager::IsBootFastStatus(
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
    return mService->IsBootFastStatus(result);
}

ECode CPowerManager::IsBootFastWakeFromStandby(
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
    return mService->IsBootFastWakeFromStandby(result);
}

ECode CPowerManager::SetBacklightBrightness(
    /* [in] */ Int32 brightness)
{
    // try {
    return mService->SetTemporaryScreenBrightnessSettingOverride(brightness);
    // } catch (RemoteException e) {
    // }
}

ECode CPowerManager::IsWakeLockLevelSupported(
    /* [in] */ Int32 level,
    /* [out] */ Boolean* isSupported)
{
    VALIDATE_NOT_NULL(isSupported);
    // try {
    return mService->IsWakeLockLevelSupported(level, isSupported);
    // } catch (RemoteException e) {
    //     return false;
    // }
}

ECode CPowerManager::IsScreenOn(
    /* [out] */ Boolean* isScreenOn)
{
    // try {
    return mService->IsScreenOn(isScreenOn);
    // } catch (RemoteException e) {
    // }
}

ECode CPowerManager::Reboot(
    /* [in] */ Boolean confirm,
    /* [in] */ const String& reason,
    /* [in] */ Boolean wait)
{
    // try {
    return mService->Reboot(FALSE, reason, TRUE);
    // } catch (RemoteException e) {
    // }
}

ECode CPowerManager::constructor(
    /* [in] */ IContext* context,
    /* [in] */ IIPowerManager* service,
    /* [in] */ IHandler* handler)
{
    mContext = context;
    mService = service;
    mHandler = handler;
    return NOERROR;
}

} // namespace Os
} // namespace Droid
} // namespace Elastos
