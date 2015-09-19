
#include "ext/frameworkext.h"
#include "graphics/drawable/shapes/CRectShape.h"

namespace Elastos {
namespace Droid {
namespace Graphics {
namespace Drawable {
namespace Shapes {

CAR_OBJECT_IMPL(CRectShape);
ECode CRectShape::constructor()
{
    return NOERROR;
}

ECode CRectShape::Clone(
    /* [out] */ IInterface** shape)
{
    VALIDATE_NOT_NULL(shape);
    AutoPtr<IRectShape> rs;
    CRectShape::New((IRectShape**)&rs);
    CloneImpl(rs);
    *shape = rs;
    REFCOUNT_ADD(*shape);
    return NOERROR;
}

} // namespace Shapes
} // namespace Drawable
} // namespace Graphics
} // namespace Droid
} // namespace Elastos
