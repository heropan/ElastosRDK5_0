
#include "ext/frameworkext.h"
#include "graphics/drawable/CGradientDrawable.h"

namespace Elastos {
namespace Droid {
namespace Graphics {
namespace Drawable {

CAR_OBJECT_IMPL(CGradientDrawable);
ECode CGradientDrawable::constructor()
{
    return GradientDrawable::constructor();
}

ECode CGradientDrawable::constructor(
    /* [in] */ GradientDrawableOrientation orientation,
    /* [in] */ ArrayOf<Int32>* colors)
{
    return GradientDrawable::constructor(orientation, colors);
}

ECode CGradientDrawable::constructor(
    /* [in] */ IDrawableConstantState* state)
{
    return GradientDrawable::constructor((GradientState*)state);
}

} // namespace Drawable
} // namespace Graphics
} // namespace Droid
} // namespace Elastos
