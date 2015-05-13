
#include "ext/frameworkext.h"
#include "graphics/CCanvas.h"

namespace Elastos {
namespace Droid {
namespace Graphics {

ECode CCanvas::constructor()
{
    return Canvas::Init();
}

ECode CCanvas::constructor(
    /* [in] */ IBitmap* bitmap)
{
    return Canvas::Init(bitmap);
}

ECode CCanvas::constructor(
    /* [in] */ Int32 nativeCanvas)
{
    return Canvas::Init(nativeCanvas);
}

PInterface CCanvas::Probe(
    /* [in] */ REIID riid)
{
    if (riid == EIID_Canvas) {
        return reinterpret_cast<PInterface>((Canvas*)this);
    }
    return _CCanvas::Probe(riid);
}

ICANVAS_METHODS_IMPL(CCanvas, Canvas);

} // namespace Graphics
} // namepsace Droid
} // namespace Elastos