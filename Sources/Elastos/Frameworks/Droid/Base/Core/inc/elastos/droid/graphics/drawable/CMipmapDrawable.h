
#ifndef __ELASTOS_DROID_GRAPHICS_DRAWABLE_CMIPMAPDRAWABLE_H__
#define __ELASTOS_DROID_GRAPHICS_DRAWABLE_CMIPMAPDRAWABLE_H__

#include "_Elastos_Droid_Graphics_Drawable_CMipmapDrawable.h"
#include "graphics/drawable/MipmapDrawable.h"

namespace Elastos {
namespace Droid {
namespace Graphics {
namespace Drawable {

CarClass(CMipmapDrawable), public MipmapDrawable
{
public:
    CARAPI_(PInterface) Probe(
        /* [in] */ REIID riid);

    IDRAWABLE_METHODS_DECL()

    IDRAWABLECALLBACK_METHODS_DECL()

    CARAPI AddDrawable(
        /* [in] */ IDrawable* drawable);

    CARAPI constructor();

private:
};

} // namespace Drawable
} // namespace Graphics
} // namespace Droid
} // namespace Elastos

#endif // __ELASTOS_DROID_GRAPHICS_DRAWABLE_CMIPMAPDRAWABLE_H__
