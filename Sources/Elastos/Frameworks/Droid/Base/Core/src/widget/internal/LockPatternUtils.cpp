
#include "internal/LockPatternUtils.h"
#include <elastos/core/Character.h>
#include <elastos/core/Math.h>
#include <elastos/core/StringUtils.h>
#include "os/UserHandle.h"
#include "os/ServiceManager.h"
#include "os/SystemClock.h"
#include "os/ServiceManager.h"
#include "os/CServiceManager.h"
#include "os/Binder.h"
#include "os/UserHandle.h"
#include "content/CIntent.h"
#include "provider/Settings.h"
#include "app/ActivityManagerNative.h"
#include "text/TextUtils.h"
#include <elastos/utility/logging/Slogger.h>

using Elastos::Core::CStringWrapper;
using Elastos::Core::Character;
using Elastos::Core::StringUtils;
using Elastos::Core::CObjectContainer;
using Elastos::Droid::App::ActivityManagerNative;
using Elastos::Droid::App::IIActivityManager;
using Elastos::Droid::View::IView;
using Elastos::Droid::View::IIWindowManager;
using Elastos::Droid::View::EIID_IIWindowManager;
using Elastos::Droid::Os::IProcess;
using Elastos::Droid::Os::ServiceManager;
using Elastos::Droid::Os::IServiceManager;
using Elastos::Droid::Os::CServiceManager;
using Elastos::Droid::Os::IUserHandle;
using Elastos::Droid::Os::SystemClock;
using Elastos::Droid::Os::Storage::IMountService;
using Elastos::Droid::Os::Storage::EIID_IMountService;
using Elastos::Droid::Os::IBinder;
using Elastos::Droid::Os::Binder;
using Elastos::Droid::Os::UserHandle;
using Elastos::Droid::Content::IIntent;
using Elastos::Droid::Content::CIntent;
using Elastos::Droid::Content::Pm::IUserInfo;
using Elastos::Droid::Content::Pm::IPackageManager;
using Elastos::Droid::Content::Pm::IPackageInfo;
using Elastos::Droid::Provider::Settings;
using Elastos::Droid::Provider::ISettingsSecure;
using Elastos::Droid::Provider::ISettingsSystem;
using Elastos::Droid::AppWidget::IAppWidgetManager;
using Elastos::Droid::Text::TextUtils;
using Elastos::Utility::Logging::Slogger;

