
#include "elastos/droid/content/pm/PackageParser.h"
#include "elastos/droid/content/pm/CActivityInfo.h"
#include "elastos/droid/content/pm/CApplicationInfo.h"
#include "elastos/droid/content/pm/CConfigurationInfo.h"
#include "elastos/droid/content/pm/CFeatureInfo.h"
#include "elastos/droid/content/pm/CInstrumentationInfo.h"
#include "elastos/droid/content/pm/CManifestDigestHelper.h"
#include "elastos/droid/content/pm/CPackageInfo.h"
#include "elastos/droid/content/pm/CPathPermission.h"
#include "elastos/droid/content/pm/CPermissionInfo.h"
#include "elastos/droid/content/pm/CPermissionGroupInfo.h"
#include "elastos/droid/content/pm/CProviderInfo.h"
#include "elastos/droid/content/pm/CPermissionInfoHelper.h"
#include "elastos/droid/content/pm/CServiceInfo.h"
#include "elastos/droid/content/pm/CSignature.h"
#include "elastos/droid/content/pm/IPackageManager::h"
#include "elastos/droid/content/res/CAssetManager.h"
#include "elastos/droid/content/res/CResources.h"
#include "elastos/droid/content/CComponentName.h"
#include "elastos/droid/os/CBundle.h"
#include "elastos/droid/os/CPatternMatcher.h"
#include "elastos/droid/os/Build.h"
#include "elastos/droid/os/UserHandle.h"
#include "elastos/droid/os/SystemClock.h"
#include "elastos/droid/utility/CDisplayMetrics.h"
#include "elastos/droid/utility/XmlUtils.h"
#include "R.h"
#include "Manifest.h"

#include <elastos/core/StringBuilder.h>
#include <elastos/utility/logging/Logger.h>
#include <elastos/utility/logging/Slogger.h>

using Elastos::Core::CString;
using Elastos::Core::StringBuilder;
using Elastos::Utility::Logging::Slogger;

using Elastos::Droid::Content::Res::IAssetManager;
using Elastos::Droid::Content::Res::CAssetManager;
using Elastos::Droid::Content::Res::IConfiguration;
using Elastos::Droid::Content::Res::CResources;
using Elastos::Droid::Os::Build;
using Elastos::Droid::Os::UserHandle;
using Elastos::Droid::Os::CPatternMatcher;
using Elastos::Droid::Os::CBundle;
using Elastos::Droid::Os::SystemClock;
using Elastos::Droid::Utility::ITypedValue;
using Elastos::Droid::Utility::CDisplayMetrics;
using Elastos::Droid::Utility::XmlUtils;

using Elastos::IO::IBufferedInputStream;
using Elastos::IO::IInputStream;
using Elastos::IO::CBufferedInputStream;
using Elastos::Utility::Jar::IAttributes;
using Elastos::Utility::Jar::IJarEntry;
using Elastos::Utility::Jar::CJarEntry;
using Elastos::Utility::Zip::IZipEntry;
using Elastos::Utility::Jar::IJarFile;
using Elastos::Utility::Jar::CJarFile;
using Elastos::Utility::Jar::IManifest;
using Elastos::Security::Cert::ICertificate;
using Elastos::Security::IPublicKey;

namespace Elastos {
namespace Droid {
namespace Content {
namespace Pm {

const Boolean PackageParser::DEBUG_JAR = FALSE;
const Boolean PackageParser::DEBUG_PARSER = FALSE;
const Boolean PackageParser::DEBUG_BACKUP = FALSE;

const String PackageParser::TAG("PackageParser");

/** File name in an APK for the Android manifest. */
const String PackageParser::ANDROID_MANIFEST_FILENAME("AndroidManifest.xml");

AutoPtr< ArrayOf<PackageParser::NewPermissionInfo*> > Init_NEW_PERMISSIONS()
{
    AutoPtr< ArrayOf<PackageParser::NewPermissionInfo*> > perms =
            ArrayOf<PackageParser::NewPermissionInfo*>::Alloc(2);
    AutoPtr<PackageParser::NewPermissionInfo> writePermission = new PackageParser::NewPermissionInfo(String("android.permission.WRITE_EXTERNAL_STORAGE")
            , Build::VERSION_CODES::DONUT, 0);
    perms->Set(0, writePermission);
    AutoPtr<PackageParser::NewPermissionInfo> readPermission = new PackageParser::NewPermissionInfo(String("android.permission.READ_PHONE_STATE")
            , Build::VERSION_CODES::DONUT, 0);
    perms->Set(1, readPermission);
    return perms;
}

const AutoPtr< ArrayOf<PackageParser::NewPermissionInfo*> > PackageParser::NEW_PERMISSIONS =
        Init_NEW_PERMISSIONS();

AutoPtr< ArrayOf<PackageParser::SplitPermissionInfo*> > Init_SPLIT_PERMISSIONS()
{
    AutoPtr<ArrayOf<String> > array1 = ArrayOf<String>::Alloc(1);
    (*array1)[0] = String("android.permission.READ_EXTERNAL_STORAGE");
    AutoPtr<ArrayOf<String> > array2 = ArrayOf<String>::Alloc(1);
    (*array2)[0] = String("android.permission.READ_CALL_LOG");
    AutoPtr<ArrayOf<String> > array3 = ArrayOf<String>::Alloc(1);
    (*array3)[0] = String("android.permission.WRITE_CALL_LOG");

    AutoPtr< ArrayOf<PackageParser::SplitPermissionInfo*> > perms =
            ArrayOf<PackageParser::SplitPermissionInfo*>::Alloc(3);
    AutoPtr<PackageParser::SplitPermissionInfo> info1 = new PackageParser::SplitPermissionInfo(String("android.permission.WRITE_EXTERNAL_STORAGE")
            ,
            array1,
            Build::VERSION_CODES::CUR_DEVELOPMENT + 1);
    perms->Set(0, info1);

    AutoPtr<PackageParser::SplitPermissionInfo> info2 = new PackageParser::SplitPermissionInfo(String("android.permission.READ_CONTACTS")
            ,
            array2,
            Build::VERSION_CODES::JELLY_BEAN);
    perms->Set(1, info2);

    AutoPtr<PackageParser::SplitPermissionInfo> info3 = new PackageParser::SplitPermissionInfo(String("android.permission.WRITE_CONTACTS")
            ,
            array3,
            Build::VERSION_CODES::JELLY_BEAN);
    perms->Set(2, info3);
    return perms;
}

const AutoPtr< ArrayOf<PackageParser::SplitPermissionInfo*> > PackageParser::SPLIT_PERMISSIONS =
        Init_SPLIT_PERMISSIONS();

const Int32 PackageParser::SDK_VERSION = Build::VERSION::SDK_INT;
const String PackageParser::SDK_CODENAMES = Build::VERSION::ACTIVE_CODENAMES;
const Int32 PackageParser::CERTIFICATE_BUFFER_SIZE = 8192;
Boolean PackageParser::sCompatibilityModeEnabled = TRUE;
const Boolean PackageParser::RIGID_PARSER = FALSE;
const Int32 PackageParser::PARSE_DEFAULT_INSTALL_LOCATION = IPackageInfo::INSTALL_LOCATION_UNSPECIFIED;

const Int32 PackageParser::PARSE_IS_SYSTEM = 1<<0;
const Int32 PackageParser::PARSE_CHATTY = 1<<1;
const Int32 PackageParser::PARSE_MUST_BE_APK = 1<<2;
const Int32 PackageParser::PARSE_IGNORE_PROCESSES = 1<<3;
const Int32 PackageParser::PARSE_FORWARD_LOCK = 1<<4;
const Int32 PackageParser::PARSE_ON_SDCARD = 1<<5;
const Int32 PackageParser::PARSE_IS_SYSTEM_DIR = 1<<6;
const Int32 PackageParser::PARSE_IS_PRIVILEGED = 1<<7;
const Int32 PackageParser::PARSE_COLLECT_CERTIFICATES = 1<<8;
const Int32 PackageParser::PARSE_TRUSTED_OVERLAY = 1<<9;

const String PackageParser::ANDROID_RESOURCES("http://schemas.android.com/apk/res/android");


//=================================================================
// PackageParser::NewPermissionInfo
//=================================================================
PackageParser::NewPermissionInfo::NewPermissionInfo(
    /* [in] */ const String& name,
    /* [in] */ Int32 sdkVersion,
    /* [in] */ Int32 fileVersion)
    : mName(name)
    , mSdkVersion(sdkVersion)
    , mFileVersion(fileVersion)
{}

PackageParser::SplitPermissionInfo::SplitPermissionInfo(
    /* [in] */ const String& rootPerm,
    /* [in] */ ArrayOf<String>* newPerms,
    /* [in] */ Int32 targetSdk)
    : mRootPerm(rootPerm)
    , mNewPerms(newPerms)
    , mTargetSdk(targetSdk)
{}

PackageParser::ParsePackageItemArgs::ParsePackageItemArgs(
    /* [in] */ Package* owner,
    /* [in] */ ArrayOf<String>* outError,
    /* [in] */ Int32 nameRes,
    /* [in] */ Int32 labelRes,
    /* [in] */ Int32 iconRes,
    /* [in] */ Int32 logoRes,
    /* [in] */ Int32 bannerRes)
    : mOwner(owner)
    , mOutError(outError)
    , mNameRes(nameRes)
    , mLabelRes(labelRes)
    , mIconRes(iconRes)
    , mLogoRes(logoRes)
    , mBannerRes(bannerRes)
{}

PackageParser::ParseComponentArgs::ParseComponentArgs(
    /* [in] */ PackageParser::Package* owner,
    /* [in] */ ArrayOf<String>* outError,
    /* [in] */ Int32 nameRes,
    /* [in] */ Int32 labelRes,
    /* [in] */ Int32 iconRes,
    /* [in] */ Int32 logoRes,
    /* [in] */ Int32 bannerRes,
    /* [in] */ ArrayOf<String>* sepProcesses,
    /* [in] */ Int32 processRes,
    /* [in] */ Int32 descriptionRes,
    /* [in] */ Int32 enabledRes)
    : ParsePackageItemArgs(owner,
            outError, nameRes, labelRes, iconRes, logoRes, bannerRes)
    , mSepProcesses(sepProcesses)
    , mProcessRes(processRes)
    , mDescriptionRes(descriptionRes)
    , mEnabledRes(enabledRes)
    , mFlags(0)
{}

//=================================================================
// PackageParser::PackageLite
//=================================================================
PackageParser::PackageLite::PackageLite(
    /* [in] */ const String& codePath,
    /* [in] */ ApkLite* baseApk,
    /* [in] */ ArrayOf<String>* splitNames,
    /* [in] */ ArrayOf<String>* splitCodePaths)
{
    mPackageName = baseApk->mPackageName;
    mVersionCode = baseApk->mVersionCode;
    mInstallLocation = baseApk->mInstallLocation;
    mVerifiers = baseApk->mVerifiers;
    mSplitNames = splitNames;
    mCodePath = codePath;
    mBaseCodePath = baseApk->mCodePath;
    mSplitCodePaths = splitCodePaths;
    mCoreApp = baseApk->mCoreApp;
    mMultiArch = baseApk->mMultiArch;
}

AutoPtr<List<String> > PackageParser::PackageLite::GetAllCodePaths()
{
    AutoPtr<List<String> > paths = new List<String>();
    paths->PushBack(baseCodePath);
    if (mSplitCodePaths != NULL && mSplitCodePaths->GetLength() > 0) {
        for (Int32 i = 0; i < mSplitCodePaths->GetLength(); ++i) {
            paths->PushBack((*mSplitCodePaths)[i]);
        }
    }
    return paths;
}

//=================================================================
// PackageParser::ApkLite
//=================================================================
PackageParser::ApkLite::ApkLite(
    /* [in] */ String codePath,
    /* [in] */ String packageName,
    /* [in] */ String splitName,
    /* [in] */ Int32 versionCode,
    /* [in] */ Int32 installLocation,
    /* [in] */ List<AutoPtr<IVerifierInfo> >* verifiers,
    /* [in] */ ArrayOf<ISignature*>* signatures,
    /* [in] */ Boolean coreApp,
    /* [in] */ Boolean multiArch)
    : mCodePath(codePath)
    , mPackageName(packageName)
    , mSplitName(splitName)
    , mVersionCode(versionCode)
    , mInstallLocation(installLocation)
    , mSignatures(signatures)
    , mCoreApp(coreApp)
    , mMultiArch(multiArch)
{
    Int32 size;
    verifiers->GetSize(&size);
    mVerifiers = ArrayOf<IVerifierInfo*>::Alloc(size);
    List<AutoPtr<IVerifierInfo> >::Iterator it;
    Int32 i = 0;
    for (it = verifiers->Begin(); it != verifiers->End(); ++it) {
        mVerifiers->Set(i++, *it);
    }
}

//=================================================================
// PackageParser::Package
//=================================================================
PackageParser::Package::Package(
    /* [in] */ const String& name)
    : mPackageName(name)
    , mVersionCode(0)
    , mSharedUserLabel(0)
    , mPreferredOrder(0)
    , mDidDexOpt(FALSE)
    , mSetEnabled(IPackageManager::COMPONENT_ENABLED_STATE_DEFAULT)
    , mOperationPending(FALSE)
    , mInstallLocation(0)
    , mIsEpk(FALSE)
{
    ASSERT_SUCCEEDED(CApplicationInfo::New((IApplicationInfo**)&mApplicationInfo));
    mApplicationInfo->SetPackageName(mPackageName);
    mApplicationInfo->SetUid(-1);
}

PackageParser::Package::~Package()
{
    mProtectedBroadcasts = NULL;
    mUsesLibraries = NULL;
    mUsesOptionalLibraries = NULL;
    mOriginalPackages = NULL;
    mAdoptPermissions = NULL;
    mReqFeatures = NULL;
}

void PackageParser::Package::SetPackageName(
    /* [in] */ const String& newName)
{
    mPackageName = newName;
    mApplicationInfo->SetPackageName(newName);
    List< AutoPtr<Permission> >::ReverseIterator perit;
    for (perit = mPermissions.RBegin(); perit != mPermissions.REnd(); ++perit) {
        (*perit)->SetPackageName(newName);
    }
    List< AutoPtr<PermissionGroup> >::ReverseIterator pergit;
    for (pergit = mPermissionGroups.RBegin(); pergit != mPermissionGroups.REnd(); ++pergit) {
        (*pergit)->SetPackageName(newName);
    }
    List< AutoPtr<Activity> >::ReverseIterator arit;
    for (arit = mActivities.RBegin(); arit != mActivities.REnd(); ++arit) {
        (*arit)->SetPackageName(newName);
    }
    List< AutoPtr<Activity> >::ReverseIterator revit;
    for (revit = mReceivers.RBegin(); revit != mReceivers.REnd(); ++revit) {
        (*revit)->SetPackageName(newName);
    }
    List< AutoPtr<Provider> >::ReverseIterator provit;
    for (provit = mProviders.RBegin(); provit != mProviders.REnd(); ++provit) {
        (*provit)->SetPackageName(newName);
    }
    List< AutoPtr<Service> >::ReverseIterator serit;
    for (serit = mServices.RBegin(); serit != mServices.REnd(); ++serit) {
        (*serit)->SetPackageName(newName);
    }
    List< AutoPtr<Instrumentation> >::ReverseIterator instit;
    for (instit = mInstrumentation.RBegin(); instit != mInstrumentation.REnd(); ++instit) {
        (*instit)->SetPackageName(newName);
    }
}

Boolean PackageParser::Package::HasComponentClassName(
    /* [in] */ const String& name)
{
    List< AutoPtr<Activity> >::ReverseIterator arit;
    for (arit = mActivities.RBegin(); arit != mActivities.REnd(); ++arit) {
        if(name.Equals((*arit)->mClassName)) {
            return TRUE;
        }
    }
    List< AutoPtr<Activity> >::ReverseIterator revit;
    for (revit = mReceivers.RBegin(); revit != mReceivers.REnd(); ++revit) {
        if(name.Equals((*revit)->mClassName)) {
            return TRUE;
        }
    }
    List< AutoPtr<Provider> >::ReverseIterator provit;
    for (provit = mProviders.RBegin(); provit != mProviders.REnd(); ++provit) {
        if(name.Equals((*provit)->mClassName)) {
            return TRUE;
        }
    }
    List< AutoPtr<Service> >::ReverseIterator serit;
    for (serit = mServices.RBegin(); serit != mServices.REnd(); ++serit) {
        if(name.Equals((*serit)->mClassName)) {
            return TRUE;
        }
    }
    List< AutoPtr<Instrumentation> >::ReverseIterator instit;
    for (instit = mInstrumentation.RBegin(); instit != mInstrumentation.REnd(); ++instit) {
        if(name.Equals((*instit)->mClassName)) {
            return TRUE;
        }
    }
    return FALSE;
}

String PackageParser::Package::ToString()
{
    // return "Package{"
    //             + Integer.toHexString(System.identityHashCode(this))
    //             + " " + packageName + "}";
    assert(0);
    return String(NULL);
}

//=================================================================
// PackageParser::Component
//=================================================================
template <typename II>
PackageParser::Component<II>::Component(
    /* [in] */ Package* owner)
    : mOwner(owner)
{}

template <typename II>
PackageParser::Component<II>::Component(
    /* [in] */ ParsePackageItemArgs* args,
    /* [in, out] */ IPackageItemInfo* outInfo)
{
    Init(args, outInfo);
}

template <typename II>
void PackageParser::Component<II>::Init(
    /* [in] */ ParsePackageItemArgs* args,
    /* [in, out] */ IPackageItemInfo* outInfo)
{
    mOwner = args->mOwner;
    String name;
    ECode ec = args->mSa->GetNonConfigurationString(args->mNameRes, 0, &name);
    if (FAILED(ec) || name.IsNull()) {
        mClassName = NULL;
        // (*args->mOutError)[0] = (const char*)(StringBuffer(args->mTag)
        //         + " does not specify android:name");
        return;
    }

    String pkgName;
    mOwner->mApplicationInfo->GetPackageName(&pkgName);
    String className = BuildClassName(pkgName, name, args->mOutError);
    outInfo->SetName(className);
    if (className.IsNull()) {
        mClassName = NULL;
        // (*args->mOutError)[0] = (const char*)(StringBuffer(args->mTag)
        //         + " does not specify android:name");
        return;
    }

    mClassName = className;

    Int32 iconVal;
    args->mSa->GetResourceId(args->mIconRes, 0, &iconVal);
    if (iconVal != 0) {
        outInfo->SetIcon(iconVal);
        outInfo->SetNonLocalizedLabel(NULL);
    }

    Int32 logoVal;
    args->mSa->GetResourceId(args->mLogoRes, 0, &logoVal);
    if (logoVal != 0) {
        outInfo->SetLogo(logoVal);
    }

    Int32 bannerVal;
    args->mSa->GetResourceId(args->bannerRes, 0, &bannerVal);
    if (bannerVal != 0) {
        outInfo->SetBanner(bannerVal);
    }

    AutoPtr<ITypedValue> v;
    args->mSa->PeekValue(args->mLabelRes, (ITypedValue**)&v);
    Int32 resId;
    if (v != NULL && (v->GetResourceId(&resId), outInfo->SetLabelRes(resId), (resId == 0))) {
        AutoPtr<ICharSequence> nonLocalizedLabel;
        v->CoerceToString((ICharSequence**)&nonLocalizedLabel);
        outInfo->SetNonLocalizedLabel(nonLocalizedLabel);
    }

    outInfo->SetPackageName(mOwner->mPackageName);
}

template <typename II>
PackageParser::Component<II>::Component(
    /* [in] */ ParseComponentArgs* args,
    /* [in] */ IComponentInfo* outInfo)
{
    Init(args, (IPackageItemInfo*)outInfo);
    if (!(*args->mOutError)[0].IsNull()) {
        return;
    }

    if (args->mProcessRes != 0) {
        AutoPtr<ICharSequence> pname;
        Int32 targetSdkVersion;
        mOwner->mApplicationInfo->GetTargetSdkVersion(&targetSdkVersion);
        if (targetSdkVersion >= Build::VERSION_CODES::FROYO) {
            String sname;
            args->mSa->GetNonConfigurationString(args->mProcessRes,
            IConfiguration::NATIVE_CONFIG_VERSION, &sname);
            CString::New(sname, (ICharSequence**)&pname);
        }
        else {
            // Some older apps have been seen to use a resource reference
            // here that on older builds was ignored (with a warning).  We
            // need to continue to do this for them so they don't break.
            String sname;
            args->mSa->GetNonResourceString(args->mProcessRes, &sname);
            CString::New(sname, (ICharSequence**)&pname);
        }
        String appPackageName;
        String appProcName;
        mOwner->mApplicationInfo->GetPackageName(&appPackageName);
        mOwner->mApplicationInfo->GetProcessName(&appProcName);
        String procName = BuildProcessName(appPackageName, appProcName, pname, args->mFlags,
                args->mSepProcesses, args->mOutError);
        outInfo->SetProcessName(procName);
    }

    if (args->mDescriptionRes != 0) {
        Int32 descRes;
        args->mSa->GetResourceId(args->mDescriptionRes, 0, &descRes);
        outInfo->SetDescriptionRes(descRes);
    }

    Boolean enabled;
    args->mSa->GetBoolean(args->mEnabledRes, TRUE, &enabled);
    outInfo->SetEnabled(enabled);
}

template <typename II>
PackageParser::Component<II>::Component(
    /* [in] */ Component<II>* clone)
{
    mOwner = clone->mOwner;
    Copy(clone->mIntents.Begin(), clone->mIntents.End(), mIntents.Begin());
    mClassName = clone->mClassName;
    mComponentName = clone->mComponentName;
    mComponentShortName = clone->mComponentShortName;
}

template <typename II>
PackageParser::Component<II>::~Component()
{
}

template <typename II>
AutoPtr<IComponentName> PackageParser::Component<II>::GetComponentName()
{
    if (mComponentName != NULL) {
        return mComponentName;
    }
    if (!mClassName.IsNull()) {
        String pname;
        mOwner->mApplicationInfo->GetPackageName(&pname);
        CComponentName::New(pname, mClassName, (IComponentName**)&mComponentName);
    }
    return mComponentName;
}

template <typename II>
void PackageParser::Component<II>::AppendComponentShortName(
    /* [in] */ StringBuilder* sb)
{
    String pname;
    mOwner->mApplicationInfo->GetPackageName(&pname);
    CComponentName::AppendShortString(sb, pname, mClassName);
}

template <typename II>
void PackageParser::Component<II>::PrintComponentShortName(
    /* [in] */ IPrintWriter* pw)
{
    String pname;
    mOwner->mApplicationInfo->GetPackageName(&pname);
    CComponentName::PrintShortString(pw, pname, mClassName);
}

template <typename II>
void PackageParser::Component<II>::SetPackageName(
    /* [in] */ const String& PackageName)
{
    mComponentName = NULL;
    mComponentShortName = NULL;
}


PackageParser::Permission::Permission(
    /* [in] */ Package* owner)
    : Component<IntentInfo>(owner)
    , mTree(FALSE)
{
    ASSERT_SUCCEEDED(CPermissionInfo::New((IPermissionInfo**)&mInfo));
}

PackageParser::Permission::Permission(
    /* [in] */ Package* owner,
    /* [in] */ IPermissionInfo* info)
    : Component<IntentInfo>(owner)
    , mInfo(info)
    , mTree(FALSE)
{}

PackageParser::Permission::~Permission()
{}

void PackageParser::Permission::SetPackageName(
    /* [in] */ const String& packageName)
{
    Component<IntentInfo>::SetPackageName(packageName);
    mInfo->SetPackageName(packageName);
}

ECode PackageParser::Permission::ToString(
    /* [in] */ String* str)
{
    // return "Permission{"
    //             + Integer.toHexString(System.identityHashCode(this))
    //             + " " + info.name + "}";
    assert(0);
    return String(NULL);
}


PackageParser::PermissionGroup::PermissionGroup(
    /* [in] */ Package* owner)
    : Component<IntentInfo>(owner)
{
    CPermissionGroupInfo::New((IPermissionGroupInfo**)&mInfo);
}

PackageParser::PermissionGroup::PermissionGroup(
    /* [in] */ Package* owner,
    /* [in] */ IPermissionGroupInfo* info)
    : Component<IntentInfo>(owner)
    , mInfo(info)
{}

PackageParser::PermissionGroup::~PermissionGroup()
{}

void PackageParser::PermissionGroup::SetPackageName(
    /* [in] */ const String& PackageName)
{
    Component<IntentInfo>::SetPackageName(PackageName);
    mInfo->SetPackageName(PackageName);
}

ECode PackageParser::PermissionGroup::ToString(
    /* [in] */ String* str)
{
    // return "PermissionGroup{"
    //             + Integer.toHexString(System.identityHashCode(this))
    //             + " " + info.name + "}";
    assert(0);
    return String(NULL);
}


PackageParser::Activity::Activity(
    /* [in] */ ParseComponentArgs* args,
    /* [in] */ IActivityInfo* info)
    : Component<ActivityIntentInfo>(args, info)
    , mInfo(info)
{
    mInfo->SetApplicationInfo(args->mOwner->mApplicationInfo);
}

void PackageParser::Activity::SetPackageName(
    /* [in] */ const String& pkgName)
{
    Component<ActivityIntentInfo>::SetPackageName(pkgName);
    mInfo->SetPackageName(pkgName);
}

ECode PackageParser::Activity::ToString(
    /* [in] */ String* str)
{
    // StringBuilder sb = new StringBuilder(128);
    // sb.append("Activity{");
    // sb.append(Integer.toHexString(System.identityHashCode(this)));
    // sb.append(' ');
    // appendComponentShortName(sb);
    // sb.append('}');
    // return sb.toString();
    assert(0);
    return String(NULL);
}


PackageParser::Service::Service(
    /* [in] */ ParseComponentArgs* args,
    /* [in] */ IServiceInfo* info)
    : Component<ServiceIntentInfo>(args, info)
    , mInfo(info)
{
    mInfo->SetApplicationInfo(args->mOwner->mApplicationInfo);
}

void PackageParser::Service::SetPackageName(
    /* [in] */ const String& pkgName)
{
    Component<ServiceIntentInfo>::SetPackageName(pkgName);
    mInfo->SetPackageName(pkgName);
}

ECode PackageParser::Service::ToString(
    /* [in] */ String* str)
{
    // StringBuilder sb = new StringBuilder(128);
    // sb.append("Service{");
    // sb.append(Integer.toHexString(System.identityHashCode(this)));
    // sb.append(' ');
    // appendComponentShortName(sb);
    // sb.append('}');
    // return sb.toString();
    assert(0);
    return String(NULL);
}


PackageParser::Provider::Provider(
    /* [in] */ ParseComponentArgs* args,
    /* [in] */ IProviderInfo* info)
    : Component<ProviderIntentInfo>(args, info)
    , mInfo(info)
    , mSyncable(FALSE)
{
    mInfo->SetApplicationInfo(args->mOwner->mApplicationInfo);
}

PackageParser::Provider::Provider(
    /* [in] */ Provider* existingProvider)
    : Component<ProviderIntentInfo>(existingProvider)
    , mInfo(existingProvider->mInfo)
    , mSyncable(existingProvider->mSyncable)
{}

void PackageParser::Provider::SetPackageName(
    /* [in] */ const String& pkgName)
{
    Component<IntentInfo>::SetPackageName(pkgName);
    mInfo->SetPackageName(pkgName);
}

ECode PackageParser::Provider::ToString(
    /* [in] */ String* str)
{
    // StringBuilder sb = new StringBuilder(128);
    // sb.append("Provider{");
    // sb.append(Integer.toHexString(System.identityHashCode(this)));
    // sb.append(' ');
    // appendComponentShortName(sb);
    // sb.append('}');
    // return sb.toString();
    assert(0);
    return String(NULL);
}


PackageParser::Instrumentation::Instrumentation(
    /* [in] */ ParsePackageItemArgs* args,
    /* [in] */ IInstrumentationInfo* info)
    : Component<IntentInfo>(args, info)
    , mInfo(info)
{}

void PackageParser::Instrumentation::SetPackageName(
    /* [in] */ const String& pkgName)
{
    Component<IntentInfo>::SetPackageName(pkgName);
    mInfo->SetPackageName(pkgName);
}

ECode PackageParser::Instrumentation::ToString
    /* [in] */ String* str
{
    // StringBuilder sb = new StringBuilder(128);
    // sb.append("Instrumentation{");
    // sb.append(Integer.toHexString(System.identityHashCode(this)));
    // sb.append(' ');
    // appendComponentShortName(sb);
    // sb.append('}');
    // return sb.toString();
    assert(0);
    return String(NULL);
}


PackageParser::IntentInfo::IntentInfo()
    : mHasDefault(FALSE)
    , mLabelRes(0)
    , mIcon(0)
    , mLogo(0)
{
    IntentFilter::Init();
}

ECode PackageParser::IntentInfo::SetPriority(
    /* [in] */ Int32 priority)
{
    return IntentFilter::SetPriority(priority);
}

ECode PackageParser::IntentInfo::GetPriority(
    /* [out] */ Int32* priority)
{
    VALIDATE_NOT_NULL(priority);

    *priority = IntentFilter::GetPriority();
    return NOERROR;
}

ECode PackageParser::IntentInfo::AddAction(
    /* [in] */ const String& action)
{
    return IntentFilter::AddAction(action);
}

ECode PackageParser::IntentInfo::CountActions(
    /* [out] */ Int32* count)
{
    VALIDATE_NOT_NULL(count);

    *count = IntentFilter::CountActions();
    return NOERROR;
}

ECode PackageParser::IntentInfo::GetAction(
    /* [in] */ Int32 index,
    /* [out] */ String* action)
{
    VALIDATE_NOT_NULL(action);

    *action = IntentFilter::GetAction(index);
    return NOERROR;
}

ECode PackageParser::IntentInfo::HasAction(
    /* [in] */ const String& action,
    /* [out] */ Boolean* hasAction)
{
    VALIDATE_NOT_NULL(hasAction);

    *hasAction = IntentFilter::HasAction(action);
    return NOERROR;
}

ECode PackageParser::IntentInfo::MatchAction(
    /* [in] */ const String& action,
    /* [out] */ Boolean* isMatched)
{
    VALIDATE_NOT_NULL(isMatched);

    *isMatched = IntentFilter::MatchAction(action);
    return NOERROR;
}

ECode PackageParser::IntentInfo::GetActions(
    /* [out, callee] */ ArrayOf<String>** actions)
{
    VALIDATE_NOT_NULL(actions);

    AutoPtr< ArrayOf<String> > acts = IntentFilter::GetActions();
    *actions = acts;
    REFCOUNT_ADD(*actions);
    return NOERROR;
}

ECode PackageParser::IntentInfo::AddDataType(
    /* [in] */ const String& type)
{
    return IntentFilter::AddDataType(type);
}

ECode PackageParser::IntentInfo::HasDataType(
    /* [in] */ const String& type,
    /* [out] */ Boolean* hasDataType)
{
    VALIDATE_NOT_NULL(hasDataType);

    *hasDataType = IntentFilter::HasDataType(type);
    return NOERROR;
}

ECode PackageParser::IntentInfo::CountDataTypes(
    /* [out] */ Int32* count)
{
    VALIDATE_NOT_NULL(count);

    *count = IntentFilter::CountDataTypes();
    return NOERROR;
}

ECode PackageParser::IntentInfo::GetDataType(
    /* [in] */ Int32 index,
    /* [out] */ String* type)
{
    VALIDATE_NOT_NULL(type);

    *type = IntentFilter::GetDataType(index);
    return NOERROR;
}

ECode PackageParser::IntentInfo::GetTypes(
    /* [out, callee] */ ArrayOf<String>** types)
{
    VALIDATE_NOT_NULL(types);

    AutoPtr< ArrayOf<String> > typs = IntentFilter::GetTypes();
    *types = typs ;
    REFCOUNT_ADD(*types);
    return NOERROR;
}

ECode PackageParser::IntentInfo::AddDataScheme(
    /* [in] */ const String& scheme)
{
    return IntentFilter::AddDataScheme(scheme);
}

ECode PackageParser::IntentInfo::CountDataSchemes(
    /* [out] */ Int32* count)
{
    VALIDATE_NOT_NULL(count);

    *count = IntentFilter::CountDataSchemes();
    return NOERROR;
}

ECode PackageParser::IntentInfo::GetDataScheme(
    /* [in] */ Int32 index,
    /* [out] */ String* scheme)
{
    VALIDATE_NOT_NULL(scheme);

    *scheme = IntentFilter::GetDataScheme(index);
    return NOERROR;
}

ECode PackageParser::IntentInfo::HasDataScheme(
    /* [in] */ const String& scheme,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);

