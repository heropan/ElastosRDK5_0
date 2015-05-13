
#ifndef  __ABSTRACTINPUTMETHODIMPL_H__
#define  __ABSTRACTINPUTMETHODIMPL_H__

#ifdef DROID_CORE
#include "inputmethodservice/AbstractInputMethodService.h"
#else
#include "AbstractInputMethodService.h"
#endif

using Elastos::Droid::InputMethodService::AbstractInputMethodService;
using Elastos::Droid::InputMethodService::IAbstractInputMethodImpl;
using Elastos::Droid::View::InputMethod::IInputMethodSession;
using Elastos::Droid::View::InputMethod::IInputMethodSessionEventCallback;

namespace Elastos {
namespace Droid {
namespace InputMethodService {

class AbstractInputMethodImpl
    : public ElRefBase
    , public IAbstractInputMethodImpl
    , public IWeakReferenceSource
{
public:
    AbstractInputMethodImpl(
        /* [in] */ AbstractInputMethodService* host);

    CARAPI GetWeakReference(
        /* [out] */ IWeakReference** weakReference);

    CARAPI_(UInt32) AddRef();

    CARAPI_(UInt32) Release();

    CARAPI_(PInterface) Probe(
        /* [in] */ REIID riid);

    CARAPI GetInterfaceID(
        /* [in] */ IInterface *pObject,
        /* [out] */ InterfaceID *pIID);

    CARAPI CreateSession(
        /* [in] */ IInputMethodSessionEventCallback* callback);

    CARAPI SetSessionEnabled(
        /* [in] */ IInputMethodSession* session,
        /* [in] */ Boolean enabled);

    CARAPI RevokeSession(
        /* [in] */ IInputMethodSession* session);

private:
    AbstractInputMethodService* mHost;
};

} // namespace InputMethodService
} // namespace Droid
} // namespace Elastos

#endif  //__ABSTRACTINPUTMETHODIMPL_H__
