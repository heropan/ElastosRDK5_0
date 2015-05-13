
#ifndef __CMIPMAPDRAWABLE_H__
#define __CMIPMAPDRAWABLE_H__

#include "_CMipmapDrawable.h"
#include "graphics/drawable/MipmapDrawable.h"
#include "graphics/drawable/DrawableMacro.h"
#include "view/ViewMacro.h"

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
    // TODO: Add your private member variables here.
};

} // namespace Drawable
} // namespace Graphics
} // namespace Droid
} // namespace Elastos

#endif // __CMIPMAPDRAWABLE_H__
