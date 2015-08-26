
#include "ResponseContent.h"
#include "CHttpVersion.h"
#include <elastos/Logger.h>
#include <elastos/core/StringUtils.h>

using Elastos::Core::StringUtils;
using Elastos::Utility::Logging::Logger;
using Org::Apache::Http::IStatusLine;
using Org::Apache::Http::IHttpStatus;
using Org::Apache::Http::IHttpMessage;
using Org::Apache::Http::IHttpEntity;
using Org::Apache::Http::IProtocolVersion;
using Org::Apache::Http::CHttpVersion;
using Org::Apache::Http::IHttpRequest;

namespace Org {
namespace Apache {
namespace Http {
namespace Protocol {

CAR_INTERFACE_IMPL(ResponseContent, Object, IHttpResponseInterceptor)

ECode ResponseContent::Process(
    /* [in] */ IHttpResponse* response,
    /* [in] */ IHttpContext* context)
{
    if (response == NULL) {
        Logger::E("ResponseContent", "HTTP response may not be null");
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    Boolean contains;
    if (response->ContainsHeader(IHTTP::TRANSFER_ENCODING, &contains), contains) {
        Logger::E("ResponseContent", "Transfer-encoding header already present");
        return E_PROTOCOL_EXCEPTION;
    }
    if (response->ContainsHeader(IHTTP::CONTENT_LEN, &contains), contains) {
        Logger::E("ResponseContent", "Content-Length header already present");
        return E_PROTOCOL_EXCEPTION;
    }
    AutoPtr<IStatusLine> sl;
    response->GetStatusLine((IStatusLine**)&sl);
    AutoPtr<IProtocolVersion> ver;
    sl->GetProtocolVersion((IProtocolVersion**)&ver);
    AutoPtr<IHttpEntity> entity;
    response->GetEntity((IHttpEntity**)&entity);
    AutoPtr<IHttpMessage> message = IHttpMessage::Probe(response);
    if (entity != NULL) {
        Int32 len;
        entity->GetContentLength(&len)
        Boolean isChunked, lessEquals;
        if ((entity->IsChunked(&isChunked), isChunked) || (ver->LessEquals(CHttpVersion::HTTP_1_0, &lessEquals), !lessEquals)) {
            message->AddHeader(IHTTP::TRANSFER_ENCODING, IHTTP::CHUNK_CODING);
        }
        else if (len >= 0) {
            message->AddHeader(IHTTP::CONTENT_LEN, StringUtils::ToString(len));
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
    else {
        Int32 status;
        sl->GetStatusCode(&status);
        if (status != IHttpStatus::SC_NO_CONTENT
                && status != IHttpStatus::SC_NOT_MODIFIED
                && status != IHttpStatus::SC_RESET_CONTENT) {
            message->AddHeader(IHTTP::CONTENT_LEN, String("0"));
        }
    }
    return NOERROR;
}

} // namespace Protocol
} // namespace Http
} // namespace Apache
} // namespace Org