    *result = IntentFilter::HasDataScheme(scheme);
    return NOERROR;
}

ECode PackageParser::IntentInfo::GetSchemes(
    /* [out, callee] */ ArrayOf<String>** schemes)
{
    VALIDATE_NOT_NULL(schemes);

    AutoPtr< ArrayOf<String> > schs = IntentFilter::GetSchemes();
    *schemes = schs;
    REFCOUNT_ADD(*schemes);
    return NOERROR;
}

ECode PackageParser::IntentInfo::AddDataAuthority(
    /* [in] */ const String& host,
    /* [in] */ const String& port)
{
    return IntentFilter::AddDataAuthority(host, port);
}

ECode PackageParser::IntentInfo::CountDataAuthorities(
    /* [out] */ Int32* count)
{
    VALIDATE_NOT_NULL(count);

    *count = IntentFilter::CountDataAuthorities();
    return NOERROR;
}

ECode PackageParser::IntentInfo::GetDataAuthority(
    /* [in] */ Int32 index,
    /* [out] */ IIntentFilterAuthorityEntry** authority)
{
    VALIDATE_NOT_NULL(authority);

    AutoPtr<IIntentFilterAuthorityEntry> tmp = IntentFilter::GetDataAuthority(index);
    *authority = tmp->Get();
    REFCOUNT_ADD(*authority);
    return NOERROR;
}

ECode PackageParser::IntentInfo::HasDataAuthority(
    /* [in] */ IUri* data,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);

    *result = IntentFilter::HasDataAuthority(data);
    return NOERROR;
}

ECode PackageParser::IntentInfo::GetAuthorities(
    /* [out, callee] */ ArrayOf<IIntentFilterAuthorityEntry *>** authorities)
{
    VALIDATE_NOT_NULL(authorities)

    AutoPtr< ArrayOf<IIntentFilterAuthorityEntry*> > auths = IntentFilter::GetAuthorities();
    *authorities = auths;
    REFCOUNT_ADD(*authorities);
    return NOERROR;
}

ECode PackageParser::IntentInfo::AddDataPath(
    /* [in] */ const String& path,
    /* [in] */ Int32 type)
{
    return IntentFilter::AddDataPath(path, type);
}

ECode PackageParser::IntentInfo::CountDataPaths(
    /* [out] */ Int32* count)
{
    VALIDATE_NOT_NULL(count);

    *count = IntentFilter::CountDataPaths();
    return NOERROR;
}

ECode PackageParser::IntentInfo::GetDataPath(
    /* [in] */ Int32 index,
    /* [out] */ IPatternMatcher** path)
{
    VALIDATE_NOT_NULL(path);

    AutoPtr<IPatternMatcher> tmp = IntentFilter::GetDataPath(index);
    *path = tmp;
    REFCOUNT_ADD(*path);
    return NOERROR;
}

ECode PackageParser::IntentInfo::HasDataPath(
    /* [in] */ const String& data,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);

    *result = IntentFilter::HasDataPath(data);
    return NOERROR;
}

ECode PackageParser::IntentInfo::GetPaths(
    /* [out, callee] */ ArrayOf<IPatternMatcher *>** paths)
{
    VALIDATE_NOT_NULL(paths)

    AutoPtr< ArrayOf<IPatternMatcher*> > pas = IntentFilter::GetPaths();
    *paths = pas;
    REFCOUNT_ADD(*paths);
    return NOERROR;
}

ECode PackageParser::IntentInfo::MatchDataAuthority(
    /* [in] */ IUri* data,
    /* [out] */ Int32* result)
{
    VALIDATE_NOT_NULL(result);

    *result = IntentFilter::MatchDataAuthority(data);
    return NOERROR;
}

ECode PackageParser::IntentInfo::MatchData(
    /* [in] */ const String& type,
    /* [in] */ const String& scheme,
    /* [in] */ IUri* data,
    /* [out] */ Int32* result)
{
    VALIDATE_NOT_NULL(result);

    *result = IntentFilter::MatchData(type, scheme, data);
    return NOERROR;
}

ECode PackageParser::IntentInfo::AddCategory(
    /* [in] */ const String& category)
{
    return IntentFilter::AddCategory(category);
}

ECode PackageParser::IntentInfo::CountCategories(
    /* [out] */ Int32* count)
{
    VALIDATE_NOT_NULL(count);

    *count = IntentFilter::CountCategories();
    return NOERROR;
}

ECode PackageParser::IntentInfo::GetCategory(
    /* [in] */ Int32 index,
    /* [out] */ String* category)
{
    VALIDATE_NOT_NULL(category);

    *category = IntentFilter::GetCategory(index);
    return NOERROR;
}

ECode PackageParser::IntentInfo::HasCategory(
    /* [in] */ const String& category,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);

    *result = IntentFilter::HasCategory(category);
    return NOERROR;
}

ECode PackageParser::IntentInfo::GetCategories(
    /* [out, callee] */ ArrayOf<String>** categories)
{
    VALIDATE_NOT_NULL(categories)

    AutoPtr< ArrayOf<String> > cats = IntentFilter::GetCategories();
    *categories = cats;
    REFCOUNT_ADD(*categories);
    return NOERROR;
}

ECode PackageParser::IntentInfo::MatchCategories(
    /* [in] */ ArrayOf<String>* categories,
    /* [out] */ String* result)
{
    VALIDATE_NOT_NULL(result);

    *result = IntentFilter::MatchCategories(categories);
    return NOERROR;
}

ECode PackageParser::IntentInfo::Match(
    /* [in] */ IContentResolver* resolver,
    /* [in] */ IIntent* intent,
    /* [in] */ Boolean resolve,
    /* [in] */ const String& logTag,
    /* [out] */ Int32* result)
{
    VALIDATE_NOT_NULL(result);

    *result = IntentFilter::Match(resolver, intent, resolve, logTag);
    return NOERROR;
}

ECode PackageParser::IntentInfo::Match(
    /* [in] */ const String& action,
    /* [in] */ const String& type,
    /* [in] */ const String& scheme,
    /* [in] */ IUri* data,
    /* [in] */ ArrayOf<String>* categories,
    /* [in] */ const String& logTag,
    /* [out] */ Int32* result)
{
    VALIDATE_NOT_NULL(result);

    *result = IntentFilter::Match(action, type, scheme, data, categories, logTag);
    return NOERROR;
}

ECode PackageParser::IntentInfo::WriteToXml(
    /* [in] */ IXmlSerializer* serializer)
{
    return IntentFilter::WriteToXml(serializer);
}

ECode PackageParser::IntentInfo::ReadFromXml(
    /* [in] */ IXmlPullParser* parser)
{
    return IntentFilter::ReadFromXml(parser);
}

ECode PackageParser::IntentInfo::Dump(
    /* [in] */ IPrinter* du,
    /* [in] */ const String& prefix)
{
    return IntentFilter::Dump(du, prefix);
}

ECode PackageParser::IntentInfo::HasPartialTypes(
    /* [out] */ Boolean* has)
{
    VALIDATE_NOT_NULL(has)
    *has = mHasPartialTypes;
    return NOERROR;
}

//================================================================

PackageParser::ActivityIntentInfo::ActivityIntentInfo(
    /* [in] */ PackageParser::Activity* activity)
    : mActivity(activity)
{}

ECode PackageParser::ActivityIntentInfo::ToString(
    /* [in] */ String* str)
{
    // StringBuilder sb = new StringBuilder(128);
    // sb.append("ActivityIntentInfo{");
    // sb.append(Integer.toHexString(System.identityHashCode(this)));
    // sb.append(' ');
    // activity.appendComponentShortName(sb);
    // sb.append('}');
    // return sb.toString();
    assert(0);
    return String(NULL);
}


PackageParser::ServiceIntentInfo::ServiceIntentInfo(
    /* [in] */ PackageParser::Service* service)
    : mService(service)
{}

ECode PackageParser::ServiceIntentInfo::ToString(
    /* [in] */ String* str)
{
    // StringBuilder sb = new StringBuilder(128);
    // sb.append("ServiceIntentInfo{");
    // sb.append(Integer.toHexString(System.identityHashCode(this)));
    // sb.append(' ');
    // service.appendComponentShortName(sb);
    // sb.append('}');
    // return sb.toString();
    assert(0);
    return String(NULL);
}

PackageParser::ProviderIntentInfo::ProviderIntentInfo(
    /* [in] */ Provider* service)
{}

ECode PackageParser::ProviderIntentInfoToString(
    /* [out] */ String* str)
{
    StringBuilder sb = new StringBuilder(128);
    sb.append("ProviderIntentInfo{");
    sb.append(Integer.toHexString(System.identityHashCode(this)));
    sb.append(' ');
    provider.appendComponentShortName(sb);
    sb.append('}');
    return sb.toString();
}

//===========================================================================
// PackageParser::SplitNameComparator
//===========================================================================

CAR_INTERFACE_IMPL(PackageParser::SplitNameComparator, Object, IComparator)

ECode PackageParser::SplitNameComparator::Compare(
    /* [in] */ ICharSequence* lhs,
    /* [in] */ ICharSequence* rhs,
    /* [out] */ Int32* result)
{
    VALIDATE_NOT_NULL(result)

    if (lhs == NULL) {
        *result = -1;
    }
    else if (rhs == NULL) {
        *result = 1;
    }
    else {
        String l, r;
        lhs->ToString(&l);
        rhs->ToString(&r);
        *result = l.Compare(r);
    }

    return NOERROR;
}

//===========================================================================
// PackageParser
//===========================================================================

const AutoPtr<IComparator> sSplitNameComparator = new PackageParser::SplitNameComparator();

PackageParser::PackageParser()
    : mOnlyCoreApps(FALSE)
    , mParseError(IPackageManager::INSTALL_SUCCEEDED)
{
    assert(0 && "TODO");
    // CDisplayMetrics::New((IDisplayMetrics**)&mMetrics);
    mMetrics->SetToDefaults();
}

void PackageParser::SetSeparateProcesses(
    /* [in] */ ArrayOf<String>* procs)
{
   mSeparateProcesses = procs;
}

void PackageParser::SetOnlyCoreApps(
    /* [in] */ Boolean onlyCoreApps)
{
    mOnlyCoreApps = onlyCoreApps;
}

Boolean PackageParser::IsApkFile(
    /* [in] */ IFile* file)
{
    assert(file != NULL);
    String name;
    file->GetName(&name);
    return IsApkPath(name);
}

Boolean PackageParser::IsApkPath(
    /* [in] */ const String& name)
{
    return name.EndWith(String(".apk")) || name.EndWith(String(".epk"));
}

AutoPtr<IPackageInfo> PackageParser::GeneratePackageInfo(
    /* [in] */ PackageParser::Package* p,
    /* [in] */ ArrayOf<Int32>* gids,
    /* [in] */ Int32 flags,
    /* [in] */ Int64 firstInstallTime,
    /* [in] */ Int64 lastUpdateTime,
    /* [in] */ HashSet<String>* grantedPermissions,
    /* [in] */ PackageUserState* state)
{
    return GeneratePackageInfo(p, gids, flags, firstInstallTime, lastUpdateTime,
            grantedPermissions, state, UserHandle::GetCallingUserId());
}

Boolean PackageParser::CheckUseInstalledOrHidden(
    /* [in] */ Int32 flags,
    /* [in] */ PackageUserState* state)
{
    return (state->mInstalled  && !state->mHidden)
        || ((flags & IPackageManager::GET_UNINSTALLED_PACKAGES) != 0);
}

Boolean PackageParser::IsAvailable(
    /* [in] */ PackageUserState* state)
{
    return CheckUseInstalledOrHidden(0, state);
}

AutoPtr<IPackageInfo> PackageParser::GeneratePackageInfo(
    /* [in] */ Package* p,
    /* [in] */ ArrayOf<Int32>* gids,
    /* [in] */ Int32 flags,
    /* [in] */ Int64 firstInstallTime,
    /* [in] */ Int64 lastUpdateTime,
    /* [in] */ HashSet<String>* grantedPermissions,
    /* [in] */ PackageUserState* state,
    /* [in] */ Int32 userId)
{
    if (!CheckUseInstalledOrHidden(flags, state)) {
        return NULL;
    }
    AutoPtr<IPackageInfo> pi;
    CPackageInfo::New((IPackageInfo**)&pi);
    CPackageInfo* cpi = (CPackageInfo*)cpi->mGet();

    cpi->mPackageName = p->mPackageName;
    cpi->mSplitNames = p->mSplitNames;
    cpi->mVersionCode = p->mVersionCode;
    cpi->mVersionName = p->mVersionName;
    cpi->mSharedUserId = p->mSharedUserId;
    cpi->mSharedUserLabel = p->mSharedUserLabel;
    cpi->mApplicationInfo = GenerateApplicationInfo(p, flags, state, userId);
    cpi->mInstallLocation = p->installLocation;
    cpi->mCoreApp = p->mCoreApp;

    Int32 aiFlags;
    IPackageItemInfo::Probe(cpi->mApplicationInfo)->GetFlags(&aiFlags);
    if ((aiFlags & IApplicationInfo::FLAG_SYSTEM) != 0
            || (aiFlags & IApplicationInfo::FLAG_UPDATED_SYSTEM_APP) != 0) {
        cpi->mRequiredForAllUsers = p->mRequiredForAllUsers;
    }
    cpi->mRestrictedAccountType = p->mRestrictedAccountType;
    cpi->mRequiredAccountType = p->mRequiredAccountType;
    cpi->mOverlayTarget = p->mOverlayTarget;
    cpi->mFirstInstallTime = firstInstallTime;
    cpi->mLastUpdateTime = lastUpdateTime;

    if ((flags & IPackageManager::GET_GIDS) != 0) {
        pi->SetGids(gids);
    }
    Int32 i, j;
    if ((flags & IPackageManager::GET_CONFIGURATIONS) != 0) {
        Int32 N = p->mConfigPreferences.GetSize();
        if (N > 0) {
            AutoPtr< ArrayOf<IConfigurationInfo*> > infos =
                    ArrayOf<IConfigurationInfo*>::Alloc(N);
            List< AutoPtr<IConfigurationInfo> >::Iterator it;
            for (it = p->mConfigPreferences.Begin(), i = 0; it != p->mConfigPreferences.End(); ++it, ++i) {
                infos->Set(i, *it);
            }
            pi->SetConfigPreferences(infos);
        }
        N = p->mReqFeatures != NULL ? p->mReqFeatures->GetSize() : 0;
        if (N > 0) {
            AutoPtr< ArrayOf<IFeatureInfo*> > infos =
                    ArrayOf<IFeatureInfo*>::Alloc(N);
            List< AutoPtr<IFeatureInfo> >::Iterator it;
            for (it = p->mReqFeatures->Begin(), i = 0; it != p->mReqFeatures->End(); ++it, ++i) {
                infos->Set(i, *it);
            }
            pi->SetReqFeatures(infos);
        }

        N = p->mFeatureGroups != NULL ? p->mFeatureGroups->GetSize() : 0;
        if (N > 0) {
            cpi->mFeatureGroups = ArrayOf<IFeatureGroupInfo*>::Alloc(N);
            List< AutoPtr<IFeatureGroupInfo> >::Iterator it;
            for (it = p->mFeatureGroups->Begin(), i = 0; it != p->mFeatureGroups->End(); ++it, ++i) {
                cpi->mFeatureGroups->Set(i, *it);
            }
        }
    }
    if ((flags & IPackageManager::GET_ACTIVITIES) != 0) {
        Int32 N = p->mActivities.GetSize();
        if (N > 0) {
            AutoPtr< ArrayOf<IActivityInfo*> > infos;
            if ((flags & IPackageManager::GET_DISABLED_COMPONENTS) != 0) {
                infos = ArrayOf<IActivityInfo*>::Alloc(N);
            }
            else {
                Int32 num = 0;
                List< AutoPtr<Activity> >::Iterator it;
                for (it = p->mActivities.Begin(); it != p->mActivities.End(); ++it) {
                    Boolean enabled;
                    (*it)->mInfo->GetEnabled(&enabled);
                    if (enabled) num++;
                }
                infos = ArrayOf<IActivityInfo*>::Alloc(num);
            }
            List< AutoPtr<Activity> >::Iterator it;
            for (it = p->mActivities.Begin(), j = 0; it != p->mActivities.End(); ++it) {
                AutoPtr<Activity> activity = *it;
                Boolean enabled;
                activity->mInfo->GetEnabled(&enabled);
                if (enabled || (flags & IPackageManager::GET_DISABLED_COMPONENTS) != 0) {
                    infos->Set(j++, GenerateActivityInfo(activity, flags, state, userId));
                }
            }
            pi->SetActivities(infos);
        }
    }
    if ((flags & IPackageManager::GET_RECEIVERS) != 0) {
        Int32 N = p->mReceivers.GetSize();
        if (N > 0) {
            AutoPtr< ArrayOf<IActivityInfo*> > infos;
            if ((flags & IPackageManager::GET_DISABLED_COMPONENTS) != 0) {
                infos = ArrayOf<IActivityInfo*>::Alloc(N);
            }
            else {
                Int32 num = 0;
                List< AutoPtr<Activity> >::Iterator it;
                for (it = p->mReceivers.Begin(); it != p->mReceivers.End(); ++it) {
                    Boolean enabled;
                    (*it)->mInfo->GetEnabled(&enabled);
                    if (enabled) num++;
                }
                infos = ArrayOf<IActivityInfo*>::Alloc(num);
            }
            List< AutoPtr<Activity> >::Iterator it;
            for (it = p->mReceivers.Begin(), j = 0; it != p->mReceivers.End(); ++it) {
                AutoPtr<Activity> activity = *it;
                Boolean enabled;
                activity->mInfo->GetEnabled(&enabled);
                if (enabled || (flags & IPackageManager::GET_DISABLED_COMPONENTS) != 0) {
                    infos->Set(j++, GenerateActivityInfo(activity, flags, state, userId));
                }
            }
            pi->SetReceivers(infos);
        }
    }
    if ((flags & IPackageManager::GET_SERVICES) != 0) {
        Int32 N = p->mServices.GetSize();
        if (N > 0) {
            AutoPtr< ArrayOf<IServiceInfo*> > infos;
            if ((flags & IPackageManager::GET_DISABLED_COMPONENTS) != 0) {
                infos = ArrayOf<IServiceInfo*>::Alloc(N);
            }
            else {
                Int32 num = 0;
                List< AutoPtr<Service> >::Iterator it;
                for (it = p->mServices.Begin(); it != p->mServices.End(); ++it) {
                    Boolean enabled;
                    (*it)->mInfo->GetEnabled(&enabled);
                    if (enabled) num++;
                }
                infos = ArrayOf<IServiceInfo*>::Alloc(num);
            }
            List< AutoPtr<Service> >::Iterator it;
            for (it = p->mServices.Begin(), j = 0; it != p->mServices.End(); ++it) {
                AutoPtr<Service> service = *it;
                Boolean enabled;
                service->mInfo->GetEnabled(&enabled);
                if (enabled || (flags & IPackageManager::GET_DISABLED_COMPONENTS) != 0) {
                    infos->Set(j++, GenerateServiceInfo(service, flags, state, userId));
                }
            }
            pi->SetServices(infos);
        }
    }
    if ((flags & IPackageManager::GET_PROVIDERS) != 0) {
        Int32 N = p->mProviders.GetSize();
        if (N > 0) {
            AutoPtr< ArrayOf<IProviderInfo*> > infos;
            if ((flags & IPackageManager::GET_DISABLED_COMPONENTS) != 0) {
                infos = ArrayOf<IProviderInfo*>::Alloc(N);
            }
            else {
                Int32 num;
                List< AutoPtr<Provider> >::Iterator it;
                for (it = p->mProviders.Begin(); it != p->mProviders.End(); ++it) {
                    Boolean enabled;
                    (*it)->mInfo->GetEnabled(&enabled);
                    if (enabled) num++;
                }
                infos = ArrayOf<IProviderInfo*>::Alloc(num);
            }
            List< AutoPtr<Provider> >::Iterator it;
            for (it = p->mProviders.Begin(), j = 0; it != p->mProviders.End(); ++it) {
                AutoPtr<Provider> provider = *it;
                Boolean enabled;
                provider->mInfo->GetEnabled(&enabled);
                if (enabled || (flags & IPackageManager::GET_DISABLED_COMPONENTS) != 0) {
                    infos->Set(j++, GenerateProviderInfo(provider, flags, state, userId));
                }
            }
            pi->SetProviders(infos);
        }
    }
    if ((flags & IPackageManager::GET_INSTRUMENTATION) != 0) {
        Int32 N = p->mInstrumentation.GetSize();
        if (N > 0) {
            AutoPtr< ArrayOf<IInstrumentationInfo*> > infos =
                    ArrayOf<IInstrumentationInfo*>::Alloc(N);
            List< AutoPtr<Instrumentation> >::Iterator it;
            for (it = p->mInstrumentation.Begin(), i = 0; it != p->mInstrumentation.End(); ++it, ++i) {
                infos->Set(i, GenerateInstrumentationInfo(*it, flags));
            }
            pi->SetInstrumentation(infos);
        }
    }
    if ((flags & IPackageManager::GET_PERMISSIONS) != 0) {
        Int32 N = p->mPermissions.GetSize();
        if (N > 0) {
            AutoPtr< ArrayOf<IPermissionInfo*> > infos = ArrayOf<IPermissionInfo*>::Alloc(N);
            List< AutoPtr<Permission> >::Iterator it;
            for (it = p->mPermissions.Begin(), i = 0; it != p->mPermissions.End(); ++it, ++i) {
                infos->Set(i, GeneratePermissionInfo(*it, flags));
            }
            pi->SetPermissions(infos);
        }
        N = p->mRequestedPermissions.GetSize();
        if (N > 0) {
            AutoPtr< ArrayOf<String> > reqPermInfos = ArrayOf<String>::Alloc(N);
            AutoPtr< ArrayOf<Int32> > reqPermFlagInfos = ArrayOf<Int32>::Alloc(N);
            List<String>::Iterator sit;
            List<Boolean>::Iterator bit;
            for (sit = p->mRequestedPermissions.Begin(), bit = p->mRequestedPermissionsRequired.Begin(), i = 0;
                sit != p->mRequestedPermissions.End() && bit != p->mRequestedPermissionsRequired.End(); ++sit, ++bit, ++i) {
                String perm = *sit;
                (*reqPermInfos)[i] = perm;
                if (*bit) {
                    (*reqPermFlagInfos)[i] |= IPackageInfo::REQUESTED_PERMISSION_REQUIRED;
                }
                if (grantedPermissions != NULL && grantedPermissions->Find(perm) != grantedPermissions->End()) {
                    (*reqPermFlagInfos)[i] |= IPackageInfo::REQUESTED_PERMISSION_GRANTED;
                }
            }
            pi->SetRequestedPermissions(reqPermInfos);
            pi->SetRequestedPermissionsFlags(reqPermFlagInfos);
        }
    }
    if ((flags & IPackageManager::GET_SIGNATURES) != 0) {
        Int32 N = (p->mSignatures != NULL) ? p->mSignatures->GetLength() : 0;
        if (N > 0) {
            AutoPtr< ArrayOf<ISignature*> > signatures = ArrayOf<ISignature*>::Alloc(N);
            for (i = 0; i < N; ++i) {
                signatures->Set(i, (*p->mSignatures)[i]);
            }
            pi->SetSignatures(signatures);
        }
    }
    return pi;
}

ECode PackageParser::LoadCertificates(
    /* [in] */ IStrictJarFile* jarFile,
    /* [in] */ IZipEntry* je,
    /* [in] */ ArrayOf<Byte>* readBuffer,
    /* [out, callee] */ ICertificateArray2** result)
{
    VALIDATE_NOT_NULL(result)
    *result = NULL;

    AutoPtr<IIoUtils> ioUtils;
    AutoPtr<IInputStream> is;
    // try {
        // We must read the stream for the JarEntry to retrieve
        // its certificates.
    ECode ec = jarFile->GetInputStream(entry, (IInputStream**)&is);
    if (ec == (ECode)E_IO_EXCEPTION || ec == E_RUNTIME_EXCEPTION) {
        ec = E_PACKAGE_PARSER_EXCEPTION;
        goto _EXIT_;
    }

    ec = ReadFullyIgnoringContents(is);
    if (ec == (ECode)E_IO_EXCEPTION || ec == E_RUNTIME_EXCEPTION) {
        ec = E_PACKAGE_PARSER_EXCEPTION;
        goto _EXIT_;
    }

    ec = jarFile->GetCertificateChains(entry, result);
    if (ec == (ECode)E_IO_EXCEPTION || ec == E_RUNTIME_EXCEPTION) {
        ec = E_PACKAGE_PARSER_EXCEPTION;
        goto _EXIT_;
    }
    // } catch (IOException | RuntimeException e) {
    //     throw new PackageParserException(INSTALL_PARSE_FAILED_UNEXPECTED_EXCEPTION,
    //             "Failed reading " + entry.getName() + " in " + jarFile, e);
    // } finally {

_EXIT_:
    if (is != NULL) {
        CIoUtils::AcquireSingleton((IIoUtils**)&ioUtils);
        ioUtils->CloseQuietly(is);
    }

    return ec;
}

ECode PackageParser::ParsePackageLite(
    /* [in] */ IFile* packageFile,
    /* [in] */ Int32 flags,
    /* [out] */ PackageLite** pkgLite)
{
    Boolean isDir;
    packageFile->IsDirectory(&isDir);
    if (isDir) {
        return ParseClusterPackageLite(packageFile, flags);
    }

    return ParseMonolithicPackageLite(packageFile, flags);
}

ECode PackageParser::ParseMonolithicPackageLite(
    /* [in] */ IFile* packageFile,
    /* [in] */ Int32 flags,
    /* [out] */ PackageLite** pkgLite)
{
    AutoPtr< ApkLite > baseApk;
    FAIL_RETURN(ParseApkLite(packageFile, flags, (ApkLite**)&baseApk))

    String packagePath;
    packageFile->GetAbsolutePath(&packagePath);
    AutoPtr<PackageLite> pl = new PackageLite(packagePath, baseApk, NULL, NULL);
    *pl = pkgLite;
    REFCOUNT_ADD(*pl)
    return NOERROR;
}

ECode PackageParser::ParseClusterPackageLite(
    /* [in] */ IFile* packageDir,
    /* [in] */ Int32 flags,
    /* [out] */ PackageLite** pkgLite)
{
    VALIDATE_NOT_NULL(pkgLite)
    *pkgLite = NULL;

    AutoPtr<ArrayOf<IFile*> > files;
    packageDir->ListFiles((IFile**)&files);
    if (files == NULL || files->GetLength() == 0) {
        // throw new PackageParserException(INSTALL_PARSE_FAILED_NOT_APK,
        //         "No packages found in split");
        Logger::E(TAG, "INSTALL_PARSE_FAILED_NOT_APK : No packages found in split");
        return E_PACKAGE_PARSER_EXCEPTION;
    }

    String packageName;
    Int32 versionCode = 0;

    AutoPtr<IArrayMap> apks;
    CArrayMap::New((IArrayMap**)&apks);

    for (Int32 i = 0; i < files->GetLength(); ++i) {
        File* file = (*files)[i];
        if (IsApkFile(file)) {
            AutoPtrPtr<ApkLite> lite;
            FAIL_RETURN(ParseApkLite(file, flags, (ApkLite**)&lite);

            // Assert that all package names and version codes are
            // consistent with the first one we encounter.
            if (packageName.IsNull()) {
                packageName = lite->mPackageName;
                versionCode = lite->mVersionCode;
            }
            else {
                if (!packageName.Equals(lite->mPackageName)) {
                    // throw new PackageParserException(INSTALL_PARSE_FAILED_BAD_MANIFEST,
                    //         "Inconsistent package " + lite->mpackageName + " in " + file
                    //         + "; expected " + packageName);
                    Logger::E(TAG, "INSTALL_PARSE_FAILED_BAD_MANIFEST : Inconsistent package %s in %s; expected %s",
                        lite->mPackageName.string(), Object::ToString(files).string(), packageName.string());
                    return E_PACKAGE_PARSER_EXCEPTION;
                }
                if (versionCode != lite->mVersionCode) {
                    // throw new PackageParserException(INSTALL_PARSE_FAILED_BAD_MANIFEST,
                    //         "Inconsistent version " + lite->mversionCode + " in " + file
                    //         + "; expected " + versionCode);
                    Logger::E(TAG, "INSTALL_PARSE_FAILED_BAD_MANIFEST : Inconsistent version %d in %s; expected %d",
                        lite->mVersionCode, Object::ToString(files).string(), versionCode);
                    return E_PACKAGE_PARSER_EXCEPTION;
                }
            }

            // Assert that each split is defined only once
            AutoPtr<IInterface> old;
            AutoPtr<ICharSequence> key = CoreUtils::Convert(lite->mSplitName);
            apks->Put(TO_IINTERFACE(key), TO_IINTERFACE(lite), (IInterface**)&old);
            if (old != NULL) {
                // throw new PackageParserException(INSTALL_PARSE_FAILED_BAD_MANIFEST,
                //         "Split name " + lite->mSplitName
                //         + " defined more than once; most recent was " + file);
                Logger::E(TAG, "INSTALL_PARSE_FAILED_BAD_MANIFEST : Split name %s  defined more than once; most recent was %s",
                    lite->mSplitName, Object::ToString(file).string());
                return E_PACKAGE_PARSER_EXCEPTION;
            }
        }
    }

    AutoPtr<IInterface> old;
    apks->Remove(NULL, (IInterface**)&old);
    if (baseApk == NULL) {
        // throw new PackageParserException(INSTALL_PARSE_FAILED_BAD_MANIFEST,
        //         "Missing base APK in " + packageDir);
       Logger::E(TAG, "INSTALL_PARSE_FAILED_BAD_MANIFEST : Missing base APK in  %s", Object::ToString(packageDir).string());
        return E_PACKAGE_PARSER_EXCEPTION;
    }

    AutoPtr<ApkLite> baseApk = (ApkLite*)IObject::Probe(old);

    // Always apply deterministic ordering based on splitName
    Int32 size;
    apks->GetSize(&size);

    AutoPtr<ArrayOf<IInterface*> > splitNames;
    AutoPtr<ArrayOf<IInterface*> > splitCodePaths;
    if (size > 0) {
        splitNames = ArrayOf<IInterface*>::Alloc(size);
        splitCodePaths = ArrayOf<IInterface*>::Alloc(size);

        AutoPtr<ISet> ks;
        apks->GetKeySet((ISet**)&ks);

        AutoPtr<ArrayOf<IInterface*> > temp;
        ks->ToArray(splitNames, (ArrayOf<IInterface*>**)&temp);
        splitNames = temp;

        Arrays::Sort(splitNames, sSplitNameComparator);

        for (Int32 i = 0; i < size; i++) {
            AutoPtr<IInterface> obj;
            apks->Get((*splitNames)[i], (IInterface**)&obj);
            ApkLite* al = (ApkLite*)IObject::Probe(obj);
            AutoPtr<ICharSequence> value = CoreUtils::Convert(al->mCodePath);
            splitCodePaths->Set(i, TO_IINTERFACE(value));
        }
    }

    AutoPtr<ArrayOf<IInterface*> > names;
    AutoPtr<ArrayOf<IInterface*> > codePaths;

    if (size > 0) {
        names = ArrayOf<String>::Alloc(size);
        codePaths = ArrayOf<String>::Alloc(size);

        for (Int32 i = 0; i < size; i++) {
            names->Set(i, Object::ToString((*splitNames)[i]));
            codePaths->Set(i, Object::ToString((*splitCodePaths)[i]));
        }
    }
    String codePath;
    packageDir->GetAbsolutePath(&codePath);

    AutoPtr<PackageLite> r = new PackageLite(codePath, baseApk, names, codePaths);
    *pkgLite = r;
    REFCOUNT_ADD(*pkgLite)
    return NOERROR;
}

ECode PackageParser::ParsePackage(
    /* [in] */ IFile* packageFile,
    /* [in] */ Int32 flags,
    /* [out] */ Package** pkgLite)
{
    Boolean isDir;
    packageFile->IsDirectory(&isDir);
    if (isDir) {
        return ParseClusterPackage(packageFile, flags);
    } else {
        return ParseMonolithicPackage(packageFile, flags);
    }
}

ECode PackageParser::ParseClusterPackage(
    /* [in] */ IFile* packageDir,
    /* [in] */ Int32 flags,
    /* [out] */ Package** pkgLite)
{
    VALIDATE_NOT_NULL(pkgLite)
    *pkgLite = NULL;

    AutoPtr<PackageLite> lite;
    FAIL_RETURN(ParseClusterPackageLite(packageDir, 0, (PackageLite**)&lite))

    if (mOnlyCoreApps && !lite->mCoreApp) {
        // throw new PackageParserException(INSTALL_PARSE_FAILED_MANIFEST_MALFORMED,
        //         "Not a coreApp: " + packageDir);
        Logger::E(TAG, "INSTALL_PARSE_FAILED_MANIFEST_MALFORMED : Not a coreApp: %s",
            Object::ToString(packageDir).string())
        return E_PACKAGE_PARSER_EXCEPTION;
    }

    AutoPtr<IIoUtils> ioUtils;
    CIoUtils::AcquireSingleton((IIoUtils**)&ioUtils);

    AutoPtr<IFile> baseApk;
    CFile::New(lite->mBaseCodePath, (IFile**)&baseApk);
    AutoPtr<Package> pkg;

    AutoPtr<IAssetManager> assets;
    CAssetManager::New((IAssetManager**)&assets);

    // try {
    // Load the base and all splits into the AssetManager
    // so that resources can be overriden when parsing the manifests.
    ECode ec = LoadApkIntoAssetManager(assets, lite->mBaseCodePath, flags);
    FAIL_GOTO(ec, _EXIT_)

    if (lite->mSplitCodePaths != NULL && lite->mSplitCodePaths->GetLength() > 0) {
        for (Int32 i = 0; i < lite->mSplitCodePaths->GetLength(); ++i)
            String path = (*lite->mSplitCodePaths)[i];
            ec = LoadApkIntoAssetManager(assets, path, flags);
            FAIL_GOTO(ec, _EXIT_)
        }
    }

    ec = ParseBaseApk(baseApk, assets, flags, (Package**)&pkg);
    FAIL_GOTO(ec, _EXIT_)

    if (pkg == NULL) {
        // throw new PackageParserException(INSTALL_PARSE_FAILED_NOT_APK,
        //         "Failed to parse base APK: " + baseApk);
        Logger::E(TAG, "INSTALL_PARSE_FAILED_NOT_APK: Failed to parse base APK: %s",
            Object::ToString(baseApk).string());
        ec = E_PACKAGE_PARSER_EXCEPTION;
        goto _EXIT_;
    }

    if (lite->mSplitNames != NULL && lite->mSplitNames->GetLength() > 0) {
        Int32 num = lite->mSplitNames->GetLength();
        pkg->mSplitNames = lite->mSplitNames;
        pkg->mSplitCodePaths = lite->mSplitCodePaths;
        pkg->mSplitFlags = ArrayOf<Int32>::Alloc(num);

        for (Int32 i = 0; i < num; i++) {
            ec = ParseSplitApk(pkg, i, assets, flags);
            FAIL_GOTO(ec, _EXIT_)
        }
    }

    packageDir->GetAbsolutePath(&pkg->mCodePath);

_EXIT_:
    // } finally {
    ioUtils->CloseQuietly(assets);
    // }

    *pkgLite = pkg;
    REFCOUNT_ADD(*pkgLite)
    return NOERROR;
}

