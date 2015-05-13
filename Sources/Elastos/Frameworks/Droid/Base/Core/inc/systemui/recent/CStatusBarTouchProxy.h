
#ifndef __CSTATISBARTOUCHPROXY_H_
#define __CSTATISBARTOUCHPROXY_H_

#include "_CStatusBarTouchProxy.h"
#include "view/ViewMacro.h"
#include "view/ViewGroupLayoutParamsMacro.h"
#include "widget/FrameLayoutMacro.h"
#include "systemui/recent/StatusBarTouchProxy.h"

namespace Elastos {
namespace Droid {
namespace SystemUI {
namespace Recent {

CarClass(CStatusBarTouchProxy), public StatusBarTouchProxy
{
public:
    IVIEW_METHODS_DECL()
    IVIEWGROUP_METHODS_DECL()
    IVIEWPARENT_METHODS_DECL()
    IVIEWMANAGER_METHODS_DECL()
    IDRAWABLECALLBACK_METHODS_DECL()
    IKEYEVENTCALLBACK_METHODS_DECL()
    IACCESSIBILITYEVENTSOURCE_METHODS_DECL()
    IFRAMELAYOUT_METHODS_DECL()

    CARAPI constructor(
        /* [in] */ IContext* context,
        /* [in] */ IAttributeSet* attrs);

    CARAPI_(PInterface) Probe(
        /* [in] */ REIID riid);

    CARAPI SetStatusBar(
        /* [in] */ IView* statusBar);
};

}// namespace Recent
}// namespace SystemUI
}// namespace Droid
}// namespace Elastos

#endif //__CSTATISBARTOUCHPROXY_H_
