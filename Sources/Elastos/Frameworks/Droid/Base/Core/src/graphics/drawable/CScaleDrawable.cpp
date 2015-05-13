
#include "ext/frameworkext.h"
#include "graphics/drawable/CScaleDrawable.h"

namespace Elastos {
namespace Droid {
namespace Graphics {
namespace Drawable {

ECode CScaleDrawable::constructor()
{
    return ScaleDrawable::Init();
}

ECode CScaleDrawable::constructor(
    /* [in] */ IDrawable* drawable,
    /* [in] */ Int32 gravity,
    /* [in] */ Float scaleWidth,
    /* [in] */ Float scaleHeight)
{
    return ScaleDrawable::Init(
            drawable, gravity, scaleWidth, scaleHeight);
}

ECode CScaleDrawable::constructor(
    /* [in] */ Handle32 state,
    /* [in] */ IResources* res)
{
    return ScaleDrawable::Init((ScaleState*)state, res);
}

PInterface CScaleDrawable::Probe(
    /* [in] */ REIID riid)
{
    return _CScaleDrawable::Probe(riid);
}

IDRAWABLE_METHODS_IMPL(
    CScaleDrawable, ScaleDrawable);

IDRAWABLECALLBACK_METHODS_IMPL(
    CScaleDrawable, ScaleDrawable);

/**
 * Returns the drawable scaled by this ScaleDrawable.
 */
ECode CScaleDrawable::GetDrawable(
    /* [out] */ IDrawable** drawable)
{
    VALIDATE_NOT_NULL(drawable);
    AutoPtr<IDrawable> d = ScaleDrawable::GetDrawable();
    *drawable = d;
    INTERFACE_ADDREF(*drawable);
    return NOERROR;
}

} // namespace Drawable
} // namespace Graphics
} // namespace Droid
} // namespace Elastos
