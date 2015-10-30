
#ifndef __ELASTOS_DROID_WEBKIT_CPLUGINMANAGERHELPER_H__
#define __ELASTOS_DROID_WEBKIT_CPLUGINMANAGERHELPER_H__

#include "_Elastos_Droid_Webkit_CPluginManagerHelper.h"
#include "elastos/droid/webkit/CPluginManager.h"
#include <elastos/core/Singleton.h>

using Elastos::Droid::Content::IContext;

namespace Elastos {
namespace Droid {
namespace Webkit {

class CPluginManager;

CarClass(CPluginManagerHelper)
    , public Singleton
    , public IPluginManagerHelper
{
public:
    CAR_INTERFACE_DECL()

    CAR_SINGLETON_DECL()

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
