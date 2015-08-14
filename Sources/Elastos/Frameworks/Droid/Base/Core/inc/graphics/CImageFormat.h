
#ifndef __ELASTOS_DROID_GRAPHICS_CIMAGEFORMAT_H__
#define __ELASTOS_DROID_GRAPHICS_CIMAGEFORMAT_H__

#include "_CImageFormat.h"

namespace Elastos {
namespace Droid {
namespace Graphics {

CarClass(CImageFormat)
{
public:
    /**
     * Use this function to retrieve the number of bits per pixel of an
     * ImageFormat.
     *
     * @param format
     * @return the number of bits per pixel of the given format or -1 if the
     *         format doesn't exist or is not supported.
     */
    CARAPI GetBitsPerPixel(
        /* [in] */ Int32 format,
        /* [out] */ Int32* bits);
};

} // namespace Graphics
} // namepsace Droid
} // namespace Elastos

#endif // __ELASTOS_DROID_GRAPHICS_CIMAGEFORMAT_H__
