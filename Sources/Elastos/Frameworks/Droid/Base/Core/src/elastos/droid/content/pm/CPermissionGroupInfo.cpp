
#include "ext/frameworkext.h"
#include "content/pm/CPermissionGroupInfo.h"

namespace Elastos {
namespace Droid {
namespace Content {
namespace Pm {

CPermissionGroupInfo::CPermissionGroupInfo()
    : mDescriptionRes(0)
    , mFlags(0)
    , mPriority(0)
{}

CPermissionGroupInfo::~CPermissionGroupInfo()
{}

ECode CPermissionGroupInfo::constructor()
{
    return NOERROR;
}

ECode CPermissionGroupInfo::constructor(
    /* [in] */ IPermissionGroupInfo* orig)
{
    assert(orig != NULL);
    PackageItemInfo::constructor((IPackageItemInfo*)orig);
    orig->GetDescriptionRes(&mDescriptionRes);
    orig->GetNonLocalizedDescription((ICharSequence**)&mNonLocalizedDescription);
    orig->GetFlags(&mFlags);
    orig->GetPriority(&mPriority);
    return NOERROR;
}

ECode CPermissionGroupInfo::LoadDescription(
    /* [in] */ IPackageManager* pm,
    /* [out] */ ICharSequence** description)
{
    VALIDATE_NOT_NULL(description);
    *description = NULL;

    if (mNonLocalizedDescription != NULL) {
        *description = mNonLocalizedDescription;
        REFCOUNT_ADD(*description);
        return NOERROR;
    }

    if (mDescriptionRes != 0) {
        pm->GetText(mPackageName, mDescriptionRes, NULL, description);
    }

    return NOERROR;
}

ECode CPermissionGroupInfo::ToString(
    /* [out] */ String* str)
{
    // return "PermissionGroupInfo{"
    //         + Integer.toHexString(System.identityHashCode(this))
    //         + " " + name + " flgs=0x" + Integer.toHexString(flags) + "}";
    assert(0);
    return E_NOT_IMPLEMENTED;
}

ECode CPermissionGroupInfo::ReadFromParcel(
    /* [in] */ IParcel* source)
{
    PackageItemInfo::ReadFromParcel(source);
    source->ReadInt32(&mDescriptionRes);
    // mNonLocalizedDescription = TextUtils.CHAR_SEQUENCE_CREATOR.createFromParcel(source);
    source->ReadInt32(&mFlags);
    source->ReadInt32(&mPriority);
    return NOERROR;
}

ECode CPermissionGroupInfo::WriteToParcel(
    /* [in] */ IParcel* dest)
{
    PackageItemInfo::WriteToParcel(dest);
    dest->WriteInt32(mDescriptionRes);
    // TextUtils.writeToParcel(nonLocalizedDescription, dest, parcelableFlags);
    dest->WriteInt32(mFlags);
    dest->WriteInt32(mPriority);
    return NOERROR;
}

ECode CPermissionGroupInfo::LoadLabel(
    /* [in] */ IPackageManager* pm,
    /* [out] */ ICharSequence** label)
{
    VALIDATE_NOT_NULL(label);
    return PackageItemInfo::LoadLabel(pm, label);
}

ECode CPermissionGroupInfo::LoadIcon(
    /* [in] */ IPackageManager* pm,
    /* [out] */ IDrawable** icon)
{
    VALIDATE_NOT_NULL(icon);
    return PackageItemInfo::LoadIcon(pm, icon);
}

ECode CPermissionGroupInfo::LoadLogo(
    /* [in] */ IPackageManager* pm,
    /* [out] */ IDrawable** logo)
{
    VALIDATE_NOT_NULL(logo);
    return PackageItemInfo::LoadLogo(pm, logo);
}

ECode CPermissionGroupInfo::LoadXmlMetaData(
    /* [in] */ IPackageManager* pm,
    /* [in] */ const String& name,
    /* [out] */ IXmlResourceParser** resource)
{
    VALIDATE_NOT_NULL(resource);
    return PackageItemInfo::LoadXmlMetaData(pm, name, resource);
}

ECode CPermissionGroupInfo::GetName(
    /* [out] */ String* name)
{
    VALIDATE_NOT_NULL(name);
    return PackageItemInfo::GetName(name);
}

ECode CPermissionGroupInfo::SetName(
    /* [in] */ const String& name)
{
    return PackageItemInfo::SetName(name);
}

ECode CPermissionGroupInfo::GetPackageName(
    /* [out] */ String* name)
{
    VALIDATE_NOT_NULL(name);
    return PackageItemInfo::GetPackageName(name);
}

ECode CPermissionGroupInfo::SetPackageName(
    /* [in] */ const String& name)
{
    return PackageItemInfo::SetPackageName(name);
}

ECode CPermissionGroupInfo::GetLabelRes(
    /* [out] */ Int32* labelRes)
{
    VALIDATE_NOT_NULL(labelRes);
    return PackageItemInfo::GetLabelRes(labelRes);
}

ECode CPermissionGroupInfo::SetLabelRes(
    /* [in] */ Int32 labelRes)
{
    return PackageItemInfo::SetLabelRes(labelRes);
}

ECode CPermissionGroupInfo::GetNonLocalizedLabel(
    /* [out] */ ICharSequence** label)
{
    VALIDATE_NOT_NULL(label);
    return PackageItemInfo::GetNonLocalizedLabel(label);
}

ECode CPermissionGroupInfo::SetNonLocalizedLabel(
    /* [in] */ ICharSequence*  label)
{
    return PackageItemInfo::SetNonLocalizedLabel(label);
}

ECode CPermissionGroupInfo::GetIcon(
    /* [out] */ Int32* icon)
{
    VALIDATE_NOT_NULL(icon);
    return PackageItemInfo::GetIcon(icon);
}

ECode CPermissionGroupInfo::SetIcon(
    /* [in] */ Int32 icon)
{
    return PackageItemInfo::SetIcon(icon);
}

ECode CPermissionGroupInfo::GetLogo(
    /* [out] */ Int32* logo)
{
    VALIDATE_NOT_NULL(logo);
    return PackageItemInfo::GetLogo(logo);
}

ECode CPermissionGroupInfo::SetLogo(
    /* [in] */ Int32 logo)
{
    return PackageItemInfo::SetLogo(logo);
}

ECode CPermissionGroupInfo::GetMetaData(
    /* [out] */ IBundle** metaData)
{
    VALIDATE_NOT_NULL(metaData);
    return PackageItemInfo::GetMetaData(metaData);
}

ECode CPermissionGroupInfo::SetMetaData(
    /* [in] */ IBundle* metaData)
{
    return PackageItemInfo::SetMetaData(metaData);
}

ECode CPermissionGroupInfo::GetDescriptionRes(
    /* [out] */ Int32* descriptionRes)
{
    VALIDATE_NOT_NULL(descriptionRes);
    *descriptionRes = mDescriptionRes;
    return NOERROR;
}

ECode CPermissionGroupInfo::SetDescriptionRes(
    /* [in] */ Int32 descriptionRes)
{
    mDescriptionRes = descriptionRes;
    return NOERROR;
}

ECode CPermissionGroupInfo::GetNonLocalizedDescription(
    /* [out] */ ICharSequence** nonLocalizedDescription)
{
    VALIDATE_NOT_NULL(nonLocalizedDescription);
    *nonLocalizedDescription = mNonLocalizedDescription;
    REFCOUNT_ADD(*nonLocalizedDescription);
    return NOERROR;
}

ECode CPermissionGroupInfo::SetNonLocalizedDescription(
    /* [in] */ ICharSequence* nonLocalizedDescription)
{
    mNonLocalizedDescription = nonLocalizedDescription;
    return NOERROR;
}

ECode CPermissionGroupInfo::GetFlags(
    /* [out] */ Int32* flags)
{
    VALIDATE_NOT_NULL(flags);
    *flags = mFlags;
    return NOERROR;
}

ECode CPermissionGroupInfo::SetFlags(
    /* [in] */ Int32 flags)
{
    mFlags = flags;
    return NOERROR;
}

ECode CPermissionGroupInfo::GetPriority(
    /* [out] */ Int32* priority)
{
    VALIDATE_NOT_NULL(priority);
    *priority = mPriority;
    return NOERROR;
}

ECode CPermissionGroupInfo::SetPriority(
    /* [in] */ Int32 priority)
{
    mPriority = priority;
    return NOERROR;
}

} // namespace Pm
} // namespace Content
} // namespace Droid
} // namespace Elastos

