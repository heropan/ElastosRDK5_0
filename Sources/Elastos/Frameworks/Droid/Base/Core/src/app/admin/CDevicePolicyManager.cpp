
#include "app/admin/CDevicePolicyManager.h"
#include "app/admin/CDeviceAdminInfo.h"
#include "os/CServiceManager.h"
#include "os/CUserHandleHelper.h"
#include "net/CProxy.h"
#include "content/pm/CResolveInfo.h"
#include <elastos/Logger.h>
#include <elastos/StringUtils.h>
#include <elastos/StringBuilder.h>

using Elastos::Net::ProxyType;
using Elastos::Net::IProxyHelper;
using Elastos::Net::CProxyHelper;
using Elastos::Net::ISocketAddress;
using Elastos::Net::IInetSocketAddress;
using Elastos::Core::StringUtils;
using Elastos::Core::StringBuilder;
using Elastos::Droid::Os::IServiceManager;
using Elastos::Droid::Os::CServiceManager;
using Elastos::Droid::Os::IUserHandleHelper;
using Elastos::Droid::Os::CUserHandleHelper;
using Elastos::Droid::App::Admin::CDeviceAdminInfo;
using Elastos::Droid::Content::Pm::IResolveInfo;
using Elastos::Droid::Content::Pm::CResolveInfo;
using Elastos::Droid::Content::Pm::IActivityInfo;
using Elastos::Droid::Content::Pm::IPackageManager;
using Elastos::Utility::Logging::Logger;

