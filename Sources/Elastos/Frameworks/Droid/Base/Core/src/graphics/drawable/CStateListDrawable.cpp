
#include "ext/frameworkext.h"
#include "graphics/drawable/CStateListDrawable.h"

namespace Elastos {
namespace Droid {
namespace Graphics {
namespace Drawable {

IDRAWABLE_METHODS_IMPL(
    CStateListDrawable, StateListDrawable);

IDRAWABLECALLBACK_METHODS_IMPL(
    CStateListDrawable, StateListDrawable);

PInterface CStateListDrawable::Probe(
    /* [in] */ REIID riid)
{
    return _CStateListDrawable::Probe(riid);
}

ECode CStateListDrawable::AddState(
    /* [in] */ const ArrayOf<Int32> & stateSet,
    /* [in] */ IDrawable* drawable)
{
    return StateListDrawable::AddState(stateSet, drawable);
}

ECode CStateListDrawable::GetStateCount(
    /* [out] */ Int32* count)
{
    VALIDATE_NOT_NULL(count);
    *count = StateListDrawable::GetStateCount();
    return NOERROR;
}

ECode CStateListDrawable::GetStateSet(
    /* [in] */ Int32 index,
    /* [out, callee] */ ArrayOf<Int32> ** stateSet)
{
    VALIDATE_NOT_NULL(stateSet);
    AutoPtr< ArrayOf<Int32> > temp = StateListDrawable::GetStateSet(index);
    *stateSet = temp;
    REFCOUNT_ADD(*stateSet);
    return NOERROR;
}

ECode CStateListDrawable::GetStateDrawable(
    /* [in] */ Int32 index,
    /* [out] */ IDrawable ** drawable)
{
    VALIDATE_NOT_NULL(drawable);
    AutoPtr<IDrawable> temp = StateListDrawable::GetStateDrawable(index);
    *drawable = temp;
    REFCOUNT_ADD(*drawable);
    return NOERROR;
}

ECode CStateListDrawable::GetStateDrawableIndex(
    /* [in] */ const ArrayOf<Int32> & stateSet,
    /* [out] */ Int32 * index)
{
    VALIDATE_NOT_NULL(index);
    *index = StateListDrawable::GetStateDrawableIndex(stateSet);
    return NOERROR;
}

ECode CStateListDrawable::SetEnterFadeDuration(
    /* [in] */ Int32 ms)
{
    return StateListDrawable::SetEnterFadeDuration(ms);
}

ECode CStateListDrawable::SetExitFadeDuration(
    /* [in] */ Int32 ms)
{
    return StateListDrawable::SetExitFadeDuration(ms);
}

ECode CStateListDrawable::SelectDrawable(
    /* [in] */ Int32 idx,
    /* [out] */ Boolean* res)
{
    VALIDATE_NOT_NULL(res);
    *res = StateListDrawable::SelectDrawable(idx);
    return NOERROR;
}

ECode CStateListDrawable::constructor()
{
    return StateListDrawable::Init(NULL, NULL);
}

ECode CStateListDrawable::constructor(
    /* [in] */ Handle32 state,
    /* [in] */ IResources* res)
{
    return StateListDrawable::Init((StateListDrawable::StateListState*)state, res);
}

} // namespace Drawable
} // namespace Graphics
} // namespace Droid
} // namespace Elastos
