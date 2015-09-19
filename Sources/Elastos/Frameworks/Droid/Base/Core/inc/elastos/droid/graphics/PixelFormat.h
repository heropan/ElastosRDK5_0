#ifndef __ELASTOS_DROID_GRAPHICS_PIXELFORMAT_H__
#define __ELASTOS_DROID_GRAPHICS_PIXELFORMAT_H__

#include "Elastos.Droid.Core_server.h"

namespace Elastos {
namespace Droid {
namespace Graphics {

class PixelFormat
{
public:
    static CARAPI_(Boolean) FormatHasAlpha(
        /* [in] */ Int32 format);

    static CARAPI GetPixelFormatInfo(
        /* [in] */ Int32 format,
        /* [in] */ IPixelFormat* info);

public:
    Int32  mBytesPerPixel;
    Int32  mBitsPerPixel;
};

} // namespace Graphics
} // namepsace Droid
} // namespace Elastos

#endif // __ELASTOS_DROID_GRAPHICS_PIXELFORMAT_H__
