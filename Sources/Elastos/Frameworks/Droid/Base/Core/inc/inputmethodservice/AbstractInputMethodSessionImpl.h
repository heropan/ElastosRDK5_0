
#ifndef  __ABSTRACTINPUTMETHODSESSIONIMPL_H__
#define  __ABSTRACTINPUTMETHODSESSIONIMPL_H__

#ifdef DROID_CORE
#include "inputmethodservice/AbstractInputMethodService.h"
#else
#include "AbstractInputMethodService.h"
#endif


using Elastos::Droid::View::IKeyEvent;
using Elastos::Droid::View::IMotionEvent;
using Elastos::Droid::View::InputMethod::ILocalInputMethodSessionEventCallback;

namespace Elastos {
namespace Droid {
namespace InputMethodService {


/**
 * Base class for derived classes to implement their {@link InputMethodSession}
 * interface.  This takes care of basic maintenance of the session,
 * but most behavior must be implemented in a derived class.
 */
class AbstractInputMethodSessionImpl
    : public ElRefBase
    , public IAbstractInputMethodSessionImpl
{
public:
    AbstractInputMethodSessionImpl(
        /* [in] */ AbstractInputMethodService* host);

    CARAPI_(PInterface) Probe(
        /* [in] */ REIID riid);

    CARAPI GetInterfaceID(
        /* [in] */ IInterface *pObject,
        /* [out] */ InterfaceID *pIID);

    /**
     * Check whether this session has been enabled by the system.  If not
     * enabled, you should not execute any calls on to it.
     */
    CARAPI IsEnabled(
        /* [out] */ Boolean* enabled);

    /**
     * Check whether this session has been revoked by the system.  Revoked
     * session is also always disabled, so there is generally no need to
     * explicitly check for this.
     */
    CARAPI IsRevoked(
        /* [out] */ Boolean* revoked);

    /**
     * Change the enabled state of the session.  This only works if the
     * session has not been revoked.
     */
    CARAPI SetEnabled(
        /* [in] */ Boolean enabled);

    /**
     * Revoke the session from the client.  This disabled the session, and
     * prevents it from ever being enabled again.
     */
    CARAPI RevokeSelf();

    /**
     * Take care of dispatching incoming key events to the appropriate
     * callbacks on the service, and tell the client when this is done.
     */
    CARAPI DispatchKeyEvent(
        /* [in] */ Int32 seq,
        /* [in] */ IKeyEvent* event,
        /* [in] */ ILocalInputMethodSessionEventCallback* callback);

    /**
     * Take care of dispatching incoming trackball events to the appropriate
     * callbacks on the service, and tell the client when this is done.
     */
    CARAPI DispatchTrackballEvent(
        /* [in] */ Int32 seq,
        /* [in] */ IMotionEvent* event,
        /* [in] */ ILocalInputMethodSessionEventCallback* callback);

    CARAPI DispatchGenericMotionEvent(
        /* [in] */ Int32 seq,
        /* [in] */ IMotionEvent* event,
        /* [in] */ ILocalInputMethodSessionEventCallback* callback);

private:
    Boolean mEnabled;
    Boolean mRevoked;

protected:
    AbstractInputMethodService* mAbstractHost;
};

} // namespace InputMethodService
} // namespace Droid
} // namespace Elastos

#endif  //__ABSTRACTINPUTMETHODSESSIONIMPL_H__
