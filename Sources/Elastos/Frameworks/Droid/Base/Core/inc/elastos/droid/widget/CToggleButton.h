#ifndef __ELASTOS_DROID_WIDGET_CCOMPOUNDBUTTON_H__
#define __ELASTOS_DROID_WIDGET_CCOMPOUNDBUTTON_H__

#include "_Elastos_Droid_Widget_CToggleButton.h"
#include "elastos/droid/widget/ToggleButton.h"




namespace Elastos {
namespace Droid {
namespace Widget {

CarClass(CToggleButton) , public ToggleButton
{
public:
    IVIEW_METHODS_DECL()
    ITEXTVIEW_METHODS_DECL()
    IDRAWABLECALLBACK_METHODS_DECL()
    IKEYEVENTCALLBACK_METHODS_DECL()
    IACCESSIBILITYEVENTSOURCE_METHODS_DECL()
    ICHECKABLE_METHODS_DECL()
    ICOMPOUNDBUTTON_METHODS_DECL()

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

    /**
     * Returns the text for when the button is in the checked state.
     *
     * @return The text.
     */
    CARAPI GetTextOn(
        /* [out] */ ICharSequence** on);

    /**
     * Sets the text for when the button is in the checked state.
     *
     * @param textOn The text.
     */
    CARAPI SetTextOn(
        /* [in] */ ICharSequence* textOn);

    /**
     * Returns the text for when the button is not in the checked state.
     *
     * @return The text.
     */
    CARAPI GetTextOff(
        /* [out] */ ICharSequence** on);

    /**
     * Sets the text for when the button is not in the checked state.
     *
     * @param textOff The text.
     */
    CARAPI SetTextOff(
        /* [in] */ ICharSequence* textOff);

    CARAPI GetButtonDrawable(
        /* [out] */ IDrawable** d);
};

}// namespace Widget
}// namespace Droid
}// namespace Elastos
#endif
