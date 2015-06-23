
#include "ext/frameworkext.h"
#include "graphics/drawable/shapes/CRoundRectShape.h"

namespace Elastos {
namespace Droid {
namespace Graphics {
namespace Drawable {
namespace Shapes {


ECode CRoundRectShape::constructor(
    /* [in] */ ArrayOf<Float>* outerRadii,
    /* [in] */ IRectF* inset,
    /* [in] */ ArrayOf<Float>* innerRadii)
{
    return RoundRectShape::Init(outerRadii, inset, innerRadii);
}

ECode CRoundRectShape::GetWidth(
    /* [out] */ Float* width)
{
    VALIDATE_NOT_NULL(width);
    *width = RoundRectShape::GetWidth();
    return NOERROR;
}

ECode CRoundRectShape::GetHeight(
    /* [out] */ Float* height)
{
    VALIDATE_NOT_NULL(height);
    *height = RoundRectShape::GetHeight();
    return NOERROR;
}

ECode CRoundRectShape::Draw(
    /* [in] */ ICanvas* canvas,
    /* [in] */ IPaint* paint)
{
    return RoundRectShape::Draw(canvas, paint);
}

ECode CRoundRectShape::Resize(
    /* [in] */ Float width,
    /* [in] */ Float height)
{
    return RoundRectShape::Resize(width, height);
}

ECode CRoundRectShape::HasAlpha(
    /* [out] */ Boolean* hasAlpha)
{
    VALIDATE_NOT_NULL(hasAlpha);
    *hasAlpha = RoundRectShape::HasAlpha();
    return NOERROR;
}

ECode CRoundRectShape::Clone(
    /* [out] */ IShape** shape)
{
    VALIDATE_NOT_NULL(shape);
    AutoPtr<CRoundRectShape> s;
    CRoundRectShape::NewByFriend(NULL, NULL, NULL, (CRoundRectShape**)&s);
    RoundRectShape::Clone((RoundRectShape*)s.Get());
    *shape = (IShape*)s.Get();
    REFCOUNT_ADD(*shape);
    return NOERROR;
}

} // namespace Shapes
} // namespace Drawable
} // namespace Graphics
} // namespace Droid
} // namespace Elastos
