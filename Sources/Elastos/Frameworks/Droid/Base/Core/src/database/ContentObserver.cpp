
#include "database/ContentObserver.h"
#ifdef DROID_CORE
#include "database/CContentObserverTransport.h"
#else
#include "Elastos.Droid.Core.h"
#endif
#include "os/Handler.h"
#include <elastos/core/AutoLock.h>

namespace Elastos {
namespace Droid {
namespace Database {

ContentObserver::NotificationRunnable::NotificationRunnable(
    /* [in] */ Boolean selfChange,
    /* [in] */ IUri* uri,
    /* [in] */ ContentObserver* owner)
    : mSelfChange(selfChange)
    , mUri(uri)
    , mOwner(owner)
{
}

ECode ContentObserver::NotificationRunnable::Run()
{
    return mOwner->OnChange(mSelfChange, mUri);
}

ContentObserver::ContentObserver(
    /* [in]  */ IHandler* handler)
    : mHandler(handler)
{}

ECode ContentObserver::Init(
    /* [in]  */ IHandler* handler)
{
    mHandler = handler;
    return NOERROR;
}

CAR_INTERFACE_IMPL(ContentObserver, Object, IContentObserver)

ECode ContentObserver::GetContentObserver(
    /* [out] */ IIContentObserver** contentObserver)
{
    VALIDATE_NOT_NULL(contentObserver)

    synchronized(mLock) {
        if (mTransport == NULL) {
            CContentObserverTransport::New((IContentObserver*)this->Probe(EIID_IContentObserver), (IContentObserverTransport**)&mTransport);
        }
        *contentObserver = IIContentObserver::Probe(mTransport);
        REFCOUNT_ADD(*contentObserver)
    }
    return NOERROR;
}

ECode ContentObserver::ReleaseContentObserver(
    /* [out] */ IIContentObserver** contentObserver)
{
    VALIDATE_NOT_NULL(contentObserver)

    synchronized(mLock) {
        AutoPtr<IContentObserverTransport> oldTransport = mTransport;
        if (oldTransport != NULL) {
            oldTransport->ReleaseContentObserver();
            mTransport = NULL;
        }
        *contentObserver = IIContentObserver::Probe(oldTransport);
        REFCOUNT_ADD(*contentObserver)
    }
    return NOERROR;
}

ECode ContentObserver::DeliverSelfNotifications(
    /* [out] */ Boolean* res)
{
    VALIDATE_NOT_NULL(res);
    *res = FALSE;
    return NOERROR;
}

ECode ContentObserver::OnChange(
    /* [in] */ Boolean selfChange)
{
    return NOERROR;
}

ECode ContentObserver::OnChange(
    /* [in] */ Boolean selfChange,
    /* [in] */ IUri* uri)
{
    return OnChange(selfChange);
}

ECode ContentObserver::DispatchChange(
    /* [in] */ Boolean selfChange)
{
    return DispatchChange(selfChange, NULL);
}

ECode ContentObserver::DispatchChange(
    /* [in] */ Boolean selfChange,
    /* [in] */ IUri* uri)
{
    if (mHandler == NULL) {
        OnChange(selfChange, uri);
    }
    else {
        Boolean res;
        AutoPtr<IRunnable> run = new NotificationRunnable(selfChange, uri, this);
        mHandler->Post(run, &res);
    }
    return NOERROR;
}

} //Database
} //Droid
} //Elastos
