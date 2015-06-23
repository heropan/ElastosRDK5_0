
#ifndef __CUSERMANAGER_H__
#define __CUSERMANAGER_H__

#include "_CUserManager.h"

using Elastos::Droid::Content::IContext;
using Elastos::Droid::Content::Pm::IUserInfo;
using Elastos::Droid::Graphics::IBitmap;

namespace Elastos {
namespace Droid {
namespace Os {

CarClass(CUserManager)
{
public:
    CARAPI constructor(
        /* [in] */ IContext* context,
        /* [in] */ IIUserManager* service);

    /**
     * Returns whether the system supports multiple users.
     * @return true if multiple users can be created, false if it is a single user device.
     * @hide
     */
    static CARAPI_(Boolean) SupportsMultipleUsers();

    /**
     * Returns the user handle for the user that this application is running for.
     * @return the user handle of the user making this call.
     * @hide
     * */
    CARAPI GetUserHandle(
        /* [out] */ Int32* userHandle);

    /**
     * Returns the user name of the user making this call.  This call is only
     * available to applications on the system image; it requires the
     * MANAGE_USERS permission.
     * @return the user name
     */
    CARAPI GetUserName(
        /* [out] */ String* userName);

    /**
     * Used to determine whether the user making this call is subject to
     * teleportations.
     * @return whether the user making this call is a goat
     */
    CARAPI IsUserAGoat(
        /* [out] */ Boolean* result);

    /**
     * Return whether the given user is actively running.  This means that
     * the user is in the "started" state, not "stopped" -- it is currently
     * allowed to run code through scheduled alarms, receiving broadcasts,
     * etc.  A started user may be either the current foreground user or a
     * background user; the result here does not distinguish between the two.
     * @param user The user to retrieve the running state for.
     */
    CARAPI IsUserRunning(
        /* [in] */ IUserHandle* user,
        /* [out] */ Boolean* result);

    /**
     * Return whether the given user is actively running <em>or</em> stopping.
     * This is like {@link #isUserRunning(UserHandle)}, but will also return
     * true if the user had been running but is in the process of being stopped
     * (but is not yet fully stopped, and still running some code).
     * @param user The user to retrieve the running state for.
     */
    CARAPI IsUserRunningOrStopping(
        /* [in] */ IUserHandle* user,
        /* [out] */ Boolean* result);

    /**
     * Returns the UserInfo object describing a specific user.
     * Requires {@link android.Manifest.permission#MANAGE_USERS} permission.
     * @param userHandle the user handle of the user whose information is being requested.
     * @return the UserInfo object for a specific user.
     * @hide
     */
    CARAPI GetUserInfo(
        /* [in] */ Int32 userHandle,
        /* [out] */ IUserInfo** userInfo);

    /**
     * Return the serial number for a user.  This is a device-unique
     * number assigned to that user; if the user is deleted and then a new
     * user created, the new users will not be given the same serial number.
     * @param user The user whose serial number is to be retrieved.
     * @return The serial number of the given user; returns -1 if the
     * given UserHandle does not exist.
     * @see #getUserForSerialNumber(long)
     */
    CARAPI GetSerialNumberForUser(
        /* [in] */ IUserHandle* user,
        /* [out] */ Int64* number);

    /**
     * Return the user associated with a serial number previously
     * returned by {@link #getSerialNumberForUser(UserHandle)}.
     * @param serialNumber The serial number of the user that is being
     * retrieved.
     * @return Return the user associated with the serial number, or null
     * if there is not one.
     * @see #getSerialNumberForUser(UserHandle)
     */
    CARAPI GetUserForSerialNumber(
        /* [in] */ Int64 serialNumber,
        /* [out] */ IUserHandle** user);

    /**
     * Creates a user with the specified name and options.
     * Requires {@link android.Manifest.permission#MANAGE_USERS} permission.
     *
     * @param name the user's name
     * @param flags flags that identify the type of user and other properties.
     * @see UserInfo
     *
     * @return the UserInfo object for the created user, or null if the user could not be created.
     * @hide
     */
    CARAPI CreateUser(
        /* [in] */ const String& name,
        /* [in] */ Int32 flags,
        /* [out] */ IUserInfo** userInfo);

    /**
     * Return the number of users currently created on the device.
     */
    CARAPI GetUserCount(
        /* [out] */ Int32* count);

    /**
     * Returns information for all users on this device.
     * Requires {@link android.Manifest.permission#MANAGE_USERS} permission.
     * @return the list of users that were created.
     * @hide
     */
    CARAPI GetUsers(
        /* [out] */ IObjectContainer** users);

    /**
     * Returns information for all users on this device.
     * Requires {@link android.Manifest.permission#MANAGE_USERS} permission.
     * @param excludeDying specify if the list should exclude users being removed.
     * @return the list of users that were created.
     * @hide
     */
    CARAPI GetUsers(
        /* [in] */ Boolean excludeDying,
        /* [out] */ IObjectContainer** users);

    /**
     * Removes a user and all associated data.
     * Requires {@link android.Manifest.permission#MANAGE_USERS} permission.
     * @param userHandle the integer handle of the user, where 0 is the primary user.
     * @hide
     */
    CARAPI RemoveUser(
        /* [in] */ Int32 userHandle,
        /* [out] */ Boolean* result);

   /**
     * Updates the user's name.
     * Requires {@link android.Manifest.permission#MANAGE_USERS} permission.
     *
     * @param userHandle the user's integer handle
     * @param name the new name for the user
     * @hide
     */
    CARAPI SetUserName(
        /* [in] */ Int32 userHandle,
        /* [in] */ const String& name);

    /**
     * Sets the user's photo.
     * @param userHandle the user for whom to change the photo.
     * @param icon the bitmap to set as the photo.
     * @hide
     */
    CARAPI SetUserIcon(
        /* [in] */ Int32 userHandle,
        /* [in] */ IBitmap* icon);

    /**
     * Returns a file descriptor for the user's photo. PNG data can be read from this file.
     * @param userHandle the user whose photo we want to read.
     * @return a {@link Bitmap} of the user's photo, or null if there's no photo.
     * @hide
     */
    CARAPI GetUserIcon(
        /* [in] */ Int32 userHandle,
        /* [out] */ IBitmap** icon);

    /**
     * Enable or disable the use of a guest account. If disabled, the existing guest account
     * will be wiped.
     * Requires {@link android.Manifest.permission#MANAGE_USERS} permission.
     * @param enable whether to enable a guest account.
     * @hide
     */
    CARAPI SetGuestEnabled(
        /* [in] */ Boolean enable);

    /**
     * Checks if a guest user is enabled for this device.
     * Requires {@link android.Manifest.permission#MANAGE_USERS} permission.
     * @return whether a guest user is enabled
     * @hide
     */
    CARAPI IsGuestEnabled(
        /* [out] */ Boolean* result);

    /**
     * Wipes all the data for a user, but doesn't remove the user.
     * Requires {@link android.Manifest.permission#MANAGE_USERS} permission.
     * @param userHandle
     * @hide
     */
    CARAPI WipeUser(
        /* [in] */ Int32 userHandle);

    /**
     * Returns the maximum number of users that can be created on this device. A return value
     * of 1 means that it is a single user device.
     * @hide
     * @return a value greater than or equal to 1
     */
    static CARAPI_(Int32) GetMaxSupportedUsers();

    /**
     * Returns a serial number on this device for a given userHandle. User handles can be recycled
     * when deleting and creating users, but serial numbers are not reused until the device is wiped.
     * @param userHandle
     * @return a serial number associated with that user, or -1 if the userHandle is not valid.
     * @hide
     */
    CARAPI GetUserSerialNumber(
        /* [in] */ Int32 userHandle,
        /* [out] */ Int32* number);

    /**
     * Returns a userHandle on this device for a given user serial number. User handles can be
     * recycled when deleting and creating users, but serial numbers are not reused until the device
     * is wiped.
     * @param userSerialNumber
     * @return the userHandle associated with that user serial number, or -1 if the serial number
     * is not valid.
     * @hide
     */
    CARAPI GetUserHandle(
        /* [in] */ Int32 userSerialNumber,
        /* [out] */ Int32* userHandle);

private:
    static String TAG;
    AutoPtr<IIUserManager> mService;
    AutoPtr<IContext> mContext;
};

} // namespace Os
} // namespace Droid
} // namespace Elastos

#endif //__CUSERMANAGER_H__

