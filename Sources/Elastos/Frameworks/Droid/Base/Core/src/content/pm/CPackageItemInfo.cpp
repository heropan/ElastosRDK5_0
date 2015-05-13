
#include "ext/frameworkext.h"
#include "content/pm/CPackageItemInfo.h"

namespace Elastos {
namespace Droid {
namespace Content {
namespace Pm {

ECode CPackageItemInfo::constructor()
{
    return PackageItemInfo::constructor();
}

ECode CPackageItemInfo::constructor(
    /* [in] */ IPackageItemInfo* orig)
{
    return PackageItemInfo::constructor(orig);
}

ECode CPackageItemInfo::LoadLabel(
    /* [in] */ IPackageManager* pm,
    /* [out] */ ICharSequence** label)
{
    VALIDATE_NOT_NULL(label);
    return PackageItemInfo::LoadLabel(pm, label);
}

ECode CPackageItemInfo::LoadIcon(
    /* [in] */ IPackageManager* pm,
    /* [out] */ IDrawable** icon)
{
    VALIDATE_NOT_NULL(icon);
    return PackageItemInfo::LoadIcon(pm, icon);
}

ECode CPackageItemInfo::LoadLogo(
    /* [in] */ IPackageManager* pm,
    /* [out] */ IDrawable** icon)
{
    VALIDATE_NOT_NULL(icon);
    return PackageItemInfo::LoadLogo(pm, icon);
}

ECode CPackageItemInfo::LoadXmlMetaData(
    /* [in] */ IPackageManager* pm,
    /* [in] */ const String& name,
    /* [out] */ IXmlResourceParser** resource)
{
    VALIDATE_NOT_NULL(resource);
    return  PackageItemInfo::LoadXmlMetaData(pm, name, resource);
}

ECode CPackageItemInfo::WriteToParcel(
        /* [in] */ IParcel* dest)
{
    return PackageItemInfo::WriteToParcel(dest);
}

ECode CPackageItemInfo::GetName(
    /* [out] */ String* name)
{
    VALIDATE_NOT_NULL(name);
    return PackageItemInfo::GetName(name);
}

ECode CPackageItemInfo::SetName(
    /* [in] */ const String& name)
{
    return PackageItemInfo::SetName(name);
}

ECode CPackageItemInfo::GetPackageName(
    /* [out] */ String* name)
{
    VALIDATE_NOT_NULL(name);
    return PackageItemInfo::GetPackageName(name);
}

ECode CPackageItemInfo::SetPackageName(
    /* [in] */ const String& name)
{
    return PackageItemInfo::SetPackageName(name);
}

ECode CPackageItemInfo::GetLabelRes(
    /* [out] */ Int32* labelRes)
{
    VALIDATE_NOT_NULL(labelRes);
    return PackageItemInfo::GetLabelRes(labelRes);
}

ECode CPackageItemInfo::SetLabelRes(
    /* [in] */ Int32 labelRes)
{
    return PackageItemInfo::SetLabelRes(labelRes);
}

ECode CPackageItemInfo::GetNonLocalizedLabel(
    /* [out] */ ICharSequence** label)
{
    VALIDATE_NOT_NULL(label);
    return PackageItemInfo::GetNonLocalizedLabel(label);
}

ECode CPackageItemInfo::SetNonLocalizedLabel(
    /* [in] */ ICharSequence* label)
{
    return PackageItemInfo::SetNonLocalizedLabel(label);
}

ECode CPackageItemInfo::GetIcon(
    /* [out] */ Int32* icon)
{
    VALIDATE_NOT_NULL(icon);
    return PackageItemInfo::GetIcon(icon);
}

ECode CPackageItemInfo::SetIcon(
    /* [in] */ Int32 icon)
{
    return PackageItemInfo::SetIcon(icon);
}

ECode CPackageItemInfo::GetLogo(
    /* [out] */ Int32* logo)
{
    VALIDATE_NOT_NULL(logo);
    return PackageItemInfo::GetLogo(logo);
}

ECode CPackageItemInfo::SetLogo(
    /* [in] */ Int32 logo)
{
    return PackageItemInfo::SetLogo(logo);
}

ECode CPackageItemInfo::GetMetaData(
    /* [out] */ IBundle** metaData)
{
    VALIDATE_NOT_NULL(metaData);
    return PackageItemInfo::GetMetaData(metaData);
}

ECode CPackageItemInfo::SetMetaData(
    /* [in] */ IBundle* metaData)
{
    return PackageItemInfo::SetMetaData(metaData);
}

} // namespace Pm
} // namespace Content
} // namespace Droid
} // namespace Elastos
