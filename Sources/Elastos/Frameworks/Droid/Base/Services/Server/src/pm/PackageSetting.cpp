
#include "pm/PackageSetting.h"

namespace Elastos {
namespace Droid {
namespace Server {
namespace Pm {

// {93602C16-4192-4A01-9355-56A477D9EFE8}
extern const InterfaceID EIID_PackageSetting =
    { 0x93602c16, 0x4192, 0x4a01, { 0x93, 0x55, 0x56, 0xa4, 0x77, 0xd9, 0xef, 0xe8 } };


CAR_INTERFACE_IMPL_WITH_CPP_CAST(PackageSetting, IInterface, PackageSetting)

PackageSetting::PackageSetting(
    /* [in] */ const String& name,
    /* [in] */ const String& realName,
    /* [in] */ IFile* codePath,
    /* [in] */ IFile* resourcePath,
    /* [in] */ const String& nativeLibraryPathString,
    /* [in] */ Int32 pVersionCode,
    /* [in] */ Int32 pkgFlags)
    : PackageSettingBase(name, realName, codePath, resourcePath,
            nativeLibraryPathString, pVersionCode, pkgFlags)
    , mAppId(0)
{}

PackageSetting::PackageSetting(
    /* [in] */ const PackageSetting* orig)
    : PackageSettingBase(orig)
    , mAppId(orig->mAppId)
    , mPkg(orig->mPkg)
    , mSharedUser(orig->mSharedUser)
{}

} // namespace Pm
} // namespace Server
} // namespace Droid
} // namespace Elastos
