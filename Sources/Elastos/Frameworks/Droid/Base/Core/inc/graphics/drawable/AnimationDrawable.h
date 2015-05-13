
#ifndef __ANIMATIONDRAWABLE_H__
#define __ANIMATIONDRAWABLE_H__

#include "graphics/drawable/DrawableContainer.h"

namespace Elastos {
namespace Droid {
namespace Graphics {
namespace Drawable {

/**
 *
 * An object used to create frame-by-frame animations, defined by a series of Drawable objects,
 * which can be used as a View object's background.
 * <p>
 * The simplest way to create a frame-by-frame animation is to define the animation in an XML
 * file, placed in the res/drawable/ folder, and set it as the background to a View object. Then, call
 * {@link #start()} to run the animation.
 * <p>
 * An AnimationDrawable defined in XML consists of a single <code>&lt;animation-list></code> element,
 * and a series of nested <code>&lt;item></code> tags. Each item defines a frame of the animation.
 * See the example below.
 * </p>
 * <p>spin_animation.xml file in res/drawable/ folder:</p>
 * <pre>&lt;!-- Animation frames are wheel0.png -- wheel5.png files inside the
 * res/drawable/ folder --&gt;
 * &lt;animation-list android:id=&quot;@+id/selected&quot; android:oneshot=&quot;false&quot;&gt;
 *    &lt;item android:drawable=&quot;@drawable/wheel0&quot; android:duration=&quot;50&quot; /&gt;
 *    &lt;item android:drawable=&quot;@drawable/wheel1&quot; android:duration=&quot;50&quot; /&gt;
 *    &lt;item android:drawable=&quot;@drawable/wheel2&quot; android:duration=&quot;50&quot; /&gt;
 *    &lt;item android:drawable=&quot;@drawable/wheel3&quot; android:duration=&quot;50&quot; /&gt;
 *    &lt;item android:drawable=&quot;@drawable/wheel4&quot; android:duration=&quot;50&quot; /&gt;
 *    &lt;item android:drawable=&quot;@drawable/wheel5&quot; android:duration=&quot;50&quot; /&gt;
 * &lt;/animation-list&gt;</pre>
 *
 * <p>Here is the code to load and play this animation.</p>
 * <pre>
 * // Load the ImageView that will host the animation and
 * // set its background to our AnimationDrawable XML resource.
 * ImageView img = (ImageView)findViewById(R.id.spinning_wheel_image);
 * img.setBackgroundResource(R.drawable.spin_animation);
 *
 * // Get the background, which has been compiled to an AnimationDrawable object.
 * AnimationDrawable frameAnimation = (AnimationDrawable) img.getBackground();
 *
 * // Start the animation (looped playback by default).
 * frameAnimation.start();
 * </pre>
 *
 * <div class="special reference">
 * <h3>Developer Guides</h3>
 * <p>For more information about animating with {@code AnimationDrawable}, read the
 * <a href="{@docRoot}guide/topics/graphics/drawable-animation.html">Drawable Animation</a>
 * developer guide.</p>
 * </div>
 *
 * @attr ref android.R.styleable#AnimationDrawable_visible
 * @attr ref android.R.styleable#AnimationDrawable_variablePadding
 * @attr ref android.R.styleable#AnimationDrawable_oneshot
 * @attr ref android.R.styleable#AnimationDrawableItem_duration
 * @attr ref android.R.styleable#AnimationDrawableItem_drawable
 */
class AnimationDrawable : public DrawableContainer
{
protected:
    class AnimationState : public DrawableContainer::DrawableContainerState
    {
    public:
        AnimationState(
            /* [in] */ AnimationState* orig,
            /* [in] */ AnimationDrawable* owner,
            /* [in] */ IResources* res);

        ~AnimationState();

        //@Override
        CARAPI NewDrawable(
            /* [out] */ IDrawable** drawable);

        //@Override
        CARAPI NewDrawableEx(
            /* [in] */ IResources* res,
            /* [out] */ IDrawable** drawable);

