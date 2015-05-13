
#ifndef __CSCALEDRAWABLE_H__
#define __CSCALEDRAWABLE_H__

#include "_CScaleDrawable.h"
#include "graphics/drawable/ScaleDrawable.h"
#include "graphics/drawable/DrawableMacro.h"

namespace Elastos {
namespace Droid {
namespace Graphics {
namespace Drawable {

CarClass(CScaleDrawable), public ScaleDrawable
{
public:
    CARAPI constructor();

    CARAPI constructor(
        /* [in] */ IDrawable* drawable,
        /* [in] */ Int32 gravity,
        /* [in] */ Float scaleWidth,
        /* [in] */ Float scaleHeight);

    CARAPI constructor(
        /* [in] */ Handle32 state,
        /* [in] */ IResources* res);

    PInterface Probe(
        /* [in] */ REIID riid);

    IDRAWABLE_METHODS_DECL()

    IDRAWABLECALLBACK_METHODS_DECL()

    /**
     * Returns the drawable scaled by this ScaleDrawable.
     */
    CARAPI GetDrawable(
        /* [out] */ IDrawable** drawable);
};

} // namespace Drawable
} // namespace Graphics
} // namespace Droid
} // namespace Elastos

#endif //__CSCALEDRAWABLE_H__
