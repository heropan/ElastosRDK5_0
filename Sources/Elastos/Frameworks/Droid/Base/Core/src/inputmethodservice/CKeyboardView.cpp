
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
    return KeyboardView::constructor(context, attrs, R::attr::keyboardViewStyle);
}

ECode CKeyboardView::constructor(
    /* [in] */ IContext* context,
    /* [in] */ IAttributeSet* attrs,
    /* [in] */ Int32 defStyle)
{
    return KeyboardView::constructor(context, attrs, defStyle);
}

} // namespace InputMethodService
} // namespace Droid
} // namespace Elastos
