
#ifndef __ELASTOS_DROID_CONTENT_PM_PACKAGEPARSER_H__
#define __ELASTOS_DROID_CONTENT_PM_PACKAGEPARSER_H__

#include "elastos/droid/ext/frameworkext.h"
#include "elastos/droid/content/IntentFilter.h"
#include "elastos/droid/content/pm/PackageUserState.h"
#include <elastos/core/StringBuilder.h>
#include <elastos/utility/etl/List.h>
#include <elastos/utility/etl/HashSet.h>

using Elastos::Droid::Content::IIntent;
using Elastos::Droid::Content::Res::ITypedArray;
using Elastos::Droid::Content::Res::IAssetManager;
using Elastos::Droid::Content::Res::IResources;
using Elastos::Droid::Content::Res::IXmlResourceParser;
using Elastos::Droid::Os::IBundle;
using Elastos::Droid::Utility::IDisplayMetrics;
using Elastos::Droid::Utility::IAttributeSet;
using Elastos::Droid::Utility::IArraySet;
using Elastos::Droid::Utility::IArrayMap;

using Elastos::Core::IComparator;
using Elastos::Core::ICharSequence;
using Elastos::Core::StringBuilder;
using Elastos::Utility::Etl::List;
using Elastos::Utility::Etl::HashSet;
using Elastos::IO::IFile;
using Elastos::IO::IInputStream;
using Elastos::IO::IPrintWriter;
using Elastos::Utility::Jar::IStrictJarFile;
using Elastos::Utility::Zip::IZipEntry;
using Elastos::Security::IPublicKey;
using Elastos::Security::Cert::ICertificate;
using Org::Xmlpull::V1::IXmlPullParser;

