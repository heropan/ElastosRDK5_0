
#ifndef __ELASTOS_DROID_GRAPHICS_CCOMPOSEPATHEFFECT_H__
#define __ELASTOS_DROID_GRAPHICS_CCOMPOSEPATHEFFECT_H__

#include "_CComposePathEffect.h"
#include "graphics/PathEffect.h"

namespace Elastos {
namespace Droid {
namespace Graphics {

CarClass(CComposePathEffect), public PathEffect
{
public:
    /**
     * Construct a PathEffect whose effect is to apply first the inner effect
     * and the the outer pathEffect (e.g. outer(inner(path))).
     */
    CARAPI constructor(
        /* [in] */ IPathEffect* outerpe,
        /* [in] */ IPathEffect* innerpe);

    CARAPI_(PInterface) Probe(
        /* [in] */ REIID riid);

private:
    static CARAPI_(Int32) NativeCreate(
        /* [in] */ Int32 outerpe,
        /* [in] */ Int32 innerpe);
};

} // namespace Graphics
} // namepsace Droid
} // namespace Elastos

#endif // __ELASTOS_DROID_GRAPHICS_CCOMPOSEPATHEFFECT_H__
