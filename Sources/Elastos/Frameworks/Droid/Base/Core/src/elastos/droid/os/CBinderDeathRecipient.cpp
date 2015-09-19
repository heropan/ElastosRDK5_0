#include <ext/frameworkdef.h>
#include "os/CBinderDeathRecipient.h"

namespace Elastos {
namespace Droid {
namespace Os {

ECode CBinderDeathRecipient::ProxyDied()
{
    return NOERROR;
}
} // namespace Os
} // namespace Droid
} // namespace Elastos
