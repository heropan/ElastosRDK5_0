#ifndef __ELASTOS_DROID_SERVER_PM_PACKAGESETTING_H__
#define __ELASTOS_DROID_SERVER_PM_PACKAGESETTING_H__

#include "pm/PackageSettingBase.h"
#include "pm/SharedUserSetting.h"
#include "elastos/droid/content/pm/PackageParser.h"

using Elastos::Droid::Content::Pm::PackageParser;

namespace Elastos {
namespace Droid {
namespace Server {
namespace Pm {

extern const InterfaceID EIID_PackageSetting;

class SharedUserSetting;

class PackageSetting
    : public PackageSettingBase
    , public IInterface
{
public:
    CAR_INTERFACE_DECL()

    PackageSetting(
        /* [in] */ const String& name,
        /* [in] */ const String& realName,
        /* [in] */ IFile* codePath,
        /* [in] */ IFile* resourcePath,
        /* [in] */ const String& nativeLibraryPathString,
        /* [in] */ Int32 pVersionCode,
        /* [in] */ Int32 pkgFlags);

    PackageSetting(
        /* [in] */ const PackageSetting* orig);

public:
    Int32 mAppId;
    AutoPtr<PackageParser::Package> mPkg;
    AutoPtr<SharedUserSetting> mSharedUser;
};

} // namespace Pm
} // namespace Server
} // namespace Droid
} // namespace Elastos

#endif //__ELASTOS_DROID_SERVER_PM_PACKAGESETTING_H__
