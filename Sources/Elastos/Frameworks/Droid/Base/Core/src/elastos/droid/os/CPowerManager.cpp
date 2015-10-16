
#include "elastos/droid/ext/frameworkext.h"
#include "elastos/droid/os/CPowerManager.h"
#include "elastos/droid/os/SystemProperties.h"
#include "elastos/droid/R.h"
#include <elastos/utility/logging/Slogger.h>

using Elastos::Utility::Logging::Slogger;
using Elastos::Droid::Content::Res::IResources;

namespace Elastos {
namespace Droid {
namespace Os {

const String CPowerManager::TAG("PowerManager");

CAR_INTERFACE_IMPL(CPowerManager, Object, IPowerManager)

CAR_OBJECT_IMPL(CPowerManager)

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

Boolean CPowerManager::UseTwilightAdjustmentFeature()
{
    Boolean bval;
    SystemProperties::GetBoolean(String("persist.power.usetwilightadj"), FALSE, &bval);
    return bval;
}

ECode CPowerManager::NewWakeLock(
    /* [in] */ Int32 levelAndFlags,
    /* [in] */ const String& tag,
    /* [out] */ IPowerManagerWakeLock** wakeLock)
{
    VALIDATE_NOT_NULL(wakeLock);
    FAIL_RETURN(ValidateWakeLockParameters(levelAndFlags, tag));
    String name;
    mContext->GetOpPackageName(&name);
    return CPowerManagerWakeLock::New(levelAndFlags, tag, name, THIS_PROBE(IPowerManager), wakeLock);
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
        case DOZE_WAKE_LOCK:
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
    return UserActivity(when, USER_ACTIVITY_EVENT_OTHER,
            noChangeLights ? USER_ACTIVITY_FLAG_NO_CHANGE_LIGHTS : 0);
}

ECode CPowerManager::UserActivity(
    /* [in] */ Int64 when,
    /* [in] */ Int32 event,
    /* [in] */ Int32 flags)
{
    // try {
    return mService->UserActivity(when, event, flags);
    // } catch (RemoteException e) {
    // }
}

ECode CPowerManager::GoToSleep(
    /* [in] */ Int64 eventTime)
{
    return GoToSleep(eventTime, GO_TO_SLEEP_REASON_APPLICATION, 0);
}

ECode CPowerManager::GoToSleep(
    /* [in] */ Int64 eventTime,
    /* [in] */ Int32 reason,
    /* [in] */ Int32 flags)
{
    // try {
    return mService->GoToSleep(eventTime, reason, flags);
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
    return IsInteractive(isScreenOn);
}

ECode CPowerManager::IsInteractive(
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
    // try {
    return mService->IsInteractive(result);
    // } catch (RemoteException e) {
    //     return false;
    // }
}

ECode CPowerManager::Reboot(
    /* [in] */ const String& reason)
{
    // try {
    return mService->Reboot(FALSE, reason, TRUE);
    // } catch (RemoteException e) {
    // }
}

ECode CPowerManager::IsPowerSaveMode(
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
    // try {
    return mService->IsPowerSaveMode(result);
    // } catch (RemoteException e) {
    // }
}

ECode CPowerManager::SetPowerSaveMode(
    /* [in] */ Boolean mode,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
    // try {
    return mService->SetPowerSaveMode(mode, result);
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
