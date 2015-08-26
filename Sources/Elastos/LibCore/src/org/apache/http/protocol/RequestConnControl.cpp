
#include "RequestConnControl.h"
#include <elastos/Logger.h>

using Elastos::Utility::Logging::Logger;
using Org::Apache::Http::IHttpMessage;
using Org::Apache::Http::Params::IHttpParams;
using Org::Apache::Http::Params::HttpProtocolParams;

namespace Org {
namespace Apache {
namespace Http {
namespace Protocol {

ECode RequestConnControl::Process(
    /* [in] */ IHttpRequest* request,
    /* [in] */ IHttpContext* context)
{
    if (Request == NULL) {
        Logger::E("RequestConnControl", "HTTP request may not be null");
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    Boolean contains;
    if (request->ContainsHeader(IHTTP::CONN_DIRECTIVE, &contains), !contains) {
        // Default policy is to keep connection alive
        // whenever possible
        request->AddHeader(IHTTP::CONN_DIRECTIVE, IHTTP::CONN_KEEP_ALIVE);
    }
    return NOERROR;
}

} // namespace Protocol
} // namespace Http
} // namespace Apache
} // namespace Org
