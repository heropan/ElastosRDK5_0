
#ifndef __ELASTOS_DROID_GRAPHICS_DRAWABLE_CANIMATEDROTATEDRAWABLE_H__
#define __ELASTOS_DROID_GRAPHICS_DRAWABLE_CANIMATEDROTATEDRAWABLE_H__

#include "_Elastos_Droid_Graphics_Drawable_CAnimatedRotateDrawable.h"
#include "graphics/drawable/AnimatedRotateDrawable.h"


namespace Elastos {
namespace Droid {
namespace Graphics {
namespace Drawable {

CarClass(CAnimatedRotateDrawable), AnimatedRotateDrawable
{
public:
    CARAPI constructor();

    CARAPI constructor(
        /* [in] */ IDrawableConstantState* state,
        /* [in] */ IResources* res);

    CARAPI_(PInterface) Probe(
        /* [in] */ REIID riid);

    IDRAWABLE_METHODS_DECL()

    IDRAWABLECALLBACK_METHODS_DECL()

    CARAPI Start();

    CARAPI Stop();

    CARAPI IsRunning(
        /* [out] */ Boolean* isRunning);

    CARAPI Run();

    CARAPI GetDrawable(
        /* [out] */ IDrawable** drawable);

    CARAPI SetFramesCount(
        /* [in] */ Int32 framesCount);

    CARAPI SetFramesDuration(
        /* [in] */ Int32 framesDuration);
};

} // namespace Drawable
} // namespace Graphics
} // namespace Droid
} // namespace Elastos

#endif // __ELASTOS_DROID_GRAPHICS_DRAWABLE_CANIMATEDROTATEDRAWABLE_H__

