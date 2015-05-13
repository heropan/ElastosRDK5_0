
#ifndef __CCOLORDRAWABLE_H__
#define __CCOLORDRAWABLE_H__

#include "_CColorDrawable.h"
#include "graphics/drawable/ColorDrawable.h"
#include "graphics/drawable/DrawableMacro.h"

namespace Elastos {
namespace Droid {
namespace Graphics {
namespace Drawable {

CarClass(CColorDrawable), public ColorDrawable
{
public:
    CARAPI constructor();

    CARAPI constructor(
        /* [in] */ Int32 color);

    CARAPI constructor(
        /* [in] */ IDrawableConstantState* state);

    CARAPI_(PInterface) Probe(
        /* [in] */ REIID riid);

    IDRAWABLE_METHODS_DECL()

    CARAPI GetColor(
        /* [out] */ Int32* color);

    CARAPI SetColor(
        /* [in] */ Int32 color);

    CARAPI GetAlpha(
        /* [out] */ Int32* alpha);
};

} // namespace Drawable
} // namespace Graphics
} // namespace Droid
} // namespace Elastos

#endif // __CCOLORDRAWABLE_H__
