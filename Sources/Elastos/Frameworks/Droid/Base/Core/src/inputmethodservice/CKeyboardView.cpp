
#include "ext/frameworkdef.h"
#include "inputmethodservice/CKeyboardView.h"
#include "R.h"

using Elastos::Droid::R;
using Elastos::Droid::View::EIID_View;

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

PInterface CKeyboardView::Probe(
    /* [in] */ REIID riid)
{
    if (riid == EIID_View) {
        return reinterpret_cast<PInterface>((View*)this);
    }
    return KeyboardView::Probe(riid);
}

} // namespace InputMethodService
} // namespace Droid
} // namespace Elastos
