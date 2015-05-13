#ifndef __CEGL10HELPER_H__
#define __CEGL10HELPER_H__

#include "_CEGL10Helper.h"

namespace Elastos {
namespace Droid {
namespace Opengl {
namespace Gles {

CarClass(CEGL10Helper)
{
typedef Elastosx::Microedition::Khronos::Egl::IEGLContext XIEGLContext;
typedef Elastosx::Microedition::Khronos::Egl::IEGLDisplay XIEGLDisplay;
typedef Elastosx::Microedition::Khronos::Egl::IEGLSurface XIEGLSurface;

public:
    CARAPI GetDefaultDisplay(
        /* [out] */ IInterface** disp);

    CARAPI GetNoDisplay(
        /* [out] */ XIEGLDisplay** disp);

    CARAPI GetNoContext(
        /* [out] */ XIEGLContext** disp);

    CARAPI GetNoSurface(
        /* [out] */ XIEGLSurface** disp);

private:
    static AutoPtr<IInterface> sDefaultDisplay;
    static AutoPtr<XIEGLDisplay> sNoDisplay;
    static AutoPtr<XIEGLContext> sNoContext;
    static AutoPtr<XIEGLSurface> sNoSurface;
};

} // namespace Gles
} // namespace Opengl
} // namespace Droid
} // namespace Elastos
#endif