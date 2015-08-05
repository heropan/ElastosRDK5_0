
#include "HttpRequestWriter.h"
#include "CCharArrayBuffer.h"
#include <elastos/Logger.h>

using Elastos::Utility::Logging::Logger;
using Org::Apache::Http::IHttpRequest;
using Org::Apache::Http::IRequestLine;
using Org::Apache::Http::Utility::CCharArrayBuffer;
using Org::Apache::Http::Utility::ICharArrayBuffer;

namespace Org {
namespace Apache {
namespace Http {
namespace Impl {
namespace IO {

HttpRequestWriter::HttpRequestWriter(
    /* [in] */ ISessionOutputBuffer* buffer,
    /* [in] */ ILineFormatter* parser,
    /* [in] */ IHttpParams* params)
    : AbstractMessageWriter(buffer, formatter, params)
{}

ECode HttpRequestWriter::WriteHeadLine(
    /* [in] */ IHttpMessage* message)
{
    AutoPtr<IHttpRequest> request = IHttpRequest::Probe(message);
    AutoPtr<IRequestLine> requestLine;
    request->GetRequestLIne((IRequestLine**)&requestline);
    AutoPtr<ICharArrayBuffer> buffer;
    mLineFormatter->FormatRequestLine(mLineBuf, requestline, (ICharArrayBuffer**)&buffer);
    return mSessionBuffer->WriteLine(buffer);
}

} // namespace IO
} // namespace Impl
} // namespace Http
} // namespace Apache
} // namespace Org