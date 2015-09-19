
#include "graphics/PixelFormat.h"
#include "graphics/CPixelFormat.h"
#include <ui/PixelFormat.h>

namespace Elastos {
namespace Droid {
namespace Graphics {

Boolean PixelFormat::FormatHasAlpha(
    /* [in] */ Int32 format)
{
    switch (format) {
        case IPixelFormat::A_8:
        case IPixelFormat::LA_88:
        case IPixelFormat::RGBA_4444:
        case IPixelFormat::RGBA_5551:
        case IPixelFormat::RGBA_8888:
        case IPixelFormat::TRANSLUCENT:
        case IPixelFormat::TRANSPARENT:
            return TRUE;
    }
    return FALSE;
}

ECode PixelFormat::GetPixelFormatInfo(
    /* [in] */ Int32 format,
    /* [in] */ IPixelFormat* infoObj)
{
    android::PixelFormatInfo info;
    android::status_t err;

    // we need this for backward compatibility with PixelFormat's
    // deprecated constants
    switch (format) {
    case HAL_PIXEL_FORMAT_YCbCr_422_SP:
        // defined as the bytes per pixel of the Y plane
        info.bytesPerPixel = 1;
        info.bitsPerPixel = 16;
        goto done;
    case HAL_PIXEL_FORMAT_YCrCb_420_SP:
        // defined as the bytes per pixel of the Y plane
        info.bytesPerPixel = 1;
        info.bitsPerPixel = 12;
        goto done;
    case HAL_PIXEL_FORMAT_YCbCr_422_I:
        // defined as the bytes per pixel of the Y plane
        info.bytesPerPixel = 1;
        info.bitsPerPixel = 16;
        goto done;
    }

    err = android::getPixelFormatInfo(format, &info);
    if (err < 0) {
        // jniThrowException(env, "java/lang/IllegalArgumentException", NULL);
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }

done:
    ((PixelFormat*)(CPixelFormat*)infoObj)->mBytesPerPixel = info.bytesPerPixel;
    ((PixelFormat*)(CPixelFormat*)infoObj)->mBitsPerPixel = info.bitsPerPixel;
    return NOERROR;
}

} // namespace Graphics
} // namepsace Droid
} // namespace Elastos
