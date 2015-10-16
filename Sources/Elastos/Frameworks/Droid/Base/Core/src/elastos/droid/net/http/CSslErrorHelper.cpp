
#include "CSslErrorHelper.h"
#include "CSslError.h"
#include "elastos/droid/ext/frameworkext.h"

namespace Elastos {
namespace Droid {
namespace Net {
namespace Http {

ECode CSslErrorHelper::SslErrorFromChromiumErrorCode(
    /* [in] */ Int32 error,
    /* [in] */ ISslCertificate* cert,
    /* [in] */ const String& url,
    /* [out] */ ISslError** sslError)
{
    VALIDATE_NOT_NULL(sslError);

    // The chromium error codes are in:
    // external/chromium/net/base/net_error_list.h
    assert (error >= -299 && error <= -200);

    if (error == -200)
        return CSslError::New(CSslError::SSL_IDMISMATCH, cert, url, sslError);
    if (error == -201)
        return CSslError::New(CSslError::SSL_DATE_INVALID, cert, url, sslError);
    if (error == -202)
        return CSslError::New(CSslError::SSL_UNTRUSTED, cert, url, sslError);
    // Map all other codes to SSL_INVALID.
    return CSslError::New(CSslError::SSL_INVALID, cert, url, sslError);
}

}
}
}
}

