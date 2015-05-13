
#ifndef __CGLSURFACEVIEW_H__
#define __CGLSURFACEVIEW_H__

#include "_CGLSurfaceView.h"
#include "GLSurfaceView.h"
#include "view/ViewMacro.h"
#include "view/SurfaceViewMacro.h"
#include "GLSurfaceViewMacro.h"

namespace Elastos {
namespace Droid {
namespace Opengl {

CarClass(CGLSurfaceView) , public GLSurfaceView
{
public:
    IVIEW_METHODS_DECL()
    IDRAWABLECALLBACK_METHODS_DECL()
    IKEYEVENTCALLBACK_METHODS_DECL()
    IACCESSIBILITYEVENTSOURCE_METHODS_DECL()
    ISURFACEVIEW_METHODS_DECL()
    ISURFACEHOLDERCALLBACK_METHODS_DECL()
    IGLSURFACEVIEW_METHODS_DECL()

    CARAPI_(PInterface) Probe(
        /* [in] */ REIID riid);

    CARAPI constructor(
        /* [in] */ IContext* context);

    CARAPI constructor(
        /* [in] */ IContext* context,
        /* [in] */ IAttributeSet* attrs);

    CARAPI GetWeakReference(
        /* [out] */ IWeakReference** weakReference);
};

} // namespace Opengl
} // namespace Droid
} // namespace Elastos

#endif // __CGLSURFACEVIEW_H__