
#include "CHttpProtocolParamBean.h"
#include "HttpConnectionParams"

namespace Org {
namespace Apache {
namespace Http {
namespace Params {

CAR_INTERFACE_IMPL(CHttpProtocolParamBean, Object, IHttpProtocolParamBean)

CAR_OBJECT_IMPL(CHttpProtocolParamBean)

ECode CHttpProtocolParamBean::SetHttpElementCharset(
    /* [in] */ const String& httpElementCharset)
{
    return HttpConnectionParams::SetHttpElementCharse(mParams, httpElementCharset);
}

ECode CHttpProtocolParamBean::SetContentCharset(
    /* [in] */ const String& contentCharset)
{
    return HttpConnectionParams::SetContentCharse(mParams, contentCharset);
}

ECode CHttpProtocolParamBean::SetVersion(
    /* [in] */ IHttpVersion* version)
{
    return HttpConnectionParams::SetVersion(mParams, version);
}

ECode CHttpProtocolParamBean::SetUserAgent(
    /* [in] */ const String& userAgent)
{
    return HttpConnectionParams::SetUserAgent(mParams, userAgent);
}

ECode CHttpProtocolParamBean::SetUseExpectContinue(
    /* [in] */ Boolean useExpectContinue)
{
    return HttpConnectionParams::SetUseExpectContinue(mParams, useExpectContinue);
}

ECode CHttpProtocolParamBean::constructor(
    /* [in] */ IHttpParams* params)
{
    return Init(params);
}

} // namespace Params
} // namespace Http
} // namespace Apache
} // namespace Org
