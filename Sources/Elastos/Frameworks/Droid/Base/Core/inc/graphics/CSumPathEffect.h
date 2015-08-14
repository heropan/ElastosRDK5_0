
#ifndef __ELASTOS_DROID_GRAPHICS_CSUMPATHEFFECT_H__
#define __ELASTOS_DROID_GRAPHICS_CSUMPATHEFFECT_H__

#include "_CSumPathEffect.h"
#include "graphics/PathEffect.h"

namespace Elastos {
namespace Droid {
namespace Graphics {

CarClass(CSumPathEffect), public PathEffect
{
public:
    /**
     * Construct a PathEffect whose effect is to apply two effects, in sequence.
     * (e.g. first(path) + second(path))
     */
    CARAPI constructor(
        /* [in] */ IPathEffect* first,
        /* [in] */ IPathEffect* second);

    CARAPI_(PInterface) Probe(
        /* [in] */ REIID riid);

private:
    static CARAPI_(Int32) NativeCreate(
        /* [in] */ Int32 first,
        /* [in] */ Int32 second);
};

} // namespace Graphics
} // namepsace Droid
} // namespace Elastos

#endif // __ELASTOS_DROID_GRAPHICS_CSUMPATHEFFECT_H__
