
#include "content/CContentQueryMap.h"
#include "content/CContentValues.h"
#include "os/CHandler.h"

using Elastos::Droid::Os::CHandler;

namespace Elastos {
namespace Droid {
namespace Content {

using Elastos::Utility::IArrayList;
using Elastos::Utility::CArrayList;
using Elastos::Utility::IObservable;
using Elastos::Utility::EIID_IObservable;
using Elastos::Utility::CObjectStringMap;
using Elastos::Core::CStringWrapper;
using Elastos::Core::ICharSequence;

CContentQueryMap::KeepUpdatedContentObserver::KeepUpdatedContentObserver(
    /* [in] */ IHandler* handler,
    /* [in] */ IWeakReference* host)
    : ContentObserver(handler)
    , mHost(host)
{
}

// @Override
ECode CContentQueryMap::KeepUpdatedContentObserver::OnChange(
    /* [in] */ Boolean selfChange)
{
    // If anyone is listening, we need to do this now to broadcast
    // to the observers.  Otherwise, we'll just set mDirty and
    // let it query lazily when they ask for the values.
    AutoPtr<IContentQueryMap> map;
    mHost->Resolve(EIID_IContentQueryMap, (IInterface**)&map);
    if (map != NULL) {
        Int32 count = 0;
        IObservable* observable = IObservable::Probe(map.Get());
        assert(observable != NULL);
        observable->CountObservers(&count);

        if (count != 0) {
            map->Requery();
        }
        else {
            ((CContentQueryMap*)map.Get())->mDirty = TRUE;
        }
    }

    return NOERROR;
}

CContentQueryMap::CContentQueryMap()
    : mKeyColumn(0)
    , mKeepUpdated(FALSE)
    , mDirty(FALSE)
    , mChanged(FALSE)
{}

CContentQueryMap::~CContentQueryMap()
{
}

ECode CContentQueryMap::SetKeepUpdated(
    /* [in] */ Boolean keepUpdated)
{
    if (keepUpdated == mKeepUpdated) return NOERROR;
    mKeepUpdated = keepUpdated;

    if (!mKeepUpdated) {
        FAIL_RETURN(mCursor->UnregisterContentObserver(mContentObserver))
        mContentObserver = NULL;
    }
    else {
        if (NULL == mHandlerForUpdateNotifications) {
            CHandler::New((IHandler**)&mHandlerForUpdateNotifications);
        }

        if (NULL == mContentObserver) {
            AutoPtr<IWeakReference> wr;
            GetWeakReference((IWeakReference**)&wr);
            mContentObserver = new KeepUpdatedContentObserver(
                mHandlerForUpdateNotifications, wr);
        }
        FAIL_RETURN(mCursor->RegisterContentObserver(mContentObserver))
        // mark dirty, since it is possible the cursor's backing data had changed before we
        // registered for changes
        mDirty = TRUE;
    }

    return NOERROR;
}

ECode CContentQueryMap::GetValues(
    /* [in] */ const String& rowName,
    /* [out] */ IContentValues** contentValues)
{
    Mutex::Autolock lock(mMethodLock);
    VALIDATE_NOT_NULL(contentValues)
    *contentValues = NULL;

    if (mDirty) {
        FAIL_RETURN(Requery())
    }

    HashMap<String, AutoPtr<IContentValues> >::Iterator it = mValues.Find(rowName);
    if (it != mValues.End()) {
        *contentValues = it->mSecond;
        REFCOUNT_ADD(*contentValues);
    }
    return NOERROR;
}

ECode CContentQueryMap::Requery()
{
    AutoPtr<ICursor> cursor = mCursor;
    if (NULL == cursor) {
        // If mCursor is null then it means there was a requery() in flight
        // while another thread called close(), which nulls out mCursor.
        // If this happens ignore the requery() since we are closed anyways.
        return NOERROR;
    }

    mDirty = FALSE;
    Boolean ret = FALSE;
    FAIL_RETURN(cursor->Requery(&ret))
    if (!ret) {
        // again, don't do anything if the cursor is already closed
        return NOERROR;
    }
    FAIL_RETURN(ReadCursorIntoCache(cursor))

    IObservable* observable = THIS_PROBE(IObservable);
    FAIL_RETURN(observable->SetChanged())
    FAIL_RETURN(observable->NotifyObservers())
    return NOERROR;
}

ECode CContentQueryMap::GetRows(
    /* [out] */ IObjectStringMap** rows)
{
    VALIDATE_NOT_NULL(rows);
    *rows = NULL;

    Mutex::Autolock lock(mMethodLock);
    VALIDATE_NOT_NULL(rows)
    if (mDirty) {
        FAIL_RETURN(Requery())
    }

    FAIL_RETURN(CObjectStringMap::New(rows))
    HashMap<String, AutoPtr<IContentValues> >::Iterator it = mValues.Begin();
    for (; it != mValues.End(); it++) {
        FAIL_RETURN((*rows)->Put(it->mFirst, (IInterface*) it->mSecond))
    }

    return NOERROR;
}

ECode CContentQueryMap::Close()
{
    Mutex::Autolock lock(mMethodLock);
    if (NULL != mContentObserver) {
        FAIL_RETURN(mCursor->UnregisterContentObserver(mContentObserver))
        mContentObserver = NULL;
    }
    FAIL_RETURN(mCursor->Close())
    mCursor = NULL;
    return NOERROR;
}

ECode CContentQueryMap::constructor(
    /* [in] */ ICursor* cursor,
    /* [in] */ const String& columnNameOfKey,
    /* [in] */ Boolean keepUpdated,
    /* [in] */ IHandler* handlerForUpdateNotifications)
{
    mCursor = cursor;
    FAIL_RETURN(mCursor->GetColumnNames((ArrayOf<String>**)&mColumnNames))
    FAIL_RETURN(mCursor->GetColumnIndexOrThrow(columnNameOfKey, &mKeyColumn))
    mHandlerForUpdateNotifications = handlerForUpdateNotifications;
    FAIL_RETURN(SetKeepUpdated(keepUpdated))

    // If we aren't keeping the cache updated with the current state of the cursor's
    // ContentProvider then read it once into the cache. Otherwise the cache will be filled
    // automatically.
    if (!keepUpdated) {
        FAIL_RETURN(ReadCursorIntoCache(cursor))
    }

    return InitObservable();
}

ECode CContentQueryMap::Finalize()
{
    if (NULL != mCursor) {
        FAIL_RETURN(Close())
    }
//    super.finalize();
    return NOERROR;
}

ECode CContentQueryMap::ReadCursorIntoCache(
    /* [in] */ ICursor* cursor)
{
    Mutex::Autolock lock(mMethodLock);
    // Make a new map so old values returned by getRows() are undisturbed.
    Int32 capacity = mValues.GetSize();
    mValues.Clear();
    mValues.Resize(capacity);
    Boolean hasNext = FALSE;
    String columnValue, keyColumn;

    while ((cursor->MoveToNext(&hasNext), hasNext)) {
        AutoPtr<IContentValues> values;
        FAIL_RETURN(CContentValues::New((IContentValues**)&values))
        for (Int32 i = 0; i < mColumnNames->GetLength(); i++) {
            if (i != mKeyColumn) {
                FAIL_RETURN(cursor->GetString(i, &columnValue))
                AutoPtr<ICharSequence> stringObj;
                FAIL_RETURN(CStringWrapper::New(columnValue, (ICharSequence**)&stringObj))
                FAIL_RETURN(values->PutString((*mColumnNames)[i], stringObj))
            }
        }
        FAIL_RETURN(cursor->GetString(mKeyColumn, &keyColumn))
        mValues[keyColumn] = values;
    }

    return NOERROR;
}

//========================================================
// CObservable
//========================================================
ECode CContentQueryMap::InitObservable()
{
    AutoPtr<IArrayList> outlist;
    FAIL_RETURN(CArrayList::New((IArrayList**)&outlist));
    mObservers = IList::Probe(outlist);
    mChanged = FALSE;
    return NOERROR;
}

ECode CContentQueryMap::AddObserver(
    /* [in] */ IObserver* observer)
{
    if (observer == NULL) {
        // throw new NullPointerException("observer == null");
        return E_NULL_POINTER_EXCEPTION;
    }
    {
        Mutex::Autolock lock(mLock);
        Boolean isflag = FALSE;
        if (!(mObservers->Contains(observer, &isflag), isflag))
            mObservers->Add(observer, &isflag);
    }
    return NOERROR;
}

ECode CContentQueryMap::CountObservers(
    /* [out] */ Int32* value)
{
    return mObservers->GetSize(value);
}

ECode CContentQueryMap::DeleteObserver(
    /* [in] */ IObserver* observer)
{
    Boolean isflag = FALSE;
    return mObservers->Remove(observer, &isflag);
}

ECode CContentQueryMap::DeleteObservers()
{
    return mObservers->Clear();
}

ECode CContentQueryMap::HasChanged(
    /* [out] */ Boolean* value)
{
    VALIDATE_NOT_NULL(value)

    *value = mChanged;
    return NOERROR;
}

ECode CContentQueryMap::NotifyObservers()
{
    return NotifyObservers(NULL);
}

ECode CContentQueryMap::NotifyObservers(
    /* [in] */ IInterface* data)
{
    Int32 size = 0;
    AutoPtr< ArrayOf<IInterface*> > arrays;
    {
        Mutex::Autolock lock(mLock);
        Boolean isflag = FALSE;
        if (HasChanged(&isflag), isflag) {
            ClearChanged();
            mObservers->GetSize(&size);
            mObservers->ToArray((ArrayOf<IInterface*>**)&arrays);
        }
    }
    if (arrays != NULL) {
        for (Int32 i = 0; i < arrays->GetLength(); i++) {
            AutoPtr<IObserver> observer = IObserver::Probe((*arrays)[i]);
            observer->Update(THIS_PROBE(IObservable), data);
        }
    }
    return NOERROR;
}

ECode CContentQueryMap::ClearChanged()
{
    mChanged = FALSE;
    return NOERROR;
}

ECode CContentQueryMap::SetChanged()
{
    mChanged = TRUE;
    return NOERROR;
}


}
}
}