namespace Elastos {
namespace Droid {
namespace Widget {
namespace Internal {

const Int32 LockPatternUtils::FAILED_ATTEMPTS_BEFORE_TIMEOUT;
const Int32 LockPatternUtils::FAILED_ATTEMPTS_BEFORE_RESET;
const Int64 LockPatternUtils::FAILED_ATTEMPT_TIMEOUT_MS;
const Int64 LockPatternUtils::FAILED_ATTEMPT_COUNTDOWN_INTERVAL_MS;
const Int32 LockPatternUtils::FAILED_ATTEMPTS_BEFORE_WIPE_GRACE;
const Int32 LockPatternUtils::MIN_LOCK_PATTERN_SIZE;
const Int32 LockPatternUtils::MIN_PATTERN_REGISTER_FAIL;
const Int32 LockPatternUtils::FLAG_BIOMETRIC_WEAK_LIVELINESS;
const Int32 LockPatternUtils::ID_DEFAULT_STATUS_WIDGET;
const String LockPatternUtils::TAG = String("LockPatternUtils");
const String LockPatternUtils::KEYGUARD_SHOW_USER_SWITCHER = String("showuserswitcher");
const String LockPatternUtils::KEYGUARD_SHOW_SECURITY_CHALLENGE = String("showsecuritychallenge");
const String LockPatternUtils::KEYGUARD_SHOW_APPWIDGET = String("showappwidget");
const String LockPatternUtils::PASSWORD_TYPE_KEY = String("lockscreen.password_type");
const String LockPatternUtils::PASSWORD_TYPE_ALTERNATE_KEY = String("lockscreen.password_type_alternate");
const String LockPatternUtils::LOCKSCREEN_BIOMETRIC_WEAK_FALLBACK = String("lockscreen.biometric_weak_fallback");
const String LockPatternUtils::BIOMETRIC_WEAK_EVER_CHOSEN_KEY = String("lockscreen.biometricweakeverchosen");
const String LockPatternUtils::LOCKSCREEN_POWER_BUTTON_INSTANTLY_LOCKS = String("lockscreen.power_button_instantly_locks");
const String LockPatternUtils::LOCKOUT_PERMANENT_KEY = String("lockscreen.lockedoutpermanently");
const String LockPatternUtils::LOCKOUT_ATTEMPT_DEADLINE = String("lockscreen.lockoutattemptdeadline");
const String LockPatternUtils::PATTERN_EVER_CHOSEN_KEY = String("lockscreen.patterneverchosen");
const String LockPatternUtils::LOCK_PASSWORD_SALT_KEY = String("lockscreen.password_salt");
const String LockPatternUtils::DISABLE_LOCKSCREEN_KEY = String("lockscreen.disabled");
const String LockPatternUtils::LOCKSCREEN_OPTIONS = String("lockscreen.options");
const String LockPatternUtils::PASSWORD_HISTORY_KEY = String("lockscreen.passwordhistory");
volatile Int32 LockPatternUtils::sCurrentUserId = IUserHandle::USER_NULL;

AutoPtr<IDevicePolicyManager> LockPatternUtils::GetDevicePolicyManager()
{
    if (!mDevicePolicyManager) {
        AutoPtr<IInterface> temp;
        mContext->GetSystemService(IContext::DEVICE_POLICY_SERVICE, (IInterface**)&temp);
        mDevicePolicyManager = IDevicePolicyManager::Probe(temp);
        if (!mDevicePolicyManager) {
            Slogger::E(TAG, "Can't get DevicePolicyManagerService: is it running?");
        }
    }
    return mDevicePolicyManager;
}

LockPatternUtils::LockPatternUtils()
{
}

LockPatternUtils::LockPatternUtils(
    /* [in] */ IContext* context)
{
    mContext = context;
    mContentResolver = NULL;
    context->GetContentResolver((IContentResolver**)&mContentResolver);
}

ECode LockPatternUtils::Init(
    /* [in] */ IContext* context)
{
    mContext = context;
    return context->GetContentResolver((IContentResolver**)&mContentResolver);
}

AutoPtr<ILockSettings> LockPatternUtils::GetLockSettings()
{
    if (!mLockSettingsService)
    {
        AutoPtr<IServiceManager> serviceManager;
        CServiceManager::AcquireSingleton((IServiceManager**)&serviceManager);
        AutoPtr<IInterface> temp;
        serviceManager->GetService(String("lock_settings"),
                        (IInterface**)&temp);
        mLockSettingsService = ILockSettings::Probe(temp);
    }
    return mLockSettingsService;
}

Int32 LockPatternUtils::GetRequestedMinimumPasswordLength()
{
    Int32 result = 0;
    GetDevicePolicyManager()->GetPasswordMinimumLength(NULL, GetCurrentOrCallingUserId(), &result);
    return result;
}

Int32 LockPatternUtils::GetRequestedPasswordQuality()
{
    Int32 result = 0;
    GetDevicePolicyManager()->GetPasswordQuality(NULL, GetCurrentOrCallingUserId(), &result);
    return result;
}

Int32 LockPatternUtils::GetRequestedPasswordHistoryLength()
{
    Int32 result = 0;
    GetDevicePolicyManager()->GetPasswordHistoryLength(NULL, GetCurrentOrCallingUserId(), &result);
    return result;
}

Int32 LockPatternUtils::GetRequestedPasswordMinimumLetters()
{
    Int32 result = 0;
    GetDevicePolicyManager()->GetPasswordMinimumLetters(NULL, GetCurrentOrCallingUserId(), &result);
    return result;
}

Int32 LockPatternUtils::GetRequestedPasswordMinimumUpperCase()
{
    Int32 result = 0;
    GetDevicePolicyManager()->GetPasswordMinimumUpperCase(NULL, GetCurrentOrCallingUserId(), &result);
    return result;
}

Int32 LockPatternUtils::GetRequestedPasswordMinimumLowerCase()
{
    Int32 result = 0;
    GetDevicePolicyManager()->GetPasswordMinimumLowerCase(NULL, GetCurrentOrCallingUserId(), &result);
    return result;
}

Int32 LockPatternUtils::GetRequestedPasswordMinimumNumeric()
{
    Int32 result = 0;
    GetDevicePolicyManager()->GetPasswordMinimumNumeric(NULL, GetCurrentOrCallingUserId(), &result);
    return result;
}

Int32 LockPatternUtils::GetRequestedPasswordMinimumSymbols()
{
    Int32 result = 0;
    GetDevicePolicyManager()->GetPasswordMinimumSymbols(NULL, GetCurrentOrCallingUserId(), &result);
    return result;
}

Int32 LockPatternUtils::GetRequestedPasswordMinimumNonLetter()
{
    Int32 result = 0;
    GetDevicePolicyManager()->GetPasswordMinimumNonLetter(NULL, GetCurrentOrCallingUserId(), &result);
    return result;
}

ECode LockPatternUtils::ReportFailedPasswordAttempt()
{
    return GetDevicePolicyManager()->ReportFailedPasswordAttempt(GetCurrentOrCallingUserId());
}

ECode LockPatternUtils::ReportSuccessfulPasswordAttempt()
{
    return GetDevicePolicyManager()->ReportSuccessfulPasswordAttempt(GetCurrentOrCallingUserId());
}

ECode LockPatternUtils::SetCurrentUser(
    /* [in] */ Int32 userId)
{
    sCurrentUserId = userId;
    return NOERROR;
}

Int32 LockPatternUtils::GetCurrentUser()
{
    if (sCurrentUserId != IUserHandle::USER_NULL) {
        return sCurrentUserId;
    }
    AutoPtr<IIActivityManager> am = ActivityManagerNative::GetDefault();
    AutoPtr<IUserInfo> info;
    ECode ec = am->GetCurrentUser((IUserInfo**)&info);
    Int32 id;
    info->GetId(&id);
    if (FAILED(ec)) {
        return IUserHandle::USER_OWNER;
    }
    return id;
}

ECode LockPatternUtils::RemoveUser(
    /* [in] */ Int32 userId)
{
    AutoPtr<ILockSettings> settings = GetLockSettings();
    ECode ec = settings->RemoveUser(userId);
    if (FAILED(ec)) {
        Slogger::E(TAG, String("Couldn't remove lock settings for user ") + StringUtils::Int32ToString(userId));
    }
    return NOERROR;
}

Int32 LockPatternUtils::GetCurrentOrCallingUserId()
{
    Int32 callingUid = Binder::GetCallingUid();
    if (callingUid == IProcess::SYSTEM_UID) {
        return GetCurrentUser();
    } else {
        return UserHandle::GetUserId(callingUid);
    }
}

Boolean LockPatternUtils::CheckPattern(
    /* [in] */ IObjectContainer* pattern)
{
    Int32 userId = GetCurrentOrCallingUserId();
    AutoPtr<ILockSettings> settings = GetLockSettings();
    Boolean matched = FALSE;
    ECode ec = settings->CheckPattern((*PatternToHash(pattern)), userId, &matched);
    if (FAILED(ec)) {
        return TRUE;
    }
    if (matched && (userId == IUserHandle::USER_OWNER)) {
        assert(0 && "TODO");
//            KeyStore::GetInstance()->Password(PatternToString(pattern));
    }
    return matched;

}

Boolean LockPatternUtils::CheckPassword(
    /* [in] */ const String& password)
{
    Int32 userId = GetCurrentOrCallingUserId();
    AutoPtr<ILockSettings> settings = GetLockSettings();
    Boolean matched = FALSE;
    ECode ec = settings->CheckPassword((*PasswordToHash(password)), userId, &matched);
    if (FAILED(ec)) {
        return TRUE;
    }
    if (matched && (userId == IUserHandle::USER_OWNER)) {
        assert(0 && "TODO");
//            KeyStore::GetInstance()->Password(password);
    }
    return matched;

}

Boolean LockPatternUtils::CheckPasswordHistory(
    /* [in] */ const String& password)
{
    String passwordHashString = String((char*)PasswordToHash(password)->GetPayload());
    String passwordHistory = GetString(PASSWORD_HISTORY_KEY);
    if (passwordHistory.IsNullOrEmpty()) {
        return FALSE;
    }
    Int32 passwordHashLength = passwordHashString.GetLength();
    Int32 passwordHistoryLength = GetRequestedPasswordHistoryLength();
    if(passwordHistoryLength == 0) {
        return FALSE;
    }
    Int32 neededPasswordHistoryLength = passwordHashLength * passwordHistoryLength
            + passwordHistoryLength - 1;
    if (passwordHistory.GetLength() > neededPasswordHistoryLength) {
        passwordHistory = passwordHistory.Substring(0, neededPasswordHistoryLength);
    }
    return passwordHistory.Contains(passwordHashString);
}

Boolean LockPatternUtils::SavedPatternExists()
{
    AutoPtr<ILockSettings> settings = GetLockSettings();
    Boolean result = FALSE;
    ECode ec = settings->HavePattern(GetCurrentOrCallingUserId(), &result);
    if (FAILED(ec)) {
        return FALSE;
    }
    return result;

}

Boolean LockPatternUtils::SavedPasswordExists()
{
    AutoPtr<ILockSettings> settings = GetLockSettings();
    Boolean result = FALSE;
    ECode ec = settings->HavePassword(GetCurrentOrCallingUserId(), &result);
    if (FAILED(ec)) {
        return FALSE;
    }
    return result;
}

Boolean LockPatternUtils::IsPatternEverChosen()
{
    return GetBoolean(PATTERN_EVER_CHOSEN_KEY, FALSE);
}

Boolean LockPatternUtils::IsBiometricWeakEverChosen()
{
    return GetBoolean(BIOMETRIC_WEAK_EVER_CHOSEN_KEY, FALSE);
}

Int32 LockPatternUtils::GetActivePasswordQuality()
{
    Int32 activePasswordQuality = IDevicePolicyManager::PASSWORD_QUALITY_UNSPECIFIED;

    Int32 quality = (Int32) GetInt64(PASSWORD_TYPE_KEY, IDevicePolicyManager::PASSWORD_QUALITY_SOMETHING);
    switch (quality) {
        case IDevicePolicyManager::PASSWORD_QUALITY_SOMETHING:
            if (IsLockPatternEnabled()) {
                activePasswordQuality = IDevicePolicyManager::PASSWORD_QUALITY_SOMETHING;
            }
            break;
        case IDevicePolicyManager::PASSWORD_QUALITY_BIOMETRIC_WEAK:
            if (IsBiometricWeakInstalled()) {
                activePasswordQuality = IDevicePolicyManager::PASSWORD_QUALITY_BIOMETRIC_WEAK;
            }
            break;
        case IDevicePolicyManager::PASSWORD_QUALITY_NUMERIC:
            if (IsLockPasswordEnabled()) {
                activePasswordQuality = IDevicePolicyManager::PASSWORD_QUALITY_NUMERIC;
            }
            break;
        case IDevicePolicyManager::PASSWORD_QUALITY_ALPHABETIC:
            if (IsLockPasswordEnabled()) {
                activePasswordQuality = IDevicePolicyManager::PASSWORD_QUALITY_ALPHABETIC;
            }
            break;
        case IDevicePolicyManager::PASSWORD_QUALITY_ALPHANUMERIC:
            if (IsLockPasswordEnabled()) {
                activePasswordQuality = IDevicePolicyManager::PASSWORD_QUALITY_ALPHANUMERIC;
            }
            break;
        case IDevicePolicyManager::PASSWORD_QUALITY_COMPLEX:
            if (IsLockPasswordEnabled()) {
                activePasswordQuality = IDevicePolicyManager::PASSWORD_QUALITY_COMPLEX;
            }
            break;
    }

    return activePasswordQuality;
}

/**
 * Clear any lock pattern or password.
 */
ECode LockPatternUtils::ClearLock(
    /* [in] */ Boolean isFallback)
{
    if(!isFallback) DeleteGallery();
    SaveLockPassword(String(""), IDevicePolicyManager::PASSWORD_QUALITY_SOMETHING);
    SetLockPatternEnabled(FALSE);
    SaveLockPattern(NULL);
    SetInt64(PASSWORD_TYPE_KEY, IDevicePolicyManager::PASSWORD_QUALITY_UNSPECIFIED);
    SetInt64(PASSWORD_TYPE_ALTERNATE_KEY, IDevicePolicyManager::PASSWORD_QUALITY_UNSPECIFIED);

    return NOERROR;
}

/**
 * Disable showing lock screen at all when the DevicePolicyManager allows it.
 * This is only meaningful if pattern, pin or password are not set.
 *
 * @param disable Disables lock screen when TRUE
 */
ECode LockPatternUtils::SetLockScreenDisabled(
    /* [in] */ Boolean disable)
{
    SetInt64(DISABLE_LOCKSCREEN_KEY, disable ? 1 : 0);

    return NOERROR;
}

/**
 * Determine if LockScreen can be disabled. This is used, for example, to tell if we should
 * show LockScreen or go straight to the home screen.
 *
 * @return TRUE if lock screen is can be disabled
 */
Boolean LockPatternUtils::IsLockScreenDisabled()
{
    return !IsSecure() && GetInt64(DISABLE_LOCKSCREEN_KEY, 0) != 0;
}

/**
 * Calls back SetupFaceLock to delete the temporary gallery file
 */
ECode LockPatternUtils::DeleteTempGallery()
{
    AutoPtr<IIntent> intent;
    CIntent::New((IIntent**)&intent);
    intent->SetAction(String("com.android.facelock.DELETE_GALLERY"));
    AutoPtr< ArrayOf<Boolean> > bArray = ArrayOf<Boolean>::Alloc(1);
    (*bArray)[0] = TRUE;
    intent->PutBooleanArrayExtra(String("deleteTempGallery"), bArray);
    mContext->SendBroadcast(intent);
    return NOERROR;
}

/**
 * Calls back SetupFaceLock to delete the gallery file when the lock type is changed
*/
ECode LockPatternUtils::DeleteGallery()
{
    if (UsingBiometricWeak()) {
        AutoPtr<IIntent> intent;
        CIntent::New((IIntent**)&intent);
        intent->SetAction(String("com.android.facelock.DELETE_GALLERY"));
        AutoPtr< ArrayOf<Boolean> > bArray = ArrayOf<Boolean>::Alloc(1);
        (*bArray)[0] = TRUE;
        intent->PutBooleanArrayExtra(String("deleteGallery"), bArray);
        mContext->SendBroadcast(intent);
    }
    return NOERROR;
}

/**
 * Save a lock pattern.
 * @param pattern The new pattern to save.
 */
ECode LockPatternUtils::SaveLockPattern(
    /* [in] */ IObjectContainer* pattern)
{
    return SaveLockPattern(pattern, FALSE);
}

/**
 * Save a lock pattern.
 * @param pattern The new pattern to save.
 * @param isFallback Specifies if this is a fallback to biometric weak
 */
ECode LockPatternUtils::SaveLockPattern(
    /* [in] */ IObjectContainer* pattern,
    /* [in] */ Boolean isFallback)
{
    // Compute the hash
    AutoPtr< ArrayOf<Byte> > hash = LockPatternUtils::PatternToHash(pattern);
    //try {
        GetLockSettings()->SetLockPattern(*hash, GetCurrentOrCallingUserId());
        AutoPtr<IDevicePolicyManager> dpm = GetDevicePolicyManager();
        assert(0 && "TODO");
//        AutoPtr<IKeyStore> keyStore = KeyStore::GetInstance();
        if (pattern) {
//            keyStore->Password(PatternToString(pattern));
            SetBoolean(PATTERN_EVER_CHOSEN_KEY, TRUE);
            if (!isFallback) {
                DeleteGallery();
                SetInt64(PASSWORD_TYPE_KEY, IDevicePolicyManager::PASSWORD_QUALITY_SOMETHING);
                Int32 count = 0;
                pattern->GetObjectCount(&count);
                dpm->SetActivePasswordState(IDevicePolicyManager::PASSWORD_QUALITY_SOMETHING,
                        count, 0, 0, 0, 0, 0, 0, GetCurrentOrCallingUserId());
            } else {
                SetInt64(PASSWORD_TYPE_KEY, IDevicePolicyManager::PASSWORD_QUALITY_BIOMETRIC_WEAK);
                SetInt64(PASSWORD_TYPE_ALTERNATE_KEY,
                        IDevicePolicyManager::PASSWORD_QUALITY_SOMETHING);
                FinishBiometricWeak();
                dpm->SetActivePasswordState(IDevicePolicyManager::PASSWORD_QUALITY_BIOMETRIC_WEAK,
                        0, 0, 0, 0, 0, 0, 0, GetCurrentOrCallingUserId());
            }
        } else {

            Boolean res = FALSE;
//            keyStore->IsEmpty(&res);
//            if (res) {
//                keyStore->Reset();
//            }
            dpm->SetActivePasswordState(IDevicePolicyManager::PASSWORD_QUALITY_UNSPECIFIED, 0, 0,
                    0, 0, 0, 0, 0, GetCurrentOrCallingUserId());
        }
    /*} catch (RemoteException re) {
        Log.e(TAG, "Couldn't save lock pattern " + re);
    }*/
    return NOERROR;
}

/**
 * Compute the password quality from the given password string.
 */
Int32 LockPatternUtils::ComputePasswordQuality(
    /* [in] */ const String& password)
{
    Boolean hasDigit = FALSE;
    Boolean hasNonDigit = FALSE;
    Int32 len = password.GetLength();
    for (Int32 i = 0; i < len; i++) {
        if (Character::IsDigit(password.GetChar(i))) {
            hasDigit = TRUE;
        } else {
            hasNonDigit = TRUE;
        }
    }

    if (hasNonDigit && hasDigit) {
        return IDevicePolicyManager::PASSWORD_QUALITY_ALPHANUMERIC;
    }
    if (hasNonDigit) {
        return IDevicePolicyManager::PASSWORD_QUALITY_ALPHABETIC;
    }
    if (hasDigit) {
        return IDevicePolicyManager::PASSWORD_QUALITY_NUMERIC;
    }
    return IDevicePolicyManager::PASSWORD_QUALITY_UNSPECIFIED;
}

/** Update the encryption password if it is enabled **/
void LockPatternUtils::UpdateEncryptionPassword(
    /* [in] */ const String& password)
{
    AutoPtr<IDevicePolicyManager> dpm = GetDevicePolicyManager();

    Int32 status;
    dpm->GetStorageEncryptionStatus(GetCurrentOrCallingUserId(), &status);
    if (status != IDevicePolicyManager::ENCRYPTION_STATUS_ACTIVE) {
        return;
    }
    // is right?
    AutoPtr<IBinder> service = IBinder::Probe(ServiceManager::GetService(String("mount")));
    if (service == NULL) {
        Slogger::E(TAG, "Could not find the mount service to update the encryption password");
        return;
    }

    AutoPtr<IMountService> mountService = (IMountService*)service->Probe(EIID_IMountService);// = IMountService.Stub.asInterface(service);
    Int32 res = 0;
    ECode ec = mountService->ChangeEncryptionPassword(password, &res);
    if (FAILED(ec)) {
        Slogger::E(TAG, "Error changing encryption password");
    }

}

/**
 * Save a lock password.  Does not ensure that the password is as good
 * as the requested mode, but will adjust the mode to be as good as the
 * pattern.
 * @param password The password to save
 * @param quality {@see DevicePolicyManager#getPasswordQuality(android.content.ComponentName)}
 */
ECode LockPatternUtils::SaveLockPassword(
    /* [in] */ const String& password,
    /* [in] */ Int32 quality)
{
    SaveLockPassword(password, quality, FALSE, GetCurrentOrCallingUserId());

    return NOERROR;
}

/**
 * Save a lock password.  Does not ensure that the password is as good
 * as the requested mode, but will adjust the mode to be as good as the
 * pattern.
 * @param password The password to save
 * @param quality {@see DevicePolicyManager#getPasswordQuality(android.content.ComponentName)}
 * @param isFallback Specifies if this is a fallback to biometric weak
 */
ECode LockPatternUtils::SaveLockPassword(
    /* [in] */ const String& password,
    /* [in] */ Int32 quality,
    /* [in] */ Boolean isFallback)
{
    SaveLockPassword(password, quality, isFallback, GetCurrentOrCallingUserId());

    return NOERROR;
}

/**
 * Save a lock password.  Does not ensure that the password is as good
 * as the requested mode, but will adjust the mode to be as good as the
 * pattern.
 * @param password The password to save
 * @param quality {@see DevicePolicyManager#getPasswordQuality(android.content.ComponentName)}
 * @param isFallback Specifies if this is a fallback to biometric weak
 * @param userHandle The userId of the user to change the password for
 */
ECode LockPatternUtils::SaveLockPassword(
    /* [in] */ const String& password,
    /* [in] */ Int32 quality,
    /* [in] */ Boolean isFallback,
    /* [in] */ Int32 userHandle)
{
    // Compute the hash
    AutoPtr< ArrayOf<Byte> > hash = PasswordToHash(password);
    //try {
        assert(0 && "TODO");
        //KeyStore is not completed
        GetLockSettings()->SetLockPassword(*hash, userHandle);
        AutoPtr<IDevicePolicyManager> dpm = GetDevicePolicyManager();
//        AutoPtr<IKeyStore> keyStore = KeyStore::GetInstance();
        if (!password.IsNullOrEmpty()) {
            if (userHandle == IUserHandle::USER_OWNER) {
                // Update the encryption password.
                UpdateEncryptionPassword(password);

                // Update the keystore password
//                keyStore->Password(password);
            }

            Int32 computedQuality = ComputePasswordQuality(password);
            if (!isFallback) {
                DeleteGallery();
                SetInt64(PASSWORD_TYPE_KEY, Elastos::Core::Math::Max(quality, computedQuality), userHandle);
                if (computedQuality != IDevicePolicyManager::PASSWORD_QUALITY_UNSPECIFIED) {
                    Int32 letters = 0;
                    Int32 uppercase = 0;
                    Int32 lowercase = 0;
                    Int32 numbers = 0;
                    Int32 symbols = 0;
                    Int32 nonletter = 0;
                    for (Int32 i = 0; i < password.GetLength(); i++) {
                        Char32 c = password.GetChar(i);
                        if (c >= 'A' && c <= 'Z') {
                            letters++;
                            uppercase++;
                        } else if (c >= 'a' && c <= 'z') {
                            letters++;
                            lowercase++;
                        } else if (c >= '0' && c <= '9') {
                            numbers++;
                            nonletter++;
                        } else {
                            symbols++;
                            nonletter++;
                        }
                    }
                    dpm->SetActivePasswordState(Elastos::Core::Math::Max(quality, computedQuality),
                            password.GetLength(), letters, uppercase, lowercase,
                            numbers, symbols, nonletter, userHandle);
                } else {
                    // The password is not anything.
                    dpm->SetActivePasswordState(
                            IDevicePolicyManager::PASSWORD_QUALITY_UNSPECIFIED,
                            0, 0, 0, 0, 0, 0, 0, userHandle);
                }
            } else {
                // Case where it's a fallback for biometric weak
                SetInt64(PASSWORD_TYPE_KEY, IDevicePolicyManager::PASSWORD_QUALITY_BIOMETRIC_WEAK,
                        userHandle);
                SetInt64(PASSWORD_TYPE_ALTERNATE_KEY, Elastos::Core::Math::Max(quality, computedQuality),
                        userHandle);
                FinishBiometricWeak();
                dpm->SetActivePasswordState(IDevicePolicyManager::PASSWORD_QUALITY_BIOMETRIC_WEAK,
                        0, 0, 0, 0, 0, 0, 0, userHandle);
            }
            // Add the password to the password history. We assume all
            // password
            // hashes have the same length for simplicity of implementation.
            String passwordHistory = GetString(PASSWORD_HISTORY_KEY, userHandle);
            if (passwordHistory.IsNullOrEmpty()) {
                passwordHistory = String("");
            }
            Int32 passwordHistoryLength = GetRequestedPasswordHistoryLength();
            if (passwordHistoryLength == 0) {
                passwordHistory = String("");
            } else {
                passwordHistory = String((char*)hash->GetPayload()) + "," + passwordHistory;
                // Cut it to contain passwordHistoryLength hashes
                // and passwordHistoryLength -1 commas.
                passwordHistory = passwordHistory.Substring(0, Elastos::Core::Math::Min(hash->GetLength()
                        * passwordHistoryLength + passwordHistoryLength - 1, passwordHistory
                        .GetLength()));
            }
            SetString(PASSWORD_HISTORY_KEY, passwordHistory, userHandle);
        } else {
            // Conditionally reset the keystore if empty. If
            // non-empty, we are just switching key guard type
            Boolean res = FALSE;
//            keyStore->IsEmpty(&res);
            if (res) {
//                keyStore->Reset();
            }
            dpm->SetActivePasswordState(
                    IDevicePolicyManager::PASSWORD_QUALITY_UNSPECIFIED, 0, 0, 0, 0, 0, 0, 0,
                    userHandle);
        }
    //} catch (RemoteException re) {
    //    // Cant do much
    //    Log.e(TAG, "Unable to save lock password " + re);
    //}
    return NOERROR;
}

/**
 * Retrieves the quality mode we're in.
 * {@see DevicePolicyManager#getPasswordQuality(android.content.ComponentName)}
 *
 * @return stored password quality
 */
Int32 LockPatternUtils::GetKeyguardStoredPasswordQuality()
{
    Int32 quality = (Int32) GetInt64(PASSWORD_TYPE_KEY, IDevicePolicyManager::PASSWORD_QUALITY_SOMETHING);
    // If the user has chosen to use weak biometric sensor, then return the backup locking
    // method and treat biometric as a special case.
    if (quality == IDevicePolicyManager::PASSWORD_QUALITY_BIOMETRIC_WEAK) {
        quality = (Int32) GetInt64(PASSWORD_TYPE_ALTERNATE_KEY,
                    IDevicePolicyManager::PASSWORD_QUALITY_SOMETHING);
    }
    return quality;
}

/**
 * @return TRUE if the lockscreen method is set to biometric weak
 */
Boolean LockPatternUtils::UsingBiometricWeak()
{
    Int32 quality = (Int32) GetInt64(PASSWORD_TYPE_KEY, IDevicePolicyManager::PASSWORD_QUALITY_SOMETHING);
    return quality == IDevicePolicyManager::PASSWORD_QUALITY_BIOMETRIC_WEAK;
}

/**
 * Deserialize a pattern.
 * @param string The pattern serialized with {@link #patternToString}
 * @return The pattern.
 */
AutoPtr<IObjectContainer> LockPatternUtils::StringToPattern(
    /* [in] */ const String& string)
{
    AutoPtr<IObjectContainer> result;
    CObjectContainer::New((IObjectContainer**)&result);

    AutoPtr<ILockPatternViewCellHelper> helper;
    assert(0 && "TODO");
    //CLockPatternViewCellHelper::AcquireSingleton((ILockPatternViewCellHelper**)&helper);
    for (Int32 i = 0; i < string.GetLength(); i++) {
        Char8 b = string[i];
        AutoPtr<ILockPatternViewCell> cell;
        helper->Of(b / 3, b % 3, (ILockPatternViewCell**)&cell);
        result->Add(cell);
    }
    return result;
}

/**
 * Serialize a pattern.
 * @param pattern The pattern.
 * @return The pattern in string form.
 */
String LockPatternUtils::PatternToString(
    /* [in] */ IObjectContainer* pattern)
{
    if (!pattern) {
        return String("");
    }
    Int32 patternSize = 0;
    pattern->GetObjectCount(&patternSize);

    AutoPtr<IObjectEnumerator> em;
    pattern->GetObjectEnumerator((IObjectEnumerator**)&em);
    AutoPtr<ArrayOf<Char8> > res = ArrayOf<Char8>::Alloc(patternSize);
    Boolean hasNext = TRUE;
    for (Int32 i = 0; i < patternSize && hasNext; i++, em->MoveNext(&hasNext)) {
        AutoPtr<ILockPatternViewCell> cell;

        em->Current((IInterface**)&cell);
        Int32 row = 0, column = 0;
        cell->GetRow(&row);
        cell->GetColumn(&column);
        (*res)[i] = (Char8) (row * 3 + column);
    }

    return String(res->GetPayload());
}

/*
 * Generate an SHA-1 hash for the pattern. Not the most secure, but it is
 * at least a second level of protection. First level is that the file
 * is in a location only readable by the system process.
 * @param pattern the gesture pattern.
 * @return the hash of the pattern in a Byte array.
 */
AutoPtr< ArrayOf<Byte> > LockPatternUtils::PatternToHash(
    /* [in] */ IObjectContainer* pattern)
{
    if (pattern == NULL) {
        return NULL;
    }

    Int32 patternSize = 0;
    pattern->GetObjectCount(&patternSize);
    AutoPtr<ArrayOf<Byte> > res = ArrayOf<Byte>::Alloc(patternSize);
    AutoPtr<IObjectEnumerator> em;
    pattern->GetObjectEnumerator((IObjectEnumerator**)&em);
    Boolean hasNext = TRUE;
    for (Int32 i = 0; i < patternSize && hasNext; i++, em->MoveNext(&hasNext)) {
        AutoPtr<ILockPatternViewCell> cell;
        em->Current((IInterface**)&cell);
        Int32 row = 0, column = 0;
        cell->GetRow(&row);
        cell->GetColumn(&column);
        (*res)[i] = (Byte) (row * 3 + column);
    }
    assert(0 && "TODO");
////    try {
//        MessageDigest md = MessageDigest.getInstance("SHA-1");
//        Byte[] hash = md.digest(res);
//        return hash;
////    } catch (NoSuchAlgorithmException nsa) {
//        return res;
////    }
    return res;
}

String LockPatternUtils::GetSalt()
{
    Int64 salt = GetInt64(LOCK_PASSWORD_SALT_KEY, 0);
    if (salt == 0) {
    assert(0 && "TODO");
        //try {
//        salt = SecureRandom::GetInstance("SHA1PRNG")->NextLong();
 //       SetInt64(LOCK_PASSWORD_SALT_KEY, salt);
            //Log.v(TAG, "Initialized lock password salt");
        //} catch (NoSuchAlgorithmException e) {
        //    // Throw an exception rather than storing a password we'll never be able to recover
        //    throw new IllegalStateException("Couldn't get SecureRandom number", e);
        //}
    }
    return StringUtils::Int64ToString(salt, 16);
}

/*
 * Generate a hash for the given password. To avoid brute force attacks, we use a salted hash.
 * Not the most secure, but it is at least a second level of protection. First level is that
 * the file is in a location only readable by the system process.
 * @param password the gesture pattern.
 * @return the hash of the pattern in a Byte array.
 */
AutoPtr< ArrayOf<Byte> > LockPatternUtils::PasswordToHash(
    /* [in] */ const String& password)
{
    if (password.IsNullOrEmpty()) {
        return NULL;
    }
    String algo;
    AutoPtr<ArrayOf<Byte> > hashed;
    //try {
        String saltedPassword = (password + GetSalt());
        assert(0 && "TODO");
//        AutoPtr<ArrayOf<Byte> > sha1 = MessageDigest::GetInstance(algo = "SHA-1")->Digest(saltedPassword);
//        AutoPtr<ArrayOf<Byte> > md5 = MessageDigest::GetInstance(algo = "MD5")->Digest(saltedPassword);
//        hashed = (toHex(sha1) + toHex(md5)).getBytes();
    /*} catch (NoSuchAlgorithmException e) {
        Log.w(TAG, "Failed to encode string because of missing algorithm: " + algo);
    }*/
    return hashed;
}

String LockPatternUtils::ToHex(ArrayOf<Byte>* ary)
{
    String hex = String("0123456789ABCDEF");
    String ret = String("");
    for (Int32 i = 0; i < ary->GetLength(); i++) {
        /*ret.Append(hex[((*ary[i]) >> 4) & 0xf]);
        ret.Append(hex[(*ary[i]) & 0xf]);*/
    }
    return ret;
}

/**
 * @return Whether the lock password is enabled, or if it is set as a backup for biometric weak
 */
Boolean LockPatternUtils::IsLockPasswordEnabled()
{
    Int64 mode = GetInt64(PASSWORD_TYPE_KEY, 0);
    Int64 backupMode = GetInt64(PASSWORD_TYPE_ALTERNATE_KEY, 0);
    Boolean passwordEnabled = mode == IDevicePolicyManager::PASSWORD_QUALITY_ALPHABETIC
            || mode == IDevicePolicyManager::PASSWORD_QUALITY_NUMERIC
            || mode == IDevicePolicyManager::PASSWORD_QUALITY_ALPHANUMERIC
            || mode == IDevicePolicyManager::PASSWORD_QUALITY_COMPLEX;
    Boolean backupEnabled = backupMode == IDevicePolicyManager::PASSWORD_QUALITY_ALPHABETIC
            || backupMode == IDevicePolicyManager::PASSWORD_QUALITY_NUMERIC
            || backupMode == IDevicePolicyManager::PASSWORD_QUALITY_ALPHANUMERIC
            || backupMode == IDevicePolicyManager::PASSWORD_QUALITY_COMPLEX;

    return SavedPasswordExists() && (passwordEnabled ||
            (UsingBiometricWeak() && backupEnabled));
}

/**
 * @return Whether the lock pattern is enabled, or if it is set as a backup for biometric weak
 */
Boolean LockPatternUtils::IsLockPatternEnabled()
{
    Boolean backupEnabled =
            GetInt64(PASSWORD_TYPE_ALTERNATE_KEY, IDevicePolicyManager::PASSWORD_QUALITY_SOMETHING)
            == IDevicePolicyManager::PASSWORD_QUALITY_SOMETHING;

    return GetBoolean(ISettingsSecure::LOCK_PATTERN_ENABLED, FALSE)
            && (GetInt64(PASSWORD_TYPE_KEY, IDevicePolicyManager::PASSWORD_QUALITY_SOMETHING)
                    == IDevicePolicyManager::PASSWORD_QUALITY_SOMETHING ||
                    (UsingBiometricWeak() && backupEnabled));
}

/**
 * @return Whether biometric weak lock is installed and that the front facing camera exists
 */
Boolean LockPatternUtils::IsBiometricWeakInstalled()
{
    AutoPtr<IPackageManager> pm;
    mContext->GetPackageManager((IPackageManager**)&pm);
    //try {
    AutoPtr<IPackageInfo> info;
    ECode ec = pm->GetPackageInfo(String("com.android.facelock"), IPackageManager::GET_ACTIVITIES, (IPackageInfo**)&info);
    if(FAILED(ec)) {
        return FALSE;
    }

    Boolean res = FALSE;
    pm->HasSystemFeature(IPackageManager::FEATURE_CAMERA_FRONT, &res);
    if (!res) {
        return FALSE;
    }
    Boolean cameraDisable = FALSE;
    GetDevicePolicyManager()->GetCameraDisabled(NULL, GetCurrentOrCallingUserId(), &cameraDisable);
    if (cameraDisable) {
        return FALSE;
    }
    return TRUE;
}

/**
 * Set whether biometric weak liveliness is enabled.
 */
ECode LockPatternUtils::SetBiometricWeakLivelinessEnabled(
    /* [in] */ Boolean enabled)
{
    Int64 currentFlag = GetInt64(ISettingsSecure::LOCK_BIOMETRIC_WEAK_FLAGS, 0L);
    Int64 newFlag = 0;
    if (enabled) {
        newFlag = currentFlag | FLAG_BIOMETRIC_WEAK_LIVELINESS;
    } else {
        newFlag = currentFlag & ~FLAG_BIOMETRIC_WEAK_LIVELINESS;
    }
    SetInt64(ISettingsSecure::LOCK_BIOMETRIC_WEAK_FLAGS, newFlag);
    return NOERROR;
}

/**
 * @return Whether the biometric weak liveliness is enabled.
 */
Boolean LockPatternUtils::IsBiometricWeakLivelinessEnabled()
{
    Int64 currentFlag = GetInt64(ISettingsSecure::LOCK_BIOMETRIC_WEAK_FLAGS, 0L);
    return ((currentFlag & FLAG_BIOMETRIC_WEAK_LIVELINESS) != 0);
}

/**
 * Set whether the lock pattern is enabled.
 */
ECode LockPatternUtils::SetLockPatternEnabled(
    /* [in] */ Boolean enabled)
{
    SetBoolean(ISettingsSecure::LOCK_PATTERN_ENABLED, enabled);

    return NOERROR;
}

/**
 * @return Whether the visible pattern is enabled.
 */
Boolean LockPatternUtils::IsVisiblePatternEnabled()
{
    return GetBoolean(ISettingsSecure::LOCK_PATTERN_VISIBLE, FALSE);
}

/**
 * Set whether the visible pattern is enabled.
 */
ECode LockPatternUtils::SetVisiblePatternEnabled(
    /* [in] */ Boolean enabled)
{
    SetBoolean(ISettingsSecure::LOCK_PATTERN_VISIBLE, enabled);

    return NOERROR;
}

/**
 * @return Whether tactile feedback for the pattern is enabled.
 */
Boolean LockPatternUtils::IsTactileFeedbackEnabled()
{
    Int32 value;
    Settings::System::GetInt32ForUser(mContentResolver,
            ISettingsSystem::HAPTIC_FEEDBACK_ENABLED, 1, IUserHandle::USER_CURRENT, &value);
    return value != 0;
}

/**
 * Set and store the lockout deadline, meaning the user can't attempt his/her unlock
 * pattern until the deadline has passed.
 * @return the chosen deadline.
 */
Int64 LockPatternUtils::SetLockoutAttemptDeadline()
{
    Int64 deadline = SystemClock::GetElapsedRealtime() + FAILED_ATTEMPT_TIMEOUT_MS;
    SetInt64(LOCKOUT_ATTEMPT_DEADLINE, deadline);
    return deadline;
}

/**
 * @return The elapsed time in millis in the future when the user is allowed to
 *   attempt to enter his/her lock pattern, or 0 if the user is welcome to
 *   enter a pattern.
 */
Int64 LockPatternUtils::GetLockoutAttemptDeadline()
{
    Int64 deadline = GetInt64(LOCKOUT_ATTEMPT_DEADLINE, 0L);
    Int64 now = SystemClock::GetElapsedRealtime();
    if (deadline < now || deadline > (now + FAILED_ATTEMPT_TIMEOUT_MS)) {
        return 0L;
    }
    return deadline;
}

/**
 * @return Whether the user is permanently locked out until they verify their
 *   credentials.  Occurs after {@link #FAILED_ATTEMPTS_BEFORE_RESET} failed
 *   attempts.
 */
Boolean LockPatternUtils::IsPermanentlyLocked()
{
    return GetBoolean(LOCKOUT_PERMANENT_KEY, FALSE);
}

/**
 * Set the state of whether the device is permanently locked, meaning the user
 * must authenticate via other means.
 *
 * @param locked Whether the user is permanently locked out until they verify their
 *   credentials.  Occurs after {@link #FAILED_ATTEMPTS_BEFORE_RESET} failed
 *   attempts.
 */
ECode LockPatternUtils::SetPermanentlyLocked(
    /* [in] */ Boolean locked)
{
    SetBoolean(LOCKOUT_PERMANENT_KEY, locked);

    return NOERROR;
}

Boolean LockPatternUtils::IsEmergencyCallCapable()
{
    AutoPtr<IResources> rs;

    mContext->GetResources((IResources**)&rs);

    Boolean res = FALSE;
    rs->GetBoolean(R::bool_::config_voice_capable, &res);

    return res;
}

Boolean LockPatternUtils::IsPukUnlockScreenEnable()
{
    AutoPtr<IResources> rs;

    mContext->GetResources((IResources**)&rs);

    Boolean res = FALSE;
    rs->GetBoolean(R::bool_::config_enable_puk_unlock_screen, &res);

    return res;
}

Boolean LockPatternUtils::IsEmergencyCallEnabledWhileSimLocked()
{
    AutoPtr<IResources> rs;

    mContext->GetResources((IResources**)&rs);

    Boolean res = FALSE;
    rs->GetBoolean(R::bool_::config_enable_emergency_call_while_sim_locked, &res);

    return res;
}

/**
 * @return A formatted string of the next alarm (for showing on the lock screen),
 *   or NULL if there is no next alarm.
 */
String LockPatternUtils::GetNextAlarm()
{
    String nextAlarm;
    Settings::System::GetStringForUser(mContentResolver,
            ISettingsSystem::NEXT_ALARM_FORMATTED, IUserHandle::USER_CURRENT, &nextAlarm);
    if (nextAlarm.IsNullOrEmpty() || TextUtils::IsEmpty(nextAlarm)) {
        return String("");
    }
    return nextAlarm;
}

Boolean LockPatternUtils::GetBoolean(
    /* [in] */ const String& secureSettingKey,
    /* [in] */ Boolean defaultValue)
{
    Boolean res = FALSE;
    ECode ec = GetLockSettings()->GetBoolean(secureSettingKey, defaultValue,
            GetCurrentOrCallingUserId(), &res);
    if (FAILED(ec)) {
        return defaultValue;
    }
    return res;
}

void LockPatternUtils::SetBoolean(
    /* [in] */ const String& secureSettingKey,
    /* [in] */ Boolean enabled)
{
    //try {
    AutoPtr<ILockSettings> settings = GetLockSettings();
    ECode ec= settings->SetBoolean(secureSettingKey, enabled, GetCurrentOrCallingUserId());
    if (FAILED(ec)) {
        Slogger::E(TAG, "Couldn't write Boolean");
    }
}

AutoPtr< ArrayOf<Int32> > LockPatternUtils::GetAppWidgets()
{
    String appWidgetIdString;
    Settings::Secure::GetStringForUser(mContentResolver, ISettingsSecure::LOCK_SCREEN_APPWIDGET_IDS,
            IUserHandle::USER_CURRENT, &appWidgetIdString);
    String delims = String(",");
    if (!appWidgetIdString.IsNullOrEmpty() && appWidgetIdString.GetLength() > 0) {
        AutoPtr< ArrayOf<String> > appWidgetStringIds;
        StringUtils::Split(appWidgetIdString, delims, (ArrayOf<String>**)&appWidgetStringIds);

        AutoPtr< ArrayOf<Int32> > appWidgetIds = ArrayOf<Int32>::Alloc(appWidgetStringIds->GetLength());
        for (Int32 i = 0; i < appWidgetStringIds->GetLength(); i++) {
            String appWidget = (*appWidgetStringIds)[i];
            assert(0 && "TODO");
            ///// is right?
            /*Int32 hex = appWidget.Find("0x");
            if(hex == -1 )
            {
                (*appWidgetIds)[i] = StringUtils::ParseInt32(appWidget);
            } else {
                String temp = appWidget.Substring(0, hex) + appWidget.SubString(hex + 2);

                (*appWidgetIds)[i] = StringUtils::ParseInt32(temp, 16);
            }*/
        }
        return appWidgetIds;
    }
    return NULL;
}

String LockPatternUtils::CombineStrings(
    /* [in] */ ArrayOf<Int32>* list,
    /* [in] */ const String& separator)
{
    Int32 listLength = list->GetLength();

    switch (listLength) {
        case 0: {
            return String("");
        }
        case 1: {
            return StringUtils::Int32ToString((*list)[0]);
        }
    }

    Int32 strLength = 0;
    Int32 separatorLength = separator.GetLength();

    AutoPtr<ArrayOf<String> > stringList = ArrayOf<String>::Alloc(list->GetLength());
    for (Int32 i = 0; i < listLength; i++) {
        (*stringList)[i] = StringUtils::Int32ToString((*list)[i]);
        strLength += (*stringList)[i].GetLength();
        if (i < listLength - 1) {
            strLength += separatorLength;
        }
    }

    StringBuilder sb(strLength);

    for (Int32 i = 0; i < listLength; i++) {
        sb.AppendInt32((*list)[i]);
        if (i < listLength - 1) {
            sb.AppendString(separator);
        }
    }
    String result;
    sb.ToString(&result);
    return result;
}

// appwidget used when appwidgets are disabled (we make an exception for
// default clock widget)
ECode LockPatternUtils::WriteFallbackAppWidgetId(
    /* [in] */ Int32 appWidgetId)
{
    Boolean result;
    return Settings::Secure::PutInt32ForUser(mContentResolver,
            ISettingsSecure::LOCK_SCREEN_FALLBACK_APPWIDGET_ID,
            appWidgetId, IUserHandle::USER_CURRENT, &result);
}

// appwidget used when appwidgets are disabled (we make an exception for
// default clock widget)
Int32 LockPatternUtils::GetFallbackAppWidgetId()
{
    Int32 result;
    Settings::Secure::GetInt32ForUser(
    mContentResolver, ISettingsSecure::LOCK_SCREEN_FALLBACK_APPWIDGET_ID,
    IAppWidgetManager::INVALID_APPWIDGET_ID, IUserHandle::USER_CURRENT, &result);
    return result;
}

void LockPatternUtils::WriteAppWidgets(
    /* [in] */ ArrayOf<Int32>* appWidgetIds)
{
    Boolean result;
    Settings::Secure::PutStringForUser(mContentResolver,
            ISettingsSecure::LOCK_SCREEN_APPWIDGET_IDS,
            CombineStrings(appWidgetIds, String(",")),
            IUserHandle::USER_CURRENT, &result);
}

// TODO: log an error if this returns FALSE
Boolean LockPatternUtils::AddAppWidget(
    /* [in] */ Int32 widgetId,
    /* [in] */ Int32 index)
{
    AutoPtr<ArrayOf<Int32> > widgets = GetAppWidgets();
    if (widgets == NULL) {
        return FALSE;
    }
    if (index < 0 || index > widgets->GetLength()) {
        return FALSE;
    }
    AutoPtr<ArrayOf<Int32> > newWidgets = ArrayOf<Int32>::Alloc(widgets->GetLength()+ 1);
    for (Int32 i = 0, j = 0; i < newWidgets->GetLength(); i++) {
        if (index == i) {
            (*newWidgets)[i] = widgetId;
            i++;
        }
        if (i < newWidgets->GetLength()) {
            (*newWidgets)[i] = (*widgets)[j];
            j++;
        }
    }
    WriteAppWidgets(newWidgets);
    return TRUE;
}

Boolean LockPatternUtils::RemoveAppWidget(
    /* [in] */ Int32 widgetId)
{
    AutoPtr<ArrayOf<Int32> > widgets = GetAppWidgets();

    if (widgets->GetLength() == 0) {
        return FALSE;
    }

    AutoPtr<ArrayOf<Int32> > newWidgets = ArrayOf<Int32>::Alloc(widgets->GetLength() - 1);
    for (Int32 i = 0, j = 0; i < widgets->GetLength(); i++) {
        if ((*widgets)[i] == widgetId) {
            // continue...
        } else if (j >= newWidgets->GetLength()) {
            // we couldn't find the widget
            return FALSE;
        } else {
            (*newWidgets)[j] = (*widgets)[i];
            j++;
        }
    }
    WriteAppWidgets(newWidgets);
    return TRUE;
}

Int64 LockPatternUtils::GetInt64(
    /* [in] */ const String& secureSettingKey,
    /* [in] */ Int64 defaultValue)
{
    AutoPtr<ILockSettings> settings = GetLockSettings();
    Int64 result = FALSE;
    ECode ec = settings->GetInt64(secureSettingKey, defaultValue,
            GetCurrentOrCallingUserId(), &result);
    if (FAILED(ec)) {
        return defaultValue;
    }
    return result;
}

void LockPatternUtils::SetInt64(
    /* [in] */ const String& secureSettingKey,
    /* [in] */ Int64 value)
{
    SetInt64(secureSettingKey, value, GetCurrentOrCallingUserId());
}

void LockPatternUtils::SetInt64(
    /* [in] */ const String& secureSettingKey,
    /* [in] */ Int64 value,
    /* [in] */ Int32 userHandle)
{
    AutoPtr<ILockSettings> settings = GetLockSettings();
    ECode ec = settings->SetInt64(secureSettingKey, value, GetCurrentOrCallingUserId());

    if (FAILED(ec)) {
        Slogger::E(TAG, "Couldn't write Int64 ");
    }
}

String LockPatternUtils::GetString(
    /* [in] */ const String& secureSettingKey)
{
    return GetString(secureSettingKey, GetCurrentOrCallingUserId());
}

String LockPatternUtils::GetString(
    /* [in] */ const String& secureSettingKey,
    /* [in] */ Int32 userHandle)
{
    AutoPtr<ILockSettings> settings = GetLockSettings();
    String result;
    ECode ec = settings->GetString(secureSettingKey, String(""), userHandle, &result);
    if (FAILED(ec)) {
        return String("");
    }
    return result;
}

void LockPatternUtils::SetString(
    /* [in] */ const String& secureSettingKey,
    /* [in] */ const String& value,
    /* [in] */ Int32 userHandle)
{
    AutoPtr<ILockSettings> settings = GetLockSettings();
    ECode ec = settings->SetString(secureSettingKey, value, userHandle);
    if (FAILED(ec)) {
        Slogger::E(TAG, "Couldn't write string ");
    }
}

Boolean LockPatternUtils::IsSecure()
{
    Int64 mode = GetKeyguardStoredPasswordQuality();
    Boolean isPattern = mode == IDevicePolicyManager::PASSWORD_QUALITY_SOMETHING;
    Boolean isPassword = mode == IDevicePolicyManager::PASSWORD_QUALITY_NUMERIC
            || mode == IDevicePolicyManager::PASSWORD_QUALITY_ALPHABETIC
            || mode == IDevicePolicyManager::PASSWORD_QUALITY_ALPHANUMERIC
            || mode == IDevicePolicyManager::PASSWORD_QUALITY_COMPLEX;
    Boolean secure = isPattern && IsLockPatternEnabled() && SavedPatternExists()
            || isPassword && SavedPasswordExists();
    return secure;
}

/**
 * Sets the emergency button visibility based on isEmergencyCallCapable().
 *
 * If the emergency button is visible, sets the text on the emergency button
 * to indicate what action will be taken.
 *
 * If there's currently a call in progress, the button will take them to the call
 * @param button the button to update
 * @param the phone state:
 *  {@link TelephonyManager#CALL_STATE_IDLE}
 *  {@link TelephonyManager#CALL_STATE_RINGING}
 *  {@link TelephonyManager#CALL_STATE_OFFHOOK}
 * @param shown indicates whether the given screen wants the emergency button to show at all
 * @param button
 * @param phoneState
 * @param shown shown if TRUE; hidden if FALSE
 * @param upperCase if TRUE, converts button label string to upper case
 */
ECode LockPatternUtils::UpdateEmergencyCallButtonState(
    /* [in] */ IButton* button,
    /* [in] */ Int32  phoneState,
    /* [in] */ Boolean shown,
    /* [in] */ Boolean upperCase,
    /* [in] */ Boolean showIcon)
{
    if (IsEmergencyCallCapable() && shown) {
        button->SetVisibility(IView::VISIBLE);
    } else {
        button->SetVisibility(IView::GONE);
        return NOERROR;
    }

    Int32 textId = 0;
    if (phoneState == 2 /*ITelephonyManager::CALL_STATE_OFFHOOK*/) {
        // show "return to call" text and show phone icon
        textId = R::string::lockscreen_return_to_call;
        Int32 phoneCallIcon = showIcon ? R::drawable::stat_sys_phone_call : 0;
        button->SetCompoundDrawablesWithIntrinsicBounds(phoneCallIcon, 0, 0, 0);
    } else {
        textId = R::string::lockscreen_emergency_call;
        Int32 emergencyIcon = showIcon ? R::drawable::ic_emergency : 0;
        button->SetCompoundDrawablesWithIntrinsicBounds(emergencyIcon, 0, 0, 0);
    }
    if (upperCase) {
        AutoPtr<ICharSequence> original;
        AutoPtr<IResources> rs;
        mContext->GetResources((IResources**)&rs);
        rs->GetText(textId, (ICharSequence**)&original);


        String upper;
        if(original) {
            original->ToString(&upper);
            upper = upper.ToUpperCase();
        }else {
            upper = String(NULL);
        }
        AutoPtr<ICharSequence> csq;
        CStringWrapper::New(upper, (ICharSequence**)&csq);
        button->SetText(csq);
    } else {
        button->SetText(textId);
    }

    return NOERROR;
}

/**
 * @deprecated
 * @param button
 * @param phoneState
 * @param shown
 */
ECode LockPatternUtils::UpdateEmergencyCallButtonState(
    /* [in] */ IButton* button,
    /* [in] */ Int32  phoneState,
    /* [in] */ Boolean shown)
{
    UpdateEmergencyCallButtonState(button, phoneState, shown, FALSE, TRUE);
    return NOERROR;
}

/**
 * Resumes a call in progress. Typically launched from the EmergencyCall button
 * on various lockscreens.
 *
 * @return TRUE if we were able to tell InCallScreen to show.
 */
Boolean LockPatternUtils::ResumeCall()
{
//    ITelephony phone = ITelephony.Stub.asInterface(ServiceManager.checkService("phone"));
//    //try {
//        Boolean res;
//        if (phone != NULL && (phone->ShowCallScreen(&res), res)) {
//            return TRUE;
//        }
//    //} catch (RemoteException e) {
//        // What can we do?
//    //}
    assert(0 && "TODO");
    return FALSE;
}

void LockPatternUtils::FinishBiometricWeak()
{
    SetBoolean(BIOMETRIC_WEAK_EVER_CHOSEN_KEY, TRUE);

    // Launch intent to show final screen, this also
    // moves the temporary gallery to the actual gallery
    AutoPtr<IIntent> intent;
    CIntent::New((IIntent**)&intent);

    //is right?
    intent->SetClassName(String("Elastos.Droid.Core.eco"),
            String("SetupEndScreen"));
    mContext->StartActivity(intent);
}

ECode LockPatternUtils::SetPowerButtonInstantlyLocks(
    /* [in] */ Boolean enabled)
{
    SetBoolean(LOCKSCREEN_POWER_BUTTON_INSTANTLY_LOCKS, enabled);

    return NOERROR;
}

Boolean LockPatternUtils::GetPowerButtonInstantlyLocks()
{
    return GetBoolean(LOCKSCREEN_POWER_BUTTON_INSTANTLY_LOCKS, TRUE);
}

Boolean LockPatternUtils::IsSafeModeEnabled()
{
    AutoPtr<IInterface> window = ServiceManager::GetService(String("window"));
    AutoPtr<IIWindowManager> manager = (IIWindowManager*)window->Probe(EIID_IIWindowManager);
    Boolean result = FALSE;
    if(manager)
    {
        manager->IsSafeModeEnabled(&result);
    }
    return result;
}

}// namespace Internal
}// namespace Widget
}// namespace Droid
}// namespace Elastos
