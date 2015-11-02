
#include "elastos/droid/app/admin/CDevicePolicyManager.h"
#include "elastos/droid/app/admin/CDeviceAdminInfo.h"
#include "elastos/droid/os/CServiceManager.h"
#include "elastos/droid/os/CUserHandleHelper.h"
#include "elastos/droid/net/CProxy.h"
#include "elastos/droid/content/pm/CResolveInfo.h"
#include <elastos/utility/logging/Logger.h>
#include <elastos/core/StringUtils.h>
#include <elastos/core/StringBuilder.h>

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

const String CDevicePolicyManager::TAG("CDevicePolicyManager");

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
    return IsAdminActiveAsUser(who, UserHandle::MyUserId(), isAdminActive);
}

ECode CDevicePolicyManager::IsAdminActive(
    /* [in] */ IComponentName* who,
    /* [in] */ Int32 userId,
    /* [out] */ Boolean* isAdminActive)
{
    VALIDATE_NOT_NULL(isAdminActive);
    *isAdminActive = FALSE;
    if (mService != NULL) {
        ECode ec = mService->IsAdminActive(who, userId, isAdminActive);
        if (FAILED(ec)) {
            Logger::W(TAG, "E_REMOTE_EXCEPTION: Failed talking with device policy service");
        }
        return ec;
    }
    return NOERROR;
}

ECode CDevicePolicyManager::GetActiveAdmins(
    /* [out] */ IList** admins)
{
    return GetActiveAdmins(UserHandle::MyUserId(), admins);
}

