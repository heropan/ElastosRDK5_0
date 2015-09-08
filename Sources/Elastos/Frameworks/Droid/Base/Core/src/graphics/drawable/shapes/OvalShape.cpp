
#include "graphics/drawable/shapes/OvalShape.h"

namespace Elastos {
namespace Droid {
namespace Graphics {
namespace Drawable {
namespace Shapes {

CAR_INTERFACE_IMPL(OvalShape, RectShape, IOvalShape);
OvalShape::OvalShape()
{}

ECode OvalShape::Draw(
    /* [in] */ ICanvas* canvas,
    /* [in] */ IPaint* paint)
{
    return canvas->DrawOval(Rect().Get(), paint);
}

} // namespace Shapes
} // namespace Drawable
} // namespace Graphics
} // namespace Droid
} // namespace Elastos
