
#ifndef __CINPUTMETHODMANAGERINPUTMETHODCALLBACK_H__
#define __CINPUTMETHODMANAGERINPUTMETHODCALLBACK_H__

#include "_CInputMethodManagerInputMethodCallback.h"

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

#endif //__CINPUTMETHODMANAGERINPUTMETHODCALLBACK_H__