ECode PackageParser::ParseMonolithicPackage(
    /* [in] */ IFile* apkFile,
    /* [in] */ Int32 flags,
    /* [out] */ Package** pkgLite)
{
    VALIDATE_NOT_NULL(pkgLite)
    *pkgLite = NULL;

    if (mOnlyCoreApps) {
        PackageLite lite = parseMonolithicPackageLite(apkFile, flags);
        if (!lite->mcoreApp) {
            throw new PackageParserException(INSTALL_PARSE_FAILED_MANIFEST_MALFORMED,
                    "Not a coreApp: " + apkFile);
        }
    }

    AutoPtr<IAssetManager> assets;
    CAssetManager::New((IAssetManager**)&assets);
    AutoPtr<Package> pkg;

    // try {
    ECode ec = ParseBaseApk(apkFile, assets, flags, (Package**)&pkg);
    apkFile->GetAbsolutePath(&pkg->mCodePath);

_EXIT_:
    // } finally {
    ioUtils->CloseQuietly(assets);
    // }

    *pkgLite = pkg;
    REFCOUNT_ADD(*pkgLite)
    return NOERROR;
}


ECode PackageParser::LoadApkIntoAssetManager(
    /* [in] */ IAssetManager* assets,
    /* [in] */ const String& apkPath,
    /* [in] */ Int32 flags,
    /* [out] */ Int32* result)
{
    VALIDATE_NOT_NULL(result)
    *result = 0;

    if ((flags & PARSE_MUST_BE_APK) != 0 && !IsApkPath(apkPath)) {
        // throw new PackageParserException(INSTALL_PARSE_FAILED_NOT_APK,
        //         "Invalid package file: " + apkPath);
        Logger::E(TAG, "INSTALL_PARSE_FAILED_NOT_APK Invalid package file: ", apkPath.string());
        return E_PACKAGE_PARSER_EXCEPTION;
    }

    // The AssetManager guarantees uniqueness for asset paths, so if this asset path
    // already exists in the AssetManager, addAssetPath will only return the cookie
    // assigned to it.
    Int32 cookie;
    assets->AddAssetPath(apkPath, &cookie);
    if (cookie == 0) {
        // throw new PackageParserException(INSTALL_PARSE_FAILED_BAD_MANIFEST,
        //         "Failed adding asset path: " + apkPath);
        Logger::E(TAG, "INSTALL_PARSE_FAILED_NOT_APK Failed adding asset path: ", apkPath.string());
        return E_PACKAGE_PARSER_EXCEPTION;
    }

    *result = cookie;
    return NOERROR;
}

ECode PackageParser::ParseBaseApk(
    /* [in] */ IFile* apkFile,
    /* [in] */ IAssetManager* assets,
    /* [in] */ Int32 flags,
    /* [out] */ Package** pkg)
{
    VALIDATE_NOT_NULL(pkg)
    *pkg = NULL;

    String apkPath;
    apkFile->GetAbsolutePath(&apkPath);

    mParseError = IPackageManager::INSTALL_SUCCEEDED;
    apkFile->GetAbsolutePath(&mArchiveSourcePath);

    if (DEBUG_JAR) Slogger::D(TAG, "Scanning base APK: %s", apkPath.string());

    Int32 cookie;
    FAIL_RETURN(LoadApkIntoAssetManager(assets, apkPath, flags, &cookie))

    AutoPtr<Package> pkg;
    AutoPtr<ArrayOf<String> > outError = ArrayOf<String>::Alloc(1);
    AutoPtr<IResources> res;
    AutoPtr<IXmlResourceParser> parser;
    // try {
    CResources::New(assets, mMetrics, NULL, (IResources**)&res);
    assets->SetConfiguration(0, 0, String(NULL), 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
            Build::VERSION::RESOURCES_SDK_INT);
    ECode ec = assets->OpenXmlResourceParser(cookie, ANDROID_MANIFEST_FILENAME, (IXmlResourceParser**)&parser);
    FAIL_GOTO(ec, _EXIT_)

    ec = ParseBaseApk(res, parser, flags, outError, (Package**)&pkg);
    FAIL_GOTO(ec, _EXIT_)

    if (pkg == NULL) {
        // throw new PackageParserException(mParseError,
        //         apkPath + " (at " + parser.getPositionDescription() + "): " + outError[0]);
        String pos;
        IXmlPullParser::Probe(parser)->GetPositionDescription(&pos);
        Logger::E(TAG, "%d %s (at %d): %s", mParseError, apkPath.string(), pos.string(),(*outError)[0].string());
        ec = E_PACKAGE_PARSER_EXCEPTION;
        goto _EXIT_;
    }

    pkg->mBaseCodePath = apkPath;
    pkg->mSignatures = NULL;

    // } catch (PackageParserException e) {
    //     throw e;
    // } catch (Exception e) {
    //     throw new PackageParserException(INSTALL_PARSE_FAILED_UNEXPECTED_EXCEPTION,
    //             "Failed to read manifest from " + apkPath, e);
    // } finally {
    //     IoUtils.closeQuietly(parser);
    // }

_EXIT_:
    if (FAILED(ec) && ec != (ECode)E_PACKAGE_PARSER_EXCEPTION) {
        ec = E_PACKAGE_PARSER_EXCEPTION;
        Logger::E(TAG, "INSTALL_PARSE_FAILED_UNEXPECTED_EXCEPTION Failed to read manifest from %s",
            apkPath.string());
    }
    // } finally {
    ioUtils->CloseQuietly(assets);
    // }

    *pkgLite = pkg;
    REFCOUNT_ADD(*pkgLite)
    return NOERROR;
}

ECode PackageParser::ParseSplitApk(
    /* [in] */ Package* pkg,
    /* [in] */ Int32 splitIndex,
    /* [in] */ IAssetManager* assets,
    /* [in] */ Int32 flags)
{
    String apkPath = (*pkg->mSplitCodePaths)[splitIndex];
    AutoPtr<IFile> apkFile;
    CFile::New(apkPath, (IFile**)&apkFile);

    mParseError = IPackageManager::INSTALL_SUCCEEDED;
    mArchiveSourcePath = apkPath;

    if (DEBUG_JAR) Slogger::D(TAG, "Scanning split APK: %s", apkPath.string());

    Int32 cookie;
    FAIL_RETURN(LoadApkIntoAssetManager(assets, apkPath, flags, &cookie))

    AutoPtr<Package> outPkg;
    AutoPtr<ArrayOf<String> > outError = ArrayOf<String>::Alloc(1);
    AutoPtr<IResources> res;
    AutoPtr<IXmlResourceParser> parser;
    // try {
    CResources::New(assets, mMetrics, NULL, (IResources**)&res);
    assets->SetConfiguration(0, 0, String(NULL), 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
            Build::VERSION::RESOURCES_SDK_INT);
    ECode ec = assets->OpenXmlResourceParser(cookie, ANDROID_MANIFEST_FILENAME, (IXmlResourceParser**)&parser);
    FAIL_GOTO(ec, _EXIT_)

    ec = ParseSplitApk(pkg, res, parser, flags, splitIndex, outError, (Package**)&outPkg);
    FAIL_GOTO(ec, _EXIT_)

    if (outPkg == NULL) {
        // throw new PackageParserException(mParseError,
        //         apkPath + " (at " + parser.getPositionDescription() + "): " + outError[0]);
        String pos;
        IXmlPullParser::Probe(parser)->GetPositionDescription(&pos);
        Logger::E(TAG, "%d %s (at %d): %s", mParseError, apkPath.string(), pos.string(),(*outError)[0].string());
        ec = E_PACKAGE_PARSER_EXCEPTION;
        goto _EXIT_;
    }

    // } catch (PackageParserException e) {
    //     throw e;
    // } catch (Exception e) {
    //     throw new PackageParserException(INSTALL_PARSE_FAILED_UNEXPECTED_EXCEPTION,
    //             "Failed to read manifest from " + apkPath, e);
    // } finally {
    //     IoUtils.closeQuietly(parser);
    // }

_EXIT_:
    if (FAILED(ec) && ec != (ECode)E_PACKAGE_PARSER_EXCEPTION) {
        ec = E_PACKAGE_PARSER_EXCEPTION;
        Logger::E(TAG, "INSTALL_PARSE_FAILED_UNEXPECTED_EXCEPTION Failed to read manifest from %s",
            apkPath.string());
    }
    // } finally {
    ioUtils->CloseQuietly(assets);
    // }

    *pkgLite = outPkg;
    REFCOUNT_ADD(*pkgLite)
    return NOERROR;
}

ECode PackageParser::ParseSplitApk(
    /* [in] */ Package* pkg,
    /* [in] */ IResources* res,
    /* [in] */ IXmlResourceParser* parser,
    /* [in] */ Int32 flags
    /* [in] */ Int32 splitIndex,
    /* [in] */ ArrayOf<String>* outError,
    /* [out] */ Package** result)
{
    VALIDATE_NOT_NULL(result)
    *result = NULL;

    AutoPtr<IAttributeSet> attrs = IAttributeSet::Probe(parser);
    IXmlPullParser* xpp = IXmlPullParser::Probe(parser);

    // We parsed manifest tag earlier; just skip past it
    FAIL_RETURN(ParsePackageSplitNames(parser, attrs, flags));

    mParseInstrumentationArgs = NULL;
    mParseActivityArgs = NULL;
    mParseServiceArgs = NULL;
    mParseProviderArgs = NULL;

    Int32 type, depth, outerDepth;

    Boolean foundApp = FALSE, bval;
    xpp->GetDepth(&outerDepth);
    while ((xpp->Next(&type), type) != IXmlPullParser::END_DOCUMENT
            && (type != IXmlPullParser::END_TAG || (xpp->GetDepth(&depth), depth) > outerDepth)) {
        if (type == IXmlPullParser::END_TAG || type == IXmlPullParser::TEXT) {
            continue;
        }

        String tagName;
        xpp->GetName(&tagName);
        if (tagName.Equals("application")) {
            if (foundApp) {
                if (RIGID_PARSER) {
                    (*outError)[0] = String("<manifest> has more than one <application>");
                    mParseError = IPackageManager::INSTALL_PARSE_FAILED_MANIFEST_MALFORMED;
                    return NOERROR;
                } else {
                    Slogger::W(TAG, "<manifest> has more than one <application>");
                    assert(0 && "TODO");
                    // XmlUtils::SkipCurrentTag(parser);
                    continue;
                }
            }

            foundApp = TRUE;
            FAIL_RETURN(ParseSplitApplication(pkg, res, parser, attrs, flags, splitIndex, outError, &bval))
            if (!bval) {
                return NOERROR;
            }

        }
        else if (RIGID_PARSER) {
            StringBuilder sb("Bad element under <manifest>: ");
            String name;
            xpp->GetName(&name);
            sb += name;
            (*outError)[0] = sb.ToString();
            mParseError = IPackageManager::INSTALL_PARSE_FAILED_MANIFEST_MALFORMED;
            return NOERROR;

        }
        else {
            String name, pos;
            xpp->GetName(&name);
            parser->GetPositionDescription(&pos);
            Slogger::W(TAG, "Unknown element under <manifest>: %s at %s %s",
                name.string(), mArchiveSourcePath.string(), pos.string());
            assert(0 && "TODO");
            // XmlUtils::SkipCurrentTag(parser);
            continue;
        }
    }

    if (!foundApp) {
        (*outError)[0] = String("<manifest> does not contain an <application>");
        mParseError = IPackageManager::INSTALL_PARSE_FAILED_MANIFEST_EMPTY;
    }

    *result = pkg;
    REFCOUNT_ADD(*result)
    return NOERROR;
}

ECode PackageParser::CollectManifestDigest(
    /* [in] */ Package* pkg)
{
    pkg->mManifestDigest = NULL;

    // TODO: extend to gather digest for split APKs
    // try {
    AutoPtr<IStrictJarFile> jarFile;
    CStrictJarFile::New(pkg->mBaseCodePath, (IStrictJarFile**)&jarFile);
    // try {
    AutoPtr<IZipEntry> je;
    ECode ec = jarFile->FindEntry(ANDROID_MANIFEST_FILENAME, (IZipEntry**)&je);
    FAIL_GOTO(ec, _EXIT_)

    if (je != NULL) {
        pkg->mManifestDigest = NULL;
        AutoPtr<IInputStream> is;
        ec = jarFile->GetInputStream(je, (IInputStream**)&is);
        FAIL_GOTO(ec, _EXIT_)

        ec = CManifestDigest::FromInputStream(is, (IManifestDigest**)&pkg->mManifestDigest);
    }
    // } finally {
_EXIT_:
    jarFile->Close();

    if (ec == (ECode)E_IO_EXCEPTION || ec == (ECode)E_RUNTIME_EXCEPTION) {
        ec = E_PACKAGE_PARSER_EXCEPTION;
        Logger::E(TAG, "INSTALL_PARSE_FAILED_MANIFEST_MALFORMED : Failed to collect manifest digest");
    }
    // }
    // } catch (IOException | RuntimeException e) {
    //     throw new PackageParserException(INSTALL_PARSE_FAILED_MANIFEST_MALFORMED,
    //             "Failed to collect manifest digest");
    // }
    return ec;
}

ECode PackageParser::CollectCertificates(
    /* [in] */ Package* pkg,
    /* [in] */ Int32 flags,
    /* [in] */ ArrayOf<Byte>* readBuffer)
{
    // assert(readBuffer != NULL && readBuffer->GetLength() >= 8192);
    assert(pkg != NULL);

    Int64 startTime = SystemClock::GetUptimeMillis();
    Int64 endTime, cost;

    pkg->mCertificates = NULL;
    pkg->mSignatures = NULL;
    pkg->mSigningKeys = NULL;

    AutoPtr<IFile> bcpFile;
    CFile::New(pkg->mBaseCodePath, (IFile**)&bcpFile);
    FAIL_RETURN(CollectCertificates(pkg, bcpFile, flags, readBuffer))

    if (pkg->mSplitCodePaths != NULL) {
        for (Int32 i = 0; i < pkg->mSplitCodePaths->GetLength(); ++i) {
            String splitCodePath = (*pkg->mSplitCodePaths)[i];
            AutoPtr<IFile> scpFile;
            CFile::New(splitCodePath, (IFile**)&scpFile);
            FAIL_RETURNCollectCertificates(pkg, scpFile, flags, readBuffer))
        }
    }

    endTime = SystemClock::GetUptimeMillis();
    cost = endTime - startTime;
    Slogger::D(TAG, "  == Elastos CollectCertificates %s cost: %lld ms", pkg->mPackageName.string(), cost);
    return NOERROR;



    // Int64 startTime = SystemClock::GetUptimeMillis();
    // Int64 endTime, cost;

    // pkg->mSignatures = NULL;

    // AutoPtr<IJarFile> jarFile;
    // CJarFile::New(mArchiveSourcePath, (IJarFile**)&jarFile);
    // AutoPtr<ArrayOf<ICertificate*> > certs;

    // if ((flags & PARSE_IS_SYSTEM) != 0) {
    //     // If this package comes from the system image, then we
    //     // can trust it...  we'll just use the AndroidManifest.xml
    //     // to retrieve its signatures, not validating all of the
    //     // files.
    //     AutoPtr<IJarEntry> jarEntry;
    //     jarFile->GetJarEntry(ANDROID_MANIFEST_FILENAME, (IJarEntry**)&jarEntry);
    //     certs = LoadCertificates(jarFile, jarEntry, readBuffer);

    //     if (certs == NULL) {
    //         if (DEBUG_JAR) {
    //             String name;
    //             jarEntry->GetName(&name);
    //             StringBuilder sb("Package ");
    //             sb += pkg->mPackageName;
    //             sb += " has no certificates at entry ";
    //             sb += name;
    //             sb += "; ignoring!";
    //             Slogger::I(TAG, sb.ToString().string());
    //         }

    //         jarFile->Close();
    //         mParseError = IPackageManager::INSTALL_PARSE_FAILED_NO_CERTIFICATES;
    //         //Todo, wait apache to open here.
    //         //return FALSE;
    //         endTime = SystemClock::GetUptimeMillis();
    //         cost = endTime - startTime;
    //         Slogger::D(TAG, "  == Elastos CollectCertificates %s 1 cost: %lld ms", pkg->mPackageName.string(), cost);
    //         return TRUE;
    //     }

    //     if (DEBUG_JAR) {
    //         String name;
    //         StringBuilder sb("File ");
    //         sb += mArchiveSourcePath + ": entry=" + (jarEntry->GetName(&name), name)
    //             + " certs=";
    //         sb += (certs != NULL ? certs->GetLength() : 0);
    //         Slogger::I(TAG, sb.ToString().string());
    //         if (certs != NULL) {
    //             const Int32 N = certs->GetLength();
    //             for (Int32 i = 0; i < N; i++) {
    //                 StringBuilder sb("  Public key: ");
    //                 String pks;
    //                 AutoPtr<IPublicKey> pubKey;
    //                 (*certs)[i]->GetPublicKey((IPublicKey**)&pubKey);
    //                 AutoPtr<ArrayOf<Byte> > encoded;
    //                 pubKey->GetEncoded((ArrayOf<Byte>**)&encoded);
    //                 sb.Append((const char*)encoded->GetPayload());
    //                 IObject::Probe(pubKey.Get())->ToString(&pks);
    //                 sb += pks;
    //                 Slogger::I(TAG, sb.ToString().string());
    //             }
    //         }
    //     }
    // }
    // else {
    //     AutoPtr<IObjectContainer> entries;
    //     jarFile->GetEntries((IObjectContainer**)&entries);
    //     AutoPtr<IManifest> manifest;
    //     jarFile->GetManifest((IManifest**)&manifest);
    //     AutoPtr<IObjectEnumerator> enumerator;
    //     entries->GetObjectEnumerator((IObjectEnumerator**)&enumerator);
    //     String ename, name;
    //     Boolean isDir;
    //     Boolean hasNext = FALSE;
    //     AutoPtr<IManifestDigestHelper> hlp;
    //     CManifestDigestHelper::AcquireSingleton((IManifestDigestHelper**)&hlp);

    //     while (enumerator->MoveNext(&hasNext), hasNext) {
    //         AutoPtr<IInterface> elm;
    //         enumerator->Current((IInterface**)&elm);
    //         IZipEntry::Probe(elm)->GetName(&ename);

    //         AutoPtr<IJarEntry> je;
    //         jarFile->GetEntry(ename, (IZipEntry**)&je);
    //         if (je->IsDirectory(&isDir), isDir) {
    //             continue;
    //         }

    //         je->GetName(&name);
    //         if (name.StartWith("META-INF/")) {
    //             continue;
    //         }

    //         if (ANDROID_MANIFEST_FILENAME.Equals(name)) {
    //             // TODO: manifest is NULL, so skip the current entry \"AndroidManifest.xml\". by caojing
    //             continue;

    //             AutoPtr<IAttributes> attributes;
    //             manifest->GetAttributes(name, (IAttributes**)&attributes);
    //             pkg->mManifestDigest = NULL;
    //             hlp->FromAttributes(attributes, (IManifestDigest**)&pkg->mManifestDigest);
    //         }

    //         AutoPtr<ArrayOf<ICertificate*> > localCerts = LoadCertificates(jarFile, je, readBuffer);
    //         if (DEBUG_JAR) {
    //             StringBuilder sb("File ");
    //             sb += " entry ";
    //             String name;
    //             je->GetName(&name);
    //             sb += name + ": certs=";
    //             sb += (const char*)certs->GetPayload();
    //             sb += " (";
    //             sb += (certs != NULL ? certs->GetLength() : 0);
    //             sb += ")";
    //             Slogger::I(TAG, sb.ToString().string());
    //         }

    //         if (localCerts == NULL) {
    //             if (DEBUG_JAR) {
    //                 StringBuilder sb("Package ");
    //                 sb += pkg->mPackageName + " has no certificates at entry ";
    //                 String name;
    //                 sb += (je->GetName(&name), name);
    //                 sb += "; ignoring!";
    //                 Slogger::I(TAG, sb.ToString().string());
    //             }

    //             jarFile->Close();
    //             mParseError = IPackageManager::INSTALL_PARSE_FAILED_NO_CERTIFICATES;
    //             //Todo, wait apache to open here.
    //             // return FALSE;
    //             endTime = SystemClock::GetUptimeMillis();
    //             cost = endTime - startTime;
    //             Slogger::D(TAG, "  == Elastos CollectCertificates %s 2 cost: %lld ms", pkg->mPackageName.string(), cost);
    //             return TRUE;
    //         }
    //         else if (certs == NULL) {
    //             certs = localCerts;
    //         }
    //         else {
    //             // Ensure all certificates match.
    //             Boolean found = FALSE, isEqual;
    //             for (Int32 i = 0; i < certs->GetLength(); i++) {
    //                 found = FALSE;
    //                 for (Int32 j = 0; j < localCerts->GetLength(); j++) {
    //                     if ((*certs)[i] != NULL &&
    //                             ((*certs)[i]->Equals((*localCerts)[j], &isEqual), isEqual)) {
    //                         found = TRUE;
    //                         break;
    //                     }
    //                 }

    //                 if (!found || certs->GetLength() != localCerts->GetLength()) {
    //                     if (DEBUG_JAR) {
    //                         StringBuilder sb("Package ");
    //                         sb += pkg->mPackageName + " has mismatched certificates at entry ";
    //                         String name;
    //                         je->GetName(&name);
    //                         sb += name + "; ignoring!";
    //                         Slogger::I(TAG, sb.ToString().string());
    //                     }
    //                     jarFile->Close();
    //                     mParseError = IPackageManager::INSTALL_PARSE_FAILED_INCONSISTENT_CERTIFICATES;
    //                     endTime = SystemClock::GetUptimeMillis();
    //                     cost = endTime - startTime;
    //                     Slogger::D(TAG, "  == Elastos CollectCertificates %s 3 cost: %lld ms", pkg->mPackageName.string(), cost);
    //                     return FALSE;
    //                 }
    //             }
    //         }
    //     } // end while
    // } // end else

    // jarFile->Close();

    // Boolean result = TRUE;
    // if (certs != NULL && certs->GetLength() > 0) {
    //     const Int32 N = certs->GetLength();
    //     pkg->mSignatures = ArrayOf<ISignature*>::Alloc(N);
    //     for (Int32 i = 0; i < N; i++) {
    //         AutoPtr<ISignature> sign;
    //         AutoPtr<ArrayOf<Byte> > code;
    //         ////////code is byte data after signature for digest hash, then we need to verify it by public key.
    //         (*certs)[i]->GetEncoded((ArrayOf<Byte>**)&code);
    //         CSignature::New(code, (ISignature**)&sign);
    //         pkg->mSignatures->Set(i, sign);
    //     }
    // }
    // else {
    //     if (DEBUG_JAR) {
    //         StringBuilder sb("Package ");
    //         sb += pkg->mPackageName + " has no certificates; ignoring!";
    //         Slogger::I(TAG, sb.ToString().string());
    //     }
    //     mParseError = IPackageManager::INSTALL_PARSE_FAILED_NO_CERTIFICATES;
    //     result = FALSE;
    // }

    // endTime = SystemClock::GetUptimeMillis();
    // cost = endTime - startTime;
    // Slogger::D(TAG, "  == Elastos CollectCertificates %s 4 cost: %lld ms", pkg->mPackageName.string(), cost);
    // return result;
}

ECode PackageParser::CollectCertificates(
    /* [in] */ Package* pkg,
    /* [in] */ IFile* apkFile,
    /* [in] */ Int32 flags,
    /* [in] */ ArrayOf<Byte>* readBuffer)
{
    String apkPath;
    apkFile->GetAbsolutePath(&apkPath);

    AutoPtr<IIoUtils> ioUtils;
    CIoUtils::AcquireSingleton((IIoUtils**)&ioUtils);
    AutoPtr<IStrictJarFile> jarFile;
    AutoPtr<IIterator> it;
    Boolean hasNext, bval;
    AutoPtr<IInterface> obj;
    String name;
    IZipEntry* entry;

    // try {
    AutoPtr<IZipEntry> manifestEntry;
    List<AutoPtr<IZipEntry> toVerify;
    List<AutoPtr<IZipEntry>::Iterator lit;
    AutoPtr<ISignature> entrySignatures;
    AutoPtr< ICertificateArray2 > entryCerts;
    ICertificate* cert;
    AutoPtr<IPublicKey> pubKey;

    ECode ec = CStrictJarFile::New(apkPath, (IStrictJarFile**)&jarFile);
    FAIL_GOTO(ec, _EXIT_)

    // Always verify manifest, regardless of source

    ec = jarFile->FindEntry(ANDROID_MANIFEST_FILENAME, (IZipEntry**)&manifestEntry);
    FAIL_GOTO(ec, _EXIT_)

    if (manifestEntry == NULL) {
        // throw new PackageParserException(INSTALL_PARSE_FAILED_BAD_MANIFEST,
        //         "Package " + apkPath + " has no manifest");
        Logger::E(TAG, "INSTALL_PARSE_FAILED_BAD_MANIFEST : Package %s has no manifest", apkPath.string());
        ec = E_PACKAGE_PARSER_EXCEPTION;
        goto _EXIT_;
    }

    toVerify->PushBack(manifestEntry);

    // If we're parsing an untrusted package, verify all contents
    if ((flags & PARSE_IS_SYSTEM) == 0) {
        jarFile->GetIterator((IIterator**)&it);
        while (it->HasNext(&hasNext), hasNext) {
            obj = NULL;
            it->GetNext((IInterface**)&obj);
            entry = IZipEntry::Probe(obj);

            if (entry->IsDirectory(&bval), bval) continue;
            entry->GetName(&name);
            if (name->StartWith("META-INF/")) continue;
            if (name->Equals(ANDROID_MANIFEST_FILENAME)) continue;

            toVerify->PushBack(entry);
        }
    }

    // Verify that entries are signed consistently with the first entry
    // we encountered. Note that for splits, certificates may have
    // already been populated during an earlier parse of a base APK.
    for (lit = toVerify.Begin(); lit != toVerify.End(); ++it) {
        entry = *it;
        entryCerts = NULL;
        ec = LoadCertificates(jarFile, entry, readBuffer, (ICertificateArray2**)&entryCerts);
        FAIL_GOTO(ec, _EXIT_)

        if (entryCerts == NULL || entryCerts->GetLength() == 0) {
            // throw new PackageParserException(INSTALL_PARSE_FAILED_NO_CERTIFICATES,
            //         "Package " + apkPath + " has no certificates at entry "
            //         + entry.getName());
            entry->GetName(&name);
            Logger::E(TAG, "INSTALL_PARSE_FAILED_NO_CERTIFICATES : Package %s has no certificates at entry %s",
                apkPath.string(), name.string());
            ec = E_PACKAGE_PARSER_EXCEPTION;
            goto _EXIT_;
        }

        entrySignatures = ConvertToSignatures(entryCerts);

        if (pkg->mCertificates == NULL) {
            pkg->mCertificates = entryCerts;
            pkg->mSignatures = entrySignatures;
            pkg->mSigningKeys = NULL;
            CArraySet::New((IArraySet**)&pkg->mSigningKeys);
            for (Int32 i = 0; i < entryCerts->GetLength(); i++) {
                cert = (*((*entryCerts)[i]))[0];
                pubKey = NULL;
                cert->GetPublicKey((IPublicKey**)&pubKey);
                pkg->mSigningKeys->Add(TO_IINTERFACE(pubKey));
            }
        }
        else {
            if (!CSignature::AreExactMatch(pkg->mSignatures, entrySignatures)) {
                // throw new PackageParserException(
                //         INSTALL_PARSE_FAILED_INCONSISTENT_CERTIFICATES, "Package " + apkPath
                //                 + " has mismatched certificates at entry "
                //                 + entry.getName());
                entry->GetName(&name);
                Logger::E(TAG, "INSTALL_PARSE_FAILED_INCONSISTENT_CERTIFICATES : Package %s  has mismatched certificates at entry %s",
                    apkPath.string(), name.string());
                ec = E_PACKAGE_PARSER_EXCEPTION;
                goto _EXIT_;
            }
        }
    }

_EXIT_:
    ioUtils->CloseQuietly(jarFile);

    if (ec == (ECode)E_GENERAL_SECURITY_EXCEPTION) {
        Logger::E(TAG, "INSTALL_PARSE_FAILED_CERTIFICATE_ENCODING : Failed to collect certificates from %s", apkPath.string());
        ec = E_PACKAGE_PARSER_EXCEPTION;
    }
    else if (ec == (ECode)E_IO_EXCEPTION || ec == (ECode)E_RUNTIME_EXCEPTION) {
        Logger::E(TAG, "INSTALL_PARSE_FAILED_NO_CERTIFICATES : Failed to collect certificates from %s", apkPath.string());
        ec = E_PACKAGE_PARSER_EXCEPTION;
    }
    // catch (GeneralSecurityException e) {
    //     throw new PackageParserException(INSTALL_PARSE_FAILED_CERTIFICATE_ENCODING,
    //             "Failed to collect certificates from " + apkPath, e);
    // } catch (IOException | RuntimeException e) {
    //     throw new PackageParserException(INSTALL_PARSE_FAILED_NO_CERTIFICATES,
    //             "Failed to collect certificates from " + apkPath, e);
    // } finally {
    //     closeQuietly(jarFile);
    // }
    return ec;
}

AutoPtr<ArrayOf<ISignature*> > PackageParser::ConvertToSignatures(
    /* [in] */ ICertificateArray2* certs)
{
    AutoPtr<ArrayOf<ISignature*> > res = ArrayOf<ISignature*>::Alloc(certs->GetLength());

    for (Int32 i = 0; i < certs->GetLength(); i++) {
        AutoPtr<ISignature> sig;
        CSignature::New((*certs)[i], (ISignature**)&sig);
        res->Set(i, (ISignature**)&sig);
    }

    return res;
}

ECode PackageParser::ParseApkLite(
    /* [in] */ IFile* apkFile,
    /* [in] */ Int32 flags,
    /* [out] */ PackageParser::ApkLite** apkLite)
{
    VALIDATE_NOT_NULL(apkLite)
    *apkLite = NULL;

    String apkPath;
    apkFile->GetAbsolutePath(&apkPath);
    AutoPtr<IAttributeSet> attrs = IAttributeSet::Probe(parser);

    AutoPtr<IIoUtils> ioUtils;
    CIoUtils::AcquireSingleton((IIoUtils**)&ioUtils);

    AutoPtr<IAssetManager> assets;
    AutoPtr<IXmlResourceParser> parser;
    AutoPtr<IResources> res;
    AutoPtr<IDisplayMetrics> metrics;
    AutoPtr<ArrayOf<ISignature*> > signatures;;
    AutoPtr<Package> tempPkg;

    // try {
    ECode ec = CAssetManager::New((IAssetManager**)&assets);
    FAIL_GOTO(ec, _EXIT_)

    ec = assets->SetConfiguration(0, 0, String(NULL), 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
            Build::VERSION::RESOURCES_SDK_INT);
    FAIL_GOTO(ec, _EXIT_)

    Int32 cookie;
    ec = assets->AddAssetPath(packageFilePath, &cookie);
    FAIL_GOTO(ec, _EXIT_)

    if (cookie == 0) {
//         throw new PackageParserException(INSTALL_PARSE_FAILED_NOT_APK,
//                 "Failed to parse " + apkPath);
        Logger::E(TAG, "INSTALL_PARSE_FAILED_NOT_APK : Failed to parse %s", apkPath);
        ec = E_PACKAGE_PARSER_EXCEPTION;
        goto _EXIT_;
    }

    CDisplayMetrics::New((IDisplayMetrics**)&metrics);
    metrics->SetToDefaults();

    ec = CResources::New(assets, metrics, NULL, (IResources**)&res);
    FAIL_GOTO(ec, _EXIT_)

    ec = assets->OpenXmlResourceParser(cookie, ANDROID_MANIFEST_FILENAME, (IXmlResourceParser**)&parser);
    FAIL_GOTO(ec, _EXIT_)


    if ((flags & PARSE_COLLECT_CERTIFICATES) != 0) {
        // TODO: factor signature related items out of Package object
        tempPkg = new Package(String(NULL));
        ec = CollectCertificates(tempPkg, apkFile, 0, readBuffer);
        FAIL_GOTO(ec, _EXIT_)

        signatures = tempPkg->mSignatures;
    }

    ec = ParseApkLite(apkPath, res, parser, attrs, flags, signatures, apkLite);

_EXIT_:
    if (ec == (ECode)E_XML_PULL_PARSER_EXCEPTION
        || ec == (ECode)E_IO_EXCEPTION
        || ec == (ECode)E_RUNTIME_EXCEPTION) {
        Logger::E(TAG, "INSTALL_PARSE_FAILED_UNEXPECTED_EXCEPTION : Failed to parser %s", apkPath.string());
        ec = E_PACKAGE_PARSER_EXCEPTION;
    }
    // } catch (XmlPullParserException | IOException | RuntimeException e) {
    //     throw new PackageParserException(INSTALL_PARSE_FAILED_UNEXPECTED_EXCEPTION,
    //             "Failed to parse " + apkPath, e);
    // } finally {
    ioUtils->CloseQuietly(parser);
    ioUtils->CloseQuietly(assets);
    // }
    return ec;
}

