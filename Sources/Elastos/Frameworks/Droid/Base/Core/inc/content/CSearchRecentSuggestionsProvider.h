
#ifndef __CSEARCHRECENTSUGGESTIONSPROVIDER_H__
#define __CSEARCHRECENTSUGGESTIONSPROVIDER_H__

#include "_CSearchRecentSuggestionsProvider.h"
#include <ext/frameworkext.h>
#include "ContentProvider.h"

using Elastos::Droid::Content::Pm::IPathPermission;
using Elastos::Droid::Content::Pm::IProviderInfo;
using Elastos::Droid::Content::Res::IConfiguration;
using Elastos::Droid::Content::Res::IAssetFileDescriptor;
using Elastos::Droid::Database::ICursor;
using Elastos::Droid::Database::Sqlite::ISQLiteDatabase;
using Elastos::Droid::Database::Sqlite::ISQLiteOpenHelper;
using Elastos::Droid::Net::IUri;
using Elastos::Droid::Os::IBundle;
using Elastos::Droid::Os::ICancellationSignal;
using Elastos::Droid::Os::IParcelFileDescriptor;
using Elastos::IO::IFileDescriptor;
using Elastos::IO::IPrintWriter;

namespace Elastos {
namespace Droid {
namespace Content {

class CSearchRecentSuggestionsProvider : public ContentProvider
{
    /**
     * Builds the database.  This version has extra support for using the version field
     * as a mode flags field, and configures the database columns depending on the mode bits
     * (features) requested by the extending class.
     *
     * @hide
     */
    class DatabaseHelper
        // : public SQLiteOpenHelper
    {
    public:
        DatabaseHelper(
            /*[in]*/ IContext* context,
            /*[in]*/ Int32 newVersion);

        ~DatabaseHelper();

        CARAPI OnCreate(
            /*[in]*/ ISQLiteDatabase* db);

        CARAPI OnUpgrade(
            /*[in]*/ ISQLiteDatabase* db,
            /*[in]*/ Int32 oldVersion,
            /*[in]*/ Int32 newVersion);

    private:
        Int32 mNewVersion;

    };

public:
    CSearchRecentSuggestionsProvider();

    ~CSearchRecentSuggestionsProvider();

    CARAPI OnConfigurationChanged(
        /* [in] */ IConfiguration* newConfig);

    CARAPI OnLowMemory();

    CARAPI OnTrimMemory(
        /* [in] */ Int32 level);

    CARAPI GetContext(
        /* [out] */ IContext** context);

    CARAPI GetReadPermission(
        /* [out] */ String* permissionName);

    CARAPI GetWritePermission(
        /* [out] */ String* permissionName);

    CARAPI GetPathPermissions(
        /* [out, callee] */ ArrayOf<IPathPermission *>** pathPermissions);

    /**
     * This method is provided for use by the ContentResolver.  Do not override, or directly
     * call from your own code.
     */
    // @Override
    CARAPI OnCreate(
        /* [out] */ Boolean* succeeded);

    /**
     * This method is provided for use by the ContentResolver.  Do not override, or directly
     * call from your own code.
     */
    // TODO: Confirm no injection attacks here, or rewrite.
    // @Override
    CARAPI Query(
        /* [in] */ IUri* uri,
        /* [in] */ ArrayOf<String>* projection,
        /* [in] */ const String& selection,
        /* [in] */ ArrayOf<String>* selectionArgs,
        /* [in] */ const String& sortOrder,
        /* [out] */ ICursor** cursor);

    CARAPI Query(
        /* [in] */ IUri* uri,
        /* [in] */ ArrayOf<String>* projection,
        /* [in] */ const String& selection,
        /* [in] */ ArrayOf<String>* selectionArgs,
        /* [in] */ const String& sortOrder,
        /* [in] */ ICancellationSignal* cancellationSignal,
        /* [out] */ ICursor** cursor);

    /**
     * This method is provided for use by the ContentResolver.  Do not override, or directly
     * call from your own code.
     */
    // @Override
    CARAPI GetType(
        /* [in] */ IUri* uri,
        /* [out] */ String* type);

    /**
     * This method is provided for use by the ContentResolver.  Do not override, or directly
     * call from your own code.
     */
    // @Override
    CARAPI Insert(
        /* [in] */ IUri* uri,
        /* [in] */ IContentValues* values,
        /* [out] */ IUri** insertedUri);

    CARAPI BulkInsert(
        /* [in] */ IUri* uri,
        /* [in] */ ArrayOf<IContentValues *>* values,
        /* [out] */ Int32* number);

    /**
     * This method is provided for use by the ContentResolver.  Do not override, or directly
     * call from your own code.
     */
    // @Override
    CARAPI Delete(
        /* [in] */ IUri* uri,
        /* [in] */ const String& selection,
        /* [in] */ ArrayOf<String>* selectionArgs,
        /* [out] */ Int32* rowsAffected);

