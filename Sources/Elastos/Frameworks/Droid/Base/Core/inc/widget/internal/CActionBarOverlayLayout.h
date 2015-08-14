#ifndef __ELASTOS_DROID_WIDGET_INTERNAL_CACTIONBAROVERLAYLAYOUT_H__
#define __ELASTOS_DROID_WIDGET_INTERNAL_CACTIONBAROVERLAYLAYOUT_H__

#include "_CActionBarOverlayLayout.h"


#include "widget/internal/ActionBarOverlayLayout.h"


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

#endif //__ELASTOS_DROID_WIDGET_INTERNAL_CACTIONBAROVERLAYLAYOUT_H__
