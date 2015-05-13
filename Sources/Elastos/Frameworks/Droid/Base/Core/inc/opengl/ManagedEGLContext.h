#ifndef __MANAGEDEGLCONTEXT_H__
#define __MANAGEDEGLCONTEXT_H__

#include "elastos/Mutex.h"
#include "Elastos.Droid.Core_server.h"
#include "ext/frameworkext.h"
#include "elastos/List.h"

using Elastos::Utility::List;
using Elastos::Core::Threading::Mutex;

namespace Elastos {
namespace Droid {
namespace Opengl {

class ManagedEGLContext
    : public IManagedEGLContext
    , public ElRefBase
{
typedef Elastosx::Microedition::Khronos::Egl::IEGLConfig XIEGLConfig;
typedef Elastosx::Microedition::Khronos::Egl::IEGLContext XIEGLContext;
typedef Elastosx::Microedition::Khronos::Egl::IEGLDisplay XIEGLDisplay;
typedef Elastosx::Microedition::Khronos::Egl::IEGLSurface XIEGLSurface;

public:
    CAR_INTERFACE_DECL()

    ManagedEGLContext(
        /* [in] */ XIEGLContext* ctx);

    CARAPI GetContext(
        /* [out] */ XIEGLContext** ctx);

    CARAPI Terminate();

    CARAPI ExecTerminate();

    static CARAPI DoTerminate(
        /* [out] */ Boolean* rst);

private:
    AutoPtr<XIEGLContext> mContext;
    static Mutex sLock;
    static List<AutoPtr<ManagedEGLContext> > sActive;
};

} // namespace Opengl
} // namespace Droid
} // namespace Elastos

#endif //__MANAGEDEGLCONTEXT_H__