
#include "ext/frameworkext.h"
#include "graphics/drawable/CColorDrawable.h"

namespace Elastos {
namespace Droid {
namespace Graphics {
namespace Drawable {

IDRAWABLE_METHODS_IMPL(CColorDrawable, ColorDrawable);

ECode CColorDrawable::constructor()
{
    return ColorDrawable::Init((ColorState*)NULL);
}

ECode CColorDrawable::constructor(
    /* [in] */ Int32 color)
{
    return ColorDrawable::Init(color);
}

ECode CColorDrawable::constructor(
    /* [in] */ IDrawableConstantState* state)
{
    return ColorDrawable::Init((ColorState*)state);
}

PInterface CColorDrawable::Probe(
    /* [in] */ REIID riid)
{
    return _CColorDrawable::Probe(riid);
}

ECode CColorDrawable::GetColor(
    /* [out] */ Int32* color)
{
    return ColorDrawable::GetColor(color);
}

ECode CColorDrawable::SetColor(
    /* [in] */ Int32 color)
{
    return ColorDrawable::SetColor(color);
}

ECode CColorDrawable::GetAlpha(
    /* [out] */ Int32* alpha)
{
    VALIDATE_NOT_NULL(alpha);
    *alpha = ColorDrawable::GetAlpha();

    return NOERROR;
}

} // namespace Drawable
} // namespace Graphics
} // namespace Droid
} // namespace Elastos
