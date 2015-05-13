
#ifndef __CLEVELLISTDRAWABLE_H__
#define __CLEVELLISTDRAWABLE_H__

#include "_CLevelListDrawable.h"
#include "graphics/drawable/LevelListDrawable.h"
#include "graphics/drawable/DrawableMacro.h"

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

#endif //__CLEVELLISTDRAWABLE_H__
