
#ifndef __ELASTOS_DROID_WEBKIT_CWEBVIEWCLIENT_H__
#define __ELASTOS_DROID_WEBKIT_CWEBVIEWCLIENT_H__

#include "_Elastos_Droid_Webkit_CWebViewClient.h"
#include "elastos/droid/webkit/WebViewClient.h"

using Elastos::Droid::Graphics::IBitmap;
using Elastos::Droid::Os::IMessage;
using Elastos::Droid::Net::Http::ISslError;
using Elastos::Droid::View::IKeyEvent;

namespace Elastos {
namespace Droid {
namespace Webkit {

CarClass(CWebViewClient), public WebViewClient
{
public:
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

#endif // __ELASTOS_DROID_WEBKIT_CWEBVIEWCLIENT_H__
