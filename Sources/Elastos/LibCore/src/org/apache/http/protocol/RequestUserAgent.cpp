
#include "RequestUserAgent.h"
#include "HttpProtocolParams.h"
#include <elastos/Logger.h>

using Elastos::Utility::Logging::Logger;
using Org::Apache::Http::IHttpMessage;
using Org::Apache::Http::Params::IHttpParams;
using Org::Apache::Http::Params::HttpProtocolParams;

namespace Org {
namespace Apache {
namespace Http {
namespace Protocol {

ECode RequestUserAgent::Process(
    /* [in] */ IHttpRequest* request,
    /* [in] */ IHttpContext* context)
{
    if (Request == NULL) {
        Logger::E("RequestUserAgent", "HTTP request may not be null");
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    Boolean contains;
    if (request->ContainsHeader(IHTTP::USER_AGENT, &contains), !contains) {
        AutoPtr<IHttpParams> params;
        request->GetParams((IHttpParams**)&params);
        String useragent;
        HttpProtocolParams::GetUserAgent(params, &useragent);
        if (!useragent.IsNull()) {
            request->AddHeader(IHTTP::USER_AGENT, useragent);
        }
    }
    return NOERROR;
}

} // namespace Protocol
} // namespace Http
} // namespace Apache
} // namespace Org
