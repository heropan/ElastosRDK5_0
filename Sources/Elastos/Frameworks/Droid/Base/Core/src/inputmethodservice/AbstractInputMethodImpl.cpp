
#ifdef DROID_CORE
#include "inputmethodservice/AbstractInputMethodImpl.h"
#include "inputmethodservice/AbstractInputMethodSessionImpl.h"
#else
#include "inputmethodservice/AbstractInputMethodImpl.h"
#include "inputmethodservice/AbstractInputMethodSessionImpl.h"
#endif

using Elastos::Droid::View::InputMethod::EIID_IInputMethod;
using Elastos::Droid::View::InputMethod::EIID_IInputMethodSession;

namespace Elastos {
namespace Droid {
namespace InputMethodService {


AbstractInputMethodImpl::AbstractInputMethodImpl(
    /* [in] */ AbstractInputMethodService* host)
    : mHost(host)
{}

ECode AbstractInputMethodImpl::GetWeakReference(
    /* [out] */ IWeakReference** weakReference)
{
    VALIDATE_NOT_NULL(weakReference)
    *weakReference = new WeakReferenceImpl(Probe(EIID_IInterface), CreateWeak(this));
    REFCOUNT_ADD(*weakReference)
    return NOERROR;
}

UInt32 AbstractInputMethodImpl::AddRef()
{
    return ElRefBase::AddRef();
}

UInt32 AbstractInputMethodImpl::Release()
{
    return ElRefBase::Release();
}

PInterface AbstractInputMethodImpl::Probe(
    /* [in] */ REIID riid)
{
    if (riid == EIID_IInterface) {
        return (IAbstractInputMethodImpl*)this;
    }
    else if (riid == EIID_IInputMethod) {
        return (IInputMethod*)this;
    }
    else if (riid == EIID_IAbstractInputMethodImpl) {
        return (IAbstractInputMethodImpl*)this;
    }
    else if (riid == EIID_IWeakReferenceSource) {
        return (IWeakReferenceSource*)this;
    }

    return NULL;
}

ECode AbstractInputMethodImpl::GetInterfaceID(
    /* [in] */ IInterface *pObject,
    /* [out] */ InterfaceID *pIID)
{
    if (pIID == NULL) {
        return E_INVALID_ARGUMENT;
    }
    assert(0);
    return NOERROR;
}

ECode AbstractInputMethodImpl::CreateSession(
    /* [in] */ IInputMethodSessionEventCallback* callback)
{
    assert(mHost != NULL);
    AutoPtr<IAbstractInputMethodSessionImpl> impl;
    mHost->OnCreateInputMethodSessionInterface((IAbstractInputMethodSessionImpl**)&impl);
    return callback->SessionCreated(
            (IInputMethodSession*)impl->Probe(EIID_IInputMethodSession));
}

ECode AbstractInputMethodImpl::SetSessionEnabled(
    /* [in] */ IInputMethodSession* session,
    /* [in] */ Boolean enabled)
{
    return ((AbstractInputMethodSessionImpl*)session)->SetEnabled(enabled);
}

ECode AbstractInputMethodImpl::RevokeSession(
    /* [in] */ IInputMethodSession* session)
{
    return ((AbstractInputMethodSessionImpl*)session)->RevokeSelf();
}

} // namespace InputMethodService
} // namespace Droid
} // namespace Elastos
