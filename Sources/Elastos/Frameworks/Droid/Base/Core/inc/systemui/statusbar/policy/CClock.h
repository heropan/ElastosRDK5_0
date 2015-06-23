
#ifndef __POLICY_CCLOCK_H__
#define __POLICY_CCLOCK_H__

#include "_CClock.h"


#include "systemui/statusbar/policy/Clock.h"

namespace Elastos {
namespace Droid {
namespace SystemUI {
namespace StatusBar {
namespace Policy {

CarClass(CClock), public Clock
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
};

}// namespace Policy
}// namespace StatusBar
}// namespace SystemUI
}// namespace Droid
}// namespace Elastos

#endif // __POLICY_CCLOCK_H__
