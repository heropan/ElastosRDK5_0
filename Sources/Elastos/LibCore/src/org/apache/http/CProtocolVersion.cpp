
#include "CProtocolVersion.h"

namespace Org {
namespace Apache {
namespace Http {

CAR_OBJECT_IMPL(CProtocolVersion)

ECode CProtocolVersion::constructor(
    /* [in] */ const String& protocol,
    /* [in] */ Int32 major,
    /* [in] */ Int32 minor)
{
    return ProtocolVersion::Init(protocol, major, minor);
}

} // namespace Http
} // namespace Apache
} // namespace Org
