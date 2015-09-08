
#include "graphics/drawable/shapes/RectShape.h"
// #include "graphics/CRectF.h"

namespace Elastos {
namespace Droid {
namespace Graphics {
namespace Drawable {
namespace Shapes {

CAR_INTERFACE_IMPL(RectShape, Shape, IRectShape);
RectShape::RectShape()
{
    assert(0 && "TODO");
    // CRectF::New((IRectF**)&mRect);
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

AutoPtr<IRectF> RectShape::Rect()
{
    return mRect;
}

ECode RectShape::CloneImpl(
    /* [in] */ IRectShape* other)
{
    assert(other != NULL);
    Shape::CloneImpl(IShape::Probe(other));
    assert(0 && "TODO");
    // return CRectF::New(mRect.Get(), (IRectF**)&((RectShape*)other)->mRect);
    return NOERROR;
}

} // namespace Shapes
} // namespace Drawable
} // namespace Graphics
} // namespace Droid
} // namespace Elastos
