
#ifndef  __ANIMATOR_H__
#define  __ANIMATOR_H__

#include "ext/frameworkext.h"
#include <elastos/List.h>

using Elastos::Utility::List;

namespace Elastos {
namespace Droid {
namespace Animation {

extern "C" const InterfaceID EIID_Animator;

class AnimatorSet;
/**
 * This is the superclass for classes which provide basic support for animations which can be
 * started, ended, and have <code>AnimatorListeners</code> added to them.
 */
class Animator /*implements Cloneable*/
{
public:
    Animator();

    virtual ~Animator();

    virtual CARAPI_(PInterface) Probe(
        /* [in] */ REIID riid) = 0;

    /**
     * Starts this animation. If the animation has a nonzero startDelay, the animation will start
     * running after that delay elapses. A non-delayed animation will have its initial
     * value(s) set immediately, followed by calls to
     * {@link AnimatorListener#onAnimationStart(Animator)} for any listeners of this animator.
     *
     * <p>The animation started by calling this method will be run on the thread that called
     * this method. This thread should have a Looper on it (a runtime exception will be thrown if
     * this is not the case). Also, if the animation will animate
     * properties of objects in the view hierarchy, then the calling thread should be the UI
     * thread for that view hierarchy.</p>
     *
     */
    virtual CARAPI Start();

    /**
     * Cancels the animation. Unlike {@link #end()}, <code>cancel()</code> causes the animation to
     * stop in its tracks, sending an
     * {@link android.animation.Animator.AnimatorListener#onAnimationCancel(Animator)} to
     * its listeners, followed by an
     * {@link android.animation.Animator.AnimatorListener#onAnimationEnd(Animator)} message.
     *
     * <p>This method must be called on the thread that is running the animation.</p>
     */
    virtual CARAPI Cancel();

    /**
     * Ends the animation. This causes the animation to assign the end value of the property being
     * animated, then calling the
     * {@link android.animation.Animator.AnimatorListener#onAnimationEnd(Animator)} method on
     * its listeners.
     *
     * <p>This method must be called on the thread that is running the animation.</p>
     */
    virtual CARAPI End();

    /**
     * The amount of time, in milliseconds, to delay starting the animation after
     * {@link #start()} is called.
     *
     * @return the number of milliseconds to delay running the animation
     */
    virtual Int64 GetStartDelay() = 0;

    /**
     * The amount of time, in milliseconds, to delay starting the animation after
     * {@link #start()} is called.

     * @param startDelay The amount of the delay, in milliseconds
     */
    virtual CARAPI SetStartDelay(
        /* [in] */ Int64 startDelay) = 0;


    /**
     * Sets the length of the animation.
     *
     * @param duration The length of the animation, in milliseconds.
     */
    virtual CARAPI SetDuration(
        /* [in] */ Int64 duration) = 0;

    /**
     * Gets the length of the animation.
     *
     * @return The length of the animation, in milliseconds.
     */
    virtual Int64 GetDuration() = 0;

    /**
     * The time interpolator used in calculating the elapsed fraction of this animation. The
     * interpolator determines whether the animation runs with linear or non-linear motion,
     * such as acceleration and deceleration. The default value is
     * {@link android.view.animation.AccelerateDecelerateInterpolator}
     *
     * @param value the interpolator to be used by this animation
     */
    virtual CARAPI SetInterpolator(
        /* [in] */ ITimeInterpolator* value) = 0;

    /**
     * Returns whether this Animator is currently running (having been started and gone past any
     * initial startDelay period and not yet ended).
     *
     * @return Whether the Animator is running.
     */
    virtual Boolean IsRunning() = 0;

    /**
     * Returns whether this Animator has been started and not yet ended. This state is a superset
     * of the state of {@link #isRunning()}, because an Animator with a nonzero
     * {@link #getStartDelay() startDelay} will return true for {@link #isStarted()} during the
     * delay phase, whereas {@link #isRunning()} will return true only after the delay phase
     * is complete.
     *
     * @return Whether the Animator has been started and not yet ended.
     */
    virtual Boolean IsStarted();

    /**
     * Adds a listener to the set of listeners that are sent events through the life of an
     * animation, such as start, repeat, and end.
     *
     * @param listener the listener to be added to the current set of listeners for this animation.
     */
    virtual CARAPI AddListener(
        /* [in] */ IAnimatorListener* listener);

    /**
     * Removes a listener from the set listening to this animation.
     *
     * @param listener the listener to be removed from the current set of listeners for this
     *                 animation.
     */
    virtual CARAPI RemoveListener(
        /* [in] */ IAnimatorListener* listener);

    /**
     * Gets the set of {@link android.animation.Animator.AnimatorListener} objects that are currently
     * listening for events on this <code>Animator</code> object.
     *
     * @return ArrayList<AnimatorListener> The set of listeners.
     */
    virtual CARAPI GetListeners(
            /* [out, callee] */ ArrayOf<IAnimatorListener*>** listeners);

//    CARAPI SetListeners(
//        /* [in] */ ArrayOf<IAnimatorListener*>* listeners);

    /**
     * Removes all listeners from this object. This is equivalent to calling
     * <code>getListeners()</code> followed by calling <code>clear()</code> on the
     * returned list of listeners.
     */
    virtual CARAPI RemoveAllListeners();

    /**
     * This method tells the object to use appropriate information to extract
     * starting values for the animation. For example, a AnimatorSet object will pass
     * this call to its child objects to tell them to set up the values. A
     * ObjectAnimator object will use the information it has about its target object
     * and PropertyValuesHolder objects to get the start values for its properties.
     * A ValueAnimator object will ignore the request since it does not have enough
     * information (such as a target object) to gather these values.
     */
    virtual CARAPI SetupStartValues();

    /**
     * This method tells the object to use appropriate information to extract
     * ending values for the animation. For example, a AnimatorSet object will pass
     * this call to its child objects to tell them to set up the values. A
     * ObjectAnimator object will use the information it has about its target object
     * and PropertyValuesHolder objects to get the start values for its properties.
     * A ValueAnimator object will ignore the request since it does not have enough
     * information (such as a target object) to gather these values.
     */
    virtual CARAPI SetupEndValues();

    /**
     * Sets the target object whose property will be animated by this animation. Not all subclasses
     * operate on target objects (for example, {@link ValueAnimator}, but this method
     * is on the superclass for the convenience of dealing generically with those subclasses
     * that do handle targets.
     *
     * @param target The object being animated
     */
    virtual CARAPI SetTarget(
        /* [in] */ IInterface* target);

protected:

    //@Override
    CARAPI CloneSuperData(
        /* [in] */ Animator* anim);

    /**
     * The set of listeners to be sent events through the life of an animation.
     */
    List<AutoPtr<IAnimatorListener> > mListeners;

    friend class AnimatorSet;
};


}   //namespace Animation
}   //namespace Droid
}   //namespace Elastos

#endif  //__ANIMATOR_H__
