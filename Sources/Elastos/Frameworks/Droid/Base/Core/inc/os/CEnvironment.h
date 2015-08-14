
#ifndef __ELASTOS_DROID_OS_CENVIRONMENT_H__
#define __ELASTOS_DROID_OS_CENVIRONMENT_H__

#include "_CEnvironment.h"

using Elastos::IO::IFile;

namespace Elastos {
namespace Droid {
namespace Os {

/**
 * Provides access to environment variables.
 */
CarClass(CEnvironment)
{
public:
    /**
     * Gets the Android root directory.
     */
    CARAPI GetRootDirectory(
        /* [out] */ IFile** dir);

    /**
     * Gets the Android data directory.
     */
    CARAPI GetDataDirectory(
        /* [out] */ IFile** dir);

    /**
     * Gets the system directory available for secure storage.
     * If Encrypted File system is enabled, it returns an encrypted directory (/data/secure/system).
     * Otherwise, it returns the unencrypted /data/system directory.
     * @return File object representing the secure storage system directory.
     * @hide
     */
    CARAPI GetSystemSecureDirectory(
        /* [out] */ IFile** dir);

    /**
     * Gets the data directory for secure storage.
     * If Encrypted File system is enabled, it returns an encrypted directory (/data/secure).
     * Otherwise, it returns the unencrypted /data directory.
     * @return File object representing the data directory for secure storage.
     * @hide
     */
    CARAPI GetSecureDataDirectory(
        /* [out] */ IFile** dir);

    /**
     * Return the system directory for a user. This is for use by system services to store
     * files relating to the user. This directory will be automatically deleted when the user
     * is removed.
     *
     * @hide
     */
    CARAPI GetUserSystemDirectory(
        /* [in] */ Int32 userId,
        /* [out] */ IFile** dir);

    /**
     * Gets the Android external storage directory.  This directory may not
     * currently be accessible if it has been mounted by the user on their
     * computer, has been removed from the device, or some other problem has
     * happened.  You can determine its current state with
     * {@link #getExternalStorageState()}.
     *
     * <p><em>Note: don't be confused by the word "external" here.  This
     * directory can better be thought as media/shared storage.  It is a
     * filesystem that can hold a relatively large amount of data and that
     * is shared across all applications (does not enforce permissions).
     * Traditionally this is an SD card, but it may also be implemented as
     * built-in storage in a device that is distinct from the protected
     * internal storage and can be mounted as a filesystem on a computer.</em></p>
     *
     * <p>In devices with multiple "external" storage directories (such as
     * both secure app storage and mountable shared storage), this directory
     * represents the "primary" external storage that the user will interact
     * with.</p>
     *
     * <p>Applications should not directly use this top-level directory, in
     * order to avoid polluting the user's root namespace.  Any files that are
     * private to the application should be placed in a directory returned
     * by {@link android.content.Context#getExternalFilesDir
     * Context.getExternalFilesDir}, which the system will take care of deleting
     * if the application is uninstalled.  Other shared files should be placed
     * in one of the directories returned by
     * {@link #getExternalStoragePublicDirectory}.
     *
     * <p>Here is an example of typical code to monitor the state of
     * external storage:</p>
     *
     * {@sample development/samples/ApiDemos/src/com/example/android/apis/content/ExternalStorage.java
     * monitor_storage}
     *
     * @see #getExternalStorageState()
     * @see #isExternalStorageRemovable()
     */
    CARAPI GetExternalStorageDirectory(
        /* [out] */ IFile** dir);

    CARAPI GetLegacyExternalStorageDirectory(
        /* [out] */ IFile** dir);

    CARAPI GetEmulatedStorageSource(
        /* [in] */ Int32 userId,
        /* [out] */ IFile** dir);

    CARAPI GetEmulatedStorageObbSource(
        /* [out] */ IFile** dir);

    /**
     * Get a top-level public external storage directory for placing files of
     * a particular type.  This is where the user will typically place and
     * manage their own files, so you should be careful about what you put here
     * to ensure you don't erase their files or get in the way of their own
     * organization.
     *
     * <p>Here is an example of typical code to manipulate a picture on
     * the public external storage:</p>
     *
     * {@sample development/samples/ApiDemos/src/com/example/android/apis/content/ExternalStorage.java
     * public_picture}
     *
     * @param type The type of storage directory to return.  Should be one of
     * {@link #DIRECTORY_MUSIC}, {@link #DIRECTORY_PODCASTS},
     * {@link #DIRECTORY_RINGTONES}, {@link #DIRECTORY_ALARMS},
     * {@link #DIRECTORY_NOTIFICATIONS}, {@link #DIRECTORY_PICTURES},
     * {@link #DIRECTORY_MOVIES}, {@link #DIRECTORY_DOWNLOADS}, or
     * {@link #DIRECTORY_DCIM}.  May not be null.
     *
     * @return Returns the File path for the directory.  Note that this
     * directory may not yet exist, so you must make sure it exists before
     * using it such as with {@link File#mkdirs File.mkdirs()}.
     */
    CARAPI GetExternalStoragePublicDirectory(
        /* [in] */ const String& type,
        /* [out] */ IFile** dir);

    /**
     * Gets the Android Download/Cache content directory.
     */
    CARAPI GetDownloadCacheDirectory(
        /* [out] */ IFile** dir);

     /**
     * Gets the current state of the primary "external" storage device.
     *
     * <p>See {@link #getExternalStorageDirectory()} for more information.
     */
    CARAPI GetExternalStorageState(
        /* [out] */ String* state);

    /**
     * Returns whether the primary "external" storage device is removable.
     * If true is returned, this device is for example an SD card that the
     * user can remove.  If false is returned, the storage is built into
     * the device and can not be physically removed.
     *
     * <p>See {@link #getExternalStorageDirectory()} for more information.
     */
    CARAPI IsExternalStorageRemovable(
        /* [out] */ Boolean* isRemovable);

    /**
     * Returns whether the device has an external storage device which is
     * emulated. If true, the device does not have real external storage, and the directory
     * returned by {@link #getExternalStorageDirectory()} will be allocated using a portion of
     * the internal storage system.
     *
     * <p>Certain system services, such as the package manager, use this
     * to determine where to install an application.
     *
     * <p>Emulated external storage may also be encrypted - see
     * {@link android.app.admin.DevicePolicyManager#setStorageEncryption(
     * android.content.ComponentName, boolean)} for additional details.
     */
    CARAPI IsExternalStorageEmulated(
        /* [out] */ Boolean* isEmulated);
};

} // namespace Os
} // namespace Droid
} // namespace Elastos

#endif //__ELASTOS_DROID_OS_CENVIRONMENT_H__
