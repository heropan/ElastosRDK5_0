
#ifndef __ELASTOS_DROID_WIDGET_PROGRESSBAR_H__
#define __ELASTOS_DROID_WIDGET_PROGRESSBAR_H__

#include "R.h"
#include "view/View.h"
#include "util/Pools.h"
#include "os/Runnable.h"

using Elastos::Droid::R;
using Elastos::Droid::Os::Runnable;
using Elastos::Droid::Utility::IPool;
using Elastos::Droid::Utility::Pools;
using Elastos::Droid::Utility::IPoolable;
using Elastos::Droid::Utility::IPoolableManager;
using Elastos::Droid::Utility::EIID_IPoolableManager;
using Elastos::Droid::View::IView;
using Elastos::Droid::Graphics::Drawable::Shapes::IShape;

namespace Elastos {
namespace Droid {
namespace Widget {

using Elastos::Droid::View::View;
using Elastos::Droid::View::Animation::IInterpolator;

class ProgressBar : public View
{
private:
    class RefreshProgressRunnable
        : public Runnable
    {
    public:
        CARAPI Run();

        RefreshProgressRunnable(
            /* [in] */ IWeakReference* host);

    private:
        AutoPtr<IWeakReference> mWeakHost;
    };// RefreshProgressRunnable

    class AccessibilityEventSender
        : public Runnable
    {
    public:
        CARAPI Run();

        AccessibilityEventSender(
            /* [in] */ IWeakReference* host);

    private:
        AutoPtr<IWeakReference> mWeakHost;
    };// AccessibilityEventSender

    class RefreshData
        : public IPoolable
        , public ElRefBase
    {
    public:
        CAR_INTERFACE_DECL()

        CARAPI Recycle();

        CARAPI SetNextPoolable(
            /* [in] */ IPoolable* element);

        CARAPI GetNextPoolable(
            /* [out] */ IPoolable** element);

        CARAPI IsPooled(
            /* [out] */ Boolean* isPooled);

        CARAPI SetPooled(
            /* [in] */ Boolean isPooled);

        static CARAPI_(AutoPtr<RefreshData>) Obtain(
                /* [in] */ Int32 id,
                /* [in] */ Int32 progress,
                /* [in] */ Boolean fromUser);

    private:
        RefreshData();

    private:
        class RefreshDataPoolableManager
            : public ElRefBase
            , public IPoolableManager
        {
        public:
            CAR_INTERFACE_DECL()

            CARAPI NewInstance(
                /* [out] */ IPoolable** element);

            CARAPI OnAcquired(
                /* [in] */ IPoolable* element);

            CARAPI OnReleased(
                /* [in] */ IPoolable* element);
        };// RefreshDataPoolableManager

    public:
        Int32 mId;
        Int32 mProgress;
        Boolean mFromUser;
    private:
        AutoPtr<RefreshData> mNext;
        Boolean mIsPooled;
        static AutoPtr<IPool> sPool;
        static const Int32 POOL_MAX = 24;
    };// RefreshData

    /**
     * Command for sending an accessibility event.
     */

public:
    /**
     * Create a new progress bar with range 0...100 and initial progress of 0.
     * @param context the application environment
     */
    ProgressBar(
        /* [in] */ IContext* context,
        /* [in] */ IAttributeSet* attrs = NULL,
        /* [in] */ Int32 defStyle = R::attr::progressBarStyle,
        /* [in] */ Int32 styleRes = 0);

    virtual CARAPI_(AutoPtr<IShape>) GetDrawableShape();

    /**
     * <p>Indicate whether this progress bar is in indeterminate mode.</p>
     *
     * @return TRUE if the progress bar is in indeterminate mode
     */
    //synchronized
    virtual CARAPI_(Boolean) IsIndeterminate();

    /**
     * <p>Change the indeterminate mode for this progress bar. In indeterminate
     * mode, the progress is ignored and the progress bar shows an infinite
     * animation instead.</p>
     *
     * If this progress bar's style only supports indeterminate mode (such as the circular
     * progress bars), then this will be ignored.
     *
     * @param indeterminate TRUE to enable the indeterminate mode
     */
    //synchronized
    virtual CARAPI SetIndeterminate(
        /* [in] */ Boolean indeterminate);

