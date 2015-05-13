
#include "CGLDebugHelper.h"
#include "GLErrorWrapper.h"
#include "GLLogWrapper.h"
#include "ext/frameworkext.h"
#include "EGLLogWrapper.h"

using Elastosx::Microedition::Khronos::Opengles::EIID_IGL;

namespace Elastos {
namespace Droid {
namespace Opengl {

ECode CGLDebugHelper::Wrap(
    /* [in] */ IGL* gl,
    /* [in] */ Int32 configFlags,
    /* [in] */ Elastos::IO::IWriter* log,
    /* [out] */ IGL** glInstance)
{
    AutoPtr<IGL> rst;
    if ( configFlags != 0 ) {
        rst = (IGL*)(new GLErrorWrapper(gl, configFlags))->Probe(EIID_IGL);
    }
    if ( log != NULL ) {
        Boolean logArgumentNames =
            (IGLDebugHelper::_CONFIG_LOG_ARGUMENT_NAMES & configFlags) != 0;
        rst = (IGL*)(new GLLogWrapper(gl, log, logArgumentNames))->Probe(EIID_IGL);
    }
    *glInstance = rst;
    INTERFACE_ADDREF(*glInstance)
    return NOERROR;
}

ECode CGLDebugHelper::WrapEx(
    /* [in] */ IEGL* egl,
    /* [in] */ Int32 configFlags,
    /* [in] */ Elastos::IO::IWriter* log,
    /* [out] */ IEGL** eglInterface)
{
    if (log != NULL) {
        *eglInterface = new EGLLogWrapper(egl, configFlags, log);
    }
    INTERFACE_ADDREF(*eglInterface)
    return NOERROR;
}

} // namespace Opengl
} // namespace Droid
} // namespace Elastos

