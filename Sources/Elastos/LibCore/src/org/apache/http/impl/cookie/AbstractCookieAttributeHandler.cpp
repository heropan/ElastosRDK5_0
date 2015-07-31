
#include "AbstractCookieAttributeHandler.h"

namespace Org {
namespace Apache {
namespace Http {
namespace Impl {
namespace Cookie {

CAR_INTERFACE_IMPL(AbstractCookieAttributeHandler, Object, ICookieAttributeHandler)

ECode AbstractCookieAttributeHandler::Validate(
    /* [in] */ ICookie* cookie,
    /* [in] */ ICookieOrigin* origin)
{
    return NOERROR;
}

ECode AbstractCookieAttributeHandler::Match(
    /* [in] */ ICookie* cookie,
    /* [in] */ ICookieOrigin* origin,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result)
    *result = TRUE;
    return NOERROR;
}

} // namespace Cookie
} // namespace Impl
} // namespace Http
} // namespace Apache
} // namespace Org