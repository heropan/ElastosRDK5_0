
#ifndef __ELASTOS_DROID_WEBKIT_CWEBVIEWTRANSPORT_H__
#define __ELASTOS_DROID_WEBKIT_CWEBVIEWTRANSPORT_H__

#include "_CWebViewTransport.h"
#include "webkit/WebView.h"

namespace Elastos {
namespace Droid {
namespace Webkit {

CarClass(CWebViewTransport), public WebView::WebViewTransport
{
public:
    CARAPI SetWebView(
        /* [in] */ IWebView* webview);

    CARAPI GetWebView(
        /* [out] */ IWebView** webView);
};

} // namespace Webkit
} // namespace Droid
} // namespace Elastos

#endif // __ELASTOS_DROID_WEBKIT_CWEBVIEWTRANSPORT_H__
