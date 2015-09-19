
#include "webkit/CPluginManager.h"
#include <elastos/utility/logging/Logger.h>

using Elastos::Utility::Logging::Logger;

namespace Elastos {
namespace Droid {
namespace Webkit {

ECode CPluginManager::RefreshPlugins(
    /* [in] */ Boolean reloadOpenPages)
{
    return PluginManager::RefreshPlugins(reloadOpenPages);
}

ECode CPluginManager::constructor()
{
        // TODO: Add your code here
        return E_NOT_IMPLEMENTED;
}

ECode CPluginManager::constructor(
            /* [in] */ Elastos::Droid::Content::IContext * pCtx)
{
    SetContext(pCtx);
    return NOERROR;
}

} // namespace Webkit
} // namespace Droid
} // namespace Elastos
