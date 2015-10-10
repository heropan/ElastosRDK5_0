
#ifndef __ELASTOS_DROID_APP_APPLICATION_H__
#define __ELASTOS_DROID_APP_APPLICATION_H__

#include "elastos/droid/ext/frameworkext.h"
#include <elastos/utility/etl/List.h>

using Elastos::Core::ICharSequence;
using Elastos::Core::IClassLoader;
using Elastos::IO::IFile;
using Elastos::IO::IInputStream;
using Elastos::IO::IFileInputStream;
using Elastos::IO::IFileOutputStream;
using Elastos::Utility::Etl::List;
using Elastos::Droid::App::IActivity;
using Elastos::Droid::Content::IContext;
using Elastos::Droid::Content::Res::IConfiguration;
using Elastos::Droid::Content::IContentResolver;
using Elastos::Droid::Content::IComponentCallbacks;
using Elastos::Droid::Content::IComponentCallbacks2;
using Elastos::Droid::Content::ISharedPreferences;
using Elastos::Droid::Content::IIntent;
using Elastos::Droid::Content::IIntentSender;
using Elastos::Droid::Content::IBroadcastReceiver;
using Elastos::Droid::Content::IIntentFilter;
using Elastos::Droid::Content::IComponentName;
using Elastos::Droid::Content::IServiceConnection;
using Elastos::Droid::Content::Pm::IApplicationInfo;
using Elastos::Droid::Content::Pm::IPackageManager;
using Elastos::Droid::Content::Res::IAssetManager;
using Elastos::Droid::Content::Res::IResources;
using Elastos::Droid::Content::Res::IResourcesTheme;
using Elastos::Droid::Content::Res::ITypedArray;
using Elastos::Droid::Database::IDatabaseErrorHandler;
using Elastos::Droid::Database::Sqlite::ISQLiteDatabase;
using Elastos::Droid::Database::Sqlite::ISQLiteDatabaseCursorFactory;
using Elastos::Droid::Graphics::IBitmap;
using Elastos::Droid::Graphics::Drawable::IDrawable;
using Elastos::Droid::Net::IUri;
using Elastos::Droid::Os::IBundle;
using Elastos::Droid::Os::IHandler;
using Elastos::Droid::Os::ILooper;
using Elastos::Droid::Os::IUserHandle;
using Elastos::Droid::View::IDisplay;
using Elastos::Droid::View::ICompatibilityInfoHolder;
using Elastos::Droid::Utility::IAttributeSet;

