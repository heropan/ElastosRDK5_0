#ifndef __ELASTOS_DROID_WIDGET_CDATETIMEVIEW_H__
#define __ELASTOS_DROID_WIDGET_CDATETIMEVIEW_H__

#include "_Elastos_Droid_Widget_CDateTimeView.h"
#include "elastos/droid/widget/DateTimeView.h"


namespace Elastos{
namespace Droid{
namespace Widget{

CarClass(CDateTimeView), public DateTimeView
{
public:
    IVIEW_METHODS_DECL();
    ITEXTVIEW_METHODS_DECL();
    IDRAWABLECALLBACK_METHODS_DECL();
    IKEYEVENTCALLBACK_METHODS_DECL();
    IACCESSIBILITYEVENTSOURCE_METHODS_DECL();

    CARAPI_(PInterface) Probe(
        /* [in] */ REIID riid);

    CARAPI constructor(
        /* [in] */ IContext* context);

    CARAPI constructor(
        /* [in] */ IContext* context,
        /* [in] */ IAttributeSet* attrs);

    CARAPI SetTime(
        /* [in] */ Int64 time);

    CARAPI Update();

    CARAPI OnPreDraw(
        /* [out] */ Boolean* result);
};

}// namespace Widget
}// namespace Droid
}// namespace Elastos

#endif //__ELASTOS_DROID_WIDGET_CDATETIMEVIEW_H__
