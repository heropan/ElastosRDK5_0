
#include "ext/frameworkext.h"
#include "content/pm/CPermissionInfo.h"

namespace Elastos {
namespace Droid {
namespace Content {
namespace Pm {

CPermissionInfo::CPermissionInfo()
    : mProtectionLevel(0)
    , mFlags(0)
    , mDescriptionRes(0)
{}

CPermissionInfo::~CPermissionInfo()
{}

ECode CPermissionInfo::constructor()
{
    return NOERROR;
}

ECode CPermissionInfo::constructor(
    /* [in] */ IPermissionInfo* orig)
{
    assert(orig != NULL);
    PackageItemInfo::constructor((IPackageItemInfo*)orig);
    orig->GetProtectionLevel(&mProtectionLevel);
    orig->GetGroup(&mGroup);
    orig->GetFlags(&mFlags);
    orig->GetDescriptionRes(&mDescriptionRes);
    orig->GetNonLocalizedDescription((ICharSequence**)&mNonLocalizedDescription);
    return NOERROR;
}

Int32 CPermissionInfo::FixProtectionLevel(
    /* [in] */ Int32 level)
{
    if (level == PROTECTION_SIGNATURE_OR_SYSTEM) {
        level = PROTECTION_SIGNATURE | PROTECTION_FLAG_SYSTEM;
    }
    return level;
}

String CPermissionInfo::ProtectionToString(
    /* [in] */ Int32 level)
{
    String protLevel("????");
    switch (level & PROTECTION_MASK_BASE) {
        case PROTECTION_DANGEROUS:
        {
            protLevel = String("dangerous");
            break;
        }
        case PROTECTION_NORMAL:
        {
            protLevel = String("normal");
            break;
        }
        case PROTECTION_SIGNATURE:
        {
            protLevel = String("signature");
            break;
        }
        case PROTECTION_SIGNATURE_OR_SYSTEM:
        {
            protLevel = String("signatureOrSystem");
            break;
        }
    }
    if ((level & IPermissionInfo::PROTECTION_FLAG_SYSTEM) != 0) {
        protLevel += String("|system");
    }
    if ((level & IPermissionInfo::PROTECTION_FLAG_DEVELOPMENT) != 0) {
        protLevel += String("|development");
    }
    if ((level& IPermissionInfo::PROTECTION_FLAG_APPOP) != 0) {
        protLevel += String("|appop");
    }

    return protLevel;
}

ECode CPermissionInfo::LoadDescription(
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

ECode CPermissionInfo::ToString(
    /* [out] */ String* str)
{
    // return "PermissionInfo{"
    //         + Integer.toHexString(System.identityHashCode(this))
    //         + " " + name + "}";
    assert(0);
    return E_NOT_IMPLEMENTED;
}

ECode CPermissionInfo::ReadFromParcel(
    /* [in] */ IParcel* source)
{
    PackageItemInfo::ReadFromParcel(source);
    source->ReadInt32(&mProtectionLevel);
    source->ReadString(&mGroup);
    source->ReadInt32(&mFlags);
    source->ReadInt32(&mDescriptionRes);
    // mNonLocalizedDescription = TextUtils.CHAR_SEQUENCE_CREATOR.createFromParcel(source);
    return NOERROR;
}

ECode CPermissionInfo::WriteToParcel(
    /* [in] */ IParcel* dest)
{
    PackageItemInfo::WriteToParcel(dest);
    dest->WriteInt32(mProtectionLevel);
    dest->WriteString(mGroup);
    dest->WriteInt32(mFlags);
    dest->WriteInt32(mDescriptionRes);
    // TextUtils.writeToParcel(mNonLocalizedDescription, dest, parcelableFlags);
    return NOERROR;
}

ECode CPermissionInfo::LoadLabel(
    /* [in] */ IPackageManager* pm,
    /* [out] */ ICharSequence** label)
{
    VALIDATE_NOT_NULL(label);
    return PackageItemInfo::LoadLabel(pm, label);
}

ECode CPermissionInfo::LoadIcon(
    /* [in] */ IPackageManager* pm,
    /* [out] */ IDrawable** icon)
{
    VALIDATE_NOT_NULL(icon)
    return PackageItemInfo::LoadIcon(pm, icon);
}

ECode CPermissionInfo::LoadLogo(
    /* [in] */ IPackageManager* pm,
    /* [out] */ IDrawable** logo)
{
    VALIDATE_NOT_NULL(logo);
    return PackageItemInfo::LoadLogo(pm, logo);
}

ECode CPermissionInfo::LoadXmlMetaData(
    /* [in] */ IPackageManager* pm,
    /* [in] */ const String& name,
    /* [out] */ IXmlResourceParser** resource)
{
    VALIDATE_NOT_NULL(resource);
    return PackageItemInfo::LoadXmlMetaData(pm, name, resource);
}

ECode CPermissionInfo::GetName(
    /* [out] */ String* name)
{
    VALIDATE_NOT_NULL(name);
    return PackageItemInfo::GetName(name);
}

ECode CPermissionInfo::SetName(
    /* [in] */ const String& name)
{
    return PackageItemInfo::SetName(name);
}

ECode CPermissionInfo::GetPackageName(
    /* [out] */ String* name)
{
    VALIDATE_NOT_NULL(name);
    return PackageItemInfo::GetPackageName(name);
}

ECode CPermissionInfo::SetPackageName(
    /* [in] */ const String& name)
{
    return PackageItemInfo::SetPackageName(name);
}

ECode CPermissionInfo::GetLabelRes(
    /* [out] */ Int32* labelRes)
{
    VALIDATE_NOT_NULL(labelRes);
    return PackageItemInfo::GetLabelRes(labelRes);
}

ECode CPermissionInfo::SetLabelRes(
    /* [in] */ Int32 labelRes)
{
    return PackageItemInfo::SetLabelRes(labelRes);
}

ECode CPermissionInfo::GetNonLocalizedLabel(
    /* [out] */ ICharSequence** label)
{
    VALIDATE_NOT_NULL(label);
    return PackageItemInfo::GetNonLocalizedLabel(label);
}

ECode CPermissionInfo::SetNonLocalizedLabel(
    /* [in] */ ICharSequence*  label)
{
    return PackageItemInfo::SetNonLocalizedLabel(label);
}

ECode CPermissionInfo::GetIcon(
    /* [out] */ Int32* icon)
{
    VALIDATE_NOT_NULL(icon);
    return PackageItemInfo::GetIcon(icon);
}

ECode CPermissionInfo::SetIcon(
    /* [in] */ Int32 icon)
{
    return PackageItemInfo::SetIcon(icon);
}

ECode CPermissionInfo::GetLogo(
    /* [out] */ Int32* logo)
{
    VALIDATE_NOT_NULL(logo);
    return PackageItemInfo::GetLogo(logo);
}

ECode CPermissionInfo::SetLogo(
    /* [in] */ Int32 logo)
{
    return PackageItemInfo::SetLogo(logo);
}

ECode CPermissionInfo::GetMetaData(
    /* [out] */ IBundle** metaData)
{
    VALIDATE_NOT_NULL(metaData);
    return PackageItemInfo::GetMetaData(metaData);
}

ECode CPermissionInfo::SetMetaData(
    /* [in] */ IBundle* metaData)
{
    return PackageItemInfo::SetMetaData(metaData);
}

ECode CPermissionInfo::GetProtectionLevel(
    /* [out] */ Int32* level)
{
    VALIDATE_NOT_NULL(level);
    *level = mProtectionLevel;
    return NOERROR;
}

ECode CPermissionInfo::SetProtectionLevel(
    /* [in] */ Int32 level)
{
    mProtectionLevel = level;
    return NOERROR;
}

ECode CPermissionInfo::GetGroup(
    /* [out] */ String* group)
{
    VALIDATE_NOT_NULL(group);
    *group = mGroup;
    return NOERROR;
}

ECode CPermissionInfo::SetGroup(
    /* [in] */ const String& group)
{
    mGroup = group;
    return NOERROR;
}

ECode CPermissionInfo::GetFlags(
    /* [out] */ Int32* flags)
{
    VALIDATE_NOT_NULL(flags);
    *flags = mFlags;
    return NOERROR;
}

ECode CPermissionInfo::SetFlags(
    /* [in] */ Int32 flags)
{
    mFlags = flags;
    return NOERROR;
}

ECode CPermissionInfo::GetDescriptionRes(
    /* [out] */ Int32* desRes)
{
    VALIDATE_NOT_NULL(desRes);
    *desRes = mDescriptionRes;
    return NOERROR;
}

ECode CPermissionInfo::SetDescriptionRes(
    /* [in] */ Int32 desRes)
{
    mDescriptionRes = desRes;
    return NOERROR;
}

ECode CPermissionInfo::GetNonLocalizedDescription(
    /* [out] */ ICharSequence** des)
{
    VALIDATE_NOT_NULL(des);
    *des = mNonLocalizedDescription;
    REFCOUNT_ADD(*des);
    return NOERROR;
}

ECode CPermissionInfo::SetNonLocalizedDescription(
    /* [in] */ ICharSequence* des)
{
    mNonLocalizedDescription = des;
    return NOERROR;
}

} // namespace Pm
} // namespace Content
} // namespace Droid
} // namespace Elastos
