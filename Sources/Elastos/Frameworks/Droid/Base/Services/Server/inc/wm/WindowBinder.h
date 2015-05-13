
#ifndef __WINDOWBINDER_H__
#define __WINDOWBINDER_H__

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

#endif //__WINDOWBINDER_H__
