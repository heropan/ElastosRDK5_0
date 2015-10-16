
#include "elastos/droid/webkit/CWebViewTransport.h"

namespace Elastos {
namespace Droid {
namespace Webkit {

ECode CWebViewTransport::SetWebView(
    /* [in] */ IWebView * webview)
{
    assert(0);
//    WebView::WebViewTransport::SetWebView(webview);
    return NOERROR;
}

ECode CWebViewTransport::GetWebView(
    /* [out] */ IWebView ** webView)
{
    VALIDATE_NOT_NULL(webView);
    assert(0);
//    AutoPtr<IWebView> _webView = WebView::WebViewTransport::GetWebView();
//    *webview = _webView.Get();
//    REFCOUNT_ADD(*webview);

    return NOERROR;
}

} // namespace Webkit
} // namepsace Droid
} // namespace Elastos