String PackageParser::ValidateName(
    /* [in] */ const String& name,
    /* [in] */ Boolean requiresSeparator)
{
    AutoPtr<ArrayOf<Char32> > charArray = name.GetChars();
    const Int32 N = charArray->GetLength();
    Char32 c;
    Boolean hasSep = FALSE;
    Boolean front = TRUE;
    for (Int32 i = 0; i < N; i++) {
        c = name.GetChar(i);
        if ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z')) {
            front = FALSE;
            continue;
        }
        if (!front) {
            if ((c >= '0' && c <= '9') || c == '_') {
                continue;
            }
        }
        if (c == '.') {
            hasSep = TRUE;
            front = TRUE;
            continue;
        }
        StringBuilder sb;
        sb.Append(String("bad character '"));
        sb.AppendChar(c);
        sb.AppendChar('\'');
        return sb.ToString();
    }
    if (hasSep || !requiresSeparator) {
        return String(NULL);
    }
    else {
        return String("must have at least one '.' separator");
    }
}

ECode PackageParser::ParsePackageSplitNames(
    /* [in] */ IXmlPullParser* parser,
    /* [in] */ IAttributeSet* attrs,
    /* [in] */ Int32 flags,
    /* [out, callee] */ ArrayOf<String>* pairString)
{
    VALIDATE_NOT_NULL(pairString)
    *pairString = NULL;

    Int32 type;
    while ((parser->Next(&type), type) != IXmlPullParser::START_TAG
            && type != IXmlPullParser::END_DOCUMENT) {
    }

    if (type != IXmlPullParser::START_TAG) {
        // throw new PackageParserException(INSTALL_PARSE_FAILED_MANIFEST_MALFORMED,
        //         "No start tag found");
        Logger::E(TAG, "INSTALL_PARSE_FAILED_MANIFEST_MALFORMED : No start tag found!");
        return E_PACKAGE_PARSER_EXCEPTION;
    }

    String name;
    parser->GetName(&name);
    if (!name.Equals("manifest")) {
        // throw new PackageParserException(INSTALL_PARSE_FAILED_MANIFEST_MALFORMED,
        //         "No <manifest> tag");
        Logger::E(TAG, "INSTALL_PARSE_FAILED_MANIFEST_MALFORMED : No <manifest> tag!");
        return E_PACKAGE_PARSER_EXCEPTION;
    }

    String packageName;
    attrs->GetAttributeValue(String(NULL), String("package"), &packageName);
    if (!packageName.Equals("android")) {
        String error = ValidateName(packageName, TRUE);
        if (!error.IsNull()) {
            // throw new PackageParserException(INSTALL_PARSE_FAILED_BAD_PACKAGE_NAME,
            //         "Invalid manifest package: " + error);
            Logger::E(TAG, "INSTALL_PARSE_FAILED_BAD_PACKAGE_NAME : Invalid manifest package: %s", error.string());
            return E_PACKAGE_PARSER_EXCEPTION;
        }
    }

    String splitName;
    attrs->GetAttributeValue(String(NULL), String("split"), &splitName);
    if (!splitName.IsNull()) {
        if (splitName.IsEmpty()) {
            splitName = String(NULL);
        }
        else {
            String error = ValidateName(splitName, FALSE);
            if (!error.IsNull()) {
                // throw new PackageParserException(INSTALL_PARSE_FAILED_BAD_PACKAGE_NAME,
                //         "Invalid manifest split: " + error);
                Logger::E(TAG, "INSTALL_PARSE_FAILED_BAD_PACKAGE_NAME : Invalid manifest split: %s", error.string());
                return E_PACKAGE_PARSER_EXCEPTION;
            }
        }
    }

    AutoPtr<ArrayOf<String> > pair = ArrayOf<String>Alloc(2);
    pair->Set(0, packageName);
    pair->Set(1, splitName);
    *pairString = pair;
    REFCOUNT_ADD(*pairString)
    return NOERROR;
}

ECode PackageParser::ParseApkLite(
    /* [in] */ const String& codePath,
    /* [in] */ IResources* res,
    /* [in] */ IXmlResourceParser* parser,
    /* [in] */ IAttributeSet* attrs,
    /* [in] */ Int32 flags,
    /* [in] */ ArrayOf<ISignature*>* signatures,
    /* [out] */ PackageParser::ApkLite** apkLite)
{
    VALIDATE_NOT_NULL(apkLite)
    *apkLite = NULL;

    AutoPtr<ArrayOf<String> > packageSplit;
    FAIL_RETURN(ParsePackageSplitNames(parser, attrs, flags, (ArrayOf<String>**)&packageSplit))

    Int32 installLocation = PARSE_DEFAULT_INSTALL_LOCATION;
    Int32 versionCode = 0;
    Boolean coreApp = FALSE;
    Boolean multiArch = FALSE;

    Int32 numFound = 0;
    Int32 count;
    attrs->GetAttributeCount(&count);
    for (Int32 i = 0; i < count; i++) {
        String attr;
        attrs->GetAttributeName(i, &attr);
        if (attr.Equals("installLocation")) {
            attrs->GetAttributeIntValue(i, PARSE_DEFAULT_INSTALL_LOCATION, &installLocation);
            numFound++;
        }
        else if (attr.Equals("versionCode")) {
            attrs->GetAttributeIntValue(i, 0, &versionCode);
            numFound++;
        }
        else if (attr.Equals("coreApp")) {
            attrs->GetAttributeBooleanValue(i, FALSE, &coreApp);
            numFound++;
        }
        if (numFound >= 3) {
            break;
        }
    }

    // Only search the tree when the tag is directly below <manifest>
    Int32 type;
    Int32 searchDepth, depth;
    parser->GetDepth(&searchDepth);
    searchDepth += 1;

    String name;
    AutoPtr<List<AutoPtr<VerifierInfo> > > = new List<AutoPtr<VerifierInfo> >();
    while ((parser->Next(&type), type) != IXmlPullParser::END_DOCUMENT
            && (type != IXmlPullParser::END_TAG || (parser->GetDepth(&depth), depth) >= searchDepth)) {
        if (type == IXmlPullParser::END_TAG || type == IXmlPullParser::TEXT) {
            continue;
        }

        parser->GetDepth(&depth);
        parser->GetName(&name);
        if (depth == searchDepth && name.Equals("package-verifier")) {
            VerifierInfo verifier = parseVerifier(res, parser, attrs, flags);
            if (verifier != NULL) {
                verifiers.add(verifier);
            }
        }

        parser->GetDepth(&depth);
        parser->GetName(&name);
        if (depth == searchDepth && name.Equals("application")) {
            attrs->GetAttributeCount(&count);
            for (Int32 i = 0; i < count; ++i) {
                String attr;
                attrs->GetAttributeName(i, &attr);
                if (attr->Equals("multiArch")) {
                    attrs->GetAttributeBooleanValue(i, FALSE, &multiArch);
                    break;
                }
            }
        }
    }

    AutoPtr<ApkLite> al = new ApkLite(codePath, (*packageSplit)[0], (*packageSplit)[1],
        versionCode, installLocation, verifiers, signatures, coreApp, multiArch);
    *apkLite = al;
    REFCOUNT_ADD(*apkLite)
    return NOERROR;
}

AutoPtr<ISignature> PackageParser::StringToSignature(
    /* [in] */ const String& str)
{
    AutoPtr< ArrayOf<Char32> > chars = str.GetChars();
    AutoPtr< ArrayOf<Byte> > sig = ArrayOf<Byte>::Alloc(chars->GetLength());
    for (Int32 i = 0; i < chars->GetLength(); ++i) {
        sig->Set(i, (Byte)(*chars)[i]);
    }
    AutoPtr<ISignature> signature;
    CSignature::New(sig, (ISignature**)&signature);
    return signature;

    // final int N = str.length();
    // byte[] sig = new byte[N];
    // for (int i=0; i<N; i++) {
    //     sig[i] = (byte)str.charAt(i);
    // }
    // return new Signature(sig);
}

ECode PackageParser::ParseBaseApk(
    /* [in] */ IResources* res,
    /* [in] */ IXmlResourceParser* parser,
    /* [in] */ Int32 flags,
    /* [in] */ ArrayOf<String>* outError,
    /* [out] */ Package** result)
{
    VALIDATE_NOT_NULL(result)
    *result = NULL;

    Boolean trustedOverlay = (flags & PARSE_TRUSTED_OVERLAY) != 0;

    AutoPtr<IAttributeSet> attrs = IAttributeSet::Probe(parser);
    assert(attrs);

    mParseInstrumentationArgs = NULL;
    mParseActivityArgs = NULL;
    mParseServiceArgs = NULL;
    mParseProviderArgs = NULL;

    String pkgName;
    String splitName;
    // try {
    AutoPtr<ArrayOf<String> > packageSplit;
    ec = ParsePackageSplitNames(parser, attrs, flags, (ArrayOf<String>**)&packageSplit);
    // } catch (PackageParserException e) {
    if (ec == (ECode)E_PACKAGE_PARSER_EXCEPTION)
        mParseError = IPackageManager::INSTALL_PARSE_FAILED_BAD_PACKAGE_NAME;
        return NOERROR;
    // }

    pkgName = (*packageSplit)[0];
    splitName = (*packageSplit)[1];

    Int32 type;

    if (!splitName.IsNullOrEmpty()) {
        StringBuilder sb("Expected base APK, but found split ");
        sb += splitName;
        (*outError)[0] = sb.ToString();
        mParseError = IPackageManager::INSTALL_PARSE_FAILED_BAD_PACKAGE_NAME;
        return NOERROR;
    }

    AutoPtr<PackageParser::Package> pkg = new PackageParser::Package(pkgName);
    Boolean foundApp = FALSE;

    AutoPtr<ITypedArray> a;
    Int32 size = ARRAY_SIZE(R::styleable::AndroidManifest);
    AutoPtr<ArrayOf<Int32> > layout = ArrayOf<Int32>::Alloc(size);
    layout->Copy(R::styleable::AndroidManifest, size);

    AutoPtr<ITypedArray> sa;
    ASSERT_SUCCEEDED(res->ObtainAttributes(
        attrs, layout, (ITypedArray**)&sa));
    sa->GetInteger(
        R::styleable::AndroidManifest_versionCode, 0, &pkg->mVersionCode);
    pkg->mApplicationInfo->SetVersionCode(pkg->mVersionCode);
    sa->GetNonConfigurationString(
        R::styleable::AndroidManifest_versionName, 0, &pkg->mVersionName);
    // if (pkg->mVersionName != NULL) {
    //     pkg->mVersionName = pkg->mVersionName.intern();
    // }
    String str;
    sa->GetNonConfigurationString(
            R::styleable::AndroidManifest_sharedUserId, 0, &str);
    if (!str.IsNullOrEmpty()) {
        String nameError = ValidateName(str, TRUE);
        if (!nameError.IsNull() && !pkgName.Equals("android")) {
            // (*outError)[0] = "<manifest> specifies bad sharedUserId name \""
            //         + str + "\": " + nameError;
            StringBuilder("<manifest> specifies bad sharedUserId name \"");
            sb += str;
            sb += "\": ";
            sb += nameError;
            (*outError)[0] =  sb.ToString();
            mParseError = IPackageManager::INSTALL_PARSE_FAILED_BAD_SHARED_USER_ID;
            return NOERROR;
        }
        pkg->mSharedUserId = str;
        sa->GetResourceId(
            R::styleable::AndroidManifest_sharedUserLabel, 0, &pkg->mSharedUserLabel);
    }

    sa->GetInteger(
        R::styleable::AndroidManifest_installLocation,
        PARSE_DEFAULT_INSTALL_LOCATION, &pkg->mInstallLocation);
    pkg->mApplicationInfo->SetInstallLocation(pkg->mInstallLocation);

    attrs->GetAttributeBooleanValue(NULL, String("coreApp"), FALSE, &pkg->mCoreApp);

    sa->Recycle();

    /* Set the global "forward lock" flag */
    if ((flags & PARSE_FORWARD_LOCK) != 0) {
        Int32 appFlags;
        pkg->mApplicationInfo->GetFlags(&appFlags);
        appFlags |= IApplicationInfo::FLAG_FORWARD_LOCK;
        pkg->mApplicationInfo->SetFlags(appFlags);
    }

    /* Set the global "on SD card" flag */
    if ((flags & PARSE_ON_SDCARD) != 0) {
        Int32 appFlags;
        pkg->mApplicationInfo->GetFlags(&appFlags);
        appFlags |= IApplicationInfo::FLAG_EXTERNAL_STORAGE;
        pkg->mApplicationInfo->SetFlags(appFlags);
    }

    // Resource Boolean are -1, so 1 means we don't know the value.
    Int32 supportsSmallScreens = 1;
    Int32 supportsNormalScreens = 1;
    Int32 supportsLargeScreens = 1;
    Int32 supportsXLargeScreens = 1;
    Int32 resizeable = 1;
    Int32 anyDensity = 1;

    Int32 outerDepth = 0;
    parser->GetDepth(&outerDepth);
    Int32 type = 0;
    Int32 depth = 0;
    while ((parser->Next(&type), type) != IXmlPullParser::END_DOCUMENT
           && (type != IXmlPullParser::END_TAG
           || (parser->GetDepth(&depth), depth) > outerDepth)) {
        if (type == IXmlPullParser::END_TAG || type == IXmlPullParser::TEXT) {
            continue;
        }

        String tagName;
        parser->GetName(&tagName);
        if (tagName.Equals("application")) {
            if (foundApp) {
                if (RIGID_PARSER) {
                    (*outError)[0] = "<manifest> has more than one <application>";
                    mParseError = IPackageManager::INSTALL_PARSE_FAILED_MANIFEST_MALFORMED;
                    return NULL;
                }
                else {
                    Slogger::W(TAG, "<manifest> has more than one <application>");
                    XmlUtils::SkipCurrentTag(IXmlPullParser::Probe(parser));
                    continue;
                }
            }

            foundApp = TRUE;
            if (!ParseBaseApplication(pkg, res, IXmlPullParser::Probe(parser), attrs, flags, outError)) {
                return NULL;
            }
        }
        else if (tagName.Equals("overlay")) {
            pkg->mTrustedOverlay = trustedOverlay;

            sa = NULL;
            res->ObtainAttributes(attrs,
                R::styleable::AndroidManifestResourceOverlay, (ITypedArray**)&sa);
            sa->GetString(
                R::styleable::AndroidManifestResourceOverlay_targetPackage, &pkg->mOverlayTarget);
            sa->GetInt32(
                R::styleable::AndroidManifestResourceOverlay_priority, -1, &pkg->mOverlayPriority);
            sa->Recycle();

            if (pkg->mOverlayTarget == NULL) {
                (*outError)[0] = String("<overlay> does not specify a target package");
                mParseError = IPackageManager::INSTALL_PARSE_FAILED_MANIFEST_MALFORMED;
                return NOERROR;
            }
            if (pkg->mOverlayPriority < 0 || pkg->mOverlayPriority > 9999) {
                (*outError)[0] = "<overlay> priority must be between 0 and 9999";
                mParseError = IPackageManager::INSTALL_PARSE_FAILED_MANIFEST_MALFORMED;
                return NOERROR;
            }
            assert(0 && "TODO");
            //XmlUtils::SkipCurrentTag(parser);

        } else if (tagName.Equals("key-sets")) {
            Boolean bval;
            FAIL_RETURN(ParseKeySets(pkg, res, parser, attrs, outError, &bval))
            if (!bval) {
                return NOERROR;
            }
        }
        else if (tagName.Equals("permission-group")) {
            AutoPtr<PermissionGroup> group;
            FAIL_RETURN(ParsePermissionGroup(pkg, flags, res, parser, attrs, outError, (PermissionGroup**)&group))
            if (group == NULL) {
                return NOERROR;
            }
        }
        else if (tagName.Equals("permission")) {
            AutoPtr<Permission> permission;
            FAIL_RETURN(ParsePermission(pkg, res, parser, attrs, outError, (Permission**)&permission))
            if (permission == NULL) {
                return NOERROR;
            }
        }
        else if (tagName.Equals("permission-tree")) {
            AutoPtr<Permission> permission;
            FAIL_RETURN(ParsePermissionTree(pkg, res, parser, attrs, outError, (Permission**)&permission))
            if (permission == NULL) {
                return NOERROR;
            }
        }
        else if (tagName.Equals("uses-permission")) {
            Boolean bval;
            FAIL_RETURN(ParseUsesPermission(pkg, res, parser, attrs, outError, &bval))
           if (!bval) {
                return NOERROR;
            }
        }
        else if (tagName.Equals("uses-configuration")) {
            AutoPtr<IConfigurationInfo> cPref;
            ASSERT_SUCCEEDED(CConfigurationInfo::New((IConfigurationInfo**)&cPref));
            sa = NULL;

            size = ARRAY_SIZE(R::styleable::AndroidManifestUsesConfiguration);
            layout = ArrayOf<Int32>::Alloc(size);
            layout->Copy(R::styleable::AndroidManifestUsesConfiguration, size);

            ASSERT_SUCCEEDED(res->ObtainAttributes(attrs, layout, (ITypedArray**)&sa));
            Int32 ivalue;
            sa->GetInt32(
                R::styleable::AndroidManifestUsesConfiguration_reqTouchScreen,
                IConfiguration::TOUCHSCREEN_UNDEFINED, &ivalue);
            cPref->SetReqTouchScreen(ivalue);
            sa->GetInt32(
                R::styleable::AndroidManifestUsesConfiguration_reqKeyboardType,
                IConfiguration::KEYBOARD_UNDEFINED, &ivalue);
            cPref->SetReqKeyboardType(ivalue);
            Boolean value = FALSE;
            sa->GetBoolean(
                R::styleable::AndroidManifestUsesConfiguration_reqHardKeyboard,
                FALSE, &value);
            if (value) {
                Int32 req;
                cPref->GetReqInputFeatures(&req);
                req |= IConfigurationInfo::INPUT_FEATURE_HARD_KEYBOARD;
                cPref->SetReqInputFeatures(req);
            }
            sa->GetInt32(
                R::styleable::AndroidManifestUsesConfiguration_reqNavigation,
                IConfiguration::NAVIGATION_UNDEFINED, &ivalue);
            cPref->SetReqNavigation(ivalue);
            sa->GetBoolean(
                R::styleable::AndroidManifestUsesConfiguration_reqFiveWayNav,
                FALSE, &value);
            if (value) {
                Int32 req;
                cPref->GetReqInputFeatures(&req);
                req |= IConfigurationInfo::INPUT_FEATURE_FIVE_WAY_NAV;
                cPref->SetReqInputFeatures(req);
            }
            sa->Recycle();

            if (pkg->mConfigPreferences == NULL) {
                pkg->mConfigPreferences = new List<AutoPtr<IConfigurationInfo> >();
            }
            pkg->mConfigPreferences->PushBack(cPref);

            assert(0 && "TODO");
            // XmlUtils::SkipCurrentTag(parser);
        }
        else if (tagName.Equals("uses-feature")) {
            AutoPtr<IFeatureInfo> fi;
            FAIL_RETURN(ParseUsesFeature(res, attrs, (IFeatureInfo**)&fi))
            if (pkg->mReqFeatures == NULL) {
                pkg->mReqFeatures = new List<AutoPtr<IFeatureInfo> >();
            }
            pkg->mReqFeatures->PushBack(fi);

            String name;
            fi->GetName(&name);
            if (name.IsNull()) {
                AutoPtr<IConfigurationInfo> cPref;
                CConfigurationInfo::New((IConfigurationInfo**)&cPref);
                Int32 version;
                fi->GetReqGlEsVersion(&version);
                cPref->SetReqGlEsVersion(version);
                if (pkg->mConfigPreferences == NULL) {
                    pkg->mConfigPreferences = new List<AutoPtr<IConfigurationInfo> >();
                }
                pkg->mConfigPreferences->PushBack(cPref);
            }

            assert(0 && "TODO");
            // XmlUtils::SkipCurrentTag(parser);
        }
        else if (tagName.Equals("feature-group")) {
            AutoPtr<IFeatureGroupInfo> group;
            CFeatureGroupInfo::New((IFeatureGroupInfo**)&group);
            AutoPtr<List<AutoPtr<FeatureInfo> > > features;
            Int32 innerDepth, depth;
            parser->GetDepth(&innerDepth);
            while ((parser->Next(&type), type) != IXmlPullParser::END_DOCUMENT
                    && (type != IXmlPullParser::END_TAG || (parser->GetDepth(&depth), depth) > innerDepth)) {
                if (type == IXmlPullParser::END_TAG || type == IXmlPullParser::TEXT) {
                    continue;
                }

                String innerTagName;
                parser->GetName(&innerTagName);
                if (innerTagName.Equals("uses-feature")) {
                    AutoPtr<IFeatureInfo> featureInfo;
                    FAIL_RETURN(ParseUsesFeature(res, attrs, (IFeatureInfo**)&featureInfo))
                    // FeatureGroups are stricter and mandate that
                    // any <uses-feature> declared are mandatory.
                    CFeatureInfo* cfi = (CFeatureInfo*)featureInfo.Get();
                    cfi->mFlags |= IFeatureInfo::FLAG_REQUIRED;
                    if (features == NULL) {
                        features = new List<AutoPtr<FeatureInfo> >();
                    }
                    features->PushBack(featureInfo);
                }
                else {
                    String pos;
                    parser->GetPositionDescription(&pos);
                    Slogger::W(TAG, "Unknown element under <feature-group>: %s at %s %s",
                        innerTagName.string(), mArchiveSourcePath.string(), pos.string());
                }

                assert(0 && "TODO");
                // XmlUtils::SkipCurrentTag(parser);
            }

            if (features != NULL) {
                AutoPtr<ArrayOf<IFeatureInfo*> > f = ArrayOf<IFeatureInfo*>::Alloc(features->GetSize());
                List<AutoPtr<FeatureInfo> > it = features->Begin();
                for (Int32 i = 0; it != featureInfo->End(); ++it) {
                    f->Set(i++, *it);
                }
                group->SetFeatures(f);
            }

            if (pkg->mFeatureGroups == NULL) {
                pkg->mFeatureGroups = new List<AutoPtr<IFeatureGroupInfo> >();
            }
            pkg->mFeatureGroups->PushBack(group);
        }

        else if (tagName.Equals("uses-sdk")) {
            if (SDK_VERSION > 0) {
                sa = NULL;

                size = ARRAY_SIZE(R::styleable::AndroidManifestUsesSdk);
                layout = ArrayOf<Int32>::Alloc(size);
                layout->Copy(R::styleable::AndroidManifestUsesSdk, size);

                ASSERT_SUCCEEDED(res->ObtainAttributes(attrs, layout, (ITypedArray**)&sa));

                Int32 minVers = 0;
                String minCode;
                Int32 targetVers = 0;
                String targetCode;

                AutoPtr<ITypedValue> val;
                sa->PeekValue(
                        R::styleable::AndroidManifestUsesSdk_minSdkVersion, (ITypedValue**)&val);
                if (val != NULL) {
                    Int32 valType;
                    val->GetType(&valType);
                    AutoPtr<ICharSequence> valString;
                    val->GetString((ICharSequence**)&valString);
                    if (valType == ITypedValue::TYPE_STRING && valString != NULL) {
                        String str;
                        valString->ToString(&str);
                        targetCode = minCode = str;
                    }
                    else {
                        // If it's not a string, it's an integer.
                        Int32 valData;
                        val->GetData(&valData);
                        targetVers = minVers = valData;
                    }
                }

                val = NULL;
                sa->PeekValue(
                        R::styleable::AndroidManifestUsesSdk_targetSdkVersion, (ITypedValue**)&val);
                if (val != NULL) {
                    Int32 valType;
                    val->GetType(&valType);
                    AutoPtr<ICharSequence> valString;
                    val->GetString((ICharSequence**)&valString);
                    if (valType == ITypedValue::TYPE_STRING && valString != NULL) {
                        String str;
                        valString->ToString(&str);
                        targetCode = minCode = str;
                    }
                    else {
                        // If it's not a string, it's an integer.
                        Int32 valData;
                        val->GetData(&valData);
                        targetVers = valData;
                    }
                }

                sa->Recycle();

                if (!minCode.IsNull()) {
                    Boolean allowedCodename = FALSE;
                    for (Int32 i = 0; i < SDK_CODENAMES->GetLength(); ++i) {
                        if (minCode.Equals((*SDK_CODENAMES)[i]) {
                            allowedCodename = TRUE;
                            break;
                        }
                    }

                    if (!allowedCodename) {
                        if (SDK_CODENAMES->GetLength() > 0) {
                            StringBuilder sb("Requires development platform ");
                            sb += minCode;
                            sb += " (current platform is any of ";
                            sb += Arrays:ToString(SDK_CODENAMES);
                            sb += ")";
                            (*outError)[0] = sb.ToString();
                        }
                        else {
                            StringBuilder sb("Requires development platform ");
                            sb += minCode;
                            sb += " but this is a release platform.";
                            (*outError)[0] = sb.ToString();
                        }

                        mParseError = IPackageManager::INSTALL_FAILED_OLDER_SDK;
                        return NOERROR;
                    }
                }
                else if (minVers > SDK_VERSION) {
                    StringBuilder sb("Requires newer sdk version #");
                    sb += minVers;
                    sb += " (current version is #";
                    sb += SDK_VERSION;
                    sb += ")";
                    (*outError)[0] = sb.ToString();
                    mParseError = IPackageManager::INSTALL_FAILED_OLDER_SDK;
                    return NOERROR;
                }

                if (!targetCode.IsNull()) {
                    Boolean allowedCodename = FALSE;
                    for (Int32 i = 0; i < SDK_CODENAMES->GetLength(); ++i) {
                        if (targetCode.Equals((*SDK_CODENAMES)[i]) {
                            allowedCodename = TRUE;
                            break;
                        }
                    }

                    if (!allowedCodename) {
                        if (SDK_CODENAMES->GetLength() > 0) {
                            StringBuilder sb("Requires development platform ");
                            sb += targetCode;
                            sb += " (current platform is any of ";
                            sb += Arrays:ToString(SDK_CODENAMES);
                            sb += ")";
                            (*outError)[0] = sb.ToString();
                        }
                        else {
                            StringBuilder sb("Requires development platform ");
                            sb += minCode;
                            sb += " but this is a release platform.";
                            (*outError)[0] = sb.ToString();
                        }

                        mParseError = IPackageManager::INSTALL_FAILED_OLDER_SDK;
                        return NOERROR;
                    }
                    // If the code matches, it definitely targets this SDK.
                    pkg->mApplicationInfo->SetTargetSdkVersion(Build::VERSION_CODES::CUR_DEVELOPMENT);
                }
                else {
                    pkg->mApplicationInfo->SetTargetSdkVersion(targetVers);
                }
            }

            assert(0 && "TODO");
            //XmlUtils::SkipCurrentTag(parser);
        }
        else if (tagName.Equals("supports-screens")) {
            sa = NULL;
            size = ARRAY_SIZE(R::styleable::AndroidManifestSupportsScreens);
            layout = ArrayOf<Int32>::Alloc(size);
            layout->Copy(R::styleable::AndroidManifestSupportsScreens, size);

            ASSERT_SUCCEEDED(res->ObtainAttributes(attrs, layout, (ITypedArray**)&sa));

            Int32 temp;
            sa->GetInteger(
                R::styleable::AndroidManifestSupportsScreens_requiresSmallestWidthDp, 0, &temp);
            pkg->mApplicationInfo->SetRequiresSmallestWidthDp(temp);
            sa->GetInteger(
                R::styleable::AndroidManifestSupportsScreens_compatibleWidthLimitDp, 0, &temp);
            pkg->mApplicationInfo->SetCompatibleWidthLimitDp(temp);
            sa->GetInteger(
                R::styleable::AndroidManifestSupportsScreens_largestWidthLimitDp, 0, &temp);
            pkg->mApplicationInfo->SetLargestWidthLimitDp(temp);

            // This is a trick to get a Boolean and still able to detect
            // if a value was actually set
            sa->GetInteger(
                R::styleable::AndroidManifestSupportsScreens_smallScreens,
                supportsSmallScreens, &supportsSmallScreens);
            sa->GetInteger(
                R::styleable::AndroidManifestSupportsScreens_normalScreens,
                supportsNormalScreens, &supportsNormalScreens);
            sa->GetInteger(
                R::styleable::AndroidManifestSupportsScreens_largeScreens,
                supportsLargeScreens, &supportsLargeScreens);
            sa->GetInteger(
                R::styleable::AndroidManifestSupportsScreens_xlargeScreens,
                supportsXLargeScreens, &supportsXLargeScreens);
            sa->GetInteger(
                R::styleable::AndroidManifestSupportsScreens_resizeable,
                resizeable, &resizeable);
            sa->GetInteger(
                R::styleable::AndroidManifestSupportsScreens_anyDensity,
                anyDensity, &anyDensity);

            sa->Recycle();

            assert(0 && "TODO");
            // XmlUtils::SkipCurrentTag(parser);
        }
        else if (tagName.Equals("protected-broadcast")) {
            sa = NULL;
            size = ARRAY_SIZE(R::styleable::AndroidManifestProtectedBroadcast);
            layout = ArrayOf<Int32>::Alloc(size);
            layout->Copy(R::styleable::AndroidManifestProtectedBroadcast, size);

            ASSERT_SUCCEEDED(res->ObtainAttributes(attrs, layout, (ITypedArray**)&sa));

            // Note: don't allow this value to be a reference to a resource
            // that may change
            String name;
            sa->GetNonResourceString(
                R::styleable::AndroidManifestProtectedBroadcast_name, &name);

            sa->Recycle();

            if (!name.IsNull() && (flags & PARSE_IS_SYSTEM) != 0) {
                if (pkg->mProtectedBroadcasts == NULL) {
                    pkg->mProtectedBroadcasts = new List<String>();
                }
                if (Find(pkg->mProtectedBroadcasts->Begin(), pkg->mProtectedBroadcasts->End(), name)
                    == pkg->mProtectedBroadcasts->End()) {
                    pkg->mProtectedBroadcasts->PushBack(name);
                }
            }

            assert(0 && "TODO");
            //XmlUtils::SkipCurrentTag(parser);
        }
        else if (tagName.Equals("instrumentation")) {
            if (ParseInstrumentation(pkg, res, parser, attrs, outError) == NULL) {
                return NOERROR;
            }
        }
        else if (tagName.Equals("original-package")) {
            sa = NULL;
            size = ARRAY_SIZE(R::styleable::AndroidManifestOriginalPackage);
            layout = ArrayOf<Int32>::Alloc(size);
            layout->Copy(R::styleable::AndroidManifestOriginalPackage, size);

            ASSERT_SUCCEEDED(res->ObtainAttributes(attrs, layout, (ITypedArray**)&sa));

            String orig;
            sa->GetNonConfigurationString(
                R::styleable::AndroidManifestOriginalPackage_name, 0, &orig);
            if (!pkg->mPackageName.Equals(orig)) {
                if (pkg->mOriginalPackages == NULL) {
                    pkg->mOriginalPackages = new List<String>();
                    pkg->mRealPackage = pkg->mPackageName;
                }
                pkg->mOriginalPackages->PushBack(orig);
            }

            sa->Recycle();

            assert(0 && "TODO");
            //XmlUtils::SkipCurrentTag(parser);
        }
        else if (tagName.Equals("adopt-permissions")) {
            sa = NULL;
            size = ARRAY_SIZE(R::styleable::AndroidManifestOriginalPackage);
            layout = ArrayOf<Int32>::Alloc(size);
            layout->Copy(R::styleable::AndroidManifestOriginalPackage, size);

            ASSERT_SUCCEEDED(res->ObtainAttributes(attrs, layout, (ITypedArray**)&sa));

            String name;
            sa->GetNonConfigurationString(
                R::styleable::AndroidManifestOriginalPackage_name, 0, &name);

            sa->Recycle();

            if (!name.IsNull()) {
                if (pkg->mAdoptPermissions == NULL) {
                    pkg->mAdoptPermissions = new List<String>();
                }
                pkg->mAdoptPermissions->PushBack(name);
            }

            assert(0 && "TODO");
            //XmlUtils::SkipCurrentTag(parser);
        }
        else if (tagName.Equals("uses-gl-texture")) {
            // Just skip this tag
            assert(0 && "TODO");
            //XmlUtils::SkipCurrentTag(parser);
            continue;
        }
        else if (tagName.Equals("compatible-screens")) {
            // Just skip this tag
            assert(0 && "TODO");
            //XmlUtils::SkipCurrentTag(parser);
            continue;
        }
        else if (tagName.Equals("supports-input")) {
            assert(0 && "TODO");
            //XmlUtils::SkipCurrentTag(parser);
            continue;
        }
        else if (tagName.Equals("eat-comment")) {
            // Just skip this tag
            assert(0 && "TODO");
            //XmlUtils::SkipCurrentTag(parser);
            continue;
        }
        else if (RIGID_PARSER) {
            // (*outError)[0] = (const char*)(
            //     StringBuffer("Bad element under <manifest>: ") + tagName);
            mParseError = IPackageManager::INSTALL_PARSE_FAILED_MANIFEST_MALFORMED;
            return NULL;

        }
        else {
            String des;
            parser->GetPositionDescription(&des);
            // Logger::W(TAG, StringBuffer("Unknown element under <manifest>: ") + tagName
            //     + " at " + mArchiveSourcePath + " " + des);
            assert(0 && "TODO");
            //XmlUtils::SkipCurrentTag(parser);
            continue;
        }
    }

    if (!foundApp && pkg->mInstrumentation.Begin() == pkg->mInstrumentation.End()) {
        (*outError)[0] = "<manifest> does not contain an <application> or <instrumentation>";
        mParseError = IPackageManager::INSTALL_PARSE_FAILED_MANIFEST_EMPTY;
    }

    const Int32 NP = NEW_PERMISSIONS->GetLength();
    AutoPtr<StringBuilder> implicitPerms;
    for (Int32 ip = 0; ip < NP; ip++) {
        AutoPtr<NewPermissionInfo> npi = (*NEW_PERMISSIONS)[ip];
        Int32 sdkVersion;
        pkg->mApplicationInfo->GetTargetSdkVersion(&sdkVersion);
        if (sdkVersion >= npi->mSdkVersion) {
            break;
        }
        if (Find(pkg->mRequestedPermissions.Begin(), pkg->mRequestedPermissions.End(), npi->mName)
            == pkg->mRequestedPermissions.End()) {
            if (implicitPerms == NULL) {
                implicitPerms = new StringBuilder(128);
                implicitPerms->AppendString(pkg->mPackageName);
                implicitPerms->AppendString(String(": compat added "));
            }
            else {
                implicitPerms->AppendChar(' ');
            }
            implicitPerms->AppendString(npi->mName);
            pkg->mRequestedPermissions.PushBack(npi->mName);
            pkg->mRequestedPermissionsRequired.PushBack(TRUE);
        }
    }
    if (implicitPerms != NULL) {
        Slogger::I(TAG, implicitPerms->ToString());
    }

    const Int32 NS = SPLIT_PERMISSIONS->GetLength();
    for (Int32 is = 0; is < NS; is++) {
        AutoPtr<PackageParser::SplitPermissionInfo> spi = (*SPLIT_PERMISSIONS)[is];
        Int32 targetSdkVersion;
        pkg->mApplicationInfo->GetTargetSdkVersion(&targetSdkVersion);
        if (targetSdkVersion >= spi->mTargetSdk
                || Find(pkg->mRequestedPermissions.Begin(), pkg->mRequestedPermissions.End(), spi->mRootPerm)
            == pkg->mRequestedPermissions.End()) {
            continue;
        }
        for (Int32 in = 0; in < spi->mNewPerms->GetLength(); in++) {
            const String perm = (*spi->mNewPerms)[in];
            if (Find(pkg->mRequestedPermissions.Begin(), pkg->mRequestedPermissions.End(), perm)
                == pkg->mRequestedPermissions.End()) {
                pkg->mRequestedPermissions.PushBack(perm);
                pkg->mRequestedPermissionsRequired.PushBack(TRUE);
            }
        }
    }

    Int32 pkgSdkVersion, pkgFlags;
    pkg->mApplicationInfo->GetTargetSdkVersion(&pkgSdkVersion);
    if (supportsSmallScreens < 0 || (supportsSmallScreens > 0
        && pkgSdkVersion >= Build::VERSION_CODES::DONUT)) {
        pkg->mApplicationInfo->GetFlags(&pkgFlags);
        pkgFlags |= IApplicationInfo::FLAG_SUPPORTS_SMALL_SCREENS;
        pkg->mApplicationInfo->SetFlags(pkgFlags);
    }
    if (supportsNormalScreens != 0) {
        pkg->mApplicationInfo->GetFlags(&pkgFlags);
        pkgFlags |= IApplicationInfo::FLAG_SUPPORTS_NORMAL_SCREENS;
        pkg->mApplicationInfo->SetFlags(pkgFlags);
    }
    if (supportsLargeScreens < 0 || (supportsLargeScreens > 0
        && pkgSdkVersion >= Build::VERSION_CODES::DONUT)) {
        pkg->mApplicationInfo->GetFlags(&pkgFlags);
        pkgFlags |= IApplicationInfo::FLAG_SUPPORTS_LARGE_SCREENS;
        pkg->mApplicationInfo->SetFlags(pkgFlags);
    }
    if (supportsXLargeScreens < 0 || (supportsXLargeScreens > 0
        && pkgSdkVersion >= Build::VERSION_CODES::GINGERBREAD)) {
        pkg->mApplicationInfo->GetFlags(&pkgFlags);
        pkgFlags |= IApplicationInfo::FLAG_SUPPORTS_XLARGE_SCREENS;
        pkg->mApplicationInfo->SetFlags(pkgFlags);
    }
    if (resizeable < 0 || (resizeable > 0
        && pkgSdkVersion >= Build::VERSION_CODES::DONUT)) {
        pkg->mApplicationInfo->GetFlags(&pkgFlags);
        pkgFlags |= IApplicationInfo::FLAG_RESIZEABLE_FOR_SCREENS;
        pkg->mApplicationInfo->SetFlags(pkgFlags);
    }
    if (anyDensity < 0 || (anyDensity > 0
        && pkgSdkVersion >= Build::VERSION_CODES::DONUT)) {
        pkg->mApplicationInfo->GetFlags(&pkgFlags);
        pkgFlags |= IApplicationInfo::FLAG_SUPPORTS_SCREEN_DENSITIES;
        pkg->mApplicationInfo->SetFlags(pkgFlags);
    }

    /*
     * b/8528162: Ignore the <uses-permission android:required> attribute if
     * targetSdkVersion < JELLY_BEAN_MR2. There are lots of apps in the wild
     * which are improperly using this attribute, even though it never worked.
     */
    Int32 targetVersion;
    pkg->mApplicationInfo->GetTargetSdkVersion(&targetVersion);
    if (targetVersion < Build::VERSION_CODES::JELLY_BEAN_MR2) {
        for (Int32 i = 0; i < pkg->mRequestedPermissionsRequired.GetSize(); i++) {
            pkg->mRequestedPermissionsRequired->Set(i, TRUE);
        }
    }

    *result = pkg;
    REFCOUNT_ADD(*result)
    return NOERROR;
}

ECode PackageParser::ParseUsesFeature(
    /* [in] */ IResources* res,
    /* [in] */ IAttributeSet* attrs,
    /* [out] */ IFeatureInfo** info)
{
    VALIDATE_NOT_NULL(info)
    *info = NULL;

    AutoPtr<IFeatureInfo> fi;
    CFeatureInfo::New((IFeatureInfo**)&fi);

    Int32 size = ARRAY_SIZE(R::styleable::AndroidManifestUsesFeature);
    AutoPtr< ArrayOf<Int32> > layout = ArrayOf<Int32>::Alloc(size);
    layout->Copy(R::styleable::AndroidManifestUsesFeature, size);

    AutoPtr<ITypedArray> sa;
    res->ObtainAttributes(attrs, layout, (ITypedArray**)&sa);
    // Note: don't allow this value to be a reference to a resource
    // that may change.
    String name;
    sa->GetNonResourceString(
        R::styleable::AndroidManifestUsesFeature_name, &name);
    fi->SetName(&name);
    if (name.IsNull()) {
        Int32 ival;
        sa->GetInt32(
            R::styleable::AndroidManifestUsesFeature_glEsVersion,
            IFeatureInfo::GL_ES_VERSION_UNDEFINED, &ival);
        fi->SetReqGlEsVersion(ival);
    }
    Boolean bval;
    sa->GetBoolean(
        R::styleable::AndroidManifestUsesFeature_required, TRUE, &bval);
    if (bval) {
        fi->GetFlags(&ival);
        ival |= IFeatureInfo::FLAG_REQUIRED;
        fi->SetFlags(ival);
    }
    sa->Recycle();

    *info = fi;
    REFCOUNT_ADD(*info)
    return NOERROR;
}

ECode PackageParser::ParseUsesPermission(
    /* [in] */ PackageParser::Package* pkg,
    /* [in] */ IResources* res,
    /* [in] */ IXmlResourceParser* parser,
    /* [in] */ IAttributeSet* attrs,
    /* [in] */ ArrayOf<String>* outError,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result)
    *result = FALSE;

    Int32 size = ARRAY_SIZE(R::styleable::AndroidManifestUsesPermission);
    AutoPtr<ArrayOf<Int32> > layout = ArrayOf<Int32>::Alloc(size);
    layout->Copy(R::styleable::AndroidManifestUsesPermission, size);

    AutoPtr<ITypedArray> sa;

    res->ObtainAttributes(attrs, layout, (ITypedArray**)&sa);

    // Note: don't allow this value to be a reference to a resource
    // that may change.
    String name;
    sa->GetNonResourceString(
        R::styleable::AndroidManifestUsesPermission_name, &name);
/*
    Boolean required = sa.getBoolean(
            R::styleable::AndroidManifestUsesPermission_required, TRUE);
*/
    Boolean required = TRUE; // Optional <uses-permission> not supported

    Int32 maxSdkVersion = 0;
    AutoPtr<ITypedValue> val;
    sa->PeekValue(
            R::styleable::AndroidManifestUsesPermission_maxSdkVersion, (ITypedValue**)&val);
    if (val != NULL) {
        Int32 type;
        val->GetType(&type);
        if (val.type >= ITypedValue::TYPE_FIRST_INT && val.type <= ITypedValue::TYPE_LAST_INT) {
            val->GetData(&maxSdkVersion);
        }
    }

    sa->Recycle();

    if ((maxSdkVersion == 0) || (maxSdkVersion >= Build::VERSION::RESOURCES_SDK_INT)) {
        if (name != NULL) {
            List<String>::Iterator it = pkg->mRequestedPermissions.Begin();
            List<Boolean>::Iterator bit = pkg->mRequestedPermissionsRequired.Begin();
            Int32 index = -1;
            for (Int32 i = 0; it != pkg->mRequestedPermissions.End(); ++i, ++it, ++bit) {
                if ((*it).Equals(name)) {
                    index = i;
                    break;
                }
            }

            if (index == -1) {
                pkg->mRequestedPermissions.PushBack(name);
                pkg->mRequestedPermissionsRequired.PushBack(required);
            }
            else {
                if (*bit != required) {
                    (*outError)[0] = String("conflicting <uses-permission> entries");
                    mParseError = IPackageManager::INSTALL_PARSE_FAILED_MANIFEST_MALFORMED;
                    return NOERROR;
                }
            }
        }
    }

    assert(0 && "TODO");
    // XmlUtils::SkipCurrentTag(parser);
    *result = TRUE;
    return NOERROR;
}

String PackageParser::BuildClassName(
    /* [in] */ const String& pkg,
    /* [in] */ const String& cls,
    /* [in] */ ArrayOf<String>* outError)
{
    if (cls.IsNullOrEmpty()) {
        (*outError)[0] = String("Empty class name in package ") + pkg;
        return String(NULL);
    }
    Char32 c = cls.GetChar(0);
    if (c == '.') {
        return pkg + cls;
    }
    if (cls.IndexOf('.') < 0) {
        StringBuilder b(pkg);
        b.AppendChar('.');
        b.AppendString(cls);
        return b.ToString();
    }
    if ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z')) {
        return cls;
    }
    (*outError)[0] = String("Bad class name ") + cls + String(" in package ") + pkg;
    return String(NULL);
}

