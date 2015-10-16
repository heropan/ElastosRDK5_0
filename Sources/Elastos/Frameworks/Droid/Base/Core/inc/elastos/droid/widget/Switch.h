
#ifndef __ELASTOS_DROID_WIDGET_SWITCH_H__
#define __ELASTOS_DROID_WIDGET_SWITCH_H__

#include "elastos/droid/ext/frameworkext.h"
#include "elastos/droid/widget/CompoundButton.h"
#include "elastos/droid/view/VelocityTracker.h"

using Elastos::Core::ICharSequence;
using Elastos::Droid::Utility::IAttributeSet;
using Elastos::Droid::Content::IContext;
using Elastos::Droid::Graphics::Drawable::IDrawable;
using Elastos::Droid::Graphics::ITypeface;
using Elastos::Droid::View::VelocityTracker;
using Elastos::Droid::Text::Method::ITransformationMethod2;

namespace Elastos {
namespace Droid {
namespace Widget {
/**
 * A Switch is a two-state toggle switch widget that can select between two
 * options. The user may drag the "thumb" back and forth to choose the selected option,
 * or simply tap to toggle as if it were a checkbox. The {@link #setText(CharSequence) text}
 * property controls the text displayed in the label for the switch, whereas the
 * {@link #setTextOff(CharSequence) off} and {@link #setTextOn(CharSequence) on} text
 * controls the text on the thumb. Similarly, the
 * {@link #setTextAppearance(android.content.Context, Int32) textAppearance} and the related
 * setTypeface() methods control the typeface and style of label text, whereas the
 * {@link #setSwitchTextAppearance(android.content.Context, Int32) switchTextAppearance} and
 * the related seSwitchTypeface() methods control that of the thumb.
 *
 * <p>See the <a href="{@docRoot}guide/topics/ui/controls/togglebutton.html">Toggle Buttons</a>
 * guide.</p>
 *
 * @attr ref android.R.styleable#Switch_textOn
 * @attr ref android.R.styleable#Switch_textOff
 * @attr ref android.R.styleable#Switch_switchMinWidth
 * @attr ref android.R.styleable#Switch_switchPadding
 * @attr ref android.R.styleable#Switch_switchTextAppearance
 * @attr ref android.R.styleable#Switch_thumb
 * @attr ref android.R.styleable#Switch_thumbTextPadding
 * @attr ref android.R.styleable#Switch_track
 */
class Switch : public CompoundButton
{
public:
    /**
     * Construct a new Switch with a default style determined by the given theme attribute,
     * overriding specific style attributes as requested.
     *
     * @param context The Context that will determine this widget's theming.
     * @param attrs Specification of attributes that should deviate from the default styling.
     * @param defStyle An attribute ID within the active theme containing a reference to the
     *                 default style for this widget. e.g. android.R.attr.switchStyle.
     */
    Switch(
        /* [in] */ IContext* context,
        /* [in] */ IAttributeSet* attrs = NULL,
        /* [in] */ Int32 defStyle = R::attr::switchStyle);

    /**
     * Sets the switch text color, size, style, hint color, and highlight color
     * from the specified TextAppearance resource.
     *
     * @attr ref android.R.styleable#Switch_switchTextAppearance
     */
    CARAPI SetSwitchTextAppearance(
        /* [in] */ IContext* context,
        /* [in] */ Int32 resid);

    /**
     * Sets the typeface and style in which the text should be displayed on the
     * switch, and turns on the fake bold and italic bits in the Paint if the
     * Typeface that you provided does not have all the bits in the
     * style that you specified.
     */
    CARAPI SetSwitchTypeface(
        /* [in] */ ITypeface* tf,
        /* [in] */ Int32 style);

    /**
     * Sets the typeface in which the text should be displayed on the switch.
     * Note that not all Typeface families actually have bold and italic
     * variants, so you may need to use
     * {@link #setSwitchTypeface(Typeface, int)} to get the appearance
     * that you actually want.
     *
     * @attr ref android.R.styleable#TextView_typeface
     * @attr ref android.R.styleable#TextView_textStyle
     */
    CARAPI SetSwitchTypeface(
        /* [in] */ ITypeface* tf);

    /**
     * Set the amount of horizontal padding between the switch and the associated text.
     *
     * @param pixels Amount of padding in pixels
     *
     * @attr ref android.R.styleable#Switch_switchPadding
     */
    CARAPI SetSwitchPadding(
        /* [in] */ Int32 pixels);

