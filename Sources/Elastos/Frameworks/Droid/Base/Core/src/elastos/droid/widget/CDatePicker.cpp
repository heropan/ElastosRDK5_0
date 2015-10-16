#include "elastos/droid/widget/CDatePicker.h"

using Elastos::Droid::View::EIID_View;
using Elastos::Droid::View::EIID_ViewGroup;

namespace Elastos{
namespace Droid{
namespace Widget{

IVIEW_METHODS_IMPL(CDatePicker, DatePicker)
IVIEWGROUP_METHODS_IMPL(CDatePicker, DatePicker)
IVIEWPARENT_METHODS_IMPL(CDatePicker, DatePicker)
IVIEWMANAGER_METHODS_IMPL(CDatePicker, DatePicker)
IDRAWABLECALLBACK_METHODS_IMPL(CDatePicker, DatePicker)
IKEYEVENTCALLBACK_METHODS_IMPL(CDatePicker, DatePicker)
IACCESSIBILITYEVENTSOURCE_METHODS_IMPL(CDatePicker, DatePicker)

PInterface CDatePicker::Probe(
    /* [in] */ REIID riid)
{
    if (riid == EIID_View) {
        return reinterpret_cast<PInterface>((View*)this);
    }
    else if (riid == EIID_ViewGroup) {
        return reinterpret_cast<PInterface>((ViewGroup*)this);
    }
    return _CDatePicker::Probe(riid);
}

ECode CDatePicker::constructor(
    /* [in] */ IContext* context)
{
    return DatePicker::Init(context);
}

ECode CDatePicker::constructor(
    /* [in] */ IContext* context,
    /* [in] */ IAttributeSet* attrs)
{
    return DatePicker::Init(context, attrs);
}

ECode CDatePicker::constructor(
    /* [in] */ IContext* context,
    /* [in] */ IAttributeSet* attrs,
    /* [in] */ Int32 defStyle)
{
    return DatePicker::Init(context, attrs, defStyle);
}

ECode CDatePicker::GetMinDate(
    /* [out] */ Int64* minDate)
{
    VALIDATE_NOT_NULL(minDate);
    *minDate = DatePicker::GetMinDate();
    return NOERROR;
}

ECode CDatePicker::SetMinDate(
    /* [in] */ Int64 minDate)
{
    return DatePicker::SetMinDate(minDate);
}

ECode CDatePicker::GetMaxDate(
    /* [out] */ Int64* maxDate)
{
    VALIDATE_NOT_NULL(maxDate);
    *maxDate = DatePicker::GetMaxDate();
    return NOERROR;
}

ECode CDatePicker::SetMaxDate(
    /* [in] */ Int64 maxDate)
{
    return DatePicker::SetMaxDate(maxDate);
}

ECode CDatePicker::GetCalendarViewShown(
    /* [out] */ Boolean* shown)
{
    VALIDATE_NOT_NULL(shown);
    *shown = DatePicker::GetCalendarViewShown();
    return NOERROR;
}

ECode CDatePicker::GetCalendarView (
    /* [out] */ ICalendarView** view)
{
    VALIDATE_NOT_NULL(view);
    AutoPtr<ICalendarView> calendarView = DatePicker::GetCalendarView();
    *view = calendarView;
    REFCOUNT_ADD(*view);
    return NOERROR;
}

ECode CDatePicker::SetCalendarViewShown(
    /* [in] */ Boolean shown)
{
    return DatePicker::SetCalendarViewShown(shown);
}

ECode CDatePicker::GetSpinnersShown(
    /* [out] */ Boolean* shown)
{
    VALIDATE_NOT_NULL(shown);
    *shown = DatePicker::GetSpinnersShown();
    return NOERROR;
}

ECode CDatePicker::SetSpinnersShown(
    /* [in] */ Boolean shown)
{
    return DatePicker::SetSpinnersShown(shown);
}

ECode CDatePicker::UpdateDate(
    /* [in] */ Int32 year,
    /* [in] */ Int32 month,
    /* [in] */ Int32 dayOfMonth)
{
    return DatePicker::UpdateDate(year, month, dayOfMonth);
}

ECode CDatePicker::Init(
    /* [in] */ Int32 year,
    /* [in] */ Int32 monthOfYear,
    /* [in] */ Int32 dayOfMonth,
    /* [in] */ IOnDateChangedListener* onDateChangedListener)
{
    return DatePicker::Init(year, monthOfYear, dayOfMonth, onDateChangedListener);
}

ECode CDatePicker::GetYear(
    /* [out] */ Int32* year)
{
    VALIDATE_NOT_NULL(year);
    *year = DatePicker::GetYear();
    return NOERROR;
}

ECode CDatePicker::GetMonth(
    /* [out] */ Int32* month)
{
    VALIDATE_NOT_NULL(month);
    *month = DatePicker::GetMonth();
    return NOERROR;
}

ECode CDatePicker::GetDayOfMonth(
    /* [out] */ Int32* day)
{
    VALIDATE_NOT_NULL(day);
    *day = DatePicker::GetDayOfMonth();
    return NOERROR;
}

ECode CDatePicker::GetForegroundGravity(
    /* [out] */ Int32* foregroundGravity)
{
    VALIDATE_NOT_NULL(foregroundGravity);
    *foregroundGravity  = DatePicker::GetForegroundGravity();
    return NOERROR;
}

ECode CDatePicker::SetForegroundGravity(
    /* [in] */ Int32 foregroundGravity)
{
    return DatePicker::SetForegroundGravity(foregroundGravity);
}

ECode CDatePicker::SetForeground(
    /* [in] */ IDrawable* drawable)
{
    return DatePicker::SetForeground(drawable);
}

ECode CDatePicker::GetForeground(
    /* [out] */ IDrawable** foreground)
{
    VALIDATE_NOT_NULL(foreground);
    AutoPtr<IDrawable> g = DatePicker::GetForeground();
    *foreground = g;
    REFCOUNT_ADD(*foreground);
    return NOERROR;
}

ECode CDatePicker::GetMeasureAllChildren(
    /* [out] */ Boolean* measureAll)
{
    VALIDATE_NOT_NULL(measureAll);
    *measureAll  = DatePicker::GetMeasureAllChildren();
    return NOERROR;
}

ECode CDatePicker::SetMeasureAllChildren(
    /* [in] */ Boolean measureAll)
{
    return DatePicker::SetMeasureAllChildren(measureAll);
}

ECode CDatePicker::GetConsiderGoneChildrenWhenMeasuring(
    /* [out] */ Boolean* consider)
{
    VALIDATE_NOT_NULL(consider);
    *consider  = DatePicker::GetConsiderGoneChildrenWhenMeasuring();
    return NOERROR;
}


}// namespace Widget
}// namespace Droid
}// namespace Elastos
