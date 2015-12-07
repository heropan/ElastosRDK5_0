
#ifndef __ELASTOS_DROID_SERVER_POWER_MOBILEDIRECTCONTROLLER_H__
#define __ELASTOS_DROID_SERVER_POWER_MOBILEDIRECTCONTROLLER_H__

#include "Elastos.Droid.Server_server.h"
#include "elastos/droid/ext/frameworkdef.h"

namespace Elastos {
namespace Droid {
namespace Server {
namespace Power {

class MobileDirectController : public ElRefBase
{
public:
    static CARAPI_(AutoPtr<MobileDirectController>) GetInstance();

    CARAPI_(Boolean) IsMobileModeAvailable();

    CARAPI_(Boolean) IsNetworkEnable();

    CARAPI_(Boolean) SetNetworkEnable(
        /* [in] */ Boolean enable);

private:
    MobileDirectController() {}

private:
    static const String TAG;
    static const Boolean DEBUG = TRUE;
    static const String ENTWORK_ENABLE_PATH;
    static AutoPtr<MobileDirectController> sInstance;
};

} // namespace Power
} // namespace Server
} // namespace Droid
} // namespace Elastos

#endif // __ELASTOS_DROID_SERVER_POWER_MOBILEDIRECTCONTROLLER_H__