    /**
     * Get the amount of horizontal padding between the switch and the associated text.
     *
     * @return Amount of padding in pixels
     *
     * @attr ref android.R.styleable#Switch_switchPadding
     */
    CARAPI_(Int32) GetSwitchPadding();

    /**
     * Set the minimum width of the switch in pixels. The switch's width will be the maximum
     * of this value and its measured width as determined by the switch drawables and text used.
     *
     * @param pixels Minimum width of the switch in pixels
     *
     * @attr ref android.R.styleable#Switch_switchMinWidth
     */
    CARAPI SetSwitchMinWidth(
        /* [in] */ Int32 pixels);

    /**
     * Get the minimum width of the switch in pixels. The switch's width will be the maximum
     * of this value and its measured width as determined by the switch drawables and text used.
     *
     * @return Minimum width of the switch in pixels
     *
     * @attr ref android.R.styleable#Switch_switchMinWidth
     */
    CARAPI_(Int32) GetSwitchMinWidth();

    /**
     * Set the horizontal padding around the text drawn on the switch itself.
     *
     * @param pixels Horizontal padding for switch thumb text in pixels
     *
     * @attr ref android.R.styleable#Switch_thumbTextPadding
     */
    CARAPI SetThumbTextPadding(
        /* [in] */ Int32 pixels);

    /**
     * Get the horizontal padding around the text drawn on the switch itself.
     *
     * @return Horizontal padding for switch thumb text in pixels
     *
     * @attr ref android.R.styleable#Switch_thumbTextPadding
     */
    CARAPI_(Int32) GetThumbTextPadding();

    /**
     * Set the drawable used for the track that the switch slides within.
     *
     * @param track Track drawable
     *
     * @attr ref android.R.styleable#Switch_track
     */
    CARAPI SetTrackDrawable(
        /* [in] */ IDrawable* track);

    /**
     * Set the drawable used for the track that the switch slides within.
     *
     * @param resId Resource ID of a track drawable
     *
     * @attr ref android.R.styleable#Switch_track
     */
    CARAPI SetTrackResource(
        /* [in] */ Int32 resId);

    /**
     * Get the drawable used for the track that the switch slides within.
     *
     * @return Track drawable
     *
     * @attr ref android.R.styleable#Switch_track
     */
    CARAPI_(AutoPtr<IDrawable>) GetTrackDrawable();

    /**
     * Set the drawable used for the switch "thumb" - the piece that the user
     * can physically touch and drag along the track.
     *
     * @param thumb Thumb drawable
     *
     * @attr ref android.R.styleable#Switch_thumb
     */
    CARAPI SetThumbDrawable(
        /* [in] */ IDrawable* thumb);

    /**
     * Set the drawable used for the switch "thumb" - the piece that the user
     * can physically touch and drag along the track.
     *
     * @param resId Resource ID of a thumb drawable
     *
     * @attr ref android.R.styleable#Switch_thumb
     */
    CARAPI SetThumbResource(
        /* [in] */ Int32 resId);

    /**
      * Get the drawable used for the switch "thumb" - the piece that the user
      * can physically touch and drag along the track.
      *
      * @return Thumb drawable
      *
      * @attr ref android.R.styleable#Switch_thumb
      */
    CARAPI_(AutoPtr<IDrawable>) GetThumbDrawable();

    /**
     * Returns the text displayed when the button is in the checked state.
     *
     * @attr ref android.R.styleable#Switch_textOn
     */
    CARAPI_(AutoPtr<ICharSequence>) GetTextOn();

    /**
     * Sets the text displayed when the button is in the checked state.
     *
     * @attr ref android.R.styleable#Switch_textOn
     */
    CARAPI SetTextOn(
        /* [in] */ ICharSequence* textOn);

    /**
     * Returns the text displayed when the button is not in the checked state.
     *
     * @attr ref android.R.styleable#Switch_textOff
     */
    CARAPI_(AutoPtr<ICharSequence>) GetTextOff();

    /**
     * Sets the text displayed when the button is not in the checked state.
     *
     * @attr ref android.R.styleable#Switch_textOff
     */
    CARAPI SetTextOff(
        /* [in] */ ICharSequence* textOff);

    ///////////////////////////////////////////////////////////////////////////
    //    @Override
    CARAPI_(void) OnMeasure(
        /* [in] */ Int32 widthMeasureSpec,
        /* [in] */ Int32 heightMeasureSpec);

    CARAPI OnPopulateAccessibilityEvent(
        /* [in] */ IAccessibilityEvent* event);

    CARAPI_(Boolean) OnTouchEvent(
        /* [in] */ IMotionEvent* ev);