namespace Elastos {
namespace Droid {
namespace App {
namespace Admin {

const String CDevicePolicyManager::TAG = String("CDevicePolicyManager");

ECode CDevicePolicyManager::constructor(
    /* [in] */ IContext* context,
    /* [in] */ IHandler* handler)
{
    mContext = context;
    AutoPtr<IServiceManager> sm;
    CServiceManager::AcquireSingleton((IServiceManager**)&sm);
    AutoPtr<IInterface> service;
    sm->GetService(IContext::DEVICE_POLICY_SERVICE, (IInterface**)&service);
    mService = IIDevicePolicyManager::Probe(service);
    assert(mService != NULL);
    return NOERROR;
}

ECode CDevicePolicyManager::IsAdminActive(
    /* [in] */ IComponentName* who,
    /* [out] */ Boolean* isAdminActive)
{
    VALIDATE_NOT_NULL(isAdminActive);
    *isAdminActive = FALSE;
    if (mService != NULL) {
        Int32 myUserId;
        AutoPtr<IUserHandleHelper> helper;
        CUserHandleHelper::AcquireSingleton((IUserHandleHelper**)&helper);
        helper->GetMyUserId(&myUserId);
        ECode ec = mService->IsAdminActive(who, myUserId, isAdminActive);
        if (FAILED(ec)) {
            Logger::W(TAG, "E_REMOTE_EXCEPTION: Failed talking with device policy service");
        }
        return ec;
    }
    return NOERROR;
}

ECode CDevicePolicyManager::GetActiveAdmins(
    /* [out] */ IObjectContainer** admins)
{
    VALIDATE_NOT_NULL(admins);
    *admins = NULL;

    if (mService != NULL) {
        Int32 myUserId;
        AutoPtr<IUserHandleHelper> helper;
        CUserHandleHelper::AcquireSingleton((IUserHandleHelper**)&helper);
        helper->GetMyUserId(&myUserId);
        ECode ec = mService->GetActiveAdmins(myUserId, admins);
        if (FAILED(ec)) {
            Logger::W(TAG, "E_REMOTE_EXCEPTION: Failed talking with device policy service");
        }
        return ec;
    }

    return NOERROR;
}

ECode CDevicePolicyManager::PackageHasActiveAdmins(
    /* [in] */ const String& packageName,
    /* [out] */ Boolean* hasAdmins)
{
    VALIDATE_NOT_NULL(hasAdmins);
    *hasAdmins = FALSE;

    if (mService != NULL) {
        Int32 myUserId;
        AutoPtr<IUserHandleHelper> helper;
        CUserHandleHelper::AcquireSingleton((IUserHandleHelper**)&helper);
        helper->GetMyUserId(&myUserId);
        ECode ec = mService->PackageHasActiveAdmins(packageName, myUserId, hasAdmins);
        if (FAILED(ec)) {
            Logger::W(TAG, "E_REMOTE_EXCEPTION: Failed talking with device policy service");
        }
        return ec;
    }

    return NOERROR;
}

ECode CDevicePolicyManager::RemoveActiveAdmin(
    /* [in] */ IComponentName* who)
{
    if (mService != NULL) {
        Int32 myUserId;
        AutoPtr<IUserHandleHelper> helper;
        CUserHandleHelper::AcquireSingleton((IUserHandleHelper**)&helper);
        helper->GetMyUserId(&myUserId);
        ECode ec = mService->RemoveActiveAdmin(who, myUserId);
        if (FAILED(ec)) {
            Logger::W(TAG, "E_REMOTE_EXCEPTION: Failed talking with device policy service");
        }
        return ec;
    }

    return NOERROR;
}

ECode CDevicePolicyManager::HasGrantedPolicy(
    /* [in] */ IComponentName* admin,
    /* [in] */ Int32 usesPolicy,
    /* [out] */ Boolean* hasGrant)
{
    VALIDATE_NOT_NULL(hasGrant);
    *hasGrant = FALSE;

    if (mService != NULL) {
        Int32 myUserId;
        AutoPtr<IUserHandleHelper> helper;
        CUserHandleHelper::AcquireSingleton((IUserHandleHelper**)&helper);
        helper->GetMyUserId(&myUserId);
        ECode ec = mService->HasGrantedPolicy(admin, usesPolicy, myUserId, hasGrant);
        if (FAILED(ec)) {
            Logger::W(TAG, "E_REMOTE_EXCEPTION: Failed talking with device policy service");
        }
        return ec;
    }

    return NOERROR;
}

ECode CDevicePolicyManager::SetPasswordQuality(
    /* [in] */ IComponentName* admin,
    /* [in] */ Int32 quality)
{
    if (mService != NULL) {
        Int32 myUserId;
        AutoPtr<IUserHandleHelper> helper;
        CUserHandleHelper::AcquireSingleton((IUserHandleHelper**)&helper);
        helper->GetMyUserId(&myUserId);
        ECode ec = mService->SetPasswordQuality(admin, quality, myUserId);
        if (FAILED(ec)) {
            Logger::W(TAG, "E_REMOTE_EXCEPTION: Failed talking with device policy service");
        }
        return ec;
    }

    return NOERROR;
}

ECode CDevicePolicyManager::GetPasswordQuality(
    /* [in] */ IComponentName* admin,
    /* [out] */ Int32* password)
{
    VALIDATE_NOT_NULL(password);
    Int32 myUserId;
    AutoPtr<IUserHandleHelper> helper;
    CUserHandleHelper::AcquireSingleton((IUserHandleHelper**)&helper);
    helper->GetMyUserId(&myUserId);
    return GetPasswordQualityEx(admin, myUserId, password);
}

ECode CDevicePolicyManager::GetPasswordQualityEx(
    /* [in] */ IComponentName* admin,
    /* [in] */ Int32 userHandle,
    /* [out] */ Int32* password)
{
    VALIDATE_NOT_NULL(password);
    *password = IDevicePolicyManager::PASSWORD_QUALITY_UNSPECIFIED;

     if (mService != NULL) {
         ECode ec = mService->GetPasswordQuality(admin, userHandle, password);
        if (FAILED(ec)) {
            Logger::W(TAG, "E_REMOTE_EXCEPTION: Failed talking with device policy service");
        }
        return ec;
     }
     return NOERROR;
}

ECode CDevicePolicyManager::SetPasswordMinimumLength(
    /* [in] */ IComponentName* admin,
    /* [in] */ Int32 length)
{
    if (mService != NULL) {
        Int32 myUserId;
        AutoPtr<IUserHandleHelper> helper;
        CUserHandleHelper::AcquireSingleton((IUserHandleHelper**)&helper);
        helper->GetMyUserId(&myUserId);
        ECode ec = mService->SetPasswordMinimumLength(admin, length, myUserId);
        if (FAILED(ec)) {
            Logger::W(TAG, "E_REMOTE_EXCEPTION: Failed talking with device policy service");
        }
        return ec;
    }
    return NOERROR;
}

ECode CDevicePolicyManager::GetPasswordMinimumLength(
    /* [in] */ IComponentName* admin,
    /* [out] */ Int32* password)
{
    VALIDATE_NOT_NULL(password);
    Int32 myUserId;
    AutoPtr<IUserHandleHelper> helper;
    CUserHandleHelper::AcquireSingleton((IUserHandleHelper**)&helper);
    helper->GetMyUserId(&myUserId);
    return GetPasswordMinimumLengthEx(admin, myUserId, password);
}

ECode CDevicePolicyManager::GetPasswordMinimumLengthEx(
    /* [in] */ IComponentName* admin,
    /* [in] */ Int32 userHandle,
    /* [out] */ Int32* password)
{
    VALIDATE_NOT_NULL(password);
    *password = 0;

    if (mService != NULL) {
        ECode ec = mService->GetPasswordMinimumLength(admin, userHandle, password);
        if (FAILED(ec)) {
            Logger::W(TAG, "E_REMOTE_EXCEPTION: Failed talking with device policy service");
        }
        return ec;
    }
    return NOERROR;
}

ECode CDevicePolicyManager::SetPasswordMinimumUpperCase(
    /* [in] */ IComponentName* admin,
    /* [in] */ Int32 length)
{
    if (mService != NULL) {
        Int32 myUserId;
        AutoPtr<IUserHandleHelper> helper;
        CUserHandleHelper::AcquireSingleton((IUserHandleHelper**)&helper);
        helper->GetMyUserId(&myUserId);
        ECode ec = mService->SetPasswordMinimumUpperCase(admin, length, myUserId);
        if (FAILED(ec)) {
            Logger::W(TAG, "E_REMOTE_EXCEPTION: Failed talking with device policy service");
        }
        return ec;
    }
    return NOERROR;
}

ECode CDevicePolicyManager::GetPasswordMinimumUpperCase(
    /* [in] */ IComponentName* admin,
    /* [out] */ Int32* password)
{
    VALIDATE_NOT_NULL(password);
    Int32 myUserId;
    AutoPtr<IUserHandleHelper> helper;
    CUserHandleHelper::AcquireSingleton((IUserHandleHelper**)&helper);
    helper->GetMyUserId(&myUserId);
    return GetPasswordMinimumUpperCaseEx(admin, myUserId, password);
}

ECode CDevicePolicyManager::GetPasswordMinimumUpperCaseEx(
    /* [in] */ IComponentName* admin,
    /* [in] */ Int32 userHandle,
    /* [out] */ Int32* password)
{
    VALIDATE_NOT_NULL(password);
    *password = 0;

    if (mService != NULL) {
        ECode ec = mService->GetPasswordMinimumUpperCase(admin, userHandle, password);
        if (FAILED(ec)) {
            Logger::W(TAG, "E_REMOTE_EXCEPTION: Failed talking with device policy service");
        }
        return ec;
    }
    return NOERROR;
}

ECode CDevicePolicyManager::SetPasswordMinimumLowerCase(
    /* [in] */ IComponentName* admin,
    /* [in] */ Int32 length)
{
    if (mService != NULL) {
        Int32 myUserId;
        AutoPtr<IUserHandleHelper> helper;
        CUserHandleHelper::AcquireSingleton((IUserHandleHelper**)&helper);
        helper->GetMyUserId(&myUserId);
        ECode ec = mService->SetPasswordMinimumLowerCase(admin, length, myUserId);
        if (FAILED(ec)) {
            Logger::W(TAG, "E_REMOTE_EXCEPTION: Failed talking with device policy service");
        }
        return ec;
    }
    return NOERROR;
}

ECode CDevicePolicyManager::GetPasswordMinimumLowerCase(
    /* [in] */ IComponentName* admin,
    /* [out] */ Int32* password)
{
    Int32 myUserId;
    AutoPtr<IUserHandleHelper> helper;
    CUserHandleHelper::AcquireSingleton((IUserHandleHelper**)&helper);
    helper->GetMyUserId(&myUserId);
    return GetPasswordMinimumLowerCaseEx(admin, myUserId, password);
}

ECode CDevicePolicyManager::GetPasswordMinimumLowerCaseEx(
    /* [in] */ IComponentName* admin,
    /* [in] */ Int32 userHandle,
    /* [out] */ Int32* password)
{
    VALIDATE_NOT_NULL(password);
    *password = 0;

    if (mService != NULL) {
        ECode ec = mService->GetPasswordMinimumLowerCase(admin, userHandle, password);
        if (FAILED(ec)) {
            Logger::W(TAG, "E_REMOTE_EXCEPTION: Failed talking with device policy service");
        }
        return ec;
    }
    return NOERROR;
}

ECode CDevicePolicyManager::SetPasswordMinimumLetters(
    /* [in] */ IComponentName* admin,
    /* [in] */ Int32 length)
{
    if (mService != NULL) {
        Int32 myUserId;
        AutoPtr<IUserHandleHelper> helper;
        CUserHandleHelper::AcquireSingleton((IUserHandleHelper**)&helper);
        helper->GetMyUserId(&myUserId);
        ECode ec = mService->SetPasswordMinimumLetters(admin, length, myUserId);
        if (FAILED(ec)) {
            Logger::W(TAG, "E_REMOTE_EXCEPTION: Failed talking with device policy service");
        }
        return ec;
    }
    return NOERROR;
}

ECode CDevicePolicyManager::GetPasswordMinimumLetters(
    /* [in] */ IComponentName* admin,
    /* [out] */ Int32* password)
{
    VALIDATE_NOT_NULL(password);
    Int32 myUserId;
    AutoPtr<IUserHandleHelper> helper;
    CUserHandleHelper::AcquireSingleton((IUserHandleHelper**)&helper);
    helper->GetMyUserId(&myUserId);
    return GetPasswordMinimumLettersEx(admin, myUserId, password);
}

ECode CDevicePolicyManager::GetPasswordMinimumLettersEx(
    /* [in] */ IComponentName* admin,
    /* [in] */ Int32 userHandle,
    /* [out] */ Int32* password)
{
    VALIDATE_NOT_NULL(password);
    *password = 0;

    if (mService != NULL) {
        ECode ec = mService->GetPasswordMinimumLetters(admin, userHandle, password);
        if (FAILED(ec)) {
            Logger::W(TAG, "E_REMOTE_EXCEPTION: Failed talking with device policy service");
        }
        return ec;
    }
    return NOERROR;
}

ECode CDevicePolicyManager::SetPasswordMinimumNumeric(
    /* [in] */ IComponentName* admin,
    /* [in] */ Int32 length)
{
    if (mService != NULL) {
        Int32 myUserId;
        AutoPtr<IUserHandleHelper> helper;
        CUserHandleHelper::AcquireSingleton((IUserHandleHelper**)&helper);
        helper->GetMyUserId(&myUserId);
        ECode ec = mService->SetPasswordMinimumNumeric(admin, length, myUserId);
        if (FAILED(ec)) {
            Logger::W(TAG, "E_REMOTE_EXCEPTION: Failed talking with device policy service");
        }
        return ec;
    }
    return NOERROR;
}

ECode CDevicePolicyManager::GetPasswordMinimumNumeric(
    /* [in] */ IComponentName* admin,
    /* [out] */ Int32* password)
{
    VALIDATE_NOT_NULL(password);
    Int32 myUserId;
    AutoPtr<IUserHandleHelper> helper;
    CUserHandleHelper::AcquireSingleton((IUserHandleHelper**)&helper);
    helper->GetMyUserId(&myUserId);
    return GetPasswordMinimumNumericEx(admin, myUserId, password);
}

ECode CDevicePolicyManager::GetPasswordMinimumNumericEx(
    /* [in] */ IComponentName* admin,
    /* [in] */ Int32 userHandle,
    /* [out] */ Int32* password)
{
    VALIDATE_NOT_NULL(password);
    *password = 0;

    if (mService != NULL) {
        ECode ec = mService->GetPasswordMinimumNumeric(admin, userHandle, password);
        if (FAILED(ec)) {
            Logger::W(TAG, "E_REMOTE_EXCEPTION: Failed talking with device policy service");
        }
        return ec;
    }
    return NOERROR;
}

ECode CDevicePolicyManager::SetPasswordMinimumSymbols(
    /* [in] */ IComponentName* admin,
    /* [in] */ Int32 length)
{
    if (mService != NULL) {
        Int32 myUserId;
        AutoPtr<IUserHandleHelper> helper;
        CUserHandleHelper::AcquireSingleton((IUserHandleHelper**)&helper);
        helper->GetMyUserId(&myUserId);
        ECode ec = mService->SetPasswordMinimumSymbols(admin, length, myUserId);
        if (FAILED(ec)) {
            Logger::W(TAG, "E_REMOTE_EXCEPTION: Failed talking with device policy service");
        }
        return ec;
    }
    return NOERROR;
}

ECode CDevicePolicyManager::GetPasswordMinimumSymbols(
    /* [in] */ IComponentName* admin,
    /* [out] */ Int32* password)
{
    VALIDATE_NOT_NULL(password);
    Int32 myUserId;
    AutoPtr<IUserHandleHelper> helper;
    CUserHandleHelper::AcquireSingleton((IUserHandleHelper**)&helper);
    helper->GetMyUserId(&myUserId);
    return GetPasswordMinimumSymbolsEx(admin, myUserId, password);
}

ECode CDevicePolicyManager::GetPasswordMinimumSymbolsEx(
    /* [in] */ IComponentName* admin,
    /* [in] */ Int32 userHandle,
    /* [out] */ Int32* password)
{
    VALIDATE_NOT_NULL(password);
    *password = 0;

    if (mService != NULL) {
        ECode ec = mService->GetPasswordMinimumSymbols(admin, userHandle, password);
        if (FAILED(ec)) {
            Logger::W(TAG, "E_REMOTE_EXCEPTION: Failed talking with device policy service");
        }
        return ec;
    }
    return NOERROR;
}

ECode CDevicePolicyManager::SetPasswordMinimumNonLetter(
    /* [in] */ IComponentName* admin,
    /* [in] */ Int32 length)
{
    if (mService != NULL) {
        Int32 myUserId;
        AutoPtr<IUserHandleHelper> helper;
        CUserHandleHelper::AcquireSingleton((IUserHandleHelper**)&helper);
        helper->GetMyUserId(&myUserId);
        ECode ec = mService->SetPasswordMinimumNonLetter(admin, length, myUserId);
        if (FAILED(ec)) {
            Logger::W(TAG, "E_REMOTE_EXCEPTION: Failed talking with device policy service");
        }
        return ec;
    }
    return NOERROR;
}

ECode CDevicePolicyManager::GetPasswordMinimumNonLetter(
    /* [in] */ IComponentName* admin,
    /* [out] */ Int32* password)
{
    VALIDATE_NOT_NULL(password);
    Int32 myUserId;
    AutoPtr<IUserHandleHelper> helper;
    CUserHandleHelper::AcquireSingleton((IUserHandleHelper**)&helper);
    helper->GetMyUserId(&myUserId);
    return GetPasswordMinimumNonLetterEx(admin, myUserId, password);
}

ECode CDevicePolicyManager::GetPasswordMinimumNonLetterEx(
    /* [in] */ IComponentName* admin,
    /* [in] */ Int32 userHandle,
    /* [out] */ Int32* password)
{
    VALIDATE_NOT_NULL(password);
    *password = 0;

    if (mService != NULL) {
        ECode ec = mService->GetPasswordMinimumNonLetter(admin, userHandle, password);
        if (FAILED(ec)) {
            Logger::W(TAG, "E_REMOTE_EXCEPTION: Failed talking with device policy service");
        }
        return ec;
    }
    return NOERROR;
}

ECode CDevicePolicyManager::SetPasswordHistoryLength(
    /* [in] */ IComponentName* admin,
    /* [in] */ Int32 length)
{
    if (mService != NULL) {
        Int32 myUserId;
        AutoPtr<IUserHandleHelper> helper;
        CUserHandleHelper::AcquireSingleton((IUserHandleHelper**)&helper);
        helper->GetMyUserId(&myUserId);
        ECode ec = mService->SetPasswordHistoryLength(admin, length, myUserId);
        if (FAILED(ec)) {
            Logger::W(TAG, "E_REMOTE_EXCEPTION: Failed talking with device policy service");
        }
        return ec;
    }
    return NOERROR;
}

ECode CDevicePolicyManager::SetPasswordExpirationTimeout(
    /* [in] */ IComponentName* admin,
    /* [in] */ Int64 timeout)
{
    if (mService != NULL) {
        Int32 myUserId;
        AutoPtr<IUserHandleHelper> helper;
        CUserHandleHelper::AcquireSingleton((IUserHandleHelper**)&helper);
        helper->GetMyUserId(&myUserId);
        ECode ec = mService->SetPasswordExpirationTimeout(admin, timeout, myUserId);
        if (FAILED(ec)) {
            Logger::W(TAG, "E_REMOTE_EXCEPTION: Failed talking with device policy service");
        }
        return ec;
    }
    return NOERROR;
}

ECode CDevicePolicyManager::GetPasswordExpirationTimeout(
    /* [in] */ IComponentName* admin,
    /* [out] */ Int64* timeout)
{
    VALIDATE_NOT_NULL(timeout);
    *timeout = 0;

    if (mService != NULL) {
        Int32 myUserId;
        AutoPtr<IUserHandleHelper> helper;
        CUserHandleHelper::AcquireSingleton((IUserHandleHelper**)&helper);
        helper->GetMyUserId(&myUserId);
        ECode ec = mService->GetPasswordExpirationTimeout(admin, myUserId, timeout);
        if (FAILED(ec)) {
            Logger::W(TAG, "E_REMOTE_EXCEPTION: Failed talking with device policy service");
        }
        return ec;
    }
    return NOERROR;
}

ECode CDevicePolicyManager::GetPasswordExpiration(
    /* [in] */ IComponentName* admin,
    /* [out] */ Int64* expiration)
{
    VALIDATE_NOT_NULL(expiration);
    *expiration = 0;

    if (mService != NULL) {
        Int32 myUserId;
        AutoPtr<IUserHandleHelper> helper;
        CUserHandleHelper::AcquireSingleton((IUserHandleHelper**)&helper);
        helper->GetMyUserId(&myUserId);
        ECode ec = mService->GetPasswordExpiration(admin, myUserId, expiration);
        if (FAILED(ec)) {
            Logger::W(TAG, "E_REMOTE_EXCEPTION: Failed talking with device policy service");
        }
        return ec;
    }
    return NOERROR;
}

ECode CDevicePolicyManager::GetPasswordHistoryLength(
    /* [in] */ IComponentName* admin,
    /* [out] */ Int32* length)
{
    VALIDATE_NOT_NULL(length);
    Int32 myUserId;
    AutoPtr<IUserHandleHelper> helper;
    CUserHandleHelper::AcquireSingleton((IUserHandleHelper**)&helper);
    helper->GetMyUserId(&myUserId);
    return GetPasswordHistoryLengthEx(admin, myUserId, length);
}

ECode CDevicePolicyManager::GetPasswordHistoryLengthEx(
    /* [in] */ IComponentName* admin,
    /* [in] */ Int32 userHandle,
    /* [out] */ Int32* length)
{
    VALIDATE_NOT_NULL(length);
    *length = 0;

    if (mService != NULL) {
        ECode ec = mService->GetPasswordHistoryLength(admin, userHandle, length);
        if (FAILED(ec)) {
            Logger::W(TAG, "E_REMOTE_EXCEPTION: Failed talking with device policy service");
        }
        return ec;
    }
    return NOERROR;
}

ECode CDevicePolicyManager::GetPasswordMaximumLength(
    /* [in] */ Int32 quality,
    /* [out] */ Int32* length)
{
    VALIDATE_NOT_NULL(length);
    // Kind-of arbitrary.
    *length = 16;
    return NOERROR;
}

ECode CDevicePolicyManager::IsActivePasswordSufficient(
    /* [out] */ Boolean* isSufficient)
{
    VALIDATE_NOT_NULL(isSufficient);
    *isSufficient = FALSE;

    if (mService != NULL) {
        Int32 myUserId;
        AutoPtr<IUserHandleHelper> helper;
        CUserHandleHelper::AcquireSingleton((IUserHandleHelper**)&helper);
        helper->GetMyUserId(&myUserId);
        ECode ec = mService->IsActivePasswordSufficient(myUserId, isSufficient);
        if (FAILED(ec)) {
            Logger::W(TAG, "E_REMOTE_EXCEPTION: Failed talking with device policy service");
        }
        return ec;
    }
    return NOERROR;
}

ECode CDevicePolicyManager::GetCurrentFailedPasswordAttempts(
    /* [out] */ Int32* atempts)
{
    VALIDATE_NOT_NULL(atempts);
    *atempts = -1;

    if (mService != NULL) {
        Int32 myUserId;
        AutoPtr<IUserHandleHelper> helper;
        CUserHandleHelper::AcquireSingleton((IUserHandleHelper**)&helper);
        helper->GetMyUserId(&myUserId);
        ECode ec = mService->GetCurrentFailedPasswordAttempts(myUserId, atempts);
        if (FAILED(ec)) {
            Logger::W(TAG, "E_REMOTE_EXCEPTION: Failed talking with device policy service");
        }
        return ec;
    }
    return NOERROR;
}

ECode CDevicePolicyManager::SetMaximumFailedPasswordsForWipe(
    /* [in] */ IComponentName* admin,
    /* [in] */ Int32 num)
{
    if (mService != NULL) {
        Int32 myUserId;
        AutoPtr<IUserHandleHelper> helper;
        CUserHandleHelper::AcquireSingleton((IUserHandleHelper**)&helper);
        helper->GetMyUserId(&myUserId);
        ECode ec = mService->SetMaximumFailedPasswordsForWipe(admin, num, myUserId);
        if (FAILED(ec)) {
            Logger::W(TAG, "E_REMOTE_EXCEPTION: Failed talking with device policy service");
        }
        return ec;
    }
    return NOERROR;
}

ECode CDevicePolicyManager::GetMaximumFailedPasswordsForWipe(
    /* [in] */ IComponentName* admin,
    /* [out] */ Int32* maximum)
{
    VALIDATE_NOT_NULL(maximum);
    Int32 myUserId;
    AutoPtr<IUserHandleHelper> helper;
    CUserHandleHelper::AcquireSingleton((IUserHandleHelper**)&helper);
    helper->GetMyUserId(&myUserId);
    return GetMaximumFailedPasswordsForWipeEx(admin, myUserId, maximum);
}

ECode CDevicePolicyManager::GetMaximumFailedPasswordsForWipeEx(
    /* [in] */ IComponentName* admin,
    /* [in] */ Int32 userHandle,
    /* [out] */ Int32* maximum)
{
    VALIDATE_NOT_NULL(maximum);
    *maximum = 0;

    if (mService != NULL) {
        Int32 myUserId;
        AutoPtr<IUserHandleHelper> helper;
        CUserHandleHelper::AcquireSingleton((IUserHandleHelper**)&helper);
        helper->GetMyUserId(&myUserId);
        ECode ec = mService->GetMaximumFailedPasswordsForWipe(admin, myUserId, maximum);
        if (FAILED(ec)) {
            Logger::W(TAG, "E_REMOTE_EXCEPTION: Failed talking with device policy service");
        }
        return ec;
    }
    return NOERROR;
}

ECode CDevicePolicyManager::ResetPassword(
    /* [in] */ const String& password,
    /* [in] */ Int32 flags,
    /* [out] */ Boolean* reset)
{
    VALIDATE_NOT_NULL(reset);
    *reset = FALSE;

    if (mService != NULL) {
        Int32 myUserId;
        AutoPtr<IUserHandleHelper> helper;
        CUserHandleHelper::AcquireSingleton((IUserHandleHelper**)&helper);
        helper->GetMyUserId(&myUserId);
        ECode ec = mService->ResetPassword(password, flags, myUserId, reset);
        if (FAILED(ec)) {
            Logger::W(TAG, "E_REMOTE_EXCEPTION: Failed talking with device policy service");
        }
        return ec;
    }
    return NOERROR;
}

ECode CDevicePolicyManager::SetMaximumTimeToLock(
    /* [in] */ IComponentName* admin,
    /* [in] */ Int64 timeMs)
{
    if (mService != NULL) {
        Int32 myUserId;
        AutoPtr<IUserHandleHelper> helper;
        CUserHandleHelper::AcquireSingleton((IUserHandleHelper**)&helper);
        helper->GetMyUserId(&myUserId);
        ECode ec = mService->SetMaximumTimeToLock(admin, timeMs, myUserId);
        if (FAILED(ec)) {
            Logger::W(TAG, "E_REMOTE_EXCEPTION: Failed talking with device policy service");
        }
        return ec;
    }
    return NOERROR;
}

ECode CDevicePolicyManager::GetMaximumTimeToLock(
    /* [in] */ IComponentName* admin,
    /* [out] */ Int64* time)
{
    VALIDATE_NOT_NULL(time);
    Int32 myUserId;
    AutoPtr<IUserHandleHelper> helper;
    CUserHandleHelper::AcquireSingleton((IUserHandleHelper**)&helper);
    helper->GetMyUserId(&myUserId);
    return GetMaximumTimeToLockEx(admin, myUserId, time);
}

ECode CDevicePolicyManager::GetMaximumTimeToLockEx(
    /* [in] */ IComponentName* admin,
    /* [in] */ Int32 userHandle,
    /* [out] */ Int64* time)
{
    VALIDATE_NOT_NULL(time);
    *time = 0;

    if (mService != NULL) {
        ECode ec = mService->GetMaximumTimeToLock(admin, userHandle, time);
        if (FAILED(ec)) {
            Logger::W(TAG, "E_REMOTE_EXCEPTION: Failed talking with device policy service");
        }
        return ec;
    }
    return NOERROR;
}

ECode CDevicePolicyManager::LockNow()
{
    if (mService != NULL) {
        ECode ec = mService->LockNow();
        if (FAILED(ec)) {
            Logger::W(TAG, "E_REMOTE_EXCEPTION: Failed talking with device policy service");
        }
        return ec;
    }
    return NOERROR;
}

ECode CDevicePolicyManager::WipeData(
    /* [in] */ Int32 flags)
{
    if (mService != NULL) {
        Int32 myUserId;
        AutoPtr<IUserHandleHelper> helper;
        CUserHandleHelper::AcquireSingleton((IUserHandleHelper**)&helper);
        helper->GetMyUserId(&myUserId);
        ECode ec = mService->WipeData(flags, myUserId);
        if (FAILED(ec)) {
            Logger::W(TAG, "E_REMOTE_EXCEPTION: Failed talking with device policy service");
        }
        return ec;
    }
    return NOERROR;
}

ECode CDevicePolicyManager::SetGlobalProxy(
    /* [in] */ IComponentName* admin,
    /* [in] */ Elastos::Net::IProxy* proxySpec,
    /* [in] */ ArrayOf<String>* exclusionList,
    /* [out] */ IComponentName** component)
{
    VALIDATE_NOT_NULL(admin);
    VALIDATE_NOT_NULL(exclusionList);
    VALIDATE_NOT_NULL(component);
    *component = NULL;

    if (proxySpec == NULL) {
        return E_NULL_POINTER_EXCEPTION;
    }

    if (mService != NULL) {
        AutoPtr<IProxyHelper> helper;
        CProxyHelper::AcquireSingleton((IProxyHelper**)&helper);
        AutoPtr<Elastos::Net::IProxy> noPorxy;
        helper->GetNO_PROXY((Elastos::Net::IProxy**)&noPorxy);

        String hostSpec;
        String exclSpec;
        Boolean equals = FALSE;
        IObject::Probe(proxySpec)->Equals(noPorxy, &equals);
        if (equals) {
            hostSpec = NULL;
            exclSpec = NULL;
        }
        else {
            ProxyType specType;
            proxySpec->GetType(&specType);
            if (specType == Elastos::Net::ProxyType_HTTP) {
                return E_ILLEGAL_ARGUMENT_EXCEPTION;
            }

            AutoPtr<IInetSocketAddress> sa;
            proxySpec->GetAddress((ISocketAddress**)&sa);

            String hostName;
            Int32 port;
            sa->GetHostName(&hostName);
            sa->GetPort(&port);

            StringBuilder sb(hostName);
            sb += ":";
            sb += StringUtils::Int32ToString(port);
            hostSpec = sb.ToString();
            if (exclusionList == NULL) {
                exclSpec = "";
            }
            else {
                sb.Reset();
                Boolean firstDomain = TRUE;

                Int32 size = exclusionList->GetLength();
                for (Int32 i = 0; i < size; ++i) {
                    String exclDomain = (*exclusionList)[i];
                    if (!firstDomain) {
                        sb += ",";
                    }
                    else {
                        firstDomain = FALSE;
                    }
                    sb += exclDomain.Trim();
                }
                exclSpec = sb.ToString();
            }

            AutoPtr<Elastos::Droid::Net::IProxy> netProxy;
            Elastos::Droid::Net::CProxy::AcquireSingleton((Elastos::Droid::Net::IProxy**)&netProxy);
            netProxy->Validate(hostName, StringUtils::Int32ToString(port), exclSpec);
        }

        Int32 myUserId;
        AutoPtr<IUserHandleHelper> uhelper;
        CUserHandleHelper::AcquireSingleton((IUserHandleHelper**)&uhelper);
        uhelper->GetMyUserId(&myUserId);
        AutoPtr<IComponentName> comp;
        ECode ec = mService->SetGlobalProxy(admin, hostSpec, exclSpec, myUserId, (IComponentName**)&comp);
        if (FAILED(ec)) {
            Logger::W(TAG, "E_REMOTE_EXCEPTION: Failed talking with device policy service");
        }
        return ec;
    }
    return NOERROR;
}

ECode CDevicePolicyManager::GetGlobalProxyAdmin(
    /* [out] */ IComponentName** global)
{
    VALIDATE_NOT_NULL(global);
    *global = NULL;

    if (mService != NULL) {
        Int32 myUserId;
        AutoPtr<IUserHandleHelper> helper;
        CUserHandleHelper::AcquireSingleton((IUserHandleHelper**)&helper);
        helper->GetMyUserId(&myUserId);
        ECode ec = mService->GetGlobalProxyAdmin(myUserId, global);
        if (FAILED(ec)) {
            Logger::W(TAG, "E_REMOTE_EXCEPTION: Failed talking with device policy service");
        }
        return ec;
    }
    return NOERROR;
}

ECode CDevicePolicyManager::SetStorageEncryption(
    /* [in] */ IComponentName* admin,
    /* [in] */ Boolean encrypt,
    /* [out] */ Int32* result)
{
    VALIDATE_NOT_NULL(result);
    *result = IDevicePolicyManager::ENCRYPTION_STATUS_UNSUPPORTED;

    if (mService != NULL) {
        Int32 myUserId;
        AutoPtr<IUserHandleHelper> helper;
        CUserHandleHelper::AcquireSingleton((IUserHandleHelper**)&helper);
        helper->GetMyUserId(&myUserId);
        ECode ec = mService->SetStorageEncryption(admin, encrypt, myUserId, result);
        if (FAILED(ec)) {
            Logger::W(TAG, "E_REMOTE_EXCEPTION: Failed talking with device policy service");
        }
        return ec;
    }
    return NOERROR;
}

ECode CDevicePolicyManager::GetStorageEncryption(
    /* [in] */ IComponentName* admin,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
    *result = FALSE;

    if (mService != NULL) {
        Int32 myUserId;
        AutoPtr<IUserHandleHelper> helper;
        CUserHandleHelper::AcquireSingleton((IUserHandleHelper**)&helper);
        helper->GetMyUserId(&myUserId);
        ECode ec = mService->GetStorageEncryption(admin, myUserId, result);
        if (FAILED(ec)) {
            Logger::W(TAG, "E_REMOTE_EXCEPTION: Failed talking with device policy service");
        }
        return ec;
    }
    return NOERROR;
}

ECode CDevicePolicyManager::GetStorageEncryptionStatus(
    /* [out] */ Int32* st)
{
    VALIDATE_NOT_NULL(st);
    Int32 myUserId;
    AutoPtr<IUserHandleHelper> helper;
    CUserHandleHelper::AcquireSingleton((IUserHandleHelper**)&helper);
    helper->GetMyUserId(&myUserId);
    return GetStorageEncryptionStatusEx(myUserId, st);
}

ECode CDevicePolicyManager::GetStorageEncryptionStatusEx(
    /* [in] */ Int32 userHandle,
    /* [out] */ Int32* st)
{
    VALIDATE_NOT_NULL(st);
    *st = IDevicePolicyManager::ENCRYPTION_STATUS_UNSUPPORTED;

    if (mService != NULL) {
        ECode ec = mService->GetStorageEncryptionStatus(userHandle, st);
        if (FAILED(ec)) {
            Logger::W(TAG, "E_REMOTE_EXCEPTION: Failed talking with device policy service");
        }
        return ec;
    }
    return NOERROR;
}

ECode CDevicePolicyManager::SetCameraDisabled(
    /* [in] */ IComponentName* admin,
    /* [in] */ Boolean disabled)
{
    if (mService != NULL) {
        Int32 myUserId;
        AutoPtr<IUserHandleHelper> helper;
        CUserHandleHelper::AcquireSingleton((IUserHandleHelper**)&helper);
        helper->GetMyUserId(&myUserId);
        ECode ec = mService->SetCameraDisabled(admin, disabled, myUserId);
        if (FAILED(ec)) {
            Logger::W(TAG, "E_REMOTE_EXCEPTION: Failed talking with device policy service");
        }
        return ec;
    }
    return NOERROR;
}

ECode CDevicePolicyManager::GetCameraDisabled(
    /* [in] */ IComponentName* admin,
    /* [out] */ Boolean* disabled)
{
    VALIDATE_NOT_NULL(disabled);
    Int32 myUserId;
    AutoPtr<IUserHandleHelper> helper;
    CUserHandleHelper::AcquireSingleton((IUserHandleHelper**)&helper);
    helper->GetMyUserId(&myUserId);
    return GetCameraDisabledEx(admin, myUserId, disabled);
}

ECode CDevicePolicyManager::GetCameraDisabledEx(
    /* [in] */ IComponentName* admin,
    /* [in] */ Int32 userHandle,
    /* [out] */ Boolean* disabled)
{
    VALIDATE_NOT_NULL(disabled);
    *disabled = FALSE;

    if (mService != NULL) {
        ECode ec = mService->GetCameraDisabled(admin, userHandle, disabled);
        if (FAILED(ec)) {
            Logger::W(TAG, "E_REMOTE_EXCEPTION: Failed talking with device policy service");
        }
        return ec;
    }
    return NOERROR;
}

ECode CDevicePolicyManager::SetKeyguardDisabledFeatures(
    /* [in] */ IComponentName* admin,
    /* [in] */ Int32 which)
{
    if (mService != NULL) {
        Int32 myUserId;
        AutoPtr<IUserHandleHelper> helper;
        CUserHandleHelper::AcquireSingleton((IUserHandleHelper**)&helper);
        helper->GetMyUserId(&myUserId);
        ECode ec = mService->SetKeyguardDisabledFeatures(admin, which, myUserId);
        if (FAILED(ec)) {
            Logger::W(TAG, "E_REMOTE_EXCEPTION: Failed talking with device policy service");
        }
        return ec;
    }
    return NOERROR;
}

ECode CDevicePolicyManager::GetKeyguardDisabledFeatures(
    /* [in] */ IComponentName* admin,
    /* [out] */ Int32* which)
{
    VALIDATE_NOT_NULL(which);
    Int32 myUserId;
    AutoPtr<IUserHandleHelper> helper;
    CUserHandleHelper::AcquireSingleton((IUserHandleHelper**)&helper);
    helper->GetMyUserId(&myUserId);
    return GetKeyguardDisabledFeaturesEx(admin, myUserId, which);
}

ECode CDevicePolicyManager::GetKeyguardDisabledFeaturesEx(
    /* [in] */ IComponentName* admin,
    /* [in] */ Int32 userHandle,
    /* [out] */ Int32* which)
{
    VALIDATE_NOT_NULL(which);
    *which = IDevicePolicyManager::KEYGUARD_DISABLE_FEATURES_NONE;

    if (mService != NULL) {
        ECode ec = mService->GetKeyguardDisabledFeatures(admin, userHandle, which);
        if (FAILED(ec)) {
            Logger::W(TAG, "E_REMOTE_EXCEPTION: Failed talking with device policy service");
        }
        return ec;
    }
    return NOERROR;
}

ECode CDevicePolicyManager::SetActiveAdmin(
    /* [in] */ IComponentName* policyReceiver,
    /* [in] */ Boolean refreshing)
{
    if (mService != NULL) {
        Int32 myUserId;
        AutoPtr<IUserHandleHelper> helper;
        CUserHandleHelper::AcquireSingleton((IUserHandleHelper**)&helper);
        helper->GetMyUserId(&myUserId);
        ECode ec = mService->SetActiveAdmin(policyReceiver, refreshing, myUserId);
        if (FAILED(ec)) {
            Logger::W(TAG, "E_REMOTE_EXCEPTION: Failed talking with device policy service");
        }
        return ec;
    }
    return NOERROR;
}

ECode CDevicePolicyManager::GetAdminInfo(
    /* [in] */ IComponentName* cn,
    /* [out] */ IDeviceAdminInfo** info)
{
    VALIDATE_NOT_NULL(info);
    *info = NULL;

    String cnStr;
    cn->ToString(&cnStr);

    AutoPtr<IPackageManager> pm;
    FAIL_RETURN(mContext->GetPackageManager((IPackageManager**)&pm));
    AutoPtr<IActivityInfo> ai;
    ECode ec = pm->GetReceiverInfo(cn, IPackageManager::GET_META_DATA, (IActivityInfo**)&ai);
    if (FAILED(ec)) {
        // catch (PackageManager.NameNotFoundException e)
        String msg("PackageManager.NameNotFoundException: Unable to retrieve device policy ");
        msg += cnStr;
        Logger::W(TAG, msg);
        return ec;
    }

    AutoPtr<IResolveInfo> ri;
    CResolveInfo::New((IResolveInfo**)&ri);
    ri->SetActivityInfo(ai.Get());

    ec = CDeviceAdminInfo::New(mContext, ri, info);
    if (ec == E_XML_PULL_PARSER_EXCEPTION) {
        String warn("XmlPullParserException: Unable to parse device policy");
        warn += cnStr;
        Logger::W(TAG, warn);

        *info = NULL;
        return ec;
    }
    else if (ec == E_IO_EXCEPTION) {
        String warn("IOException: Unable to parse device policy");
        warn += cnStr;
        Logger::W(TAG, warn);

        *info = NULL;
        return ec;
    }

    return ec;
}

ECode CDevicePolicyManager::GetRemoveWarning(
    /* [in] */ IComponentName* admin,
    /* [in] */ IRemoteCallback* result)
{
    if (mService != NULL) {
        Int32 myUserId;
        AutoPtr<IUserHandleHelper> helper;
        CUserHandleHelper::AcquireSingleton((IUserHandleHelper**)&helper);
        helper->GetMyUserId(&myUserId);
        ECode ec = mService->GetRemoveWarning(admin, result, myUserId);
        if (FAILED(ec)) {
            Logger::W(TAG, "E_REMOTE_EXCEPTION: Failed talking with device policy service");
        }
        return ec;
    }
    return NOERROR;
}

ECode CDevicePolicyManager::SetActivePasswordState(
    /* [in] */ Int32 quality,
    /* [in] */ Int32 length,
    /* [in] */ Int32 letters,
    /* [in] */ Int32 uppercase,
    /* [in] */ Int32 lowercase,
    /* [in] */ Int32 numbers,
    /* [in] */ Int32 symbols,
    /* [in] */ Int32 nonletter,
    /* [in] */ Int32 userHandle)
{
    if (mService != NULL) {
        ECode ec = mService->SetActivePasswordState(
                quality, length, letters, uppercase, lowercase,
                numbers, symbols, nonletter, userHandle);
        if (FAILED(ec)) {
            Logger::W(TAG, "E_REMOTE_EXCEPTION: Failed talking with device policy service");
        }
        return ec;
    }
    return NOERROR;
}

ECode CDevicePolicyManager::ReportFailedPasswordAttempt(
    /* [in] */ Int32 userHandle)
{
    if (mService != NULL) {
        ECode ec = mService->ReportFailedPasswordAttempt(userHandle);
        if (FAILED(ec)) {
            Logger::W(TAG, "E_REMOTE_EXCEPTION: Failed talking with device policy service");
        }
        return ec;
    }
    return NOERROR;
}

ECode CDevicePolicyManager::ReportSuccessfulPasswordAttempt(
    /* [in] */ Int32 userHandle)
{
    if (mService != NULL) {
        ECode ec = mService->ReportSuccessfulPasswordAttempt(userHandle);
        if (FAILED(ec)) {
            Logger::W(TAG, "E_REMOTE_EXCEPTION: Failed talking with device policy service");
        }
        return ec;
    }
    return NOERROR;
}

} // namespace Admin
} // namespace App
} // namespace Droid
} // namespace Elastos
