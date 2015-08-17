
#include "ext/frameworkext.h"
#include "app/CDownloadManagerRequest.h"
#include "app/CDownloadManagerQuery.h"
#include "app/CDownloadManager.h"
#include "content/CContentUris.h"
#include "content/CContentValues.h"
#include "net/Uri.h"
#include "provider/CDownloadsImpl.h"
#include "provider/CSettingsGlobal.h"
#include "text/TextUtils.h"
#include <elastos/utility/logging/Slogger.h>
#include <elastos/core/StringUtils.h>
#include <elastos/core/StringBuilder.h>

using Elastos::IO::CFile;
using Elastos::Core::CInteger32;
using Elastos::Core::CString;
using Elastos::Core::IInteger32;
using Elastos::Core::StringUtils;
using Elastos::Core::StringBuilder;
using Elastos::Utility::Logging::Slogger;
using Elastos::Droid::Content::CContentUris;
using Elastos::Droid::Content::IContentUris;
using Elastos::Droid::Content::CContentValues;
using Elastos::Droid::Content::IContentValues;
using Elastos::Droid::Database::EIID_ICursor;
using Elastos::Droid::Database::EIID_ICursorWrapper;
using Elastos::Droid::Net::Uri;
using Elastos::Droid::Provider::CDownloadsImpl;
using Elastos::Droid::Provider::IBaseColumns;
using Elastos::Droid::Provider::IDownloadsImpl;
using Elastos::Droid::Provider::CSettingsGlobal;
using Elastos::Droid::Provider::ISettingsGlobal;
using Elastos::Droid::Text::TextUtils;