ECode CDevicePolicyManager::GetActiveAdmins(
    /* [in] */ Int32 userId,
    /* [out] */ IList** admins)
{
    VALIDATE_NOT_NULL(admins);
    *admins = NULL;

    if (mService != NULL) {
        ECode ec = mService->GetActiveAdmins(userId, admins);
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
    return GetPasswordQuality(admin, myUserId, password);
}

ECode CDevicePolicyManager::GetPasswordQuality(
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
    return GetPasswordMinimumLength(admin, myUserId, password);
}

ECode CDevicePolicyManager::GetPasswordMinimumLength(
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
    return GetPasswordMinimumUpperCase(admin, myUserId, password);
}

ECode CDevicePolicyManager::GetPasswordMinimumUpperCase(
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
    return GetPasswordMinimumLowerCase(admin, myUserId, password);
}

ECode CDevicePolicyManager::GetPasswordMinimumLowerCase(
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
    return GetPasswordMinimumLetters(admin, myUserId, password);
}

ECode CDevicePolicyManager::GetPasswordMinimumLetters(
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
    return GetPasswordMinimumNumeric(admin, myUserId, password);
}

ECode CDevicePolicyManager::GetPasswordMinimumNumeric(
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
    return GetPasswordMinimumSymbols(admin, myUserId, password);
}

ECode CDevicePolicyManager::GetPasswordMinimumSymbols(
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
    return GetPasswordMinimumNonLetter(admin, myUserId, password);
}

ECode CDevicePolicyManager::GetPasswordMinimumNonLetter(
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
    return GetPasswordHistoryLength(admin, myUserId, length);
}

ECode CDevicePolicyManager::GetPasswordHistoryLength(
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
    return GetMaximumFailedPasswordsForWipe(admin, myUserId, maximum);
}

ECode CDevicePolicyManager::GetMaximumFailedPasswordsForWipe(
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
    return GetMaximumTimeToLock(admin, myUserId, time);
}

ECode CDevicePolicyManager::GetMaximumTimeToLock(
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
    VALIDATE_NOT_NULL(component);
    *component = NULL;
    VALIDATE_NOT_NULL(admin);
    VALIDATE_NOT_NULL(exclusionList);

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
            sb += StringUtils::ToString(port);
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
            Int32 result;
            netProxy->Validate(hostName, StringUtils::ToString(port), exclSpec, &result);
            if (result != Elastos::Droid::Net::IProxy::PROXY_VALID) {
                return E_ILLEGAL_ARGUMENT_EXCEPTION;
            }
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

ECode CDevicePolicyManager::SetRecommendedGlobalProxy(
    /* [in] */ IComponentName* admin,
    /* [in] */ IProxyInfo* proxyInfo);
{
    if (mService != NULL) {
        // try {
            return mService->SetRecommendedGlobalProxy(admin, proxyInfo);
        // } catch (RemoteException e) {
        //     Log.w(TAG, "Failed talking with device policy service", e);
        // }
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
    return GetStorageEncryptionStatus(myUserId, st);
}

ECode CDevicePolicyManager::GetStorageEncryptionStatus(
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

ECode CDevicePolicyManager::InstallCaCert(
    /* [in] */ IComponentName* admin,
    /* [in] */ ArrayOf<Byte>* certBuffer,
    /* [out] */ Booelan* result)
{
    VALIDATE_NOT_NULL(result)
    *result = FALSE;

    if (mService != NULL) {
        // try {
            return mService->InstallCaCert(admin, certBuffer, result);
        // } catch (RemoteException e) {
        //     Log.w(TAG, "Failed talking with device policy service", e);
        // }
    }
    return NOERROR;
}

ECode CDevicePolicyManager::UninstallCaCert(
    /* [in] */ IComponentName* admin,
    /* [in] */ ArrayOf<Byte>* certBuffer)
{
    ECode ec = NOERROR;
    if (mService != NULL) {
        // try {
        String alias;
        GetCaCertAlias(certBuffer, &alias);
        ec = mService->UninstallCaCert(admin, alias);
        if (ec == (ECode)E_CERTIFICATE_EXCEPTION) {
            Logger::W(TAG, "Unable to parse certificate: CertificateException");
            ec = NOERROR;
        }
        else if (ec == (ECode)E_REMOTE_EXCEPTION) {
            Logger::W(TAG, "Failed talking with device policy service RemoteException");
            ec = NOERROR;
        }
        // } catch (CertificateException e) {
        //     Log.w(TAG, "Unable to parse certificate", e);
        // } catch (RemoteException e) {
        //     Log.w(TAG, "Failed talking with device policy service", e);
        // }
    }
    return ec;
}

ECode CDevicePolicyManager::GetInstalledCaCerts(
    /* [in] */ IComponentName* admin,
    /* [out] */ IList** list)
{
    VALIDATE_NOT_NULL(list)
    assert(0 && "TODO");
    List<byte[]> certs = new ArrayList<byte[]>();
    if (mService != null) {
        try {
            mService.enforceCanManageCaCerts(admin);
            final TrustedCertificateStore certStore = new TrustedCertificateStore();
            for (String alias : certStore.userAliases()) {
                try {
                    certs.add(certStore.getCertificate(alias).getEncoded());
                } catch (CertificateException ce) {
                    Log.w(TAG, "Could not encode certificate: " + alias, ce);
                }
            }
        } catch (RemoteException re) {
            Log.w(TAG, "Failed talking with device policy service", re);
        }
    }
    // return certs;
    return NOERROR;
}

ECode CDevicePolicyManager::UninstallAllUserCaCerts(
    /* [in] */ IComponentName* admin)
{
    if (mService != null) {
        for (String alias : new TrustedCertificateStore().userAliases()) {
            try {
                mService.uninstallCaCert(admin, alias);
            } catch (RemoteException re) {
                Log.w(TAG, "Failed talking with device policy service", re);
            }
        }
    }
}

ECode CDevicePolicyManager::HasCaCertInstalled(
    /* [in] */ IComponentName* admin,
    /* [in] */ ArrayOf<Byte>* certBuffer,
    /* [out] */ Boolean* result)
{
    if (mService != NULL) {
        try {
            mService->EnforceCanManageCaCerts(admin);
            return GetCaCertAlias(certBuffer) != null;
        } catch (RemoteException re) {
            Log.w(TAG, "Failed talking with device policy service", re);
        } catch (CertificateException ce) {
            Log.w(TAG, "Could not parse certificate", ce);
        }
    }
    // return false;
}

ECode CDevicePolicyManager::InstallKeyPair(
    /* [in] */ IComponentName* who,
    /* [in] */ IPrivateKey* privKey,
    /* [in] */ ICertificate* cert,
    /* [in] */ const String& alias,
    /* [out] */ Boolean* result)
{
    try {
        final byte[] pemCert = Credentials.convertToPem(cert);
        return mService.installKeyPair(who, privKey.getEncoded(), pemCert, alias);
    } catch (CertificateException e) {
        Log.w(TAG, "Error encoding certificate", e);
    } catch (IOException e) {
        Log.w(TAG, "Error writing certificate", e);
    } catch (RemoteException e) {
        Log.w(TAG, "Failed talking with device policy service", e);
    }
    return false;
}

ECode CDevicePolicyManager::GetCaCertAlias(
    /* [in] */ Arrayof<Byte>* certBuffer,
    /* [out] */ String* alias)
{
    final CertificateFactory certFactory = CertificateFactory.getInstance("X.509");
    final X509Certificate cert = (X509Certificate) certFactory.generateCertificate(
                          new ByteArrayInputStream(certBuffer));
    return new TrustedCertificateStore().getCertificateAlias(cert);
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
    return GetCameraDisabled(admin, myUserId, disabled);
}

ECode CDevicePolicyManager::GetCameraDisabled(
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

//=======================

ECode CDevicePolicyManager::SetScreenCaptureDisabled(
    /* [in] */ IComponentName* admin,
    /* [in] */ Boolean disabled)
{
    if (mService != null) {
        try {
            mService.setScreenCaptureDisabled(admin, UserHandle.myUserId(), disabled);
        } catch (RemoteException e) {
            Log.w(TAG, "Failed talking with device policy service", e);
        }
    }
}

ECode CDevicePolicyManager::GetScreenCaptureDisabled(
    /* [in] */ IComponentName* admin,
    /* [out] */ Boolean* result)
{
    return getScreenCaptureDisabled(admin, UserHandle.myUserId());
}

ECode CDevicePolicyManager::GetScreenCaptureDisabled(
    /* [in] */ IComponentName* admin,
    /* [in] */ Int32 userHandle,
    /* [out] */ Boolean* result)
{
    if (mService != null) {
        try {
            return mService.getScreenCaptureDisabled(admin, userHandle);
        } catch (RemoteException e) {
            Log.w(TAG, "Failed talking with device policy service", e);
        }
    }
    return false;
}

ECode CDevicePolicyManager::SetAutoTimeRequired(
    /* [in] */ IComponentName* admin,
    /* [in] */ Boolean required)
{
    if (mService != null) {
        try {
            mService.setAutoTimeRequired(admin, UserHandle.myUserId(), required);
        } catch (RemoteException e) {
            Log.w(TAG, "Failed talking with device policy service", e);
        }
    }
}

ECode CDevicePolicyManager::GetAutoTimeRequired(
    /* [out] */ Boolean* result)
{
    if (mService != null) {
        try {
            return mService.getAutoTimeRequired();
        } catch (RemoteException e) {
            Log.w(TAG, "Failed talking with device policy service", e);
        }
    }
    return false;
}

//=======================

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
    return GetKeyguardDisabledFeatures(admin, myUserId, which);
}

ECode CDevicePolicyManager::GetKeyguardDisabledFeatures(
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
    return SetActiveAdmin(policyReceiver,  refreshing, UserHandle::MyUserId());
}

ECode CDevicePolicyManager::SetActiveAdmin(
    /* [in] */ IComponentName* policyReceiver,
    /* [in] */ Boolean refreshing,
    /* [in] */ Int32 userHandle)
{
    if (mService != NULL) {
        ECode ec = mService->SetActiveAdmin(policyReceiver, refreshing, userHandle);
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
    if (ec == (ECode)E_XML_PULL_PARSER_EXCEPTION) {
        String warn("XmlPullParserException: Unable to parse device policy");
        warn += cnStr;
        Logger::W(TAG, warn);

        *info = NULL;
        return ec;
    }
    else if (ec == (ECode)E_IO_EXCEPTION) {
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

//=================================================

/**
 * @hide
 * Sets the given package as the device owner. The package must already be installed and there
 * shouldn't be an existing device owner registered, for this call to succeed. Also, this
 * method must be called before the device is provisioned.
 * @param packageName the package name of the application to be registered as the device owner.
 * @return whether the package was successfully registered as the device owner.
 * @throws IllegalArgumentException if the package name is null or invalid
 * @throws IllegalStateException if a device owner is already registered or the device has
 *         already been provisioned.
 */
public boolean setDeviceOwner(String packageName) throws IllegalArgumentException,
        IllegalStateException {
    return setDeviceOwner(packageName, null);
}

/**
 * @hide
 * Sets the given package as the device owner. The package must already be installed and there
 * shouldn't be an existing device owner registered, for this call to succeed. Also, this
 * method must be called before the device is provisioned.
 * @param packageName the package name of the application to be registered as the device owner.
 * @param ownerName the human readable name of the institution that owns this device.
 * @return whether the package was successfully registered as the device owner.
 * @throws IllegalArgumentException if the package name is null or invalid
 * @throws IllegalStateException if a device owner is already registered or the device has
 *         already been provisioned.
 */
public boolean setDeviceOwner(String packageName, String ownerName)
        throws IllegalArgumentException, IllegalStateException {
    if (mService != null) {
        try {
            return mService.setDeviceOwner(packageName, ownerName);
        } catch (RemoteException re) {
            Log.w(TAG, "Failed to set device owner");
        }
    }
    return false;
}

/**
 * Used to determine if a particular package has been registered as a Device Owner app.
 * A device owner app is a special device admin that cannot be deactivated by the user, once
 * activated as a device admin. It also cannot be uninstalled. To check if a particular
 * package is currently registered as the device owner app, pass in the package name from
 * {@link Context#getPackageName()} to this method.<p/>This is useful for device
 * admin apps that want to check if they are also registered as the device owner app. The
 * exact mechanism by which a device admin app is registered as a device owner app is defined by
 * the setup process.
 * @param packageName the package name of the app, to compare with the registered device owner
 * app, if any.
 * @return whether or not the package is registered as the device owner app.
 */
public boolean isDeviceOwnerApp(String packageName) {
    if (mService != null) {
        try {
            return mService.isDeviceOwner(packageName);
        } catch (RemoteException re) {
            Log.w(TAG, "Failed to check device owner");
        }
    }
    return false;
}

/**
 * @hide
 * Redirect to isDeviceOwnerApp.
 */
public boolean isDeviceOwner(String packageName) {
    return isDeviceOwnerApp(packageName);
}

/**
 * Clears the current device owner.  The caller must be the device owner.
 *
 * This function should be used cautiously as once it is called it cannot
 * be undone.  The device owner can only be set as a part of device setup
 * before setup completes.
 *
 * @param packageName The package name of the device owner.
 */
public void clearDeviceOwnerApp(String packageName) {
    if (mService != null) {
        try {
            mService.clearDeviceOwner(packageName);
        } catch (RemoteException re) {
            Log.w(TAG, "Failed to clear device owner");
        }
    }
}

/** @hide */
@SystemApi
public String getDeviceOwner() {
    if (mService != null) {
        try {
            return mService.getDeviceOwner();
        } catch (RemoteException re) {
            Log.w(TAG, "Failed to get device owner");
        }
    }
    return null;
}

/** @hide */
public String getDeviceOwnerName() {
    if (mService != null) {
        try {
            return mService.getDeviceOwnerName();
        } catch (RemoteException re) {
            Log.w(TAG, "Failed to get device owner");
        }
    }
    return null;
}

/**
 * @hide
 * @deprecated Use #ACTION_SET_PROFILE_OWNER
 * Sets the given component as an active admin and registers the package as the profile
 * owner for this user. The package must already be installed and there shouldn't be
 * an existing profile owner registered for this user. Also, this method must be called
 * before the user setup has been completed.
 * <p>
 * This method can only be called by system apps that hold MANAGE_USERS permission and
 * MANAGE_DEVICE_ADMINS permission.
 * @param admin The component to register as an active admin and profile owner.
 * @param ownerName The user-visible name of the entity that is managing this user.
 * @return whether the admin was successfully registered as the profile owner.
 * @throws IllegalArgumentException if packageName is null, the package isn't installed, or
 *         the user has already been set up.
 */
@SystemApi
public boolean setActiveProfileOwner(ComponentName admin, String ownerName)
        throws IllegalArgumentException {
    if (mService != null) {
        try {
            final int myUserId = UserHandle.myUserId();
            mService.setActiveAdmin(admin, false, myUserId);
            return mService.setProfileOwner(admin, ownerName, myUserId);
        } catch (RemoteException re) {
            Log.w(TAG, "Failed to set profile owner " + re);
            throw new IllegalArgumentException("Couldn't set profile owner.", re);
        }
    }
    return false;
}

/**
 * @hide
 * Clears the active profile owner and removes all user restrictions. The caller must
 * be from the same package as the active profile owner for this user, otherwise a
 * SecurityException will be thrown.
 *
 * @param admin The component to remove as the profile owner.
 * @return
 */
@SystemApi
public void clearProfileOwner(ComponentName admin) {
    if (mService != null) {
        try {
            mService.clearProfileOwner(admin);
        } catch (RemoteException re) {
            Log.w(TAG, "Failed to clear profile owner " + admin + re);
        }
    }
}

/**
 * @hide
 * Checks if the user was already setup.
 */
public boolean hasUserSetupCompleted() {
    if (mService != null) {
        try {
            return mService.hasUserSetupCompleted();
        } catch (RemoteException re) {
            Log.w(TAG, "Failed to check if user setup has completed");
        }
    }
    return true;
}

/**
 * @deprecated Use setProfileOwner(ComponentName ...)
 * @hide
 * Sets the given package as the profile owner of the given user profile. The package must
 * already be installed and there shouldn't be an existing profile owner registered for this
 * user. Also, this method must be called before the user has been used for the first time.
 * @param packageName the package name of the application to be registered as profile owner.
 * @param ownerName the human readable name of the organisation associated with this DPM.
 * @param userHandle the userId to set the profile owner for.
 * @return whether the package was successfully registered as the profile owner.
 * @throws IllegalArgumentException if packageName is null, the package isn't installed, or
 *         the user has already been set up.
 */
public boolean setProfileOwner(String packageName, String ownerName, int userHandle)
        throws IllegalArgumentException {
    if (packageName == null) {
        throw new NullPointerException("packageName cannot be null");
    }
    return setProfileOwner(new ComponentName(packageName, ""), ownerName, userHandle);
}

/**
 * @hide
 * Sets the given component as the profile owner of the given user profile. The package must
 * already be installed and there shouldn't be an existing profile owner registered for this
 * user. Only the system can call this API if the user has already completed setup.
 * @param admin the component name to be registered as profile owner.
 * @param ownerName the human readable name of the organisation associated with this DPM.
 * @param userHandle the userId to set the profile owner for.
 * @return whether the component was successfully registered as the profile owner.
 * @throws IllegalArgumentException if admin is null, the package isn't installed, or
 *         the user has already been set up.
 */
public boolean setProfileOwner(ComponentName admin, String ownerName, int userHandle)
        throws IllegalArgumentException {
    if (admin == null) {
        throw new NullPointerException("admin cannot be null");
    }
    if (mService != null) {
        try {
            if (ownerName == null) {
                ownerName = "";
            }
            return mService.setProfileOwner(admin, ownerName, userHandle);
        } catch (RemoteException re) {
            Log.w(TAG, "Failed to set profile owner", re);
            throw new IllegalArgumentException("Couldn't set profile owner.", re);
        }
    }
    return false;
}

/**
 * Sets the enabled state of the profile. A profile should be enabled only once it is ready to
 * be used. Only the profile owner can call this.
 *
 * @see #isProfileOwnerApp
 *
 * @param admin Which {@link DeviceAdminReceiver} this request is associated with.
 */
public void setProfileEnabled(ComponentName admin) {
    if (mService != null) {
        try {
            mService.setProfileEnabled(admin);
        } catch (RemoteException e) {
            Log.w(TAG, "Failed talking with device policy service", e);
        }
    }
}

/**
 * Sets the name of the profile. In the device owner case it sets the name of the user
 * which it is called from. Only a profile owner or device owner can call this. If this is
 * never called by the profile or device owner, the name will be set to default values.
 *
 * @see #isProfileOwnerApp
 * @see #isDeviceOwnerApp
 *
 * @param profileName The name of the profile.
 */
public void setProfileName(ComponentName who, String profileName) {
    if (mService != null) {
        try {
        mService.setProfileName(who, profileName);
    } catch (RemoteException e) {
        Log.w(TAG, "Failed talking with device policy service", e);
    }
}
}

/**
 * Used to determine if a particular package is registered as the profile owner for the
 * current user. A profile owner is a special device admin that has additional privileges
 * within the profile.
 *
 * @param packageName The package name of the app to compare with the registered profile owner.
 * @return Whether or not the package is registered as the profile owner.
 */
public boolean isProfileOwnerApp(String packageName) {
    if (mService != null) {
        try {
            ComponentName profileOwner = mService.getProfileOwner(
                    Process.myUserHandle().getIdentifier());
            return profileOwner != null
                    && profileOwner.getPackageName().equals(packageName);
        } catch (RemoteException re) {
            Log.w(TAG, "Failed to check profile owner");
        }
    }
    return false;
}

/**
 * @hide
 * @return the packageName of the owner of the given user profile or null if no profile
 * owner has been set for that user.
 * @throws IllegalArgumentException if the userId is invalid.
 */
@SystemApi
public ComponentName getProfileOwner() throws IllegalArgumentException {
    return getProfileOwnerAsUser(Process.myUserHandle().getIdentifier());
}

/**
 * @see #getProfileOwner()
 * @hide
 */
public ComponentName getProfileOwnerAsUser(final int userId) throws IllegalArgumentException {
    if (mService != null) {
        try {
            return mService.getProfileOwner(userId);
        } catch (RemoteException re) {
            Log.w(TAG, "Failed to get profile owner");
            throw new IllegalArgumentException(
                    "Requested profile owner for invalid userId", re);
        }
    }
    return null;
}

/**
 * @hide
 * @return the human readable name of the organisation associated with this DPM or null if
 *         one is not set.
 * @throws IllegalArgumentException if the userId is invalid.
 */
public String getProfileOwnerName() throws IllegalArgumentException {
    if (mService != null) {
        try {
            return mService.getProfileOwnerName(Process.myUserHandle().getIdentifier());
        } catch (RemoteException re) {
            Log.w(TAG, "Failed to get profile owner");
            throw new IllegalArgumentException(
                    "Requested profile owner for invalid userId", re);
        }
    }
    return null;
}

/**
 * @hide
 * @param user The user for whom to fetch the profile owner name, if any.
 * @return the human readable name of the organisation associated with this profile owner or
 *         null if one is not set.
 * @throws IllegalArgumentException if the userId is invalid.
 */
@SystemApi
public String getProfileOwnerNameAsUser(int userId) throws IllegalArgumentException {
    if (mService != null) {
        try {
            return mService.getProfileOwnerName(userId);
        } catch (RemoteException re) {
            Log.w(TAG, "Failed to get profile owner");
            throw new IllegalArgumentException(
                    "Requested profile owner for invalid userId", re);
        }
    }
    return null;
}

/**
 * Called by a profile owner or device owner to add a default intent handler activity for
 * intents that match a certain intent filter. This activity will remain the default intent
 * handler even if the set of potential event handlers for the intent filter changes and if
 * the intent preferences are reset.
 *
 * <p>The default disambiguation mechanism takes over if the activity is not installed
 * (anymore). When the activity is (re)installed, it is automatically reset as default
 * intent handler for the filter.
 *
 * <p>The calling device admin must be a profile owner or device owner. If it is not, a
 * security exception will be thrown.
 *
 * @param admin Which {@link DeviceAdminReceiver} this request is associated with.
 * @param filter The IntentFilter for which a default handler is added.
 * @param activity The Activity that is added as default intent handler.
 */
public void addPersistentPreferredActivity(ComponentName admin, IntentFilter filter,
        ComponentName activity) {
    if (mService != null) {
        try {
            mService.addPersistentPreferredActivity(admin, filter, activity);
        } catch (RemoteException e) {
            Log.w(TAG, "Failed talking with device policy service", e);
        }
    }
}

/**
 * Called by a profile owner or device owner to remove all persistent intent handler preferences
 * associated with the given package that were set by {@link #addPersistentPreferredActivity}.
 *
 * <p>The calling device admin must be a profile owner. If it is not, a security
 * exception will be thrown.
 *
 * @param admin Which {@link DeviceAdminReceiver} this request is associated with.
 * @param packageName The name of the package for which preferences are removed.
 */
public void clearPackagePersistentPreferredActivities(ComponentName admin,
        String packageName) {
    if (mService != null) {
        try {
            mService.clearPackagePersistentPreferredActivities(admin, packageName);
        } catch (RemoteException e) {
            Log.w(TAG, "Failed talking with device policy service", e);
        }
    }
}

/**
 * Called by a profile or device owner to set the application restrictions for a given target
 * application running in the profile.
 *
 * <p>The provided {@link Bundle} consists of key-value pairs, where the types of values may be
 * boolean, int, String, or String[].
 *
 * <p>The application restrictions are only made visible to the target application and the
 * profile or device owner.
 *
 * <p>The calling device admin must be a profile or device owner; if it is not, a security
 * exception will be thrown.
 *
 * @param admin Which {@link DeviceAdminReceiver} this request is associated with.
 * @param packageName The name of the package to update restricted settings for.
 * @param settings A {@link Bundle} to be parsed by the receiving application, conveying a new
 * set of active restrictions.
 */
public void setApplicationRestrictions(ComponentName admin, String packageName,
        Bundle settings) {
    if (mService != null) {
        try {
            mService.setApplicationRestrictions(admin, packageName, settings);
        } catch (RemoteException e) {
            Log.w(TAG, "Failed talking with device policy service", e);
        }
    }
}

/**
 * Sets a list of features to enable for a TrustAgent component. This is meant to be
 * used in conjunction with {@link #KEYGUARD_DISABLE_TRUST_AGENTS}, which will disable all
 * trust agents but those with features enabled by this function call.
 *
 * <p>The calling device admin must have requested
 * {@link DeviceAdminInfo#USES_POLICY_DISABLE_KEYGUARD_FEATURES} to be able to call
 * this method; if it has not, a security exception will be thrown.
 *
 * @param admin Which {@link DeviceAdminReceiver} this request is associated with.
 * @param agent Which component to enable features for.
 * @param features List of features to enable. Consult specific TrustAgent documentation for
 * the feature list.
 * @hide
 */
public void setTrustAgentFeaturesEnabled(ComponentName admin, ComponentName agent,
        List<String> features) {
    if (mService != null) {
        try {
            mService.setTrustAgentFeaturesEnabled(admin, agent, features, UserHandle.myUserId());
        } catch (RemoteException e) {
            Log.w(TAG, "Failed talking with device policy service", e);
        }
    }
}

/**
 * Gets list of enabled features for the given TrustAgent component. If admin is
 * null, this will return the intersection of all features enabled for the given agent by all
 * admins.
 *
 * @param admin Which {@link DeviceAdminReceiver} this request is associated with.
 * @param agent Which component to get enabled features for.
 * @return List of enabled features.
 * @hide
 */
public List<String> getTrustAgentFeaturesEnabled(ComponentName admin, ComponentName agent) {
    if (mService != null) {
        try {
            return mService.getTrustAgentFeaturesEnabled(admin, agent, UserHandle.myUserId());
        } catch (RemoteException e) {
            Log.w(TAG, "Failed talking with device policy service", e);
        }
    }
    return new ArrayList<String>(); // empty list
}

/**
 * Called by a profile owner of a managed profile to set whether caller-Id information from
 * the managed profile will be shown in the parent profile, for incoming calls.
 *
 * <p>The calling device admin must be a profile owner. If it is not, a
 * security exception will be thrown.
 *
 * @param who Which {@link DeviceAdminReceiver} this request is associated with.
 * @param disabled If true caller-Id information in the managed profile is not displayed.
 */
public void setCrossProfileCallerIdDisabled(ComponentName who, boolean disabled) {
    if (mService != null) {
        try {
            mService.setCrossProfileCallerIdDisabled(who, disabled);
        } catch (RemoteException e) {
            Log.w(TAG, "Failed talking with device policy service", e);
        }
    }
}

/**
 * Called by a profile owner of a managed profile to determine whether or not caller-Id
 * information has been disabled.
 *
 * <p>The calling device admin must be a profile owner. If it is not, a
 * security exception will be thrown.
 *
 * @param who Which {@link DeviceAdminReceiver} this request is associated with.
 */
public boolean getCrossProfileCallerIdDisabled(ComponentName who) {
    if (mService != null) {
        try {
            return mService.getCrossProfileCallerIdDisabled(who);
        } catch (RemoteException e) {
            Log.w(TAG, "Failed talking with device policy service", e);
        }
    }
    return false;
}

/**
 * Determine whether or not caller-Id information has been disabled.
 *
 * @param userHandle The user for whom to check the caller-id permission
 * @hide
 */
public boolean getCrossProfileCallerIdDisabled(UserHandle userHandle) {
    if (mService != null) {
        try {
            return mService.getCrossProfileCallerIdDisabledForUser(userHandle.getIdentifier());
        } catch (RemoteException e) {
            Log.w(TAG, "Failed talking with device policy service", e);
        }
    }
    return false;
}

/**
 * Called by the profile owner of a managed profile so that some intents sent in the managed
 * profile can also be resolved in the parent, or vice versa.
 * @param admin Which {@link DeviceAdminReceiver} this request is associated with.
 * @param filter The {@link IntentFilter} the intent has to match to be also resolved in the
 * other profile
 * @param flags {@link DevicePolicyManager#FLAG_MANAGED_CAN_ACCESS_PARENT} and
 * {@link DevicePolicyManager#FLAG_PARENT_CAN_ACCESS_MANAGED} are supported.
 */
public void addCrossProfileIntentFilter(ComponentName admin, IntentFilter filter, int flags) {
    if (mService != null) {
        try {
            mService.addCrossProfileIntentFilter(admin, filter, flags);
        } catch (RemoteException e) {
            Log.w(TAG, "Failed talking with device policy service", e);
        }
    }
}

/**
 * Called by a profile owner of a managed profile to remove the cross-profile intent filters
 * that go from the managed profile to the parent, or from the parent to the managed profile.
 * Only removes those that have been set by the profile owner.
 * @param admin Which {@link DeviceAdminReceiver} this request is associated with.
 */
public void clearCrossProfileIntentFilters(ComponentName admin) {
    if (mService != null) {
        try {
            mService.clearCrossProfileIntentFilters(admin);
        } catch (RemoteException e) {
            Log.w(TAG, "Failed talking with device policy service", e);
        }
    }
}

/**
 * Called by a profile or device owner to set the permitted accessibility services. When
 * set by a device owner or profile owner the restriction applies to all profiles of the
 * user the device owner or profile owner is an admin for.
 *
 * By default the user can use any accessiblity service. When zero or more packages have
 * been added, accessiblity services that are not in the list and not part of the system
 * can not be enabled by the user.
 *
 * <p> Calling with a null value for the list disables the restriction so that all services
 * can be used, calling with an empty list only allows the builtin system's services.
 *
 * <p> System accesibility services are always available to the user the list can't modify
 * this.
 *
 * @param admin Which {@link DeviceAdminReceiver} this request is associated with.
 * @param packageNames List of accessibility service package names.
 *
 * @return true if setting the restriction succeeded. It fail if there is
 * one or more non-system accessibility services enabled, that are not in the list.
 */
public boolean setPermittedAccessibilityServices(ComponentName admin,
        List<String> packageNames) {
    if (mService != null) {
        try {
            return mService.setPermittedAccessibilityServices(admin, packageNames);
        } catch (RemoteException e) {
            Log.w(TAG, "Failed talking with device policy service", e);
        }
    }
    return false;
}

/**
 * Returns the list of permitted accessibility services set by this device or profile owner.
 *
 * <p>An empty list means no accessibility services except system services are allowed.
 * Null means all accessibility services are allowed.
 *
 * @param admin Which {@link DeviceAdminReceiver} this request is associated with.
 * @return List of accessiblity service package names.
 */
public List<String> getPermittedAccessibilityServices(ComponentName admin) {
    if (mService != null) {
        try {
            return mService.getPermittedAccessibilityServices(admin);
        } catch (RemoteException e) {
            Log.w(TAG, "Failed talking with device policy service", e);
        }
    }
    return null;
}

/**
 * Returns the list of accessibility services permitted by the device or profiles
 * owners of this user.
 *
 * <p>Null means all accessibility services are allowed, if a non-null list is returned
 * it will contain the intersection of the permitted lists for any device or profile
 * owners that apply to this user. It will also include any system accessibility services.
 *
 * @param userId which user to check for.
 * @return List of accessiblity service package names.
 * @hide
 */
 @SystemApi
 public List<String> getPermittedAccessibilityServices(int userId) {
    if (mService != null) {
        try {
            return mService.getPermittedAccessibilityServicesForUser(userId);
        } catch (RemoteException e) {
            Log.w(TAG, "Failed talking with device policy service", e);
        }
    }
    return null;
 }

/**
 * Called by a profile or device owner to set the permitted input methods services. When
 * set by a device owner or profile owner the restriction applies to all profiles of the
 * user the device owner or profile owner is an admin for.
 *
 * By default the user can use any input method. When zero or more packages have
 * been added, input method that are not in the list and not part of the system
 * can not be enabled by the user.
 *
 * This method will fail if it is called for a admin that is not for the foreground user
 * or a profile of the foreground user.
 *
 * <p> Calling with a null value for the list disables the restriction so that all input methods
 * can be used, calling with an empty list disables all but the system's own input methods.
 *
 * <p> System input methods are always available to the user this method can't modify this.
 *
 * @param admin Which {@link DeviceAdminReceiver} this request is associated with.
 * @param packageNames List of input method package names.
 * @return true if setting the restriction succeeded. It will fail if there is
 *     one or more input method enabled, that are not in the list or user if the foreground
 *     user.
 */
public boolean setPermittedInputMethods(ComponentName admin, List<String> packageNames) {
    if (mService != null) {
        try {
            return mService.setPermittedInputMethods(admin, packageNames);
        } catch (RemoteException e) {
            Log.w(TAG, "Failed talking with device policy service", e);
        }
    }
    return false;
}


/**
 * Returns the list of permitted input methods set by this device or profile owner.
 *
 * <p>An empty list means no input methods except system input methods are allowed.
 * Null means all input methods are allowed.
 *
 * @param admin Which {@link DeviceAdminReceiver} this request is associated with.
 * @return List of input method package names.
 */
public List<String> getPermittedInputMethods(ComponentName admin) {
    if (mService != null) {
        try {
            return mService.getPermittedInputMethods(admin);
        } catch (RemoteException e) {
            Log.w(TAG, "Failed talking with device policy service", e);
        }
    }
    return null;
}

/**
 * Returns the list of input methods permitted by the device or profiles
 * owners of the current user.
 *
 * <p>Null means all input methods are allowed, if a non-null list is returned
 * it will contain the intersection of the permitted lists for any device or profile
 * owners that apply to this user. It will also include any system input methods.
 *
 * @return List of input method package names.
 * @hide
 */
@SystemApi
public List<String> getPermittedInputMethodsForCurrentUser() {
    if (mService != null) {
        try {
            return mService.getPermittedInputMethodsForCurrentUser();
        } catch (RemoteException e) {
            Log.w(TAG, "Failed talking with device policy service", e);
        }
    }
    return null;
}

/**
 * Called by a device owner to create a user with the specified name. The UserHandle returned
 * by this method should not be persisted as user handles are recycled as users are removed and
 * created. If you need to persist an identifier for this user, use
 * {@link UserManager#getSerialNumberForUser}.
 *
 * @param admin Which {@link DeviceAdminReceiver} this request is associated with.
 * @param name the user's name
 * @see UserHandle
 * @return the UserHandle object for the created user, or null if the user could not be created.
 */
public UserHandle createUser(ComponentName admin, String name) {
    try {
        return mService.createUser(admin, name);
    } catch (RemoteException re) {
        Log.w(TAG, "Could not create a user", re);
    }
    return null;
}

/**
 * Called by a device owner to create a user with the specified name. The UserHandle returned
 * by this method should not be persisted as user handles are recycled as users are removed and
 * created. If you need to persist an identifier for this user, use
 * {@link UserManager#getSerialNumberForUser}.  The new user will be started in the background
 * immediately.
 *
 * <p> profileOwnerComponent is the {@link DeviceAdminReceiver} to be the profile owner as well
 * as registered as an active admin on the new user.  The profile owner package will be
 * installed on the new user if it already is installed on the device.
 *
 * <p>If the optionalInitializeData is not null, then the extras will be passed to the
 * profileOwnerComponent when onEnable is called.
 *
 * @param admin Which {@link DeviceAdminReceiver} this request is associated with.
 * @param name the user's name
 * @param ownerName the human readable name of the organisation associated with this DPM.
 * @param profileOwnerComponent The {@link DeviceAdminReceiver} that will be an active admin on
 *      the user.
 * @param adminExtras Extras that will be passed to onEnable of the admin receiver
 *      on the new user.
 * @see UserHandle
 * @return the UserHandle object for the created user, or null if the user could not be created.
 */
public UserHandle createAndInitializeUser(ComponentName admin, String name, String ownerName,
        ComponentName profileOwnerComponent, Bundle adminExtras) {
    try {
        return mService.createAndInitializeUser(admin, name, ownerName, profileOwnerComponent,
                adminExtras);
    } catch (RemoteException re) {
        Log.w(TAG, "Could not create a user", re);
    }
    return null;
}

/**
 * Called by a device owner to remove a user and all associated data. The primary user can
 * not be removed.
 *
 * @param admin Which {@link DeviceAdminReceiver} this request is associated with.
 * @param userHandle the user to remove.
 * @return {@code true} if the user was removed, {@code false} otherwise.
 */
public boolean removeUser(ComponentName admin, UserHandle userHandle) {
    try {
        return mService.removeUser(admin, userHandle);
    } catch (RemoteException re) {
        Log.w(TAG, "Could not remove user ", re);
        return false;
    }
}

/**
 * Called by a device owner to switch the specified user to the foreground.
 *
 * @param admin Which {@link DeviceAdminReceiver} this request is associated with.
 * @param userHandle the user to switch to; null will switch to primary.
 * @return {@code true} if the switch was successful, {@code false} otherwise.
 *
 * @see Intent#ACTION_USER_FOREGROUND
 */
public boolean switchUser(ComponentName admin, UserHandle userHandle) {
    try {
        return mService.switchUser(admin, userHandle);
    } catch (RemoteException re) {
        Log.w(TAG, "Could not switch user ", re);
        return false;
    }
}

/**
 * Called by a profile or device owner to get the application restrictions for a given target
 * application running in the profile.
 *
 * <p>The calling device admin must be a profile or device owner; if it is not, a security
 * exception will be thrown.
 *
 * @param admin Which {@link DeviceAdminReceiver} this request is associated with.
 * @param packageName The name of the package to fetch restricted settings of.
 * @return {@link Bundle} of settings corresponding to what was set last time
 * {@link DevicePolicyManager#setApplicationRestrictions} was called, or an empty {@link Bundle}
 * if no restrictions have been set.
 */
public Bundle getApplicationRestrictions(ComponentName admin, String packageName) {
    if (mService != null) {
        try {
            return mService.getApplicationRestrictions(admin, packageName);
        } catch (RemoteException e) {
            Log.w(TAG, "Failed talking with device policy service", e);
        }
    }
    return null;
}

/**
 * Called by a profile or device owner to set a user restriction specified
 * by the key.
 * <p>
 * The calling device admin must be a profile or device owner; if it is not,
 * a security exception will be thrown.
 *
 * @param admin Which {@link DeviceAdminReceiver} this request is associated
 *            with.
 * @param key The key of the restriction. See the constants in
 *            {@link android.os.UserManager} for the list of keys.
 */
public void addUserRestriction(ComponentName admin, String key) {
    if (mService != null) {
        try {
            mService.setUserRestriction(admin, key, true);
        } catch (RemoteException e) {
            Log.w(TAG, "Failed talking with device policy service", e);
        }
    }
}

/**
 * Called by a profile or device owner to clear a user restriction specified
 * by the key.
 * <p>
 * The calling device admin must be a profile or device owner; if it is not,
 * a security exception will be thrown.
 *
 * @param admin Which {@link DeviceAdminReceiver} this request is associated
 *            with.
 * @param key The key of the restriction. See the constants in
 *            {@link android.os.UserManager} for the list of keys.
 */
public void clearUserRestriction(ComponentName admin, String key) {
    if (mService != null) {
        try {
            mService.setUserRestriction(admin, key, false);
        } catch (RemoteException e) {
            Log.w(TAG, "Failed talking with device policy service", e);
        }
    }
}

/**
 * Called by device or profile owner to hide or unhide packages. When a package is hidden it
 * is unavailable for use, but the data and actual package file remain.
 *
 * @param admin Which {@link DeviceAdminReceiver} this request is associated with.
 * @param packageName The name of the package to hide or unhide.
 * @param hidden {@code true} if the package should be hidden, {@code false} if it should be
 *                 unhidden.
 * @return boolean Whether the hidden setting of the package was successfully updated.
 */
public boolean setApplicationHidden(ComponentName admin, String packageName,
        boolean hidden) {
    if (mService != null) {
        try {
            return mService.setApplicationHidden(admin, packageName, hidden);
        } catch (RemoteException e) {
            Log.w(TAG, "Failed talking with device policy service", e);
        }
    }
    return false;
}

/**
 * Called by device or profile owner to determine if a package is hidden.
 *
 * @param admin Which {@link DeviceAdminReceiver} this request is associated with.
 * @param packageName The name of the package to retrieve the hidden status of.
 * @return boolean {@code true} if the package is hidden, {@code false} otherwise.
 */
public boolean isApplicationHidden(ComponentName admin, String packageName) {
    if (mService != null) {
        try {
            return mService.isApplicationHidden(admin, packageName);
        } catch (RemoteException e) {
            Log.w(TAG, "Failed talking with device policy service", e);
        }
    }
    return false;
}

/**
 * Called by profile or device owner to re-enable a system app that was disabled by default
 * when the user was initialized.
 *
 * @param admin Which {@link DeviceAdminReceiver} this request is associated with.
 * @param packageName The package to be re-enabled in the current profile.
 */
public void enableSystemApp(ComponentName admin, String packageName) {
    if (mService != null) {
        try {
            mService.enableSystemApp(admin, packageName);
        } catch (RemoteException e) {
            Log.w(TAG, "Failed to install package: " + packageName);
        }
    }
}

/**
 * Called by profile or device owner to re-enable system apps by intent that were disabled
 * by default when the user was initialized.
 *
 * @param admin Which {@link DeviceAdminReceiver} this request is associated with.
 * @param intent An intent matching the app(s) to be installed. All apps that resolve for this
 *               intent will be re-enabled in the current profile.
 * @return int The number of activities that matched the intent and were installed.
 */
public int enableSystemApp(ComponentName admin, Intent intent) {
    if (mService != null) {
        try {
            return mService.enableSystemAppWithIntent(admin, intent);
        } catch (RemoteException e) {
            Log.w(TAG, "Failed to install packages matching filter: " + intent);
        }
    }
    return 0;
}

/**
 * Called by a profile owner to disable account management for a specific type of account.
 *
 * <p>The calling device admin must be a profile owner. If it is not, a
 * security exception will be thrown.
 *
 * <p>When account management is disabled for an account type, adding or removing an account
 * of that type will not be possible.
 *
 * @param admin Which {@link DeviceAdminReceiver} this request is associated with.
 * @param accountType For which account management is disabled or enabled.
 * @param disabled The boolean indicating that account management will be disabled (true) or
 * enabled (false).
 */
public void setAccountManagementDisabled(ComponentName admin, String accountType,
        boolean disabled) {
    if (mService != null) {
        try {
            mService.setAccountManagementDisabled(admin, accountType, disabled);
        } catch (RemoteException e) {
            Log.w(TAG, "Failed talking with device policy service", e);
        }
    }
}

/**
 * Gets the array of accounts for which account management is disabled by the profile owner.
 *
 * <p> Account management can be disabled/enabled by calling
 * {@link #setAccountManagementDisabled}.
 *
 * @return a list of account types for which account management has been disabled.
 *
 * @see #setAccountManagementDisabled
 */
public String[] getAccountTypesWithManagementDisabled() {
    return getAccountTypesWithManagementDisabledAsUser(UserHandle.myUserId());
}

/**
 * @see #getAccountTypesWithManagementDisabled()
 * @hide
 */
public String[] getAccountTypesWithManagementDisabledAsUser(int userId) {
    if (mService != null) {
        try {
            return mService.getAccountTypesWithManagementDisabledAsUser(userId);
        } catch (RemoteException e) {
            Log.w(TAG, "Failed talking with device policy service", e);
        }
    }

    return null;
}

/**
 * Sets which packages may enter lock task mode.
 *
 * <p>Any packages that shares uid with an allowed package will also be allowed
 * to activate lock task.
 *
 * This function can only be called by the device owner.
 * @param packages The list of packages allowed to enter lock task mode
 * @param admin Which {@link DeviceAdminReceiver} this request is associated with.
 *
 * @see Activity#startLockTask()
 * @see DeviceAdminReceiver#onLockTaskModeChanged(Context, Intent, boolean, String)
 * @see UserManager#DISALLOW_CREATE_WINDOWS
 */
public void setLockTaskPackages(ComponentName admin, String[] packages)
        throws SecurityException {
    if (mService != null) {
        try {
            mService.setLockTaskPackages(admin, packages);
        } catch (RemoteException e) {
            Log.w(TAG, "Failed talking with device policy service", e);
        }
    }
}

/**
 * This function returns the list of packages allowed to start the lock task mode.
 *
 * @param admin Which {@link DeviceAdminReceiver} this request is associated with.
 * @hide
 */
public String[] getLockTaskPackages(ComponentName admin) {
    if (mService != null) {
        try {
            return mService.getLockTaskPackages(admin);
        } catch (RemoteException e) {
            Log.w(TAG, "Failed talking with device policy service", e);
        }
    }
    return null;
}

/**
 * This function lets the caller know whether the given component is allowed to start the
 * lock task mode.
 * @param pkg The package to check
 */
public boolean isLockTaskPermitted(String pkg) {
    if (mService != null) {
        try {
            return mService.isLockTaskPermitted(pkg);
        } catch (RemoteException e) {
            Log.w(TAG, "Failed talking with device policy service", e);
        }
    }
    return false;
}

/**
 * Called by device owners to update {@link Settings.Global} settings. Validation that the value
 * of the setting is in the correct form for the setting type should be performed by the caller.
 * <p>The settings that can be updated with this method are:
 * <ul>
 * <li>{@link Settings.Global#ADB_ENABLED}</li>
 * <li>{@link Settings.Global#AUTO_TIME}</li>
 * <li>{@link Settings.Global#AUTO_TIME_ZONE}</li>
 * <li>{@link Settings.Global#BLUETOOTH_ON}</li>
 * <li>{@link Settings.Global#DATA_ROAMING}</li>
 * <li>{@link Settings.Global#DEVELOPMENT_SETTINGS_ENABLED}</li>
 * <li>{@link Settings.Global#MODE_RINGER}</li>
 * <li>{@link Settings.Global#NETWORK_PREFERENCE}</li>
 * <li>{@link Settings.Global#USB_MASS_STORAGE_ENABLED}</li>
 * <li>{@link Settings.Global#WIFI_ON}</li>
 * <li>{@link Settings.Global#WIFI_SLEEP_POLICY}</li>
 * </ul>
 *
 * @param admin Which {@link DeviceAdminReceiver} this request is associated with.
 * @param setting The name of the setting to update.
 * @param value The value to update the setting to.
 */
public void setGlobalSetting(ComponentName admin, String setting, String value) {
    if (mService != null) {
        try {
            mService.setGlobalSetting(admin, setting, value);
        } catch (RemoteException e) {
            Log.w(TAG, "Failed talking with device policy service", e);
        }
    }
}

/**
 * Called by profile or device owners to update {@link Settings.Secure} settings. Validation
 * that the value of the setting is in the correct form for the setting type should be performed
 * by the caller.
 * <p>The settings that can be updated by a profile or device owner with this method are:
 * <ul>
 * <li>{@link Settings.Secure#DEFAULT_INPUT_METHOD}</li>
 * <li>{@link Settings.Secure#SKIP_FIRST_USE_HINTS}</li>
 * </ul>
 * <p>A device owner can additionally update the following settings:
 * <ul>
 * <li>{@link Settings.Secure#LOCATION_MODE}</li>
 * </ul>
 * @param admin Which {@link DeviceAdminReceiver} this request is associated with.
 * @param setting The name of the setting to update.
 * @param value The value to update the setting to.
 */
public void setSecureSetting(ComponentName admin, String setting, String value) {
    if (mService != null) {
        try {
            mService.setSecureSetting(admin, setting, value);
        } catch (RemoteException e) {
            Log.w(TAG, "Failed talking with device policy service", e);
        }
    }
}

/**
 * Designates a specific service component as the provider for
 * making permission requests of a local or remote administrator of the user.
 * <p/>
 * Only a profile owner can designate the restrictions provider.
 * @param admin Which {@link DeviceAdminReceiver} this request is associated with.
 * @param provider The component name of the service that implements
 * {@link RestrictionsReceiver}. If this param is null,
 * it removes the restrictions provider previously assigned.
 */
public void setRestrictionsProvider(ComponentName admin, ComponentName provider) {
    if (mService != null) {
        try {
            mService.setRestrictionsProvider(admin, provider);
        } catch (RemoteException re) {
            Log.w(TAG, "Failed to set permission provider on device policy service");
        }
    }
}

/**
 * Called by profile or device owners to set the master volume mute on or off.
 *
 * @param admin Which {@link DeviceAdminReceiver} this request is associated with.
 * @param on {@code true} to mute master volume, {@code false} to turn mute off.
 */
public void setMasterVolumeMuted(ComponentName admin, boolean on) {
    if (mService != null) {
        try {
            mService.setMasterVolumeMuted(admin, on);
        } catch (RemoteException re) {
            Log.w(TAG, "Failed to setMasterMute on device policy service");
        }
    }
}

/**
 * Called by profile or device owners to check whether the master volume mute is on or off.
 *
 * @param admin Which {@link DeviceAdminReceiver} this request is associated with.
 * @return {@code true} if master volume is muted, {@code false} if it's not.
 */
public boolean isMasterVolumeMuted(ComponentName admin) {
    if (mService != null) {
        try {
            return mService.isMasterVolumeMuted(admin);
        } catch (RemoteException re) {
            Log.w(TAG, "Failed to get isMasterMute on device policy service");
        }
    }
    return false;
}

/**
 * Called by profile or device owners to change whether a user can uninstall
 * a package.
 *
 * @param admin Which {@link DeviceAdminReceiver} this request is associated with.
 * @param packageName package to change.
 * @param uninstallBlocked true if the user shouldn't be able to uninstall the package.
 */
public void setUninstallBlocked(ComponentName admin, String packageName,
        boolean uninstallBlocked) {
    if (mService != null) {
        try {
            mService.setUninstallBlocked(admin, packageName, uninstallBlocked);
        } catch (RemoteException re) {
            Log.w(TAG, "Failed to call block uninstall on device policy service");
        }
    }
}

/**
 * Called by profile or device owners to check whether a user has been blocked from
 * uninstalling a package.
 *
 * @param admin Which {@link DeviceAdminReceiver} this request is associated with.
 * @param packageName package to check.
 * @return true if the user shouldn't be able to uninstall the package.
 */
public boolean isUninstallBlocked(ComponentName admin, String packageName) {
    if (mService != null) {
        try {
            return mService.isUninstallBlocked(admin, packageName);
        } catch (RemoteException re) {
            Log.w(TAG, "Failed to call block uninstall on device policy service");
        }
    }
    return false;
}

/**
 * Called by the profile owner of a managed profile to enable widget providers from a
 * given package to be available in the parent profile. As a result the user will be able to
 * add widgets from the white-listed package running under the profile to a widget
 * host which runs under the parent profile, for example the home screen. Note that
 * a package may have zero or more provider components, where each component
 * provides a different widget type.
 * <p>
 * <strong>Note:</strong> By default no widget provider package is white-listed.
 * </p>
 *
 * @param admin Which {@link DeviceAdminReceiver} this request is associated with.
 * @param packageName The package from which widget providers are white-listed.
 * @return Whether the package was added.
 *
 * @see #removeCrossProfileWidgetProvider(android.content.ComponentName, String)
 * @see #getCrossProfileWidgetProviders(android.content.ComponentName)
 */
public boolean addCrossProfileWidgetProvider(ComponentName admin, String packageName) {
    if (mService != null) {
        try {
            return mService.addCrossProfileWidgetProvider(admin, packageName);
        } catch (RemoteException re) {
            Log.w(TAG, "Error calling addCrossProfileWidgetProvider", re);
        }
    }
    return false;
}

/**
 * Called by the profile owner of a managed profile to disable widget providers from a given
 * package to be available in the parent profile. For this method to take effect the
 * package should have been added via {@link #addCrossProfileWidgetProvider(
 * android.content.ComponentName, String)}.
 * <p>
 * <strong>Note:</strong> By default no widget provider package is white-listed.
 * </p>
 *
 * @param admin Which {@link DeviceAdminReceiver} this request is associated with.
 * @param packageName The package from which widget providers are no longer
 *     white-listed.
 * @return Whether the package was removed.
 *
 * @see #addCrossProfileWidgetProvider(android.content.ComponentName, String)
 * @see #getCrossProfileWidgetProviders(android.content.ComponentName)
 */
public boolean removeCrossProfileWidgetProvider(ComponentName admin, String packageName) {
    if (mService != null) {
        try {
            return mService.removeCrossProfileWidgetProvider(admin, packageName);
        } catch (RemoteException re) {
            Log.w(TAG, "Error calling removeCrossProfileWidgetProvider", re);
        }
    }
    return false;
}

/**
 * Called by the profile owner of a managed profile to query providers from which packages are
 * available in the parent profile.
 *
 * @param admin Which {@link DeviceAdminReceiver} this request is associated with.
 * @return The white-listed package list.
 *
 * @see #addCrossProfileWidgetProvider(android.content.ComponentName, String)
 * @see #removeCrossProfileWidgetProvider(android.content.ComponentName, String)
 */
public List<String> getCrossProfileWidgetProviders(ComponentName admin) {
    if (mService != null) {
        try {
            List<String> providers = mService.getCrossProfileWidgetProviders(admin);
            if (providers != null) {
                return providers;
            }
        } catch (RemoteException re) {
            Log.w(TAG, "Error calling getCrossProfileWidgetProviders", re);
        }
    }
    return Collections.emptyList();
}

} // namespace Admin
} // namespace App
} // namespace Droid
} // namespace Elastos
