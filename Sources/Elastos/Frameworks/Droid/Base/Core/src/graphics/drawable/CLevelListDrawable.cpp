
#include "ext/frameworkext.h"
#include "graphics/drawable/CLevelListDrawable.h"

namespace Elastos {
namespace Droid {
namespace Graphics {
namespace Drawable {

ECode CLevelListDrawable::constructor()
{
    return LevelListDrawable::Init();
}

ECode CLevelListDrawable::constructor(
    /* [in] */ Handle32 state,
    /* [in] */ IResources* res)
{
    return LevelListDrawable::Init((LevelListState*)state, res);
}

PInterface CLevelListDrawable::Probe(
    /* [in] */ REIID riid)
{
    return _CLevelListDrawable::Probe(riid);
}

IDRAWABLE_METHODS_IMPL(
    CLevelListDrawable, LevelListDrawable);

IDRAWABLECALLBACK_METHODS_IMPL(
    CLevelListDrawable, LevelListDrawable);


ECode CLevelListDrawable::AddLevel(
    /* [in] */ Int32 low,
    /* [in] */ Int32 high,
    /* [in] */ IDrawable* drawable)
{
    return LevelListDrawable::AddLevel(low, high, drawable);
}

ECode CLevelListDrawable::SetEnterFadeDuration(
    /* [in] */ Int32 ms)
{
    return LevelListDrawable::SetEnterFadeDuration(ms);
}

ECode CLevelListDrawable::SetExitFadeDuration(
    /* [in] */ Int32 ms)
{
    return LevelListDrawable::SetExitFadeDuration(ms);
}

ECode CLevelListDrawable::SelectDrawable(
    /* [in] */ Int32 idx,
    /* [out] */ Boolean* res)
{
    VALIDATE_NOT_NULL(res);
    *res = LevelListDrawable::SelectDrawable(idx);

    return NOERROR;
}

} // namespace Drawable
} // namespace Graphics
} // namespace Droid
} // namespace Elastos
