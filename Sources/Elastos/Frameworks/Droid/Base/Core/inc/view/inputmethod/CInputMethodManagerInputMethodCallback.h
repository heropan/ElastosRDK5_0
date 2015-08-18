
#ifndef __ELASTOS_DROID_VIEW_INPUTMETHOD_CINPUTMETHODMANAGERINPUTMETHODCALLBACK_H__
#define __ELASTOS_DROID_VIEW_INPUTMETHOD_CINPUTMETHODMANAGERINPUTMETHODCALLBACK_H__

#include "_Elastos_Droid_View_InputMethod_CInputMethodManagerInputMethodCallback.h"

using Elastos::Droid::Internal::View::IIInputMethodSession;

namespace Elastos {
namespace Droid {
namespace View {
namespace InputMethod {

CarClass(CInputMethodManagerInputMethodCallback)
{
public:
    CARAPI constructor(
        /* [in] */ IInputMethodManager* imm);

    CARAPI FinishedEvent(
        /* [in] */ Int32 seq,
        /* [in]*/ Boolean handled);

    CARAPI SessionCreated(
        /* [in] */ IIInputMethodSession* session);

    CARAPI ToString(
        /* [out] */ String* str);

private:
    AutoPtr<IInputMethodManager> mIMM;
};

} // namespace InputMethod
} // namespace View
} // namespace Droid
} // namespace Elastos

#endif //__ELASTOS_DROID_VIEW_INPUTMETHOD_CINPUTMETHODMANAGERINPUTMETHODCALLBACK_H__
