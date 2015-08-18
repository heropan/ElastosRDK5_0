
#include "CHttpConnectionParams.h"
#include "HttpConnectionParams.h"

namespace Org {
namespace Apache {
namespace Http {
namespace Params {

CAR_INTERFACE_IMPL_2(CHttpConnectionParams, Object, IHttpConnectionParams, ICoreConnectionPNames)

CAR_OBJECT_IMPL(CHttpConnectionParams)

ECode CHttpProtocolParams::GetSoTimeout(
    /* [in] */ IHttpParams* params,
    /* [out] */ Int32* timeout)
{
    VALIDATE_NOT_NULL(timeout)
    return HttpProtocolParams::GetSoTimeout(params, timeout);
}

ECode CHttpProtocolParams::SetSoTimeout(
    /* [in] */ IHttpParams* params,
    /* [in] */ Int32 timeout)
{
    return HttpProtocolParams::SetSoTimeout(params, timeout);
}

ECode CHttpProtocolParams::GetTcpNoDelay(
    /* [in] */ IHttpParams* params,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result)
    return HttpProtocolParams::GetTcpNoDelay(params, result);
}

ECode CHttpProtocolParams::SetTcpNoDelay(
    /* [in] */ IHttpParams* params,
    /* [in] */ Boolean value)
{
    return HttpProtocolParams::SetSoTimeout(params, value);
}

ECode CHttpProtocolParams::GetSocketBufferSize(
    /* [in] */ IHttpParams* params,
    /* [out] */ Int32* size)
{
    VALIDATE_NOT_NULL(size)
    return HttpProtocolParams::GetSocketBufferSize(params, size);
}

ECode CHttpProtocolParams::SetSocketBufferSize(
    /* [in] */ IHttpParams* params,
    /* [in] */ Int32 size)
{
    return HttpProtocolParams::SetSoTimeout(params, size);
}

ECode CHttpProtocolParams::GetLinger(
    /* [in] */ IHttpParams* params,
    /* [out] */ Int32* linger)
{
    VALIDATE_NOT_NULL(linger)
    return HttpProtocolParams::GetLinger(params, linger);
}

ECode CHttpProtocolParams::SetLinger(
    /* [in] */ IHttpParams* params,
    /* [in] */ Int32 value)
{
    return HttpProtocolParams::SetLinger(params, value);
}

ECode CHttpProtocolParams::GetConnectionTimeout(
    /* [in] */ IHttpParams* params,
    /* [out] */ Int32* timeout)
{
    VALIDATE_NOT_NULL(timeout)
    return HttpProtocolParams::GetConnectionTimeout(params, timeout);
}

ECode CHttpProtocolParams::SetConnectionTimeout(
    /* [in] */ IHttpParams* params,
    /* [in] */ Int32 timeout)
{
    return HttpProtocolParams::SetConnectionTimeout(params, timeout);
}

ECode CHttpProtocolParams::IsStaleCheckingEnabled(
    /* [in] */ IHttpParams* params,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result)
    return HttpProtocolParams::IsStaleCheckingEnabled(params, result);
}

ECode CHttpProtocolParams::SetStaleCheckingEnabled(
    /* [in] */ IHttpParams* params,
    /* [in] */ Boolean value)
{
    return HttpProtocolParams::SetStaleCheckingEnabled(params, value);
}

} // namespace Params
} // namespace Http
} // namespace Apache
} // namespace Org
