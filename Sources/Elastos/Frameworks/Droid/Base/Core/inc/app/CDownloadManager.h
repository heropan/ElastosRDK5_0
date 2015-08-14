
#ifndef __ELASTOS_DROID_APP_CDOWNLOADMANAGER_H__
#define __ELASTOS_DROID_APP_CDOWNLOADMANAGER_H__

#include "_CDownloadManager.h"
#include "database/CursorWrapper.h"

using Elastos::Droid::Content::IContext;
using Elastos::Droid::Database::ICursor;
using Elastos::Droid::Database::CursorWrapper;
using Elastos::Droid::Database::ICursorWrapper;
using Elastos::Droid::Database::ICharArrayBuffer;
using Elastos::Droid::Database::IContentObserver;
using Elastos::Droid::Database::IDataSetObserver;
using Elastos::Droid::Os::IParcelFileDescriptor;

namespace Elastos {
namespace Droid {
namespace App {

CarClass(CDownloadManager)
{
private:
    class CursorTranslator
        : public ElRefBase
        , public CursorWrapper
        , public ICursorWrapper
    {
    public:
        CAR_INTERFACE_DECL();

        CursorTranslator(
            /* [in] */ ICursor* cursor,
            /* [in] */ IUri* baseUri);

        CARAPI GetInt32(
             /* [in] */ Int32 columnIndex,
             /* [out] */ Int32* value);

        CARAPI GetInt64(
            /* [in] */ Int32 columnIndex,
            /* [out] */ Int64* value);

        CARAPI GetString(
            /* [in] */ Int32 columnIndex,
            /* [out] */ String* value);

        CARAPI GetWrappedCursor(
            /* [out] */ ICursor** cursor);

        CARAPI Close();

        CARAPI IsClosed(
            /* [out] */ Boolean* isClosed);

        CARAPI GetCount(
            /* [out] */ Int32* count);

        CARAPI Deactivate();

        CARAPI MoveToFirst(
            /* [out] */ Boolean* succeeded);

        CARAPI GetColumnCount(
            /* [out] */ Int32* count);

        CARAPI GetColumnIndex(
            /* [in] */ const String& columnName,
            /* [out] */ Int32* index);

        CARAPI GetColumnIndexOrThrow(
            /* [in] */  const String& columnName,
            /* [out] */ Int32* columnIndex);

        CARAPI GetColumnName(
            /* [in] */ Int32 columnIndex,
            /* [out] */ String* name);

        CARAPI GetColumnNames(
            /* [out, callee] */ ArrayOf<String>** columnNames);

        CARAPI GetDouble(
            /* [in] */ Int32 columnIndex,
            /* [out] */ Double* value);

        CARAPI GetExtras(
            /* [out] */ IBundle** extras);

        CARAPI GetFloat(
            /* [in] */ Int32 columnIndex,
            /* [out] */ Float* value);

        CARAPI GetInt16(
            /* [in] */ Int32 columnIndex,
            /* [out] */ Int16* value);

        CARAPI CopyStringToBuffer(
            /* [in] */ Int32 columnIndex,
            /* [in] */ ICharArrayBuffer* buffer);

        CARAPI GetBlob(
            /* [in] */  Int32 columnIndex,
            /* [out,callee] */ ArrayOf<Byte>** blob);

        CARAPI GetWantsAllOnMoveCalls(
            /* [out] */ Boolean* value);

        CARAPI IsAfterLast(
            /* [out] */ Boolean* result);

        CARAPI IsBeforeFirst(
            /* [out] */ Boolean* result);

        CARAPI IsFirst(
            /* [out] */ Boolean* result);

        CARAPI IsLast(
            /* [out] */ Boolean* result);

        CARAPI GetType(
            /* [in] */ Int32 columnIndex,
            /* [out] */ Int32* type);

        CARAPI IsNull(
            /* [in] */ Int32 columnIndex,
            /* [out] */ Boolean* result);

        CARAPI MoveToLast(
            /* [out] */ Boolean* succeeded);

        CARAPI Move(
            /* [in] */ Int32 offset,
            /* [out] */ Boolean* succeeded);

        CARAPI MoveToPosition(
            /* [in] */ Int32 position,
            /* [out] */ Boolean* succeeded);

        CARAPI MoveToNext(
            /* [out] */ Boolean* succeeded);

        CARAPI GetPosition(
            /*[out]*/ Int32* position);

        CARAPI MoveToPrevious(
            /* [out] */ Boolean* succeeded);

        CARAPI RegisterContentObserver(
            /* [in] */ IContentObserver* observer);

        CARAPI RegisterDataSetObserver(
            /* [in] */IDataSetObserver* observer);

        CARAPI Requery(
            /* [out] */ Boolean* succeeded);

        CARAPI Respond(
            /* [in] */ IBundle* extras,
            /* [out] */ IBundle** bundle);

        CARAPI SetNotificationUri(
            /* [in] */ IContentResolver* cr,
            /* [in] */ IUri* uri);

        CARAPI UnregisterContentObserver(
            /* [in] */ IContentObserver* observer);

        CARAPI UnregisterDataSetObserver(
            /* [in] */ IDataSetObserver* observer);

    private:
        CARAPI_(String) GetLocalUri();

        CARAPI_(Int64) GetReason(
            /* [in] */ Int32 status);

        CARAPI_(Int64) GetPausedReason(
            /* [in] */ Int32 status);

        CARAPI_(Int64) GetErrorCode(
            /* [in] */ Int32 status);

        CARAPI_(Int32) TranslateStatus(
            /* [in] */ Int32 status);

    private:
        AutoPtr<IUri> mBaseUri;
    };

public:
    CARAPI constructor(
        /* [in] */ IContentResolver* resolver,
        /* [in] */ const String& packageName);

    /**
     * Makes this object access the download provider through /all_downloads URIs rather than
     * /my_downloads URIs, for clients that have permission to do so.
     * @hide
     */
    CARAPI SetAccessAllDownloads(
        /* [in] */ Boolean accessAllDownloads);

    /**
     * Enqueue a new download.  The download will start automatically once the download manager is
     * ready to execute it and connectivity is available.
     *
     * @param request the parameters specifying this download
     * @return an ID for the download, unique across the system.  This ID is used to make future
     * calls related to this download.
     */
    CARAPI Enqueue(
        /* [in] */ IDownloadManagerRequest* request,
        /* [out] */ Int64* id);

    /**
     * Marks the specified download as 'to be deleted'. This is done when a completed download
     * is to be removed but the row was stored without enough info to delete the corresponding
     * metadata from Mediaprovider database. Actual cleanup of this row is done in DownloadService.
     *
     * @param ids the IDs of the downloads to be marked 'deleted'
     * @return the number of downloads actually updated
     * @hide
     */
    CARAPI MarkRowDeleted(
        /* [in] */ ArrayOf<Int64>* ids,
        /* [out] */ Int32* number);

    /**
     * Cancel downloads and remove them from the download manager.  Each download will be stopped if
     * it was running, and it will no longer be accessible through the download manager.
     * If there is a downloaded file, partial or complete, it is deleted.
     *
     * @param ids the IDs of the downloads to remove
     * @return the number of downloads actually removed
     */
    CARAPI Remove(
        /* [in] */ ArrayOf<Int64>* ids,
        /* [out] */ Int32* number);

    /**
     * Query the download manager about downloads that have been requested.
     * @param query parameters specifying filters for this query
     * @return a Cursor over the result set of downloads, with columns consisting of all the
     * COLUMN_* constants.
     */
    CARAPI Query(
        /* [in] */ IDownloadManagerQuery* query,
        /* [out] */ ICursor** cursor);

    /**
     * Open a downloaded file for reading.  The download must have completed.
     * @param id the ID of the download
     * @return a read-only {@link ParcelFileDescriptor}
     * @throws FileNotFoundException if the destination file does not already exist
     */
    CARAPI OpenDownloadedFile(
        /* [in] */ Int64 id,
        /* [out] */ IParcelFileDescriptor** descriptor);

    /**
     * Returns {@link Uri} for the given downloaded file id, if the file is
     * downloaded successfully. otherwise, null is returned.
     *<p>
     * If the specified downloaded file is in external storage (for example, /sdcard dir),
     * then it is assumed to be safe for anyone to read and the returned {@link Uri} corresponds
     * to the filepath on sdcard.
     *
     * @param id the id of the downloaded file.
     * @return the {@link Uri} for the given downloaded file id, if download was successful. null
     * otherwise.
     */
    CARAPI GetUriForDownloadedFile(
        /* [in] */ Int64 id,
        /* [out] */ IUri** uri);

    /**
     * Returns {@link Uri} for the given downloaded file id, if the file is
     * downloaded successfully. otherwise, null is returned.
     *<p>
     * If the specified downloaded file is in external storage (for example, /sdcard dir),
     * then it is assumed to be safe for anyone to read and the returned {@link Uri} corresponds
     * to the filepath on sdcard.
     *
     * @param id the id of the downloaded file.
     * @return the {@link Uri} for the given downloaded file id, if download was successful. null
     * otherwise.
     */
    CARAPI GetMimeTypeForDownloadedFile(
        /* [in] */ Int64 id,
        /* [out] */ String* uri);

    /**
     * Restart the given downloads, which must have already completed (successfully or not).  This
     * method will only work when called from within the download manager's process.
     * @param ids the IDs of the downloads
     * @hide
     */
    CARAPI RestartDownload(
        /* [in] */ ArrayOf<Int64>* ids);

    static CARAPI GetMaxBytesOverMobile(
        /* [in] */ IContext* context,
        /* [out] */ Int64* size);

    static CARAPI GetRecommendedMaxBytesOverMobile(
        /* [in] */ IContext* context,
        /* [out] */ Int64* size);

    static CARAPI IsActiveNetworkExpensive(
        /* [in] */ IContext* context,
        /* [out] */ Boolean* actived);

    static CARAPI GetActiveNetworkWarningBytes(
        /* [in] */ IContext* context,
        /* [out] */ Int64* bytes);

    /**
     * Adds a file to the downloads database system, so it could appear in Downloads App
     * (and thus become eligible for management by the Downloads App).
     * <p>
     * It is helpful to make the file scannable by MediaScanner by setting the param
     * isMediaScannerScannable to true. It makes the file visible in media managing
     * applications such as Gallery App, which could be a useful purpose of using this API.
     *
     * @param title the title that would appear for this file in Downloads App.
     * @param description the description that would appear for this file in Downloads App.
     * @param isMediaScannerScannable true if the file is to be scanned by MediaScanner. Files
     * scanned by MediaScanner appear in the applications used to view media (for example,
     * Gallery app).
     * @param mimeType mimetype of the file.
     * @param path absolute pathname to the file. The file should be world-readable, so that it can
     * be managed by the Downloads App and any other app that is used to read it (for example,
     * Gallery app to display the file, if the file contents represent a video/image).
     * @param length length of the downloaded file
     * @param showNotification true if a notification is to be sent, false otherwise
     * @return  an ID for the download entry added to the downloads app, unique across the system
     * This ID is used to make future calls related to this download.
     */
    CARAPI AddCompletedDownload(
        /* [in] */ const String& title,
        /* [in] */ const String& description,
        /* [in] */ Boolean isMediaScannerScannable,
        /* [in] */ const String& mimeType,
        /* [in] */ const String& path,
        /* [in] */ Int64 length,
        /* [in] */ Boolean showNotification,
        /* [out] */ Int64* id);

    /**
     * Get the DownloadProvider URI for the download with the given ID.
     */
    CARAPI_(AutoPtr<IUri>) GetDownloadUri(
        /* [in] */ Int64 id);

    /**
     * Get a parameterized SQL WHERE clause to select a bunch of IDs.
     */
    static CARAPI_(String) GetWhereClauseForIds(
        /* [in] */ ArrayOf<Int64>* ids);

    /**
     * Get the selection args for a clause returned by {@link #getWhereClauseForIds(long[])}.
     */
    static CARAPI_(AutoPtr< ArrayOf<String> >) GetWhereArgsForIds(
        /* [in] */ ArrayOf<Int64>* ids);

private:
    static CARAPI ValidateArgumentIsNonEmpty(
        /* [in] */ const String& paramName,
        /* [in] */ const String& val);

    static CARAPI_(AutoPtr<ArrayOf<String> >) InitUNDERLYINGCOLUMNS();

public:
    /**
     * columns to request from DownloadProvider.
     * @hide
     */
    static const AutoPtr<ArrayOf<String> > UNDERLYING_COLUMNS;

private:
    AutoPtr<IContentResolver> mResolver;
    String mPackageName;
    AutoPtr<IUri> mBaseUri;

    static const String NON_DOWNLOADMANAGER_DOWNLOAD;
};

} // namespace App
} // namespace Droid
} // namespace Elastos

#endif //__ELASTOS_DROID_APP_CDOWNLOADMANAGER_H__
