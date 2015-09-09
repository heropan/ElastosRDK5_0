
#include "content/CContentValues.h"
#include "webkit/NativeUtil.h"
#include "webkit/WebTextView.h"
#include "webkit/WebViewDatabaseClassic.h"
#include <elastos/core/StringBuilder.h>
#include <elastos/core/StringUtils.h>
#include <elastos/utility/logging/Logger.h>

using Elastos::Core::CInteger64;
using Elastos::Core::IInteger64;
using Elastos::Core::CStringWrapper;
using Elastos::Core::StringBuilder;
using Elastos::Core::StringUtils;
using Elastos::Utility::CArrayList;
using Elastos::Utility::IIterator;
using Elastos::Utility::IMapEntry;
using Elastos::Utility::ISet;
using Elastos::Utility::Logging::Logger;
using Elastos::Droid::Content::CContentValues;
using Elastos::Droid::Content::IContentValues;
using Elastos::Droid::Database::ICursor;

namespace Elastos {
namespace Droid {
namespace Webkit {

//===============================================================
//             WebViewDatabaseClassic::MyThread
//===============================================================

WebViewDatabaseClassic::MyThread::MyThread(
    /* [in] */ WebViewDatabaseClassic* owner,
    /* [in] */ IContext* context)
    : mOwner(owner)
    , mContext(context)
{
    Thread::constructor();
}

ECode WebViewDatabaseClassic::MyThread::Run()
{
    mOwner->Init(mContext);
    return NOERROR;
}


//===============================================================
//                  WebViewDatabaseClassic
//===============================================================

static AutoPtr< ArrayOf<String> > ID_PROJECTIONCreate()
{
    AutoPtr< ArrayOf<String> > array = ArrayOf<String>::Alloc(1);
    (*array)[0] = "_id";
    return array;
}

const String WebViewDatabaseClassic::LOGTAG("WebViewDatabaseClassic");
const String WebViewDatabaseClassic::DATABASE_FILE("webview.db");
const String WebViewDatabaseClassic::CACHE_DATABASE_FILE("webviewCache.db");

const Int32 WebViewDatabaseClassic::DATABASE_VERSION;

AutoPtr<WebViewDatabaseClassic> WebViewDatabaseClassic::sInstance;
Mutex WebViewDatabaseClassic::sInstanceLock;
AutoPtr<ISQLiteDatabase> WebViewDatabaseClassic::sDatabase;

const String WebViewDatabaseClassic::mTableNames[] = {
    String("password"),
    String("formurl"),
    String("formdata"),
    String("httpauth")
};

// Table ids (they are index to mTableNames)
const Int32 WebViewDatabaseClassic::TABLE_PASSWORD_ID;
const Int32 WebViewDatabaseClassic::TABLE_FORMURL_ID;
const Int32 WebViewDatabaseClassic::TABLE_FORMDATA_ID;
const Int32 WebViewDatabaseClassic::TABLE_HTTPAUTH_ID;

// column id strings for "_id" which can be used by any table
const String WebViewDatabaseClassic::ID_COL("_id");

const AutoPtr< ArrayOf<String> > WebViewDatabaseClassic::ID_PROJECTION = ID_PROJECTIONCreate();

// column id strings for "password" table
const String WebViewDatabaseClassic::PASSWORD_HOST_COL("host");
const String WebViewDatabaseClassic::PASSWORD_USERNAME_COL("username");
const String WebViewDatabaseClassic::PASSWORD_PASSWORD_COL("password");

// column id strings for "formurl" table
const String WebViewDatabaseClassic::FORMURL_URL_COL("url");

// column id strings for "formdata" table
const String WebViewDatabaseClassic::FORMDATA_URLID_COL("urlid");
const String WebViewDatabaseClassic::FORMDATA_NAME_COL("name");
const String WebViewDatabaseClassic::FORMDATA_VALUE_COL("value");

// column id strings for "httpauth" table
const String WebViewDatabaseClassic::HTTPAUTH_HOST_COL("host");
const String WebViewDatabaseClassic::HTTPAUTH_REALM_COL("realm");
const String WebViewDatabaseClassic::HTTPAUTH_USERNAME_COL("username");
const String WebViewDatabaseClassic::HTTPAUTH_PASSWORD_COL("password");

WebViewDatabaseClassic::WebViewDatabaseClassic(
    /* [in] */ IContext* context)
    : mInitialized(FALSE)
{
    NativeUtil::SetContext(context);
    AutoPtr<IThread> t = new MyThread(this, context);
    t->Start();

    // Singleton only, use getInstance()
}

AutoPtr<WebViewDatabaseClassic> WebViewDatabaseClassic::GetInstance(
    /* [in] */ IContext* context)
{
    AutoLock lock(sInstanceLock);

    if (sInstance == NULL) {
        sInstance = new WebViewDatabaseClassic(context);
    }
    return sInstance;
}

void WebViewDatabaseClassic::Init(
    /* [in] */ IContext* context)
{
    AutoLock lock(this);

    if (mInitialized) {
        return;
    }

    InitDatabase(context);
    // Before using the Chromium HTTP stack, we stored the WebKit cache in
    // our own DB. Clean up the DB file if it's still around.
    Boolean result;
    context->DeleteDatabase(CACHE_DATABASE_FILE, &result);

    // Thread done, notify.
    mInitialized = TRUE;
    Notify();
}

void WebViewDatabaseClassic::InitDatabase(
    /* [in] */ IContext* context)
{
    //try {
    sDatabase = NULL;
    context->OpenOrCreateDatabase(DATABASE_FILE, 0, NULL,
            (ISQLiteDatabase**)&sDatabase);
    //} catch (SQLiteException e) {
        // try again by deleting the old db and create a new one
    //    if (context.deleteDatabase(DATABASE_FILE)) {
    //        sDatabase = context.openOrCreateDatabase(DATABASE_FILE, 0,
    //                null);
    //    }
    //}

    // sDatabase should not be null,
    // the only case is RequestAPI test has problem to create db
    if (sDatabase == NULL) {
        mInitialized = TRUE;
        Notify();
        return;
    }

    Int32 version;
    sDatabase->GetVersion(&version);
    if (version != DATABASE_VERSION) {
        sDatabase->BeginTransactionNonExclusive();
        //try {
        UpgradeDatabase();
        sDatabase->SetTransactionSuccessful();
        //} finally {
        sDatabase->EndTransaction();
        //}
    }
}

void WebViewDatabaseClassic::UpgradeDatabase()
{
    UpgradeDatabaseToV10();
    UpgradeDatabaseFromV10ToV11();
    // Add future database upgrade functions here, one version at a
    // time.
    sDatabase->SetVersion(DATABASE_VERSION);
}

void WebViewDatabaseClassic::UpgradeDatabaseFromV10ToV11()
{
    Int32 oldVersion;
    sDatabase->GetVersion(&oldVersion);

    if (oldVersion >= 11) {
        // Nothing to do.
        return;
    }

    // Clear out old java stack cookies - this data is now stored in
    // a separate database managed by the Chrome stack.
    sDatabase->ExecSQL(String("DROP TABLE IF EXISTS cookies"));

    // Likewise for the old cache table.
    sDatabase->ExecSQL(String("DROP TABLE IF EXISTS cache"));

    // Update form autocomplete  URLs to match new ICS formatting.
    AutoPtr<ICursor> c;
    sDatabase->Query(mTableNames[TABLE_FORMURL_ID], NULL, String(NULL),
            NULL, String(NULL), String(NULL), String(NULL), (ICursor**)&c);
    Boolean succeeded = FALSE;
    while (c->MoveToNext(&succeeded), succeeded) {
        Int32 index;
        c->GetColumnIndex(ID_COL, &index);
        Int64 i64Value;
        c->GetInt64(index, &i64Value);
        String urlId = StringUtils::ToString(i64Value);
        c->GetColumnIndex(FORMURL_URL_COL, &index);
        String url;
        c->GetString(index, &url);
        AutoPtr<IContentValues> cv;
        CContentValues::New(1, (IContentValues**)&cv);
        AutoPtr<ICharSequence> cs;
        CStringWrapper::New(WebTextView::UrlForAutoCompleteData(url), (ICharSequence**)&cs);
        cv->PutString(FORMURL_URL_COL, cs);
        AutoPtr< ArrayOf<String> > whereArgs = ArrayOf<String>::Alloc(1);
        (*whereArgs)[0] = urlId;
        Int32 result;
        sDatabase->Update(mTableNames[TABLE_FORMURL_ID], cv, ID_COL + "=?",
                whereArgs, &result);
    }
    c->Close();
}

void WebViewDatabaseClassic::UpgradeDatabaseToV10()
{
    Int32 oldVersion;
    sDatabase->GetVersion(&oldVersion);

    if (oldVersion >= 10) {
        // Nothing to do.
        return;
    }

    if (oldVersion != 0) {
        Logger::I(LOGTAG, "Upgrading database from version %d to %d, which will destroy old data",
                oldVersion, DATABASE_VERSION);
    }

    StringBuilder sb;
    if (9 == oldVersion) {
        sDatabase->ExecSQL(String("DROP TABLE IF EXISTS ")
                + mTableNames[TABLE_HTTPAUTH_ID]);

        sb.Append("CREATE TABLE ");
        sb.Append(mTableNames[TABLE_HTTPAUTH_ID]);
        sb.Append(" INTEGER PRIMARY KEY, ");
        sb.Append(" (");
        sb.Append(ID_COL);
        sb.Append(HTTPAUTH_HOST_COL);
        sb.Append(" TEXT, ");
        sb.Append(HTTPAUTH_REALM_COL);
        sb.Append(" TEXT, ");
        sb.Append(HTTPAUTH_USERNAME_COL);
        sb.Append(" TEXT, ");
        sb.Append(HTTPAUTH_PASSWORD_COL);
        sb.Append(" TEXT,");
        sb.Append(" UNIQUE (");
        sb.Append(HTTPAUTH_HOST_COL);
        sb.Append(", ");
        sb.Append(HTTPAUTH_REALM_COL);
        sb.Append(") ON CONFLICT REPLACE);");
        sDatabase->ExecSQL(sb.ToString());
        return;
    }

    sDatabase->ExecSQL(String("DROP TABLE IF EXISTS cookies"));
    sDatabase->ExecSQL(String("DROP TABLE IF EXISTS cache"));
    sDatabase->ExecSQL(String("DROP TABLE IF EXISTS ")
            + mTableNames[TABLE_FORMURL_ID]);
    sDatabase->ExecSQL(String("DROP TABLE IF EXISTS ")
            + mTableNames[TABLE_FORMDATA_ID]);
    sDatabase->ExecSQL(String("DROP TABLE IF EXISTS ")
            + mTableNames[TABLE_HTTPAUTH_ID]);
    sDatabase->ExecSQL(String("DROP TABLE IF EXISTS ")
            + mTableNames[TABLE_PASSWORD_ID]);

    sb.Append("CREATE TABLE ");
    sb.Append(mTableNames[TABLE_FORMURL_ID]);
    sb.Append(" (");
    sb.Append(ID_COL);
    sb.Append(" INTEGER PRIMARY KEY, ");
    sb.Append(FORMURL_URL_COL);
    sb.Append(" TEXT");
    sb.Append(");");
    // formurl
    sDatabase->ExecSQL(sb.ToString());

    sb.Reset();
    sb.Append("CREATE TABLE ");
    sb.Append(mTableNames[TABLE_FORMDATA_ID]);
    sb.Append(" (");
    sb.Append(ID_COL);
    sb.Append(" INTEGER PRIMARY KEY, ");
    sb.Append(FORMDATA_URLID_COL);
    sb.Append(" INTEGER, ");
    sb.Append(FORMDATA_NAME_COL);
    sb.Append(" TEXT, ");
    sb.Append(FORMDATA_VALUE_COL);
    sb.Append(" TEXT,");
    sb.Append(" UNIQUE (");
    sb.Append(FORMDATA_URLID_COL);
    sb.Append(", ");
    sb.Append(FORMDATA_NAME_COL);
    sb.Append(", ");
    sb.Append(FORMDATA_VALUE_COL);
    sb.Append(") ON CONFLICT IGNORE);");
    // formdata
    sDatabase->ExecSQL(sb.ToString());


    sb.Reset();
    sb.Append("CREATE TABLE ");
    sb.Append(mTableNames[TABLE_HTTPAUTH_ID]);
    sb.Append(" (");
    sb.Append(ID_COL);
    sb.Append(" INTEGER PRIMARY KEY, ");
    sb.Append(HTTPAUTH_HOST_COL);
    sb.Append(" TEXT, ");
    sb.Append(HTTPAUTH_REALM_COL);
    sb.Append(" TEXT, ");
    sb.Append(HTTPAUTH_USERNAME_COL);
    sb.Append(" TEXT, ");
    sb.Append(HTTPAUTH_PASSWORD_COL);
    sb.Append(" TEXT,");
    sb.Append(" UNIQUE (");
    sb.Append(HTTPAUTH_HOST_COL);
    sb.Append(", ");
    sb.Append(HTTPAUTH_REALM_COL);
    sb.Append(") ON CONFLICT REPLACE);");
    // httpauth
    sDatabase->ExecSQL(sb.ToString());

    sb.Reset();
    sb.Append("CREATE TABLE ");
    sb.Append(mTableNames[TABLE_PASSWORD_ID]);
    sb.Append(" (");
    sb.Append(ID_COL);
    sb.Append(" INTEGER PRIMARY KEY, ");
    sb.Append(PASSWORD_HOST_COL);
    sb.Append(" TEXT, ");
    sb.Append(PASSWORD_USERNAME_COL);
    sb.Append(" TEXT, ");
    sb.Append(PASSWORD_PASSWORD_COL);
    sb.Append(" TEXT,");
    sb.Append(" UNIQUE (");
    sb.Append(PASSWORD_HOST_COL);
    sb.Append(", ");
    sb.Append(PASSWORD_USERNAME_COL);
    sb.Append(") ON CONFLICT REPLACE);");
    // passwords
    sDatabase->ExecSQL(sb.ToString());
}

// Wait for the background initialization thread to complete and check the
// database creation status.
Boolean WebViewDatabaseClassic::CheckInitialized()
{
    {
        AutoLock lock(this);
        while (!mInitialized) {
            //try {
            Wait();
            //} catch (InterruptedException e) {
            //    Log.e(LOGTAG, "Caught exception while checking " +
            //                  "initialization");
            //    Log.e(LOGTAG, Log.getStackTraceString(e));
            //}
        }
    }
    return sDatabase != NULL;
}

Boolean WebViewDatabaseClassic::HasEntries(
    /* [in] */ Int32 tableId)
{
    if (!CheckInitialized()) {
        return FALSE;
    }

    AutoPtr<ICursor> cursor;
    Boolean ret = FALSE;
    //try {
    sDatabase->Query(mTableNames[tableId], ID_PROJECTION,
            String(NULL), NULL, String(NULL), String(NULL), String(NULL), (ICursor**)&cursor);
    Boolean succeeded = FALSE;
    cursor->MoveToFirst(&succeeded);
    ret = succeeded == TRUE;
    //} catch (IllegalStateException e) {
//        Log.e(LOGTAG, "hasEntries", e);
    //} finally {
    if (cursor != NULL) cursor->Close();
    //}
    return ret;
}


//
// password functions
//

/**
 * Set password. Tuple (PASSWORD_HOST_COL, PASSWORD_USERNAME_COL) is unique.
 *
 * @param schemePlusHost The scheme and host for the password
 * @param username The username for the password. If it is null, it means
 *            password can't be saved.
 * @param password The password
 */
void WebViewDatabaseClassic::SetUsernamePassword(
    /* [in] */ const String& schemePlusHost,
    /* [in] */ const String& username,
    /* [in] */ const String& password)
{
    if (schemePlusHost.IsNull() || !CheckInitialized()) {
        return;
    }

    {
        AutoLock lock(mPasswordLock);
        AutoPtr<IContentValues> c;
        CContentValues::New((IContentValues**)&c);
        AutoPtr<ICharSequence> cs;
        CStringWrapper::New(schemePlusHost, (ICharSequence**)&cs);
        c->PutString(PASSWORD_HOST_COL, cs);
        cs = NULL;
        CStringWrapper::New(username, (ICharSequence**)&cs);
        c->PutString(PASSWORD_USERNAME_COL, cs);
        cs = NULL;
        CStringWrapper::New(password, (ICharSequence**)&cs);
        c->PutString(PASSWORD_PASSWORD_COL, cs);
        Int64 result;
        sDatabase->Insert(mTableNames[TABLE_PASSWORD_ID], PASSWORD_HOST_COL,
                c, &result);
    }
}

/**
 * Retrieve the username and password for a given host
 *
 * @param schemePlusHost The scheme and host which passwords applies to
 * @return CARAPI_(AutoPtr< ArrayOf<String> >) if found, String[0] is username, which can be null and
 *         String[1] is password. Return null if it can't find anything.
 */
AutoPtr< ArrayOf<String> > WebViewDatabaseClassic::GetUsernamePassword(
    /* [in] */ const String& schemePlusHost)
{
    if (schemePlusHost.IsNull() || !CheckInitialized()) {
        return NULL;
    }

    AutoPtr< ArrayOf<String> > columns = ArrayOf<String>::Alloc(2);
    (*columns)[0] = PASSWORD_USERNAME_COL;
    (*columns)[1] = PASSWORD_PASSWORD_COL;

    StringBuilder sb;
    sb.Append("(");
    sb.Append(PASSWORD_HOST_COL);
    sb.Append(" == ?)");
    String selection = sb.ToString();
    {
        AutoLock lock(mPasswordLock);
        AutoPtr< ArrayOf<String> > ret;
        AutoPtr<ICursor> cursor;
        //try {
        AutoPtr< ArrayOf<String> > selectionArgs = ArrayOf<String>::Alloc(1);
        (*selectionArgs)[0] = schemePlusHost;
        sDatabase->Query(mTableNames[TABLE_PASSWORD_ID],
                columns, selection, selectionArgs, String(NULL),
                String(NULL), String(NULL), (ICursor**)&cursor);
        Boolean succeeded = FALSE;
        if (cursor->MoveToFirst(&succeeded), succeeded) {
            ret = ArrayOf<String>::Alloc(2);
            String str;
            Int32 index;
            cursor->GetColumnIndex(PASSWORD_USERNAME_COL, &index);
            cursor->GetString(index, &str);
            (*ret)[0] = str;
            cursor->GetColumnIndex(PASSWORD_PASSWORD_COL, &index);
            cursor->GetString(index, &str);
            (*ret)[1] = str;
        }
        //} catch (IllegalStateException e) {
        //    Log.e(LOGTAG, "getUsernamePassword", e);
        //} finally {
        if (cursor != NULL) cursor->Close();
        //}
        return ret;
    }
}

/**
 * @see WebViewDatabase#hasUsernamePassword
 */
ECode WebViewDatabaseClassic::HasUsernamePassword(
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
    AutoLock lock(mPasswordLock);
    *result = HasEntries(TABLE_PASSWORD_ID);
    return NOERROR;
}

/**
 * @see WebViewDatabase#clearUsernamePassword
 */
ECode WebViewDatabaseClassic::ClearUsernamePassword()
{
    if (!CheckInitialized()) {
        return NOERROR;
    }

    {
        AutoLock lock(mPasswordLock);
        Int32 value;
        sDatabase->Delete(mTableNames[TABLE_PASSWORD_ID], String(NULL), NULL, &value);
    }
    return NOERROR;
}

//
// http authentication password functions
//

/**
 * Set HTTP authentication password. Tuple (HTTPAUTH_HOST_COL,
 * HTTPAUTH_REALM_COL, HTTPAUTH_USERNAME_COL) is unique.
 *
 * @param host The host for the password
 * @param realm The realm for the password
 * @param username The username for the password. If it is null, it means
 *            password can't be saved.
 * @param password The password
 */
void WebViewDatabaseClassic::SetHttpAuthUsernamePassword(
    /* [in] */ const String& host,
    /* [in] */ const String& realm,
    /* [in] */ const String& username,
    /* [in] */ const String& password)
{
    if (host.IsNull() || realm.IsNull() || !CheckInitialized()) {
        return;
    }

    {
        AutoLock lock(mHttpAuthLock);
        AutoPtr<IContentValues> c;
        CContentValues::New((IContentValues**)&c);
        AutoPtr<ICharSequence> cs;
        CStringWrapper::New(host, (ICharSequence**)&cs);
        c->PutString(HTTPAUTH_HOST_COL, cs);
        cs = NULL;
        CStringWrapper::New(realm, (ICharSequence**)&cs);
        c->PutString(HTTPAUTH_REALM_COL, cs);
        cs = NULL;
        CStringWrapper::New(username, (ICharSequence**)&cs);
        c->PutString(HTTPAUTH_USERNAME_COL, cs);
        cs = NULL;
        CStringWrapper::New(password, (ICharSequence**)&cs);
        c->PutString(HTTPAUTH_PASSWORD_COL, cs);
        Int64 result;
        sDatabase->Insert(mTableNames[TABLE_HTTPAUTH_ID], HTTPAUTH_HOST_COL,
                c, &result);
    }
}

/**
 * Retrieve the HTTP authentication username and password for a given
 * host+realm pair
 *
 * @param host The host the password applies to
 * @param realm The realm the password applies to
 * @return CARAPI_(AutoPtr< ArrayOf<String> >) if found, String[0] is username, which can be null and
 *         String[1] is password. Return null if it can't find anything.
 */
AutoPtr< ArrayOf<String> > WebViewDatabaseClassic::GetHttpAuthUsernamePassword(
    /* [in] */ const String& host,
    /* [in] */ const String& realm)
{
    if (host.IsNull() || realm.IsNull() || !CheckInitialized()){
        return NULL;
    }

    AutoPtr< ArrayOf<String> > columns = ArrayOf<String>::Alloc(2);
    (*columns)[0] = HTTPAUTH_USERNAME_COL;
    (*columns)[1] = HTTPAUTH_PASSWORD_COL;
    StringBuilder sb;
    sb.Append("(");
    sb.Append(HTTPAUTH_HOST_COL);
    sb.Append(" == ?) AND (");
    sb.Append(HTTPAUTH_REALM_COL);
    sb.Append(" == ?)");
    String selection = sb.ToString();
    {
        AutoLock lock(mHttpAuthLock);
        AutoPtr< ArrayOf<String> > ret;
        AutoPtr<ICursor> cursor;
        //try {
        AutoPtr< ArrayOf<String> > selectionArgs = ArrayOf<String>::Alloc(2);
        (*selectionArgs)[0] = host;
        (*selectionArgs)[1] = realm;
        sDatabase->Query(mTableNames[TABLE_HTTPAUTH_ID],
                columns, selection, selectionArgs, String(NULL),
                String(NULL), String(NULL), (ICursor**)&cursor);
        Boolean succeeded = FALSE;
        if (cursor->MoveToFirst(&succeeded), succeeded) {
            ret = ArrayOf<String>::Alloc(2);
            String str;
            Int32 index;
            cursor->GetColumnIndex(HTTPAUTH_USERNAME_COL, &index);
            cursor->GetString(index, &str);
            (*ret)[0] = str;
            cursor->GetColumnIndex(HTTPAUTH_PASSWORD_COL, &index);
            cursor->GetString(index, &str);
            (*ret)[1] = str;
        }
        //} catch (IllegalStateException e) {
        //    Log.e(LOGTAG, "getHttpAuthUsernamePassword", e);
        //} finally {
        if (cursor != NULL) cursor->Close();
        //}
        return ret;
    }
}

/**
 * @see WebViewDatabase#hasHttpAuthUsernamePassword
 */
ECode WebViewDatabaseClassic::HasHttpAuthUsernamePassword(
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
    AutoLock lock(mHttpAuthLock);
    *result = HasEntries(TABLE_HTTPAUTH_ID);
    return NOERROR;
}

/**
 * @see WebViewDatabase#clearHttpAuthUsernamePassword
 */
ECode WebViewDatabaseClassic::ClearHttpAuthUsernamePassword()
{
    if (!CheckInitialized()) {
        return NOERROR;
    }

    {
        AutoLock lock(mHttpAuthLock);
        Int32 value;
        sDatabase->Delete(mTableNames[TABLE_HTTPAUTH_ID], String(NULL), NULL, &value);
    }
    return NOERROR;
}

//
// form data functions
//

/**
 * Set form data for a site. Tuple (FORMDATA_URLID_COL, FORMDATA_NAME_COL,
 * FORMDATA_VALUE_COL) is unique
 *
 * @param url The url of the site
 * @param formdata The form data in HashMap
 */
void WebViewDatabaseClassic::SetFormData(
    /* [in] */ const String& url,
    /* [in] */ IMap* formdata)
{
    if (url.IsNull() || formdata == NULL || !CheckInitialized()) {
        return;
    }

    StringBuilder sb;
    sb.Append("(");
    sb.Append(FORMURL_URL_COL);
    sb.Append(" == ?)");
    String selection = sb.ToString();
    {
        AutoLock lock(mFormLock);
        Int64 urlid = -1;
        AutoPtr<ICursor> cursor;
        //try {
        AutoPtr< ArrayOf<String> > selectionArgs = ArrayOf<String>::Alloc(1);
        (*selectionArgs)[0] = url;
        sDatabase->Query(mTableNames[TABLE_FORMURL_ID],
                ID_PROJECTION, selection, selectionArgs, String(NULL), String(NULL),
                String(NULL), (ICursor**)&cursor);
        Boolean succeeded = FALSE;
        if (cursor->MoveToFirst(&succeeded), succeeded) {
            Int32 index;
            cursor->GetColumnIndex(ID_COL, &index);
            cursor->GetInt64(index, &urlid);
        }
        else {
            AutoPtr<IContentValues> c;
            CContentValues::New((IContentValues**)&c);
            AutoPtr<ICharSequence> cs;
            CStringWrapper::New(url, (ICharSequence**)&cs);
            c->PutString(FORMURL_URL_COL, cs);
            sDatabase->Insert(
                    mTableNames[TABLE_FORMURL_ID], String(""), c, &urlid);
        }
        //} catch (IllegalStateException e) {
        //    Log.e(LOGTAG, "setFormData", e);
        //} finally {
        if (cursor != NULL) cursor->Close();
        //}
        if (urlid >= 0) {
            AutoPtr<ISet> set;
            formdata->EntrySet((ISet**)&set);
            AutoPtr<IIterator> iter;
            set->GetIterator((IIterator**)&iter);
            AutoPtr<IContentValues> map;
            CContentValues::New((IContentValues**)&map);
            AutoPtr<IInteger64> iUrlid;
            CInteger64::New(urlid, (IInteger64**)&iUrlid);
            map->PutInt64(FORMDATA_URLID_COL, iUrlid);
            Boolean hasNext = FALSE;
            while(iter->HasNext(&hasNext), hasNext) {
                AutoPtr<IMapEntry> entry;
                iter->Next((IInterface**)&entry);
                AutoPtr<ICharSequence> key;
                entry->GetKey((IInterface**)&key);
                map->PutString(FORMDATA_NAME_COL, key);
                AutoPtr<ICharSequence> value;
                entry->GetValue((IInterface**)&value);
                map->PutString(FORMDATA_VALUE_COL, value);
                Int64 result;
                sDatabase->Insert(mTableNames[TABLE_FORMDATA_ID], String(NULL), map, &result);
            }
        }
    }
}

/**
 * Get all the values for a form entry with "name" in a given site
 *
 * @param url The url of the site
 * @param name The name of the form entry
 * @return A list of values. Return empty list if nothing is found.
 */
AutoPtr<IArrayList> WebViewDatabaseClassic::GetFormData(
    /* [in] */ const String& url,
    /* [in] */ const String& name)
{
    AutoPtr<IArrayList> values;
    CArrayList::New((IArrayList**)&values);
    if (url.IsNull() || name.IsNull() || !CheckInitialized()) {
        return values;
    }

    StringBuilder sb;
    sb.Append("(");
    sb.Append(FORMURL_URL_COL);
    sb.Append(" == ?)");
    String urlSelection = sb.ToString();
    sb.Reset();
    sb.Append("(");
    sb.Append(FORMDATA_URLID_COL);
    sb.Append(" == ?) AND (");
    sb.Append(FORMDATA_NAME_COL);
    String dataSelection = sb.ToString();
    {
        AutoLock lock(mFormLock);
        AutoPtr<ICursor> cursor;
        //try {
            AutoPtr< ArrayOf<String> > selectionArgs = ArrayOf<String>::Alloc(1);
            (*selectionArgs)[0] = url;
            sDatabase->Query(mTableNames[TABLE_FORMURL_ID],
                    ID_PROJECTION, urlSelection, selectionArgs, String(NULL),
                    String(NULL), String(NULL), (ICursor**)&cursor);
            Boolean succeeded = FALSE;
            while (cursor->MoveToNext(&succeeded), succeeded) {
                Int32 columnIndex;
                cursor->GetColumnIndex(ID_COL, &columnIndex);
                Int64 urlid;
                cursor->GetInt64(columnIndex, &urlid);
                AutoPtr<ICursor> dataCursor;
                //try {
                    AutoPtr< ArrayOf<String> > columns = ArrayOf<String>::Alloc(2);
                    (*columns)[0] = ID_COL;
                    (*columns)[1] = FORMDATA_VALUE_COL;
                    AutoPtr< ArrayOf<String> > selectionArgs = ArrayOf<String>::Alloc(2);
                    (*selectionArgs)[0] = StringUtils::ToString(urlid);
                    (*selectionArgs)[1] = name;
                    sDatabase->Query(
                            mTableNames[TABLE_FORMDATA_ID],
                            columns,
                            dataSelection,
                            selectionArgs,
                            String(NULL), String(NULL), String(NULL), (ICursor**)&dataCursor);
                    if (dataCursor->MoveToFirst(&succeeded), succeeded) {
                        Int32 valueCol;
                        dataCursor->GetColumnIndex(
                                FORMDATA_VALUE_COL, &valueCol);
                        do {
                            String str;
                            dataCursor->GetString(valueCol, &str);
                            AutoPtr<ICharSequence> cs;
                            CStringWrapper::New(str, (ICharSequence**)&cs);
                            Boolean result = FALSE;
                            values->Add(cs, &result);
                        } while (dataCursor->MoveToNext(&succeeded), succeeded);
                    }
                //} catch (IllegalStateException e) {
                //    Log.e(LOGTAG, "getFormData dataCursor", e);
                //} finally {
                    if (dataCursor != NULL) dataCursor->Close();
                //}
            }
        //} catch (IllegalStateException e) {
        //    Log.e(LOGTAG, "getFormData cursor", e);
        //} finally {
            if (cursor != NULL) cursor->Close();
        //}
        return values;
    }
}

/**
 * @see WebViewDatabase#hasFormData
 */
ECode WebViewDatabaseClassic::HasFormData(
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
    AutoLock lock(mFormLock);
    *result = HasEntries(TABLE_FORMURL_ID);
    return NOERROR;
}

/**
 * @see WebViewDatabase#clearFormData
 */
ECode WebViewDatabaseClassic::ClearFormData()
{
    if (!CheckInitialized()) {
        return NOERROR;
    }

    {
        AutoLock lock(mFormLock);
        Int32 result;
        sDatabase->Delete(mTableNames[TABLE_FORMURL_ID], String(NULL), NULL, &result);
        sDatabase->Delete(mTableNames[TABLE_FORMDATA_ID], String(NULL), NULL, &result);
    }
    return NOERROR;
}

} // namespace Webkit
} // namepsace Droid
} // namespace Elastos
