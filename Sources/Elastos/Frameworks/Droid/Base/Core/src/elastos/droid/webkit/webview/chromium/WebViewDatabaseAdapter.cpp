
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
    assert(0);
    return FALSE;
}

ECode WebViewDatabaseAdapter::ClearUsernamePassword()
{
    // ==================before translated======================
    // // This is a deprecated API: intentional no-op.
    assert(0);
    return NOERROR;
}

Boolean WebViewDatabaseAdapter::HasHttpAuthUsernamePassword()
{
    // ==================before translated======================
    // return mHttpAuthDatabase.hasHttpAuthUsernamePassword();
    assert(0);
    return FALSE;
}

ECode WebViewDatabaseAdapter::ClearHttpAuthUsernamePassword()
{
    // ==================before translated======================
    // mHttpAuthDatabase.clearHttpAuthUsernamePassword();
    assert(0);
    return NOERROR;
}

Boolean WebViewDatabaseAdapter::HasFormData()
{
    // ==================before translated======================
    // return mFormDatabase.hasFormData();
    assert(0);
    return FALSE;
}

ECode WebViewDatabaseAdapter::ClearFormData()
{
    // ==================before translated======================
    // mFormDatabase.clearFormData();
    assert(0);
    return NOERROR;
}

} // namespace Chromium
} // namespace Webview
} // namespace Webkit
} // namespace Droid
} // namespace Elastos


