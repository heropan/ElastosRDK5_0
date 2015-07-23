
#include "os/CMessengerImpl.h"
#include "os/Binder.h"

namespace Elastos {
namespace Droid {
namespace Os {

CAR_INTERFACE_IMPL(CMessengerImpl, Object, IIMessenger)

CAR_OBJECT_IMPL(CMessengerImpl)

ECode CMessengerImpl::constructor(
    /* [in] */ IHandler* handler)
{
    mOwner = handler;
    return NOERROR;
}

ECode CMessengerImpl::Send(
    /* [in] */ IMessage* message)
{
    Boolean result;
    Int32 uid = Binder::GetCallingUid();
    message->SetSendingUid(uid);
    return mOwner->SendMessage(message, &result);
}

} // namespace Os
} // namespace Droid
} // namespace Elastos
