
#include "inputmethodservice/CExtractButton.h"

using Elastos::Droid::View::EIID_View;
using Elastos::Droid::Widget::EIID_TextView;

namespace Elastos {
namespace Droid {
namespace InputMethodService {

CAR_OBJECT_IMPL(CExtractButton);

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

    return Button::Probe(riid);
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

Boolean CExtractButton::HasWindowFocus()
{
    return IsEnabled() && GetVisibility() == IView::VISIBLE;
}

} // namespace InputMethodService
} // namespace Droid
} // namespace Elastos