String PackageParser::BuildCompoundName(
    /* [in] */ const String& pkg,
    /* [in] */ ICharSequence* procSeq,
    /* [in] */ const String& type,
    /* [in] */ ArrayOf<String>* outError)
{
    String proc;
    procSeq->ToString(&proc);
    Char32 c = proc.GetChar(0);
    if (!pkg->IsNull() && c == ':') {
        if (proc.GetLength() < 2) {
            // (*outError)[0] = StringBuffer("Bad ") + type + " name " + proc + " in package " + pkg
            //         + ": must be at least two characters";
            return String(NULL);
        }
        String subName = proc.Substring(1);
        String nameError = ValidateName(subName, FALSE);
        if (!nameError.IsNull()) {
            // (*outError)[0] = StringBuffer("Invalid ") + type + " name " + proc + " in package "
            //         + pkg + ": " + nameError;
            return String(NULL);
        }
        return pkg + proc;
    }
    String nameError = ValidateName(proc, TRUE);
    if (!nameError.IsNull() && !proc.Equals("system")) {
        // (*outError)[0] = StringBuffer("Invalid ") + type + " name " + proc + " in package "
        //         + pkg + ": " + nameError;
        return String(NULL);
    }
    return proc;
}

String PackageParser::BuildProcessName(
    /* [in] */ const String& pkg,
    /* [in] */ const String& defProc,
    /* [in] */ ICharSequence* procSeq,
    /* [in] */ Int32 flags,
    /* [in] */ ArrayOf<String>* separateProcesses,
    /* [in] */ ArrayOf<String>* outError)
{
    String sprocSeq;
    if (procSeq != NULL) procSeq->ToString(&sprocSeq);
    if ((flags & PARSE_IGNORE_PROCESSES) != 0 && !sprocSeq.Equals("system")) {
        return !defProc.IsNull() ? defProc : pkg;
    }
    if (separateProcesses != NULL) {
        for (Int32 i = separateProcesses->GetLength() - 1; i >= 0; i--) {
            String sp = (*separateProcesses)[i];
            if (sp->Equals(pkg) || sp->Equals(defProc) || sp->Equals(sprocSeq)) {
                return pkg;
            }
        }
    }
    if (sprocSeq.IsNullOrEmpty()) {
        return defProc;
    }
    return BuildCompoundName(pkg, procSeq, String("process"), outError);
}

String PackageParser::BuildTaskAffinityName(
    /* [in] */ const String& pkg,
    /* [in] */ const String& defProc,
    /* [in] */ ICharSequence* procSeq,
    /* [in] */ ArrayOf<String>* outError)
{
    if (procSeq == NULL) {
        return defProc;
    }
    Int32 len;
    if (procSeq->GetLength(&len), len <= 0) {
        return String(NULL);
    }
    return BuildCompoundName(pkg, procSeq, String("taskAffinity"), outError);
}

ECode PackageParser::ParseKeySets(
    /* [in] */ Package* owner,
    /* [in] */ IResources* res,
    /* [in] */ IXmlResourceParser* parser,
    /* [in] */ IAttributeSet* attrs,
    /* [in] */ ArrayOf<String>* outError,
    /* [out] */ Boolean* result)
{
//     // we've encountered the 'key-sets' tag
//     // all the keys and keysets that we want must be defined here
//     // so we're going to iterate over the parser and pull out the things we want
//     Int32 outerDepth = parser.getDepth();
//     Int32 currentKeySetDepth = -1;
//     Int32 type;
//     String currentKeySet = NULL;
//     ArrayMap<String, PublicKey> publicKeys = new ArrayMap<String, PublicKey>();
//     ArraySet<String> upgradeKeySets = new ArraySet<String>();
//     ArrayMap<String, ArraySet<String>> definedKeySets = new ArrayMap<String, ArraySet<String>>();
//     ArraySet<String> improperKeySets = new ArraySet<String>();
//     while ((type = parser.next()) != IXmlPullParser::END_DOCUMENT
//             && (type != IXmlPullParser::END_TAG || parser.getDepth() > outerDepth)) {
//         if (type == IXmlPullParser::END_TAG) {
//             if (parser.getDepth() == currentKeySetDepth) {
//                 currentKeySet = NULL;
//                 currentKeySetDepth = -1;
//             }
//             continue;
//         }
//         String tagName = parser.getName();
//         if (tagName.Equals("key-set")) {
//             if (currentKeySet != NULL) {
//                 Slog.w(TAG, "Improperly nested 'key-set' tag at "
//                         + parser.getPositionDescription());
//                 return FALSE;
//             }
//             TypedArray sa = res.obtainAttributes(attrs,
//                     R::styleable::AndroidManifestKeySet);
//             String keysetName = sa.getNonResourceString(
//                 R::styleable::AndroidManifestKeySet_name);
//             definedKeySets.put(keysetName, new ArraySet<String>());
//             currentKeySet = keysetName;
//             currentKeySetDepth = parser.getDepth();
//             sa.recycle();
//         } else if (tagName.Equals("public-key")) {
//             if (currentKeySet == NULL) {
//                 Slog.w(TAG, "Improperly nested 'public-key' tag at "
//                         + parser.getPositionDescription());
//                 return FALSE;
//             }
//             TypedArray sa = res.obtainAttributes(attrs,
//                     R::styleable::AndroidManifestPublicKey);
//             String publicKeyName = sa.getNonResourceString(
//                     R::styleable::AndroidManifestPublicKey_name);
//             String encodedKey = sa.getNonResourceString(
//                         R::styleable::AndroidManifestPublicKey_value);
//             if (encodedKey == NULL && publicKeys.get(publicKeyName) == NULL) {
//                 Slog.w(TAG, "'public-key' " + publicKeyName + " must define a public-key value"
//                         + " on first use at " + parser.getPositionDescription());
//                 sa.recycle();
//                 return FALSE;
//             } else if (encodedKey != NULL) {
//                 PublicKey currentKey = parsePublicKey(encodedKey);
//                 if (currentKey == NULL) {
//                     Slog.w(TAG, "No recognized valid key in 'public-key' tag at "
//                             + parser.getPositionDescription() + " key-set " + currentKeySet
//                             + " will not be added to the package's defined key-sets.");
//                     sa.recycle();
//                     improperKeySets.add(currentKeySet);
//                     XmlUtils.skipCurrentTag(parser);
//                     continue;
//                 }
//                 if (publicKeys.get(publicKeyName) == NULL
//                         || publicKeys.get(publicKeyName).Equals(currentKey)) {

//                     /* public-key first definition, or matches old definition */
//                     publicKeys.put(publicKeyName, currentKey);
//                 } else {
//                     Slog.w(TAG, "Value of 'public-key' " + publicKeyName
//                            + " conflicts with previously defined value at "
//                            + parser.getPositionDescription());
//                     sa.recycle();
//                     return FALSE;
//                 }
//             }
//             definedKeySets.get(currentKeySet).add(publicKeyName);
//             sa.recycle();
//             XmlUtils.skipCurrentTag(parser);
//         } else if (tagName.Equals("upgrade-key-set")) {
//             TypedArray sa = res.obtainAttributes(attrs,
//                     R::styleable::AndroidManifestUpgradeKeySet);
//             String name = sa.getNonResourceString(
//                     R::styleable::AndroidManifestUpgradeKeySet_name);
//             upgradeKeySets.add(name);
//             sa.recycle();
//             XmlUtils.skipCurrentTag(parser);
//         } else if (RIGID_PARSER) {
//             Slog.w(TAG, "Bad element under <key-sets>: " + parser.getName()
//                     + " at " + mArchiveSourcePath + " "
//                     + parser.getPositionDescription());
//             return FALSE;
//         } else {
//             Slog.w(TAG, "Unknown element under <key-sets>: " + parser.getName()
//                     + " at " + mArchiveSourcePath + " "
//                     + parser.getPositionDescription());
//             XmlUtils.skipCurrentTag(parser);
//             continue;
//         }
//     }
//     Set<String> publicKeyNames = publicKeys.keySet();
//     if (publicKeyNames.removeAll(definedKeySets.keySet())) {
//         Slog.w(TAG, "Package" + owner.packageName + " AndroidManifext.xml "
//                + "'key-set' and 'public-key' names must be distinct.");
//         return FALSE;
//     }
//     owner.mKeySetMapping = new ArrayMap<String, ArraySet<PublicKey>>();
//     for (ArrayMap.Entry<String, ArraySet<String>> e: definedKeySets.entrySet()) {
//         String keySetName = e.getKey();
//         if (e.getValue().size() == 0) {
//             Slog.w(TAG, "Package" + owner.packageName + " AndroidManifext.xml "
//                     + "'key-set' " + keySetName + " has no valid associated 'public-key'."
//                     + " Not including in package's defined key-sets.");
//             continue;
//         } else if (improperKeySets.contains(keySetName)) {
//             Slog.w(TAG, "Package" + owner.packageName + " AndroidManifext.xml "
//                     + "'key-set' " + keySetName + " contained improper 'public-key'"
//                     + " tags. Not including in package's defined key-sets.");
//             continue;
//         }
//         owner.mKeySetMapping.put(keySetName, new ArraySet<PublicKey>());
//         for (String s : e.getValue()) {
//             owner.mKeySetMapping.get(keySetName).add(publicKeys.get(s));
//         }
//     }
//     if (owner.mKeySetMapping.keySet().containsAll(upgradeKeySets)) {
//         owner.mUpgradeKeySets = upgradeKeySets;
//     } else {
//         Slog.w(TAG, "Package" + owner.packageName + " AndroidManifext.xml "
//                + "does not define all 'upgrade-key-set's .");
//         return FALSE;
//     }
//     return TRUE;
    return NOERROR;
}

AutoPtr<PackageParser::PermissionGroup> PackageParser::ParsePermissionGroup(
    /* [in] */ Package* owner,
    /* [in] */ Int32 flags,
    /* [in] */ IResources* res,
    /* [in] */ IXmlPullParser* parser,
    /* [in] */ IAttributeSet* attrs,
    /* [in] */ ArrayOf<String>* outError)
{
    AutoPtr<PermissionGroup> perm = new PermissionGroup(owner);

    Int32 size = ARRAY_SIZE(R::styleable::AndroidManifestPermissionGroup);
    AutoPtr<ArrayOf<Int32> > layout = ArrayOf<Int32>::Alloc(size);
    layout->Copy(R::styleable::AndroidManifestPermissionGroup, size);

    AutoPtr<ITypedArray> sa;
    res->ObtainAttributes(attrs, layout, (ITypedArray**)&sa);

    if (!ParsePackageItemInfo(owner, perm->mInfo, outError,
            String("<permission-group>"), sa,
            R::styleable::AndroidManifestPermissionGroup_name,
            R::styleable::AndroidManifestPermissionGroup_label,
            R::styleable::AndroidManifestPermissionGroup_icon,
            R::styleable::AndroidManifestPermissionGroup_logo,
            R::styleable::AndroidManifestPermissionGroup_banner)) {
        sa->Recycle();
        mParseError = IPackageManager::INSTALL_PARSE_FAILED_MANIFEST_MALFORMED;
        return NULL;
    }

    Int32 resId;
    sa->GetResourceId(
        R::styleable::AndroidManifestPermissionGroup_description,
        0, &resId);
    perm->mInfo->SetDescriptionRes(resId);
    Int32 pgFlags;
    sa->GetInt32(
        R::styleable::AndroidManifestPermissionGroup_permissionGroupFlags,
        0, &pgFlags);
    perm->mInfo->SetFlags(pgFlags);
    Int32 priority;
    sa->GetInt32(
        R::styleable::AndroidManifestPermissionGroup_priority,
        0, &priority);
    perm->mInfo->SetPriority(priority);
    if (priority > 0 && (flags & PARSE_IS_SYSTEM) == 0) {
        perm->mInfo->SetPriority(0);
    }

    sa->Recycle();

    if (!ParseAllMetaData(res, parser, attrs, String("<permission-group>"), perm.Get(),
            outError)) {
        mParseError = IPackageManager::INSTALL_PARSE_FAILED_MANIFEST_MALFORMED;
        return NULL;
    }

    owner->mPermissionGroups.PushBack(perm);

    return perm;
}

AutoPtr<PackageParser::Permission> PackageParser::ParsePermission(
    /* [in] */ Package* owner,
    /* [in] */ IResources* res,
    /* [in] */ IXmlPullParser* parser,
    /* [in] */ IAttributeSet* attrs,
    /* [in] */ ArrayOf<String>* outError)
{
    AutoPtr<Permission> perm = new Permission(owner);

    Int32 size = ARRAY_SIZE(R::styleable::AndroidManifestPermission);
    AutoPtr<ArrayOf<Int32> > layout = ArrayOf<Int32>::Alloc(size);
    layout->Copy(R::styleable::AndroidManifestPermission, size);

    AutoPtr<ITypedArray> sa;
    res->ObtainAttributes(attrs, layout, (ITypedArray**)&sa);

    if (!ParsePackageItemInfo(owner, perm->mInfo, outError,
            String("<permission>"), sa,
            R::styleable::AndroidManifestPermission_name,
            R::styleable::AndroidManifestPermission_label,
            R::styleable::AndroidManifestPermission_icon,
            R::styleable::AndroidManifestPermission_logo,
            R::styleable::AndroidManifestPermission_banner)) {
        sa->Recycle();
        mParseError = IPackageManager::INSTALL_PARSE_FAILED_MANIFEST_MALFORMED;
        return NULL;
    }

    // Note: don't allow this value to be a reference to a resource
    // that may change.
    String group;
    sa->GetNonResourceString(
        R::styleable::AndroidManifestPermission_permissionGroup,
        &group);
    perm->mInfo->SetGroup(group);

    Int32 desRes;
    sa->GetResourceId(
        R::styleable::AndroidManifestPermission_description,
        0, &desRes);
    perm->mInfo->SetDescriptionRes(desRes);

    Int32 level;,
    ,
    sa->GetInt32(
        R::styleable::AndroidManifestPermission_protectionLevel,
        IPermissionInfo::PROTECTION_NORMAL, &level);
    perm->mInfo->SetProtectionLevel(level);

    Int32 flags;
    sa->GetInt32(
        R::styleable::AndroidManifestPermission_permissionFlags,
        0, &flags);
    perm->mInfo->SetFlags(flags);

    sa->Recycle();

    if (level == -1) {
        (*outError)[0] = "<permission> does not specify protectionLevel";
        mParseError = IPackageManager::INSTALL_PARSE_FAILED_MANIFEST_MALFORMED;
        return NULL;
    }

    Int32 pLevel;
    perm->mInfo->GetProtectionLevel(&pLevel);
    AutoPtr<IPermissionInfoHelper> helper;
    CPermissionInfoHelper::AcquireSingleton((IPermissionInfoHelper**)&helper);
    helper->FixProtectionLevel(pLevel, &pLevel);
    perm->mInfo->SetProtectionLevel(pLevel);

    if ((pLevel & IPermissionInfo::PROTECTION_MASK_FLAGS) != 0) {
        if ((pLevel & IPermissionInfo::PROTECTION_MASK_BASE) !=
                IPermissionInfo::PROTECTION_SIGNATURE) {
            (*outError)[0] = "<permission>  protectionLevel specifies a flag but is not based on signature type";
            mParseError = IPackageManager::INSTALL_PARSE_FAILED_MANIFEST_MALFORMED;
            return NULL;
        }
    }

    if (!ParseAllMetaData(res, parser, attrs, String("<permission>"), perm.Get(),
            outError)) {
        mParseError = IPackageManager::INSTALL_PARSE_FAILED_MANIFEST_MALFORMED;
        return NULL;
    }

    owner->mPermissions.PushBack(perm);

    return perm;
}

AutoPtr<PackageParser::Permission> PackageParser::ParsePermissionTree(
    /* [in] */ Package* owner,
    /* [in] */ IResources* res,
    /* [in] */ IXmlPullParser* parser,
    /* [in] */ IAttributeSet* attrs,
    /* [in] */ ArrayOf<String>* outError)
{
    AutoPtr<Permission> perm = new Permission(owner);

    Int32 size = ARRAY_SIZE(R::styleable::AndroidManifestPermissionTree);
    AutoPtr<ArrayOf<Int32> > layout = ArrayOf<Int32>::Alloc(size);
    layout->Copy(R::styleable::AndroidManifestPermissionTree, size);

    AutoPtr<ITypedArray> sa;
    res->ObtainAttributes(attrs, layout, (ITypedArray**)&sa);

    if (!ParsePackageItemInfo(owner, perm->mInfo, outError,
            String("<permission-tree>"), sa,
            R::styleable::AndroidManifestPermissionTree_name,
            R::styleable::AndroidManifestPermissionTree_label,
            R::styleable::AndroidManifestPermissionTree_icon,
            R::styleable::AndroidManifestPermissionTree_logo,
            R::styleable::AndroidManifestPermissionTree_banner)) {
        sa->Recycle();
        mParseError = IPackageManager::INSTALL_PARSE_FAILED_MANIFEST_MALFORMED;
        return NULL;
    }

    sa->Recycle();

    String permName;
    perm->mInfo->GetName(&permName);
    Int32 index = permName.IndexOf('.');
    if (index > 0) {
        index = permName.IndexOf('.', index + 1);
    }
    if (index < 0) {
        // (*outError)[0] = (const char*)(
        //     StringBuffer("<permission-tree> name has less than three segments: ")
        //     + permName);
        mParseError = IPackageManager::INSTALL_PARSE_FAILED_MANIFEST_MALFORMED;
        return NULL;
    }

    perm->mInfo->SetDescriptionRes(0);
    perm->mInfo->SetProtectionLevel(IPermissionInfo::PROTECTION_NORMAL);
    perm->mTree = TRUE;

    if (!ParseAllMetaData(res, parser, attrs, String("<permission-tree>"), perm.Get(),
            outError)) {
        mParseError = IPackageManager::INSTALL_PARSE_FAILED_MANIFEST_MALFORMED;
        return NULL;
    }

    owner->mPermissions.PushBack(perm);

    return perm;
}

AutoPtr<PackageParser::Instrumentation> PackageParser::ParseInstrumentation(
    /* [in] */ Package* owner,
    /* [in] */ IResources* res,
    /* [in] */ IXmlPullParser* parser,
    /* [in] */ IAttributeSet* attrs,
    /* [in] */ ArrayOf<String>* outError)
{
    Int32 size = ARRAY_SIZE(R::styleable::AndroidManifestInstrumentation);
    AutoPtr<ArrayOf<Int32> > layout = ArrayOf<Int32>::Alloc(size);
    layout->Copy(R::styleable::AndroidManifestInstrumentation, size);

    AutoPtr<ITypedArray> sa;
    res->ObtainAttributes(attrs, layout, (ITypedArray**)&sa);

    if (mParseInstrumentationArgs == NULL) {
        mParseInstrumentationArgs = new ParsePackageItemArgs(owner, outError,
                R::styleable::AndroidManifestInstrumentation_name,
                R::styleable::AndroidManifestInstrumentation_label,
                R::styleable::AndroidManifestInstrumentation_icon,
                R::styleable::AndroidManifestInstrumentation_logo,
                R::styleable::AndroidManifestInstrumentation_banner);
        mParseInstrumentationArgs->mTag = "<instrumentation>";
    }

    mParseInstrumentationArgs->mSa = sa;

    AutoPtr<IInstrumentationInfo> info;
    FAIL_RETURN(CInstrumentationInfo::New((IInstrumentationInfo**)&info));
    AutoPtr<Instrumentation> a = new Instrumentation(mParseInstrumentationArgs, info);
    if (!(*outError)[0].IsNull()) {
        sa->Recycle();
        mParseError = IPackageManager::INSTALL_PARSE_FAILED_MANIFEST_MALFORMED;
        return NULL;
    }

    // Note: don't allow this value to be a reference to a resource
    // that may change.
    String targetPackage;
    sa->GetNonResourceString(
        R::styleable::AndroidManifestInstrumentation_targetPackage,
        &targetPackage);
    a->mInfo->SetTargetPackage(targetPackage);

    Boolean bvalue;
    sa->GetBoolean(
        R::styleable::AndroidManifestInstrumentation_handleProfiling,
        FALSE, &bvalue);
    a->mInfo->SetHandleProfiling(bvalue);

    sa->GetBoolean(
        R::styleable::AndroidManifestInstrumentation_functionalTest,
        FALSE, &bvalue);
    a->mInfo->SetFunctionalTest(bvalue);

    sa->Recycle();

    if (targetPackage.IsNull()) {
        (*outError)[0] = "<instrumentation> does not specify targetPackage";
        mParseError = IPackageManager::INSTALL_PARSE_FAILED_MANIFEST_MALFORMED;
        return NULL;
    }

    if (!ParseAllMetaData(res, parser, attrs, String("<instrumentation>"), a.Get(),
            outError)) {
        mParseError = IPackageManager::INSTALL_PARSE_FAILED_MANIFEST_MALFORMED;
        return NULL;
    }

    owner->mInstrumentation.PushBack(a);

    return a;
}

