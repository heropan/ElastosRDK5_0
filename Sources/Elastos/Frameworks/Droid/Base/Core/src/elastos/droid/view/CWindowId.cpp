
#include "elastos/droid/view/CWindowId.h"
#include "elastos/droid/view/WindowIdFocusObserver.h"
#include <elastos/core/AutoLock.h>
#include <elastos/core/StringBuilder.h>
#include <elastos/core/StringUtils.h>

using Elastos::Core::ISystem;
using Elastos::Core::CSystem;
using Elastos::Core::StringBuilder;
using Elastos::Core::StringUtils;

namespace Elastos {
namespace Droid {
namespace View {

CAR_OBJECT_IMPL(CWindowId);
CAR_INTERFACE_IMPL_2(CWindowId, Object, IWindowId, IParcelable);
ECode CWindowId::constructor()
{
    return NOERROR;
}

ECode CWindowId::IsFocused(
    /* [out] */ Boolean* focused)
{
    VALIDATE_NOT_NULL(focused);
    if (mToken->IsFocused(focused) == E_REMOTE_EXCEPTION) {
        *focused = FALSE;
    }
    return NOERROR;
}

ECode CWindowId::RegisterFocusObserver(
    /* [in] */ IWindowIdFocusObserver* _observer)
{
    WindowIdFocusObserver* observer = (WindowIdFocusObserver*)_observer;
    AutoPtr<IHashMap> map = observer->mRegistrations;
    synchronized (map) {
        Boolean contains = FALSE;
        if (map->ContainsKey(IBinder::Probe(mToken), &contains), contains) {
            // throw new IllegalStateException(
            //         "Focus observer already registered with input token");
            return E_ILLEGAL_STATE_EXCEPTION;
        }
        map->Put(IBinder::Probe(mToken), THIS_PROBE(IWindowId));
        // try {
        mToken->RegisterFocusObserver(observer->mIObserver);
        // } catch (RemoteException e) {
        // }
    }
    return NOERROR;
}

ECode CWindowId::UnregisterFocusObserver(
    /* [in] */ IWindowIdFocusObserver* _observer)
{
    WindowIdFocusObserver* observer = (WindowIdFocusObserver*)_observer;
    AutoPtr<IHashMap> map = observer->mRegistrations;
    synchronized (map) {
        AutoPtr<IInterface> obj;
        map->Remove(IBinder::Probe(mToken), (IInterface**)&obj);
        if (obj.Get() == NULL) {
            // throw new IllegalStateException("Focus observer not registered with input token");
            return E_ILLEGAL_STATE_EXCEPTION;
        }
        // try {
        mToken->UnregisterFocusObserver(observer->mIObserver);
        // } catch (RemoteException e) {
        // }
    }
    return NOERROR;
}

ECode CWindowId::Equals(
    /* [in] */ IInterface* otherObj,
    /* [out] */ Boolean* e)
{
    VALIDATE_NOT_NULL(e);
    if (IWindowId::Probe(otherObj)) {
        return IObject::Probe(mToken)->Equals(IBinder::Probe(((CWindowId*)(IWindowId*)otherObj)->mToken), e);
    }
    *e = FALSE;
    return NOERROR;
}

ECode CWindowId::GetHashCode(
    /* [out] */ Int32* code)
{
    VALIDATE_NOT_NULL(code);
    return IObject::Probe(mToken)->GetHashCode(code);
}

ECode CWindowId::ToString(
    /* [out] */ String* str)
{
    StringBuilder sb;
    sb.Append("IntentSender{");
    AutoPtr<ISystem> system;
    CSystem::AcquireSingleton((ISystem**)&system);
    Int32 code = 0;
    system->IdentityHashCode(THIS_PROBE(IInterface), &code);
    sb.Append(StringUtils::ToHexString(code));
    sb.Append(": ");
    if (mToken != NULL) {
        sb.Append(mToken.Get());
    }
    else {
        sb.Append("NULL");
    }
    sb.AppendChar('}');
    return sb.ToString(str);
}

ECode CWindowId::WriteToParcel(
    /* [in] */ IParcel* out)
{
    IBinder* binder = IBinder::Probe(mToken);
    return out->WriteInterfacePtr((IInterface*)binder);
}

ECode CWindowId::ReadFromParcel(
    /* [in] */ IParcel* source)
{
    return source->ReadInterfacePtr((Handle32*)&mToken);
}

ECode CWindowId::GetTarget(
    /* [out] */ IIWindowId** id)
{
    VALIDATE_NOT_NULL(id);
    *id = mToken;
    REFCOUNT_ADD(*id);
    return NOERROR;
}

ECode CWindowId::constructor(
    /* [in] */ IIWindowId* target)
{
    mToken = target;
    return NOERROR;
}

ECode CWindowId::constructor(
    /* [in] */ IBinder* target)
{
    mToken = IIWindowId::Probe(target);
    return NOERROR;
}


} // namespace View
} // namepsace Droid
} // namespace Elastos
