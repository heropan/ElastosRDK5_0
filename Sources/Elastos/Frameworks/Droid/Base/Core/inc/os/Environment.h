#ifndef __ENVIRONMENT_H__
#define __ENVIRONMENT_H__

#include "Elastos.Droid.Core_server.h"
#include <elastos/Mutex.h>

using Elastos::IO::IFile;
using Elastos::Core::Threading::Mutex;
using Elastos::Droid::Os::Storage::IMountService;
using Elastos::Droid::Os::Storage::IStorageVolume;

namespace Elastos {
namespace Droid {
namespace Os {

/**
 * Provides access to environment variables.
 */
class Environment
{
public:
    /**
     * Gets the Android root directory.
     */
    static CARAPI_(AutoPtr<IFile>) GetRootDirectory();

    /**
     * Gets the system directory available for secure storage.
     * If Encrypted File system is enabled, it returns an encrypted directory (/data/secure/system).
     * Otherwise, it returns the unencrypted /data/system directory.
     * @return File object representing the secure storage system directory.
     * @hide
     */
    static CARAPI_(AutoPtr<IFile>) GetSystemSecureDirectory();

    /**
     * Gets the data directory for secure storage.
     * If Encrypted File system is enabled, it returns an encrypted directory (/data/secure).
     * Otherwise, it returns the unencrypted /data directory.
     * @return File object representing the data directory for secure storage.
     * @hide
     */
    static CARAPI_(AutoPtr<IFile>) GetSecureDataDirectory();

    /**
     * Return the system directory for a user. This is for use by system services to store
     * files relating to the user. This directory will be automatically deleted when the user
     * is removed.
     *
     * @hide
     */
    static CARAPI_(AutoPtr<IFile>) GetUserSystemDirectory(
        /* [in] */ Int32 userId);

    static CARAPI_(Boolean) IsEncryptedFilesystemEnabled();

    /**
     * Gets the Android data directory.
     */
    static CARAPI_(AutoPtr<IFile>) GetDataDirectory();

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
    static CARAPI_(AutoPtr<IFile>) GetExternalStorageDirectory();

    static CARAPI_(AutoPtr<IFile>) GetLegacyExternalStorageDirectory();

    static CARAPI_(AutoPtr<IFile>) GetEmulatedStorageSource(
        /* [in] */ Int32 userId);

    static CARAPI_(AutoPtr<IFile>) GetEmulatedStorageObbSource();

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
    static CARAPI_(AutoPtr<IFile>) GetExternalStoragePublicDirectory(
        /* [in] */ const String& type);

    /**
     * Returns the path for android-specific data on the SD card.
     * @hide
     */
    static CARAPI_(AutoPtr<IFile>) GetExternalStorageElastosDataDir();

    /**
     * Generates the raw path to an application's data
     * @hide
     */
    static CARAPI_(AutoPtr<IFile>) GetExternalStorageAppDataDirectory(
        /* [in] */ const String& capsuleName);

    /**
     * Generates the raw path to an application's media
     * @hide
     */
    static CARAPI_(AutoPtr<IFile>) GetExternalStorageAppMediaDirectory(
        /* [in] */ const String& capsuleName);

    /**
     * Generates the raw path to an application's OBB files
     * @hide
     */
    static CARAPI_(AutoPtr<IFile>) GetExternalStorageAppObbDirectory(
        /* [in] */ const String& capsuleName);

    /**
     * Generates the path to an application's files.
     * @hide
     */
    static CARAPI_(AutoPtr<IFile>) GetExternalStorageAppFilesDirectory(
        /* [in] */ const String& capsuleName);

    /**
     * Generates the path to an application's cache.
     * @hide
     */
    static CARAPI_(AutoPtr<IFile>) GetExternalStorageAppCacheDirectory(
        /* [in] */ const String& capsuleName);

    /**
     * Gets the Android Download/Cache content directory.
     */
    static CARAPI_(AutoPtr<IFile>) GetDownloadCacheDirectory();

    /**
     * Gets the current state of the primary "external" storage device.
     *
     * <p>See {@link #getExternalStorageDirectory()} for more information.
     */
    static CARAPI_(String) GetExternalStorageState();

