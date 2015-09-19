
#include "ext/frameworkdef.h"
#include "inputmethodservice/CKeyboardView.h"
#include "R.h"

using Elastos::Droid::R;

namespace Elastos {
namespace Droid {
namespace InputMethodService {

CAR_OBJECT_IMPL(CKeyboardView);

ECode CKeyboardView::constructor(
    /* [in] */ IContext* context,
    /* [in] */ IAttributeSet* attrs)
{
    return KeyboardView::constructor(context, attrs);
}

ECode CKeyboardView::constructor(
    /* [in] */ IContext* context,
    /* [in] */ IAttributeSet* attrs,
    /* [in] */ Int32 defStyleAttr)
{
    return KeyboardView::constructor(context, attrs, defStyleAttr);
}

ECode CKeyboardView::constructor(
    /* [in] */ IContext* context,
    /* [in] */ IAttributeSet* attrs,
    /* [in] */ Int32 defStyleAttr,
    /* [in] */ Int32 defStyleRes)
{
    return KeyboardView::constructor(context, attrs, defStyleAttr, defStyleRes);
}

} // namespace InputMethodService
} // namespace Droid
} // namespace Elastos