    /**
     * <p>Get the drawable used to draw the progress bar in
     * indeterminate mode.</p>
     *
     * @return a {@link android.graphics.drawable.Drawable} instance
     *
     * @see #setIndeterminateDrawable(android.graphics.drawable.Drawable)
     * @see #setIndeterminate(Boolean)
     */
    virtual CARAPI_(AutoPtr<IDrawable>) GetIndeterminateDrawable();

    /**
     * <p>Define the drawable used to draw the progress bar in
     * indeterminate mode.</p>
     *
     * @param d the new drawable
     *
     * @see #getIndeterminateDrawable()
     * @see #setIndeterminate(Boolean)
     */
    virtual CARAPI SetIndeterminateDrawable(
        /* [in] */ IDrawable* d);

    /**
     * <p>Get the drawable used to draw the progress bar in
     * progress mode.</p>
     *
     * @return a {@link android.graphics.drawable.Drawable} instance
     *
     * @see #setProgressDrawable(android.graphics.drawable.Drawable)
     * @see #setIndeterminate(Boolean)
     */
    virtual CARAPI_(AutoPtr<IDrawable>) GetProgressDrawable();

    /**
     * <p>Define the drawable used to draw the progress bar in
     * progress mode.</p>
     *
     * @param d the new drawable
     *
     * @see #getProgressDrawable()
     * @see #setIndeterminate(Boolean)
     */
    virtual CARAPI SetProgressDrawable(
        /* [in] */ IDrawable* d);

    /**
     * @return The drawable currently used to draw the progress bar
     */
    virtual CARAPI_(AutoPtr<IDrawable>) GetCurrentDrawable();

    using View::PostInvalidate;

    //@Override
    CARAPI PostInvalidate();

    virtual CARAPI_(void) OnProgressRefresh(
        /* [in] */ Float scale,
        /* [in] */ Boolean fromUser);

    /**
     * <p>Set the current progress to the specified value. Does not do anything
     * if the progress bar is in indeterminate mode.</p>
     *
     * @param progress the new progress, between 0 and {@link #getMax()}
     *
     * @see #setIndeterminate(Boolean)
     * @see #isIndeterminate()
     * @see #getProgress()
     * @see #incrementProgressBy(Int32)
     */
    //synchronized
    virtual CARAPI SetProgress(
        /* [in] */ Int32 progress);

    //synchronized
    virtual CARAPI SetProgress(
        /* [in] */ Int32 progress,
        /* [in] */ Boolean fromUser);

    /**
     * <p>
     * Set the current secondary progress to the specified value. Does not do
     * anything if the progress bar is in indeterminate mode.
     * </p>
     *
     * @param secondaryProgress the new secondary progress, between 0 and {@link #getMax()}
     * @see #setIndeterminate(Boolean)
     * @see #isIndeterminate()
     * @see #getSecondaryProgress()
     * @see #incrementSecondaryProgressBy(Int32)
     */
    //synchronized
    virtual CARAPI SetSecondaryProgress(
        /* [in] */ Int32 secondaryProgress);

    /**
     * <p>Get the progress bar's current level of progress. Return 0 when the
     * progress bar is in indeterminate mode.</p>
     *
     * @return the current progress, between 0 and {@link #getMax()}
     *
     * @see #setIndeterminate(Boolean)
     * @see #isIndeterminate()
     * @see #setProgress(Int32)
     * @see #setMax(Int32)
     * @see #getMax()
     */
    //synchronized
    virtual CARAPI_(Int32) GetProgress();

    /**
     * <p>Get the progress bar's current level of secondary progress. Return 0 when the
     * progress bar is in indeterminate mode.</p>
     *
     * @return the current secondary progress, between 0 and {@link #getMax()}
     *
     * @see #setIndeterminate(Boolean)
     * @see #isIndeterminate()
     * @see #setSecondaryProgress(Int32)
     * @see #setMax(Int32)
     * @see #getMax()
     */
    //synchronized
    virtual CARAPI_(Int32) GetSecondaryProgress();

