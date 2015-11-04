
#include "elastos/droid/webkit/CPluginManagerHelper.h"
#include <elastos/utility/logging/Logger.h>
#include <elastos/core/AutoLock.h>

//#include <unistd.h>
//#include <sys/syscall.h>

//pid_t gettid(void)
//{
//    return syscall(__NR_gettid);
//}

using Elastos::Core::AutoLock;
using Elastos::Utility::Logging::Logger;

namespace Elastos {
namespace Droid {
namespace Webkit {

Object CPluginManagerHelper::sLock;
AutoPtr<CPluginManager> CPluginManagerHelper::mInstance;

CAR_INTERFACE_IMPL(CPluginManagerHelper, Object, IPluginManagerHelper);

CAR_SINGLETON_IMPL(CPluginManagerHelper);

ECode CPluginManagerHelper::GetInstance(
    /* [in] */ IContext* context,
    /* [out] */ IPluginManager** pluginManager)
{
    // TODO: Add your code here
    AutoLock lock(sLock);
    if (mInstance == NULL) {
        if (context == NULL)
        {
            Logger::E("CPluginManagerHelper", "First call to PluginManager need a valid context.");
            assert(0);
        }
        CPluginManager::New(context, (IPluginManager**)&mInstance);
    }
    *pluginManager = mInstance.Get();
    mInstance->AddRef();
    return NOERROR;
}

} // namespace Webkit
} // namepsace Droid
} // namespace Elastos
