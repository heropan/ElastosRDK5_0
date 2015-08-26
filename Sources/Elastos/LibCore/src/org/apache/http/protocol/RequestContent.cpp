
#include "RequestContent.h"
#include "CHttpVersion.h"
#include <elastos/Logger.h>
#include <elastos/core/StringUtils.h>

using Elastos::Core::StringUtils;
using Elastos::Utility::Logging::Logger;
using Org::Apache::Http::IHttpEntityEnclosingRequest;
using Org::Apache::Http::IRequestLine;
using Org::Apache::Http::IHttpMessage;
using Org::Apache::Http::IHttpEntity;
using Org::Apache::Http::IProtocolVersion;
using Org::Apache::Http::CHttpVersion;
using Org::Apache::Http::IHttpRequest;

namespace Org {
namespace Apache {
namespace Http {
namespace Protocol {

CAR_INTERFACE_IMPL(RequestContent, Object, IHttpRequestInterceptor)

ECode RequestContent::Process(
    /* [in] */ IHttpRequest* request,
    /* [in] */ IHttpContext* context)
{
    if (request == NULL) {
        Logger::E("RequestContent", "HTTP request may not be null");
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    AutoPtr<IHttpEntityEnclosingRequest> enclosingRequest = IHttpEntityEnclosingRequest::Probe(request);
    if (enclosingRequest != NULL) {
        Boolean contains;
        if (request->ContainsHeader(IHTTP::TRANSFER_ENCODING, &contains), contains) {
            Logger::E("RequestContent", "Transfer-encoding header already present");
            return E_PROTOCOL_EXCEPTION;
        }
        Boolean contains;
        if (request->ContainsHeader(IHTTP::CONTENT_LEN, &contains), contains) {
            Logger::E("RequestContent", "Content-Length header already present");
            return E_PROTOCOL_EXCEPTION;
        }
        AutoPtr<IRequestLine> rl;
        request->GetRequestLine((IRequestLine**)&rl);
        AutoPtr<IProtocolVersion> ver;
        rl->GetProtocolVersion((IProtocolVersion**)&ver);
        AutoPtr<IHttpEntity> entity;
        enclosingRequest->GetEntity((IHttpEntity**)&entity);
        AutoPtr<IHttpMessage> message = IHttpMessage::Probe(request);
        if (entity == NULL) {
            message->AddHeader(IHTTP::CONTENT_LEN, String("0"));
            return NOERROR;
        }
        // Must specify a transfer encoding or a content length
        Boolean isChunked;
        Int32 len;
        if ((entity->IsChunked(&isChunked), isChunked) || (entity->GetContentLength(&len), len < 0)) {
            Boolean lessEquals;
            if (ver->LessEquals(CHttpVersion::HTTP_1_0, &lessEquals), lessEquals) {
                Logger::E("RequestContent", "Chunked transfer encoding not allowed for %p", ver.Get());
                return E_PROTOCOL_EXCEPTION;
            }
            message->AddHeader(IHTTP::TRANSFER_ENCODING, IHTTP::CHUNK_CODING);
        }
        else {
            Int32 length;
            entity->GetContentLength(&length);
            message->AddHeader(IHTTP::CONTENT_LEN, StringUtils::ToString(length));
        }
        // Specify a content type if known
        String type;
        if ((entity->GetContentType(&type), !type.IsNull())
                && (request->ContainsHeader(IHTTP::CONTENT_TYPE, &contains), !contains)) {
            message->AddHeader(type);
        }
        // Specify a content encoding if known
        String encoding;
        if ((entity->GetContentEncoding(&encoding), !encoding.IsNull())
                && (request->ContainsHeader(IHTTP::CONTENT_ENCODING, &contains), !contains)) {
            message->AddHeader(encoding);
        }
    }
    return NOERROR;
}

} // namespace Protocol
} // namespace Http
} // namespace Apache
} // namespace Org
