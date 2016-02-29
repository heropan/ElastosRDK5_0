#ifndef  __ELASTOS_DROID_PACKAGES_LAUNCHER2_PACKAGECHANGEDRECEIVER_H__
#define  __ELASTOS_DROID_PACKAGES_LAUNCHER2_PACKAGECHANGEDRECEIVER_H__

#include "elastos/droid/ext/frameworkext.h"

namespace Elastos {
namespace Droid {
namespace Packages {
namespace Launcher2 {

class PackageChangedReceiver
    : public BroadcastReceiver
    , public IPackageChangedReceiver
{
public:
    CAR_INTERFACE_DECL();

    //@Override
    CARAPI OnReceive(
        /* [in] */ IContext* context,
        /* [in] */ IIntent* intent);
};

} // namespace Launcher2
} // namespace Packages
} // namespace Droid
} // namespace Elastos

#endif // __ELASTOS_DROID_PACKAGES_LAUNCHER2_PACKAGECHANGEDRECEIVER_H__