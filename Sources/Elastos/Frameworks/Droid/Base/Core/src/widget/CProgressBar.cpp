#include "CProgressBar.h"

using Elastos::Droid::View::EIID_View;

namespace Elastos {
namespace Droid {
namespace Widget {

IVIEW_METHODS_IMPL(CProgressBar, ProgressBar)
IDRAWABLECALLBACK_METHODS_IMPL(CProgressBar, ProgressBar)
IKEYEVENTCALLBACK_METHODS_IMPL(CProgressBar, ProgressBar)
IACCESSIBILITYEVENTSOURCE_METHODS_IMPL(CProgressBar, ProgressBar)

PInterface CProgressBar::Probe(
    /* [in] */ REIID riid)
{
    if (riid == EIID_View) {
        return reinterpret_cast<PInterface>((View*)this);
    }

    return _CProgressBar::Probe(riid);
}

Mutex* CProgressBar::GetSelfLock()
{
    return &(ProgressBar::mLock);
}


ECode CProgressBar::constructor(
    /* [in] */ IContext* context)
{
    return ProgressBar::Init(context);
}

ECode CProgressBar::constructor(
    /* [in] */ IContext* context,
    /* [in] */ IAttributeSet* attrs)
{
    return ProgressBar::Init(context, attrs);
}

ECode CProgressBar::constructor(
    /* [in] */ IContext* context,
    /* [in] */ IAttributeSet* attrs,
    /* [in] */ Int32 defStyle)
{
    return ProgressBar::Init(context, attrs, defStyle);
}

ECode CProgressBar::constructor(
    /* [in] */ IContext* context,
    /* [in] */ IAttributeSet* attrs,
    /* [in] */ Int32 defStyle,
    /* [in] */ Int32 styleRes)
{
    return ProgressBar::Init(context, attrs, defStyle, styleRes);
}
/**
 * <p>Indicate whether this progress bar is in indeterminate mode.</p>
 *
 * @return true if the progress bar is in indeterminate mode
 */
ECode CProgressBar::IsIndeterminate(
/* [out] */ Boolean* indeterminate)
{
    *indeterminate = ProgressBar::IsIndeterminate();
    return NOERROR;
}

/**
 * <p>Change the indeterminate mode for this progress bar. In indeterminate
 * mode, the progress is ignored and the progress bar shows an infinite
 * animation instead.</p>
 *
 * If this progress bar's style only supports indeterminate mode (such as the circular
 * progress bars), then this will be ignored.
 *
 * @param indeterminate true to enable the indeterminate mode
 */
ECode CProgressBar::SetIndeterminate(
/* [in] */ Boolean indeterminate)
{
    return ProgressBar::SetIndeterminate(indeterminate);
}

/**
 * <p>Get the drawable used to draw the progress bar in
 * indeterminate mode.</p>
 *
 * @return a {@link android.graphics.drawable.Drawable} instance
 *
 * @see #setIndeterminateDrawable(android.graphics.drawable.Drawable)
 * @see #setIndeterminate(boolean)
 */
ECode CProgressBar::GetIndeterminateDrawable(
/* [out] */ IDrawable** d)
{
    AutoPtr<IDrawable> temp;
    temp = ProgressBar::GetIndeterminateDrawable();
    *d = temp;
    REFCOUNT_ADD(*d);
    return NOERROR;
}

/**
 * <p>Define the drawable used to draw the progress bar in
 * indeterminate mode.</p>
 *
 * @param d the new drawable
 *
 * @see #getIndeterminateDrawable()
 * @see #setIndeterminate(boolean)
 */
ECode CProgressBar::SetIndeterminateDrawable(
/* [in] */ IDrawable* d)
{
    return ProgressBar::SetIndeterminateDrawable(d);
}

/**
 * <p>Get the drawable used to draw the progress bar in
 * progress mode.</p>
 *
 * @return a {@link android.graphics.drawable.Drawable} instance
 *
 * @see #setProgressDrawable(android.graphics.drawable.Drawable)
 * @see #setIndeterminate(boolean)
 */
ECode CProgressBar::GetProgressDrawable(
/* [out] */ IDrawable** d)
{
    AutoPtr<IDrawable> temp;
    temp = ProgressBar::GetProgressDrawable();
    *d = temp;
    REFCOUNT_ADD(*d);
    return NOERROR;
}

/**
 * <p>Define the drawable used to draw the progress bar in
 * progress mode.</p>
 *
 * @param d the new drawable
 *
 * @see #getProgressDrawable()
 * @see #setIndeterminate(boolean)
 */
ECode CProgressBar::SetProgressDrawable(
/* [in] */ IDrawable* d)
{
    return ProgressBar::SetProgressDrawable(d);
}

/**
 * <p>Set the current progress to the specified value. Does not do anything
 * if the progress bar is in indeterminate mode.</p>
 *
 * @param progress the new progress, between 0 and {@link #getMax()}
 *
 * @see #setIndeterminate(boolean)
 * @see #isIndeterminate()
 * @see #getProgress()
 * @see #incrementProgressBy(int)
 */
ECode CProgressBar::SetProgress(
/* [in] */ Int32 progress)
{
    return ProgressBar::SetProgress(progress);
}

/**
 * <p>
 * Set the current secondary progress to the specified value. Does not do
 * anything if the progress bar is in indeterminate mode.
 * </p>
 *
 * @param secondaryProgress the new secondary progress, between 0 and {@link #getMax()}
 * @see #setIndeterminate(boolean)
 * @see #isIndeterminate()
 * @see #getSecondaryProgress()
 * @see #incrementSecondaryProgressBy(int)
 */
ECode CProgressBar::SetSecondaryProgress(
/* [in] */ Int32 secondaryProgress)
{
    return ProgressBar::SetSecondaryProgress(secondaryProgress);
}

/**
 * <p>Get the progress bar's current level of progress. Return 0 when the
 * progress bar is in indeterminate mode.</p>
 *
 * @return the current progress, between 0 and {@link #getMax()}
 *
 * @see #setIndeterminate(boolean)
 * @see #isIndeterminate()
 * @see #setProgress(int)
 * @see #setMax(int)
 * @see #getMax()
 */
ECode CProgressBar::GetProgress(
/* [out] */ Int32* progress)
{
    *progress = ProgressBar::GetProgress();
    return NOERROR;
}

/**
 * <p>Get the progress bar's current level of secondary progress. Return 0 when the
 * progress bar is in indeterminate mode.</p>
 *
 * @return the current secondary progress, between 0 and {@link #getMax()}
 *
 * @see #setIndeterminate(boolean)
 * @see #isIndeterminate()
 * @see #setSecondaryProgress(int)
 * @see #setMax(int)
 * @see #getMax()
 */
ECode CProgressBar::GetSecondaryProgress(
/* [out] */ Int32* secondaryProgress)
{
    *secondaryProgress = ProgressBar::GetSecondaryProgress();
    return NOERROR;
}

/**
 * <p>Return the upper limit of this progress bar's range.</p>
 *
 * @return a positive integer
 *
 * @see #setMax(int)
 * @see #getProgress()
 * @see #getSecondaryProgress()
 */
ECode CProgressBar::GetMax(
/* [out] */ Int32* max)
{
    *max = ProgressBar::GetMax();
    return NOERROR;
}

/**
 * <p>Set the range of the progress bar to 0...<tt>max</tt>.</p>
 *
 * @param max the upper range of this progress bar
 *
 * @see #getMax()
 * @see #setProgress(int)
 * @see #setSecondaryProgress(int)
 */
ECode CProgressBar::SetMax(
/* [in] */ Int32 max)
{
    return ProgressBar::SetMax(max);
}

/**
 * <p>Increase the progress bar's progress by the specified amount.</p>
 *
 * @param diff the amount by which the progress must be increased
 *
 * @see #setProgress(int)
 */
ECode CProgressBar::IncrementProgressBy(
/* [in] */ Int32 diff)
{
    return ProgressBar::IncrementProgressBy(diff);
}

/**
 * <p>Increase the progress bar's secondary progress by the specified amount.</p>
 *
 * @param diff the amount by which the secondary progress must be increased
 *
 * @see #setSecondaryProgress(int)
 */
ECode CProgressBar::IncrementSecondaryProgressBy(
/* [in] */ Int32 diff)
{
    return ProgressBar::IncrementSecondaryProgressBy(diff);
}

/**
 * Sets the acceleration curve for the indeterminate animation.
 * The interpolator is loaded as a resource from the specified context.
 *
 * @param context The application environment
 * @param resID The resource identifier of the interpolator to load
 */
ECode CProgressBar::SetInterpolator(
/* [in] */ IContext* ctx,
/* [in] */ Int32 resID)
{
    return ProgressBar::SetInterpolator(ctx, resID);
}

/**
 * Sets the acceleration curve for the indeterminate animation.
 * Defaults to a linear interpolation.
 *
 * @param interpolator The interpolator which defines the acceleration curve
 */
ECode CProgressBar::SetInterpolator(
/* [in] */ IInterpolator* interpolator)
{
    return ProgressBar::SetInterpolator(interpolator);
}

/**
 * Gets the acceleration curve type for the indeterminate animation.
 *
 * @return the {@link Interpolator} associated to this animation
 */
ECode CProgressBar::GetInterpolator(
/* [out] */ IInterpolator** interpolator)
{
    AutoPtr<IInterpolator> temp = ProgressBar::GetInterpolator();
    *interpolator = temp;
    REFCOUNT_ADD(*interpolator);
    return NOERROR;
}

}// namespace Widget
}// namespace Droid
}// namespace Elastos
