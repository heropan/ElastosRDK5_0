
#ifndef __CPLUGINMANAGERHELPER_H__
#define __CPLUGINMANAGERHELPER_H__

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
    static Mutex sLock;
    static AutoPtr<CPluginManager> mInstance;
};

} // namespace Webkit
} // namespace Droid
} // namespace Elastos

#endif // __CPLUGINMANAGERHELPER_H__
