
#include "elastos/droid/ext/frameworkext.h"
#include "elastos/droid/graphics/drawable/CShapeDrawable.h"

namespace Elastos {
namespace Droid {
namespace Graphics {
namespace Drawable {

CAR_OBJECT_IMPL(CShapeDrawable);
ECode CShapeDrawable::constructor()
{
    return ShapeDrawable::constructor();
}

ECode CShapeDrawable::constructor(
    /* [in] */ IShape* s)
{
    return ShapeDrawable::constructor(s);
}

ECode CShapeDrawable::constructor(
    /* [in] */ Handle32 state)
{
    return ShapeDrawable::constructor((ShapeState*)state);
}

} // namespace Drawable
} // namespace Graphics
} // namespace Droid
} // namespace Elastos
