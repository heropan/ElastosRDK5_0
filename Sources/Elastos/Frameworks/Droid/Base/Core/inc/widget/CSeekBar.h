
#ifndef __CSEEKBAR_H__
#define __CSEEKBAR_H__

#include "_CSeekBar.h"
#include "widget/SeekBar.h"



namespace Elastos {
namespace Droid {
namespace Widget {

/**
 * A SeekBar is an extension of ProgressBar that adds a draggable thumb. The user can touch
 * the thumb and drag left or right to set the current progress level or use the arrow keys.
 * Placing focusable widgets to the left or right of a SeekBar is discouraged.
 * <p>
 * Clients of the SeekBar can attach a {@link SeekBar.OnSeekBarChangeListener} to
 * be notified of the user's actions.
 *
 * @attr ref android.R.styleable#SeekBar_thumb
 */
CarClass(CSeekBar) , public SeekBar
{
public:
    IVIEW_METHODS_DECL()
    IDRAWABLECALLBACK_METHODS_DECL()
    IKEYEVENTCALLBACK_METHODS_DECL()
    IACCESSIBILITYEVENTSOURCE_METHODS_DECL()

    CARAPI constructor(
        /* [in] */ IContext* ctx);

    CARAPI constructor(
        /* [in] */ IContext* ctx,
        /* [in] */ IAttributeSet* attrs);

    CARAPI constructor(
        /* [in] */ IContext* ctx,
        /* [in] */ IAttributeSet* attrs,
        /* [in] */ Int32 defStyle);

    CARAPI_(PInterface) Probe(
        /* [in] */ REIID riid);

    // inteface IProgressBar

    CARAPI IsIndeterminate(
        /* [out] */ Boolean* isIndeterminate);

    CARAPI SetIndeterminate(
        /* [in] */ Boolean indeterminate);

    CARAPI GetIndeterminateDrawable(
        /* [out] */ IDrawable** drawable);

    CARAPI SetIndeterminateDrawable(
        /* [in] */ IDrawable* drawable);

    CARAPI GetProgressDrawable(
        /* [out] */ IDrawable** drawable);

    CARAPI SetProgressDrawable(
        /* [in] */ IDrawable* drawable);

    CARAPI SetProgress(
        /* [in] */ Int32 progress);

    CARAPI SetSecondaryProgress(
        /* [in] */ Int32 secondaryProgress);

    CARAPI GetProgress(
        /* [out] */ Int32* progress);

    CARAPI GetSecondaryProgress(
        /* [out] */ Int32* secProgress);

    CARAPI GetMax(
        /* [out] */ Int32* max);

    CARAPI SetMax(
        /* [in] */ Int32 max);

    CARAPI IncrementProgressBy(
        /* [in] */ Int32 diff);

    CARAPI IncrementSecondaryProgressBy(
        /* [in] */ Int32 diff);

    CARAPI SetInterpolator(
        /* [in] */ IContext* ctx,
        /* [in] */ Int32 resID);

    CARAPI SetInterpolator(
        /* [in] */ IInterpolator* interpolator);

    CARAPI GetInterpolator(
        /* [out] */ IInterpolator** interpolator);

    // interface IProgressBar

    CARAPI SetThumb(
        /* [in] */ IDrawable* thumb);

    CARAPI GetThumb(
        /* [out] */ IDrawable** thumb);

    CARAPI GetThumbOffset(
        /* [out] */ Int32 * pOffset);

    CARAPI SetThumbOffset(
        /* [in] */ Int32 thumbOffset);

    CARAPI SetKeyProgressIncrement(
        /* [in] */ Int32 increment);

    CARAPI GetKeyProgressIncrement(
        /* [out] */ Int32* increment);

    /**
     * Sets a listener to receive notifications of changes to the SeekBar's progress level. Also
     * provides notifications of when the user starts and stops a touch gesture within the SeekBar.
     *
     * @param l The seek bar notification listener
     *
     * @see SeekBar.OnSeekBarChangeListener
     */
    CARAPI SetOnSeekBarChangeListener(
        /* [in] */ ISeekBarOnSeekBarChangeListener* l);

private:
    CARAPI_(Object*) GetSelfLock();
};

}// namespace Widget
}// namespace Droid
}// namespace Elastos

#endif //__CSEEKBAR_H__
