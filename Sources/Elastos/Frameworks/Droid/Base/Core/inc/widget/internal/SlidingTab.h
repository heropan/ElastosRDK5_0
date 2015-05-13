
#ifndef __SLIDINGTAB_H__
#define __SLIDINGTAB_H__

#include "view/ViewGroup.h"
#include <elastos/Mutex.h>
#include <elastos/Math.h>
#include "R.h"

using Elastos::Core::Math;
using Elastos::Core::Threading::Mutex;
using Elastos::Droid::R;
using Elastos::Droid::View::IView;
using Elastos::Droid::View::IViewGroup;
using Elastos::Droid::View::Animation::IAnimation;
using Elastos::Droid::View::Animation::IAnimationListener;
using Elastos::Droid::View::ViewGroup;
using Elastos::Droid::View::IMotionEvent;
using Elastos::Droid::Content::IContext;
using Elastos::Droid::Utility::IAttributeSet;
using Elastos::Droid::Widget::ITextView;
using Elastos::Droid::Widget::IImageView;
using Elastos::Droid::Graphics::IRect;

namespace Elastos {
namespace Droid {
namespace Widget {
namespace Internal {

/**
 * A special widget containing two Sliders and a threshold for each.  Moving either slider beyond
 * the threshold will cause the registered OnTriggerListener.onTrigger() to be called with
 * whichHandle being {@link OnTriggerListener#LEFT_HANDLE} or {@link OnTriggerListener#RIGHT_HANDLE}
 * Equivalently, selecting a tab will result in a call to
 * {@link OnTriggerListener#onGrabbedStateChange(View, Int32)} with one of these two states. Releasing
 * the tab will result in whichHandle being {@link OnTriggerListener#NO_HANDLE}.
 *
 */
class SlidingTab : public ViewGroup
{
private:
    /**
     * Simple container class for all things pertinent to a slider.
     * A slider consists of 3 Views:
     *
     * {@link #tab} is the tab shown on the screen in the default state.
     * {@link #text} is the view revealed as the user slides the tab out.
     * {@link #target} is the target the user must drag the slider past to trigger the slider.
     *
     */
    class Slider : public ElRefBase
    {
    public:

        /**
         * Constructor
         *
         * @param parent the container view of this one
         * @param tabId drawable for the tab
         * @param barId drawable for the bar
         * @param targetId drawable for the target
         */
        Slider(
            /* [in] */ IViewGroup* parent,
            /* [in] */ Int32 tabId,
            /* [in] */ Int32 barId,
            /* [in] */ Int32 targetId);

        CARAPI_(void) SetIcon(
            /* [in] */ Int32 iconId);

        CARAPI_(void) SetTabBackgroundResource(
            /* [in] */ Int32 tabId);

        CARAPI_(void) SetBarBackgroundResource(
            /* [in] */ Int32 barId);

        CARAPI_(void) SetHintText(
            /* [in] */ Int32 resId);

        CARAPI_(void) Hide();

        CARAPI_(void) Show(
            /* [in] */ Boolean animate);

        CARAPI_(void) SetState(
            /* [in] */ Int32 state);

        CARAPI_(void) ShowTarget();

        CARAPI_(void) Reset(
            /* [in] */ Boolean animate);

        CARAPI_(void) SetTarget(
            /* [in] */ Int32 targetId);

        /**
         * Layout the given widgets within the parent.
         *
         * @param l the parent's left border
         * @param t the parent's top border
         * @param r the parent's right border
         * @param b the parent's bottom border
         * @param alignment which side to align the widget to
         */
        CARAPI_(void) Layout(
            /* [in] */ Int32 l,
            /* [in] */ Int32 t,
            /* [in] */ Int32 r,
            /* [in] */ Int32 b,
            /* [in] */ Int32 alignment);

        CARAPI_(void) UpdateDrawableStates();

        /**
         * Ensure all the dependent widgets are measured.
         */
        CARAPI_(void) Measure();

        /**
         * Get the measured tab width. Must be called after {@link Slider#measure()}.
         * @return
         */
        CARAPI_(Int32) GetTabWidth();

        /**
         * Get the measured tab width. Must be called after {@link Slider#measure()}.
         * @return
         */
        CARAPI_(Int32) GetTabHeight();

        /**
         * Start animating the slider. Note we need two animations since a ValueAnimator
         * keeps internal state of the invalidation region which is just the view being animated.
         *
         * @param anim1
         * @param anim2
         */
        CARAPI_(void) StartAnimation(
            /* [in] */ IAnimation* anim1,
            /* [in] */ IAnimation* anim2);