    /**
     * <p>Return the upper limit of this progress bar's range.</p>
     *
     * @return a positive integer
     *
     * @see #setMax(Int32)
     * @see #getProgress()
     * @see #getSecondaryProgress()
     */
    //synchronized
    virtual CARAPI_(Int32) GetMax();

    /**
     * <p>Set the range of the progress bar to 0...<tt>max</tt>.</p>
     *
     * @param max the upper range of this progress bar
     *
     * @see #getMax()
     * @see #setProgress(Int32)
     * @see #setSecondaryProgress(Int32)
     */
    //synchronized
    virtual CARAPI SetMax(
        /* [in] */ Int32 max);

    /**
     * <p>Increase the progress bar's progress by the specified amount.</p>
     *
     * @param diff the amount by which the progress must be increased
     *
     * @see #setProgress(Int32)
     */
    //synchronized
    CARAPI IncrementProgressBy(
        /* [in] */ Int32 diff);

    /**
     * <p>Increase the progress bar's secondary progress by the specified amount.</p>
     *
     * @param diff the amount by which the secondary progress must be increased
     *
     * @see #setSecondaryProgress(Int32)
     */
    //synchronized
    CARAPI IncrementSecondaryProgressBy(
        /* [in] */ Int32 diff);

    using View::StartAnimation;

    /**
     * <p>Start the indeterminate progress animation.</p>
     */
    virtual CARAPI_(void) StartAnimation();

    /**
     * <p>Stop the indeterminate progress animation.</p>
     */
    virtual CARAPI_(void) StopAnimation();

    /**
     * Sets the acceleration curve for the indeterminate animation.
     * The interpolator is loaded as a resource from the specified context.
     *
     * @param context The application environment
     * @param resID The resource identifier of the interpolator to load
     */
    virtual CARAPI SetInterpolator(
        /* [in] */ IContext* context,
        /* [in] */ Int32 resID);

    /**
     * Sets the acceleration curve for the indeterminate animation.
     * Defaults to a linear interpolation.
     *
     * @param interpolator The interpolator which defines the acceleration curve
     */
    virtual CARAPI SetInterpolator(
        /* [in] */ IInterpolator* interpolator);

    /**
     * Gets the acceleration curve type for the indeterminate animation.
     *
     * @return the {@link Interpolator} associated to this animation
     */
    virtual CARAPI_(AutoPtr<IInterpolator>) GetInterpolator();

    virtual CARAPI SetVisibility(
        /* [in] */ Int32 v);

    //@Override
    virtual CARAPI InvalidateDrawable(
            /* [in] */ IDrawable* dr);

    //@Override
    virtual CARAPI_(AutoPtr<IParcelable>) OnSaveInstanceState();

    //@Override
    virtual CARAPI_(void) OnRestoreInstanceState(
        /* [in] */ IParcelable* state);

    CARAPI JumpDrawablesToCurrentState();

    CARAPI OnResolveDrawables(
        /* [in] */ Int32 layoutDirection);

    CARAPI OnInitializeAccessibilityEvent(
        /* [in] */ IAccessibilityEvent* event);

    CARAPI OnInitializeAccessibilityNodeInfo(
        /* [in] */ IAccessibilityNodeInfo* info);

protected:
    ProgressBar();

    CARAPI Init(
        /* [in] */ IContext* context,
        /* [in] */ IAttributeSet* attrs = NULL,
        /* [in] */ Int32 defStyle = R::attr::progressBarStyle,
        /* [in] */ Int32 styleRes = 0);

    //@Override
    virtual CARAPI_(Boolean) VerifyDrawable(
            /* [in] */ IDrawable* who);

    //@Override
    virtual CARAPI_(void) OnVisibilityChanged(
        /* [in] */ IView* changedView,
        /* [in] */ Int32 visibility);

    //@Override
    virtual CARAPI_(void) OnSizeChanged(
        /* [in] */ Int32 w,
        /* [in] */ Int32 h,
        /* [in] */ Int32 oldw,
        /* [in] */ Int32 oldh);

