
#include "SocketOutputBuffer.h"
#include <elastos/Logger.h>

using Elastos::Utility::Logging::Logger;

namespace Org {
namespace Apache {
namespace Http {
namespace Impl {
namespace IO {

SocketOutputBuffer::SocketOutputBuffer(
    /* [in] */ ISocket* socket,
    /* [in] */ Int32 buffersize,
    /* [in] */ IHttpParams* params)
    : AbstractSessionInputBuffer()
{
    if (socket == NULL) {
        Logger::E("SocketOutputBuffer", "Socket may not be null");
        assert(0);
        // throw new IllegalArgumentException("Socket may not be null");
    }
    // BEGIN android-changed
    // Workaround for http://b/1083103 and http://b/3514259. We take
    // 'buffersize' as a hint in the weakest sense, and always use
    // an 8KiB heap buffer and leave the kernel buffer size alone,
    // trusting the system to have set a network-appropriate default.
    AutoPtr<IInputStream> in;
    socket->GetInputStream((IInputStream**)&in);
    Init(in, 8192, params);
    // END android-changed
}

} // namespace IO
} // namespace Impl
} // namespace Http
} // namespace Apache
} // namespace Org