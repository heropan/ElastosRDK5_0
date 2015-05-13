#include "CEGL10Helper.h"
#include "CEGLDisplayImpl.h"
#include "CEGLContextImpl.h"
#include "CEGLSurfaceImpl.h"
#include "ext/frameworkext.h"

namespace Elastos {
namespace Droid {
namespace Opengl {
namespace Gles {

static AutoPtr<CEGLDisplayImpl> InitDisplay()
{
    AutoPtr<CEGLDisplayImpl> display;
    CEGLDisplayImpl::NewByFriend(0, (CEGLDisplayImpl**)&display);
    return display;
}

static AutoPtr<CEGLContextImpl> InitContext()
{
    AutoPtr<CEGLContextImpl> display;
    CEGLContextImpl::NewByFriend(0, (CEGLContextImpl**)&display);
    return display;
}

static AutoPtr<CEGLSurfaceImpl> InitSurface()
{
    AutoPtr<CEGLSurfaceImpl> display;
    CEGLSurfaceImpl::NewByFriend(0, (CEGLSurfaceImpl**)&display);
    return display;
}


AutoPtr<IInterface> CEGL10Helper::sDefaultDisplay = NULL;
AutoPtr<Elastosx::Microedition::Khronos::Egl::IEGLDisplay> CEGL10Helper::sNoDisplay = InitDisplay();
AutoPtr<Elastosx::Microedition::Khronos::Egl::IEGLContext> CEGL10Helper::sNoContext = InitContext();
AutoPtr<Elastosx::Microedition::Khronos::Egl::IEGLSurface> CEGL10Helper::sNoSurface = InitSurface();

ECode CEGL10Helper::GetDefaultDisplay(
    /* [out] */ IInterface** disp)
{
    *disp = sDefaultDisplay;
    INTERFACE_ADDREF(*disp);
    return NOERROR;
}

ECode CEGL10Helper::GetNoDisplay(
    /* [out] */ XIEGLDisplay** disp)
{
    *disp = sNoDisplay;
    INTERFACE_ADDREF(*disp);
    return NOERROR;
}

ECode CEGL10Helper::GetNoContext(
    /* [out] */ XIEGLContext** ctx)
{
    *ctx = sNoContext;
    INTERFACE_ADDREF(*ctx);
    return NOERROR;
}

ECode CEGL10Helper::GetNoSurface(
    /* [out] */ XIEGLSurface** sfc)
{
    *sfc = sNoSurface;
    INTERFACE_ADDREF(*sfc);
    return NOERROR;
}


} // namespace Gles
} // namespace Opengl
} // namespace Droid
} // namespace Elastos