    //@Override
    //synchronized
    virtual CARAPI_(void) OnDraw(
        /* [in] */ ICanvas* canvas);

    //@Override
    //synchronized
    virtual CARAPI_(void) OnMeasure(
        /* [in] */ Int32 widthMeasureSpec,
        /* [in] */ Int32 heightMeasureSpec);

    //@Override
    virtual CARAPI DrawableStateChanged();

    //@Override
    virtual CARAPI OnAttachedToWindow();

    //@Override
    virtual CARAPI OnDetachedFromWindow();

    virtual CARAPI_(Object*) GetSelfLock() = 0;


private:
    CARAPI InitFromAttributes(
        /* [in] */ IContext* context,
        /* [in] */ IAttributeSet* attrs,
        /* [in] */ Int32 defStyle,
        /* [in] */ Int32 styleRes);

    /**
     * Converts a drawable to a tiled version of itself. It will recursively
     * traverse layer and state list drawables.
     */
    CARAPI_(AutoPtr<IDrawable>) Tileify(
        /* [in] */ IDrawable* drawable,
        /* [in] */ Boolean clip);

    /**
     * Convert a AnimationDrawable for use as a barberpole animation.
     * Each frame of the animation is wrapped in a ClipDrawable and
     * given a tiling BitmapShader.
     */
    CARAPI_(AutoPtr<IDrawable>) TileifyIndeterminate(
        /* [in] */ IDrawable* drawable);

    /**
     * <p>
     * Initialize the progress bar's default values:
     * </p>
     * <ul>
     * <li>progress = 0</li>
     * <li>max = 100</li>
     * <li>animation duration = 4000 ms</li>
     * <li>indeterminate = FALSE</li>
     * <li>behavior = repeat</li>
     * </ul>
     */
    CARAPI_(void) InitProgressBar();

    //synchronized
    CARAPI_(void) DoRefreshProgress(
        /* [in] */ Int32 id,
        /* [in] */ Int32 progress,
        /* [in] */ Boolean fromUser,
        /* [in] */ Boolean callBackToApp);

    //synchronized
    CARAPI_(void) RefreshProgress(
        /* [in] */ Int32 id,
        /* [in] */ Int32 progress,
        /* [in] */ Boolean fromUser);

    CARAPI_(void) UpdateDrawableBounds(
        /* [in] */ Int32 w,
        /* [in] */ Int32 h);

    CARAPI_(void) UpdateDrawableState();

    CARAPI_(void) ScheduleAccessibilityEventSender();

private:
    static const Int32 MAX_LEVEL = 10000;
    static const Int32 TIMEOUT_SEND_ACCESSIBILITY_EVENT = 200;

protected:
    Int32 mMinWidth;
    Int32 mMaxWidth;
    Int32 mMinHeight;
    Int32 mMaxHeight;
    AutoPtr<IBitmap> mSampleTile;

private:
    Int32 mProgress;
    Int32 mSecondaryProgress;
    Int32 mMax;

    Int32 mBehavior;
    Int32 mDuration;
    Boolean mIndeterminate;
    Boolean mOnlyIndeterminate;
    AutoPtr<ITransformation> mTransformation;
    AutoPtr<IAnimation> mAnimation; //AlphaAnimation
    Boolean mHasAnimation;
    AutoPtr<IDrawable> mIndeterminateDrawable;
    AutoPtr<IDrawable> mProgressDrawable;
    AutoPtr<IDrawable> mCurrentDrawable;

    Boolean mNoInvalidate;
    AutoPtr<IInterpolator> mInterpolator;
    AutoPtr<RefreshProgressRunnable> mRefreshProgressRunnable;
    Int64 mUiThreadId;
    Boolean mShouldStartAnimationDrawable;

    Boolean mInDrawing;

    Boolean mAttached;
    Boolean mRefreshIsPosted;

    List<AutoPtr<RefreshData> > mRefreshData;

    AutoPtr<AccessibilityEventSender> mAccessibilityEventSender;
protected:
    Object mLock;
};

}// namespace Widget
}// namespace Droid
}// namespace Elastos

#endif //__ELASTOS_DROID_WIDGET_PROGRESSBAR_H__