Boolean PackageParser::ParseBaseApplication(
    /* [in] */ Package* owner,
    /* [in] */ IResources* res,
    /* [in] */ IXmlPullParser* parser,
    /* [in] */ IAttributeSet* attrs,
    /* [in] */ Int32 flags,
    /* [in] */ ArrayOf<String>* outError)
{
    AutoPtr<IApplicationInfo> ai = owner->mApplicationInfo;
    String pkgName;
    ai->GetPackageName(&pkgName);

    Int32 size = ARRAY_SIZE(R::styleable::AndroidManifestApplication);
    AutoPtr<ArrayOf<Int32> > layout = ArrayOf<Int32>::Alloc(size);
    layout->Copy(R::styleable::AndroidManifestApplication, size);

    AutoPtr<ITypedArray> sa;
    FAIL_RETURN(res->ObtainAttributes(attrs, layout, (ITypedArray**)&sa));

    String name;
    sa->GetNonConfigurationString(
        R::styleable::AndroidManifestApplication_name,
        IConfiguration::NATIVE_CONFIG_VERSION, &name);
    if (!name.IsNull()) {
        String clsName = BuildClassName(pkgName, name, outError);
        ai->SetClassName(clsName);
        if (clsName.IsNull()) {
            sa->Recycle();
            mParseError = IPackageManager::INSTALL_PARSE_FAILED_MANIFEST_MALFORMED;
            return FALSE;
        }
    }

    String manageSpaceActivity;
    sa->GetNonConfigurationString(
        R::styleable::AndroidManifestApplication_manageSpaceActivity,
        IConfiguration::NATIVE_CONFIG_VERSION,
        &manageSpaceActivity);
    if (!manageSpaceActivity.IsNull()) {
        String name = BuildClassName(pkgName, manageSpaceActivity, outError);
        ai->SetManageSpaceActivityName(name);
    }

    Int32 aiFlags;
    Boolean allowBackup = FALSE;
    sa->GetBoolean(
        R::styleable::AndroidManifestApplication_allowBackup, TRUE,
        &allowBackup);
    if (allowBackup) {
        ai->GetFlags(&aiFlags);
        aiFlags |= IApplicationInfo::FLAG_ALLOW_BACKUP;
        ai->SetFlags(aiFlags);

        // backupAgent, killAfterRestore, and restoreAnyVersion are only relevant
        // if backup is possible for the given application.
        String backupAgent;
        sa->GetNonConfigurationString(
            R::styleable::AndroidManifestApplication_backupAgent, 0,
            &backupAgent);
        if (!backupAgent.IsNull()) {
            String agentName = BuildClassName(pkgName, backupAgent, outError);
            ai->SetBackupAgentName(agentName);
            if (DEBUG_BACKUP) {
                // SLogger::V(TAG, StringBuffer("android:backupAgent = ") + agentName
                //         + " from " + pkgName + "+" + backupAgent);
            }

            Boolean value = FALSE;
            sa->GetBoolean(
                R::styleable::AndroidManifestApplication_killAfterRestore,
                TRUE, &value);
            if (value) {
                ai->GetFlags(&aiFlags);
                aiFlags |= IApplicationInfo::FLAG_KILL_AFTER_RESTORE;
                ai->SetFlags(aiFlags);
            }
            sa->GetBoolean(
                R::styleable::AndroidManifestApplication_restoreAnyVersion,
                FALSE, &value);
            if (value) {
                ai->GetFlags(&aiFlags);
                aiFlags |= IApplicationInfo::FLAG_RESTORE_ANY_VERSION;
                ai->SetFlags(aiFlags);
            }

            sa->GetBoolean(
                R::styleable::AndroidManifestApplication_fullBackupOnly,
                FALSE, &value);
            if (value) {
                ai->GetFlags(&aiFlags);
                aiFlags |= IApplicationInfo::FLAG_FULL_BACKUP_ONLY;
                ai->SetFlags(aiFlags);
            }
        }
    }

    AutoPtr<ITypedValue> v;
    ASSERT_SUCCEEDED(sa->PeekValue(
            R::styleable::AndroidManifestApplication_label, (ITypedValue**)&v));
    Int32 ivalue;
    if (v != NULL && (v->GetResourceId(&ivalue), ai->SetLabelRes(ivalue), ivalue) == 0) {
        AutoPtr<ICharSequence> label;
        v->CoerceToString((ICharSequence**)&label);
        ai->SetNonLocalizedLabel(label);
    }

    sa->GetResourceId(
        R::styleable::AndroidManifestApplication_icon, 0, &ivalue);
    ai->SetIcon(ivalue);
    sa->GetResourceId(
        R::styleable::AndroidManifestApplication_logo, 0, &ivalue);
    ai->SetLogo(ivalue);
    sa->GetResourceId(
        R::styleable::AndroidManifestApplication_banner, 0, &ivalue);
    ai->SetBanner(ivalue);
    sa->GetResourceId(
        R::styleable::AndroidManifestApplication_theme, 0, &ivalue);
    ai->SetTheme(ivalue);
    sa->GetResourceId(
        R::styleable::AndroidManifestApplication_description, 0, &ivalue);
    ai->SetDescriptionRes(ivalue);

    if ((flags & PARSE_IS_SYSTEM) != 0) {
        Boolean value = FALSE;
        sa->GetBoolean(
            R::styleable::AndroidManifestApplication_persistent,
            FALSE, &value);
        if (value) {
            ai->GetFlags(&aiFlags);
            aiFlags |= IApplicationInfo::FLAG_PERSISTENT;
            ai->SetFlags(aiFlags);
        }
    }

    Boolean bvalue = FALSE;

    // if (sa.getBoolean(
    //         R::styleable::AndroidManifestApplication_requiredForAllUsers,
    //         FALSE)) {
    //     owner.mRequiredForAllUsers = TRUE;
    // }

    // String restrictedAccountType = sa.getString(com.android.internal.R.styleable
    //         .AndroidManifestApplication_restrictedAccountType);
    // if (restrictedAccountType != NULL && restrictedAccountType.length() > 0) {
    //     owner.mRestrictedAccountType = restrictedAccountType;
    // }

    // String requiredAccountType = sa.getString(com.android.internal.R.styleable
    //         .AndroidManifestApplication_requiredAccountType);
    // if (requiredAccountType != NULL && requiredAccountType.length() > 0) {
    //     owner.mRequiredAccountType = requiredAccountType;
    // }

    sa->GetBoolean(
        R::styleable::AndroidManifestApplication_debuggable,
        FALSE, &bvalue);
    if (bvalue) {
        ai->GetFlags(&aiFlags);
        aiFlags |= IApplicationInfo::FLAG_DEBUGGABLE;
        ai->SetFlags(aiFlags);
    }

    sa->GetBoolean(
        R::styleable::AndroidManifestApplication_vmSafeMode,
        FALSE, &bvalue);
    if (bvalue) {
        ai->GetFlags(&aiFlags);
        aiFlags |= IApplicationInfo::FLAG_VM_SAFE_MODE;
        ai->SetFlags(aiFlags);
    }

    Int32 targetVersion;
    owner->mApplicationInfo->GetTargetSdkVersion(&targetVersion);
    Boolean hardwareAccelerated;
    sa->GetBoolean(
        R::styleable::AndroidManifestApplication_hardwareAccelerated,
        targetVersion >= Build::VERSION_CODES::ICE_CREAM_SANDWICH,
        &hardwareAccelerated);
    owner->mBaseHardwareAccelerated = hardwareAccelerated;

    sa->GetBoolean(
        R::styleable::AndroidManifestApplication_hasCode,
        TRUE, &bvalue);
    if (bvalue) {
        ai->GetFlags(&aiFlags);
        aiFlags |= IApplicationInfo::FLAG_HAS_CODE;
        ai->SetFlags(aiFlags);
    }

    sa->GetBoolean(
        R::styleable::AndroidManifestApplication_allowTaskReparenting,
        FALSE, &bvalue);
    if (bvalue) {
        ai->GetFlags(&aiFlags);
        aiFlags |= IApplicationInfo::FLAG_ALLOW_TASK_REPARENTING;
        ai->SetFlags(aiFlags);
    }

    sa->GetBoolean(
        R::styleable::AndroidManifestApplication_allowClearUserData,
        TRUE, &bvalue);
    if (bvalue) {
        ai->GetFlags(&aiFlags);
        aiFlags |= IApplicationInfo::FLAG_ALLOW_CLEAR_USER_DATA;
        ai->SetFlags(aiFlags);
    }

    sa->GetBoolean(
        R::styleable::AndroidManifestApplication_testOnly,
        FALSE, &bvalue);
    if (bvalue) {
        ai->GetFlags(&aiFlags);
        aiFlags |= IApplicationInfo::FLAG_TEST_ONLY;
        ai->SetFlags(aiFlags);
    }

    sa->GetBoolean(
        R::styleable::AndroidManifestApplication_largeHeap,
        FALSE, &bvalue);
    if (bvalue) {
        ai->GetFlags(&aiFlags);
        aiFlags |= IApplicationInfo::FLAG_LARGE_HEAP;
        ai->SetFlags(aiFlags);
    }

    sa->GetBoolean(
        R::styleable::AndroidManifestApplication_supportsRtl,
        FALSE /* default is no RTL support*/, &bvalue);
    if (bvalue) {
        ai->GetFlags(&aiFlags);
        aiFlags |=IApplicationInfo::FLAG_SUPPORTS_RTL;
        ai->SetFlags(aiFlags);
    }

    sa->GetBoolean(
        R::styleable::AndroidManifestApplication_multiArch,
        FALSE, &bvalue);
    if (bvalue) {
        ai->GetFlags(&aiFlags);
        aiFlags |=IApplicationInfo::FLAG_MULTIARCH;
        ai->SetFlags(aiFlags);
    }

    String str;
    sa->GetNonConfigurationString(
        R::styleable::AndroidManifestApplication_permission,
        IConfiguration::NATIVE_CONFIG_VERSION, &str);
    ai->SetPermission((!str.IsNullOrEmpty()) ? str : String(NULL));

    //todo:
    Int32 ownerSdkVersion = Build::VERSION_CODES::JELLY_BEAN_MR1;
//    owner->mApplicationInfo->GetTargetSdkVersion(&ownerSdkVersion);
    if (ownerSdkVersion >= Build::VERSION_CODES::FROYO) {
        sa->GetNonConfigurationString(
            R::styleable::AndroidManifestApplication_taskAffinity, 0, &str);
    }
    else {
        // Some older apps have been seen to use a resource reference
        // here that on older builds was ignored (with a warning).  We
        // need to continue to do this for them so they don't break.
        sa->GetNonResourceString(
            R::styleable::AndroidManifestApplication_taskAffinity, &str);
    }
    AutoPtr<ICharSequence> cStr;
    CString::New(str, (ICharSequence**)&cStr);
    String aiTask = BuildTaskAffinityName(pkgName, pkgName, cStr, outError);
    ai->SetTaskAffinity(aiTask);

    if ((*outError)[0].IsNull()) {
        String pname;
        if (ownerSdkVersion >= Build::VERSION_CODES::FROYO) {
            sa->GetNonConfigurationString(
                R::styleable::AndroidManifestApplication_process,
                IConfiguration::NATIVE_CONFIG_VERSION, &pname);
        }
        else {
            // Some older apps have been seen to use a resource reference
            // here that on older builds was ignored (with a warning).  We
            // need to continue to do this for them so they don't break.
            sa->GetNonResourceString(
                R::styleable::AndroidManifestApplication_process,
                &pname);
        }
        AutoPtr<ICharSequence> cpname;
        CString::New(pname, (ICharSequence**)&cpname);
        String aiPName = BuildProcessName(pkgName, pkgName/*String(NULL)*/, cpname,
                flags, mSeparateProcesses, outError);
        ai->SetProcessName(aiPName);

        Boolean aiEnabled;
        sa->GetBoolean(
            R::styleable::AndroidManifestApplication_enabled, TRUE,
            &aiEnabled);
        ai->SetEnabled(aiEnabled);

        Boolean value = FALSE;
        sa->GetBoolean(
            R::styleable::AndroidManifestApplication_isGame,
            FALSE, &value);
        if (value) {
            ai->GetFlags(&aiFlags);
            aiFlags |= IApplicationInfo::FLAG_IS_GAME;
            ai->SetFlags(aiFlags);
        }

        if (FALSE) {
            sa->GetBoolean(
                R::styleable::AndroidManifestApplication_cantSaveState,
                FALSE, &value);
            if (value) {
                ai->GetFlags(&aiFlags);
                aiFlags |= IApplicationInfo::FLAG_CANT_SAVE_STATE;
                ai->SetFlags(aiFlags);

                // A heavy-weight application can not be in a custom process.
                // We can do direct compare because we intern all strings.
                if (!aiPName.IsNull() && !aiPName.Equals(pkgName)) {
                    (*outError)[0] = "cantSaveState applications can not use custom processes";
                }
            }
        }
    }

    Int32 uiOptions;
    sa->GetInt32(
        R::styleable::AndroidManifestApplication_uiOptions,
        0, &uiOptions);
    ai->SetUiOptions(uiOptions);

    sa->Recycle();

    if (!(*outError)[0].IsNull()) {
        mParseError = IPackageManager::INSTALL_PARSE_FAILED_MANIFEST_MALFORMED;
        return FALSE;
    }

    Int32 innerDepth = 0;
    parser->GetDepth(&innerDepth);

    Int32 type;
    while ((type = parser.next()) != IXmlPullParser::END_DOCUMENT
            && (type != IXmlPullParser::END_TAG || parser.getDepth() > innerDepth)) {
        if (type == IXmlPullParser::END_TAG || type == IXmlPullParser::TEXT) {
            continue;
        }

        String tagName = parser.getName();
        if (tagName.Equals("activity")) {
            Activity a = parseActivity(owner, res, parser, attrs, flags, outError, FALSE,
                    owner.baseHardwareAccelerated);
            if (a == NULL) {
                mParseError = IPackageManager::INSTALL_PARSE_FAILED_MANIFEST_MALFORMED;
                return FALSE;
            }

            owner.activities.add(a);

        } else if (tagName.Equals("receiver")) {
            Activity a = parseActivity(owner, res, parser, attrs, flags, outError, TRUE, FALSE);
            if (a == NULL) {
                mParseError = IPackageManager::INSTALL_PARSE_FAILED_MANIFEST_MALFORMED;
                return FALSE;
            }

            owner.receivers.add(a);

        } else if (tagName.Equals("service")) {
            Service s = parseService(owner, res, parser, attrs, flags, outError);
            if (s == NULL) {
                mParseError = IPackageManager::INSTALL_PARSE_FAILED_MANIFEST_MALFORMED;
                return FALSE;
            }

            owner.services.add(s);

        } else if (tagName.Equals("provider")) {
            Provider p = parseProvider(owner, res, parser, attrs, flags, outError);
            if (p == NULL) {
                mParseError = IPackageManager::INSTALL_PARSE_FAILED_MANIFEST_MALFORMED;
                return FALSE;
            }

            owner.providers.add(p);

        } else if (tagName.Equals("activity-alias")) {
            Activity a = parseActivityAlias(owner, res, parser, attrs, flags, outError);
            if (a == NULL) {
                mParseError = IPackageManager::INSTALL_PARSE_FAILED_MANIFEST_MALFORMED;
                return FALSE;
            }

            owner.activities.add(a);

        } else if (parser.getName().Equals("meta-data")) {
            // note: application meta-data is stored off to the side, so it can
            // remain NULL in the primary copy (we like to avoid extra copies because
            // it can be large)
            if ((owner.mAppMetaData = parseMetaData(res, parser, attrs, owner.mAppMetaData,
                    outError)) == NULL) {
                mParseError = IPackageManager::INSTALL_PARSE_FAILED_MANIFEST_MALFORMED;
                return FALSE;
            }

        } else if (tagName.Equals("library")) {
            sa = res.obtainAttributes(attrs,
                    R::styleable::AndroidManifestLibrary);

            // Note: don't allow this value to be a reference to a resource
            // that may change.
            String lname = sa.getNonResourceString(
                    R::styleable::AndroidManifestLibrary_name);

            sa.recycle();

            if (lname != NULL) {
                lname = lname.intern();
                if (!ArrayUtils.contains(owner.libraryNames, lname)) {
                    owner.libraryNames = ArrayUtils.add(owner.libraryNames, lname);
                }
            }

            XmlUtils.skipCurrentTag(parser);

        } else if (tagName.Equals("uses-library")) {
            sa = res.obtainAttributes(attrs,
                    R::styleable::AndroidManifestUsesLibrary);

            // Note: don't allow this value to be a reference to a resource
            // that may change.
            String lname = sa.getNonResourceString(
                    R::styleable::AndroidManifestUsesLibrary_name);
            Boolean req = sa.getBoolean(
                    R::styleable::AndroidManifestUsesLibrary_required,
                    TRUE);

            sa.recycle();

            if (lname != NULL) {
                lname = lname.intern();
                if (req) {
                    owner.usesLibraries = ArrayUtils.add(owner.usesLibraries, lname);
                } else {
                    owner.usesOptionalLibraries = ArrayUtils.add(
                            owner.usesOptionalLibraries, lname);
                }
            }

            XmlUtils.skipCurrentTag(parser);

        } else if (tagName.Equals("uses-package")) {
            // Dependencies for app installers; we don't currently try to
            // enforce this.
            XmlUtils.skipCurrentTag(parser);

        } else {
            if (!RIGID_PARSER) {
                Slog.w(TAG, "Unknown element under <application>: " + tagName
                        + " at " + mArchiveSourcePath + " "
                        + parser.getPositionDescription());
                XmlUtils.skipCurrentTag(parser);
                continue;
            } else {
                (*outError)[0] = "Bad element under <application>: " + tagName;
                mParseError = IPackageManager::INSTALL_PARSE_FAILED_MANIFEST_MALFORMED;
                return FALSE;
            }
        }
    }

    return TRUE;
}

ECode PackageParser::ParseSplitApplication(
    /* [in] */ PackageParser::Package* owner,
    /* [in] */ IResources* res,
    /* [in] */ IXmlPullParser* parser,
    /* [in] */ IAttributeSet* attrs,
    /* [in] */ Int32 flags,
    /* [in] */ Int32 splitIndex,
    /* [out] */ ArrayOf<String>* outError,
    /* [out] */ Boolean* result)
{
    TypedArray sa = res.obtainAttributes(attrs,
            R::styleable::AndroidManifestApplication);

    if (sa.getBoolean(
            R::styleable::AndroidManifestApplication_hasCode, TRUE)) {
        owner.splitFlags[splitIndex] |= ApplicationInfo.FLAG_HAS_CODE;
    }

    Int32 innerDepth = parser.getDepth();

    Int32 type = 0;
    Int32 depth = 0;
    while ((parser->Next(&type), type) != IXmlPullParser::END_DOCUMENT
           && (type != IXmlPullParser::END_TAG
           || (parser->GetDepth(&depth), depth) > innerDepth)) {
        if (type == IXmlPullParser::END_TAG || type == IXmlPullParser::TEXT) {
            continue;
        }

        String tagName, tmpName;
        parser->GetName(&tagName);
        if (tagName.Equals("activity")) {
            AutoPtr<Activity> a = ParseActivity(owner, res, parser, attrs,
                    flags, outError, FALSE, owner->mBaseHardwareAccelerated);
            if (a == NULL) {
                mParseError = IPackageManager::INSTALL_PARSE_FAILED_MANIFEST_MALFORMED;
                return FALSE;
            }

            owner->mActivities.PushBack(a);

        }
        else if (tagName.Equals("receiver")) {
            AutoPtr<Activity> a = ParseActivity(owner, res, parser, attrs,
                    flags, outError, TRUE, FALSE);
            if (a == NULL) {
                mParseError = IPackageManager::INSTALL_PARSE_FAILED_MANIFEST_MALFORMED;
                return FALSE;
            }

            owner->mReceivers.PushBack(a);

        }
        else if (tagName.Equals("service")) {
            AutoPtr<Service> s = ParseService(owner, res, parser, attrs, flags, outError);
            if (s == NULL) {
                mParseError = IPackageManager::INSTALL_PARSE_FAILED_MANIFEST_MALFORMED;
                return FALSE;
            }

            owner->mServices.PushBack(s);

        }
        else if (tagName.Equals("provider")) {
            AutoPtr<Provider> p = ParseProvider(owner, res, parser, attrs, flags, outError);
            if (p == NULL) {
                mParseError = IPackageManager::INSTALL_PARSE_FAILED_MANIFEST_MALFORMED;
                return FALSE;
            }

            owner->mProviders.PushBack(p);

        }
        else if (tagName.Equals("activity-alias")) {
            AutoPtr<Activity> a = ParseActivityAlias(owner, res, parser, attrs, flags, outError);
            if (a == NULL) {
                mParseError = IPackageManager::INSTALL_PARSE_FAILED_MANIFEST_MALFORMED;
                return FALSE;
            }

            owner->mActivities.PushBack(a);

        }
        else if ((parser->GetName(&tmpName), tmpName).Equals("meta-data")) {
            // note: application meta-data is stored off to the side, so it can
            // remain NULL in the primary copy (we like to avoid extra copies because
            // it can be large)
            if ((owner->mAppMetaData = ParseMetaData(res, parser, attrs, owner->mAppMetaData,
                    outError)) == NULL) {
                mParseError = IPackageManager::INSTALL_PARSE_FAILED_MANIFEST_MALFORMED;
                return FALSE;
            }

        }
        else if (tagName.Equals("uses-library")) {
            sa = NULL;
            size = ARRAY_SIZE(R::styleable::AndroidManifestUsesLibrary);
            layout = ArrayOf<Int32>::Alloc(size);
            layout->Copy(R::styleable::AndroidManifestUsesLibrary, size);

            res->ObtainAttributes(attrs, layout, (ITypedArray**)&sa);

            // Note: don't allow this value to be a reference to a resource
            // that may change
            String lname;
            sa->GetNonResourceString(
                R::styleable::AndroidManifestUsesLibrary_name, &lname);
            Boolean req = FALSE;
            sa->GetBoolean(
                R::styleable::AndroidManifestUsesLibrary_required,
                TRUE, &req);

            sa->Recycle();

            if (!lname.IsNull()) {
                if (req) {
                    // Upgrade to treat as stronger constraint
                    owner.usesLibraries = ArrayUtils.add(owner.usesLibraries, lname);
                    owner.usesOptionalLibraries = ArrayUtils.remove(
                            owner.usesOptionalLibraries, lname);
                } else {
                    // Ignore if someone already defined as required
                    if (!ArrayUtils.contains(owner.usesLibraries, lname)) {
                        owner.usesOptionalLibraries = ArrayUtils.add(
                                owner.usesOptionalLibraries, lname);
                    }
                }
                //============================
                // if (req) {
                //     if (owner->mUsesLibraries == NULL) {
                //         owner->mUsesLibraries = new List<String>();
                //     }
                //     if (Find(owner->mUsesLibraries->Begin(), owner->mUsesLibraries->End(), lname)
                //         == owner->mUsesLibraries->End()) {
                //         owner->mUsesLibraries->PushBack(lname);
                //     }
                // }
                // else {
                //     if (owner->mUsesOptionalLibraries == NULL) {
                //         owner->mUsesOptionalLibraries = new List<String>();
                //     }
                //     if (Find(owner->mUsesOptionalLibraries->Begin(),
                //             owner->mUsesOptionalLibraries->End(), lname)
                //         == owner->mUsesOptionalLibraries->End()) {
                //         owner->mUsesOptionalLibraries->PushBack(lname);
                //     }
                // }
            }

            assert(0 && "TODO");
            //XmlUtils::SkipCurrentTag(parser);

        }
        else if (tagName.Equals("uses-package")) {
            // Dependencies for app installers; we don't currently try to
            // enforce m
            assert(0 && "TODO");
            //XmlUtils::SkipCurrentTag(parser);

        }
        else {
            if (!RIGID_PARSER) {
                String des;
                parser->GetPositionDescription(&des);
                // SLogger::W(TAG, StringBuffer("Unknown element under <application>: ") + tagName
                //         + " at " + mArchiveSourcePath + " " + des);
                assert(0 && "TODO");
            //XmlUtils::SkipCurrentTag(parser);
                continue;
            }
            else {
                // (*outError)[0] = (const char*)(StringBuffer("Bad element under <application>: ") + tagName);
                mParseError = IPackageManager::INSTALL_PARSE_FAILED_MANIFEST_MALFORMED;
                return FALSE;
            }
        }
    }

    return TRUE;
}

Boolean PackageParser::ParsePackageItemInfo(
    /* [in] */ Package* owner,
    /* [in] */ IPackageItemInfo* outInfo,
    /* [in] */ ArrayOf<String>* outError,
    /* [in] */ const String& tag,
    /* [in] */ ITypedArray* sa,
    /* [in] */ Int32 nameRes,
    /* [in] */ Int32 labelRes,
    /* [in] */ Int32 iconRes,
    /* [in] */ Int32 logoRes,
    /* [in] */ Int32 bannerRes)
{
    String name;
    sa->GetNonConfigurationString(nameRes, 0, &name);
    if (name.IsNull()) {
        // (*outError)[0] = (const char*)(
        //     StringBuffer(tag) + " does not specify android:name");
        return FALSE;
    }

    String ownerPName;
    owner->mApplicationInfo->GetPackageName(&ownerPName);
    name = BuildClassName(ownerPName, name, outError);
    outInfo->SetName(name);
    if (name.IsNull()) {
        return FALSE;
    }

    Int32 ival;
    sa->GetResourceId(iconRes, 0, &ival);
    if (ival != 0) {
        outInfo->SetIcon(ival);
        outInfo->SetNonLocalizedLabel(NULL);
    }

    sa->GetResourceId(logoRes, 0, &ival);
    if (ival != 0) {
        outInfo->SetLogo(ival);
    }

    sa->GetResourceId(bannerRes, 0, &ival);
    if (ival != 0) {
        outInfo->SetBanner(ival);
    }

    AutoPtr<ITypedValue> v;
    sa->PeekValue(labelRes, (ITypedValue**)&v);
    Int32 vResId;
    if (v != NULL && (v->GetResourceId(&vResId), outInfo->SetLabelRes(vResId), vResId) == 0) {
        AutoPtr<ICharSequence> label;
        v->CoerceToString((ICharSequence**)&label);
        outInfo->SetNonLocalizedLabel(label);
    }

    outInfo->SetPackageName(owner->mPackageName);

    return TRUE;
}

