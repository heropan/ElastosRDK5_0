
#ifndef __CINSETDRAWABLE_H__
#define __CINSETDRAWABLE_H__

#include "_CInsetDrawable.h"
#include "graphics/drawable/InsetDrawable.h"


namespace Elastos {
namespace Droid {
namespace Graphics {
namespace Drawable {

CarClass(CInsetDrawable), public InsetDrawable
{
public:
    CARAPI constructor();

    CARAPI constructor(
        /* [in] */ IDrawable* drawable,
        /* [in] */ Int32 inset);

    CARAPI constructor(
        /* [in] */ IDrawable* drawable,
        /* [in] */ Int32 insetLeft,
        /* [in] */ Int32 insetTop,
        /* [in] */ Int32 insetRight,
        /* [in] */ Int32 insetBottom);

    CARAPI constructor(
        /* [in] */ Handle32 state,
        /* [in] */ IResources* res);

    CARAPI_(PInterface) Probe(
        /* [in] */ REIID riid);

    IDRAWABLE_METHODS_DECL()

    IDRAWABLECALLBACK_METHODS_DECL()
};

} // namespace Drawable
} // namespace Graphics
} // namespace Droid
} // namespace Elastos

#endif //__CINSETDRAWABLE_H__
