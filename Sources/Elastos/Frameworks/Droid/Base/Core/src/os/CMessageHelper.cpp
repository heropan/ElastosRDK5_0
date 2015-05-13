
#include "ext/frameworkdef.h"
#include "os/CMessageHelper.h"
#include "os/CMessage.h"

namespace Elastos {
namespace Droid {
namespace Os {

ECode CMessageHelper::Obtain(
    /* [out] */ IMessage** msg)
{
    VALIDATE_NOT_NULL(msg);
    AutoPtr<IMessage> m = CMessage::Obtain();
    *msg = m;
    INTERFACE_ADDREF(*msg);
    return NOERROR;
}

ECode CMessageHelper::ObtainEx(
    /* [in] */ IMessage* orig,
    /* [out] */ IMessage** msg)
{
    VALIDATE_NOT_NULL(msg);
    AutoPtr<IMessage> m = CMessage::Obtain(orig);
    *msg = m;
    INTERFACE_ADDREF(*msg);
    return NOERROR;
}

ECode CMessageHelper::ObtainEx2(
    /* [in] */ IHandler* target,
    /* [out] */ IMessage** msg)
{
    VALIDATE_NOT_NULL(msg);
    AutoPtr<IMessage> m = CMessage::Obtain(target);
    *msg = m;
    INTERFACE_ADDREF(*msg);
    return NOERROR;
}

ECode CMessageHelper::ObtainEx3(
    /* [in] */ IHandler* target,
    /* [in] */ Int32 what,
    /* [out] */ IMessage** msg)
{
    VALIDATE_NOT_NULL(msg);
    AutoPtr<IMessage> m = CMessage::Obtain(target, what);
    *msg = m;
    INTERFACE_ADDREF(*msg);
    return NOERROR;
}

ECode CMessageHelper::ObtainEx4(
    /* [in] */ IHandler* target,
    /* [in] */ Int32 what,
    /* [in] */ IInterface* obj,
    /* [out] */ IMessage** msg)
{
    VALIDATE_NOT_NULL(msg);
    AutoPtr<IMessage> m = CMessage::Obtain(target, what, obj);
    *msg = m;
    INTERFACE_ADDREF(*msg);
    return NOERROR;
}

ECode CMessageHelper::ObtainEx5(
    /* [in] */ IHandler* target,
    /* [in] */ Int32 what,
    /* [in] */ Int32 arg1,
    /* [in] */ Int32 arg2,
    /* [out] */ IMessage** msg)
{
    VALIDATE_NOT_NULL(msg);
    AutoPtr<IMessage> m = CMessage::Obtain(target, what, arg1, arg2);
    *msg = m;
    INTERFACE_ADDREF(*msg);
    return NOERROR;
}

ECode CMessageHelper::ObtainEx6(
    /* [in] */ IHandler* target,
    /* [in] */ Int32 what,
    /* [in] */ Int32 arg1,
    /* [in] */ Int32 arg2,
    /* [in] */ IInterface* obj,
    /* [out] */ IMessage** msg)
{
    VALIDATE_NOT_NULL(msg);
    AutoPtr<IMessage> m = CMessage::Obtain(target, what, arg1, arg2, obj);
    *msg = m;
    INTERFACE_ADDREF(*msg);
    return NOERROR;
}

ECode CMessageHelper::ObtainEx7(
    /* [in] */ IHandler* target,
    /* [in] */ IRunnable* callback,
    /* [out] */ IMessage** msg)
{
    VALIDATE_NOT_NULL(msg);
    AutoPtr<IMessage> m = CMessage::Obtain(target, callback);
    *msg = m;
    INTERFACE_ADDREF(*msg);
    return NOERROR;
}

} // namespace Os
} // namespace Droid
} // namespace Elastos