AutoPtr<PackageParser::Activity> PackageParser::ParseActivity(
    /* [in] */ PackageParser::Package* owner,
    /* [in] */ IResources* res,
    /* [in] */ IXmlPullParser* parser,
    /* [in] */ IAttributeSet* attrs,
    /* [in] */ Int32 flags,
    /* [in] */ ArrayOf<String>* outError,
    /* [in] */ Boolean receiver,
    /* [in] */ Boolean hardwareAccelerated)
{
    assert(res);
    assert(attrs);

    Int32 size = ARRAY_SIZE(R::styleable::AndroidManifestActivity);
    AutoPtr<ArrayOf<Int32> > layout = ArrayOf<Int32>::Alloc(size);
    layout->Copy(R::styleable::AndroidManifestActivity, size);

    AutoPtr<ITypedArray> sa;
    FAIL_RETURN(res->ObtainAttributes(attrs, layout, (ITypedArray**)&sa));

    if (mParseActivityArgs == NULL) {
        mParseActivityArgs = new ParseComponentArgs(owner, outError,
            R::styleable::AndroidManifestActivity_name,
            R::styleable::AndroidManifestActivity_label,
            R::styleable::AndroidManifestActivity_icon,
            R::styleable::AndroidManifestActivity_logo,
            R::styleable::AndroidManifestActivity_banner,
            mSeparateProcesses,
            R::styleable::AndroidManifestActivity_process,
            R::styleable::AndroidManifestActivity_description,
            R::styleable::AndroidManifestActivity_enabled);
    }

    mParseActivityArgs->mTag = receiver ? "<receiver>" : "<activity>";
    mParseActivityArgs->mSa = sa;
    mParseActivityArgs->mFlags = flags;

    AutoPtr<IActivityInfo> info;
    FAIL_RETURN(CActivityInfo::New((IActivityInfo**)&info));
    AutoPtr<Activity> a = new Activity(mParseActivityArgs, info);
    if (!(*outError)[0].IsNull()) {
        sa->Recycle();
        return NULL;
    }

    Boolean setExported = FALSE;
    sa->HasValue(R::styleable::AndroidManifestActivity_exported,
        &setExported);
    if (setExported) {
        Boolean isExported;
        sa->GetBoolean(R::styleable::AndroidManifestActivity_exported,
            FALSE, &isExported);
        a->mInfo->SetExported(isExported);
    }

    Int32 ivalue;
    sa->GetResourceId(
        R::styleable::AndroidManifestActivity_theme, 0, &ivalue);
    a->mInfo->SetTheme(ivalue);

    AutoPtr<IApplicationInfo> appInfo;
    a->mInfo->GetApplicationInfo((IApplicationInfo**)&appInfo);
    Int32 uiOptions;
    appInfo->GetUiOptions(&uiOptions);
    sa->GetInt32(
        R::styleable::AndroidManifestActivity_uiOptions, uiOptions, &ivalue);
    a->mInfo->SetUiOptions(ivalue);

    String parentName;
    sa->GetNonConfigurationString(
        R::styleable::AndroidManifestActivity_parentActivityName,
        IConfiguration::NATIVE_CONFIG_VERSION, &parentName);
    if (!parentName.IsNull()) {
        String packageName;
        a->mInfo->GetPackageName(&packageName);
        String parentClassName = BuildClassName(packageName, parentName, outError);
        if ((*outError)[0].IsNull()) {
            a->mInfo->SetParentActivityName(parentClassName);
        }
        else {
            // Logger::E(TAG, String("Activity ") + packageName + " specified invalid parentActivityName " +
            //         parentName);
            (*outError)[0] = NULL;
        }
    }

    String str;
    sa->GetNonConfigurationString(
        R::styleable::AndroidManifestActivity_permission,
        IConfiguration::NATIVE_CONFIG_VERSION, &str);
    if (str.IsNull()) {
        String perm;
        owner->mApplicationInfo->GetPermission(&perm);
        a->mInfo->SetPermission(perm);
    }
    else {
        a->mInfo->SetPermission(!str.IsNullOrEmpty() ? str : String(NULL));
    }

    sa->GetNonConfigurationString(
        R::styleable::AndroidManifestActivity_taskAffinity,
        IConfiguration::NATIVE_CONFIG_VERSION, &str);
    AutoPtr<ICharSequence> cStr;
    CString::New(str, (ICharSequence**)&cStr);
    String ownerPName, ownerTaskAffinity;
    owner->mApplicationInfo->GetPackageName(&ownerPName);
    owner->mApplicationInfo->GetTaskAffinity(&ownerTaskAffinity);
    String taName = BuildTaskAffinityName(ownerPName, ownerTaskAffinity, cStr, outError);
    a->mInfo->SetTaskAffinity(taName);

    a->mInfo->SetFlags(0);
    Boolean bvalue;
    sa->GetBoolean(
        R::styleable::AndroidManifestActivity_multiprocess,
        FALSE, &bvalue);
    if (bvalue) {
        a->mInfo->GetFlags(&ivalue);
        ivalue |= IActivityInfo::FLAG_MULTIPROCESS;
        a->mInfo->SetFlags(ivalue);
    }

    sa->GetBoolean(
        R::styleable::AndroidManifestActivity_finishOnTaskLaunch,
        FALSE, &bvalue);
    if (bvalue) {
        a->mInfo->GetFlags(&ivalue);
        ivalue |= IActivityInfo::FLAG_FINISH_ON_TASK_LAUNCH;
        a->mInfo->SetFlags(ivalue);
    }

    sa->GetBoolean(
        R::styleable::AndroidManifestActivity_clearTaskOnLaunch,
        FALSE, &bvalue);
    if (bvalue) {
        a->mInfo->GetFlags(&ivalue);
        ivalue |= IActivityInfo::FLAG_CLEAR_TASK_ON_LAUNCH;
        a->mInfo->SetFlags(ivalue);
    }

    sa->GetBoolean(
        R::styleable::AndroidManifestActivity_noHistory,
        FALSE, &bvalue);
    if (bvalue) {
        a->mInfo->GetFlags(&ivalue);
        ivalue |= IActivityInfo::FLAG_NO_HISTORY;
        a->mInfo->SetFlags(ivalue);
    }

    sa->GetBoolean(
        R::styleable::AndroidManifestActivity_alwaysRetainTaskState,
        FALSE, &bvalue);
    if (bvalue) {
        a->mInfo->GetFlags(&ivalue);
        ivalue |= IActivityInfo::FLAG_ALWAYS_RETAIN_TASK_STATE;
        a->mInfo->SetFlags(ivalue);
    }

    sa->GetBoolean(
        R::styleable::AndroidManifestActivity_stateNotNeeded,
        FALSE, &bvalue);
    if (bvalue) {
        a->mInfo->GetFlags(&ivalue);
        ivalue |= IActivityInfo::FLAG_STATE_NOT_NEEDED;
        a->mInfo->SetFlags(ivalue);
    }

    sa->GetBoolean(
        R::styleable::AndroidManifestActivity_excludeFromRecents,
        FALSE, &bvalue);
    if (bvalue) {
        a->mInfo->GetFlags(&ivalue);
        ivalue |= IActivityInfo::FLAG_EXCLUDE_FROM_RECENTS;
        a->mInfo->SetFlags(ivalue);
    }

    Int32 ownerFlags;
    owner->mApplicationInfo->GetFlags(&ownerFlags);
    sa->GetBoolean(
        R::styleable::AndroidManifestActivity_allowTaskReparenting,
        (ownerFlags & IActivityInfo::FLAG_ALLOW_TASK_REPARENTING) != 0, &bvalue);
    if (bvalue) {
        a->mInfo->GetFlags(&ivalue);
        ivalue |= IActivityInfo::FLAG_ALLOW_TASK_REPARENTING;
        a->mInfo->SetFlags(ivalue);
    }

    sa->GetBoolean(
        R::styleable::AndroidManifestActivity_finishOnCloseSystemDialogs,
        FALSE, &bvalue);
    if (bvalue) {
        a->mInfo->GetFlags(&ivalue);
        ivalue |= IActivityInfo::FLAG_FINISH_ON_CLOSE_SYSTEM_DIALOGS;
        a->mInfo->SetFlags(ivalue);
    }

    sa->GetBoolean(
        R::styleable::AndroidManifestActivity_showOnLockScreen,
        FALSE, &bvalue);
    if (bvalue) {
        a->mInfo->GetFlags(&ivalue);
        ivalue |= IActivityInfo::FLAG_SHOW_ON_LOCK_SCREEN;
        a->mInfo->SetFlags(ivalue);
    }

    sa->GetBoolean(
        R::styleable::AndroidManifestActivity_immersive,
        FALSE, &bvalue);
    if (bvalue) {
        a->mInfo->GetFlags(&ivalue);
        ivalue |= IActivityInfo::FLAG_IMMERSIVE;
        a->mInfo->SetFlags(ivalue);
    }

    if (!receiver) {
        sa->GetBoolean(
            R::styleable::AndroidManifestActivity_hardwareAccelerated,
            hardwareAccelerated, &bvalue);
        if (bvalue) {
            a->mInfo->GetFlags(&ivalue);
            ivalue |= IActivityInfo::FLAG_HARDWARE_ACCELERATED;
            a->mInfo->SetFlags(ivalue);
        }

        sa->GetInt32(
            R::styleable::AndroidManifestActivity_launchMode,
            IActivityInfo::LAUNCH_MULTIPLE, &ivalue);
        a->mInfo->SetLaunchMode(ivalue);
        a->mInfo->SetDocumentLaunchMode = sa.getInt(
                R::styleable::AndroidManifestActivity_documentLaunchMode,
                ActivityInfo.DOCUMENT_LAUNCH_NONE);
        a->mInfo->SetMaxRecents = sa.getInt(
                R::styleable::AndroidManifestActivity_maxRecents,
                ActivityManager.getDefaultAppRecentsLimitStatic());
        sa->GetInt32(
            R::styleable::AndroidManifestActivity_screenOrientation,
            IActivityInfo::SCREEN_ORIENTATION_UNSPECIFIED, &ivalue);
        a->mInfo->SetScreenOrientation(ivalue);
        sa->GetInt32(
            R::styleable::AndroidManifestActivity_configChanges,
            0, &ivalue);
        a->mInfo->SetConfigChanges(ivalue);
        sa->GetInt32(
            R::styleable::AndroidManifestActivity_windowSoftInputMode,
            0, &ivalue);
        a->mInfo->SetSoftInputMode(ivalue);

        a.info.persistableMode = sa.getInteger(
                R::styleable::AndroidManifestActivity_persistableMode,
                ActivityInfo.PERSIST_ROOT_ONLY);

        if (sa.getBoolean(
                R::styleable::AndroidManifestActivity_allowEmbedded,
                FALSE)) {
            a.info.flags |= ActivityInfo.FLAG_ALLOW_EMBEDDED;
        }

        if (sa.getBoolean(
                R::styleable::AndroidManifestActivity_autoRemoveFromRecents,
                FALSE)) {
            a.info.flags |= ActivityInfo.FLAG_AUTO_REMOVE_FROM_RECENTS;
        }

        if (sa.getBoolean(
                R::styleable::AndroidManifestActivity_relinquishTaskIdentity,
                FALSE)) {
            a.info.flags |= ActivityInfo.FLAG_RELINQUISH_TASK_IDENTITY;
        }

        if (sa.getBoolean(
                R::styleable::AndroidManifestActivity_resumeWhilePausing,
                FALSE)) {
            a.info.flags |= ActivityInfo.FLAG_RESUME_WHILE_PAUSING;
        }
    }
    else {
        a->mInfo->SetLaunchMode(IActivityInfo::LAUNCH_MULTIPLE);
        a->mInfo->SetConfigChanges(0);
    }

    if (receiver) {
        sa->GetBoolean(
            R::styleable::AndroidManifestActivity_singleUser,
            FALSE, &bvalue);
        if (bvalue) {
            a->mInfo->GetFlags(&ivalue);
            ivalue |= IActivityInfo::FLAG_SINGLE_USER;
            a->mInfo->SetFlags(ivalue);
            Boolean exported;
            a->mInfo->GetExported(&exported);
            if (exported && (flags & PARSE_IS_PRIVILEGED) == 0) {
                String description;
                parser->GetPositionDescription(&description);
                // Slogger::W(TAG, String("Activity exported request ignored due to singleUser: ")
                //         + a->mClassName + " at " + mArchiveSourcePath + " "
                //         + description);
                a->mInfo->SetExported(FALSE);
                setExported = TRUE;
            }
        }
        sa->GetBoolean(
            R::styleable::AndroidManifestActivity_primaryUserOnly,
            FALSE, &bvalue);
        if (bvalue) {
            a->mInfo->GetFlags(&ivalue);
            ivalue |= IActivityInfo::FLAG_PRIMARY_USER_ONLY;
            a->mInfo->SetFlags(ivalue);
        }
    }

    sa->Recycle();

    if (receiver && (ownerFlags & IApplicationInfo::FLAG_CANT_SAVE_STATE) != 0) {
        // A heavy-weight application can not have receives in its main process
        // We can do direct compare because we intern all strings.
        String pname;
        a->mInfo->GetProcessName(&pname);
        if (pname.Equals(owner->mPackageName)) {
            (*outError)[0] = "Heavy-weight applications can not have receivers in main process";
        }
    }

    if (!(*outError)[0].IsNull()) {
        return NULL;
    }

    Int32 outerDepth;
    parser->GetDepth(&outerDepth);

    Int32 type = 0;
    Int32 depth = 0;
    while ((parser->Next(&type), type) != IXmlPullParser::END_DOCUMENT
           && (type != IXmlPullParser::END_TAG
                   || (parser->GetDepth(&depth), depth) > outerDepth)) {
        if (type == IXmlPullParser::END_TAG || type == IXmlPullParser::TEXT) {
            continue;
        }

        String name;
        parser->GetName(&name);
        if (name.Equals("intent-filter")) {
            AutoPtr<ActivityIntentInfo> intent = new ActivityIntentInfo(a);
            if (!ParseIntent(res, parser, attrs, TRUE, intent, outError)) {
                return NULL;
            }
            if (intent->CountActions() == 0) {
                String des;
                parser->GetPositionDescription(&des);
                // SLogger::W(TAG, StringBuffer("No actions in intent filter at ")
                //         + mArchiveSourcePath + " " + des);
            }
            else {
                a->mIntents.PushBack(intent);
            }
        }
        else if (!receiver && parser.getName().Equals("preferred")) {
                ActivityIntentInfo intent = new ActivityIntentInfo(a);
                if (!parseIntent(res, parser, attrs, FALSE, intent, outError)) {
                    return NULL;
                }
                if (intent.countActions() == 0) {
                    Slog.w(TAG, "No actions in preferred at "
                            + mArchiveSourcePath + " "
                            + parser.getPositionDescription());
                } else {
                    if (owner.preferredActivityFilters == NULL) {
                        owner.preferredActivityFilters = new ArrayList<ActivityIntentInfo>();
                    }
                    owner.preferredActivityFilters.add(intent);
                }
            }
        else if (name.Equals("meta-data")) {
            if ((a->mMetaData = ParseMetaData(res, parser, attrs, a->mMetaData,
                    outError)) == NULL) {
                return NULL;
            }
        }
        else {
            if (!RIGID_PARSER) {
                // SLogger::W(TAG, StringBuffer("Problem in package ") + mArchiveSourcePath + ":");
                String des;
                parser->GetPositionDescription(&des);
                if (receiver) {
                    // SLogger::W(TAG, StringBuffer("Unknown element under <receiver>: ") + name
                    //     + " at " + mArchiveSourcePath + " " + des);
                }
                else {
                    // SLogger::W(TAG, StringBuffer("Unknown element under <activity>: ") + name
                    //         + " at " + mArchiveSourcePath + " " + des);
                }
                assert(0 && "TODO");
            //XmlUtils::SkipCurrentTag(parser);
                continue;
            } else {
                if (receiver) {
                    // (*outError)[0] = (const char*)(StringBuffer("Bad element under <receiver>: ") + name);
                }
                else {
                    // (*outError)[0] = (const char*)(StringBuffer("Bad element under <activity>: ") + name);
                }
                return NULL;
            }
        }
    }

    if (!setExported) {
        a->mInfo->SetExported(a->mIntents.Begin() != a->mIntents.End());
    }

    return a;
}

AutoPtr<PackageParser::Activity> PackageParser::ParseActivityAlias(
    /* [in] */ Package* owner,
    /* [in] */ IResources* res,
    /* [in] */ IXmlPullParser* parser,
    /* [in] */ IAttributeSet* attrs,
    /* [in] */ Int32 flags,
    /* [in] */ ArrayOf<String>* outError)
{
    Int32 size = ARRAY_SIZE(R::styleable::AndroidManifestActivityAlias);
    AutoPtr<ArrayOf<Int32> > layout = ArrayOf<Int32>::Alloc(size);
    layout->Copy(R::styleable::AndroidManifestActivityAlias, size);

    AutoPtr<ITypedArray> sa;
    FAIL_RETURN(res->ObtainAttributes(attrs, layout, (ITypedArray**)&sa));

    String targetActivity;
    sa->GetNonConfigurationString(
        R::styleable::AndroidManifestActivityAlias_targetActivity,
        IConfiguration::NATIVE_CONFIG_VERSION, &targetActivity);
    if (targetActivity.IsNull()) {
        (*outError)[0] = "<activity-alias> does not specify android:targetActivity";
        sa->Recycle();
        return NULL;
    }

    String ownerPName;
    owner->mApplicationInfo->GetPackageName(&ownerPName);
    targetActivity = BuildClassName(ownerPName, targetActivity, outError);
    if (targetActivity.IsNull()) {
        sa->Recycle();
        return NULL;
    }

    if (mParseActivityAliasArgs == NULL) {
        mParseActivityAliasArgs = new ParseComponentArgs(owner, outError,
                R::styleable::AndroidManifestActivityAlias_name,
                R::styleable::AndroidManifestActivityAlias_label,
                R::styleable::AndroidManifestActivityAlias_icon,
                R::styleable::AndroidManifestActivityAlias_logo,
                R::styleable::AndroidManifestActivityAlias_banner,
                mSeparateProcesses,
                0,
                R::styleable::AndroidManifestActivityAlias_description,
                R::styleable::AndroidManifestActivityAlias_enabled);
        mParseActivityAliasArgs->mTag = "<activity-alias>";
    }

    mParseActivityAliasArgs->mSa = sa;
    mParseActivityAliasArgs->mFlags = flags;

    AutoPtr<Activity> target;

    List< AutoPtr<Activity> >::Iterator it;
    for (it = owner->mActivities.Begin(); it != owner->mActivities.End(); ++it) {
        String name;
        (*it)->mInfo->GetName(&name);
        if (targetActivity.Equals(name)) {
            target = *it;
            break;
        }
    }

    if (target == NULL) {
        // (*outError)[0] = (const char*)(
        //     StringBuffer("<activity-alias> target activity ") + targetActivity
        //     + " not found in manifest");
        sa->Recycle();
        return NULL;
    }

    AutoPtr<IActivityInfo> info;
    FAIL_RETURN(CActivityInfo::New((IActivityInfo**)&info));


    info.targetActivity = targetActivity;
    info.configChanges = target.info.configChanges;
    info.flags = target.info.flags;
    info.icon = target.info.icon;
    info.logo = target.info.logo;
    info.labelRes = target.info.labelRes;
    info.nonLocalizedLabel = target.info.nonLocalizedLabel;
    info.launchMode = target.info.launchMode;
    info.processName = target.info.processName;
    if (info.descriptionRes == 0) {
        info.descriptionRes = target.info.descriptionRes;
    }
    info.screenOrientation = target.info.screenOrientation;
    info.taskAffinity = target.info.taskAffinity;
    info.theme = target.info.theme;
    info.softInputMode = target.info.softInputMode;
    info.uiOptions = target.info.uiOptions;
    info.parentActivityName = target.info.parentActivityName;


    AutoPtr<Activity> a = new Activity(mParseActivityAliasArgs, info);
    if (!(*outError)[0].IsNull()) {
        sa->Recycle();
        return NULL;
    }

    Boolean setExported;
    sa->HasValue(
        R::styleable::AndroidManifestActivityAlias_exported, &setExported);
    if (setExported) {
        Boolean isExported;
        sa->GetBoolean(
            R::styleable::AndroidManifestActivityAlias_exported,
            FALSE, &isExported);
        a->mInfo->SetExported(isExported);
    }

    String str;
    sa->GetNonConfigurationString(
        R::styleable::AndroidManifestActivityAlias_permission,
        0, &str);
    if (!str.IsNull()) {
        a->mInfo->SetPermission(!str.IsNullOrEmpty() ? str : String(NULL));
    }

    String parentName;
    sa->GetNonConfigurationString(
            R::styleable::AndroidManifestActivityAlias_parentActivityName,
            IConfiguration::NATIVE_CONFIG_VERSION, &parentName);
    if (!parentName.IsNull()) {
        String packageName;
        a->mInfo->GetPackageName(&packageName);
        String parentClassName = BuildClassName(packageName, parentName, outError);
        if ((*outError)[0] == NULL) {
            a->mInfo->SetParentActivityName(parentClassName);
        }
        else {
            String name;
            a->mInfo->GetName(&name);
            // Logger::E(TAG, String("Activity alias ") + name +
            //         " specified invalid parentActivityName " + parentName);
            (*outError)[0] = NULL;
        }
    }

    sa->Recycle();

    if (!(*outError)[0].IsNull()) {
        return NULL;
    }

    Int32 outerDepth = 0;
    parser->GetDepth(&outerDepth);
    Int32 type = 0;
    Int32 depth = 0;
    while ((parser->Next(&type), type) != IXmlPullParser::END_DOCUMENT
           && (type != IXmlPullParser::END_TAG
                   || (parser->GetDepth(&depth), depth) > outerDepth)) {
        if (type == IXmlPullParser::END_TAG || type == IXmlPullParser::TEXT) {
            continue;
        }

        String name;
        parser->GetName(&name);
        if (name.Equals("intent-filter")) {
            AutoPtr<ActivityIntentInfo> intent = new ActivityIntentInfo(a);
            if (!ParseIntent(res, parser, attrs, TRUE, intent, outError)) {
                return NULL;
            }
            if (intent->CountActions() == 0) {
                String des;
                parser->GetPositionDescription(&des);
                // SLogger::W(TAG, StringBuffer("No actions in intent filter at ")
                //         + mArchiveSourcePath + " " + des);
            }
            else {
                a->mIntents.PushBack(intent);
            }
        }
        else if (name.Equals("meta-data")) {
            if ((a->mMetaData = ParseMetaData(res, parser, attrs, a->mMetaData,
                    outError)) == NULL) {
                return NULL;
            }
        }
        else {
            if (!RIGID_PARSER) {
                String des;
                parser->GetPositionDescription(&des);
                // SLogger::W(TAG, StringBuffer("Unknown element under <activity-alias>: ")
                //         + name + " at " + mArchiveSourcePath + " " + des);
                assert(0 && "TODO");
                //XmlUtils::SkipCurrentTag(parser);
                continue;
            }
            else {
                // (*outError)[0] = (const char*)(
                // StringBuffer("Bad element under <activity-alias>: ") + name);
                return NULL;
            }
        }
    }

    if (!setExported) {
        a->mInfo->SetExported(a->mIntents.Begin() != a->mIntents.End());
    }

    return a;
}

AutoPtr<PackageParser::Provider> PackageParser::ParseProvider(
    /* [in] */ Package* owner,
    /* [in] */ IResources* res,
    /* [in] */ IXmlPullParser* parser,
    /* [in] */ IAttributeSet* attrs,
    /* [in] */ Int32 flags,
    /* [in] */ ArrayOf<String>* outError)
{
    Int32 size = ARRAY_SIZE(R::styleable::AndroidManifestProvider);
    AutoPtr<ArrayOf<Int32> > layout = ArrayOf<Int32>::Alloc(size);
    layout->Copy(R::styleable::AndroidManifestProvider, size);

    AutoPtr<ITypedArray> sa;
    FAIL_RETURN(res->ObtainAttributes(attrs, layout, (ITypedArray**)&sa));

    if (mParseProviderArgs == NULL) {
        mParseProviderArgs = new ParseComponentArgs(owner, outError,
                R::styleable::AndroidManifestProvider_name,
                R::styleable::AndroidManifestProvider_label,
                R::styleable::AndroidManifestProvider_icon,
                R::styleable::AndroidManifestProvider_logo,
                R::styleable::AndroidManifestProvider_banner,
                mSeparateProcesses,
                R::styleable::AndroidManifestProvider_process,
                R::styleable::AndroidManifestProvider_description,
                R::styleable::AndroidManifestProvider_enabled);
        mParseProviderArgs->mTag = "<provider>";
    }

    mParseProviderArgs->mSa = sa;
    mParseProviderArgs->mFlags = flags;

    AutoPtr<IProviderInfo> info;
    FAIL_RETURN(CProviderInfo::New((IProviderInfo**)&info));
    AutoPtr<Provider> p = new Provider(mParseProviderArgs, info);
    if (!(*outError)[0].IsNull()) {
        sa->Recycle();
        return NULL;
    }

    Boolean providerExportedDefault = FALSE;

    Int32 targetSdkVersion;
    owner->mApplicationInfo->GetTargetSdkVersion(&targetSdkVersion);
    if (targetSdkVersion < Build::VERSION_CODES::JELLY_BEAN_MR1) {
        // For compatibility, applications targeting API level 16 or lower
        // should have their content providers exported by default, unless they
        // specify otherwise.
        providerExportedDefault = TRUE;
    }

    Boolean bvalue;
    sa->GetBoolean(
            R::styleable::AndroidManifestProvider_exported,
            providerExportedDefault, &bvalue);
    p->mInfo->SetExported(bvalue);

    String cpname;
    sa->GetNonConfigurationString(
            R::styleable::AndroidManifestProvider_authorities,
            0, &cpname);

    sa->GetBoolean(
            R::styleable::AndroidManifestProvider_syncable,
            FALSE, &bvalue);
    p->mInfo->SetIsSyncable(bvalue);

    String permission;
    sa->GetNonConfigurationString(
            R::styleable::AndroidManifestProvider_permission,
            0, &permission);
    String str;
    sa->GetNonConfigurationString(
            R::styleable::AndroidManifestProvider_readPermission,
            0, &str);
    if (str.IsNull()) {
        str = permission;
    }
    if (str.IsNull()) {
        String perm;
        owner->mApplicationInfo->GetPermission(&perm);
        p->mInfo->SetReadPermission(perm);
    }
    else {
        p->mInfo->SetReadPermission(!str.IsNullOrEmpty() ? str : String(NULL));
    }
    sa->GetNonConfigurationString(
            R::styleable::AndroidManifestProvider_writePermission,
            0, &str);
    if (str.IsNull()) {
        str = permission;
    }
    if (str.IsNull()) {
        String perm;
        owner->mApplicationInfo->GetPermission(&perm);
        p->mInfo->SetWritePermission(perm);
    }
    else {
        p->mInfo->SetWritePermission(!str.IsNullOrEmpty() ? str : String(NULL));
    }

    sa->GetBoolean(
            R::styleable::AndroidManifestProvider_grantUriPermissions,
            FALSE, &bvalue);
    p->mInfo->SetGrantUriPermissions(bvalue);

    sa->GetBoolean(
            R::styleable::AndroidManifestProvider_multiprocess,
            FALSE, &bvalue);
    p->mInfo->SetMultiprocess(bvalue);

    Int32 initOrder;
    sa->GetInt32(
            R::styleable::AndroidManifestProvider_initOrder,
            0, &initOrder);
    p->mInfo->SetInitOrder(initOrder);

    p->mInfo->SetFlags(0);

    sa->GetBoolean(
            R::styleable::AndroidManifestProvider_singleUser,
            FALSE, &bvalue);
    if (bvalue) {
        Int32 infoFlags;
        p->mInfo->GetFlags(&infoFlags);
        infoFlags |= IProviderInfo::FLAG_SINGLE_USER;
        p->mInfo->SetFlags(infoFlags);
        Boolean exported;
        p->mInfo->GetExported(&exported);
        if (exported && (flags & PARSE_IS_PRIVILEGED) == 0) {
            String description;
            parser->GetPositionDescription(&description);
            // Slogger::W(TAG, String("Provider exported request ignored due to singleUser: ")
            //         + p->mClassName + " at " + mArchiveSourcePath + " "
            //         + description);
            p->mInfo->SetExported(FALSE);
        }
    }

    sa->Recycle();

    Int32 ownerFlags;
    owner->mApplicationInfo->GetFlags(&ownerFlags);
    if ((ownerFlags & IApplicationInfo::FLAG_CANT_SAVE_STATE) != 0) {
        // A heavy-weight application can not have providers in its main process
        // We can do direct compare because we intern all strings.
        String pname;
        p->mInfo->GetProcessName(&pname);
        if (pname.Equals(owner->mPackageName)) {
            (*outError)[0] = "Heavy-weight applications can not have providers in main process";
            return NULL;
        }
    }

    if (cpname.IsNull()) {
        (*outError)[0] = "<provider> does not include authorities attribute";
        return NULL;
    }
    p->mInfo->SetAuthority(cpname);

    if (!ParseProviderTags(res, parser, attrs, p, outError)) {
        return NULL;
    }

    return p;
}

Boolean PackageParser::ParseProviderTags(
    /* [in] */ IResources* res,
    /* [in] */ IXmlPullParser* parser,
    /* [in] */ IAttributeSet* attrs,
    /* [in] */ Provider* outInfo,
    /* [in] */ ArrayOf<String>* outError)
{
    Int32 outerDepth;
    parser->GetDepth(&outerDepth);
    Int32 type = 0;
    Int32 depth = 0;
    while ((parser->Next(&type), type) != IXmlPullParser::END_DOCUMENT
           && (type != IXmlPullParser::END_TAG
                   || (parser->GetDepth(&depth), depth) > outerDepth)) {
        if (type == IXmlPullParser::END_TAG || type == IXmlPullParser::TEXT) {
            continue;
        }

        String name;
        parser->GetName(&name);
        // if (name.Equals("meta-data")) {
        //     if ((outInfo->mMetaData = ParseMetaData(res, parser, attrs,
        //             outInfo->mMetaData, outError)) == NULL) {
        //         return FALSE;
        //     }

        // }
        if (parser.getName().Equals("intent-filter")) {
            ProviderIntentInfo intent = new ProviderIntentInfo(outInfo);
            if (!parseIntent(res, parser, attrs, TRUE, intent, outError)) {
                return FALSE;
            }
            outInfo.intents.add(intent);

        } else if (parser.getName().Equals("meta-data")) {
            if ((outInfo.metaData=parseMetaData(res, parser, attrs,
                    outInfo.metaData, outError)) == NULL) {
                return FALSE;
            }

        }
        else if (name.Equals("grant-uri-permission")) {
            Int32 size = ARRAY_SIZE(R::styleable::AndroidManifestGrantUriPermission);
            AutoPtr<ArrayOf<Int32> > layout = ArrayOf<Int32>::Alloc(size);
            layout->Copy(R::styleable::AndroidManifestGrantUriPermission, size);

            AutoPtr<ITypedArray> sa;
            res->ObtainAttributes(attrs, layout, (ITypedArray**)&sa);

            AutoPtr<IPatternMatcher> pa;

            String str;
            sa->GetNonConfigurationString(
                R::styleable::AndroidManifestGrantUriPermission_path, 0, &str);
            if (!str.IsNull()) {
                CPatternMatcher::New(str, IPatternMatcher::PATTERN_LITERAL,
                    (IPatternMatcher**)&pa);
            }

            sa->GetNonConfigurationString(
                R::styleable::AndroidManifestGrantUriPermission_pathPrefix, 0, &str);
            if (!str.IsNull()) {
                pa = NULL;
                CPatternMatcher::New(str, IPatternMatcher::PATTERN_PREFIX,
                    (IPatternMatcher**)&pa);
            }

            sa->GetNonConfigurationString(
                R::styleable::AndroidManifestGrantUriPermission_pathPattern, 0, &str);
            if (!str.IsNull()) {
                pa = NULL;
                CPatternMatcher::New(str, IPatternMatcher::PATTERN_SIMPLE_GLOB,
                    (IPatternMatcher**)&pa);
            }

            sa->Recycle();

            if (pa != NULL) {
                AutoPtr< ArrayOf<IPatternMatcher*> > uriPermissionPatterns;
                outInfo->mInfo->GetUriPermissionPatterns((ArrayOf<IPatternMatcher*>**)&uriPermissionPatterns);
                if (uriPermissionPatterns == NULL) {
                    uriPermissionPatterns = ArrayOf<IPatternMatcher*>::Alloc(1);
                    uriPermissionPatterns->Set(0, pa);
                    outInfo->mInfo->SetUriPermissionPatterns(uriPermissionPatterns);
                }
                else {
                    const Int32 N = uriPermissionPatterns->GetLength();
                    AutoPtr< ArrayOf<IPatternMatcher*> > newp = ArrayOf<IPatternMatcher*>::Alloc(N + 1);
                    newp->Copy(uriPermissionPatterns, N);
                    newp->Set(N, pa);
                    outInfo->mInfo->SetUriPermissionPatterns(newp);
                }
                outInfo->mInfo->SetGrantUriPermissions(TRUE);
            }
            else {
                if (!RIGID_PARSER) {
                    String des;
                    parser->GetPositionDescription(&des);
                    // SLogger::W(TAG, StringBuffer("Unknown element under <path-permission>: ")
                    //         + name + " at " + mArchiveSourcePath + " " + des);
                    assert(0 && "TODO");
                    //XmlUtils::SkipCurrentTag(parser);
                    continue;
                }
                else {
                    (*outError)[0] = "No path, pathPrefix, or pathPattern for <path-permission>";
                    return FALSE;
                }
            }
            assert(0 && "TODO");
            //XmlUtils::SkipCurrentTag(parser);

        }
        else if (name.Equals("path-permission")) {
            Int32 size = ARRAY_SIZE(R::styleable::AndroidManifestPathPermission);
            AutoPtr<ArrayOf<Int32> > layout = ArrayOf<Int32>::Alloc(size);
            layout->Copy(R::styleable::AndroidManifestPathPermission, size);

            AutoPtr<ITypedArray> sa;
            res->ObtainAttributes(attrs, layout, (ITypedArray**)&sa);

            AutoPtr<IPathPermission> pa;

            String permission;
            sa->GetNonConfigurationString(
                R::styleable::AndroidManifestPathPermission_permission,
                0, &permission);
            String readPermission;
            sa->GetNonConfigurationString(
                R::styleable::AndroidManifestPathPermission_readPermission,
                0, &readPermission);
            if (readPermission.IsNull()) {
                readPermission = permission;
            }
            String writePermission;
            sa->GetNonConfigurationString(
                R::styleable::AndroidManifestPathPermission_writePermission,
                0, &writePermission);
            if (writePermission.IsNull()) {
                writePermission = permission;
            }

            Boolean havePerm = FALSE;
            if (!readPermission.IsNull()) {
                havePerm = TRUE;
            }
            if (!writePermission.IsNull()) {
                havePerm = TRUE;
            }

            if (!havePerm) {
                if (!RIGID_PARSER) {
                    String des;
                    parser->GetPositionDescription(&des);
                    // Logger::W(TAG, StringBuffer("No readPermission or writePermssion for <path-permission>: ")
                    //         + name + " at " + mArchiveSourcePath + " " + des);
                    assert(0 && "TODO");
            //XmlUtils::SkipCurrentTag(parser);
                    continue;
                }
                else {
                    (*outError)[0] = "No readPermission or writePermssion for <path-permission>";
                    return FALSE;
                }
            }

            String path;
            sa->GetNonConfigurationString(
                R::styleable::AndroidManifestPathPermission_path, 0, &path);
            if (!path.IsNull()) {
                CPathPermission::New(path,
                    IPatternMatcher::PATTERN_LITERAL, readPermission, writePermission,
                    (IPathPermission**)&pa);
            }

            sa->GetNonConfigurationString(
                R::styleable::AndroidManifestPathPermission_pathPrefix, 0, &path);
            if (!path.IsNull()) {
                pa = NULL;
                CPathPermission::New(path,
                    IPatternMatcher::PATTERN_PREFIX, readPermission, writePermission,
                    (IPathPermission**)&pa);
            }

            sa->GetNonConfigurationString(
                R::styleable::AndroidManifestPathPermission_pathPattern, 0, &path);
            if (!path.IsNull()) {
                pa = NULL;
                CPathPermission::New(path,
                    IPatternMatcher::PATTERN_SIMPLE_GLOB, readPermission, writePermission,
                    (IPathPermission**)&pa);
            }

            sa->Recycle();

            if (pa != NULL) {
                AutoPtr< ArrayOf<IPathPermission*> > pathPermissions;
                outInfo->mInfo->GetPathPermissions((ArrayOf<IPathPermission*>**)&pathPermissions);
                if (pathPermissions == NULL) {
                    pathPermissions = ArrayOf<IPathPermission*>::Alloc(1);
                    pathPermissions->Set(0, pa);
                    outInfo->mInfo->SetPathPermissions(pathPermissions);
                } else {
                    const Int32 N = pathPermissions->GetLength();
                    AutoPtr<ArrayOf<IPathPermission*> > newp = ArrayOf<IPathPermission*>::Alloc(N + 1);
                    for (Int32 i = 0; i < N; i++) {
                        newp->Set(i, (*pathPermissions)[i]);
                    }
                    newp->Set(N, pa);
                    outInfo->mInfo->SetPathPermissions(newp);
                }
            }
            else {
                if (!RIGID_PARSER) {
                    String des;
                    parser->GetPositionDescription(&des);
                    // SLogger::W(TAG, StringBuffer("No path, pathPrefix, or pathPattern for <path-permission>: ")
                    //         + name + " at " + mArchiveSourcePath + " " + des);
                    assert(0 && "TODO");
            //XmlUtils::SkipCurrentTag(parser);
                    continue;
                }
                (*outError)[0] = "No path, pathPrefix, or pathPattern for <path-permission>";
                return FALSE;
            }
            assert(0 && "TODO");
            //XmlUtils::SkipCurrentTag(parser);

        }
        else {
            if (!RIGID_PARSER) {
                String des;
                parser->GetPositionDescription(&des);
                // Logger::W(TAG, StringBuffer("Unknown element under <provider>: ")
                //         + name + " at " + mArchiveSourcePath + " " + des);
                assert(0 && "TODO");
            //XmlUtils::SkipCurrentTag(parser);
                continue;
            }
            else {
                // (*outError)[0] = (const char*)(
                //     StringBuffer("Bad element under <provider>: ") + name);
                return FALSE;
            }
        }
    }
    return TRUE;
}

