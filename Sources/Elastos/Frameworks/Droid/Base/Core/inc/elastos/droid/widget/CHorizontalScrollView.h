#ifndef __ELASTOS_DROID_WIDGET_CHORIZONTALSCROLLVIEW_H__
#define __ELASTOS_DROID_WIDGET_CHORIZONTALSCROLLVIEW_H__

#include "_Elastos_Droid_Widget_CHorizontalScrollView.h"
#include "elastos/droid/widget/HorizontalScrollView.h"




namespace Elastos {
namespace Droid {
namespace Widget {

CarClass(CHorizontalScrollView), public HorizontalScrollView
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

    CARAPI constructor(
        /* [in] */ IContext* context,
        /* [in] */ IAttributeSet* attrs,
        /* [in] */ Int32 defStyle);

public:
    CARAPI ArrowScroll(
        /* [in] */ Int32 direction,
        /* [out] */ Boolean* consumed);

    CARAPI ExecuteKeyEvent(
        /* [in] */ IKeyEvent* event,
        /* [out] */ Boolean* handled);

    CARAPI Fling(
        /* [in] */ Int32 velocityY);

    CARAPI FullScroll(
        /* [in] */ Int32 direction,
        /* [out] */ Boolean* consumed);

    CARAPI GetMaxScrollAmount(
        /* [out] */ Int32* maxScrollAmount);

    CARAPI IsFillViewport(
        /* [out] */ Boolean* isFilled);

    CARAPI IsSmoothScrollingEnabled(
        /* [out] */ Boolean* enabled);

    CARAPI PageScroll(
        /* [in] */ Int32 direction,
        /* [out] */ Boolean* consumed);

    CARAPI SetFillViewport(
        /* [in] */ Boolean fillViewport);

    CARAPI SetSmoothScrollingEnabled(
        /* [in] */ Boolean smoothScrollingEnabled);

    CARAPI SmoothScrollBy(
        /* [in] */ Int32 dx,
        /* [in] */ Int32 dy);

    CARAPI SmoothScrollTo(
        /* [in] */ Int32 x,
        /* [in] */ Int32 y);
};

}// namespace Widget
}// namespace Droid
}// namespace Elastos

#endif //__ELASTOS_DROID_WIDGET_CHORIZONTALSCROLLVIEW_H__
