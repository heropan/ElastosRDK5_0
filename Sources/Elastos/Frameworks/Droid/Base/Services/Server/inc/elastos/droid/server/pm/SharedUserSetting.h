#ifndef __ELASTOS_DROID_SERVER_PM_SHAREDUSERSETTING_H__
#define __ELASTOS_DROID_SERVER_PM_SHAREDUSERSETTING_H__

#include "elastos/droid/ext/frameworkext.h"
#include "Elastos.Droid.Server.h"
#include "pm/GrantedPermissions.h"
#include "pm/PackageSetting.h"
#include "pm/PackageSignatures.h"
#include <elastos/utility/etl/HashSet.h>

using Elastos::Utility::Etl::HashSet;

namespace Elastos {
namespace Droid {
namespace Server {
namespace Pm {

class PackageSetting;

} // namespace Pm
} // namespace Server
} // namespace Droid
} // namespace Elastos

_ETL_NAMESPACE_BEGIN
template<> struct Hash<Elastos::Droid::Server::Pm::PackageSetting *>
{
    size_t operator()(const Elastos::Droid::Server::Pm::PackageSetting * s) const { return (size_t)s; }
};
_ETL_NAMESPACE_END

namespace Elastos {
namespace Droid {
namespace Server {
namespace Pm {

extern const InterfaceID EIID_SharedUserSetting;

class SharedUserSetting
    : public GrantedPermissions
    , public IInterface
{
public:
    CAR_INTERFACE_DECL()

    SharedUserSetting(
        /* [in] */ const String& name,
        /* [in] */ Int32 pkgFlags);

    virtual ~SharedUserSetting();

public:
    String mName;

    Int32 mUserId;

    HashSet<AutoPtr<PackageSetting> > mPackages; // TODO using IWeakReference

    AutoPtr<PackageSignatures> mSignatures;
};

} // namespace Pm
} // namespace Server
} // namespace Droid
} // namespace Elastos

#endif //__ELASTOS_DROID_SERVER_PM_SHAREDUSERSETTING_H__
