
#ifndef __ELASTOS_DROID_CONTENT_PM_PACKAGEPARSER_H__
#define __ELASTOS_DROID_CONTENT_PM_PACKAGEPARSER_H__

#include "ext/frameworkext.h"
#include "content/IntentFilter.h"
#include "content/pm/PackageUserState.h"
#include <elastos/utility/etl/List.h>
#include <elastos/utility/etl/HashSet.h>

using Elastos::Core::ICharSequence;
using Elastos::Utility::Etl::List;
using Elastos::Utility::Etl::HashSet;
using Elastos::IO::IFile;
using Elastos::Utility::Jar::IJarFile;
using Elastos::Utility::Jar::IJarEntry;
using Elastos::Security::Cert::ICertificate;
using Org::Xmlpull::V1::IXmlPullParser;

using Elastos::Droid::Content::Res::ITypedArray;
using Elastos::Droid::Content::Res::IResources;
using Elastos::Droid::Content::Res::IXmlResourceParser;
using Elastos::Droid::Os::IBundle;
using Elastos::Droid::Utility::IDisplayMetrics;
using Elastos::Droid::Utility::IAttributeSet;

namespace Elastos {
namespace Droid {
namespace Content {
namespace Pm {

/**
 * Package archive parsing
 *
 * {@hide}
 */
 class PackageParser
    : public Object
{
public:
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
            /* [in] */ Int32 logoRes);

    public:
        Package* mOwner;
        AutoPtr< ArrayOf<String> > mOutError;
        Int32 mNameRes;
        Int32 mLabelRes;
        Int32 mIconRes;
        Int32 mLogoRes;

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

    /* Light weight package info.
     * @hide
     */
    class PackageLite : public Object
    {
    public:
        PackageLite(
            /* [in] */ const String& packageName,
            /* [in] */ Int32 versionCode,
            /* [in] */ Int32 installLocation,
            /* [in] */ List< AutoPtr<IVerifierInfo> >* verifiers);

    public:
        String mPackageName;
        Int32 mVersionCode;
        Int32 mInstallLocation;
        AutoPtr< ArrayOf<IVerifierInfo*> > mVerifiers;
    };

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

        CARAPI_(String) ToString();

    public:
        String mPackageName;

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

        AutoPtr< List<String> > mUsesLibraries;
        AutoPtr< List<String> > mUsesOptionalLibraries;
        AutoPtr< ArrayOf<String> > mUsesLibraryFiles;

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

        // For use by package manager service for quick lookup of
        // preferred up order.
        Int32 mPreferredOrder;

        // For use by the package manager to keep track of the path to the
        // file an app came from.
        String mScanPath;

        // For use by package manager to keep track of where it has done dexopt.
        Boolean mDidDexOpt;

        // // User set enabled state.
        // Int32 mSetEnabled;

        // Additional data supplied by callers.
        AutoPtr<IInterface> mExtras; //this member point to PackageSetting who has strong reference each other

        // Whether an operation is currently pending on this package
        Boolean mOperationPending;

        /*
         *  Applications hardware preferences
         */
        List< AutoPtr<IConfigurationInfo> > mConfigPreferences;

        /*
         *  Applications requested features
         */
        AutoPtr< List< AutoPtr<IFeatureInfo> > > mReqFeatures;

        Int32 mInstallLocation;

        /**
         * Digest suitable for comparing whether this package's manifest is the
         * same as another.
         */
        AutoPtr<IManifestDigest> mManifestDigest;

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

        virtual CARAPI_(String) GetComponentShortName();

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

        CARAPI_(String) ToString();

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

        CARAPI_(String) ToString();

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

        CARAPI_(String) ToString();

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

        CARAPI_(String) ToString();

    public:
        AutoPtr<IServiceInfo> mInfo;
    };

    class Provider : public Component<IntentInfo>
    {
    public:
        Provider(
            /* [in] */ Provider* existingProvider);

        Provider(
            /* [in] */ ParseComponentArgs* args,
            /* [in] */ IProviderInfo* info);

        CARAPI_(void) SetPackageName(
            /* [in] */ const String& capName);

        CARAPI_(String) ToString();

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

        CARAPI_(String) ToString();

    public:
        AutoPtr<IInstrumentationInfo> mInfo;
    };

