
#include "elastos/droid/internal/widget/LockPatternUtils.h"
#include "elastos/droid/internal/widget/CLockPatternUtilsCache.h"
#include "Elastos.Droid.App.h"
#include "elastos/droid/app/ActivityManagerNative.h"
#include "Elastos.Droid.Content.h"
#include "elastos/droid/content/CIntent.h"
#include "elastos/droid/Manifest.h"
#include "elastos/droid/os/UserHandle.h"
#include "elastos/droid/os/ServiceManager.h"
#include "elastos/droid/os/SystemClock.h"
#include "elastos/droid/os/Binder.h"
#include "elastos/droid/os/UserHandle.h"
#include "Elastos.Droid.Provider.h"
#include "elastos/droid/provider/Settings.h"
#include "elastos/droid/R.h"
#include "elastos/droid/text/TextUtils.h"
#include <elastos/core/Character.h>
#include <elastos/core/Math.h>
#include <elastos/core/StringUtils.h>
#include <elastos/utility/logging/Slogger.h>

using Elastos::Droid::App::ActivityManagerNative;
using Elastos::Droid::App::IIActivityManager;
using Elastos::Droid::AppWidget::IAppWidgetManager;
using Elastos::Droid::Content::IIntent;
using Elastos::Droid::Content::CIntent;
using Elastos::Droid::Content::Pm::IUserInfo;
using Elastos::Droid::Content::Pm::IPackageManager;
using Elastos::Droid::Content::Pm::IPackageInfo;
using Elastos::Droid::Manifest;
using Elastos::Droid::Os::IProcess;
using Elastos::Droid::Os::ServiceManager;
using Elastos::Droid::Os::IServiceManager;
using Elastos::Droid::Os::IUserHandle;
using Elastos::Droid::Os::SystemClock;
using Elastos::Droid::Os::Storage::IMountService;
using Elastos::Droid::Os::Storage::EIID_IMountService;
using Elastos::Droid::Os::IBinder;
using Elastos::Droid::Os::Binder;
using Elastos::Droid::Os::UserHandle;
using Elastos::Droid::Provider::Settings;
using Elastos::Droid::Provider::ISettingsSecure;
using Elastos::Droid::Provider::ISettingsSystem;
using Elastos::Droid::R;
using Elastos::Droid::Text::TextUtils;
using Elastos::Droid::View::IView;
using Elastos::Droid::View::IIWindowManager;
using Elastos::Droid::View::EIID_IIWindowManager;
using Elastos::Core::CString;
using Elastos::Core::Character;
using Elastos::Core::StringUtils;
using Elastos::Utility::Logging::Slogger;

