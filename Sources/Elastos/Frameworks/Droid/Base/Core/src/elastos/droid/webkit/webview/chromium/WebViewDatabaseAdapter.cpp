
#include "elastos/droid/webkit/webview/chromium/WebViewDatabaseAdapter.h"

namespace Elastos {
namespace Droid {
namespace Webkit {
namespace Webview {
namespace Chromium {

//=====================================================================
//                        WebViewDatabaseAdapter
//=====================================================================
WebViewDatabaseAdapter::WebViewDatabaseAdapter(
    /* [in] */ AwFormDatabase* formDatabase,
    /* [in] */ HttpAuthDatabase* httpAuthDatabase)
    : mFormDatabase(formDatabase)
    , mHttpAuthDatabase(httpAuthDatabase)
{
    // ==================before translated======================
    // mFormDatabase = formDatabase;
    // mHttpAuthDatabase = httpAuthDatabase;
}

Boolean WebViewDatabaseAdapter::HasUsernamePassword()
{
    // ==================before translated======================
    // // This is a deprecated API: intentional no-op.
    // return false;

    return FALSE;
}

ECode WebViewDatabaseAdapter::ClearUsernamePassword()
{
    // ==================before translated======================
    // // This is a deprecated API: intentional no-op.

    return NOERROR;
}

Boolean WebViewDatabaseAdapter::HasHttpAuthUsernamePassword()
{
    // ==================before translated======================
    // return mHttpAuthDatabase.hasHttpAuthUsernamePassword();

    return mHttpAuthDatabase->HasHttpAuthUsernamePassword();
}

ECode WebViewDatabaseAdapter::ClearHttpAuthUsernamePassword()
{
    // ==================before translated======================
    // mHttpAuthDatabase.clearHttpAuthUsernamePassword();

    mHttpAuthDatabase->ClearHttpAuthUsernamePassword();
    return NOERROR;
}

Boolean WebViewDatabaseAdapter::HasFormData()
{
    // ==================before translated======================
    // return mFormDatabase.hasFormData();

    return mFormDatabase->HasFormData();
}

ECode WebViewDatabaseAdapter::ClearFormData()
{
    // ==================before translated======================
    // mFormDatabase.clearFormData();

    mFormDatabase->ClearFormData();
    return NOERROR;
}

} // namespace Chromium
} // namespace Webview
} // namespace Webkit
} // namespace Droid
} // namespace Elastos


