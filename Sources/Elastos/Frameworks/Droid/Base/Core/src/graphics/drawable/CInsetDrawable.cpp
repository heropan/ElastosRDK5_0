
#include "graphics/drawable/CInsetDrawable.h"

namespace Elastos {
namespace Droid {
namespace Graphics {
namespace Drawable {

ECode CInsetDrawable::constructor()
{
    return InsetDrawable::Init();
}

ECode CInsetDrawable::constructor(
    /* [in] */ IDrawable* drawable,
    /* [in] */ Int32 inset)
{
    return InsetDrawable::Init(drawable, inset);
}

ECode CInsetDrawable::constructor(
    /* [in] */ IDrawable* drawable,
    /* [in] */ Int32 insetLeft,
    /* [in] */ Int32 insetTop,
    /* [in] */ Int32 insetRight,
    /* [in] */ Int32 insetBottom)
{
    return InsetDrawable::Init(
            drawable, insetLeft, insetTop, insetRight, insetBottom);
}

ECode CInsetDrawable::constructor(
    /* [in] */ Handle32 state,
    /* [in] */ IResources* res)
{
    return InsetDrawable::Init((InsetState*)state, res);
}

PInterface CInsetDrawable::Probe(
    /* [in] */ REIID riid)
{
    return _CInsetDrawable::Probe(riid);
}

IDRAWABLE_METHODS_IMPL(
    CInsetDrawable, InsetDrawable);

IDRAWABLECALLBACK_METHODS_IMPL(
    CInsetDrawable, InsetDrawable);

} // namespace Drawable
} // namespace Graphics
} // namespace Droid
} // namespace Elastos