namespace Elastos {
namespace Droid {
namespace Internal {
namespace Widget {

LockPatternUtils::LockAsyncTask::LockAsyncTask(
    /* [in] */ Int32 type,
    /* [in] */ const String& password,
    /* [in] */ IInterface* service)
    : mType(type)
    , mPassword(password)
    , mService(service)
{
    REFCOUNT_ADD(this);
}

ECode LockPatternUtils::LockAsyncTask::DoInBackground(
    /* [in] */ ArrayOf<IInterface*>* params,
    /* [out] */ IInterface** result)
{
    VALIDATE_NOT_NULL(result);
    AutoPtr<IMountService> mountService = IMountService::Probe(mService);
    Int32 value = 0;
    if (FAILED(mountService->ChangeEncryptionPassword(mType, mPassword, &value))) {
        Slogger::E(LockPatternUtils::TAG, "Error changing encryption password");
    }
    *result = NULL;
    return NOERROR;
}

ECode LockPatternUtils::LockAsyncTask::OnPostExecute(
    /* [in] */ IInterface* result)
{
    REFCOUNT_RELEASE(this);
    return NOERROR;
}

const String LockPatternUtils::TAG("LockPatternUtils");
const Boolean LockPatternUtils::DEBUG = FALSE;
const String LockPatternUtils::LOCK_SCREEN_OWNER_INFO = ISettingsSecure::LOCK_SCREEN_OWNER_INFO;
const String LockPatternUtils::LOCK_SCREEN_OWNER_INFO_ENABLED = ISettingsSecure::LOCK_SCREEN_OWNER_INFO_ENABLED;
const String LockPatternUtils::ENABLED_TRUST_AGENTS("lockscreen.enabledtrustagents");
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

AutoPtr<ITrustManager> LockPatternUtils::GetTrustManager()
{
    AutoPtr<IInterface> obj;
    mContext->GetSystemService(IContext::TRUST_SERVICE, (IInterface**)&obj);
    AutoPtr<ITrustManager> trust = ITrustManager::Probe(obj);
    if (trust == NULL) {
        Slogger::E(TAG, "Can't get TrustManagerService: is it running?"/*,
                new IllegalStateException("Stack trace:")*/);
    }
    return trust;
}

CAR_INTERFACE_IMPL(LockPatternUtils, Object, ILockPatternUtils);
LockPatternUtils::LockPatternUtils()
    : mMultiUserMode(FALSE)
{
}

ECode LockPatternUtils::constructor(
    /* [in] */ IContext* context)
{
    mContext = context;
    context->GetContentResolver((IContentResolver**)&mContentResolver);

    // If this is being called by the system or by an application like keyguard that
    // has permision INTERACT_ACROSS_USERS, then LockPatternUtils will operate in multi-user
    // mode where calls are for the current user rather than the user of the calling process.
    Int32 result = 0;
    context->CheckCallingOrSelfPermission(
            Manifest::permission::INTERACT_ACROSS_USERS_FULL, &result);
    mMultiUserMode = result == IPackageManager::PERMISSION_GRANTED;
    return NOERROR;
}

AutoPtr<IILockSettings> LockPatternUtils::GetLockSettings()
{
    if (mLockSettingsService == NULL) {
        AutoPtr<IInterface> temp = ServiceManager::GetService(String("lock_settings"));
        mLockSettingsService = IILockSettings::Probe(
                CLockPatternUtilsCache::GetInstance(IILockSettings::Probe(temp)));
    }
    return mLockSettingsService;
}

ECode LockPatternUtils::GetRequestedMinimumPasswordLength(
    /* [out] */ Int32* result)
{
    VALIDATE_NOT_NULL(result);
    return GetDevicePolicyManager()->GetPasswordMinimumLength(NULL, GetCurrentOrCallingUserId(), result);
}

ECode LockPatternUtils::GetRequestedPasswordQuality(
    /* [out] */ Int32* result)
{
    VALIDATE_NOT_NULL(result);
    return GetDevicePolicyManager()->GetPasswordQuality(NULL, GetCurrentOrCallingUserId(), result);
}

ECode LockPatternUtils::GetRequestedPasswordHistoryLength(
    /* [out] */ Int32* result)
{
    VALIDATE_NOT_NULL(result);
    return GetDevicePolicyManager()->GetPasswordHistoryLength(NULL, GetCurrentOrCallingUserId(), result);
}

ECode LockPatternUtils::GetRequestedPasswordMinimumLetters(
    /* [out] */ Int32* result)
{
    VALIDATE_NOT_NULL(result);
    return GetDevicePolicyManager()->GetPasswordMinimumLetters(NULL, GetCurrentOrCallingUserId(), result);
}

ECode LockPatternUtils::GetRequestedPasswordMinimumUpperCase(
    /* [out] */ Int32* result)
{
    VALIDATE_NOT_NULL(result);
    return GetDevicePolicyManager()->GetPasswordMinimumUpperCase(NULL, GetCurrentOrCallingUserId(), result);
}

ECode LockPatternUtils::GetRequestedPasswordMinimumLowerCase(
    /* [out] */ Int32* result)
{
    VALIDATE_NOT_NULL(result);
    return GetDevicePolicyManager()->GetPasswordMinimumLowerCase(NULL, GetCurrentOrCallingUserId(), result);
}

ECode LockPatternUtils::GetRequestedPasswordMinimumNumeric(
    /* [out] */ Int32* result)
{
    VALIDATE_NOT_NULL(result);
    return GetDevicePolicyManager()->GetPasswordMinimumNumeric(NULL, GetCurrentOrCallingUserId(), result);
}

ECode LockPatternUtils::GetRequestedPasswordMinimumSymbols(
    /* [out] */ Int32* result)
{
    VALIDATE_NOT_NULL(result);
    return GetDevicePolicyManager()->GetPasswordMinimumSymbols(NULL, GetCurrentOrCallingUserId(), result);
}

ECode LockPatternUtils::GetRequestedPasswordMinimumNonLetter(
    /* [out] */ Int32* result)
{
    VALIDATE_NOT_NULL(result);
    return GetDevicePolicyManager()->GetPasswordMinimumNonLetter(NULL, GetCurrentOrCallingUserId(), result);
}

ECode LockPatternUtils::ReportFailedPasswordAttempt()
{
    Int32 userId = GetCurrentOrCallingUserId();
    GetDevicePolicyManager()->ReportFailedPasswordAttempt(userId);
    GetTrustManager()->ReportUnlockAttempt(FALSE /* authenticated */, userId);
    GetTrustManager()->ReportRequireCredentialEntry(userId);
    return NOERROR;
}

ECode LockPatternUtils::ReportSuccessfulPasswordAttempt()
{
    GetDevicePolicyManager()->ReportSuccessfulPasswordAttempt(GetCurrentOrCallingUserId());
    getTrustManager().reportUnlockAttempt(TRUE /* authenticated */,
            getCurrentOrCallingUserId());
}

ECode LockPatternUtils::SetCurrentUser(
    /* [in] */ Int32 userId)
{
    sCurrentUserId = userId;
    return NOERROR;
}

ECode LockPatternUtils::GetCurrentUser(
    /* [out] */ Int32* user)
{
    VALIDATE_NOT_NULL(user);
    if (sCurrentUserId != IUserHandle::USER_NULL) {
        *user = sCurrentUserId;
        return NOERROR;
    }
    AutoPtr<IIActivityManager> am = ActivityManagerNative::GetDefault();
    AutoPtr<IUserInfo> info;
    ECode ec = am->GetCurrentUser((IUserInfo**)&info);
    Int32 id;
    info->GetId(&id);
    if (FAILED(ec)) {
        *user = IUserHandle::USER_OWNER;
        return NOERROR;
    }
    *user = id;
    return NOERROR;
}

ECode LockPatternUtils::RemoveUser(
    /* [in] */ Int32 userId)
{
    ECode ec = GetLockSettings()->RemoveUser(userId);
    if (FAILED(ec)) {
        Slogger::E(TAG, String("Couldn't remove lock settings for user ") + StringUtils::Int32ToString(userId));
    }
    return NOERROR;
}

Int32 LockPatternUtils::GetCurrentOrCallingUserId()
{
    if (mMultiUserMode) {
        return GetCurrentUser();
    }
    else {
        return UserHandle::GetCallingUserId();
    }
}

ECode LockPatternUtils::CheckPattern(
    /* [in] */ IList* pattern,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
    Int32 userId = GetCurrentOrCallingUserId();
    Boolean matched = FALSE;
    ECode ec = GetLockSettings()->CheckPattern(PatternToString(pattern), userId, &matched);
    if (FAILED(ec)) {
        *result = TRUE;
        return NOERROR;
    }
    *result = matched;
    return NOERROR;
}

ECode LockPatternUtils::CheckPassword(
    /* [in] */ const String& password,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
    Int32 userId = GetCurrentOrCallingUserId();
    Boolean matched = FALSE;
    ECode ec = GetLockSettings()->CheckPassword(password, userId, &matched);
    if (FAILED(ec)) {
        *result = TRUE;
        return NOERROR;
    }
    *result = matched;
    return NOERROR;
}

ECode LockPatternUtils::CheckVoldPassword(
    /* [out] */ Boolean* match)
{
    VALIDATE_NOT_NULL(match);
    final Int32 userId = getCurrentOrCallingUserId();
    try {
        return getLockSettings().checkVoldPassword(userId);
    } catch (RemoteException re) {
        return FALSE;
    }
}

ECode LockPatternUtils::CheckPasswordHistory(
    /* [in] */ const String& password,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
    String passwordHashString = String(PasswordToHash(password, GetCurrentOrCallingUserId()));
    String passwordHistory = GetString(PASSWORD_HISTORY_KEY);
    if (passwordHistory.IsNullOrEmpty()) {
        *result = FALSE;
        return NOERROR;
    }
    Int32 passwordHashLength = passwordHashString.GetLength();
    Int32 passwordHistoryLength = GetRequestedPasswordHistoryLength();
    if(passwordHistoryLength == 0) {
        *result = FALSE;
        return NOERROR;
    }
    Int32 neededPasswordHistoryLength = passwordHashLength * passwordHistoryLength
            + passwordHistoryLength - 1;
    if (passwordHistory.GetLength() > neededPasswordHistoryLength) {
        passwordHistory = passwordHistory.Substring(0, neededPasswordHistoryLength);
    }
    *result = passwordHistory.Contains(passwordHashString);
    return NOERROR;
}

ECode LockPatternUtils::SavedPatternExists(
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
    ECode ec = GetLockSettings()->HavePattern(GetCurrentOrCallingUserId(), result);
    if (FAILED(ec)) {
        *result = FALSE;
        return NOERROR;
    }
    return NOERROR;
}

ECode LockPatternUtils::SavedPasswordExists(
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
    ECode ec = GetLockSettings()->HavePassword(GetCurrentOrCallingUserId(), result);
    if (FAILED(ec)) {
        *result = FALSE;
        return NOERROR;
    }
    return NOERROR;
}

ECode LockPatternUtils::IsPatternEverChosen(
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
    *result = GetBoolean(PATTERN_EVER_CHOSEN_KEY, FALSE);
    return NOERROR;
}

ECode LockPatternUtils::IsBiometricWeakEverChosen(
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
    *result = GetBoolean(BIOMETRIC_WEAK_EVER_CHOSEN_KEY, FALSE);
    return NOERROR;
}

ECode LockPatternUtils::GetActivePasswordQuality(
    /* [out] */ Int32* result)
{
    VALIDATE_NOT_NULL(result);
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
        case IDevicePolicyManager::PASSWORD_QUALITY_NUMERIC_COMPLEX:
            if (IsLockPasswordEnabled()) {
                activePasswordQuality = IDevicePolicyManager::PASSWORD_QUALITY_NUMERIC_COMPLEX;
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

    *result = activePasswordQuality;
    return NOERROR;
}

ECode LockPatternUtils::ClearLock(
    /* [in] */ Boolean isFallback)
{
    if(!isFallback) DeleteGallery();
    SaveLockPassword(String(""), IDevicePolicyManager::PASSWORD_QUALITY_SOMETHING);
    SetLockPatternEnabled(FALSE);
    SaveLockPattern(NULL);
    SetInt64(PASSWORD_TYPE_KEY, IDevicePolicyManager::PASSWORD_QUALITY_UNSPECIFIED);
    SetInt64(PASSWORD_TYPE_ALTERNATE_KEY, IDevicePolicyManager::PASSWORD_QUALITY_UNSPECIFIED);
    OnAfterChangingPassword();
    return NOERROR;
}

ECode LockPatternUtils::SetLockScreenDisabled(
    /* [in] */ Boolean disable)
{
    SetInt64(DISABLE_LOCKSCREEN_KEY, disable ? 1 : 0);

    return NOERROR;
}

ECode LockPatternUtils::IsLockScreenDisabled(
    /* [out] */ Boolean* disabled)
{
    VALIDATE_NOT_NULL(disabled);
    Boolean value = FALSE;
    if ((IsSecure(&value), !value) && GetInt64(DISABLE_LOCKSCREEN_KEY, 0) != 0) {
        // Check if the number of switchable users forces the lockscreen.
        AutoPtr<IList/*<UserInfo*/> users;
        UserManager::Get(mContext)->GetUsers(TRUE, (IList**)&users);
        Int32 userCount = 0;
        users->GetSize(&userCount);
        Int32 switchableUsers = 0;
        for (Int32 i = 0; i < userCount; i++) {
            AutoPtr<IInterface> item;
            users->Get(i, (IInterface**)&item);
            if (IUserInfo::Probe(item)->SupportsSwitchTo(&value), value) {
                switchableUsers++;
            }
        }
        *disabled = switchableUsers < 2;
        return NOERROR;
    }
    *disabled = FALSE;
    return NOERROR;
}

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

ECode LockPatternUtils::DeleteGallery()
{
    Boolean result = FALSE;
    if (UsingBiometricWeak(&result), result) {
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

ECode LockPatternUtils::SaveLockPattern(
    /* [in] */ IList* pattern)
{
    return SaveLockPattern(pattern, FALSE);
}

ECode LockPatternUtils::SaveLockPattern(
    /* [in] */ IList* pattern,
    /* [in] */ Boolean isFallback)
{
    // try {
    Int32 userId = GetCurrentOrCallingUserId();
    AutoPtr<IDevicePolicyManager> dpm;
    Boolean required = FALSE;
    Int32 size = 0;
    ECode ec = NOERROR;
    ec = GetLockSettings()->SetLockPattern(PatternToString(pattern), userId);
    FAIL_GOTO(ec, Error);
    dpm = GetDevicePolicyManager();
    if (pattern != NULL) {
        // Update the device encryption password.
        if (userId == IUserHandle::USER_OWNER && LockPatternUtils::IsDeviceEncryptionEnabled()) {
            IsCredentialRequiredToDecrypt(TRUE, &required);
            if (!required) {
                ClearEncryptionPassword();
            }
            else {
                String stringPattern = PatternToString(pattern);
                UpdateEncryptionPassword(IStorageManager::CRYPT_TYPE_PATTERN, stringPattern);
            }
        }

        SetBoolean(PATTERN_EVER_CHOSEN_KEY, TRUE);
        if (!isFallback) {
            DeleteGallery();
            SetInt64(PASSWORD_TYPE_KEY, IDevicePolicyManager::PASSWORD_QUALITY_SOMETHING);
            pattern->GetSize(&size);
            ec = dpm->SetActivePasswordState(IDevicePolicyManager::PASSWORD_QUALITY_SOMETHING,
                    size, 0, 0, 0, 0, 0, 0, userId);
            FAIL_GOTO(ec, Error);
        }
        else {
            SetInt64(PASSWORD_TYPE_KEY, IDevicePolicyManager::PASSWORD_QUALITY_BIOMETRIC_WEAK);
            SetInt64(PASSWORD_TYPE_ALTERNATE_KEY,
                    IDevicePolicyManager::PASSWORD_QUALITY_SOMETHING);
            FinishBiometricWeak();
            ec = dpm->SetActivePasswordState(IDevicePolicyManager::PASSWORD_QUALITY_BIOMETRIC_WEAK,
                    0, 0, 0, 0, 0, 0, 0, userId);
            FAIL_GOTO(ec, Error);
        }
    }
    else {
        ec = dpm->SetActivePasswordState(IDevicePolicyManager::PASSWORD_QUALITY_UNSPECIFIED, 0, 0,
                0, 0, 0, 0, 0, userId);
        FAIL_GOTO(ec, Error);
    }
    OnAfterChangingPassword();

Error:
    if (FAILED(ec)) {
        Slogger::E(TAG, "Couldn't save lock pattern ");
    }
    // } catch (RemoteException re) {
    //     Log.e(TAG, "Couldn't save lock pattern " + re);
    // }
    return NOERROR;
}

void LockPatternUtils::UpdateCryptoUserInfo()
{
    Int32 userId = getCurrentOrCallingUserId();
    if (userId != IUserHandle::USER_OWNER) {
        return;
    }

    Boolean enabled = FALSE;
    IsOwnerInfoEnabled(&enabled);
    String ownerInfo;
    if (enabled) {
        GetOwnerInfo(userId, ownerInfo);
    }
    else {
        ownerInfo = String("");
    }

    AutoPtr<IInterface> service = ServiceManager::GetService(String("mount"));
    if (service == NULL) {
        Slogger::E(TAG, "Could not find the mount service to update the user info");
        return;
    }

    AutoPtr<IMountService> mountService = IMountService::Probe(service);
    Slogger::D(TAG, "Setting owner info");
    if (FAILED(mountService->SetField(IStorageManager::OWNER_INFO_KEY, ownerInfo))) {
        Slogger::E(TAG, "Error changing user info");
    }
}

ECode LockPatternUtils::SetOwnerInfo(
    /* [in] */ const String& info,
    /* [in] */ Int32 userId)
{
    SetString(LOCK_SCREEN_OWNER_INFO, info, userId);
    UpdateCryptoUserInfo();
    return NOERROR;
}

ECode LockPatternUtils::SetOwnerInfoEnabled(
    /* [in] */ Boolean enabled)
{
    SetBoolean(LOCK_SCREEN_OWNER_INFO_ENABLED, enabled);
    UpdateCryptoUserInfo();
    return NOERROR;
}

ECode LockPatternUtils::GetOwnerInfo(
    /* [in] */ Int32 userId,
    /* [out] */ String* info)
{
    VALIDATE_NOT_NULL(info);
    return GetString(LOCK_SCREEN_OWNER_INFO, info);
}

ECode LockPatternUtils::IsOwnerInfoEnabled(
    /* [out] */ Boolean* enabled)
{
    VALIDATE_NOT_NULL(enabled);
    return GetBoolean(LOCK_SCREEN_OWNER_INFO_ENABLED, FALSE, enabled);
}

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
        return MaxLengthSequence(password) > MAX_ALLOWED_SEQUENCE
                ? DevicePolicyManager.PASSWORD_QUALITY_NUMERIC
                : DevicePolicyManager.PASSWORD_QUALITY_NUMERIC_COMPLEX;
    }
    return IDevicePolicyManager::PASSWORD_QUALITY_UNSPECIFIED;
}

Int32 LockPatternUtils::CategoryChar(
    /* [in] */ Char32 c)
{
    if ('a' <= c && c <= 'z') return 0;
    if ('A' <= c && c <= 'Z') return 1;
    if ('0' <= c && c <= '9') return 2;
    return 3;
}

Int32 LockPatternUtils::MaxDiffCategory(
    /* [in] */ Int32 category)
{
    if (category == 0 || category == 1) return 1;
    else if (category == 2) return 10;
    return 0;
}

Int32 LockPatternUtils::MaxLengthSequence(
    /* [in] */ const String& string)
{
    if (string.length() == 0) return 0;
    char previousChar = string.charAt(0);
    Int32 category = categoryChar(previousChar); //current category of the sequence
    Int32 diff = 0; //difference between two consecutive characters
    boolean hasDiff = FALSE; //if we are currently targeting a sequence
    Int32 maxLength = 0; //maximum length of a sequence already found
    Int32 startSequence = 0; //where the current sequence started
    for (Int32 current = 1; current < string.length(); current++) {
        char currentChar = string.charAt(current);
        Int32 categoryCurrent = categoryChar(currentChar);
        Int32 currentDiff = (Int32) currentChar - (Int32) previousChar;
        if (categoryCurrent != category || Math.abs(currentDiff) > maxDiffCategory(category)) {
            maxLength = Math.max(maxLength, current - startSequence);
            startSequence = current;
            hasDiff = FALSE;
            category = categoryCurrent;
        }
        else {
            if(hasDiff && currentDiff != diff) {
                maxLength = Math.max(maxLength, current - startSequence);
                startSequence = current - 1;
            }
            diff = currentDiff;
            hasDiff = TRUE;
        }
        previousChar = currentChar;
    }
    maxLength = Math.max(maxLength, string.length() - startSequence);
    return maxLength;
}

void LockPatternUtils::UpdateEncryptionPassword(
    /* [in] */ Int32 type,
    /* [in] */ const String& password)
{
    if (!IsDeviceEncryptionEnabled()) {
        return;
    }
    AutoPtr<IInterface> service = ServiceManager::GetService(String("mount"));
    if (service == NULL) {
        Slogger::E(TAG, "Could not find the mount service to update the encryption password");
        return;
    }

    AutoPtr<LockAsyncTask> task = new LockAsyncTask(service);
    task->Execute();
}

ECode LockPatternUtils::SaveLockPassword(
    /* [in] */ const String& password,
    /* [in] */ Int32 quality)
{
    SaveLockPassword(password, quality, FALSE, GetCurrentOrCallingUserId());

    return NOERROR;
}

ECode LockPatternUtils::SaveLockPassword(
    /* [in] */ const String& password,
    /* [in] */ Int32 quality,
    /* [in] */ Boolean isFallback)
{
    SaveLockPassword(password, quality, isFallback, GetCurrentOrCallingUserId());

    return NOERROR;
}

ECode LockPatternUtils::SaveLockPassword(
    /* [in] */ const String& password,
    /* [in] */ Int32 quality,
    /* [in] */ Boolean isFallback,
    /* [in] */ Int32 userHandle)
{
    // try {
    DevicePolicyManager dpm = getDevicePolicyManager();
    if (!TextUtils.isEmpty(password)) {
        getLockSettings().setLockPassword(password, userHandle);
        Int32 computedQuality = computePasswordQuality(password);

        // Update the device encryption password.
        if (userHandle == UserHandle.USER_OWNER
                && LockPatternUtils.isDeviceEncryptionEnabled()) {
            if (!isCredentialRequiredToDecrypt(TRUE)) {
                clearEncryptionPassword();
            } else {
                boolean numeric = computedQuality
                        == DevicePolicyManager.PASSWORD_QUALITY_NUMERIC;
                boolean numericComplex = computedQuality
                        == DevicePolicyManager.PASSWORD_QUALITY_NUMERIC_COMPLEX;
                Int32 type = numeric || numericComplex ? StorageManager.CRYPT_TYPE_PIN
                        : StorageManager.CRYPT_TYPE_PASSWORD;
                updateEncryptionPassword(type, password);
            }
        }
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
        }
        else {
            byte[] hash = passwordToHash(password, userHandle);
            passwordHistory = String((char*)hash->GetPayload()) + "," + passwordHistory;
            // Cut it to contain passwordHistoryLength hashes
            // and passwordHistoryLength -1 commas.
            passwordHistory = passwordHistory.Substring(0, Elastos::Core::Math::Min(hash->GetLength()
                    * passwordHistoryLength + passwordHistoryLength - 1, passwordHistory
                    .GetLength()));
        }
        SetString(PASSWORD_HISTORY_KEY, passwordHistory, userHandle);
    }
    else {
        // Empty password
        GetLockSettings()->SetLockPassword(NULL, userHandle);
        if (userHandle == IUserHandle::USER_OWNER) {
            // Set the encryption password to default.
            UpdateEncryptionPassword(IStorageManager::CRYPT_TYPE_DEFAULT, NULL);
        }
        dpm->SetActivePasswordState(
                IDevicePolicyManager::PASSWORD_QUALITY_UNSPECIFIED, 0, 0, 0, 0, 0, 0, 0,
                userHandle);
    }

    OnAfterChangingPassword();
    //} catch (RemoteException re) {
    //    // Cant do much
    //    Log.e(TAG, "Unable to save lock password " + re);
    //}
    return NOERROR;
}

Boolean LockPatternUtils::IsDeviceEncrypted()
{
    IMountService mountService = IMountService.Stub.asInterface(
            ServiceManager.getService("mount"));
    try {
        return mountService.getEncryptionState() != IMountService.ENCRYPTION_STATE_NONE
                && mountService.getPasswordType() != StorageManager.CRYPT_TYPE_DEFAULT;
    } catch (RemoteException re) {
        Log.e(TAG, "Error getting encryption state", re);
    }
    return TRUE;
}

Boolean LockPatternUtils::IsDeviceEncryptionEnabled()
{
    final String status = SystemProperties.get("ro.crypto.state", "unsupported");
    return "encrypted".equalsIgnoreCase(status);
}

ECode LockPatternUtils::ClearEncryptionPassword()
{
    UpdateEncryptionPassword(StorageManager.CRYPT_TYPE_DEFAULT, null);
    return NOERROR;
}

ECode LockPatternUtils::GetKeyguardStoredPasswordQuality(
    /* [out] */ Int32* result)
{
    VALIDATE_NOT_NULL(result);
    return GetKeyguardStoredPasswordQuality(GetCurrentOrCallingUserId(), result);
}

ECode LockPatternUtils::GetKeyguardStoredPasswordQuality(
    /* [in] */ Int32 userHandle,
    /* [out] */ Int32* result)
{
    VALIDATE_NOT_NULL(result);
    Int32 quality = (Int32) GetInt64(PASSWORD_TYPE_KEY,
            IDevicePolicyManager::PASSWORD_QUALITY_UNSPECIFIED, userHandle);
    // If the user has chosen to use weak biometric sensor, then return the backup locking
    // method and treat biometric as a special case.
    if (quality == IDevicePolicyManager::PASSWORD_QUALITY_BIOMETRIC_WEAK) {
        quality = (Int32) GetInt64(PASSWORD_TYPE_ALTERNATE_KEY,
                    IDevicePolicyManager::PASSWORD_QUALITY_UNSPECIFIED, userHandle);
    }
    *result = quality;
    return NOERROR;
}

ECode LockPatternUtils::UsingBiometricWeak(
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
    Int32 quality = (Int32) GetInt64(PASSWORD_TYPE_KEY, IDevicePolicyManager::PASSWORD_QUALITY_UNSPECIFIED);
    *result = quality == IDevicePolicyManager::PASSWORD_QUALITY_BIOMETRIC_WEAK;
    return NOERROR;
}

AutoPtr<IList> LockPatternUtils::StringToPattern(
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

AutoPtr< ArrayOf<Byte> > LockPatternUtils::PatternToHash(
    /* [in] */ IList* pattern)
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

String LockPatternUtils::GetSalt(
    /* [in] */ Int32 userId)
{
    Int64 salt = GetInt64(LOCK_PASSWORD_SALT_KEY, 0);
    if (salt == 0) {
    assert(0 && "TODO");
        //try {
//        salt = SecureRandom::GetInstance("SHA1PRNG")->NextLong();
 //       SetInt64(LOCK_PASSWORD_SALT_KEY, salt, userId);
            //Log.v(TAG, "Initialized lock password salt for user: " + userId);
        //} catch (NoSuchAlgorithmException e) {
        //    // Throw an exception rather than storing a password we'll never be able to recover
        //    throw new IllegalStateException("Couldn't get SecureRandom number", e);
        //}
    }
    return StringUtils::Int64ToString(salt, 16);
}

ECode LockPatternUtils::PasswordToHash(
    /* [in] */ const String& password,
    /* [in] */ Int32 userId,
    /* [out, callee] */ ArrayOf<Byte>** arr)
{
    VALIDATE_NOT_NULL(arr);
    *arr = NULL;
    if (password.IsNullOrEmpty()) {
        return NOERROR;
    }
    String algo;
    AutoPtr<ArrayOf<Byte> > hashed;
    //try {
        String saltedPassword = (password + GetSalt(userId));
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

ECode LockPatternUtils::IsLockPasswordEnabled(
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
    Int64 mode = GetInt64(PASSWORD_TYPE_KEY, 0);
    Int64 backupMode = GetInt64(PASSWORD_TYPE_ALTERNATE_KEY, 0);
    Boolean passwordEnabled = mode == IDevicePolicyManager::PASSWORD_QUALITY_ALPHABETIC
            || mode == IDevicePolicyManager::PASSWORD_QUALITY_NUMERIC
            || mode == IDevicePolicyManager::PASSWORD_QUALITY_NUMERIC_COMPLEX
            || mode == IDevicePolicyManager::PASSWORD_QUALITY_ALPHANUMERIC
            || mode == IDevicePolicyManager::PASSWORD_QUALITY_COMPLEX;
    Boolean backupEnabled = backupMode == IDevicePolicyManager::PASSWORD_QUALITY_ALPHABETIC
            || backupMode == IDevicePolicyManager::PASSWORD_QUALITY_NUMERIC
            || backupMode == IDevicePolicyManager::PASSWORD_QUALITY_NUMERIC_COMPLEX
            || backupMode == IDevicePolicyManager::PASSWORD_QUALITY_ALPHANUMERIC
            || backupMode == IDevicePolicyManager::PASSWORD_QUALITY_COMPLEX;

    *result = SavedPasswordExists() && (passwordEnabled ||
            (UsingBiometricWeak() && backupEnabled));
    return NOERROR;
}

ECode LockPatternUtils::IsLockPatternEnabled(
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
    Boolean backupEnabled =
            GetInt64(PASSWORD_TYPE_ALTERNATE_KEY, IDevicePolicyManager::PASSWORD_QUALITY_UNSPECIFIED)
            == IDevicePolicyManager::PASSWORD_QUALITY_SOMETHING;

    *result = GetBoolean(ISettingsSecure::LOCK_PATTERN_ENABLED, FALSE)
            && (GetInt64(PASSWORD_TYPE_KEY, IDevicePolicyManager::PASSWORD_QUALITY_UNSPECIFIED)
                    == IDevicePolicyManager::PASSWORD_QUALITY_SOMETHING ||
                    (UsingBiometricWeak() && backupEnabled));
    return NOERROR;
}

ECode LockPatternUtils::IsBiometricWeakInstalled(
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
    *result = FALSE;
    AutoPtr<IPackageManager> pm;
    mContext->GetPackageManager((IPackageManager**)&pm);
    //try {
    AutoPtr<IPackageInfo> info;
    ECode ec = pm->GetPackageInfo(String("com.android.facelock"), IPackageManager::GET_ACTIVITIES, (IPackageInfo**)&info);
    if(FAILED(ec)) {
        return NOERROR;
    }

    Boolean res = FALSE;
    pm->HasSystemFeature(IPackageManager::FEATURE_CAMERA_FRONT, &res);
    if (!res) {
        return NOERROR;
    }
    Boolean cameraDisable = FALSE;
    GetDevicePolicyManager()->GetCameraDisabled(NULL, GetCurrentOrCallingUserId(), &cameraDisable);
    if (cameraDisable) {
        return NOERROR;
    }
    // TODO: If we decide not to proceed with Face Unlock as a trustlet, this must be changed
    // back to returning TRUE.  If we become certain that Face Unlock will be a trustlet, this
    // entire function and a lot of other code can be removed.
    if (DEBUG) Log.d(TAG, "Forcing isBiometricWeakInstalled() to return FALSE to disable it");
    return NOERROR;
}

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

ECode LockPatternUtils::IsBiometricWeakLivelinessEnabled(
    /* [out] */ Boolean enabled)
{
    VALIDATE_NOT_NULL(enabled);
    Int64 currentFlag = GetInt64(ISettingsSecure::LOCK_BIOMETRIC_WEAK_FLAGS, 0L);
    *enabled = ((currentFlag & FLAG_BIOMETRIC_WEAK_LIVELINESS) != 0);
    return NOERROR;
}

ECode LockPatternUtils::SetLockPatternEnabled(
    /* [in] */ Boolean enabled)
{
    SetBoolean(ISettingsSecure::LOCK_PATTERN_ENABLED, enabled);

    return NOERROR;
}

ECode LockPatternUtils::IsVisiblePatternEnabled(
    /* [out] */ Boolean enabled)
{
    VALIDATE_NOT_NULL(enabled);
    *enabled = GetBoolean(ISettingsSecure::LOCK_PATTERN_VISIBLE, FALSE);
    return NOERROR;
}

ECode LockPatternUtils::SetVisiblePatternEnabled(
    /* [in] */ Boolean enabled)
{
    SetBoolean(ISettingsSecure::LOCK_PATTERN_VISIBLE, enabled);

    // Update for crypto if owner
    Int32 userId = getCurrentOrCallingUserId();
    if (userId != UserHandle.USER_OWNER) {
        return;
    }

    IBinder service = ServiceManager.getService("mount");
    if (service == null) {
        Log.e(TAG, "Could not find the mount service to update the user info");
        return;
    }

    IMountService mountService = IMountService.Stub.asInterface(service);
    try {
        mountService.setField(StorageManager.PATTERN_VISIBLE_KEY, enabled ? "1" : "0");
    } catch (RemoteException e) {
        Log.e(TAG, "Error changing pattern visible state", e);
    }
}

ECode LockPatternUtils::IsTactileFeedbackEnabled(
    /* [out] */ Boolean enabled)
{
    VALIDATE_NOT_NULL(enabled);
    Int32 value;
    Settings::System::GetInt32ForUser(mContentResolver,
            ISettingsSystem::HAPTIC_FEEDBACK_ENABLED, 1, IUserHandle::USER_CURRENT, &value);
    *enabled = value != 0;
    return NOERROR;
}

ECode LockPatternUtils::SetLockoutAttemptDeadline(
    /* [out] */ Int64* result)
{
    VALIDATE_NOT_NULL(result);
    Int64 deadline = SystemClock::GetElapsedRealtime() + FAILED_ATTEMPT_TIMEOUT_MS;
    SetInt64(LOCKOUT_ATTEMPT_DEADLINE, deadline);
    *result = deadline;
    return NOERROR;
}

ECode LockPatternUtils::GetLockoutAttemptDeadline(
    /* [out] */ Int64* result)
{
    VALIDATE_NOT_NULL(result);
    Int64 deadline = GetInt64(LOCKOUT_ATTEMPT_DEADLINE, 0L);
    Int64 now = SystemClock::GetElapsedRealtime();
    if (deadline < now || deadline > (now + FAILED_ATTEMPT_TIMEOUT_MS)) {
        *result = 0L;
        return NOERROR;
    }
    *result = deadline;
    return NOERROR;
}

ECode LockPatternUtils::IsPermanentlyLocked(
    /* [out] */ Boolean* locked)
{
    VALIDATE_NOT_NULL(locked);
    *locked = GetBoolean(LOCKOUT_PERMANENT_KEY, FALSE);
    return NOERROR;
}

ECode LockPatternUtils::SetPermanentlyLocked(
    /* [in] */ Boolean locked)
{
    SetBoolean(LOCKOUT_PERMANENT_KEY, locked);

    return NOERROR;
}

ECode LockPatternUtils::IsEmergencyCallCapable(
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
    AutoPtr<IResources> rs;
    mContext->GetResources((IResources**)&rs);

    return rs->GetBoolean(R::bool_::config_voice_capable, result);
}

ECode LockPatternUtils::IsPukUnlockScreenEnable(
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
    AutoPtr<IResources> rs;
    mContext->GetResources((IResources**)&rs);

    return rs->GetBoolean(R::bool_::config_enable_puk_unlock_screen, result);
}

ECode LockPatternUtils::IsEmergencyCallEnabledWhileSimLocked(
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
    AutoPtr<IResources> rs;
    mContext->GetResources((IResources**)&rs);

    return rs->GetBoolean(R::bool_::config_enable_emergency_call_while_sim_locked, result);
}

ECode LockPatternUtils::GetNextAlarm(
    /* [out] */ IAlarmClockInfo** alarm)
{
    AlarmManager alarmManager = (AlarmManager) mContext.getSystemService(Context.ALARM_SERVICE);
    return alarmManager.getNextAlarmClock(UserHandle.USER_CURRENT);
}

Boolean LockPatternUtils::GetBoolean(
    /* [in] */ const String& secureSettingKey,
    /* [in] */ Boolean defaultValue,
    /* [in] */ Int32 userId)
{
    Boolean res = FALSE;
    ECode ec = GetLockSettings()->GetBoolean(secureSettingKey, defaultValue,
            userId, &res);
    if (FAILED(ec)) {
        return defaultValue;
    }
    return res;
}

Boolean LockPatternUtils::GetBoolean(
    /* [in] */ const String& secureSettingKey,
    /* [in] */ Boolean defaultValue)
{
    return GetBoolean(secureSettingKey, defaultValue, GetCurrentOrCallingUserId());
}

void LockPatternUtils::SetBoolean(
    /* [in] */ const String& secureSettingKey,
    /* [in] */ Boolean enabled,
    /* [in] */ Int32 userId)
{
    ECode ec= GetLockSettings()->SetBoolean(secureSettingKey, enabled, userId);
    if (FAILED(ec)) {
        Slogger::E(TAG, "Couldn't write Boolean");
    }
}

void LockPatternUtils::SetBoolean(
    /* [in] */ const String& secureSettingKey,
    /* [in] */ Boolean enabled)
{
    SetBoolean(secureSettingKey, enabled, GetCurrentOrCallingUserId());
}

ECode LockPatternUtils::GetAppWidgets(
    /* [out, callee] */ ArrayOf<Int32>** widgets)
{
    VALIDATE_NOT_NULL(widgets);
    AutoPtr<ArrayOf<Int32> > ws = GetAppWidgets(IUserHandle::USER_CURRENT);
    *widgets = ws;
    REFCOUNT_ADD(*widgets);
    return NOERROR;
}

AutoPtr<ArrayOf<Int32> > LockPatternUtils::GetAppWidgets(
    /* [in] */ Int32 userId)
{
    String appWidgetIdString;
    Settings::Secure::GetStringForUser(mContentResolver, ISettingsSecure::LOCK_SCREEN_APPWIDGET_IDS,
            userId, &appWidgetIdString);
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
        (*stringList)[i] = StringUtils::ToString((*list)[i]);
        strLength += (*stringList)[i].GetLength();
        if (i < listLength - 1) {
            strLength += separatorLength;
        }
    }

    StringBuilder sb(strLength);

    for (Int32 i = 0; i < listLength; i++) {
        sb.Append((*list)[i]);
        if (i < listLength - 1) {
            sb.Append(separator);
        }
    }
    String result;
    sb.ToString(&result);
    return result;
}

ECode LockPatternUtils::WriteFallbackAppWidgetId(
    /* [in] */ Int32 appWidgetId)
{
    Boolean result;
    return Settings::Secure::PutInt32ForUser(mContentResolver,
            ISettingsSecure::LOCK_SCREEN_FALLBACK_APPWIDGET_ID,
            appWidgetId, IUserHandle::USER_CURRENT, &result);
}

ECode LockPatternUtils::GetFallbackAppWidgetId(
    /* [out] */ Int32* id)
{
    VALIDATE_NOT_NULL(id);
    Settings::Secure::GetInt32ForUser(
        mContentResolver, ISettingsSecure::LOCK_SCREEN_FALLBACK_APPWIDGET_ID,
        IAppWidgetManager::INVALID_APPWIDGET_ID, IUserHandle::USER_CURRENT, id);
    return NOERROR;
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

ECode LockPatternUtils::AddAppWidget(
    /* [in] */ Int32 widgetId,
    /* [in] */ Int32 index,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
    *result = FALSE;
    AutoPtr<ArrayOf<Int32> > widgets = GetAppWidgets();
    if (widgets == NULL) {
        return NOERROR;
    }
    if (index < 0 || index > widgets->GetLength()) {
        return NOERROR;
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
    *result = TRUE;
    return NOERROR;
}

ECode LockPatternUtils::RemoveAppWidget(
    /* [in] */ Int32 widgetId,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
    *result = FALSE;
    AutoPtr<ArrayOf<Int32> > widgets = GetAppWidgets();

    if (widgets->GetLength() == 0) {
        return NOERROR;
    }

    AutoPtr<ArrayOf<Int32> > newWidgets = ArrayOf<Int32>::Alloc(widgets->GetLength() - 1);
    for (Int32 i = 0, j = 0; i < widgets->GetLength(); i++) {
        if ((*widgets)[i] == widgetId) {
            // continue...
        }
        else if (j >= newWidgets->GetLength()) {
            // we couldn't find the widget
            return NOERROR;
        }
        else {
            (*newWidgets)[j] = (*widgets)[i];
            j++;
        }
    }
    WriteAppWidgets(newWidgets);
    *result = TRUE;
    return NOERROR;
}

Int64 LockPatternUtils::GetLong(
    /* [in] */ const String& secureSettingKey,
    /* [in] */ Int64 defaultValue,
    /* [in] */ Int32 userHandle)
{
    try {
        return getLockSettings().GetInt64(secureSettingKey, defaultValue, userHandle);
    } catch (RemoteException re) {
        return defaultValue;
    }
}

Int64 LockPatternUtils::GetInt64(
    /* [in] */ const String& secureSettingKey,
    /* [in] */ Int64 defaultValue)
{
    Int64 result = FALSE;
    ECode ec = GetLockSettings()->GetInt64(secureSettingKey, defaultValue,
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
    ECode ec = GetLockSettings()->SetInt64(secureSettingKey, value, userHandle);

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
    String result;
    ECode ec = GetLockSettings()->GetString(secureSettingKey, String(""), userHandle, &result);
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
    ECode ec = GetLockSettings()->SetString(secureSettingKey, value, userHandle);
    if (FAILED(ec)) {
        Slogger::E(TAG, "Couldn't write string ");
    }
}

ECode LockPatternUtils::IsSecure(
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
    Int64 mode = GetKeyguardStoredPasswordQuality();
    Boolean isPattern = mode == IDevicePolicyManager::PASSWORD_QUALITY_SOMETHING;
    Boolean isPassword = mode == IDevicePolicyManager::PASSWORD_QUALITY_NUMERIC
            || mode == IDevicePolicyManager::PASSWORD_QUALITY_NUMERIC_COMPLEX
            || mode == IDevicePolicyManager::PASSWORD_QUALITY_ALPHABETIC
            || mode == IDevicePolicyManager::PASSWORD_QUALITY_ALPHANUMERIC
            || mode == IDevicePolicyManager::PASSWORD_QUALITY_COMPLEX;
    Boolean secure = isPattern && IsLockPatternEnabled() && SavedPatternExists()
            || isPassword && SavedPasswordExists();
    *result = secure;
    return NOERROR;
}

ECode LockPatternUtils::UpdateEmergencyCallButtonState(
    /* [in] */ IButton* button,
    /* [in] */ Boolean shown,
    /* [in] */ Boolean showIcon)
{
    if (IsEmergencyCallCapable() && shown) {
        button->SetVisibility(IView::VISIBLE);
    } else {
        button->SetVisibility(IView::GONE);
        return NOERROR;
    }

    Int32 textId = 0;
    if (isInCall()) {
        // show "return to call" text and show phone icon
        textId = R::string::lockscreen_return_to_call;
        Int32 phoneCallIcon = showIcon ? R::drawable::stat_sys_phone_call : 0;
        button->SetCompoundDrawablesWithIntrinsicBounds(phoneCallIcon, 0, 0, 0);
    }
    else {
        textId = R::string::lockscreen_emergency_call;
        Int32 emergencyIcon = showIcon ? R::drawable::ic_emergency : 0;
        button->SetCompoundDrawablesWithIntrinsicBounds(emergencyIcon, 0, 0, 0);
    }

    button->SetText(textId);
    return NOERROR;
}

ECode LockPatternUtils::ResumeCall()
{
    assert(0 && "TODO");
    // GetTelecommManager().showInCallScreen(FALSE);
    return NOERROR;
}

ECode LockPatternUtils::IsInCall(
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
    assert(0 && "TODO");
    // *result = GetTelecommManager().isInCall();
    return NOERROR;
}

// AutoPtr<ITelecomManager> LockPatternUtils::GetTelecommManager()
// {
//     return (TelecomManager) mContext.getSystemService(Context.TELECOM_SERVICE);
// }

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

ECode LockPatternUtils::GetPowerButtonInstantlyLocks(
    /* [out] */ Boolean* locks)
{
    VALIDATE_NOT_NULL(locks);
    *locks = GetBoolean(LOCKSCREEN_POWER_BUTTON_INSTANTLY_LOCKS, TRUE);
    return NOERROR;
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

ECode LockPatternUtils::HasWidgetsEnabledInKeyguard(
    /* [in] */ Int32 userid,
    /* [out] */ Boolean* has)
{
    VALIDATE_NOT_NULL(has);
    Int32 widgets[] = getAppWidgets(userid);
    for (Int32 i = 0; i < widgets.length; i++) {
        if (widgets[i] > 0) {
            return TRUE;
        }
    }
    return FALSE;
}

ECode LockPatternUtils::GetWidgetsEnabled(
    /* [out] */ Boolean* enabled)
{
    VALIDATE_NOT_NULL(enabled);
    return getWidgetsEnabled(getCurrentOrCallingUserId());
}

ECode LockPatternUtils::GetWidgetsEnabled(
    /* [in] */ Int32 userId,
    /* [out] */ Boolean* enabled)
{
    VALIDATE_NOT_NULL(enabled);
    *enabled = GetBoolean(LOCKSCREEN_WIDGETS_ENABLED, FALSE, userId);
    return NOERROR;
}

ECode LockPatternUtils::SetWidgetsEnabled(
    /* [in] */ Boolean enabled)
{
    SetWidgetsEnabled(enabled, getCurrentOrCallingUserId());
}

ECode LockPatternUtils::SetWidgetsEnabled(
    /* [in] */ Boolean enabled,
    /* [in] */ Int32 userId)
{
    SetBoolean(LOCKSCREEN_WIDGETS_ENABLED, enabled, userId);
    return NOERROR;
}

ECode LockPatternUtils::SetEnabledTrustAgents(
    /* [in] */ ICollection/*<ComponentName>*/* activeTrustAgents)
{
    SetEnabledTrustAgents(activeTrustAgents, getCurrentOrCallingUserId());
    return NOERROR;
}

ECode LockPatternUtils::GetEnabledTrustAgents(
    /* [out] */ IList** list)
{
    VALIDATE_NOT_NULL(list);
    return getEnabledTrustAgents(getCurrentOrCallingUserId());
}

ECode LockPatternUtils::SetEnabledTrustAgents(
    /* [in] */ ICollection/*<ComponentName>*/* activeTrustAgents,
    /* [in] */ Int32 userId)
{
    StringBuilder sb = new StringBuilder();
    for (ComponentName cn : activeTrustAgents) {
        if (sb.length() > 0) {
            sb.append(',');
        }
        sb.append(cn.flattenToShortString());
    }
    setString(ENABLED_TRUST_AGENTS, sb.toString(), userId);
    getTrustManager().reportEnabledTrustAgentsChanged(getCurrentOrCallingUserId());
}

ECode LockPatternUtils::GetEnabledTrustAgents(
    /* [in] */ Int32 userId,
    /* [out] */ IList** list)
{
    VALIDATE_NOT_NULL(list);
    String serialized = getString(ENABLED_TRUST_AGENTS, userId);
    if (TextUtils.isEmpty(serialized)) {
        return null;
    }
    String[] split = serialized.split(",");
    ArrayList<ComponentName> activeTrustAgents = new ArrayList<ComponentName>(split.length);
    for (String s : split) {
        if (!TextUtils.isEmpty(s)) {
            activeTrustAgents.add(ComponentName.unflattenFromString(s));
        }
    }
    return activeTrustAgents;
}

ECode LockPatternUtils::RequireCredentialEntry(
    /* [in] */ Int32 userId)
{
    getTrustManager().reportRequireCredentialEntry(userId);
}

void LockPatternUtils::OnAfterChangingPassword()
{
    getTrustManager().reportEnabledTrustAgentsChanged(getCurrentOrCallingUserId());
}

ECode LockPatternUtils::IsCredentialRequiredToDecrypt(
    /* [in] */ Boolean defaultValue,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
    final Int32 value = Settings.Global.getInt(mContentResolver,
            Settings.Global.REQUIRE_PASSWORD_TO_DECRYPT, -1);
    return value == -1 ? defaultValue : (value != 0);
}

ECode LockPatternUtils::SetCredentialRequiredToDecrypt(
    /* [in] */ Boolean required)
{
    if (getCurrentUser() != UserHandle.USER_OWNER) {
        Log.w(TAG, "Only device owner may call setCredentialRequiredForDecrypt()");
        return;
    }
    Settings.Global.putInt(mContext.getContentResolver(),
            Settings.Global.REQUIRE_PASSWORD_TO_DECRYPT, required ? 1 : 0);
}

}// namespace Widget
}// namespace Internal
}// namespace Droid
}// namespace Elastos
