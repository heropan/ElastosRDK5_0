
#ifndef __CBUTTON_H__
#define __CBUTTON_H__

#include "_CButton.h"
#include "widget/Button.h"



namespace Elastos{
namespace Droid{
namespace Widget{

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
    // TODO: Add your private member variables here.
};

}// namespace Widget
}// namespace Droid
}// namespace Elastos

#endif // __CBUTTON_H__