    class IntentInfo
        : public IntentFilter
    {
    public:
        IntentInfo();

        CARAPI SetPriority(
        /* [in] */ Int32 priority);

        CARAPI GetPriority(
            /* [out] */ Int32* priority);

        CARAPI AddAction(
            /* [in] */ const String& action);

        CARAPI CountActions(
            /* [out] */ Int32* count);

        CARAPI GetAction(
            /* [in] */ Int32 index,
            /* [out] */ String* action);

        CARAPI HasAction(
            /* [in] */ const String& action,
            /* [out] */ Boolean* hasAction);

        CARAPI MatchAction(
            /* [in] */ const String& action,
            /* [out] */ Boolean* isMatched);

        CARAPI GetActions(
            /* [out, callee] */ ArrayOf<String>** actions);

        CARAPI AddDataType(
            /* [in] */ const String& type);

        CARAPI HasDataType(
            /* [in] */ const String& type,
            /* [out] */ Boolean* hasDataType);

        CARAPI CountDataTypes(
            /* [out] */ Int32* Count);

        CARAPI GetDataType(
            /* [in] */ Int32 index,
            /* [out] */ String* type);

        CARAPI GetTypes(
            /* [out, callee] */ ArrayOf<String>** types);

        CARAPI AddDataScheme(
            /* [in] */ const String& scheme);

        CARAPI CountDataSchemes(
            /* [out] */ Int32* count);

        CARAPI GetDataScheme(
            /* [in] */ Int32 index,
            /* [out] */ String* scheme);

        CARAPI HasDataScheme(
            /* [in] */ const String& scheme,
            /* [out] */ Boolean* hasDataScheme);

        CARAPI GetSchemes(
            /* [out, callee] */ ArrayOf<String>** schemes);

        CARAPI AddDataAuthority(
            /* [in] */ const String& host,
            /* [in] */ const String& port);

        CARAPI CountDataAuthorities(
            /* [out] */ Int32* count);

        CARAPI GetDataAuthority(
            /* [in] */ Int32 index,
            /* [out] */ IIntentFilterAuthorityEntry** authority);

        CARAPI HasDataAuthority(
            /* [in] */ IUri* Data,
            /* [out] */ Boolean* hasDataAuthority);

        CARAPI GetAuthorities(
            /* [out, callee] */ ArrayOf<IIntentFilterAuthorityEntry *>** authorities);

        CARAPI AddDataPath(
            /* [in] */ const String& path,
            /* [in] */ Int32 type);

        CARAPI CountDataPaths(
            /* [out] */ Int32* count);

        CARAPI GetDataPath(
            /* [in] */ Int32 index,
            /* [out] */ IPatternMatcher** path);

        CARAPI HasDataPath(
            /* [in] */ const String& data,
            /* [out] */ Boolean* hasDataPath);

        CARAPI GetPaths(
            /* [out, callee] */ ArrayOf<IPatternMatcher *>** paths);

        CARAPI MatchDataAuthority(
            /* [in] */ IUri* data,
            /* [out] */ Int32* result);

        CARAPI MatchData(
            /* [in] */ const String& type,
            /* [in] */ const String& scheme,
            /* [in] */ IUri* data,
            /* [out] */ Int32* result);

        CARAPI AddCategory(
            /* [in] */ const String& category);

        CARAPI CountCategories(
            /* [out] */ Int32* count);

        CARAPI GetCategory(
            /* [in] */ Int32 index,
            /* [out] */ String* category);

        CARAPI HasCategory(
            /* [in] */ const String& category,
            /* [out] */ Boolean* hasCategory);

        CARAPI GetCategories(
            /* [out, callee] */ ArrayOf<String>** categories);

        CARAPI MatchCategories(
            /* [in] */ ArrayOf<String>* categories,
            /* [out] */ String* result);

        CARAPI Match(
            /* [in] */ IContentResolver* resolver,
            /* [in] */ IIntent* intent,
            /* [in] */ Boolean resolve,
            /* [in] */ const String& logTag,
            /* [out] */ Int32* result);

        CARAPI Match(
            /* [in] */ const String& action,
            /* [in] */ const String& type,
            /* [in] */ const String& scheme,
            /* [in] */ IUri* data,
            /* [in] */ ArrayOf<String>* categories,
            /* [in] */ const String& logTag,
            /* [out] */ Int32* result);

        CARAPI ReadFromXml(
            /* [in] */ IXmlPullParser* parser);

        CARAPI WriteToXml(
            /* [in] */ IXmlSerializer* serializer);

        CARAPI Dump(
            /* [in] */ IPrinter* du,
            /* [in] */ const String& prefix);

        CARAPI HasPartialTypes(
            /* [out] */ Boolean* has);

        using IntentFilter::Match;
        using IntentFilter::HasCategory;
        using IntentFilter::GetPriority;
        using IntentFilter::CountActions;

    public:
        Boolean mHasDefault;
        Int32 mLabelRes;
        AutoPtr<ICharSequence> mNonLocalizedLabel;
        Int32 mIcon;
        Int32 mLogo;
    };

