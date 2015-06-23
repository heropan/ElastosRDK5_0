#ifndef __CDIALOGTITLE_H_
#define __CDIALOGTITLE_H_

#include "_CDialogTitle.h"


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

#endif //__CDIALOGTITLE_H_
