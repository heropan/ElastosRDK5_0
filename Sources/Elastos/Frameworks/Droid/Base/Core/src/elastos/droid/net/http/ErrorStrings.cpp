
#include "elastos/droid/net/http/ErrorStrings.h"

using Elastos::Droid::Content::IContext;
using Elastos::Droid::Utility::ILog;

namespace Elastos {
namespace Droid {
namespace Net {
namespace Http {

const String ErrorStrings::LOGTAG("Http");

ErrorStrings::ErrorStrings()
{
    /* Utility class, don't instantiate. */
}

ECode ErrorStrings::GetString(
    /* [in] */ Int32 errorCode,
    /* [in] */ IContext* context,
    /* [out] */ String* result)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translated before. Need check.
    VALIDATE_NOT_NULL(str);

    Int32 res;
    GetResource(errorCode, &res);
    AutoPtr<Elastos::Core::ICharSequence> cs;
    ECode ec = context->GetText(res, (Elastos::Core::ICharSequence**)&cs);
    if (SUCCEEDED(ec)) {
        cs->ToString(str);
    }

    return ec;
#endif
}

ECode ErrorStrings::GetResource(
    /* [in] */ Int32 errorCode,
    /* [out] */ Int32* result)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translated before. Need check.
    VALIDATE_NOT_NULL(res);

    switch(errorCode) {
        case IEventHandler::OK: {
            *res = R::string::httpErrorOk;
            break;
        }
        case IEventHandler::ERROR: {
            *res = R::string::httpError;
            break;
        }
        case IEventHandler::ERROR_LOOKUP: {
            *res = R::string::httpErrorLookup;
            break;
        }
        case IEventHandler::ERROR_UNSUPPORTED_AUTH_SCHEME: {
            *res = R::string::httpErrorUnsupportedAuthScheme;
            break;
        }
        case IEventHandler::ERROR_AUTH: {
            *res = R::string::httpErrorAuth;
            break;
        }
        case IEventHandler::ERROR_PROXYAUTH: {
            *res = R::string::httpErrorProxyAuth;
            break;
        }
        case IEventHandler::ERROR_CONNECT: {
            *res = R::string::httpErrorConnect;
            break;
        }
        case IEventHandler::ERROR_IO: {
            *res = R::string::httpErrorIO;
            break;
        }
        case IEventHandler::ERROR_TIMEOUT: {
            *res = R::string::httpErrorTimeout;
            break;
        }
        case IEventHandler::ERROR_REDIRECT_LOOP: {
            *res = R::string::httpErrorRedirectLoop;
            break;
        }
        case IEventHandler::ERROR_UNSUPPORTED_SCHEME: {
            *res = R::string::httpErrorUnsupportedScheme;
            break;
        }
        case IEventHandler::ERROR_FAILED_SSL_HANDSHAKE: {
            *res = R::string::httpErrorFailedSslHandshake;
            break;
        }
        case IEventHandler::ERROR_BAD_URL: {
            *res = R::string::httpErrorBadUrl;
            break;
        }
        case IEventHandler::FILE_ERROR: {
            *res = R::string::httpErrorFile;
            break;
        }
        case IEventHandler::FILE_NOT_FOUND_ERROR: {
            *res = R::string::httpErrorFileNotFound;
            break;
        }
        case IEventHandler::TOO_MANY_REQUESTS_ERROR: {
            *res = R::string::httpErrorTooManyRequests;
            break;
        }
        default: {
            // Log.w(LOGTAG, "Using generic message for unknown error code: " + errorCode);
            *res = R::string::httpError;
            break;
        }
    }

    return NOERROR;
#endif
}

} // namespace Http
} // namespace Net
} // namespace Droid
} // namespace Elastos
