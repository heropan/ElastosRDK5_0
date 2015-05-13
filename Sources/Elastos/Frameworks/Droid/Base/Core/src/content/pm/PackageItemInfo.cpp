
#include "ext/frameworkext.h"
#include "content/pm/PackageItemInfo.h"

using Elastos::Core::CStringWrapper;

namespace Elastos {
namespace Droid {
namespace Content {
namespace Pm {

PackageItemInfo::PackageItemInfo()
    : mLabelRes(0)
    , mIcon(0)
    , mLogo(0)
{}

PackageItemInfo::~PackageItemInfo()
{}

ECode PackageItemInfo::constructor()
{
    return NOERROR;
}

ECode PackageItemInfo::constructor(
    /* [in] */ IPackageItemInfo* orig)
{
    assert(orig != NULL);

    orig->GetName(&mName);
    if (!mName.IsNull()) mName = mName.Trim();
    orig->GetPackageName(&mPackageName);
    orig->GetLabelRes(&mLabelRes);
    orig->GetNonLocalizedLabel((ICharSequence**)&mNonLocalizedLabel);
    if (mNonLocalizedLabel != NULL) {
        String str;
        mNonLocalizedLabel->ToString(&str);
        mNonLocalizedLabel = NULL;
        CStringWrapper::New(str.Trim(), (ICharSequence**)&mNonLocalizedLabel);
    }
    orig->GetIcon(&mIcon);
    orig->GetLogo(&mLogo);
    orig->GetMetaData((IBundle**)&mMetaData);
    return NOERROR;
}

ECode PackageItemInfo::LoadLabel(
    /* [in] */ IPackageManager* pm,
    /* [out] */ ICharSequence** label)
{
    assert(label != NULL);
    *label = NULL;

    if (mNonLocalizedLabel != NULL) {
        *label = mNonLocalizedLabel;
        INTERFACE_ADDREF(*label);
        return NOERROR;
    }
    if (mLabelRes != 0) {
        AutoPtr<ICharSequence> lb;
        pm->GetText(mPackageName, mLabelRes, GetApplicationInfo(), (ICharSequence**)&lb);
        if (lb != NULL) {
            String str;
            lb->ToString(&str);
            return CStringWrapper::New(str.Trim(), label);
        }
    }
    if (!mName.IsNull()) {
        return CStringWrapper::New(mName, label);
    }
    return CStringWrapper::New(mPackageName, label);
}

ECode PackageItemInfo::LoadIcon(
    /* [in] */ IPackageManager* pm,
    /* [out] */ IDrawable** icon)
{
    assert(icon != NULL);

    if (mIcon != 0) {
        pm->GetDrawable(mPackageName, mIcon, GetApplicationInfo(), icon);
        if (*icon != NULL) {
            return NOERROR;
        }
    }
    return LoadDefaultIcon(pm, icon);
}

ECode PackageItemInfo::LoadDefaultIcon(
    /* [in] */ IPackageManager* pm,
    /* [out] */ IDrawable** icon)
{
    return pm->GetDefaultActivityIcon(icon);
}

ECode PackageItemInfo::LoadLogo(
    /* [in] */ IPackageManager* pm,
    /* [out] */ IDrawable** logo)
{
    assert(logo != NULL);

    if (mLogo != 0) {
       pm->GetDrawable(mPackageName, mLogo, GetApplicationInfo(), logo);
       if (*logo != NULL) {
           return NOERROR;
       }
    }
    return LoadDefaultLogo(pm, logo);
}

ECode PackageItemInfo::LoadDefaultLogo(
    /* [in] */ IPackageManager* pm,
    /* [out] */ IDrawable** icon)
{
    *icon = NULL;
    return NOERROR;
}

ECode PackageItemInfo::LoadXmlMetaData(
    /* [in] */ IPackageManager* pm,
    /* [in] */ const String& name,
    /* [out] */ IXmlResourceParser** resource)
{
    assert(resource != NULL);

    if (mMetaData != NULL) {
        Int32 resid = 0;
        mMetaData->GetInt32(name, &resid);
        if (resid != 0) {
            return pm->GetXml(mPackageName, resid, GetApplicationInfo(), resource);
        }
    }
    *resource = NULL;
    return NOERROR;
}

ECode PackageItemInfo::DumpFront(
    /* [in] */ IPrinter* pw,
    /* [in] */ const String& prefix)
{
    // if (name != null) {
    //     pw.println(prefix + "name=" + name);
    // }
    // pw.println(prefix + "packageName=" + packageName);
    // if (labelRes != 0 || nonLocalizedLabel != null || icon != 0) {
    //     pw.println(prefix + "labelRes=0x" + Integer.toHexString(labelRes)
    //             + " nonLocalizedLabel=" + nonLocalizedLabel
    //             + " icon=0x" + Integer.toHexString(icon));
    // }
    assert(0);
    return E_NOT_IMPLEMENTED;
}

ECode PackageItemInfo::DumpBack(
    /* [in] */ IPrinter* pw,
    /* [in] */ const String& prefix)
{
    // no back here
    return NOERROR;
}

ECode PackageItemInfo::WriteToParcel(
        /* [in] */ IParcel* dest)
{
    assert(dest != NULL);

    dest->WriteString(mName);
    dest->WriteString(mPackageName);
    dest->WriteInt32(mLabelRes);
    // TextUtils.writeToParcel(nonLocalizedLabel, dest, parcelableFlags);
    dest->WriteInt32(mIcon);
    dest->WriteInt32(mLogo);
    dest->WriteInterfacePtr(mMetaData);
    return NOERROR;
}

ECode PackageItemInfo::ReadFromParcel(
    /* [in] */ IParcel* src)
{
    assert(src != NULL);

    src->ReadString(&mName);
    src->ReadString(&mPackageName);
    src->ReadInt32(&mLabelRes);
    // nonLocalizedLabel
    //         = TextUtils.CHAR_SEQUENCE_CREATOR.createFromParcel(source);
    src->ReadInt32(&mIcon);
    src->ReadInt32(&mLogo);
    AutoPtr<IInterface> obj;
    src->ReadInterfacePtr((Handle32*)&obj);
    mMetaData = obj != NULL ? IBundle::Probe(obj) : NULL;
    return NOERROR;
}

AutoPtr<IApplicationInfo> PackageItemInfo::GetApplicationInfo()
{
    return NULL;
}

ECode PackageItemInfo::GetName(
    /* [out] */ String* name)
{
    assert(name != NULL);
    *name = mName;
    return NOERROR;
}

ECode PackageItemInfo::SetName(
    /* [in] */ const String& name)
{
    mName = name;
    return NOERROR;
}

ECode PackageItemInfo::GetPackageName(
    /* [out] */ String* name)
{
    assert(name != NULL);
    *name = mPackageName;
    return NOERROR;
}

ECode PackageItemInfo::SetPackageName(
    /* [in] */ const String& name)
{
    mPackageName = name;
    return NOERROR;
}

ECode PackageItemInfo::GetLabelRes(
    /* [out] */ Int32* labelRes)
{
    assert(labelRes != NULL);
    *labelRes = mLabelRes;
    return NOERROR;
}

ECode PackageItemInfo::SetLabelRes(
    /* [in] */ Int32 labelRes)
{
    mLabelRes = labelRes;
    return NOERROR;
}

ECode PackageItemInfo::GetNonLocalizedLabel(
    /* [out] */ ICharSequence** label)
{
    assert(label != NULL);
    *label = mNonLocalizedLabel;
    INTERFACE_ADDREF(*label);
    return NOERROR;
}

ECode PackageItemInfo::SetNonLocalizedLabel(
    /* [in] */ ICharSequence* label)
{
    mNonLocalizedLabel = label;
    return NOERROR;
}

ECode PackageItemInfo::GetIcon(
    /* [out] */ Int32* icon)
{
    assert(icon != NULL);
    *icon = mIcon;
    return NOERROR;
}

ECode PackageItemInfo::SetIcon(
    /* [in] */ Int32 icon)
{
    mIcon = icon;
    return NOERROR;
}

ECode PackageItemInfo::GetLogo(
    /* [out] */ Int32* logo)
{
    assert(logo != NULL);
    *logo = mLogo;
    return NOERROR;
}

ECode PackageItemInfo::SetLogo(
    /* [in] */ Int32 logo)
{
    mLogo = logo;
    return NOERROR;
}

ECode PackageItemInfo::GetMetaData(
    /* [out] */ IBundle** metaData)
{
    assert(metaData != NULL);
    *metaData = mMetaData;
    INTERFACE_ADDREF(*metaData);
    return NOERROR;
}

ECode PackageItemInfo::SetMetaData(
    /* [in] */ IBundle* metaData)
{
    mMetaData = metaData;
    return NOERROR;
}

} // namespace Pm
} // namespace Content
} // namespace Droid
} // namespace Elastos
