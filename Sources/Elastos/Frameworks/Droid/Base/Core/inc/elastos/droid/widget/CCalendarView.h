#include "_Elastos_Droid_Widget_CCalendarView.h"
#include "CalendarView.h"


namespace Elastos {
namespace Droid {
namespace Widget {

CarClass(CCalendarView) , public CalendarView
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

    /**
     * Sets the number of weeks to be shown.
     *
     * @param count The shown week count.
     *
     * @attr ref android.R.styleable#CalendarView_shownWeekCount
     */
    CARAPI SetShownWeekCount(
        /* [in] */ Int32 count);

    CARAPI GetShownWeekCount(
        /* [out] */ Int32* count);

    CARAPI SetSelectedWeekBackgroundColor(
        /* [in] */ Int32 color);

    CARAPI GetSelectedWeekBackgroundColor(
        /* [out] */ Int32* color);

    CARAPI SetFocusedMonthDateColor(
        /* [in] */ Int32 color);

    CARAPI GetFocusedMonthDateColor(
        /* [out] */ Int32* color);

    CARAPI SetUnfocusedMonthDateColor(
        /* [in] */ Int32 color);

    CARAPI GetUnfocusedMonthDateColor(
        /* [out] */ Int32* color);

    CARAPI SetWeekNumberColor(
        /* [in] */ Int32 color);

    CARAPI GetWeekNumberColor(
        /* [out] */ Int32* color);

    CARAPI SetWeekSeparatorLineColor(
        /* [in] */ Int32 color);

    CARAPI GetWeekSeparatorLineColor(
        /* [out] */ Int32* color);

    CARAPI SetSelectedDateVerticalBar(
        /* [in] */ Int32 resourceId);

    CARAPI SetSelectedDateVerticalBar(
        /* [in] */ IDrawable* drawable);

    CARAPI GetSelectedDateVerticalBar(
        /* [out] */ IDrawable** drawable);

    CARAPI SetWeekDayTextAppearance(
        /* [in] */ Int32 resourceId);

    CARAPI GetWeekDayTextAppearance(
        /* [out] */ Int32* resourceId);

    CARAPI SetDateTextAppearance(
        /* [in] */ Int32 resourceId);

    CARAPI GetDateTextAppearance(
        /* [out] */ Int32* resourceId);

    CARAPI GetMinDate(
        /* [out] */ Int64* minDate);

    CARAPI SetMinDate(
        /* [in] */ Int64 minDate);

    CARAPI GetMaxDate(
        /* [out] */ Int64* maxDate);

    CARAPI SetMaxDate(
        /* [in] */ Int64 maxDate);

    CARAPI SetShowWeekNumber(
        /* [in] */ Boolean showWeekNumber);

    CARAPI GetShowWeekNumber(
        /* [out] */ Boolean* showWeekNumber);

    CARAPI GetFirstDayOfWeek(
        /* [out] */ Int32* firstDayOfWeek);

    CARAPI SetFirstDayOfWeek(
        /* [in] */ Int32 firstDayOfWeek);

    CARAPI SetOnDateChangeListener(
        /* [in] */ IOnDateChangeListener* listener);

    CARAPI GetDate(
        /* [out] */ Int64* data);

    CARAPI SetDate(
        /* [in] */ Int64 date);

    CARAPI SetDate(
        /* [in] */ Int64 date,
        /* [in] */ Boolean animate,
        /* [in] */ Boolean center);

};
}// namespace Widget
}// namespace Droid
}// namespace Elastos
