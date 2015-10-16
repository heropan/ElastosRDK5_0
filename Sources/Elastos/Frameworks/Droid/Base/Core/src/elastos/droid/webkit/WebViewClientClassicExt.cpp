
#include "elastos/droid/webkit/WebViewClientClassicExt.h"
#include "elastos/droid/webkit/HttpAuthHandler.h"
#include "elastos/droid/webkit/ClientCertRequestHandler.h"

namespace Elastos {
namespace Droid {
namespace Webkit {

class ClientCertRequestHandler;

/**
 * Notify the host application that an SSL error occurred while loading a
 * resource, but the WebView chose to proceed anyway based on a
 * decision retained from a previous response to onReceivedSslError().
 */
ECode WebViewClientClassicExt::OnProceededAfterSslError(
    /* [in] */ IWebView* view,
    /* [in] */ ISslError* error)
{
    return NOERROR;
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
ECode WebViewClientClassicExt::OnReceivedClientCertRequest(
    /* [in] */ IWebView* view,
    /* [in] */ ClientCertRequestHandler* handler,
    /* [in] */ const String& host_and_port)
{
    VALIDATE_NOT_NULL(handler);
    handler->Cancel();
    return NOERROR;
}

} // namespace Webkit
} // namepsace Droid
} // namespace Elastos