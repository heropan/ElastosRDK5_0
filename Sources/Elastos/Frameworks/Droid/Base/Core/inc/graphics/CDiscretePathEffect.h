
#ifndef __ELASTOS_DROID_GRAPHICS_CDISCRETEPATHEFFECT_H__
#define __ELASTOS_DROID_GRAPHICS_CDISCRETEPATHEFFECT_H__

#include "_CDiscretePathEffect.h"
#include "graphics/PathEffect.h"

namespace Elastos {
namespace Droid {
namespace Graphics {

CarClass(CDiscretePathEffect), public PathEffect
{
public:
    /**
     * Chop the path into lines of segmentLength, randomly deviating from the
     * original path by deviation.
     */
    CARAPI constructor(
        /* [in] */ Float segmentLength,
        /* [in] */ Float deviation);

    CARAPI_(PInterface) Probe(
        /* [in] */ REIID riid);

private:
    static CARAPI_(Int32) NativeCreate(
        /* [in] */ Float length,
        /* [in] */ Float deviation);
};

} // namespace Graphics
} // namepsace Droid
} // namespace Elastos

#endif // __ELASTOS_DROID_GRAPHICS_CDISCRETEPATHEFFECT_H__
