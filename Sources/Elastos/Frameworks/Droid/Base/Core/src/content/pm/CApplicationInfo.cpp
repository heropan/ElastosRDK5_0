
#include "ext/frameworkext.h"
#include "content/pm/CApplicationInfo.h"
#include <elastos/StringBuilder.h>
#include <elastos/StringUtils.h>

using Elastos::Core::StringUtils;
using Elastos::Core::StringBuilder;

namespace Elastos {
namespace Droid {
namespace Content {
namespace Pm {

CApplicationInfo::CApplicationInfo()
    : mDescriptionRes(0)
    , mTheme(0)
    , mUiOptions(0)
    , mFlags(0)
    , mRequiresSmallestWidthDp(0)
    , mCompatibleWidthLimitDp(0)
    , mLargestWidthLimitDp(0)
    , mResourceDirs(NULL)
    , mSharedLibraryFiles(NULL)
    , mUid(0)
    , mTargetSdkVersion(0)
    , mEnabled(TRUE)
    , mEnabledSetting(IPackageManager::COMPONENT_ENABLED_STATE_DEFAULT)
    , mInstallLocation(IPackageInfo::INSTALL_LOCATION_UNSPECIFIED)
{}

CApplicationInfo::~CApplicationInfo()
{}

ECode CApplicationInfo::constructor()
{
    return PackageItemInfo::constructor();
}

ECode CApplicationInfo::constructor(
    /* [in] */ IApplicationInfo* orig)
{
    assert(orig != NULL);

    PackageItemInfo::constructor((IPackageItemInfo*)orig);
    orig->GetTaskAffinity(&mTaskAffinity);
    orig->GetPermission(&mPermission);
    orig->GetProcessName(&mProcessName);
    orig->GetClassName(&mClassName);
    orig->GetTheme(&mTheme);
    orig->GetFlags(&mFlags);
    orig->GetRequiresSmallestWidthDp(&mRequiresSmallestWidthDp);
    orig->GetCompatibleWidthLimitDp(&mCompatibleWidthLimitDp);
    orig->GetLargestWidthLimitDp(&mLargestWidthLimitDp);
    orig->GetSourceDir(&mSourceDir);
    orig->GetPublicSourceDir(&mPublicSourceDir);
    orig->GetNativeLibraryDir(&mNativeLibraryDir);
    orig->GetResourceDirs((ArrayOf<String>**)&mResourceDirs);
    orig->GetSharedLibraryFiles((ArrayOf<String>**)&mSharedLibraryFiles);
    orig->GetDataDir(&mDataDir);
    orig->GetUid(&mUid);
    orig->GetTargetSdkVersion(&mTargetSdkVersion);
    orig->GetEnabled(&mEnabled);
    orig->GetEnabledSetting(&mEnabledSetting);
    orig->GetInstallLocation(&mInstallLocation);
    orig->GetManageSpaceActivityName(&mManageSpaceActivityName);
    orig->GetDescriptionRes(&mDescriptionRes);
    orig->GetUiOptions(&mUiOptions);
    orig->GetBackupAgentName(&mBackupAgentName);
    return NOERROR;
}

ECode CApplicationInfo::Dump(
    /* [in] */ IPrinter* pw,
    /* [in] */ const String& prefix)
{
    // super.dumpFront(pw, prefix);
    // if (className != null) {
    //     pw.println(prefix + "className=" + className);
    // }
    // if (permission != null) {
    //     pw.println(prefix + "permission=" + permission);
    // }
    // pw.println(prefix + "processName=" + processName);
    // pw.println(prefix + "taskAffinity=" + taskAffinity);
    // pw.println(prefix + "uid=" + uid + " flags=0x" + Integer.toHexString(flags)
    //         + " theme=0x" + Integer.toHexString(theme));
    // pw.println(prefix + "requiresSmallestWidthDp=" + requiresSmallestWidthDp
    //         + " compatibleWidthLimitDp=" + compatibleWidthLimitDp
    //         + " largestWidthLimitDp=" + largestWidthLimitDp);
    // pw.println(prefix + "sourceDir=" + sourceDir);
    // if (sourceDir == null) {
    //     if (publicSourceDir != null) {
    //         pw.println(prefix + "publicSourceDir=" + publicSourceDir);
    //     }
    // } else if (!sourceDir.equals(publicSourceDir)) {
    //     pw.println(prefix + "publicSourceDir=" + publicSourceDir);
    // }
    // if (resourceDirs != null) {
    //     pw.println(prefix + "resourceDirs=" + resourceDirs);
    // }
    // pw.println(prefix + "dataDir=" + dataDir);
    // if (sharedLibraryFiles != null) {
    //     pw.println(prefix + "sharedLibraryFiles=" + sharedLibraryFiles);
    // }
    // pw.println(prefix + "enabled=" + enabled + " targetSdkVersion=" + targetSdkVersion);
    // if (manageSpaceActivityName != null) {
    //     pw.println(prefix + "manageSpaceActivityName="+manageSpaceActivityName);
    // }
    // if (descriptionRes != 0) {
    //     pw.println(prefix + "description=0x"+Integer.toHexString(descriptionRes));
    // }
    // if (uiOptions != 0) {
    //     pw.println(prefix + "uiOptions=0x" + Integer.toHexString(uiOptions));
    // }
    // pw.println(prefix + "supportsRtl=" + (hasRtlSupport() ? "true" : "false"));
    // super.dumpBack(pw, prefix);
    assert(0);
    return E_NOT_IMPLEMENTED;
}

ECode CApplicationInfo::HasRtlSupport(
    /* [out] */ Boolean* isSupport)
{
    VALIDATE_NOT_NULL(isSupport);

    *isSupport = ((mFlags & FLAG_SUPPORTS_RTL) == FLAG_SUPPORTS_RTL);
    return NOERROR;
}

ECode CApplicationInfo::ToString(
    /* [out] */ String* str)
{
    VALIDATE_NOT_NULL(str);

    StringBuilder sb("ApplicationInfo:0x");
    sb += StringUtils::Int32ToHexString((Int32)this);
    sb += ":";
    sb += mPackageName;
    sb += ", ";
    sb += mProcessName;
    sb += ", ";
    sb += mClassName;
    sb += ", ";
    sb += mSourceDir;
    sb.ToString(str);
    return NOERROR;
}

ECode CApplicationInfo::ReadFromParcel(
    /* [in] */ IParcel* source)
{
    assert(source != NULL);

    PackageItemInfo::ReadFromParcel(source);
    source->ReadString(&mTaskAffinity);
    source->ReadString(&mPermission);
    source->ReadString(&mProcessName);
    source->ReadString(&mClassName);
    source->ReadInt32(&mDescriptionRes);
    source->ReadInt32(&mTheme);
    source->ReadString(&mManageSpaceActivityName);
    source->ReadString(&mBackupAgentName);
    source->ReadInt32(&mUiOptions);
    source->ReadInt32(&mFlags);
    source->ReadInt32(&mRequiresSmallestWidthDp);
    source->ReadInt32(&mCompatibleWidthLimitDp);
    source->ReadInt32(&mLargestWidthLimitDp);
    source->ReadString(&mSourceDir);
    source->ReadString(&mPublicSourceDir);
    source->ReadArrayOfString((ArrayOf<String>**)&mResourceDirs);
    source->ReadArrayOfString((ArrayOf<String>**)&mSharedLibraryFiles);
    source->ReadString(&mDataDir);
    source->ReadString(&mNativeLibraryDir);
    source->ReadInt32(&mUid);
    source->ReadInt32(&mTargetSdkVersion);
    source->ReadBoolean(&mEnabled);
    source->ReadInt32(&mEnabledSetting);
    source->ReadInt32(&mInstallLocation);
    return NOERROR;
}

ECode CApplicationInfo::WriteToParcel(
    /* [in] */ IParcel* dest)
{
    assert(dest != NULL);

    PackageItemInfo::WriteToParcel(dest);
    dest->WriteString(mTaskAffinity);
    dest->WriteString(mPermission);
    dest->WriteString(mProcessName);
    dest->WriteString(mClassName);
    dest->WriteInt32(mDescriptionRes);
    dest->WriteInt32(mTheme);
    dest->WriteString(mManageSpaceActivityName);
    dest->WriteString(mBackupAgentName);
    dest->WriteInt32(mUiOptions);
    dest->WriteInt32(mFlags);
    dest->WriteInt32(mRequiresSmallestWidthDp);
    dest->WriteInt32(mCompatibleWidthLimitDp);
    dest->WriteInt32(mLargestWidthLimitDp);
    dest->WriteString(mSourceDir);
    dest->WriteString(mPublicSourceDir);
    dest->WriteArrayOfString(mResourceDirs.Get());
    dest->WriteArrayOfString(mSharedLibraryFiles.Get());
    dest->WriteString(mDataDir);
    dest->WriteString(mNativeLibraryDir);
    dest->WriteInt32(mUid);
    dest->WriteInt32(mTargetSdkVersion);
    dest->WriteBoolean(mEnabled);
    dest->WriteInt32(mEnabledSetting);
    dest->WriteInt32(mInstallLocation);
    return NOERROR;
}

ECode CApplicationInfo::LoadDescription(
    /* [in] */ IPackageManager* pm,
    /* [out] */ ICharSequence** label)
{
    VALIDATE_NOT_NULL(label);
    *label = NULL;

    if (mDescriptionRes != 0) {
        pm->GetText(mPackageName, mDescriptionRes, this, label);
    }

    return NOERROR;
}

ECode CApplicationInfo::DisableCompatibilityMode()
{
    mFlags |= (FLAG_SUPPORTS_LARGE_SCREENS | FLAG_SUPPORTS_NORMAL_SCREENS |
            FLAG_SUPPORTS_SMALL_SCREENS | FLAG_RESIZEABLE_FOR_SCREENS |
            FLAG_SUPPORTS_SCREEN_DENSITIES | FLAG_SUPPORTS_XLARGE_SCREENS);
    return NOERROR;
}

ECode CApplicationInfo::LoadDefaultIcon(
    /* [in] */ IPackageManager* pm,
    /* [out] */ IDrawable** icon)
{
    VALIDATE_NOT_NULL(icon);

    if ((mFlags & FLAG_EXTERNAL_STORAGE) != 0
            && IsPackageUnavailable(pm)) {
        // return Resources.getSystem().getDrawable(
        //         com.android.internal.R.drawable.sym_app_on_sd_unavailable_icon);
        assert(0);
        return E_NOT_IMPLEMENTED;
    }
    return pm->GetDefaultActivityIcon(icon);
}

Boolean CApplicationInfo::IsPackageUnavailable(
    /* [in] */ IPackageManager* pm)
{
    // try {
        AutoPtr<IPackageInfo> pkgInfo;
        ECode ec = pm->GetPackageInfo(mPackageName, 0, (IPackageInfo**)&pkgInfo);
        return SUCCEEDED(ec) ? pkgInfo == NULL : TRUE;
    // } catch (NameNotFoundException ex) {
    //     return true;
    // }
}

AutoPtr<IApplicationInfo> CApplicationInfo::GetApplicationInfo()
{
    return this;
}

ECode CApplicationInfo::LoadLabel(
    /* [in] */ IPackageManager* pm,
    /* [out] */ ICharSequence** label)
{
    VALIDATE_NOT_NULL(label);
    return PackageItemInfo::LoadLabel(pm, label);
}

ECode CApplicationInfo::LoadIcon(
    /* [in] */ IPackageManager* pm,
    /* [out] */ IDrawable** icon)
{
    VALIDATE_NOT_NULL(icon);
    return PackageItemInfo::LoadIcon(pm, icon);
}

ECode CApplicationInfo::LoadLogo(
    /* [in] */ IPackageManager* pm,
    /* [out] */ IDrawable** logo)
{
    VALIDATE_NOT_NULL(logo);
    return PackageItemInfo::LoadLogo(pm, logo);
}

ECode CApplicationInfo::LoadXmlMetaData(
    /* [in] */ IPackageManager* pm,
    /* [in] */ const String& name,
    /* [out] */ IXmlResourceParser** resource)
{
    VALIDATE_NOT_NULL(resource);
    return PackageItemInfo::LoadXmlMetaData(pm, name, resource);
}

ECode CApplicationInfo::GetName(
    /* [out] */ String* name)
{
    VALIDATE_NOT_NULL(name);
    return PackageItemInfo::GetName(name);
}

ECode CApplicationInfo::SetName(
    /* [in] */ const String& name)
{
    return PackageItemInfo::SetName(name);
}

ECode CApplicationInfo::GetPackageName(
    /* [out] */ String* name)
{
    VALIDATE_NOT_NULL(name);
    return PackageItemInfo::GetPackageName(name);
}

ECode CApplicationInfo::SetPackageName(
    /* [in] */ const String& name)
{
    return PackageItemInfo::SetPackageName(name);
}

ECode CApplicationInfo::GetLabelRes(
    /* [out] */ Int32* labelRes)
{
    VALIDATE_NOT_NULL(labelRes);
    return PackageItemInfo::GetLabelRes(labelRes);
}

ECode CApplicationInfo::SetLabelRes(
    /* [in] */ Int32 labelRes)
{
    return PackageItemInfo::SetLabelRes(labelRes);
}

ECode CApplicationInfo::GetNonLocalizedLabel(
    /* [out] */ ICharSequence** label)
{
    VALIDATE_NOT_NULL(label);
    return PackageItemInfo::GetNonLocalizedLabel(label);
}

ECode CApplicationInfo::SetNonLocalizedLabel(
    /* [in] */ ICharSequence* label)
{
    return PackageItemInfo::SetNonLocalizedLabel(label);
}

ECode CApplicationInfo::GetIcon(
    /* [out] */ Int32* icon)
{
    VALIDATE_NOT_NULL(icon);
    return PackageItemInfo::GetIcon(icon);
}

ECode CApplicationInfo::SetIcon(
    /* [in] */ Int32 icon)
{
    return PackageItemInfo::SetIcon(icon);
}

ECode CApplicationInfo::GetLogo(
    /* [out] */ Int32* logo)
{
    VALIDATE_NOT_NULL(logo);
    return PackageItemInfo::GetLogo(logo);
}

ECode CApplicationInfo::SetLogo(
    /* [in] */ Int32 logo)
{
    return PackageItemInfo::SetLogo(logo);
}

ECode CApplicationInfo::GetMetaData(
    /* [out] */ IBundle** metaData)
{
    VALIDATE_NOT_NULL(metaData);
    return PackageItemInfo::GetMetaData(metaData);
}

ECode CApplicationInfo::SetMetaData(
    /* [in] */ IBundle* metaData)
{
    VALIDATE_NOT_NULL(metaData);
    return PackageItemInfo::SetMetaData(metaData);
}

ECode CApplicationInfo::GetTaskAffinity(
    /* [out] */ String* task)
{
    VALIDATE_NOT_NULL(task);
    *task = mTaskAffinity;
    return NOERROR;
}

ECode CApplicationInfo::SetTaskAffinity(
    /* [in] */ const String& task)
{
    mTaskAffinity = task;
    return NOERROR;
}

ECode CApplicationInfo::GetPermission(
    /* [out] */ String* permission)
{
    VALIDATE_NOT_NULL(permission);
    *permission = mPermission;
    return NOERROR;
}

ECode CApplicationInfo::SetPermission(
    /* [in] */ const String& permission)
{
    mPermission = permission;
    return NOERROR;
}

ECode CApplicationInfo::GetProcessName(
    /* [out] */ String* name)
{
    VALIDATE_NOT_NULL(name);
    *name = mProcessName;
    return NOERROR;
}

ECode CApplicationInfo::SetProcessName(
    /* [in] */ const String& name)
{
    mProcessName = name;
    return NOERROR;
}

ECode CApplicationInfo::GetClassName(
    /* [out] */ String* name)
{
    VALIDATE_NOT_NULL(name);
    *name = mClassName;
    return NOERROR;
}

ECode CApplicationInfo::SetClassName(
    /* [in] */ const String& name)
{
    mClassName = name;
    return NOERROR;
}

ECode CApplicationInfo::GetDescriptionRes(
    /* [out] */ Int32* desRes)
{
    VALIDATE_NOT_NULL(desRes);
    *desRes = mDescriptionRes;
    return NOERROR;
}

ECode CApplicationInfo::SetDescriptionRes(
    /* [in] */ Int32 desRes)
{
    mDescriptionRes = desRes;
    return NOERROR;
}

ECode CApplicationInfo::GetTheme(
    /* [out] */ Int32* theme)
{
    VALIDATE_NOT_NULL(theme);
    *theme = mTheme;
    return NOERROR;
}

ECode CApplicationInfo::SetTheme(
    /* [in] */ Int32 theme)
{
    mTheme = theme;
    return NOERROR;
}

ECode CApplicationInfo::GetManageSpaceActivityName(
    /* [out] */ String* name)
{
    VALIDATE_NOT_NULL(name);
    *name = mManageSpaceActivityName;
    return NOERROR;
}

ECode CApplicationInfo::SetManageSpaceActivityName(
    /* [in] */ const String& name)
{
    mManageSpaceActivityName = name;
    return NOERROR;
}

ECode CApplicationInfo::GetBackupAgentName(
    /* [out] */ String* name)
{
    VALIDATE_NOT_NULL(name);
    *name = mBackupAgentName;
    return NOERROR;
}

ECode CApplicationInfo::SetBackupAgentName(
    /* [in] */ const String& name)
{
    mBackupAgentName = name;
    return NOERROR;
}

ECode CApplicationInfo::GetUiOptions(
    /* [out] */ Int32* uiOptions)
{
    VALIDATE_NOT_NULL(uiOptions);
    *uiOptions = mUiOptions;
    return NOERROR;
}

ECode CApplicationInfo::SetUiOptions(
    /* [in] */ Int32 uiOptions)
{
    mUiOptions = uiOptions;
    return NOERROR;
}

ECode CApplicationInfo::GetFlags(
    /* [out] */ Int32* flags)
{
    VALIDATE_NOT_NULL(flags);
    *flags = mFlags;
    return NOERROR;
}

ECode CApplicationInfo::SetFlags(
    /* [in] */ Int32 flags)
{
    mFlags = flags;
    return NOERROR;
}

ECode CApplicationInfo::GetRequiresSmallestWidthDp(
    /* [out] */ Int32* requiresSmallestWidthDp)
{
    VALIDATE_NOT_NULL(requiresSmallestWidthDp);
    *requiresSmallestWidthDp = mRequiresSmallestWidthDp;
    return NOERROR;
}

ECode CApplicationInfo::SetRequiresSmallestWidthDp(
    /* [in] */ Int32 requiresSmallestWidthDp)
{
    mRequiresSmallestWidthDp = requiresSmallestWidthDp;
    return NOERROR;
}

ECode CApplicationInfo::GetCompatibleWidthLimitDp(
    /* [out] */ Int32* compatibleWidthLimitDp)
{
    VALIDATE_NOT_NULL(compatibleWidthLimitDp);
    *compatibleWidthLimitDp = mCompatibleWidthLimitDp;
    return NOERROR;
}

ECode CApplicationInfo::SetCompatibleWidthLimitDp(
    /* [in] */ Int32 compatibleWidthLimitDp)
{
    mCompatibleWidthLimitDp = compatibleWidthLimitDp;
    return NOERROR;
}

ECode CApplicationInfo::GetLargestWidthLimitDp(
    /* [out] */ Int32* largestWidthLimitDp)
{
    VALIDATE_NOT_NULL(largestWidthLimitDp);
    *largestWidthLimitDp = mLargestWidthLimitDp;
    return NOERROR;
}

ECode CApplicationInfo::SetLargestWidthLimitDp(
    /* [in] */ Int32 largestWidthLimitDp)
{
    mLargestWidthLimitDp = largestWidthLimitDp;
    return NOERROR;
}

ECode CApplicationInfo::GetSourceDir(
    /* [out] */ String* sourceDir)
{
    VALIDATE_NOT_NULL(sourceDir);
    *sourceDir = mSourceDir;
    return NOERROR;
}

ECode CApplicationInfo::SetSourceDir(
    /* [in] */ const String& sourceDir)
{
    mSourceDir = sourceDir;
    return NOERROR;
}

ECode CApplicationInfo::GetPublicSourceDir(
    /* [out] */ String* publicSourceDir)
{
    VALIDATE_NOT_NULL(publicSourceDir);
    *publicSourceDir = mPublicSourceDir;
    return NOERROR;
}

ECode CApplicationInfo::SetPublicSourceDir(
    /* [in] */ const String& pubicSourceDir)
{
    mPublicSourceDir = pubicSourceDir;
    return NOERROR;
}

ECode CApplicationInfo::GetResourceDirs(
    /* [out, callee] */ ArrayOf<String>** resourceDirs)
{
    VALIDATE_NOT_NULL(resourceDirs);
    *resourceDirs = mResourceDirs;
    INTERFACE_ADDREF(*resourceDirs);
    return NOERROR;
}

ECode CApplicationInfo::SetResourceDirs(
    /* [in] */ ArrayOf<String>* resourceDirs)
{
    mResourceDirs = resourceDirs;
    return NOERROR;
}

ECode CApplicationInfo::GetSharedLibraryFiles(
    /* [out, callee] */ ArrayOf<String>** sharedLibraryFiles)
{
    VALIDATE_NOT_NULL(sharedLibraryFiles);
    *sharedLibraryFiles = mSharedLibraryFiles;
    INTERFACE_ADDREF(*sharedLibraryFiles);
    return NOERROR;
}

ECode CApplicationInfo::SetSharedLibraryFiles(
    /* [in] */ ArrayOf<String>* sharedLibraryFiles)
{
    mSharedLibraryFiles = sharedLibraryFiles;
    return NOERROR;
}

ECode CApplicationInfo::GetDataDir(
    /* [out] */ String* dataDir)
{
    VALIDATE_NOT_NULL(dataDir);
    *dataDir = mDataDir;
    return NOERROR;
}

ECode CApplicationInfo::SetDataDir(
    /* [in] */ const String& dataDir)
{
    mDataDir = dataDir;
    return NOERROR;
}

ECode CApplicationInfo::GetNativeLibraryDir(
    /* [out] */ String* libraryDir)
{
    VALIDATE_NOT_NULL(libraryDir);
    *libraryDir = mNativeLibraryDir;
    return NOERROR;
}

ECode CApplicationInfo::SetNativeLibraryDir(
    /* [in] */ const String& libraryDir)
{
    mNativeLibraryDir = libraryDir;
    return NOERROR;
}

ECode CApplicationInfo::GetUid(
    /* [out] */ Int32* uid)
{
    VALIDATE_NOT_NULL(uid);
    *uid = mUid;
    return NOERROR;
}

ECode CApplicationInfo::SetUid(
    /* [in] */ Int32 uid)
{
    mUid = uid;
    return NOERROR;
}

ECode CApplicationInfo::GetTargetSdkVersion(
    /* [out] */ Int32* sdkVersion)
{
    VALIDATE_NOT_NULL(sdkVersion);
    *sdkVersion = mTargetSdkVersion;
    return NOERROR;
}

ECode CApplicationInfo::SetTargetSdkVersion(
    /* [in] */ Int32 sdkVersion)
{
    mTargetSdkVersion = sdkVersion;
    return NOERROR;
}

ECode CApplicationInfo::GetEnabled(
    /* [out] */ Boolean* enabled)
{
    VALIDATE_NOT_NULL(enabled);
    *enabled = mEnabled;
    return NOERROR;
}

ECode CApplicationInfo::SetEnabled(
    /* [in] */ Boolean enabled)
{
    mEnabled = enabled;
    return NOERROR;
}

ECode CApplicationInfo::GetEnabledSetting(
    /* [out] */ Int32* enabledSetting)
{
    VALIDATE_NOT_NULL(enabledSetting);
    *enabledSetting = mEnabledSetting;
    return NOERROR;
}

ECode CApplicationInfo::SetEnabledSetting(
    /* [in] */ Int32 enabledSetting)
{
    mEnabledSetting = enabledSetting;
    return NOERROR;
}

ECode CApplicationInfo::GetInstallLocation(
    /* [out] */ Int32* location)
{
    VALIDATE_NOT_NULL(location);
    *location = mInstallLocation;
    return NOERROR;
}

ECode CApplicationInfo::SetInstallLocation(
    /* [in] */ Int32 location)
{
    mInstallLocation = location;
    return NOERROR;
}

} // namespace Pm
} // namespace Content
} // namespace Droid
} // namespace Elastos
