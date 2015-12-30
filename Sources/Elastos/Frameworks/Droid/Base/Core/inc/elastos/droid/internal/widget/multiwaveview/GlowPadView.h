
#ifndef __ELASTOS_DROID_INTERNAL_WIDGET_MULTIWAVEVIEW_GLOWPADVIEW_H__
#define __ELASTOS_DROID_INTERNAL_WIDGET_MULTIWAVEVIEW_GLOWPADVIEW_H__

#include "Elastos.Droid.Internal.h"
#include "Elastos.Droid.Os.h"
#include "elastos/droid/ext/frameworkext.h"
#include "elastos/droid/view/View.h"
#include "elastos/droid/animation/AnimatorListenerAdapter.h"

#include <elastos/utility/AbstractList.h>

using Elastos::Droid::Animation::AnimatorListenerAdapter;
using Elastos::Droid::Animation::IAnimator;
using Elastos::Droid::Animation::IAnimatorUpdateListener;
using Elastos::Droid::Animation::IValueAnimator;
using Elastos::Droid::Animation::IAnimatorListener;
using Elastos::Droid::Content::IComponentName;
using Elastos::Droid::Media::IAudioAttributes;
using Elastos::Droid::Os::IVibrator;
using Elastos::Droid::View::IMotionEvent;

using Elastos::Utility::AbstractList;

