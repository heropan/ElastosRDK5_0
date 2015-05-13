
#include "ext/frameworkext.h"
#include "graphics/drawable/shapes/CPathShape.h"

namespace Elastos {
namespace Droid {
namespace Graphics {
namespace Drawable {
namespace Shapes {

ECode CPathShape::constructor(
    /* [in] */ IPath* path,
    /* [in] */ Float stdWidth,
    /* [in] */ Float stdHeight)
{
    return PathShape::Init(path, stdWidth, stdHeight);
}

ECode CPathShape::GetWidth(
    /* [out] */ Float* width)
{
    VALIDATE_NOT_NULL(width);
    *width = PathShape::GetWidth();
    return NOERROR;
}

ECode CPathShape::GetHeight(
    /* [out] */ Float* height)
{
    VALIDATE_NOT_NULL(height);
    *height = PathShape::GetHeight();
    return NOERROR;
}

ECode CPathShape::Draw(
    /* [in] */ ICanvas* canvas,
    /* [in] */ IPaint* paint)
{
    return PathShape::Draw(canvas, paint);
}

ECode CPathShape::Resize(
    /* [in] */ Float width,
    /* [in] */ Float height)
{
    return PathShape::Resize(width, height);
}

ECode CPathShape::HasAlpha(
    /* [out] */ Boolean* hasAlpha)
{
    VALIDATE_NOT_NULL(hasAlpha);
    *hasAlpha = PathShape::HasAlpha();
    return NOERROR;
}

ECode CPathShape::Clone(
    /* [out] */ IShape** shape)
{
    VALIDATE_NOT_NULL(shape);
    AutoPtr<CPathShape> s;
    CPathShape::NewByFriend(NULL, 0, 0, (CPathShape**)&s);
    PathShape::Clone((PathShape*)s.Get());
    *shape = (IShape*)s.Get();
    INTERFACE_ADDREF(*shape);
    return NOERROR;
}

} // namespace Shapes
} // namespace Drawable
} // namespace Graphics
} // namespace Droid
} // namespace Elastos