    class ActivityIntentInfo : public IntentInfo
    {
    public:
        ActivityIntentInfo(
            /* [in] */ Activity* activity);

        CARAPI_(String) ToString();

    public:
        Activity* mActivity;
    };

    class ServiceIntentInfo : public IntentInfo
    {
    public:
        ServiceIntentInfo(
            /* [in] */ Service* service);

        CARAPI_(String) ToString();

    public:
        Service* mService;
    };

public:
    PackageParser(
        /* [in] */ const String& archiveSourcePath);

    CARAPI_(void) SetSeparateProcesses(
        /* [in] */ ArrayOf<String>* procs);

    CARAPI_(void) SetOnlyCoreApps(
        /* [in] */ Boolean onlyCoreApps);

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

    static CARAPI_(AutoPtr<IPackageInfo>) GeneratePackageInfo(
        /* [in] */ Package* c,
        /* [in] */ ArrayOf<Int32>* gids,
        /* [in] */ Int32 flags,
        /* [in] */ Int64 firstInstallTime,
        /* [in] */ Int64 lastUpdateTime,
        /* [in] */ HashSet<String>* grantedPermissions,
        /* [in] */ PackageUserState* state,
        /* [in] */ Int32 userId);

    CARAPI_(Int32) GetParseError();

    CARAPI_(AutoPtr<Package>) ParsePackage(
        /* [in] */ IFile* sourceFile,
        /* [in] */ const String& destCodePath,
        /* [in] */ IDisplayMetrics* metrics,
        /* [in] */ Int32 flags,
        /* [in] */ Boolean isEpk = FALSE);

    CARAPI_(Boolean) CollectCertificates(
        /* [in] */ Package* pkg,
        /* [in] */ Int32 flags,
        /* [in] */ ArrayOf<Byte>* readBuffer);

    /*
     * Utility method that retrieves just the package name and install
     * location from the apk location at the given file path.
     * @param packageFilePath file location of the apk
     * @param flags Special parse flags
     * @return PackageLite object with package information or null on failure.
     */
    static CARAPI_(AutoPtr<PackageLite>) ParsePackageLite(
        /* [in] */ const String& packageFilePath,
        /* [in] */ Int32 flags);

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

private:
    static CARAPI_(Boolean) IsPackageFilename(
        /* [in] */ const String& name);

    static CARAPI_(Boolean) CheckUseInstalled(
        /* [in] */ Int32 flags,
        /* [in] */ PackageUserState* state);

    CARAPI_(AutoPtr< ArrayOf<ICertificate*> >) LoadCertificates(
        /* [in] */ IJarFile* jarFile,
        /* [in] */ IJarEntry* je,
        /* [in] */ ArrayOf<Byte>* readBuffer);

    static CARAPI_(String) ValidateName(
        /* [in] */ const String& name,
        /* [in] */ Boolean requiresSeparator);

    static CARAPI_(String) ParsePackageName(
        /* [in] */ IXmlPullParser* parser,
        /* [in] */ IAttributeSet* attrs,
        /* [in] */ Int32 flags,
        /* [out] */ ArrayOf<String>* outError);

    static CARAPI_(AutoPtr<PackageLite>) ParsePackageLite(
        /* [in] */ IResources* res,
        /* [in] */ IXmlPullParser* parser,
        /* [in] */ IAttributeSet* attrs,
        /* [in] */ Int32 flags,
        /* [out] */ ArrayOf<String>* outError);

    CARAPI_(AutoPtr<Package>) ParsePackage(
        /* [in] */ IResources* res,
        /* [in] */ IXmlResourceParser* parser,
        /* [in] */ Int32 flags,
        /* [out] */ ArrayOf<String>* outError);

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

