
#ifndef  __ELASTOS_DROID_INPUTMETHODSERVICE_ABSTRACTINPUTMETHODIMPL_H__
#define  __ELASTOS_DROID_INPUTMETHODSERVICE_ABSTRACTINPUTMETHODIMPL_H__

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
    : public Object
    , public IAbstractInputMethodImpl
    , public IWeakReferenceSource
{
public:
    CAR_INTERFACE_DECL();

    AbstractInputMethodImpl(
        /* [in] */ AbstractInputMethodService* host);

    CARAPI GetWeakReference(
        /* [out] */ IWeakReference** weakReference);

    CARAPI CreateSession(
        /* [in] */ IInputMethodSessionEventCallback* callback);

    CARAPI SetSessionEnabled(
        /* [in] */ IInputMethodSession* session,
        /* [in] */ Boolean enabled);

    CARAPI RevokeSession(
        /* [in] */ IInputMethodSession* session);

    CARAPI ToString(
        /* [out] */ String* info);

private:
    AbstractInputMethodService* mHost;
};

} // namespace InputMethodService
} // namespace Droid
} // namespace Elastos

#endif  //__ELASTOS_DROID_INPUTMETHODSERVICE_ABSTRACTINPUTMETHODIMPL_H__
