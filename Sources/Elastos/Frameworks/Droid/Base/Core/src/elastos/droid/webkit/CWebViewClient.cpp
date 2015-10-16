
#include "elastos/droid/webkit/CWebViewClient.h"

namespace Elastos {
namespace Droid {
namespace Webkit {

ECode CWebViewClient::ShouldOverrideUrlLoading(
    /* [in] */ IWebView* view,
    /* [in] */ const String& url,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
    *result = WebViewClient::ShouldOverrideUrlLoading(view, url);
    return NOERROR;
}

ECode CWebViewClient::OnPageStarted(
    /* [in] */ IWebView* view,
    /* [in] */ const String& url,
    /* [in] */ IBitmap* favicon)
{
    WebViewClient::OnPageStarted(view, url, favicon);
    return NOERROR;
}

ECode CWebViewClient::OnPageFinished(
    /* [in] */ IWebView* view,
    /* [in] */ const String& url)
{
    WebViewClient::OnPageFinished(view, url);
    return NOERROR;
}

ECode CWebViewClient::OnLoadResource(
    /* [in] */ IWebView* view,
    /* [in] */ const String& url)
{
    WebViewClient::OnLoadResource(view, url);
    return NOERROR;
}

ECode CWebViewClient::ShouldInterceptRequest(
    /* [in] */ IWebView* view,
    /* [in] */ const String& url,
    /* [out] */ IWebResourceResponse** wrr)
{
    VALIDATE_NOT_NULL(wrr);
//    AutoPtr<IWebResourceResponse> _wrr = WebViewClient::ShouldInterceptRequest(view, url);
//    *wrr = _wrr.Get();
//    REFCOUNT_ADD(*wrr);

    return NOERROR;
}

ECode CWebViewClient::OnTooManyRedirects(
    /* [in] */ IWebView* view,
    /* [in] */ IMessage* cancelMsg,
    /* [in] */ IMessage* continueMsg)
{
    return WebViewClient::OnTooManyRedirects(view, cancelMsg, continueMsg);
}

ECode CWebViewClient::OnReceivedError(
    /* [in] */ IWebView* view,
    /* [in] */ Int32 errorCode,
    /* [in] */ const String& description,
    /* [in] */ const String& failingUrl)
{
    WebViewClient::OnReceivedError(view, errorCode, description, failingUrl);
    return NOERROR;
}

ECode CWebViewClient::OnFormResubmission(
    /* [in] */ IWebView* view,
    /* [in] */ IMessage* dontResend,
    /* [in] */ IMessage* resend)
{
    return WebViewClient::OnFormResubmission(view, dontResend, resend);
}

ECode CWebViewClient::DoUpdateVisitedHistory(
    /* [in] */ IWebView* view,
    /* [in] */ const String& url,
    /* [in] */ Boolean isReload)
{
    WebViewClient::DoUpdateVisitedHistory(view, url, isReload);
    return NOERROR;
}

ECode CWebViewClient::OnReceivedSslError(
    /* [in] */ IWebView* view,
    /* [in] */ ISslErrorHandler* handler,
    /* [in] */ ISslError* error)
{
    return WebViewClient::OnReceivedSslError(view, handler, error);
}

ECode CWebViewClient::OnReceivedHttpAuthRequest(
    /* [in] */ IWebView* view,
    /* [in] */ IHttpAuthHandler* handler,
    /* [in] */ const String& host,
    /* [in] */ const String& realm)
{
//    WebViewClient::OnReceivedHttpAuthRequest(view, handler, host, realm);
    return NOERROR;
}

ECode CWebViewClient::ShouldOverrideKeyEvent(
    /* [in] */ IWebView* view,
    /* [in] */ IKeyEvent* event,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
    *result = WebViewClient::ShouldOverrideKeyEvent(view, event);
    return NOERROR;
}

ECode CWebViewClient::OnUnhandledKeyEvent(
    /* [in] */ IWebView* view,
    /* [in] */ IKeyEvent* event)
{
    WebViewClient::OnUnhandledKeyEvent(view, event);
    return NOERROR;
}

ECode CWebViewClient::OnScaleChanged(
    /* [in] */ IWebView* view,
    /* [in] */ Float oldScale,
    /* [in] */ Float newScale)
{
    WebViewClient::OnScaleChanged(view, oldScale, newScale);
    return NOERROR;
}

ECode CWebViewClient::OnReceivedLoginRequest(
    /* [in] */ IWebView* view,
    /* [in] */ const String& realm,
    /* [in] */ const String& account,
    /* [in] */ const String& args)
{
    WebViewClient::OnReceivedLoginRequest(view, realm, account, args);
    return NOERROR;
}

} // namespace Webkit
} // namepsace Droid
} // namespace Elastos