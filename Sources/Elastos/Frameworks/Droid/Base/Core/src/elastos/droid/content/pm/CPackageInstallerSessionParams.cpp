
#include "Elastos.Droid.Graphics.h"
#include "elastos/droid/content/pm/CPackageInstallerSessionParams.h"
//#include "elastos/droid/graphics/CBitmap.h"
#include "elastos/droid/net/Uri.h"

//using Elastos::Droid::Graphics::CBitmap;
using Elastos::Droid::Net::Uri;

namespace Elastos {
namespace Droid {
namespace Content {
namespace Pm {

CAR_INTERFACE_IMPL_2(CPackageInstallerSessionParams, Object, IPackageInstallerSessionInfo, IParcelable)

CAR_OBJECT_IMPL(CPackageInstallerSessionParams)

CPackageInstallerSessionParams::CPackageInstallerSessionParams()
    : mMode(IPackageInstallerSessionParams::MODE_INVALID)
    , mInstallFlags(0)
    , mInstallLocation(IPackageInfo::INSTALL_LOCATION_INTERNAL_ONLY)
    , mSizeBytes(-1)
    , mAppIconLastModified(-1)
{
}

CPackageInstallerSessionParams::~CPackageInstallerSessionParams()
{
}

ECode CPackageInstallerSessionParams::constructor()
{
    return NOERROR;
}

ECode CPackageInstallerSessionParams::constructor(
    /* [in] */ Int32 mode)
{
    mMode = mode;
    return NOERROR;
}

ECode CPackageInstallerSessionParams::WriteToParcel(
    /* [in] */ IParcel* dest)
{
    dest->WriteInt32(mMode);
    dest->WriteInt32(mInstallFlags);
    dest->WriteInt32(mInstallLocation);
    dest->WriteInt64(mSizeBytes);
    dest->WriteString(mAppPackageName);
    IParcelable::Probe(mAppIcon)->WriteToParcel(dest);
    dest->WriteString(mAppLabel);
    IParcelable::Probe(mOriginatingUri)->WriteToParcel(dest);
    IParcelable::Probe(mReferrerUri)->WriteToParcel(dest);
    dest->WriteString(mAbiOverride);
    return NOERROR;
}

ECode CPackageInstallerSessionParams::ReadFromParcel(
    /* [in] */ IParcel* source)
{
    source->ReadInt32(&mMode);
    source->ReadInt32(&mInstallFlags);
    source->ReadInt32(&mInstallLocation);
    source->ReadInt64(&mSizeBytes);
    source->ReadString(&mAppPackageName);
    assert(0 && "TODO");
    // CBitmap::New((IBitmap**)&mAppIcon);
    IParcelable::Probe(mAppIcon)->ReadFromParcel(source);
    source->ReadString(&mAppLabel);
    IParcelable::Probe(mOriginatingUri)->ReadFromParcel(source);
    IParcelable::Probe(mReferrerUri)->ReadFromParcel(source);
    source->ReadString(&mAbiOverride);
    return NOERROR;
}

ECode CPackageInstallerSessionParams::SetInstallLocation(
    /* [in] */ Int32 installLocation)
{
    mInstallLocation = installLocation;
    return NOERROR;
}

ECode CPackageInstallerSessionParams::SetSize(
    /* [in] */ Int64 sizeBytes)
{
    mSizeBytes = sizeBytes;
    return NOERROR;
}

ECode CPackageInstallerSessionParams::SetAppPackageName(
    /* [in] */ const String& appPackageName)
{
    mAppPackageName = appPackageName;
    return NOERROR;
}

ECode CPackageInstallerSessionParams::SetAppIcon(
    /* [in] */ IBitmap* appIcon)
{
    mAppIcon = appIcon;
    return NOERROR;
}

ECode CPackageInstallerSessionParams::SetAppLabel(
    /* [in] */ ICharSequence* appLabel)
{
    mAppLabel = (appLabel != NULL) ? Object::ToString(appLabel) : String(NULL);
    return NOERROR;
}

ECode CPackageInstallerSessionParams::SetOriginatingUri(
    /* [in] */ IUri* originatingUri)
{
    mOriginatingUri = originatingUri;
    return NOERROR;
}

ECode CPackageInstallerSessionParams::SetReferrerUri(
    /* [in] */ IUri* referrerUri)
{
    mReferrerUri = referrerUri;
    return NOERROR;
}

/** {@hide} */
ECode CPackageInstallerSessionParams::SetInstallFlagsInternal()
{
    mInstallFlags |= IPackageManager::INSTALL_INTERNAL;
    mInstallFlags &= ~IPackageManager::INSTALL_EXTERNAL;
    return NOERROR;
}

/** {@hide} */
ECode CPackageInstallerSessionParams::SetInstallFlagsExternal()
{
    mInstallFlags |= IPackageManager::INSTALL_EXTERNAL;
    mInstallFlags &= ~IPackageManager::INSTALL_INTERNAL;
    return NOERROR;
}

} // namespace Pm
} // namespace Content
} // namespace Droid
} // namespace Elastos