
#ifndef __CSTATELISTDRAWABLE_H__
#define __CSTATELISTDRAWABLE_H__

#include "_CStateListDrawable.h"
#include "graphics/drawable/StateListDrawable.h"
#include "graphics/drawable/DrawableMacro.h"

namespace Elastos {
namespace Droid {
namespace Graphics {
namespace Drawable {

CarClass(CStateListDrawable), public StateListDrawable
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

    CARAPI AddState(
        /* [in] */ const ArrayOf<Int32>& stateSet,
        /* [in] */ IDrawable* drawable);

    CARAPI GetStateCount(
        /* [out] */ Int32* count);

    CARAPI GetStateSet(
        /* [in] */ Int32 index,
        /* [out, callee] */ ArrayOf<Int32>** stateSet);

    CARAPI GetStateDrawable(
        /* [in] */ Int32 index,
        /* [out] */ IDrawable** drawable);

    CARAPI GetStateDrawableIndex(
        /* [in] */ const ArrayOf<Int32>& stateSet,
        /* [out] */ Int32* index);

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

#endif // __CSTATELISTDRAWABLE_H__
