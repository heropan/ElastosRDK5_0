
#include "graphics/CImageFormat.h"

namespace Elastos {
namespace Droid {
namespace Graphics {

CAR_SINGLETON_IMPL(CImageFormat);
CAR_INTERFACE_IMPL(CImageFormat, Singleton, IImageFormat);
ECode CImageFormat::GetBitsPerPixel(
    /* [in] */ Int32 format,
    /* [out] */ Int32* bits)
{
    switch (format) {
        case IImageFormat::RGB_565:
            *bits = 16;
            break;
        case IImageFormat::NV16:
            *bits = 16;
            break;
        case IImageFormat::YUY2:
            *bits = 16;
            break;
        case IImageFormat::YV12:
            *bits = 12;
            break;
        case IImageFormat::NV21:
            *bits = 12;
            break;
        case IImageFormat::BAYER_RGGB:
            *bits = 16;
            break;
        default:
            *bits = -1;
    }

    return NOERROR;
}

} // namespace Graphics
} // namepsace Droid
} // namespace Elastos