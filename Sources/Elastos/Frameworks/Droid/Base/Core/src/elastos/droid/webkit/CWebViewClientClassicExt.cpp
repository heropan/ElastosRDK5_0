
#include "webkit/CWebViewClientClassicExt.h"

namespace Elastos {
namespace Droid {
namespace Webkit {

PInterface CWebViewClientClassicExt::Probe(
    /* [in] */ REIID riid)
{
	return _CWebViewClientClassicExt::Probe(riid);
}

UInt32 CWebViewClientClassicExt::AddRef()
{
    return _CWebViewClientClassicExt::AddRef();
}

UInt32 CWebViewClientClassicExt::Release()
{
    return _CWebViewClientClassicExt::Release();
}

ECode CWebViewClientClassicExt::GetInterfaceID(
    /* [in] */ IInterface* object,
    /* [out] */ InterfaceID* iid)
{
	return _CWebViewClientClassicExt::GetInterfaceID(object, iid);
}

/**
 * Notify the host application that an SSL error occurred while loading a
 * resource, but the WebView chose to proceed anyway based on a
 * decision retained from a previous response to onReceivedSslError().
 */
ECode CWebViewClientClassicExt::OnProceededAfterSslError(
    /* [in] */ IWebView* view,
    /* [in] */ ISslError* error)
{
	return WebViewClientClassicExt::OnProceededAfterSslError(view, error);
}

/**
 * Notify the host application to handle a SSL client certificate
 * request (display the request to the user and ask whether to
 * proceed with a client certificate or not). The host application
 * has to call either handler.cancel() or handler.proceed() as the
 * connection is suspended and waiting for the response. The
 * default behavior is to cancel, returning no client certificate.
 *
 * @param view The WebView that is initiating the callback.
 * @param handler A ClientCertRequestHandler object that will
 *            handle the user's response.
 * @param host_and_port The host and port of the requesting server.
 */
ECode CWebViewClientClassicExt::OnReceivedClientCertRequest(
    /* [in] */ IWebView* view,
    /* [in] */ IHandler* handler,
    /* [in] */ const String& host_and_port)
{
    return WebViewClientClassicExt::OnReceivedClientCertRequest(view, (ClientCertRequestHandler*)handler, host_and_port);
}

/***************************CWebViewClient****************************/

ECode CWebViewClientClassicExt::ShouldOverrideUrlLoading(
    /* [in] */ IWebView* view,
    /* [in] */ const String& url,
    /* [out] */ Boolean* result)
{
	VALIDATE_NOT_NULL(result);
	*result = WebViewClient::ShouldOverrideUrlLoading(view, url);
	return NOERROR;
}

ECode CWebViewClientClassicExt::OnPageStarted(
    /* [in] */ IWebView* view,
    /* [in] */ const String& url,
    /* [in] */ IBitmap* favicon)
{
	WebViewClient::OnPageStarted(view, url, favicon);
	return NOERROR;
}

ECode CWebViewClientClassicExt::OnPageFinished(
    /* [in] */ IWebView* view,
    /* [in] */ const String& url)
{
	WebViewClient::OnPageFinished(view, url);
	return NOERROR;
}

ECode CWebViewClientClassicExt::OnLoadResource(
    /* [in] */ IWebView* view,
    /* [in] */ const String& url)
{
	WebViewClient::OnLoadResource(view, url);
	return NOERROR;
}

ECode CWebViewClientClassicExt::ShouldInterceptRequest(
    /* [in] */ IWebView* view,
    /* [in] */ const String& url,
    /* [out] */ IWebResourceResponse** wrr)
{
	VALIDATE_NOT_NULL(wrr);
	WebViewClient::ShouldInterceptRequest(view, url);
	return NOERROR;
}

ECode CWebViewClientClassicExt::OnTooManyRedirects(
    /* [in] */ IWebView* view,
    /* [in] */ IMessage* cancelMsg,
    /* [in] */ IMessage* continueMsg)
{
    return WebViewClient::OnTooManyRedirects(view, cancelMsg, continueMsg);
}

ECode CWebViewClientClassicExt::OnReceivedError(
    /* [in] */ IWebView* view,
    /* [in] */ Int32 errorCode,
    /* [in] */ const String& description,
    /* [in] */ const String& failingUrl)
{
	WebViewClient::OnReceivedError(view, errorCode, description, failingUrl);
	return NOERROR;
}

ECode CWebViewClientClassicExt::OnFormResubmission(
    /* [in] */ IWebView* view,
    /* [in] */ IMessage* dontResend,
    /* [in] */ IMessage* resend)
{
    return WebViewClient::OnFormResubmission(view, dontResend, resend);
}

ECode CWebViewClientClassicExt::DoUpdateVisitedHistory(
    /* [in] */ IWebView* view,
    /* [in] */ const String& url,
    /* [in] */ Boolean isReload)
{
	WebViewClient::DoUpdateVisitedHistory(view, url, isReload);
	return NOERROR;
}

ECode CWebViewClientClassicExt::OnReceivedSslError(
    /* [in] */ IWebView* view,
    /* [in] */ ISslErrorHandler* handler,
    /* [in] */ ISslError* error)
{
    return WebViewClient::OnReceivedSslError(view, handler, error);
}

ECode CWebViewClientClassicExt::OnReceivedHttpAuthRequest(
    /* [in] */ IWebView* view,
    /* [in] */ IHttpAuthHandler* handler,
    /* [in] */ const String& host,
    /* [in] */ const String& realm)
{
	WebViewClient::OnReceivedHttpAuthRequest(view, handler, host, realm);
	return NOERROR;
}

ECode CWebViewClientClassicExt::ShouldOverrideKeyEvent(
    /* [in] */ IWebView* view,
    /* [in] */ IKeyEvent* event,
    /* [out] */ Boolean * result)
{
	VALIDATE_NOT_NULL(result);
	*result = WebViewClient::ShouldOverrideKeyEvent(view, event);
	return NOERROR;
}

ECode CWebViewClientClassicExt::OnUnhandledKeyEvent(
    /* [in] */ IWebView* view,
    /* [in] */ IKeyEvent* event)
{
	WebViewClient::OnUnhandledKeyEvent(view, event);
	return NOERROR;
}

ECode CWebViewClientClassicExt::OnScaleChanged(
    /* [in] */ IWebView* view,
    /* [in] */ Float oldScale,
    /* [in] */ Float newScale)
{
	WebViewClient::OnScaleChanged(view, oldScale, newScale);
	return NOERROR;
}

ECode CWebViewClientClassicExt::OnReceivedLoginRequest(
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