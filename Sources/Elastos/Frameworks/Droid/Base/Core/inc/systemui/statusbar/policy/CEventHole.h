
#ifndef __CEVENTHOLE_H_
#define __CEVENTHOLE_H_

#include "_CEventHole.h"
#include "view/ViewMacro.h"
#include "systemui/statusbar/policy/EventHole.h"

namespace Elastos {
namespace Droid {
namespace SystemUI {
namespace StatusBar {
namespace Policy {

CarClass(CEventHole), public EventHole
{
public:
    IVIEW_METHODS_DECL()
    IDRAWABLECALLBACK_METHODS_DECL()
    IKEYEVENTCALLBACK_METHODS_DECL()
    IACCESSIBILITYEVENTSOURCE_METHODS_DECL()

    CARAPI_(PInterface) Probe(
        /* [in] */ REIID riid);

    CARAPI constructor(
        /* [in] */ IContext* pCtx,
        /* [in] */ IAttributeSet* pAttrs);

    CARAPI constructor(
        /* [in] */ IContext* pCtx,
        /* [in] */ IAttributeSet* pAttrs,
        /* [in] */ Int32 defStyle);

    CARAPI OnComputeInternalInsets(
        /* [in] */ IInternalInsetsInfo* inoutInfo);
};

}// namespace Tablet
}// namespace StatusBar
}// namespace SystemUI
}// namespace Droid
}// namespace Elastos

#endif // __CEVENTHOLE_H_
