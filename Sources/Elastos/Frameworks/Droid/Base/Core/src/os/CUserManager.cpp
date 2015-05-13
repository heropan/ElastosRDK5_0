
#include "ext/frameworkdef.h"
#include "os/CUserManager.h"
#include "os/CUserHandle.h"
#include "app/ActivityManagerNative.h"

using Elastos::Droid::App::ActivityManagerNative;

namespace Elastos {
namespace Droid {
namespace Os {

String CUserManager::TAG("CUserManager");

ECode CUserManager::constructor(
    /* [in] */ IContext* context,
    /* [in] */ IIUserManager* service)
{
    mService = service;
    mContext = context;
    return NOERROR;
}

Boolean CUserManager::SupportsMultipleUsers()
{
    return GetMaxSupportedUsers() > 1;
}

ECode CUserManager::GetUserHandle(
    /* [out] */ Int32* userHandle)
{
    VALIDATE_NOT_NULL(userHandle);
    *userHandle = CUserHandle::GetMyUserId();
    return NOERROR;
}

ECode CUserManager::GetUserName(
    /* [out] */ String* userName)
{
    VALIDATE_NOT_NULL(userName);
    // try {
    Int32 user;
    GetUserHandle(&user);
    AutoPtr<IUserInfo> userInfo;
    ECode ec = mService->GetUserInfo(user, (IUserInfo**)&userInfo);
    if (FAILED(ec)) {
        *userName = "";
        return NOERROR;
    }
    else {
        return userInfo->GetName(userName);
    }
    // } catch (RemoteException re) {
    //     Log.w(TAG, "Could not get user name", re);
    //     return "";
    // }
}

ECode CUserManager::IsUserAGoat(
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
    *result = FALSE;
    return NOERROR;
}

ECode CUserManager::IsUserRunning(
    /* [in] */ IUserHandle* user,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
    // try {
    Int32 id;
    user->GetIdentifier(&id);
    ECode ec = ActivityManagerNative::GetDefault()->IsUserRunning(
            id, FALSE, result);
    if (FAILED(ec)) *result = FALSE;
    return NOERROR;
    // } catch (RemoteException e) {
    //     return false;
    // }
}

ECode CUserManager::IsUserRunningOrStopping(
    /* [in] */ IUserHandle* user,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
    // try {
    Int32 id;
    user->GetIdentifier(&id);
    ECode ec = ActivityManagerNative::GetDefault()->IsUserRunning(
            id, TRUE, result);
    if (FAILED(ec)) *result = FALSE;
    return NOERROR;
    // } catch (RemoteException e) {
    //     return false;
    // }
}

ECode CUserManager::GetUserInfo(
    /* [in] */ Int32 userHandle,
    /* [out] */ IUserInfo** userInfo)
{
    VALIDATE_NOT_NULL(userInfo);
    // try {
    ECode ec = mService->GetUserInfo(userHandle, userInfo);
    if (FAILED(ec)) *userInfo = NULL;
    return NOERROR;
    // } catch (RemoteException re) {
    //     Log.w(TAG, "Could not get user info", re);
    //     return null;
    // }
}

ECode CUserManager::GetSerialNumberForUser(
    /* [in] */ IUserHandle* user,
    /* [out] */ Int64* number)
{
    VALIDATE_NOT_NULL(number);
    Int32 id;
    user->GetIdentifier(&id);
    Int32 n;
    FAIL_RETURN(GetUserSerialNumber(id, &n));
    *number = n;
    return NOERROR;
}

ECode CUserManager::GetUserForSerialNumber(
    /* [in] */ Int64 serialNumber,
    /* [out] */ IUserHandle** user)
{
    VALIDATE_NOT_NULL(user);
    *user = NULL;

    Int32 ident;
    FAIL_RETURN(GetUserHandleEx((Int32)serialNumber, &ident));
    if (ident >= 0) {
        return CUserHandle::New(ident, user);
    }

    return NOERROR;
}

ECode CUserManager::CreateUser(
    /* [in] */ const String& name,
    /* [in] */ Int32 flags,
    /* [out] */ IUserInfo** userInfo)
{
    VALIDATE_NOT_NULL(userInfo);
    // try {
    ECode ec = mService->CreateUser(name, flags, userInfo);
    if (FAILED(ec)) *userInfo = NULL;
    return NOERROR;
    // } catch (RemoteException re) {
    //     Log.w(TAG, "Could not create a user", re);
    //     return null;
    // }
}

ECode CUserManager::GetUserCount(
    /* [out] */ Int32* count)
{
    VALIDATE_NOT_NULL(count);
    AutoPtr<IObjectContainer> users;
    GetUsers((IObjectContainer**)&users);
    if (users != NULL) {
        users->GetObjectCount(count);
    }
    else *count = 1;
    return NOERROR;
}

ECode CUserManager::GetUsers(
    /* [out] */ IObjectContainer** users)
{
    VALIDATE_NOT_NULL(users);
    // try {
    ECode ec = mService->GetUsers(FALSE, users);
    if (FAILED(ec)) *users = NULL;
    return NOERROR;
    // } catch (RemoteException re) {
    //     Log.w(TAG, "Could not get user list", re);
    //     return null;
    // }
}

ECode CUserManager::GetUsersEx(
    /* [in] */ Boolean excludeDying,
    /* [out] */ IObjectContainer** users)
{
    VALIDATE_NOT_NULL(users);
    // try {
    ECode ec = mService->GetUsers(excludeDying, users);
    if (FAILED(ec)) *users = NULL;
    return NOERROR;
    // } catch (RemoteException re) {
    //     Log.w(TAG, "Could not get user list", re);
    //     return null;
    // }
}

ECode CUserManager::RemoveUser(
    /* [in] */ Int32 userHandle,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
    // try {
    ECode ec = mService->RemoveUser(userHandle, result);
    if (FAILED(ec)) *result = FALSE;
    return NOERROR;
    // } catch (RemoteException re) {
    //     Log.w(TAG, "Could not remove user ", re);
    //     return false;
    // }
}

ECode CUserManager::SetUserName(
    /* [in] */ Int32 userHandle,
    /* [in] */ const String& name)
{
    // try {
    return mService->SetUserName(userHandle, name);
    // } catch (RemoteException re) {
    //     Log.w(TAG, "Could not set the user name ", re);
    // }
}

ECode CUserManager::SetUserIcon(
    /* [in] */ Int32 userHandle,
    /* [in] */ IBitmap* icon)
{
    // try {
    return mService->SetUserIcon(userHandle, icon);
    // } catch (RemoteException re) {
    //     Log.w(TAG, "Could not set the user icon ", re);
    // }
}

ECode CUserManager::GetUserIcon(
    /* [in] */ Int32 userHandle,
    /* [out] */ IBitmap** icon)
{
    VALIDATE_NOT_NULL(icon);
    // try {
    ECode ec = mService->GetUserIcon(userHandle, icon);
    if (FAILED(ec)) *icon = NULL;
    return NOERROR;
    // } catch (RemoteException re) {
    //     Log.w(TAG, "Could not get the user icon ", re);
    //     return null;
    // }
}

ECode CUserManager::SetGuestEnabled(
    /* [in] */ Boolean enable)
{
    // try {
    return mService->SetGuestEnabled(enable);
    // } catch (RemoteException re) {
    //     Log.w(TAG, "Could not change guest account availability to " + enable);
    // }
}

ECode CUserManager::IsGuestEnabled(
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
    // try {
    ECode ec = mService->IsGuestEnabled(result);
    if (FAILED(ec)) *result = FALSE;
    return NOERROR;
    // } catch (RemoteException re) {
    //     Log.w(TAG, "Could not retrieve guest enabled state");
    //     return false;
    // }
}

ECode CUserManager::WipeUser(
    /* [in] */ Int32 userHandle)
{
    // try {
    return mService->WipeUser(userHandle);
    // } catch (RemoteException re) {
    //     Log.w(TAG, "Could not wipe user " + userHandle);
    // }
}

/**
 * Returns the maximum number of users that can be created on this device. A return value
 * of 1 means that it is a single user device.
 * @hide
 * @return a value greater than or equal to 1
 */
Int32 CUserManager::GetMaxSupportedUsers()
{
    // Don't allow multiple users on certain builds
    // if (android.os.Build.ID.startsWith("JVP")) return 1;
    // return SystemProperties.getInt("fw.max_users",
    //         Resources.getSystem().getInteger(R.integer.config_multiuserMaximumUsers));
    assert(0);
    return 1;
}

ECode CUserManager::GetUserSerialNumber(
    /* [in] */ Int32 userHandle,
    /* [out] */ Int32* number)
{
    VALIDATE_NOT_NULL(number);
    // try {
    ECode ec = mService->GetUserSerialNumber(userHandle, number);
    if (FAILED(ec)) *number = -1;
    return NOERROR;
    // } catch (RemoteException re) {
    //     Log.w(TAG, "Could not get serial number for user " + userHandle);
    // }
    // return -1;
}

ECode CUserManager::GetUserHandleEx(
    /* [in] */ Int32 userSerialNumber,
    /* [out] */ Int32* userHandle)
{
    VALIDATE_NOT_NULL(userHandle);
    // try {
    ECode ec = mService->GetUserHandle(userSerialNumber, userHandle);
    if (FAILED(ec)) *userHandle = -1;
    return NOERROR;
    // } catch (RemoteException re) {
    //     Log.w(TAG, "Could not get userHandle for user " + userSerialNumber);
    // }
    // return -1;
}

} // namespace Os
} // namespace Droid
} // namespace Elastos
