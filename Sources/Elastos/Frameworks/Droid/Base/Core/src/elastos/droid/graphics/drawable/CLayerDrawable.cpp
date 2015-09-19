
#include "ext/frameworkext.h"
#include "graphics/drawable/CLayerDrawable.h"

namespace Elastos {
namespace Droid {
namespace Graphics {
namespace Drawable {

CAR_OBJECT_IMPL(CLayerDrawable);
ECode CLayerDrawable::constructor()
{
    assert(0 && "TODO");
    return NOERROR;
}
ECode CLayerDrawable::constructor(
    /* [in] */ ArrayOf<IDrawable*>* layers)
{
    return LayerDrawable::constructor(layers);
}

ECode CLayerDrawable::constructor(
    /* [in] */ ArrayOf<IDrawable*>* layers,
    /* [in] */ IDrawableConstantState* state)
{
    return LayerDrawable::constructor(layers, (LayerState*)state);
}

ECode CLayerDrawable::constructor(
    /* [in] */ IDrawableConstantState* state,
    /* [in] */ IResources* res)
{
    return LayerDrawable::constructor((LayerState*)state, res);
}

} // namespace Drawable
} // namespace Graphics
} // namespace Droid
} // namespace Elastos
