#ifndef __CNOTIFICATIONPANELTITLE_H_
#define __CNOTIFICATIONPANELTITLE_H_

#include "_CNotificationPanelTitle.h"
#include "view/ViewMacro.h"
#include "view/ViewGroupLayoutParamsMacro.h"
#include "widget/RelativeLayoutMacro.h"
#include "systemui/statusbar/tablet/NotificationPanelTitle.h"

namespace Elastos {
namespace Droid {
namespace SystemUI {
namespace StatusBar {
namespace Tablet {

CarClass(CNotificationPanelTitle), public NotificationPanelTitle
{
public:
    IVIEW_METHODS_DECL()
    IVIEWGROUP_METHODS_DECL()
    IVIEWPARENT_METHODS_DECL()
    IVIEWMANAGER_METHODS_DECL()
    IDRAWABLECALLBACK_METHODS_DECL()
    IKEYEVENTCALLBACK_METHODS_DECL()
    IACCESSIBILITYEVENTSOURCE_METHODS_DECL()
    IRELATIVELAYOUT_METHODS_DECL()

    CARAPI_(PInterface) Probe(
        /* [in] */ REIID riid);

    CARAPI constructor(
        /* [in] */ IContext* context,
        /* [in] */ IAttributeSet* attrs);

    CARAPI SetPanel(
        /* [in] */ INotificationPanel* b);

    CARAPI OnClick(
        /* [in] */ IView* v);
};

}// namespace Tablet
}// namespace StatusBar
}// namespace SystemUI
}// namespace Droid
}// namespace Elastos

#endif //__CNOTIFICATIONPANELTITLE_H_
