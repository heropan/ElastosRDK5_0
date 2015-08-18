
#ifndef __ELASTOS_DROID_GRAPHICS_CPIXELXORXFERMODE_H__
#define __ELASTOS_DROID_GRAPHICS_CPIXELXORXFERMODE_H__

#include "_Elastos_Droid_Graphics_CPixelXorXfermode.h"
#include "graphics/Xfermode.h"

namespace Elastos {
namespace Droid {
namespace Graphics {

/**
 * PixelXorXfermode implements a simple pixel xor (op ^ src ^ dst).
 * This transformation does not follow premultiplied conventions, therefore
 * this mode *always* returns an opaque color (alpha == 255). Thus it is
 * not really usefull for operating on blended colors.
 */
CarClass(CPixelXorXfermode), public Xfermode
{
public:
    CARAPI constructor(
        /* [in] */ Int32 opColor);

    CARAPI_(PInterface) Probe(
            /* [in]  */ REIID riid);

private:
    static CARAPI_(Int32) NativeCreate(
        /* [in] */ Int32 opColor);
};

} // namespace Graphics
} // namepsace Droid
} // namespace Elastos

#endif // __ELASTOS_DROID_GRAPHICS_CPIXELXORXFERMODE_H__
