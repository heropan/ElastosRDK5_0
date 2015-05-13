
#include "ext/frameworkext.h"
#include "graphics/drawable/CClipDrawable.h"

namespace Elastos {
namespace Droid {
namespace Graphics {
namespace Drawable {

ECode CClipDrawable::constructor()
{
    return ClipDrawable::Init();
}

/**
 * @param orientation Bitwise-or of {@link #HORIZONTAL} and/or {@link #VERTICAL}
 */
ECode CClipDrawable::constructor(
    /* [in] */ IDrawable* drawable,
    /* [in] */ Int32 gravity,
    /* [in] */ Int32 orientation)
{
    return ClipDrawable::Init(drawable, gravity, orientation);
}

ECode CClipDrawable::constructor(
    /* [in] */ IDrawableConstantState* state,
    /* [in] */ IResources* res)
{
    return ClipDrawable::Init((ClipState*)state, res);
}

PInterface CClipDrawable::Probe(
    /* [in] */ REIID riid)
{
    return _CClipDrawable::Probe(riid);
}

IDRAWABLE_METHODS_IMPL(CClipDrawable, ClipDrawable);

IDRAWABLECALLBACK_METHODS_IMPL(CClipDrawable, ClipDrawable);

} // namespace Drawable
} // namespace Graphics
} // namespace Droid
} // namespace Elastos
