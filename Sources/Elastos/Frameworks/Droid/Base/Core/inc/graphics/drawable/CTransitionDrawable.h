
#ifndef __CTRANSITIONDRAWABLE_H__
#define __CTRANSITIONDRAWABLE_H__

#include "_CTransitionDrawable.h"
#include "graphics/drawable/TransitionDrawable.h"


namespace Elastos {
namespace Droid {
namespace Graphics {
namespace Drawable {

CarClass(CTransitionDrawable), public TransitionDrawable
{
public:
    /**
     * Create a new transition drawable with the specified list of layers. At least
     * 2 layers are required for this drawable to work properly.
     */
    CARAPI constructor(
        /* [in] */ ArrayOf<IDrawable*>* layers);

    /**
     * Create a new transition drawable with no layer. To work correctly, at least 2
     * layers must be added to this drawable.
     *
     * @see #TransitionDrawable(Drawable[])
     */
    CARAPI constructor();

    CARAPI constructor(
        /* [in] */ Handle32 state,
        /* [in] */ IResources* res);

    CARAPI constructor(
        /* [in] */ Handle32 state,
        /* [in] */ ArrayOf<IDrawable*>* layers);

    PInterface Probe(
        /* [in] */ REIID riid);

    IDRAWABLE_METHODS_DECL()

    IDRAWABLECALLBACK_METHODS_DECL()

    CARAPI FindDrawableByLayerId(
        /* [in] */ Int32 id,
        /* [out] */ IDrawable** drawable);

    CARAPI SetId(
        /* [in] */ Int32 index,
        /* [in] */ Int32 id);

    CARAPI GetNumberOfLayers(
        /* [out] */ Int32* number);

    CARAPI GetDrawable(
        /* [in] */ Int32 index,
        /* [out] */ IDrawable** drawable);

    CARAPI GetId(
        /* [in] */ Int32 index,
        /* [out] */ Int32* id);

    CARAPI SetDrawableByLayerId(
        /* [in] */ Int32 id,
        /* [in] */ IDrawable* drawable,
        /* [out] */ Boolean* res);

    CARAPI SetLayerInset(
        /* [in] */ Int32 index,
        /* [in] */ Int32 l,
        /* [in] */ Int32 t,
        /* [in] */ Int32 r,
        /* [in] */ Int32 b);

    CARAPI SetOpacity(
        /* [in] */ Int32 opacity);

    /**
     * Begin the second layer on top of the first layer.
     *
     * @param durationMillis The length of the transition in milliseconds
     */
    CARAPI StartTransition(
        /* [in] */ Int32 durationMillis);

    /**
     * Show only the first layer.
     */
    CARAPI ResetTransition();

    /**
     * Reverses the transition, picking up where the transition currently is.
     * If the transition is not currently running, this will start the transition
     * with the specified duration. If the transition is already running, the last
     * known duration will be used.
     *
     * @param duration The duration to use if no transition is running.
     */
    CARAPI ReverseTransition(
        /* [in] */ Int32 duration);

    /**
     * Enables or disables the cross fade of the drawables. When cross fade
     * is disabled, the first drawable is always drawn opaque. With cross
     * fade enabled, the first drawable is drawn with the opposite alpha of
     * the second drawable. Cross fade is disabled by default.
     *
     * @param enabled True to enable cross fading, false otherwise.
     */
    CARAPI SetCrossFadeEnabled(
        /* [in] */ Boolean enabled);

    /**
     * Indicates whether the cross fade is enabled for this transition.
     *
     * @return True if cross fading is enabled, false otherwise.
     */
    CARAPI IsCrossFadeEnabled(
        /* [out] */ Boolean* enabled);
};

} // namespace Drawable
} // namespace Graphics
} // namespace Droid
} // namespace Elastos

#endif //__CTRANSITIONDRAWABLE_H__
