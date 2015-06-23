
#ifndef __CLAYERRASTERIZER_H__
#define __CLAYERRASTERIZER_H__

#include "_CLayerRasterizer.h"
#include "graphics/Rasterizer.h"

namespace Elastos {
namespace Droid {
namespace Graphics {

CarClass(CLayerRasterizer), public Rasterizer
{
public:
    CARAPI constructor();

    /**
     * Add a new layer (above any previous layers) to the rasterizer.
     * The layer will extract those fields that affect the mask from
     * the specified paint, but will not retain a reference to the paint
     * object itself, so it may be reused without danger of side-effects.
     */
    CARAPI AddLayer(
        /* [in] */ IPaint* paint,
        /* [in] */ Float dx,
        /* [in] */ Float dy);

    CARAPI AddLayer(
        /* [in] */ IPaint* paint);

    CARAPI_(PInterface) Probe(
        /* [in] */ REIID riid);

private:
    static CARAPI_(Int32) NativeConstructor();

    static CARAPI_(void) NativeAddLayer(
        /* [in] */ Int32 nativeLayer,
        /* [in] */ Int32 nativePaint,
        /* [in] */ Float dx,
        /* [in] */ Float dy);
};

} // namespace Graphics
} // namepsace Droid
} // namespace Elastos

#endif // __CLAYERRASTERIZER_H__
