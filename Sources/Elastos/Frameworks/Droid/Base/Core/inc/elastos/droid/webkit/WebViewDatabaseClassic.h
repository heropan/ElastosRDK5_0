
#ifndef __ELASTOS_DROID_WEBKIT_WEBVIEWDATABASECLASSIC_H__
#define __ELASTOS_DROID_WEBKIT_WEBVIEWDATABASECLASSIC_H__

#include "elastos/droid/ext/frameworkext.h"
#include "elastos/droid/webkit/WebViewDatabase.h"

using Elastos::Utility::IArrayList;
using Elastos::Utility::IMap;
using Elastos::Droid::Database::Sqlite::ISQLiteDatabase;

namespace Elastos {
namespace Droid {
namespace Webkit {

class WebViewDatabaseClassic
    : public WebViewDatabase
    , public Object
{
private:
    class MyThread : public ThreadBase
    {
    public:
        MyThread(
            /* [in] */ WebViewDatabaseClassic* owner,
            /* [in] */ IContext* context);

        CARAPI Run();

    private:
        WebViewDatabaseClassic* mOwner;
        AutoPtr<IContext> mContext;
    };

public:
    static CARAPI_(AutoPtr<WebViewDatabaseClassic>) GetInstance(
        /* [in] */ IContext* context);

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
    CARAPI_(void) SetUsernamePassword(
        /* [in] */ const String& schemePlusHost,
        /* [in] */ const String& username,
        /* [in] */ const String& password);

    /**
     * Retrieve the username and password for a given host
     *
     * @param schemePlusHost The scheme and host which passwords applies to
     * @return virtual CARAPI_(AutoPtr< ArrayOf<String> >) if found, String[0] is username, which can be null and
     *         String[1] is password. Return null if it can't find anything.
     */
    CARAPI_(AutoPtr< ArrayOf<String> >) GetUsernamePassword(
        /* [in] */ const String& schemePlusHost);

    /**
     * @see WebViewDatabase#hasUsernamePassword
     */
    CARAPI HasUsernamePassword(
        /* [out] */ Boolean* result);

    /**
     * @see WebViewDatabase#clearUsernamePassword
     */
    CARAPI ClearUsernamePassword();

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
    CARAPI_(void) SetHttpAuthUsernamePassword(
        /* [in] */ const String& host,
        /* [in] */ const String& realm,
        /* [in] */ const String& username,
        /* [in] */ const String& password);

    /**
     * Retrieve the HTTP authentication username and password for a given
     * host+realm pair
     *
     * @param host The host the password applies to
     * @param realm The realm the password applies to
     * @return virtual CARAPI_(AutoPtr< ArrayOf<String> >) if found, String[0] is username, which can be null and
     *         String[1] is password. Return null if it can't find anything.
     */
    CARAPI_(AutoPtr< ArrayOf<String> >) GetHttpAuthUsernamePassword(
        /* [in] */ const String& host,
        /* [in] */ const String& realm);

    /**
     * @see WebViewDatabase#hasHttpAuthUsernamePassword
     */
    CARAPI HasHttpAuthUsernamePassword(
        /* [out] */ Boolean* result);

    /**
     * @see WebViewDatabase#clearHttpAuthUsernamePassword
     */
    CARAPI ClearHttpAuthUsernamePassword();

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
    CARAPI_(void) SetFormData(
        /* [in] */ const String& url,
        /* [in] */ IMap* formdata);

    /**
     * Get all the values for a form entry with "name" in a given site
     *
     * @param url The url of the site
     * @param name The name of the form entry
     * @return A list of values. Return empty list if nothing is found.
     */
    CARAPI_(AutoPtr<IArrayList>) GetFormData(
        /* [in] */ const String& url,
        /* [in] */ const String& name);

    /**
     * @see WebViewDatabase#hasFormData
     */
    CARAPI HasFormData(
        /* [out] */ Boolean* result);

    /**
     * @see WebViewDatabase#clearFormData
     */
    CARAPI ClearFormData();

private:
    WebViewDatabaseClassic(
        /* [in] */ IContext* context);

    CARAPI_(void) Init(
        /* [in] */ IContext* context);

    CARAPI_(void) InitDatabase(
        /* [in] */ IContext* context);

    static CARAPI_(void) UpgradeDatabase();

    static CARAPI_(void) UpgradeDatabaseFromV10ToV11();

    static CARAPI_(void) UpgradeDatabaseToV10();

    // Wait for the background initialization thread to complete and check the
    // database creation status.
    CARAPI_(Boolean) CheckInitialized();

    CARAPI_(Boolean) HasEntries(
        /* [in] */ Int32 tableId);

private:
    static const String LOGTAG;
    static const String DATABASE_FILE;
    static const String CACHE_DATABASE_FILE;

    static const Int32 DATABASE_VERSION = 11;
    // 2 -> 3 Modified Cache table to allow cache of redirects
    // 3 -> 4 Added Oma-Downloads table
    // 4 -> 5 Modified Cache table to support persistent contentLength
    // 5 -> 4 Removed Oma-Downoads table
    // 5 -> 6 Add INDEX for cache table
    // 6 -> 7 Change cache localPath from Int32 to String
    // 7 -> 8 Move cache to its own db
    // 8 -> 9 Store both scheme and host when storing passwords
    // 9 -> 10 Update httpauth table UNIQUE
    // 10 -> 11 Drop cookies and cache now managed by the chromium stack,
    //          and update the form data table to use the new format
    //          implemented for b/5265606.

    static AutoPtr<WebViewDatabaseClassic> sInstance;
    static Object sInstanceLock;

    static AutoPtr<ISQLiteDatabase> sDatabase;

    // synchronize locks
    Object mPasswordLock;
    Object mFormLock;
    Object mHttpAuthLock;

    static const String mTableNames[];

    // Table ids (they are index to mTableNames)
    static const Int32 TABLE_PASSWORD_ID = 0;
    static const Int32 TABLE_FORMURL_ID = 1;
    static const Int32 TABLE_FORMDATA_ID = 2;
    static const Int32 TABLE_HTTPAUTH_ID = 3;

    // column id strings for "_id" which can be used by any table
    static const String ID_COL;

    static const AutoPtr< ArrayOf<String> > ID_PROJECTION;

    // column id strings for "password" table
    static const String PASSWORD_HOST_COL;
    static const String PASSWORD_USERNAME_COL;
    static const String PASSWORD_PASSWORD_COL;

    // column id strings for "formurl" table
    static const String FORMURL_URL_COL;

    // column id strings for "formdata" table
    static const String FORMDATA_URLID_COL;
    static const String FORMDATA_NAME_COL;
    static const String FORMDATA_VALUE_COL;

    // column id strings for "httpauth" table
    static const String HTTPAUTH_HOST_COL;
    static const String HTTPAUTH_REALM_COL;
    static const String HTTPAUTH_USERNAME_COL;
    static const String HTTPAUTH_PASSWORD_COL;

    // Initially true until the background thread completes.
    Boolean mInitialized;
};

} // namespace Webkit
} // namespace Droid
} // namespace Elastos

#endif //__ELASTOS_DROID_WEBKIT_WEBVIEWDATABASECLASSIC_H__
