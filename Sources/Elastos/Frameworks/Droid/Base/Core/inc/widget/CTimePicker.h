
#ifndef __ELASTOS_DROID_WIDGET_CTIMEPICKER_H__
#define __ELASTOS_DROID_WIDGET_CTIMEPICKER_H__

#include "_CTimePicker.h"
#include "widget/TimePicker.h"



namespace Elastos{
namespace Droid{
namespace Widget{

CarClass(CTimePicker), public TimePicker
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
    CARAPI SetOnTimeChangedListener(
        /* [in] */ IOnTimeChangedListener* onTimeChangedListener);

    CARAPI GetCurrentHour(
        /* [out] */ IInteger32** hour);

    CARAPI SetCurrentHour(
        /* [in] */ IInteger32* currentHour);

    CARAPI SetIs24HourView(
        /* [in] */ Boolean is24HourView);

    CARAPI Is24HourView(
        /* [out] */ Boolean* res);

    CARAPI GetCurrentMinute(
        /* [out] */ IInteger32** currentMinute);

    CARAPI SetCurrentMinute(
        /* [in] */ IInteger32* currentMinute);

};

}// namespace Widget
}// namespace Droid
}// namespace Elastos

#endif //__ELASTOS_DROID_WIDGET_CTIMEPICKER_H__
