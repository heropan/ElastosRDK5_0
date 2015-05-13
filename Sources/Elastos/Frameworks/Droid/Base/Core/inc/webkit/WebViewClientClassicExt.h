
#ifndef __WEBVIEWCLIENTCLASSICEXT_H__
#define __WEBVIEWCLIENTCLASSICEXT_H__

#include "ext/frameworkext.h"
#include "webkit/WebViewClient.h"

using Elastos::Droid::Net::Http::ISslError;

namespace Elastos {
namespace Droid {
namespace Webkit {

class HttpAuthHandler;
class ClientCertRequestHandler;

/**
 * Adds WebViewClassic specific extension methods to the WebViewClient callback class.
 * These are not part of the public WebView API, so the class is hidden.
 * @hide
 */
class WebViewClientClassicExt
    : public ElRefBase
    , public WebViewClient
{
public:

    /**
     * Notify the host application that an SSL error occurred while loading a
     * resource, but the WebView chose to proceed anyway based on a
     * decision retained from a previous response to onReceivedSslError().
     */
    virtual CARAPI OnProceededAfterSslError(
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
    virtual CARAPI OnReceivedClientCertRequest(
        /* [in] */ IWebView* view,
        /* [in] */ ClientCertRequestHandler* handler,
        /* [in] */ const String& host_and_port);
};

} // namespace Webkit
} // namespace Droid
} // namespace Elastos

#endif //__WEBVIEWCLIENTCLASSICEXT_H__
