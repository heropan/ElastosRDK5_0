#ifndef __ELASTOS_DROID_WIDGET_CCOMPOUNDBUTTON_H__
#define __ELASTOS_DROID_WIDGET_CCOMPOUNDBUTTON_H__

#include "_CRadioButton.h"
#include "widget/RadioButton.h"




namespace Elastos {
namespace Droid {
namespace Widget {

CarClass(CRadioButton) , public RadioButton
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


    CARAPI GetButtonDrawable(
        /* [out] */ IDrawable** d);
};

}// namespace Widget
}// namespace Droid
}// namespace Elastos

#endif
