
#include "ext/frameworkext.h"
#include "content/pm/ComponentInfo.h"

using Elastos::Core::CString;

namespace Elastos {
namespace Droid {
namespace Content {
namespace Pm {

ComponentInfo::ComponentInfo()
    : mDescriptionRes(0)
    , mEnabled(TRUE)
    , mExported(FALSE)
{}

ComponentInfo::~ComponentInfo()
{}

ECode ComponentInfo::constructor()
{
    return PackageItemInfo::constructor();
}

ECode ComponentInfo::constructor(
    /* [in] */ IComponentInfo* orig)
{
    assert(orig != NULL);

    PackageItemInfo::constructor((IPackageItemInfo*)orig);
    orig->GetApplicationInfo((IApplicationInfo**)&mApplicationInfo);
    orig->GetProcessName(&mProcessName);
    orig->GetDescriptionRes(&mDescriptionRes);
    orig->GetEnabled(&mEnabled);
    orig->GetExported(&mExported);
    return NOERROR;
}

ECode ComponentInfo::LoadLabel(
    /* [in] */ IPackageManager* pm,
    /* [out] */ ICharSequence** label)
{
    VALIDATE_NOT_NULL(label);
    *label = NULL;

    if (mNonLocalizedLabel != NULL) {
        *label = mNonLocalizedLabel;
        REFCOUNT_ADD(*label);
        return NOERROR;
    }

    AutoPtr<IApplicationInfo> ai = mApplicationInfo;
    if (mLabelRes != 0) {
        pm->GetText(mPackageName, mLabelRes, ai, label);
        if (*label != NULL) {
             return NOERROR;
        }
    }
    AutoPtr<ICharSequence> nonLocalizedLabel;
    ai->GetNonLocalizedLabel((ICharSequence**)&nonLocalizedLabel);
    if (nonLocalizedLabel != NULL) {
        *label = nonLocalizedLabel;
        REFCOUNT_ADD(*label);
        return NOERROR;
    }
    Int32 labelRes;
    ai->GetLabelRes(&labelRes);
    if (labelRes != 0) {
        pm->GetText(mPackageName, labelRes, ai, label);
        if (*label != NULL) {
            return NOERROR;
       }
    }
    return CString::New(mName, label);
}

ECode ComponentInfo::IsEnabled(
    /* [out] */ Boolean* isEnable)
{
    assert(isEnable != NULL);

    Boolean enable = FALSE;
    mApplicationInfo->GetEnabled(&enable);
    *isEnable = mEnabled && enable;
    return NOERROR;
}

ECode ComponentInfo::GetIconResource(
    /* [out] */ Int32* icon)
{
    assert(icon != NULL);

    if(mIcon !=0) {
        *icon = mIcon;
    }
    else {
        mApplicationInfo->GetIcon(icon);
    }
    return NOERROR;
}

ECode ComponentInfo::DumpFront(
    /* [in] */ IPrinter* pw,
    /* [in] */ const String& prefix)
{
    // super.dumpFront(pw, prefix);
    // pw.println(prefix + "enabled=" + enabled + " exported=" + exported
    //         + " processName=" + processName);
    // if (descriptionRes != 0) {
    //     pw.println(prefix + "description=" + descriptionRes);
    // }
    assert(0);
    return E_NOT_IMPLEMENTED;
}

ECode ComponentInfo::DumpBack(
    /* [in] */ IPrinter* pw,
    /* [in] */ const String& prefix)
{
    // if (applicationInfo != null) {
    //     pw.println(prefix + "ApplicationInfo:");
    //     applicationInfo.dump(pw, prefix + "  ");
    // } else {
    //     pw.println(prefix + "ApplicationInfo: null");
    // }
    // super.dumpBack(pw, prefix);
    assert(0);
    return E_NOT_IMPLEMENTED;
}

ECode ComponentInfo::WriteToParcel(
        /* [in] */ IParcel* dest)
{
    assert(dest != NULL);

    PackageItemInfo::WriteToParcel(dest);
    dest->WriteInterfacePtr(mApplicationInfo);
    dest->WriteString(mProcessName);
    dest->WriteInt32(mDescriptionRes);
    dest->WriteBoolean(mEnabled);
    dest->WriteBoolean(mExported);
    return NOERROR;
}

ECode ComponentInfo::ReadFromParcel(
    /* [in] */ IParcel* source)
{
    assert(source != NULL);

    PackageItemInfo::ReadFromParcel(source);
    AutoPtr<IInterface> obj;
    source->ReadInterfacePtr((Handle32*)&obj);
    mApplicationInfo = obj != NULL ? IApplicationInfo::Probe(obj) : NULL;
    source->ReadString(&mProcessName);
    source->ReadInt32(&mDescriptionRes);
    source->ReadBoolean(&mEnabled);
    source->ReadBoolean(&mExported);
    return NOERROR;
}

ECode ComponentInfo::LoadDefaultIcon(
    /* [in] */ IPackageManager* pm,
    /* [out] */ IDrawable** icon)
{
    assert(icon != NULL);
    return mApplicationInfo->LoadIcon(pm, icon);
}

ECode ComponentInfo::LoadDefaultLogo(
    /* [in] */ IPackageManager* pm,
    /* [out] */ IDrawable** logo)
{
    assert(logo != NULL);
    return mApplicationInfo->LoadLogo(pm, logo);
}

AutoPtr<IApplicationInfo> ComponentInfo::GetApplicationInfo()
{
    return mApplicationInfo;
}

ECode ComponentInfo::GetApplicationInfo(
    /* [out] */ IApplicationInfo** info)
{
    assert(info != NULL);
    *info = mApplicationInfo;
    REFCOUNT_ADD(*info);
    return NOERROR;
}

ECode ComponentInfo::SetApplicationInfo(
    /* [in] */ IApplicationInfo* info)
{
    mApplicationInfo = info;
    return NOERROR;
}

ECode ComponentInfo::GetProcessName(
    /* [out] */ String* processName)
{
    assert(processName != NULL);
    *processName = mProcessName;
    return NOERROR;
}

ECode ComponentInfo::SetProcessName(
    /* [in] */ const String& processName)
{
    mProcessName = processName;
    return NOERROR;
}

ECode ComponentInfo::GetDescriptionRes(
    /* [out] */ Int32* desRes)
{
    assert(desRes != NULL);
    *desRes = mDescriptionRes;
    return NOERROR;
}

ECode ComponentInfo::SetDescriptionRes(
    /* [in] */ Int32 desRes)
{
    mDescriptionRes = desRes;
    return NOERROR;
}

ECode ComponentInfo::GetEnabled(
    /* [out] */ Boolean* enabled)
{
    assert(enabled != NULL);
    *enabled = mEnabled;
    return NOERROR;
}

ECode ComponentInfo::SetEnabled(
    /* [in] */ Boolean enabled)
{
    mEnabled = enabled;
    return NOERROR;
}

ECode ComponentInfo::GetExported(
    /* [out] */ Boolean* exported)
{
    assert(exported != NULL);
    *exported = mExported;
    return NOERROR;
}

ECode ComponentInfo::SetExported(
    /* [in] */ Boolean exported)
{
    mExported = exported;
    return NOERROR;
}

} // namespace Pm
} // namespace Content
} // namespace Droid
} // namespace Elastos
