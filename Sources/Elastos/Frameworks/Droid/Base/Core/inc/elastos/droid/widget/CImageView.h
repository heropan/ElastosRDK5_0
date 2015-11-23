#ifndef __ELASTOS_DROID_WIDGET_CIMAGEVIEW_H__
#define __ELASTOS_DROID_WIDGET_CIMAGEVIEW_H__

#include "_Elastos_Droid_Widget_CImageView.h"

#include "elastos/droid/widget/ImageView.h"


namespace Elastos {
namespace Droid {
namespace Widget {

CarClass(CImageView), public ImageView
{
public:
    IVIEW_METHODS_DECL()
    IDRAWABLECALLBACK_METHODS_DECL()
    IKEYEVENTCALLBACK_METHODS_DECL()
    IACCESSIBILITYEVENTSOURCE_METHODS_DECL()
    IIMAGEVIEW_METHODS_DECL()

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

    CARAPI SetAlpha(
        /* [in] */ Int32 alpha);
};

}// namespace Widget
}// namespace Droid
}// namespace Elastos

#endif //__ELASTOS_DROID_WIDGET_CIMAGEVIEW_H__
