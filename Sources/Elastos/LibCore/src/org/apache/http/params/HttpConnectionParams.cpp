
#include "HttpProtocolParams.h"
#include <elastos/Logger.h>

using Elastos::Utility::Logging::Logger;

namespace Org {
namespace Apache {
namespace Http {
namespace Params {

ECode HttpProtocolParams::GetSoTimeout(
    /* [in] */ IHttpParams* params,
    /* [out] */ Int32* timeout)
{
    VALIDATE_NOT_NULL(timeout)
    *timeout = 0;
    if (params == NULL) {
        Logger::E("HttpProtocolParams", "HTTP parameters may not be null");
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    return params->GetInt32Parameter(ICoreConnectionPNames::SO_TIMEOUT, 0, timeout);
}

ECode HttpProtocolParams::SetSoTimeout(
    /* [in] */ IHttpParams* params,
    /* [in] */ Int32 timeout)
{
    if (params == NULL) {
        Logger::E("HttpProtocolParams", "HTTP parameters may not be null");
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    return params->SetInt32Parameter(ICoreConnectionPNames::SO_TIMEOUT, timeout);
}

ECode HttpProtocolParams::GetTcpNoDelay(
    /* [in] */ IHttpParams* params,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result)
    *result = TRUE;
    if (params == NULL) {
        Logger::E("HttpProtocolParams", "HTTP parameters may not be null");
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    return params->GetBooleanParameter(ICoreConnectionPNames::TCP_NODELAY, TRUE, result);
}

ECode HttpProtocolParams::SetTcpNoDelay(
    /* [in] */ IHttpParams* params,
    /* [in] */ Boolean value)
{
    if (params == NULL) {
        Logger::E("HttpProtocolParams", "HTTP parameters may not be null");
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    return params->SetBooleanParameter(ICoreConnectionPNames::TCP_NODELAY, value);
}

ECode HttpProtocolParams::GetSocketBufferSize(
    /* [in] */ IHttpParams* params,
    /* [out] */ Int32* size)
{
    VALIDATE_NOT_NULL(size)
    *size = -1;
    if (params == NULL) {
        Logger::E("HttpProtocolParams", "HTTP parameters may not be null");
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    return params->GetInt32Parameter(ICoreConnectionPNames::SOCKET_BUFFER_SIZE, -1, size);
}

ECode HttpProtocolParams::SetSocketBufferSize(
    /* [in] */ IHttpParams* params,
    /* [in] */ Int32 size)
{
    if (params == NULL) {
        Logger::E("HttpProtocolParams", "HTTP parameters may not be null");
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    return params->SetInt32Parameter(ICoreConnectionPNames::SOCKET_BUFFER_SIZE, size);
}

ECode HttpProtocolParams::GetLinger(
    /* [in] */ IHttpParams* params,
    /* [out] */ Int32* linger)
{
    VALIDATE_NOT_NULL(linger)
    *linger = -1;
    if (params == NULL) {
        Logger::E("HttpProtocolParams", "HTTP parameters may not be null");
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    return params->GetInt32Parameter(ICoreConnectionPNames::SO_LINGER, -1, linger);
}

ECode HttpProtocolParams::SetLinger(
    /* [in] */ IHttpParams* params,
    /* [in] */ Int32 value)
{
    if (params == NULL) {
        Logger::E("HttpProtocolParams", "HTTP parameters may not be null");
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    return params->SetInt32Parameter(ICoreConnectionPNames::SO_LINGER, value);
}

ECode HttpProtocolParams::GetConnectionTimeout(
    /* [in] */ IHttpParams* params,
    /* [out] */ Int32* timeout)
{
    VALIDATE_NOT_NULL(timeout)
    *timeout = 0;
    if (params == NULL) {
        Logger::E("HttpProtocolParams", "HTTP parameters may not be null");
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    return params->GetInt32Parameter(ICoreConnectionPNames::CONNECTION_TIMEOUT, 0, timeout);
}

ECode HttpProtocolParams::SetConnectionTimeout(
    /* [in] */ IHttpParams* params,
    /* [in] */ Int32 timeout)
{
    if (params == NULL) {
        Logger::E("HttpProtocolParams", "HTTP parameters may not be null");
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    return params->SetInt32Parameter(ICoreConnectionPNames::CONNECTION_TIMEOUT, timeout);
}

ECode HttpProtocolParams::IsStaleCheckingEnabled(
    /* [in] */ IHttpParams* params,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result)
    *result = TRUE;
    if (params == NULL) {
        Logger::E("HttpProtocolParams", "HTTP parameters may not be null");
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    return params->GetBooleanParameter(ICoreConnectionPNames::STALE_CONNECTION_CHECK, TRUE, result);
}

ECode HttpProtocolParams::SetStaleCheckingEnabled(
    /* [in] */ IHttpParams* params,
    /* [in] */ Boolean value)
{
    if (params == NULL) {
        Logger::E("HttpProtocolParams", "HTTP parameters may not be null");
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    return params->SetBooleanParameter(ICoreConnectionPNames::STALE_CONNECTION_CHECK, value);
}

} // namespace Params
} // namespace Http
} // namespace Apache
} // namespace Org
