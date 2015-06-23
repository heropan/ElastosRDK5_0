#ifndef __CLOCKPATTERNUTILS__H_
#define __CLOCKPATTERNUTILS__H_

#include "_CLockPatternUtils.h"
#include "widget/internal/LockPatternUtils.h"

namespace Elastos {
namespace Droid {
namespace Widget {
namespace Internal {

CarClass(CLockPatternUtils), public LockPatternUtils
{
public:
    CARAPI_(PInterface) Probe(
        /* [in] */ REIID riid);

    CARAPI GetDevicePolicyManager(
        /* [out] */ IDevicePolicyManager** manager);

    CARAPI constructor(
        /* [in] */ IContext* context);

    CARAPI GetRequestedMinimumPasswordLength(
        /* [out] */ Int32* length);

    CARAPI GetRequestedPasswordQuality(
        /* [out] */ Int32* quality);

    CARAPI GetRequestedPasswordHistoryLength(
        /* [out] */ Int32* length);

    CARAPI GetRequestedPasswordMinimumLetters(
        /* [out] */ Int32* letters);

    CARAPI GetRequestedPasswordMinimumUpperCase(
        /* [out] */ Int32* upperCase);

    CARAPI GetRequestedPasswordMinimumLowerCase(
        /* [out] */ Int32* lowerCase);

    CARAPI GetRequestedPasswordMinimumNumeric(
        /* [out] */ Int32* numeric);

    CARAPI GetRequestedPasswordMinimumSymbols(
        /* [out] */ Int32* symbols);

    CARAPI GetRequestedPasswordMinimumNonLetter(
        /* [out] */ Int32* letter);

    CARAPI ReportFailedPasswordAttempt();

    CARAPI ReportSuccessfulPasswordAttempt();

    CARAPI SetCurrentUser(
        /* [in] */ Int32 userId);

    CARAPI GetCurrentUser(
        /* [out] */ Int32* user);

    CARAPI RemoveUser(
        /* [in] */ Int32 userId);

    CARAPI CheckPattern(
        /* [in] */ IObjectContainer* pattern,
        /* [out] */ Boolean* result);

    CARAPI CheckPassword(
        /* [in] */ const String& password,
        /* [out] */ Boolean* res);

    CARAPI CheckPasswordHistory(
        /* [in] */ const String& password,
        /* [out] */ Boolean* history);

    CARAPI SavedPatternExists(
        /* [out] */ Boolean* exists);

    CARAPI SavedPasswordExists(
        /* [out] */ Boolean* exists);

    CARAPI IsPatternEverChosen(
        /* [out] */ Boolean* chosen);

    CARAPI IsBiometricWeakEverChosen(
        /* [out] */ Boolean* chosen);

    CARAPI GetActivePasswordQuality(
        /* [out] */ Int32* quality);

    CARAPI ClearLock(
        /* [in] */ Boolean isFallback);

    CARAPI SetLockScreenDisabled(
        /* [in] */ Boolean isDisable);

    CARAPI IsLockScreenDisabled(
        /* [out] */ Boolean* disabled);

    CARAPI DeleteTempGallery();

    CARAPI SaveLockPattern(
        /* [in] */ IObjectContainer* pattern);

    CARAPI SaveLockPattern(
        /* [in] */ IObjectContainer* pattern,
        /* [in] */ Boolean isFallback);

    CARAPI SaveLockPassword(
        /* [in] */ const String& password,
        /* [in] */ Int32 quality);

    CARAPI SaveLockPassword(
        /* [in] */ const String& password,
        /* [in] */ Int32 quality,
        /* [in] */ Boolean isFallback);

    CARAPI SaveLockPassword(
        /* [in] */ const String& password,
        /* [in] */ Int32 quality,
        /* [in] */ Boolean isFallback,
        /* [in] */ Int32 userHandle);

    CARAPI GetKeyguardStoredPasswordQuality(
        /* [out] */ Int32* quality);

    CARAPI UsingBiometricWeak(
        /* [out] */ Boolean* weak);

    CARAPI PasswordToHash(
        /* [in] */ const String& password,
        /* [out, callee] */ ArrayOf<Byte>** hash);

    CARAPI IsLockPasswordEnabled(
        /* [out] */ Boolean* enabled);

    CARAPI IsLockPatternEnabled(
        /* [out] */ Boolean* enabled);

    CARAPI IsBiometricWeakInstalled(
        /* [out] */ Boolean* installed);

    CARAPI SetBiometricWeakLivelinessEnabled(
        /* [in] */ Boolean enabled);

    CARAPI IsBiometricWeakLivelinessEnabled(
        /* [out] */ Boolean* enabled);

    CARAPI SetLockPatternEnabled(
        /* [in] */ Boolean enabled);

    CARAPI IsVisiblePatternEnabled(
        /* [out] */ Boolean* enabled);

    CARAPI SetVisiblePatternEnabled(
        /* [in] */ Boolean enabled);

    CARAPI IsTactileFeedbackEnabled(
        /* [out] */ Boolean* enabled);

    CARAPI SetLockoutAttemptDeadline(
        /* [out] */ Int64* deadline);

    CARAPI GetLockoutAttemptDeadline(
        /* [out] */ Int64* deadline);

    CARAPI IsPermanentlyLocked(
        /* [out] */ Boolean* locked);

    CARAPI SetPermanentlyLocked(
        /* [in] */ Boolean locked);

    CARAPI IsEmergencyCallCapable(
        /* [out] */ Boolean* capable);

    CARAPI IsPukUnlockScreenEnable(
        /* [out] */ Boolean* isEnable);

    CARAPI IsEmergencyCallEnabledWhileSimLocked(
        /* [out] */ Boolean* locked);

    CARAPI GetNextAlarm(
        /* [out] */ String* alarm);

    CARAPI GetAppWidgets(
        /* [out, callee] */ ArrayOf<Int32>** widgets);

    CARAPI WriteFallbackAppWidgetId(
        /* [in] */ Int32 appWidgetId);

    CARAPI GetFallbackAppWidgetId(
        /* [out] */ Int32* id);

    CARAPI AddAppWidget(
        /* [in] */ Int32 widgetId,
        /* [in] */ Int32 index,
        /* [out] */ Boolean* res);

    CARAPI RemoveAppWidget(
        /* [in] */ Int32 widgetId,
        /* [out] */ Boolean* res);

    CARAPI IsSecure(
        /* [out] */ Boolean* secure);

    CARAPI UpdateEmergencyCallButtonState(
        /* [in] */ IButton* button,
        /* [in] */ Int32  phoneState,
        /* [in] */ Boolean shown,
        /* [in] */ Boolean upperCase,
        /* [in] */ Boolean showIcon);

    CARAPI UpdateEmergencyCallButtonState(
        /* [in] */ IButton* button,
        /* [in] */ Int32  phoneState,
        /* [in] */ Boolean shown);

    CARAPI ResumeCall(
        /* [out] */ Boolean* res);

    CARAPI SetPowerButtonInstantlyLocks(
        /* [in] */ Boolean enabled);

    CARAPI GetPowerButtonInstantlyLocks(
        /* [out] */ Boolean* locks);
};

}// namespace Internal
}// namespace Widget
}// namespace Droid
}// namespace Elastos

#endif //__CLOCKPATTERNUTILS__H_