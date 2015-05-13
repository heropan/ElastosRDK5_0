
#ifndef __SSLERRORHANDLER_H__
#define __SSLERRORHANDLER_H__

#include "ext/frameworkext.h"
#include "os/Handler.h"

using Elastos::Droid::Os::Handler;

namespace Elastos {
namespace Droid {
namespace Webkit {

/**
 * SslErrorHandler: class responsible for handling SSL errors.
 * This class is passed as a parameter to BrowserCallback.displaySslErrorDialog
 * and is meant to receive the user's response.
 */
class SslErrorHandler : public Handler
{
public:
    /**
     * @hide Only for use by WebViewProvider implementations.
     */
    SslErrorHandler();

    /**
     * Proceed with the SSL certificate.
     */
    virtual CARAPI Proceed();

    /**
     * Cancel this request and all pending requests for the WebView that had
     * the error.
     */
    virtual CARAPI Cancel();
};

} // namespace Webkit
} // namespace Droid
} // namespace Elastos

#endif //__SSLERRORHANDLER_H__
