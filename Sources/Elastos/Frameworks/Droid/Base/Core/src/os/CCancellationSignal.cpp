#include "os/CCancellationSignal.h"

namespace Elastos {
namespace Droid {
namespace Os {

// b4be01c2-5eff-4962-bad3-6b09feb1e384
extern "C" const InterfaceID EIID_Transport =
        { 0xb4be01c2, 0x5eff, 0x4962, { 0xba, 0xd3, 0x6b, 0x09, 0xfe, 0xb1, 0xe3, 0x84 } };

//===============================================================================
//                  CCancellationSignal::Transport
//===============================================================================
UInt32 CCancellationSignal::Transport::AddRef()
{
    return ElRefBase::AddRef();
}

UInt32 CCancellationSignal::Transport::Release()
{
    return ElRefBase::Release();
}

PInterface CCancellationSignal::Transport::Probe(
    /* [in]  */ REIID riid)
{
    if (riid == EIID_IInterface) {
        return (PInterface)(IICancellationSignal*)this;
    }
    else if (riid == EIID_IICancellationSignal) {
        return (PInterface)(IICancellationSignal*)this;
    }
    else if (riid == EIID_Transport) {
        return reinterpret_cast<PInterface>(this);
    }

    return NULL;
}

ECode CCancellationSignal::Transport::GetInterfaceID(
    /* [in] */ IInterface *pObject,
    /* [out] */ InterfaceID *pIID)
{
    VALIDATE_NOT_NULL(pIID);

    if (pObject == (IInterface*)(IICancellationSignal*)this) {
        *pIID = EIID_IICancellationSignal;
    }
    return NOERROR;
}

CCancellationSignal::Transport::Transport()
{
    CCancellationSignal::New((ICancellationSignal**)&mCancellationSignal);
}

ECode CCancellationSignal::Transport::Cancel()
{
    return mCancellationSignal->Cancel();;
}

//===============================================================================
//                  CCancellationSignal
//===============================================================================
CCancellationSignal::CCancellationSignal()
    : mIsCanceled(FALSE)
    , mCancelInProgress(FALSE)
{
    pthread_cond_init(&mCond, NULL);
    pthread_mutex_init(&mMutex, NULL);
}

CCancellationSignal::~CCancellationSignal()
{
    pthread_cond_destroy(&mCond);
    pthread_mutex_destroy(&mMutex);
}

ECode CCancellationSignal::constructor()
{
    return NOERROR;
}

ECode CCancellationSignal::IsCanceled(
    /* [out] */ Boolean* result)
{
    Autolock lock(&mMutex);
    *result = mIsCanceled;
    return NOERROR;
}

ECode CCancellationSignal::ThrowIfCanceled()
{
    Boolean isCanceled;
    if (IsCanceled(&isCanceled), isCanceled) {
        return E_OPERATION_CANCELED_EXCEPTION;
    }
    return NOERROR;
}

ECode CCancellationSignal::Cancel()
{
    AutoPtr<ICancellationSignalOnCancelListener> listener;
    AutoPtr<IICancellationSignal> remote;
    {
        Autolock lock(&mMutex);
        if (mIsCanceled) {
            return NOERROR;
        }

        mIsCanceled = TRUE;
        mCancelInProgress = TRUE;
        listener = mOnCancelListener;
        remote = mRemote;
    }

    // try {
    if (listener != NULL) {
        listener->OnCancel();
    }
    if (remote != NULL) {
        // try {
        remote->Cancel();
        // } catch (RemoteException ex) {
        // }
    }
    // } finally {
    {
        Autolock lock(&mMutex);
        mCancelInProgress = FALSE;
        // NotifyAll();
        pthread_cond_signal(&mCond);
    }
    // }
    return NOERROR;
}

ECode CCancellationSignal::SetOnCancelListener(
    /* [in] */ ICancellationSignalOnCancelListener* listener)
{
    {
        Autolock lock(&mMutex);
        WaitForCancelFinishedLocked();

        if (mOnCancelListener.Get() == listener) {
            return NOERROR;
        }

        mOnCancelListener = listener;
        if (!mIsCanceled || listener == NULL) {
            return NOERROR;
        }
    }

    listener->OnCancel();
    return NOERROR;
}

ECode CCancellationSignal::SetRemote(
    /* [in] */ IICancellationSignal* remote)
{
    {
        Autolock lock(&mMutex);
        WaitForCancelFinishedLocked();

        if (mRemote.Get() == remote) {
            return NOERROR;
        }

        mRemote = remote;
        if (!mIsCanceled || remote == NULL) {
            return NOERROR;
        }
    }

    // try {
    remote->Cancel();
    // } catch (RemoteException ex) {
    // }
    return NOERROR;
}

void CCancellationSignal::WaitForCancelFinishedLocked()
{
    while (mCancelInProgress) {
        // try {
        pthread_cond_wait(&mCond, &mMutex);
        //Wait();
        // } catch (InterruptedException ex) {
        // }
    }
}

AutoPtr<IICancellationSignal> CCancellationSignal::CreateTransport()
{
    AutoPtr<IICancellationSignal> transport = new Transport();
    return transport;
}

AutoPtr<ICancellationSignal> CCancellationSignal::FromTransport(
    /* [in] */ IICancellationSignal* cancellationSignal)
{
    if (cancellationSignal != NULL) {
        Transport* transport = reinterpret_cast<Transport*>(cancellationSignal->Probe(EIID_Transport));
        if (transport) {
            return transport->mCancellationSignal;
        }
    }

    return NULL;
}


} // namespace Os
} // namespace Droid
} // namespace Elastos
