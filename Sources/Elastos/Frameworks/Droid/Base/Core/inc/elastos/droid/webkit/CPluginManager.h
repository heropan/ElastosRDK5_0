
#ifndef __ELASTOS_DROID_WEBKIT_CPLUGINMANAGER_H__
#define __ELASTOS_DROID_WEBKIT_CPLUGINMANAGER_H__

#include "_Elastos_Droid_Webkit_CPluginManager.h"
#include "elastos/droid/webkit/PluginManager.h"

namespace Elastos {
namespace Droid {
namespace Webkit {

CarClass(CPluginManager), public PluginManager
{
public:
    CAR_OBJECT_DECL();

    CARAPI RefreshPlugins(
        /* [in] */ Boolean reloadOpenPages);
};

} // namespace Webkit
} // namespace Droid
} // namespace Elastos

#endif // __ELASTOS_DROID_WEBKIT_CPLUGINMANAGER_H__
