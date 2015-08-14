
#ifndef __ELASTOS_DROID_GRAPHICS_DRAWABLE_CLAYERDRAWABLE_H__
#define __ELASTOS_DROID_GRAPHICS_DRAWABLE_CLAYERDRAWABLE_H__

#include "_CLayerDrawable.h"
#include "graphics/drawable/LayerDrawable.h"


namespace Elastos {
namespace Droid {
namespace Graphics {
namespace Drawable {

CarClass(CLayerDrawable), LayerDrawable
{
public:
    CARAPI constructor();

    CARAPI constructor(
        /* [in] */ ArrayOf<IDrawable*>* layers);

    CARAPI constructor(
        /* [in] */ ArrayOf<IDrawable*>* layers,
        /* [in] */ IDrawableConstantState* state);

    CARAPI constructor(
        /* [in] */ IDrawableConstantState* state,
        /* [in] */ IResources* res);

    PInterface Probe(
        /* [in] */ REIID riid);

    IDRAWABLE_METHODS_DECL()

    IDRAWABLECALLBACK_METHODS_DECL()

    CARAPI FindDrawableByLayerId(
        /* [in] */ Int32 id,
        /* [out] */ IDrawable** drawable);

    CARAPI SetId(
        /* [in] */ Int32 index,
        /* [in] */ Int32 id);

    CARAPI GetNumberOfLayers(
        /* [out] */ Int32* number);

    CARAPI GetDrawable(
        /* [in] */ Int32 index,
        /* [out] */ IDrawable** drawable);

    CARAPI GetId(
        /* [in] */ Int32 index,
        /* [out] */ Int32* id);

    CARAPI SetDrawableByLayerId(
        /* [in] */ Int32 id,
        /* [in] */ IDrawable* drawable,
        /* [out] */ Boolean* res);

    CARAPI SetLayerInset(
        /* [in] */ Int32 index,
        /* [in] */ Int32 l,
        /* [in] */ Int32 t,
        /* [in] */ Int32 r,
        /* [in] */ Int32 b);

    CARAPI SetOpacity(
        /* [in] */ Int32 opacity);
};

} // namespace Drawable
} // namespace Graphics
} // namespace Droid
} // namespace Elastos

#endif // __ELASTOS_DROID_GRAPHICS_DRAWABLE_CLAYERDRAWABLE_H__
