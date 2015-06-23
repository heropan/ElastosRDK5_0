#ifndef __CDEADZONE_H_
#define __CDEADZONE_H_

#include "_CDeadZone.h"

#include "systemui/statusbar/policy/DeadZone.h"

namespace Elastos {
namespace Droid {
namespace SystemUI {
namespace StatusBar {
namespace Policy {

CarClass(CDeadZone), public DeadZone
{
public:
    IVIEW_METHODS_DECL()
    IDRAWABLECALLBACK_METHODS_DECL()
    IKEYEVENTCALLBACK_METHODS_DECL()
    IACCESSIBILITYEVENTSOURCE_METHODS_DECL()

    CARAPI_(PInterface) Probe(
        /* [in] */ REIID riid);

    CARAPI constructor(
        /* [in] */ IContext* context,
        /* [in] */ IAttributeSet* attrs);

    CARAPI constructor(
        /* [in] */ IContext* context,
        /* [in] */ IAttributeSet* attrs,
        /* [in] */ Int32 defStyle);

    CARAPI SetFlashOnTouchCapture(
        /* [in] */ Boolean dbg);

    CARAPI Poke(
        /* [in] */ IMotionEvent* event);

    CARAPI SetFlash(
        /* [in] */ Float f);

    CARAPI GetFlash(
        /* [out] */ Float* f);
};

}// namespace Policy
}// namespace StatusBar
}// namespace SystemUI
}// namespace Droid
}// namespace Elastos

#endif //__CDEADZONE_H_
