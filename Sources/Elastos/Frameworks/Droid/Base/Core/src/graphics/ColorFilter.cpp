
#include "graphics/ColorFilter.h"
#include <SkColorFilter.h>
#include <SkiaColorFilter.h>

namespace Elastos {
namespace Droid {
namespace Graphics {

// {2C6350C4-CC6F-4B0C-AB2D-B43F37E35194}
extern const InterfaceID EIID_ColorFilter =
    { 0x2c6350c4, 0xcc6f, 0x4b0c, { 0xab, 0x2d, 0xb4, 0x3f, 0x37, 0xe3, 0x51, 0x94 } };

CAR_INTERFACE_IMPL(ColorFilter, Object, IColorFilter);
ColorFilter::~ColorFilter()
{
    Finalizer(mNativeInstance, mNativeColorFilter);
}

void ColorFilter::Finalizer(
    /* [in] */ Int32 nativeInstance,
    /* [in] */ Int32 nativeColorFilter)
{
    SkColorFilter* obj = reinterpret_cast<SkColorFilter*>(nativeInstance);
    android::uirenderer::SkiaColorFilter* filter =
            reinterpret_cast<android::uirenderer::SkiaColorFilter*>(nativeColorFilter);

    SkSafeUnref(obj);
    // filter == NULL when not !USE_OPENGL_RENDERER, so no need to delete outside the ifdef
#ifdef USE_OPENGL_RENDERER
    if (android::uirenderer::Caches::hasInstance()) {
        android::uirenderer::Caches::getInstance().resourceCache.destructor(filter);
    }
    else {
        delete filter;
    }
#endif
}

} // namespace Graphics
} // namepsace Droid
} // namespace Elastos
