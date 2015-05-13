#ifndef __CACTIONBAROVERLAYLAYOUT_H_
#define __CACTIONBAROVERLAYLAYOUT_H_

#include "_CActionBarOverlayLayout.h"
#include "view/ViewMacro.h"
#include "view/ViewGroupLayoutParamsMacro.h"
#include "widget/internal/ActionBarOverlayLayout.h"
#include "widget/FrameLayoutMacro.h"

namespace Elastos {
namespace Droid {
namespace Widget {
namespace Internal {

CarClass(CActionBarOverlayLayout), public ActionBarOverlayLayout
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

    CARAPI_(PInterface) Probe(
        /* [in] */ REIID riid);

    CARAPI constructor(
        /* [in] */ IContext* context);

    CARAPI constructor(
        /* [in] */ IContext* context,
        /* [in] */ IAttributeSet* attrs);

    CARAPI SetActionBar(
        /* [in] */ IActionBarImpl* impl);

    CARAPI SetShowingForActionMode(
        /* [in] */ Boolean showing);
};

}// namespace Internal
}// namespace Widget
}// namespace Droid
}// namespace Elastos

#endif //__CACTIONBAROVERLAYLAYOUT_H_