        CARAPI_(void) HideTarget();
    public:
        static const Int32 ALIGN_LEFT = 0;
        static const Int32 ALIGN_RIGHT = 1;
        static const Int32 ALIGN_TOP = 2;
        static const Int32 ALIGN_BOTTOM = 3;
        static const Int32 ALIGN_UNKNOWN = 4;

    /**
     * States for the view.
     */
    private:
        static const Int32 STATE_NORMAL = 0;
        static const Int32 STATE_PRESSED = 1;
        static const Int32 STATE_ACTIVE = 2;

    private:
        AutoPtr<IImageView> mTab;
        AutoPtr<ITextView> mText;
        AutoPtr<IImageView> mTarget;
        Int32 mCurrentState;// = STATE_NORMAL;
        Int32 mAlignment;// = ALIGN_UNKNOWN;
        Int32 mAlignment_value;

        friend class SlidingTab;
    };

    class AnimationDoneListener
            : public ElRefBase
            , public IAnimationListener
    {
    public:
        CAR_INTERFACE_DECL()

        AnimationDoneListener(
            /* [in] */ SlidingTab* host);

        virtual CARAPI OnAnimationStart(
            /* [in] */ IAnimation* animation);

        virtual CARAPI OnAnimationEnd(
            /* [in] */ IAnimation* animation);

        virtual CARAPI OnAnimationRepeat(
            /* [in] */ IAnimation* animation);

    protected:
        SlidingTab* mHost;
    };

    class StartAnimationListener
            : public ElRefBase
            , public IAnimationListener
    {
    public:
        CAR_INTERFACE_DECL()

        StartAnimationListener(
            /* [in] */ SlidingTab* host,
            /* [in] */ Boolean holdAfter,
            /* [in] */ Int32 dx,
            /* [in] */ Int32 dy);

        virtual CARAPI OnAnimationStart(
            /* [in] */ IAnimation* animation);

        virtual CARAPI OnAnimationEnd(
            /* [in] */ IAnimation* animation);

        virtual CARAPI OnAnimationRepeat(
            /* [in] */ IAnimation* animation);

    protected:
        SlidingTab* mHost;
    private:
        Boolean mHoldAfter;
        Int32 mDx;
        Int32 mDy;
    };
public:

    SlidingTab();
    /**
     * Constructor used when this widget is created from a layout file.
     */
    SlidingTab(
        /* [in] */ IContext* context,
        /* [in] */ IAttributeSet* attrs = NULL);


    virtual CARAPI_(Boolean) OnInterceptTouchEvent(
        /* [in] */ IMotionEvent* event);

    /**
     * Reset the tabs to their original state and stop any existing animation.
     * Animate them back into place if animate is TRUE.
     *
     * @param animate
     */
    virtual CARAPI Reset(
        /* [in] */ Boolean animate);

    virtual CARAPI SetVisibility(
        /* [in] */ Int32 visibility);

    virtual CARAPI_(Boolean) OnTouchEvent(
        /* [in] */ IMotionEvent* event);

    /**
     * Sets the left handle icon to a given resource.
     *
     * The resource should refer to a Drawable object, or use 0 to remove
     * the icon.
     *
     * @param iconId the resource ID of the icon drawable
     * @param targetId the resource of the target drawable
     * @param barId the resource of the bar drawable (stateful)
     * @param tabId the resource of the
     */
    virtual CARAPI SetLeftTabResources(
        /* [in] */ Int32 iconId,
        /* [in] */ Int32 targetId,
        /* [in] */ Int32 barId,
        /* [in] */ Int32 tabId);

    /**
     * Sets the left handle hint text to a given resource string.
     *
     * @param resId
     */
    virtual CARAPI SetLeftHintText(
        /* [in] */ Int32 resId);

    /**
     * Sets the right handle icon to a given resource.
     *
     * The resource should refer to a Drawable object, or use 0 to remove
     * the icon.
     *
     * @param iconId the resource ID of the icon drawable
     * @param targetId the resource of the target drawable
     * @param barId the resource of the bar drawable (stateful)
     * @param tabId the resource of the
     */
    virtual CARAPI SetRightTabResources(
        /* [in] */ Int32 iconId,
        /* [in] */ Int32 targetId,
        /* [in] */ Int32 barId,
        /* [in] */ Int32 tabId);

    /**
     * Sets the left handle hint text to a given resource string.
     *
     * @param resId
     */
    virtual CARAPI SetRightHintText(
        /* [in] */ Int32 resId);

