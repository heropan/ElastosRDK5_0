
#include "ext/frameworkext.h"
#include "graphics/drawable/CLayerDrawable.h"

namespace Elastos {
namespace Droid {
namespace Graphics {
namespace Drawable {

IDRAWABLE_METHODS_IMPL(
    CLayerDrawable, LayerDrawable);

IDRAWABLECALLBACK_METHODS_IMPL(
    CLayerDrawable, LayerDrawable);

ECode CLayerDrawable::constructor(
    /* [in] */ ArrayOf<IDrawable*>* layers)
{
    return LayerDrawable::Init(layers);
}

ECode CLayerDrawable::constructor(
    /* [in] */ ArrayOf<IDrawable*>* layers,
    /* [in] */ IDrawableConstantState* state)
{
    return LayerDrawable::Init(layers, (LayerState*)state);
}

ECode CLayerDrawable::constructor(
    /* [in] */ IDrawableConstantState* state,
    /* [in] */ IResources* res)
{
    return LayerDrawable::Init((LayerState*)state, res);
}

PInterface CLayerDrawable::Probe(
    /* [in] */ REIID riid)
{
    return _CLayerDrawable::Probe(riid);
}

ECode CLayerDrawable::FindDrawableByLayerId(
    /* [in] */ Int32 id,
    /* [out] */ IDrawable** drawable)
{
    VALIDATE_NOT_NULL(drawable);
    AutoPtr<IDrawable> temp = LayerDrawable::FindDrawableByLayerId(id);
    *drawable = temp;
    INTERFACE_ADDREF(*drawable);

    return NOERROR;
}

ECode CLayerDrawable::SetId(
    /* [in] */ Int32 index,
    /* [in] */ Int32 id)
{
    return LayerDrawable::SetId(index, id);
}

ECode CLayerDrawable::GetNumberOfLayers(
    /* [out] */ Int32* number)
{
    VALIDATE_NOT_NULL(number);
    *number = LayerDrawable::GetNumberOfLayers();

    return NOERROR;
}

ECode CLayerDrawable::GetDrawable(
    /* [in] */ Int32 index,
    /* [out] */ IDrawable** drawable)
{
    VALIDATE_NOT_NULL(drawable);
    AutoPtr<IDrawable> temp = LayerDrawable::GetDrawable(index);
    *drawable = temp;
    INTERFACE_ADDREF(*drawable);

    return NOERROR;
}

ECode CLayerDrawable::GetId(
    /* [in] */ Int32 index,
    /* [out] */ Int32* id)
{
    VALIDATE_NOT_NULL(id);
    *id = LayerDrawable::GetId(index);

    return NOERROR;
}

ECode CLayerDrawable::SetDrawableByLayerId(
    /* [in] */ Int32 id,
    /* [in] */ IDrawable* drawable,
    /* [out] */ Boolean* res)
{
    VALIDATE_NOT_NULL(res);
    *res = LayerDrawable::SetDrawableByLayerId(id, drawable);

    return NOERROR;
}

ECode CLayerDrawable::SetLayerInset(
    /* [in] */ Int32 index,
    /* [in] */ Int32 l,
    /* [in] */ Int32 t,
    /* [in] */ Int32 r,
    /* [in] */ Int32 b)
{
    return LayerDrawable::SetLayerInset(index, l, t, r, b);
}

ECode CLayerDrawable::SetOpacity(
    /* [in] */ Int32 opacity)
{
    return LayerDrawable::SetOpacity(opacity);
}

ECode CLayerDrawable::constructor()
{
    return LayerDrawable::Init((LayerState*)NULL, NULL);
}

} // namespace Drawable
} // namespace Graphics
} // namespace Droid
} // namespace Elastos
