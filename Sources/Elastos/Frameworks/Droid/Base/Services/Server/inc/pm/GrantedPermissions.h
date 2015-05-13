#ifndef __ELASTOS_DROID_SERVER_PM_GRANTEDPERMISSIONS_H__
#define __ELASTOS_DROID_SERVER_PM_GRANTEDPERMISSIONS_H__

#include "ext/frameworkext.h"
#include "Elastos.Droid.Server_server.h"
#include <elastos/HashSet.h>

using Elastos::Utility::HashSet;

namespace Elastos {
namespace Droid {
namespace Server {
namespace Pm {

class GrantedPermissions : public ElRefBase
{
public:
    GrantedPermissions(
        /* [in] */ Int32 pkgFlags);

    GrantedPermissions(
        /* [in] */ const GrantedPermissions* base);

    CARAPI_(void) SetFlags(
        /* [in] */ Int32 pkgFlags);

public:
    Int32 mPkgFlags;

    HashSet<String> mGrantedPermissions;

    AutoPtr< ArrayOf<Int32> > mGids;
};

} // namespace Pm
} // namespace Server
} // namespace Droid
} // namespace Elastos

#endif //__ELASTOS_DROID_SERVER_PM_GRANTEDPERMISSIONS_H__
