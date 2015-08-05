
#include "HttpResponseWriter.h"
#include "CCharArrayBuffer.h"
#include <elastos/Logger.h>

using Elastos::Utility::Logging::Logger;
using Org::Apache::Http::IHttpResponse;
using Org::Apache::Http::IRequestLine;
using Org::Apache::Http::Utility::CCharArrayBuffer;
using Org::Apache::Http::Utility::ICharArrayBuffer;

namespace Org {
namespace Apache {
namespace Http {
namespace Impl {
namespace IO {

HttpResponseWriter::HttpResponseWriter(
    /* [in] */ ISessionOutputBuffer* buffer,
    /* [in] */ ILineFormatter* parser,
    /* [in] */ IHttpParams* params)
    : AbstractMessageWriter(buffer, formatter, params)
{}

ECode HttpResponseWriter::WriteHeadLine(
    /* [in] */ IHttpMessage* message)
{
    AutoPtr<IHttpResponse> response = IHttpResponse::Probe(message);
    AutoPtr<IStatusLine> statusLine;
    request->GetStatusLine((IStatusLine**)&statusLine);
    AutoPtr<ICharArrayBuffer> buffer;
    mLineFormatter->FormatStatusLine(mLineBuf, statusLine, (ICharArrayBuffer**)&buffer);
    return mSessionBuffer->WriteLine(buffer);
}

} // namespace IO
} // namespace Impl
} // namespace Http
} // namespace Apache
} // namespace Org