namespace Elastos {
namespace Droid {
namespace App {

const String CDownloadManager::NON_DOWNLOADMANAGER_DOWNLOAD =
        String("non-dwnldmngr-download-dont-retry2download");

const AutoPtr<ArrayOf<String> > CDownloadManager::UNDERLYING_COLUMNS = CDownloadManager::InitUNDERLYINGCOLUMNS();

PInterface CDownloadManager::CursorTranslator::Probe(
    /* [in] */ REIID riid)
{
    if (riid == EIID_ICursorWrapper) {
        return (PInterface)(ICursorWrapper*)this;
    }
    return NULL;
}

UInt32 CDownloadManager::CursorTranslator::AddRef()
{
    return ElRefBase::AddRef();
}

UInt32 CDownloadManager::CursorTranslator::Release()
{
    return ElRefBase::Release();
}

ECode CDownloadManager::CursorTranslator::GetInterfaceID(
    /* [in] */ IInterface *pObject,
    /* [out] */ InterfaceID *pIID)
{
    if (NULL == pIID) return E_INVALID_ARGUMENT;

    if (pObject == (IInterface *)(ICursorWrapper *)this) {
        *pIID = EIID_ICursorWrapper;
        return NOERROR;
    }
    else if (pObject == (IInterface *)(ICursor *)this) {
        *pIID = EIID_ICursor;
        return NOERROR;
    }

    return E_INVALID_ARGUMENT;
}

CDownloadManager::CursorTranslator::CursorTranslator(
    /* [in] */ ICursor* cursor,
    /* [in] */ IUri* baseUri)
    : mBaseUri(baseUri)
{
    CursorWrapper::Init(cursor);
}

//  @Override
ECode CDownloadManager::CursorTranslator::GetInt32(
    /* [in] */ Int32 columnIndex,
    /* [out] */ Int32* value)
{
    VALIDATE_NOT_NULL(value);

    Int64 ivalue;
    GetInt64(columnIndex, &ivalue);
    *value = (Int32)ivalue;
    return NOERROR;
}

//  @Override
ECode CDownloadManager::CursorTranslator::GetInt64(
    /* [in] */ Int32 columnIndex,
    /* [out] */ Int64* value)
{
    VALIDATE_NOT_NULL(value);

    String name;
    GetColumnName(columnIndex, &name);
    if (name.Equals(COLUMN_REASON)) {
        Int32 index;
        GetColumnIndex(IDownloadsImpl::COLUMN_STATUS, &index);
        Int32 indexValue;
        CursorWrapper::GetInt32(index, &indexValue);
        *value = GetReason(indexValue);
        return NOERROR;
    } else if (name.Equals(COLUMN_STATUS)) {
        Int32 index;
        GetColumnIndex(IDownloadsImpl::COLUMN_STATUS, &index);
        Int32 indexValue;
        CursorWrapper::GetInt32(index, &indexValue);
        *value = TranslateStatus(indexValue);
        return NOERROR;
    } else {
        return CursorWrapper::GetInt64(columnIndex, value);
    }
}

//  @Override
ECode CDownloadManager::CursorTranslator::GetString(
    /* [in] */ Int32 columnIndex,
    /* [out] */ String* value)
{
    VALIDATE_NOT_NULL(value);

    String name;
    GetColumnName(columnIndex, &name);
    String svalue;
    CursorWrapper::GetString(columnIndex, &svalue);
    *value = (name.Equals(COLUMN_LOCAL_URI)) ? GetLocalUri() : svalue;
    return NOERROR;
}

String CDownloadManager::CursorTranslator::GetLocalUri()
{
    Int32 index;
    GetColumnIndex(IDownloadsImpl::COLUMN_DESTINATION, &index);
    Int64 destinationType;
    GetInt64(index, &destinationType);
    if (destinationType == IDownloadsImpl::DESTINATION_FILE_URI ||
            destinationType == IDownloadsImpl::DESTINATION_EXTERNAL ||
            destinationType == IDownloadsImpl::DESTINATION_NON_DOWNLOADMANAGER_DOWNLOAD) {
        Int32 cindex;
        GetColumnIndex(COLUMN_LOCAL_FILENAME, &cindex);
        String localPath;
        GetString(cindex, &localPath);
        if (localPath.IsNull()) {
            return String(NULL);
        }
        AutoPtr<IFile> file;
        CFile::New(localPath, (IFile**)&file);
        AutoPtr<IUri> uri;
        Uri::FromFile(file, (IUri**)&uri);
        String uStr;
        uri->ToString(&uStr);
        return uStr;
    }

    // return content URI for cache download
    Int32 idIndex;
    GetColumnIndex(IBaseColumns::ID, &idIndex);
    Int64 downloadId;
    GetInt64(idIndex, &downloadId);
    AutoPtr<IContentUris> cUris;
    CContentUris::AcquireSingleton((IContentUris**)&cUris);
    AutoPtr<IUri> uri;
    cUris->WithAppendedId(mBaseUri, downloadId, (IUri**)&uri);
    String uriStr;
    uri->ToString(&uriStr);
    return uriStr;
}

Int64 CDownloadManager::CursorTranslator::GetReason(
    /* [in] */ Int32 status)
{
    switch (TranslateStatus(status)) {
        case STATUS_FAILED:
            return GetErrorCode(status);

        case STATUS_PAUSED:
            return GetPausedReason(status);

        default:
            return 0; // arbitrary value when status is not an error
    }
}

Int64 CDownloadManager::CursorTranslator::GetPausedReason(
    /* [in] */ Int32 status)
{
    switch (status) {
        case IDownloadsImpl::STATUS_WAITING_TO_RETRY:
            return PAUSED_WAITING_TO_RETRY;

        case IDownloadsImpl::STATUS_WAITING_FOR_NETWORK:
            return PAUSED_WAITING_FOR_NETWORK;

        case IDownloadsImpl::STATUS_QUEUED_FOR_WIFI:
            return PAUSED_QUEUED_FOR_WIFI;

        default:
            return PAUSED_UNKNOWN;
    }
}

Int64 CDownloadManager::CursorTranslator::GetErrorCode(
    /* [in] */ Int32 status)
{
    if ((400 <= status && status < IDownloadsImpl::MIN_ARTIFICIAL_ERROR_STATUS)
            || (500 <= status && status < 600)) {
        // HTTP status code
        return status;
    }

    switch (status) {
        case IDownloadsImpl::STATUS_FILE_ERROR:
            return ERROR_FILE_ERROR;

        case IDownloadsImpl::STATUS_UNHANDLED_HTTP_CODE:
        case IDownloadsImpl::STATUS_UNHANDLED_REDIRECT:
            return ERROR_UNHANDLED_HTTP_CODE;

        case IDownloadsImpl::STATUS_HTTP_DATA_ERROR:
            return ERROR_HTTP_DATA_ERROR;

        case IDownloadsImpl::STATUS_TOO_MANY_REDIRECTS:
            return ERROR_TOO_MANY_REDIRECTS;

        case IDownloadsImpl::STATUS_INSUFFICIENT_SPACE_ERROR:
            return ERROR_INSUFFICIENT_SPACE;

        case IDownloadsImpl::STATUS_DEVICE_NOT_FOUND_ERROR:
            return ERROR_DEVICE_NOT_FOUND;

        case IDownloadsImpl::STATUS_CANNOT_RESUME:
            return ERROR_CANNOT_RESUME;

        case IDownloadsImpl::STATUS_FILE_ALREADY_EXISTS_ERROR:
            return ERROR_FILE_ALREADY_EXISTS;

        default:
            return ERROR_UNKNOWN;
    }
}

Int32 CDownloadManager::CursorTranslator::TranslateStatus(
    /* [in] */ Int32 status)
{
    switch (status) {
        case IDownloadsImpl::STATUS_PENDING:
            return STATUS_PENDING;
        case IDownloadsImpl::STATUS_RUNNING:
            return STATUS_RUNNING;

        case IDownloadsImpl::STATUS_PAUSED_BY_APP:
        case IDownloadsImpl::STATUS_WAITING_TO_RETRY:
        case IDownloadsImpl::STATUS_WAITING_FOR_NETWORK:
        case IDownloadsImpl::STATUS_QUEUED_FOR_WIFI:
            return STATUS_PAUSED;

        case IDownloadsImpl::STATUS_SUCCESS:
            return STATUS_SUCCESSFUL;

        default:
        {
            AutoPtr<IDownloadsImpl> impl;
            CDownloadsImpl::AcquireSingleton((IDownloadsImpl**)&impl);
            Boolean res;
            impl->IsStatusError(status, &res);
            assert(res != FALSE);
            return STATUS_FAILED;
        }
    }
}

ECode CDownloadManager::CursorTranslator::GetWrappedCursor(
    /* [out] */ ICursor** cursor)
{
    return CursorWrapper::GetWrappedCursor(cursor);
}

ECode CDownloadManager::CursorTranslator::Close()
{
    return CursorWrapper::Close();
}

ECode CDownloadManager::CursorTranslator::IsClosed(
    /* [out] */ Boolean* isClosed)
{
    return CursorWrapper::IsClosed(isClosed);
}

ECode CDownloadManager::CursorTranslator::GetCount(
    /* [out] */ Int32* count)
{
    return CursorWrapper::GetCount(count);
}

ECode CDownloadManager::CursorTranslator::Deactivate()
{
    return CursorWrapper::Deactivate();
}

ECode CDownloadManager::CursorTranslator::MoveToFirst(
    /* [out] */ Boolean* succeeded)
{
    return CursorWrapper::MoveToFirst(succeeded);
}

ECode CDownloadManager::CursorTranslator::GetColumnCount(
    /* [out] */ Int32* count)
{
    return CursorWrapper::GetColumnCount(count);
}

ECode CDownloadManager::CursorTranslator::GetColumnIndex(
    /* [in] */ const String& columnName,
    /* [out] */ Int32* index)
{
    return CursorWrapper::GetColumnIndex(columnName, index);
}

ECode CDownloadManager::CursorTranslator::GetColumnIndexOrThrow(
    /* [in] */  const String& columnName,
    /* [out] */ Int32* columnIndex)
{
    return CursorWrapper::GetColumnIndexOrThrow(columnName, columnIndex);
}

ECode CDownloadManager::CursorTranslator::GetColumnName(
    /* [in] */ Int32 columnIndex,
    /* [out] */ String* name)
{
    return CursorWrapper::GetColumnName(columnIndex, name);
}

ECode CDownloadManager::CursorTranslator::GetColumnNames(
    /* [out, callee] */ ArrayOf<String>** columnNames)
{
    return CursorWrapper::GetColumnNames(columnNames);
}

ECode CDownloadManager::CursorTranslator::GetDouble(
    /* [in] */ Int32 columnIndex,
    /* [out] */ Double* value)
{
    return CursorWrapper::GetDouble(columnIndex, value);
}

ECode CDownloadManager::CursorTranslator::GetExtras(
    /* [out] */ IBundle** extras)
{
    return CursorWrapper::GetExtras(extras);
}

ECode CDownloadManager::CursorTranslator::GetFloat(
    /* [in] */ Int32 columnIndex,
    /* [out] */ Float* value)
{
    return CursorWrapper::GetFloat(columnIndex, value);
}

ECode CDownloadManager::CursorTranslator::GetInt16(
    /* [in] */ Int32 columnIndex,
    /* [out] */ Int16* value)
{
    return CursorWrapper::GetInt16(columnIndex, value);
}

ECode CDownloadManager::CursorTranslator::CopyStringToBuffer(
    /* [in] */ Int32 columnIndex,
    /* [in] */ ICharArrayBuffer* buffer)
{
    return CursorWrapper::CopyStringToBuffer(columnIndex, buffer);
}

ECode CDownloadManager::CursorTranslator::GetBlob(
    /* [in] */  Int32 columnIndex,
    /* [out,callee] */ ArrayOf<Byte>** blob)
{
    return CursorWrapper::GetBlob(columnIndex, blob);
}

ECode CDownloadManager::CursorTranslator::GetWantsAllOnMoveCalls(
    /* [out] */ Boolean* value)
{
    return CursorWrapper::GetWantsAllOnMoveCalls(value);
}

ECode CDownloadManager::CursorTranslator::IsAfterLast(
    /* [out] */ Boolean* result)
{
    return CursorWrapper::IsAfterLast(result);
}

ECode CDownloadManager::CursorTranslator::IsBeforeFirst(
    /* [out] */ Boolean* result)
{
    return CursorWrapper::IsBeforeFirst(result);
}

ECode CDownloadManager::CursorTranslator::IsFirst(
    /* [out] */ Boolean* result)
{
    return CursorWrapper::IsFirst(result);
}

ECode CDownloadManager::CursorTranslator::IsLast(
    /* [out] */ Boolean* result)
{
    return CursorWrapper::IsLast(result);
}

ECode CDownloadManager::CursorTranslator::GetType(
    /* [in] */ Int32 columnIndex,
    /* [out] */ Int32* type)
{
    return CursorWrapper::GetType(columnIndex, type);
}

ECode CDownloadManager::CursorTranslator::IsNull(
    /* [in] */ Int32 columnIndex,
    /* [out] */ Boolean* result)
{
    return CursorWrapper::IsNull(columnIndex, result);
}

ECode CDownloadManager::CursorTranslator::MoveToLast(
    /* [out] */ Boolean* succeeded)
{
    return CursorWrapper::MoveToLast(succeeded);
}

ECode CDownloadManager::CursorTranslator::Move(
    /* [in] */ Int32 offset,
    /* [out] */ Boolean* succeeded)
{
    return CursorWrapper::Move(offset, succeeded);
}

ECode CDownloadManager::CursorTranslator::MoveToPosition(
    /* [in] */ Int32 position,
    /* [out] */ Boolean* succeeded)
{
    return CursorWrapper::MoveToPosition(position, succeeded);
}

ECode CDownloadManager::CursorTranslator::MoveToNext(
    /* [out] */ Boolean* succeeded)
{
    return CursorWrapper::MoveToNext(succeeded);
}

ECode CDownloadManager::CursorTranslator::GetPosition(
    /*[out]*/ Int32* position)
{
    return CursorWrapper::GetPosition(position);
}

ECode CDownloadManager::CursorTranslator::MoveToPrevious(
    /* [out] */ Boolean* succeeded)
{
    return CursorWrapper::MoveToPrevious(succeeded);
}

ECode CDownloadManager::CursorTranslator::RegisterContentObserver(
    /* [in] */ IContentObserver* observer)
{
    return CursorWrapper::RegisterContentObserver(observer);
}

ECode CDownloadManager::CursorTranslator::RegisterDataSetObserver(
    /* [in] */IDataSetObserver* observer)
{
    return CursorWrapper::RegisterDataSetObserver(observer);
}

ECode CDownloadManager::CursorTranslator::Requery(
    /* [out] */ Boolean* succeeded)
{
    return CursorWrapper::Requery(succeeded);
}

ECode CDownloadManager::CursorTranslator::Respond(
    /* [in] */ IBundle* extras,
    /* [out] */ IBundle** bundle)
{
    return CursorWrapper::Respond(extras, bundle);
}

ECode CDownloadManager::CursorTranslator::SetNotificationUri(
    /* [in] */ IContentResolver* cr,
    /* [in] */ IUri* uri)
{
    return CursorWrapper::SetNotificationUri(cr, uri);
}

ECode CDownloadManager::CursorTranslator::UnregisterContentObserver(
    /* [in] */ IContentObserver* observer)
{
    return CursorWrapper::UnregisterContentObserver(observer);
}

ECode CDownloadManager::CursorTranslator::UnregisterDataSetObserver(
    /* [in] */ IDataSetObserver* observer)
{
    return CursorWrapper::UnregisterDataSetObserver(observer);
}

AutoPtr<ArrayOf<String> > CDownloadManager::InitUNDERLYINGCOLUMNS()
{
    AutoPtr<ArrayOf<String> > columns = ArrayOf<String>::Alloc(15);

    columns->Set(0, IBaseColumns::ID);
    columns->Set(1, IDownloadsImpl::DATA + " AS " + IDownloadManager::COLUMN_LOCAL_FILENAME);
    columns->Set(2, IDownloadsImpl::COLUMN_MEDIAPROVIDER_URI);
    columns->Set(3, IDownloadsImpl::COLUMN_DESTINATION);
    columns->Set(4, IDownloadsImpl::COLUMN_TITLE);
    columns->Set(5, IDownloadsImpl::COLUMN_DESCRIPTION);
    columns->Set(6, IDownloadsImpl::COLUMN_URI);
    columns->Set(7, IDownloadsImpl::COLUMN_STATUS);
    columns->Set(8, IDownloadsImpl::COLUMN_FILE_NAME_HINT);
    columns->Set(9, IDownloadsImpl::COLUMN_MIME_TYPE + " AS " + IDownloadManager::COLUMN_MEDIA_TYPE);
    columns->Set(10, IDownloadsImpl::COLUMN_TOTAL_BYTES + " AS " + IDownloadManager::COLUMN_TOTAL_SIZE_BYTES);
    columns->Set(11, IDownloadsImpl::COLUMN_LAST_MODIFICATION + " AS " + IDownloadManager::COLUMN_LAST_MODIFIED_TIMESTAMP);
    columns->Set(12, IDownloadsImpl::COLUMN_CURRENT_BYTES + " AS " + IDownloadManager::COLUMN_BYTES_DOWNLOADED_SO_FAR);
    /* add the following 'computed' columns to the cursor.
     * they are not 'returned' by the database, but their inclusion
     * eliminates need to have lot of methods in CursorTranslator
     */
    columns->Set(13, String("'placeholder' AS ") + IDownloadManager::COLUMN_LOCAL_URI);
    columns->Set(14, String("'placeholder' AS ") + IDownloadManager::COLUMN_REASON);
    return columns;
}

/**
 * @hide
 */
ECode CDownloadManager::constructor(
    /* [in] */ IContentResolver* resolver,
    /* [in] */ const String& packageName)
{
    mResolver = resolver;
    mPackageName = packageName;
    AutoPtr<IDownloadsImpl> impl;
    CDownloadsImpl::AcquireSingleton((IDownloadsImpl**)&impl);
    impl->GetCONTENT_URI((IUri**)&mBaseUri);
    return NOERROR;
}

ECode CDownloadManager::SetAccessAllDownloads(
    /* [in] */ Boolean accessAllDownloads)
{
    AutoPtr<IDownloadsImpl> dImpl;
    CDownloadsImpl::AcquireSingleton((IDownloadsImpl**)&dImpl);
    if (accessAllDownloads) {
        dImpl->GetALLDOWNLOADSCONTENTURI((IUri**)&mBaseUri);
    } else {
        dImpl->GetCONTENT_URI((IUri**)&mBaseUri);
    }
    return NOERROR;
}

ECode CDownloadManager::Enqueue(
    /* [in] */ IDownloadManagerRequest* request,
    /* [out] */ Int64* id)
{
    VALIDATE_NOT_NULL(id);
    AutoPtr<IContentValues> values = ((CDownloadManagerRequest*)request)->ToContentValues(mPackageName);
    AutoPtr<IDownloadsImpl> dImpl;
    CDownloadsImpl::AcquireSingleton((IDownloadsImpl**)&dImpl);
    AutoPtr<IUri> uri;
    dImpl->GetCONTENT_URI((IUri**)&uri);
    AutoPtr<IUri> downloadUri;
    mResolver->Insert(uri, values, (IUri**)&downloadUri);
    String segment;
    downloadUri->GetLastPathSegment(&segment);
    *id = StringUtils::ParseInt64(segment);
    return NOERROR;
}

ECode CDownloadManager::MarkRowDeleted(
    /* [in] */ ArrayOf<Int64>* ids,
    /* [out] */ Int32* number)
{
    VALIDATE_NOT_NULL(number);

    if (ids == NULL || ids->GetLength() == 0) {
        // called with nothing to remove!
        // throw new IllegalArgumentException("input param 'ids' can't be null");
        Slogger::E("CDownloadManager", "input param 'ids' can't be null");
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    AutoPtr<IContentValues> values;
    CContentValues::New((IContentValues**)&values);
    AutoPtr<IInteger32> column;
    CInteger32::New(1, (IInteger32**)&column);
    values->PutInt32(IDownloadsImpl::COLUMN_DELETED, column);
    // if only one id is passed in, then include it in the uri itself.
    // this will eliminate a full database scan in the download service.
    if (ids->GetLength() == 1) {
        AutoPtr<IContentUris> curis;
        CContentUris::AcquireSingleton((IContentUris**)&curis);
        AutoPtr<IUri> uri;
        curis->WithAppendedId(mBaseUri, (*ids)[0], (IUri**)&uri);
        return mResolver->Update(uri, values, String(NULL), NULL, number);
    }
    AutoPtr<ArrayOf<String> > args = GetWhereArgsForIds(ids);
    return mResolver->Update(mBaseUri, values, GetWhereClauseForIds(ids),
            args, number);
}

ECode CDownloadManager::Remove(
    /* [in] */ ArrayOf<Int64>* ids,
    /* [out] */ Int32* number)
{
    VALIDATE_NOT_NULL(number);

    return MarkRowDeleted(ids, number);
}

ECode CDownloadManager::Query(
    /* [in] */ IDownloadManagerQuery* query,
    /* [out] */ ICursor** cursor)
{
    VALIDATE_NOT_NULL(cursor);

    AutoPtr<ICursor> underlyingCursor = ((CDownloadManagerQuery*)query)->RunQuery(mResolver, UNDERLYING_COLUMNS, mBaseUri);
    if (underlyingCursor == NULL) {
        *cursor = NULL;
        return NOERROR;
    }
    *cursor = new CursorTranslator(underlyingCursor, mBaseUri);
    REFCOUNT_ADD(*cursor);
    return NOERROR;
}

ECode CDownloadManager::OpenDownloadedFile(
    /* [in] */ Int64 id,
    /* [out] */ IParcelFileDescriptor** descriptor)
{
    VALIDATE_NOT_NULL(descriptor);

    return mResolver->OpenFileDescriptor(GetDownloadUri(id), String("r"), descriptor);
}

ECode CDownloadManager::GetUriForDownloadedFile(
    /* [in] */ Int64 id,
    /* [out] */ IUri** uri)
{
    VALIDATE_NOT_NULL(uri);
    *uri = NULL;

    // to check if the file is in cache, get its destination from the database
    AutoPtr<IDownloadManagerQuery> query;
    CDownloadManagerQuery::New((IDownloadManagerQuery**)&query);
    AutoPtr<ArrayOf<Int64> > idArray = ArrayOf<Int64>::Alloc(1);
    idArray->Set(0, id);
    query->SetFilterById(idArray);
    AutoPtr<ICursor> cursor;
    // try {
    Query(query, (ICursor**)&cursor);
    if (cursor == NULL) {
        return NOERROR;
    }
    Boolean hasFirst;
    if (cursor->MoveToFirst(&hasFirst), hasFirst) {
        Int32 cindex;
        cursor->GetColumnIndexOrThrow(COLUMN_STATUS, &cindex);
        Int32 status;
        cursor->GetInt32(cindex, &status);
        if (IDownloadManager::STATUS_SUCCESSFUL == status) {
            Int32 indx;
            cursor->GetColumnIndexOrThrow(
                    IDownloadsImpl::COLUMN_DESTINATION, &indx);
            Int32 destination;
            cursor->GetInt32(indx, &destination);
            // TODO: if we ever add API to DownloadManager to let the caller specify
            // non-external storage for a downloaded file, then the following code
            // should also check for that destination.
            if (destination == IDownloadsImpl::DESTINATION_CACHE_PARTITION ||
                    destination == IDownloadsImpl::DESTINATION_SYSTEMCACHE_PARTITION ||
                    destination == IDownloadsImpl::DESTINATION_CACHE_PARTITION_NOROAMING ||
                    destination == IDownloadsImpl::DESTINATION_CACHE_PARTITION_PURGEABLE) {
                // return private uri
                AutoPtr<IContentUris> cUris;
                CContentUris::AcquireSingleton((IContentUris**)&cUris);
                AutoPtr<IDownloadsImpl> impl;
                CDownloadsImpl::AcquireSingleton((IDownloadsImpl**)&impl);
                AutoPtr<IUri> contentUri;
                impl->GetCONTENT_URI((IUri**)&contentUri);
                cUris->WithAppendedId(contentUri, id, uri);
                if (cursor != NULL) {
                    cursor->Close();
                }
                return NOERROR;
            } else {
                // return public uri
                Int32 index;
                cursor->GetColumnIndexOrThrow(COLUMN_LOCAL_FILENAME, &index);
                String path;
                cursor->GetString(index, &path);
                AutoPtr<IFile> file;
                CFile::New(path, (IFile**)&file);
                Uri::FromFile(file, uri);
                if (cursor != NULL) {
                    cursor->Close();
                }
                return NOERROR;
            }
        }
    }
    // } finally {
    if (cursor != NULL) {
        cursor->Close();
    }
    // }
    // downloaded file not found or its status is not 'successfully completed'
    return NOERROR;
}

ECode CDownloadManager::GetMimeTypeForDownloadedFile(
    /* [in] */ Int64 id,
    /* [out] */ String* uri)
{
    VALIDATE_NOT_NULL(uri);
    *uri = String(NULL);

    AutoPtr<IDownloadManagerQuery> query;
    CDownloadManagerQuery::New((IDownloadManagerQuery**)&query);
    AutoPtr<ArrayOf<Int64> > idArray = ArrayOf<Int64>::Alloc(1);
    idArray->Set(0, id);
    query->SetFilterById(idArray);
    AutoPtr<ICursor> cursor;
    // try {
    Query(query, (ICursor**)&cursor);
    if (cursor == NULL) {
        return NOERROR;
    }
    Boolean toFirst;
    while (cursor->MoveToFirst(&toFirst), toFirst) {
        Int32 index;
        cursor->GetColumnIndexOrThrow(COLUMN_MEDIA_TYPE, &index);
        cursor->GetString(index, uri);
        if (cursor != NULL) {
            cursor->Close();
        }
        return NOERROR;
    }
    // } finally {
    if (cursor != NULL) {
        cursor->Close();
    }
    // }
    // downloaded file not found or its status is not 'successfully completed'
    return NOERROR;
}

ECode CDownloadManager::RestartDownload(
    /* [in] */ ArrayOf<Int64>* ids)
{
    AutoPtr<IDownloadManagerQuery> query;
    CDownloadManagerQuery::New((IDownloadManagerQuery**)&query);
    query->SetFilterById(ids);
    AutoPtr<ICursor> cursor;
    Query(query, (ICursor**)&cursor);
    assert(cursor != NULL);
    // try {
    Boolean toFirst = FALSE;
    cursor->MoveToFirst(&toFirst);
    Boolean afterLast = FALSE;
    Boolean hasNext = FALSE;
    if (toFirst) {
        for (; (cursor->IsAfterLast(&afterLast), !afterLast); cursor->MoveToNext(&hasNext), hasNext) {
            Int32 index;
            cursor->GetColumnIndex(COLUMN_STATUS, &index);
            Int32 status;
            cursor->GetInt32(index, &status);
            if (status != STATUS_SUCCESSFUL && status != STATUS_FAILED) {
                // throw new IllegalArgumentException("Cannot restart incomplete download: "
                //         + cursor.getLong(cursor.getColumnIndex(COLUMN_ID)));
                Int32 idIndex;
                cursor->GetColumnIndex(COLUMN_ID, &idIndex);
                Int64 idValue;
                cursor->GetInt64(idIndex, &idValue);
                Slogger::E("CDownloadManager", "Cannot restart incomplete download: %d", idValue);
                cursor->Close();
                return E_ILLEGAL_ARGUMENT_EXCEPTION;
            }
        }
    }

    // } finally {
    cursor->Close();
    // }

    AutoPtr<IContentValues> values;
    CContentValues::New((IContentValues**)&values);
    AutoPtr<IInteger32> value;
    CInteger32::New(0, (IInteger32**)&value);
    values->PutInt32(IDownloadsImpl::COLUMN_CURRENT_BYTES, value);
    value = NULL;
    CInteger32::New(-1, (IInteger32**)&value);
    values->PutInt32(IDownloadsImpl::COLUMN_TOTAL_BYTES, value);
    values->PutNull(IDownloadsImpl::DATA);
    value = NULL;
    CInteger32::New(IDownloadsImpl::STATUS_PENDING, (IInteger32**)&value);
    values->PutInt32(IDownloadsImpl::COLUMN_STATUS, value);
    AutoPtr<ArrayOf<String> > args = GetWhereArgsForIds(ids);
    Int32 res;
    mResolver->Update(mBaseUri, values, GetWhereClauseForIds(ids), args, &res);
    return NOERROR;
}


ECode CDownloadManager::GetMaxBytesOverMobile(
    /* [in] */ IContext* context,
    /* [out] */ Int64* size)
{
    VALIDATE_NOT_NULL(size);
    // try {
    AutoPtr<IContentResolver> resolver;
    context->GetContentResolver((IContentResolver**)&resolver);
    AutoPtr<ISettingsGlobal> global;
    CSettingsGlobal::AcquireSingleton((ISettingsGlobal**)&global);
    return global->GetInt64(resolver,
            ISettingsGlobal::DOWNLOAD_MAX_BYTES_OVER_MOBILE, size);
//     } catch (SettingNotFoundException exc) {
//         return null;
//     }
}


ECode CDownloadManager::GetRecommendedMaxBytesOverMobile(
    /* [in] */ IContext* context,
    /* [out] */ Int64* size)
{
    VALIDATE_NOT_NULL(size);
    // try {
    AutoPtr<IContentResolver> resolver;
    context->GetContentResolver((IContentResolver**)&resolver);
    AutoPtr<ISettingsGlobal> global;
    CSettingsGlobal::AcquireSingleton((ISettingsGlobal**)&global);
    return global->GetInt64(resolver,
            ISettingsGlobal::DOWNLOAD_RECOMMENDED_MAX_BYTES_OVER_MOBILE, size);
    // } catch (SettingNotFoundException exc) {
    //     return null;
    // }
}

ECode CDownloadManager::IsActiveNetworkExpensive(
    /* [in] */ IContext* context,
    /* [out] */ Boolean* actived)
{
    VALIDATE_NOT_NULL(actived);
    // TODO: connect to NetworkPolicyManager
    *actived = FALSE;
    return NOERROR;
}

ECode CDownloadManager::GetActiveNetworkWarningBytes(
    /* [in] */ IContext* context,
    /* [out] */ Int64* bytes)
{
    VALIDATE_NOT_NULL(bytes);
    // TODO: connect to NetworkPolicyManager
    *bytes = -1;
    return NOERROR;
}

ECode CDownloadManager::AddCompletedDownload(
    /* [in] */ const String& title,
    /* [in] */ const String& description,
    /* [in] */ Boolean isMediaScannerScannable,
    /* [in] */ const String& mimeType,
    /* [in] */ const String& path,
    /* [in] */ Int64 length,
    /* [in] */ Boolean showNotification,
    /* [out] */ Int64* id)
{
    VALIDATE_NOT_NULL(id);

    // make sure the input args are non-null/non-zero
    ValidateArgumentIsNonEmpty(String("title"), title);
    ValidateArgumentIsNonEmpty(String("description"), description);
    ValidateArgumentIsNonEmpty(String("path"), path);
    ValidateArgumentIsNonEmpty(String("mimeType"), mimeType);
    if (length < 0) {
        // throw new IllegalArgumentException(" invalid value for param: totalBytes");
        Slogger::E("CDownloadManager", " invalid value for param: totalBytes");
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }

    // if there is already an entry with the given path name in downloads.db, return its id
    AutoPtr<IDownloadManagerRequest> request;
    CDownloadManagerRequest::New(NON_DOWNLOADMANAGER_DOWNLOAD, (IDownloadManagerRequest**)&request);
    AutoPtr<ICharSequence> cs;
    CString::New(title, (ICharSequence**)&cs);
    request->SetTitle(cs);
    cs = NULL;
    CString::New(description, (ICharSequence**)&cs);
    request->SetDescription(cs);
    request->SetMimeType(mimeType);
    AutoPtr<IContentValues> values = ((CDownloadManagerRequest*)request.Get())->ToContentValues(String(NULL));
    AutoPtr<IInteger32> value;
    CInteger32::New(IDownloadsImpl::DESTINATION_NON_DOWNLOADMANAGER_DOWNLOAD, (IInteger32**)&value);
    values->PutInt32(IDownloadsImpl::COLUMN_DESTINATION, value);
    cs = NULL;
    CString::New(path, (ICharSequence**)&cs);
    values->PutString(IDownloadsImpl::DATA, cs);
    value = NULL;
    CInteger32::New(IDownloadsImpl::STATUS_SUCCESS, (IInteger32**)&value);
    values->PutInt32(IDownloadsImpl::COLUMN_STATUS, value);
    value = NULL;
    CInteger32::New(length, (IInteger32**)&value);
    values->PutInt32(IDownloadsImpl::COLUMN_TOTAL_BYTES, value);
    AutoPtr<IInteger32> yes, no;
    CInteger32::New(CDownloadManagerRequest::SCANNABLE_VALUE_YES, (IInteger32**)&yes);
    CInteger32::New(CDownloadManagerRequest::SCANNABLE_VALUE_NO, (IInteger32**)&no);
    values->PutInt32(IDownloadsImpl::COLUMN_MEDIA_SCANNED,
            (isMediaScannerScannable) ? yes : no);
    AutoPtr<IInteger32> completion, hidden;
    CInteger32::New(IDownloadManagerRequest::VISIBILITY_VISIBLE_NOTIFY_ONLY_COMPLETION, (IInteger32**)&completion);
    CInteger32::New(IDownloadManagerRequest::VISIBILITY_HIDDEN, (IInteger32**)&hidden);
    values->PutInt32(IDownloadsImpl::COLUMN_VISIBILITY, (showNotification) ?
            completion : hidden);
    AutoPtr<IDownloadsImpl> impl;
    CDownloadsImpl::AcquireSingleton((IDownloadsImpl**)&impl);
    AutoPtr<IUri> contentUri;
    impl->GetCONTENT_URI((IUri**)&contentUri);
    AutoPtr<IUri> downloadUri;
    mResolver->Insert(contentUri, values, (IUri**)&downloadUri);
    if (downloadUri == NULL) {
        *id = -1;
        return NOERROR;
    }
    String segment;
    downloadUri->GetLastPathSegment(&segment);
    *id = StringUtils::ParseInt64(segment);
    return NOERROR;
}

ECode CDownloadManager::ValidateArgumentIsNonEmpty(
    /* [in] */ const String& paramName,
    /* [in] */ const String& val)
{
    if (TextUtils::IsEmpty(val)) {
        // throw new IllegalArgumentException(paramName + " can't be null");
        Slogger::E("CDownloadManager", "%s can't be null", paramName.string());
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    return NOERROR;
}

AutoPtr<IUri> CDownloadManager::GetDownloadUri(
    /* [in] */ Int64 id)
{
    AutoPtr<IContentUris> cUris;
    CContentUris::AcquireSingleton((IContentUris**)&cUris);
    AutoPtr<IUri> uri;
    cUris->WithAppendedId(mBaseUri, id, (IUri**)&uri);
    return uri;
}

/**
 * Get a parameterized SQL WHERE clause to select a bunch of IDs.
 */
String CDownloadManager::GetWhereClauseForIds(
    /* [in] */ ArrayOf<Int64>* ids)
{
    StringBuilder whereClause;
    whereClause.AppendCStr("(");
    for (Int32 i = 0; i < ids->GetLength(); i++) {
        if (i > 0) {
            whereClause.AppendCStr("OR ");
        }
        whereClause.AppendString(IBaseColumns::ID);
        whereClause.AppendCStr(" = ? ");
    }
    whereClause.AppendCStr(")");
    return whereClause.ToString();
}

/**
 * Get the selection args for a clause returned by {@link #getWhereClauseForIds(long[])}.
 */
AutoPtr< ArrayOf<String> > CDownloadManager::GetWhereArgsForIds(
    /* [in] */ ArrayOf<Int64>* ids)
{
    Int32 length = ids->GetLength();
    AutoPtr<ArrayOf<String> > whereArgs = ArrayOf<String>::Alloc(length);
    for (Int32 i = 0; i < length; i++) {
        whereArgs->Set(i, StringUtils::Int64ToString((*ids)[i]));
    }
    return whereArgs;
}

} // namespace App
} // namespace Droid
} // namespace Elastos
