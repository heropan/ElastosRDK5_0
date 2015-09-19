#include "ext/frameworkext.h"
#include "widget/CNumberPickerCustomEditText.h"

using Elastos::Droid::View::EIID_View;

namespace Elastos{
namespace Droid{
namespace Widget{

IVIEW_METHODS_IMPL(CNumberPickerCustomEditText, NumberPickerCustomEditText)
ITEXTVIEW_METHODS_IMPL(CNumberPickerCustomEditText, NumberPickerCustomEditText);
IDRAWABLECALLBACK_METHODS_IMPL(CNumberPickerCustomEditText, NumberPickerCustomEditText)
IKEYEVENTCALLBACK_METHODS_IMPL(CNumberPickerCustomEditText, NumberPickerCustomEditText)
IACCESSIBILITYEVENTSOURCE_METHODS_IMPL(CNumberPickerCustomEditText, NumberPickerCustomEditText)


PInterface CNumberPickerCustomEditText::Probe(
    /* [in] */ REIID riid)
{
    if (riid == EIID_View) {
        return reinterpret_cast<PInterface>((View*)this);
    }
    return _CNumberPickerCustomEditText::Probe(riid);
}


ECode CNumberPickerCustomEditText::constructor(
        /* [in] */ IContext* context,
        /* [in] */ IAttributeSet* attrs)
{
    return NumberPickerCustomEditText::Init(context, attrs);
}


ECode CNumberPickerCustomEditText::SetSelection(
        /* [in] */ Int32 start,
        /* [in] */ Int32 stop)
{
    return NumberPickerCustomEditText::SetSelection(start, stop);
}

ECode CNumberPickerCustomEditText::SetSelection(
        /* [in] */ Int32 index)
{
    return NumberPickerCustomEditText::SetSelection(index);
}

ECode CNumberPickerCustomEditText::SelectAll()
{
    return NumberPickerCustomEditText::SelectAll();
}

ECode CNumberPickerCustomEditText::ExtendSelection(
        /* [in] */ Int32 index)
{
    return NumberPickerCustomEditText::ExtendSelection(index);
}

ECode CNumberPickerCustomEditText::OnPreDraw(
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
    *result = NumberPickerCustomEditText::OnPreDraw();

    return NOERROR;
}

}// namespace Widget{
}// namespace Droid
}// namespace Elastos