AutoPtr<PackageParser::Service> PackageParser::ParseService(
    /* [in] */ Package* owner,
    /* [in] */ IResources* res,
    /* [in] */ IXmlPullParser* parser,
    /* [in] */ IAttributeSet* attrs,
    /* [in] */ Int32 flags,
    /* [in] */ ArrayOf<String>* outError)
{
    Int32 size = ARRAY_SIZE(R::styleable::AndroidManifestService);
    AutoPtr<ArrayOf<Int32> > layout = ArrayOf<Int32>::Alloc(size);
    layout->Copy(R::styleable::AndroidManifestService, size);

    AutoPtr<ITypedArray> sa;
    res->ObtainAttributes(attrs, layout, (ITypedArray**)&sa);

    if (mParseServiceArgs == NULL) {
        mParseServiceArgs = new ParseComponentArgs(owner, outError,
            R::styleable::AndroidManifestService_name,
            R::styleable::AndroidManifestService_label,
            R::styleable::AndroidManifestService_icon,
            R::styleable::AndroidManifestService_logo,
            R::styleable::AndroidManifestService_banner,
            mSeparateProcesses,
            R::styleable::AndroidManifestService_process,
            R::styleable::AndroidManifestService_description,
            R::styleable::AndroidManifestService_enabled);
        mParseServiceArgs->mTag = "<service>";
    }

    mParseServiceArgs->mSa = sa;
    mParseServiceArgs->mFlags = flags;

    AutoPtr<IServiceInfo> si;
    CServiceInfo::New((IServiceInfo**)&si);
    AutoPtr<Service> s = new Service(mParseServiceArgs, si);
    if (!(*outError)[0].IsNull()) {
        sa->Recycle();
        return NULL;
    }

    Boolean setExported = FALSE;
    sa->HasValue(
        R::styleable::AndroidManifestService_exported,
        &setExported);
    if (setExported) {
        Boolean isExported;
        sa->GetBoolean(
            R::styleable::AndroidManifestService_exported,
            FALSE, &isExported);
        s->mInfo->SetExported(isExported);
    }

    String str;
    sa->GetNonConfigurationString(
        R::styleable::AndroidManifestService_permission,
        0, &str);
    if (str.IsNull()) {
        String ownerPerm;
        owner->mApplicationInfo->GetPermission(&ownerPerm);
        s->mInfo->SetPermission(ownerPerm);
    }
    else {
        s->mInfo->SetPermission(!str.IsNullOrEmpty() ? str : String(NULL));
    }

    s->mInfo->SetFlags(0);
    Int32 ivalue;
    Boolean bvalue;
    sa->GetBoolean(
        R::styleable::AndroidManifestService_stopWithTask,
        FALSE, &bvalue);
    if (bvalue) {
        s->mInfo->GetFlags(&ivalue);
        ivalue |= IServiceInfo::FLAG_STOP_WITH_TASK;
        s->mInfo->SetFlags(ivalue);
    }
    sa->GetBoolean(
        R::styleable::AndroidManifestService_isolatedProcess,
        FALSE, &bvalue);
    if (bvalue) {
        s->mInfo->GetFlags(&ivalue);
        ivalue |= IServiceInfo::FLAG_ISOLATED_PROCESS;
        s->mInfo->SetFlags(ivalue);
    }
    sa->GetBoolean(
        R::styleable::AndroidManifestService_singleUser,
        FALSE, &bvalue);
    if (bvalue) {
        s->mInfo->GetFlags(&ivalue);
        ivalue |= IServiceInfo::FLAG_SINGLE_USER;
        s->mInfo->SetFlags(ivalue);
        Boolean isExported;
        s->mInfo->GetExported(&isExported);
        if (isExported && (flags & PARSE_IS_PRIVILEGED) == 0) {
            String description;
            parser->GetPositionDescription(&description);
            // Slogger::W(TAG, String("Service exported request ignored due to singleUser: ")
            //         + s->mClassName + " at " + mArchiveSourcePath + " "
            //         + description);
            s->mInfo->SetExported(FALSE);
            setExported = TRUE;
        }
    }

    sa->Recycle();

    Int32 ownerFlags;
    owner->mApplicationInfo->GetFlags(&ownerFlags);
    if ((ownerFlags & IApplicationInfo::FLAG_CANT_SAVE_STATE) != 0) {
        // A heavy-weight application can not have services in its main process
        // We can do direct compare because we intern all strings
        String pname;
        s->mInfo->GetProcessName(&pname);
        if (pname.Equals(owner->mPackageName)) {
            (*outError)[0] = "Heavy-weight applications can not have services in main process";
            return NULL;
        }
    }

    Int32 outerDepth;
    parser->GetDepth(&outerDepth);
    Int32 type = 0;
    Int32 depth = 0;
    while ((parser->Next(&type), type) != IXmlPullParser::END_DOCUMENT
           && (type != IXmlPullParser::END_TAG
                   || (parser->GetDepth(&depth), depth) > outerDepth)) {
        if (type == IXmlPullParser::END_TAG || type == IXmlPullParser::TEXT) {
            continue;
        }

        String name;
        parser->GetName(&name);
        if (name.Equals("intent-filter")) {
            AutoPtr<ServiceIntentInfo> intent = new ServiceIntentInfo(s);
            if (!ParseIntent(res, parser, attrs, TRUE, intent, outError)) {
                return NULL;
            }

            s->mIntents.PushBack(intent);
        }
        else if (name.Equals("meta-data")) {
            if ((s->mMetaData = ParseMetaData(res, parser, attrs, s->mMetaData,
                    outError)) == NULL) {
                return NULL;
            }
        }
        else {
            if (!RIGID_PARSER) {
                String des;
                parser->GetPositionDescription(&des);
                // SLogger::W(TAG, StringBuffer("Unknown element under <service>: ")
                //         + name + " at " + mArchiveSourcePath + " " + des);
                assert(0 && "TODO");
            //XmlUtils::SkipCurrentTag(parser);
                continue;
            }
            else {
                // (*outError)[0] = (const char*)(
                //     StringBuffer("Bad element under <service>: ") + name);
                return NULL;
            }
        }
    }

    if (!setExported) {
        s->mInfo->SetExported(s->mIntents.Begin() != s->mIntents.End());
    }

    return s;
}

template <typename T>
Boolean PackageParser::ParseAllMetaData(
    /* [in] */ IResources* res,
    /* [in] */ IXmlPullParser* parser,
    /* [in] */ IAttributeSet* attrs,
    /* [in] */ const String& tag,
    /* [in] */ Component<T>* outInfo,
    /* [in] */ ArrayOf<String>* outError)
{
    Int32 outerDepth = 0;
    parser->GetDepth(&outerDepth);
    Int32 type = 0;
    Int32 depth = 0;
    while ((parser->Next(&type), type) != IXmlPullParser::END_DOCUMENT
        && (type != IXmlPullParser::END_TAG
        || (parser->GetDepth(&depth), depth) > outerDepth)) {
        if (type == IXmlPullParser::END_TAG || type == IXmlPullParser::TEXT) {
            continue;
        }

        String name;
        parser->GetName(&name);
        if (name.Equals("meta-data")) {
            if ((outInfo->mMetaData = ParseMetaData(res, parser, attrs,
                outInfo->mMetaData, outError)) == NULL) {
                return FALSE;
            }
        }
        else {
            if (!RIGID_PARSER) {
                String des;
                parser->GetPositionDescription(&des);
                // SLogger::W(TAG, StringBuffer("Unknown element under ") + tag + ": "
                //     + name + " at " + mArchiveSourcePath + " "+ des);
                assert(0 && "TODO");
            //XmlUtils::SkipCurrentTag(parser);
                continue;
            }
            else {
                // (*outError)[0] = (const char*)(
                //     StringBuffer("Bad element under ") + tag + ": " + name);
                return FALSE;
            }
        }
    }
    return TRUE;
}

AutoPtr<IBundle> PackageParser::ParseMetaData(
    /* [in] */ IResources* res,
    /* [in] */ IXmlPullParser* parser,
    /* [in] */ IAttributeSet* attrs,
    /* [in] */ IBundle* _data,
    /* [in] */ ArrayOf<String>* outError)
{
    Int32 size = ARRAY_SIZE(R::styleable::AndroidManifestMetaData);
    AutoPtr<ArrayOf<Int32> > layout = ArrayOf<Int32>::Alloc(size);
    layout->Copy(R::styleable::AndroidManifestMetaData, size);

    AutoPtr<IBundle> data = _data;
    AutoPtr<ITypedArray> sa;
    FAIL_RETURN(res->ObtainAttributes(attrs, layout, (ITypedArray**)&sa));

    if (data == NULL) {
        FAIL_RETURN(CBundle::New((IBundle**)&data));
    }

    String name;
    sa->GetNonConfigurationString(
        R::styleable::AndroidManifestMetaData_name, 0, &name);
    if (name.IsNull()) {
        (*outError)[0] = "<meta-data> requires an android:name attribute";
        sa->Recycle();
        return NULL;
    }

    AutoPtr<ITypedValue> v;
    sa->PeekValue(
        R::styleable::AndroidManifestMetaData_resource,
        (ITypedValue**)&v);
    Int32 vResId;
    if (v != NULL && (v->GetResourceId(&vResId), vResId != 0)) {
        //SLog::i(TAG, "Meta data ref " + name + ": " + v);
        data->PutInt32(name, vResId);
    }
    else {
        v = NULL;
        sa->PeekValue(
            R::styleable::AndroidManifestMetaData_value,
            (ITypedValue**)&v);
        //SLog::i(TAG, "Meta data " + name + ": " + v);
        if (v != NULL) {
            Int32 vType;
            v->GetType(&vType);
            if (vType == ITypedValue::TYPE_STRING) {
                AutoPtr<ICharSequence> cs;
                v->CoerceToString((ICharSequence**)&cs);
                String csStr;
                if (cs != NULL) cs->ToString(&csStr);
                data->PutString(name, csStr);
            }
            else if (vType == ITypedValue::TYPE_INT_BOOLEAN) {
                Int32 vData;
                v->GetData(&vData);
                data->PutBoolean(name, vData != 0);
            }
            else if (vType >= ITypedValue::TYPE_FIRST_INT
                && vType <= ITypedValue::TYPE_LAST_INT) {
                Int32 vData;
                v->GetData(&vData);
                data->PutInt32(name, vData);
            }
            else if (vType == ITypedValue::TYPE_FLOAT) {
                Float f = 0;
                v->GetFloat(&f);
                data->PutFloat(name, f);
            }
            else {
                if (!RIGID_PARSER) {
                    String name;
                    String des;
                    parser->GetName(&name);
                    parser->GetPositionDescription(&des);
                    // SLogger::W(TAG,
                    //     StringBuffer("<meta-data> only supports string, integer, float, color, Boolean, and resource reference types: ")
                    //     + name + " at " + (String)mArchiveSourcePath + " " + des);
                }
                else {
                    (*outError)[0] =
                            "<meta-data> only supports string, integer, float, color, Boolean, and resource reference types";
                    data = NULL;
                }
            }
        }
        else {
            (*outError)[0] = "<meta-data> requires an android:value or android:resource attribute";
            data = NULL;
        }
    }

    sa->Recycle();

    XmlUtils::SkipCurrentTag(parser);

    return data;
}

AutoPtr<IVerifierInfo> PackageParser::ParseVerifier(
    /* [in] */ IResources* res,
    /* [in] */ IXmlPullParser* parser,
    /* [in] */ IAttributeSet* attrs,
    /* [in] */ Int32 flags)
{
    Int32 size = ARRAY_SIZE(R::styleable::AndroidManifestPackageVerifier);
    AutoPtr<ArrayOf<Int32> > layout = ArrayOf<Int32>::Alloc(size);
    layout->Copy(R::styleable::AndroidManifestPackageVerifier, size);

    AutoPtr<ITypedArray> sa;
    res->ObtainAttributes(attrs, layout, (ITypedArray**)&sa);

    String packageName;
    sa->GetNonResourceString(
            R::styleable::AndroidManifestPackageVerifier_name, &packageName);

    String encodedPublicKey;
    sa->GetNonResourceString(
            R::styleable::AndroidManifestPackageVerifier_publicKey, &encodedPublicKey);

    sa->Recycle();

    if (packageName.IsNullOrEmpty()) {
        Slogger::I(TAG, "verifier package name was NULL; skipping");
        return NULL;
    }

    PublicKey publicKey = parsePublicKey(encodedPublicKey);
    if (publicKey == NULL) {
        Slog.i(TAG, "Unable to parse verifier public key for " + packageName);
        return NULL;
    }

    return new VerifierInfo(packageName, publicKey);
}

ECode PackageParser::ParsePublicKey(
    /* [in] */ const String& encodedPublicKey,
    /* [out] */ IPublicKey** key)
{
    if (encodedPublicKey == NULL) {
        Slog.i(TAG, "Could not parse NULL public key");
        return NULL;
    }

    EncodedKeySpec keySpec;
    try {
        byte[] encoded = Base64.decode(encodedPublicKey, Base64.DEFAULT);
        keySpec = new X509EncodedKeySpec(encoded);
    } catch (IllegalArgumentException e) {
        Slog.i(TAG, "Could not parse verifier public key; invalid Base64");
        return NULL;
    }

    /* First try the key as an RSA key. */
    try {
        KeyFactory keyFactory = KeyFactory.getInstance("RSA");
        return keyFactory.generatePublic(keySpec);
    } catch (NoSuchAlgorithmException e) {
        Log.wtf(TAG, "Could not parse public key because RSA isn't included in build");
        return NULL;
    } catch (InvalidKeySpecException e) {
        // Not a RSA public key.
    }

    /* Now try it as a DSA key. */
    try {
        KeyFactory keyFactory = KeyFactory.getInstance("DSA");
        return keyFactory.generatePublic(keySpec);
    } catch (NoSuchAlgorithmException e) {
        Log.wtf(TAG, "Could not parse public key because DSA isn't included in build");
        return NULL;
    } catch (InvalidKeySpecException e) {
        // Not a DSA public key.
    }

    return NULL;
}

Boolean PackageParser::ParseIntent(
    /* [in] */ IResources* res,
    /* [in] */ IXmlPullParser* parser,
    /* [in] */ IAttributeSet* attrs,
    /* [in] */　Boolean allowGlobs
    /* [in] */ IntentInfo* outInfo,
    /* [out] */ ArrayOf<String>* outError)
{
    Int32 size = ARRAY_SIZE(R::styleable::AndroidManifestIntentFilter);
    AutoPtr<ArrayOf<Int32> > layout = ArrayOf<Int32>::Alloc(size);
    layout->Copy(R::styleable::AndroidManifestIntentFilter, size);

    AutoPtr<ITypedArray> sa;
    FAIL_RETURN(res->ObtainAttributes(attrs, layout, (ITypedArray**)&sa));

    Int32 priority = 0;
    sa->GetInt32(
        R::styleable::AndroidManifestIntentFilter_priority, 0,
        &priority);
    outInfo->SetPriority(priority);

    AutoPtr<ITypedValue> v;
    sa->PeekValue(
        R::styleable::AndroidManifestIntentFilter_label,
        (ITypedValue**)&v);
    if (v != NULL && (v->GetResourceId(&outInfo->mLabelRes), outInfo->mLabelRes) == 0) {
        outInfo->mNonLocalizedLabel = NULL;
        v->CoerceToString((ICharSequence**)&outInfo->mNonLocalizedLabel);
    }

    sa->GetResourceId(
        R::styleable::AndroidManifestIntentFilter_icon, 0,
        &outInfo->mIcon);

    sa->GetResourceId(
        R::styleable::AndroidManifestIntentFilter_logo, 0,
        &outInfo->mLogo);

    sa->GetResourceId(
        R::styleable::AndroidManifestIntentFilter_banner, 0,
        &outInfo->mBanner);

    sa->Recycle();

    Int32 outerDepth;
    parser->GetDepth(&outerDepth);
    Int32 type = 0;
    Int32 depth = 0;
    while ((parser->Next(&type), type) != IXmlPullParser::END_DOCUMENT
           && (type != IXmlPullParser::END_TAG
           || (parser->GetDepth(&depth), depth) > outerDepth)) {
        if (type == IXmlPullParser::END_TAG || type == IXmlPullParser::TEXT) {
            continue;
        }

        String nodeName;
        parser->GetName(&nodeName);
        if (nodeName.Equals("action")) {
            String value;
            ECode ec = attrs->GetAttributeValue(ANDROID_RESOURCES, String("name"), &value);
            if (FAILED(ec) || value.IsNullOrEmpty()) {
                (*outError)[0] = "No value supplied for <android:name>";
                return FALSE;
            }
            assert(0 && "TODO");
            //XmlUtils::SkipCurrentTag(parser);

            outInfo->AddAction(value);
        }
        else if (nodeName.Equals("category")) {
            String value;
            ECode ec = attrs->GetAttributeValue(ANDROID_RESOURCES, String("name"), &value);
            if (FAILED(ec) || value.IsNullOrEmpty()) {
                (*outError)[0] = "No value supplied for <android:name>";
                return FALSE;
            }
            assert(0 && "TODO");
            //XmlUtils::SkipCurrentTag(parser);

            outInfo->AddCategory(value);
        }
        else if (nodeName.Equals("data")) {
            Int32 size = ARRAY_SIZE(R::styleable::AndroidManifestData);
            AutoPtr<ArrayOf<Int32> > layout = ArrayOf<Int32>::Alloc(size);
            layout->Copy(R::styleable::AndroidManifestData, size);

            sa = NULL;
            res->ObtainAttributes(attrs, layout, (ITypedArray**)&sa);

            String str;
            sa->GetNonConfigurationString(
                R::styleable::AndroidManifestData_mimeType, 0, &str);
            if (!str.IsNull()) {
                // try {
                if (FAILED(outInfo->AddDataType(str))) {
                    (*outError)[0] = "E_RUNTIME_EXCEPTION";
                    sa->Recycle();
                    return FALSE;
                }
                // } catch (IntentFilter.MalformedMimeTypeException e) {
                //     (*outError)[0] = "E_RUNTIME_EXCEPTION";
                //     sa->Recycle();
                //     return FALSE;
                // }
            }

            sa->GetNonConfigurationString(
                R::styleable::AndroidManifestData_scheme, 0, &str);
            if (!str.IsNull()) {
                outInfo->AddDataScheme(str);
            }

            str = sa.getNonConfigurationString(
                    R::styleable::AndroidManifestData_ssp, 0);
            if (str != NULL) {
                outInfo.addDataSchemeSpecificPart(str, PatternMatcher.PATTERN_LITERAL);
            }

            str = sa.getNonConfigurationString(
                    R::styleable::AndroidManifestData_sspPrefix, 0);
            if (str != NULL) {
                outInfo.addDataSchemeSpecificPart(str, PatternMatcher.PATTERN_PREFIX);
            }

            str = sa.getNonConfigurationString(
                    R::styleable::AndroidManifestData_sspPattern, 0);
            if (str != NULL) {
                if (!allowGlobs) {
                    (*outError)[0] = "sspPattern not allowed here; ssp must be literal";
                    return FALSE;
                }
                outInfo.addDataSchemeSpecificPart(str, PatternMatcher.PATTERN_SIMPLE_GLOB);
            }

            String host;
            sa->GetNonConfigurationString(
                R::styleable::AndroidManifestData_host, 0, &host);
            String port;
            sa->GetNonConfigurationString(
                R::styleable::AndroidManifestData_port, 0, &port);
            if (!host.IsNull()) {
                outInfo->AddDataAuthority(host, port);
            }

            sa->GetNonConfigurationString(
                R::styleable::AndroidManifestData_path, 0, &str);
            if (!str.IsNull()) {
                outInfo->AddDataPath(str, IPatternMatcher::PATTERN_LITERAL);
            }

            sa->GetNonConfigurationString(
                R::styleable::AndroidManifestData_pathPrefix, 0, &str);
            if (!str.IsNull()) {
                outInfo->AddDataPath(str, IPatternMatcher::PATTERN_PREFIX);
            }

            sa->GetNonConfigurationString(
                R::styleable::AndroidManifestData_pathPattern, 0, &str);
            if (!str.IsNull()) {
                if (!allowGlobs) {
                    (*outError)[0] = "pathPattern not allowed here; path must be literal";
                    return FALSE;
                }
                outInfo->AddDataPath(str, IPatternMatcher::PATTERN_SIMPLE_GLOB);
            }

            sa->Recycle();
            assert(0 && "TODO");
            //XmlUtils::SkipCurrentTag(parser);
        }
        else if (!RIGID_PARSER) {
            String name;
            String des;
            parser->GetName(&name);
            parser->GetPositionDescription(&des);
            // SLogger::W(TAG, StringBuffer("Unknown element under <intent-filter>: ")
            //         + name + " at " + (String)mArchiveSourcePath + " " + des);
            assert(0 && "TODO");
            //XmlUtils::SkipCurrentTag(parser);
        }
        else {
            String name;
            parser->GetName(&name);
            // (*outError)[0] = (const char*)(
            //     StringBuffer("Bad element under <intent-filter>: ") + name);
            return FALSE;
        }
    }

    outInfo->mHasDefault = outInfo->HasCategory(IIntent::CATEGORY_DEFAULT);

    // if (DEBUG_PARSER) {
    //     StringBuilder cats = new StringBuilder("Intent d=");
    //     cats.append(outInfo.hasDefault);
    //     cats.append(", cat=");

    //     Iterator<String> it = outInfo.categoriesIterator();
    //     if (it != NULL) {
    //         while (it.hasNext()) {
    //             cats.append(' ');
    //             cats.append(it.next());
    //         }
    //     }
    //     Slogger::D(TAG, cats.toString());
    // }

    return TRUE;
}

Boolean PackageParser::CopyNeeded(
    /* [in] */ Int32 flags,
    /* [in] */ PackageParser::Package* p,
    /* [in] */ PackageUserState* state,
    /* [in] */ IBundle* metaData,
    /* [in] */ Int32 userId)
{
    if (userId != 0) {
        // We always need to copy for other users, since we need
        // to fix up the uid.
        return TRUE;
    }

    if (state->mEnabled != IPackageManager::COMPONENT_ENABLED_STATE_DEFAULT) {
        Boolean enabled = state->mEnabled == IPackageManager::COMPONENT_ENABLED_STATE_ENABLED;
        Boolean appEnabled;
        p->mApplicationInfo->GetEnabled(&appEnabled);
        if (appEnabled != enabled) {
            return TRUE;
        }
    }
    if (!state->mInstalled || state->mHidden) {
        return TRUE;
    }
    if (state->mStopped) {
        return TRUE;
    }
    if ((flags & IPackageManager::GET_META_DATA) != 0
            && (metaData != NULL || p->mAppMetaData != NULL)) {
        return TRUE;
    }
    if ((flags & IPackageManager::GET_SHARED_LIBRARY_FILES) != 0
            && p->mUsesLibraryFiles != NULL) {
        return TRUE;
    }
    return FALSE;
}

AutoPtr<IApplicationInfo> PackageParser::GenerateApplicationInfo(
    /* [in] */ Package* p,
    /* [in] */ Int32 flags,
    /* [in] */ PackageUserState* state)
{
    return GenerateApplicationInfo(p, flags, state, UserHandle::GetCallingUserId());
}

ECode PackageParser::UpdateApplicationInfo(
    /* [in] */ IApplicationInfo* ai,
    /* [in] */ Int32 flags,
    /* [in] */ PackageUserState* state)
{
    // CompatibilityMode is global state.
    if (!sCompatibilityModeEnabled) {
        ai.disableCompatibilityMode();
    }
    if (state.installed) {
        ai.flags |= ApplicationInfo.FLAG_INSTALLED;
    } else {
        ai.flags &= ~ApplicationInfo.FLAG_INSTALLED;
    }
    if (state.hidden) {
        ai.flags |= ApplicationInfo.FLAG_HIDDEN;
    } else {
        ai.flags &= ~ApplicationInfo.FLAG_HIDDEN;
    }
    if (state.enabled == IPackageManager::COMPONENT_ENABLED_STATE_ENABLED) {
        ai.enabled = TRUE;
    } else if (state.enabled == IPackageManager::COMPONENT_ENABLED_STATE_DISABLED_UNTIL_USED) {
        ai.enabled = (flags&IPackageManager::GET_DISABLED_UNTIL_USED_COMPONENTS) != 0;
    } else if (state.enabled == IPackageManager::COMPONENT_ENABLED_STATE_DISABLED
            || state.enabled == IPackageManager::COMPONENT_ENABLED_STATE_DISABLED_USER) {
        ai.enabled = FALSE;
    }
    ai.enabledSetting = state.enabled;
    return NOERROR
}

AutoPtr<IApplicationInfo> PackageParser::GenerateApplicationInfo(
    /* [in] */ Package* p,
    /* [in] */ Int32 flags,
    /* [in] */ PackageUserState* state,
    /* [in] */ Int32 userId)
{
    if (p == NULL) return NULL;
    if (!CheckUseInstalledOrHidden(flags, state)) {
        return NULL;
    }

    if (!copyNeeded(flags, p, state, NULL, userId)
            && ((flags&IPackageManager::GET_DISABLED_UNTIL_USED_COMPONENTS) == 0
                    || state.enabled != IPackageManager::COMPONENT_ENABLED_STATE_DISABLED_UNTIL_USED)) {
        // In this case it is safe to directly modify the internal ApplicationInfo state:
        // - CompatibilityMode is global state, so will be the same for every call.
        // - We only come in to here if the app should reported as installed; this is the
        // default state, and we will do a copy otherwise.
        // - The enable state will always be reported the same for the application across
        // calls; the only exception is for the UNTIL_USED mode, and in that case we will
        // be doing a copy.
        updateApplicationInfo(p.applicationInfo, flags, state);
        return p.applicationInfo;
    }

    // Make shallow copy so we can store the metadata/libraries safely
    AutoPtr<IApplicationInfo> ai;
    ASSERT_SUCCEEDED(CApplicationInfo::New(
        p->mApplicationInfo, (IApplicationInfo**)&ai));
    if (userId != 0) {
        Int32 aiUid;
        ai->GetUid(&aiUid);
        ai->SetUid(UserHandle::GetUid(userId, aiUid));
        String packageName;
        ai->GetPackageName(&packageName);
        ai->SetDataDir(PackageManager::GetDataDirForUser(userId, packageName));
    }
    if ((flags & IPackageManager::GET_META_DATA) != 0) {
        ai->SetMetaData(p->mAppMetaData);
    }
    if ((flags & IPackageManager::GET_SHARED_LIBRARY_FILES) != 0) {
        ai->SetSharedLibraryFiles(p->mUsesLibraryFiles);
    }

    Int32 aiFlags;
    if (state->mStopped) {
        ai->GetFlags(&aiFlags);
        aiFlags |= IApplicationInfo::FLAG_STOPPED;
        ai->SetFlags(aiFlags);
    }
    else {
        ai->GetFlags(&aiFlags);
        aiFlags &= ~IApplicationInfo::FLAG_STOPPED;
        ai->SetFlags(aiFlags);
    }

    updateApplicationInfo(ai, flags, state);
    return ai;
}

AutoPtr<IApplicationInfo> GenerateApplicationInfo(
    /* [in] */ ApplicationInfo* ai,
    /* [in] */ Int32 flags,
    /* [in] */ PackageUserState* state,
    /* [in] */ Int32 userId
{
    if (state->mInstalled) {
        ai->GetFlags(&aiFlags);
        aiFlags |= IApplicationInfo::FLAG_INSTALLED;
        ai->SetFlags(aiFlags);
    }
    else {
        ai->GetFlags(&aiFlags);
        aiFlags &= ~IApplicationInfo::FLAG_INSTALLED;
        ai->SetFlags(aiFlags);
    }
    if (state->mEnabled == IPackageManager::COMPONENT_ENABLED_STATE_ENABLED) {
        ai->SetEnabled(TRUE);
    }
    else if (state->mEnabled == IPackageManager::COMPONENT_ENABLED_STATE_DISABLED
            || state->mEnabled == IPackageManager::COMPONENT_ENABLED_STATE_DISABLED_USER) {
        ai->SetEnabled(FALSE);
    }
    ai->SetEnabledSetting(state->mEnabled);
    return ai;
}

AutoPtr<IPermissionInfo> PackageParser::GeneratePermissionInfo(
    /* [in] */ Permission* p,
    /* [in] */ Int32 flags)
{
    if (p == NULL) return NULL;
    if ((flags & IPackageManager::GET_META_DATA) == 0) {
        return p->mInfo;
    }
    AutoPtr<IPermissionInfo> pi;
    ASSERT_SUCCEEDED(CPermissionInfo::New(
            p->mInfo, (IPermissionInfo**)&pi));
    pi->SetMetaData(p->mMetaData);
    return pi;
}

AutoPtr<IPermissionGroupInfo> PackageParser::GeneratePermissionGroupInfo(
    /* [in] */ PermissionGroup* pg,
    /* [in] */ Int32 flags)
{
    if (pg == NULL) return NULL;
    if ((flags & IPackageManager::GET_META_DATA) == 0) {
        return pg->mInfo;
    }
    AutoPtr<IPermissionGroupInfo> pgi;
    ASSERT_SUCCEEDED(CPermissionGroupInfo::New(
        pg->mInfo, (IPermissionGroupInfo**)&pgi));
    pgi->SetMetaData(pg->mMetaData);
    return pgi;
}

AutoPtr<IActivityInfo> PackageParser::GenerateActivityInfo(
    /* [in] */ Activity* activity,
    /* [in] */ Int32 flags,
    /* [in] */ PackageUserState* state,
    /* [in] */ Int32 userId)
{
    if (activity == NULL) return NULL;
    if (!CheckUseInstalledOrHidden(flags, state)) {
        return NULL;
    }
    if (!CopyNeeded(flags, activity->mOwner, state, activity->mMetaData, userId)) {
        return activity->mInfo;
    }
    // Make shallow copies so we can store the metadata safely
    AutoPtr<IActivityInfo> ai;
    ASSERT_SUCCEEDED(CActivityInfo::New(
            activity->mInfo, (IActivityInfo**)&ai));
    ai->SetMetaData(activity->mMetaData);
    ai->SetApplicationInfo(GenerateApplicationInfo(activity->mOwner, flags, state, userId));
    return ai;
}

AutoPtr<IActivityInfo> PackageParser::GenerateActivityInfo(
    /* [in] */ IActivityInfo* ai,,
    /* [in] */ Int32 flags,
    /* [in] */ PackageUserState* state,
    /* [in] */ Int32 userId)
{
    if (ai == NULL) return NULL;
    if (!checkUseInstalledOrHidden(flags, state)) {
        return NULL;
    }
    // This is only used to return the ResolverActivity; we will just always
    // make a copy.
    ai = new ActivityInfo(ai);
    ai.applicationInfo = generateApplicationInfo(ai.applicationInfo, flags, state, userId);
    return ai;
}

AutoPtr<IServiceInfo> PackageParser::GenerateServiceInfo(
    /* [in] */ Service* service,
    /* [in] */ Int32 flags,
    /* [in] */ PackageUserState* state,
    /* [in] */ Int32 userId)
{
    if (service == NULL) return NULL;
    if (!CheckUseInstalledOrHidden(flags, state)) {
        return NULL;
    }
    if (!CopyNeeded(flags, service->mOwner, state, service->mMetaData, userId)) {
        return service->mInfo;
    }
    // Make shallow copies so we can store the metadata safely
    AutoPtr<IServiceInfo> si;
    ASSERT_SUCCEEDED(CServiceInfo::New(
            service->mInfo, (IServiceInfo**)&si));
    si->SetMetaData(service->mMetaData);
    si->SetApplicationInfo(GenerateApplicationInfo(service->mOwner, flags, state, userId));
    return si;
}

AutoPtr<IProviderInfo> PackageParser::GenerateProviderInfo(
    /* [in] */ Provider* provider,
    /* [in] */ Int32 flags,
    /* [in] */ PackageUserState* state,
    /* [in] */ Int32 userId)
{
    if (provider == NULL) return NULL;
    if (!CheckUseInstalledOrHidden(flags, state)) {
        return NULL;
    }
    AutoPtr< ArrayOf<IPatternMatcher*> > uriPermissions;
    provider->mInfo->GetUriPermissionPatterns((ArrayOf<IPatternMatcher*>**)&uriPermissions);
    if (!CopyNeeded(flags, provider->mOwner, state, provider->mMetaData, userId)
        && ((flags & IPackageManager::GET_URI_PERMISSION_PATTERNS) != 0
        || uriPermissions == NULL)) {
        return provider->mInfo;
    }
    // Make shallow copies so we can store the metadata safely
    AutoPtr<IProviderInfo> pi;
    ASSERT_SUCCEEDED(CProviderInfo::New(
        provider->mInfo, (IProviderInfo**)&pi));
    pi->SetMetaData(provider->mMetaData);
    if ((flags & IPackageManager::GET_URI_PERMISSION_PATTERNS) == 0) {
        pi->SetUriPermissionPatterns(NULL);
    }
    pi->SetApplicationInfo(GenerateApplicationInfo(provider->mOwner, flags, state, userId));
    return pi;
}

AutoPtr<IInstrumentationInfo> PackageParser::GenerateInstrumentationInfo(
    /* [in] */ PackageParser::Instrumentation* i,
    /* [in] */ Int32 flags)
{
    if (i == NULL) return NULL;
    if ((flags & IPackageManager::GET_META_DATA) == 0) {
        return i->mInfo;
    }
    AutoPtr<IInstrumentationInfo> info;
    ASSERT_SUCCEEDED(CInstrumentationInfo::New(
        i->mInfo, (IInstrumentationInfo**)&info));
    info->SetMetaData(i->mMetaData);
    return info;
}

void PackageParser::SetCompatibilityModeEnabled(
    /* [in] */ Boolean compatibilityModeEnabled)
{
    sCompatibilityModeEnabled = compatibilityModeEnabled;
}

ECode PackageParser::ReadFullyIgnoringContents(
    /* [in] */ IInputStream* in,
    /* [out] */ Int64* result)
{
    byte[] buffer = sBuffer.getAndSet(NULL);
    if (buffer == NULL) {
        buffer = new byte[4096];
    }

    Int32 n = 0;
    Int32 count = 0;
    while ((n = in.read(buffer, 0, buffer.length)) != -1) {
        count += n;
    }

    sBuffer.set(buffer);
    return count;
}

ECode PackageParser::CloseQuietly(
    /* [in] */ IStrictJarFile* jarFile)
{
    // if (jarFile != NULL) {
    //     try {
    //         jarFile.close();
    //     } catch (Exception ignored) {
    //     }
    // }
    return NOERROR;
}

} // namespace Pm
} // namespace Content
} // namespace Droid
} // namespace Elastos
