
#include "elastos/droid/ext/frameworkext.h"
#include "elastos/droid/graphics/drawable/CTransitionDrawable.h"

namespace Elastos {
namespace Droid {
namespace Graphics {
namespace Drawable {

CAR_OBJECT_IMPL(CTransitionDrawable);
ECode CTransitionDrawable::constructor(
    /* [in] */ ArrayOf<IDrawable*>* layers)
{
    return TransitionDrawable::constructor(layers);
}

ECode CTransitionDrawable::constructor()
{
    return TransitionDrawable::constructor();
}

ECode CTransitionDrawable::constructor(
    /* [in] */ Handle32 state,
    /* [in] */ IResources* res)
{
    return TransitionDrawable::constructor((TransitionState*)state, res);
}

ECode CTransitionDrawable::constructor(
    /* [in] */ Handle32 state,
    /* [in] */ ArrayOf<IDrawable*>* layers)
{
    return TransitionDrawable::constructor((TransitionState*)state, layers);
}

} // namespace Drawable
} // namespace Graphics
} // namespace Droid
} // namespace Elastos