    /**
     * Returns whether the primary "external" storage device is removable.
     * If true is returned, this device is for example an SD card that the
     * user can remove.  If false is returned, the storage is built into
     * the device and can not be physically removed.
     *
     * <p>See {@link #getExternalStorageDirectory()} for more information.
     */
    static CARAPI_(Boolean) IsExternalStorageRemovable();

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
    static CARAPI_(Boolean) IsExternalStorageEmulated();

private:
    static CARAPI_(AutoPtr<IFile>) GetDirectory(
        /* [in] */ const String& variableName,
        /* [in] */ const String& defaultPath);

    static CARAPI_(void) Init();

    static CARAPI_(AutoPtr<IStorageVolume>) GetPrimaryVolume();

public:

    class UserEnvironment
    {
    public:
        CARAPI Init(
            /* [in] */ Int32 userId);

        CARAPI_(AutoPtr<IFile>) GetExternalStorageDirectory();

        CARAPI_(AutoPtr<IFile>) GetExternalStorageObbDirectory();

        CARAPI_(AutoPtr<IFile>) GetExternalStoragePublicDirectory(
            /* [in] */ const String& type);

        CARAPI_(AutoPtr<IFile>) GetExternalStorageAndroidDataDir();

        CARAPI_(AutoPtr<IFile>) GetExternalStorageAppDataDirectory(
            /* [in] */ const String& packageName);

        CARAPI_(AutoPtr<IFile>) GetExternalStorageAppMediaDirectory(
            /* [in] */ const String& packageName);

        CARAPI_(AutoPtr<IFile>) GetExternalStorageAppObbDirectory(
            /* [in] */ const String& packageName);

        CARAPI_(AutoPtr<IFile>) GetExternalStorageAppFilesDirectory(
            /* [in] */ const String& packageName);

        CARAPI_(AutoPtr<IFile>) GetExternalStorageAppCacheDirectory(
            /* [in] */ const String& packageName);

        CARAPI_(AutoPtr<IFile>) GetMediaStorageDirectory();

    private:
        static CARAPI_(AutoPtr<IFile>) BuildPath(
            /* [in] */ IFile* base,
            /* [in] */ const ArrayOf<String>& segments);

        static CARAPI_(AutoPtr<IFile>) BuildPath(
            /* [in] */ IFile* base,
            /* [in] */ const String& segment);

    private:
        AutoPtr<IFile> mExternalStorage;
        AutoPtr<IFile> mExternalStorageAndroidData;
        AutoPtr<IFile> mExternalStorageAndroidMedia;
        AutoPtr<IFile> mExternalStorageAndroidObb;
        AutoPtr<IFile> mMediaStorage;
    };
private:
    static const String ENV_EXTERNAL_STORAGE;
    static const String ENV_EMULATED_STORAGE_SOURCE;

    static AutoPtr<IFile> ROOT_DIRECTORY;

    static const String SYSTEM_PROPERTY_EFS_ENABLED;

    static AutoPtr<IMountService> mMntSvc;

    static AutoPtr<IStorageVolume> sPrimaryVolume;
    static volatile Boolean sIsStorageVolumeInited;

    static AutoPtr<IFile> DATA_DIRECTORY;

    static Mutex sLock;

    /**
     * @hide
     */
    static AutoPtr<IFile> SECURE_DATA_DIRECTORY;
    static AutoPtr<IFile> EXTERNAL_STORAGE_DIRECTORY;
    static AutoPtr<IFile> EXTERNAL_STORAGE_ELASTOS_DATA_DIRECTORY;
    static AutoPtr<IFile> EXTERNAL_STORAGE_ELASTOS_MEDIA_DIRECTORY;
    static AutoPtr<IFile> EXTERNAL_STORAGE_ELASTOS_OBB_DIRECTORY;
    static AutoPtr<IFile> DOWNLOAD_CACHE_DIRECTORY;

    static Boolean sIsInitilized;
};

} // namespace Os
} // namespace Droid
} // namespace Elastos

#endif //__ENVIRONMENT_H__
