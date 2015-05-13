
#ifndef __CPIXELFORMAT_H__
#define __CPIXELFORMAT_H__

#include "_CPixelFormat.h"
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

#endif // __CPIXELFORMAT_H__