    /**
     * This method is provided for use by the ContentResolver.  Do not override, or directly
     * call from your own code.
     */
    // @Override
    CARAPI Update(
        /* [in] */ IUri* uri,
        /* [in] */ IContentValues* values,
        /* [in] */ const String& selection,
        /* [in] */ ArrayOf<String>* selectionArgs,
        /* [out] */ Int32* rowsAffected);

    CARAPI OpenFile(
        /* [in] */ IUri* uri,
        /* [in] */ const String& mode,
        /* [out] */ IParcelFileDescriptor** fileDescriptor);

    CARAPI OpenAssetFile(
        /* [in] */ IUri* uri,
        /* [in] */ const String& mode,
        /* [out] */ IAssetFileDescriptor** fileDescriptor);

    CARAPI GetStreamTypes(
        /* [in] */ IUri* uri,
        /* [in] */ const String& mimeTypeFilter,
        /* [out, callee] */ ArrayOf<String>** streamTypes);

    CARAPI OpenTypedAssetFile(
        /* [in] */ IUri* uri,
        /* [in] */ const String& mimeTypeFilter,
        /* [in] */ IBundle* opts,
        /* [out] */ IAssetFileDescriptor** fileDescriptor);

    CARAPI OpenPipeHelper(
        /* [in] */ IUri* uri,
        /* [in] */ const String& mimeType,
        /* [in] */ IBundle* opts,
        /* [in] */ IInterface* args,
        /* [in] */ IContentProviderPipeDataWriter* func,
        /* [out] */ IParcelFileDescriptor** fileDescriptor);

    CARAPI GetIContentProvider(
        /* [out] */ IIContentProvider** contentProvider);

    CARAPI AttachInfo(
        /* [in] */ IContext* context,
        /* [in] */ IProviderInfo* info);

    CARAPI ApplyBatch(
        /* [in] */ IObjectContainer* operations,
        /* [out, callee] */ ArrayOf<IContentProviderResult *>** providerResults);

    CARAPI Call(
        /* [in] */ const String& method,
        /* [in] */ const String& arg,
        /* [in] */ IBundle* extras,
        /* [out] */ IBundle** bundle);

    CARAPI Shutdown();

    CARAPI Dump(
        /* [in] */ IFileDescriptor* fd,
        /* [in] */ IPrintWriter* writer,
        /* [in] */ ArrayOf<String>* args);

protected:
    /**
     * In order to use this class, you must extend it, and call this setup function from your
     * constructor.  In your application or activities, you must provide the same values when
     * you create the {@link android.provider.SearchRecentSuggestions} helper.
     *
     * @param authority This must match the authority that you've declared in your manifest.
     * @param mode You can use mode flags here to determine certain functional aspects of your
     * database.  Note, this value should not change from run to run, because when it does change,
     * your suggestions database may be wiped.
     *
     * @see #DATABASE_MODE_QUERIES
     * @see #DATABASE_MODE_2LINES
     */
    CARAPI SetupSuggestions(
        /* [in] */ const String& authority,
        /* [in] */ Int32 mode);

public:
    /**
     * This mode bit configures the database to record recent queries.  <i>required</i>
     *
     * @see #setupSuggestions(String, int)
     */
    static const Int32 DATABASE_MODE_QUERIES = 1;

    /**
     * This mode bit configures the database to include a 2nd annotation line with each entry.
     * <i>optional</i>
     *
     * @see #setupSuggestions(String, int)
     */
    static const Int32 DATABASE_MODE_2LINES = 2;

private:
    // debugging support
    static const String TAG;
    static const String sDatabaseName;
    static const String sSuggestions;
    static const String ORDER_BY;
    static const String NULL_COLUMN;

    // Table of database versions.  Don't forget to update!
    // NOTE:  These version values are shifted left 8 bits (x 256) in order to create space for
    // a small set of mode bitflags in the version int.
    //
    // 1      original implementation with queries, and 1 or 2 display columns
    // 1->2   added UNIQUE constraint to display1 column
    static const Int32 DATABASE_VERSION = 2 * 256;
    // Uri and query support
    static const Int32 URI_MATCH_SUGGEST = 1;

private:
    // client-provided configuration values
    String mAuthority;
    Int32 mMode;
    Boolean mTwoLineDisplay;
    // general database configuration and tables
    AutoPtr<ISQLiteOpenHelper> mOpenHelper;
    AutoPtr<IUri> mSuggestionsUri;
    AutoPtr<IUriMatcher> mUriMatcher;
    String mSuggestSuggestionClause;
    AutoPtr<ArrayOf<String> > mSuggestionProjection;

};

}
}
}

#endif // __CSEARCHRECENTSUGGESTIONSPROVIDER_H__
