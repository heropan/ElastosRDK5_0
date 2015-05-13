
#include "ext/frameworkext.h"
#include "os/CMessenger.h"

namespace Elastos {
namespace Droid {
namespace Os {

ECode CMessenger::constructor()
{
    return NOERROR;
}

ECode CMessenger::constructor(
    /* [in] */ IHandler* handler)
{
    return handler->GetIMessenger((IIMessenger**)&mTarget);
}

ECode CMessenger::constructor(
    /* [in] */ IIMessenger* target)
{
    mTarget = target;
    return NOERROR;
}

ECode CMessenger::Send(
    /* [in] */ IMessage* message)
{
    return mTarget->Send(message);
}

ECode CMessenger::ReadFromParcel(
    /* [in] */ IParcel* source)
{
    return source->ReadInterfacePtr((Handle32*)&mTarget);
}

ECode CMessenger::WriteToParcel(
    /* [in] */ IParcel* dest)
{
    return dest->WriteInterfacePtr(mTarget.Get());
}

ECode CMessenger::GetIMessenger(
    /* [in] */ IIMessenger** mgr)
{
    VALIDATE_NOT_NULL(mgr);
    *mgr = mTarget;
    INTERFACE_ADDREF(*mgr);
    return NOERROR;
}

ECode CMessenger::GetHashCode(
    /* [out] */ Int32* hashcode)
{
    VALIDATE_NOT_NULL(hashcode);
    *hashcode = (Int32)mTarget.Get();
    return NOERROR;
}

ECode CMessenger::EqualsEx(
    /* [in] */ IMessenger* other,
    /* [out] */ Boolean* equals)
{
    VALIDATE_NOT_NULL(equals);
    *equals = FALSE;

    if (other != NULL) {
        AutoPtr<IIMessenger> msgr;
        other->GetIMessenger((IIMessenger**)&msgr);
        *equals = (mTarget.Get() == msgr.Get());
    }
    return NOERROR;
}

ECode CMessenger::Equals(
    /* [in] */ IInterface* other,
    /* [out] */ Boolean* equals)
{
    VALIDATE_NOT_NULL(equals);
    return EqualsEx(IMessenger::Probe(other), equals);
}

} // namespace Os
} // namespace Droid
} // namespace Elastos
