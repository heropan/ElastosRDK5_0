
#include "database/CCursorToBulkCursorAdaptor.h"
#include "database/CCrossProcessCursorWrapper.h"
#include "database/CCursorWindow.h"
#include <elastos/utility/logging/Slogger.h>

using Elastos::Utility::Logging::Slogger;

namespace Elastos {
namespace Droid {
namespace Database {

CCursorToBulkCursorAdaptor::ContentObserverProxy::ContentObserverProxy(
    /* [in] */ IIContentObserver* remoteObserver,
    /* [in] */ IProxyDeathRecipient* recipient)
    : ContentObserver(NULL)
    , mRemote(remoteObserver)
{
    // try {
    AutoPtr<IProxy> proxy = (IProxy*)mRemote->Probe(EIID_IProxy);
    if (proxy != NULL) {
        proxy->LinkToDeath(recipient, 0);
    }
    // } catch (RemoteException e) {
    //     // Do nothing, the far side is dead
    // }
}


ECode CCursorToBulkCursorAdaptor::ContentObserverProxy::UnlinkToDeath(
    /* [in] */ IProxyDeathRecipient* recipient,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result)
    AutoPtr<IProxy> proxy = (IProxy*)mRemote->Probe(EIID_IProxy);
    assert(proxy != NULL);
    return proxy->UnlinkToDeath(recipient, 0, result);
}

ECode CCursorToBulkCursorAdaptor::ContentObserverProxy::DeliverSelfNotifications(
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result)
    *result = FALSE;
    return NOERROR;
}

ECode CCursorToBulkCursorAdaptor::ContentObserverProxy::OnChange(
    /* [in] */ Boolean selfChange,
    /* [in] */ IUri* uri)
{
    // try {
    return mRemote->OnChange(selfChange, uri);
    // } catch (RemoteException ex) {
    //     // Do nothing, the far side is dead
    // }
}

const String CCursorToBulkCursorAdaptor::TAG("Cursor");

void CCursorToBulkCursorAdaptor::CloseFilledWindowLocked()
{
    if (mFilledWindow != NULL) {
        mFilledWindow->Close();
        mFilledWindow = NULL;
    }
}

void CCursorToBulkCursorAdaptor::DisposeLocked()
{
    if (mCursor != NULL) {
        UnregisterObserverProxyLocked();
        mCursor->Close();
        mCursor = NULL;
    }

    CloseFilledWindowLocked();
}

ECode CCursorToBulkCursorAdaptor::ThrowIfCursorIsClosed()
{
    if (mCursor == NULL) {
        //throw new StaleDataException("Attempted to access a cursor after it has been closed.");
        Slogger::E(TAG, "Attempted to access a cursor after it has been closed.");
        return E_STALE_DATA_EXCEPTION;
    }
    return NOERROR;
}

ECode CCursorToBulkCursorAdaptor::ProxyDied()
{
    AutoLock lock(_m_syncLock);
    DisposeLocked();
    return NOERROR;
}

ECode CCursorToBulkCursorAdaptor::GetBulkCursorDescriptor(
    /* [out] */ CBulkCursorDescriptor** result)
{
    VALIDATE_NOT_NULL(result);

    AutoLock lock(_m_syncLock);
    FAIL_RETURN(ThrowIfCursorIsClosed())

    AutoPtr<CBulkCursorDescriptor> d;
    CBulkCursorDescriptor::NewByFriend((CBulkCursorDescriptor**)&d);
    d->mCursor = (IBulkCursor*)this;
    mCursor->GetColumnNames((ArrayOf<String>**)&d->mColumnNames);
    mCursor->GetWantsAllOnMoveCalls(&d->mWantsAllOnMoveCalls);
    mCursor->GetCount(&d->mCount);
    mCursor->GetWindow((ICursorWindow**)&d->mWindow);
    if (d->mWindow != NULL) {
        // Acquire a reference to the window because its reference count will be
        // decremented when it is returned as part of the binder call reply parcel.
        d->mWindow->AcquireReference();
    }
    *result = d;
    REFCOUNT_ADD(*result)
    return NOERROR;
}

ECode CCursorToBulkCursorAdaptor::GetWindow(
    /* [in] */ Int32 position,
    /* [out] */ ICursorWindow** result)
{
    VALIDATE_NOT_NULL(result)
    *result = NULL;

    AutoLock lock(_m_syncLock);
    FAIL_RETURN(ThrowIfCursorIsClosed())

    Boolean isSuccess;
    if (mCursor->MoveToPosition(position, &isSuccess), !isSuccess) {
        CloseFilledWindowLocked();
        return NOERROR;
    }

    AutoPtr<ICursorWindow> window;
    mCursor->GetWindow((ICursorWindow**)&window);
    if (window != NULL) {
        CloseFilledWindowLocked();
    }
    else {
        window = mFilledWindow;
        if (window == NULL) {
            CCursorWindow::New(mProviderName, (ICursorWindow**)&mFilledWindow);
            window = mFilledWindow;
        }
        else {
            Int32 pos, rows;
            window->GetStartPosition(&pos);
            window->GetNumRows(&rows);
            if (position < pos || position >= pos + rows) {
                window->Clear();
            }
        }
        mCursor->FillWindow(position, window);
    }

    if (window != NULL) {
        // Acquire a reference to the window because its reference count will be
        // decremented when it is returned as part of the binder call reply parcel.
        window->AcquireReference();
    }
    *result = window;
    REFCOUNT_ADD(*result)
    return NOERROR;
}

ECode CCursorToBulkCursorAdaptor::OnMove(
    /* [in] */ Int32 position)
{
    AutoLock lock(_m_syncLock);
    FAIL_RETURN(ThrowIfCursorIsClosed())
    Int32 mPosition;
    mCursor->GetPosition(&mPosition);
    Boolean isSuccess;
    return mCursor->OnMove(mPosition, position, &isSuccess);
}

ECode CCursorToBulkCursorAdaptor::Deactivate()
{
    AutoLock lock(_m_syncLock);
    if (mCursor != NULL) {
        UnregisterObserverProxyLocked();
        mCursor->Deactivate();
    }

    CloseFilledWindowLocked();
    return NOERROR;
}

ECode CCursorToBulkCursorAdaptor::Close()
{
    AutoLock lock(_m_syncLock);
    DisposeLocked();
    return NOERROR;
}

ECode CCursorToBulkCursorAdaptor::Requery(
    /* [in] */ IIContentObserver* observer,
    /* [out] */ Int32* result)
{
    VALIDATE_NOT_NULL(result)

    AutoLock lock(_m_syncLock);

    FAIL_RETURN(ThrowIfCursorIsClosed())

    CloseFilledWindowLocked();

    //try {
    Boolean isSuccess;
    ECode ec = mCursor->Requery(&isSuccess);
    if (ec == (ECode)E_ILLEGAL_STATE_EXCEPTION) {
        Boolean isClosed;
        mCursor->IsClosed(&isClosed);
        Slogger::E(TAG, "%s Requery misuse db, mCursor isClosed:%d, 0x%08x", mProviderName.string(), isClosed, ec);
        return ec;
    }
    if (!isSuccess) {
        *result = -1;
        return NOERROR;
    }
    //} catch (IllegalStateException e) {
        // IllegalStateException leakProgram = new IllegalStateException(
        //         mProviderName + " Requery misuse db, mCursor isClosed:" +
        //         mCursor.isClosed(), e);
        // throw leakProgram;
    //}
    UnregisterObserverProxyLocked();
    CreateAndRegisterObserverProxyLocked(observer);
    return mCursor->GetCount(result);
}

ECode CCursorToBulkCursorAdaptor::CreateAndRegisterObserverProxyLocked(
    /* [in] */ IIContentObserver* observer)
{
    if (mObserver != NULL) {
        //throw new IllegalStateException("an observer is already registered");
        Slogger::E(TAG, "an observer is already registered");
        return E_ILLEGAL_STATE_EXCEPTION;
    }
    mObserver = new ContentObserverProxy(observer, (IProxyDeathRecipient*)this->Probe(EIID_IProxyDeathRecipient));
    return mCursor->RegisterContentObserver((IContentObserver*)mObserver);
}

ECode CCursorToBulkCursorAdaptor::UnregisterObserverProxyLocked()
{
    if (mObserver != NULL) {
        mCursor->UnregisterContentObserver(mObserver);
        Boolean result;
        mObserver->UnlinkToDeath(THIS_PROBE(IProxyDeathRecipient), &result);
        mObserver = NULL;
    }
    return NOERROR;
}

ECode CCursorToBulkCursorAdaptor::GetExtras(
    /* [out] */ IBundle** result)
{
    AutoLock lock(_m_syncLock);

    FAIL_RETURN(ThrowIfCursorIsClosed())
    return mCursor->GetExtras(result);
}

ECode CCursorToBulkCursorAdaptor::Respond(
    /* [in] */ IBundle* extras,
    /* [out] */ IBundle** result)
{
    AutoLock lock(_m_syncLock);
    FAIL_RETURN(ThrowIfCursorIsClosed())
    return mCursor->Respond(extras, result);
}

ECode CCursorToBulkCursorAdaptor::ToString(
    /* [out] */ String* str)
{
    VALIDATE_NOT_NULL(str);
    *str = String(NULL);
    return NOERROR;
}

ECode CCursorToBulkCursorAdaptor::constructor(
    /* [in] */ ICursor* cursor,
    /* [in] */ IIContentObserver* observer,
    /* [in] */ const String& providerName)
{
    if (ICrossProcessCursor::Probe(cursor) != NULL) {
        mCursor = ICrossProcessCursor::Probe(cursor);
    }
    else {
        CCrossProcessCursorWrapper::New(cursor, (ICrossProcessCursorWrapper**)&mCursor);
    }
    mProviderName = providerName;

    AutoLock lock(_m_syncLock);
    return CreateAndRegisterObserverProxyLocked(observer);
}

} //Database
} //Droid
} //Elastos
