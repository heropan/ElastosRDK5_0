
#include "ext/frameworkext.h"
#include "graphics/drawable/CAnimatedRotateDrawable.h"

namespace Elastos {
namespace Droid {
namespace Graphics {
namespace Drawable {

CAR_OBJECT_IMPL(CAnimatedRotateDrawable);
ECode CAnimatedRotateDrawable::constructor()
{
    return NOERROR;
}

ECode CAnimatedRotateDrawable::constructor(
    /* [in] */ IDrawableConstantState* state,
    /* [in] */ IResources* res)
{
    return AnimatedRotateDrawable::constructor((AnimatedRotateState*)state, res);
}

} // namespace Drawable
} // namespace Graphics
} // namespace Droid
} // namespace Elastos