namespace Elastos {
namespace Droid {
namespace Internal {
namespace Widget {
namespace Multiwaveview {

/**
 * A re-usable widget containing a center, outer ring and wave animation.
 */
class GlowPadView
    : public Elastos::Droid::View::View
    , public IGlowPadView
{
private:
    class AnimationBundle
//        : public ArrayList
        : public AbstractList
        , public IAnimationBundle
    {
    public:
        CAR_INTERFACE_DECL()

        CARAPI Start();

        CARAPI Cancel();

        CARAPI Stop();

        CARAPI SetSuspended(
            /* [in] */ Boolean suspend);

        CARAPI Get(
            /* [in] */ Int32 location,
            /* [out] */ IInterface** object);

    private:
        static Int64 mSerialVersionUID;
        Boolean mSuspended;
    };

    class AnimatorListenerAdapter_1
        : public AnimatorListenerAdapter
    {
    public:
        AnimatorListenerAdapter_1(
            /* [in] */ GlowPadView* host);

        CARAPI OnAnimationEnd(
            /* [in] */ IAnimator* animator);

    public:
        GlowPadView* mHost;
    };

    class AnimatorListenerAdapter_2
        : public AnimatorListenerAdapter
    {
    public:
        AnimatorListenerAdapter_2(
            /* [in] */ GlowPadView* host);

        CARAPI OnAnimationEnd(
            /* [in] */ IAnimator* animator);

    public:
        GlowPadView* mHost;
    };

    class AnimatorUpdateListener_1
        : public Object
        , public IAnimatorUpdateListener
    {
    public:
        CAR_INTERFACE_DECL()

        AnimatorUpdateListener_1(
            /* [in] */ GlowPadView* host);

        CARAPI OnAnimationUpdate(
            /* [in] */ IValueAnimator* animation);

    public:
        GlowPadView* mHost;
    };

    class AnimatorListenerAdapter_3
        : public AnimatorListenerAdapter
    {
    public:
        AnimatorListenerAdapter_3(
            /* [in] */ GlowPadView* host);

        CARAPI OnAnimationEnd(
            /* [in] */ IAnimator* animator);

    public:
        GlowPadView* mHost;
    };

    class AnimatorListenerAdapter_4
        : public AnimatorListenerAdapter
    {
    public:
        AnimatorListenerAdapter_4(
            /* [in] */ GlowPadView* host);

        CARAPI OnAnimationEnd(
            /* [in] */ IAnimator* animator);

    public:
        GlowPadView* mHost;
    };

public:
    CAR_INTERFACE_DECL()

    GlowPadView();

    CARAPI constructor(
        /* [in] */ IContext* context);

    CARAPI constructor(
        /* [in] */ IContext* context,
        /* [in] */ IAttributeSet* attrs);

    CARAPI SuspendAnimations();

    CARAPI ResumeAnimations();

    /**
     * Loads an array of drawables from the given resourceId.
     *
     * @param resourceId
     */
    CARAPI SetTargetResources(
        /* [in] */ Int32 resourceId);

    CARAPI GetTargetResourceId(
        /* [out] */ Int32* result);

    /**
     * Sets the resource id specifying the target descriptions for accessibility.
     *
     * @param resourceId The resource id.
     */
    CARAPI SetTargetDescriptionsResourceId(
        /* [in] */ Int32 resourceId);

    /**
     * Gets the resource id specifying the target descriptions for accessibility.
     *
     * @return The resource id.
     */
    CARAPI GetTargetDescriptionsResourceId(
        /* [out] */ Int32* result);

    /**
     * Sets the resource id specifying the target direction descriptions for accessibility.
     *
     * @param resourceId The resource id.
     */
    CARAPI SetDirectionDescriptionsResourceId(
        /* [in] */ Int32 resourceId);

    /**
     * Gets the resource id specifying the target direction descriptions.
     *
     * @return The resource id.
     */
    CARAPI GetDirectionDescriptionsResourceId(
        /* [out] */ Int32* result);

    /**
     * Enable or disable vibrate on touch.
     *
     * @param enabled
     */
    CARAPI SetVibrateEnabled(
        /* [in] */ Boolean enabled);

    /**
     * Starts wave animation.
     *
     */
    CARAPI Ping();

    /**
     * Resets the widget to default state and cancels all animation. If animate is 'true', will
     * animate objects into place. Otherwise, objects will snap back to place.
     *
     * @param animate
     */
    CARAPI Reset(
        /* [in] */ Boolean animate);

    CARAPI OnTouchEvent(
        /* [in] */ IMotionEvent* event,
        /* [out] */ Boolean* result);

    CARAPI OnHoverEvent(
        /* [in] */ IMotionEvent* event,
        /* [out] */ Boolean* result);

    CARAPI SetOnTriggerListener(
        /* [in] */ IOnTriggerListener* listener);

    CARAPI GetResourceIdForTarget(
        /* [in] */ Int32 index,
        /* [out] */ Int32* result);

    CARAPI SetEnableTarget(
        /* [in] */ Int32 resourceId,
        /* [in] */ Boolean enabled);

    /**
     * Gets the position of a target in the array that matches the given resource.
     * @param resourceId
     * @return the index or -1 if not found
     */
    CARAPI GetTargetPosition(
        /* [in] */ Int32 resourceId,
        /* [out] */ Int32* result);

    /**
     * Searches the given package for a resource to use to replace the Drawable on the
     * target with the given resource id
     * @param component of the .apk that contains the resource
     * @param name of the metadata in the .apk
     * @param existingResId the resource id of the target to search for
     * @return true if found in the given package and replaced at least one target Drawables
     */
    CARAPI ReplaceTargetDrawablesIfPresent(
        /* [in] */ IComponentName* component,
        /* [in] */ const String& name,
        /* [in] */ Int32 existingResId,
        /* [out] */ Boolean* result);

protected:
    CARAPI_(Int32) GetSuggestedMinimumWidth();

    CARAPI_(Int32) GetSuggestedMinimumHeight();

    /**
     * This gets the suggested width accounting for the ring's scale factor.
     */
    CARAPI_(Int32) GetScaledSuggestedMinimumWidth();

    /**
     * This gets the suggested height accounting for the ring's scale factor.
     */
    CARAPI_(Int32) GetScaledSuggestedMinimumHeight();

    CARAPI_(void) OnMeasure(
        /* [in] */ Int32 widthMeasureSpec,
        /* [in] */ Int32 heightMeasureSpec);

    CARAPI OnLayout(
        /* [in] */ Boolean changed,
        /* [in] */ Int32 left,
        /* [in] */ Int32 top,
        /* [in] */ Int32 right,
        /* [in] */ Int32 bottom);

    CARAPI_(void) OnDraw(
        /* [in] */ ICanvas* canvas);

private:
    CARAPI_(Int32) GetResourceId(
        /* [in] */ ITypedArray* a,
        /* [in] */ Int32 id);

    CARAPI_(void) Dump();

    CARAPI_(Int32) ResolveMeasured(
        /* [in] */ Int32 measureSpec,
        /* [in] */ Int32 desired);

    CARAPI_(void) SwitchToState(
        /* [in] */ Int32 state,
        /* [in] */ Float x,
        /* [in] */ Float y);

    CARAPI_(void) ShowGlow(
        /* [in] */ Int32 duration,
        /* [in] */ Int32 delay,
        /* [in] */ Float finalAlpha,
        /* [in] */ IAnimatorListener* finishListener);

    CARAPI_(void) HideGlow(
        /* [in] */ Int32 duration,
        /* [in] */ Int32 delay,
        /* [in] */ Float finalAlpha,
        /* [in] */ IAnimatorListener* finishListener);

    CARAPI_(void) DeactivateTargets();

    /**
     * Dispatches a trigger event to listener. Ignored if a listener is not set.
     * @param whichTarget the target that was triggered.
     */
    CARAPI_(void) DispatchTriggerEvent(
        /* [in] */ Int32 whichTarget);

    CARAPI_(void) DispatchOnFinishFinalAnimation();

    CARAPI_(void) DoFinish();

    CARAPI_(void) HighlightSelected(
        /* [in] */ Int32 activeTarget);

    CARAPI_(void) HideUnselected(
        /* [in] */ Int32 active);

    CARAPI_(void) HideTargets(
        /* [in] */ Boolean animate,
        /* [in] */ Boolean expanded);

    CARAPI_(void) ShowTargets(
        /* [in] */ Boolean animate);

    CARAPI_(void) Vibrate();

    CARAPI_(AutoPtr<IArrayList>) LoadDrawableArray(
        /* [in] */ Int32 resourceId);

    CARAPI_(void) InternalSetTargetResources(
        /* [in] */ Int32 resourceId);

    CARAPI_(void) StopAndHideWaveAnimation();

    CARAPI_(void) StartWaveAnimation();

    CARAPI_(void) StartBackgroundAnimation(
        /* [in] */ Int32 duration,
        /* [in] */ Float alpha);

    CARAPI_(void) UpdateGlowPosition(
        /* [in] */ Float x,
        /* [in] */ Float y);

    CARAPI_(void) HandleDown(
        /* [in] */ IMotionEvent* event);

    CARAPI_(void) HandleUp(
        /* [in] */ IMotionEvent* event);

    CARAPI_(void) HandleCancel(
        /* [in] */ IMotionEvent* event);

    CARAPI_(void) HandleMove(
        /* [in] */ IMotionEvent* event);

    /**
     * Sets the current grabbed state, and dispatches a grabbed state change
     * event to our listener.
     */
    CARAPI_(void) SetGrabbedState(
        /* [in] */ Int32 newState);

    CARAPI_(Boolean) TrySwitchToFirstTouchState(
        /* [in] */ Float x,
        /* [in] */ Float y);

    CARAPI_(void) AssignDefaultsIfNeeded();

    CARAPI_(void) ComputeInsets(
        /* [in] */ Int32 dx,
        /* [in] */ Int32 dy);

    /**
     * Given the desired width and height of the ring and the allocated width and height, compute
     * how much we need to scale the ring.
     */
    CARAPI_(Float) ComputeScaleFactor(
        /* [in] */ Int32 desiredWidth,
        /* [in] */ Int32 desiredHeight,
        /* [in] */ Int32 actualWidth,
        /* [in] */ Int32 actualHeight);

    CARAPI_(Float) GetRingWidth();

    CARAPI_(Float) GetRingHeight();

    CARAPI_(void) UpdateTargetPosition(
        /* [in] */ Int32 i,
        /* [in] */ Float centerX,
        /* [in] */ Float centerY);

    CARAPI_(void) UpdateTargetPosition(
        /* [in] */ Int32 i,
        /* [in] */ Float centerX,
        /* [in] */ Float centerY,
        /* [in] */ Float angle);

    CARAPI_(void) UpdateTargetPositions(
        /* [in] */ Float centerX,
        /* [in] */ Float centerY);

    CARAPI_(void) UpdateTargetPositions(
        /* [in] */ Float centerX,
        /* [in] */ Float centerY,
        /* [in] */ Boolean skipActive);

    CARAPI_(Float) GetAngle(
        /* [in] */ Float alpha,
        /* [in] */ Int32 i);

    CARAPI_(Float) GetSliceAngle();

    CARAPI_(void) UpdatePointCloudPosition(
        /* [in] */ Float centerX,
        /* [in] */ Float centerY);

    CARAPI_(Float) Square(
        /* [in] */ Float d);

    CARAPI_(Float) Dist2(
        /* [in] */ Float dx,
        /* [in] */ Float dy);

    CARAPI_(Float) GetScaledGlowRadiusSquared();

    CARAPI_(void) AnnounceTargets();

    CARAPI_(String) GetTargetDescription(
        /* [in] */ Int32 index);

    CARAPI_(String) GetDirectionDescription(
        /* [in] */ Int32 index);

    CARAPI_(AutoPtr<IArrayList>) LoadDescriptions(
        /* [in] */ Int32 resourceId);

    CARAPI_(Boolean) ReplaceTargetDrawables(
        /* [in] */ IResources* res,
        /* [in] */ Int32 existingResourceId,
        /* [in] */ Int32 newResourceId);

private:
    static String TAG;
    static Boolean DEBUG;

    // Wave state machine
    static const Int32 STATE_IDLE;
    static const Int32 STATE_START;
    static const Int32 STATE_FIRST_TOUCH;
    static const Int32 STATE_TRACKING;
    static const Int32 STATE_SNAP;
    static const Int32 STATE_FINISH;

    // Animation properties.
    static Float SNAP_MARGIN_DEFAULT; // distance to ring before we snap to it

    // Tuneable parameters for animation
    static Int32 WAVE_ANIMATION_DURATION;
    static Int32 RETURN_TO_HOME_DELAY;
    static Int32 RETURN_TO_HOME_DURATION;
    static Int32 HIDE_ANIMATION_DELAY;
    static Int32 HIDE_ANIMATION_DURATION;
    static Int32 SHOW_ANIMATION_DURATION;
    static Int32 SHOW_ANIMATION_DELAY;
    static Int32 INITIAL_SHOW_HANDLE_DURATION;
    static Int32 REVEAL_GLOW_DELAY;
    static Int32 REVEAL_GLOW_DURATION;

    static Float TAP_RADIUS_SCALE_ACCESSIBILITY_ENABLED;
    static Float TARGET_SCALE_EXPANDED;
    static Float TARGET_SCALE_COLLAPSED;
    static Float RING_SCALE_EXPANDED;
    static Float RING_SCALE_COLLAPSED;

    static AutoPtr<IAudioAttributes> VIBRATION_ATTRIBUTES;

    AutoPtr<IArrayList> mTargetDrawables;
    AutoPtr<IAnimationBundle> mWaveAnimations;
    AutoPtr<IAnimationBundle> mTargetAnimations;
    AutoPtr<IAnimationBundle> mGlowAnimations;
    AutoPtr<IArrayList> mTargetDescriptions;
    AutoPtr<IArrayList> mDirectionDescriptions;
    AutoPtr<IOnTriggerListener> mOnTriggerListener;
    AutoPtr<ITargetDrawable> mHandleDrawable;
    AutoPtr<ITargetDrawable> mOuterRing;
    AutoPtr<IVibrator> mVibrator;

    Int32 mFeedbackCount;
    Int32 mVibrationDuration;
    Int32 mGrabbedState;
    Int32 mActiveTarget;
    Float mGlowRadius;
    Float mWaveCenterX;
    Float mWaveCenterY;
    Int32 mMaxTargetHeight;
    Int32 mMaxTargetWidth;
    Float mRingScaleFactor;
    Boolean mAllowScaling;

    Float mOuterRadius;
    Float mSnapMargin;
    Float mFirstItemOffset;
    Boolean mMagneticTargets;
    Boolean mDragging;
    Int32 mNewTargetResources;

    AutoPtr<IAnimatorListener> mResetListener;

    AutoPtr<IAnimatorListener> mResetListenerWithPing;

    AutoPtr<IAnimatorUpdateListener> mUpdateListener;

    Boolean mAnimatingTargets;
    AutoPtr<IAnimatorListener> mTargetUpdateListener;

    Int32 mTargetResourceId;
    Int32 mTargetDescriptionsResourceId;
    Int32 mDirectionDescriptionsResourceId;
    Boolean mAlwaysTrackFinger;
    Int32 mHorizontalInset;
    Int32 mVerticalInset;
    Int32 mGravity;
    Boolean mInitialLayout;
    AutoPtr<ITweener> mBackgroundAnimator;
    AutoPtr<IPointCloud> mPointCloud;
    Float mInnerRadius;
    Int32 mPointerId;
};

} // namespace Multiwaveview
} // namespace Widget
} // namespace Internal
} // namespace Droid
} // namespace Elastos

#endif // __ELASTOS_DROID_INTERNAL_WIDGET_MULTIWAVEVIEW_GLOWPADVIEW_H__
