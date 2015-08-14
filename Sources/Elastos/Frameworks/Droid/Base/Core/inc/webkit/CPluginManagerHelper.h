
#ifndef __ELASTOS_DROID_WEBKIT_CPLUGINMANAGERHELPER_H__
#define __ELASTOS_DROID_WEBKIT_CPLUGINMANAGERHELPER_H__

#include "_CPluginManagerHelper.h"
#include "webkit/CPluginManager.h"

using Elastos::Droid::Content::IContext;

namespace Elastos {
namespace Droid {
namespace Webkit {

class CPluginManager;

CarClass(CPluginManagerHelper)
{
public:
    CARAPI GetInstance(
        /* [in] */ IContext* context,
        /* [out] */ IPluginManager** pluginManager);

private:
    static Object sLock;
    static AutoPtr<CPluginManager> mInstance;
};

} // namespace Webkit
} // namespace Droid
} // namespace Elastos

#endif // __ELASTOS_DROID_WEBKIT_CPLUGINMANAGERHELPER_H__
