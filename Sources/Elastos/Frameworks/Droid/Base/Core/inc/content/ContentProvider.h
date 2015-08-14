
#ifndef __ELASTOS_DROID_CONTENT_CONTENTPROVIDER_H__
#define __ELASTOS_DROID_CONTENT_CONTENTPROVIDER_H__

#include "ext/frameworkext.h"
#include "os/AsyncTask.h"

using Elastos::IO::IFileDescriptor;
using Elastos::IO::IPrintWriter;

using Elastos::Droid::Content::Pm::IProviderInfo;
using Elastos::Droid::Content::Pm::IPathPermission;
using Elastos::Droid::Content::Res::IConfiguration;
using Elastos::Droid::Content::Res::IAssetFileDescriptor;
using Elastos::Droid::Database::ICursor;
using Elastos::Droid::Net::IUri;
using Elastos::Droid::Os::ICancellationSignal;
using Elastos::Droid::Os::IBundle;
using Elastos::Droid::Os::IParcelFileDescriptor;
using Elastos::Droid::Os::AsyncTask;

namespace Elastos {
namespace Droid {
namespace Content {

class ContentProvider
    : public ElRefBase
    , public IObject
    , public IContentProvider
    , public IWeakReferenceSource
{
private:
    class OpenPipeAsyncTask : public AsyncTask
    {
    public:
        OpenPipeAsyncTask(
            /* [in] */ IContentProviderPipeDataWriter* func,
            /* [in] */ ArrayOf<IParcelFileDescriptor*>* fds,
            /* [in] */ IUri* uri,
            /* [in] */ const String& mimeType,
            /* [in] */ IBundle* opts,
            /* [in] */ IInterface* args);

        CARAPI_(AutoPtr<IInterface>) DoInBackground(
            /* [in] */ ArrayOf<IInterface*>* params);

    private:
        AutoPtr<IContentProviderPipeDataWriter> mFunc;
        AutoPtr< ArrayOf<IParcelFileDescriptor*> > mFds;
        AutoPtr<IUri> mUri;
        String mMimeType;
        AutoPtr<IBundle> mOpts;
        AutoPtr<IInterface> mArgs;
    };

public:
    /**
     * Construct a ContentProvider instance.  Content providers must be
     * <a href="{@docRoot}guide/topics/manifest/provider-element.html">declared
     * in the manifest</a>, accessed with {@link ContentResolver}, and created
     * automatically by the system, so applications usually do not create
     * ContentProvider instances directly.
     *
     * <p>At construction time, the object is uninitialized, and most fields and
     * methods are unavailable.  Subclasses should initialize themselves in
     * {@link #onCreate}, not the constructor.
     *
     * <p>Content providers are created on the application main thread at
     * application launch time.  The constructor must not perform lengthy
     * operations, or application startup will be delayed.
     */
    ContentProvider();

    virtual ~ContentProvider();

    CAR_INTERFACE_DECL()

    CARAPI Initialize();

    CARAPI Aggregate(
        /* [in] */ AggrType aggrType,
        /* [in] */ PInterface pObject);

    CARAPI GetDomain(
        /* [out] */ PInterface *ppObject);

    CARAPI GetClassID(
        /* [out] */ ClassID *pCLSID);

    CARAPI Equals(
        /* [in] */ IInterface* other,
        /* [out] */ Boolean * result);

    CARAPI GetHashCode(
        /* [out] */ Int32* hash);

    CARAPI ToString(
        /* [out] */ String* info);

    CARAPI GetWeakReference(
        /* [out] */ IWeakReference** weakReference);

    /**
     * Retrieves the Context this provider is running in.  Only available once
     * {@link #onCreate} has been called -- this will return null in the
     * constructor.
     */
    CARAPI GetContext(
        /* [out] */ IContext** context);

    /**
     * Return the name of the permission required for read-only access to
     * this content provider.  This method can be called from multiple
     * threads, as described in
     * <a href="{@docRoot}guide/topics/fundamentals/processes-and-threads.html#Threads">Processes
     * and Threads</a>.
     */
    CARAPI GetReadPermission(
        /* [out] */ String* permissionName);

    /**
     * Return the name of the permission required for read/write access to
     * this content provider.  This method can be called from multiple
     * threads, as described in
     * <a href="{@docRoot}guide/topics/fundamentals/processes-and-threads.html#Threads">Processes
     * and Threads</a>.
     */
    CARAPI GetWritePermission(
        /* [out] */ String* permissionName);

    /**
     * Return the path-based permissions required for read and/or write access to
     * this content provider.  This method can be called from multiple
     * threads, as described in
     * <a href="{@docRoot}guide/topics/fundamentals/processes-and-threads.html#Threads">Processes
     * and Threads</a>.
     */
    CARAPI GetPathPermissions(
        /* [out, callee] */ ArrayOf<IPathPermission*>** pathPermissions);

    /**
     * {@inheritDoc}
     * This method is always called on the application main thread, and must
     * not perform lengthy operations.
     *
     * <p>The default content provider implementation does nothing.
     * Override this method to take appropriate action.
     * (Content providers do not usually care about things like screen
     * orientation, but may want to know about locale changes.)
     */
    CARAPI OnConfigurationChanged(
        /* [in] */ IConfiguration* newConfig);

    /**
     * {@inheritDoc}
     * This method is always called on the application main thread, and must
     * not perform lengthy operations.
     *
     * <p>The default content provider implementation does nothing.
     * Subclasses may override this method to take appropriate action.
     */
    CARAPI OnLowMemory();

    CARAPI OnTrimMemory(
        /* [in] */ Int32 level);

    /**
     * Implement this to handle query requests from clients with support for cancellation.
     * This method can be called from multiple threads, as described in
     * <a href="{@docRoot}guide/topics/fundamentals/processes-and-threads.html#Threads">Processes
     * and Threads</a>.
     * <p>
     * Example client call:<p>
     * <pre>// Request a specific record.
     * Cursor managedCursor = managedQuery(
                ContentUris.withAppendedId(Contacts.People.CONTENT_URI, 2),
                projection,    // Which columns to return.
                null,          // WHERE clause.
                null,          // WHERE clause value substitution
                People.NAME + " ASC");   // Sort order.</pre>
     * Example implementation:<p>
     * <pre>// SQLiteQueryBuilder is a helper class that creates the
        // proper SQL syntax for us.
        SQLiteQueryBuilder qBuilder = new SQLiteQueryBuilder();

        // Set the table we're querying.
        qBuilder.setTables(DATABASE_TABLE_NAME);

        // If the query ends in a specific record number, we're
        // being asked for a specific record, so set the
        // WHERE clause in our query.
        if((URI_MATCHER.match(uri)) == SPECIFIC_MESSAGE){
            qBuilder.appendWhere("_id=" + uri.getPathLeafId());
        }

        // Make the query.
        Cursor c = qBuilder.query(mDb,
                projection,
                selection,
                selectionArgs,
                groupBy,
                having,
                sortOrder);
        c.setNotificationUri(getContext().getContentResolver(), uri);
        return c;</pre>
     * <p>
     * If you implement this method then you must also implement the version of
     * {@link #query(Uri, String[], String, String[], String)} that does not take a cancellation
     * signal to ensure correct operation on older versions of the Android Framework in
     * which the cancellation signal overload was not available.
     *
     * @param uri The URI to query. This will be the full URI sent by the client;
     *      if the client is requesting a specific record, the URI will end in a record number
     *      that the implementation should parse and add to a WHERE or HAVING clause, specifying
     *      that _id value.
     * @param projection The list of columns to put into the cursor. If
     *      null all columns are included.
     * @param selection A selection criteria to apply when filtering rows.
     *      If null then all rows are included.
     * @param selectionArgs You may include ?s in selection, which will be replaced by
     *      the values from selectionArgs, in order that they appear in the selection.
     *      The values will be bound as Strings.
     * @param sortOrder How the rows in the cursor should be sorted.
     *      If null then the provider is free to define the sort order.
     * @param cancellationSignal A signal to cancel the operation in progress, or null if none.
     * If the operation is canceled, then {@link OperationCanceledException} will be thrown
     * when the query is executed.
     * @return a Cursor or null.
     */
    CARAPI Query(
        /* [in] */ IUri* uri,
        /* [in] */ ArrayOf<String>* projection,
        /* [in] */ const String& selection,
        /* [in] */ ArrayOf<String>* selectionArgs,
        /* [in] */ const String& sortOrder,
        /* [in] */ ICancellationSignal* cancellationSignal,
        /* [out] */ ICursor** cursor);

    /**
     * Override this to handle requests to insert a set of new rows, or the
     * default implementation will iterate over the values and call
     * {@link #insert} on each of them.
     * As a courtesy, call {@link ContentResolver#notifyChange(android.net.Uri ,android.database.ContentObserver) notifyChange()}
     * after inserting.
     * This method can be called from multiple threads, as described in
     * <a href="{@docRoot}guide/topics/fundamentals/processes-and-threads.html#Threads">Processes
     * and Threads</a>.
     *
     * @param uri The content:// URI of the insertion request.
     * @param values An array of sets of column_name/value pairs to add to the database.
     * @return The number of values that were inserted.
     */
    CARAPI BulkInsert(
        /* [in] */ IUri* uri,
        /* [in] */ ArrayOf<IContentValues*>* values,
        /* [out] */ Int32* number);

    /**
     * Override this to handle requests to open a file blob.
     * The default implementation always throws {@link FileNotFoundException}.
     * This method can be called from multiple threads, as described in
     * <a href="{@docRoot}guide/topics/fundamentals/processes-and-threads.html#Threads">Processes
     * and Threads</a>.
     *
     * <p>This method returns a ParcelFileDescriptor, which is returned directly
     * to the caller.  This way large data (such as images and documents) can be
     * returned without copying the content.
     *
     * <p>The returned ParcelFileDescriptor is owned by the caller, so it is
     * their responsibility to close it when done.  That is, the implementation
     * of this method should create a new ParcelFileDescriptor for each call.
     *
     * @param uri The URI whose file is to be opened.
     * @param mode Access mode for the file.  May be "r" for read-only access,
     * "rw" for read and write access, or "rwt" for read and write access
     * that truncates any existing file.
     *
     * @return Returns a new ParcelFileDescriptor which you can use to access
     * the file.
     *
     * @throws FileNotFoundException Throws FileNotFoundException if there is
     * no file associated with the given URI or the mode is invalid.
     * @throws SecurityException Throws SecurityException if the caller does
     * not have permission to access the file.
     *
     * @see #openAssetFile(Uri, String)
     * @see #openFileHelper(Uri, String)
     */
    CARAPI OpenFile(
        /* [in] */ IUri* uri,
        /* [in] */ const String& mode,
        /* [out] */ IParcelFileDescriptor** fileDescriptor);

    /**
     * This is like {@link #openFile}, but can be implemented by providers
     * that need to be able to return sub-sections of files, often assets
     * inside of their .apk.
     * This method can be called from multiple threads, as described in
     * <a href="{@docRoot}guide/topics/fundamentals/processes-and-threads.html#Threads">Processes
     * and Threads</a>.
     *
     * <p>If you implement this, your clients must be able to deal with such
     * file slices, either directly with
     * {@link ContentResolver#openAssetFileDescriptor}, or by using the higher-level
     * {@link ContentResolver#openInputStream ContentResolver.openInputStream}
     * or {@link ContentResolver#openOutputStream ContentResolver.openOutputStream}
     * methods.
     *
     * <p class="note">If you are implementing this to return a full file, you
     * should create the AssetFileDescriptor with
     * {@link AssetFileDescriptor#UNKNOWN_LENGTH} to be compatible with
     * applications that can not handle sub-sections of files.</p>
     *
     * @param uri The URI whose file is to be opened.
     * @param mode Access mode for the file.  May be "r" for read-only access,
     * "w" for write-only access (erasing whatever data is currently in
     * the file), "wa" for write-only access to append to any existing data,
     * "rw" for read and write access on any existing data, and "rwt" for read
     * and write access that truncates any existing file.
     *
     * @return Returns a new AssetFileDescriptor which you can use to access
     * the file.
     *
     * @throws FileNotFoundException Throws FileNotFoundException if there is
     * no file associated with the given URI or the mode is invalid.
     * @throws SecurityException Throws SecurityException if the caller does
     * not have permission to access the file.
     *
     * @see #openFile(Uri, String)
     * @see #openFileHelper(Uri, String)
     */
    CARAPI OpenAssetFile(
        /* [in] */ IUri* uri,
        /* [in] */ const String& mode,
        /* [out] */ IAssetFileDescriptor** fileDescriptor);

    /**
     * Called by a client to determine the types of data streams that this
     * content provider supports for the given URI.  The default implementation
     * returns null, meaning no types.  If your content provider stores data
     * of a particular type, return that MIME type if it matches the given
     * mimeTypeFilter.  If it can perform type conversions, return an array
     * of all supported MIME types that match mimeTypeFilter.
     *
     * @param uri The data in the content provider being queried.
     * @param mimeTypeFilter The type of data the client desires.  May be
     * a pattern, such as * \/ * to retrieve all possible data types.
     * @return Returns null if there are no possible data streams for the
     * given mimeTypeFilter.  Otherwise returns an array of all available
     * concrete MIME types.
     *
     * @see #getType(Uri)
     * @see #openTypedAssetFile(Uri, String, Bundle)
     * @see ClipDescription#compareMimeTypes(String, String)
     */
    CARAPI GetStreamTypes(
        /* [in] */ IUri* uri,
        /* [in] */ const String& mimeTypeFilter,
        /* [out, callee] */ ArrayOf<String>** streamTypes);

    /**
     * Called by a client to open a read-only stream containing data of a
     * particular MIME type.  This is like {@link #openAssetFile(Uri, String)},
     * except the file can only be read-only and the content provider may
     * perform data conversions to generate data of the desired type.
     *
     * <p>The default implementation compares the given mimeType against the
     * result of {@link #getType(Uri)} and, if the match, simple calls
     * {@link #openAssetFile(Uri, String)}.
     *
     * <p>See {@link ClipData} for examples of the use and implementation
     * of this method.
     *
     * @param uri The data in the content provider being queried.
     * @param mimeTypeFilter The type of data the client desires.  May be
     * a pattern, such as * \/ *, if the caller does not have specific type
     * requirements; in this case the content provider will pick its best
     * type matching the pattern.
     * @param opts Additional options from the client.  The definitions of
     * these are specific to the content provider being called.
     *
     * @return Returns a new AssetFileDescriptor from which the client can
     * read data of the desired type.
     *
     * @throws FileNotFoundException Throws FileNotFoundException if there is
     * no file associated with the given URI or the mode is invalid.
     * @throws SecurityException Throws SecurityException if the caller does
     * not have permission to access the data.
     * @throws IllegalArgumentException Throws IllegalArgumentException if the
     * content provider does not support the requested MIME type.
     *
     * @see #getStreamTypes(Uri, String)
     * @see #openAssetFile(Uri, String)
     * @see ClipDescription#compareMimeTypes(String, String)
     */
    CARAPI OpenTypedAssetFile(
        /* [in] */ IUri* uri,
        /* [in] */ const String& mimeTypeFilter,
        /* [in] */ IBundle* opts,
        /* [out] */ IAssetFileDescriptor** fileDescriptor);

    /**
     * A helper function for implementing {@link #openTypedAssetFile}, for
     * creating a data pipe and background thread allowing you to stream
     * generated data back to the client.  This function returns a new
     * ParcelFileDescriptor that should be returned to the caller (the caller
     * is responsible for closing it).
     *
     * @param uri The URI whose data is to be written.
     * @param mimeType The desired type of data to be written.
     * @param opts Options supplied by caller.
     * @param args Your own custom arguments.
     * @param func Interface implementing the function that will actually
     * stream the data.
     * @return Returns a new ParcelFileDescriptor holding the read side of
     * the pipe.  This should be returned to the caller for reading; the caller
     * is responsible for closing it when done.
     */
    CARAPI OpenPipeHelper(
        /* [in] */ IUri* uri,
        /* [in] */ const String& mimeType,
        /* [in] */ IBundle* opts,
        /* [in] */ IInterface* args,
        /* [in] */ IContentProviderPipeDataWriter* func,
        /* [out] */ IParcelFileDescriptor** fileDescriptor);

    /**
     * Returns the Binder object for this provider.
     *
     * @return the Binder object for this provider
     * @hide
     */
    CARAPI GetIContentProvider(
        /* [out] */ IIContentProvider** contentProvider);

    /**
     * After being instantiated, this is called to tell the content provider
     * about itself.
     *
     * @param context The context this provider is running in
     * @param info Registered information about this content provider
     */
    CARAPI AttachInfo(
        /* [in] */ IContext* context,
        /* [in] */ IProviderInfo* info);

    /**
     * Override this to handle requests to perform a batch of operations, or the
     * default implementation will iterate over the operations and call
     * {@link ContentProviderOperation#apply} on each of them.
     * If all calls to {@link ContentProviderOperation#apply} succeed
     * then a {@link ContentProviderResult} array with as many
     * elements as there were operations will be returned.  If any of the calls
     * fail, it is up to the implementation how many of the others take effect.
     * This method can be called from multiple threads, as described in
     * <a href="{@docRoot}guide/topics/fundamentals/processes-and-threads.html#Threads">Processes
     * and Threads</a>.
     *
     * @param operations the operations to apply
     * @return the results of the applications
     * @throws OperationApplicationException thrown if any operation fails.
     * @see ContentProviderOperation#apply
     */
    CARAPI ApplyBatch(
        /* [in] */ IObjectContainer* operations,
        /* [out, callee] */ ArrayOf<IContentProviderResult*>** providerResults);

    /**
     * Call a provider-defined method.  This can be used to implement
     * interfaces that are cheaper and/or unnatural for a table-like
     * model.
     *
     * @param method method name to call.  Opaque to framework, but should not be null.
     * @param arg provider-defined String argument.  May be null.
     * @param extras provider-defined Bundle argument.  May be null.
     * @return provider-defined return value.  May be null.  Null is also
     *   the default for providers which don't implement any call methods.
     */
    CARAPI Call(
        /* [in] */ const String& method,
        /* [in] */ const String& arg,
        /* [in] */ IBundle* extras,
        /* [out] */ IBundle** bundle);

    /**
     * Implement this to shut down the ContentProvider instance. You can then
     * invoke this method in unit tests.
     *
     * <p>
     * Android normally handles ContentProvider startup and shutdown
     * automatically. You do not need to start up or shut down a
     * ContentProvider. When you invoke a test method on a ContentProvider,
     * however, a ContentProvider instance is started and keeps running after
     * the test finishes, even if a succeeding test instantiates another
     * ContentProvider. A conflict develops because the two instances are
     * usually running against the same underlying data source (for example, an
     * sqlite database).
     * </p>
     * <p>
     * Implementing shutDown() avoids this conflict by providing a way to
     * terminate the ContentProvider. This method can also prevent memory leaks
     * from multiple instantiations of the ContentProvider, and it can ensure
     * unit test isolation by allowing you to completely clean up the test
     * fixture before moving on to the next test.
     * </p>
     */
    CARAPI Shutdown();

    /**
     * Print the Provider's state into the given stream.  This gets invoked if
     * you run "adb shell dumpsys activity provider &lt;provider_component_name&gt;".
     *
     * @param prefix Desired prefix to prepend at each line of output.
     * @param fd The raw file descriptor that the dump is being sent to.
     * @param writer The PrintWriter to which you should dump your state.  This will be
     * closed for you after you return.
     * @param args additional arguments to the dump request.
     * @hide
     */
    CARAPI Dump(
        /* [in] */ IFileDescriptor* fd,
        /* [in] */ IPrintWriter* writer,
        /* [in] */ ArrayOf<String>* args);

    static CARAPI CoerceToLocalContentProvider(
        /* [in] */ IIContentProvider* abstractInterface,
        /* [out] */ IContentProvider** provider);

protected:
    CARAPI Init(
        /* [in] */ IContext* context,
        /* [in] */ const String& readPermission,
        /* [in] */ const String& writePermission,
        /* [in] */ ArrayOf<IPathPermission*>* pathPermissions);

    /**
     * Change the permission required to read data from the content
     * provider.  This is normally set for you from its manifest information
     * when the provider is first created.
     *
     * @param permission Name of the permission required for read-only access.
     */
    CARAPI SetReadPermission(
        /* [in] */ const String& permission);

    /**
     * Change the permission required to read and write data in the content
     * provider.  This is normally set for you from its manifest information
     * when the provider is first created.
     *
     * @param permission Name of the permission required for read/write access.
     */
    CARAPI SetWritePermission(
        /* [in] */ const String& permission);

    /**
     * Change the path-based permission required to read and/or write data in
     * the content provider.  This is normally set for you from its manifest
     * information when the provider is first created.
     *
     * @param permissions Array of path permission descriptions.
     */
    CARAPI SetPathPermissions(
        /* [in] */ ArrayOf<IPathPermission*>* permissions);

    /**
     * Convenience for subclasses that wish to implement {@link #openFile}
     * by looking up a column named "_data" at the given URI.
     *
     * @param uri The URI to be opened.
     * @param mode The file mode.  May be "r" for read-only access,
     * "w" for write-only access (erasing whatever data is currently in
     * the file), "wa" for write-only access to append to any existing data,
     * "rw" for read and write access on any existing data, and "rwt" for read
     * and write access that truncates any existing file.
     *
     * @return Returns a new ParcelFileDescriptor that can be used by the
     * client to access the file.
     */
    CARAPI OpenFileHelper(
        /* [in] */ IUri* uri,
        /* [in] */ const String& mode,
        /* [out] */ IParcelFileDescriptor** fileDescriptor);

    /**
     * Returns true if this instance is a temporary content provider.
     * @return true if this instance is a temporary content provider
     */
    virtual CARAPI_(Boolean) IsTemporary();

private:
    friend class CContentProviderTransport;

    static const String TAG;

    AutoPtr<IContext> mContext;
    Int32 mMyUid;
    String mReadPermission;
    String mWritePermission;
    AutoPtr< ArrayOf<IPathPermission*> > mPathPermissions;
    Boolean mExported;
    AutoPtr<IContentProviderTransport> mTransport;
};

} // namespace Content
} // namespace Droid
} // namespace Elastos

#endif //__CCONTENTPROVIDER_H__
