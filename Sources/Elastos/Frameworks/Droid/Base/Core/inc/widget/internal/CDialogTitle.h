#ifndef __ELASTOS_DROID_WIDGET_INTERNAL_CDIALOGTITLE_H__
#define __ELASTOS_DROID_WIDGET_INTERNAL_CDIALOGTITLE_H__

#include "_Elastos_Droid_Widget_Internal_CDialogTitle.h"


#include "widget/internal/DialogTitle.h"

namespace Elastos {
namespace Droid {
namespace Widget {
namespace Internal {

CarClass(CDialogTitle), public DialogTitle
{
public:
    IVIEW_METHODS_DECL()
    ITEXTVIEW_METHODS_DECL();
    IDRAWABLECALLBACK_METHODS_DECL()
    IKEYEVENTCALLBACK_METHODS_DECL()
    IACCESSIBILITYEVENTSOURCE_METHODS_DECL()

    CARAPI_(PInterface) Probe(
        /* [in] */ REIID riid);

    CARAPI constructor(
        /* [in] */ IContext* context);

    CARAPI constructor(
        /* [in] */ IContext* context,
        /* [in] */ IAttributeSet* attrs);

    CARAPI constructor(
        /* [in] */ IContext* context,
        /* [in] */ IAttributeSet* attrs,
        /* [in] */ Int32 defStyle);

    CARAPI OnPreDraw(
        /* [out] */ Boolean* result);
};

}// namespace Internal
}// namespace Widget
}// namespace Droid
}// namespace Elastos

#endif //__ELASTOS_DROID_WIDGET_INTERNAL_CDIALOGTITLE_H__
