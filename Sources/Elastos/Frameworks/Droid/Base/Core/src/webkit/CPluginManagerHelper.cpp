
#include "webkit/CPluginManagerHelper.h"
#include <elastos/utility/logging/Logger.h>
//#include <unistd.h>
//#include <sys/syscall.h>

//pid_t gettid(void)
//{
//    return syscall(__NR_gettid);
//}

using Elastos::Utility::Logging::Logger;

namespace Elastos {
namespace Droid {
namespace Webkit {

Mutex CPluginManagerHelper::sLock;
AutoPtr<CPluginManager> CPluginManagerHelper::mInstance;

ECode CPluginManagerHelper::GetInstance(
    /* [in] */ IContext* context,
    /* [out] */ IPluginManager** pluginManager)
{
    // TODO: Add your code here
    Mutex::Autolock lock(sLock);
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
