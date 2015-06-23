
#ifndef __CCHECKBOX_H__
#define __CCHECKBOX_H__

#include "_CCheckBox.h"
#include "widget/CheckBox.h"




namespace Elastos{
namespace Droid{
namespace Widget{

CarClass(CCheckBox), public CheckBox
{
public:

    IVIEW_METHODS_DECL()
    IDRAWABLECALLBACK_METHODS_DECL()
    IKEYEVENTCALLBACK_METHODS_DECL()
    IACCESSIBILITYEVENTSOURCE_METHODS_DECL()
    ITEXTVIEW_METHODS_DECL()
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

    CARAPI OnPreDraw(
        /* [out] */ Boolean* result);

    CARAPI GetButtonDrawable(
        /* [out] */ IDrawable** d);

private:
    // TODO: Add your private member variables here.
};

}// namespace Widget
}// namespace Droid
}// namespace Elastos

#endif // __CCHECKBOX_H__
