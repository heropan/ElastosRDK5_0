#ifndef __ELASTOS_DROID_WIDGET_CCOMPOUNDBUTTON_H__
#define __ELASTOS_DROID_WIDGET_CCOMPOUNDBUTTON_H__

#include "_Elastos_Droid_Widget_CCompoundButton.h"
#include "elastos/droid/widget/CompoundButton.h"



namespace Elastos {
namespace Droid {
namespace Widget {

CarClass(CCompoundButton) , public CompoundButton
{
public:

    IVIEW_METHODS_DECL()

    ITEXTVIEW_METHODS_DECL()

    IDRAWABLECALLBACK_METHODS_DECL()

    IKEYEVENTCALLBACK_METHODS_DECL()

    IACCESSIBILITYEVENTSOURCE_METHODS_DECL()

    CARAPI_(PInterface) Probe(
        /* [in] */ REIID riid);

    CARAPI constructor(
        /* [in] */ IContext * pCtx);

    CARAPI constructor(
        /* [in] */ IContext * pCtx,
        /* [in] */ IAttributeSet * pAttrs);

    CARAPI constructor(
        /* [in] */ IContext * pContext,
        /* [in] */ IAttributeSet * pAttrs,
        /* [in] */ Int32 defStyle);
    /**
     * Change the checked state of the view
     *
     * @param checked The new checked state
     */
    CARAPI SetChecked(
        /* [in] */ Boolean checked);

    /**
     * @return The current checked state of the view
     */
    CARAPI IsChecked(
        /* [out] */ Boolean* isChecked);

    /**
     * Change the checked state of the view to the inverse of its current state
     *
     */
    CARAPI Toggle();

    /**
         * Register a callback to be invoked when the checked state of this button
         * changes.
         *
         * @param listener the callback to call on checked state change
         */
    CARAPI SetOnCheckedChangeListener(
        /* [in] */ ICompoundButtonOnCheckedChangeListener* listener);

    /**
         * Register a callback to be invoked when the checked state of this button
         * changes. This callback is used for internal purpose only.
         *
         * @param listener the callback to call on checked state change
         * @hide
         */
    CARAPI SetOnCheckedChangeWidgetListener(
        /* [in] */ ICompoundButtonOnCheckedChangeListener* listener);

    /**
         * Set the background to a given Drawable, identified by its resource id.
         *
         * @param resid the resource id of the drawable to use as the background
         */
    CARAPI SetButtonDrawable(
        /* [in] */ Int32 resid);

    /**
         * Set the background to a given Drawable
         *
         * @param d The Drawable to use as the background
         */
    CARAPI SetButtonDrawable(
        /* [in] */ IDrawable* d);

    CARAPI GetButtonDrawable(
        /* [out] */ IDrawable** d);
    /**
     * Callback method to be invoked when the view tree is about to be drawn. At this point, all
     * views in the tree have been measured and given a frame. Clients can use this to adjust
     * their scroll bounds or even to request a new layout before drawing occurs.
     *
     * @return Return true to proceed with the current drawing pass, or false to cancel.
     *
     * @see android.view.View#onMeasure
     * @see android.view.View#onLayout
     * @see android.view.View#onDraw
     */
    CARAPI OnPreDraw(
        /* [out] */ Boolean* result);
};

}// namespace Widget
}// namespace Droid
}// namespace Elastos
#endif
