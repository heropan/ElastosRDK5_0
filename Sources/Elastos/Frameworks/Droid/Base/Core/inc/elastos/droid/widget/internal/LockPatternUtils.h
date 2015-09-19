
#ifndef __ELASTOS_DROID_WIDGET_INTERNAL_LOCKPATTERNUTILS_H__
#define __ELASTOS_DROID_WIDGET_INTERNAL_LOCKPATTERNUTILS_H__

#ifdef DROID_CORE
#include "Elastos.Droid.Core_server.h"
#else
#include "Elastos.Droid.Core.h"
#endif
#include "os/CBundle.h"
#include "R.h"

using Elastos::Droid::R;
using Elastos::Droid::Os::IBundle;
using Elastos::Droid::Os::CBundle;
using Elastos::Droid::Content::IContext;
using Elastos::Droid::Content::IContentResolver;
using Elastos::Droid::App::Admin::IDevicePolicyManager;

namespace Elastos {
namespace Droid {
namespace Widget {
namespace Internal {

class LockPatternUtils
{
public:
    AutoPtr<IDevicePolicyManager> GetDevicePolicyManager();

    LockPatternUtils(
        /* [in] */ IContext* context);

    virtual CARAPI_(Int32) GetRequestedMinimumPasswordLength();

    virtual CARAPI_(Int32) GetRequestedPasswordQuality();

    virtual CARAPI_(Int32) GetRequestedPasswordHistoryLength();

    virtual CARAPI_(Int32) GetRequestedPasswordMinimumLetters();

    virtual CARAPI_(Int32) GetRequestedPasswordMinimumUpperCase();

    virtual CARAPI_(Int32) GetRequestedPasswordMinimumLowerCase();

    virtual CARAPI_(Int32) GetRequestedPasswordMinimumNumeric();

    virtual CARAPI_(Int32) GetRequestedPasswordMinimumSymbols();

    virtual CARAPI_(Int32) GetRequestedPasswordMinimumNonLetter();

    virtual CARAPI ReportFailedPasswordAttempt();

    virtual CARAPI ReportSuccessfulPasswordAttempt();

    virtual CARAPI SetCurrentUser(
        /* [in] */ Int32 userId);

    virtual CARAPI_(Int32) GetCurrentUser();

    virtual CARAPI RemoveUser(
        /* [in] */ Int32 userId);


    virtual CARAPI_(Boolean) CheckPattern(
        /* [in] */ IObjectContainer* pattern);

    virtual CARAPI_(Boolean) CheckPassword(
        /* [in] */ const String& password);

    virtual CARAPI_(Boolean) CheckPasswordHistory(
        /* [in] */ const String& password);

    virtual CARAPI_(Boolean) SavedPatternExists();

    virtual CARAPI_(Boolean) SavedPasswordExists();

    virtual CARAPI_(Boolean) IsPatternEverChosen();

    virtual CARAPI_(Boolean) IsBiometricWeakEverChosen();

    virtual CARAPI_(Int32) GetActivePasswordQuality();

    virtual CARAPI ClearLock(
        /* [in] */ Boolean isFallback);

    virtual CARAPI SetLockScreenDisabled(
        /* [in] */ Boolean disable);

    virtual CARAPI_(Boolean) IsLockScreenDisabled();

    virtual CARAPI DeleteTempGallery();

    virtual CARAPI DeleteGallery();

    virtual CARAPI SaveLockPattern(
        /* [in] */ IObjectContainer* pattern);

    virtual CARAPI SaveLockPattern(
        /* [in] */ IObjectContainer* pattern,
        /* [in] */ Boolean isFallback);

    static CARAPI_(Int32) ComputePasswordQuality(
        /* [in] */ const String& password);

    virtual CARAPI SaveLockPassword(
        /* [in] */ const String& password,
        /* [in] */ Int32 quality);

    virtual CARAPI SaveLockPassword(
        /* [in] */ const String& password,
        /* [in] */ Int32 quality,
        /* [in] */ Boolean isFallback);