    CARAPI_(AutoPtr<PermissionGroup>) ParsePermissionGroup(
        /* [in] */ Package* owner,
        /* [in] */ Int32 flags,
        /* [in] */ IResources* res,
        /* [in] */ IXmlPullParser* parser,
        /* [in] */ IAttributeSet* attrs,
        /* [out] */ ArrayOf<String>* outError);

    CARAPI_(AutoPtr<Permission>) ParsePermission(
        /* [in] */ Package* owner,
        /* [in] */ IResources* res,
        /* [in] */ IXmlPullParser* parser,
        /* [in] */ IAttributeSet* attrs,
        /* [out] */ ArrayOf<String>* outError);

    CARAPI_(AutoPtr<Permission>) ParsePermissionTree(
        /* [in] */ Package* owner,
        /* [in] */ IResources* res,
        /* [in] */ IXmlPullParser* parser,
        /* [in] */ IAttributeSet* attrs,
        /* [out] */ ArrayOf<String>* outError);

    CARAPI_(AutoPtr<Instrumentation>) ParseInstrumentation(
        /* [in] */ Package* owner,
        /* [in] */ IResources* res,
        /* [in] */ IXmlPullParser* parser,
        /* [in] */ IAttributeSet* attrs,
        /* [out] */ ArrayOf<String>* outError);

    CARAPI_(Boolean) ParseApplication(
        /* [in] */ PackageParser::Package* owner,
        /* [in] */ IResources* res,
        /* [in] */ IXmlPullParser* parser,
        /* [in] */ IAttributeSet* attrs,
        /* [in] */ Int32 flags,
        /* [out] */ ArrayOf<String>* outError);

    CARAPI_(Boolean) ParsePackageItemInfo(
        /* [in] */ Package* owner,
        /* [in] */ IPackageItemInfo* outInfo,
        /* [out] */ ArrayOf<String>* outError,
        /* [in] */ const String& tag,
        /* [in] */ ITypedArray* sa,
        /* [in] */ Int32 nameRes,
        /* [in] */ Int32 labelRes,
        /* [in] */ Int32 iconRes,
        /* [in] */ Int32 logoRes);

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
        /* [in] */ Int32 flags,
        /* [out] */ ArrayOf<String>* outError);

    CARAPI_(Boolean) ParseIntent(
        /* [in] */ IResources* res,
        /* [in] */ IXmlPullParser* parser,
        /* [in] */ IAttributeSet* attrs,
        /* [in] */ Int32 flags,
        /* [in] */ IntentInfo* outInfo,
        /* [out] */ ArrayOf<String>* outError,
        /* [in] */ Boolean isActivity);

    static CARAPI_(Boolean) CopyNeeded(
        /* [in] */ Int32 flags,
        /* [in] */ Package* p,
        /* [in] */ PackageUserState* state,
        /* [in] */ IBundle* metaData,
        /* [in] */ Int32 userId);

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

    static const Int32 PARSE_IS_SYSTEM;
    static const Int32 PARSE_CHATTY;
    static const Int32 PARSE_MUST_BE_APK;
    static const Int32 PARSE_IGNORE_PROCESSES;
    static const Int32 PARSE_FORWARD_LOCK;
    static const Int32 PARSE_ON_SDCARD;
    static const Int32 PARSE_IS_SYSTEM_DIR;

    static const Int32 CERTIFICATE_BUFFER_SIZE;
private:
    static const Boolean DEBUG_JAR ;
    static const Boolean DEBUG_PARSER;
    static const Boolean DEBUG_BACKUP;

    static const String TAG;

    /** File name in an APK for the Android manifest. */
    static const String ANDROID_MANIFEST_FILENAME;

    String mArchiveSourcePath;
    AutoPtr< ArrayOf<String> > mSeparateProcesses;
    Boolean mOnlyCoreApps;

    static const Int32 SDK_VERSION;
    static const String SDK_CODENAME;

    Int32 mParseError;

    static Boolean sCompatibilityModeEnabled;
    static const Int32 PARSE_DEFAULT_INSTALL_LOCATION;

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
};

} // namespace Pm
} // namespace Content
} // namespace Droid
} // namespace Elastos

#endif //__ELASTOS_DROID_CONTENT_PM_PACKAGEPARSER_H__
