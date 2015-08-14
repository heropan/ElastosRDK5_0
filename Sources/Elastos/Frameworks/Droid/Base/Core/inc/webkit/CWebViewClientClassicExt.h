
#ifndef __ELASTOS_DROID_WEBKIT_CWEBVIEWCLIENTCLASSICEXT_H__
#define __ELASTOS_DROID_WEBKIT_CWEBVIEWCLIENTCLASSICEXT_H__

#include "_CWebViewClientClassicExt.h"
#include "webkit/WebViewClientClassicExt.h"

using Elastos::Droid::Os::IHandler;
using Elastos::Droid::Net::Http::ISslError;

namespace Elastos {
namespace Droid {
namespace Webkit {

/**
 * Adds WebViewClassic specific extension methods to the WebViewClient callback class.
 * These are not part of the public WebView API, so the class is hidden.
 * @hide
 */
CarClass(CWebViewClientClassicExt), public WebViewClientClassicExt
{
public:
    CAR_INTERFACE_DECL();

    /**
     * Notify the host application that an SSL error occurred while loading a
     * resource, but the WebView chose to proceed anyway based on a
     * decision retained from a previous response to onReceivedSslError().
     */
    CARAPI OnProceededAfterSslError(
        /* [in] */ IWebView* view,
        /* [in] */ ISslError* error);

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
    CARAPI OnReceivedClientCertRequest(
        /* [in] */ IWebView* view,
        /* [in] */ IHandler* handler,
        /* [in] */ const String& host_and_port);

/***************************CWebViewClient****************************/

    CARAPI ShouldOverrideUrlLoading(
        /* [in] */ IWebView* view,
        /* [in] */ const String& url,
        /* [out] */ Boolean* result);

    CARAPI OnPageStarted(
        /* [in] */ IWebView* view,
        /* [in] */ const String& url,
        /* [in] */ IBitmap* favicon);

    CARAPI OnPageFinished(
        /* [in] */ IWebView* view,
        /* [in] */ const String& url);

    CARAPI OnLoadResource(
        /* [in] */ IWebView* view,
        /* [in] */ const String& url);

    CARAPI ShouldInterceptRequest(
        /* [in] */ IWebView* view,
        /* [in] */ const String& url,
        /* [out] */ IWebResourceResponse** wrr);

    CARAPI OnTooManyRedirects(
        /* [in] */ IWebView* view,
        /* [in] */ IMessage* cancelMsg,
        /* [in] */ IMessage* continueMsg);

    CARAPI OnReceivedError(
        /* [in] */ IWebView* view,
        /* [in] */ Int32 errorCode,
        /* [in] */ const String& description,
        /* [in] */ const String& failingUrl);

    CARAPI OnFormResubmission(
        /* [in] */ IWebView* view,
        /* [in] */ IMessage* dontResend,
        /* [in] */ IMessage* resend);

    CARAPI DoUpdateVisitedHistory(
        /* [in] */ IWebView* view,
        /* [in] */ const String& url,
        /* [in] */ Boolean isReload);

    CARAPI OnReceivedSslError(
        /* [in] */ IWebView* view,
        /* [in] */ ISslErrorHandler* handler,
        /* [in] */ ISslError* error);

    CARAPI OnReceivedHttpAuthRequest(
        /* [in] */ IWebView* view,
        /* [in] */ IHttpAuthHandler* handler,
        /* [in] */ const String& host,
        /* [in] */ const String& realm);

    CARAPI ShouldOverrideKeyEvent(
        /* [in] */ IWebView* view,
        /* [in] */ IKeyEvent* event,
        /* [out] */ Boolean * pResult);

    CARAPI OnUnhandledKeyEvent(
        /* [in] */ IWebView* view,
        /* [in] */ IKeyEvent* event);

    CARAPI OnScaleChanged(
        /* [in] */ IWebView* view,
        /* [in] */ Float oldScale,
        /* [in] */ Float newScale);

    CARAPI OnReceivedLoginRequest(
        /* [in] */ IWebView* view,
        /* [in] */ const String& realm,
        /* [in] */ const String& account,
        /* [in] */ const String& args);
};

} // namespace Webkit
} // namespace Droid
} // namespace Elastos

#endif //__ELASTOS_DROID_WEBKIT_CWEBVIEWCLIENTCLASSICEXT_H__