    virtual CARAPI SaveLockPassword(
        /* [in] */ const String& password,
        /* [in] */ Int32 quality,
        /* [in] */ Boolean isFallback,
        /* [in] */ Int32 userHandle);

    virtual CARAPI_(Int32) GetKeyguardStoredPasswordQuality();

    virtual CARAPI_(Boolean) UsingBiometricWeak();

    static CARAPI_(AutoPtr<IObjectContainer>) StringToPattern(
        /* [in] */ const String& string);

    static CARAPI_(String) PatternToString(
        /* [in] */ IObjectContainer* pattern);

    CARAPI_(AutoPtr<ArrayOf<Byte> >) PasswordToHash(
        /* [in] */ const String& password);

    virtual CARAPI_(Boolean) IsLockPasswordEnabled();

    virtual CARAPI_(Boolean) IsLockPatternEnabled();

    virtual CARAPI_(Boolean) IsBiometricWeakInstalled();

    virtual CARAPI SetBiometricWeakLivelinessEnabled(
        /* [in] */ Boolean enabled);

    virtual CARAPI_(Boolean) IsBiometricWeakLivelinessEnabled();

    virtual CARAPI SetLockPatternEnabled(
        /* [in] */ Boolean enabled);

    virtual CARAPI_(Boolean) IsVisiblePatternEnabled();

    virtual CARAPI SetVisiblePatternEnabled(
        /* [in] */ Boolean enabled);

    virtual CARAPI_(Boolean) IsTactileFeedbackEnabled();

    virtual CARAPI_(Int64) SetLockoutAttemptDeadline();

    virtual CARAPI_(Int64) GetLockoutAttemptDeadline();

    virtual CARAPI_(Boolean) IsPermanentlyLocked();

    virtual CARAPI SetPermanentlyLocked(
        /* [in] */ Boolean locked);

    virtual CARAPI_(Boolean) IsEmergencyCallCapable();

    virtual CARAPI_(Boolean) IsPukUnlockScreenEnable();

    virtual CARAPI_(Boolean) IsEmergencyCallEnabledWhileSimLocked();

    virtual CARAPI_(String) GetNextAlarm();

    virtual CARAPI_(AutoPtr< ArrayOf<Int32> >) GetAppWidgets();

    virtual CARAPI WriteFallbackAppWidgetId(
        /* [in] */ Int32 appWidgetId);

    virtual CARAPI_(Int32) GetFallbackAppWidgetId();

    virtual CARAPI_(Boolean) AddAppWidget(
        /* [in] */ Int32 widgetId,
        /* [in] */ Int32 index);

    virtual CARAPI_(Boolean) RemoveAppWidget(
        /* [in] */ Int32 widgetId);

    virtual CARAPI_(Boolean) IsSecure();

    virtual CARAPI UpdateEmergencyCallButtonState(
        /* [in] */ IButton* button,
        /* [in] */ Int32  phoneState,
        /* [in] */ Boolean shown,
        /* [in] */ Boolean upperCase,
        /* [in] */ Boolean showIcon);

    virtual CARAPI UpdateEmergencyCallButtonState(
        /* [in] */ IButton* button,
        /* [in] */ Int32  phoneState,
        /* [in] */ Boolean shown);

    virtual CARAPI_(Boolean) ResumeCall();

    virtual CARAPI SetPowerButtonInstantlyLocks(
        /* [in] */ Boolean enabled);

    virtual CARAPI_(Boolean) GetPowerButtonInstantlyLocks();

    static CARAPI_(Boolean) IsSafeModeEnabled();

protected:
    LockPatternUtils();

    CARAPI Init(
        /* [in] */ IContext* context);

private:
    CARAPI_(AutoPtr<ILockSettings>) GetLockSettings();

    CARAPI_(Int32) GetCurrentOrCallingUserId();

    CARAPI_(void) UpdateEncryptionPassword(
        /* [in] */ const String& password);

    static CARAPI_(AutoPtr < ArrayOf<Byte> >) PatternToHash(
        /* [in] */ IObjectContainer* pattern);

