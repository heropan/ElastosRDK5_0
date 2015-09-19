
#include "ext/frameworkext.h"
#include "graphics/drawable/CPictureDrawable.h"

namespace Elastos {
namespace Droid {
namespace Graphics {
namespace Drawable {

CAR_OBJECT_IMPL(CPictureDrawable);
ECode CPictureDrawable::constructor(
    /* [in] */ IPicture* picture)
{
    return PictureDrawable::constructor(picture);
}

} // namespace Drawable
} // namespace Graphics
} // namespace Droid
} // namespace Elastos
