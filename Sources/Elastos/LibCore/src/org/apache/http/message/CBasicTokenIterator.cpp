
#include "CBasicTokenIterator.h"

namespace Org {
namespace Apache {
namespace Http {
namespace Message {

CAR_OBJECT_IMPL(CBasicTokenIterator)

ECode CBasicTokenIterator::constructor(
    /* [in] */ IHeaderIterator* headerIterator)
{
    return Init(headerIterator);
}

} // namespace Message
} // namespace Http
} // namespace Apache
} // namespace Org
