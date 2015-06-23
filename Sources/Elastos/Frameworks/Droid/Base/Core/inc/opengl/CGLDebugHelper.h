
#ifndef __CGLDEBUGHELPER_H__
#define __CGLDEBUGHELPER_H__

#include "_CGLDebugHelper.h"

using Elastosx::Microedition::Khronos::Opengles::IGL;
using Elastosx::Microedition::Khronos::Egl::IEGL;

namespace Elastos {
namespace Droid {
namespace Opengl {

CarClass(CGLDebugHelper)
{
public:
    CARAPI Wrap(
        /* [in] */ IGL* gl,
        /* [in] */ Int32 configFlags,
        /* [in] */ Elastos::IO::IWriter* log,
        /* [out] */ IGL** glInstance);

    CARAPI Wrap(
        /* [in] */ IEGL* egl,
        /* [in] */ Int32 configFlags,
        /* [in] */ Elastos::IO::IWriter* log,
        /* [out] */ IEGL** eglInterface);

private:
    // TODO: Add your private member variables here.
};

} // namespace Opengl
} // namespace Droid
} // namespace Elastos

#endif // __CGLDEBUGHELPER_H__
