
#ifndef __CCANVAS_H__
#define __CCANVAS_H__

#include "_CCanvas.h"
#include "graphics/Canvas.h"
#include "graphics/CanvasMacro.h"

namespace Elastos {
namespace Droid {
namespace Graphics {

CarClass(CCanvas), public Canvas
{
public:
    CARAPI constructor();

    CARAPI constructor(
        /* [in] */ IBitmap* bitmap);

    CARAPI constructor(
        /* [in] */ Int32 nativeCanvas);

    CARAPI_(PInterface) Probe(
        /* [in] */ REIID riid);

    ICANVAS_METHODS_DECL();
};

} // namespace Graphics
} // namepsace Droid
} // namespace Elastos

#endif // __CCANVAS_H__
