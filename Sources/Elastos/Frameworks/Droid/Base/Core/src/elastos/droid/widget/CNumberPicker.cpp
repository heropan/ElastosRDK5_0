#include "ext/frameworkext.h"
#include "widget/CNumberPicker.h"

using Elastos::Droid::View::EIID_View;
using Elastos::Droid::View::EIID_ViewGroup;

namespace Elastos{
namespace Droid{
namespace Widget{

IVIEW_METHODS_IMPL(CNumberPicker, NumberPicker)
IVIEWGROUP_METHODS_IMPL(CNumberPicker, NumberPicker)
IVIEWPARENT_METHODS_IMPL(CNumberPicker, NumberPicker)
IVIEWMANAGER_METHODS_IMPL(CNumberPicker, NumberPicker)
IDRAWABLECALLBACK_METHODS_IMPL(CNumberPicker, NumberPicker)
IKEYEVENTCALLBACK_METHODS_IMPL(CNumberPicker, NumberPicker)
IACCESSIBILITYEVENTSOURCE_METHODS_IMPL(CNumberPicker, NumberPicker)
ILINEARLAYOUT_METHODS_IMPL(CNumberPicker, NumberPicker)

PInterface CNumberPicker::Probe(
        /* [in] */ REIID riid)
{
    if (riid == EIID_View) {
        return reinterpret_cast<PInterface>((View*)this);
    } else if (riid == EIID_ViewGroup) {
        return reinterpret_cast<PInterface>((ViewGroup*)this);
    }
    return _CNumberPicker::Probe(riid);
}

ECode CNumberPicker::constructor(
        /* [in] */ IContext* context)
{
    return NumberPicker::Init(context);
}

ECode CNumberPicker::constructor(
        /* [in] */ IContext* context,
        /* [in] */ IAttributeSet* attrs)
{
    return NumberPicker::Init(context, attrs);
}

ECode CNumberPicker::constructor(
        /* [in] */ IContext* context,
        /* [in] */ IAttributeSet* attrs,
        /* [in] */ Int32 defStyle)
{
    return NumberPicker::Init(context, attrs, defStyle);
}

ECode CNumberPicker::SetOnValueChangedListener(
        /* [in] */ INumberPickerOnValueChangeListener* onValueChangedListener)
{
    return NumberPicker::SetOnValueChangedListener(onValueChangedListener);
}

ECode CNumberPicker::SetOnScrollListener(
        /* [in] */ INumberPickerOnScrollListener* onScrollListener)
{
    return NumberPicker::SetOnScrollListener(onScrollListener);
}

ECode CNumberPicker::SetFormatter(
        /* [in] */ INumberPickerFormatter* formatter)
{
    return NumberPicker::SetFormatter(formatter);
}

ECode CNumberPicker::SetValue(
        /* [in] */ Int32 value)
{
    return NumberPicker::SetValue(value);
}

ECode CNumberPicker::GetWrapSelectorWheel(
        /* [out] */ Boolean* res)
{
    VALIDATE_NOT_NULL(res);
    *res = NumberPicker::GetWrapSelectorWheel();
    return NOERROR;
}

ECode CNumberPicker::SetWrapSelectorWheel(
        /* [in] */ Boolean wrapSelectorWheel)
{
    return NumberPicker::SetWrapSelectorWheel(wrapSelectorWheel);
}

ECode CNumberPicker::SetOnLongPressUpdateInterval(
        /* [in] */ Int64 intervalMillis)
{
    return NumberPicker::SetOnLongPressUpdateInterval(intervalMillis);
}

ECode CNumberPicker::GetValue(
        /* [out] */ Int32* value)
{
    VALIDATE_NOT_NULL(value);
    *value = NumberPicker::GetValue();
    return NOERROR;
}

ECode CNumberPicker::GetMinValue(
        /* [out] */ Int32* MinValue)
{
    VALIDATE_NOT_NULL(MinValue);
    *MinValue = NumberPicker::GetMinValue();
    return NOERROR;
}

ECode CNumberPicker::SetMinValue(
        /* [in] */ Int32 MinValue)
{
    return NumberPicker::SetMinValue(MinValue);
}

ECode CNumberPicker::GetMaxValue(
        /* [out] */ Int32* MaxValue)
{
    VALIDATE_NOT_NULL(MaxValue);
    *MaxValue = NumberPicker::GetMaxValue();
    return NOERROR;
}

ECode CNumberPicker::SetMaxValue(
        /* [in] */ Int32 maxValue)
{
    return NumberPicker::SetMaxValue(maxValue);
}

ECode CNumberPicker::GetDisplayedValues(
    /* [out, callee] */ ArrayOf<String>** displayedValues)
{
    VALIDATE_NOT_NULL(displayedValues);
    *displayedValues = NULL;
    AutoPtr< ArrayOf<String> > array = NumberPicker::GetDisplayedValues();
    if (array != NULL) {
        *displayedValues = array->Clone();
        REFCOUNT_ADD(*displayedValues);
    }
    return NOERROR;
}

ECode CNumberPicker::SetDisplayedValues(
        /* [in] */ ArrayOf<String>* displayedValues)
{
    return NumberPicker::SetDisplayedValues(displayedValues);
}

}// namespace Widget
}// namespace Droid
}// namespace Elastos
