
#include "ext/frameworkext.h"
#include "graphics/drawable/CLevelListDrawable.h"

namespace Elastos {
namespace Droid {
namespace Graphics {
namespace Drawable {

CAR_OBJECT_IMPL(CLevelListDrawable);
ECode CLevelListDrawable::constructor()
{
    return LevelListDrawable::constructor();
}

ECode CLevelListDrawable::constructor(
    /* [in] */ Handle32 state,
    /* [in] */ IResources* res)
{
    return LevelListDrawable::constructor((LevelListState*)state, res);
}

} // namespace Drawable
} // namespace Graphics
} // namespace Droid
} // namespace Elastos