        CARAPI_(void) AddFrame(
            /* [in] */ IDrawable* dr,
            /* [in] */ Int32 dur);

        //@Override
        CARAPI_(void) GrowArray(
            /* [in] */ Int32 oldSize,
            /* [in] */ Int32 newSize);

    public:
        AutoPtr< ArrayOf<Int32> > mDurations;
        Boolean mOneShot;
    };

public:
    AnimationDrawable();

    //@Override
    CARAPI_(Boolean) SetVisible(
        /* [in] */ Boolean visible,
        /* [in] */ Boolean restart);

    /**
     * <p>Starts the animation, looping if necessary. This method has no effect
     * if the animation is running. Do not call this in the {@link android.app.Activity#onCreate}
     * method of your activity, because the {@link android.graphics.drawable.AnimationDrawable} is
     * not yet fully attached to the window. If you want to play
     * the animation immediately, without requiring interaction, then you might want to call it
     * from the {@link android.app.Activity#onWindowFocusChanged} method in your activity,
     * which will get called when Android brings your window into focus.</p>
     *
     * @see #isRunning()
     * @see #stop()
     */
    virtual CARAPI Start();

    /**
     * <p>Stops the animation. This method has no effect if the animation is
     * not running.</p>
     *
     * @see #isRunning()
     * @see #start()
     */
    virtual CARAPI Stop();

    /**
     * <p>Indicates whether the animation is currently running or not.</p>
     *
     * @return true if the animation is running, false otherwise
     */
    virtual CARAPI_(Boolean) IsRunning();

    /**
     * <p>This method exists for implementation purpose only and should not be
     * called directly. Invoke {@link #start()} instead.</p>
     *
     * @see #start()
     */
    virtual CARAPI Run();

    //@Override
    CARAPI UnscheduleSelf(
        /* [in] */ IRunnable* what);

    /**
     * @return The number of frames in the animation
     */
    virtual CARAPI_(Int32) GetNumberOfFrames();

    /**
     * @return The Drawable at the specified frame index
     */
    virtual CARAPI_(AutoPtr<IDrawable>) GetFrame(
        /* [in] */ Int32 index);

    /**
     * @return The duration in milliseconds of the frame at the
     * specified index
     */
    virtual CARAPI_(Int32) GetDuration(
        /* [in] */ Int32 i);

    /**
     * @return True of the animation will play once, false otherwise
     */
    virtual CARAPI_(Boolean) IsOneShot();

    /**
     * Sets whether the animation should play once or repeat.
     *
     * @param oneShot Pass true if the animation should only play once
     */
    virtual CARAPI SetOneShot(
        /* [in] */ Boolean oneShot);

    /**
     * Add a frame to the animation
     *
     * @param frame The frame to add
     * @param duration How long in milliseconds the frame should appear
     */
    virtual CARAPI AddFrame(
        /* [in] */ IDrawable* frame,
        /* [in] */ Int32 duration);

    //@Override
    CARAPI Inflate(
        /* [in] */ IResources* r,
        /* [in] */ IXmlPullParser* parser,
        /* [in] */ IAttributeSet* attrs);

    //@Override
    AutoPtr<IDrawable> Mutate();

protected:
    CARAPI Init(
        /* [in] */ AnimationState* state = NULL,
        /* [in] */ IResources* res = NULL);

private:
    AnimationDrawable(
        /* [in] */ AnimationState* state,
        /* [in] */ IResources* res);

    CARAPI NextFrame(
        /* [in] */ Boolean unschedule);

    CARAPI SetFrame(
        /* [in] */ Int32 frame,
        /* [in] */ Boolean unschedule,
        /* [in] */ Boolean animate);

private:
    AutoPtr<AnimationState> mAnimationState;
    Int32 mCurFrame;
    Boolean mMutated;
};

} // namespace Drawable
} // namespace Graphics
} // namespace Droid
} // namespace Elastos

#endif //__ANIMATIONDRAWABLE_H__
