
#include "ext/frameworkext.h"
#include "content/pm/CActivityInfo.h"
#include "os/Build.h"
#include <elastos/core/StringUtils.h>

using Elastos::Core::StringUtils;
using Elastos::Droid::Os::Build;

namespace Elastos {
namespace Droid {
namespace Content {
namespace Pm {

static AutoPtr< ArrayOf<Int32> > Init_CONFIG_NATIVE_BITS()
{
    AutoPtr< ArrayOf<Int32> > int32Array = ArrayOf<Int32>::Alloc(14);
    (*int32Array)[0] = CConfiguration::NATIVE_CONFIG_MNC;//,                    // MNC
    (*int32Array)[1] = CConfiguration::NATIVE_CONFIG_MCC;//,                    // MCC
    (*int32Array)[2] = CConfiguration::NATIVE_CONFIG_LOCALE;//,                 // LOCALE
    (*int32Array)[3] = CConfiguration::NATIVE_CONFIG_TOUCHSCREEN;//,            // TOUCH SCREEN
    (*int32Array)[4] = CConfiguration::NATIVE_CONFIG_KEYBOARD;//,               // KEYBOARD
    (*int32Array)[5] = CConfiguration::NATIVE_CONFIG_KEYBOARD_HIDDEN;//,        // KEYBOARD HIDDEN
    (*int32Array)[6] = CConfiguration::NATIVE_CONFIG_NAVIGATION;//,             // NAVIGATION
    (*int32Array)[7] = CConfiguration::NATIVE_CONFIG_ORIENTATION;//,            // ORIENTATION
    (*int32Array)[8] = CConfiguration::NATIVE_CONFIG_SCREEN_LAYOUT;//,          // SCREEN LAYOUT
    (*int32Array)[9] = CConfiguration::NATIVE_CONFIG_UI_MODE;//,                // UI MODE
    (*int32Array)[10] = CConfiguration::NATIVE_CONFIG_SCREEN_SIZE;//,            // SCREEN SIZE
    (*int32Array)[11] = CConfiguration::NATIVE_CONFIG_SMALLEST_SCREEN_SIZE,   // SMALLEST SCREEN SIZE
    (*int32Array)[12] = CConfiguration::NATIVE_CONFIG_DENSITY;//,                // DENSITY
    (*int32Array)[13] = CConfiguration::NATIVE_CONFIG_LAYOUTDIR;//,              // LAYOUT DIRECTION
    return int32Array;
}

AutoPtr< ArrayOf<Int32> > CActivityInfo::CONFIG_NATIVE_BITS = Init_CONFIG_NATIVE_BITS();

CAR_INTERFACE_IMPL(CActivityInfo, ComponentInfo, IActivityInfo)

CAR_OBJECT_IMPL(CActivityInfo)

CActivityInfo::CActivityInfo()
    : mTheme(0)
    , mLaunchMode(0)
    , mFlags(0)
    , mScreenOrientation(SCREEN_ORIENTATION_UNSPECIFIED)
    , mConfigChanges(0)
    , mSoftInputMode(0)
    , mUiOptions(0)
    , mPersistableMode(0)
    , mMaxRecents(0)
{}

CActivityInfo::~CActivityInfo()
{}

ECode CActivityInfo::constructor()
{
    return ComponentInfo::constructor();
}

ECode CActivityInfo::constructor(
    /* [in] */ IActivityInfo* orig)
{
    assert(orig != NULL);

    ComponentInfo::constructor((IComponentInfo*)orig);
    orig->GetTheme(&mTheme);
    orig->GetLaunchMode(&mLaunchMode);
    orig->GetPermission(&mPermission);
    orig->GetTaskAffinity(&mTaskAffinity);
    orig->GetTargetActivity(&mTargetActivity);
    orig->GetFlags(&mFlags);
    orig->GetScreenOrientation(&mScreenOrientation);
    orig->GetConfigChanges(&mConfigChanges);
    orig->GetSoftInputMode(&mSoftInputMode);
    orig->GetUiOptions(&mUiOptions);
    orig->GetParentActivityName(&mParentActivityName);
    orig->GetMaxRecents(&mMaxRecents);
    return NOERROR;
}

Int32 CActivityInfo::ActivityInfoConfigToNative(
    /* [in] */ Int32 input)
{
    Int32 output = 0;
    for (Int32 i = 0; i < CONFIG_NATIVE_BITS->GetLength(); i++) {
        if ((input & (1 << i)) != 0) {
            output |= (*CONFIG_NATIVE_BITS)[i];
        }
    }
    return output;
}

ECode CActivityInfo::GetRealConfigChanged(
    /* [out] */ Int32* value)
{
    VALIDATE_NOT_NULL(value);

    Int32 targetSdkVersion = 0;
    mApplicationInfo->GetTargetSdkVersion(&targetSdkVersion);
    if(targetSdkVersion < Build::VERSION_CODES::HONEYCOMB_MR2) {
        *value = (mConfigChanges | IActivityInfo::CONFIG_SCREEN_SIZE
                        | IActivityInfo::CONFIG_SMALLEST_SCREEN_SIZE);
    }
    else {
        *value = mConfigChanges;
    }

    return NOERROR;
}

ECode CActivityInfo::GetThemeResource(
    /* [out] */ Int32* theme)
{
    VALIDATE_NOT_NULL(theme);

    if (mTheme !=0) {
        *theme = mTheme;
    }
    else {
        mApplicationInfo->GetTheme(theme);
    }
    return NOERROR;
}

String CActivityInfo::PersistableModeToString()
{
    switch(mPersistableMode) {
        case PERSIST_ROOT_ONLY: return String("PERSIST_ROOT_ONLY");
        case PERSIST_NEVER: return String("PERSIST_NEVER");
        case PERSIST_ACROSS_REBOOTS: return String("PERSIST_ACROSS_REBOOTS");
        default: {
            StringBuilder sb("UNKNOWN");
            sb += mPersistableMode;
            return sb.ToString();
        }
    }
}

ECode CActivityInfo::Dump(
    /* [in] */ IPrinter* pw,
    /* [in] */ const String& prefix)
{
    // super.dumpFront(pw, prefix);
    // if (permission != null) {
    //     pw.println(prefix + "permission=" + permission);
    // }
    // pw.println(prefix + "taskAffinity=" + taskAffinity
    //         + " targetActivity=" + targetActivity);
    // if (launchMode != 0 || flags != 0 || theme != 0) {
    //     pw.println(prefix + "launchMode=" + launchMode
    //             + " flags=0x" + Integer.toHexString(flags)
    //             + " theme=0x" + Integer.toHexString(theme));
    // }
    // if (screenOrientation != SCREEN_ORIENTATION_UNSPECIFIED
    //         || configChanges != 0 || softInputMode != 0) {
    //     pw.println(prefix + "screenOrientation=" + screenOrientation
    //             + " configChanges=0x" + Integer.toHexString(configChanges)
    //             + " softInputMode=0x" + Integer.toHexString(softInputMode));
    // }
    // if (uiOptions != 0) {
    //     pw.println(prefix + " uiOptions=0x" + Integer.toHexString(uiOptions));
    // }
    // super.dumpBack(pw, prefix);
    assert(0);
    return E_NOT_IMPLEMENTED;
}

ECode CActivityInfo::ToString(
    /* [out] */ String* str)
{
    VALIDATE_NOT_NULL(str)
    Int32 hashCode;
    THIS_PROBE(IObject)->GetHashCode(&hashCode);
    str->AppendFormat("ActivityInfo{%s %s}", StringUtils::Int32ToHexString(hashCode).string(), mName.string());
    return NOERROR;
}

ECode CActivityInfo::ReadFromParcel(
    /* [in] */ IParcel* source)
{
    assert(source != NULL);

    ComponentInfo::ReadFromParcel(source);
    source->ReadInt32(&mTheme);
    source->ReadInt32(&mLaunchMode);
    source->ReadString(&mPermission);
    source->ReadString(&mTaskAffinity);
    source->ReadString(&mTargetActivity);
    source->ReadInt32(&mFlags);
    source->ReadInt32(&mScreenOrientation);
    source->ReadInt32(&mConfigChanges);
    source->ReadInt32(&mSoftInputMode);
    source->ReadInt32(&mUiOptions);
    source->ReadString(&mParentActivityName);
    source->ReadInt32(&mPersistableMode);
    source->ReadInt32(&mMaxRecents);
    return NOERROR;
}

ECode CActivityInfo::WriteToParcel(
    /* [in] */ IParcel* dest)
{
    ComponentInfo::WriteToParcel(dest);
    dest->WriteInt32(mTheme);
    dest->WriteInt32(mLaunchMode);
    dest->WriteString(mPermission);
    dest->WriteString(mTaskAffinity);
    dest->WriteString(mTargetActivity);
    dest->WriteInt32(mFlags);
    dest->WriteInt32(mScreenOrientation);
    dest->WriteInt32(mConfigChanges);
    dest->WriteInt32(mSoftInputMode);
    dest->WriteInt32(mUiOptions);
    dest->WriteString(mParentActivityName);
    dest->WriteInt32(mPersistableMode);
    dest->WriteInt32(mMaxRecents);
    return NOERROR;
}

ECode CActivityInfo::LoadLabel(
    /* [in] */ IPackageManager* pm,
    /* [out] */ ICharSequence** label)
{
    VALIDATE_NOT_NULL(label);
    return ComponentInfo::LoadLabel(pm, label);
}

ECode CActivityInfo::LoadIcon(
    /* [in] */ IPackageManager* pm,
    /* [out] */ IDrawable** icon)
{
    VALIDATE_NOT_NULL(icon);
    return ComponentInfo::LoadIcon(pm, icon);
}

ECode CActivityInfo::LoadLogo(
    /* [in] */ IPackageManager* pm,
    /* [out] */ IDrawable** logo)
{
    VALIDATE_NOT_NULL(logo);
    return ComponentInfo::LoadLogo(pm, logo);;
}

ECode CActivityInfo::LoadXmlMetaData(
    /* [in] */ IPackageManager* pm,
    /* [in] */ const String& name,
    /* [out] */ IXmlResourceParser** resource)
{
    VALIDATE_NOT_NULL(resource);
    return ComponentInfo::LoadXmlMetaData(pm, name, resource);
}

ECode CActivityInfo::GetName(
    /* [out] */ String* name)
{
    VALIDATE_NOT_NULL(name);
    return ComponentInfo::GetName(name);
}

ECode CActivityInfo::SetName(
    /* [in] */ const String& name)
{
    return ComponentInfo::SetName(name);
}

ECode CActivityInfo::GetPackageName(
    /* [out] */ String* name)
{
    VALIDATE_NOT_NULL(name);
    return ComponentInfo::GetPackageName(name);
}

ECode CActivityInfo::SetPackageName(
    /* [in] */ const String& name)
{
    return ComponentInfo::SetPackageName(name);
}

ECode CActivityInfo::GetLabelRes(
    /* [out] */ Int32* labelRes)
{
    VALIDATE_NOT_NULL(labelRes);
    return ComponentInfo::GetLabelRes(labelRes);
}

ECode CActivityInfo::SetLabelRes(
    /* [in] */ Int32 labelRes)
{
    return ComponentInfo::SetLabelRes(labelRes);
}

ECode CActivityInfo::GetNonLocalizedLabel(
    /* [out] */ ICharSequence** label)
{
    VALIDATE_NOT_NULL(label);
    return ComponentInfo::GetNonLocalizedLabel(label);
}

ECode CActivityInfo::SetNonLocalizedLabel(
    /* [in] */ ICharSequence* label)
{
    return ComponentInfo::SetNonLocalizedLabel(label);
}

ECode CActivityInfo::GetIcon(
    /* [out] */ Int32* icon)
{
    VALIDATE_NOT_NULL(icon);
    return ComponentInfo::GetIcon(icon);
}

ECode CActivityInfo::SetIcon(
    /* [in] */ Int32 icon)
{
    return ComponentInfo::SetIcon(icon);
}

ECode CActivityInfo::GetLogo(
    /* [out] */ Int32* logo)
{
    VALIDATE_NOT_NULL(logo);
    return ComponentInfo::GetLogo(logo);
}

ECode CActivityInfo::SetLogo(
    /* [in] */ Int32 logo)
{
    return ComponentInfo::SetLogo(logo);
}

ECode CActivityInfo::GetMetaData(
    /* [out] */ IBundle** metaData)
{
    VALIDATE_NOT_NULL(metaData);
    return ComponentInfo::GetMetaData(metaData);
}

ECode CActivityInfo::SetMetaData(
    /* [in] */ IBundle* metaData)
{
    return ComponentInfo::SetMetaData(metaData);
}

ECode CActivityInfo::IsEnabled(
    /* [out] */ Boolean* isEnable)
{
    VALIDATE_NOT_NULL(isEnable);
    return ComponentInfo::IsEnabled(isEnable);
}

ECode CActivityInfo::GetIconResource(
    /* [out] */ Int32* icon)
{
    VALIDATE_NOT_NULL(icon);
    return ComponentInfo::GetIconResource(icon);
}

ECode CActivityInfo::GetApplicationInfo(
    /* [out] */ IApplicationInfo** info)
{
    VALIDATE_NOT_NULL(info);
    return ComponentInfo::GetApplicationInfo(info);
}

ECode CActivityInfo::SetApplicationInfo(
    /* [in] */ IApplicationInfo* info)
{
    return ComponentInfo::SetApplicationInfo(info);
}

ECode CActivityInfo::GetProcessName(
    /* [out] */ String* processName)
{
    VALIDATE_NOT_NULL(processName);
    return ComponentInfo::GetProcessName(processName);
}

ECode CActivityInfo::SetProcessName(
    /* [in] */ const String& processName)
{
    return ComponentInfo::SetProcessName(processName);
}

ECode CActivityInfo::GetDescriptionRes(
    /* [out] */ Int32* desRes)
{
    VALIDATE_NOT_NULL(desRes);
    return ComponentInfo::GetDescriptionRes(desRes);
}

ECode CActivityInfo::SetDescriptionRes(
    /* [in] */ Int32 desRes)
{
    return ComponentInfo::SetDescriptionRes(desRes);
}

ECode CActivityInfo::GetEnabled(
    /* [out] */ Boolean* enabled)
{
    VALIDATE_NOT_NULL(enabled);
    return ComponentInfo::GetEnabled(enabled);
}

ECode CActivityInfo::SetEnabled(
    /* [in] */ Boolean enabled)
{
    return ComponentInfo::SetEnabled(enabled);
}

ECode CActivityInfo::GetExported(
    /* [out] */ Boolean* exported)
{
    VALIDATE_NOT_NULL(exported);
    return ComponentInfo::GetExported(exported);
}

ECode CActivityInfo::SetExported(
    /* [in] */ Boolean exported)
{
    return ComponentInfo::SetExported(exported);
}

ECode CActivityInfo::GetTheme(
    /* [out] */ Int32* theme)
{
    VALIDATE_NOT_NULL(theme);
    *theme = mTheme;
    return NOERROR;
}

ECode CActivityInfo::SetTheme(
    /* [in] */ Int32 theme)
{
    mTheme = theme;
    return NOERROR;
}

ECode CActivityInfo::GetLaunchMode(
    /* [out] */ Int32* mode)
{
    VALIDATE_NOT_NULL(mode);
    *mode = mLaunchMode;
    return NOERROR;
}

ECode CActivityInfo::SetLaunchMode(
    /* [in] */ Int32 mode)
{
    mLaunchMode = mode;
    return NOERROR;
}

ECode CActivityInfo::GetPermission(
    /* [out] */ String* permission)
{
    VALIDATE_NOT_NULL(permission);
    *permission = mPermission;
    return NOERROR;
}

ECode CActivityInfo::SetPermission(
    /* [in] */ const String& permission)
{
    mPermission = permission;
    return NOERROR;
}

ECode CActivityInfo::GetTaskAffinity(
    /* [out] */ String* task)
{
    VALIDATE_NOT_NULL(task);
    *task = mTaskAffinity;
    return NOERROR;
}

ECode CActivityInfo::SetTaskAffinity(
    /* [in] */ const String& task)
{
    mTaskAffinity = task;
    return NOERROR;
}

ECode CActivityInfo::GetTargetActivity(
    /* [out] */ String* target)
{
    VALIDATE_NOT_NULL(target);
    *target = mTargetActivity;
    return NOERROR;
}

ECode CActivityInfo::SetTargetActivity(
    /* [in] */ const String& target)
{
    mTargetActivity = target;
    return NOERROR;
}

ECode CActivityInfo::GetFlags(
    /* [out] */ Int32* flags)
{
    VALIDATE_NOT_NULL(flags);
    *flags = mFlags;
    return NOERROR;
}

ECode CActivityInfo::SetFlags(
    /* [in] */ Int32 flags)
{
    mFlags = flags;
    return NOERROR;
}

ECode CActivityInfo::GetScreenOrientation(
    /* [out] */ Int32* orientation)
{
    VALIDATE_NOT_NULL(orientation);
    *orientation = mScreenOrientation;
    return NOERROR;
}

ECode CActivityInfo::SetScreenOrientation(
    /* [in] */ Int32 orientation)
{
    mScreenOrientation = orientation;
    return NOERROR;
}

ECode CActivityInfo::GetConfigChanges(
    /* [out] */ Int32* changes)
{
    VALIDATE_NOT_NULL(changes);
    *changes = mConfigChanges;
    return NOERROR;
}

ECode CActivityInfo::SetConfigChanges(
    /* [in] */ Int32 changes)
{
    mConfigChanges = changes;
    return NOERROR;
}

ECode CActivityInfo::GetSoftInputMode(
    /* [out] */ Int32* mode)
{
    VALIDATE_NOT_NULL(mode);
    *mode = mSoftInputMode;
    return NOERROR;
}

ECode CActivityInfo::SetSoftInputMode(
    /* [in] */ Int32 mode)
{
    mSoftInputMode = mode;
    return NOERROR;
}

ECode CActivityInfo::GetUiOptions(
    /* [out] */ Int32* uiOptions)
{
    VALIDATE_NOT_NULL(uiOptions);
    *uiOptions = mUiOptions;
    return NOERROR;
}

ECode CActivityInfo::SetUiOptions(
    /* [in] */ Int32 uiOptions)
{
    mUiOptions = uiOptions;
    return NOERROR;
}

ECode CActivityInfo::GetParentActivityName(
    /* [out] */ String* parentActivityName)
{
    VALIDATE_NOT_NULL(parentActivityName);
    *parentActivityName = mParentActivityName;
    return NOERROR;
}

ECode CActivityInfo::SetParentActivityName(
    /* [in] */ const String& parentActivityName)
{
    mParentActivityName = parentActivityName;
    return NOERROR;
}

} // namespace Pm
} // namespace Content
} // namespace Droid
} // namespace Elastos
