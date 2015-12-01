
#ifndef __ELASTOS_DROID_WIDGET_COMPOUNDBUTTON_H__
#define __ELASTOS_DROID_WIDGET_COMPOUNDBUTTON_H__

#include "elastos/droid/widget/Button.h"
#include "elastos/droid/view/View.h"

using Elastos::Droid::Content::Res::IColorStateList;
using Elastos::Droid::View::View;

namespace Elastos {
namespace Droid {
namespace Widget {

/**
 * <p>
 * A button with two states, checked and unchecked. When the button is pressed
 * or clicked, the state changes automatically.
 * </p>
 *
 * <p><strong>XML attributes</strong></p>
 * <p>
 * See {@link android.R.styleable#CompoundButton
 * CompoundButton Attributes}, {@link android.R.styleable#Button Button
 * Attributes}, {@link android.R.styleable#TextView TextView Attributes}, {@link
 * android.R.styleable#View View Attributes}
 * </p>
 */

class CompoundButton
    : public Button
    , public ICompoundButton
    , public ICheckable
{
protected:
    class CompoundButtonSavedState
        : public View::BaseSavedState
    {
    public:
        CompoundButtonSavedState();

        CARAPI constructor();

        CARAPI constructor(
            /* [in] */ IParcelable* superState);

        CARAPI WriteToParcel(
            /* [in] */ IParcel* dest);

        CARAPI ReadFromParcel(
            /* [in] */ IParcel* source);

    public:
        Boolean mChecked;
    };

public:
    CAR_INTERFACE_DECL()

    CompoundButton();

    CARAPI constructor(
        /* [in] */ IContext* context,
        /* [in] */ IAttributeSet* attrs = NULL,
        /* [in] */ Int32 defStyleAttr = 0,
        /* [in] */ Int32 defStyleRes = 0);

    virtual CARAPI Toggle();

    //@Override
    CARAPI_(Boolean) PerformClick();

    virtual CARAPI_(Boolean) IsChecked();

    /**
     * <p>Changes the checked state of this button.</p>
     *
     * @param checked TRUE to check the button, FALSE to uncheck it
     */
    virtual CARAPI SetChecked(
        /* [in] */ Boolean checked);

    /**
     * Register a callback to be invoked when the checked state of this button
     * changes.
     *
     * @param listener the callback to call on checked state change
     */
    virtual CARAPI SetOnCheckedChangeListener(
        /* [in] */ ICompoundButtonOnCheckedChangeListener* listener);

    /**
     * Register a callback to be invoked when the checked state of this button
     * changes. This callback is used for internal purpose only.
     *
     * @param listener the callback to call on checked state change
     * @hide
     */
    virtual CARAPI SetOnCheckedChangeWidgetListener(
        /* [in] */ ICompoundButtonOnCheckedChangeListener* listener);

    /**
     * Set the background to a given Drawable, identified by its resource id.
     *
     * @param resid the resource id of the drawable to use as the background
     */
    virtual CARAPI SetButtonDrawable(
        /* [in] */ Int32 resid);

    /**
     * Set the background to a given Drawable
     *
     * @param d The Drawable to use as the background
     */
    virtual CARAPI SetButtonDrawable(
        /* [in] */ IDrawable* d);

    //@Override
    virtual CARAPI_(AutoPtr<IParcelable>) OnSaveInstanceState();

    //@Override
    virtual CARAPI_(void) OnRestoreInstanceState(
        /* [in] */ IParcelable* state);

    virtual CARAPI OnInitializeAccessibilityEvent(
        /* [in] */ IAccessibilityEvent* event);

    virtual CARAPI OnInitializeAccessibilityNodeInfo(
        /* [in] */ IAccessibilityNodeInfo* info);

    virtual CARAPI_(Int32) GetCompoundPaddingLeft();

    virtual CARAPI_(Int32) GetCompoundPaddingRight();

    virtual CARAPI JumpDrawablesToCurrentState();

    virtual CARAPI_(Int32) GetHorizontalOffsetForDrawables();

    CARAPI SetButtonTintList(
        /* [in] */ IColorStateList* tint);

    CARAPI GetButtonTintList(
        /* [out] */ IColorStateList** tint);

    CARAPI SetButtonTintMode(
        /* [in] */ Elastos::Droid::Graphics::PorterDuffMode tintMode);

    CARAPI GetButtonTintMode(
        /* [out] */ Elastos::Droid::Graphics::PorterDuffMode* tintMode);

    CARAPI DrawableHotspotChanged(
        /* [in] */ Float x,
        /* [in] */ Float y);

protected:
    //@Override
    virtual CARAPI_(void) OnDraw(
        /* [in] */ ICanvas* canvas);

    //@Override
    virtual CARAPI OnCreateDrawableState(
        /* [in] */ Int32 extraSpace,
        /* [out] */ ArrayOf<Int32>** drawableState);

    //@Override
    virtual CARAPI DrawableStateChanged();

    //@Override
    virtual CARAPI_(Boolean) VerifyDrawable(
        /* [in] */ IDrawable* who);

private:
    CARAPI InitFromAttributes(
        /* [in] */ IContext* context,
        /* [in] */ IAttributeSet* attrs = NULL,
        /* [in] */ Int32 defStyleAttr = 0,
        /* [in] */ Int32 defStyleRes = 0);

    CARAPI ApplyButtonTint();

private:
    Boolean mChecked;
    Int32 mButtonResource;
    Boolean mBroadcasting;
    AutoPtr<IDrawable> mButtonDrawable;
    AutoPtr<IColorStateList> mButtonTintList;
    Elastos::Droid::Graphics::PorterDuffMode mButtonTintMode;
    Boolean mHasButtonTint;
    Boolean mHasButtonTintMode;

    AutoPtr<ICompoundButtonOnCheckedChangeListener> mOnCheckedChangeListener;
    AutoPtr<ICompoundButtonOnCheckedChangeListener> mOnCheckedChangeWidgetListener;

    static AutoPtr<ArrayOf<Int32> > CHECKED_STATE_SET;
};

}// namespace Widget
}// namespace Droid
}// namespace Elastos

#endif //__ELASTOS_DROID_WIDGET_COMPOUNDBUTTON_H__
