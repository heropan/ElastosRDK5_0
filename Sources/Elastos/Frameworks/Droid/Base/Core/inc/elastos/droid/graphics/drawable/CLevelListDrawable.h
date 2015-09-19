
#ifndef __ELASTOS_DROID_GRAPHICS_DRAWABLE_CLEVELLISTDRAWABLE_H__
#define __ELASTOS_DROID_GRAPHICS_DRAWABLE_CLEVELLISTDRAWABLE_H__

#include "_Elastos_Droid_Graphics_Drawable_CLevelListDrawable.h"
#include "graphics/drawable/LevelListDrawable.h"


namespace Elastos {
namespace Droid {
namespace Graphics {
namespace Drawable {

CarClass(CLevelListDrawable), public LevelListDrawable
{
public:
    CAR_OBJECT_DECL();

    CARAPI constructor();

    CARAPI constructor(
        /* [in] */ Handle32 state,
        /* [in] */ IResources* res);
};

} // namespace Drawable
} // namespace Graphics
} // namespace Droid
} // namespace Elastos

#endif //__ELASTOS_DROID_GRAPHICS_DRAWABLE_CLEVELLISTDRAWABLE_H__
