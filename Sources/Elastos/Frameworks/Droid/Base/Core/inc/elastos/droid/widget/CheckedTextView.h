
#ifndef __ELASTOS_DROID_WIDGET_CHECKEDTEXTVIEW_H__
#define __ELASTOS_DROID_WIDGET_CHECKEDTEXTVIEW_H__

#include "elastos/droid/widget/TextView.h"
#include "elastos/droid/R.h"

using Elastos::Droid::R;

namespace Elastos {
namespace Droid {
namespace Widget {

/**
 * An extension to TextView that supports the {@link android.widget.Checkable} interface.
 * This is useful when used in a {@link android.widget.ListView ListView} where the it's
 * {@link android.widget.ListView#setChoiceMode(Int32) setChoiceMode} has been set to
 * something other than {@link android.widget.ListView#CHOICE_MODE_NONE CHOICE_MODE_NONE}.
 *
 */
class CheckedTextView : public TextView
{
public:
    CheckedTextView();

    CheckedTextView(
        /* [in] */ IContext* context);

    CheckedTextView(
        /* [in] */ IContext* context,
        /* [in] */ IAttributeSet* attrs);

    CheckedTextView(
        /* [in] */ IContext* context,
        /* [in] */ IAttributeSet* attrs,
        /* [in] */ Int32 defStyle);

    virtual CARAPI Toggle();

    virtual CARAPI_(Boolean) IsChecked();

    /**
     * <p>Changes the checked state of this text view.</p>
     *
     * @param checked true to check the text, false to uncheck it
     */
    virtual CARAPI SetChecked(
        /* [in] */ Boolean checked);


    /**
     * Set the checkmark to a given Drawable, identified by its resourece id. This will be drawn
     * when {@link #isChecked()} is true.
     *
     * @param resid The Drawable to use for the checkmark.
     */
    virtual CARAPI SetCheckMarkDrawable(
        /* [in] */ Int32 resid);

    /**
     * Set the checkmark to a given Drawable. This will be drawn when {@link #isChecked()} is true.
     *
     * @param d The Drawable to use for the checkmark.
     */
    virtual CARAPI SetCheckMarkDrawable(
        /* [in] */ IDrawable* d);

    virtual CARAPI_(AutoPtr<IDrawable>) GetCheckMarkDrawable();

    virtual CARAPI_(void) OnDraw(
        /* [in] */ ICanvas* canvas);

    virtual CARAPI OnCreateDrawableState(
        /* [in] */ Int32 extraSpace,
        /* [out] */ ArrayOf<Int32>** drawableState);

    virtual CARAPI DrawableStateChanged();

    virtual CARAPI OnRtlPropertiesChanged(
        /* [in] */ Int32 layoutDirection);

    virtual CARAPI OnInitializeAccessibilityEvent(
        /* [in] */ IAccessibilityEvent* event);

    virtual CARAPI OnInitializeAccessibilityNodeInfo(
        /* [in] */ IAccessibilityNodeInfo* info);

    CARAPI Init(
        /* [in] */ IContext* context);

    CARAPI Init(
        /* [in] */ IContext* context,
        /* [in] */ IAttributeSet* attrs);

    CARAPI Init(
        /* [in] */ IContext* context,
        /* [in] */ IAttributeSet* attrs,
        /* [in] */ Int32 defStyle);

protected:
    //@Override
    CARAPI_(void) InternalSetPadding(
        /* [in] */ Int32 left,
        /* [in] */ Int32 top,
        /* [in] */ Int32 right,
        /* [in] */ Int32 bottom);

private:
    CARAPI_(void) UpdatePadding();

    CARAPI_(void) SetBasePadding(
        /* [in] */ Boolean isLayoutRtl);

private:
    Boolean mChecked;
    Int32 mCheckMarkResource;
    AutoPtr<IDrawable> mCheckMarkDrawable;
    Int32 mBasePadding;
    Int32 mCheckMarkWidth;
    Boolean mNeedRequestlayout;

    static AutoPtr<ArrayOf<Int32> > CHECKED_STATE_SET;
};

}// namespace Widget
}// namespace Droid
}// namespace Elastos
#endif
