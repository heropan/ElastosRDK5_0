
#include "ext/frameworkext.h"
#include "content/pm/CServiceInfo.h"

namespace Elastos {
namespace Droid {
namespace Content {
namespace Pm {

CServiceInfo::CServiceInfo()
    : mFlags(0)
{}

CServiceInfo::~CServiceInfo()
{}

ECode CServiceInfo::constructor()
{
    return ComponentInfo::constructor();
}

ECode CServiceInfo::constructor(
    /* [in] */ IServiceInfo* orig)
{
    assert(orig != NULL);
    ComponentInfo::constructor((IComponentInfo*)orig);
    orig->GetPermission(&mPermission);
    orig->GetFlags(&mFlags);
    return NOERROR;
}

ECode CServiceInfo::Dump(
    /* [in] */ IPrinter* pw,
    /* [in] */ const String& prefix)
{
    // super.dumpFront(pw, prefix);
    // pw.println(prefix + "permission=" + permission);
    // pw.println(prefix + "flags=0x" + Integer.toHexString(flags));
    assert(0);
    return E_NOT_IMPLEMENTED;
}

ECode CServiceInfo::ToString(
    /* [out] */ String* str)
{
    VALIDATE_NOT_NULL(str);
    // return "ServiceInfo{"
    //         + Integer.toHexString(System.identityHashCode(this))
    //         + " " + name + "}";
    assert(0);
    return E_NOT_IMPLEMENTED;
}

ECode CServiceInfo::ReadFromParcel(
    /* [in] */ IParcel* source)
{
    ComponentInfo::ReadFromParcel(source);
    source->ReadString(&mPermission);
    source->ReadInt32(&mFlags);
    return NOERROR;
}

ECode CServiceInfo::WriteToParcel(
    /* [in] */ IParcel* dest)
{
    ComponentInfo::WriteToParcel(dest);
    dest->WriteString(mPermission);
    dest->WriteInt32(mFlags);
    return NOERROR;
}

ECode CServiceInfo::LoadLabel(
    /* [in] */ IPackageManager* pm,
    /* [out] */ ICharSequence** label)
{
    VALIDATE_NOT_NULL(label);
    return ComponentInfo::LoadLabel(pm, label);
}

ECode CServiceInfo::LoadIcon(
    /* [in] */ IPackageManager* pm,
    /* [out] */ IDrawable** icon)
{
    VALIDATE_NOT_NULL(icon);
    return ComponentInfo::LoadIcon(pm, icon);
}

ECode CServiceInfo::LoadLogo(
    /* [in] */ IPackageManager* pm,
    /* [out] */ IDrawable** logo)
{
    VALIDATE_NOT_NULL(logo);
    return ComponentInfo::LoadLogo(pm, logo);;
}

ECode CServiceInfo::LoadXmlMetaData(
    /* [in] */ IPackageManager* pm,
    /* [in] */ const String& name,
    /* [out] */ IXmlResourceParser** resource)
{
    VALIDATE_NOT_NULL(resource);
    return ComponentInfo::LoadXmlMetaData(pm, name, resource);
}

ECode CServiceInfo::GetName(
    /* [out] */ String* name)
{
    VALIDATE_NOT_NULL(name);
    return ComponentInfo::GetName(name);
}

ECode CServiceInfo::SetName(
    /* [in] */ const String& name)
{
    return ComponentInfo::SetName(name);
}

ECode CServiceInfo::GetPackageName(
    /* [out] */ String* name)
{
    VALIDATE_NOT_NULL(name);
    return ComponentInfo::GetPackageName(name);
}

ECode CServiceInfo::SetPackageName(
    /* [in] */ const String& name)
{
    return ComponentInfo::SetPackageName(name);
}

ECode CServiceInfo::GetLabelRes(
    /* [out] */ Int32* labelRes)
{
    VALIDATE_NOT_NULL(labelRes);
    return ComponentInfo::GetLabelRes(labelRes);
}

ECode CServiceInfo::SetLabelRes(
    /* [in] */ Int32 labelRes)
{
    return ComponentInfo::SetLabelRes(labelRes);
}

ECode CServiceInfo::GetNonLocalizedLabel(
    /* [out] */ ICharSequence** label)
{
    VALIDATE_NOT_NULL(label);
    return ComponentInfo::GetNonLocalizedLabel(label);
}

ECode CServiceInfo::SetNonLocalizedLabel(
    /* [in] */ ICharSequence* label)
{
    return ComponentInfo::SetNonLocalizedLabel(label);
}

ECode CServiceInfo::GetIcon(
    /* [out] */ Int32* icon)
{
    VALIDATE_NOT_NULL(icon);
    return ComponentInfo::GetIcon(icon);
}

ECode CServiceInfo::SetIcon(
    /* [in] */ Int32 icon)
{
    return ComponentInfo::SetIcon(icon);
}

ECode CServiceInfo::GetLogo(
    /* [out] */ Int32* logo)
{
    VALIDATE_NOT_NULL(logo);
    return ComponentInfo::GetLogo(logo);
}

ECode CServiceInfo::SetLogo(
    /* [in] */ Int32 logo)
{
    return ComponentInfo::SetLogo(logo);
}

ECode CServiceInfo::GetMetaData(
    /* [out] */ IBundle** metaData)
{
    VALIDATE_NOT_NULL(metaData);
    return ComponentInfo::GetMetaData(metaData);
}

ECode CServiceInfo::SetMetaData(
    /* [in] */ IBundle* metaData)
{
    return ComponentInfo::SetMetaData(metaData);
}

ECode CServiceInfo::IsEnabled(
    /* [out] */ Boolean* isEnable)
{
    VALIDATE_NOT_NULL(isEnable);
    return ComponentInfo::IsEnabled(isEnable);
}

ECode CServiceInfo::GetIconResource(
    /* [out] */ Int32* icon)
{
    VALIDATE_NOT_NULL(icon);
    return ComponentInfo::GetIconResource(icon);
}

ECode CServiceInfo::GetApplicationInfo(
    /* [out] */ IApplicationInfo** info)
{
    VALIDATE_NOT_NULL(info);
    return ComponentInfo::GetApplicationInfo(info);
}

ECode CServiceInfo::SetApplicationInfo(
    /* [in] */ IApplicationInfo* info)
{
    return ComponentInfo::SetApplicationInfo(info);
}

ECode CServiceInfo::GetProcessName(
    /* [out] */ String* processName)
{
    VALIDATE_NOT_NULL(processName);
    return ComponentInfo::GetProcessName(processName);
}

ECode CServiceInfo::SetProcessName(
    /* [in] */ const String& processName)
{
    return ComponentInfo::SetProcessName(processName);
}

ECode CServiceInfo::GetDescriptionRes(
    /* [out] */ Int32* desRes)
{
    VALIDATE_NOT_NULL(desRes);
    return ComponentInfo::GetDescriptionRes(desRes);
}

ECode CServiceInfo::SetDescriptionRes(
    /* [in] */ Int32 desRes)
{
    return ComponentInfo::SetDescriptionRes(desRes);
}

ECode CServiceInfo::GetEnabled(
    /* [out] */ Boolean* enabled)
{
    VALIDATE_NOT_NULL(enabled);
    return ComponentInfo::GetEnabled(enabled);
}

ECode CServiceInfo::SetEnabled(
    /* [in] */ Boolean enabled)
{
    return ComponentInfo::SetEnabled(enabled);
}

ECode CServiceInfo::GetExported(
    /* [out] */ Boolean* exported)
{
    VALIDATE_NOT_NULL(exported);
    return ComponentInfo::GetExported(exported);
}

ECode CServiceInfo::SetExported(
    /* [in] */ Boolean exported)
{
    return ComponentInfo::SetExported(exported);
}

ECode CServiceInfo::GetPermission(
    /* [out] */ String* permission)
{
    VALIDATE_NOT_NULL(permission);
    *permission = mPermission;
    return NOERROR;
}

ECode CServiceInfo::SetPermission(
    /* [in] */ const String& permission)
{
    mPermission = permission;
    return NOERROR;
}

ECode CServiceInfo::GetFlags(
    /* [out] */ Int32* flags)
{
    VALIDATE_NOT_NULL(flags);
    *flags = mFlags;
    return NOERROR;
}

ECode CServiceInfo::SetFlags(
    /* [in] */ Int32 flags)
{
    mFlags = flags;
    return NOERROR;
}

} // namespace Pm
} // namespace Content
} // namespace Droid
} // namespace Elastos
