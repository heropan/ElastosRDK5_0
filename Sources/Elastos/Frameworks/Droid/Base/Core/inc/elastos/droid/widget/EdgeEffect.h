
#ifndef __ELASTOS_DROID_WIDGET_EDGEEFFECT_H__
#define __ELASTOS_DROID_WIDGET_EDGEEFFECT_H__

#include "elastos/droid/ext/frameworkext.h"

using Elastos::Droid::Content::IContext;
using Elastos::Droid::Graphics::Drawable::IDrawable;
using Elastos::Droid::Graphics::ICanvas;
using Elastos::Droid::Graphics::IPaint;
using Elastos::Droid::Graphics::IRect;
using Elastos::Droid::View::Animation::IInterpolator;

namespace Elastos {
namespace Droid {
namespace Widget {

/**
 * This class performs the glow effect used at the edges of scrollable widgets.
 * @hide
 */
class EdgeEffect
    : public Object
    , public IEdgeEffect
{
public:
    CAR_INTERFACE_DECL();

    EdgeEffect();

    ~EdgeEffect();

    /**
     * Construct a new EdgeEffect with a theme appropriate for the provided context.
     * @param context Context used to provide theming and resource information for the EdgeEffect
     */
    CARAPI constructor(
        /* [in] */ IContext* context);

    /**
     * Set the size of this edge effect in pixels.
     *
     * @param width Effect width in pixels
     * @param height Effect height in pixels
     */
    CARAPI SetSize(
        /* [in] */ Int32 width,
        /* [in] */ Int32 height);

    /**
     * Reports if this EdgeEffect's animation is finished. If this method returns false
     * after a call to {@link #draw(Canvas)} the host widget should schedule another
     * drawing pass to continue the animation.
     *
     * @return true if animation is finished, false if drawing should continue on the next frame.
     */
    CARAPI IsFinished(
        /* [out] */ Boolean* isFinished);

    /**
     * Immediately finish the current animation.
     * After this call {@link #isFinished()} will return true.
     */
    CARAPI Finish();

    /**
     * A view should call this when content is pulled away from an edge by the user.
     * This will update the state of the current visual effect and its associated animation.
     * The host view should always {@link android.view.View#invalidate()} after this
     * and draw the results accordingly.
     *
     * <p>Views using EdgeEffect should favor {@link #onPull(float, float)} when the displacement
     * of the pull point is known.</p>
     *
     * @param deltaDistance Change in distance since the last call. Values may be 0 (no change) to
     *                      1.f (full length of the view) or negative values to express change
     *                      back toward the edge reached to initiate the effect.
     */
    CARAPI OnPull(
        /* [in] */ Float deltaDistance);

    /**
     * A view should call this when content is pulled away from an edge by the user.
     * This will update the state of the current visual effect and its associated animation.
     * The host view should always {@link android.view.View#invalidate()} after this
     * and draw the results accordingly.
     *
     * @param deltaDistance Change in distance since the last call. Values may be 0 (no change) to
     *                      1.f (full length of the view) or negative values to express change
     *                      back toward the edge reached to initiate the effect.
     * @param displacement The displacement from the starting side of the effect of the point
     *                     initiating the pull. In the case of touch this is the finger position.
     *                     Values may be from 0-1.
     */
    CARAPI OnPull(
        /* [in] */ Float deltaDistance,
        /* [in] */ Float displacement);

    /**
     * Call when the object is released after being pulled.
     * This will begin the "decay" phase of the effect. After calling this method
     * the host view should {@link android.view.View#invalidate()} and thereby
     * draw the results accordingly.
     */
    CARAPI OnRelease();

    /**
     * Call when the effect absorbs an impact at the given velocity.
     * Used when a fling reaches the scroll boundary.
     *
     * <p>When using a {@link android.widget.Scroller} or {@link android.widget.OverScroller},
     * the method <code>getCurrVelocity</code> will provide a reasonable approximation
     * to use here.</p>
     *
     * @param velocity Velocity at impact in pixels per second.
     */
    CARAPI OnAbsorb(
        /* [in] */ Int32 velocity);

    /**
     * Set the color of this edge effect in argb.
     *
     * @param color Color in argb
     */
    CARAPI SetColor(
        /* [in] */ Int32 color);

    /**
     * Return the color of this edge effect in argb.
     * @return The color of this edge effect in argb
     */
    CARAPI GetColor(
        /* [out] */ Int32* color);

    /**
     * Draw into the provided canvas. Assumes that the canvas has been rotated
     * accordingly and the size has been set. The effect will be drawn the full
     * width of X=0 to X=width, beginning from Y=0 and extending to some factor <
     * 1.f of height.
     *
     * @param canvas Canvas to draw into
     * @return true if drawing should continue beyond this frame to continue the
     *         animation
     */
    CARAPI Draw(
        /* [in] */ ICanvas* canvas,
        /* [out] */ Boolean* res);

    /**
     * Return the maximum height that the edge effect will be drawn at given the original
     * {@link #setSize(int, int) input size}.
     * @return The maximum height of the edge effect
     */
    CARAPI GetMaxHeight(
        /* [out] */ Int32* height);

private:
    CARAPI_(void) Update();

private:
    static const String TAG;
    // Time it will take the effect to fully recede in ms
    static const Int32 RECEDE_TIME;

    // Time it will take before a pulled glow begins receding
    static const Int32 PULL_TIME;

    // Time it will take for a pulled glow to decay to partial strength before release
    static const Int32 PULL_DECAY_TIME;

    static const Float MAX_ALPHA;

    static const Float MAX_GLOW_SCALE;

    static const Float PULL_GLOW_BEGIN;

    // Minimum velocity that will be absorbed
    static const Int32 MIN_VELOCITY;
    // Maximum velocity, clamps at this value
    static const Int32 MAX_VELOCITY;

    static const Float EPSILON;

    static const Double ANGLE;
    static const Float SIN;
    static const Float COS;

    static const Int32 STATE_IDLE = 0;
    static const Int32 STATE_PULL = 1;
    static const Int32 STATE_ABSORB = 2;
    static const Int32 STATE_RECEDE = 3;
    static const Int32 STATE_PULL_DECAY = 4;

    static const Float PULL_DISTANCE_ALPHA_GLOW_FACTOR;

    static const Int32 VELOCITY_GLOW_FACTOR;

private:
    Float mGlowAlpha;
    Float mGlowScaleY;

    Float mGlowAlphaStart;
    Float mGlowAlphaFinish;
    Float mGlowScaleYStart;
    Float mGlowScaleYFinish;

    Int64 mStartTime;
    Float mDuration;

    AutoPtr<IInterpolator> mInterpolator;

    Int32 mState;

    Float mPullDistance;

    AutoPtr<IRect> mBounds;// = new Rect();
    AutoPtr<IPaint> mPaint;// = new Paint();

    Float mRadius;
    Float mBaseGlowScale;
    Float mDisplacement;
    Float mTargetDisplacement;
};

} // namespace Widget
} // namespace Droid
} // namespace Elastos

#endif //__ELASTOS_DROID_WIDGET_EDGEEFFECT_H__
