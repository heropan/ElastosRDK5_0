
#include "ext/frameworkext.h"
#include "graphics/drawable/CTransitionDrawable.h"

namespace Elastos {
namespace Droid {
namespace Graphics {
namespace Drawable {

/**
 * Create a new transition drawable with the specified list of layers. At least
 * 2 layers are required for this drawable to work properly.
 */
ECode CTransitionDrawable::constructor(
    /* [in] */ ArrayOf<IDrawable*>* layers)
{
    return TransitionDrawable::Init(layers);
}

/**
 * Create a new transition drawable with no layer. To work correctly, at least 2
 * layers must be added to this drawable.
 *
 * @see #TransitionDrawable(Drawable[])
 */
ECode CTransitionDrawable::constructor()
{
    return TransitionDrawable::Init();
}

ECode CTransitionDrawable::constructor(
    /* [in] */ Handle32 state,
    /* [in] */ IResources* res)
{
    return TransitionDrawable::Init((TransitionState*)state, res);
}

ECode CTransitionDrawable::constructor(
    /* [in] */ Handle32 state,
    /* [in] */ ArrayOf<IDrawable*>* layers)
{
    return TransitionDrawable::Init((TransitionState*)state, layers);
}

PInterface CTransitionDrawable::Probe(
    /* [in] */ REIID riid)
{
    return _CTransitionDrawable::Probe(riid);
}

IDRAWABLE_METHODS_IMPL(
    CTransitionDrawable, TransitionDrawable);

IDRAWABLECALLBACK_METHODS_IMPL(
    CTransitionDrawable, TransitionDrawable);

ECode CTransitionDrawable::FindDrawableByLayerId(
    /* [in] */ Int32 id,
    /* [out] */ IDrawable** drawable)
{
    VALIDATE_NOT_NULL(drawable);
    AutoPtr<IDrawable> temp = TransitionDrawable::FindDrawableByLayerId(id);
    *drawable = temp;
    REFCOUNT_ADD(*drawable);
    return NOERROR;
}

ECode CTransitionDrawable::SetId(
    /* [in] */ Int32 index,
    /* [in] */ Int32 id)
{
    return TransitionDrawable::SetId(index, id);
}

ECode CTransitionDrawable::GetNumberOfLayers(
    /* [out] */ Int32* number)
{
    VALIDATE_NOT_NULL(number);
    *number = TransitionDrawable::GetNumberOfLayers();

    return NOERROR;
}

ECode CTransitionDrawable::GetDrawable(
    /* [in] */ Int32 index,
    /* [out] */ IDrawable** drawable)
{
    VALIDATE_NOT_NULL(drawable);
    AutoPtr<IDrawable> temp = TransitionDrawable::GetDrawable(index);
    *drawable = temp;
    REFCOUNT_ADD(*drawable);
    return NOERROR;
}

ECode CTransitionDrawable::GetId(
    /* [in] */ Int32 index,
    /* [out] */ Int32* id)
{
    VALIDATE_NOT_NULL(id);
    *id = TransitionDrawable::GetId(index);

    return NOERROR;
}

ECode CTransitionDrawable::SetDrawableByLayerId(
    /* [in] */ Int32 id,
    /* [in] */ IDrawable* drawable,
    /* [out] */ Boolean* res)
{
    VALIDATE_NOT_NULL(res);
    *res = TransitionDrawable::SetDrawableByLayerId(id, drawable);

    return NOERROR;
}

ECode CTransitionDrawable::SetLayerInset(
    /* [in] */ Int32 index,
    /* [in] */ Int32 l,
    /* [in] */ Int32 t,
    /* [in] */ Int32 r,
    /* [in] */ Int32 b)
{
    return TransitionDrawable::SetLayerInset(index, l, t, r, b);
}

ECode CTransitionDrawable::SetOpacity(
    /* [in] */ Int32 opacity)
{
    return TransitionDrawable::SetOpacity(opacity);
}

/**
 * Begin the second layer on top of the first layer.
 *
 * @param durationMillis The length of the transition in milliseconds
 */
ECode CTransitionDrawable::StartTransition(
    /* [in] */ Int32 durationMillis)
{
    return TransitionDrawable::StartTransition(durationMillis);
}

/**
 * Show only the first layer.
 */
ECode CTransitionDrawable::ResetTransition()
{
    return TransitionDrawable::ResetTransition();
}

/**
 * Reverses the transition, picking up where the transition currently is.
 * If the transition is not currently running, this will start the transition
 * with the specified duration. If the transition is already running, the last
 * known duration will be used.
 *
 * @param duration The duration to use if no transition is running.
 */
ECode CTransitionDrawable::ReverseTransition(
    /* [in] */ Int32 duration)
{
    return TransitionDrawable::ReverseTransition(duration);
}

/**
 * Enables or disables the cross fade of the drawables. When cross fade
 * is disabled, the first drawable is always drawn opaque. With cross
 * fade enabled, the first drawable is drawn with the opposite alpha of
 * the second drawable. Cross fade is disabled by default.
 *
 * @param enabled True to enable cross fading, false otherwise.
 */
ECode CTransitionDrawable::SetCrossFadeEnabled(
    /* [in] */ Boolean enabled)
{
    return TransitionDrawable::SetCrossFadeEnabled(enabled);
}

/**
 * Indicates whether the cross fade is enabled for this transition.
 *
 * @return True if cross fading is enabled, false otherwise.
 */
ECode CTransitionDrawable::IsCrossFadeEnabled(
    /* [out] */ Boolean* enabled)
{
    VALIDATE_NOT_NULL(enabled);
    *enabled = TransitionDrawable::IsCrossFadeEnabled();
    return NOERROR;
}

} // namespace Drawable
} // namespace Graphics
} // namespace Droid
} // namespace Elastos
