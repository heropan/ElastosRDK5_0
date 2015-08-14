
#ifndef __ELASTOS_DROID_WIDGET_ABSSEEKBAR_H__
#define __ELASTOS_DROID_WIDGET_ABSSEEKBAR_H__

#include "widget/ProgressBar.h"
#include "R.h"

namespace Elastos {
namespace Droid {
namespace Widget {

using Elastos::Droid::R;
//using Elastos::Droid::View::IMotionEvent;
using Elastos::Droid::View::IKeyEvent;
using Elastos::Droid::View::Accessibility::IAccessibilityNodeInfo;

class AbsSeekBar : public ProgressBar
{
public:
    AbsSeekBar();

    AbsSeekBar(
        /* [in] */ IContext* context);

    AbsSeekBar(
        /* [in] */ IContext* context,
        /* [in] */ IAttributeSet* attrs);

    AbsSeekBar(
        /* [in] */ IContext* context,
        /* [in] */ IAttributeSet* attrs,
        /* [in] */ Int32 defStyle);

    /**
     * Sets the thumb that will be drawn at the end of the progress meter within the SeekBar.
     * <p>
     * If the thumb is a valid drawable (i.e. not null), half its width will be
     * used as the new thumb offset (@see #setThumbOffset(Int32)).
     *
     * @param thumb Drawable representing the thumb
     */
    virtual CARAPI SetThumb(
        /* [in] */ IDrawable* thumb);

    AutoPtr<IDrawable> GetThumb();

    /**
     * @see #setThumbOffset(Int32)
     */
    virtual CARAPI_(Int32) GetThumbOffset();

    /**
     * Sets the thumb offset that allows the thumb to extend out of the range of
     * the track.
     *
     * @param thumbOffset The offset amount in pixels.
     */
    virtual CARAPI SetThumbOffset(
        /* [in] */ Int32 thumbOffset);

    /**
     * Sets the amount of progress changed via the arrow keys.
     *
     * @param increment The amount to increment or decrement when the user
     *            presses the arrow keys.
     */
    virtual CARAPI SetKeyProgressIncrement(
        /* [in] */ Int32 increment);

    /**
     * Returns the amount of progress changed via the arrow keys.
     * <p>
     * By default, this will be a value that is derived from the max progress.
     *
     * @return The amount to increment or decrement when the user presses the
     *         arrow keys. This will be positive.
     */
    virtual CARAPI_(Int32) GetKeyProgressIncrement();

    //@Override
    CARAPI SetMax(
        /* [in] */ Int32 max);

    //@Override
    CARAPI_(Boolean) OnTouchEvent(
        /* [in] */ IMotionEvent* event);

    //@Override
    CARAPI_(Boolean) OnKeyDown(
        /* [in] */ Int32 keyCode,
        /* [in] */ IKeyEvent* event);

    virtual CARAPI JumpDrawablesToCurrentState();

    virtual CARAPI OnResolveDrawables(
        /* [in] */ Int32 layoutDirection);

    virtual CARAPI OnInitializeAccessibilityEvent(
        /* [in] */ IAccessibilityEvent* event);

    //@Override
    virtual CARAPI OnInitializeAccessibilityNodeInfo(
        /* [in] */ IAccessibilityNodeInfo* info);

    //@Override
    virtual CARAPI_(Boolean) PerformAccessibilityAction(
        /* [in] */ Int32 action,
        /* [in] */ IBundle* arguments);

    virtual CARAPI OnRtlPropertiesChanged(
        /* [in] */ Int32 layoutDirection);

protected:
    CARAPI Init(
        /* [in] */ IContext* context);

    CARAPI Init(
        /* [in] */ IContext* context,
        /* [in] */ IAttributeSet* attrs);

    CARAPI Init(
        /* [in] */ IContext* context,
        /* [in] */ IAttributeSet* attrs,
        /* [in] */ Int32 defStyle);

    //@Override
    CARAPI_(Boolean) VerifyDrawable(
        /* [in] */ IDrawable* who);

    //@Override
    CARAPI DrawableStateChanged();

    //@Override
    CARAPI_(void) OnProgressRefresh(
        /* [in] */ Float scale,
        /* [in] */ Boolean fromUser);

    //@Overide
    CARAPI_(void) OnSizeChanged(
        /* [in] */ Int32 w,
        /* [in] */ Int32 h,
        /* [in] */ Int32 oldw,
        /* [in] */ Int32 oldh);

    //@Override
    CARAPI_(void) OnDraw(
        /* [in] */ ICanvas* canvas);

    //@Override
    CARAPI_(void) OnMeasure(
        /* [in] */ Int32 widthMeasureSpec,
        /* [in] */ Int32 heightMeasureSpec);

    //java package access permission
    /**
     * This is called when the user has started touching this widget.
     */
    virtual CARAPI_(void) OnStartTrackingTouch();

    /**
     * This is called when the user either releases his touch or the touch is
     * canceled.
     */
    virtual CARAPI_(void) OnStopTrackingTouch();

    /**
     * Called when the user changes the seekbar's progress by using a key event.
     */
    virtual CARAPI_(void) OnKeyChange();

    virtual CARAPI_(Object*) GetSelfLock() = 0;

private:
    /**
     * @param gap If set to {@link Integer#MIN_VALUE}, this will be ignored and
     */
    CARAPI_(void) SetThumbPos(
        /* [in] */ Int32 w,
        /* [in] */ IDrawable* thumb,
        /* [in] */ Float scale,
        /* [in] */ Int32 gap);

    CARAPI_(void) TrackTouchEvent(
        /* [in] */ IMotionEvent* event);

    /**
     * Tries to claim the user's drag motion, and requests disallowing any
     * ancestors from stealing events in the drag.
     */
    CARAPI_(void) AttemptClaimDrag();

    CARAPI InitFromAttributes(
        /* [in] */ IContext* context,
        /* [in] */ IAttributeSet* attrs,
        /* [in] */ Int32 defStyle);

    CARAPI_(void) UpdateThumbPos(
        /* [in] */ Int32 w,
        /* [in] */ Int32 h);

private:
    AutoPtr<IDrawable> mThumb;
    Int32 mThumbOffset;

    /**
     * On key presses (right or left), the amount to increment/decrement the
     * progress.
     */
    Int32 mKeyProgressIncrement;

    static const Int32 NO_ALPHA = 0xFF;
    Float mDisabledAlpha;

    Int32 mScaledTouchSlop;
    Float mTouchDownX;
    Boolean mIsDragging;
protected:
    /**
     * On touch, this offset plus the scaled value from the position of the
     * touch will form the progress value. Usually 0.
     */
    Float mTouchProgressOffset;

    /**
     * Whether this is user seekable.
     */
    Boolean mIsUserSeekable;
};

}// namespace Elastos
}// namespace Droid
}// namespace Widget

#endif
