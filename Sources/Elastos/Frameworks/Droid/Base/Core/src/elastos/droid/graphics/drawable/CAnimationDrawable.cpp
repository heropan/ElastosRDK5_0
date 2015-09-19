
#include "ext/frameworkext.h"
#include "graphics/drawable/CAnimationDrawable.h"

namespace Elastos {
namespace Droid {
namespace Graphics {
namespace Drawable {

CAR_OBJECT_IMPL(CAnimationDrawable);
ECode CAnimationDrawable::constructor()
{
    return AnimationDrawable::constructor();
}

ECode CAnimationDrawable::constructor(
    /* [in] */ Handle32 state,
    /* [in] */ IResources* res)
{
    return AnimationDrawable::constructor((AnimationState*)state, res);
}

} // namespace Drawable
} // namespace Graphics
} // namespace Droid
} // namespace Elastos
