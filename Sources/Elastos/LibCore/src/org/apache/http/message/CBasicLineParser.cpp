
#include "CBasicLineParser.h"

namespace Org {
namespace Apache {
namespace Http {
namespace Message {

CAR_OBJECT_IMPL(CBasicLineParser)

ECode CBasicLineParser::constructor(
    /* [in] */ IProtocolVersion* proto)
{
    return Init(buffer);
}

ECode CBasicLineParser::constructor()
{
    return Init(NULL);
}

} // namespace Message
} // namespace Http
} // namespace Apache
} // namespace Org