namespace Elastos {
namespace Droid {
namespace App {

class Application
    : public ElRefBase
    , public IObject
    , public IWeakReferenceSource
    , public IApplication
    , public IComponentCallbacks2
{
public:
    Application();

    virtual ~Application();

    virtual CARAPI Initialize();

    CAR_INTERFACE_DECL()

    virtual CARAPI Aggregate(
        /* [in] */ AggrType aggrType,
        /* [in] */ PInterface pObject);

    virtual CARAPI GetDomain(
        /* [out] */ PInterface *ppObject);

    virtual CARAPI GetClassID(
        /* [out] */ ClassID *pCLSID);

    virtual CARAPI Equals(
        /* [in] */ IInterface* other,
        /* [out] */ Boolean * result);

    virtual CARAPI GetHashCode(
        /* [out] */ Int32* hash);

    virtual CARAPI ToString(
        /* [out] */ String* str);

    virtual CARAPI GetWeakReference(
        /* [out] */ IWeakReference** weakReference);

    /**
     * Called when the application is starting, before any other application
     * objects have been created.  Implementations should be as quick as
     * possible (for example using lazy initialization of state) since the time
     * spent in this function directly impacts the performance of starting the
     * first activity, service, or receiver in a process.
     * If you override this method, be sure to call super.onCreate().
     */
    virtual CARAPI OnCreate();

    /**
     * This method is for use in emulated process environments.  It will
     * never be called on a production Android device, where processes are
     * removed by simply killing them; no user code (including this callback)
     * is executed when doing so.
     */
    virtual CARAPI OnTerminate();

    virtual CARAPI OnConfigurationChanged(
        /* [in] */ IConfiguration* newConfig);

    virtual CARAPI OnLowMemory();

    virtual CARAPI OnTrimMemory(
        /* [in] */ Int32 level);

    /**
     * Add a new {@sa ComponentCallbacks} to the base application of the
     * Context, which will be called at the same times as the ComponentCallbacks
     * methods of activities and other components are called.  Note that you
     * <em>must</em> be sure to use {@sa #unregisterComponentCallbacks} when
     * appropriate in the future; this will not be removed for you.
     *
     * @param callback The interface to call.  This can be either a
     * {@sa ComponentCallbacks} or {@sa ComponentCallbacks2} interface.
     */
    virtual CARAPI RegisterComponentCallbacks(
        /* [in] */ IComponentCallbacks* componentCallback);

    /**
     * Remove a {@sa ComponentCallbacks} objec that was previously registered
     * with {@sa #registerComponentCallbacks(ComponentCallbacks)}.
     */
    virtual CARAPI UnregisterComponentCallbacks(
        /* [in] */ IComponentCallbacks* componentCallback);

    virtual CARAPI RegisterActivityLifecycleCallbacks(
        /* [in] */ IActivityLifecycleCallbacks* cb);

    virtual CARAPI UnregisterActivityLifecycleCallbacks(
        /* [in] */ IActivityLifecycleCallbacks* cb);

    /**
     * @hide
     */
    /* package */
    virtual CARAPI Attach(
        /* [in] */ IContext* ctx);

    /* package */
    virtual CARAPI DispatchActivityCreated(
        /* [in] */ IActivity* activity,
        /* [in] */ IBundle* b);

    /* package */
    virtual CARAPI DispatchActivityStarted(
        /* [in] */ IActivity* activity);

    /* package */
    virtual CARAPI DispatchActivityResumed(
        /* [in] */ IActivity* activity);

    /* package */
    virtual CARAPI DispatchActivityPaused(
        /* [in] */ IActivity* activity);

    /* package */
    virtual CARAPI DispatchActivityStopped(
        /* [in] */ IActivity* activity);

    /* package */
    virtual CARAPI DispatchActivitySaveInstanceState(
        /* [in] */ IActivity* activity,
        /* [in] */ IBundle* b);

    /* package */
    virtual CARAPI DispatchActivityDestroyed(
        /* [in] */ IActivity* activity);

    /**
     * @return the base context as set by the constructor or setBaseContext
     */
    virtual CARAPI GetBaseContext(
        /* [out] */ IContext** context);

    /** Return an AssetManager instance for your application's package. */
    virtual CARAPI GetAssets(
        /* [out] */ IAssetManager** assetManager);

    /** Return a Resources instance for your application's package. */
    virtual CARAPI GetResources(
        /* [out] */ IResources** resources);

    /** Return PackageManager instance to find global package information. */
    virtual CARAPI GetPackageManager(
        /* [out] */ IPackageManager** packageManager);

    /** Return a ContentResolver instance for your application's package. */
    virtual CARAPI GetContentResolver(
        /* [out] */ IContentResolver** resolver);

    /**
     * Return the Looper for the main thread of the current process.  This is
     * the thread used to dispatch calls to application components (activities,
     * services, etc).
     */
    virtual CARAPI GetMainLooper(
        /* [out] */ ILooper** looper);

    /**
     * Return the context of the single, global Application object of the
     * current process.  This generally should only be used if you need a
     * Context whose lifecycle is separate from the current context, that is
     * tied to the lifetime of the process rather than the current component.
     *
     * <p>Consider for example how this interacts with
     * {@sa #registerReceiver(BroadcastReceiver, IntentFilter)}:
     * <ul>
     * <li> <p>If used from an Activity context, the receiver is being registered
     * within that activity.  This means that you are expected to unregister
     * before the activity is done being destroyed; in fact if you do not do
     * so, the framework will clean up your leaked registration as it removes
     * the activity and log an error.  Thus, if you use the Activity context
     * to register a receiver that is static (global to the process, not
     * associated with an Activity instance) then that registration will be
     * removed on you at whatever point the activity you used is destroyed.
     * <li> <p>If used from the Context returned here, the receiver is being
     * registered with the global state associated with your application.  Thus
     * it will never be unregistered for you.  This is necessary if the receiver
     * is associated with static data, not a particular component.  However
     * using the ApplicationContext elsewhere can easily lead to serious leaks
     * if you forget to unregister, unbind, etc.
     * </ul>
     */
    virtual CARAPI GetApplicationContext(
        /* [out] */ IContext** ctx);

    /**
     * Return a localized, styled CharSequence from the application's package's
     * default string table.
     *
     * @param resId Resource id for the CharSequence text
     */
    virtual CARAPI GetText(
        /* [in] */ Int32 resId,
        /* [out] */ ICharSequence** text);

    /**
     * Return a localized string from the application's package's
     * default string table.
     *
     * @param resId Resource id for the string
     */
    virtual CARAPI GetString(
        /* [in] */ Int32 resId,
        /* [out] */ String* str);

    /**
     * Return a localized formatted string from the application's package's
     * default string table, substituting the format arguments as defined in
     * {@sa java.util.Formatter} and {@sa java.lang.String#format}.
     *
     * @param resId Resource id for the format string
     * @param formatArgs The format arguments that will be used for substitution.
     */
    virtual CARAPI GetString(
        /* [in] */ Int32 resId,
        /* [in] */ ArrayOf<IInterface*>* formatArgs,
        /* [out] */ String* str);

    /**
     * Set the base theme for this context.  Note that this should be called
     * before any views are instantiated in the Context (for example before
     * calling {@sa android.app.Activity#setContentView} or
     * {@sa android.view.LayoutInflater#inflate}).
     *
     * @param resid The style resource describing the theme.
     */
    virtual CARAPI SetTheme(
        /* [in] */ Int32 resId);

    /**
     * @hide
     * Needed for some internal implementation...  not public because
     * you can't assume this actually means anything.
     */
    virtual CARAPI GetThemeResId(
        /* [out] */ Int32* resId);

    /**
     * Return the Theme object associated with this Context.
     */
    virtual CARAPI GetTheme(
        /* [out] */ IResourcesTheme** theme);

    /**
     * Retrieve styled attribute information in this Context's theme.  See
     * {@sa Resources.Theme#obtainStyledAttributes(int[])}
     * for more information.
     *
     * @see Resources.Theme#obtainStyledAttributes(int[])
     */
    virtual CARAPI ObtainStyledAttributes(
        /* [in] */ ArrayOf<Int32>* attrs,
        /* [out] */ ITypedArray** styles);

    /**
     * Retrieve styled attribute information in this Context's theme.  See
     * {@sa Resources.Theme#obtainStyledAttributes(int, int[])}
     * for more information.
     *
     * @see Resources.Theme#obtainStyledAttributes(int, int[])
     */
    virtual CARAPI ObtainStyledAttributes(
        /* [in] */ Int32 resid,
        /* [in] */ ArrayOf<Int32>* attrs,
        /* [out] */ ITypedArray** styles);

    /**
     * Retrieve styled attribute information in this Context's theme.  See
     * {@sa Resources.Theme#obtainStyledAttributes(AttributeSet, int[], int, int)}
     * for more information.
     *
     * @see Resources.Theme#obtainStyledAttributes(AttributeSet, int[], int, int)
     */
    virtual CARAPI ObtainStyledAttributes(
        /* [in] */ IAttributeSet* set,
        /* [in] */ ArrayOf<Int32>* attrs,
        /* [out] */ ITypedArray** styles);

    /**
     * Retrieve styled attribute information in this Context's theme.  See
     * {@sa Resources.Theme#obtainStyledAttributes(AttributeSet, int[], int, int)}
     * for more information.
     *
     * @see Resources.Theme#obtainStyledAttributes(AttributeSet, int[], int, int)
     */
    virtual CARAPI ObtainStyledAttributes(
        /* [in] */ IAttributeSet* set,
        /* [in] */ ArrayOf<Int32>* attrs,
        /* [in] */ Int32 defStyleAttr,
        /* [in] */ Int32 defStyleRes,
        /* [out] */ ITypedArray** styles);

    /**
     * Return a class loader you can use to retrieve classes in this package.
     */
    virtual CARAPI GetClassLoader(
        /* [out] */ IClassLoader** loader);

    /** Return the name of this application's package. */
    virtual CARAPI GetPackageName(
        /* [out] */ String* packageName);

    /** Return the full application info for this context's package. */
    virtual CARAPI GetApplicationInfo(
        /* [out] */ IApplicationInfo** info);

    /**
     * Return the full path to this context's primary Android package.
     * The Android package is a ZIP file which contains the application's
     * primary resources.
     *
     * <p>Note: this is not generally useful for applications, since they should
     * not be directly accessing the file system.
     *
     * @return String Path to the resources.
     */
    virtual CARAPI GetPackageResourcePath(
        /* [out] */ String* path);

    /**
     * Return the full path to this context's primary Android package.
     * The Android package is a ZIP file which contains application's
     * primary code and assets.
     *
     * <p>Note: this is not generally useful for applications, since they should
     * not be directly accessing the file system.
     *
     * @return String Path to the code and assets.
     */
    virtual CARAPI GetPackageCodePath(
        /* [out] */ String* codePath);

    /**
     * {@hide}
     * Return the full path to the shared prefs file for the given prefs group name.
     *
     * <p>Note: this is not generally useful for applications, since they should
     * not be directly accessing the file system.
     */
    virtual CARAPI GetSharedPrefsFile(
        /* [in] */ const String& name,
        /* [out] */ IFile** file);

    /**
     * Retrieve and hold the contents of the preferences file 'name', returning
     * a SharedPreferences through which you can retrieve and modify its
     * values.  Only one instance of the SharedPreferences object is returned
     * to any callers for the same name, meaning they will see each other's
     * edits as soon as they are made.
     *
     * @param name Desired preferences file. If a preferences file by this name
     * does not exist, it will be created when you retrieve an
     * editor (SharedPreferences.edit()) and then commit changes (Editor.commit()).
     * @param mode Operating mode.  Use 0 or {@sa #MODE_PRIVATE} for the
     * default operation, {@sa #MODE_WORLD_READABLE}
     * and {@sa #MODE_WORLD_WRITEABLE} to control permissions.  The bit
     * {@sa #MODE_MULTI_PROCESS} can also be used if multiple processes
     * are mutating the same SharedPreferences file.  {@sa #MODE_MULTI_PROCESS}
     * is always on in apps targetting Gingerbread (Android 2.3) and below, and
     * off by default in later versions.
     *
     * @return Returns the single SharedPreferences instance that can be used
     *         to retrieve and modify the preference values.
     *
     * @see #MODE_PRIVATE
     * @see #MODE_WORLD_READABLE
     * @see #MODE_WORLD_WRITEABLE
     * @see #MODE_MULTI_PROCESS
     */
    virtual CARAPI GetSharedPreferences(
        /* [in] */ const String& name,
        /* [in] */ Int32 mode,
        /* [out] */ ISharedPreferences** prefs);

    /**
     * Open a private file associated with this Context's application package
     * for reading.
     *
     * @param name The name of the file to open; can not contain path
     *             separators.
     *
     * @return FileInputStream Resulting input stream.
     *
     * @see #openFileOutput
     * @see #fileList
     * @see #deleteFile
     * @see java.io.FileInputStream#FileInputStream(String)
     */
    virtual CARAPI OpenFileInput(
        /* [in] */ const String& name,
        /* [out] */ IFileInputStream** fileInputStream);

    /**
     * Open a private file associated with this Context's application package
     * for writing.  Creates the file if it doesn't already exist.
     *
     * @param name The name of the file to open; can not contain path
     *             separators.
     * @param mode Operating mode.  Use 0 or {@sa #MODE_PRIVATE} for the
     * default operation, {@sa #MODE_APPEND} to append to an existing file,
     * {@sa #MODE_WORLD_READABLE} and {@sa #MODE_WORLD_WRITEABLE} to control
     * permissions.
     *
     * @return FileOutputStream Resulting output stream.
     *
     * @see #MODE_APPEND
     * @see #MODE_PRIVATE
     * @see #MODE_WORLD_READABLE
     * @see #MODE_WORLD_WRITEABLE
     * @see #openFileInput
     * @see #fileList
     * @see #deleteFile
     * @see java.io.FileOutputStream#FileOutputStream(String)
     */
    virtual CARAPI OpenFileOutput(
        /* [in] */ const String& name,
        /* [in] */ Int32 mode,
        /* [out] */ IFileOutputStream** fileOutputStream);

    /**
     * Delete the given private file associated with this Context's
     * application package.
     *
     * @param name The name of the file to delete; can not contain path
     *             separators.
     *
     * @return True if the file was successfully deleted; else
     *         false.
     *
     * @see #openFileInput
     * @see #openFileOutput
     * @see #fileList
     * @see java.io.File#delete()
     */
    virtual CARAPI DeleteFile(
        /* [in] */ const String& name,
        /* [out] */ Boolean* succeeded);

    /**
     * Returns the absolute path on the filesystem where a file created with
     * {@sa #openFileOutput} is stored.
     *
     * @param name The name of the file for which you would like to get
     *          its path.
     *
     * @return Returns an absolute path to the given file.
     *
     * @see #openFileOutput
     * @see #getFilesDir
     * @see #getDir
     */
    virtual CARAPI GetFileStreamPath(
        /* [in] */ const String& name,
        /* [out] */ IFile** file);

    /**
     * Returns the absolute path to the directory on the filesystem where
     * files created with {@sa #openFileOutput} are stored.
     *
     * @return Returns the path of the directory holding application files.
     *
     * @see #openFileOutput
     * @see #getFileStreamPath
     * @see #getDir
     */
    virtual CARAPI GetFilesDir(
        /* [out] */ IFile** filesDir);

    /**
     * Returns the absolute path to the directory on the external filesystem
     * (that is somewhere on {@sa android.os.Environment#getExternalStorageDirectory()
     * Environment.getExternalStorageDirectory()}) where the application can
     * place persistent files it owns.  These files are private to the
     * applications, and not typically visible to the user as media.
     *
     * <p>This is like {@sa #getFilesDir()} in that these
     * files will be deleted when the application is uninstalled, however there
     * are some important differences:
     *
     * <ul>
     * <li>External files are not always available: they will disappear if the
     * user mounts the external storage on a computer or removes it.  See the
     * APIs on {@sa android.os.Environment} for information in the storage state.
     * <li>There is no security enforced with these files.  All applications
     * can read and write files placed here.
     * </ul>
     *
     * <p>On devices with multiple users (as described by {@sa UserManager}),
     * each user has their own isolated external storage. Applications only
     * have access to the external storage for the user they're running as.</p>
     *
     * <p>Here is an example of typical code to manipulate a file in
     * an application's private storage:</p>
     *
     * {@sample development/samples/ApiDemos/src/com/example/android/apis/content/ExternalStorage.java
     * private_file}
     *
     * <p>If you supply a non-null <var>type</var> to this function, the returned
     * file will be a path to a sub-directory of the given type.  Though these files
     * are not automatically scanned by the media scanner, you can explicitly
     * add them to the media database with
     * {@sa android.media.MediaScannerConnection#scanFile(Context, String[], String[],
     *      OnScanCompletedListener) MediaScannerConnection.scanFile}.
     * Note that this is not the same as
     * {@sa android.os.Environment#getExternalStoragePublicDirectory
     * Environment.getExternalStoragePublicDirectory()}, which provides
     * directories of media shared by all applications.  The
     * directories returned here are
     * owned by the application, and their contents will be removed when the
     * application is uninstalled.  Unlike
     * {@sa android.os.Environment#getExternalStoragePublicDirectory
     * Environment.getExternalStoragePublicDirectory()}, the directory
     * returned here will be automatically created for you.
     *
     * <p>Here is an example of typical code to manipulate a picture in
     * an application's private storage and add it to the media database:</p>
     *
     * {@sample development/samples/ApiDemos/src/com/example/android/apis/content/ExternalStorage.java
     * private_picture}
     *
     * <p>Writing to this path requires the
     * {@sa android.Manifest.permission#WRITE_EXTERNAL_STORAGE} permission.</p>
     *
     * @param type The type of files directory to return.  May be null for
     * the root of the files directory or one of
     * the following Environment constants for a subdirectory:
     * {@sa android.os.Environment#DIRECTORY_MUSIC},
     * {@sa android.os.Environment#DIRECTORY_PODCASTS},
     * {@sa android.os.Environment#DIRECTORY_RINGTONES},
     * {@sa android.os.Environment#DIRECTORY_ALARMS},
     * {@sa android.os.Environment#DIRECTORY_NOTIFICATIONS},
     * {@sa android.os.Environment#DIRECTORY_PICTURES}, or
     * {@sa android.os.Environment#DIRECTORY_MOVIES}.
     *
     * @return Returns the path of the directory holding application files
     * on external storage.  Returns null if external storage is not currently
     * mounted so it could not ensure the path exists; you will need to call
     * this method again when it is available.
     *
     * @see #getFilesDir
     * @see android.os.Environment#getExternalStoragePublicDirectory
     */
    virtual CARAPI GetExternalFilesDir(
        /* [in] */ const String& type,
        /* [out] */ IFile** filesDir);

    /**
     * Return the directory where this application's OBB files (if there
     * are any) can be found.  Note if the application does not have any OBB
     * files, this directory may not exist.
     *
     * <p>On devices with multiple users (as described by {@sa UserManager}),
     * multiple users may share the same OBB storage location. Applications
     * should ensure that multiple instances running under different users
     * don't interfere with each other.</p>
     */
    virtual CARAPI GetObbDir(
        /* [out] */ IFile** obbDir);

    /**
     * Returns the absolute path to the application specific cache directory
     * on the filesystem. These files will be ones that get deleted first when the
     * device runs low on storage.
     * There is no guarantee when these files will be deleted.
     *
     * <strong>Note: you should not <em>rely</em> on the system deleting these
     * files for you; you should always have a reasonable maximum, such as 1 MB,
     * for the amount of space you consume with cache files, and prune those
     * files when exceeding that space.</strong>
     *
     * @return Returns the path of the directory holding application cache files.
     *
     * @see #openFileOutput
     * @see #getFileStreamPath
     * @see #getDir
     */
    virtual CARAPI GetCacheDir(
        /* [out] */ IFile** cacheDir);

    /**
     * Returns the absolute path to the directory on the external filesystem
     * (that is somewhere on {@sa android.os.Environment#getExternalStorageDirectory()
     * Environment.getExternalStorageDirectory()} where the application can
     * place cache files it owns.
     *
     * <p>This is like {@sa #getCacheDir()} in that these
     * files will be deleted when the application is uninstalled, however there
     * are some important differences:
     *
     * <ul>
     * <li>The platform does not always monitor the space available in external
     * storage, and thus may not automatically delete these files.  Currently
     * the only time files here will be deleted by the platform is when running
     * on {@sa android.os.Build.VERSION_CODES#JELLY_BEAN_MR1} or later and
     * {@sa android.os.Environment#isExternalStorageEmulated()
     * Environment.isExternalStorageEmulated()} returns true.  Note that you should
     * be managing the maximum space you will use for these anyway, just like
     * with {@sa #getCacheDir()}.
     * <li>External files are not always available: they will disappear if the
     * user mounts the external storage on a computer or removes it.  See the
     * APIs on {@sa android.os.Environment} for information in the storage state.
     * <li>There is no security enforced with these files.  All applications
     * can read and write files placed here.
     * </ul>
     *
     * <p>On devices with multiple users (as described by {@sa UserManager}),
     * each user has their own isolated external storage. Applications only
     * have access to the external storage for the user they're running as.</p>
     *
     * <p>Writing to this path requires the
     * {@sa android.Manifest.permission#WRITE_EXTERNAL_STORAGE} permission.</p>
     *
     * @return Returns the path of the directory holding application cache files
     * on external storage.  Returns null if external storage is not currently
     * mounted so it could not ensure the path exists; you will need to call
     * this method again when it is available.
     *
     * @see #getCacheDir
     */
    virtual CARAPI GetExternalCacheDir(
        /* [out] */ IFile** externalDir);

    /**
     * Returns an array of strings naming the private files associated with
     * this Context's application package.
     *
     * @return Array of strings naming the private files.
     *
     * @see #openFileInput
     * @see #openFileOutput
     * @see #deleteFile
     */
    virtual CARAPI GetFileList(
        /* [out, callee] */ ArrayOf<String>** fileList);

    /**
     * Retrieve, creating if needed, a new directory in which the application
     * can place its own custom data files.  You can use the returned File
     * object to create and access files in this directory.  Note that files
     * created through a File object will only be accessible by your own
     * application; you can only set the mode of the entire directory, not
     * of individual files.
     *
     * @param name Name of the directory to retrieve.  This is a directory
     * that is created as part of your application data.
     * @param mode Operating mode.  Use 0 or {@sa #MODE_PRIVATE} for the
     * default operation, {@sa #MODE_WORLD_READABLE} and
     * {@sa #MODE_WORLD_WRITEABLE} to control permissions.
     *
     * @return Returns a File object for the requested directory.  The directory
     * will have been created if it does not already exist.
     *
     * @see #openFileOutput(String, int)
     */
    virtual CARAPI GetDir(
        /* [in] */ const String& name,
        /* [in] */ Int32 mode,
        /* [out] */ IFile** dir);

    /**
     * Open a new private SQLiteDatabase associated with this Context's
     * application package.  Create the database file if it doesn't exist.
     *
     * @param name The name (unique in the application package) of the database.
     * @param mode Operating mode.  Use 0 or {@sa #MODE_PRIVATE} for the
     *     default operation, {@sa #MODE_WORLD_READABLE}
     *     and {@sa #MODE_WORLD_WRITEABLE} to control permissions.
     *     CARAPI Use {@sa #MODE_ENABLE_WRITE_AHEAD_LOGGING} to enable write-ahead logging by default.
     * @param factory An optional factory class that is called to instantiate a
     *     cursor when query is called.
     *
     * @return The contents of a newly created database with the given name.
     * @return ECode = NOERROR(0) if success, else as follows:
     * Value | Meaning |
     * :-|:------------|
     * android.database.sqlite.SQLiteException | if the database file could not be opened.
     *
     * @see #MODE_PRIVATE
     * @see #MODE_WORLD_READABLE
     * @see #MODE_WORLD_WRITEABLE
     * @see #MODE_ENABLE_WRITE_AHEAD_LOGGING
     * @see #deleteDatabase
     */
    virtual CARAPI OpenOrCreateDatabase(
        /* [in] */ const String& name,
        /* [in] */ Int32 mode,
        /* [in] */ ISQLiteDatabaseCursorFactory* factory,
        /* [out] */ ISQLiteDatabase** sqliteDB);

    /**
     * Open a new private SQLiteDatabase associated with this Context's
     * application package.  Creates the database file if it doesn't exist.
     *
     * <p>Accepts input param: a concrete instance of {@sa DatabaseErrorHandler} to be
     * used to handle corruption when sqlite reports database corruption.</p>
     *
     * @param name The name (unique in the application package) of the database.
     * @param mode Operating mode.  Use 0 or {@sa #MODE_PRIVATE} for the
     *     default operation, {@sa #MODE_WORLD_READABLE}
     *     and {@sa #MODE_WORLD_WRITEABLE} to control permissions.
     *     CARAPI Use {@sa #MODE_ENABLE_WRITE_AHEAD_LOGGING} to enable write-ahead logging by default.
     * @param factory An optional factory class that is called to instantiate a
     *     cursor when query is called.
     * @param errorHandler the {@sa DatabaseErrorHandler} to be used when sqlite reports database
     * corruption. if null, {@sa android.database.DefaultDatabaseErrorHandler} is assumed.
     * @return The contents of a newly created database with the given name.
     * @return ECode = NOERROR(0) if success, else as follows:
     * Value | Meaning |
     * :-|:------------|
     * android.database.sqlite.SQLiteException | if the database file could not be opened.
     *
     * @see #MODE_PRIVATE
     * @see #MODE_WORLD_READABLE
     * @see #MODE_WORLD_WRITEABLE
     * @see #MODE_ENABLE_WRITE_AHEAD_LOGGING
     * @see #deleteDatabase
     */
    virtual CARAPI OpenOrCreateDatabase(
        /* [in] */ const String& name,
        /* [in] */ Int32 mode,
        /* [in] */ ISQLiteDatabaseCursorFactory* factory,
        /* [in] */ IDatabaseErrorHandler* errorHandler,
        /* [out] */ ISQLiteDatabase** sqliteDB);

    /**
     * Delete an existing private SQLiteDatabase associated with this Context's
     * application package.
     *
     * @param name The name (unique in the application package) of the
     *             database.
     *
     * @return True if the database was successfully deleted; else false.
     *
     * @see #openOrCreateDatabase
     */
    virtual CARAPI DeleteDatabase(
        /* [in] */ const String& name,
        /* [out] */ Boolean* succeeded);

    /**
     * Returns the absolute path on the filesystem where a database created with
     * {@sa #openOrCreateDatabase} is stored.
     *
     * @param name The name of the database for which you would like to get
     *          its path.
     *
     * @return Returns an absolute path to the given database.
     *
     * @see #openOrCreateDatabase
     */
    virtual CARAPI GetDatabasePath(
        /* [in] */ const String& name,
        /* [out] */ IFile** path);

    /**
     * Returns an array of strings naming the private databases associated with
     * this Context's application package.
     *
     * @return Array of strings naming the private databases.
     *
     * @see #openOrCreateDatabase
     * @see #deleteDatabase
     */
    virtual CARAPI GetDatabaseList(
        /* [out, callee] */ ArrayOf<String>** databaseList);

    /**
     * @deprecated Use {@sa android.app.WallpaperManager#getDrawable
     * WallpaperManager.get()} instead.
     */
    virtual CARAPI GetWallpaper(
        /* [out] */ IDrawable** drawable);

    /**
     * @deprecated Use {@sa android.app.WallpaperManager#peekDrawable
     * WallpaperManager.peek()} instead.
     */
    virtual CARAPI PeekWallpaper(
        /* [out] */ IDrawable** drawable);

    /**
     * @deprecated Use {@sa android.app.WallpaperManager#getDesiredMinimumWidth()
     * WallpaperManager.getDesiredMinimumWidth()} instead.
     */
    virtual CARAPI GetWallpaperDesiredMinimumWidth(
        /* [out] */ Int32* minWidth);

    /**
     * @deprecated Use {@sa android.app.WallpaperManager#getDesiredMinimumHeight()
     * WallpaperManager.getDesiredMinimumHeight()} instead.
     */
    virtual CARAPI GetWallpaperDesiredMinimumHeight(
        /* [out] */ Int32* minHeight);

    /**
     * @deprecated Use {@sa android.app.WallpaperManager#setBitmap(Bitmap)
     * WallpaperManager.set()} instead.
     * <p>This method requires the caller to hold the permission
     * {@sa android.Manifest.permission#SET_WALLPAPER}.
     */
    virtual CARAPI SetWallpaper(
        /* [in] */ IBitmap* bitmap);

    /**
     * @deprecated Use {@sa android.app.WallpaperManager#setStream(InputStream)
     * WallpaperManager.set()} instead.
     * <p>This method requires the caller to hold the permission
     * {@sa android.Manifest.permission#SET_WALLPAPER}.
     */
    virtual CARAPI SetWallpaper(
        /* [in] */ IInputStream* data);

    /**
     * @deprecated Use {@sa android.app.WallpaperManager#clear
     * WallpaperManager.clear()} instead.
     * <p>This method requires the caller to hold the permission
     * {@sa android.Manifest.permission#SET_WALLPAPER}.
     */
    virtual CARAPI ClearWallpaper();

    /**
     * Same as {@sa #startActivity(Intent, Bundle)} with no options
     * specified.
     *
     * @param intent The description of the activity to start.
     *
     * @return ECode = NOERROR(0) if success, else as follows:
     * Value |
     * :-|
     * ActivityNotFoundException
     *
     * @see {@sa #startActivity(Intent, Bundle)}
     * @see PackageManager#resolveActivity
     */
    virtual CARAPI StartActivity(
        /* [in] */ IIntent* intent);

    /**
     * Version of {@sa #startActivity(Intent)} that allows you to specify the
     * user the activity will be started for.  This is not available to applications
     * that are not pre-installed on the system image.  Using it requires holding
     * the INTERACT_ACROSS_USERS_FULL permission.
     * @param intent The description of the activity to start.
     * @param user The UserHandle of the user to start this activity for.
     * @return ECode = NOERROR(0) if success, else as follows:
     * Value |
     * :-|
     * ActivityNotFoundException
     * @hide
     */
    virtual CARAPI StartActivityAsUser(
        /* [in] */ IIntent* intent,
        /* [in] */ IUserHandle* user);

    /**
     * Launch a new activity.  You will not receive any information about when
     * the activity exits.
     *
     * <p>Note that if this method is being called from outside of an
     * {@sa android.app.Activity} Context, then the Intent must include
     * the {@sa Intent#FLAG_ACTIVITY_NEW_TASK} launch flag.  This is because,
     * without being started from an existing Activity, there is no existing
     * task in which to place the new activity and thus it needs to be placed
     * in its own separate task.
     *
     * <p>This method throws {@sa ActivityNotFoundException}
     * if there was no Activity found to run the given Intent.
     *
     * @param intent The description of the activity to start.
     * @param options Additional options for how the Activity should be started.
     * May be null if there are no options.  See {@sa android.app.ActivityOptions}
     * for how to build the Bundle supplied here; there are no supported definitions
     * for building it manually.
     *
     * @return ECode = NOERROR(0) if success, else as follows:
     * Value |
     * :-|
     * ActivityNotFoundException
     *
     * @see #startActivity(Intent)
     * @see PackageManager#resolveActivity
     */
    virtual CARAPI StartActivity(
        /* [in] */ IIntent* intent,
        /* [in] */ IBundle* options);

    /**
     * Version of {@sa #startActivity(Intent, Bundle)} that allows you to specify the
     * user the activity will be started for.  This is not available to applications
     * that are not pre-installed on the system image.  Using it requires holding
     * the INTERACT_ACROSS_USERS_FULL permission.
     * @param intent The description of the activity to start.
     * @param options Additional options for how the Activity should be started.
     * May be null if there are no options.  See {@sa android.app.ActivityOptions}
     * for how to build the Bundle supplied here; there are no supported definitions
     * for building it manually.
     * @param user The UserHandle of the user to start this activity for.
     * @return ECode = NOERROR(0) if success, else as follows:
     * Value |
     * :-|
     * ActivityNotFoundException
     * @hide
     */
    virtual CARAPI StartActivityAsUser(
        /* [in] */ IIntent* intent,
        /* [in] */ IBundle* options,
        /* [in] */ IUserHandle* userId);

    /**
     * Same as {@sa #startActivities(Intent[], Bundle)} with no options
     * specified.
     *
     * @param intents An array of Intents to be started.
     *
     * @return ECode = NOERROR(0) if success, else as follows:
     * Value |
     * :-|
     * ActivityNotFoundException
     *
     * @see {@sa #startActivities(Intent[], Bundle)}
     * @see PackageManager#resolveActivity
     */
    virtual CARAPI StartActivities(
        /* [in] */ ArrayOf<IIntent*>* intents);

    /**
     * Launch multiple new activities.  This is generally the same as calling
     * {@sa #startActivity(Intent)} for the first Intent in the array,
     * that activity during its creation calling {@sa #startActivity(Intent)}
     * for the second entry, etc.  Note that unlike that approach, generally
     * none of the activities except the last in the array will be created
     * at this point, but rather will be created when the user first visits
     * them (due to pressing back from the activity on top).
     *
     * <p>This method throws {@sa ActivityNotFoundException}
     * if there was no Activity found for <em>any</em> given Intent.  In this
     * case the state of the activity stack is undefined (some Intents in the
     * list may be on it, some not), so you probably want to avoid such situations.
     *
     * @param intents An array of Intents to be started.
     * @param options Additional options for how the Activity should be started.
     * See {@sa android.content.Context#startActivity(Intent, Bundle)
     * Context.startActivity(Intent, Bundle)} for more details.
     *
     * @return ECode = NOERROR(0) if success, else as follows:
     * Value |
     * :-|
     * ActivityNotFoundException
     *
     * @see {@sa #startActivities(Intent[])}
     * @see PackageManager#resolveActivity
     */
    virtual CARAPI StartActivities(
        /* [in] */ ArrayOf<IIntent*>* intents,
        /* [in] */ IBundle* options);

    /**
     * @hide
     * Launch multiple new activities.  This is generally the same as calling
     * {@sa #startActivity(Intent)} for the first Intent in the array,
     * that activity during its creation calling {@sa #startActivity(Intent)}
     * for the second entry, etc.  Note that unlike that approach, generally
     * none of the activities except the last in the array will be created
     * at this point, but rather will be created when the user first visits
     * them (due to pressing back from the activity on top).
     *
     * <p>This method throws {@sa ActivityNotFoundException}
     * if there was no Activity found for <em>any</em> given Intent.  In this
     * case the state of the activity stack is undefined (some Intents in the
     * list may be on it, some not), so you probably want to avoid such situations.
     *
     * @param intents An array of Intents to be started.
     * @param options Additional options for how the Activity should be started.
     * @param userHandle The user for whom to launch the activities
     * See {@sa android.content.Context#startActivity(Intent, Bundle)
     * Context.startActivity(Intent, Bundle)} for more details.
     *
     * @return ECode = NOERROR(0) if success, else as follows:
     * Value |
     * :---|
     * ActivityNotFoundException
     *
     * @see {@sa #startActivities(Intent[])}
     * @see PackageManager#resolveActivity
     */
    virtual CARAPI StartActivitiesAsUser(
        /* [in] */ ArrayOf<IIntent*>* intents,
        /* [in] */ IBundle* options,
        /* [in] */ IUserHandle* userHandle);

    /**
     * Same as {@sa #startIntentSender(IntentSender, Intent, int, int, int, Bundle)}
     * with no options specified.
     *
     * @param intent The IntentSender to launch.
     * @param fillInIntent If non-null, this will be provided as the
     * intent parameter to {@sa IntentSender#sendIntent}.
     * @param flagsMask Intent flags in the original IntentSender that you
     * would like to change.
     * @param flagsValues Desired values for any bits set in
     * <var>flagsMask</var>
     * @param extraFlags Always set to 0.
     *
     * @see #startActivity(Intent)
     * @see #startIntentSender(IntentSender, Intent, int, int, int, Bundle)
     */
    virtual CARAPI StartIntentSender(
        /* [in] */ IIntentSender* intent,
        /* [in] */ IIntent* fillInIntent,
        /* [in] */ Int32 flagsMask,
        /* [in] */ Int32 flagsValues,
        /* [in] */ Int32 extraFlags);

    /**
     * Like {@sa #startActivity(Intent, Bundle)}, but taking a IntentSender
     * to start.  If the IntentSender is for an activity, that activity will be started
     * as if you had called the regular {@sa #startActivity(Intent)}
     * here; otherwise, its associated action will be executed (such as
     * sending a broadcast) as if you had called
     * {@sa IntentSender#sendIntent IntentSender.sendIntent} on it.
     *
     * @param intent The IntentSender to launch.
     * @param fillInIntent If non-null, this will be provided as the
     * intent parameter to {@sa IntentSender#sendIntent}.
     * @param flagsMask Intent flags in the original IntentSender that you
     * would like to change.
     * @param flagsValues Desired values for any bits set in
     * <var>flagsMask</var>
     * @param extraFlags Always set to 0.
     * @param options Additional options for how the Activity should be started.
     * See {@sa android.content.Context#startActivity(Intent, Bundle)
     * Context.startActivity(Intent, Bundle)} for more details.  If options
     * have also been supplied by the IntentSender, options given here will
     * override any that conflict with those given by the IntentSender.
     *
     * @see #startActivity(Intent, Bundle)
     * @see #startIntentSender(IntentSender, Intent, int, int, int)
     */
    virtual CARAPI StartIntentSender(
        /* [in] */ IIntentSender* intent,
        /* [in] */ IIntent* fillInIntent,
        /* [in] */ Int32 flagsMask,
        /* [in] */ Int32 flagsValues,
        /* [in] */ Int32 extraFlags,
        /* [in] */ IBundle* options);

    /**
     * Broadcast the given intent to all interested BroadcastReceivers.  This
     * call is asynchronous; it returns immediately, and you will continue
     * executing while the receivers are run.  No results are propagated from
     * receivers and receivers can not abort the broadcast. If you want
     * to allow receivers to propagate results or abort the broadcast, you must
     * send an ordered broadcast using
     * {@sa #sendOrderedBroadcast(Intent, String)}.
     *
     * <p>See {@sa BroadcastReceiver} for more information on Intent broadcasts.
     *
     * @param intent The Intent to broadcast; all receivers matching this
     *               Intent will receive the broadcast.
     *
     * @see android.content.BroadcastReceiver
     * @see #registerReceiver
     * @see #sendBroadcast(Intent, String)
     * @see #sendOrderedBroadcast(Intent, String)
     * @see #sendOrderedBroadcast(Intent, String, BroadcastReceiver, Handler, int, String, Bundle)
     */
    virtual CARAPI SendBroadcast(
        /* [in] */ IIntent* intent);

    /**
     * Broadcast the given intent to all interested BroadcastReceivers, allowing
     * an optional required permission to be enforced.  This
     * call is asynchronous; it returns immediately, and you will continue
     * executing while the receivers are run.  No results are propagated from
     * receivers and receivers can not abort the broadcast. If you want
     * to allow receivers to propagate results or abort the broadcast, you must
     * send an ordered broadcast using
     * {@sa #sendOrderedBroadcast(Intent, String)}.
     *
     * <p>See {@sa BroadcastReceiver} for more information on Intent broadcasts.
     *
     * @param intent The Intent to broadcast; all receivers matching this
     *               Intent will receive the broadcast.
     * @param receiverPermission (optional) String naming a permission that
     *               a receiver must hold in order to receive your broadcast.
     *               If null, no permission is required.
     *
     * @see android.content.BroadcastReceiver
     * @see #registerReceiver
     * @see #sendBroadcast(Intent)
     * @see #sendOrderedBroadcast(Intent, String)
     * @see #sendOrderedBroadcast(Intent, String, BroadcastReceiver, Handler, int, String, Bundle)
     */
    virtual CARAPI SendBroadcast(
        /* [in] */ IIntent* intent,
        /* [in] */ const String& receiverPermission);

    /**
     * Broadcast the given intent to all interested BroadcastReceivers, delivering
     * them one at a time to allow more preferred receivers to consume the
     * broadcast before it is delivered to less preferred receivers.  This
     * call is asynchronous; it returns immediately, and you will continue
     * executing while the receivers are run.
     *
     * <p>See {@sa BroadcastReceiver} for more information on Intent broadcasts.
     *
     * @param intent The Intent to broadcast; all receivers matching this
     *               Intent will receive the broadcast.
     * @param receiverPermission (optional) String naming a permissions that
     *               a receiver must hold in order to receive your broadcast.
     *               If null, no permission is required.
     *
     * @see android.content.BroadcastReceiver
     * @see #registerReceiver
     * @see #sendBroadcast(Intent)
     * @see #sendOrderedBroadcast(Intent, String, BroadcastReceiver, Handler, int, String, Bundle)
     */
    virtual CARAPI SendOrderedBroadcast(
        /* [in] */ IIntent* intent,
        /* [in] */ const String& receiverPermission);

    /**
     * Version of {@sa #sendBroadcast(Intent)} that allows you to
     * receive data back from the broadcast.  This is accomplished by
     * supplying your own BroadcastReceiver when calling, which will be
     * treated as a final receiver at the end of the broadcast -- its
     * {@sa BroadcastReceiver#onReceive} method will be called with
     * the result values collected from the other receivers.  The broadcast will
     * be serialized in the same way as calling
     * {@sa #sendOrderedBroadcast(Intent, String)}.
     *
     * <p>Like {@sa #sendBroadcast(Intent)}, this method is
     * asynchronous; it will return before
     * resultReceiver.onReceive() is called.
     *
     * <p>See {@sa BroadcastReceiver} for more information on Intent broadcasts.
     *
     * @param intent The Intent to broadcast; all receivers matching this
     *               Intent will receive the broadcast.
     * @param receiverPermission String naming a permissions that
     *               a receiver must hold in order to receive your broadcast.
     *               If null, no permission is required.
     * @param resultReceiver Your own BroadcastReceiver to treat as the final
     *                       receiver of the broadcast.
     * @param scheduler A custom Handler with which to schedule the
     *                  resultReceiver callback; if null it will be
     *                  scheduled in the Context's main thread.
     * @param initialCode An initial value for the result code.  Often
     *                    Activity.RESULT_OK.
     * @param initialData An initial value for the result data.  Often
     *                    null.
     * @param initialExtras An initial value for the result extras.  Often
     *                      null.
     *
     * @see #sendBroadcast(Intent)
     * @see #sendBroadcast(Intent, String)
     * @see #sendOrderedBroadcast(Intent, String)
     * @see #sendStickyBroadcast(Intent)
     * @see #sendStickyOrderedBroadcast(Intent, BroadcastReceiver, Handler, int, String, Bundle)
     * @see android.content.BroadcastReceiver
     * @see #registerReceiver
     * @see android.app.Activity#RESULT_OK
     */
    virtual CARAPI SendOrderedBroadcast(
        /* [in] */ IIntent* intent,
        /* [in] */ const String& receiverPermission,
        /* [in] */ IBroadcastReceiver* resultReceiver,
        /* [in] */ IHandler* scheduler,
        /* [in] */ Int32 initialCode,
        /* [in] */ const String& initialData,
        /* [in] */ IBundle* initialExtras);

    /**
     * Version of {@sa #sendBroadcast(Intent)} that allows you to specify the
     * user the broadcast will be sent to.  This is not available to applications
     * that are not pre-installed on the system image.  Using it requires holding
     * the INTERACT_ACROSS_USERS permission.
     * @param intent The intent to broadcast
     * @param user UserHandle to send the intent to.
     * @see #sendBroadcast(Intent)
     */
    virtual CARAPI SendBroadcastAsUser(
        /* [in] */ IIntent* intent,
        /* [in] */ IUserHandle* user);

    /**
     * Version of {@sa #sendBroadcast(Intent, String)} that allows you to specify the
     * user the broadcast will be sent to.  This is not available to applications
     * that are not pre-installed on the system image.  Using it requires holding
     * the INTERACT_ACROSS_USERS permission.
     *
     * @param intent The Intent to broadcast; all receivers matching this
     *               Intent will receive the broadcast.
     * @param user UserHandle to send the intent to.
     * @param receiverPermission (optional) String naming a permission that
     *               a receiver must hold in order to receive your broadcast.
     *               If null, no permission is required.
     *
     * @see #sendBroadcast(Intent, String)
     */
    virtual CARAPI SendBroadcastAsUser(
        /* [in] */ IIntent* intent,
        /* [in] */ IUserHandle* user,
        /* [in] */ const String& receiverPermission);

    /**
     * Version of
     * {@sa #sendOrderedBroadcast(Intent, String, BroadcastReceiver, Handler, int, String, Bundle)}
     * that allows you to specify the
     * user the broadcast will be sent to.  This is not available to applications
     * that are not pre-installed on the system image.  Using it requires holding
     * the INTERACT_ACROSS_USERS permission.
     *
     * <p>See {@sa BroadcastReceiver} for more information on Intent broadcasts.
     *
     * @param intent The Intent to broadcast; all receivers matching this
     *               Intent will receive the broadcast.
     * @param user UserHandle to send the intent to.
     * @param receiverPermission String naming a permissions that
     *               a receiver must hold in order to receive your broadcast.
     *               If null, no permission is required.
     * @param resultReceiver Your own BroadcastReceiver to treat as the final
     *                       receiver of the broadcast.
     * @param scheduler A custom Handler with which to schedule the
     *                  resultReceiver callback; if null it will be
     *                  scheduled in the Context's main thread.
     * @param initialCode An initial value for the result code.  Often
     *                    Activity.RESULT_OK.
     * @param initialData An initial value for the result data.  Often
     *                    null.
     * @param initialExtras An initial value for the result extras.  Often
     *                      null.
     *
     * @see #sendOrderedBroadcast(Intent, String, BroadcastReceiver, Handler, int, String, Bundle)
     */
    virtual CARAPI SendOrderedBroadcastAsUser(
        /* [in] */ IIntent* intent,
        /* [in] */ IUserHandle* user,
        /* [in] */ const String& receiverPermission,
        /* [in] */ IBroadcastReceiver* resultReceiver,
        /* [in] */ IHandler* scheduler,
        /* [in] */ Int32 initialCode,
        /* [in] */ const String& initialData,
        /* [in] */ IBundle* initialExtras);

    /**
     * Perform a {@sa #sendBroadcast(Intent)} that is "sticky," meaning the
     * Intent you are sending stays around after the broadcast is complete,
     * so that others can quickly retrieve that data through the return
     * value of {@sa #registerReceiver(BroadcastReceiver, IntentFilter)}.  In
     * all other ways, this behaves the same as
     * {@sa #sendBroadcast(Intent)}.
     *
     * <p>You must hold the {@sa android.Manifest.permission#BROADCAST_STICKY}
     * permission in order to use this API.  If you do not hold that
     * permission, {@sa SecurityException} will be thrown.
     *
     * @param intent The Intent to broadcast; all receivers matching this
     * Intent will receive the broadcast, and the Intent will be held to
     * be re-broadcast to future receivers.
     *
     * @see #sendBroadcast(Intent)
     * @see #sendStickyOrderedBroadcast(Intent, BroadcastReceiver, Handler, int, String, Bundle)
     */
    virtual CARAPI SendStickyBroadcast(
        /* [in] */ IIntent* intent);

    /**
     * Version of {@sa #sendStickyBroadcast} that allows you to
     * receive data back from the broadcast.  This is accomplished by
     * supplying your own BroadcastReceiver when calling, which will be
     * treated as a final receiver at the end of the broadcast -- its
     * {@sa BroadcastReceiver#onReceive} method will be called with
     * the result values collected from the other receivers.  The broadcast will
     * be serialized in the same way as calling
     * {@sa #sendOrderedBroadcast(Intent, String)}.
     *
     * <p>Like {@sa #sendBroadcast(Intent)}, this method is
     * asynchronous; it will return before
     * resultReceiver.onReceive() is called.  Note that the sticky data
     * stored is only the data you initially supply to the broadcast, not
     * the result of any changes made by the receivers.
     *
     * <p>See {@sa BroadcastReceiver} for more information on Intent broadcasts.
     *
     * @param intent The Intent to broadcast; all receivers matching this
     *               Intent will receive the broadcast.
     * @param resultReceiver Your own BroadcastReceiver to treat as the final
     *                       receiver of the broadcast.
     * @param scheduler A custom Handler with which to schedule the
     *                  resultReceiver callback; if null it will be
     *                  scheduled in the Context's main thread.
     * @param initialCode An initial value for the result code.  Often
     *                    Activity.RESULT_OK.
     * @param initialData An initial value for the result data.  Often
     *                    null.
     * @param initialExtras An initial value for the result extras.  Often
     *                      null.
     *
     * @see #sendBroadcast(Intent)
     * @see #sendBroadcast(Intent, String)
     * @see #sendOrderedBroadcast(Intent, String)
     * @see #sendStickyBroadcast(Intent)
     * @see android.content.BroadcastReceiver
     * @see #registerReceiver
     * @see android.app.Activity#RESULT_OK
     */
    virtual CARAPI SendStickyOrderedBroadcast(
        /* [in] */ IIntent* intent,
        /* [in] */ IBroadcastReceiver* resultReceiver,
        /* [in] */ IHandler* scheduler,
        /* [in] */ Int32 initialCode,
        /* [in] */ const String& initialData,
        /* [in] */ IBundle* initialExtras);

    /**
     * Remove the data previously sent with {@sa #sendStickyBroadcast},
     * so that it is as if the sticky broadcast had never happened.
     *
     * <p>You must hold the {@sa android.Manifest.permission#BROADCAST_STICKY}
     * permission in order to use this API.  If you do not hold that
     * permission, {@sa SecurityException} will be thrown.
     *
     * @param intent The Intent that was previously broadcast.
     *
     * @see #sendStickyBroadcast
     */
    virtual CARAPI RemoveStickyBroadcast(
        /* [in] */ IIntent* intent);

    /**
     * Version of {@sa #sendStickyBroadcast(Intent)} that allows you to specify the
     * user the broadcast will be sent to.  This is not available to applications
     * that are not pre-installed on the system image.  Using it requires holding
     * the INTERACT_ACROSS_USERS permission.
     *
     * @param intent The Intent to broadcast; all receivers matching this
     * Intent will receive the broadcast, and the Intent will be held to
     * be re-broadcast to future receivers.
     * @param user UserHandle to send the intent to.
     *
     * @see #sendBroadcast(Intent)
     */
    virtual CARAPI SendStickyBroadcastAsUser(
        /* [in] */ IIntent* intent,
        /* [in] */ IUserHandle* user);

    /**
     * Version of
     * {@sa #sendStickyOrderedBroadcast(Intent, BroadcastReceiver, Handler, int, String, Bundle)}
     * that allows you to specify the
     * user the broadcast will be sent to.  This is not available to applications
     * that are not pre-installed on the system image.  Using it requires holding
     * the INTERACT_ACROSS_USERS permission.
     *
     * <p>See {@sa BroadcastReceiver} for more information on Intent broadcasts.
     *
     * @param intent The Intent to broadcast; all receivers matching this
     *               Intent will receive the broadcast.
     * @param user UserHandle to send the intent to.
     * @param resultReceiver Your own BroadcastReceiver to treat as the final
     *                       receiver of the broadcast.
     * @param scheduler A custom Handler with which to schedule the
     *                  resultReceiver callback; if null it will be
     *                  scheduled in the Context's main thread.
     * @param initialCode An initial value for the result code.  Often
     *                    Activity.RESULT_OK.
     * @param initialData An initial value for the result data.  Often
     *                    null.
     * @param initialExtras An initial value for the result extras.  Often
     *                      null.
     *
     * @see #sendStickyOrderedBroadcast(Intent, BroadcastReceiver, Handler, int, String, Bundle)
     */
    virtual CARAPI SendStickyOrderedBroadcastAsUser(
        /* [in] */ IIntent* intent,
        /* [in] */ IUserHandle* user,
        /* [in] */ IBroadcastReceiver* resultReceiver,
        /* [in] */ IHandler* scheduler,
        /* [in] */ Int32 initialCode,
        /* [in] */ const String& initialData,
        /* [in] */ IBundle* initialExtras);

    /**
     * Version of {@sa #removeStickyBroadcast(Intent)} that allows you to specify the
     * user the broadcast will be sent to.  This is not available to applications
     * that are not pre-installed on the system image.  Using it requires holding
     * the INTERACT_ACROSS_USERS permission.
     *
     * <p>You must hold the {@sa android.Manifest.permission#BROADCAST_STICKY}
     * permission in order to use this API.  If you do not hold that
     * permission, {@sa SecurityException} will be thrown.
     *
     * @param intent The Intent that was previously broadcast.
     * @param user UserHandle to remove the sticky broadcast from.
     *
     * @see #sendStickyBroadcastAsUser
     */
    virtual CARAPI RemoveStickyBroadcastAsUser(
        /* [in] */ IIntent* intent,
        /* [in] */ IUserHandle* user);

    /**
     * Register a BroadcastReceiver to be run in the main activity thread.  The
     * <var>receiver</var> will be called with any broadcast Intent that
     * matches <var>filter</var>, in the main application thread.
     *
     * <p>The system may broadcast Intents that are "sticky" -- these stay
     * around after the broadcast as finished, to be sent to any later
     * registrations. If your IntentFilter matches one of these sticky
     * Intents, that Intent will be returned by this function
     * <strong>and</strong> sent to your <var>receiver</var> as if it had just
     * been broadcast.
     *
     * <p>There may be multiple sticky Intents that match <var>filter</var>,
     * in which case each of these will be sent to <var>receiver</var>.  In
     * this case, only one of these can be returned directly by the function;
     * which of these that is returned is arbitrarily decided by the system.
     *
     * <p>If you know the Intent your are registering for is sticky, you can
     * supply null for your <var>receiver</var>.  In this case, no receiver is
     * registered -- the function simply returns the sticky Intent that
     * matches <var>filter</var>.  In the case of multiple matches, the same
     * rules as described above apply.
     *
     * <p>See {@sa BroadcastReceiver} for more information on Intent broadcasts.
     *
     * <p>As of {@sa android.os.Build.VERSION_CODES#ICE_CREAM_SANDWICH}, receivers
     * registered with this method will correctly respect the
     * {@sa Intent#setPackage(String)} specified for an Intent being broadcast.
     * Prior to that, it would be ignored and delivered to all matching registered
     * receivers.  Be careful if using this for security.</p>
     *
     * <p class="note">Note: this method <em>cannot be called from a
     * {@sa BroadcastReceiver} component;</em> that is, from a BroadcastReceiver
     * that is declared in an application's manifest.  It is okay, however, to call
     * this method from another BroadcastReceiver that has itself been registered
     * at run time with {@sa #registerReceiver}, since the lifetime of such a
     * registered BroadcastReceiver is tied to the object that registered it.</p>
     *
     * @param receiver The BroadcastReceiver to handle the broadcast.
     * @param filter Selects the Intent broadcasts to be received.
     *
     * @return The first sticky intent found that matches <var>filter</var>,
     *         or null if there are none.
     *
     * @see #registerReceiver(BroadcastReceiver, IntentFilter, String, Handler)
     * @see #sendBroadcast
     * @see #unregisterReceiver
     */
    virtual CARAPI RegisterReceiver(
        /* [in] */ IBroadcastReceiver* receiver,
        /* [in] */ IIntentFilter* filter,
        /* [out] */ IIntent** stickyIntent);

    /**
     * Register to receive intent broadcasts, to run in the context of
     * <var>scheduler</var>.  See
     * {@sa #registerReceiver(BroadcastReceiver, IntentFilter)} for more
     * information.  This allows you to enforce permissions on who can
     * broadcast intents to your receiver, or have the receiver run in
     * a different thread than the main application thread.
     *
     * <p>See {@sa BroadcastReceiver} for more information on Intent broadcasts.
     *
     * <p>As of {@sa android.os.Build.VERSION_CODES#ICE_CREAM_SANDWICH}, receivers
     * registered with this method will correctly respect the
     * {@sa Intent#setPackage(String)} specified for an Intent being broadcast.
     * Prior to that, it would be ignored and delivered to all matching registered
     * receivers.  Be careful if using this for security.</p>
     *
     * @param receiver The BroadcastReceiver to handle the broadcast.
     * @param filter Selects the Intent broadcasts to be received.
     * @param broadcastPermission String naming a permissions that a
     *      broadcaster must hold in order to send an Intent to you.  If null,
     *      no permission is required.
     * @param scheduler Handler identifying the thread that will receive
     *      the Intent.  If null, the main thread of the process will be used.
     *
     * @return The first sticky intent found that matches <var>filter</var>,
     *         or null if there are none.
     *
     * @see #registerReceiver(BroadcastReceiver, IntentFilter)
     * @see #sendBroadcast
     * @see #unregisterReceiver
     */
    virtual CARAPI RegisterReceiver(
        /* [in] */ IBroadcastReceiver* receiver,
        /* [in] */ IIntentFilter* filter,
        /* [in] */ const String& broadcastPermission,
        /* [in] */ IHandler* scheduler,
        /* [out] */ IIntent** stickyIntent);

    /**
     * @hide
     * Same as {@sa #registerReceiver(BroadcastReceiver, IntentFilter, String, Handler)
     * but for a specific user.  This receiver will receiver broadcasts that
     * are sent to the requested user.  It
     * requires holding the {@sa android.Manifest.permission#INTERACT_ACROSS_USERS_FULL}
     * permission.
     *
     * @param receiver The BroadcastReceiver to handle the broadcast.
     * @param user UserHandle to send the intent to.
     * @param filter Selects the Intent broadcasts to be received.
     * @param broadcastPermission String naming a permissions that a
     *      broadcaster must hold in order to send an Intent to you.  If null,
     *      no permission is required.
     * @param scheduler Handler identifying the thread that will receive
     *      the Intent.  If null, the main thread of the process will be used.
     *
     * @return The first sticky intent found that matches <var>filter</var>,
     *         or null if there are none.
     *
     * @see #registerReceiver(BroadcastReceiver, IntentFilter, String, Handler
     * @see #sendBroadcast
     * @see #unregisterReceiver
     */
    virtual CARAPI RegisterReceiverAsUser(
        /* [in] */ IBroadcastReceiver* receiver,
        /* [in] */ IUserHandle* user,
        /* [in] */ IIntentFilter* filter,
        /* [in] */ const String& broadcastPermission,
        /* [in] */ IHandler* scheduler,
        /* [out] */ IIntent** stickyIntent);

    /**
     * Unregister a previously registered BroadcastReceiver.  <em>All</em>
     * filters that have been registered for this BroadcastReceiver will be
     * removed.
     *
     * @param receiver The BroadcastReceiver to unregister.
     *
     * @see #registerReceiver
     */
    virtual CARAPI UnregisterReceiver(
        /* [in] */ IBroadcastReceiver* receiver);

    /**
     * Request that a given application service be started.  The Intent
     * can either contain the complete class name of a specific service
     * implementation to start, or an abstract definition through the
     * action and other fields of the kind of service to start.  If this service
     * is not already running, it will be instantiated and started (creating a
     * process for it if needed); if it is running then it remains running.
     *
     * <p>Every call to this method will result in a corresponding call to
     * the target service's {@sa android.app.Service#onStartCommand} method,
     * with the <var>intent</var> given here.  This provides a convenient way
     * to submit jobs to a service without having to bind and call on to its
     * interface.
     *
     * <p>Using startService() overrides the default service lifetime that is
     * managed by {@sa #bindService}: it requires the service to remain
     * running until {@sa #stopService} is called, regardless of whether
     * any clients are connected to it.  Note that calls to startService()
     * are not nesting: no matter how many times you call startService(),
     * a single call to {@sa #stopService} will stop it.
     *
     * <p>The system attempts to keep running services around as much as
     * possible.  The only time they should be stopped is if the current
     * foreground application is using so many resources that the service needs
     * to be killed.  If any errors happen in the service's process, it will
     * automatically be restarted.
     *
     * <p>This function will throw {@sa SecurityException} if you do not
     * have permission to start the given service.
     *
     * @param service Identifies the service to be started.  The Intent may
     *      specify either an explicit component name to start, or a logical
     *      description (action, category, etc) to match an
     *      {@sa IntentFilter} published by a service.  Additional values
     *      may be included in the Intent extras to supply arguments along with
     *      this specific start call.
     *
     * @return If the service is being started or is already running, the
     * {@sa ComponentName} of the actual service that was started is
     * returned; else if the service does not exist null is returned.
     *
     * @return ECode = NOERROR(0) if success, else as follows:
     * Value |
     * :-|
     * SecurityException
     *
     * @see #stopService
     * @see #bindService
     */
    virtual CARAPI StartService(
        /* [in] */ IIntent* service,
        /* [out] */ IComponentName** name);

    /**
     * Request that a given application service be stopped.  If the service is
     * not running, nothing happens.  Otherwise it is stopped.  Note that calls
     * to startService() are not counted -- this stops the service no matter
     * how many times it was started.
     *
     * <p>Note that if a stopped service still has {@sa ServiceConnection}
     * objects bound to it with the {@sa #BIND_AUTO_CREATE} set, it will
     * not be destroyed until all of these bindings are removed.  See
     * the {@sa android.app.Service} documentation for more details on a
     * service's lifecycle.
     *
     * <p>This function will throw {@sa SecurityException} if you do not
     * have permission to stop the given service.
     *
     * @param service Description of the service to be stopped.  The Intent may
     *      specify either an explicit component name to start, or a logical
     *      description (action, category, etc) to match an
     *      {@sa IntentFilter} published by a service.
     *
     * @return If there is a service matching the given Intent that is already
     * running, then it is stopped and true is returned; else false is returned.
     *
     * @return ECode = NOERROR(0) if success, else as follows:
     * Value |
     * :-|
     * SecurityException
     *
     * @see #startService
     */
    virtual CARAPI StopService(
        /* [in] */ IIntent* service,
        /* [out] */ Boolean* succeeded);

    /**
     * @hide like {@sa #startService(Intent)} but for a specific user.
     */
    virtual CARAPI StartServiceAsUser(
        /* [in] */ IIntent* service,
        /* [in] */ IUserHandle* user,
        /* [out] */ IComponentName** name);

    /**
     * @hide like {@sa #stopService(Intent)} but for a specific user.
     */
    virtual CARAPI StopServiceAsUser(
        /* [in] */ IIntent* service,
        /* [in] */ IUserHandle* user,
        /* [out] */ Boolean* succeeded);

    /**
     * Connect to an application service, creating it if needed.  This defines
     * a dependency between your application and the service.  The given
     * <var>conn</var> will receive the service object when it is created and be
     * told if it dies and restarts.  The service will be considered required
     * by the system only for as long as the calling context exists.  For
     * example, if this Context is an Activity that is stopped, the service will
     * not be required to continue running until the Activity is resumed.
     *
     * <p>This function will throw {@sa SecurityException} if you do not
     * have permission to bind to the given service.
     *
     * <p class="note">Note: this method <em>can not be called from a
     * {@sa BroadcastReceiver} component</em>.  A pattern you can use to
     * communicate from a BroadcastReceiver to a Service is to call
     * {@sa #startService} with the arguments containing the command to be
     * sent, with the service calling its
     * {@sa android.app.Service#stopSelf(int)} method when done executing
     * that command.  See the API demo App/Service/Service Start Arguments
     * Controller for an illustration of this.  It is okay, however, to use
     * this method from a BroadcastReceiver that has been registered with
     * {@sa #registerReceiver}, since the lifetime of this BroadcastReceiver
     * is tied to another object (the one that registered it).</p>
     *
     * @param service Identifies the service to connect to.  The Intent may
     *      specify either an explicit component name, or a logical
     *      description (action, category, etc) to match an
     *      {@sa IntentFilter} published by a service.
     * @param conn Receives information as the service is started and stopped.
     *      This must be a valid ServiceConnection object; it must not be null.
     * @param flags Operation options for the binding.  May be 0,
     *          {@sa #BIND_AUTO_CREATE}, {@sa #BIND_DEBUG_UNBIND},
     *          {@sa #BIND_NOT_FOREGROUND}, {@sa #BIND_ABOVE_CLIENT},
     *          {@sa #BIND_ALLOW_OOM_MANAGEMENT}, or
     *          {@sa #BIND_WAIVE_PRIORITY}.
     * @return If you have successfully bound to the service, true is returned;
     *         false is returned if the connection is not made so you will not
     *         receive the service object.
     *
     * @return ECode = NOERROR(0) if success, else as follows:
     * Value |
     * :-|
     * SecurityException
     *
     * @see #unbindService
     * @see #startService
     * @see #BIND_AUTO_CREATE
     * @see #BIND_DEBUG_UNBIND
     * @see #BIND_NOT_FOREGROUND
     */
    virtual CARAPI BindService(
        /* [in] */ IIntent* service,
        /* [in] */ IServiceConnection* conn,
        /* [in] */ Int32 flags,
        /* [out] */ Boolean* succeeded);

    /**
     * Same as {@sa #bindService(Intent, ServiceConnection, int)}, but with an explicit userHandle
     * argument for use by system server and other multi-user aware code.
     * @hide
     */
    virtual CARAPI BindService(
        /* [in] */ IIntent* service,
        /* [in] */ IServiceConnection* conn,
        /* [in] */ Int32 flags,
        /* [in] */ Int32 userHandle,
        /* [out] */ Boolean* succeeded);

    /**
     * Disconnect from an application service.  You will no longer receive
     * calls as the service is restarted, and the service is now allowed to
     * stop at any time.
     *
     * @param conn The connection interface previously supplied to
     *             CARAPI BindService().  This parameter must not be null.
     *
     * @see #bindService
     */
    virtual CARAPI UnbindService(
        /* [in] */ IServiceConnection* conn);

    /**
     * Start executing an {@sa android.app.Instrumentation} class.  The given
     * Instrumentation component will be run by killing its target application
     * (if currently running), starting the target process, instantiating the
     * instrumentation component, and then letting it drive the application.
     *
     * <p>This function is not synchronous -- it returns as soon as the
     * instrumentation has started and while it is running.
     *
     * <p>Instrumentation is normally only allowed to run against a package
     * that is either unsigned or signed with a signature that the
     * the instrumentation package is also signed with (ensuring the target
     * trusts the instrumentation).
     *
     * @param className Name of the Instrumentation component to be run.
     * @param profileFile Optional path to write profiling data as the
     * instrumentation runs, or null for no profiling.
     * @param arguments Additional optional arguments to pass to the
     * instrumentation, or null.
     *
     * @return Returns true if the instrumentation was successfully started,
     * else false if it could not be found.
     */
    virtual CARAPI StartInstrumentation(
        /* [in] */ IComponentName* className,
        /* [in] */ const String& profileFile,
        /* [in] */ IBundle* arguments,
        /* [out] */ Boolean* succeeded);

    /**
     * Return the handle to a system-level service by name. The class of the
     * returned object varies by the requested name. Currently available names
     * are:
     *
     * <dl>
     *  <dt> {@sa #WINDOW_SERVICE} ("window")
     *  <dd> The top-level window manager in which you can place custom
     *  windows.  The returned object is a {@sa android.view.WindowManager}.
     *  <dt> {@sa #LAYOUT_INFLATER_SERVICE} ("layout_inflater")
     *  <dd> A {@sa android.view.LayoutInflater} for inflating layout resources
     *  in this context.
     *  <dt> {@sa #ACTIVITY_SERVICE} ("activity")
     *  <dd> A {@sa android.app.ActivityManager} for interacting with the
     *  global activity state of the system.
     *  <dt> {@sa #POWER_SERVICE} ("power")
     *  <dd> A {@sa android.os.PowerManager} for controlling power
     *  management.
     *  <dt> {@sa #ALARM_SERVICE} ("alarm")
     *  <dd> A {@sa android.app.AlarmManager} for receiving intents at the
     *  time of your choosing.
     *  <dt> {@sa #NOTIFICATION_SERVICE} ("notification")
     *  <dd> A {@sa android.app.NotificationManager} for informing the user
     *   of background events.
     *  <dt> {@sa #KEYGUARD_SERVICE} ("keyguard")
     *  <dd> A {@sa android.app.KeyguardManager} for controlling keyguard.
     *  <dt> {@sa #LOCATION_SERVICE} ("location")
     *  <dd> A {@sa android.location.LocationManager} for controlling location
     *   (e.g., GPS) updates.
     *  <dt> {@sa #SEARCH_SERVICE} ("search")
     *  <dd> A {@sa android.app.SearchManager} for handling search.
     *  <dt> {@sa #VIBRATOR_SERVICE} ("vibrator")
     *  <dd> A {@sa android.os.Vibrator} for interacting with the vibrator
     *  hardware.
     *  <dt> {@sa #CONNECTIVITY_SERVICE} ("connection")
     *  <dd> A {@sa android.net.ConnectivityManager ConnectivityManager} for
     *  handling management of network connections.
     *  <dt> {@sa #WIFI_SERVICE} ("wifi")
     *  <dd> A {@sa android.net.wifi.WifiManager WifiManager} for management of
     * Wi-Fi connectivity.
     * <dt> {@sa #INPUT_METHOD_SERVICE} ("input_method")
     * <dd> An {@sa android.view.inputmethod.InputMethodManager InputMethodManager}
     * for management of input methods.
     * <dt> {@sa #UI_MODE_SERVICE} ("uimode")
     * <dd> An {@sa android.app.UiModeManager} for controlling UI modes.
     * <dt> {@sa #DOWNLOAD_SERVICE} ("download")
     * <dd> A {@sa android.app.DownloadManager} for requesting HTTP downloads
     * </dl>
     *
     * <p>Note:  System services obtained via this API may be closely associated with
     * the Context in which they are obtained from.  In general, do not share the
     * service objects between various different contexts (Activities, Applications,
     * Services, Providers, etc.)
     *
     * @param name The name of the desired service.
     *
     * @return The service or null if the name does not exist.
     *
     * @see #WINDOW_SERVICE
     * @see android.view.WindowManager
     * @see #LAYOUT_INFLATER_SERVICE
     * @see android.view.LayoutInflater
     * @see #ACTIVITY_SERVICE
     * @see android.app.ActivityManager
     * @see #POWER_SERVICE
     * @see android.os.PowerManager
     * @see #ALARM_SERVICE
     * @see android.app.AlarmManager
     * @see #NOTIFICATION_SERVICE
     * @see android.app.NotificationManager
     * @see #KEYGUARD_SERVICE
     * @see android.app.KeyguardManager
     * @see #LOCATION_SERVICE
     * @see android.location.LocationManager
     * @see #SEARCH_SERVICE
     * @see android.app.SearchManager
     * @see #SENSOR_SERVICE
     * @see android.hardware.SensorManager
     * @see #STORAGE_SERVICE
     * @see android.os.storage.StorageManager
     * @see #VIBRATOR_SERVICE
     * @see android.os.Vibrator
     * @see #CONNECTIVITY_SERVICE
     * @see android.net.ConnectivityManager
     * @see #WIFI_SERVICE
     * @see android.net.wifi.WifiManager
     * @see #AUDIO_SERVICE
     * @see android.media.AudioManager
     * @see #MEDIA_ROUTER_SERVICE
     * @see android.media.MediaRouter
     * @see #TELEPHONY_SERVICE
     * @see android.telephony.TelephonyManager
     * @see #INPUT_METHOD_SERVICE
     * @see android.view.inputmethod.InputMethodManager
     * @see #UI_MODE_SERVICE
     * @see android.app.UiModeManager
     * @see #DOWNLOAD_SERVICE
     * @see android.app.DownloadManager
     */
    virtual CARAPI GetSystemService(
        /* [in] */ const String& name,
        /* [out] */ IInterface** object);

    /**
     * Determine whether the given permission is allowed for a particular
     * process and user ID running in the system.
     *
     * @param permission The name of the permission being checked.
     * @param pid The process ID being checked against.  Must be > 0.
     * @param uid The user ID being checked against.  A uid of 0 is the root
     * user, which will pass every permission check.
     *
     * @return Returns {@sa PackageManager#PERMISSION_GRANTED} if the given
     * pid/uid is allowed that permission, or
     * {@sa PackageManager#PERMISSION_DENIED} if it is not.
     *
     * @see PackageManager#checkPermission(String, String)
     * @see #checkCallingPermission
     */
    virtual CARAPI CheckPermission(
        /* [in] */ const String& permission,
        /* [in] */ Int32 pid,
        /* [in] */ Int32 uid,
        /* [out] */ Int32* result);

    /**
     * Determine whether the calling process of an IPC you are handling has been
     * granted a particular permission.  This is basically the same as calling
     * {@sa #checkPermission(String, int, int)} with the pid and uid returned
     * by {@sa android.os.Binder#getCallingPid} and
     * {@sa android.os.Binder#getCallingUid}.  One important difference
     * is that if you are not currently processing an IPC, this function
     * will always fail.  This is done to protect against accidentally
     * leaking permissions; you can use {@sa #checkCallingOrSelfPermission}
     * to avoid this protection.
     *
     * @param permission The name of the permission being checked.
     *
     * @return Returns {@sa PackageManager#PERMISSION_GRANTED} if the calling
     * pid/uid is allowed that permission, or
     * {@sa PackageManager#PERMISSION_DENIED} if it is not.
     *
     * @see PackageManager#checkPermission(String, String)
     * @see #checkPermission
     * @see #checkCallingOrSelfPermission
     */
    virtual CARAPI CheckCallingPermission(
        /* [in] */ const String& permission,
        /* [out] */ Int32* result);

    /**
     * Determine whether the calling process of an IPC <em>or you</em> have been
     * granted a particular permission.  This is the same as
     * {@sa #checkCallingPermission}, except it grants your own permissions
     * if you are not currently processing an IPC.  Use with care!
     *
     * @param permission The name of the permission being checked.
     *
     * @return Returns {@sa PackageManager#PERMISSION_GRANTED} if the calling
     * pid/uid is allowed that permission, or
     * {@sa PackageManager#PERMISSION_DENIED} if it is not.
     *
     * @see PackageManager#checkPermission(String, String)
     * @see #checkPermission
     * @see #checkCallingPermission
     */
    virtual CARAPI CheckCallingOrSelfPermission(
        /* [in] */ const String& permission,
        /* [out] */ Int32* result);

    /**
     * If the given permission is not allowed for a particular process
     * and user ID running in the system, throw a {@sa SecurityException}.
     *
     * @param permission The name of the permission being checked.
     * @param pid The process ID being checked against.  Must be &gt; 0.
     * @param uid The user ID being checked against.  A uid of 0 is the root
     * user, which will pass every permission check.
     * @param message A message to include in the exception if it is thrown.
     *
     * @see #checkPermission(String, int, int)
     */
    virtual CARAPI EnforcePermission(
        /* [in] */ const String& permission,
        /* [in] */ Int32 pid,
        /* [in] */ Int32 uid,
        /* [in] */ const String& message);

    /**
     * If the calling process of an IPC you are handling has not been
     * granted a particular permission, throw a {@sa
     * SecurityException}.  This is basically the same as calling
     * {@sa #enforcePermission(String, int, int, String)} with the
     * pid and uid returned by {@sa android.os.Binder#getCallingPid}
     * and {@sa android.os.Binder#getCallingUid}.  One important
     * difference is that if you are not currently processing an IPC,
     * this function will always throw the SecurityException.  This is
     * done to protect against accidentally leaking permissions; you
     * can use {@sa #enforceCallingOrSelfPermission} to avoid this
     * protection.
     *
     * @param permission The name of the permission being checked.
     * @param message A message to include in the exception if it is thrown.
     *
     * @see #checkCallingPermission(String)
     */
    virtual CARAPI EnforceCallingPermission(
        /* [in] */ const String& permission,
        /* [in] */ const String& message);

    /**
     * If neither you nor the calling process of an IPC you are
     * handling has been granted a particular permission, throw a
     * {@sa SecurityException}.  This is the same as {@sa
     * #enforceCallingPermission}, except it grants your own
     * permissions if you are not currently processing an IPC.  Use
     * with care!
     *
     * @param permission The name of the permission being checked.
     * @param message A message to include in the exception if it is thrown.
     *
     * @see #checkCallingOrSelfPermission(String)
     */
    virtual CARAPI EnforceCallingOrSelfPermission(
        /* [in] */ const String& permission,
        /* [in] */ const String& message);

    /**
     * Grant permission to access a specific Uri to another package, regardless
     * of whether that package has general permission to access the Uri's
     * content provider.  This can be used to grant specific, temporary
     * permissions, typically in response to user interaction (such as the
     * user opening an attachment that you would like someone else to
     * display).
     *
     * <p>Normally you should use {@sa Intent#FLAG_GRANT_READ_URI_PERMISSION
     * Intent.FLAG_GRANT_READ_URI_PERMISSION} or
     * {@sa Intent#FLAG_GRANT_WRITE_URI_PERMISSION
     * Intent.FLAG_GRANT_WRITE_URI_PERMISSION} with the Intent being used to
     * start an activity instead of this function directly.  If you use this
     * function directly, you should be sure to call
     * {@sa #revokeUriPermission} when the target should no longer be allowed
     * to access it.
     *
     * <p>To succeed, the content provider owning the Uri must have set the
     * {@sa android.R.styleable#AndroidManifestProvider_grantUriPermissions
     * grantUriPermissions} attribute in its manifest or included the
     * {@sa android.R.styleable#AndroidManifestGrantUriPermission
     * &lt;grant-uri-permissions&gt;} tag.
     *
     * @param toPackage The package you would like to allow to access the Uri.
     * @param uri The Uri you would like to grant access to.
     * @param modeFlags The desired access modes.  Any combination of
     * {@sa Intent#FLAG_GRANT_READ_URI_PERMISSION
     * Intent.FLAG_GRANT_READ_URI_PERMISSION} or
     * {@sa Intent#FLAG_GRANT_WRITE_URI_PERMISSION
     * Intent.FLAG_GRANT_WRITE_URI_PERMISSION}.
     *
     * @see #revokeUriPermission
     */
    virtual CARAPI GrantUriPermission(
        /* [in] */ const String& toPackage,
        /* [in] */ IUri* uri,
        /* [in] */ Int32 modeFlags);

    /**
     * Remove all permissions to access a particular content provider Uri
     * that were previously added with {@sa #grantUriPermission}.  The given
     * Uri will match all previously granted Uris that are the same or a
     * sub-path of the given Uri.  That is, revoking "content://foo/one" will
     * revoke both "content://foo/target" and "content://foo/target/sub", but not
     * "content://foo".
     *
     * @param uri The Uri you would like to revoke access to.
     * @param modeFlags The desired access modes.  Any combination of
     * {@sa Intent#FLAG_GRANT_READ_URI_PERMISSION
     * Intent.FLAG_GRANT_READ_URI_PERMISSION} or
     * {@sa Intent#FLAG_GRANT_WRITE_URI_PERMISSION
     * Intent.FLAG_GRANT_WRITE_URI_PERMISSION}.
     *
     * @see #grantUriPermission
     */
    virtual CARAPI RevokeUriPermission(
        /* [in] */ IUri* uri,
        /* [in] */ Int32 modeFlags);

    /**
     * Determine whether a particular process and user ID has been granted
     * permission to access a specific URI.  This only checks for permissions
     * that have been explicitly granted -- if the given process/uid has
     * more general access to the URI's content provider then this check will
     * always fail.
     *
     * @param uri The uri that is being checked.
     * @param pid The process ID being checked against.  Must be &gt; 0.
     * @param uid The user ID being checked against.  A uid of 0 is the root
     * user, which will pass every permission check.
     * @param modeFlags The type of access to grant.  May be one or both of
     * {@sa Intent#FLAG_GRANT_READ_URI_PERMISSION Intent.FLAG_GRANT_READ_URI_PERMISSION} or
     * {@sa Intent#FLAG_GRANT_WRITE_URI_PERMISSION Intent.FLAG_GRANT_WRITE_URI_PERMISSION}.
     *
     * @return Returns {@sa PackageManager#PERMISSION_GRANTED} if the given
     * pid/uid is allowed to access that uri, or
     * {@sa PackageManager#PERMISSION_DENIED} if it is not.
     *
     * @see #checkCallingUriPermission
     */
    virtual CARAPI CheckUriPermission(
        /* [in] */ IUri* uri,
        /* [in] */ Int32 pid,
        /* [in] */ Int32 uid,
        /* [in] */ Int32 modeFlags,
        /* [out] */ Int32* result);

    /**
     * Determine whether the calling process and user ID has been
     * granted permission to access a specific URI.  This is basically
     * the same as calling {@sa #checkUriPermission(Uri, int, int,
     * int)} with the pid and uid returned by {@sa
     * android.os.Binder#getCallingPid} and {@sa
     * android.os.Binder#getCallingUid}.  One important difference is
     * that if you are not currently processing an IPC, this function
     * will always fail.
     *
     * @param uri The uri that is being checked.
     * @param modeFlags The type of access to grant.  May be one or both of
     * {@sa Intent#FLAG_GRANT_READ_URI_PERMISSION Intent.FLAG_GRANT_READ_URI_PERMISSION} or
     * {@sa Intent#FLAG_GRANT_WRITE_URI_PERMISSION Intent.FLAG_GRANT_WRITE_URI_PERMISSION}.
     *
     * @return Returns {@sa PackageManager#PERMISSION_GRANTED} if the caller
     * is allowed to access that uri, or
     * {@sa PackageManager#PERMISSION_DENIED} if it is not.
     *
     * @see #checkUriPermission(Uri, int, int, int)
     */
    virtual CARAPI CheckCallingUriPermission(
        /* [in] */ IUri* uri,
        /* [in] */ Int32 modeFlags,
        /* [out] */ Int32* result);

    /**
     * Determine whether the calling process of an IPC <em>or you</em> has been granted
     * permission to access a specific URI.  This is the same as
     * {@sa #checkCallingUriPermission}, except it grants your own permissions
     * if you are not currently processing an IPC.  Use with care!
     *
     * @param uri The uri that is being checked.
     * @param modeFlags The type of access to grant.  May be one or both of
     * {@sa Intent#FLAG_GRANT_READ_URI_PERMISSION Intent.FLAG_GRANT_READ_URI_PERMISSION} or
     * {@sa Intent#FLAG_GRANT_WRITE_URI_PERMISSION Intent.FLAG_GRANT_WRITE_URI_PERMISSION}.
     *
     * @return Returns {@sa PackageManager#PERMISSION_GRANTED} if the caller
     * is allowed to access that uri, or
     * {@sa PackageManager#PERMISSION_DENIED} if it is not.
     *
     * @see #checkCallingUriPermission
     */
    virtual CARAPI CheckCallingOrSelfUriPermission(
        /* [in] */ IUri* uri,
        /* [in] */ Int32 modeFlags,
        /* [out] */ Int32* result);

    /**
     * Check both a Uri and normal permission.  This allows you to perform
     * both {@sa #checkPermission} and {@sa #checkUriPermission} in one
     * call.
     *
     * @param uri The Uri whose permission is to be checked, or null to not
     * do this check.
     * @param readPermission The permission that provides overall read access,
     * or null to not do this check.
     * @param writePermission The permission that provides overall write
     * acess, or null to not do this check.
     * @param pid The process ID being checked against.  Must be &gt; 0.
     * @param uid The user ID being checked against.  A uid of 0 is the root
     * user, which will pass every permission check.
     * @param modeFlags The type of access to grant.  May be one or both of
     * {@sa Intent#FLAG_GRANT_READ_URI_PERMISSION Intent.FLAG_GRANT_READ_URI_PERMISSION} or
     * {@sa Intent#FLAG_GRANT_WRITE_URI_PERMISSION Intent.FLAG_GRANT_WRITE_URI_PERMISSION}.
     *
     * @return Returns {@sa PackageManager#PERMISSION_GRANTED} if the caller
     * is allowed to access that uri or holds one of the given permissions, or
     * {@sa PackageManager#PERMISSION_DENIED} if it is not.
     */
    virtual CARAPI CheckUriPermission(
        /* [in] */ IUri* uri,
        /* [in] */ const String& readPermission,
        /* [in] */ const String& writePermission,
        /* [in] */ Int32 pid,
        /* [in] */ Int32 uid,
        /* [in] */ Int32 modeFlags,
        /* [out] */ Int32* result);

    /**
     * If a particular process and user ID has not been granted
     * permission to access a specific URI, throw {@sa
     * SecurityException}.  This only checks for permissions that have
     * been explicitly granted -- if the given process/uid has more
     * general access to the URI's content provider then this check
     * will always fail.
     *
     * @param uri The uri that is being checked.
     * @param pid The process ID being checked against.  Must be &gt; 0.
     * @param uid The user ID being checked against.  A uid of 0 is the root
     * user, which will pass every permission check.
     * @param modeFlags The type of access to grant.  May be one or both of
     * {@sa Intent#FLAG_GRANT_READ_URI_PERMISSION Intent.FLAG_GRANT_READ_URI_PERMISSION} or
     * {@sa Intent#FLAG_GRANT_WRITE_URI_PERMISSION Intent.FLAG_GRANT_WRITE_URI_PERMISSION}.
     * @param message A message to include in the exception if it is thrown.
     *
     * @see #checkUriPermission(Uri, int, int, int)
     */
    virtual CARAPI EnforceUriPermission(
        /* [in] */ IUri* uri,
        /* [in] */ Int32 pid,
        /* [in] */ Int32 uid,
        /* [in] */ Int32 modeFlags,
        /* [in] */ const String& message);

    /**
     * If the calling process and user ID has not been granted
     * permission to access a specific URI, throw {@sa
     * SecurityException}.  This is basically the same as calling
     * {@sa #enforceUriPermission(Uri, int, int, int, String)} with
     * the pid and uid returned by {@sa
     * android.os.Binder#getCallingPid} and {@sa
     * android.os.Binder#getCallingUid}.  One important difference is
     * that if you are not currently processing an IPC, this function
     * will always throw a SecurityException.
     *
     * @param uri The uri that is being checked.
     * @param modeFlags The type of access to grant.  May be one or both of
     * {@sa Intent#FLAG_GRANT_READ_URI_PERMISSION Intent.FLAG_GRANT_READ_URI_PERMISSION} or
     * {@sa Intent#FLAG_GRANT_WRITE_URI_PERMISSION Intent.FLAG_GRANT_WRITE_URI_PERMISSION}.
     * @param message A message to include in the exception if it is thrown.
     *
     * @see #checkCallingUriPermission(Uri, int)
     */
    virtual CARAPI EnforceCallingUriPermission(
        /* [in] */ IUri* uri,
        /* [in] */ Int32 modeFlags,
        /* [in] */ const String& message);

    /**
     * If the calling process of an IPC <em>or you</em> has not been
     * granted permission to access a specific URI, throw {@sa
     * SecurityException}.  This is the same as {@sa
     * #enforceCallingUriPermission}, except it grants your own
     * permissions if you are not currently processing an IPC.  Use
     * with care!
     *
     * @param uri The uri that is being checked.
     * @param modeFlags The type of access to grant.  May be one or both of
     * {@sa Intent#FLAG_GRANT_READ_URI_PERMISSION Intent.FLAG_GRANT_READ_URI_PERMISSION} or
     * {@sa Intent#FLAG_GRANT_WRITE_URI_PERMISSION Intent.FLAG_GRANT_WRITE_URI_PERMISSION}.
     * @param message A message to include in the exception if it is thrown.
     *
     * @see #checkCallingOrSelfUriPermission(Uri, int)
     */
    virtual CARAPI EnforceCallingOrSelfUriPermission(
        /* [in] */ IUri* uri,
        /* [in] */ Int32 modeFlags,
        /* [in] */ const String& message);

    /**
     * Enforce both a Uri and normal permission.  This allows you to perform
     * both {@sa #enforcePermission} and {@sa #enforceUriPermission} in one
     * call.
     *
     * @param uri The Uri whose permission is to be checked, or null to not
     * do this check.
     * @param readPermission The permission that provides overall read access,
     * or null to not do this check.
     * @param writePermission The permission that provides overall write
     * acess, or null to not do this check.
     * @param pid The process ID being checked against.  Must be &gt; 0.
     * @param uid The user ID being checked against.  A uid of 0 is the root
     * user, which will pass every permission check.
     * @param modeFlags The type of access to grant.  May be one or both of
     * {@sa Intent#FLAG_GRANT_READ_URI_PERMISSION Intent.FLAG_GRANT_READ_URI_PERMISSION} or
     * {@sa Intent#FLAG_GRANT_WRITE_URI_PERMISSION Intent.FLAG_GRANT_WRITE_URI_PERMISSION}.
     * @param message A message to include in the exception if it is thrown.
     *
     * @see #checkUriPermission(Uri, String, String, int, int, int)
     */
    virtual CARAPI EnforceUriPermission(
        /* [in] */ IUri* uri,
        /* [in] */ const String& readPermission,
        /* [in] */ const String& writePermission,
        /* [in] */ Int32 pid,
        /* [in] */ Int32 uid,
        /* [in] */ Int32 modeFlags,
        /* [in] */ const String& message);

    /**
     * Return a new Context object for the given application name.  This
     * Context is the same as what the named application gets when it is
     * launched, containing the same resources and class loader.  Each call to
     * this method returns a new instance of a Context object; Context objects
     * are not shared, however they share common state (Resources, ClassLoader,
     * etc) so the Context instance itself is fairly lightweight.
     *
     * <p>Throws {@sa PackageManager.NameNotFoundException} if there is no
     * application with the given package name.
     *
     * <p>Throws {@sa java.lang.SecurityException} if the Context requested
     * can not be loaded into the caller's process for security reasons (see
     * {@sa #CONTEXT_INCLUDE_CODE} for more information}.
     *
     * @param packageName Name of the application's package.
     * @param flags Option flags, one of {@sa #CONTEXT_INCLUDE_CODE}
     *              or {@sa #CONTEXT_IGNORE_SECURITY}.
     *
     * @return A Context for the application.
     *
     * @return ECode = NOERROR(0) if success, else as follows:
     * Value | Meaning |
     * :-|:------------|
     * java.lang.SecurityException
     * PackageManager.NameNotFoundException | if there is no application with
     * the given package name
     */
    virtual CARAPI CreatePackageContext(
        /* [in] */ const String& packageName,
        /* [in] */ Int32 flags,
        /* [out] */ IContext** ctx);

    /**
     * Similar to {@sa #createPackageContext(String, int)}, but with a
     * different {@sa UserHandle}. For example, {@sa #getContentResolver()}
     * will open any {@sa Uri} as the given user.
     *
     * @hide
     */
    virtual CARAPI CreatePackageContextAsUser(
        /* [in] */ const String& packageName,
        /* [in] */ Int32 flags,
        /* [in] */ IUserHandle* user,
        /* [out] */ IContext** ctx);

    /**
     * Return a new Context object for the current Context but whose resources
     * are adjusted to match the given Configuration.  Each call to this method
     * returns a new instance of a Context object; Context objects are not
     * shared, however common state (ClassLoader, other Resources for the
     * same configuration) may be so the Context itself can be fairly lightweight.
     *
     * @param overrideConfiguration A {@sa Configuration} specifying what
     * values to modify in the base Configuration of the original Context's
     * resources.  If the base configuration changes (such as due to an
     * orientation change), the resources of this context will also change except
     * for those that have been explicitly overridden with a value here.
     *
     * @return A Context with the given configuration override.
     */
    virtual CARAPI CreateConfigurationContext(
        /* [in] */ IConfiguration* overrideConfiguration,
        /* [out] */ IContext** ctx);

    /**
     * Return a new Context object for the current Context but whose resources
     * are adjusted to match the metrics of the given Display.  Each call to this method
     * returns a new instance of a Context object; Context objects are not
     * shared, however common state (ClassLoader, other Resources for the
     * same configuration) may be so the Context itself can be fairly lightweight.
     *
     * The returned display Context provides a {@sa WindowManager}
     * (see {@sa #getSystemService(String)}) that is configured to show windows
     * on the given display.  The WindowManager's {@sa WindowManager#getDefaultDisplay}
     * method can be used to retrieve the Display from the returned Context.
     *
     * @param display A {@sa Display} object specifying the display
     * for whose metrics the Context's resources should be tailored and upon which
     * new windows should be shown.
     *
     * @return A Context for the display.
     */
    virtual CARAPI CreateDisplayContext(
        /* [in] */ IDisplay* display,
        /* [out] */ IContext** ctx);

    /**
     * Gets the compatibility info holder for this context.  This information
     * is provided on a per-application basis and is used to simulate lower density
     * display metrics for legacy applications.
     *
     * @param displayId The display id for which to get compatibility info.
     * @return The compatibility info holder, or null if not required by the application.
     * @hide
     */
    virtual CARAPI GetCompatibilityInfo(
        /* [in] */ Int32 displayId,
        /* [out] */ ICompatibilityInfoHolder** infoHolder);

    /**
     * Indicates whether this Context is restricted.
     *
     * @return True if this Context is restricted, false otherwise.
     *
     * @see #CONTEXT_RESTRICTED
     */
    virtual CARAPI IsRestricted(
        /* [out] */ Boolean* isRestricted);

protected:
    /**
     * Set the base context for this ContextWrapper.  All calls will then be
     * delegated to the base context.  Throws
     * IllegalStateException if a base context has already been set.
     *
     * @param base The new base context for this wrapper.
     */
    CARAPI AttachBaseContext(
        /* [in] */ IContext* base);

private:
    CARAPI_(AutoPtr< ArrayOf<IComponentCallbacks*> >) CollectComponentCallbacks();

    CARAPI_(AutoPtr< ArrayOf<IActivityLifecycleCallbacks*> >) CollectActivityLifecycleCallbacks();

public:
    // AutoPtr<LoadedPkg> mLoadedPkg;

private:
    static const String TAG;

    AutoPtr<IContext> mBase;

    List< AutoPtr<IComponentCallbacks> > mComponentCallbacks;
    Object mComponentCallbacksLock;
    List< AutoPtr<IActivityLifecycleCallbacks> > mActivityLifecycleCallbacks;
    Object mActivityLifecycleCallbacksLock;
};

} // namespace App
} // namespace Droid
} // namespace Elastos

#endif //__ELASTOS_DROID_APP_APPLICATION_H__
