#ifndef __ELASTOS_DROID_WIDGET_CZOOMCONTROLS_H__
#define __ELASTOS_DROID_WIDGET_CZOOMCONTROLS_H__

#include "_Elastos_Droid_Widget_CZoomControls.h"
#include "elastos/droid/widget/ZoomControls.h"



namespace Elastos {
namespace Droid {
namespace Widget {

CarClass(CZoomControls) , public ZoomControls
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

    CARAPI constructor(
        /* [in] */ IContext* context,
        /* [in] */ IAttributeSet* attrs = NULL);

    CARAPI_(PInterface) Probe(
        /* [in] */ REIID riid);

    CARAPI SetOnZoomInClickListener(
        /* [in] */ IViewOnClickListener* listener);

    CARAPI SetOnZoomOutClickListener(
        /* [in] */ IViewOnClickListener* listener);

    /*
     * Sets how fast you get zoom events when the user holds down the
     * zoom in/out buttons.
     */
    CARAPI SetZoomSpeed(
        /* [in] */ Int64 speed);

    CARAPI Show();

    CARAPI Hide();

    CARAPI SetIsZoomInEnabled(
        /* [in] */ Boolean isEnabled);

    CARAPI SetIsZoomOutEnabled(
        /* [in] */ Boolean isEnabled);
};

}// namespace Widget
}// namespace Droid
}// namespace Elastos

#endif
