
#ifndef __ELASTOS_DROID_GRAPHICS_CPIXELFORMAT_H__
#define __ELASTOS_DROID_GRAPHICS_CPIXELFORMAT_H__

#include "_Elastos_Droid_Graphics_CPixelFormat.h"
#include "graphics/PixelFormat.h"

namespace Elastos {
namespace Droid {
namespace Graphics {

CarClass(CPixelFormat), PixelFormat
{
public:
    CARAPI FormatHasAlpha(
        /* [in] */ Int32 format,
        /* [out] */ Boolean* result);

    CARAPI GetPixelFormatInfo(
        /* [in] */ Int32 format,
        /* [in] */ IPixelFormat* info);
};

} // namespace Graphics
} // namepsace Droid
} // namespace Elastos

#endif // __ELASTOS_DROID_GRAPHICS_CPIXELFORMAT_H__
