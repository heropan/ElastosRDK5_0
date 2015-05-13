
#include "graphics/drawable/CRotateDrawable.h"

namespace Elastos {
namespace Droid {
namespace Graphics {
namespace Drawable {

ECode CRotateDrawable::constructor()
{
    return RotateDrawable::Init();
}

ECode CRotateDrawable::constructor(
    /* [in] */ Handle32 state,
    /* [in] */ IResources* res)
{
    return RotateDrawable::Init((RotateState*)state, res);
}

PInterface CRotateDrawable::Probe(
    /* [in] */ REIID riid)
{
    return _CRotateDrawable::Probe(riid);
}

IDRAWABLE_METHODS_IMPL(
    CRotateDrawable, RotateDrawable);

IDRAWABLECALLBACK_METHODS_IMPL(
    CRotateDrawable, RotateDrawable);

ECode CRotateDrawable::GetDrawable(
    /* [out] */ IDrawable** drawable)
{
    VALIDATE_NOT_NULL(drawable);
    AutoPtr<IDrawable> d = RotateDrawable::GetDrawable();
    *drawable = d;
    INTERFACE_ADDREF(*drawable);
    return NOERROR;
}

} // namespace Drawable
} // namespace Graphics
} // namespace Droid
} // namespace Elastos
