
#include "inputmethodservice/CExtractButton.h"

using Elastos::Droid::View::EIID_View;
using Elastos::Droid::Widget::EIID_TextView;

namespace Elastos {
namespace Droid {
namespace InputMethodService {

IVIEW_METHODS_IMPL(CExtractButton, _ExtractButton);
IDRAWABLECALLBACK_METHODS_IMPL(CExtractButton, _ExtractButton);
IKEYEVENTCALLBACK_METHODS_IMPL(CExtractButton, _ExtractButton);
IACCESSIBILITYEVENTSOURCE_METHODS_IMPL(CExtractButton, _ExtractButton);
ITEXTVIEW_METHODS_IMPL(CExtractButton, _ExtractButton);


_ExtractButton::_ExtractButton()
{}

_ExtractButton::~_ExtractButton()
{}

Boolean _ExtractButton::HasWindowFocus()
{
    return IsEnabled() && GetVisibility() == IView::VISIBLE;
}

CExtractButton::CExtractButton()
{}

PInterface CExtractButton::Probe(
    /* [in] */ REIID riid)
{
    if (riid == EIID_View) {
        return reinterpret_cast<PInterface>((View*)this);
    }
    else if (riid == EIID_TextView) {
        return reinterpret_cast<PInterface>((TextView*)this);
    }

    return _CExtractButton::Probe(riid);
}

ECode CExtractButton::constructor(
    /* [in] */ IContext* context)
{
    return Button::Init(context, NULL);
}

ECode CExtractButton::constructor(
    /* [in] */ IContext* context,
    /* [in] */ IAttributeSet* attrs)
{
    return Button::Init(context, attrs, 0x01010048 /*com.android.internal.R.attr.buttonStyle*/);
}

ECode CExtractButton::constructor(
    /* [in] */ IContext* context,
    /* [in] */ IAttributeSet* attrs,
    /* [in] */ Int32 defStyle)
{
    return Button::Init(context, attrs, defStyle);
}

ECode CExtractButton::OnPreDraw(
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
    *result = Button::OnPreDraw();

    return NOERROR;
}

} // namespace InputMethodService
} // namespace Droid
} // namespace Elastos