    CARAPI SetChecked(
        /* [in] */ Boolean checked);

    CARAPI_(void) OnLayout(
        /* [in] */ Boolean changed,
        /* [in] */ Int32 left,
        /* [in] */ Int32 top,
        /* [in] */ Int32 right,
        /* [in] */ Int32 bottom);

    CARAPI_(void) OnDraw(
        /* [in] */ ICanvas* canvas);

    CARAPI_(Int32) GetCompoundPaddingLeft();

    CARAPI_(Int32) GetCompoundPaddingRight();

    CARAPI OnCreateDrawableState(
        /* [in] */ Int32 extraSpace,
        /* [out, callee] */ ArrayOf<Int32>** drawableState);

    CARAPI DrawableStateChanged();

    CARAPI_(Boolean) VerifyDrawable(
        /* [in] */ IDrawable* who);

    CARAPI JumpDrawablesToCurrentState();

    CARAPI OnInitializeAccessibilityEvent(
        /* [in] */ IAccessibilityEvent* event);

    CARAPI OnInitializeAccessibilityNodeInfo(
        /* [in] */ IAccessibilityNodeInfo* info);

protected:
    Switch();

    CARAPI Init(
        /* [in] */ IContext* context,
        /* [in] */ IAttributeSet* attrs = NULL,
        /* [in] */ Int32 defStyle = R::attr::switchStyle);

    CARAPI InitImpl(
        /* [in] */ IContext* context,
        /* [in] */ IAttributeSet* attrs = NULL,
        /* [in] */ Int32 defStyle = R::attr::switchStyle);

private:
    void SetSwitchTypefaceByIndex(
        /* [in] */ Int32 typefaceIndex,
        /* [in] */ Int32 styleIndex);

    AutoPtr<ILayout> MakeLayout(
        /* [in] */ ICharSequence* text);

    /**
     * @return true if (x, y) is within the target area of the switch thumb
     */
    Boolean HitThumb(
        /* [in] */ Float x,
        /* [in] */ Float y);

    void CancelSuperTouch(
        /* [in] */ IMotionEvent* ev);

    /**
     * Called from onTouchEvent to end a drag operation.
     *
     * @param ev Event that triggered the end of drag mode - ACTION_UP or ACTION_CANCEL
     */
    void StopDrag(
        /* [in] */ IMotionEvent* ev);

    void AnimateThumbToCheckedState(
        /* [in] */ Boolean newCheckedState);

    Boolean GetTargetCheckedState();

    void SetThumbPosition(
        /* [in] */ Boolean checked);

    Int32 GetThumbScrollRange();

private:
    static const Int32 TOUCH_MODE_IDLE = 0;
    static const Int32 TOUCH_MODE_DOWN = 1;
    static const Int32 TOUCH_MODE_DRAGGING = 2;

    // Enum for the "typeface" XML parameter.
    static const Int32 SANS = 1;
    static const Int32 SERIF = 2;
    static const Int32 MONOSPACE = 3;

    AutoPtr<IDrawable> mThumbDrawable;
    AutoPtr<IDrawable> mTrackDrawable;
    Int32 mThumbTextPadding;
    Int32 mSwitchMinWidth;
    Int32 mSwitchPadding;
    AutoPtr<ICharSequence> mTextOn;
    AutoPtr<ICharSequence> mTextOff;

    Int32 mTouchMode;
    Int32 mTouchSlop;
    Float mTouchX;
    Float mTouchY;
    AutoPtr<VelocityTracker> mVelocityTracker;// = VelocityTracker.obtain();
    Int32 mMinFlingVelocity;

    Float mThumbPosition;
    Int32 mSwitchWidth;
    Int32 mSwitchHeight;
    Int32 mThumbWidth; // Does not include padding

    Int32 mSwitchLeft;
    Int32 mSwitchTop;
    Int32 mSwitchRight;
    Int32 mSwitchBottom;

    AutoPtr<ITextPaint> mTextPaint;
    AutoPtr<IColorStateList> mTextColors;
    AutoPtr<ILayout> mOnLayout;
    AutoPtr<ILayout> mOffLayout;
    AutoPtr<ITransformationMethod2> mSwitchTransformationMethod;

    AutoPtr<IRect> mTempRect;// = new Rect();

    static const AutoPtr<ArrayOf<Int32> > CHECKED_STATE_SET;
    const static String SWITCH_NAME;
};

} // namespace Widget
} // namespace Droid
} // namespace Elastos

#endif //__ELASTOS_DROID_WIDGET_SWITCH_H__
