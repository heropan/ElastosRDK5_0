
#ifndef  __CIINPUTMETHODCLIENT_H__
#define  __CIINPUTMETHODCLIENT_H__

#include "_CIInputMethodClient.h"
#include "view/inputmethod/CInputMethodManager.h"



namespace Elastos {
namespace Droid {
namespace View {
namespace InputMethod {


CarClass(CIInputMethodClient)
{
public:
    CARAPI constructor(
        /* [in] */ IInputMethodManager* host);

    CARAPI SetUsingInputMethod(
        /* [in] */ Boolean state);

    CARAPI OnBindMethod(
        /* [in] */ IInputBindResult* res);

    CARAPI OnUnbindMethod(
        /* [in] */ Int32 sequence);

    CARAPI SetActive(
        /* [in] */ Boolean active);

    CARAPI ToString(
        /* [out] */ String* str);

private:
    AutoPtr<CInputMethodManager> mHost;
};

} // namespace InputMethod
} // namespace View
} // namespace Droid
} // namespace Elastos

#endif  //__CIINPUTMETHODCLIENT_H__
