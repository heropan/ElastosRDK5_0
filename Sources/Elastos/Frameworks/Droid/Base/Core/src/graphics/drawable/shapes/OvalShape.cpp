
#include "graphics/drawable/shapes/OvalShape.h"

namespace Elastos {
namespace Droid {
namespace Graphics {
namespace Drawable {
namespace Shapes {

/**
 * OvalShape constructor.
 */
OvalShape::OvalShape()
{}

ECode OvalShape::Init()
{
    return NOERROR;
}

ECode OvalShape::Draw(
    /* [in] */ ICanvas* canvas,
    /* [in] */ IPaint* paint)
{
    return canvas->DrawOval(Rect().Get(), paint);
}

void OvalShape::Clone(
    /* [in] */ OvalShape* other)
{
    RectShape::Clone(other);
}

} // namespace Shapes
} // namespace Drawable
} // namespace Graphics
} // namespace Droid
} // namespace Elastos
