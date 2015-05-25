
#include "CRawHeadersHelper.h"
#include "CRawHeaders.h"

namespace Elastos {
namespace Net {
namespace Http {

ECode CRawHeadersHelper::FromMultimap(
    /* [in] */ IMap* map,
    /* [out] */ IRawHeaders** rh)
{
    return CRawHeaders::FromMultimap(map,rh);
}

} // namespace Http
} // namespace Net
} // namespace Elastos