    CARAPI_(String) GetSalt();

    static CARAPI_(String) ToHex(
        /* [in] */ ArrayOf<Byte>* ary);


    CARAPI_(Boolean) GetBoolean(
        /* [in] */ const String& secureSettingKey,
        /* [in] */ Boolean defaultValue);

    CARAPI_(void) SetBoolean(
        /* [in] */ const String& secureSettingKey,
        /* [in] */ Boolean enabled);

    static CARAPI_(String) CombineStrings(
        /* [in] */ ArrayOf<Int32>* list,
        /* [in] */ const String& separator);

    CARAPI_(void) WriteAppWidgets(
        /* [in] */ ArrayOf<Int32>* AppWidgetIds);

    CARAPI_(Int64) GetInt64(
        /* [in] */ const String& secureSettingKey,
        /* [in] */ Int64 defaultValue);

    CARAPI_(void) SetInt64(
        /* [in] */ const String& secureSettingKey,
        /* [in] */ Int64 value);

    CARAPI_(void) SetInt64(
        /* [in] */ const String& secureSettingKey,
        /* [in] */ Int64 value,
        /* [in] */ Int32 userHandle);

    CARAPI_(String) GetString(
        /* [in] */ const String& secureSettingKey);

    CARAPI_(String) GetString(
        /* [in] */ const String& secureSettingKey,
        /* [in] */ Int32 userHandle);

    CARAPI_(void) SetString(
        /* [in] */ const String& secureSettingKey,
        /* [in] */ const String& value,
        /* [in] */ Int32 userHandle);

    CARAPI_(void) FinishBiometricWeak();

public:
    static const Int32 FAILED_ATTEMPTS_BEFORE_TIMEOUT = 5;
    static const Int32 FAILED_ATTEMPTS_BEFORE_RESET = 20;
    static const Int64 FAILED_ATTEMPT_TIMEOUT_MS = 30000L;
    static const Int64 FAILED_ATTEMPT_COUNTDOWN_INTERVAL_MS = 1000L;
    static const Int32 FAILED_ATTEMPTS_BEFORE_WIPE_GRACE = 5;
    static const Int32 MIN_LOCK_PATTERN_SIZE = 4;
    static const Int32 MIN_PATTERN_REGISTER_FAIL = MIN_LOCK_PATTERN_SIZE;
    static const String KEYGUARD_SHOW_USER_SWITCHER;
    static const String KEYGUARD_SHOW_SECURITY_CHALLENGE;
    static const String KEYGUARD_SHOW_APPWIDGET;
    static const Int32 FLAG_BIOMETRIC_WEAK_LIVELINESS = 0x1;
    static const Int32 ID_DEFAULT_STATUS_WIDGET = -2;
    static const String PASSWORD_TYPE_KEY;
    static const String PASSWORD_TYPE_ALTERNATE_KEY;
    static const String LOCKSCREEN_BIOMETRIC_WEAK_FALLBACK;
    static const String BIOMETRIC_WEAK_EVER_CHOSEN_KEY;
    static const String LOCKSCREEN_POWER_BUTTON_INSTANTLY_LOCKS;

protected:
    static const String LOCKOUT_PERMANENT_KEY;
    static const String LOCKOUT_ATTEMPT_DEADLINE;
    static const String PATTERN_EVER_CHOSEN_KEY;
    static const String LOCK_PASSWORD_SALT_KEY;
    static const String DISABLE_LOCKSCREEN_KEY;
    static const String LOCKSCREEN_OPTIONS;

    static const String PASSWORD_HISTORY_KEY;

private:
    static const String TAG;
    AutoPtr<IContext> mContext;
    AutoPtr<IContentResolver> mContentResolver;
    AutoPtr<IDevicePolicyManager> mDevicePolicyManager;
    AutoPtr<ILockSettings> mLockSettingsService;

    static volatile Int32 sCurrentUserId;
};

}// namespace Internal
}// namespace Widget
}// namespace Droid
}// namespace Elastos

#endif
