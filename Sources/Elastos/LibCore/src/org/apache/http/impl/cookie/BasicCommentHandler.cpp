
#include "BasicCommentHandler.h"
#include <elastos/Logger.h>

using Elastos::Utility::Logging::Logger;

namespace Org {
namespace Apache {
namespace Http {
namespace Impl {
namespace Cookie {

ECode BasicCommentHandler::Parse(
    /* [in] */ ISetCookie* cookie,
    /* [in] */ const String& value)
{
    if (cookie == NULL) {
        Logger::E("BasicCommentHandler", "Cookie may not be null");
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    return cookie->SetComment(value);
}

} // namespace Cookie
} // namespace Impl
} // namespace Http
} // namespace Apache
} // namespace Org