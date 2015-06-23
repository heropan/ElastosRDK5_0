
#include "ext/frameworkext.h"
#include "graphics/drawable/shapes/CArcShape.h"

namespace Elastos {
namespace Droid {
namespace Graphics {
namespace Drawable {
namespace Shapes {

ECode CArcShape::constructor(
    /* [in] */ Float startAngle,
    /* [in] */ Float sweepAngle)
{
    return ArcShape::Init(startAngle, sweepAngle);
}

ECode CArcShape::GetWidth(
    /* [out] */ Float* width)
{
    VALIDATE_NOT_NULL(width);
    *width = ArcShape::GetWidth();
    return NOERROR;
}

ECode CArcShape::GetHeight(
    /* [out] */ Float* height)
{
    VALIDATE_NOT_NULL(height);
    *height = ArcShape::GetHeight();
    return NOERROR;
}

ECode CArcShape::Draw(
    /* [in] */ ICanvas* canvas,
    /* [in] */ IPaint* paint)
{
    return ArcShape::Draw(canvas, paint);
}

ECode CArcShape::Resize(
    /* [in] */ Float width,
    /* [in] */ Float height)
{
    return ArcShape::Resize(width, height);
}

ECode CArcShape::HasAlpha(
    /* [out] */ Boolean* hasAlpha)
{
    VALIDATE_NOT_NULL(hasAlpha);
    *hasAlpha = ArcShape::HasAlpha();
    return NOERROR;
}

ECode CArcShape::Clone(
    /* [out] */ IShape** shape)
{
    VALIDATE_NOT_NULL(shape);
    AutoPtr<CArcShape> s;
    CArcShape::NewByFriend(0, 0, (CArcShape**)&s);
    ArcShape::Clone((ArcShape*)s.Get());
    *shape = (IShape*)s.Get();
    REFCOUNT_ADD(*shape);
    return NOERROR;
}

} // namespace Shapes
} // namespace Drawable
} // namespace Graphics
} // namespace Droid
} // namespace Elastos
