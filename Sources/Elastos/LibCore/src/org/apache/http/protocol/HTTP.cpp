
#include "HTTP.h"

namespace Org {
namespace Apache {
namespace Http {
namespace Protocol {

CAR_INTERFACE_IMPL(HTTP, Object, IHTTP)

Boolean HTTP::IsWhitespace(
    /* [in] */ Char32 ch)
{
    String str(ch);
    return str == SP || str == HT || str == CR || str == LF;
}

} // namespace Protocol
} // namespace Http
} // namespace Apache
} // namespace Org
