
#ifndef __ELASTOS_DROID_WIDGET_CBUTTON_H__
#define __ELASTOS_DROID_WIDGET_CBUTTON_H__

#include "_Elastos_Droid_Widget_CButton.h"
#include "elastos/droid/widget/Button.h"



namespace Elastos {
namespace Droid {
namespace Widget {

CarClass(CButton), public Button
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

    CARAPI OnPreDraw(
        /* [out] */ Boolean* result);


private:
};

}// namespace Widget
}// namespace Droid
}// namespace Elastos

#endif // __ELASTOS_DROID_WIDGET_CBUTTON_H__
