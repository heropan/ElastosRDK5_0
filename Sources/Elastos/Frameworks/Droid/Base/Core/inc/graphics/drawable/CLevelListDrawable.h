
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
    CARAPI constructor();

    CARAPI constructor(
        /* [in] */ Handle32 state,
        /* [in] */ IResources* res);

    CARAPI_(PInterface) Probe(
        /* [in] */ REIID riid);

    IDRAWABLE_METHODS_DECL()

    IDRAWABLECALLBACK_METHODS_DECL()

    CARAPI AddLevel(
        /* [in] */ Int32 low,
        /* [in] */ Int32 high,
        /* [in] */ IDrawable* drawable);

    CARAPI SetEnterFadeDuration(
        /* [in] */ Int32 ms);

    CARAPI SetExitFadeDuration(
        /* [in] */ Int32 ms);

    CARAPI SelectDrawable(
        /* [in] */ Int32 idx,
        /* [out] */ Boolean* res);
};

} // namespace Drawable
} // namespace Graphics
} // namespace Droid
} // namespace Elastos

#endif //__ELASTOS_DROID_GRAPHICS_DRAWABLE_CLEVELLISTDRAWABLE_H__
