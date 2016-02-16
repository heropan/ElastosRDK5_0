
#ifndef __ELASTOS_DROID_GRAPHICS_DRAWABLE_CRIPPLEDRAWABLE_H__
#define __ELASTOS_DROID_GRAPHICS_DRAWABLE_CRIPPLEDRAWABLE_H__

#include "_Elastos_Droid_Graphics_Drawable_CRippleDrawable.h"
#include "elastos/droid/graphics/drawable/RippleDrawable.h"

namespace Elastos {
namespace Droid {
namespace Graphics {
namespace Drawable {

CarClass(CRippleDrawable), public RippleDrawable
{
public:
    CAR_OBJECT_DECL();

    CARAPI constructor();

    CARAPI constructor(
        /* [in] */ /*@NonNull*/ IColorStateList* color,
        /* [in] */ /*@Nullable*/ IDrawable* content,
        /* [in] */ /*@Nullable*/ IDrawable* mask);

    CARAPI constructor(
        /* [in] */ IDrawableConstantState* state,
        /* [in] */ IResources* res,
        /* [in] */ IResourcesTheme* theme);
};

} // namespace Drawable
} // namespace Graphics
} // namespace Droid
} // namespace Elastos

#endif // __ELASTOS_DROID_GRAPHICS_DRAWABLE_CRIPPLEDRAWABLE_H__
