
#include "elastos/droid/ext/frameworkext.h"
#include "elastos/droid/graphics/drawable/CStateListDrawable.h"

namespace Elastos {
namespace Droid {
namespace Graphics {
namespace Drawable {

CAR_OBJECT_IMPL(CStateListDrawable);
ECode CStateListDrawable::constructor()
{
    return StateListDrawable::constructor(NULL, NULL);
}

ECode CStateListDrawable::constructor(
    /* [in] */ Handle32 state,
    /* [in] */ IResources* res)
{
    return StateListDrawable::constructor((StateListDrawable::StateListState*)state, res);
}

} // namespace Drawable
} // namespace Graphics
} // namespace Droid
} // namespace Elastos
