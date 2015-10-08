#include "graphics/Rasterizer.h"
#include <SkRasterizer.h>

namespace Elastos {
namespace Droid {
namespace Graphics {

// {2D3A6B44-0939-4A44-A9C2-6C1E9E71D8FC}
extern const InterfaceID EIID_Rasterizer =
{ 0x2d3a6b44, 0x939, 0x4a44, { 0xa9, 0xc2, 0x6c, 0x1e, 0x9e, 0x71, 0xd8, 0xfc } };

CAR_INTERFACE_IMPL(Rasterizer, Object, IRasterizer);
Rasterizer::~Rasterizer()
{
    Finalizer(mNativeInstance);
}

void Rasterizer::Finalizer(
    /* [in] */ Int64 nativeInstance)
{
    assert(0 && "TODO");
    // delete reinterpret_cast<NativeRasterizer *>(objHandle);
}

} // namespace Graphics
} // namepsace Droid
} // namespace Elastos
