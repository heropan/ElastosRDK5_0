#include "ext/frameworkext.h"
#include "widget/internal/CLockPatternUtils.h"

namespace Elastos {
namespace Droid {
namespace Widget {
namespace Internal {

PInterface CLockPatternUtils::Probe(
    /* [in] */ REIID riid)
{
    return _CLockPatternUtils::Probe(riid);
}

ECode CLockPatternUtils::GetDevicePolicyManager(
    /* [out] */ IDevicePolicyManager** manager)
{
    VALIDATE_NOT_NULL(manager);
    AutoPtr<IDevicePolicyManager> dm = LockPatternUtils::GetDevicePolicyManager();
    *manager = dm;
    INTERFACE_ADDREF(*manager);
    return NOERROR;
}

ECode CLockPatternUtils::constructor(
    /* [in] */ IContext* context)
{
    return LockPatternUtils::Init(context);
}

ECode CLockPatternUtils::GetRequestedMinimumPasswordLength(
    /* [out] */ Int32* length)
{
    VALIDATE_NOT_NULL(length);
    *length = LockPatternUtils::GetRequestedMinimumPasswordLength();
    return NOERROR;
}

ECode CLockPatternUtils::GetRequestedPasswordQuality(
    /* [out] */ Int32* quality)
{
    VALIDATE_NOT_NULL(quality);
    *quality = LockPatternUtils::GetRequestedPasswordQuality();
    return NOERROR;
}

ECode CLockPatternUtils::GetRequestedPasswordHistoryLength(
    /* [out] */ Int32* length)
{
    VALIDATE_NOT_NULL(length);
    *length = LockPatternUtils::GetRequestedPasswordHistoryLength();
    return NOERROR;
}

ECode CLockPatternUtils::GetRequestedPasswordMinimumLetters(
    /* [out] */ Int32* letters)
{
    VALIDATE_NOT_NULL(letters);
    *letters = LockPatternUtils::GetRequestedPasswordMinimumLetters();
    return NOERROR;
}

ECode CLockPatternUtils::GetRequestedPasswordMinimumUpperCase(
    /* [out] */ Int32* upperCase)
{
    VALIDATE_NOT_NULL(upperCase);
    *upperCase = LockPatternUtils::GetRequestedPasswordMinimumUpperCase();
    return NOERROR;
}

ECode CLockPatternUtils::GetRequestedPasswordMinimumLowerCase(
    /* [out] */ Int32* lowerCase)
{
    VALIDATE_NOT_NULL(lowerCase);
    *lowerCase = LockPatternUtils::GetRequestedPasswordMinimumLowerCase();
    return NOERROR;
}

ECode CLockPatternUtils::GetRequestedPasswordMinimumNumeric(
    /* [out] */ Int32* numeric)
{
    VALIDATE_NOT_NULL(numeric);
    *numeric = LockPatternUtils::GetRequestedPasswordMinimumNumeric();
    return NOERROR;
}

ECode CLockPatternUtils::GetRequestedPasswordMinimumSymbols(
    /* [out] */ Int32* symbols)
{
    VALIDATE_NOT_NULL(symbols);
    *symbols = LockPatternUtils::GetRequestedPasswordMinimumSymbols();
    return NOERROR;
}

ECode CLockPatternUtils::GetRequestedPasswordMinimumNonLetter(
    /* [out] */ Int32* letter)
{
    VALIDATE_NOT_NULL(letter);
    *letter = LockPatternUtils::GetRequestedPasswordMinimumNonLetter();
    return NOERROR;
}

ECode CLockPatternUtils::ReportFailedPasswordAttempt()
{
    return LockPatternUtils::ReportFailedPasswordAttempt();
}

ECode CLockPatternUtils::ReportSuccessfulPasswordAttempt()
{
    return LockPatternUtils::ReportSuccessfulPasswordAttempt();
}

ECode CLockPatternUtils::SetCurrentUser(
    /* [in] */ Int32 userId)
{
    return LockPatternUtils::SetCurrentUser(userId);
}

ECode CLockPatternUtils::GetCurrentUser(
    /* [out] */ Int32* user)
{
    VALIDATE_NOT_NULL(user);
    *user = LockPatternUtils::GetCurrentUser();
    return NOERROR;
}

ECode CLockPatternUtils::RemoveUser(
    /* [in] */ Int32 userId)
{
    return LockPatternUtils::RemoveUser(userId);
}

ECode CLockPatternUtils::CheckPattern(
    /* [in] */ IObjectContainer* pattern,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
    *result = LockPatternUtils::CheckPattern(pattern);
    return NOERROR;
}

ECode CLockPatternUtils::CheckPassword(
    /* [in] */ const String& password,
    /* [out] */ Boolean* res)
{
    VALIDATE_NOT_NULL(res);
    *res = LockPatternUtils::CheckPassword(password);
    return NOERROR;
}

ECode CLockPatternUtils::CheckPasswordHistory(
    /* [in] */ const String& password,
    /* [out] */ Boolean* history)
{
    VALIDATE_NOT_NULL(history);
    *history = LockPatternUtils::CheckPasswordHistory(password);
    return NOERROR;
}

ECode CLockPatternUtils::SavedPatternExists(
    /* [out] */ Boolean* exists)
{
    VALIDATE_NOT_NULL(exists);
    *exists = LockPatternUtils::SavedPatternExists();
    return NOERROR;
}

ECode CLockPatternUtils::SavedPasswordExists(
    /* [out] */ Boolean* exists)
{
    VALIDATE_NOT_NULL(exists);
    *exists = LockPatternUtils::SavedPasswordExists();
    return NOERROR;
}

ECode CLockPatternUtils::IsPatternEverChosen(
    /* [out] */ Boolean* chosen)
{
    VALIDATE_NOT_NULL(chosen);
    *chosen = LockPatternUtils::IsPatternEverChosen();
    return NOERROR;
}

ECode CLockPatternUtils::IsBiometricWeakEverChosen(
    /* [out] */ Boolean* chosen)
{
    VALIDATE_NOT_NULL(chosen);
    *chosen = LockPatternUtils::IsBiometricWeakEverChosen();
    return NOERROR;
}

ECode CLockPatternUtils::GetActivePasswordQuality(
    /* [out] */ Int32* quality)
{
    VALIDATE_NOT_NULL(quality);
    *quality = LockPatternUtils::GetActivePasswordQuality();
    return NOERROR;
}

ECode CLockPatternUtils::ClearLock(
    /* [in] */ Boolean isFallback)
{
    return LockPatternUtils::ClearLock(isFallback);
}

ECode CLockPatternUtils::SetLockScreenDisabled(
    /* [in] */ Boolean isDisable)
{
    return LockPatternUtils::SetLockScreenDisabled(isDisable);
}

ECode CLockPatternUtils::IsLockScreenDisabled(
    /* [out] */ Boolean* disabled)
{
    VALIDATE_NOT_NULL(disabled);
    *disabled = LockPatternUtils::IsLockScreenDisabled();
    return NOERROR;
}

ECode CLockPatternUtils::DeleteTempGallery()
{
    return LockPatternUtils::DeleteTempGallery();
}

ECode CLockPatternUtils::SaveLockPattern(
    /* [in] */ IObjectContainer* pattern)
{
    return LockPatternUtils::SaveLockPattern(pattern);
}

ECode CLockPatternUtils::SaveLockPattern(
    /* [in] */ IObjectContainer* pattern,
    /* [in] */ Boolean isFallback)
{
    return LockPatternUtils::SaveLockPattern(pattern, isFallback);
}

ECode CLockPatternUtils::SaveLockPassword(
    /* [in] */ const String& password,
    /* [in] */ Int32 quality)
{
    return LockPatternUtils::SaveLockPassword(password, quality);
}

ECode CLockPatternUtils::SaveLockPassword(
    /* [in] */ const String& password,
    /* [in] */ Int32 quality,
    /* [in] */ Boolean isFallback)
{
    return LockPatternUtils::SaveLockPassword(password, quality, isFallback);
}

ECode CLockPatternUtils::SaveLockPassword(
    /* [in] */ const String& password,
    /* [in] */ Int32 quality,
    /* [in] */ Boolean isFallback,
    /* [in] */ Int32 userHandle)
{
    return LockPatternUtils::SaveLockPassword(password, quality, isFallback, userHandle);
}

ECode CLockPatternUtils::GetKeyguardStoredPasswordQuality(
    /* [out] */ Int32* quality)
{
    VALIDATE_NOT_NULL(quality);
    *quality = LockPatternUtils::GetKeyguardStoredPasswordQuality();
    return NOERROR;
}

ECode CLockPatternUtils::UsingBiometricWeak(
    /* [out] */ Boolean* weak)
{
    VALIDATE_NOT_NULL(weak);
    *weak = LockPatternUtils::UsingBiometricWeak();
    return NOERROR;
}

ECode CLockPatternUtils::PasswordToHash(
    /* [in] */ const String& password,
    /* [out, callee] */ ArrayOf<Byte>** hash)
{
    VALIDATE_NOT_NULL(hash);
    AutoPtr< ArrayOf<Byte> > temp = LockPatternUtils::PasswordToHash(password);
    *hash = temp->Clone();
    ARRAYOF_ADDREF(*hash);
    return NOERROR;
}

ECode CLockPatternUtils::IsLockPasswordEnabled(
    /* [out] */ Boolean* enabled)
{
    VALIDATE_NOT_NULL(enabled);
    *enabled = LockPatternUtils::IsLockPasswordEnabled();
    return NOERROR;
}

ECode CLockPatternUtils::IsLockPatternEnabled(
    /* [out] */ Boolean* enabled)
{
    VALIDATE_NOT_NULL(enabled);
    *enabled = LockPatternUtils::IsLockPatternEnabled();
    return NOERROR;
}

ECode CLockPatternUtils::IsBiometricWeakInstalled(
    /* [out] */ Boolean* installed)
{
    VALIDATE_NOT_NULL(installed);
    *installed = LockPatternUtils::IsBiometricWeakInstalled();
    return NOERROR;
}

ECode CLockPatternUtils::SetBiometricWeakLivelinessEnabled(
    /* [in] */ Boolean enabled)
{
    return LockPatternUtils::SetBiometricWeakLivelinessEnabled(enabled);
}

ECode CLockPatternUtils::IsBiometricWeakLivelinessEnabled(
    /* [out] */ Boolean* enabled)
{
    VALIDATE_NOT_NULL(enabled);
    *enabled = LockPatternUtils::IsBiometricWeakLivelinessEnabled();
    return NOERROR;
}

ECode CLockPatternUtils::SetLockPatternEnabled(
    /* [in] */ Boolean enabled)
{
    return LockPatternUtils::SetLockPatternEnabled(enabled);
}

ECode CLockPatternUtils::IsVisiblePatternEnabled(
    /* [out] */ Boolean* enabled)
{
    VALIDATE_NOT_NULL(enabled);
    *enabled = LockPatternUtils::IsVisiblePatternEnabled();
    return NOERROR;
}

ECode CLockPatternUtils::SetVisiblePatternEnabled(
    /* [in] */ Boolean enabled)
{
    return LockPatternUtils::SetVisiblePatternEnabled(enabled);
}

ECode CLockPatternUtils::IsTactileFeedbackEnabled(
    /* [out] */ Boolean* enabled)
{
    VALIDATE_NOT_NULL(enabled);
    *enabled = LockPatternUtils::IsTactileFeedbackEnabled();
    return NOERROR;
}

ECode CLockPatternUtils::SetLockoutAttemptDeadline(
    /* [out] */ Int64* deadline)
{
    VALIDATE_NOT_NULL(deadline);
    *deadline = LockPatternUtils::SetLockoutAttemptDeadline();
    return NOERROR;
}

ECode CLockPatternUtils::GetLockoutAttemptDeadline(
    /* [out] */ Int64* deadline)
{
    VALIDATE_NOT_NULL(deadline);
    *deadline = LockPatternUtils::GetLockoutAttemptDeadline();
    return NOERROR;
}

ECode CLockPatternUtils::IsPermanentlyLocked(
    /* [out] */ Boolean* locked)
{
    VALIDATE_NOT_NULL(locked);
    *locked = LockPatternUtils::IsPermanentlyLocked();
    return NOERROR;
}

ECode CLockPatternUtils::SetPermanentlyLocked(
    /* [in] */ Boolean locked)
{
    return LockPatternUtils::SetPermanentlyLocked(locked);
}

ECode CLockPatternUtils::IsEmergencyCallCapable(
    /* [out] */ Boolean* capable)
{
    VALIDATE_NOT_NULL(capable);
    *capable = LockPatternUtils::IsEmergencyCallCapable();
    return NOERROR;
}

ECode CLockPatternUtils::IsPukUnlockScreenEnable(
    /* [out] */ Boolean* isEnable)
{
    VALIDATE_NOT_NULL(isEnable);
    *isEnable = LockPatternUtils::IsPukUnlockScreenEnable();
    return NOERROR;
}

ECode CLockPatternUtils::IsEmergencyCallEnabledWhileSimLocked(
    /* [out] */ Boolean* locked)
{
    VALIDATE_NOT_NULL(locked);
    *locked = LockPatternUtils::IsEmergencyCallEnabledWhileSimLocked();
    return NOERROR;
}

ECode CLockPatternUtils::GetNextAlarm(
    /* [out] */ String* alarm)
{
    VALIDATE_NOT_NULL(alarm);
    *alarm = LockPatternUtils::GetNextAlarm();
    return NOERROR;
}

ECode CLockPatternUtils::GetAppWidgets(
    /* [out, callee] */ ArrayOf<Int32>** widgets)
{
    VALIDATE_NOT_NULL(widgets);
    AutoPtr< ArrayOf<Int32> > temp = LockPatternUtils::GetAppWidgets();
    *widgets = temp->Clone();
    ARRAYOF_ADDREF(*widgets);
    return NOERROR;
}

ECode CLockPatternUtils::WriteFallbackAppWidgetId(
    /* [in] */ Int32 appWidgetId)
{
    return LockPatternUtils::WriteFallbackAppWidgetId(appWidgetId);
}

ECode CLockPatternUtils::GetFallbackAppWidgetId(
    /* [out] */ Int32* id)
{
    VALIDATE_NOT_NULL(id);
    *id = LockPatternUtils::GetFallbackAppWidgetId();
    return NOERROR;
}

ECode CLockPatternUtils::AddAppWidget(
    /* [in] */ Int32 widgetId,
    /* [in] */ Int32 index,
    /* [out] */ Boolean* res)
{
    VALIDATE_NOT_NULL(res);
    *res = LockPatternUtils::AddAppWidget(widgetId, index);
    return NOERROR;
}

ECode CLockPatternUtils::RemoveAppWidget(
    /* [in] */ Int32 widgetId,
    /* [out] */ Boolean* res)
{
    VALIDATE_NOT_NULL(res);
    *res = LockPatternUtils::RemoveAppWidget(widgetId);
    return NOERROR;
}

ECode CLockPatternUtils::IsSecure(
    /* [out] */ Boolean* secure)
{
    VALIDATE_NOT_NULL(secure);
    *secure = LockPatternUtils::IsSecure();
    return NOERROR;
}

ECode CLockPatternUtils::UpdateEmergencyCallButtonState(
    /* [in] */ IButton* button,
    /* [in] */ Int32  phoneState,
    /* [in] */ Boolean shown,
    /* [in] */ Boolean upperCase,
    /* [in] */ Boolean showIcon)
{
    return LockPatternUtils::UpdateEmergencyCallButtonState(button, phoneState, shown, upperCase, showIcon);
}

ECode CLockPatternUtils::UpdateEmergencyCallButtonState(
    /* [in] */ IButton* button,
    /* [in] */ Int32  phoneState,
    /* [in] */ Boolean shown)
{
    return LockPatternUtils::UpdateEmergencyCallButtonState(button, phoneState, shown);
}

ECode CLockPatternUtils::ResumeCall(
    /* [out] */ Boolean* res)
{
    VALIDATE_NOT_NULL(res);
    *res = LockPatternUtils::ResumeCall();
    return NOERROR;
}

ECode CLockPatternUtils::SetPowerButtonInstantlyLocks(
    /* [in] */ Boolean enabled)
{
    return LockPatternUtils::SetPowerButtonInstantlyLocks(enabled);
}

ECode CLockPatternUtils::GetPowerButtonInstantlyLocks(
    /* [out] */ Boolean* locks)
{
    VALIDATE_NOT_NULL(locks);
    *locks = LockPatternUtils::GetPowerButtonInstantlyLocks();
    return NOERROR;
}

}// namespace Internal
}// namespace Widget
}// namespace Droid
}// namespace Elastos