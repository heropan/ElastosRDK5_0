
#include "ext/frameworkext.h"
#include "graphics/drawable/CAnimationDrawable.h"

namespace Elastos {
namespace Droid {
namespace Graphics {
namespace Drawable {

ECode CAnimationDrawable::constructor()
{
    return AnimationDrawable::Init();
}

ECode CAnimationDrawable::constructor(
    /* [in] */ Handle32 state,
    /* [in] */ IResources* res)
{
    return AnimationDrawable::Init((AnimationState*)state, res);
}

//@Override
PInterface CAnimationDrawable::Probe(
    /* [in] */ REIID riid)
{
    return _CAnimationDrawable::Probe(riid);
}

IDRAWABLE_METHODS_IMPL(
    CAnimationDrawable, AnimationDrawable);

IDRAWABLECALLBACK_METHODS_IMPL(
    CAnimationDrawable, AnimationDrawable);

ECode CAnimationDrawable::SetEnterFadeDuration(
    /* [in] */ Int32 ms)
{
    return AnimationDrawable::SetEnterFadeDuration(ms);
}

ECode CAnimationDrawable::SetExitFadeDuration(
    /* [in] */ Int32 ms)
{
    return AnimationDrawable::SetExitFadeDuration(ms);
}

ECode CAnimationDrawable::SelectDrawable(
/* [in] */ Int32 idx,
/* [out] */ Boolean* res)
{
VALIDATE_NOT_NULL(res);
*res = AnimationDrawable::SelectDrawable(idx);

return NOERROR;
}

/**
 * @return The number of frames in the animation
 */
ECode CAnimationDrawable::GetNumberOfFrames(
    /* [out] */ Int32* number)
{
    VALIDATE_NOT_NULL(number);
    *number = AnimationDrawable::GetNumberOfFrames();
    return NOERROR;
}

/**
 * @return The Drawable at the specified frame index
 */
ECode CAnimationDrawable::GetFrame(
    /* [in] */ Int32 index,
    /* [out] */ IDrawable** drawable)
{
    VALIDATE_NOT_NULL(drawable);
    AutoPtr<IDrawable> dr = AnimationDrawable::GetFrame(index);
    *drawable = dr.Get();
    REFCOUNT_ADD(*drawable);
    return NOERROR;
}

/**
 * @return The duration in milliseconds of the frame at the
 * specified index
 */
ECode CAnimationDrawable::GetDuration(
    /* [in] */ Int32 i,
    /* [out] */ Int32* duration)
{
    VALIDATE_NOT_NULL(duration);
    *duration = AnimationDrawable::GetDuration(i);
    return NOERROR;
}

/**
 * @return True of the animation will play once, false otherwise
 */
ECode CAnimationDrawable::IsOneShot(
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
    *result = AnimationDrawable::IsOneShot();
    return NOERROR;
}

/**
 * Sets whether the animation should play once or repeat.
 *
 * @param oneShot Pass true if the animation should only play once
 */
ECode CAnimationDrawable::SetOneShot(
    /* [in] */ Boolean oneShot)
{
    return AnimationDrawable::SetOneShot(oneShot);
}

/**
 * Add a frame to the animation
 *
 * @param frame The frame to add
 * @param duration How long in milliseconds the frame should appear
 */
ECode CAnimationDrawable::AddFrame(
    /* [in] */ IDrawable* frame,
    /* [in] */ Int32 duration)
{
    return AnimationDrawable::AddFrame(frame, duration);
}

ECode CAnimationDrawable::Start()
{
    return AnimationDrawable::Start();
}

ECode CAnimationDrawable::Stop()
{
    return AnimationDrawable::Stop();
}

ECode CAnimationDrawable::IsRunning(
    /* [out] */ Boolean* isRunning)
{
    VALIDATE_NOT_NULL(isRunning);
    *isRunning = AnimationDrawable::IsRunning();
    return NOERROR;
}

ECode CAnimationDrawable::Run()
{
    return AnimationDrawable::Run();
}

} // namespace Drawable
} // namespace Graphics
} // namespace Droid
} // namespace Elastos
