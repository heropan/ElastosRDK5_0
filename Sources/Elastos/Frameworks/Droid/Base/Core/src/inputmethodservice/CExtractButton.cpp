
#include "inputmethodservice/CExtractButton.h"

namespace Elastos {
namespace Droid {
namespace InputMethodService {

CAR_OBJECT_IMPL(CExtractButton);
CExtractButton::CExtractButton()
{}

ECode CExtractButton::constructor(
    /* [in] */ IContext* context)
{
    // return Button::constructor(context, NULL);
    assert(0 && "TODO");
    return NOERROR;
}

ECode CExtractButton::constructor(
    /* [in] */ IContext* context,
    /* [in] */ IAttributeSet* attrs)
{
    // return Button::constructor(context, attrs, 0x01010048 /*com.android.internal.R.attr.buttonStyle*/);
    assert(0 && "TODO");
    return NOERROR;
}

ECode CExtractButton::constructor(
    /* [in] */ IContext* context,
    /* [in] */ IAttributeSet* attrs,
    /* [in] */ Int32 defStyle)
{
    // return Button::constructor(context, attrs, defStyle);
    assert(0 && "TODO");
    return NOERROR;
}

Boolean CExtractButton::HasWindowFocus()
{
    // return IsEnabled() && GetVisibility() == IView::VISIBLE;
    assert(0 && "TODO");
    return FALSE;
}

} // namespace InputMethodService
} // namespace Droid
} // namespace Elastos
