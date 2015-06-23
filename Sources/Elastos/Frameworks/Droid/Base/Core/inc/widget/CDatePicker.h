#ifndef __CDATEPICKER_H_
#define __CDATEPICKER_H_

#include "_CDatePicker.h"
#include "widget/DatePicker.h"



namespace Elastos{
namespace Droid{
namespace Widget{

CarClass(CDatePicker), public DatePicker
{
public:

    IVIEW_METHODS_DECL()
    IVIEWGROUP_METHODS_DECL()
    IVIEWPARENT_METHODS_DECL()
    IVIEWMANAGER_METHODS_DECL()
    IDRAWABLECALLBACK_METHODS_DECL()
    IKEYEVENTCALLBACK_METHODS_DECL()
    IACCESSIBILITYEVENTSOURCE_METHODS_DECL()

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

    CARAPI GetMinDate(
        /* [out] */ Int64* minDate);

    CARAPI SetMinDate(
        /* [in] */ Int64 minDate);

    CARAPI GetMaxDate(
        /* [out] */ Int64* maxDate);

    CARAPI SetMaxDate(
        /* [in] */ Int64 maxDate);

    CARAPI GetCalendarViewShown(
        /* [out] */ Boolean* shown);

    CARAPI GetCalendarView (
        /* [out] */ ICalendarView** view);

    CARAPI SetCalendarViewShown(
        /* [in] */ Boolean shown);

    CARAPI GetSpinnersShown(
        /* [out] */ Boolean* shown);

    CARAPI SetSpinnersShown(
        /* [in] */ Boolean shown);

    CARAPI UpdateDate(
        /* [in] */ Int32 year,
        /* [in] */ Int32 month,
        /* [in] */ Int32 dayOfMonth);

    CARAPI Init(
        /* [in] */ Int32 year,
        /* [in] */ Int32 monthOfYear,
        /* [in] */ Int32 dayOfMonth,
        /* [in] */ IOnDateChangedListener* onDateChangedListener);

    CARAPI GetYear(
        /* [out] */ Int32* year);

    CARAPI GetMonth(
        /* [out] */ Int32* month);

    CARAPI GetDayOfMonth(
        /* [out] */ Int32* day);

    CARAPI GetForegroundGravity(
        /* [out] */ Int32* foregroundGravity);

    CARAPI SetForegroundGravity(
        /* [in] */ Int32 foregroundGravity);

    CARAPI SetForeground(
        /* [in] */ IDrawable* drawable);

    CARAPI GetForeground(
        /* [out] */ IDrawable** foreground);

    CARAPI GetMeasureAllChildren(
        /* [out] */ Boolean* measureAll);

    CARAPI SetMeasureAllChildren(
        /* [in] */ Boolean measureAll);

    CARAPI GetConsiderGoneChildrenWhenMeasuring(
        /* [out] */ Boolean* measureAll);
};

}// namespace Widget
}// namespace Droid
}// namespace Elastos

#endif //__CDATEPICKER_H_
