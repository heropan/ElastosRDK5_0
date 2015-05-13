
#include "ext/frameworkext.h"
#include "graphics/drawable/shapes/COvalShape.h"

namespace Elastos {
namespace Droid {
namespace Graphics {
namespace Drawable {
namespace Shapes {

ECode COvalShape::constructor()
{
    return OvalShape::Init();
}

ECode COvalShape::GetWidth(
    /* [out] */ Float* width)
{
    VALIDATE_NOT_NULL(width);
    *width = OvalShape::GetWidth();
    return NOERROR;
}

ECode COvalShape::GetHeight(
    /* [out] */ Float* height)
{
    VALIDATE_NOT_NULL(height);
    *height = OvalShape::GetHeight();
    return NOERROR;
}

ECode COvalShape::Draw(
    /* [in] */ ICanvas* canvas,
    /* [in] */ IPaint* paint)
{
    return OvalShape::Draw(canvas, paint);
}

ECode COvalShape::Resize(
    /* [in] */ Float width,
    /* [in] */ Float height)
{
    return OvalShape::Resize(width, height);
}

ECode COvalShape::HasAlpha(
    /* [out] */ Boolean* hasAlpha)
{
    VALIDATE_NOT_NULL(hasAlpha);
    *hasAlpha = OvalShape::HasAlpha();
    return NOERROR;
}

ECode COvalShape::Clone(
    /* [out] */ IShape** shape)
{
    VALIDATE_NOT_NULL(shape);
    AutoPtr<COvalShape> s;
    COvalShape::NewByFriend((COvalShape**)&s);
    OvalShape::Clone((OvalShape*)s.Get());
    *shape = (IShape*)s.Get();
    INTERFACE_ADDREF(*shape);
    return NOERROR;
}

} // namespace Shapes
} // namespace Drawable
} // namespace Graphics
} // namespace Droid
} // namespace Elastos
