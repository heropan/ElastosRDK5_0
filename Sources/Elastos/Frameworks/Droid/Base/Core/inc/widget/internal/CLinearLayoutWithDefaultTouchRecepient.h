#ifndef __CLINEARLAYOUTWITHDEFAULTTOUCHRECEPIENT_H__
#define __CLINEARLAYOUTWITHDEFAULTTOUCHRECEPIENT_H__

#include "_CLinearLayoutWithDefaultTouchRecepient.h"
#include "internal/LinearLayoutWithDefaultTouchRecepient.h"
#include "view/ViewMacro.h"
#include "widget/LinearLayoutMacro.h"

namespace Elastos {
namespace Droid {
namespace Widget {
namespace Internal {

CarClass(CLinearLayoutWithDefaultTouchRecepient) , public LinearLayoutWithDefaultTouchRecepient
{
public:

    ILINEARLAYOUT_METHODS_DECL()

    IVIEW_METHODS_DECL()

    IVIEWGROUP_METHODS_DECL()

    IVIEWPARENT_METHODS_DECL()

    IVIEWMANAGER_METHODS_DECL()

    IDRAWABLECALLBACK_METHODS_DECL()

    IKEYEVENTCALLBACK_METHODS_DECL()

    IACCESSIBILITYEVENTSOURCE_METHODS_DECL()

    CARAPI_(PInterface) Probe(
        /* [in] */ REIID riid);

    CARAPI constructor(
        /* [in] */ IContext* ctx);

    CARAPI constructor(
        /* [in] */ IContext* ctx,
        /* [in] */ IAttributeSet* attrs);

    CARAPI SetDefaultTouchRecepient(
        /* [in] */ IView* defaultTouchRecepient);
};

}// namespace Internal
}// namespace Widget
}// namespace Droid
}// namespace Elastos

#endif
