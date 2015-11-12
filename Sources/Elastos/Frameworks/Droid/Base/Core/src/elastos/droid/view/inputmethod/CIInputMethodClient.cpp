
#include "elastos/droid/view/inputmethod/CIInputMethodClient.h"

namespace Elastos {
namespace Droid {
namespace View {
namespace InputMethod {

ECode CIInputMethodClient::constructor(
    /* [in] */ IInputMethodManager* host)
{
    mHost = (CInputMethodManager*)host;
    return NOERROR;
}

ECode CIInputMethodClient::SetUsingInputMethod(
    /* [in] */ Boolean state)
{
    return NOERROR;
}

ECode CIInputMethodClient::OnBindMethod(
    /* [in] */ IInputBindResult* res)
{
    AutoPtr<IMessage> msg;
    mHost->mH->ObtainMessage(CInputMethodManager::MSG_BIND,
        res, (IMessage**)&msg);
    Boolean result;
    return mHost->mH->SendMessage(msg, &result);
}

ECode CIInputMethodClient::OnUnbindMethod(
    /* [in] */ Int32 sequence)
{
    AutoPtr<IMessage> msg;
    mHost->mH->ObtainMessage(CInputMethodManager::MSG_UNBIND,
        sequence, 0, (IMessage**)&msg);
    Boolean result;
    return mHost->mH->SendMessage(msg, &result);
}

ECode CIInputMethodClient::SetActive(
    /* [in] */ Boolean active)
{
    AutoPtr<IMessage> msg;
    mHost->mH->ObtainMessage(CInputMethodManager::MSG_SET_ACTIVE,
        active ? 1 : 0, 0, (IMessage**)&msg);
    Boolean result;
    return mHost->mH->SendMessage(msg, &result);
}

ECode CIInputMethodClient::ToString(
    /* [out] */ String* str)
{
    return E_NOT_IMPLEMENTED;
}

@Override
public void setUserActionNotificationSequenceNumber(int sequenceNumber) {
    mH.sendMessage(mH.obtainMessage(MSG_SET_USER_ACTION_NOTIFICATION_SEQUENCE_NUMBER,
            sequenceNumber, 0));
}

} // namespace InputMethod
} // namespace View
} // namespace Droid
} // namespace Elastos
