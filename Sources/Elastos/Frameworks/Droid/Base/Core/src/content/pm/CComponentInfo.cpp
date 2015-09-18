
#include "ext/frameworkext.h"
#include "content/pm/CComponentInfo.h"

namespace Elastos {
namespace Droid {
namespace Content {
namespace Pm {

CAR_OBJECT_IMPL(CComponentInfo)

CComponentInfo::CComponentInfo()
{}

CComponentInfo::~CComponentInfo()
{}

ECode CComponentInfo::constructor()
{
    return ComponentInfo::constructor();
}

ECode CComponentInfo::constructor(
    /* [in] */ IComponentInfo* orig)
{
    return ComponentInfo::constructor(orig);
}

ECode CComponentInfo::LoadLabel(
    /* [in] */ IPackageManager* pm,
    /* [out] */ ICharSequence** label)
{
    VALIDATE_NOT_NULL(label);
    return ComponentInfo::LoadLabel(pm, label);
}

ECode CComponentInfo::IsEnabled(
    /* [out] */ Boolean* isEnable)
{
    VALIDATE_NOT_NULL(isEnable);
    return ComponentInfo::IsEnabled(isEnable);
}

ECode CComponentInfo::GetIconResource(
    /* [out] */ Int32* icon)
{
    VALIDATE_NOT_NULL(icon);
    return ComponentInfo::GetIconResource(icon);
}

ECode CComponentInfo::WriteToParcel(
    /* [in] */ IParcel* dest)
{
    return ComponentInfo::WriteToParcel(dest);
}

ECode CComponentInfo::ReadFromParcel(
    /* [in] */ IParcel* src)
{
    return ComponentInfo::ReadFromParcel(src);
}

ECode CComponentInfo::LoadIcon(
    /* [in] */ IPackageManager* pm,
    /* [out] */ IDrawable** icon)
{
    VALIDATE_NOT_NULL(icon);
    return PackageItemInfo::LoadIcon(pm, icon);
}

ECode CComponentInfo::LoadLogo(
    /* [in] */ IPackageManager* pm,
    /* [out] */ IDrawable** logo)
{
    VALIDATE_NOT_NULL(logo);
    return PackageItemInfo::LoadLogo(pm, logo);
}

ECode CComponentInfo::LoadXmlMetaData(
    /* [in] */ IPackageManager* pm,
    /* [in] */ const String& name,
    /* [out] */ IXmlResourceParser** resource)
{
    VALIDATE_NOT_NULL(resource);
    return PackageItemInfo::LoadXmlMetaData(pm, name, resource);
}

ECode CComponentInfo::GetName(
    /* [out] */ String* name)
{
    VALIDATE_NOT_NULL(name);
    return ComponentInfo::GetName(name);
}

ECode CComponentInfo::SetName(
    /* [in] */ const String& name)
{
    return ComponentInfo::SetName(name);
}

ECode CComponentInfo::GetPackageName(
    /* [out] */ String* name)
{
    VALIDATE_NOT_NULL(name);
    return ComponentInfo::GetPackageName(name);
}

ECode CComponentInfo::SetPackageName(
    /* [in] */ const String& name)
{
    return ComponentInfo::SetPackageName(name);
}

ECode CComponentInfo::GetLabelRes(
    /* [out] */ Int32* labelRes)
{
    VALIDATE_NOT_NULL(labelRes);
    return ComponentInfo::GetLabelRes(labelRes);
}

ECode CComponentInfo::SetLabelRes(
    /* [in] */ Int32 labelRes)
{
    return ComponentInfo::SetLabelRes(labelRes);
}

ECode CComponentInfo::GetNonLocalizedLabel(
    /* [out] */ ICharSequence** label)
{
    VALIDATE_NOT_NULL(label);
    return ComponentInfo::GetNonLocalizedLabel(label);
}

ECode CComponentInfo::SetNonLocalizedLabel(
    /* [in] */ ICharSequence* label)
{
    return ComponentInfo::SetNonLocalizedLabel(label);
}

ECode CComponentInfo::GetIcon(
    /* [out] */ Int32* icon)
{
    VALIDATE_NOT_NULL(icon);
    return ComponentInfo::GetIcon(icon);
}

ECode CComponentInfo::SetIcon(
    /* [in] */ Int32 icon)
{
    return ComponentInfo::SetIcon(icon);
}

ECode CComponentInfo::GetLogo(
    /* [out] */ Int32* logo)
{
    VALIDATE_NOT_NULL(logo);
    return ComponentInfo::GetLogo(logo);
}

ECode CComponentInfo::SetLogo(
    /* [in] */ Int32 logo)
{
    return ComponentInfo::SetLogo(logo);
}

ECode CComponentInfo::GetMetaData(
    /* [out] */ IBundle** metaData)
{
    VALIDATE_NOT_NULL(metaData);
    return ComponentInfo::GetMetaData(metaData);
}

ECode CComponentInfo::SetMetaData(
    /* [in] */ IBundle* metaData)
{
    return ComponentInfo::SetMetaData(metaData);
}

ECode CComponentInfo::GetApplicationInfo(
    /* [out] */ IApplicationInfo** info)
{
    VALIDATE_NOT_NULL(info);
    return ComponentInfo::GetApplicationInfo(info);
}

ECode CComponentInfo::SetApplicationInfo(
    /* [in] */ IApplicationInfo* info)
{
    return ComponentInfo::SetApplicationInfo(info);
}

ECode CComponentInfo::GetProcessName(
    /* [out] */ String* processName)
{
    VALIDATE_NOT_NULL(processName);
    return ComponentInfo::GetProcessName(processName);
}

ECode CComponentInfo::SetProcessName(
    /* [in] */ const String& processName)
{
    return ComponentInfo::SetProcessName(processName);
}

ECode CComponentInfo::GetDescriptionRes(
    /* [out] */ Int32* desRes)
{
    VALIDATE_NOT_NULL(desRes);
    return ComponentInfo::GetDescriptionRes(desRes);
}

ECode CComponentInfo::SetDescriptionRes(
    /* [in] */ Int32 desRes)
{
    return ComponentInfo::SetDescriptionRes(desRes);
}

ECode CComponentInfo::GetEnabled(
    /* [out] */ Boolean* enabled)
{
    VALIDATE_NOT_NULL(enabled);
    return ComponentInfo::GetEnabled(enabled);
}

ECode CComponentInfo::SetEnabled(
    /* [in] */ Boolean enabled)
{
    return ComponentInfo::SetEnabled(enabled);
}

ECode CComponentInfo::GetExported(
    /* [out] */ Boolean* exported)
{
    VALIDATE_NOT_NULL(exported);
    return ComponentInfo::GetExported(exported);
}

ECode CComponentInfo::SetExported(
    /* [in] */ Boolean exported)
{
    return ComponentInfo::SetExported(exported);
}

} // namespace Pm
} // namespace Content
} // namespace Droid
} // namespace Elastos
