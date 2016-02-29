#ifndef  __ELASTOS_DROID_PACKAGES_LAUNCHER2_USERINITIALIZERECEIVER_H__
#define  __ELASTOS_DROID_PACKAGES_LAUNCHER2_USERINITIALIZERECEIVER_H__

#include "elastos/droid/ext/frameworkext.h"

namespace Elastos {
namespace Droid {
namespace Packages {
namespace Launcher2 {

class UserInitializeReceiver
    : public BroadcastReceiver
    , public IUserInitializeReceiver
{
public:
    CAR_INTERFACE_DECL();

    //@Override
    CARAPI OnReceive(
        /* [in] */ IContext* context,
        /* [in] */ IIntent* intent);

private:
    CARAPI_(void) AddWallpapers(
        /* [in] */ IResources* resources,
        /* [in] */ const String& packageName,
        /* [in] */ Int32 resid,
        /* [in] */ IArrayList* outList);
};

} // namespace Launcher2
} // namespace Packages
} // namespace Droid
} // namespace Elastos

#endif // __ELASTOS_DROID_PACKAGES_LAUNCHER2_USERINITIALIZERECEIVER_H__