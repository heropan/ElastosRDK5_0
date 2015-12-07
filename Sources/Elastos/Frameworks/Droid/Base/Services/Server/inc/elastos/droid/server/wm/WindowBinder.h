
#ifndef __ELASTOS_DROID_SERVER_WM_WINDOWBINDER_H__
#define __ELASTOS_DROID_SERVER_WM_WINDOWBINDER_H__

#include "Elastos.Droid.Server_server.h"

namespace Elastos {
namespace Droid {
namespace Server {
namespace Wm {

class WindowBinder : public ElRefBase
{
public:
    WindowBinder();

    ~WindowBinder();

    CARAPI Register();

private:
    Int32 mNativeBinder;
};

} // namespace Wm
} // namespace Server
} // namespace Droid
} // namespace Elastos

#endif //__ELASTOS_DROID_SERVER_WM_WINDOWBINDER_H__
