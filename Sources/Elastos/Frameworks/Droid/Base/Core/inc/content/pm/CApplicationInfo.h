
#ifndef __CAPPLICATIONINFO_H__
#define __CAPPLICATIONINFO_H__

#include "_CApplicationInfo.h"
#include "PackageItemInfo.h"

namespace Elastos {
namespace Droid {
namespace Content {
namespace Pm {

/**
 * Information you can retrieve about a particular application.  This
 * corresponds to information collected from the AndroidManifest.xml's
 * &lt;application&gt; tag.
 */
CarClass(CApplicationInfo), public PackageItemInfo
{
public:
    CApplicationInfo();

    ~CApplicationInfo();

    CARAPI constructor();

    CARAPI constructor(
        /* [in] */ IApplicationInfo* orig);

    CARAPI Dump(
        /* [in] */ IPrinter* pw,
        /* [in] */ const String& prefix);

    /**
     * @return true if "supportsRtl" has been set to true in the AndroidManifest
     * @hide
     */
    CARAPI HasRtlSupport(
        /* [out] */ Boolean* isSupport);

    CARAPI ToString(
        /* [out] */ String* str);

    CARAPI ReadFromParcel(
        /* [in] */ IParcel* source);

    CARAPI WriteToParcel(
        /* [in] */ IParcel* dest);

    /**
     * Retrieve the textual description of the application.  This
     * will call back on the given PackageManager to load the description from
     * the application.
     *
     * @param pm A PackageManager from which the label can be loaded; usually
     * the PackageManager from which you originally retrieved this item.
     *
     * @return Returns a CharSequence containing the application's description.
     * If there is no description, null is returned.
     */
    CARAPI LoadDescription(
            /* [in] */ IPackageManager* pm,
            /* [out] */ ICharSequence** label);

    CARAPI DisableCompatibilityMode();

    CARAPI LoadLabel(
        /* [in] */ IPackageManager* pm,
        /* [out] */ ICharSequence** label);

    CARAPI LoadIcon(
        /* [in] */ IPackageManager* pm,
        /* [out] */ IDrawable** icon);

    CARAPI LoadLogo(
        /* [in] */ IPackageManager* pm,
        /* [out] */ IDrawable** logo);

    CARAPI LoadXmlMetaData(
        /* [in] */ IPackageManager* pm,
        /* [in] */ const String& name,
        /* [out] */ IXmlResourceParser** resource);

    CARAPI GetName(
        /* [out] */ String* name);

    CARAPI SetName(
        /* [in] */ const String& name);

    CARAPI GetPackageName(
        /* [out] */ String* name);

    CARAPI SetPackageName(
        /* [in] */ const String& name);

    CARAPI GetLabelRes(
        /* [out] */ Int32* labelRes);

    CARAPI SetLabelRes(
        /* [in] */ Int32 labelRes);

    CARAPI GetNonLocalizedLabel(
        /* [out] */ ICharSequence** label);

    CARAPI SetNonLocalizedLabel(
        /* [in] */ ICharSequence* label);

    CARAPI GetIcon(
        /* [out] */ Int32* icon);

    CARAPI SetIcon(
        /* [in] */ Int32 icon);

    CARAPI GetLogo(
        /* [out] */ Int32* logo);

    CARAPI SetLogo(
        /* [in] */ Int32 logo);

    CARAPI GetMetaData(
        /* [out] */ IBundle** metaData);

    CARAPI SetMetaData(
        /* [in] */ IBundle* metaData);

    CARAPI GetTaskAffinity(
        /* [out] */ String* task);

    CARAPI SetTaskAffinity(
        /* [in] */ const String& task);

    CARAPI GetPermission(
        /* [out] */ String* permission);

    CARAPI SetPermission(
        /* [in] */ const String& permission);

    CARAPI GetProcessName(
        /* [out] */ String* name);

    CARAPI SetProcessName(
        /* [in] */ const String& name);

    CARAPI GetClassName(
        /* [out] */ String* name);

    CARAPI SetClassName(
        /* [in] */ const String& name);

    CARAPI GetDescriptionRes(
        /* [out] */ Int32* desRes);

    CARAPI SetDescriptionRes(
        /* [in] */ Int32 desRes);

    CARAPI GetTheme(
        /* [out] */ Int32* theme);

    CARAPI SetTheme(
        /* [in] */ Int32 theme);

    CARAPI GetManageSpaceActivityName(
        /* [out] */ String* name);

    CARAPI SetManageSpaceActivityName(
        /* [in] */ const String& name);

    CARAPI GetBackupAgentName(
        /* [out] */ String* name);

    CARAPI SetBackupAgentName(
        /* [in] */ const String& name);

    CARAPI GetUiOptions(
        /* [out] */ Int32* uiOptions);

    CARAPI SetUiOptions(
        /* [in] */ Int32 uiOptions);

    CARAPI GetFlags(
        /* [out] */ Int32* flags);

    CARAPI SetFlags(
        /* [in] */ Int32 flags);

    CARAPI GetRequiresSmallestWidthDp(
        /* [out] */ Int32* requiresSmallestWidthDp);

    CARAPI SetRequiresSmallestWidthDp(
        /* [in] */ Int32 requiresSmallestWidthDp);

    CARAPI GetCompatibleWidthLimitDp(
        /* [out] */ Int32* compatibleWidthLimitDp);

    CARAPI SetCompatibleWidthLimitDp(
        /* [in] */ Int32 compatibleWidthLimitDp);

    CARAPI GetLargestWidthLimitDp(
        /* [out] */ Int32* largestWidthLimitDp);

    CARAPI SetLargestWidthLimitDp(
        /* [in] */ Int32 largestWidthLimitDp);

    CARAPI GetSourceDir(
        /* [out] */ String* sourceDir);

    CARAPI SetSourceDir(
        /* [in] */ const String& sourceDir);

    CARAPI GetPublicSourceDir(
        /* [out] */ String* publicSourceDir);

    CARAPI SetPublicSourceDir(
        /* [in] */ const String& pubicSourceDir);

    CARAPI GetResourceDirs(
        /* [out, callee] */ ArrayOf<String>** resourceDirs);

    CARAPI SetResourceDirs(
        /* [in] */ ArrayOf<String>* resourceDirs);

    CARAPI GetSharedLibraryFiles(
        /* [out, callee] */ ArrayOf<String>** sharedLibraryFiles);

    CARAPI SetSharedLibraryFiles(
        /* [in] */ ArrayOf<String>* sharedLibraryFiles);

    CARAPI GetDataDir(
        /* [out] */ String* dataDir);

    CARAPI SetDataDir(
        /* [in] */ const String& dataDir);

    CARAPI GetNativeLibraryDir(
        /* [out] */ String* libraryDir);

    CARAPI SetNativeLibraryDir(
        /* [in] */ const String& libraryDir);

    CARAPI GetUid(
        /* [out] */ Int32* uid);

    CARAPI SetUid(
        /* [in] */ Int32 uid);

    CARAPI GetTargetSdkVersion(
        /* [out] */ Int32* sdkVersion);

    CARAPI SetTargetSdkVersion(
        /* [in] */ Int32 sdkVersion);

    CARAPI GetEnabled(
        /* [out] */ Boolean* enabled);

    CARAPI SetEnabled(
        /* [in] */ Boolean enabled);

    CARAPI GetEnabledSetting(
        /* [out] */ Int32* enabledSetting);

    CARAPI SetEnabledSetting(
        /* [in] */ Int32 enabledSetting);

    CARAPI GetInstallLocation(
        /* [out] */ Int32* location);

    CARAPI SetInstallLocation(
        /* [in] */ Int32 location);

protected:
    // @override
    CARAPI LoadDefaultIcon(
        /* [in] */ IPackageManager* pm,
        /* [out] */ IDrawable** icon);

    // @override
    CARAPI_(AutoPtr<IApplicationInfo>) GetApplicationInfo();

private:
    CARAPI_(Boolean) IsPackageUnavailable(
        /* [in] */ IPackageManager* pm);

public:
    /**
     * Default task affinity of all activities in this application. See
     * {@link ActivityInfo#taskAffinity} for more information.  This comes
     * from the "taskAffinity" attribute.
     */
    String mTaskAffinity;

    /**
     * Optional name of a permission required to be able to access this
     * application's components.  From the "permission" attribute.
     */
    String mPermission;

    /**
     * The name of the process this application should run in.  From the
     * "process" attribute or, if not set, the same as
     * <var>packageName</var>.
     */
    String mProcessName;

    /**
     * Class implementing the Application object.  From the "class"
     * attribute.
     */
    String mClassName;

    /**
     * A style resource identifier (in the package's resources) of the
     * description of an application.  From the "description" attribute
     * or, if not set, 0.
     */
    Int32 mDescriptionRes;

    /**
     * A style resource identifier (in the package's resources) of the
     * default visual theme of the application.  From the "theme" attribute
     * or, if not set, 0.
     */
    Int32 mTheme;

    /**
     * Class implementing the Application's manage space
     * functionality.  From the "manageSpaceActivity"
     * attribute. This is an optional attribute and will be null if
     * applications don't specify it in their manifest
     */
    String mManageSpaceActivityName;

    /**
     * Class implementing the Application's backup functionality.  From
     * the "backupAgent" attribute.  This is an optional attribute and
     * will be null if the application does not specify it in its manifest.
     *
     * <p>If android:allowBackup is set to false, this attribute is ignored.
     */
    String mBackupAgentName;

    /**
     * The default extra UI options for activities in this application.
     * Set from the {@link android.R.attr#uiOptions} attribute in the
     * activity's manifest.
     */
    Int32 mUiOptions;

    /**
     * Flags associated with the application.  Any combination of
     * {@link #FLAG_SYSTEM}, {@link #FLAG_DEBUGGABLE}, {@link #FLAG_HAS_CODE},
     * {@link #FLAG_PERSISTENT}, {@link #FLAG_FACTORY_TEST}, and
     * {@link #FLAG_ALLOW_TASK_REPARENTING}
     * {@link #FLAG_ALLOW_CLEAR_USER_DATA}, {@link #FLAG_UPDATED_SYSTEM_APP},
     * {@link #FLAG_TEST_ONLY}, {@link #FLAG_SUPPORTS_SMALL_SCREENS},
     * {@link #FLAG_SUPPORTS_NORMAL_SCREENS},
     * {@link #FLAG_SUPPORTS_LARGE_SCREENS}, {@link #FLAG_SUPPORTS_XLARGE_SCREENS},
     * {@link #FLAG_RESIZEABLE_FOR_SCREENS},
     * {@link #FLAG_SUPPORTS_SCREEN_DENSITIES}, {@link #FLAG_VM_SAFE_MODE},
     * {@link #FLAG_INSTALLED}.
     */
    Int32 mFlags;

    /**
     * The required smallest screen width the application can run on.  If 0,
     * nothing has been specified.  Comes from
     * {@link android.R.styleable#AndroidManifestSupportsScreens_requiresSmallestWidthDp
     * android:requiresSmallestWidthDp} attribute of the &lt;supports-screens&gt; tag.
     */
    Int32 mRequiresSmallestWidthDp;

    /**
     * The maximum smallest screen width the application is designed for.  If 0,
     * nothing has been specified.  Comes from
     * {@link android.R.styleable#AndroidManifestSupportsScreens_compatibleWidthLimitDp
     * android:compatibleWidthLimitDp} attribute of the &lt;supports-screens&gt; tag.
     */
    Int32 mCompatibleWidthLimitDp;

    /**
     * The maximum smallest screen width the application will work on.  If 0,
     * nothing has been specified.  Comes from
     * {@link android.R.styleable#AndroidManifestSupportsScreens_largestWidthLimitDp
     * android:largestWidthLimitDp} attribute of the &lt;supports-screens&gt; tag.
     */
    Int32 mLargestWidthLimitDp;

    /**
     * Full path to the location of this package.
     */
    String mSourceDir;

    /**
     * Full path to the location of the publicly available parts of this
     * package (i.e. the primary resource package and manifest).  For
     * non-forward-locked apps this will be the same as {@link #sourceDir).
     */
    String mPublicSourceDir;

    /**
     * Full paths to the locations of extra resource packages this application
     * uses. This field is only used if there are extra resource packages,
     * otherwise it is null.
     *
     * {@hide}
     */
    AutoPtr< ArrayOf<String> > mResourceDirs;

    /**
     * Paths to all shared libraries this application is linked against.  This
     * field is only set if the {@link PackageManager#GET_SHARED_LIBRARY_FILES
     * PackageManager.GET_SHARED_LIBRARY_FILES} flag was used when retrieving
     * the structure.
     */
    AutoPtr< ArrayOf<String> > mSharedLibraryFiles;

    /**
     * Full path to a directory assigned to the package for its persistent
     * data.
     */
    String mDataDir;

    /**
     * Full path to the directory where native JNI libraries are stored.
     */
    String mNativeLibraryDir;

    /**
     * The kernel user-ID that has been assigned to this application;
     * currently this is not a unique ID (multiple applications can have
     * the same uid).
     */
    Int32 mUid;

    /**
     * The minimum SDK version this application targets.  It may run on earlier
     * versions, but it knows how to work with any new behavior added at this
     * version.  Will be {@link android.os.Build.VERSION_CODES#CUR_DEVELOPMENT}
     * if this is a development build and the app is targeting that.  You should
     * compare that this number is >= the SDK version number at which your
     * behavior was introduced.
     */
    Int32 mTargetSdkVersion;

    /**
     * When false, indicates that all components within this application are
     * considered disabled, regardless of their individually set enabled status.
     */
    Boolean mEnabled;

    /**
     * For convenient access to the current enabled setting of this app.
     * @hide
     */
    Int32 mEnabledSetting;

    /**
     * For convenient access to package's install location.
     * @hide
     */
    Int32 mInstallLocation;
};

} // namespace Pm
} // namespace Content
} // namespace Droid
} // namespace Elastos

#endif // __CAPPLICATIONINFO_H__
