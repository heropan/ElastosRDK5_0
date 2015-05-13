#ifndef __PENDINGPACKAGE_H__
#define __PENDINGPACKAGE_H__

#include "Elastos.Droid.Server_server.h"
#include "pm/PackageSettingBase.h"

using Elastos::IO::IFile;

namespace Elastos {
namespace Droid {
namespace Server {
namespace Pm {

class PendingPackage : public PackageSettingBase
{
public:
    PendingPackage(
        /* [in] */ const String& name,
        /* [in] */ const String& realName,
        /* [in] */ IFile* codePath,
        /* [in] */ IFile* resourcePath,
        /* [in] */ const String& nativeLibraryPathString,
        /* [in] */ Int32 sharedId,
        /* [in] */ Int32 pVersionCode,
        /* [in] */ Int32 pkgFlags)
        : PackageSettingBase(name, realName, codePath, resourcePath,
                nativeLibraryPathString, pVersionCode, pkgFlags)
        , mSharedId(sharedId)
    {}

public:
    Int32 mSharedId;
};

} // namespace Pm
} // namespace Server
} // namespace Droid
} // namespace Elastos

#endif //__PENDINGPACKAGE_H__
