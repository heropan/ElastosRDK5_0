
#ifndef __ELASTOS_DROID_GRAPHICS_DRAWABLE_CCOLORDRAWABLE_H__
#define __ELASTOS_DROID_GRAPHICS_DRAWABLE_CCOLORDRAWABLE_H__

#include "_Elastos_Droid_Graphics_Drawable_CColorDrawable.h"
#include "elastos/droid/graphics/drawable/ColorDrawable.h"

namespace Elastos {
namespace Droid {
namespace Graphics {
namespace Drawable {

CarClass(CColorDrawable), public ColorDrawable
{
public:
    CAR_OBJECT_DECL();

    CARAPI constructor();

    CARAPI constructor(
        /* [in] */ Int32 color);

    CARAPI constructor(
        /* [in] */ IDrawableConstantState* state,
        /* [in] */ IResources* res,
        /* [in] */ IResourcesTheme* theme);
};

} // namespace Drawable
} // namespace Graphics
} // namespace Droid
} // namespace Elastos

#endif // __ELASTOS_DROID_GRAPHICS_DRAWABLE_CCOLORDRAWABLE_H__