    virtual CARAPI SetHoldAfterTrigger(
        /* [in] */ Boolean holdLeft,
        /* [in] */ Boolean holdRight);

    /**
     * Registers a callback to be invoked when the user triggers an event.
     *
     * @param listener the OnDialTriggerListener to attach to this view
     */
    virtual CARAPI SetOnTriggerListener(
        /* [in] */ ISlidingTabOnTriggerListener* listener);

protected:
    virtual CARAPI_(void) OnMeasure(
        /* [in] */ Int32 widthMeasureSpec,
        /* [in] */ Int32 heightMeasureSpec);

    virtual CARAPI_(void) OnLayout(
        /* [in] */ Boolean changed,
        /* [in] */ Int32 l,
        /* [in] */ Int32 t,
        /* [in] */ Int32 r,
        /* [in] */ Int32 b);

    virtual CARAPI_(void) OnVisibilityChanged(
        /* [in] */ IView* changedView,
        /* [in] */ Int32 visibility);

    CARAPI Init(
        /* [in] */ IContext* context,
        /* [in] */ IAttributeSet* attrs = NULL);


private:
    CARAPI_(void) CancelGrab();

    CARAPI_(void) StartAnimating(
        /* [in] */ Boolean holdAfter);

    CARAPI_(void) OnAnimationDone();

    CARAPI_(Boolean) WithinView(
        /* [in] */ Float x,
        /* [in] */ Float y,
        /* [in] */ View* view);

    CARAPI_(Boolean) IsHorizontal();

    CARAPI_(void) ResetView();

    CARAPI_(void) MoveHandle(
        /* [in] */ Float x,
        /* [in] */ Float y);

    /**
     * Triggers haptic feedback.
     */
    CARAPI_(void) Vibrate(
        /* [in] */ Int64 duration);



    /**
     * Dispatches a trigger event to listener. Ignored if a listener is not set.
     * @param whichHandle the handle that triggered the event.
     */
    CARAPI_(void) DispatchTriggerEvent(
        /* [in] */ Int32 whichHandle);


    /**
     * Sets the current grabbed state, and dispatches a grabbed state change
     * event to our listener.
     */
    CARAPI_(void) SetGrabbedState(
        /* [in] */ Int32 newState);

    CARAPI_(void) Log(
        /* [in] */ const String& msg);

    CARAPI AnimationEndInStartAnimating(
        /* [in] */ Boolean holdAfter,
        /* [in] */ Int32 dx,
        /* [in] */ Int32 dy);

    CARAPI InitFromResource(
        /* [in] */ IContext* context,
        /* [in] */ IAttributeSet* attrs = NULL);

private:

    static const String TAG;
    static const Boolean DBG = FALSE;
    static const Int32 HORIZONTAL = 0; // as defined in attrs.xml
    static const Int32 VERTICAL = 1;

// TODO: Make these configurable
    static const Float THRESHOLD = 2.0f / 3.0f;
    static const Int64 VIBRATE_SHORT = 30;
    static const Int64 VIBRATE_LONG = 40;
    static const Int32 TRACKING_MARGIN = 50;
    static const Int32 ANIM_DURATION = 250; // Time for most animations (in ms)
    static const Int32 ANIM_TARGET_TIME = 500; // Time to show targets (in ms)

private:
    Boolean mHoldLeftOnTransition;// = true;
    Boolean mHoldRightOnTransition;// = true;

    AutoPtr<ISlidingTabOnTriggerListener> mOnTriggerListener;
    Int32 mGrabbedState;// = OnTriggerListener.NO_HANDLE;
    Boolean mTriggered;// = false;
//    AutoPtr<IVibrator> mVibrator;
    Float mDensity; // used to scale dimensions for bitmaps.

    /**
     * Either {@link #HORIZONTAL} or {@link #VERTICAL}.
     */
    Int32 mOrientation;

    AutoPtr<Slider> mLeftSlider;
    AutoPtr<Slider> mRightSlider;
    AutoPtr<Slider> mCurrentSlider;
    Boolean mTracking;
    Float mThreshold;
    AutoPtr<Slider> mOtherSlider;
    Boolean mAnimating;
    AutoPtr<IRect> mTmpRect;
    AutoPtr<AnimationDoneListener> mAnimationDoneListener;
protected:
    Mutex mLock;
};

}// namespace Internal
}// namespace Widget
}// namespace Droid
}// namespace Elastos

#endif
