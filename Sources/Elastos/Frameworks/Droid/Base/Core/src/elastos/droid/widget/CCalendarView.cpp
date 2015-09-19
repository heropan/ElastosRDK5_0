#include "CCalendarView.h"

using Elastos::Droid::View::EIID_View;
using Elastos::Droid::View::EIID_ViewGroup;

namespace Elastos {
namespace Droid {
namespace Widget {

IVIEW_METHODS_IMPL(CCalendarView, CalendarView)

IVIEWGROUP_METHODS_IMPL(CCalendarView, CalendarView)

IVIEWPARENT_METHODS_IMPL(CCalendarView, CalendarView)

IVIEWMANAGER_METHODS_IMPL(CCalendarView, CalendarView)

IDRAWABLECALLBACK_METHODS_IMPL(CCalendarView, CalendarView)

IKEYEVENTCALLBACK_METHODS_IMPL(CCalendarView, CalendarView)

IACCESSIBILITYEVENTSOURCE_METHODS_IMPL(CCalendarView, CalendarView)

IFRAMELAYOUT_METHODS_IMPL(CCalendarView, CalendarView)

PInterface CCalendarView::Probe(
    /* [in] */ REIID riid)
{
    if (riid == EIID_View) {
        return reinterpret_cast<PInterface>((View*)this);
    }
    else if (riid == EIID_ViewGroup) {
        return reinterpret_cast<PInterface>((ViewGroup*)this);
    }
    return _CCalendarView::Probe(riid);
}

ECode CCalendarView::constructor(
    /* [in] */ IContext* context)
{
    return CalendarView::Init(context);
}

ECode CCalendarView::constructor(
    /* [in] */ IContext* context,
    /* [in] */ IAttributeSet* attrs)
{
    return CalendarView::Init(context, attrs);
}

ECode CCalendarView::constructor(
    /* [in] */ IContext* context,
    /* [in] */ IAttributeSet* attrs,
    /* [in] */ Int32 defStyle)
{
    return CalendarView::Init(context, attrs, defStyle);
}

/**
 * Sets the number of weeks to be shown.
 *
 * @param count The shown week count.
 *
 * @attr ref android.R.styleable#CalendarView_shownWeekCount
 */
ECode CCalendarView::SetShownWeekCount(
    /* [in] */ Int32 count)
{
    return CalendarView::SetShownWeekCount(count);
}

ECode CCalendarView::GetShownWeekCount(
    /* [out] */ Int32* count)
{
    *count = CalendarView::GetShownWeekCount();
    return NOERROR;
}

ECode CCalendarView::SetSelectedWeekBackgroundColor(
    /* [in] */ Int32 color)
{
    return CalendarView::SetSelectedWeekBackgroundColor(color);
}

ECode CCalendarView::GetSelectedWeekBackgroundColor(
    /* [out] */ Int32* color)
{
    *color = CalendarView::GetSelectedWeekBackgroundColor();
    return NOERROR;
}

ECode CCalendarView::SetFocusedMonthDateColor(
    /* [in] */ Int32 color)
{
    return CalendarView::SetFocusedMonthDateColor(color);
}

ECode CCalendarView::GetFocusedMonthDateColor(
    /* [out] */ Int32* color)
{
    *color = CalendarView::GetFocusedMonthDateColor();
    return NOERROR;
}

ECode CCalendarView::SetUnfocusedMonthDateColor(
    /* [in] */ Int32 color)
{
    return CalendarView::SetUnfocusedMonthDateColor(color);
}

ECode CCalendarView::GetUnfocusedMonthDateColor(
    /* [out] */ Int32* color)
{
    *color = CalendarView::GetUnfocusedMonthDateColor();
    return NOERROR;
}

ECode CCalendarView::SetWeekNumberColor(
    /* [in] */ Int32 color)
{
    return CalendarView::SetWeekNumberColor(color);
}

ECode CCalendarView::GetWeekNumberColor(
    /* [out] */ Int32* color)
{
    *color = CalendarView::GetWeekNumberColor();
    return NOERROR;
}

ECode CCalendarView::SetWeekSeparatorLineColor(
    /* [in] */ Int32 color)
{
    return CalendarView::SetWeekSeparatorLineColor(color);
}

ECode CCalendarView::GetWeekSeparatorLineColor(
    /* [out] */ Int32* color)
{
    *color = CalendarView::GetWeekSeparatorLineColor();
    return NOERROR;
}

ECode CCalendarView::SetSelectedDateVerticalBar(
    /* [in] */ Int32 resourceId)
{
    return CalendarView::SetSelectedDateVerticalBar(resourceId);
}

ECode CCalendarView::SetSelectedDateVerticalBar(
    /* [in] */ IDrawable* drawable)
{
    return CalendarView::SetSelectedDateVerticalBar(drawable);
}

ECode CCalendarView::GetSelectedDateVerticalBar(
    /* [out] */ IDrawable** drawable)
{
    AutoPtr<IDrawable> temp = CalendarView::GetSelectedDateVerticalBar();
    *drawable = temp;
    REFCOUNT_ADD(*drawable);
    return NOERROR;
}

ECode CCalendarView::SetWeekDayTextAppearance(
    /* [in] */ Int32 resourceId)
{
    return CalendarView::SetWeekDayTextAppearance(resourceId);
}

ECode CCalendarView::GetWeekDayTextAppearance(
    /* [out] */ Int32* resourceId)
{
    *resourceId = CalendarView::GetWeekDayTextAppearance();
    return NOERROR;
}

ECode CCalendarView::SetDateTextAppearance(
    /* [in] */ Int32 resourceId)
{
    return CalendarView::SetDateTextAppearance(resourceId);
}

ECode CCalendarView::GetDateTextAppearance(
    /* [out] */ Int32* resourceId)
{
    *resourceId = CalendarView::GetDateTextAppearance();
    return NOERROR;
}

ECode CCalendarView::GetMinDate(
    /* [out] */ Int64* minDate)
{
    *minDate = CalendarView::GetMinDate();
    return NOERROR;
}

ECode CCalendarView::SetMinDate(
    /* [in] */ Int64 minDate)
{
    return CalendarView::SetMinDate(minDate);
}

ECode CCalendarView::GetMaxDate(
    /* [out] */ Int64* maxDate)
{
    *maxDate = CalendarView::GetMaxDate();
    return NOERROR;
}

ECode CCalendarView::SetMaxDate(
    /* [in] */ Int64 maxDate)
{
    return CalendarView::SetMaxDate(maxDate);
}

ECode CCalendarView::SetShowWeekNumber(
    /* [in] */ Boolean showWeekNumber)
{
    return CalendarView::SetShowWeekNumber(showWeekNumber);
}

ECode CCalendarView::GetShowWeekNumber(
    /* [out] */ Boolean* showWeekNumber)
{
    *showWeekNumber = CalendarView::GetShowWeekNumber();
    return NOERROR;
}

ECode CCalendarView::GetFirstDayOfWeek(
    /* [out] */ Int32* firstDayOfWeek)
{
    *firstDayOfWeek = CalendarView::GetFirstDayOfWeek();
    return NOERROR;
}

ECode CCalendarView::SetFirstDayOfWeek(
    /* [in] */ Int32 firstDayOfWeek)
{
    return CalendarView::SetFirstDayOfWeek(firstDayOfWeek);
}

ECode CCalendarView::SetOnDateChangeListener(
    /* [in] */ IOnDateChangeListener* listener)
{
    return CalendarView::SetOnDateChangeListener(listener);
}

ECode CCalendarView::GetDate(
    /* [out] */ Int64* data)
{
    *data = CalendarView::GetDate();
    return NOERROR;
}

ECode CCalendarView::SetDate(
    /* [in] */ Int64 date)
{
    return CalendarView::SetDate(date);
}

ECode CCalendarView::SetDate(
    /* [in] */ Int64 date,
    /* [in] */ Boolean animate,
    /* [in] */ Boolean center)
{
    return CalendarView::SetDate(date, animate, center);
}

}// namespace Widget
}// namespace Droid
}// namespace Elastos
