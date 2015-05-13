#include "graphics/drawable/shapes/RectShape.h"

namespace Elastos {
namespace Droid {
namespace Graphics {
namespace Drawable {
namespace Shapes {

/**
 * RectShape constructor.
 */
RectShape::RectShape()
{
    CRectF::NewByFriend((CRectF**)&mRect);
}

ECode RectShape::Init()
{
    return NOERROR;
}

ECode RectShape::Draw(
    /* [in] */ ICanvas* canvas,
    /* [in] */ IPaint* paint)
{
    return canvas->DrawRect(mRect.Get(), paint);
}

void RectShape::OnResize(
    /* [in] */ Float width,
    /* [in] */ Float height)
{
    mRect->Set(0, 0, width, height);
}

/**
 * Returns the RectF that defines this rectangle's bounds.
 */
AutoPtr<CRectF> RectShape::Rect()
{
    return mRect;
}

void RectShape::Clone(
    /* [in] */ RectShape* other)
{
    Shape::Clone((Shape*)other);
    CRectF::NewByFriend(mRect.Get(), (CRectF**)&other->mRect);
}

} // namespace Shapes
} // namespace Drawable
} // namespace Graphics
} // namespace Droid
} // namespace Elastos