namespace Elastos {
namespace Droid {
namespace Content {
namespace Pm {

typedef ArrayOf< AutoPtr< ArrayOf<ICertificate*> > > ICertificateArray2;

/**
 * Parser for package files (APKs) on disk. This supports apps packaged either
 * as a single "monolithic" APK, or apps packaged as a "cluster" of multiple
 * APKs in a single directory.
 * <p>
 * Apps packaged as multiple APKs always consist of a single "base" APK (with a
 * {@code null} split name) and zero or more "split" APKs (with unique split
 * names). Any subset of those split APKs are a valid install, as long as the
 * following constraints are met:
 * <ul>
 * <li>All APKs must have the exact same package name, version code, and signing
 * certificates.
 * <li>All APKs must have unique split names.
 * <li>All installations must contain a single base APK.
 * </ul>
 *
 * @hide
 */
class PackageParser
    : public Object
{
public:
    class ApkLite;
    class Package;
    class Permission;
    class PermissionGroup;
    class Activity;
    class Provider;
    class Service;
    class Instrumentation;
    class IntentInfo;
    class ActivityIntentInfo;
    class ServiceIntentInfo;
    class ProviderIntentInfo;

    /** @hide */
    class NewPermissionInfo : public Object
    {
    public:
        NewPermissionInfo(
            /* [in] */ const String& name,
            /* [in] */ Int32 sdkVersion,
            /* [in] */ Int32 fileVersion);

    public:
        String mName;
        Int32 mSdkVersion;
        Int32 mFileVersion;
    };

    /** @hide */
    class SplitPermissionInfo : public Object
    {
    public:
        SplitPermissionInfo(
            /* [in] */ const String& rootPerm,
            /* [in] */ ArrayOf<String>* newPerms,
            /* [in] */ Int32 targetSdk);

    public:
        String mRootPerm;
        AutoPtr< ArrayOf<String> > mNewPerms;
        Int32 mTargetSdk;
    };

    class ParsePackageItemArgs : public Object
    {
    public:
        ParsePackageItemArgs(
            /* [in] */ Package* owner,
            /* [in] */ ArrayOf<String>* outError,
            /* [in] */ Int32 nameRes,
            /* [in] */ Int32 labelRes,
            /* [in] */ Int32 iconRes,
            /* [in] */ Int32 logoRes,
            /* [in] */ Int32 bannerRes);

    public:
        Package* mOwner;
        AutoPtr< ArrayOf<String> > mOutError;
        Int32 mNameRes;
        Int32 mLabelRes;
        Int32 mIconRes;
        Int32 mLogoRes;
        Int32 mBannerRes;

        String mTag;
        AutoPtr<ITypedArray> mSa;
    };

    class ParseComponentArgs : public ParsePackageItemArgs
    {
    public:
        ParseComponentArgs(
            /* [in] */ Package* owner,
            /* [in] */ ArrayOf<String>* outError,
            /* [in] */ Int32 nameRes,
            /* [in] */ Int32 labelRes,
            /* [in] */ Int32 iconRes,
            /* [in] */ Int32 logoRes,
            /* [in] */ Int32 bannerRes,
            /* [in] */ ArrayOf<String>* sepProcesses,
            /* [in] */ Int32 processRes,
            /* [in] */ Int32 descriptionRes,
            /* [in] */ Int32 enabledRes);

    public:
        AutoPtr<ArrayOf<String> > mSepProcesses;
        Int32 mProcessRes;
        Int32 mDescriptionRes;
        Int32 mEnabledRes;
        Int32 mFlags;
    };

    /**
     * Lightweight parsed details about a single package.
     */
    class PackageLite
        : public Object
        , public IPackageLite
    {
    public:
        CAR_INTERFACE_DECL()

        PackageLite(
            /* [in] */ const String& codePath,
            /* [in] */ ApkLite* baseApk,
            /* [in] */ ArrayOf<String>* splitNames,
            /* [in] */ ArrayOf<String>* splitCodePaths);

        CARAPI_(AutoPtr<List<String> >) GetAllCodePaths();

        CARAPI GetPackageName(
            /* [out] */ String* name);

        CARAPI GetVersionCode(
            /* [out] */ Int32* code);

        CARAPI GetInstallLocation(
            /* [out] */ Int32* location);

        CARAPI GetVerifiers(
            /* [out, callee] */ ArrayOf<IVerifierInfo*> ** verifiers);

        CARAPI GetSplitNames(
            /* [out, callee] */ ArrayOf<String> ** splitNames);

        CARAPI GetCodePath(
            /* [out] */ String* path);

        CARAPI GetBaseCodePath(
            /* [out] */ String* path);

        CARAPI GetSplitCodePaths(
            /* [out, callee] */ ArrayOf<String> ** paths);

        CARAPI GetCoreApp(
            /* [out] */ Boolean* app);

        CARAPI GetMultiArch(
            /* [out] */ Boolean* arch);

    public:
        String mPackageName;
        Int32 mVersionCode;
        Int32 mInstallLocation;
        AutoPtr< ArrayOf<IVerifierInfo*> > mVerifiers;

        /** Names of any split APKs, ordered by parsed splitName */
        AutoPtr<ArrayOf<String> > mSplitNames;

        /**
         * Path where this package was found on disk. For monolithic packages
         * this is path to single base APK file; for cluster packages this is
         * path to the cluster directory.
         */
        String mCodePath;

        /** Path of base APK */
        String mBaseCodePath;
        /** Paths of any split APKs, ordered by parsed splitName */
        AutoPtr<ArrayOf<String> > mSplitCodePaths;

        Boolean mCoreApp;
        Boolean mMultiArch;
    };

    /**
     * Lightweight parsed details about a single APK file.
     */

    class ApkLite
        : public Object
    {
    public:
        ApkLite(
            /* [in] */ const String& codePath,
            /* [in] */ const String& packageName,
            /* [in] */ const String& splitName,
            /* [in] */ Int32 versionCode,
            /* [in] */ Int32 installLocation,
            /* [in] */ List<AutoPtr<IVerifierInfo> >* verifiers,
            /* [in] */ ArrayOf<ISignature*>* signatures,
            /* [in] */ Boolean coreApp,
            /* [in] */ Boolean multiArch);

    public:
        String mCodePath;
        String mPackageName;
        String mSplitName;
        Int32 mVersionCode;
        Int32 mInstallLocation;
        AutoPtr<ArrayOf<IVerifierInfo*> > mVerifiers;
        AutoPtr<ArrayOf<ISignature*> > mSignatures;
        Boolean mCoreApp;
        Boolean mMultiArch;
    };

    /**
     * Representation of a full package parsed from APK files on disk. A package
     * consists of a single base APK, and zero or more split APKs.
     */
    class Package
        : public Object
    {
    public:
        Package(
            /* [in] */ const String& name);

        ~Package();

        CARAPI_(void) SetPackageName(
            /* [in] */ const String& newName);

        CARAPI_(Boolean) HasComponentClassName(
            /* [in] */ const String& name);

        CARAPI ToString(
            /* [out] */ String* str);

        AutoPtr<List<String> > GetAllCodePaths();

        /**
         * Filtered set of {@link #getAllCodePaths()} that excludes
         * resource-only APKs.
         */
        AutoPtr<List<String> > GetAllCodePathsExcludingResourceOnly();

    public:
        String mPackageName;

        /** Names of any split APKs, ordered by parsed splitName */
        AutoPtr<ArrayOf<String> > mSplitNames;

        // TODO: work towards making these paths invariant

        /**
         * Path where this package was found on disk. For monolithic packages
         * this is path to single base APK file; for cluster packages this is
         * path to the cluster directory.
         */
        String mCodePath;

        /** Path of base APK */
        String mBaseCodePath;
        /** Paths of any split APKs, ordered by parsed splitName */
        AutoPtr<ArrayOf<String> > mSplitCodePaths;

        /** Flags of any split APKs; ordered by parsed splitName */
        AutoPtr<ArrayOf<Int32> > mSplitFlags;

        Boolean mBaseHardwareAccelerated;

        AutoPtr<IApplicationInfo> mApplicationInfo;

        List< AutoPtr<Permission> > mPermissions;
        List< AutoPtr<PermissionGroup> > mPermissionGroups;
        List< AutoPtr<Activity> > mActivities;
        List< AutoPtr<Activity> > mReceivers;
        List< AutoPtr<Provider> > mProviders;
        List< AutoPtr<Service> > mServices;
        List< AutoPtr<Instrumentation> > mInstrumentation;

        List<String> mRequestedPermissions;
        List<Boolean> mRequestedPermissionsRequired;

        AutoPtr< List<String> > mProtectedBroadcasts;

        AutoPtr< List<String> > mLibraryNames;//
        AutoPtr< List<String> > mUsesLibraries;
        AutoPtr< List<String> > mUsesOptionalLibraries;
        AutoPtr< ArrayOf<String> > mUsesLibraryFiles;

        AutoPtr< List< AutoPtr<ActivityIntentInfo> > > mPreferredActivityFilters;

        AutoPtr< List<String> > mOriginalPackages;
        String mRealPackage;
        AutoPtr< List<String> > mAdoptPermissions;

        // We store the application meta-data independently to avoid multiple unwanted references
        AutoPtr<IBundle> mAppMetaData;

        // If this is a 3rd party app, this is the path of the zip file.
        String mPath;

        // The version code declared for this package.
        Int32 mVersionCode;

        // The version name declared for this package.
        String mVersionName;

        // The shared user id that this package wants to use.
        String mSharedUserId;

        // The shared user label that this package wants to use.
        Int32 mSharedUserLabel;

        // Signatures that were read from the package.
        AutoPtr< ArrayOf<ISignature*> > mSignatures;
        AutoPtr<ICertificateArray2> mCertificates;

        // For use by package manager service for quick lookup of
        // preferred up order.
        Int32 mPreferredOrder;

        // For use by package manager to keep track of where it needs to do dexopt.
        // public final ArraySet<String> mDexOptPerformed = new ArraySet<>(4);
        AutoPtr<IArraySet> mDexOptPerformed;

        // For use by package manager to keep track of when a package was last used.
        Int64 mLastPackageUsageTimeInMills;

        // // User set enabled state.
        // Int32 mSetEnabled;

        // Additional data supplied by callers.
        AutoPtr<IInterface> mExtras; //this member point to PackageSetting who has strong reference each other

        // Whether an operation is currently pending on this package
        Boolean mOperationPending;

        /*
         *  Applications hardware preferences
         */
        AutoPtr <List< AutoPtr<IConfigurationInfo> > > mConfigPreferences;

        /*
         *  Applications requested features
         */
        AutoPtr< List< AutoPtr<IFeatureInfo> > > mReqFeatures;

        // Applications requested feature groups
        AutoPtr< List< AutoPtr<IFeatureGroupInfo> > > mFeatureGroups;

        Int32 mInstallLocation;

        Boolean mCoreApp;

        /* An app that's required for all users and cannot be uninstalled for a user */
        Boolean mRequiredForAllUsers;

        /* The restricted account authenticator type that is used by this application */
        String mRestrictedAccountType;

        /* The required account type without which this application will not function */
        String mRequiredAccountType;

        /**
         * Digest suitable for comparing whether this package's manifest is the
         * same as another.
         */
        AutoPtr<IManifestDigest> mManifestDigest;

        String mOverlayTarget;
        Int32 mOverlayPriority;
        Boolean mTrustedOverlay;

        /**
         * Data used to feed the KeySetManagerService
         */
        // public ArraySet<PublicKey> mSigningKeys;
        // public ArraySet<String> mUpgradeKeySets;
        // public ArrayMap<String, ArraySet<PublicKey>> mKeySetMapping;

        AutoPtr<IArraySet> mSigningKeys;
        AutoPtr<IArraySet> mUpgradeKeySets;
        AutoPtr<IArrayMap> mKeySetMapping;

        /**
         * The install time abi override for this package, if any.
         *
         * TODO: This seems like a horrible place to put the abiOverride because
         * this isn't something the packageParser parsers. However, this fits in with
         * the rest of the PackageManager where package scanning randomly pushes
         * and prods fields out of {@code this.applicationInfo}.
         */
        String mCpuAbiOverride;

        // for epk
        Boolean mIsEpk;
    };

    template <typename II>
    class Component : public Object
    {
    public:
        Component(
            /* [in] */ Package* owner);

        Component(
            /* [in] */ ParsePackageItemArgs* args,
            /* [in, out] */ IPackageItemInfo* outInfo);

        Component(
            /* [in] */ ParseComponentArgs* args,
            /* [in] */ IComponentInfo* outInfo);

        Component(
            /* [in] */ Component<II>* clone);

        virtual ~Component();

        virtual CARAPI_(AutoPtr<IComponentName>) GetComponentName();

        CARAPI_(void) AppendComponentShortName(
            /* [in] */ StringBuilder* sb);

        CARAPI_(void) PrintComponentShortName(
            /* [in] */ IPrintWriter* pw);

        virtual CARAPI_(void) SetPackageName(
            /* [in] */ const String& PackageName);

    private:
        void Init(
            /* [in] */ ParsePackageItemArgs* args,
            /* [in, out] */ IPackageItemInfo* outInfo);

    public:
        Package* mOwner;
        List< AutoPtr<II> > mIntents;
        String mClassName;
        AutoPtr<IBundle> mMetaData;

        AutoPtr<IComponentName> mComponentName;
        String mComponentShortName;
    };

    class Permission : public Component<IntentInfo>
    {
    public:
        Permission(
            /* [in] */ Package* owner);

        Permission(
            /* [in] */ Package* owner,
            /* [in] */ IPermissionInfo* info);

        ~Permission();

        CARAPI_(void) SetPackageName(
            /* [in] */ const String& PackageName);

        CARAPI ToString(
            /* [out] */ String* str);

    public:
        AutoPtr<IPermissionInfo> mInfo;
        Boolean mTree;
        AutoPtr<PermissionGroup> mGroup;
    };

    class PermissionGroup : public Component<IntentInfo>
    {
    public:
        PermissionGroup(
            /* [in] */ Package* owner);

        PermissionGroup(
            /* [in] */ Package* owner,
            /* [in] */ IPermissionGroupInfo* info);

        ~PermissionGroup();

        CARAPI_(void) SetPackageName(
            /* [in] */ const String& PackageName);

        CARAPI ToString(
            /* [out] */ String* str);

    public:
        AutoPtr<IPermissionGroupInfo> mInfo;
    };

    class Activity : public Component<ActivityIntentInfo>
    {
    public:
        Activity(
            /* [in] */ ParseComponentArgs* args,
            /* [in] */ IActivityInfo* info);

        CARAPI_(void) SetPackageName(
            /* [in] */ const String& capName);

        CARAPI ToString(
            /* [out] */ String* str);

    public:
        AutoPtr<IActivityInfo> mInfo;
    };

    class Service : public Component<ServiceIntentInfo>
    {
    public:
        Service(
            /* [in] */ ParseComponentArgs* args,
            /* [in] */ IServiceInfo* info);

        CARAPI_(void) SetPackageName(
            /* [in] */ const String& capName);

        CARAPI ToString(
            /* [out] */ String* str);

    public:
        AutoPtr<IServiceInfo> mInfo;
    };

    class Provider : public Component<ProviderIntentInfo>
    {
    public:
        Provider(
            /* [in] */ Provider* existingProvider);

        Provider(
            /* [in] */ ParseComponentArgs* args,
            /* [in] */ IProviderInfo* info);

        CARAPI_(void) SetPackageName(
            /* [in] */ const String& capName);

        CARAPI ToString(
            /* [out] */ String* str);

    public:
        AutoPtr<IProviderInfo> mInfo;
        Boolean mSyncable;
    };

    class Instrumentation : public Component<IntentInfo>
    {
    public:
        Instrumentation(
            /* [in] */ ParsePackageItemArgs* args,
            /* [in] */ IInstrumentationInfo* info);

        CARAPI_(void) SetPackageName(
            /* [in] */ const String& capName);

        CARAPI ToString(
            /* [out] */ String* str);

    public:
        AutoPtr<IInstrumentationInfo> mInfo;
    };

    class IntentInfo
        : public IntentFilter
    {
    public:
        IntentInfo();

    public:
        Boolean mHasDefault;
        Int32 mLabelRes;
        AutoPtr<ICharSequence> mNonLocalizedLabel;
        Int32 mIcon;
        Int32 mLogo;
        Int32 mBanner;
        Int32 mPreferred;
    };

    class ActivityIntentInfo : public IntentInfo
    {
    public:
        ActivityIntentInfo(
            /* [in] */ Activity* activity);

        CARAPI ToString(
            /* [out] */ String* str);

    public:
        Activity* mActivity;
    };

    class ServiceIntentInfo : public IntentInfo
    {
    public:
        ServiceIntentInfo(
            /* [in] */ Service* service);

        CARAPI ToString(
            /* [out] */ String* str);

    public:
        Service* mService;
    };


    class ProviderIntentInfo : public IntentInfo
    {
    public:
        ProviderIntentInfo(
            /* [in] */ Provider* provider);

        CARAPI ToString(
            /* [out] */ String* str);

    public:
        Provider* mProvider;
    };

    /**
     * Used to sort a set of APKs based on their split names, always placing the
     * base APK (with {@code null} split name) first.
     */
    class SplitNameComparator
        : public Object
        , public IComparator {
    public:
        CAR_INTERFACE_DECL()

        CARAPI Compare(
            /* [in] */ IInterface* lhs,
            /* [in] */ IInterface* rhs,
            /* [out] */ Int32* result);
    };

public:

    PackageParser();

    CARAPI_(void) SetSeparateProcesses(
        /* [in] */ ArrayOf<String>* procs);

    /**
     * Flag indicating this parser should only consider apps with
     * {@code coreApp} manifest attribute to be valid apps. This is useful when
     * creating a minimalist boot environment.
     */
    CARAPI_(void) SetOnlyCoreApps(
        /* [in] */ Boolean onlyCoreApps);

    CARAPI_(void) SetDisplayMetrics(
        /* [in] */ IDisplayMetrics* metrics);

    static CARAPI_(Boolean) IsApkFile(
        /* [in] */ IFile* file);

    /**
     * Generate and return the {@link PackageInfo} for a parsed package.
     *
     * @param p the parsed package.
     * @param flags indicating which optional information is included.
     */
    static CARAPI_(AutoPtr<IPackageInfo>) GeneratePackageInfo(
        /* [in] */ Package* p,
        /* [in] */ ArrayOf<Int32>* gids,
        /* [in] */ Int32 flags,
        /* [in] */ Int64 firstInstallTime,
        /* [in] */ Int64 lastUpdateTime,
        /* [in] */ HashSet<String>* grantedPermissions,
        /* [in] */ PackageUserState* state);

    static CARAPI_(Boolean) IsAvailable(
        /* [in] */ PackageUserState* state);

    static CARAPI_(AutoPtr<IPackageInfo>) GeneratePackageInfo(
        /* [in] */ Package* c,
        /* [in] */ ArrayOf<Int32>* gids,
        /* [in] */ Int32 flags,
        /* [in] */ Int64 firstInstallTime,
        /* [in] */ Int64 lastUpdateTime,
        /* [in] */ HashSet<String>* grantedPermissions,
        /* [in] */ PackageUserState* state,
        /* [in] */ Int32 userId);

    /**
     * Parse only lightweight details about the package at the given location.
     * Automatically detects if the package is a monolithic style (single APK
     * file) or cluster style (directory of APKs).
     * <p>
     * This performs sanity checking on cluster style packages, such as
     * requiring identical package name and version codes, a single base APK,
     * and unique split names.
     *
     * @see PackageParser#parsePackage(File, int)
     */
    static CARAPI ParsePackageLite(
        /* [in] */ IFile* packageFile,
        /* [in] */ Int32 flags,
        /* [in] */ ArrayOf<Byte>* readBuffer,
        /* [out] */ IPackageLite** pkgLite);

    /**
     * Parse the package at the given location. Automatically detects if the
     * package is a monolithic style (single APK file) or cluster style
     * (directory of APKs).
     * <p>
     * This performs sanity checking on cluster style packages, such as
     * requiring identical package name and version codes, a single base APK,
     * and unique split names.
     * <p>
     * Note that this <em>does not</em> perform signature verification; that
     * must be done separately in {@link #collectCertificates(Package, int)}.
     *
     * @see #parsePackageLite(File, int)
     */
    CARAPI ParsePackage(
        /* [in] */ IFile* packageFile,
        /* [in] */ Int32 flags,
        /* [in] */ ArrayOf<Byte>* readBuffer,
        /* [out] */ Package** pkgLite);

    /**
     * Gathers the {@link ManifestDigest} for {@code pkg} if it exists in the
     * APK. If it successfully scanned the package and found the
     * {@code AndroidManifest.xml}, {@code true} is returned.
     */
    CARAPI CollectManifestDigest(
        /* [in] */ Package* pkg);

    /**
     * Collect certificates from all the APKs described in the given package,
     * populating {@link Package#mSignatures}. This also asserts that all APK
     * contents are signed correctly and consistently.
     */
    CARAPI CollectCertificates(
        /* [in] */ Package* pkg,
        /* [in] */ Int32 flags,
        /* [in] */ ArrayOf<Byte>* readBuffer);

    /**
     * Utility method that retrieves lightweight details about a single APK
     * file, including package name, split name, and install location.
     *
     * @param apkFile path to a single APK
     * @param flags optional parse flags, such as
     *            {@link #PARSE_COLLECT_CERTIFICATES}
     */
    static CARAPI ParseApkLite(
        /* [in] */ IFile* apkFile,
        /* [in] */ Int32 flags,
        /* [in] */ ArrayOf<Byte>* readBuffer,
        /* [out] */ PackageParser::ApkLite** apkLite);

    /**
     * Temporary.
     */
    static CARAPI_(AutoPtr<ISignature>) StringToSignature(
        /* [in] */ const String& str);

    static CARAPI_(AutoPtr<IApplicationInfo>) GenerateApplicationInfo(
        /* [in] */ Package* p,
        /* [in] */ Int32 flags,
        /* [in] */ PackageUserState* state);

    static CARAPI_(AutoPtr<IApplicationInfo>) GenerateApplicationInfo(
        /* [in] */ Package* p,
        /* [in] */ Int32 flags,
        /* [in] */ PackageUserState* state,
        /* [in] */ Int32 userId);

    static CARAPI_(AutoPtr<IApplicationInfo>) GenerateApplicationInfo(
        /* [in] */ IApplicationInfo* ai,
        /* [in] */ Int32 flags,
        /* [in] */ PackageUserState* state,
        /* [in] */ Int32 userId);

    static CARAPI_(AutoPtr<IPermissionInfo>) GeneratePermissionInfo(
        /* [in] */ Permission* p,
        /* [in] */ Int32 flags);

    static CARAPI_(AutoPtr<IPermissionGroupInfo>) GeneratePermissionGroupInfo(
        /* [in] */ PermissionGroup* pg,
        /* [in] */ Int32 flags);

    static CARAPI_(AutoPtr<IActivityInfo>) GenerateActivityInfo(
        /* [in] */ Activity* activity,
        /* [in] */ Int32 flags,
        /* [in] */ PackageUserState* state,
        /* [in] */ Int32 userId);

    static CARAPI_(AutoPtr<IActivityInfo>) GenerateActivityInfo(
        /* [in] */ IActivityInfo* ai,
        /* [in] */ Int32 flags,
        /* [in] */ PackageUserState* state,
        /* [in] */ Int32 userId);

    static CARAPI_(AutoPtr<IServiceInfo>) GenerateServiceInfo(
        /* [in] */ Service* service,
        /* [in] */ Int32 flags,
        /* [in] */ PackageUserState* state,
        /* [in] */ Int32 userId);

    static CARAPI_(AutoPtr<IProviderInfo>) GenerateProviderInfo(
        /* [in] */ Provider* provider,
        /* [in] */ Int32 flags,
        /* [in] */ PackageUserState* state,
        /* [in] */ Int32 userId);

    static CARAPI_(AutoPtr<IInstrumentationInfo>) GenerateInstrumentationInfo(
        /* [in] */ Instrumentation* i,
        /* [in] */ Int32 flags);

    static CARAPI_(void) SetCompatibilityModeEnabled(
        /* [in] */ Boolean compatibilityModeEnabled);

    static CARAPI ReadFullyIgnoringContents(
        /* [in] */ IInputStream* in,
        /* [in] */ ArrayOf<Byte>* readBuffer,
        /* [out] */ Int64* result);

    static CloseQuietly(
        /* [in] */ IStrictJarFile* jarFile);

    /**
     * Parse the given APK file, treating it as as a single monolithic package.
     * <p>
     * Note that this <em>does not</em> perform signature verification; that
     * must be done separately in {@link #collectCertificates(Package, int)}.
     *
     * @deprecated external callers should move to
     *             {@link #parsePackage(File, int)}. Eventually this method will
     *             be marked private.
     */
    CARAPI ParseMonolithicPackage(
        /* [in] */ IFile* packageFile,
        /* [in] */ Int32 flags,
        /* [in] */ ArrayOf<Byte>* readBuffer,
        /* [out] */ Package** pkgLite);

    static CARAPI ParsePublicKey(
        /* [in] */ const String& encodedPublicKey,
        /* [out] */ IPublicKey** key);

    static CARAPI_(Int32) GetParseError();

private:
    static CARAPI_(Boolean) IsApkPath(
        /* [in] */ const String& name);

    /**
     * Returns true if the package is installed and not hidden, or if the caller
     * explicitly wanted all uninstalled and hidden packages as well.
     */
    static CARAPI_(Boolean) CheckUseInstalledOrHidden(
        /* [in] */ Int32 flags,
        /* [in] */ PackageUserState* state);

    static CARAPI LoadCertificates(
        /* [in] */ IStrictJarFile* jarFile,
        /* [in] */ IZipEntry* je,
        /* [in] */ ArrayOf<Byte>* readBuffer,
        /* [in] */ ICertificateArray2** result);

    static CARAPI_(String) ValidateName(
        /* [in] */ const String& name,
        /* [in] */ Boolean requiresSeparator);

    static CARAPI ParsePackageSplitNames(
        /* [in] */ IXmlPullParser* parser,
        /* [in] */ IAttributeSet* attrs,
        /* [in] */ Int32 flags,
        /* [out, callee] */ ArrayOf<String>** pairString);

    static CARAPI ParseMonolithicPackageLite(
        /* [in] */ IFile* packageFile,
        /* [in] */ Int32 flags,
        /* [in] */ ArrayOf<Byte>* readBuffer,
        /* [out] */ IPackageLite** pkgLite);

    static CARAPI ParseClusterPackageLite(
        /* [in] */ IFile* packageFile,
        /* [in] */ Int32 flags,
        /* [in] */ ArrayOf<Byte>* readBuffer,
        /* [out] */ IPackageLite** pkgLite);

    /**
     * Parse all APKs contained in the given directory, treating them as a
     * single package. This also performs sanity checking, such as requiring
     * identical package name and version codes, a single base APK, and unique
     * split names.
     * <p>
     * Note that this <em>does not</em> perform signature verification; that
     * must be done separately in {@link #collectCertificates(Package, int)}.
     */
    CARAPI ParseClusterPackage(
        /* [in] */ IFile* packageFile,
        /* [in] */ Int32 flags,
        /* [in] */ ArrayOf<Byte>* readBuffer,
        /* [out] */ Package** pkgLite);

    CARAPI LoadApkIntoAssetManager(
        /* [in] */ IAssetManager* assets,
        /* [in] */ const String& apkPath,
        /* [in] */ Int32 flags,
        /* [out] */ Int32* result);

    CARAPI ParseBaseApk(
        /* [in] */ IFile* apkFile,
        /* [in] */ IAssetManager* assets,
        /* [in] */ Int32 flags,
        /* [out] */ Package** pkg);

    CARAPI ParseSplitApk(
        /* [in] */ Package* pkg,
        /* [in] */ Int32 splitIndex,
        /* [in] */ IAssetManager* assets,
        /* [in] */ Int32 flags);

    /**
     * Parse the manifest of a <em>split APK</em>.
     * <p>
     * Note that split APKs have many more restrictions on what they're capable
     * of doing, so many valid features of a base APK have been carefully
     * omitted here.
     */
    CARAPI ParseSplitApk(
        /* [in] */ Package* pkg,
        /* [in] */ IResources* res,
        /* [in] */ IXmlResourceParser* parser,
        /* [in] */ Int32 flags,
        /* [in] */ Int32 splitIndex,
        /* [in] */ ArrayOf<String>* outError,
        /* [out] */ Package** result);

    static CARAPI CollectCertificates(
        /* [in] */ Package* pkg,
        /* [in] */ IFile* apkFile,
        /* [in] */ Int32 flags,
        /* [in] */ ArrayOf<Byte>* readBuffer);

    static AutoPtr<ArrayOf<ISignature*> > ConvertToSignatures(
        /* [in] */ ICertificateArray2 * certs);

    static CARAPI ParseApkLite(
        /* [in] */ const String& codePath,
        /* [in] */ IResources* res,
        /* [in] */ IXmlResourceParser* parser,
        /* [in] */ IAttributeSet* attrs,
        /* [in] */ Int32 flags,
        /* [in] */ ArrayOf<ISignature*>* signatures,
        /* [out] */ ApkLite** apkLite);

    /**
     * Parse the manifest of a <em>base APK</em>.
     * <p>
     * When adding new features, carefully consider if they should also be
     * supported by split APKs.
     */
    CARAPI ParseBaseApk(
        /* [in] */ IResources* res,
        /* [in] */ IXmlResourceParser* parser,
        /* [in] */ Int32 flags,
        /* [in] */ ArrayOf<String>* outError,
        /* [out] */ Package** pkg);

    CARAPI ParseUsesFeature(
        /* [in] */ IResources* res,
        /* [in] */ IAttributeSet* attrs,
        /* [out] */ IFeatureInfo** info);

    CARAPI ParseUsesPermission(
        /* [in] */ Package* pkg,
        /* [in] */ IResources* res,
        /* [in] */ IXmlResourceParser* parser,
        /* [in] */ IAttributeSet* attrs,
        /* [in] */ ArrayOf<String>* outError,
        /* [out] */ Boolean* result);

    static CARAPI_(String) BuildClassName(
        /* [in] */ const String& pkg,
        /* [in] */ const String& clsSeq,
        /* [out] */ ArrayOf<String>* outError);

    static CARAPI_(String) BuildCompoundName(
        /* [in] */ const String& pkg,
        /* [in] */ ICharSequence* procSeq,
        /* [in] */ const String& type,
        /* [out] */ ArrayOf<String>* outError);

    static CARAPI_(String) BuildProcessName(
        /* [in] */ const String& pkg,
        /* [in] */ const String& defProc,
        /* [in] */ ICharSequence* procSeq,
        /* [in] */ Int32 flags,
        /* [in] */ ArrayOf<String>* separateProcesses,
        /* [out] */ ArrayOf<String>* outError);

    static CARAPI_(String) BuildTaskAffinityName(
        /* [in] */ const String& pkg,
        /* [in] */ const String& defProc,
        /* [in] */ ICharSequence* procSeq,
        /* [out] */ ArrayOf<String>* outError);

    CARAPI ParseKeySets(
        /* [in] */ Package* owner,
        /* [in] */ IResources* res,
        /* [in] */ IXmlResourceParser* parser,
        /* [in] */ IAttributeSet* attrs,
        /* [in] */ ArrayOf<String>* outError,
        /* [out] */ Boolean* result);

    CARAPI ParsePermissionGroup(
        /* [in] */ Package* owner,
        /* [in] */ Int32 flags,
        /* [in] */ IResources* res,
        /* [in] */ IXmlPullParser* parser,
        /* [in] */ IAttributeSet* attrs,
        /* [out] */ ArrayOf<String>* outError,
        /* [out] */ PermissionGroup** group);

    CARAPI ParsePermission(
        /* [in] */ Package* owner,
        /* [in] */ IResources* res,
        /* [in] */ IXmlPullParser* parser,
        /* [in] */ IAttributeSet* attrs,
        /* [out] */ ArrayOf<String>* outError,
        /* [out] */ Permission** permission);

    CARAPI ParsePermissionTree(
        /* [in] */ Package* owner,
        /* [in] */ IResources* res,
        /* [in] */ IXmlPullParser* parser,
        /* [in] */ IAttributeSet* attrs,
        /* [out] */ ArrayOf<String>* outError,
        /* [out] */ Permission** permission);

    CARAPI_(AutoPtr<Instrumentation>) ParseInstrumentation(
        /* [in] */ Package* owner,
        /* [in] */ IResources* res,
        /* [in] */ IXmlPullParser* parser,
        /* [in] */ IAttributeSet* attrs,
        /* [out] */ ArrayOf<String>* outError);

    /**
     * Parse the {@code application} XML tree at the current parse location in a
     * <em>base APK</em> manifest.
     * <p>
     * When adding new features, carefully consider if they should also be
     * supported by split APKs.
     */
    CARAPI_(Boolean) ParseBaseApplication(
        /* [in] */ Package* owner,
        /* [in] */ IResources* res,
        /* [in] */ IXmlPullParser* parser,
        /* [in] */ IAttributeSet* attrs,
        /* [in] */ Int32 flags,
        /* [out] */ ArrayOf<String>* outError);

    /**
     * Parse the {@code application} XML tree at the current parse location in a
     * <em>split APK</em> manifest.
     * <p>
     * Note that split APKs have many more restrictions on what they're capable
     * of doing, so many valid features of a base APK have been carefully
     * omitted here.
     */
    CARAPI ParseSplitApplication(
        /* [in] */ Package* owner,
        /* [in] */ IResources* res,
        /* [in] */ IXmlPullParser* parser,
        /* [in] */ IAttributeSet* attrs,
        /* [in] */ Int32 flags,
        /* [in] */ Int32 splitIndex,
        /* [out] */ ArrayOf<String>* outError,
        /* [out] */ Boolean* result);

    CARAPI_(Boolean) ParsePackageItemInfo(
        /* [in] */ Package* owner,
        /* [in] */ IPackageItemInfo* outInfo,
        /* [out] */ ArrayOf<String>* outError,
        /* [in] */ const String& tag,
        /* [in] */ ITypedArray* sa,
        /* [in] */ Int32 nameRes,
        /* [in] */ Int32 labelRes,
        /* [in] */ Int32 iconRes,
        /* [in] */ Int32 logoRes,
        /* [in] */ Int32 bannerRes);

    CARAPI_(AutoPtr<Activity>) ParseActivity(
        /* [in] */ Package* owner,
        /* [in] */ IResources* res,
        /* [in] */ IXmlPullParser* parser,
        /* [in] */ IAttributeSet* attrs,
        /* [in] */ Int32 flags,
        /* [out] */ ArrayOf<String>* outError,
        /* [in] */ Boolean receiver,
        /* [in] */ Boolean hardwareAccelerated);

    CARAPI_(AutoPtr<Activity>) ParseActivityAlias(
        /* [in] */ Package* owner,
        /* [in] */ IResources* res,
        /* [in] */ IXmlPullParser* parser,
        /* [in] */ IAttributeSet* attrs,
        /* [in] */ Int32 flags,
        /* [out] */ ArrayOf<String>* outError);

    CARAPI_(AutoPtr<Provider>) ParseProvider(
        /* [in] */ Package* owner,
        /* [in] */ IResources* res,
        /* [in] */ IXmlPullParser* parser,
        /* [in] */ IAttributeSet* attrs,
        /* [in] */ Int32 flags,
        /* [out] */ ArrayOf<String>* outError);

    CARAPI_(Boolean) ParseProviderTags(
        /* [in] */ IResources* res,
        /* [in] */ IXmlPullParser* parser,
        /* [in] */ IAttributeSet* attrs,
        /* [in] */ Provider* outInfo,
        /* [in] */ ArrayOf<String>* outError);

    CARAPI_(AutoPtr<Service>) ParseService(
        /* [in] */ Package* owner,
        /* [in] */ IResources* res,
        /* [in] */ IXmlPullParser* parser,
        /* [in] */ IAttributeSet* attrs,
        /* [in] */ Int32 flags,
        /* [out] */ ArrayOf<String>* outError);

    template <typename T>
    CARAPI_(Boolean) ParseAllMetaData(
        /* [in] */ IResources* res,
        /* [in] */ IXmlPullParser* parser,
        /* [in] */ IAttributeSet* attrs,
        /* [in] */ const String& tag,
        /* [in] */ Component<T>* outInfo,
        /* [out] */ ArrayOf<String>* outError);

    CARAPI_(AutoPtr<IBundle>) ParseMetaData(
        /* [in] */ IResources* res,
        /* [in] */ IXmlPullParser* parser,
        /* [in] */ IAttributeSet* attrs,
        /* [in] */ IBundle* data,
        /* [out] */ ArrayOf<String>* outError);

    static CARAPI_(AutoPtr<IVerifierInfo>) ParseVerifier(
        /* [in] */ IResources* res,
        /* [in] */ IXmlPullParser* parser,
        /* [in] */ IAttributeSet* attrs,
        /* [in] */ Int32 flags);

    CARAPI_(Boolean) ParseIntent(
        /* [in] */ IResources* res,
        /* [in] */ IXmlPullParser* parser,
        /* [in] */ IAttributeSet* attrs,
        /* [in] */ Boolean allowGlobs,
        /* [in] */ IntentInfo* outInfo,
        /* [out] */ ArrayOf<String>* outError);

    static CARAPI_(Boolean) CopyNeeded(
        /* [in] */ Int32 flags,
        /* [in] */ Package* p,
        /* [in] */ PackageUserState* state,
        /* [in] */ IBundle* metaData,
        /* [in] */ Int32 userId);

    static CARAPI UpdateApplicationInfo(
        /* [in] */ IApplicationInfo* ai,
        /* [in] */ Int32 flags,
        /* [in] */ PackageUserState* state);

public:
    /**
     * List of new permissions that have been added since 1.0.
     * NOTE: These must be declared in SDK version order, with permissions
     * added to older SDKs appearing before those added to newer SDKs.
     * If sdkVersion is 0, then this is not a permission that we want to
     * automatically add to older apps, but we do want to allow it to be
     * granted during a platform update.
     * @hide
     */
    static const AutoPtr< ArrayOf<NewPermissionInfo*> > NEW_PERMISSIONS;

    /**
     * List of permissions that have been split into more granular or dependent
     * permissions.
     * @hide
     */
    static const AutoPtr< ArrayOf<SplitPermissionInfo*> > SPLIT_PERMISSIONS;

    static const Int32 PARSE_IS_SYSTEM;// = 1<<0;
    static const Int32 PARSE_CHATTY;// = 1<<1;
    static const Int32 PARSE_MUST_BE_APK;// = 1<<2;
    static const Int32 PARSE_IGNORE_PROCESSES;// = 1<<3;
    static const Int32 PARSE_FORWARD_LOCK;// = 1<<4;
    static const Int32 PARSE_ON_SDCARD;// = 1<<5;
    static const Int32 PARSE_IS_SYSTEM_DIR;// = 1<<6;
    static const Int32 PARSE_IS_PRIVILEGED;// = 1<<7;
    static const Int32 PARSE_COLLECT_CERTIFICATES;// = 1<<8;
    static const Int32 PARSE_TRUSTED_OVERLAY;// = 1<<9;

    static const Int32 CERTIFICATE_BUFFER_SIZE;

private:
    static const Boolean DEBUG_JAR ;
    static const Boolean DEBUG_PARSER;
    static const Boolean DEBUG_BACKUP;

    static const String TAG;

    // TODO: switch outError users to PackageParserException
    // TODO: refactor "codePath" to "apkPath"

    /** File name in an APK for the Android manifest. */
    static const String ANDROID_MANIFEST_FILENAME;

    /**
     * @deprecated callers should move to explicitly passing around source path.
     */
    String mArchiveSourcePath;

    AutoPtr< ArrayOf<String> > mSeparateProcesses;
    Boolean mOnlyCoreApps;
    AutoPtr<IDisplayMetrics> mMetrics;

    static const Int32 SDK_VERSION;
    static const AutoPtr< ArrayOf<String> > SDK_CODENAMES;

    static Int32 sParseError;

    static Boolean sCompatibilityModeEnabled;

    static const Int32 PARSE_DEFAULT_INSTALL_LOCATION;//PackageInfo.INSTALL_LOCATION_UNSPECIFIED;

    AutoPtr<ParsePackageItemArgs> mParseInstrumentationArgs;
    AutoPtr<ParseComponentArgs> mParseActivityArgs;
    AutoPtr<ParseComponentArgs> mParseActivityAliasArgs;
    AutoPtr<ParseComponentArgs> mParseServiceArgs;
    AutoPtr<ParseComponentArgs> mParseProviderArgs;

    /** If set to true, we will only allow package files that exactly match
     *  the DTD.  Otherwise, we try to get as much from the package as we
     *  can without failing.  This should normally be set to false, to
     *  support extensions to the DTD in future versions. */
    static const Boolean RIGID_PARSER;

    static const String ANDROID_RESOURCES;

    static const AutoPtr<IComparator> sSplitNameComparator;// = new SplitNameComparator();


    // static AtomicReference<byte[]> sBuffer = new AtomicReference<byte[]>();
};

} // namespace Pm
} // namespace Content
} // namespace Droid
} // namespace Elastos

DEFINE_CONVERSION_FOR(Elastos::Droid::Content::Pm::PackageParser::ActivityIntentInfo, IInterface)
DEFINE_CONVERSION_FOR(Elastos::Droid::Content::Pm::PackageParser::ServiceIntentInfo, IInterface)
DEFINE_CONVERSION_FOR(Elastos::Droid::Content::Pm::PackageParser::ProviderIntentInfo, IInterface)
DEFINE_CONVERSION_FOR(Elastos::Droid::Content::Pm::PackageParser::Package, IInterface)

DEFINE_OBJECT_HASH_FUNC_FOR(Elastos::Droid::Content::Pm::PackageParser::ActivityIntentInfo)
DEFINE_OBJECT_HASH_FUNC_FOR(Elastos::Droid::Content::Pm::PackageParser::ProviderIntentInfo)
DEFINE_OBJECT_HASH_FUNC_FOR(Elastos::Droid::Content::Pm::PackageParser::ServiceIntentInfo)
DEFINE_OBJECT_HASH_FUNC_FOR(Elastos::Droid::Content::Pm::PackageParser::Package)

#endif //__ELASTOS_DROID_CONTENT_PM_PACKAGEPARSER_H__
