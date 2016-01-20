#ifndef __ELASTOS_DROID_OPENGL_GLES_CEGL10HELPER_H__
#define __ELASTOS_DROID_OPENGL_GLES_CEGL10HELPER_H__

#include "_Elastos_Droid_Opengl_Gles_CEGL10Helper.h"
#include "Elastos.Droid.Opengl.h"

#include <elastos/core/Singleton.h>

using Elastos::Core::Singleton;
using Elastosx::Microedition::Khronos::Egl::IEGL10Helper;

namespace Elastos {
namespace Droid {
namespace Opengl {
namespace Gles {

CarClass(CEGL10Helper)
    , public Singleton
    , public IEGL10Helper
{
typedef Elastosx::Microedition::Khronos::Egl::IEGLContext XIEGLContext;
typedef Elastosx::Microedition::Khronos::Egl::IEGLDisplay XIEGLDisplay;
typedef Elastosx::Microedition::Khronos::Egl::IEGLSurface XIEGLSurface;

public:
    CAR_INTERFACE_DECL()

    CAR_SINGLETON_DECL()

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
