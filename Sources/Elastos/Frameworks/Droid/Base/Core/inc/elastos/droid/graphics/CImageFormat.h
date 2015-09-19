
#ifndef __ELASTOS_DROID_GRAPHICS_CIMAGEFORMAT_H__
#define __ELASTOS_DROID_GRAPHICS_CIMAGEFORMAT_H__

#include "_Elastos_Droid_Graphics_CImageFormat.h"
#include <elastos/core/Singleton.h>

using Elastos::Core::Singleton;

namespace Elastos {
namespace Droid {
namespace Graphics {

CarClass(CImageFormat)
    , public Singleton
    , public IImageFormat
{
public:
    CAR_INTERFACE_DECL();

    CAR_SINGLETON_DECL();

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
