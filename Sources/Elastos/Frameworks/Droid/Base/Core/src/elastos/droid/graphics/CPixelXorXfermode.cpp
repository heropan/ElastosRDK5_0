
#include "graphics/CPixelXorXfermode.h"
#include <skia/effects/SkPixelXorXfermode.h>

namespace Elastos {
namespace Droid {
namespace Graphics {

ECode CPixelXorXfermode::constructor(
    /* [in] */ Int32 opColor)
{
    mNativeInstance = NativeCreate(opColor);
    return NOERROR;
}

PInterface CPixelXorXfermode::Probe(
    /* [in]  */ REIID riid)
{
    if (riid == EIID_Xfermode) {
        return reinterpret_cast<PInterface>((Xfermode*)this);
    }
    return _CPixelXorXfermode::Probe(riid);
}

Int32 CPixelXorXfermode::NativeCreate(
    /* [in] */ Int32 opColor)
{
    return (Int32)new SkPixelXorXfermode(opColor);
}

} // namespace Graphics
} // namepsace Droid
} // namespace Elastos
