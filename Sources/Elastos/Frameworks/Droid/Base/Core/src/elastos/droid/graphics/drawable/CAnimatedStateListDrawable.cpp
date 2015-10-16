
#include "elastos/droid/graphics/drawable/CAnimatedStateListDrawable.h"

namespace Elastos {
namespace Droid {
namespace Graphics {
namespace Drawable {

CAR_OBJECT_IMPL(CAnimatedStateListDrawable);

ECode CAnimatedStateListDrawable::constructor(
    /* [in] */ /*@Nullable*/ IDrawableConstantState* state,
    /* [in] */ /*@Nullable*/ IResources* res)
{
    return AnimatedStateListDrawable::constructor((AnimatedStateListState*)state, res);
}

} // namespace Drawable
} // namespace Graphics
} // namespace Droid
} // namespace Elastos
