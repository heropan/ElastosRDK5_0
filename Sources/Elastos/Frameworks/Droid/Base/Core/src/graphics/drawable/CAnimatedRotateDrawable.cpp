
#include "ext/frameworkext.h"
#include "graphics/drawable/CAnimatedRotateDrawable.h"

namespace Elastos {
namespace Droid {
namespace Graphics {
namespace Drawable {

IDRAWABLE_METHODS_IMPL(
    CAnimatedRotateDrawable, AnimatedRotateDrawable);

IDRAWABLECALLBACK_METHODS_IMPL(
    CAnimatedRotateDrawable, AnimatedRotateDrawable);

ECode CAnimatedRotateDrawable::constructor()
{
    return NOERROR;
}

ECode CAnimatedRotateDrawable::constructor(
    /* [in] */ IDrawableConstantState* state,
    /* [in] */ IResources* res)
{
    return AnimatedRotateDrawable::Init((AnimatedRotateState*)state, res);
}

PInterface CAnimatedRotateDrawable::Probe(
    /* [in] */ REIID riid)
{
    return _CAnimatedRotateDrawable::Probe(riid);
}

ECode CAnimatedRotateDrawable::Start()
{
    return AnimatedRotateDrawable::Start();
}

ECode CAnimatedRotateDrawable::Stop()
{
    return AnimatedRotateDrawable::Stop();
}

ECode CAnimatedRotateDrawable::IsRunning(
    /* [out] */ Boolean* isRunning)
{
    VALIDATE_NOT_NULL(isRunning);
    *isRunning = AnimatedRotateDrawable::IsRunning();

    return NOERROR;
}

ECode CAnimatedRotateDrawable::Run()
{
    return AnimatedRotateDrawable::Run();
}

ECode CAnimatedRotateDrawable::GetDrawable(
    /* [out] */ IDrawable** drawable)
{
    VALIDATE_NOT_NULL(drawable);
    AutoPtr<IDrawable> temp = AnimatedRotateDrawable::GetDrawable();
    *drawable = temp;
    REFCOUNT_ADD(*drawable);
    return NOERROR;
}

CARAPI CAnimatedRotateDrawable::SetFramesCount(
    /* [in] */ Int32 framesCount)
{
    return AnimatedRotateDrawable::SetFramesCount(framesCount);
}

CARAPI CAnimatedRotateDrawable::SetFramesDuration(
    /* [in] */ Int32 framesDuration)
{
    return AnimatedRotateDrawable::SetFramesDuration(framesDuration);
}

} // namespace Drawable
} // namespace Graphics
} // namespace Droid
} // namespace Elastos
