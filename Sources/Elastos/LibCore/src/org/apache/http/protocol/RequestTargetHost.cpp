
#include "RequestTargetHost.h"
#include "CHttpVersion.h"
#include "CHttpHost.h"
#include "Logger.h"
#include "elastos/core/StringUtils.h"

using Elastos::Core::StringUtils;
using Elastos::Net::IInetAddress;
using Elastos::Utility::Logging::Logger;
using Org::Apache::Http::IHttpConnection;
using Org::Apache::Http::IHttpHost;
using Org::Apache::Http::CHttpHost;
using Org::Apache::Http::IRequestLine;
using Org::Apache::Http::IHttpMessage;
using Org::Apache::Http::IHttpInetConnection;
using Org::Apache::Http::IProtocolVersion;
using Org::Apache::Http::CHttpVersion;
using Org::Apache::Http::IHttpRequest;

namespace Org {
namespace Apache {
namespace Http {
namespace Protocol {

CAR_INTERFACE_IMPL(RequestTargetHost, Object, IHttpRequestInterceptor)

ECode RequestTargetHost::Process(
    /* [in] */ IHttpRequest* request,
    /* [in] */ IHttpContext* context)
{
    if (request == NULL) {
        Logger::E("RequestTargetHost", "HTTP request may not be null");
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    if (context == NULL) {
        Logger::E("RequestTargetHost", "HTTP context may not be null");
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    AutoPtr<IHttpMessage> message = IHttpMessage::Probe(request);
    Boolean contains;
    if (message->ContainsHeader(IHTTP::TARGET_HOST, &contains), !contains) {
        AutoPtr<IInterface> attr;
        context->GetAttribute(IExecutionContext::HTTP_TARGET_HOST, (IInterface**)&attr);
        AutoPtr<IHttpHost> targethost = IHttpHost::Probe(attr);
        if (targethost == NULL) {
            AutoPtr<IInterface> connAttr;
            context->GetAttribute(IExecutionContext::HTTP_CONNECTION, (IInterface**)&connAttr);
            AutoPtr<IHttpConnection> conn = IHttpConnection::Probe(connAttr);
            AutoPtr<IHttpInetConnection> netConn = IHttpInetConnection::Probe(conn);
            if (netConn != NULL) {
                // Populate the context with a default HTTP host based on the
                // inet address of the target host
                AutoPtr<IInetAddress> address;
                netConn->GetRemoteAddress((IInetAddress**)&address);
                Int32 port;
                netConn->GetRemotePort(&port);
                if (address != NULL) {
                    String name;
                    address->GetHostName(&name);
                    targethost = NULL;
                    CHttpHost::New(name, port, (IHttpHost**)&targethost);
                }
            }
            if (targethost == NULL) {
                AutoPtr<IRequestLine> rl;
                request->GetRequestLine((IRequestLine**)&rl);
                AutoPtr<IProtocolVersion> ver;
                rl->GetProtocolVersion((IProtocolVersion**)&ver);
                Boolean lessEquals;
                if (ver->LessEquals(IProtocolVersion::Probe(CHttpVersion::HTTP_1_0), &lessEquals), lessEquals) {
                    return NOERROR;
                }
                else {
                    Logger::E("RequestTargetHost", "Target host missing");
                    return E_PROTOCOL_EXCEPTION;
                }
            }
        }
        String str;
        targethost->ToHostString(&str);
        message->AddHeader(IHTTP::TARGET_HOST, str);
    }
    return NOERROR;
}

} // namespace Protocol
} // namespace Http
} // namespace Apache
} // namespace Org
