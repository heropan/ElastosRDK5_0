
#include "ext/frameworkext.h"
#include "content/pm/CPackageInfoLite.h"

namespace Elastos {
namespace Droid {
namespace Content {
namespace Pm {

CPackageInfoLite::CPackageInfoLite()
    : mVersionCode(0)
    , mRecommendedInstallLocation(0)
    , mInstallLocation(0)
{}

CPackageInfoLite::~CPackageInfoLite()
{}

ECode CPackageInfoLite::constructor()
{
    return NOERROR;
}

ECode CPackageInfoLite::ToString(
    /* [out] */ String* str)
{
    VALIDATE_NOT_NULL(str);
    // return "PackageInfoLite{"
    //         + Integer.toHexString(System.identityHashCode(this))
    //         + " " + packageName + "}";
    assert(0);
    return E_NOT_IMPLEMENTED;
}

ECode CPackageInfoLite::ReadFromParcel(
    /* [in] */ IParcel* source)
{
    assert(source != NULL);
    source->ReadString(&mPackageName);
    source->ReadInt32(&mVersionCode);
    source->ReadInt32(&mRecommendedInstallLocation);
    source->ReadInt32(&mInstallLocation);
    Int32 verifiersLength;
    source->ReadInt32(&verifiersLength);
    if (verifiersLength == 0) {
        mVerifiers = ArrayOf<IVerifierInfo*>::Alloc(0);
    }
    else if (verifiersLength != 0) {
        source->ReadArrayOf((Handle32*)&mVerifiers);
    }
    return NOERROR;
}

ECode CPackageInfoLite::WriteToParcel(
    /* [in] */ IParcel* dest)
{
    assert(dest != NULL);
    dest->WriteString(mPackageName);
    dest->WriteInt32(mVersionCode);
    dest->WriteInt32(mRecommendedInstallLocation);
    dest->WriteInt32(mInstallLocation);
    if (mVerifiers == NULL || mVerifiers->GetLength() == 0) {
        dest->WriteInt32(0);
    }
    else {
        dest->WriteInt32(1);
        dest->WriteArrayOf((Handle32)mVerifiers.Get());
    }
    return NOERROR;
}

ECode CPackageInfoLite::GetPackageName(
    /* [out] */ String* name)
{
    VALIDATE_NOT_NULL(name);
    *name = mPackageName;
    return NOERROR;
}

ECode CPackageInfoLite::SetPackageName(
    /* [in] */ const String& name)
{
    mPackageName = name;
    return NOERROR;
}

ECode CPackageInfoLite::GetVersionCode(
    /* [out] */ Int32* versionCode)
{
    VALIDATE_NOT_NULL(versionCode);
    *versionCode = mVersionCode;
    return NOERROR;
}

ECode CPackageInfoLite::SetVersionCode(
    /* [in] */ Int32 versionCode)
{
    mVersionCode = versionCode;
    return NOERROR;
}

ECode CPackageInfoLite::GetRecommendedInstallLocation(
    /* [out] */ Int32* location)
{
    VALIDATE_NOT_NULL(location);
    *location = mRecommendedInstallLocation;
    return NOERROR;
}

ECode CPackageInfoLite::SetRecommendedInstallLocation(
    /* [in] */ Int32 location)
{
    mRecommendedInstallLocation = location;
    return NOERROR;
}

ECode CPackageInfoLite::GetInstallLocation(
    /* [out] */ Int32* location)
{
    VALIDATE_NOT_NULL(location);
    *location = mInstallLocation;
    return NOERROR;
}

ECode CPackageInfoLite::SetInstallLocation(
    /* [in] */ Int32 location)
{
    mInstallLocation = location;
    return NOERROR;
}

ECode CPackageInfoLite::GetVerifiers(
    /* [out, callee] */ ArrayOf<IVerifierInfo *>** verifiers)
{
    VALIDATE_NOT_NULL(verifiers);
    *verifiers = mVerifiers;
    INTERFACE_ADDREF(*verifiers);
    return NOERROR;
}

ECode CPackageInfoLite::SetVerifiers(
    /* [in] */ ArrayOf<IVerifierInfo *>* verifiers)
{
    mVerifiers = verifiers;
    return NOERROR;
}

} // namespace Pm
} // namespace Content
} // namespace Droid
} // namespace Elastos
