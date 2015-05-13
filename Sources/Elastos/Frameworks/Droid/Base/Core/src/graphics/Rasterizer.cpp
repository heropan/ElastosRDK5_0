#include "graphics/Rasterizer.h"
#include <SkRasterizer.h>

namespace Elastos {
namespace Droid {
namespace Graphics {

// {2D3A6B44-0939-4A44-A9C2-6C1E9E71D8FC}
extern const InterfaceID EIID_Rasterizer =
{ 0x2d3a6b44, 0x939, 0x4a44, { 0xa9, 0xc2, 0x6c, 0x1e, 0x9e, 0x71, 0xd8, 0xfc } };

Rasterizer::~Rasterizer()
{
    Finalizer(mNativeInstance);
}

void Rasterizer::Finalizer(
    /* [in] */ Int32 nativeInstance)
{
    SkRasterizer* obj = reinterpret_cast<SkRasterizer*>(nativeInstance);
    SkSafeUnref(obj);
}

} // namespace Graphics
} // namepsace Droid
} // namespace Elastos
