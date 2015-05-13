
#include "CDeleteOnExitHelper.h"
#include "CDeleteOnExit.h"

namespace Org {
namespace Apache {
namespace Harmony {
namespace Luni {
namespace Util {

ECode CDeleteOnExitHelper::GetInstance(
    /* [out] */ IDeleteOnExit **instance)
{
    return CDeleteOnExit::GetInstance(instance);
}

} // namespace Util
} // namespace Luni
} // namespace Harmony
} // namespace Apache
} // namespace Org

