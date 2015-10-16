#ifndef __ELASTOS_DROID_WEBKIT_ANDROIDWEBVIEW_AWBROWSERCONTEXT_H__
#define __ELASTOS_DROID_WEBKIT_ANDROIDWEBVIEW_AWBROWSERCONTEXT_H__
#include "elastos/droid/ext/frameworkext.h"
#include "elastos/droid/webkit/native/content/browser/ContentViewStatics.h"
#include "elastos/droid/webkit/native/android_webview/AwGeolocationPermissions.h"
#include "elastos/droid/webkit/native/android_webview/AwCookieManager.h"
#include "elastos/droid/webkit/native/android_webview/AwFormDatabase.h"
#include "elastos/droid/webkit/native/android_webview/HttpAuthDatabase.h"
#include "elastos/droid/webkit/native/net/DefaultAndroidKeyStore.h"

using Elastos::Droid::Content::IContext;
using Elastos::Droid::Content::ISharedPreferences;
using Elastos::Droid::Webkit::Content::Browser::ContentViewStatics;
using Elastos::Droid::Webkit::Net::DefaultAndroidKeyStore;
using Elastos::Droid::Webkit::AndroidWebview::AwGeolocationPermissions;
using Elastos::Droid::Webkit::AndroidWebview::AwCookieManager;
using Elastos::Droid::Webkit::AndroidWebview::AwFormDatabase;
using Elastos::Droid::Webkit::AndroidWebview::HttpAuthDatabase;

namespace Elastos {
namespace Droid {
namespace Webkit {
namespace AndroidWebview {

/**
 * Java side of the Browser Context: contains all the java side objects needed to host one
 * browing session (i.e. profile).
 * Note that due to running in single process mode, and limitations on renderer process only
 * being able to use a single browser context, currently there can only be one AwBrowserContext
 * instance, so at this point the class mostly exists for conceptual clarity.
 */
class AwBrowserContext
{
public:
    AwBrowserContext(
        /* [in] */ ISharedPreferences* sharedPreferences);

    virtual CARAPI_(AutoPtr<AwGeolocationPermissions>) GetGeolocationPermissions();

    virtual CARAPI_(AutoPtr<AwCookieManager>) GetCookieManager();

    virtual CARAPI_(AutoPtr<AwFormDatabase>) GetFormDatabase();

    virtual CARAPI_(AutoPtr<HttpAuthDatabase>) GetHttpAuthDatabase(
        /* [in] */ IContext* context);

    virtual CARAPI_(AutoPtr<DefaultAndroidKeyStore>) GetKeyStore();

    /**
     * @see android.webkit.WebView#pauseTimers()
     */
    virtual CARAPI_(void) PauseTimers();

    /**
     * @see android.webkit.WebView#resumeTimers()
     */
    virtual CARAPI_(void) ResumeTimers();

private:
    static const String HTTP_AUTH_DATABASE_FILE;

    AutoPtr<ISharedPreferences> mSharedPreferences;

    AutoPtr<AwGeolocationPermissions> mGeolocationPermissions;
    AutoPtr<AwCookieManager> mCookieManager;
    AutoPtr<AwFormDatabase> mFormDatabase;
    AutoPtr<HttpAuthDatabase> mHttpAuthDatabase;
    AutoPtr<DefaultAndroidKeyStore> mLocalKeyStore;
};

} // namespace AndroidWebview
} // namespace Webkit
} // namespace Droid
} // namespace Elastos

#endif//__ELASTOS_DROID_WEBKIT_ANDROIDWEBVIEW_AWBROWSERCONTEXT_H__
