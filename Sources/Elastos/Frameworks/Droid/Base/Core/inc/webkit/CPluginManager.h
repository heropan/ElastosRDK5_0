
#ifndef __CPLUGINMANAGER_H__
#define __CPLUGINMANAGER_H__

#include "_CPluginManager.h"
#include "webkit/PluginManager.h"

namespace Elastos {
namespace Droid {
namespace Webkit {

CarClass(CPluginManager), public PluginManager
{
public:
    CARAPI RefreshPlugins(
        /* [in] */ Boolean reloadOpenPages);

    CARAPI constructor();

    CARAPI constructor(
                /* [in] */ Elastos::Droid::Content::IContext * pCtx);

};

} // namespace Webkit
} // namespace Droid
} // namespace Elastos

#endif // __CPLUGINMANAGER_H__
