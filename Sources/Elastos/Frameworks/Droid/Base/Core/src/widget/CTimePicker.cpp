#include "ext/frameworkext.h"
#include "widget/CTimePicker.h"

using Elastos::Droid::View::EIID_View;
using Elastos::Droid::View::EIID_ViewGroup;

namespace Elastos{
namespace Droid{
namespace Widget{

IVIEW_METHODS_IMPL(CTimePicker, TimePicker)
IVIEWGROUP_METHODS_IMPL(CTimePicker, TimePicker)
IVIEWPARENT_METHODS_IMPL(CTimePicker, TimePicker)
IVIEWMANAGER_METHODS_IMPL(CTimePicker, TimePicker)
IDRAWABLECALLBACK_METHODS_IMPL(CTimePicker, TimePicker)
IKEYEVENTCALLBACK_METHODS_IMPL(CTimePicker, TimePicker)
IACCESSIBILITYEVENTSOURCE_METHODS_IMPL(CTimePicker, TimePicker)
IFRAMELAYOUT_METHODS_IMPL(CTimePicker, TimePicker)

PInterface CTimePicker::Probe(
    /* [in] */ REIID riid)
{
    if (riid == EIID_View) {
        return reinterpret_cast<PInterface>((View*)this);
    }
    else if (riid == EIID_ViewGroup) {
        return reinterpret_cast<PInterface>((ViewGroup*)this);
    }
    return _CTimePicker::Probe(riid);
}

ECode CTimePicker::constructor(
    /* [in] */ IContext* context)
{
    return TimePicker::Init(context);
}

ECode CTimePicker::constructor(
    /* [in] */ IContext* context,
    /* [in] */ IAttributeSet* attrs)
{
    return TimePicker::Init(context, attrs);
}

ECode CTimePicker::constructor(
    /* [in] */ IContext* context,
    /* [in] */ IAttributeSet* attrs,
    /* [in] */ Int32 defStyle)
{
    return TimePicker::Init(context, attrs, defStyle);
}

ECode CTimePicker::SetOnTimeChangedListener(
    /* [in] */ IOnTimeChangedListener* onTimeChangedListener)
{
    return TimePicker::SetOnTimeChangedListener(onTimeChangedListener);
}

ECode CTimePicker::GetCurrentHour(
    /* [out] */ IInteger32** hour)
{
    VALIDATE_NOT_NULL(hour);
    AutoPtr<IInteger32> current = TimePicker::GetCurrentHour();
    *hour = current;
    REFCOUNT_ADD(*hour);
    return NOERROR;
}

ECode CTimePicker::SetCurrentHour(
    /* [in] */ IInteger32* currentHour)
{
    return TimePicker::SetCurrentHour(currentHour);
}

ECode CTimePicker::SetIs24HourView(
    /* [in] */ Boolean is24HourView)
{
    return TimePicker::SetIs24HourView(is24HourView);
}

ECode CTimePicker::Is24HourView(
    /* [out] */ Boolean* res)
{
    VALIDATE_NOT_NULL(res);
    *res = TimePicker::Is24HourView();
    return NOERROR;
}

ECode CTimePicker::GetCurrentMinute(
    /* [out] */ IInteger32** currentMinute)
{
    VALIDATE_NOT_NULL(currentMinute);
    AutoPtr<IInteger32> current = TimePicker::GetCurrentMinute();
    *currentMinute = current;
    REFCOUNT_ADD(*currentMinute);
    return NOERROR;
}

ECode CTimePicker::SetCurrentMinute(
    /* [in] */ IInteger32* currentMinute)
{
    return TimePicker::SetCurrentMinute(currentMinute);
}

}// namespace Widget
}// namespace Droid
}// namespace Elastos
