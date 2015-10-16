
#include "elastos/droid/view/inputmethod/CInputMethodManagerInputMethodCallback.h"
#include "elastos/droid/view/inputmethod/CInputMethodManager.h"

namespace Elastos {
namespace Droid {
namespace View {
namespace InputMethod {

ECode CInputMethodManagerInputMethodCallback::constructor(
    /* [in] */ IInputMethodManager* imm)
{
    mIMM = imm;
    return NOERROR;
}

ECode CInputMethodManagerInputMethodCallback::FinishedEvent(
    /* [in] */ Int32 seq,
    /* [in]*/ Boolean handled)
{
    ((CInputMethodManager*)mIMM.Get())->FinishedEvent(seq, handled);
    return NOERROR;
}

ECode CInputMethodManagerInputMethodCallback::SessionCreated(
   /* [in] */ IIInputMethodSession* session)
{
    return NOERROR;
}

ECode CInputMethodManagerInputMethodCallback::ToString(
    /* [out] */ String* str)
{
    assert(0);
    return E_NOT_IMPLEMENTED;
}

} // namespace InputMethod
} // namespace View
} // namespace Droid
} // namespace Elastos
